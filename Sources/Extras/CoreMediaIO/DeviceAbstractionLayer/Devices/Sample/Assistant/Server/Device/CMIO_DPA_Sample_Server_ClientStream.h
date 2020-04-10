/*
	    File: CMIO_DPA_Sample_Server_ClientStream.h
	Abstract: n/a
	 Version: 1.2

*/

#if !defined(__CMIO_DPA_Sample_Server_ClientStream_h__)
#define __CMIO_DPA_Sample_Server_ClientStream_h__

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Internal Includes
#include "CMIO_DPA_Sample_Server_Common.h"
#include "CMIO_DPA_Sample_Shared.h"

// Public Utilities
#include "CMIO_CMA_SimpleQueue.h"

// CA Public Utilities
#include "CAGuard.h"
#include "CAPThread.h"


namespace CMIO { namespace DPA { namespace Sample { namespace Server
{
	class Frame;
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// ClientStream
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class ClientStream
	{
	// Construction/Destruction
	public:
										ClientStream(Client client, mach_port_t messagePort, CAGuard& frameAvailableGuard);
		virtual							~ClientStream();

	// Queue
	public:
		CMA::SimpleQueue<Frame*>&		GetQueue() { return mQueue; }

	private:
		CMA::SimpleQueue<Frame*>		mQueue;

	// Attributes
	public:
		void							SetDiscontinuityFlags(UInt32 discontinuityFlags) { mDiscontinuityFlags = discontinuityFlags; }
		UInt32							GetDiscontinuityFlags() const { return mDiscontinuityFlags; }
	
	private:
		UInt32							mDiscontinuityFlags;
		UInt64							mExtendedFrameHostTime;
		CMA::SampleBuffer::TimingInfo	mExtendedFrameTimingInfo;
	
	// Message Thread
	public:
		void							StartMessageThread();
		void							StopMessageThread();

	private:
		static void*					MessageThreadEntry(ClientStream& clientStream);
		void							SendFrameArrivedMessage(mach_port_t& recipient, Frame& frame);

		mach_port_t						mClient;
		mach_port_t						mMessagePort;
		bool							mStopMessageLoop;
		CAPThread						mMessageThread;
		CAGuard							mMessageThreadGuard;
		CAGuard&						mFrameAvailableGuard;
	};
}}}}
#endif
