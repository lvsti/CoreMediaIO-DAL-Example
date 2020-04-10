/*
     File: CAProcess.h
 Abstract: Part of CoreAudio Utility Classes
  Version: 1.1

*/
#if !defined(__CAProcess_h__)
#define __CAProcess_h__

//=============================================================================
//	Includes
//=============================================================================

#include <sys/types.h>

//=============================================================================
//	CAProcess
//
//	Wrapper for utilities for dealing with Unix Processes.
//=============================================================================

class CAProcess
{

//	operations
public:
	static bool		ProcessExists(pid_t inPID);
	static pid_t	GetPID();

private:
	static pid_t	sPID;

};

#endif
