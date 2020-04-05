/*
	    File: CMIO_IOKA_PowerNotificationPort.h
	Abstract: The IOKit Assisistant (IOKA) consists of various objects to facilitate using the "Acquisition is Initialization" design pattern.
				NOTE: the IOKA is sparsely implemented, meaning that it does not attempt to provide wrappers/access to all IOKit features, but just those needed in the CMIO namespace.
	
	 Version: 1.2

*/

#if !defined(__CMIO_IOKA_PowerNotificationPort_h__)
#define __CMIO_IOKA_PowerNotificationPort_h__

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Public Utility Includes
#include "CMIODebugMacros.h"
#include "CMIO_IOKA_Object.h"

// CA Public Utility Includes
#include "CAException.h"

// System Includes
#include <IOKit/pwr_mgt/IOPMLib.h>

namespace CMIO { namespace IOKA
{
	//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// PowerNotificationPort
	//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class PowerNotificationPort
	{
	// Construction/Destruction
	public:
								PowerNotificationPort(CFRunLoopRef runLoop = 0, CFStringRef runLoopMode = 0, IOServiceInterestCallback callback = 0, void* refCon = 0) : mPowerNotificationPort(0), mRunLoop(NULL), mRunLoopMode(NULL), mRootPowerDomain(), mNotifier(0) { Reset(runLoop, runLoopMode, callback, refCon); }
								~PowerNotificationPort() { Reset(); }
		
		void					Reset(CFRunLoopRef runLoop = 0, CFStringRef runLoopMode = 0, IOServiceInterestCallback callback = 0, void* refCon = 0)
								{
									if (0 != mPowerNotificationPort)
									{
										// Remove the notification port's run loop source
										CFRunLoopRemoveSource(mRunLoop, IONotificationPortGetRunLoopSource(mPowerNotificationPort), mRunLoopMode);
										
										// Deregister from power notifications.  No need to call IOObjectRelease() on mNotifier since that is done by IODeregisterForSystemPower().
										IODeregisterForSystemPower(&mNotifier);
										
										// Destroy the port on which power notifications had been recieved
										IONotificationPortDestroy(mPowerNotificationPort);
										mPowerNotificationPort = 0;
										mRunLoop = NULL;
										mRunLoopMode = NULL;

										// Release the root power domain connection
										mRootPowerDomain.Reset();
									}
									
									if (0 != runLoop and 0 != runLoopMode and 0 != callback)
									{
										// Try and register for power notifications
										mRootPowerDomain.Reset(IORegisterForSystemPower(refCon, &mPowerNotificationPort, callback, &mNotifier));
										ThrowIf(0 == mRootPowerDomain.Get(), CAException(kIOReturnError), "CMIO::IOKA::PowerNotificationPort::Reset: IORegisterForSystemPower() failed"); 
											
										// Add the notification port's run loop source to the specified run loop
										CFRunLoopAddSource(runLoop, IONotificationPortGetRunLoopSource(mPowerNotificationPort), runLoopMode);
										mRunLoop = runLoop;
										mRunLoopMode = runLoopMode;
									}
								}

	private:
		PowerNotificationPort&	operator=(PowerNotificationPort& that);	// Don't allow copying
		IONotificationPortRef	mPowerNotificationPort;					// Port on which power notifications are received		
		CFRunLoopRef			mRunLoop;								// The run loop to which the the port's run loop source was added		
		CFStringRef				mRunLoopMode;							// The mode which the port's port's run loop source was operating
		Object					mRootPowerDomain;						// Representation of the Root Power Domain IOService 
		io_object_t				mNotifier;								// 'cookie' from IORegisterForSystemPower() to be passed to IODeregisterForSystemPower()

	// Value Access
	public:
		operator				IONotificationPortRef() const { return mPowerNotificationPort; }
		IONotificationPortRef	Get() const	{ return mPowerNotificationPort; }													
		IONotificationPortRef*	GetAddress() { return &mPowerNotificationPort; }													
		io_object_t				GetRootPowerDomain() const	{ return mRootPowerDomain; }													
	};
}}

#endif
