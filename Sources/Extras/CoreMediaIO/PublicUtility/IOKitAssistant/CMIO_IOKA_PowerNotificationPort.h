/*
	    File: CMIO_IOKA_PowerNotificationPort.h
	Abstract: The IOKit Assisistant (IOKA) consists of various objects to facilitate using the "Acquisition is Initialization" design pattern.
				NOTE: the IOKA is sparsely implemented, meaning that it does not attempt to provide wrappers/access to all IOKit features, but just those needed in the CMIO namespace.
	
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

#if !defined(__CMIO_IOKA_PowerNotificationPort_h__)
#define __CMIO_IOKA_PowerNotificationPort_h__

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Public Utility Includes
#include "CMIODebugMacros.h"
#include "CMIO_IOKA_Object.h"

// CA Public Utility Includes
#include "CAException.h"

// System Includes
#include <IOKit/pwr_mgt/IOPMLib.h>

namespace CMIO { namespace IOKA
{
	//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// PowerNotificationPort
	//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class PowerNotificationPort
	{
	// Construction/Destruction
	public:
								PowerNotificationPort(CFRunLoopRef runLoop = 0, CFStringRef runLoopMode = 0, IOServiceInterestCallback callback = 0, void* refCon = 0) : mPowerNotificationPort(0), mRunLoop(NULL), mRunLoopMode(NULL), mRootPowerDomain(), mNotifier(0) { Reset(runLoop, runLoopMode, callback, refCon); }
								~PowerNotificationPort() { Reset(); }
		
		void					Reset(CFRunLoopRef runLoop = 0, CFStringRef runLoopMode = 0, IOServiceInterestCallback callback = 0, void* refCon = 0)
								{
									if (0 != mPowerNotificationPort)
									{
										// Remove the notification port's run loop source
										CFRunLoopRemoveSource(mRunLoop, IONotificationPortGetRunLoopSource(mPowerNotificationPort), mRunLoopMode);
										
										// Deregister from power notifications.  No need to call IOObjectRelease() on mNotifier since that is done by IODeregisterForSystemPower().
										IODeregisterForSystemPower(&mNotifier);
										
										// Destroy the port on which power notifications had been recieved
										IONotificationPortDestroy(mPowerNotificationPort);
										mPowerNotificationPort = 0;
										mRunLoop = NULL;
										mRunLoopMode = NULL;

										// Release the root power domain connection
										mRootPowerDomain.Reset();
									}
									
									if (0 != runLoop and 0 != runLoopMode and 0 != callback)
									{
										// Try and register for power notifications
										mRootPowerDomain.Reset(IORegisterForSystemPower(refCon, &mPowerNotificationPort, callback, &mNotifier));
										ThrowIf(0 == mRootPowerDomain.Get(), CAException(kIOReturnError), "CMIO::IOKA::PowerNotificationPort::Reset: IORegisterForSystemPower() failed"); 
											
										// Add the notification port's run loop source to the specified run loop
										CFRunLoopAddSource(runLoop, IONotificationPortGetRunLoopSource(mPowerNotificationPort), runLoopMode);
										mRunLoop = runLoop;
										mRunLoopMode = runLoopMode;
									}
								}

	private:
		PowerNotificationPort&	operator=(PowerNotificationPort& that);	// Don't allow copying
		IONotificationPortRef	mPowerNotificationPort;					// Port on which power notifications are received		
		CFRunLoopRef			mRunLoop;								// The run loop to which the the port's run loop source was added		
		CFStringRef				mRunLoopMode;							// The mode which the port's port's run loop source was operating
		Object					mRootPowerDomain;						// Representation of the Root Power Domain IOService 
		io_object_t				mNotifier;								// 'cookie' from IORegisterForSystemPower() to be passed to IODeregisterForSystemPower()

	// Value Access
	public:
		operator				IONotificationPortRef() const { return mPowerNotificationPort; }
		IONotificationPortRef	Get() const	{ return mPowerNotificationPort; }													
		IONotificationPortRef*	GetAddress() { return &mPowerNotificationPort; }													
		io_object_t				GetRootPowerDomain() const	{ return mRootPowerDomain; }													
	};
}}

#endif
