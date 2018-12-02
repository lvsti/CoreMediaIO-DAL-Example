/*
	    File: CMIO_DPA_Sample_Server_IOBackedStream.cpp
	Abstract: n/a
	 Version: 1.2
	
	Disclaimer: IMPORTANT:  This Apple software is supplied to you by Apple
	Inc. ("Apple") in consideration of your agreement to the following
	terms, and your use, installation, modification or redistribution of
	this Apple software constitutes acceptance of these terms.  If you do
	not agree with these terms, please do not use, install, modify or
	redistribute this Apple software.
	
	In consideration of your agreement to abide by the following terms, and
	subject to these terms, Apple grants you a personal, non-exclusive
	license, under Apple's copyrights in this original Apple software (the
	"Apple Software"), to use, reproduce, modify and redistribute the Apple
	Software, with or without modifications, in source and/or binary forms;
	provided that if you redistribute the Apple Software in its entirety and
	without modifications, you must retain this notice and the following
	text and disclaimers in all such redistributions of the Apple Software.
	Neither the name, trademarks, service marks or logos of Apple Inc. may
	be used to endorse or promote products derived from the Apple Software
	without specific prior written permission from Apple.  Except as
	expressly stated in this notice, no other rights or licenses, express or
	implied, are granted by Apple herein, including but not limited to any
	patent rights that may be infringed by your derivative works or by other
	works in which the Apple Software may be incorporated.
	
	The Apple Software is provided by Apple on an "AS IS" basis.  APPLE
	MAKES NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION
	THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS
	FOR A PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND
	OPERATION ALONE OR IN COMBINATION WITH YOUR PRODUCTS.
	
	IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL
	OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
	SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
	INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION,
	MODIFICATION AND/OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED
	AND WHETHER UNDER THEORY OF CONTRACT, TORT (INCLUDING NEGLIGENCE),
	STRICT LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN ADVISED OF THE
	POSSIBILITY OF SUCH DAMAGE.
	
	Copyright (C) 2012 Apple Inc. All Rights Reserved.
	
*/

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Self Include
#include "CMIO_DPA_Sample_Server_IOBackedStream.h"

// Internal Includes
#include "CMIO_DPA_Sample_Server_ClientStream.h"
#include "CMIO_DPA_Sample_Server_Device.h"
#include "CMIO_DPA_Sample_Server_IOBackedFrame.h"
#include "CMIO_DPA_Sample_Server_Deck.h"
#include "CMIO_DPA_Sample_Shared.h"

// Public Utility Includes
#include "CMIODebugMacros.h"
#include "CMIO_PTA_NotificationPortThread.h"

// CA Public Utility Includes
#include "CACFNumber.h"
#include "CAHostTimeBase.h"

// System Includes
#include <IOKit/video/IOVideoTypes.h>
#include <CoreMediaIO/CMIOHardware.h>
#include "CMIO_CVA_Pixel_Buffer.h"
#include "CMIO_SA_Assistance.h"
#include <mach/mach.h>

#define kMaxRequestsPerCallback 4

namespace CMIO { namespace DPA { namespace Sample { namespace Server
{
	#pragma mark -
	#pragma mark IOBackedStream
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// IOBackedStream()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	IOBackedStream::IOBackedStream(Device* device, IOKA::Object& registryEntry, CFDictionaryRef streamDictionary, CMIOObjectPropertyScope scope) :
        Stream(device, streamDictionary, scope),
		mRegistryEntry(registryEntry),
        mIOSPAlugIn(IOSA::AllocatePlugIn(mRegistryEntry)),
        mIOSAStream(IOSA::AllocateStream(mIOSPAlugIn))
	{
		// Specify the stream's callback
		mIOSAStream.SetOutputCallback(reinterpret_cast<IOStreamOutputCallback>(StreamOutputCallback), this);
				
		// Add the stream's run loop source to the notification thread's run loop
		mIOSAStream.AddToRunLoop(GetOwningDevice().GetNotificationThread().GetRunLoop());
	}
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// ~IOBackedStream()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	IOBackedStream::~IOBackedStream()
	{
		if (mIOSAStream.IsValid())
		{
			// Remove the stream's callback
			(void) (**mIOSAStream).SetOutputCallback(mIOSAStream, NULL, NULL);

			// Remove the stream from the notification thread's run loop
			(void) (**mIOSAStream).RemoveFromRunLoop(mIOSAStream, GetOwningDevice().GetNotificationThread().GetRunLoop());
		}
	}

