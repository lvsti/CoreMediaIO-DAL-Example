/*
	    File: CMIODebugMacros.cpp
	Abstract: Debug macros for CMIO and variations of some CADebugMacros.h to extend or correct some of them:

		* Extended
		DebugPrint()
		DebugMessage()
		ThrowIfKernelError()

		* Added
		DebugMessageIf()
		DebugMessageLevelIf()
		DebugMessageIfError()
		DebugMessageLevelIfError()

		* Correct
		FailIfError()

		*** IMPORTANT NOTE : one cannot use both CMIODebugMacros.cpp and CADebugMacros.cpp at the same time, because they have identical function definitions, with the difference
		being the inclusion of a version on DebugPrint().
	
	 Version: 1.2

*/

#include "CMIODebugMacros.h"
#include "CACFDictionary.h"
#include "CACFArray.h"

#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <time.h>
#include <pthread.h>

#if !USE_DTRACE_LOGGING
	static bool sCMIODebug = false;
#endif

static CFDictionaryRef sCMIOPrefsCache = NULL;
static pthread_once_t sInitCMIOPrefsOnce = PTHREAD_ONCE_INIT;

extern "C" void CMIOInitPreferencesOnce();
extern "C" void CMIOInitPreferencesOnce()
{
	// this code only executes once
	
	CACFArray keyArray( CFPreferencesCopyKeyList(CFSTR("com.apple.cmio"), kCFPreferencesCurrentUser, kCFPreferencesAnyHost), true );
	
	if ( keyArray.GetCFArray() )
		sCMIOPrefsCache = CFPreferencesCopyMultiple( keyArray.GetCFArray(), CFSTR("com.apple.cmio"), kCFPreferencesCurrentUser, kCFPreferencesAnyHost);
	
#if !USE_DTRACE_LOGGING
	if ( sCMIOPrefsCache )
	{
		CACFDictionary(sCMIOPrefsCache, false).GetBool(CFSTR("Debug"), sCMIODebug);
	}
#endif
}

#if USE_DTRACE_LOGGING
	// CMIODebug()
	//	Reports true if the DTrace probe com_apple_cmio_CMIODebugMacros is currently enabled.
	//	This MUST be true for any module specific DTracing to be active.
	extern "C" Boolean CMIODebug(void)
	{
		pthread_once(&sInitCMIOPrefsOnce, CMIOInitPreferencesOnce);
		return (Boolean)( CMIOGlobalDTraceProbeIsEnabled() && ( NULL != sCMIOPrefsCache ) );
	}
#else
	// CMIODebug()
	//	Reports true if 'defaults write com.apple.cmio Debug true' has been set via Terminal in the current user account prior to running the application.
	//	This MUST be true for any module specific debuging to be logged.
	extern "C" Boolean CMIODebug(void)
	{
		pthread_once(&sInitCMIOPrefsOnce, CMIOInitPreferencesOnce);
		return (Boolean)sCMIODebug;
	}
#endif

// CMIOModuleDebugLevel()
//	Reports true if 'defaults write com.apple.cmio {moduleKey} {levelValue}' has been set via terminal prior to running the application and true == CMIODebug() and debugLevel <= levelValue.
//	There are two special values that can be used for {levelValue} :
//		-1 == always report true
//		 0 == never report true, 
extern "C" Boolean CMIOModuleDebugLevel(CFStringRef moduleKey, CFIndex debugLevel)
{
	if (CMIODebug())
	{
		// If the key is in the cache and ((0 != cachedDebugLevel and debugLevel <= cachedDebugLevel) or (-1 == cachedDebugLevel)), return true;
		SInt32 cachedDebugLevel = 0;
		
		if ( sCMIOPrefsCache )
			CACFDictionary( sCMIOPrefsCache, false ).GetSInt32(moduleKey, cachedDebugLevel);
			
		return ((0 != cachedDebugLevel and debugLevel <= cachedDebugLevel) or (-1 == cachedDebugLevel));
	}
	
	return false;
}

// CMIOModuleDebugLevelExclusive()
//  Reports true if 'defaults write com.apple.cmio {moduleKey} {levelValue}' has been set via terminal prior to running the application and true == CMIODebug() and debugLevel == levelValue.
extern "C" Boolean CMIOModuleDebugLevelExclusive(CFStringRef moduleKey, CFIndex debugLevel)
{
	if (CMIODebug())
	{
		// If the key is in the cache and ((0 != cachedDebugLevel and debugLevel <= cachedDebugLevel) or (-1 == cachedDebugLevel)), return true;
		SInt32 cachedDebugLevel = 0;
		
		if ( sCMIOPrefsCache )
			CACFDictionary( sCMIOPrefsCache, false ).GetSInt32(moduleKey, cachedDebugLevel);
			
		return (cachedDebugLevel == debugLevel);
	}
	
	return false;
}

