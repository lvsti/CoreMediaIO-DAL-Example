/*
	    File: CMIO_PTA_NotificationPortThread.h
	Abstract: A thread for getting messages on a IOKit Notifcation port (using the CMIO::IOKA::NotificationPort wrappers)
	 Version: 1.2

*/

#if !defined(__CMIO_PTA_NotificationPortThread_h__)
#define __CMIO_PTA_NotificationPortThread_h__

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Public Utility Includes
#include "CMIO_IOKA_NotificationPort.h"
#include "CMIO_PTA_Assistance.h"

// CA Public Utility Includes
#include "CACFObject.h"

namespace CMIO { namespace PTA
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// NotificationPortThread
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class NotificationPortThread
	{
	// Thread State
	public:
		enum State { kInvalid, kStarting, kRunning, kStopping};

	// Construction/Destruction
	public:
								NotificationPortThread(bool spawn = false) : mNotificationPort(NULL), mPThread(NULL), mRunLoop(NULL), mState(kInvalid), mStoppingMutex(PTHREAD_MUTEX_RECURSIVE), mStoppingCondition() { Reset(spawn); }
								~NotificationPortThread() { Reset(); }
		void					Reset(bool spawn = false);
		
	private:
		IOKA::NotificationPort	mNotificationPort;
		pthread_t				mPThread;	
		CACFObject<CFRunLoopRef> mRunLoop;
		volatile State			mState;
		Mutex					mStoppingMutex;								// Used for waiting on mStoppingCondition
		Mutex::Condition		mStoppingCondition;							// Allows for explict control of thread completion

		NotificationPortThread& operator=(NotificationPortThread& rhs);		// Don't allow copying

	// Value Access
	public:
		operator				pthread_t() const { return mPThread; }
		operator				CFRunLoopRef() const { return mRunLoop.GetCFObject(); }
		pthread_t				GetPThread() const { return mPThread; }
		CFRunLoopRef			GetRunLoop() const { return mRunLoop.GetCFObject(); }
		State					GetState() const { return mState; }
		IONotificationPortRef   GetNotificationPort() const { return mNotificationPort; }

	// Implemenation
	private:
		static void*			Start(NotificationPortThread* thread);
	};
}}

#endif

