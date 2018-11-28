/*
	    File: CMIO_DP_HogMode.cpp
	Abstract: n/a
	 Version: 1.2
	
	Disclaimer: IMPORTANT:  This Apple software is supplied to you by Apple
	Inc. ("Apple") in consideration of your agreement to the following
	terms, and your use, installation, modification or redistribution of
	this Apple software constitutes acceptance of these terms.  If you do
	not agree with these terms, please do not use, install, modify or
	redistribute this Apple software.
	
	In consideration of your agreement to abide by the following terms, and
	subject to these terms, Apple grants you a personal, non-exclusive
	license, under Apple's copyrights in this original Apple software (the
	"Apple Software"), to use, reproduce, modify and redistribute the Apple
	Software, with or without modifications, in source and/or binary forms;
	provided that if you redistribute the Apple Software in its entirety and
	without modifications, you must retain this notice and the following
	text and disclaimers in all such redistributions of the Apple Software.
	Neither the name, trademarks, service marks or logos of Apple Inc. may
	be used to endorse or promote products derived from the Apple Software
	without specific prior written permission from Apple.  Except as
	expressly stated in this notice, no other rights or licenses, express or
	implied, are granted by Apple herein, including but not limited to any
	patent rights that may be infringed by your derivative works or by other
	works in which the Apple Software may be incorporated.
	
	The Apple Software is provided by Apple on an "AS IS" basis.  APPLE
	MAKES NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION
	THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS
	FOR A PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND
	OPERATION ALONE OR IN COMBINATION WITH YOUR PRODUCTS.
	
	IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL
	OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
	SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
	INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION,
	MODIFICATION AND/OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED
	AND WHETHER UNDER THEORY OF CONTRACT, TORT (INCLUDING NEGLIGENCE),
	STRICT LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN ADVISED OF THE
	POSSIBILITY OF SUCH DAMAGE.
	
	Copyright (C) 2012 Apple Inc. All Rights Reserved.
	
*/

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Self Include
#include "CMIO_DP_HogMode.h"

// Internal Includes
#include "CMIO_DP_Device.h"

// Public Utility Includes
#include "CMIODebugMacros.h"

// CA Public Utility Includes
#include "CACFNumber.h"
#include "CACFPreferences.h"
#include "CACFString.h"
#include "CAException.h"

// System Includes
#include <CoreFoundation/CFNotificationCenter.h>

namespace CMIO { namespace DP
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//	HogMode
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	HogMode::HogMode(Device* device) :
		mToken(0),
		mDevice(device),
		mPrefName(NULL),
		mOwner(-3)
	{
		// Get our token
		if (sTokenMap == NULL)
		{
			sTokenMap = new CATokenMap<HogMode>();
		}
		mToken = sTokenMap->MapObject(this);
		
		// Construct the name of the preference
		mPrefName = CFStringCreateMutable(NULL, 0);
		ThrowIfNULL(mPrefName, CAException(kCMIOHardwareUnspecifiedError), "CMIO::DP::HogMode::HogMode: couldn't allocate the pref name string");
		CFStringAppendCString((CFMutableStringRef)mPrefName, "com.apple.cmio.CMIO.HogMode.Owner-", kCFStringEncodingASCII);
		CACFString uid(device->CopyDeviceUID());
		CFStringAppend((CFMutableStringRef)mPrefName, uid.GetCFString());
		
		// Sign up for notifications
		CFNotificationCenterAddObserver(CFNotificationCenterGetDistributedCenter(), (const void*)mToken, (CFNotificationCallback)ChangeNotification, mPrefName, NULL, CFNotificationSuspensionBehaviorCoalesce);
	}

	HogMode::~HogMode()
	{
		CFNotificationCenterRemoveObserver(CFNotificationCenterGetDistributedCenter(), (const void*)mToken, mPrefName, NULL);
		CFRelease(mPrefName);
	}

	pid_t HogMode::GetOwner() const
	{
		if (mOwner == -3)
		{
			const_cast<HogMode*>(this)->mOwner = GetOwnerFromPreference(true);
		}
		return mOwner;
	}

	bool HogMode::IsFree() const
	{
		if (mOwner == -3)
		{
			const_cast<HogMode*>(this)->mOwner = GetOwnerFromPreference(true);
		}
		return (mOwner == -1);
	}

	bool HogMode::CurrentProcessIsOwner() const
	{
		if (mOwner == -3)
		{
			const_cast<HogMode*>(this)->mOwner = GetOwnerFromPreference(true);
		}
		return (mOwner == CAProcess::GetPID());
	}

