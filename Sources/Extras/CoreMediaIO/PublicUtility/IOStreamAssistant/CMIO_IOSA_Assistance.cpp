/*
	    File: CMIO_IOSA_Assistance.cpp
	Abstract: The IOStream Assisistant (IOSA) consists of various objects to facilitate using the "Acquisition is Initialization" design pattern.
				NOTE: the IOSA is sparsely implemented, meaning that it does not attempt to provide wrappers/access to all IOStream features, but just those needed by the IOStream plugin.
	
	 Version: 1.2

*/

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//	Self Include
#include "CMIO_IOSA_Assistance.h"

namespace CMIO { namespace IOSA
{
    //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    // AllocatePlugIn()
    //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    IOCFPlugInInterface** AllocatePlugIn(io_service_t service)
	{
        IOCFPlugInInterface** plugIn;
        SInt32 score;
        
        IOReturn ioReturn = IOCreatePlugInInterfaceForService(service, kIOStreamLibTypeID, kIOCFPlugInInterfaceID, &plugIn, &score);
		ThrowIfKernelError(ioReturn, CAException(ioReturn), "CMIO::IOSA::AllocatePlugIn() failed");
        
        return plugIn;
	}

    //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// AllocateStream()
    //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	IOStreamRef AllocateStream(IOCFPlugInInterface** plugIn)
	{
		IOStreamRef stream;
		HRESULT hResult = (**plugIn).QueryInterface(plugIn, CFUUIDGetUUIDBytes(kIOStreamInterfaceID),  reinterpret_cast<void**>(&stream));
		ThrowIfError(hResult, CAException(hResult), "CMIO::IOSA::AllocateStream() failed");
		
		return stream;
	}
}}

