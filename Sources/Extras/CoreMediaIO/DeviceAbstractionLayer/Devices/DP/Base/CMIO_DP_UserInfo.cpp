/*
	    File: CMIO_DP_UserInfo.cpp
	Abstract: n/a
	 Version: 1.2

*/

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Self Include
#include "CMIO_DP_UserInfo.h"

// Public Utility Includes
#include "CMIO_PropertyAddress.h"

namespace CMIO { namespace DP
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// UserInfo()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void UserInfo::Initialize()
	{
		//	set up the user session status including initializing it
		if (!sIsInitialized)
		{
			PropertyAddress address('user');	//	kCMIOHardwarePropertyUserSessionIsActiveOrHeadless
			CMIOObjectAddPropertyListener(kCMIOObjectSystemObject, &address, UserSessionListener, NULL);
			UserSessionListener(kCMIOObjectSystemObject, 1, &address, NULL);
			sIsInitialized = true;
		}
	}

	void UserInfo::Teardown()
	{
		if (sIsInitialized)
		{
			PropertyAddress address('user');	//	kCMIOHardwarePropertyUserSessionIsActiveOrHeadless
			CMIOObjectRemovePropertyListener(kCMIOObjectSystemObject, &address, UserSessionListener, NULL);
			sIsInitialized = false;
		}
	}
		
	OSStatus UserInfo::UserSessionListener(CMIOObjectID /*objectID*/, UInt32 /*numberAddresses*/, const CMIOObjectPropertyAddress /*addresses*/[], void* /*clientData*/)
	{
		PropertyAddress address('user');	//	kCMIOHardwarePropertyUserSessionIsActiveOrHeadless
		UInt32 userSessionIsActiveOrHeadless = 1;
		UInt32 dataUsed = 0;
		CMIOObjectGetPropertyData(kCMIOObjectSystemObject, &address, 0, NULL, sizeof(UInt32), &dataUsed, &userSessionIsActiveOrHeadless);
		sCurrentUserSessionIsActiveOrHeadless = userSessionIsActiveOrHeadless != 0;
		return 0;
	}

	bool UserInfo::sIsInitialized = false;
	bool UserInfo::sCurrentUserSessionIsActiveOrHeadless = true;
}}
