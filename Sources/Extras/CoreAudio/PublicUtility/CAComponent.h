/*
     File: CAComponent.h
 Abstract: Part of CoreAudio Utility Classes
  Version: 1.1

*/
#ifndef __CAComponent_h__
#define __CAComponent_h__

#if !defined(__COREAUDIO_USE_FLAT_INCLUDES__)
#else
	#include <ConditionalMacros.h>
#endif

#include "CAComponentDescription.h"

class CAComponent 
{
public:
	CAComponent ()
		: mComp (0), mDesc(), mManuName(0), mAUName(0), mCompName(0) {}
		
		// if next is specifed that is used to find the next component after that one
	CAComponent (const AudioComponentDescription& inDesc, CAComponent* next = 0);
	
	CAComponent (const CAComponent& y) 
		: mComp (0), mDesc(), mManuName(0), mAUName(0), mCompName(0) { *this = y; }

	CAComponent (const AudioComponent& comp);
	
	CAComponent (const AudioComponentInstance& compInst);

	CAComponent (OSType inType, OSType inSubtype = 0, OSType inManu = 0);
	
	~CAComponent ();
	
	CAComponent&	operator= (const CAComponent& y);
	
		// returns true if this object references a valid component
	bool			IsValid () const { return Comp() != 0; }
	
	bool			HasAUStrings() const { 	SetCompNames (); return mManuName != 0; }

		// CFStringRef should be retained by caller if needed beyond lifetime of this object
		
		// Can return NULL if component doesn't follow AU naming conventions
	CFStringRef		GetAUManu () const { SetCompNames (); return mManuName; }
	CFStringRef		GetAUName () const { SetCompNames (); return mAUName ? mAUName : mCompName; }
		
		// Return value of NULL indicates a problem getting that information from the component
	CFStringRef		GetCompName () const { SetCompNames(); return mCompName; }
	
	const CAComponentDescription&	Desc () const { return mDesc; }
			
	OSStatus		Open (AudioComponentInstance& outInst) const 
	{
		return AudioComponentInstanceNew (Comp(), &outInst);
  	}

	OSStatus			GetVersion (UInt32 &outVersion) const;
	
	const AudioComponent&		Comp() const { return mComp; }
	
	void				Print(FILE* file = stdout) const;

	OSStatus			Save (CFPropertyListRef *outData) const;
		
	OSStatus			Restore (CFPropertyListRef &inData);
	
private:
	AudioComponent mComp;
	CAComponentDescription mDesc;
	
	CFStringRef mManuName, mAUName, mCompName;

	void	SetCompNames () const;
	void	SetCompInfo () const;
	void	Clear ();
};

#endif
