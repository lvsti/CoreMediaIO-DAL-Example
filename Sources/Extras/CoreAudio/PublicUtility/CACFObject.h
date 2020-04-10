/*
     File: CACFObject.h
 Abstract: Part of CoreAudio Utility Classes
  Version: 1.1

*/
#if !defined(__CACFObject_h__)
#define __CACFObject_h__

//=============================================================================
//	Includes
//=============================================================================

#if !defined(__COREAUDIO_USE_FLAT_INCLUDES__)
	#include <CoreAudio/CoreAudioTypes.h>
	#include <CoreFoundation/CoreFoundation.h>
#else
	#include <CoreAudioTypes.h>
	#include <CoreFoundation.h>
#endif

//=============================================================================
//	Wrappers for CFRetain and CFRelease that check for NULL
//=============================================================================

template <typename CFObjectType>
CFObjectType CACFRetain(CFObjectType inObject)
{
	CFObjectType theAnswer = inObject;
	if(inObject != NULL)
	{
		theAnswer = reinterpret_cast<CFObjectType>(CFRetain(inObject));
	}
	return theAnswer;
}

inline void	CACFRelease(CFTypeRef inObject)
{
	if(inObject != NULL)
	{
		CFRelease(inObject);
	}
}

//=============================================================================
//	CACFObject
//
//	Notes
//	-	Using the AssignWithoutRetain() method will fool the static analyzer into thinking that the
//		CFObject being assigned will be leaked. This is because the static analyzer is not smart
//		enough to understand that the destructor will release the object.
//=============================================================================

template <class CFObjectType>
class CACFObject
{

//	Construction/Destruction
public:
						CACFObject() : mCFObject(NULL), mWillRelease(true) {}
	explicit			CACFObject(CFObjectType inCFObject) : mCFObject(inCFObject), mWillRelease(true) {}
						CACFObject(CFObjectType inCFObject, bool inWillRelease) : mCFObject(inCFObject), mWillRelease(inWillRelease) {}
						~CACFObject() { Release(); }
						CACFObject(const CACFObject& inObject) : mCFObject(inObject.mCFObject), mWillRelease(inObject.mWillRelease) { Retain(); }
	CACFObject&			operator=(const CACFObject& inObject) { Release(); mCFObject = inObject.mCFObject; mWillRelease = inObject.mWillRelease; Retain(); return *this; }
	CACFObject&			operator=(CFObjectType inCFObject) { Release(); mCFObject = inCFObject; mWillRelease = true; Retain(); return *this; }
	void				AssignWithoutRetain(CFObjectType inObject) { if (inObject != mCFObject) { Release(); mCFObject = inObject; } mWillRelease = true; }

private:
	void				Retain() { if(mWillRelease && (mCFObject != NULL)) { CFRetain(mCFObject); } }
	void				Release() { if(mWillRelease && (mCFObject != NULL)) { CFRelease(mCFObject); mCFObject = NULL; } }
	
	CFObjectType		mCFObject;
	bool				mWillRelease;

//	Operations
public:
	void				AllowRelease() { mWillRelease = true; }
	void				DontAllowRelease() { mWillRelease = false; }
	bool				IsValid() const { return mCFObject != NULL; }
	CFTypeID			GetTypeID() const { return CFGetTypeID(mCFObject); }
	bool				IsEqual(CFObjectType inCFObject) const { return CFEqual(inCFObject, mCFObject) != 0; }

//	Value Access
public:
	CFObjectType		GetCFObject() const { return mCFObject; }
	CFObjectType		CopyCFObject() const { if(mCFObject != NULL) { CFRetain(mCFObject); } return mCFObject; }
	const CFObjectType*	GetPointerToStorage() const	{ return &mCFObject; }

};

typedef	CACFObject<CFBundleRef>			CACFBundle;
typedef	CACFObject<CFPropertyListRef>	CACFPropertyList;
typedef	CACFObject<CFTypeRef>			CACFType;
typedef	CACFObject<CFUUIDRef>			CACFUUID;
typedef	CACFObject<CFURLRef>			CACFURL;

#endif
