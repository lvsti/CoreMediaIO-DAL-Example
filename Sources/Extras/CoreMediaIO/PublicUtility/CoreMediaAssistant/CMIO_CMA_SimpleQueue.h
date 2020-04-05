/*
	    File: CMIO_CMA_SimpleQueue.h
	Abstract: C++ wrapper for CMSimpleQueueRef.
				
				------ WARNING -------
				As written, T MUST be a pointer type.
	
	 Version: 1.2

*/

#if !defined(__CMIO_CMA_SimpleQueue_h__)
#define __CMIO_CMA_SimpleQueue_h__

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Public Utility Includes
#include "CMIODebugMacros.h"

// CA Public Utility Includes
#include "CAException.h"

// System Includes
#include <CoreMedia/CMSimpleQueue.h>

namespace CMIO { namespace CMA
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// SimpleQueue
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	template <typename T> class SimpleQueue
	{
	// Construction/Destruction
	public:
									SimpleQueue(CMSimpleQueueRef ref = NULL, bool retain = false, bool release = true) : mRef(NULL) { Reset(ref, retain, release); }
									~SimpleQueue() { Reset(); }
		void						Reset(CMSimpleQueueRef ref = NULL, bool retain = false, bool release = true) { if (NULL != mRef and mRelease) CFRelease(mRef); if (retain and NULL != ref) CFRetain(ref); mRef = ref; mRelease = release; }
		SimpleQueue&				operator=(const SimpleQueue& that) { if (this != &that) this->Reset(that.Get(), true, that.WillRelease()); return *this; }

	// Excecption throwing versions of the CMSimpleQueueCreateXXXXX routines for the underlying CMSimpleQueueRefs
	public:
		static CMSimpleQueueRef	Create(CFAllocatorRef allocator, UInt32 size) { CMSimpleQueueRef queueRef; OSStatus err = CMSimpleQueueCreate(allocator, size, &queueRef); ThrowIfError(err, CAException(err), "CMIO::SimpleQueue::Create: CMSimpleQueueCreate() failed"); return queueRef; }

	private:
		CMSimpleQueueRef			mRef;
		bool						mRelease;

	// Attributes
	public:
		bool						IsValid() const { return mRef != NULL; }
		bool						WillRelease() const { return mRelease; }
		void						ShouldRelease(bool release) { mRelease = release; }
		
	// Queue Operations
	public:
		void						Enqueue(const T value) { OSStatus err = CMSimpleQueueEnqueue(mRef, value); ThrowIfError(err, CAException(err), "CMIO::SimpleQueue::Enqueue: CMSimpleQueueEnqueue() failed"); }
		T							Dequeue() { T value = static_cast<T>(const_cast<void*>(CMSimpleQueueDequeue(mRef))); ThrowIfNULL(value, CAException(kCMSimpleQueueError_ParameterOutOfRange), "CMIO::SimpleQueue::Dequeue: CMSimpleQueueDequeue() returned NULL"); return value;}
		T							GetHead() { T value = static_cast<T>(const_cast<void*>(CMSimpleQueueGetHead(mRef))); ThrowIfNULL(value, CAException(kCMSimpleQueueError_ParameterOutOfRange), "CMIO::SimpleQueue::GetHead: CMSimpleQueueGetHead() returned NULL"); return value;}
		UInt32						Capacity() const { return CMSimpleQueueGetCapacity(mRef); }
		UInt32						GetCount() const { return CMSimpleQueueGetCount(mRef); }
		Float32						Fullness() const { return CMSimpleQueueGetFullness(mRef); }
		void						ResetToEmpty() { OSStatus err = CMSimpleQueueReset(mRef); ThrowIfError(err, CAException(err), "CMIO::SimpleQueue::ResetToEmpty: CMSimpleQueueGetCount() failed"); }

	// Value Access
	public:
		operator					CMSimpleQueueRef() const { return mRef; }
		CMSimpleQueueRef			Get() const { return mRef; }
	};
}}
#endif
