/*
     File: CACFData.h
 Abstract: Part of CoreAudio Utility Classes
  Version: 1.1

*/
#if !defined(__CACFData_h__)
#define __CACFData_h__

//=============================================================================
//	Includes
//=============================================================================

#if !defined(__COREAUDIO_USE_FLAT_INCLUDES__)
	#include <CoreAudio/CoreAudioTypes.h>
	#include <CoreFoundation/CFData.h>
#else
	#include <CoreAudioTypes.h>
	#include <CFData.h>
#endif

#include "CADebugMacros.h"

//=============================================================================
//	CACFData
//=============================================================================

class CACFData
{

//	Construction/Destruction
public:
	explicit	CACFData(CFDataRef inCFData) : mCFData(inCFData), mWillRelease(true) {}
				CACFData(CFDataRef inCFData, bool inWillRelease) : mCFData(inCFData), mWillRelease(inWillRelease) {}
				CACFData(const void* inData, UInt32 inDataSize) : mCFData(NULL), mWillRelease(true) { mCFData = CFDataCreate(NULL, static_cast<const UInt8*>(inData), static_cast<CFIndex>(inDataSize)); }
				~CACFData() { Release(); }
				CACFData(const CACFData& inNumber) : mCFData(inNumber.mCFData), mWillRelease(inNumber.mWillRelease) { Retain(); }
	CACFData&	operator=(const CACFData& inNumber) { Release(); mCFData = inNumber.mCFData; mWillRelease = inNumber.mWillRelease; Retain(); return *this; }
	CACFData&	operator=(CFDataRef inCFData) { Release(); mCFData = inCFData; mWillRelease = true; return *this; }

private:
	void		Retain() { if(mWillRelease && (mCFData != NULL)) { CFRetain(mCFData); } }
	void		Release() { if(mWillRelease && (mCFData != NULL)) { CFRelease(mCFData); } }
	
	CFDataRef	mCFData;
	bool		mWillRelease;

//	Operations
public:
	void		AllowRelease() { mWillRelease = true; }
	void		DontAllowRelease() { mWillRelease = false; }
	bool		IsValid() { return mCFData != NULL; }

//	Value Access
public:
	CFDataRef	GetCFData() const { return mCFData; }
	CFDataRef	CopyCFData() const { if(mCFData != NULL) { CFRetain(mCFData); } return mCFData; }
	
	UInt32		GetSize() const { return ToUInt32(CFDataGetLength(mCFData)); }
	const void*	GetDataPtr() const { return CFDataGetBytePtr(mCFData); }
	void		CopyData(UInt32 inStartOffset, void* outData, UInt32 inDataSize) const { CFRange theRange = { static_cast<CFIndex>(inStartOffset), static_cast<CFIndex>(inDataSize) }; CFDataGetBytes(mCFData, theRange, static_cast<UInt8*>(outData)); }
	
	SInt32		GetSInt32() const { SInt32 theAnswer = 0; CopyData(0, &theAnswer, SizeOf32(SInt32)); return theAnswer; }
	Float32		GetFloat32() const { Float32 theAnswer = 0; CopyData(0, &theAnswer, SizeOf32(Float32)); return theAnswer; }

};

#endif
