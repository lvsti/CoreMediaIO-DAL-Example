/*
	    File: CMIO_DP_DeviceCommands.cpp
	Abstract: n/a
	 Version: 1.2

*/

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Self Include
#include "CMIO_DP_DeviceCommands.h"

// Internal Includes
#include "CMIO_DP_Device.h"

namespace CMIO { namespace DP
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// DeviceCommands
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	ExampleCommand::ExampleCommand(UInt32 exampleCommandInput) :
		Command(kID),
		mExampleCommandState(exampleCommandInput)
	{
	}

	ExampleCommand::~ExampleCommand()
	{
	}

	void ExampleCommand::Execute(Device* device)
	{
		if (device != NULL)
		{
			//device->SetExample(mExampleCommandState);
		}
	}
}}
