/*
	    File: CMIO_DP_DeviceCommands.h
	Abstract: n/a
	 Version: 1.2

*/

#if !defined(__CMIO_DP_DeviceCommands_h__)
#define __CMIO_DP_DeviceCommands_h__

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Super Class Includes
#include "CMIO_DP_Command.h"

// System Includes
#include <CoreMediaIO/CMIOHardware.h>

namespace CMIO { namespace DP
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// DeviceCommands
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class ExampleCommand : public Command
	{
	// Constants
	public:
		enum
		{
							kID	= 'exam'
		};

	// Construction/Destruction
	public:
							ExampleCommand(UInt32 exampleCommandInput);
		virtual				~ExampleCommand();

	// Operations
	public:
		virtual void		Execute(Device* device);

	private:
		UInt32				mExampleCommandState;
	};
}}
#endif
