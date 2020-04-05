/*
	    File: CMIO_DP_Sample_Property_HogMode.cpp
	Abstract: Implements the kCMIODevicePropertyHogMode property with a non-settable value of -1, meaning that the device can be used by any process.
	
	 Version: 1.2

*/

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Self Include
#include "CMIO_DP_Sample_Property_HogMode.h"

// CA Public Utility Includes
#include "CADebugMacros.h"
#include "CAException.h"

namespace CMIO { namespace DP { namespace Sample { namespace Property
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
				ThrowIf(dataSize != sizeof(pid_t), CAException(kCMIOHardwareBadPropertySizeError), "CMIO::DP::Sample::Property::HogMode::GetPropertyData: wrong data size for kCMIODevicePropertyHogMode");
				*(static_cast<pid_t*>(data)) = -1;
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
}}}}
