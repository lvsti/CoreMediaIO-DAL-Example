/*
     File: CACFPlugIn.h
 Abstract: Part of CoreAudio Utility Classes
  Version: 1.1

*/
#if !defined(__CACFPlugIn_h__)
#define __CACFPlugIn_h__

//==================================================================================================
//	Includes
//==================================================================================================

//	System Includes
#include <CoreAudio/CoreAudioTypes.h>
#include <CoreFoundation/CFPlugIn.h>

//==================================================================================================
//	CACFPlugIn
//==================================================================================================

class CACFPlugIn
{

//	Construction/Destruction
public:
					CACFPlugIn()														: mCFPlugIn(NULL), mWillRelease(true) {}
					CACFPlugIn(CFPlugInRef inCFPlugIn, bool inWillRelease = true)		: mCFPlugIn(inCFPlugIn), mWillRelease(inWillRelease) {}
					CACFPlugIn(CFURLRef inURL, bool inWillRelease = true)				: mCFPlugIn(CFPlugInCreate(NULL, inURL)), mWillRelease(inWillRelease) {}
					~CACFPlugIn()														{ Release(); }
					CACFPlugIn(const CACFPlugIn& inObject)								: mCFPlugIn(inObject.mCFPlugIn), mWillRelease(inObject.mWillRelease) { Retain(); }
	CACFPlugIn&		operator=(const CACFPlugIn& inObject)								{ Release(); mCFPlugIn = inObject.mCFPlugIn; mWillRelease = inObject.mWillRelease; Retain(); return *this; }
	CACFPlugIn&		operator=(CFPlugInRef inCFPlugIn)									{ Release(); mCFPlugIn = inCFPlugIn; mWillRelease = true; return *this; }

private:
	void			Retain()															{ if(mWillRelease && (mCFPlugIn != NULL)) { CFRetain(mCFPlugIn); } }
	void			Release()															{ if(mWillRelease && (mCFPlugIn != NULL)) { CFRelease(mCFPlugIn); mCFPlugIn = NULL; } }
	
	CFPlugInRef		mCFPlugIn;
	bool			mWillRelease;

//	Operations
public:
	void			AllowRelease()														{ mWillRelease = true; }
	void			DontAllowRelease()													{ mWillRelease = false; }
	bool			IsValid() const														{ return mCFPlugIn != NULL; }
	
	CFBundleRef		GetBundle() const													{ CFBundleRef theAnswer = NULL; if(IsValid()) { theAnswer = CFPlugInGetBundle(mCFPlugIn); } return theAnswer; }
	CFStringRef		CopyBundleID() const												{ CFStringRef theAnswer = NULL; CFBundleRef theBundle = GetBundle(); if(IsValid() && (theBundle != NULL)) { theAnswer = CFBundleGetIdentifier(theBundle); if(theAnswer != NULL) { CFRetain(theAnswer); } } return theAnswer; }
	UInt32			GetBundleVersion() const											{ UInt32 theAnswer = 0; CFBundleRef theBundle = GetBundle(); if(IsValid() && (theBundle != NULL)) { theAnswer = CFBundleGetVersionNumber(theBundle); } return theAnswer; }
	CFDictionaryRef CopyBundleInfo() const												{ CFDictionaryRef theAnswer = NULL; CFBundleRef theBundle = GetBundle(); if(IsValid() && (theBundle != NULL)) { theAnswer = CFBundleGetInfoDictionary(theBundle); if(theAnswer != NULL) { CFRetain(theAnswer); } } return theAnswer; }
	CFArrayRef		FindFactoriesForType(CFUUIDRef inTypeUUID) const					{ CFArrayRef theAnswer = NULL; if(IsValid()) { theAnswer = CFPlugInFindFactoriesForPlugInTypeInPlugIn(inTypeUUID, mCFPlugIn); } return theAnswer; }

//	Value Access
public:
	CFPlugInRef		GetCFPlugIn() const													{ return mCFPlugIn; }
	CFPlugInRef		CopyCFPlugIn() const												{ if(mCFPlugIn != NULL) { CFRetain(mCFPlugIn); } return mCFPlugIn; }

};

#endif
