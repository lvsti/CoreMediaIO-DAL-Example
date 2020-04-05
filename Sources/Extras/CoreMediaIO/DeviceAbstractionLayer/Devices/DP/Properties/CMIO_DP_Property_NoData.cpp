/*
	    File: CMIO_DP_Property_NoData.cpp
	Abstract: Implements the kCMIOStreamPropertyNoDataTimeoutInMSec, kCMIOStreamPropertyDeviceSyncTimeoutInMSec, and kCMIOStreamPropertyNoDataEventCount properties.
				These deal with a device detecting that no data is coming in (such as a DV device on a section of blank tape).
	
	 Version: 1.2

*/

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Self Include
#include "CMIO_DP_Property_NoData.h"

// Internal Includes
#include "CMIO_DP_Stream.h"

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
	bool NoData::IsActive(const CMIOObjectPropertyAddress& /*address*/) const
	{
		return true;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// IsPropertySettable()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool NoData::IsPropertySettable(const CMIOObjectPropertyAddress& address) const
	{
		bool answer = false;
		
		switch (address.mSelector)
		{
			case kCMIOStreamPropertyNoDataTimeoutInMSec:
			case kCMIOStreamPropertyDeviceSyncTimeoutInMSec:
				answer = true;
				break;
			
			case kCMIOStreamPropertyNoDataEventCount:
				answer = false;
				break;
		};
		
		return answer;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetPropertyDataSize()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	UInt32 NoData::GetPropertyDataSize(const CMIOObjectPropertyAddress& address, UInt32 /*qualifierDataSize*/, const void* /*qualifierData*/) const
	{
		UInt32 answer = 0;
		
		switch (address.mSelector)
		{
			case kCMIOStreamPropertyNoDataTimeoutInMSec:
			case kCMIOStreamPropertyDeviceSyncTimeoutInMSec:
			case kCMIOStreamPropertyNoDataEventCount:
				answer = sizeof(UInt32);
				break;
		};
		
		return answer;
	}
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetPropertyData()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void NoData::GetPropertyData(const CMIOObjectPropertyAddress& address, UInt32 /*qualifierDataSize*/, const void* /*qualifierData*/, UInt32 dataSize, UInt32& dataUsed, void* data) const
	{
		switch (address.mSelector)
		{
			case kCMIOStreamPropertyNoDataTimeoutInMSec:
				ThrowIf(dataSize != sizeof(UInt32), CAException(kCMIOHardwareBadPropertySizeError), "CMIO::DP::NoData::GetPropertyData: wrong data size for kCMIOStreamPropertyNoDataTimeoutInMSec");
				*(static_cast<UInt32*>(data)) = mTimeout;
				dataUsed = sizeof(UInt32);
				break;
			
			case kCMIOStreamPropertyDeviceSyncTimeoutInMSec:
				ThrowIf(dataSize != sizeof(UInt32), CAException(kCMIOHardwareBadPropertySizeError), "CMIO::DP::NoData::GetPropertyData: wrong data size for kCMIOStreamPropertyDeviceSyncTimeoutInMSec");
				*(static_cast<UInt32*>(data)) = mDeviceSyncTimeout;
				dataUsed = sizeof(UInt32);
				break;
			
			case kCMIOStreamPropertyNoDataEventCount:
				ThrowIf(dataSize != sizeof(UInt32), CAException(kCMIOHardwareBadPropertySizeError), "CMIO::DP::NoData::GetPropertyData: wrong data size for kCMIOStreamPropertyNoDataEventCount");
				*(static_cast<UInt32*>(data)) = mEventCount;
				dataUsed = sizeof(UInt32);
				break;
		};
	}
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// SetPropertyData()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void NoData::SetPropertyData(const CMIOObjectPropertyAddress& address, UInt32 /*qualifierDataSize*/, const void* /*qualifierData*/, UInt32 dataSize, const void* data)
	{
		switch (address.mSelector)
		{
			case kCMIOStreamPropertyNoDataTimeoutInMSec:
			{
				ThrowIf(dataSize != sizeof(UInt32), CAException(kCMIOHardwareBadPropertySizeError), "CMIO::DP::NoData::SetPropertyData: wrong data size for kCMIOStreamPropertyNoDataTimeoutInMSec");
				UInt32 timeout = *(static_cast<const UInt32*>(data));
				
				if (timeout != mTimeout)
				{
					// Instruct the device that the force discontinuity state is changing
					GetOwningStream().SetNoDataTimeout(timeout);
					mTimeout = timeout;
					
					// Notify that the property changed
					PropertyAddress changedAddress(kCMIOStreamPropertyNoDataTimeoutInMSec);
					GetOwningStream().PropertiesChanged(1, &changedAddress);
				}
				break;
			}
			
			case kCMIOStreamPropertyDeviceSyncTimeoutInMSec:
			{
				ThrowIf(dataSize != sizeof(UInt32), CAException(kCMIOHardwareBadPropertySizeError), "CMIO::DP::NoData::SetPropertyData: wrong data size for kCMIOStreamPropertyDeviceSyncTimeoutInMSec");
				UInt32 deviceSyncTimeout = *(static_cast<const UInt32*>(data));
				
				if (deviceSyncTimeout != mDeviceSyncTimeout)
				{
					// Instruct the device that the force discontinuity state is changing
					GetOwningStream().SetDeviceSyncTimeout(deviceSyncTimeout);
					mDeviceSyncTimeout = deviceSyncTimeout;
					
					// Notify that the property changed
					PropertyAddress changedAddress(kCMIOStreamPropertyDeviceSyncTimeoutInMSec);
					GetOwningStream().PropertiesChanged(1, &changedAddress);
				}
				break;
			}
		};
	}
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetNumberAddressesImplemented()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	UInt32 NoData::GetNumberAddressesImplemented() const
	{
		return 3;
	}
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetImplementedAddressByIndex()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void NoData::GetImplementedAddressByIndex(UInt32 index, CMIOObjectPropertyAddress& address) const
	{
		switch (index)
		{
			case 0:
				address.mSelector = kCMIOStreamPropertyNoDataTimeoutInMSec;
				address.mScope = kCMIOObjectPropertyScopeWildcard;
				address.mElement = kCMIOObjectPropertyElementWildcard;
				break;
			case 1:
				address.mSelector = kCMIOStreamPropertyDeviceSyncTimeoutInMSec;
				address.mScope = kCMIOObjectPropertyScopeWildcard;
				address.mElement = kCMIOObjectPropertyElementWildcard;
				break;
			case 2:
				address.mSelector = kCMIOStreamPropertyNoDataEventCount;
				address.mScope = kCMIOObjectPropertyScopeWildcard;
				address.mElement = kCMIOObjectPropertyElementWildcard;
				break;
		};
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// BumpNoDataEventCount()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void NoData::BumpNoDataEventCount(bool sendChangeNotifications)
	{
		// Bump the event count
		++mEventCount;
		
		// Send the property changed notification
		if (sendChangeNotifications)
		{
			PropertyAddress address(kCMIOStreamPropertyNoDataEventCount);
			GetOwningStream().PropertiesChanged(1, &address);
		}
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// SetNoDataTimeoutInMSec()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void NoData::SetNoDataTimeoutInMSec(UInt32 timeout, bool sendChangeNotifications) 
	{
		// Send out property change notifications there has been a change
		if (timeout != mTimeout)
		{
			mTimeout = timeout;
			
			if (sendChangeNotifications)
			{
				PropertyAddress changedAddress(kCMIOStreamPropertyNoDataTimeoutInMSec);
				GetOwningStream().PropertiesChanged(1, &changedAddress);
			}
		}
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// SetDeviceSyncTimeoutInMSec()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void NoData::SetDeviceSyncTimeoutInMSec(UInt32 deviceSyncTimeout, bool sendChangeNotifications) 
	{
		// Send out property change notifications there has been a change
		if (deviceSyncTimeout != mDeviceSyncTimeout)
		{
			mDeviceSyncTimeout = deviceSyncTimeout;
			
			if (sendChangeNotifications)
			{
				PropertyAddress changedAddress(kCMIOStreamPropertyNoDataTimeoutInMSec);
				GetOwningStream().PropertiesChanged(1, &changedAddress);
			}
		}
	}
}}}
