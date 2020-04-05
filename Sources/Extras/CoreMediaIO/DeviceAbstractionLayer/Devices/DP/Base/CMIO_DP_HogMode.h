/*
	    File: CMIO_DP_HogMode.h
	Abstract: n/a
	 Version: 1.2

*/

#if !defined(__CMIO_DP_HogMode_h__)
#define __CMIO_DP_HogMode_h__

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Public Utility Includes
#include "CAProcess.h"
#include "CATokenMap.h"

// System Includes
#include <CoreFoundation/CoreFoundation.h>

namespace CMIO { namespace DP
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Types
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class Device;

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// HogMode
	//	Implements tracking hog mode on behalf of a device. Note that it is currently implemented in terms of a global preference and uses CFDistributedNotificationCenter for notifications.
	//	It would be better to have this implemented in a way that avoided the disk, such as by having a message protocol for use with Mach messages.
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class HogMode
	{
	// Construction/Destruction
	public:
							HogMode(Device* device);
		virtual				~HogMode();

	// Operations
	public:
		pid_t				GetOwner() const;
		bool				IsFree() const;
		bool				CurrentProcessIsOwner() const;
		bool				CurrentProcessIsOwnerOrIsFree() const;

		void				Take();
		void				Release();
		void				SetOwner(pid_t owner) { mOwner = owner; }

	// Implementation
	private:
		Device*				GetDevice() const { return mDevice; }
		pid_t				GetOwnerFromPreference(bool sendNotifications) const;
		void				SetOwnerInPreference(pid_t owner) const;
		void				SendHogModeChangedNotification() const;
		static void			ChangeNotification(CFNotificationCenterRef center, const void* hogModeToken, CFStringRef notificationName, const void* object, CFDictionaryRef userInfo);
		
		UInt32				mToken;
		Device*				mDevice;
		CFStringRef			mPrefName;
		pid_t				mOwner;
		
		static CATokenMap<HogMode>*	sTokenMap;
	};
}}
#endif
