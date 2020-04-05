/*
	    File: CMIO_DPA_Sample_Server_Frame.h
	Abstract: n/a
	 Version: 1.2

*/

#if !defined(__CMIO_DPA_Sample_Server_Frame_h__)
#define __CMIO_DPA_Sample_Server_Frame_h__

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Internal Includes
#include "CMIO_DPA_Sample_Shared.h"
#include "CMIO_DPA_Sample_Server_Common.h"

// Public Utility Includes
#include "CMIO_CMA_SampleBuffer.h"

// CA Public Utility Includes
#include "CAMutex.h"

// System Includes
#include <CoreMediaIO/CMIOSampleBuffer.h>

namespace CMIO { namespace DPA { namespace Sample { namespace Server
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Frame
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class Frame
	{		
	public:
	// Construction/Destruction
										Frame(FrameType frameType, UInt64 hostTime, const CMA::SampleBuffer::TimingInfo& timingInfo, UInt32 discontinuityFlags, UInt32 droppedFrameCount, UInt64 firstFrameTime, size_t size, void* data);
		virtual							~Frame();										
		
	private:
		Frame&							operator=(Frame& that);							// Unimplemented - don't allow copying

	// Attributes
	public:
		FrameType						GetFrameType() const { return mFrameType; }

	protected:
		FrameType						mFrameType;

	// Time Stamps
	public:
		void							SetHostTime(UInt64 hostTime) { mHostTime = hostTime; }
		UInt64							GetHostTime() const { return mHostTime; }
		void							SetTimingInfo(const CMA::SampleBuffer::TimingInfo& timingInfo)  { mTimingInfo = timingInfo; }
		CMA::SampleBuffer::TimingInfo	GetTimingInfo() const { return mTimingInfo; }
	
	protected:
		UInt64							mHostTime;				// Host time for frame
		CMA::SampleBuffer::TimingInfo	mTimingInfo;			// Samplebuffer timing info for frame
	
	// Discontinuity Flags
	public:
		void							SetDiscontinuityFlags(UInt32 discontinuityFlags) { mDiscontinuityFlags = discontinuityFlags; }
		UInt32							GetDiscontinuityFlags() const { return mDiscontinuityFlags; }
	
	protected:
		UInt32							mDiscontinuityFlags;	// Discontinuity flags associated with the frame
	
	// Dropped frame count
	public:
		void							SetDroppedFrameCount(UInt32 droppedFrameCount) { mDroppedFrameCount = droppedFrameCount; }
		UInt32							GetDroppedFrameCount() const { return mDroppedFrameCount; }
	
	protected:
		UInt32							mDroppedFrameCount;		// Dropped frame count associated with the frame
	
	// First frame time
	public:
		void							SetFirstFrameTime(UInt32 firstFrameTime) { mFirstFrameTime = firstFrameTime; }
		UInt64							GetFirstFrameTime() const { return mFirstFrameTime; }
	
	protected:
		UInt64							mFirstFrameTime;		// Hosttime associated with first frame from device

	// Accessors
	public:
		size_t							Size() { return mSize; }
		void*							Get() { return mFrameData; }

	protected:
		size_t							mSize;
		void*							mFrameData;

	// Clients
	public:
		void							AddClient(Client client);
		void							RemoveClient(Client client);

	protected:
		Clients							mClients;				// Clients to send the message frame to
		CAMutex							mClientsMutex;			// Protect state of the set of clients
	};
}}}}

#endif
