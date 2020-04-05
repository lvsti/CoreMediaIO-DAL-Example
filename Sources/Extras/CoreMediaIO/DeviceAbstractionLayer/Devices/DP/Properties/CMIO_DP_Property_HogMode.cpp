/*
	    File: CMIO_DP_Property_HogMode.cpp
	Abstract: Implements the kCMIODevicePropertyHogMode property with a non-settable value of -1 (the device can be used by any process).
				However, it does allow the property to be set internally to a valid PID, in the event it is necessary to signify that the device is in use by other software such as a
				QuickTime video digitizer.
				
				NOTE:  This has NO RELATION to the CMIO::DP::HogMode object which is implemented in terms of a global preference and uses CFDistributedNotificationCenter for notifications.
				That object is intended for devices which are not shareable across processes.
	
	 Version: 1.2

*/

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Self Include
#include "CMIO_DP_Property_HogMode.h"

// Internal Includes
#include "CMIO_DP_Device.h"

// Public Utility Includes
#include "CMIODebugMacros.h"

// CA Public Utility Includes
#include "CAException.h"

namespace CMIO { namespace DP { namespace Property
{
	#pragma mark -
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetPropertyDataSize()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	UInt32 HogMode::GetPropertyDataSize(const CMIOObjectPropertyAddress& address, UInt32 /*qualifierDataSize*/, const void* /*qualifierData*/) const
	{
		UInt32 answer = 0;
		
		switch (address.mSelector)
		{
			case kCMIODevicePropertyHogMode:
				answer = sizeof(pid_t);
				break;
		};
		
		return answer;
	}
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetPropertyData()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void HogMode::GetPropertyData(const CMIOObjectPropertyAddress& address, UInt32 /*qualifierDataSize*/, const void* /*qualifierData*/, UInt32 dataSize, UInt32& dataUsed, void* data) const
	{
		switch (address.mSelector)
		{
			case kCMIODevicePropertyHogMode:
				ThrowIf(dataSize != sizeof(pid_t), CAException(kCMIOHardwareBadPropertySizeError), "CMIO::DP::IIDC::Property::HogMode::GetPropertyData: wrong data size for kCMIODevicePropertyHogMode");
				*(static_cast<pid_t*>(data)) = mOwner;
				dataUsed = sizeof(pid_t);
				break;
		};
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetImplementedAddressByIndex()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void HogMode::GetImplementedAddressByIndex(UInt32 index, CMIOObjectPropertyAddress& address) const
	{
		switch(index)
		{
			case 0:
				address.mSelector = kCMIODevicePropertyHogMode;
				address.mScope = kCMIOObjectPropertyScopeGlobal;
				address.mElement = kCMIOObjectPropertyElementMaster;
				break;
		};
	}

	#pragma mark -
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// UpdateOwner()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void HogMode::SetHogMode(pid_t owner, bool sendChangeNotifications) 
	{
		// Send out property change notifications if the master is different
		if (owner != mOwner)
		{
			mOwner = owner;
			
			if (sendChangeNotifications)
			{
				PropertyAddress changedAddress(kCMIODevicePropertyHogMode);
				mOwningDevice.PropertiesChanged(1, &changedAddress);
			}
		}
	}
}}}
