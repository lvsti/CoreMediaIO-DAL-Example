/*
	    File: CMIO_DPA_Sample_Server_IOBackedFrame.cpp
	Abstract: n/a
	 Version: 1.2
	
*/

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Self Include
#include "CMIO_DPA_Sample_Server_IOBackedFrame.h"


namespace CMIO { namespace DPA { namespace Sample { namespace Server
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// IOBackedFrame
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	IOBackedFrame::IOBackedFrame(IOStreamRef stream,
                                 FrameType frameType,
                                 UInt64 hostTime,
                                 const CMA::SampleBuffer::TimingInfo& timingInfo,
                                 UInt32 discontinuityFlags,
                                 UInt32 droppedFrameCount,
                                 UInt64 firstFrameTime,
                                 IOStreamBufferID bufferID,
                                 size_t size,
                                 void* data) :
        Frame(frameType, hostTime, timingInfo, discontinuityFlags, droppedFrameCount, firstFrameTime, size, data),
        mStream(stream),
        mBufferID(bufferID)
	{
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// ~IOBackedFrame()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	IOBackedFrame::~IOBackedFrame()
	{
		// Return the frame to the queue
		(**mStream).EnqueueInputBuffer(mStream, mBufferID, 0, 0, 0, 0);
		(**mStream).SendInputNotification(mStream, 0xAA);
	}
	
}}}}
