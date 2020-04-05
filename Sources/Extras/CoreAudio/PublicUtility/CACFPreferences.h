/*
     File: CACFPreferences.h
 Abstract: Part of CoreAudio Utility Classes
  Version: 1.1

*/
#if !defined(__CACFPreferences_h__)
#define __CACFPreferences_h__

//==================================================================================================
//	Includes
//==================================================================================================

//	System Includes
#if !defined(__COREAUDIO_USE_FLAT_INCLUDES__)
	#include <CoreFoundation/CoreFoundation.h>
#else
	#include <CoreFoundation.h>
#endif

//==================================================================================================
//	CACFPreferences
//==================================================================================================

class CACFPreferences
{

//	Operations
public:
	static CFPropertyListRef	CopyValue(CFStringRef inKey, bool inCurrentUser, bool inCurrentHost);
	static CFStringRef			CopyStringValue(CFStringRef inKey, bool inCurrentUser, bool inCurrentHost);
	static CFNumberRef			CopyNumberValue(CFStringRef inKey, bool inCurrentUser, bool inCurrentHost);
	static CFArrayRef			CopyArrayValue(CFStringRef inKey, bool inCurrentUser, bool inCurrentHost);
	static CFDictionaryRef		CopyDictionaryValue(CFStringRef inKey, bool inCurrentUser, bool inCurrentHost);
	static void					SetValue(CFStringRef inKey, CFPropertyListRef inValue, bool inCurrentUser, bool inCurrentHost, bool inSynchronize);
	static void					DeleteValue(CFStringRef inKey, bool inCurrentUser, bool inCurrentHost, bool inSynchronize);
	static void					Synchronize(bool inCurrentUser, bool inCurrentHost, bool inForce);
	static void					MarkPrefsOutOfDate(bool inCurrentUser, bool inCurrentHost);
	static void					MarkPrefsClean(bool inCurrentUser, bool inCurrentHost);
	static void					SendNotification(CFStringRef inName);

private:
	static bool					ArePrefsOutOfDate(bool inCurrentUser, bool inCurrentHost);
	
	static bool					sAnyUserAnyHostPrefsOutOfDate;
	static bool					sAnyUserCurrentHostPrefsOutOfDate;
	static bool					sCurrentUserAnyHostPrefsOutOfDate;
	static bool					sCurrentUserCurrentHostPrefsOutOfDate;

};

#endif
