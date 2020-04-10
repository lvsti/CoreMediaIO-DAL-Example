/*
	    File: CMIO_DP_Sample_PlugIn.h
	Abstract: n/a
	 Version: 1.2

*/

#if !defined(__CMIO_DP_Sample_PlugIn_h__)
#define __CMIO_DP_Sample_PlugIn_h__

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Super Class Includes
#include "CMIO_DP_PlugIn.h"

// Internal Includes
#include "CMIO_DPA_Sample_ClientExtras.h"

// CA Public Utility Includes
#include "CACFMachPort.h"

namespace CMIO { namespace DP { namespace Sample
{
	class Device;

	class PlugIn : public DP::PlugIn
	{
	// Construction/Destruction
	public:
							PlugIn(CFUUIDRef factoryUUID, const char* assistantServiceName);
		virtual				~PlugIn();

		virtual void		InitializeWithObjectID(CMIOObjectID objectID);
		virtual void		Teardown();

	// Property Access
	public:
		virtual bool		HasProperty(const CMIOObjectPropertyAddress& address) const;
		virtual bool		IsPropertySettable(const CMIOObjectPropertyAddress& address) const;
		virtual UInt32		GetPropertyDataSize(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData) const;
		virtual void		GetPropertyData(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData, UInt32 dataSize, UInt32& dataUsed, void* data) const;
		virtual void		SetPropertyData(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData, UInt32 dataSize, const void* data);

	// SampleAssistant
	protected:
		DPA::Sample::Port	mAssistantPort;				// Mach port used to send messages to SampleAssistant
		CACFMachPort*		mDeviceEventPort;			// Port from which messages are received from SampleAssistant
		dispatch_source_t	mDeviceEventDispatchSource;	// Dispatch source for the device event port
		UInt64				mAssistantCrashAnchorTime;	// Anchor time for tracking how often the Assistant crashes
		UInt32				mAssistantCrashCount;		// Number of crashes since the crash anchor time
        char*               mAssistantServiceName;

	// Device Management
	protected:
		static void			DeviceEvent(PlugIn& plugIn);
		void				UpdateDeviceStates();
		Device&				GetDeviceByGUID(UInt64 guid) const;
 		void				DeviceArrived(UInt64 guid, const io_string_t& registryPath, Boolean isIOBacked);
		void				DeviceRemoved(Device& device);
	};
}}}

#endif
