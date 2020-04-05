/*
	    File: CMIO_DPA_Sample_Server_Stream.h
	Abstract: n/a
	 Version: 1.2

*/

#if !defined(__CMIO_DPA_Sample_Server_Stream_h__)
#define __CMIO_DPA_Sample_Server_Stream_h__

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Internal Includes
#include "CMIO_DPA_Sample_Server_Deck.h"

// Public Utility Includes
#include "CMIO_CMA_BlockBuffer.h"

// CA Public Utility Includes
#include "CACFArray.h"
#include "CACFDictionary.h"
#include "CAGuard.h"

// System Includes
#include <CoreMedia/CMTime.h>
#include <IOKit/video/IOVideoTypes.h>

// Standard Library Includes
#include <map>
#include <list>

namespace CMIO { namespace DPA { namespace Sample { namespace Server
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Types in the CMIO::DPA::Sample::Server namespace
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class Device;
	class ClientStream;
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Stream
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class Stream
	{
	// Construction/Destruction
	public:
										Stream(Device* device, CFDictionaryRef streamDictionary, CMIOObjectPropertyScope scope);
		virtual							~Stream();

	private:
		Stream&							operator=(Stream& that); // Don't allow copying

	protected:
		Device*							mDevice;
		CACFDictionary					mStreamDictionary;

	// Attributes & Properties
	public:
		Device&							GetOwningDevice() { return *mDevice; }
		bool							IsInput() const { return mIsInput; }
		bool							IsOutput() const { return not mIsInput; }
		CMIOObjectPropertyScope			GetDevicePropertyScope() const { return mIsInput ? kCMIODevicePropertyScopeInput : kCMIODevicePropertyScopeOutput; }
		CMIOObjectPropertyScope			GetOppositeDevicePropertyScope() const	{ return mIsInput ? kCMIODevicePropertyScopeOutput : kCMIODevicePropertyScopeInput; }
		UInt32							GetStartingDeviceChannelNumber() const;
		UInt32							GetCurrentNumberChannels() const;
		bool							Streaming() const { return mStreaming; }
		UInt32							GetDiscontinuityFlags() const { return mDiscontinuityFlags; }
		void							SetDiscontinuityFlags(UInt32 discontinuityFlags) { mDiscontinuityFlags = discontinuityFlags; }
		
		void							GetProperties(UInt64 time, const PropertyAddress& matchAddress, PropertyAddressList& matches) const;

		UInt32							GetNoDataTimeout() const { return mNoDataTimeout; }
		void							SetNoDataTimeout(UInt32 noDataTimeout);
		UInt32							GetNoDataEventCount() const { return mNoDataEventCount; }
		UInt32							GetDeviceSyncTimeout() const { return mDeviceSyncTimeout; }
		void							SetDeviceSyncTimeout(UInt32 deviceSyncTimeout);

		bool							GetEndOfData() const { return mEndOfData; }
		void							SetEndOfData(bool endOfData);
        UInt32                          FrameRateToCodecFlags(Float64 framerate);
        Float64                         CodecFlagsToFrameRate(UInt32 codecFlags);

	protected:
		CAMutex							mStateMutex;				// Controls access to device's state
		bool							mIsInput;
		bool							mStreaming;					// True when streaming
		bool							mInOutputCallBack;			// True when in StreamOutputCallback()
		
	// Input items
		UInt32							mDiscontinuityFlags;

	// Output items
		CMA::BlockBuffer				mOutputBuffer;
		mach_port_t						mOutputBufferRequestPort;
		Client							mOutputClient;
        UInt64                          mLastOutputSequenceNumber;
        UInt64                          mCurrentOutputSequenceNumber;
        UInt32                          mCurrentDiscontinuityFlags;
        SMPTETime                       mCurrentSMPTETime;


	
		Properties						mProperties;

	// Input Stream Only Properties
		UInt32							mNoDataTimeout;						// kCMIOStreamPropertyNoDataTimeoutInMSec
		UInt32							mNoDataEventCount;					// kCMIOStreamPropertyNoDataEventCount
		bool							mNoDataNotificationHasOccured;
		UInt32							mDeviceSyncTimeout;					// kCMIOStreamPropertyDeviceSyncTimeoutInMSec
		bool							mDeviceSyncTimeoutChanged;			// true if mDeviceSyncTimeout has changed since the last time the NoData() callback has been invoked 
		UInt32							mDeviceSyncCount;

	// Output only properties
		bool							mEndOfData;							// kCMIOStreamPropertyEndOfData
		UInt32							mUnderrunCount;						// kCMIOStreamPropertyOutputBufferUnderrunCount

	// Format & Frame Rate
	public:
		vm_size_t						GetFrameFormats(FrameFormat** formats) const;
		FrameType						GetFrameType() const { return mFrameType; }
		void							SetFrameType(FrameType frameType);
		vm_size_t						GetFrameRates(const FrameType* qualifier, Float64** frameRates) const;
		Float64							GetFrameRate() const { return mFrameRate; }
		void							SetFrameRate(Float64 frameRate);
		CMTime							GetNominalFrameDuration() { return mFrameRatesMap[mFrameType][mFrameRate]; }		

    protected:
        virtual void					SetStreamFormat(IOVideoStreamDescription *newStreamFormat) = 0;
        
	protected:
		typedef std::set<FrameFormat, FrameFormat::Less> FrameFormats;
		typedef std::map<Float64, CMTime> FrameRates;			
		typedef std::map<FrameType, FrameRates> FrameTypeToFrameRatesMap;	

		FrameFormats					mFrameFormats;			// The frame types and associated format info
		FrameType						mFrameType;				// The device's current FrameType
		FrameTypeToFrameRatesMap		mFrameRatesMap;			// The map of frame rates each FrameType is capable of generating 
		Float64							mFrameRate;				// The device's current frame rate
		CMTime							mNominalFrameDuration;	// The device's current frame duration	

	// Management
	public:
		virtual void					Start(Client client, mach_port_t messagePort, UInt32 initialDiscontinuityFlags) = 0;
		virtual void					Stop(Client client) = 0;
		
		void							GetOutputBuffer(mach_port_t& recipient);
		static void						ReleaseOutputBufferCallBack(void* refCon, void *doomedMemoryBlock, size_t sizeInBytes);
		static void						ReleasePixelBufferCallback(void* refCon, void *doomedMemoryBlock, size_t sizeInBytes);

	protected:
		struct EventInfo
		{
			EventInfo() : mEventTime(kCMTimeInvalid), mHostTimeInNanos(0ULL), mEventFrameCount(0ULL), mDroppedFrameCount(0) {}
			CMTime	mEventTime;
			UInt64	mHostTimeInNanos;
			UInt64  mEventFrameCount;
			UInt32  mDroppedFrameCount;
		};
		
		EventInfo						mEvents[2];
		EventInfo						mPreviousEvent;
		EventInfo						mPreviousNotedEvent;
		UInt32							mCurrentEventIndex;
		
		
	protected:
		typedef std::map<UInt32, Stream> StreamMap;
		typedef std::map<Client, ClientStream*> ClientStreamMap;
		typedef std::set<Client> DeckListeners;														// Clients which have started the deck threads so they can listen to deck property changes

		ClientStreamMap					mClientStreams;				// Ports to message when a frame arrives
		CAMutex							mClientStreamsMutex;		// Mutex to protect mClientStreams when adding/removing items to the map
		CAGuard							mFrameAvailableGuard;


    protected:
		Deck							mDeck;
		DeckListeners					mDeckListeners;
		
	// Deck
	public:
		void							StartDeckThreads(Client client);
		void							StopDeckThreads(Client client);

		void							DeckPlay() { mDeck.Play(); }
		void							DeckStop() { mDeck.Stop(); }
		void							DeckJog(SInt32 speed)  { mDeck.Jog(speed); }
		void							DeckCueTo( Float64 requestedTimecode, Boolean playOnCue) { mDeck.CueTo(requestedTimecode, playOnCue); }

		
		
		CMIOStreamDeck		    		GetStreamDeck() const { return mDeck.GetStreamDeck(); }
		Float64							GetTimecode() const { return mDeck.GetTimecode(); }
		SInt32							GetCueState() const { return mDeck.GetCueState(); } 

		void							StreamDeckChanged();
		void							DeckTimecodeChanged();
		void							DeckCueingChanged();

	};
}}}}
#endif
