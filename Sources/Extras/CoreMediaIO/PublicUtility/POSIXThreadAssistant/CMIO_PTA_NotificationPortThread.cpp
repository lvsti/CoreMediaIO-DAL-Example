/*
	    File: CMIO_PTA_NotificationPortThread.cpp
	Abstract: A thread for getting messages on a IOKit Notifcation port (using the CMIO::IOKA::NotificationPort wrappers)
	 Version: 1.2

*/

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Public Utility Includes
#include "CMIODebugMacros.h"
#include "CMIO_PTA_NotificationPortThread.h"

// System Includes
#include <objc/objc-auto.h>

namespace CMIO { namespace PTA
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Reset()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void NotificationPortThread::Reset(bool spawn)
	{
		// Wait for the thread to be running or invalid prior to tearing it down.  
		while (kStarting == mState)
			pthread_yield_np();

		if (kRunning == mState)
		{
			// Wait for the thread's run loop to be "waiting."  This will avoid a small window of having set 'mState = kRunning' prior but not having invoked CFRunLoopRun() yet.
			while (not CFRunLoopIsWaiting(mRunLoop.GetCFObject()))
				pthread_yield_np();
				
			// Stop the thread's run loop
			CFRunLoopStop(mRunLoop.GetCFObject());
			
			// Set the thread's state to kStopping
			mState = kStopping;
			
			// Wait for the thread to run to completion (it will set the state to kInvalid) 
			while (kStopping == mState)
			{
				(void) pthread_cond_signal(mStoppingCondition);
				pthread_yield_np();
			}
		}
		
		mNotificationPort.Reset();
		mPThread	= 0;
		mRunLoop	= 0;
		mState		= kInvalid;

		if (spawn)
		{
			typedef void* (*PThreadStart)(void*);
			
			mState	= kStarting;
			int err = pthread_create(&mPThread, 0, reinterpret_cast<PThreadStart>(Start), this);
			ThrowIfError(err, CAException(err), "CMIO::PTA::NotificationPortThread::Reset: pthread_create() failed");
				
			(void) pthread_detach(mPThread);
		}
	}

	#pragma mark -
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Start()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void* NotificationPortThread::Start(NotificationPortThread* thread)
	{
		thread->mRunLoop = CFRunLoopGetCurrent();
		
		try
		{
			// Setup the IOKA::NotificationPort used by this thread
			thread->mNotificationPort.Reset(IOKA::AllocateNotificationPort(), CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
			
			// Indicate that the run loop will need to be stopped during the next call to Reset()
			thread->mState = kRunning;
	
			CFRunLoopRun();

			// In the unlikey event that CFRunLoopRun() returned prior to CFRunLoopStop() being called in Reset(), wait until the stopping condition has been explicitly signaled prior to
			// letting the thread run to completion.  This will make sure that the run loop is still valid when CFRunLoopStop() is called.
			{
				PTA::Mutex::Locker locker(thread->mStoppingMutex);
				(void) pthread_cond_wait(thread->mStoppingCondition, thread->mStoppingMutex);
				thread->mState = kInvalid;
			}
		}
		catch (...)
		{
			thread->mState = kInvalid;
			return reinterpret_cast<void*>(-1);
		}

		return 0;
	}
}}
