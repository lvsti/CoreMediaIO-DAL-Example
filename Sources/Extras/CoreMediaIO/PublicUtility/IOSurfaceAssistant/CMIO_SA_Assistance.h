/*
	    File: CMIO_SA_Assistance.h
	Abstract: The IOSurface Assisistant (SA) consists of various objects to facilitate using the "Acquisition is Initialization" design pattern.
				NOTE: the SA is sparsely implemented, meaning that it does not attempt to provide wrappers/access to all IOSurface features, just those needed.
	
	 Version: 1.2

*/

#if !defined(__CMIO_SA_Assistance_h__)
#define __CMIO_SA_Assistance_h__

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Public Utility Includes
#include "CMIODebugMacros.h"

// CA Public Utility Includes
#include "CAException.h"

// System Includes
#include <IOSurface/IOSurface.h>

namespace CMIO { namespace SA
{
    //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Surface
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class Surface
	{
	// Construction/Destruction
	public:
							Surface(IOSurfaceRef ref = NULL, bool retain = false, bool release = true) : mSurface(NULL) { Reset(ref, retain, release); }
		virtual				~Surface() { Reset(); }
		void				Reset(IOSurfaceRef ref = NULL, bool retain = false, bool release = true) { if (NULL != mSurface and mRelease) CFRelease(mSurface); if (retain and NULL != ref) CFRetain(ref); mSurface = ref; mRelease = release; }
		Surface&			operator=(const Surface& that) { if (this != &that) this->Reset(that.Get(), true, that.WillRelease());  return *this; }
        
	// Exception throwing versions of the CMBlockBufferCreateXXXXX routines for the underlying CMBlockBufferRef
	public:
		static IOSurfaceRef	Create(CFDictionaryRef properties) { IOSurfaceRef surface = IOSurfaceCreate(properties); ThrowIfNULL(surface, CAException(kIOReturnNoResources), "CMIO::SA::Surface::Create() - failed"); return surface; }
		static IOSurfaceRef	LookupFromMachPort(mach_port_t port) { IOSurfaceRef surface = IOSurfaceLookupFromMachPort(port); ThrowIfNULL(surface, CAException(kIOReturnNoResources), "CMIO::SA::Surface::LookupFromMachPort() - failed"); return surface; }

	private:
		IOSurfaceRef		mSurface;
		bool				mRelease;
		
	// Attributes
	public:
		bool				IsValid() const { return NULL != mSurface; }
		bool				WillRelease() const { return mRelease; }
		void				ShouldRelease(bool release) { mRelease = release; }

   // Operations
	public:
	mach_port_t				CreateMachPort() { mach_port_t port = IOSurfaceCreateMachPort(mSurface); ThrowIf(MACH_PORT_NULL == port, CAException(kIOReturnNoResources), "CMIO::SA::Surface::CreateMachPort() - failed"); return port; }
	
	// Value Access
	public:
		operator			IOSurfaceRef() const { return mSurface; }
		IOSurfaceRef		Get() const { return mSurface; }													
	};
}}

#endif
