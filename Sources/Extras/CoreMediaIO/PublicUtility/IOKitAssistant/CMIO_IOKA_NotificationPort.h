/*
	    File: CMIO_IOKA_NotificationPort.h
	Abstract: The IOKit Assisistant (IOKA) consists of various objects to facilitate using the "Acquisition is Initialization" design pattern.
				NOTE: the IOKA is sparsely implemented, meaning that it does not attempt to provide wrappers/access to all IOKit features, but just those needed in the CMIO namespace.
	
	 Version: 1.2

*/

#if !defined(__CMIO_IOKA_NotificationPort_h__)
#define __CMIO_IOKA_NotificationPort_h__

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#include <IOKit/pwr_mgt/IOPM.h>

// Public Utility Includes
#include "CMIODebugMacros.h"
#include "CMIO_IOKA_Object.h"

namespace CMIO { namespace IOKA
{
	IONotificationPortRef AllocateNotificationPort();
	void IsClamshellClosed( bool *isClosed);
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// NotificationPort
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class NotificationPort
	{
	// Construction/Destruction
	public:
								NotificationPort(IONotificationPortRef notificationPort = NULL, CFRunLoopRef runLoop = NULL, CFStringRef runLoopMode = NULL) : mNotificationPort(NULL), mRunLoop(NULL), mRunLoopMode(NULL) { Reset(notificationPort, runLoop, runLoopMode);}
								~NotificationPort() { Reset(); }
		
		void					Reset(IONotificationPortRef notificationPort = NULL, CFRunLoopRef runLoop = NULL, CFStringRef runLoopMode = NULL)
								{
									if (NULL != mNotificationPort)
									{
										// Remove the notification port's run loop source and then destroy the port
										CFRunLoopRemoveSource(mRunLoop, IONotificationPortGetRunLoopSource(mNotificationPort), mRunLoopMode);
										IONotificationPortDestroy(mNotificationPort);
										
										mNotificationPort = NULL;
										mRunLoop = NULL;
										mRunLoopMode = NULL;
									}
									
									if (NULL != notificationPort)
									{
										mNotificationPort = notificationPort;

										if (NULL != runLoop and NULL != runLoopMode)
										{
											// Add the notification port's run loop source to the specified run loop
											CFRunLoopAddSource(runLoop, IONotificationPortGetRunLoopSource(mNotificationPort), runLoopMode);
											mRunLoop = runLoop;
											mRunLoopMode = runLoopMode;
										}
									}
								}

	private:
		NotificationPort&		operator=(NotificationPort& that);		// Don't allow copying
		IONotificationPortRef	mNotificationPort;						// Port on which notifications are received		
		CFRunLoopRef			mRunLoop;								// The run loop to which the the port's run loop source was added		
		CFStringRef				mRunLoopMode;							// The mode which the port's port's run loop source was operating

	// Operations
	public:
		void					AddInterestNotification(io_service_t service, const io_name_t interestType, IOServiceInterestCallback callback, void* refCon, IOKA::Object& notifier)
								{
									IOReturn ioReturn = IOServiceAddInterestNotification(mNotificationPort, service, interestType, callback, refCon, notifier.GetAddress());
									ThrowIfError(ioReturn, CAException(ioReturn), "CMIO::IOKA::NotificationPort::AddInterestNotification: IOServiceAddInterestNotification() failed");
								}

	// Value Access
	public:
		operator				IONotificationPortRef()	const { return mNotificationPort; }
		IONotificationPortRef	Get() const { return mNotificationPort; }													
	};
}}

#endif

