/*
	    File: CMIO_DPA_Sample_Server_Frame.cpp
	Abstract: n/a
	 Version: 1.2

*/

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Self Include
#include "CMIO_DPA_Sample_Server_Frame.h"


namespace CMIO { namespace DPA { namespace Sample { namespace Server
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Frame
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	Frame::Frame(FrameType frameType, UInt64 hostTime, const CMA::SampleBuffer::TimingInfo& timingInfo, UInt32 discontinuityFlags, UInt32 droppedFrameCount, UInt64 firstFrameTime, size_t size, void* data) :
		mFrameType(frameType),
		mHostTime(hostTime),
		mTimingInfo(timingInfo),
		mDiscontinuityFlags(discontinuityFlags),
		mDroppedFrameCount(droppedFrameCount),
		mFirstFrameTime(firstFrameTime),
		mSize(size),
		mFrameData(data),
		mClients(),
		mClientsMutex("frame clients mutex")
	{
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Frame
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    Frame::~Frame()
    {
    }

    //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// AddClient()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void Frame::AddClient(Client client)
	{
		// Grab the mutex for the the set of clients
		CAMutex::Locker locker(mClientsMutex);

		// Add the client to the set of that will be messaged with the frame
		DebugMessageLevel(4, "Frame::AddClient: frame (%x) - Adding client - %d with count: %d", this, client, mClients.size() + 1);
		mClients.insert(client);
	}
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// RemoveClient()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void Frame::RemoveClient(Client client)
	{
		{
			// Grab the mutex for the the set of clients
			CAMutex::Locker locker(mClientsMutex);

			// Remove the client from the to-be-messaged set
			mClients.erase(client);
			DebugMessageLevel(4, "Frame::RemoveClient: Native frame (%x) - Removing client - %d with count: %d", this, client, mClients.size());
			
			// If there are still clients, simply return
			if (not mClients.empty())
				return;
		}
			
		// Clear the discontinuity flags
		SetDiscontinuityFlags(kCMIOSampleBufferNoDiscontinuities);

		// This frame is no longer needed so delete it
		delete this;
	}
}}}}
