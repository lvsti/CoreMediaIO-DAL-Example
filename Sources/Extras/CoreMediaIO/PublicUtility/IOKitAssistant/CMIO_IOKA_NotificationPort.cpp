/*
	    File: CMIO_IOKA_NotificationPort.cpp
	Abstract: The IOKit Assisistant (IOKA) consists of various objects to facilitate using the "Acquisition is Initialization" design pattern.
				NOTE: the IOKA is sparsely implemented, meaning that it does not attempt to provide wrappers/access to all IOKit features, but just those needed in the CMIO namespace.
	
	 Version: 1.2

*/

#include "CMIO_IOKA_NotificationPort.h"

namespace CMIO { namespace IOKA
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// AllocateNotificationPort()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	IONotificationPortRef AllocateNotificationPort()
	{
		// Retrieve the IOKit's master port so a notification port can be created
		mach_port_t masterPort;
		IOReturn ioReturn = IOMasterPort(MACH_PORT_NULL, &masterPort);
		ThrowIfError(ioReturn, CAException(ioReturn), "CMIO::IOKA::AllocateNotificationPort: IOMasterPort() failed");

		// Create the notification port
		IONotificationPortRef notificationPort = IONotificationPortCreate(masterPort);
		ThrowIfNULL(notificationPort, CAException(-1), "CMIO::IOKA::AllocateNotificationPort: IONotificationPortCreate() failed");

		return notificationPort;
	}

	//--------------------------------------------------------------------------------------------------------------------
	// IsClamshellClosed( &bool isClosed)
	//
	// On Intel-based Powerbooks with a Built-in iSight, the "AppleClamshellClosed" state indicates whether the lid is closed or not.
	// We need to look and see if this property exists to know the current state of the lid.  The property is published in the IOPMRootDomain.
	//--------------------------------------------------------------------------------------------------------------------
	void IsClamshellClosed( bool *isClosed)
	{
		io_registry_entry_t		rootDomain;
		mach_port_t				masterPort;
		CFTypeRef				clamShellStateRef = NULL;
		
		// Retrieve the IOKit's master port so a notification port can be created
		IOReturn ioReturn = IOMasterPort(MACH_PORT_NULL, &masterPort);
		ThrowIfError(ioReturn, CAException(ioReturn), "CMIO::IOKA::IsClamshellClosed: IOMasterPort() failed");
		
		// Check to see if the "AppleClamshellClosed" property is in the PM root domain:
		rootDomain = IORegistryEntryFromPath(masterPort, kIOPowerPlane ":/IOPowerConnection/IOPMrootDomain");
		ThrowIf(rootDomain == MACH_PORT_NULL, CAException(-1), "CMIO::IOKA::IsClamshellClosed: IORegistryEntryFromPath returned NULL root domain");
		
		clamShellStateRef = IORegistryEntryCreateCFProperty( rootDomain,CFSTR(kAppleClamshellStateKey), kCFAllocatorDefault, 0);
		if (clamShellStateRef == NULL)
		{
			DebugMessageLevel(5, "CMIO::IOKA::IsClamshellClosed  Could not find AppleClamshellState in IOPMRootDomain");
			*isClosed = false;
			if ( rootDomain )
				IOObjectRelease(rootDomain);
			ioReturn = kIOReturnNoResources;
			ThrowIfKernelError(ioReturn, CAException(ioReturn), "CMIO::IOKA::IsClamshellClosed: Could not find AppleClamshellState in IOPMRootDomain");
		}
		
		if ( CFBooleanGetValue( (CFBooleanRef)(clamShellStateRef) ) == true )
		{
			DebugMessageLevel(6,"CMIO::IOKA::IsClamshellClosed is TRUE");
			*isClosed = true;
		}
		else if ( CFBooleanGetValue( (CFBooleanRef)(clamShellStateRef) ) == false )
		{
			DebugMessageLevel(6,"CMIO::IOKA::IsClamshellClosed is FALSE");
			*isClosed = false;
		}
		
		if ( rootDomain )
			IOObjectRelease(rootDomain);
		
		if (clamShellStateRef)
			CFRelease(clamShellStateRef);
		
		ThrowIfError(ioReturn, CAException(ioReturn), "CMIO::IOKA::IsClamshellClosed: IOMasterPort() failed");
	}
	
}}
