/*
	    File: CMIO_DP_Property_SMPTETimeCallback.cpp
	Abstract: Implements the kCMIODevicePropertySMPTETimeCallback property which allows a client to provide a callback which can be used to get the current SMPTE timestamp from a device.
	
	 Version: 1.2

*/

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Self Include
#include "CMIO_DP_Property_SMPTETimeCallback.h"

// Internal Includes
#include "CMIO_DP_Device.h"

// Public Utility Includes
#include "CMIODebugMacros.h"
#include "CMIO_PropertyAddress.h"

// CA Public Utility Includes
#include "CAException.h"

namespace CMIO { namespace DP { namespace Property
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// IsActive()
	//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool SMPTETimeCallback::IsActive(const CMIOObjectPropertyAddress& /*address*/) const
	{
		return true;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// IsPropertySettable()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool SMPTETimeCallback::IsPropertySettable(const CMIOObjectPropertyAddress& address) const
	{
		bool answer = false;
		
		switch (address.mSelector)
		{
			case kCMIODevicePropertySMPTETimeCallback:
				answer = true;
				break;
		};
		
		return answer;
	}


	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetPropertyDataSize()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	UInt32 SMPTETimeCallback::GetPropertyDataSize(const CMIOObjectPropertyAddress& address, UInt32 /*qualifierDataSize*/, const void* /*qualifierData*/) const
	{
		UInt32 answer = 0;
		
		switch (address.mSelector)
		{
			case kCMIODevicePropertySMPTETimeCallback:
				answer = sizeof(CMIODeviceSMPTETimeCallback);
				break;
		};
		
		return answer;
	}
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetPropertyData()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void SMPTETimeCallback::GetPropertyData(const CMIOObjectPropertyAddress& address, UInt32 /*qualifierDataSize*/, const void* /*qualifierData*/, UInt32 dataSize, UInt32& dataUsed, void* data) const
	{
		switch (address.mSelector)
		{
			case kCMIODevicePropertySMPTETimeCallback:
				ThrowIf(dataSize != sizeof(CMIODeviceSMPTETimeCallback), CAException(kCMIOHardwareBadPropertySizeError), "SMPTETimeCallback::GetData: wrong data size for kCMIODevicePropertySMPTETimeCallback");
				*(static_cast<CMIODeviceSMPTETimeCallback*>(data)) = mSMPTECallback;
				dataUsed = sizeof(CMIODeviceSMPTETimeCallback);
				break;
		};
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// SetPropertyData()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void SMPTETimeCallback::SetPropertyData(const CMIOObjectPropertyAddress& address, UInt32 /*qualifierDataSize*/, const void* /*qualifierData*/, UInt32 dataSize, const void* data)
	{
		switch (address.mSelector)
		{
			case kCMIODevicePropertySMPTETimeCallback:
				ThrowIf(dataSize != sizeof(CMIODeviceSMPTETimeCallback), CAException(kCMIOHardwareBadPropertySizeError), "SMPTETimeCallback::SetData: wrong data size for kCMIODevicePropertySMPTETimeCallback");
				mSMPTECallback = *(static_cast<const CMIODeviceSMPTETimeCallback*>(data));
				PropertyAddress address1(kCMIODevicePropertySMPTETimeCallback);
				mOwningDevice.PropertiesChanged(1, &address1);
				break;
		};
	}
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetNumberAddressesImplemented()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	UInt32 SMPTETimeCallback::GetNumberAddressesImplemented() const
	{
		return 1;
	}
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetImplementedAddressByIndex()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void SMPTETimeCallback::GetImplementedAddressByIndex(UInt32 index, CMIOObjectPropertyAddress& address) const
	{
		switch (index)
		{
			case 0:
				address.mSelector = kCMIODevicePropertySMPTETimeCallback;
				address.mScope = kCMIOObjectPropertyScopeWildcard;
				address.mElement = kCMIOObjectPropertyElementWildcard;
				break;
		};
	}
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// DoCallback()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	OSStatus SMPTETimeCallback::DoCallback(UInt64* frameNumber, Boolean* isDropFrame, UInt32* tolerance) const
	{
		if (NULL == mSMPTECallback.mGetSMPTETimeProc)
			return kCMIOHardwareIllegalOperationError;
			
		return (mSMPTECallback.mGetSMPTETimeProc)(mSMPTECallback.mRefCon, frameNumber, isDropFrame, tolerance);
	}
}}}
