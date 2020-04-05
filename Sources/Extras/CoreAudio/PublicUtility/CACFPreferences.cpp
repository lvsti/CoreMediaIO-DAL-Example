/*
     File: CACFPreferences.cpp
 Abstract: CACFPreferences.h
  Version: 1.1

*/
//==================================================================================================
//	Includes
//==================================================================================================

//	Self Include
#include "CACFPreferences.h"

//	PublicUtility Includes
#include "CACFDistributedNotification.h"
#include "CADebugMacros.h"

//==================================================================================================
//	CACFPreferences
//==================================================================================================

CFPropertyListRef	CACFPreferences::CopyValue(CFStringRef inKey, bool inCurrentUser, bool inCurrentHost)
{
	//	synchronize to make sure that what's in memory matches what's on disk
	Synchronize(inCurrentUser, inCurrentHost, false);
	
	CFPropertyListRef theAnswer = NULL;
	CFStringRef theUser = inCurrentUser ? kCFPreferencesCurrentUser : kCFPreferencesAnyUser;
	CFStringRef theHost = inCurrentHost ? kCFPreferencesCurrentHost : kCFPreferencesAnyHost;
	
	theAnswer = CFPreferencesCopyValue(inKey, kCFPreferencesAnyApplication, theUser, theHost);
	
	return theAnswer;
}

CFStringRef	CACFPreferences::CopyStringValue(CFStringRef inKey, bool inCurrentUser, bool inCurrentHost)
{
	CFStringRef theAnswer = NULL;
	
	//	get the raw value
	CFPropertyListRef theRawValue = CopyValue(inKey, inCurrentUser, inCurrentHost);
	
	if(theRawValue != NULL)
	{
		//	get it's type ID and make sure it's a CFString
		CFTypeID theTypeID = CFGetTypeID(theRawValue);
		if(theTypeID == CFStringGetTypeID())
		{
			//	cast the value
			theAnswer = static_cast<CFStringRef>(theRawValue);
		}
		else
		{
			CFRelease(theRawValue);
			DebugMessage("CACFPreferences::CopyStringValue: not a CFString");
		}
	}
	
	return theAnswer;
}

CFNumberRef	CACFPreferences::CopyNumberValue(CFStringRef inKey, bool inCurrentUser, bool inCurrentHost)
{
	CFNumberRef theAnswer = NULL;
	
	//	get the raw value
	CFPropertyListRef theRawValue = CopyValue(inKey, inCurrentUser, inCurrentHost);
	
	if(theRawValue != NULL)
	{
		//	get it's type ID and make sure it's a CFNumber
		CFTypeID theTypeID = CFGetTypeID(theRawValue);
		if(theTypeID == CFNumberGetTypeID())
		{
			//	cast the value
			theAnswer = static_cast<CFNumberRef>(theRawValue);
		}
		else
		{
			CFRelease(theRawValue);
			DebugMessage("CACFPreferences::CopyNumberValue: not a CFNumber");
		}
	}
	
	return theAnswer;
}

CFArrayRef	CACFPreferences::CopyArrayValue(CFStringRef inKey, bool inCurrentUser, bool inCurrentHost)
{
	CFArrayRef theAnswer = NULL;
	
	//	get the raw value
	CFPropertyListRef theRawValue = CopyValue(inKey, inCurrentUser, inCurrentHost);
	
	if(theRawValue != NULL)
	{
		//	get it's type ID and make sure it's a CFArray
		CFTypeID theTypeID = CFGetTypeID(theRawValue);
		if(theTypeID == CFArrayGetTypeID())
		{
			//	cast the value
			theAnswer = static_cast<CFArrayRef>(theRawValue);
		}
		else
		{
			CFRelease(theRawValue);
			DebugMessage("CACFPreferences::CopyArrayValue: not a CFArray");
		}
	}
	
	return theAnswer;
}

CFDictionaryRef	CACFPreferences::CopyDictionaryValue(CFStringRef inKey, bool inCurrentUser, bool inCurrentHost)
{
	CFDictionaryRef theAnswer = NULL;
	
	//	get the raw value
	CFPropertyListRef theRawValue = CopyValue(inKey, inCurrentUser, inCurrentHost);
	
	if(theRawValue != NULL)
	{
		//	get it's type ID and make sure it's a CFDictionary
		CFTypeID theTypeID = CFGetTypeID(theRawValue);
		if(theTypeID == CFDictionaryGetTypeID())
		{
			//	cast the value
			theAnswer = static_cast<CFDictionaryRef>(theRawValue);
		}
		else
		{
			CFRelease(theRawValue);
			DebugMessage("CACFPreferences::CopyDictionaryValue: not a CFDictionary");
		}
	}
	
	return theAnswer;
}

