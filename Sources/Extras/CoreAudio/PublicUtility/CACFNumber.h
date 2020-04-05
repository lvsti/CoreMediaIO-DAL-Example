/*
     File: CACFNumber.h
 Abstract: Part of CoreAudio Utility Classes
  Version: 1.1

*/
#if !defined(__CACFNumber_h__)
#define __CACFNumber_h__

//=============================================================================
//	Includes
//=============================================================================

#if !defined(__COREAUDIO_USE_FLAT_INCLUDES__)
	#include <CoreAudio/CoreAudioTypes.h>
	#include <CoreFoundation/CFNumber.h>
#else
	#include <CoreAudioTypes.h>
	#include <CFNumber.h>
#endif

//=============================================================================
//	CACFBoolean
//=============================================================================

class	CACFBoolean
{
//	Construction/Destruction
public:
	explicit		CACFBoolean(CFBooleanRef inCFBoolean) : mCFBoolean(inCFBoolean), mWillRelease(true) {}
					CACFBoolean(CFBooleanRef inCFBoolean, bool inWillRelease) : mCFBoolean(inCFBoolean), mWillRelease(inWillRelease) {}
	explicit		CACFBoolean(bool inValue) : mCFBoolean(inValue ? kCFBooleanTrue : kCFBooleanFalse), mWillRelease(true) { Retain(); }
					~CACFBoolean() { Release(); }
					CACFBoolean(const CACFBoolean& inBoolean) : mCFBoolean(inBoolean.mCFBoolean), mWillRelease(inBoolean.mWillRelease) { Retain(); }
	CACFBoolean&	operator=(const CACFBoolean& inBoolean) { Release(); mCFBoolean = inBoolean.mCFBoolean; mWillRelease = inBoolean.mWillRelease; Retain(); return *this; }
	CACFBoolean&	operator=(CFBooleanRef inCFBoolean) { Release(); mCFBoolean = inCFBoolean; mWillRelease = true; return *this; }

private:
	void			Retain() { if(mWillRelease && (mCFBoolean != NULL)) { CFRetain(mCFBoolean); } }
	void			Release() { if(mWillRelease && (mCFBoolean != NULL)) { CFRelease(mCFBoolean); } }
	
	CFBooleanRef	mCFBoolean;
	bool			mWillRelease;

//	Operations
public:
	void			AllowRelease() { mWillRelease = true; }
	void			DontAllowRelease() { mWillRelease = false; }
	bool			IsValid() { return mCFBoolean != NULL; }

//	Value Access
public:
	CFBooleanRef	GetCFBoolean() const { return mCFBoolean; }
	CFBooleanRef	CopyCFBoolean() const { if(mCFBoolean != NULL) { CFRetain(mCFBoolean); } return mCFBoolean; }

	bool			GetBoolean() const { bool theAnswer = false; if(mCFBoolean != NULL) { theAnswer = CFEqual(mCFBoolean, kCFBooleanTrue); } return theAnswer; }
	
					CACFBoolean(const void*);	// prevent accidental instantiation with a pointer via bool constructor
};

//=============================================================================
//	CACFNumber
//=============================================================================

class	CACFNumber
{
//	Construction/Destruction
public:
	explicit	CACFNumber(CFNumberRef inCFNumber) : mCFNumber(inCFNumber), mWillRelease(true) {}
				CACFNumber(CFNumberRef inCFNumber, bool inWillRelease) : mCFNumber(inCFNumber), mWillRelease(inWillRelease) {}
				CACFNumber(SInt32 inValue) : mCFNumber(CFNumberCreate(NULL, kCFNumberSInt32Type, &inValue)), mWillRelease(true) {}
				CACFNumber(UInt32 inValue) : mCFNumber(CFNumberCreate(NULL, kCFNumberSInt32Type, &inValue)), mWillRelease(true) {}
				CACFNumber(SInt64 inValue) : mCFNumber(CFNumberCreate(NULL, kCFNumberSInt64Type, &inValue)), mWillRelease(true) {}
				CACFNumber(UInt64 inValue) : mCFNumber(CFNumberCreate(NULL, kCFNumberSInt64Type, &inValue)), mWillRelease(true) {}
				CACFNumber(Float32 inValue) : mCFNumber(CFNumberCreate(NULL, kCFNumberFloat32Type, &inValue)), mWillRelease(true) {}
				CACFNumber(Float64 inValue) : mCFNumber(CFNumberCreate(NULL, kCFNumberFloat64Type, &inValue)), mWillRelease(true) {}
				~CACFNumber() { Release(); }
				CACFNumber(const CACFNumber& inNumber) : mCFNumber(inNumber.mCFNumber), mWillRelease(inNumber.mWillRelease) { Retain(); }
	CACFNumber&	operator=(const CACFNumber& inNumber) { Release(); mCFNumber = inNumber.mCFNumber; mWillRelease = inNumber.mWillRelease; Retain(); return *this; }
	CACFNumber&	operator=(CFNumberRef inCFNumber) { Release(); mCFNumber = inCFNumber; mWillRelease = true; return *this; }

private:
	void		Retain() { if(mWillRelease && (mCFNumber != NULL)) { CFRetain(mCFNumber); } }
	void		Release() { if(mWillRelease && (mCFNumber != NULL)) { CFRelease(mCFNumber); } }
	
	CFNumberRef	mCFNumber;
	bool		mWillRelease;

//	Operations
public:
	void		AllowRelease() { mWillRelease = true; }
	void		DontAllowRelease() { mWillRelease = false; }
	bool		IsValid() const { return mCFNumber != NULL; }

//	Value Access
public:
	CFNumberRef	GetCFNumber() const { return mCFNumber; }
	CFNumberRef	CopyCFNumber() const { if(mCFNumber != NULL) { CFRetain(mCFNumber); } return mCFNumber; }

	SInt8		GetSInt8() const { SInt8 theAnswer = 0; if(mCFNumber != NULL) { CFNumberGetValue(mCFNumber, kCFNumberSInt8Type, &theAnswer); } return theAnswer; }
	SInt32		GetSInt32() const { SInt32 theAnswer = 0; if(mCFNumber != NULL) { CFNumberGetValue(mCFNumber, kCFNumberSInt32Type, &theAnswer); } return theAnswer; }
	UInt32		GetUInt32() const { UInt32 theAnswer = 0; if(mCFNumber != NULL) { CFNumberGetValue(mCFNumber, kCFNumberSInt32Type, &theAnswer); } return theAnswer; }
	Float32		GetFloat32() const { Float32 theAnswer = 0.0f; if(mCFNumber != NULL) { CFNumberGetValue(mCFNumber, kCFNumberFloat32Type, &theAnswer); } return theAnswer; }
	Float32		GetFixed32() const;
	Float64		GetFixed64() const;
	SInt64		GetSInt64() const { SInt64 theAnswer = 0; if(mCFNumber != NULL) { CFNumberGetValue(mCFNumber, kCFNumberSInt64Type, &theAnswer); } return theAnswer; }
	
				CACFNumber(const void*);	// prevent accidental instantiation with a pointer via bool constructor
};

#endif
