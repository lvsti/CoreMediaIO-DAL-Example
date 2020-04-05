/*
	    File: CMIO_DP_UserInfo.h
	Abstract: n/a
	 Version: 1.2

*/

#if !defined(__CMIO_DP_UserInfo_h__)
#define __CMIO_DP_UserInfo_h__

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// System Includes
#include <CoreMediaIO/CMIOHardware.h>

namespace CMIO { namespace DP
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//	UserInfo
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class UserInfo
	{
	// Construction/Destruction
	public:
		static void		Initialize();
		static void		Teardown();

	// Operations
	public:
		static bool		IsCurrentUserSessionActiveOrHeadless() { return sCurrentUserSessionIsActiveOrHeadless; }

	// Implementation
	private:
		static OSStatus	UserSessionListener(CMIOObjectID objectID, UInt32 numberAddresses, const CMIOObjectPropertyAddress addresses[], void* clientData);

		static bool		sIsInitialized;
		static bool		sCurrentUserSessionIsActiveOrHeadless;
	};
}}
#endif
