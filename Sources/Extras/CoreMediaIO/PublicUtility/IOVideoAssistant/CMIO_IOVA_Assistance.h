/*
	    File: CMIO_IOVA_Assistance.h
	Abstract: The IOVideo Assisistant (IOVA) consists of various objects to facilitate using the "Acquisition is Initialization" design pattern.
				NOTE: much of the IOVA is sparsely implemented, meaning that it does not attempt to provide wrappers/access  to all IOVideo features, but just those needed by the IOVideo
				plugin.
	
	 Version: 1.2

*/

#if !defined(__CMIO_IOVA_Assistance_h__)
#define __CMIO_IOVA_Assistance_h__

// CA Public Utility Includes
#include "CMIODebugMacros.h"
#include "CAException.h"

// System Includes
#include <IOKit/video/IOVideoDeviceLib.h>

namespace CMIO { namespace IOVA
{
    IOCFPlugInInterface**	AllocatePlugIn(io_service_t service);
    IOVideoDeviceRef		AllocateDevice(IOCFPlugInInterface** plugIn);

    //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    // PlugIn
    //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    class PlugIn
    {
	// Construction/Destruction
	public:
		explicit				PlugIn(IOCFPlugInInterface** plugIn) : mPlugin(0) { Reset(plugIn); }
								~PlugIn() { Reset(0); }
		void					Reset(IOCFPlugInInterface** plugIn = 0) { if (0 != mPlugin) IODestroyPlugInInterface(mPlugin); mPlugin = plugIn; }
        
	private:
		PlugIn&					operator=(PlugIn& that); // Don't allow copying
		IOCFPlugInInterface**	mPlugin;
		
	// Value Access
	public:
		operator				IOCFPlugInInterface**()	const { return mPlugin; }
		IOCFPlugInInterface**	Get() const { return mPlugin; }													
    };
    
    //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Device
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class Device
	{
	// Construction/Destruction
	public:
        
								Device(IOVideoDeviceRef device) : mDevice(NULL), mOpened(false) { Reset(device); }
								~Device() { Reset(); }
		void					Reset(IOVideoDeviceRef device = NULL)
								{
									if (NULL != mDevice)
									{
										Close();
										(**mDevice).Release(mDevice);
										mDevice = NULL;
									} 
									
									if (NULL != device)
									{
										mDevice = device;
									}
								}

	private:
		Device&				operator=(Device& that); // Don't allow copying

		IOVideoDeviceRef	mDevice;
		bool				mOpened;
		
	// Operations
	public:
		void				Open() { IOReturn err = (**mDevice).Open(mDevice, 0); ThrowIfKernelError(err, CAException(err), "CMIO::IOVA::Device::Open() failed"); mOpened = true; }
		void				Close() { if (not mOpened) return; IOReturn err = (**mDevice).Close(mDevice); ThrowIfKernelError(err, CAException(err), "CMIO::IOVA::Device::Close() failed"); mOpened = false; }
        void				AddToRunLoop(CFRunLoopRef runLoop) { IOReturn err = (**mDevice).AddToRunLoop(mDevice, runLoop); ThrowIfKernelError(err, CAException(err), "CMIO::IOVA::Device::AddToRunLoop() failed"); }
        void				RemoveFromRunLoop(CFRunLoopRef runLoop) { IOReturn err = (**mDevice).RemoveFromRunLoop(mDevice, runLoop); ThrowIfKernelError(err, CAException(err), "CMIO::IOVA::Device::RemoveFromRunLoop() failed"); }
        void				SetNotificationCallback(IOVideoDeviceNotificationCallback deviceNotification, void* context) { IOReturn err = (**mDevice).SetNotificationCallback(mDevice, deviceNotification, context); ThrowIfKernelError(err, CAException(err), "CMIO::IOVA::Device::SetNotificationCallback() failed"); }
		void				SetControl(UInt32 controlID, UInt32 value, UInt32* newValue) { IOReturn err = (**mDevice).SetControlValue(mDevice, controlID, value, newValue); ThrowIfKernelError(err, CAException(err), "CMIO::IOVA::Device::SetControl() failed"); }
		void				SetStreamFormat(UInt32 streamID, IOVideoStreamDescription *newStreamFormat) { IOReturn err = (**mDevice).SetStreamFormat(mDevice, streamID, newStreamFormat); ThrowIfKernelError(err, CAException(err), "CMIO::IOVA::Device::SetFormat() failed"); }
	// Value Access
	public:
		operator			IOVideoDeviceRef() const { return mDevice; }
		IOVideoDeviceRef	Get() const { return mDevice; }													
		bool				Opened() const { return mOpened; }													
	};
}}

#endif
