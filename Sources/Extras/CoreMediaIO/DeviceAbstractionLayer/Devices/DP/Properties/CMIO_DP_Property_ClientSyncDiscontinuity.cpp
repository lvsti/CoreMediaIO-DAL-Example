/*
	    File: CMIO_DP_Property_ClientSyncDiscontinuity.cpp
	Abstract: Implements the kCMIODevicePropertyClientSyncDiscontinuity property which allows a client to force a discontinuity into a stream in order to synchronize with data coming
				from a CMIO processing graph.
	
	 Version: 1.2

*/

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Self Include
#include "CMIO_DP_Property_ClientSyncDiscontinuity.h"

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
	bool ClientSyncDiscontinuity::IsActive(const CMIOObjectPropertyAddress& /*address*/) const
	{
		return true;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// IsPropertySettable()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool ClientSyncDiscontinuity::IsPropertySettable(const CMIOObjectPropertyAddress& address) const
	{
		bool answer = false;
		
		switch (address.mSelector)
		{
			case kCMIODevicePropertyClientSyncDiscontinuity:
				answer = true;
				break;
		};
		
		return answer;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetPropertyDataSize()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	UInt32 ClientSyncDiscontinuity::GetPropertyDataSize(const CMIOObjectPropertyAddress& address, UInt32 /*qualifierDataSize*/, const void* /*qualifierData*/) const
	{
		UInt32 answer = 0;
		
		switch (address.mSelector)
		{
			case kCMIODevicePropertyClientSyncDiscontinuity:
				answer = sizeof(Boolean);
				break;
		};
		
		return answer;
	}
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetPropertyData()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void ClientSyncDiscontinuity::GetPropertyData(const CMIOObjectPropertyAddress& address, UInt32 /*qualifierDataSize*/, const void* /*qualifierData*/, UInt32 dataSize, UInt32& dataUsed, void* data) const
	{
		switch (address.mSelector)
		{
			case kCMIODevicePropertyClientSyncDiscontinuity:
				ThrowIf(dataSize != sizeof(Boolean), CAException(kCMIOHardwareBadPropertySizeError), "CMIO::DP::ClientSyncDiscontinuity::GetPropertyData: wrong data size for kCMIODevicePropertyClientSyncDiscontinuity");
				*(static_cast<Boolean*>(data)) = mForceDiscontinuity;
				dataUsed = sizeof(Boolean);
				break;
		};
	}
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// SetPropertyData()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void ClientSyncDiscontinuity::SetPropertyData(const CMIOObjectPropertyAddress& address, UInt32 /*qualifierDataSize*/, const void* /*qualifierData*/, UInt32 dataSize, const void* data)
	{
		switch (address.mSelector)
		{
			case kCMIODevicePropertyClientSyncDiscontinuity:
				ThrowIf(dataSize != sizeof(Boolean), CAException(kCMIOHardwareBadPropertySizeError), "CMIO::DP::ClientSyncDiscontinuity::SetPropertyData: wrong data size for kCMIODevicePropertyClientSyncDiscontinuity");
				Boolean forceDiscontinuity = *(static_cast<const Boolean*>(data));
				
				if (forceDiscontinuity != mForceDiscontinuity)
				{
					// Instruct the device that the force discontinuity state is changing
					GetOwningDevice().SetForceDiscontinuity(forceDiscontinuity);
					mForceDiscontinuity = forceDiscontinuity;
					
					// Notify that the property changed
					PropertyAddress changedAddress(kCMIODevicePropertyClientSyncDiscontinuity);
					GetOwningDevice().PropertiesChanged(1, &changedAddress);
				}
				break;
		};
	}
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetNumberAddressesImplemented()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	UInt32 ClientSyncDiscontinuity::GetNumberAddressesImplemented() const
	{
		return 1;
	}
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetImplementedAddressByIndex()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void ClientSyncDiscontinuity::GetImplementedAddressByIndex(UInt32 index, CMIOObjectPropertyAddress& address) const
	{
		switch (index)
		{
			case 0:
				address.mSelector = kCMIODevicePropertyClientSyncDiscontinuity;
				address.mScope = kCMIOObjectPropertyScopeWildcard;
				address.mElement = kCMIOObjectPropertyElementWildcard;
				break;
		};
	}
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// SetForceDiscontinuity()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void ClientSyncDiscontinuity::SetForceDiscontinuity(Boolean forceDiscontinuity, bool sendChangeNotifications) 
	{
		// Send out property change notifications if the master is different
		if (forceDiscontinuity != mForceDiscontinuity)
		{
			mForceDiscontinuity = forceDiscontinuity;
			
			if (sendChangeNotifications)
			{
				PropertyAddress changedAddress(kCMIODevicePropertyClientSyncDiscontinuity);
				GetOwningDevice().PropertiesChanged(1, &changedAddress);
			}
		}
	}
}}}
