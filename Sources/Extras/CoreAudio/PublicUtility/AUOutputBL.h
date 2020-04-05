/*
     File: AUOutputBL.h
 Abstract: Part of CoreAudio Utility Classes
  Version: 1.1
*/
#ifndef __AUOutputBL_h__
#define __AUOutputBL_h__

#include "CAStreamBasicDescription.h"
#if !defined(__COREAUDIO_USE_FLAT_INCLUDES__)
#else
#endif

// ____________________________________________________________________________
//
//	AUOutputBL - Simple Buffer List wrapper targetted to use with retrieving AU output
// Works in one of two ways (both adjustable)... Can use it with NULL pointers, or allocate
// memory to receive the data in.

// Before using this with any call to AudioUnitRender, it needs to be Prepared
// as some calls to AudioUnitRender can reset the ABL

class AUOutputBL {
public:
											
											// you CANNOT use one of these - it will crash!
//										AUOutputBL ();
										
											// this is the constructor that you use
											// it can't be reset once you've constructed it
										AUOutputBL (const CAStreamBasicDescription &inDesc, UInt32 inDefaultNumFrames = 512);
										~AUOutputBL();

	void 								Prepare ()
										{
											Prepare (mFrames);
										}
									
								// this version can throw if this is an allocted ABL and inNumFrames is > AllocatedFrames()
								// you can set the bool to true if you want a NULL buffer list even if allocated
								// inNumFrames must be a valid number (will throw if inNumFrames is 0)
	void 								Prepare (UInt32 inNumFrames, bool inWantNullBufferIfAllocated = false);
	
	AudioBufferList*					ABL() { return mBufferList; }
								
								// You only need to call this if you want to allocate a buffer list
								// if you want an empty buffer list, just call Prepare()
								// if you want to dispose previously allocted memory, pass in 0
								// then you either have an empty buffer list, or you can re-allocate
								// Memory is kept around if an Allocation request is less than what is currently allocated
	void								Allocate (UInt32 inNumberFrames);
	
	UInt32								AllocatedFrames() const { return mFrames; }
	
	const CAStreamBasicDescription&		GetFormat() const { return mFormat; }

#if DEBUG
	void								Print();
#endif
	
private:
	UInt32						AllocatedBytes () const { return (mBufferSize * mNumberBuffers); }

	CAStreamBasicDescription	mFormat;
	Byte*						mBufferMemory;
	AudioBufferList* 			mBufferList;
	UInt32						mNumberBuffers;
	UInt32						mBufferSize;
	UInt32						mFrames;

// don't want to copy these.. can if you want, but more code to write!
	AUOutputBL () {}
	AUOutputBL (const AUOutputBL &c);
	AUOutputBL& operator= (const AUOutputBL& c);
};

#endif // __AUOutputBL_h__
