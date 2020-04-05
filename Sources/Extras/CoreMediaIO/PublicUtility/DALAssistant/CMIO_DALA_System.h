/*
	    File: CMIO_DALA_System.h
	Abstract: C++ wrapper for the kCMIOObjectSystemObject
	 Version: 1.2

*/

#if !defined(__CMIO_DALA_System_h__)
#define __CMIO_DALA_System_h__

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// System Includes
#include <CoreFoundation/CFArray.h>
#include <CoreMediaIO/CMIOHardware.h>

namespace CMIO { namespace DALA
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// System
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class System
	{
	//	Operations
	public:
		static bool						IsInitingOrExiting();
		static UInt32					GetNumberDevices();
		static void						GetDevices(UInt32& ioNumberDevices, CMIODeviceID* devices);
		static CMIODeviceID				GetDeviceAtIndex(UInt32 index);
		static UInt32					GetIndexForDevice(const CMIODeviceID device);
		static CMIODeviceID				GetDeviceForUID(CFStringRef uid);
		
		static CMIODeviceID				GetDefaultDevice(CMIOObjectPropertyScope scope);
		static void						SetDefaultDevice(CMIOObjectPropertyScope scope, CMIODeviceID device);

		static bool						AllowsIdleSleepDuringIO();
		static void						SetAllowsIdleSleepDuringIO(bool allowIdleSleep);
		
		static bool						IsMaster();
		
	// Property Operations
	public:
		static bool						HasProperty(const CMIOObjectPropertyAddress& address);
		static bool						IsPropertySettable(const CMIOObjectPropertyAddress& address);
		
		static UInt32					GetPropertyDataSize(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData);
		static void						GetPropertyData(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData, UInt32 dataSize, UInt32& dataUsed, void* data);
		static void						SetPropertyData(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData, UInt32 dataSize, const void* data);
		
		static void						AddPropertyListener(const CMIOObjectPropertyAddress& address, CMIOObjectPropertyListenerProc listenerProc, void* clientData);
		static void						RemovePropertyListener(const CMIOObjectPropertyAddress& address, CMIOObjectPropertyListenerProc listenerProc, void* clientData);
	};
}}

#endif
