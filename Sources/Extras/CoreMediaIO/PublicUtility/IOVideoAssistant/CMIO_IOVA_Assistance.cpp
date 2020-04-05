/*
	    File: CMIO_IOVA_Assistance.cpp
	Abstract: The IOVideo Assisistant (IOVA) consists of various objects to facilitate using the "Acquisition is Initialization" design pattern.
				NOTE: much of the IOVA is sparsely implemented, meaning that it does not attempt to provide wrappers/access  to all IOVideo features, but just those needed by the IOVideo
				plugin.
	
	 Version: 1.2

*/

// Self Include
#include "CMIO_IOVA_Assistance.h"

namespace CMIO { namespace IOVA
{
    //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    // AllocatePlugIn()
    //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    IOCFPlugInInterface** AllocatePlugIn(io_service_t service)
	{
        IOCFPlugInInterface** plugIn;
        SInt32 score;
        
        IOReturn ioReturn = IOCreatePlugInInterfaceForService(service, kIOVideoDeviceLibTypeID, kIOCFPlugInInterfaceID, &plugIn, &score);
		ThrowIfKernelError(ioReturn, CAException(ioReturn), "CMIO::IOVA::AllocatePlugIn() failed");
        
        return plugIn;
	}

	//--------------------------------------------------------------------------------------------------------------------
	// AllocateDevice()
	//--------------------------------------------------------------------------------------------------------------------
	IOVideoDeviceRef AllocateDevice(IOCFPlugInInterface** plugIn)
	{
		IOVideoDeviceRef device;
		HRESULT hResult = (**plugIn).QueryInterface(plugIn, CFUUIDGetUUIDBytes(kIOVideoDeviceInterfaceID), reinterpret_cast<void**>(&device));
		ThrowIfError(hResult, CAException(hResult), "CMIO::IOVideoDevice::AllocateDevice() failed");
		
		return device;
	}

}}

