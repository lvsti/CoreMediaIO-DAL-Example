/*
     File: CACFDistributedNotification.h
 Abstract: Part of CoreAudio Utility Classes
  Version: 1.1

*/
#if !defined(__CACFDistributedNotification_h__)
#define __CACFDistributedNotification_h__

//==================================================================================================
//	Includes
//==================================================================================================

//	System Includes
#include <CoreAudio/CoreAudioTypes.h>
#include <CoreFoundation/CoreFoundation.h>

//==================================================================================================
//	CACFDistributedNotification
//==================================================================================================

class CACFDistributedNotification
{

//	Operations
public:
	static void		AddObserver(const void* inObserver, CFNotificationCallback inCallback, CFStringRef inName, CFNotificationSuspensionBehavior inSuspensionBehavior = CFNotificationSuspensionBehaviorCoalesce);
	static void		RemoveObserver(const void* inObserver, CFStringRef inName);
	static void		PostNotification(CFStringRef inName, CFDictionaryRef inUserInfo, bool inPostToAllSessions);

};

#endif
