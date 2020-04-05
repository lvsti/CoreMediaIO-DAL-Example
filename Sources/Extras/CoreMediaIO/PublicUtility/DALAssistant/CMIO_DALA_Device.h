/*
	    File: CMIO_DALA_Device.h
	Abstract: C++ wrapper for CMIODeviceID
	 Version: 1.2

*/

#if !defined(__CMIO_DALA_Device_h__)
#define __CMIO_DALA_Device_h__

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Public Utility Includes
#include "CMIO_DALA_Object.h"

// System Includes
#include <CoreMedia/CMFormatDescription.h>
#include <CoreMediaIO/CMIOHardwareDevice.h>
#include <sys/types.h>

namespace CMIO { namespace DALA
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Device
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class Device : public Object
	{
	// Construction/Destruction
	public:
								Device(CMIODeviceID cmioDeviceID);
		virtual					~Device();

	// General Operations
	public:
		CMIODeviceID			GetCMIODeviceID() const { return GetObjectID(); }
		CFStringRef				CopyUID() const;
		bool					HasModelUID() const;
		CFStringRef				CopyModelUID() const;
		UInt32					GetTransportType() const;
		bool					CanBeDefaultDevice(CMIOObjectPropertyScope scope) const;
		
		bool					HasDevicePlugInStatus() const;
		OSStatus				GetDevicePlugInStatus() const;
		
		bool					IsAlive() const;
		bool					IsRunning() const;
		void					SetIsRunning(bool isRunning);
		bool					IsRunningSomewhere() const;
		
		bool					HogModeIsSettable() const;
		pid_t					GetHogModeOwner() const;
		bool					TakeHogMode();
		void					ReleaseHogMode();
		
		pid_t					GetDeviceMasterOwner() const;
		bool					TakeDeviceMaster();
		void					ReleaseDeviceMaster();

		bool					HasExcludeNonDALAccess() const;
		bool					GetExcludeNonDALAccess() const;
		void					SetExcludeNonDALAccess(bool exclude);
		
		bool					HasVideoDigitizerComponents() const;
		UInt32					GetNumberVideoDigitizerComponents() const;
		void					GetVideoDigitizerComponents(UInt32& ioNumberVideoDigitizerComponents, ComponentDescription* componentDescriptions) const;
		
		bool					CanProcessAVCCommands() const;
		bool					CanProcessRS422Commands() const;
		
		bool					HasSuspendedByUser() const;
		bool					SuspendedByUser() const;
		
	// IO Operations
	public:
		UInt32					GetLatency(CMIOObjectPropertyScope scope) const;
		
	// Stream Operations
	public:
		UInt32					GetNumberStreams(CMIOObjectPropertyScope scope) const;
		void					GetStreams(CMIOObjectPropertyScope scope, UInt32& ioNumberStreams, CMIOStreamID* streamList) const;
		CMIOStreamID			GetStreamByIndex(CMIOObjectPropertyScope scope, UInt32 index) const;
		
		bool					HasSection(CMIOObjectPropertyScope scope) const { return GetNumberStreams(scope) > 0; }
		UInt32					GetTotalNumberChannels(CMIOObjectPropertyScope scope) const;

		void					StartStream(CMIOStreamID streamID);
		void					StopStream(CMIOStreamID streamID);

	// Format Operations
	public:
		void					GetCurrentFormats(CMIOObjectPropertyScope scope, UInt32& ioNumberStreams, CMFormatDescriptionRef* descriptions) const;
		
	// Utility Operations
	public:
		static void				GetNameForTransportType(UInt32 transportType, char* name, UInt32 maximumNameLength);
	};
}}

#endif
