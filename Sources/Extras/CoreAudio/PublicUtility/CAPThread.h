/*
     File: CAPThread.h
 Abstract: Part of CoreAudio Utility Classes
  Version: 1.1

*/
#if !defined(__CAPThread_h__)
#define __CAPThread_h__

//==================================================================================================
//	Includes
//==================================================================================================

//	System Includes
#if !defined(__COREAUDIO_USE_FLAT_INCLUDES__)
	#include <CoreFoundation/CFBase.h>
#else
	#include <CFBase.h>
#endif

#if TARGET_OS_MAC
	#include <pthread.h>
	#include <unistd.h>
#elif TARGET_OS_WIN32
	#include <windows.h>
#else
	#error	Unsupported operating system
#endif

//==================================================================================================
//	CAPThread
//
//	This class wraps a pthread and a Win32 thread.
//	caution: long-running fixed priority threads can make the system unresponsive
//==================================================================================================

class	CAPThread
{

//	Types
public:
	typedef void*			(*ThreadRoutine)(void* inParameter);

//	Constants
public:
	enum
	{
#if	TARGET_OS_MAC
							kMinThreadPriority = 1,
							kMaxThreadPriority = 63,
							kDefaultThreadPriority = 31,
							kMaxThreadNameLength = 64
#elif TARGET_OS_WIN32
							kMinThreadPriority = 1,
							kMaxThreadPriority = 31,
							kDefaultThreadPriority = THREAD_PRIORITY_NORMAL,
							kMaxThreadNameLength = 256
#endif
	};

//	Construction/Destruction
public:
							CAPThread(ThreadRoutine inThreadRoutine, void* inParameter, UInt32 inPriority = kDefaultThreadPriority, bool inFixedPriority=false, bool inAutoDelete=false, const char* inThreadName = NULL);
							CAPThread(ThreadRoutine inThreadRoutine, void* inParameter, UInt32 inPeriod, UInt32 inComputation, UInt32 inConstraint, bool inIsPreemptible, bool inAutoDelete=false, const char* inThreadName = NULL);
	virtual					~CAPThread();

//	Properties
public:
#if TARGET_OS_MAC
	typedef pthread_t		NativeThread;

	NativeThread			GetNativeThread() { return mPThread; }
	static NativeThread		GetCurrentThread() { return pthread_self(); }
	static bool				IsNativeThreadsEqual(NativeThread a, NativeThread b) { return (a==b); }

	bool					operator==(NativeThread b) { return pthread_equal(mPThread,b); }

	pthread_t				GetPThread() const { return mPThread; }
	bool					IsCurrentThread() const { return (0 != mPThread) && (pthread_self() == mPThread); }
	bool					IsRunning() const { return 0 != mPThread; }
    static UInt32			getScheduledPriority(pthread_t inThread, int inPriorityKind);
#elif TARGET_OS_WIN32
	typedef unsigned long	NativeThread;
	
	NativeThread			GetNativeThread() { return mThreadID; }
	static NativeThread		GetCurrentThread() { return GetCurrentThreadId(); }
	static bool				IsNativeThreadsEqual(NativeThread a, NativeThread b) { return (a==b); }

	bool					operator ==(NativeThread b) { return (mThreadID==b); }

	HANDLE					GetThreadHandle() const { return mThreadHandle; }
	UInt32					GetThreadID() const { return mThreadID; }
	bool					IsCurrentThread() const { return (0 != mThreadID) && (GetCurrentThreadId() == mThreadID); }
	bool					IsRunning() const { return 0 != mThreadID; }
#endif

	bool					IsTimeShareThread() const { return !mTimeConstraintSet; }
	bool					IsTimeConstraintThread() const { return mTimeConstraintSet; }

	UInt32					GetPriority() const { return mPriority; }
    UInt32					GetScheduledPriority();
	static UInt32			GetScheduledPriority(NativeThread thread);
    void					SetPriority(UInt32 inPriority, bool inFixedPriority=false);
	static void				SetPriority(NativeThread inThread, UInt32 inPriority, bool inFixedPriority = false);

	void					GetTimeConstraints(UInt32& outPeriod, UInt32& outComputation, UInt32& outConstraint, bool& outIsPreemptible) const { outPeriod = mPeriod; outComputation = mComputation; outConstraint = mConstraint; outIsPreemptible = mIsPreemptible; }
	void					SetTimeConstraints(UInt32 inPeriod, UInt32 inComputation, UInt32 inConstraint, bool inIsPreemptible);
	void					ClearTimeConstraints() { SetPriority(mPriority); }
	
	bool					WillAutoDelete() const { return mAutoDelete; }
	void					SetAutoDelete(bool b) { mAutoDelete = b; }
	
	void					SetName(const char* inThreadName);

#if CoreAudio_Debug	
	void					DebugPriority(const char *label);
#endif

//	Actions
public:
	virtual void			Start();

//	Implementation
protected:
#if TARGET_OS_MAC
	static void*			Entry(CAPThread* inCAPThread);
#elif TARGET_OS_WIN32
	static UInt32 WINAPI	Entry(CAPThread* inCAPThread);
#endif

#if	TARGET_OS_MAC
	pthread_t				mPThread;
    UInt32					mSpawningThreadPriority;
#elif TARGET_OS_WIN32
	HANDLE					mThreadHandle;
	unsigned long			mThreadID;
#endif
	ThreadRoutine			mThreadRoutine;
	void*					mThreadParameter;
	char					mThreadName[kMaxThreadNameLength];
	UInt32					mPriority;
	UInt32					mPeriod;
	UInt32					mComputation;
	UInt32					mConstraint;
	bool					mIsPreemptible;
	bool					mTimeConstraintSet;
    bool					mFixedPriority;
	bool					mAutoDelete;		// delete self when thread terminates
};

#endif