// CMIOPreferencesGetBooleanValue()
//  Allow direct access to a boolean value in the cmio prefs (com.apple.cmio.plist).
Boolean CMIOPreferencesGetBooleanValue(CFStringRef key, Boolean *keyExistsAndHasValidFormat)
{
	bool isValid = false;
	bool retVal = false;
	
	CMIODebug();  // force one-time initialization, in case it hasn't been done yet.
	
	if ( sCMIOPrefsCache )
		isValid = CACFDictionary(sCMIOPrefsCache, false).GetBool(key, retVal);
		
	if (keyExistsAndHasValidFormat)
		*keyExistsAndHasValidFormat = (Boolean)isValid;
		
	return retVal;
}


// CMIOPreferencesGetIntegerValue()
//  Allow direct access to an integer value in the cmio prefs (com.apple.cmio.plist).
CFIndex CMIOPreferencesGetIntegerValue(CFStringRef key, Boolean *keyExistsAndHasValidFormat)
{
	bool isValid = false;
	CFIndex retVal = 0; // CFIndex is a signed long, so it's size changes in 64-bit
	
	CMIODebug();  // force one-time initialization, in case it hasn't been done yet.
	
	if ( sCMIOPrefsCache )
	{
		if ( sizeof(CFIndex) == sizeof(SInt32) )
			isValid = CACFDictionary(sCMIOPrefsCache, false).GetSInt32(key, (SInt32&)retVal);
		else if ( sizeof(CFIndex) == sizeof(SInt64) )
			isValid = CACFDictionary(sCMIOPrefsCache, false).GetSInt64(key, (SInt64&)retVal);
	}
		
	if (keyExistsAndHasValidFormat)
		*keyExistsAndHasValidFormat = (Boolean)isValid;
		
	return retVal;
}


// CMIOPreferencesGetIntegerValue()
//  Allow direct access to an integer value in the cmio prefs (com.apple.cmio.plist).
Float32 CMIOPreferencesGetFloat32Value(CFStringRef key, Boolean *keyExistsAndHasValidFormat)
{
	bool isValid = false;
	Float32 retVal = 0.0f;
	
	CMIODebug();  // force one-time initialization, in case it hasn't been done yet.
	
	if ( sCMIOPrefsCache )
		isValid = CACFDictionary(sCMIOPrefsCache, false).GetFloat32(key, retVal);
	
	if (keyExistsAndHasValidFormat)
		*keyExistsAndHasValidFormat = (Boolean)isValid;
	
	return retVal;
}

// CMIOPreferencesGetDictionary()
//  Allow direct access to a dictionary in the cmio prefs (com.apple.cmio.plist).
CFDictionaryRef CMIOPreferencesGetDictionary(CFStringRef key, Boolean *keyExistsAndHasValidFormat)
{
	bool isValid = false;
	CFDictionaryRef retVal = NULL;
	
	CMIODebug();  // force one-time initialization, in case it hasn't been done yet.
	
	if ( sCMIOPrefsCache )
		isValid = CACFDictionary(sCMIOPrefsCache, false).GetDictionary(key, retVal);
		
	if (keyExistsAndHasValidFormat)
		*keyExistsAndHasValidFormat = (Boolean)isValid;
		
	return retVal;
}

// CMIOFILE()
//	A helper which just returns the leaf name from the standard __FILE__ macro
extern "C" const char * CMIOFILE(	const char * inStr	)
{ 
	size_t count = strlen( inStr ); 
	
	while ( ( 0 != count ) && ( '/' != *(inStr + count - 1) ) )
	{
		count--;
	}
	
	return inStr + count;
}

// CMIOAsctime()
//	A helper which removes the trailing \n from the asctime() result
extern "C" char * CMIOAsctime(void)
{
	time_t t = time(NULL);
	char * theTime = asctime( localtime( &t ) );
	// asctime has a \n at the end, let's remove it
	theTime[ strlen(theTime) - 1 ] = '\0';
	return theTime;
}


// DebugPrint()
//	Log a message to the indidicated file if true == CMIOModuleDebugLevel(moduleKey, debugLevel) 
void DebugPrint(FILE* logFile, CFStringRef moduleKey, CFIndex debugLevel, const char *message, ...)
{
	if (CMIOModuleDebugLevel(moduleKey, debugLevel))
	{
		va_list args;
		va_start(args, message);
		vfprintf(logFile, message, args);
		va_end(args);
	}
}

// DebugSysLogPrint()
// Invokes syslog() with the indicated log level if true == CMIOModuleDebugLevel(moduleKey, debugLevel).  Used by CMIODebugPrintf.h's "DebugPrintfRtn" macro.
void DebugSysLogPrint(int logLevel, CFStringRef moduleKey, CFIndex debugLevel, const char *message, ...)
{
	if (CMIOModuleDebugLevel(moduleKey, debugLevel))
	{
		va_list args;
		va_start(args, message);
		vsyslog(logLevel, message, args);
		va_end(args);
	}
}
