/*
     File: CACFDistributedNotification.cpp
 Abstract: CACFDistributedNotification.h
  Version: 1.1

*/
//==================================================================================================
//	Includes
//==================================================================================================

//	Self Include
#include "CACFDistributedNotification.h"

//	PublicUtility Includes
#include "CADebugMacros.h"

//==================================================================================================
//	CACFDistributedNotification
//==================================================================================================

void	CACFDistributedNotification::AddObserver(const void* inObserver, CFNotificationCallback inCallback, CFStringRef inName, CFNotificationSuspensionBehavior inSuspensionBehavior)
{
#if	!TARGET_OS_IPHONE
	CFNotificationCenterRef theCenter = CFNotificationCenterGetDistributedCenter();
	CFNotificationSuspensionBehavior theSuspensionBehavior = inSuspensionBehavior;
#else
	#pragma unused(inSuspensionBehavior)
	CFNotificationCenterRef theCenter = CFNotificationCenterGetDarwinNotifyCenter();
	CFNotificationSuspensionBehavior theSuspensionBehavior = static_cast<CFNotificationSuspensionBehavior>(0);
#endif
	 
	CFNotificationCenterAddObserver(theCenter, inObserver, inCallback, inName, NULL, theSuspensionBehavior);
}

void	CACFDistributedNotification::RemoveObserver(const void* inObserver, CFStringRef inName)
{
#if	!TARGET_OS_IPHONE
	CFNotificationCenterRef theCenter = CFNotificationCenterGetDistributedCenter();
#else
	CFNotificationCenterRef theCenter = CFNotificationCenterGetDarwinNotifyCenter();
#endif
	 
	CFNotificationCenterRemoveObserver(theCenter, inObserver, inName, NULL);
}

void	CACFDistributedNotification::PostNotification(CFStringRef inName, CFDictionaryRef inUserInfo, bool inPostToAllSessions)
{
#if	!TARGET_OS_IPHONE
	CFNotificationCenterRef theCenter = CFNotificationCenterGetDistributedCenter();
	CFDictionaryRef theUserInfo = inUserInfo;
	CFOptionFlags theFlags = kCFNotificationDeliverImmediately;
	if(inPostToAllSessions)
	{
		theFlags += kCFNotificationPostToAllSessions;
	}
#else
	//	flag unsupported features
	Assert(inUserInfo == NULL, "CACFDistributedNotification::PostNotification: distributed notifications do not support a payload");
	Assert(inPostToAllSessions, "CACFDistributedNotification::PostNotification: distributed notifications do not support per-session delivery");
	
	CFNotificationCenterRef theCenter = CFNotificationCenterGetDarwinNotifyCenter();
	CFDictionaryRef theUserInfo = NULL;
	CFOptionFlags theFlags = 0;
#endif
	 
	 CFNotificationCenterPostNotificationWithOptions(theCenter, inName, NULL, theUserInfo, theFlags);
}
