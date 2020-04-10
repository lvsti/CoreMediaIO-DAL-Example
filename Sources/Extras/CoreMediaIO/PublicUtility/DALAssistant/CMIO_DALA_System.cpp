/*
	    File: CMIO_DALA_System.cpp
	Abstract: C++ wrapper for the kCMIOObjectSystemObject
	 Version: 1.2

*/

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Self Include
#include "CMIO_DALA_System.h"

// Public Utility Includes
#include "CMIODebugMacros.h"
#include "CMIO_PropertyAddress.h"

// CA Public Utility Includes
#include "CAAutoDisposer.h"
#include "CAException.h"

namespace CMIO { namespace DALA
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// IsInitingOrExiting()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool System::IsInitingOrExiting()
	{
		UInt32 isInitingOrExiting = 0;
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIOHardwarePropertyIsInitingOrExiting), 0, NULL, sizeof(UInt32), dataUsed, &isInitingOrExiting);
		return isInitingOrExiting != 0;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetNumberDevices()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	UInt32 System::GetNumberDevices()
	{
		UInt32 answer = GetPropertyDataSize(PropertyAddress(kCMIOHardwarePropertyDevices), 0, NULL);
		answer /= sizeof(CMIODeviceID);
		return answer;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetDevices()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void System::GetDevices(UInt32& ioNumberDevices, CMIODeviceID* devices)
	{

		UInt32 size = ioNumberDevices * SizeOf32(CMIODeviceID);
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIOHardwarePropertyDevices), 0, NULL, size, dataUsed, devices);
		ioNumberDevices = dataUsed / SizeOf32(CMIODeviceID);
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetDeviceAtIndex()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	CMIODeviceID System::GetDeviceAtIndex(UInt32 index)
	{
		CMIODeviceID answer = 0;
		UInt32 numberDevices = GetNumberDevices();

		if ((numberDevices > 0) and (index < numberDevices))
		{
			CAAutoArrayDelete<CMIODeviceID> deviceList(numberDevices);
			UInt32 size = numberDevices * sizeof(CMIODeviceID);
			UInt32 dataUsed = 0;
			GetPropertyData(PropertyAddress(kCMIOHardwarePropertyDevices), 0, NULL, size, dataUsed, deviceList);
			answer = deviceList[index];
		}

		return answer;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetIndexForDevice()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	UInt32 System::GetIndexForDevice(const CMIODeviceID device)
	{
		UInt32 answer = 0xFFFFFFFF;
		UInt32 numberDevices = GetNumberDevices();

		if (numberDevices > 0)
		{
			CAAutoArrayDelete<CMIODeviceID> deviceList(numberDevices);
			UInt32 size = numberDevices * sizeof(CMIODeviceID);
			UInt32 dataUsed = 0;
			GetPropertyData(PropertyAddress(kCMIOHardwarePropertyDevices), 0, NULL, size, dataUsed, deviceList);
			for (UInt32 index = 0; index < numberDevices; ++index)
			{
				if (device == deviceList[index])
				{
					answer = index;
					break;
				}
			}
		}
		return answer;
	}

	CMIODeviceID System::GetDeviceForUID(CFStringRef uid)
	{
		CMIODeviceID answer = 0;
		AudioValueTranslation value = { &uid, sizeof(CFStringRef), &answer, sizeof(CMIODeviceID) };
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIOHardwarePropertyDeviceForUID), 0, NULL, sizeof(AudioValueTranslation), dataUsed, &value);
		return answer;
	}

	CMIODeviceID System::GetDefaultDevice(CMIOObjectPropertyScope scope)
	{
		CMIODeviceID answer = 0;
		CMIOHardwarePropertyID propertyID = 0;
		if (kCMIODevicePropertyScopeInput == scope)
		{
			propertyID = kCMIOHardwarePropertyDefaultInputDevice;
		}
		else
		{
			propertyID = kCMIOHardwarePropertyDefaultOutputDevice;
		}
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(propertyID), 0, NULL, sizeof(CMIODeviceID), dataUsed, &answer);
		return answer;
	}

	void System::SetDefaultDevice(CMIOObjectPropertyScope scope, CMIODeviceID device)
	{
		CMIOHardwarePropertyID propertyID = 0;
		if (kCMIODevicePropertyScopeInput == scope)
		{
			propertyID = kCMIOHardwarePropertyDefaultInputDevice;
		}
		else
		{
			propertyID = kCMIOHardwarePropertyDefaultOutputDevice;
		}
		UInt32 size = sizeof(CMIODeviceID);
		SetPropertyData(PropertyAddress(propertyID), 0, NULL, size, &device);
	}

	bool System::AllowsIdleSleepDuringIO()
	{
		UInt32 value = 0;
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIOHardwarePropertySleepingIsAllowed), 0, NULL, sizeof(UInt32), dataUsed, &value);
		return value != 0;
	}

	void System::SetAllowsIdleSleepDuringIO(bool allowIdleSleep)
	{
		UInt32 value = allowIdleSleep ? 1 : 0;
		SetPropertyData(PropertyAddress(kCMIOHardwarePropertySleepingIsAllowed), 0, NULL, sizeof(UInt32), &value);
	}

	#pragma mark -
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// IsMaster()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool System::IsMaster()
	{
		UInt32 isMaster = 0;
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIOHardwarePropertyProcessIsMaster), 0, NULL, sizeof(UInt32), dataUsed, &isMaster);
		return isMaster != 0;
	}

	#pragma mark -
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// HasProperty()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool System::HasProperty(const CMIOObjectPropertyAddress& address)
	{
		return CMIOObjectHasProperty(kCMIOObjectSystemObject, &address);
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// PropertyIsSettable()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool System::IsPropertySettable(const CMIOObjectPropertyAddress& address)
	{
		Boolean isSettable = false;
		OSStatus error = CMIOObjectIsPropertySettable(kCMIOObjectSystemObject, &address, &isSettable);
		if (error)
		{
			char selector[5] = CA4CCToCString(address.mSelector);
			char scope[5] = CA4CCToCString(address.mScope);
			char err[5] = CA4CCToCString(error);
			DebugMessage("CMIO::System::PropertyIsSettable: error %d (%s) getting property selector (%s) scope (%s) element %d", error, err, selector, scope, address.mElement);
			throw CAException(error);
		} 

		return isSettable != 0;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetPropertyDataSize()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	UInt32 System::GetPropertyDataSize(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData)
	{
		UInt32 size = 0;
		OSStatus error = CMIOObjectGetPropertyDataSize(kCMIOObjectSystemObject, &address, qualifierDataSize, qualifierData, &size);
		if (error)
		{
			char selector[5] = CA4CCToCString(address.mSelector);
			char scope[5] = CA4CCToCString(address.mScope);
			char err[5] = CA4CCToCString(error);
			DebugMessage("CMIO::System::GetPropertyDataSize: error %d (%s) getting property selector (%s) scope (%s) element %d", error, err, selector, scope, address.mElement);
			throw CAException(error);
		} 

		return size;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetPropertyData()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void System::GetPropertyData(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData, UInt32 dataSize, UInt32& dataUsed, void* data)
	{
		OSStatus error = CMIOObjectGetPropertyData(kCMIOObjectSystemObject, &address, qualifierDataSize, qualifierData, dataSize, &dataUsed, data);
		if (error)
		{
			char selector[5] = CA4CCToCString(address.mSelector);
			char scope[5] = CA4CCToCString(address.mScope);
			char err[5] = CA4CCToCString(error);
			DebugMessage("CMIO::System::GetPropertyData: error %d (%s) getting property selector (%s) scope (%s) element %d", error, err, selector, scope, address.mElement);
			throw CAException(error);
		} 
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// SetPropertyData()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void System::SetPropertyData(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData, UInt32 dataSize, const void* data)
	{
		OSStatus error = CMIOObjectSetPropertyData(kCMIOObjectSystemObject, &address, qualifierDataSize, qualifierData, dataSize, const_cast<void*>(data));
		if (error)
		{
			char selector[5] = CA4CCToCString(address.mSelector);
			char scope[5] = CA4CCToCString(address.mScope);
			char err[5] = CA4CCToCString(error);
			DebugMessage("CMIO::System::SetPropertyData: error %d (%s) getting property selector (%s) scope (%s) element %d", error, err, selector, scope, address.mElement);
			throw CAException(error);
		} 
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// AddPropertyListener()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void System::AddPropertyListener(const CMIOObjectPropertyAddress& address, CMIOObjectPropertyListenerProc listenerProc, void* clientData)
	{
		OSStatus error = CMIOObjectAddPropertyListener(kCMIOObjectSystemObject, &address, listenerProc, clientData);
		ThrowIfError(error, CAException(error), "CMIO::System::AddPropertyListener: got an error adding a property listener");
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// RemovePropertyListener()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void System::RemovePropertyListener(const CMIOObjectPropertyAddress& address, CMIOObjectPropertyListenerProc listenerProc, void* clientData)
	{
		OSStatus error = CMIOObjectRemovePropertyListener(kCMIOObjectSystemObject, &address, listenerProc, clientData);
		ThrowIfError(error, CAException(error), "CMIO::System::RemovePropertyListener: got an error removing a property listener");
	}
}}
