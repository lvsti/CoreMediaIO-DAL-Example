/*
     File: CAComponent.cpp
 Abstract: CAComponent.h
  Version: 1.1

*/
#include "CAComponent.h"
#include "CAComponentDescription.h"
#include "CACFDictionary.h"
#include <stdlib.h>
#include "CAAutoDisposer.h"

CAComponent::CAComponent (const AudioComponentDescription& inDesc, CAComponent* next)
	: mManuName(0), mAUName(0), mCompName(0)
{
	mComp = AudioComponentFindNext ((next ? next->Comp() : NULL), &inDesc);
	if (mComp)
		AudioComponentGetDescription(Comp(), &mDesc);
	else
		memcpy (&mDesc, &inDesc, sizeof(AudioComponentDescription));
}

CAComponent::CAComponent (const AudioComponent& comp) 
	: mComp (comp),
	  mManuName(0), 
	  mAUName(0), 
	  mCompName(0)
{
	AudioComponentGetDescription (Comp(), &mDesc);
}

CAComponent::CAComponent (const AudioComponentInstance& compInst) 
	: mComp (NULL), 
	  mManuName(0), 
	  mAUName(0), 
	  mCompName(0)
{ 
	mComp = AudioComponentInstanceGetComponent (compInst);
	AudioComponentGetDescription (Comp(), &mDesc);
}

CAComponent::CAComponent (OSType inType, OSType inSubtype, OSType inManu)
	: mDesc (inType, inSubtype, inManu),
	  mManuName(0), mAUName(0), mCompName(0)
{
	mComp = AudioComponentFindNext (NULL, &mDesc);
	AudioComponentGetDescription (Comp(), &mDesc);
}

CAComponent::~CAComponent ()
{
	Clear();
}

OSStatus		CAComponent::GetVersion (UInt32 &outVersion) const
{
	return AudioComponentGetVersion (mComp, &outVersion);
}

void			CAComponent::Clear ()
{
	if (mManuName) { CFRelease (mManuName); mManuName = 0; }
	if (mAUName) { CFRelease (mAUName);  mAUName = 0; }
	if (mCompName) { CFRelease (mCompName); mCompName = 0; }
}

CAComponent&	CAComponent::operator= (const CAComponent& y)
{
	Clear();

	mComp = y.mComp;
	mDesc = y.mDesc;

	if (y.mManuName) { mManuName = y.mManuName; CFRetain (mManuName); }
	if (y.mAUName) { mAUName = y.mAUName; CFRetain (mAUName); }
	if (y.mCompName) { mCompName = y.mCompName; CFRetain (mCompName); } 

	return *this;
}

void 		CAComponent::SetCompNames () const
{
	if (!mCompName) {
	
		CFStringRef compName;
		OSStatus result = AudioComponentCopyName (Comp(), &compName);
		if (result) return;
		
		const_cast<CAComponent*>(this)->mCompName = compName;
		if (compName)
		{
			CFArrayRef splitStrArray = CFStringCreateArrayBySeparatingStrings(NULL, compName, CFSTR(":"));
			
			// we need to retain these values so the strings are not lost when the array is released
			const_cast<CAComponent*>(this)->mManuName = (CFStringRef)CFArrayGetValueAtIndex(splitStrArray, 0);
            CFRetain(this->mManuName);
			if (CFArrayGetCount(splitStrArray) > 1)
			{
				CFStringRef str = (CFStringRef)CFArrayGetValueAtIndex(splitStrArray, 1);
				
				CFMutableStringRef mstr = CFStringCreateMutableCopy (NULL, CFStringGetLength(str), str);

				// this needs to trim out white space:
				
				CFStringTrimWhitespace (mstr);
			
				const_cast<CAComponent*>(this)->mAUName = mstr;
			} else
				const_cast<CAComponent*>(this)->mAUName = NULL;
			
			CFRelease(splitStrArray);
		}
	}
}

static void	_ShowCF (FILE* file, CFStringRef str)
{
	if (CFGetTypeID(str) != CFStringGetTypeID()) {
		CFShow(str);
		return;
	}

	CFIndex len = CFStringGetLength(str);
	char* chars = (char*)CA_malloc (len * 2); // give us plenty of room for unichar chars
	if (CFStringGetCString (str, chars, len * 2, kCFStringEncodingUTF8))
		fprintf (file, "%s", chars);
	else
		CFShow (str);

	free (chars);
}

void	CAComponent::Print(FILE* file) const
{
	fprintf (file, "CAComponent: %p", Comp());
	if (mManuName) {
		fprintf (file, ", Manu:"); _ShowCF (file, mManuName);
		if (mAUName) fprintf (file, ", Name:"); _ShowCF (file, mAUName);
	}
	fprintf (file, ", "); 
	Desc ().Print(file);
}
