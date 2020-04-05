/*
     File: CAProcess.cpp
 Abstract: CAProcess.h
  Version: 1.1

*/
//=============================================================================
//	Includes
//=============================================================================

#include "CAProcess.h"
#include <signal.h>
#include <unistd.h>
#include <errno.h>

//=============================================================================
//	CAProcess
//=============================================================================

bool	CAProcess::ProcessExists(pid_t inPID)
{
	//	pids <= 0 are reserved for special purposes and -1 is
	//	used as a sentinel value by the HAL.
	bool theAnswer = inPID > 0;
	
	if(theAnswer)
	{
		//	according to kill(2), the process exists if kill(pid, 0) returns 0
		int wasKilled = kill(inPID, 0);
		if(wasKilled != 0)
		{
			//	The kill call failed for some reason, but there is only one error code that
			//	that indicates that the process doesn't exist.
			theAnswer = errno != ESRCH;
		}
	}

	
	return theAnswer;
}

pid_t	CAProcess::GetPID()
{
	if(sPID == -1)
	{
		sPID = getpid();
	}
	
	return sPID;
}

pid_t	CAProcess::sPID = -1;
