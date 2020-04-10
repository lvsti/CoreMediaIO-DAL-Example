/*
	    File: CMIO_DP_Property_SuspendedByUser.cpp
	Abstract: Implements the kCMIODevicePropertySuspendedByUser property.
	 Version: 1.2

*/

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Self Include
#include "CMIO_DP_Property_SuspendedByUser.h"

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
	UInt32 SuspendedByUser::GetPropertyDataSize(const CMIOObjectPropertyAddress& address, UInt32 /*qualifierDataSize*/, const void* /*qualifierData*/) const
	{
		UInt32 answer = 0;
		
		switch (address.mSelector)
		{
			case kCMIODevicePropertySuspendedByUser:
				answer = sizeof(UInt32);
				break;
		};
		
		return answer;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetPropertyData()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void SuspendedByUser::GetPropertyData(const CMIOObjectPropertyAddress& address, UInt32 /*qualifierDataSize*/, const void* /*qualifierData*/, UInt32 dataSize, UInt32& dataUsed, void* data) const
	{
		switch (address.mSelector)
		{
			case kCMIODevicePropertySuspendedByUser:
				ThrowIf(dataSize != sizeof(UInt32), CAException(kCMIOHardwareBadPropertySizeError), "CMIO::DP::VDC::Property::SuspendedByUser::GetPropertyData: wrong data size for kCMIODevicePropertySuspendedByUser");
				*static_cast<UInt32*>(data) = IsSuspendedByUser() ? 1 : 0;
				dataUsed = sizeof(UInt32);
				break;
		};
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetImplementedAddressByIndex()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void SuspendedByUser::GetImplementedAddressByIndex(UInt32 index, CMIOObjectPropertyAddress& address) const
	{
		switch(index)
		{
			case 0:
				address.mSelector = kCMIODevicePropertySuspendedByUser;
				address.mScope = kCMIOObjectPropertyScopeGlobal;
				address.mElement = kCMIOObjectPropertyElementMaster;
				break;
		};
	}

	#pragma mark -
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// SetSuspendedByUser()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void SuspendedByUser::SetSuspendedByUser(bool suspendedByUser) 
	{
		// Send out property change notifications if the state is different
		if (suspendedByUser != mSuspendedByUser)
		{
			mSuspendedByUser = suspendedByUser;
			PropertyAddress changedAddress(kCMIODevicePropertySuspendedByUser);
			mOwningDevice.PropertiesChanged(1, &changedAddress);
		}
	}
}}}
