/*
	    File: CMIO_DPA_Sample_Server_IOBackedDevice.h
	Abstract: n/a
	 Version: 1.2
	
*/

#if !defined(__CMIO_DPA_Sample_Server_IOBackedDevice_h__)
#define __CMIO_DPA_Sample_Server_IOBackedDevice_h__

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "CMIO_DPA_Sample_Server_Device.h"

// Internal Includes
#include "CMIO_DPA_Sample_Server_Common.h"
#include "CMIO_DPA_Sample_Server_IOBackedStream.h"
#include "CMIO_DPA_Sample_Shared.h"

// Public Utility Includes
#include "CMIO_IOKA_PowerNotificationPort.h"
#include "CMIO_IOKA_Object.h"
#include "CMIO_IOVA_Assistance.h"

// CA Public Utility Includes
#include "CACFArray.h"
#include "CACFDictionary.h"
#include "CAGuard.h"

// System Includes
#include <CoreMedia/CMTime.h>
#include <CoreMediaIO/CMIOSampleBuffer.h>
#include <IOKit/video/IOVideoTypes.h>

namespace CMIO { namespace PTA
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Types in the CMIO::PTA namespace
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class NotificationPortThread;
}}

namespace CMIO { namespace DPA { namespace Sample { namespace Server
{
    class IOBackedDevice: public Device
	{
	public:

    #pragma mark -
	#pragma mark Device
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Device
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Construction/Destruction
	public:
										IOBackedDevice(IOKA::Object& registryEntry, PTA::NotificationPortThread& notificationPortThread);
		virtual							~IOBackedDevice();
	
	private:
		IOBackedDevice&					operator=(IOBackedDevice& that); // Don't allow copying
		
	protected:
		IOKA::Object					mRegistryEntry;				// The IOKit registry entry for the device
		io_string_t						mRegistryPath;				// The registry path for the device
        IOVA::PlugIn					mIOVAPlugIn;				// IOVA wrapper kIOVideoDeviceLibTypeID's IOCFPlugInInterface** 
        IOVA::Device					mIOVADevice;				// IOVA wrapper for IOVideoDeviceRef

	// Attributes
	public:
		void							GetRegistryPath(io_string_t path) const { (void) strlcpy(path, mRegistryPath, sizeof(io_string_t)); }
		IOVA::Device&					GetIOVADevice() { return mIOVADevice; }
	
	// Stream Management
	protected:
		void	        				DiscoverStreams();

	// Controls
	public:
		virtual void					SetControl(Client client, UInt32 controlID, UInt32 value, UInt32* newValue) override;

	protected:
		void							InitializeControls();

	
	// Notifications
	public:
		PTA::NotificationPortThread&	GetNotificationThread()	{ return mNotificationThread; }
	protected:
        static void						IOVDeviceNotification(IOVideoDeviceRef deviceRef, IOBackedDevice& device, const IOVideoDeviceNotificationMessage& message);
		void							DeviceNotification(const IOVideoDeviceNotification& notification);
		void							StreamNotification(const IOVideoDeviceNotification& notification, Stream& stream);
		void							ControlNotification(const IOVideoDeviceNotification& notification, UInt64 shadowTime);
		static void						PowerNotification(IOBackedDevice& device, io_service_t unused, natural_t messageType, void* message);

		PTA::NotificationPortThread&	mNotificationThread;		// Assistant's thread for getting IOKit notifications
		IOKA::PowerNotificationPort		mPowerNotificationPort;		// For receiving power notifications

	// Misc
	protected:
		bool							mSleeping;					// True when put to sleep
		bool							mRestartStreamOnWake;		// True if the stream should be restarted upon waking from sleep
	
		void							Sleep();
		void							Wake();
	};
}}}}
#endif