    #pragma mark -
    //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    // Start()
    //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    void IOBackedStream::SetStreamFormat(IOVideoStreamDescription *newStreamFormat) {
        UInt32 streamID = CACFNumber(static_cast<CFNumberRef>(CFDictionaryGetValue(mStreamDictionary.GetCFDictionary(), CFSTR(kIOVideoStreamKey_StreamID))), false).GetSInt32();
        GetOwningDevice().GetIOVADevice().SetStreamFormat(streamID, newStreamFormat);
    }

	#pragma mark -
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Start()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void IOBackedStream::Start(Client client, mach_port_t messagePort, UInt32 initialDiscontinuityFlags)
	{
		try
		{
			// Grab the mutex for the device's state
			CAMutex::Locker locker(mStateMutex);
			
			// Add the  Client to the set of clients to which are listening to the stream
			if (MACH_PORT_NULL != client)
			{
				// Don't do anything if the client is already in the map of ClientStreams
				if (mClientStreams.end() != mClientStreams.find(client))
					return;
					
				// Create the ClientStream object to help manage this client's connection to the stream
				ClientStream* clientStream = new ClientStream(client, messagePort, mFrameAvailableGuard);
				
				// Add it to the map of clients using the stream
				CAMutex::Locker clientStreamsLocker(mClientStreamsMutex);
				mClientStreams[client] = clientStream;
			}
			
			// If the stream is currently active nothing else needs to be done so simply return
			if (mStreaming)
				return;
			
			// Attempt to open the stream
			mIOSAStream.Open();
			
			try
			{
				if (IsInput())
				{
					// Start the stream
					mIOSAStream.Start();
				}
				else
				{
					// Make sure the client provided a valid messagePort from which to request buffers
					ThrowIf(MACH_PORT_NULL == messagePort and MACH_PORT_NULL != client, CAException(kCMIOHardwareIllegalOperationError), "Device::StartStream: output client provided an invalid message port");

					if (MACH_PORT_NULL != messagePort)
					{
						// Remember where to request frames from
						mOutputBufferRequestPort = messagePort;

						// Remember which client started the output stream 
						mOutputClient = client;
					}

					// Release any lingering output sample buffer
					mOutputBuffer.Reset();
					mFreeList.clear();

					// Intialize event times for providing device clock information
					mEvents[0].mEventTime		= kCMTimeInvalid;
					mEvents[0].mHostTimeInNanos	= 0LL;
					mPreviousEvent				= mEvents[0];
					mPreviousNotedEvent			= mEvents[0];
					mCurrentEventIndex			= 0;

					// Start the stream
					mIOSAStream.Start();
				}
			}
			catch (...)
			{
				// Something went wrong, so close the stream 
				mIOSAStream.Close();
				throw;
			}

			SetDiscontinuityFlags(initialDiscontinuityFlags);			// Set the initial discontinuity flags

			// The stream is active now
			mStreaming = true;
		}
		catch (...)
		{
			// Something went wrong, so clean up
			ClientStreamMap::iterator i = mClientStreams.find(client);
			if (i != mClientStreams.end())
			{
				// The ClientStream had been succesfully created, so delete it and erase it from the map
				CAMutex::Locker clientStreamsLocker(mClientStreamsMutex);
				delete (*i).second;
				mClientStreams.erase(i);
			}
			else if (MACH_PORT_NULL != messagePort)
			{
				// The ClientStream had not been created, so deallocate the message port since no ~ClientStreams() was invoked (which normally handles the deallocation)
				(void) mach_port_deallocate(mach_task_self(), messagePort);

				// Reset mOutputBufferRequestPort & mOutputClient to MACH_PORT_NULL
				mOutputBufferRequestPort = MACH_PORT_NULL;
				mOutputClient = MACH_PORT_NULL;
			}
			throw;
		}
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Stop()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void IOBackedStream::Stop(Client client)
	{
		// Grab the mutex for the overall device's state
		CAMutex::Locker locker(mStateMutex);

		// Delete and erase the client's from the ClientStream map
		ClientStreamMap::iterator i = mClientStreams.find(client);
		if (i != mClientStreams.end())
		{
			CAMutex::Locker clientStreamsLocker(mClientStreamsMutex);
			delete (*i).second;
			mClientStreams.erase(i);
		}

		// Don't stop the stream if it isn't streaming
		if (not mStreaming)
			return;

		// Don't stop the stream if it is an input stream but there are still clients watching it
		if (IsInput() and (not mClientStreams.empty() and MACH_PORT_NULL != client))
			return;

		// Throw an exception if this is an output stream but the stop request is coming from a client that did not make the initial start request
		ThrowIf(IsOutput() and (mOutputClient != client and MACH_PORT_NULL != client), CAException(kCMIODevicePermissionsError), "Device::StopStream: stream was started by a different client"); 

		// The stream is no longer active
		mStreaming = false;

		// Make sure StreamOutputCallback() is not being invoked on another thread
		while (mInOutputCallBack)
			usleep(1000);
			
		if (IsInput())
		{
			// Stop the IOStream
			mIOSAStream.Stop();
		
			// Notify all the client stream message threads that a frame might be available
			mFrameAvailableGuard.NotifyAll();		

			// If there are still clients streaming, wait for each client queue of any existing frames be drained
			for (ClientStreamMap::iterator i = mClientStreams.begin() ; i != mClientStreams.end() ; std::advance(i, 1))
			{
				// Yield if a client still has frames in its queue to send
				while (0 != (*i).second->GetQueue().GetCount())
					pthread_yield_np();
			}
		}
		else
		{
			// Stop the IOStream
			mIOSAStream.Stop();
		
			// Release any lingering output buffer
			mOutputBuffer.Reset();
			mFreeList.clear();
			
			if (client != MACH_PORT_NULL)
			{
				// Release the Mach port from which frames were requested 
				(void) mach_port_deallocate(mach_task_self(), mOutputBufferRequestPort);
				mOutputBufferRequestPort = MACH_PORT_NULL;
				
				// Indicate no client is using the output stream
				mOutputClient = MACH_PORT_NULL;
			}
		}
	
		// Close the IOSteam
		mIOSAStream.Close();
	}

 	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// StreamOutputCallback()
	//	Called when a new buffer is available from the kernel
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    void IOBackedStream::StreamOutputCallback(IOStreamRef /*streamRef*/, IOBackedStream& stream)
    {
		// Indicate that the output callback is being invoked so Stop() won't release resources  
		stream.mInOutputCallBack = true;
		
		if (not stream.mStreaming)
		{
			// The stream has been stopped already, so exit from callback
			stream.mInOutputCallBack = false;
			return;
		}
			
		if (stream.IsInput())
		{
            IOStreamBufferQueueEntry entry;
			while (kIOReturnSuccess == (**stream.mIOSAStream).DequeueOutputEntry(stream.mIOSAStream, &entry))
			{
				// Don't let any exceptions leave this callback
				try
				{
					stream.FrameArrived(entry);
				}
				catch (...)
				{
					// Something went wrong, so return the entry to the queue
					(**stream.mIOSAStream).EnqueueInputBuffer(stream.mIOSAStream, entry.bufferID, 0, 0, 0, 0);
					(**stream.mIOSAStream).SendInputNotification(stream.mIOSAStream, 0xAA);
				}
			}
		}
		else
		{
			// Process the entries on the output buffer list and move them to the free list
			IOStreamBufferQueueEntry entry;
			while (kIOReturnSuccess == (**stream.mIOSAStream).DequeueOutputEntry(stream.mIOSAStream, &entry))
			{
				SampleVideoDeviceControlBuffer theBufferControl;
				bcopy(stream.mIOSAStream.GetControlBuffer(entry.bufferID), &theBufferControl, sizeof(SampleVideoDeviceControlBuffer));
				DebugMessage("Buffer Control info vbitime = %lld dmatime = %lld framecount=%lld dropcount = %d lastSequenceNumber = %lld discontinuity flags = %ld", theBufferControl.vbiTime, theBufferControl.outputTime, theBufferControl.totalFrameCount, theBufferControl.droppedFrameCount, theBufferControl.sequenceNumber, theBufferControl.discontinuityFlags);
				if ((theBufferControl.discontinuityFlags & kCMIOSampleBufferDiscontinuityFlag_DataWasFlushed))
				{
					DebugMessage("A flushed buffer skip timing");
					stream.mLastOutputSequenceNumber = theBufferControl.sequenceNumber; 
				}
				else
				{
					stream.mLastOutputSequenceNumber = theBufferControl.sequenceNumber; 
					// Determine clock time based on frame count
					if (CMTIME_IS_INVALID(stream.mEvents[0].mEventTime))
					{
						DebugMessage("DPA::Sample::Server::Stream::StreamOutputCallback invalid mEventTime");
						// RFP FixMe and support for other sample rates
						stream.mEvents[1].mEventTime = stream.mNominalFrameDuration;
						stream.mEvents[1].mEventFrameCount = theBufferControl.totalFrameCount; 
						stream.mEvents[1].mDroppedFrameCount = (UInt32)theBufferControl.droppedFrameCount; 
						
						stream.mEvents[0].mEventTime = CMTimeMake(stream.mEvents[1].mEventTime.timescale, stream.mEvents[1].mEventTime.timescale);
						stream.mEvents[0].mEventFrameCount = stream.mEvents[1].mEventFrameCount;
						stream.mEvents[0].mDroppedFrameCount = stream.mEvents[1].mDroppedFrameCount;
					}
					else
					{
						// Bump to next frame
						stream.mEvents[0].mEventFrameCount = theBufferControl.totalFrameCount;
						stream.mEvents[0].mDroppedFrameCount = (UInt32)theBufferControl.droppedFrameCount;
						stream.mEvents[0].mEventTime = CMTimeAdd(stream.mEvents[0].mEventTime, CMTimeMake(stream.mNominalFrameDuration.value*(stream.mEvents[0].mEventFrameCount- stream.mEvents[1].mEventFrameCount), stream.mNominalFrameDuration.timescale));
						if (stream.mEvents[0].mDroppedFrameCount != stream.mEvents[1].mDroppedFrameCount)
						{
							++stream.mUnderrunCount;
							
							// The kTundraStreamPropertyOutputBufferUnderrunCount property has changed, so send out notifications
							stream.mProperties[PropertyAddress(kCMIOStreamPropertyOutputBufferUnderrunCount, stream.GetDevicePropertyScope(), stream.GetStartingDeviceChannelNumber())].mShadowTime = CAHostTimeBase::GetTheCurrentTime();
							(stream.GetOwningDevice()).SendPropertyStatesChangedMessage();
							DebugMessage("DPA::Sample::Server::Stream::StreamOutputCallback:  Dropped a frame (%ld)", stream.mUnderrunCount);
						}
						stream.mEvents[1].mEventFrameCount = theBufferControl.totalFrameCount; 
						stream.mEvents[1].mDroppedFrameCount = (UInt32)theBufferControl.droppedFrameCount;
					}
					
					// Save hosttime
					stream.mEvents[0].mHostTimeInNanos = CAHostTimeBase::ConvertToNanos(theBufferControl.vbiTime);
                }
                        
				stream.mFreeList.push_back(entry);
			}
                        
			// Get a mOutputBuffer
            UInt32 buffersServiced = 0;
            while (buffersServiced < kMaxRequestsPerCallback)
            {
                if (not stream.mOutputBuffer.IsValid())
                    stream.GetOutputBuffer(stream.mOutputBufferRequestPort);
                        
                if (not stream.mOutputBuffer.IsValid() or not stream.mStreaming)
                    break;
                
				if (not stream.mFreeList.empty())
				{
					entry = stream.mFreeList.front();
					stream.mFreeList.pop_front();
                    
                    //copy the data from the mOutputBuffer to the entry and send back
                    if (stream.mOutputBuffer.GetDataLength() <= entry.dataLength)
                    {
                        SampleVideoDeviceControlBuffer theBufferControl;
                        bcopy(stream.mIOSAStream.GetControlBuffer(entry.bufferID), &theBufferControl, sizeof(SampleVideoDeviceControlBuffer));
									
						bcopy(stream.mOutputBuffer.GetDataPointer(0, NULL, NULL), stream.mIOSAStream.GetDataBuffer(entry.bufferID), stream.mOutputBuffer.GetDataLength());
                        
                        theBufferControl.sequenceNumber = stream.mCurrentOutputSequenceNumber;
                        theBufferControl.discontinuityFlags = stream.mCurrentDiscontinuityFlags;
                        theBufferControl.smpteTime = stream.mCurrentSMPTETime;

//                        DebugMessage("theBufferControl.mCounter = %ld",theBufferControl.smpteTime.mCounter); 
//                        DebugMessage("theBufferControl.mHours = %ld",theBufferControl.smpteTime.mHours); 
//                        DebugMessage("theBufferControl.mMinutes = %ld",theBufferControl.smpteTime.mMinutes); 
//                        DebugMessage("theBufferControl.mSeconds = %ld",theBufferControl.smpteTime.mSeconds); 
//                        DebugMessage("theBufferControl.mFrames = %ld",theBufferControl.smpteTime.mFrames); 

                        
                        bcopy(&theBufferControl, stream.mIOSAStream.GetControlBuffer(entry.bufferID), sizeof(SampleVideoDeviceControlBuffer));
					}
                    else
                    {
                        DebugMessage("MIO::DPA::Sample::Server::Stream::StreamOutputCallback:  Mismatched Data length buffer = %ld entry = %ld", stream.mOutputBuffer.GetDataLength(), entry.dataLength);
                    }
					(**stream.mIOSAStream).EnqueueInputBuffer(stream.mIOSAStream, entry.bufferID, 0, 0, 0, 0);
					(**stream.mIOSAStream).SendInputNotification(stream.mIOSAStream, 0xAA);
                    
                    stream.mOutputBuffer.Reset();
                }
                else
                {
                    DebugMessage("MIO::DPA::Sample::Server::Stream::StreamOutputCallback: NO BUFFERS IN STREAM");
                    break;
				}
				
                ++buffersServiced;
			}
		}
 
		// Indicate that the output callback is not being invoked
		stream.mInOutputCallBack = false;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// FrameArrived()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void IOBackedStream::FrameArrived(IOStreamBufferQueueEntry& entry)
	{
		// Get the host time of the the frame (currently reported as the sole contents of the entry's control buffer)
		SampleVideoDeviceControlBuffer* theBufferControl = reinterpret_cast<SampleVideoDeviceControlBuffer*>(mIOSAStream.GetControlBuffer(entry.bufferID));
		ThrowIfNULL(theBufferControl, CAException(kCMIOHardwareUnspecifiedError), "Stream::FrameArrived: unable to get control buffer");
		
		// Create the presentation time stamp
		CMTime presentationTimeStamp = CMTimeMakeWithSeconds(CAHostTimeBase::ConvertToNanos(theBufferControl->vbiTime) / 1000000000.0, GetNominalFrameDuration().timescale);
		
		// Create the timing information
		CMA::SampleBuffer::TimingInfo timingInfo(GetNominalFrameDuration(), presentationTimeStamp, kCMTimeInvalid);
		
		// Wrap the entry in a Frame
		Frame* frame = new IOBackedFrame(mIOSAStream, GetFrameType(), theBufferControl->vbiTime, timingInfo, GetDiscontinuityFlags(), (UInt32)theBufferControl->droppedFrameCount, theBufferControl->firstVBITime, entry.bufferID, entry.dataLength, mIOSAStream.GetDataBuffer(entry.bufferID));

		// Clear the discontinuity flags since any accumulated discontinuties have passed onward with the frame
		SetDiscontinuityFlags(kCMIOSampleBufferNoDiscontinuities);
	
		// Create a queue of client queues into which the frame should be inserted
		std::vector<CMA::SimpleQueue<Frame*>*> queues;
		
		// Grab the ClientStreams mutex so they won't be altered in the midst of this routine
		CAMutex::Locker clientStreamsLocker(mClientStreamsMutex);

		// Insert the frame into each client's queue
		for (ClientStreamMap::iterator i = mClientStreams.begin() ; i != mClientStreams.end() ; std::advance(i, 1))
		{
			if (1.0 != (*i).second->GetQueue().Fullness())
			{
				// Add the client to the set of clients to which this frame will be messaged
				frame->AddClient((*i).first);
				queues.push_back(&(*i).second->GetQueue());
			}
			else
			{
				// This condition should never be hit, but if that assumption proves false then some code changes would be necessary to properly record the dropped frame.
				DebugMessage("Stream::FrameArrived: client (%d) queue full <---------- NEED TO EXTEND", (*i).first);
			}
		}

		// Enqueue all the frame for all the clients that had room
		for (std::vector<CMA::SimpleQueue<Frame*>*>::iterator i = queues.begin() ; i != queues.end() ; std::advance(i, 1))
			(**i).Enqueue(frame);

		// Make sure at least one client wanted the message
		if (queues.empty())
		{
			// There were no clients to message this too, so invoke RemoveClient(MACH_PORT_NULL) on the frame to make it available for refilling 
			frame->RemoveClient(MACH_PORT_NULL);
		}
		
		if (true)
		{
			mDeck.FrameArrived();
		}
		
		// Notify all the client stream message threads that a frame is available
		mFrameAvailableGuard.NotifyAll();		
	}

}}}}
