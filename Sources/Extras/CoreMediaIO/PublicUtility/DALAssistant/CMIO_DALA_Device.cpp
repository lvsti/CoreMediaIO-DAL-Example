/*
	    File: CMIO_DALA_Device.cpp
	Abstract: C++ wrapper for CMIODeviceID
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
#include "CMIO_DALA_Device.h"

//	Internal Includes
#include "CMIO_DALA_Stream.h"
#include "CMIO_PropertyAddress.h"

//	CA PublicUtility Includes
#include "CAAutoDisposer.h"
#include "CMIODebugMacros.h"
#include "CAException.h"

// System Includes
#include <IOKit/audio/IOAudioTypes.h>
#include <unistd.h>

// Standard Library Includes
#include <algorithm>

namespace CMIO { namespace DALA
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Device()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	Device::Device(CMIODeviceID cmioDeviceID) :
		Object(cmioDeviceID)
	{
	}

	Device::~Device()
	{
	}

	CFStringRef Device::CopyUID() const
	{
		CFStringRef answer = NULL;
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIODevicePropertyDeviceUID), 0, NULL, sizeof(CFStringRef), dataUsed, &answer);
		return answer;
	}

	bool Device::HasModelUID() const
	{
		return HasProperty(PropertyAddress(kCMIODevicePropertyModelUID));
	}

	CFStringRef Device::CopyModelUID() const
	{
		CFStringRef answer = NULL;
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIODevicePropertyModelUID), 0, NULL, sizeof(CFStringRef), dataUsed, &answer);
		return answer;
	}

	UInt32 Device::GetTransportType() const
	{
		UInt32 answer = 0;
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIODevicePropertyTransportType), 0, NULL, sizeof(UInt32), dataUsed, &answer);
		return answer;
	}

	bool Device::CanBeDefaultDevice(CMIOObjectPropertyScope scope) const
	{
		UInt32 answer = 0;
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIODevicePropertyDeviceCanBeDefaultDevice, scope), 0, NULL, sizeof(UInt32), dataUsed, &answer);
		return answer != 0;
	}

	bool Device::HasDevicePlugInStatus() const
	{
		return HasProperty(PropertyAddress(kCMIODevicePropertyPlugIn));
	}

	OSStatus Device::GetDevicePlugInStatus() const
	{
		OSStatus answer = 0;
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIODevicePropertyPlugIn), 0, NULL, sizeof(OSStatus), dataUsed, &answer);
		return answer;
	}

	bool Device::IsAlive() const
	{
		bool answer = HasProperty(PropertyAddress(kCMIODevicePropertyDeviceIsAlive));
		if (answer)
		{
			UInt32 isAlive = 0;
			UInt32 dataUsed = 0;
			GetPropertyData(PropertyAddress(kCMIODevicePropertyDeviceIsAlive), 0, NULL, sizeof(UInt32), dataUsed, &isAlive);
			answer = isAlive != 0;
		}
		return answer;
	}

	bool Device::IsRunning() const
	{
		UInt32 answer = 0;
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIODevicePropertyDeviceIsRunning), 0, NULL, sizeof(UInt32), dataUsed, &answer);
		return answer != 0;
	}

	void Device::SetIsRunning(bool isRunning)
	{
		UInt32 value = isRunning ? 1 : 0;
		UInt32 size = sizeof(UInt32);
		SetPropertyData(PropertyAddress(kCMIODevicePropertyDeviceIsRunning), 0, NULL, size, &value);
	}

	bool Device::IsRunningSomewhere() const
	{
		UInt32 answer = 0;
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIODevicePropertyDeviceIsRunningSomewhere), 0, NULL, sizeof(UInt32), dataUsed, &answer);
		return answer != 0;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// HogModeIsSettable()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool Device::HogModeIsSettable() const
	{
		return IsPropertySettable(PropertyAddress(kCMIODevicePropertyHogMode));
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetHogModeOwner()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	pid_t Device::GetHogModeOwner() const
	{
		pid_t answer = 0;
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIODevicePropertyHogMode), 0, NULL, sizeof(pid_t), dataUsed, &answer);
		return answer;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// TakeHogMode()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool Device::TakeHogMode()
	{
		try
		{
			pid_t pid = getpid();
			UInt32 size = sizeof(pid_t);
			SetPropertyData(PropertyAddress(kCMIODevicePropertyHogMode), 0, NULL, size, &pid);
		}
		catch (...)
		{
			// Unable to acquire the hog
			return false;
		}
		
		return true;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// ReleaseHogMode()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void Device::ReleaseHogMode()
	{
		pid_t pid = -1;
		UInt32 size = sizeof(pid_t);
		PropertyAddress address(kCMIODevicePropertyHogMode);
		(void) CMIOObjectSetPropertyData(GetObjectID(), &address, 0, NULL, size, &pid);
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetDeviceMasterOwner()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	pid_t Device::GetDeviceMasterOwner() const
	{
		pid_t answer = 0;
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIODevicePropertyDeviceMaster), 0, NULL, sizeof(pid_t), dataUsed, &answer);
		return answer;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// TakeDeviceMaster()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool Device::TakeDeviceMaster()
	{
		pid_t pid = getpid();
		UInt32 size = sizeof(pid_t);
		try
		{
			SetPropertyData(PropertyAddress(kCMIODevicePropertyDeviceMaster), 0, NULL, size, &pid);
		}
		catch (...)
		{
			// Unable to acquire mastership
			return false;
		}
		
		return true;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// ReleaseDeviceMaster()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void Device::ReleaseDeviceMaster()
	{
		pid_t pid = -1;
		UInt32 size = sizeof(pid_t);
		SetPropertyData(PropertyAddress(kCMIODevicePropertyDeviceMaster), 0, NULL, size, &pid);
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// HasExcludeNonDALAccess()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool Device::HasExcludeNonDALAccess() const
	{
		return HasProperty(PropertyAddress(kCMIODevicePropertyExcludeNonDALAccess));
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetExcludeNonDALAccess()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool Device::GetExcludeNonDALAccess() const
	{
		UInt32 excludeNonDALAccess = 0;
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIODevicePropertyExcludeNonDALAccess), 0, NULL, sizeof(UInt32), dataUsed, &excludeNonDALAccess);
		return excludeNonDALAccess != 0;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// SetExcludeNonDALAccess()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void Device::SetExcludeNonDALAccess(bool exclude)
	{
		UInt32 excludeNonDALAccess = exclude ? 1 : 0;
		UInt32 size = sizeof(UInt32);
		SetPropertyData(PropertyAddress(kCMIODevicePropertyDeviceMaster), 0, NULL, size, &excludeNonDALAccess);
	}

	bool Device::HasVideoDigitizerComponents() const
	{
		return HasProperty(PropertyAddress(kCMIODevicePropertyVideoDigitizerComponents));
	}

	UInt32 Device::GetNumberVideoDigitizerComponents() const
	{
		UInt32 answer = 0;
		PropertyAddress address(kCMIODevicePropertyVideoDigitizerComponents);

		if (HasProperty(address))
		{
			answer = GetPropertyDataSize(address, 0, NULL);
			answer = answer / sizeof(ComponentDescription);
		}
		return answer;
	}

	void Device::GetVideoDigitizerComponents(UInt32& ioNumberVideoDigitizerComponents, ComponentDescription* componentDescriptions) const
	{
		UInt32 numberVideoDigitizerComponents = std::min(GetNumberVideoDigitizerComponents(), ioNumberVideoDigitizerComponents);
		UInt32 size = numberVideoDigitizerComponents * sizeof(ComponentDescription);
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIODevicePropertyVideoDigitizerComponents), 0, NULL, size, dataUsed, componentDescriptions);
		ioNumberVideoDigitizerComponents = size / sizeof(ComponentDescription);
	}

	bool Device::HasSuspendedByUser() const
	{
		return HasProperty(PropertyAddress(kCMIODevicePropertySuspendedByUser));
	}

	bool Device::SuspendedByUser() const
	{
		UInt32 answer = 0;
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIODevicePropertySuspendedByUser), 0, NULL, sizeof(UInt32), dataUsed, &answer);
		return answer != 0;
	}

	bool Device::CanProcessAVCCommands() const
	{
		Boolean answer = 0;
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIODevicePropertyCanProcessAVCCommand), 0, NULL, sizeof(Boolean), dataUsed, &answer);
		return answer;
	}

	bool Device::CanProcessRS422Commands() const
	{
		Boolean answer = 0;
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIODevicePropertyCanProcessRS422Command), 0, NULL, sizeof(Boolean), dataUsed, &answer);
		return answer;
	}

	UInt32 Device::GetLatency(CMIOObjectPropertyScope scope) const
	{
		UInt32 answer = 0;
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIODevicePropertyLatency, scope), 0, NULL, sizeof(UInt32), dataUsed, &answer);
		return answer;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// StartStream()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void Device::StartStream(CMIOStreamID streamID)
	{
		OSStatus err = CMIODeviceStartStream(GetObjectID(), streamID);
		ThrowIfError(err, CAException(err), "CMIO::DALA::Device::StartStream: CMIODeviceStartStream() failed");
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// StopStream()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void Device::StopStream(CMIOStreamID streamID)
	{
		OSStatus err = CMIODeviceStopStream(GetObjectID(), streamID);
		ThrowIfError(err, CAException(err), "CMIO::DALA::Device::StopStream: CMIODeviceStopStream() failed");
	}

	#pragma mark -
	UInt32 Device::GetNumberStreams(CMIOObjectPropertyScope scope) const
	{
		UInt32 size = GetPropertyDataSize(PropertyAddress(kCMIODevicePropertyStreams, scope), 0, NULL);
		return size / sizeof(CMIOStreamID);
	}

	void Device::GetStreams(CMIOObjectPropertyScope scope, UInt32& ioNumberStreams, CMIOStreamID* streamList) const
	{
		ioNumberStreams = std::min(GetNumberStreams(scope), ioNumberStreams);
		UInt32 size = ioNumberStreams * sizeof(CMIOStreamID);
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIODevicePropertyStreams, scope), 0, NULL, size, dataUsed, streamList);
		ioNumberStreams = size / sizeof(CMIOStreamID);
		CMIOStreamID* firstItem = &(streamList[0]);
		CMIOStreamID* lastItem = firstItem + ioNumberStreams;
		std::sort(firstItem, lastItem);
	}

	CMIOStreamID Device::GetStreamByIndex(CMIOObjectPropertyScope scope, UInt32 index) const
	{
		CMIOStreamID answer = 0;
		
		UInt32 numberStreams = GetNumberStreams(scope);
		if ((numberStreams > 0) && (index < numberStreams))
		{
			CAAutoArrayDelete<CMIOStreamID> streamList(numberStreams);
			GetStreams(scope, numberStreams, streamList);
			answer = streamList[index];
		}
		return answer;
	}

	UInt32 Device::GetTotalNumberChannels(CMIOObjectPropertyScope scope) const
	{
		UInt32 answer = 0;
		#warning Device::GetTotalNumberChannels() needs to be implemented
		PropertyAddress address(kCMIODevicePropertyStreamConfiguration, scope);
		UInt32 size = GetPropertyDataSize(address, 0, NULL);
		CAAutoFree<CMIODeviceStreamConfiguration> streamConfiguration(size);
		UInt32 dataUsed = 0;
		GetPropertyData(address, 0, NULL, size, dataUsed, streamConfiguration);

		for(UInt32 index = 0; index < streamConfiguration->mNumberStreams; ++index)
		{
			answer += streamConfiguration->mNumberChannels[index];
		}
		return answer;
	}

	void Device::GetCurrentFormats(CMIOObjectPropertyScope scope, UInt32& ioNumberStreams, CMFormatDescriptionRef* descriptions) const
	{
		ioNumberStreams = std::min(ioNumberStreams, GetNumberStreams(scope));
		for(UInt32 index = 0; index < ioNumberStreams; ++index)
		{
			Stream stream(GetStreamByIndex(scope, index));
			descriptions[index] = stream.GetCurrentFormat();
		}
	}

	#pragma mark -
	void Device::GetNameForTransportType(UInt32 transportType, char* name, UInt32 maximumNameLength)
	{
		switch(transportType)
		{
			case 0:
				snprintf(name, maximumNameLength, "%s", "Unknown");
				break;
				
			case kIOAudioDeviceTransportTypeBuiltIn:
				snprintf(name, maximumNameLength, "%s", "Built-In");
				break;
				
			case kIOAudioDeviceTransportTypePCI:
				snprintf(name, maximumNameLength, "%s", "PCI");
				break;
				
			case kIOAudioDeviceTransportTypeUSB:
				snprintf(name, maximumNameLength, "%s", "USB");
				break;
				
			case kIOAudioDeviceTransportTypeFireWire:
				snprintf(name, maximumNameLength, "%s", "FireWire");
				break;
				
			case kIOAudioDeviceTransportTypeNetwork:
				snprintf(name, maximumNameLength, "%s", "Network");
				break;
				
			case kIOAudioDeviceTransportTypeWireless:
				snprintf(name, maximumNameLength, "%s", "Wireless");
				break;
				
			case kIOAudioDeviceTransportTypeOther:
				snprintf(name, maximumNameLength, "%s", "Other");
				break;
			
			case 'virt':	//	kIOAudioDeviceTransportTypeVirtual
				snprintf(name, maximumNameLength, "%s", "Virtual");
				break;
				
			default:
				{
					char* the4CC = (char*)&transportType;
					if ( 4 < maximumNameLength )
					{
						name[0] = the4CC[0];
						name[1] = the4CC[1];
						name[2] = the4CC[2];
						name[3] = the4CC[3];
						name[4] = 0;
					}
					else if ( 0 < maximumNameLength )
					{
						name[0] = 0;
					}
				}
				break;
		};
	}
}}
