/*
	    File: CMIO_DPA_Sample_Server_Common.h
	Abstract: Items common to server portion of the Assistant.
	 Version: 1.2

*/

#if !defined(__CMIO_DPA_Sample_Server_Common_h__)
#define __CMIO_DPA_Sample_Server_Common_h__

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Public Utility Includes
#include "CMIO_PropertyAddress.h"

// System Includes
#include <mach/port.h>

// Standard Library Includes
#include <set>
#include <map>

namespace CMIO { namespace DPA { namespace Sample { namespace Server
{
		typedef mach_port_t			Client;						// Port on which client sends message to Assistant
		typedef std::set<Client>	Clients;					// Set of clients 

		// PropertyShadow
		struct PropertyShadow
		{
					PropertyShadow() : mShadowTime(0) {};
			UInt64	mShadowTime;
		};

		typedef std::map<PropertyAddress, PropertyShadow, PropertyAddress::LessThan> Properties;

		// ControlShadow
		struct ControlShadow
		{
					ControlShadow() : mValueShadowTime(0), mRangeShadowTime(0) {};
			UInt64	mValueShadowTime;
			UInt64	mRangeShadowTime;
		};

		typedef std::map<UInt32, ControlShadow> Controls;
}}}}

#endif