	bool HogMode::CurrentProcessIsOwnerOrIsFree() const
	{
		if (mOwner == -3)
		{
			const_cast<HogMode*>(this)->mOwner = GetOwnerFromPreference(true);
		}
		return (CurrentProcessIsOwner() || IsFree());
	}

	void HogMode::Take()
	{
		if (IsFree())
		{
			pid_t oldOwner = mOwner;
			
			try
			{
				// Set the new owner before GetDevice()->Lock(),
				// because when GetDevice()->Lock() is called, it invokes kIOMessageServiceIsAttemptingOpen message.
				// At that time, AVC Plugin checks the current owner by calling HogModeGetOwner(),
				// which returns mOwner, to see who tries to open the device.
				mOwner = CAProcess::GetPID();
			
				// Grab exclusive access to the device
				// This can throw a exception
				GetDevice()->Lock();
				
				// Write the new owner's pid to the preferences
				SetOwnerInPreference(mOwner);
	
				// Signal that hog mode changed to the device
				PropertyAddress address(kCMIODevicePropertyHogMode);
				mDevice->PropertiesChanged(1, &address);
				
				// Signal that hog mode changed to the world
				SendHogModeChangedNotification();
			}
			catch(...)
			{
				// If GetDevice()->Lock() throw a exception,
				// get back mOwner to the old value.
				mOwner = oldOwner;
			}
		}
	}

	void HogMode::Release()
	{
		if (CurrentProcessIsOwner())
		{
			// Release exclusive access to the device
			GetDevice()->Unlock();
			
			// Set the owner to free
			mOwner = -1;
			
			// Delete the pref
			SetOwnerInPreference(-1);
			
			// Signal that hog mode changed to the device
			PropertyAddress address(kCMIODevicePropertyHogMode);
			mDevice->PropertiesChanged(1, &address);
			
			// Signal that hog mode changed to the world
			SendHogModeChangedNotification();
		}
	}

	pid_t HogMode::GetOwnerFromPreference(bool sendNotifications) const
	{
		pid_t answer = -1;
		
		// Get the preference
		CFNumberRef cfnumber = CACFPreferences::CopyNumberValue(mPrefName, false, true);
		if (cfnumber != NULL)
		{
			// Get the number
			pid_t owner = -1;
			CFNumberGetValue(cfnumber, kCFNumberSInt32Type, &owner);
			
			// Make sure the process exists
			if (CAProcess::ProcessExists(owner))
			{
				// It does, so set the return value
				answer = owner;
			}
			else
			{
				// It doesn't, so delete the pref
				SetOwnerInPreference((pid_t)-1);
				
				if (sendNotifications)
				{
					// Signal that hog mode changed
					SendHogModeChangedNotification();
				}
			}
		}
		
		return answer;
	}

	void HogMode::SetOwnerInPreference(pid_t owner) const
	{
		if (owner != -1)
		{
			CACFNumber number(static_cast<SInt32>(owner));
			CACFPreferences::SetValue(mPrefName, number.GetCFNumber(), false, true, true);
		}
		else
		{
			CACFPreferences::DeleteValue(mPrefName, false, true, true);
		}
		CACFPreferences::Synchronize(false, true, true);
	}

	void HogMode::SendHogModeChangedNotification() const
	{
		CACFPreferences::SendNotification(mPrefName);
	}

	void HogMode::ChangeNotification(CFNotificationCenterRef /*center*/, const void* hogMode, CFStringRef /*notificationName*/, const void* /*object*/, CFDictionaryRef /*userInfo*/)
	{
		try
		{
			if (sTokenMap != NULL)
			{
				HogMode* hogModeObject = sTokenMap->GetObject(hogMode);
				if (hogModeObject != NULL)
				{
					// Mark the prefs as dirty
					CACFPreferences::MarkPrefsOutOfDate(false, true);
					
					// Get the new owner
					pid_t newOwner = hogModeObject->GetOwnerFromPreference(false);
					pid_t oldOwner = hogModeObject->GetOwner();
					
					if (newOwner != oldOwner)
					{
						// It's different for this process
						hogModeObject->SetOwner(newOwner);
						hogModeObject->GetDevice()->HogModeStateChanged();
						
						// Send the notification
						PropertyAddress address(kCMIODevicePropertyHogMode);
						hogModeObject->GetDevice()->PropertiesChanged(1, &address);
					}
				}
			}
		}
		catch(...)
		{
		}
	}

	CATokenMap<HogMode>* HogMode::sTokenMap = NULL;
}}