void	CACFPreferences::SetValue(CFStringRef inKey, CFPropertyListRef inValue, bool inCurrentUser, bool inCurrentHost, bool inSynchronize)
{
	CFStringRef theUser = inCurrentUser ? kCFPreferencesCurrentUser : kCFPreferencesAnyUser;
	CFStringRef theHost = inCurrentHost ? kCFPreferencesCurrentHost : kCFPreferencesAnyHost;
	CFPreferencesSetValue(inKey, inValue, kCFPreferencesAnyApplication, theUser, theHost);
	
	if(inSynchronize)
	{
		Synchronize(inCurrentUser, inCurrentHost, true);
	}
}

void	CACFPreferences::DeleteValue(CFStringRef inKey, bool inCurrentUser, bool inCurrentHost, bool inSynchronize)
{
	CFStringRef theUser = inCurrentUser ? kCFPreferencesCurrentUser : kCFPreferencesAnyUser;
	CFStringRef theHost = inCurrentHost ? kCFPreferencesCurrentHost : kCFPreferencesAnyHost;
	CFPreferencesSetValue(inKey, NULL, kCFPreferencesAnyApplication, theUser, theHost);
	
	if(inSynchronize)
	{
		Synchronize(theUser, inCurrentHost, true);
	}
}

void	CACFPreferences::Synchronize(bool inCurrentUser, bool inCurrentHost, bool inForce)
{
	if(inForce || ArePrefsOutOfDate(inCurrentUser, inCurrentHost))
	{
		CFStringRef theUser = inCurrentUser ? kCFPreferencesCurrentUser : kCFPreferencesAnyUser;
		CFStringRef theHost = inCurrentHost ? kCFPreferencesCurrentHost : kCFPreferencesAnyHost;
		CFPreferencesSynchronize(kCFPreferencesAnyApplication, theUser, theHost);
		MarkPrefsClean(inCurrentUser, inCurrentHost);
	}
}

void	CACFPreferences::MarkPrefsOutOfDate(bool inCurrentUser, bool inCurrentHost)
{
	if(!inCurrentUser && !inCurrentHost)
	{
		sAnyUserAnyHostPrefsOutOfDate = true;
	}
	else if(!inCurrentUser && inCurrentHost)
	{
		sAnyUserCurrentHostPrefsOutOfDate = true;
	}
	else if(inCurrentUser && !inCurrentHost)
	{
		sCurrentUserAnyHostPrefsOutOfDate = true;
	}
	else if(inCurrentUser && inCurrentHost)
	{
		sCurrentUserCurrentHostPrefsOutOfDate = true;
	}
}

void	CACFPreferences::MarkPrefsClean(bool inCurrentUser, bool inCurrentHost)
{
	if(!inCurrentUser && !inCurrentHost)
	{
		sAnyUserAnyHostPrefsOutOfDate = false;
	}
	else if(!inCurrentUser && inCurrentHost)
	{
		sAnyUserCurrentHostPrefsOutOfDate = false;
	}
	else if(inCurrentUser && !inCurrentHost)
	{
		sCurrentUserAnyHostPrefsOutOfDate = false;
	}
	else if(inCurrentUser && inCurrentHost)
	{
		sCurrentUserCurrentHostPrefsOutOfDate = false;
	}
}

void	CACFPreferences::SendNotification(CFStringRef inName)
{
	CACFDistributedNotification::PostNotification(inName, NULL, true);
}

bool	CACFPreferences::ArePrefsOutOfDate(bool inCurrentUser, bool inCurrentHost)
{
	bool theAnswer = false;
	
	if(!inCurrentUser && !inCurrentHost)
	{
		theAnswer = sAnyUserAnyHostPrefsOutOfDate;
	}
	else if(!inCurrentUser && inCurrentHost)
	{
		theAnswer = sAnyUserCurrentHostPrefsOutOfDate;
	}
	else if(inCurrentUser && !inCurrentHost)
	{
		theAnswer = sCurrentUserAnyHostPrefsOutOfDate;
	}
	else if(inCurrentUser && inCurrentHost)
	{
		theAnswer = sCurrentUserCurrentHostPrefsOutOfDate;
	}
	
	return theAnswer;
}

bool	CACFPreferences::sAnyUserAnyHostPrefsOutOfDate = false;
bool	CACFPreferences::sAnyUserCurrentHostPrefsOutOfDate = false;
bool	CACFPreferences::sCurrentUserAnyHostPrefsOutOfDate = false;
bool	CACFPreferences::sCurrentUserCurrentHostPrefsOutOfDate = false;
