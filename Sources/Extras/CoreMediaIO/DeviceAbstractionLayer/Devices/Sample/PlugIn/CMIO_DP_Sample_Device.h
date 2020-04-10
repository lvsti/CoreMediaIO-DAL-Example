/*
	    File: CMIO_DP_Sample_Device.h
	Abstract: n/a
	 Version: 1.2

*/

#if !defined(__CMIO_DP_Sample_Device_h__)
#define __CMIO_DP_Sample_Device_h__

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Super Class Includes
#include "CMIO_DP_Device.h"

// Internal Includes
#include "CMIO_DP_Property_SMPTETimeCallback.h"

// Public Utility Includes
#include "CMIO_CMA_FormatDescription.h"

// CA Public Utility Includes
#include "CACFDictionary.h"
#include "CACFMachPort.h"
#include "CACFString.h"

// System Includes
#include <IOKit/audio/IOAudioTypes.h>

namespace CMIO
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Types in the CMIO namespace
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class Buffer;
}

namespace CMIO { namespace DP { namespace Property
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Types in the CMIO::DP::Property namespace
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class ClientSyncDiscontinuity;
	class HogMode;
}}}

namespace CMIO { namespace DP { namespace Sample
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Types in the CMIO::DP::Sample namespace
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class PlugIn;
}}}

namespace CMIO { namespace DP { namespace Sample
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Device
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class Device : public DP::Device
	{
	// Construction/Destruction
	protected:
												Device(PlugIn& plugIn, CMIODeviceID deviceID, mach_port_t assistantPort, UInt64 guid);
    
    public:
		virtual									~Device();
	
		virtual void							Initialize();
		virtual void							Teardown();
		virtual void							Finalize();

	protected:
		UInt64									mDeviceGUID;	// The 'pseduo-GUID' of the device returned from the Assistant

	// Attributes
	public:
		PlugIn&									GetPlugIn()	{ return reinterpret_cast<PlugIn&>(DP::Device::GetPlugIn()); }
		virtual CFStringRef						CopyDeviceName() const ;
		virtual CFStringRef						CopyDeviceManufacturerName() const { return mDeviceManufacturerName.CopyCFString(); }
		virtual CFStringRef						CopyDeviceUID() const { return mDeviceUID.CopyCFString(); }
		virtual CFStringRef						CopyModelUID() const;
		virtual bool							HogModeIsOwnedBySelfOrIsFree() const;
		virtual void							SetDeviceMaster(pid_t masterPID);
		virtual void							SetExcludeNonDALAccess(bool excludeNonDALAccess);
		virtual void							SetForceDiscontinuity(Boolean forceDiscontinuity);
		virtual UInt32							GetTransportType() const { return kIOAudioDeviceTransportTypePCI; }
		virtual bool							IsDeviceRunningSomewhere() const { return mDeviceIsRunningSomewhere; }
		UInt64									GetDeviceGUID() const { return mDeviceGUID; }	
		mach_port_t								GetAssistantPort() const { return mAssistantPort; }
		
	protected:
		mach_port_t								mAssistantPort;					// Mach port used to send messages to IIDCVideoAssistant
        CACFString								mDeviceUID;		
		CACFString								mDeviceName;
		CACFString								mDeviceManufacturerName;
		
	// Property Access
	public:
		virtual bool							HasProperty(const CMIOObjectPropertyAddress& address) const;
		virtual bool							IsPropertySettable(const CMIOObjectPropertyAddress& address) const;
		virtual UInt32							GetPropertyDataSize(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData) const;
		virtual void							GetPropertyData(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData, UInt32 dataSize, UInt32& dataUsed, void* data) const;
		virtual void							SetPropertyData(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData, UInt32 dataSize, const void* data);

	protected:
		virtual void							PropertyListenerAdded(const CMIOObjectPropertyAddress& address);
		void									CreateProperties();
		void									ReleaseProperties();

		UInt64									mPropertyCacheTime;				// When the values of the properties were last cached
		DP::Property::HogMode*					mHogMode;
		DP::Property::ClientSyncDiscontinuity*	mClientSyncDiscontinuity;
		DP::Property::SMPTETimeCallback*		mSMPTETimeCallback;
		bool									mDeviceIsRunningSomewhere;

	// Controls
	public:
		DP::Control*							GetControlByControlID(UInt32 controlID) const;
		virtual void							SetControlValue(UInt32 controlID, UInt32 value, UInt32* newValue);
		virtual CFDictionaryRef 				CopyControlDictionaryByControlID(UInt32 controlID) const;

	protected:
		virtual void							CreateRegistryControls();
		void									CreatePluginControls();
		void									ReleaseControls(bool reportDeath = false);

		UInt64									mControlCacheTime;				// When the values of the controls was last cached

	// Property / Control Updates from Assistant
	protected:
		static void								Event(CFMachPortRef port, mach_msg_header_t* header, CFIndex size, Device& device);
		void									UpdateControlStates(bool sendChangeNotifications);
		void									UpdatePropertyStates();
		CACFMachPort&							GetEventPort()  { return mEventPort; }

		CACFMachPort							mEventPort;						// Port from which control/property event messages are received from the Assistant

	// Command Management
	protected:
		virtual bool							IsSafeToExecuteCommand(DP::Command* command);
		virtual bool							StartCommandExecution(void** savedCommandState);
		virtual void							FinishCommandExecution(void* savedCommandState);
	
	// RS422 Commands
	public:
		virtual void							ProcessRS422Command(CMIODeviceRS422Command* ioRS422Command);

	// Unplugging Management
	public:
		virtual void							Unplug();
		
	// Stream Management
	public:
		virtual void							StartStream(CMIOStreamID streamID);
		virtual void							StopStream(CMIOStreamID streamID);
		virtual void							StopAllStreams();
		virtual void							SuspendAllStreams();
		virtual void							StreamDirectionChanged(CMIOObjectPropertyScope newScope);
		
	protected:
		void									CreateStreams(CMIOObjectPropertyScope scope);
		void									ReleaseStreams(bool reportDeath = false);
	
	// Accessors / Setters for Properties
	public:
		OSStatus								GetSMPTETime(UInt64* frameCount, Boolean* isDropFrame, UInt32* tolerance) { if (mSMPTETimeCallback) return mSMPTETimeCallback->DoCallback(frameCount, isDropFrame, tolerance); else return kCMIOHardwareIllegalOperationError; }
	};
}}}
#endif
