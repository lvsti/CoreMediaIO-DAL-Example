/*
	    File: CMIO_PTA_CFMachPortThread.h
	Abstract: A thread for getting messages on a Mach port (using the CACFMachPort wrappers)
	 Version: 1.2

*/

#if !defined(__CMIO_PTA_CFMachPortThread_h__)
#define __CMIO_PTA_CFMachPortThread_h__

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Public Utility Includes
#include "CMIO_PTA_Assistance.h"

// CA Public Utility Includes
#include "CACFObject.h"

// System Includes
#include <CoreFoundation/CFMachPort.h>

namespace CMIO { namespace PTA
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// CFMachPortThread
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class CFMachPortThread
	{
	// Thread State
	public:
		enum State { kInvalid, kStarting, kRunning, kStopping};

	// Construction/Destruction
	public:
							CFMachPortThread(CFMachPortCallBack portCallBack = 0, void* userData = 0) : mPort(MACH_PORT_NULL), mPortCallBack(0), mUserData(0), mPThread(0), mRunLoop(0), mState(kInvalid), mStoppingMutex(PTHREAD_MUTEX_RECURSIVE), mStoppingCondition() { Reset(portCallBack, userData); }
							~CFMachPortThread() { Reset(); }
		void				Reset(CFMachPortCallBack portCallBack = 0, void* userData = 0);
		
	private:
		mach_port_t			mPort;						
		CFMachPortCallBack	mPortCallBack;
		void*				mUserData;
		pthread_t			mPThread;	
		CACFObject<CFRunLoopRef> mRunLoop;
		volatile State		mState;
		Mutex				mStoppingMutex;						// Used for waiting on mStoppingCondition
		Mutex::Condition	mStoppingCondition;					// Allows for explict control of thread completion

		CFMachPortThread&	operator=(CFMachPortThread& rhs);	// Don't allow copying

	// Value Access
	public:
		operator			pthread_t() const { return mPThread; }
		operator			CFRunLoopRef() const { return mRunLoop.GetCFObject(); }
		pthread_t			GetPThread() const { return mPThread; }
		CFRunLoopRef		GetRunLoop() const { return mRunLoop.GetCFObject(); }
		State				GetState() const { return mState; }
		mach_port_t			GetMachPort() const { return mPort; }

	// Implemenation
	private:
		static void*		Start(CFMachPortThread* thread);
	};
}}

#endif

