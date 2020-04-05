/*
     File: CAAudioBufferList.h
 Abstract: Part of CoreAudio Utility Classes
  Version: 1.1
*/
#if !defined(__CAAudioBufferList_h__)
#define __CAAudioBufferList_h__

//=============================================================================
//	Includes
//=============================================================================

//	System Includes
#if !defined(__COREAUDIO_USE_FLAT_INCLUDES__)
	#include <CoreAudio/CoreAudioTypes.h>
#else
	#include <CoreAudioTypes.h>
#endif

#include "CAException.h"
#include "CADebugMacros.h"

//=============================================================================
//	Types
//=============================================================================

typedef AudioBufferList*	AudioBufferListPtr;

//=============================================================================
//	CAAudioBufferList
//=============================================================================

struct	CAAudioBufferList
{

//	Construction/Destruction
public:
	static AudioBufferList*	Create(UInt32 inNumberBuffers);
	static void				Destroy(AudioBufferList* inBufferList);
	static UInt32			CalculateByteSize(UInt32 inNumberBuffers);

//	Operations
public:
	static UInt32			GetTotalNumberChannels(const AudioBufferList& inBufferList);
	static bool				GetBufferForChannel(const AudioBufferList& inBufferList, UInt32 inChannel, UInt32& outBufferNumber, UInt32& outBufferChannel);
	static void				Clear(AudioBufferList& outBufferList);
	static void				Copy(const AudioBufferList& inSource, UInt32 inStartingSourceChannel, AudioBufferList& outDestination, UInt32 inStartingDestinationChannel);
	static void				CopyChannel(const AudioBuffer& inSource, UInt32 inSourceChannel, AudioBuffer& outDestination, UInt32 inDestinationChannel);
	static void				Sum(const AudioBufferList& inSourceBufferList, AudioBufferList& ioSummedBufferList);
	static bool				HasData(AudioBufferList& inBufferList);
#if	CoreAudio_Debug
	static void				PrintToLog(const AudioBufferList& inBufferList);
#endif

	static const AudioBufferList*	GetEmptyBufferList();

};

// Declare a variably-sized AudioBufferList on the stack
#define STACK_ABL(name, nbufs) \
	ThrowIf(nbufs < 1 || nbufs > 64, CAException(kAudio_ParamError), "STACK_ABL: invalid number of buffers") \
	const size_t name##_ByteSize = sizeof(AudioBufferList) + (nbufs - 1) * sizeof(AudioBuffer); \
	AudioBufferList &name = *(AudioBufferList *)alloca(name##_ByteSize); \
	name.mNumberBuffers = nbufs;


#endif
