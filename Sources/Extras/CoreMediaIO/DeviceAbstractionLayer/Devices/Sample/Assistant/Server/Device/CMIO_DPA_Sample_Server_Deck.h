/*
	    File: CMIO_DPA_Sample_Server_Deck.h
	Abstract: n/a
	 Version: 1.2

*/

#if !defined(__CMIO_DPA_Sample_Server_Deck_h__)
#define __CMIO_DPA_Sample_Server_Deck_h__

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Internal Includes
#include "CMIO_DPA_Sample_Server_Common.h"
#include "CMIO_DPA_Sample_Shared.h"

// CA Public Utility Includes
#include "CACFString.h"
#include "CAAutoDisposer.h"
#include "CAGuard.h"
#include "CAPThread.h"

namespace CMIO { namespace DPA { namespace Sample { namespace Server
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Types
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class Stream;
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Deck
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class Deck
	{
	#pragma mark -
	#pragma mark Types in class Deck
	public:
		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// Deck::StreamDeck
		//	Deck::StreamDeck extends the CMIOStreamDeck structure to C++ including constructors and other utility operations.
		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		struct StreamDeck : public CMIOStreamDeck 
		{
		// Construction/Destruction
		public:
						StreamDeck()											: CMIOStreamDeck() { mStatus = kCMIODeckStatusLocal; mState = 0; mState2 = 0; }
						StreamDeck(UInt32 status, UInt32 state, UInt32 state2)	: CMIOStreamDeck() { mStatus = status; mState = state; mState2 = state2; }
						StreamDeck(const CMIOStreamDeck& streamDeck)			: CMIOStreamDeck(streamDeck) {}
						StreamDeck(const StreamDeck& streamDeck)				: CMIOStreamDeck(streamDeck) {}
			StreamDeck&	operator=(const CMIOStreamDeck& streamDeck)			{ CMIOStreamDeck::operator=(streamDeck); return *this; }
			StreamDeck&	operator=(const StreamDeck& streamDeck)					{ CMIOStreamDeck::operator=(streamDeck); return *this; }
		};

	#pragma mark -
	#pragma mark Deck
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Deck
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Construction/Destruction
	public:
							Deck(Stream& stream);
		virtual				~Deck();
	
	protected:
		CAMutex				mStateMutex;				// Controls access to decks's state
		Stream&				mOwningStream;

	private:
		bool				mDeckThreadRunning;			// True when deck thread is running
		
		
	// Deck Control
	public:
		void				StartThreads();
		void				StopThreads();
		void				Play();
		void				Stop();
		void				Jog(SInt32 speed);
		void				CueTo(Float64 requestedTimecode, Boolean playOnCue);

		CMIOStreamDeck	GetStreamDeck() const { return mStreamDeck; } 
		Float64				GetTimecode() const { return mTimecode; } 
		SInt32				GetCueState() const { return mCueState; } 

		void				FrameArrived();

	private:
		void				SetStreamDeck(UInt32 status, UInt32 state, UInt32 state2) { mStreamDeck.mStatus = status; mStreamDeck.mState = state; mStreamDeck.mState2 = state2; }


		void				StartSimulationThread();
		void				StopSimulationThread();
		void				SetDeckMode(UInt32 mode);

	private:
		StreamDeck			mStreamDeck;
		Float64				mTimecode;
		SInt32				mCueState;
		bool				mDirection;
		
		Float64				mCurrentFrameCount;
		UInt32				mCurrentDeckMode;
		UInt32				mNewDeckMode;
		CAPThread			mSimulationThread;
		CAGuard				mSimulationGuard;
		bool				mStopSimulationThread;
		CAPThread			mTimecodeUpdateThread;
		CAGuard				mTimecodeUpdateGuard;
		bool				mStopTimecodeUpdateThread;
		CAPThread			mCueingThread;
		CAGuard				mCueingGuard;
		bool				mStopCueingThread;
		bool				mCueing;
		Float64				mRequestedCuePoint;
		bool				mStepping;
		UInt32				mStepCount;
		Boolean				mPlayOnCue;
		
	private:
		static void*		SimulationEntry(Deck& deck);
		static void*		TimecodeUpdateEntry(Deck& deck);
		static void*		CueingEntry(Deck& deck);
	};
}}}}

#endif


