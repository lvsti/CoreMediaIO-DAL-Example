/*
	    File: CMIODebugPrintf.h
	Abstract: CoreMediaIO variant of CADebugPrintf.h
	 Version: 1.2

*/

#if !defined(__CMIODebugPrintf_h__)
#define __CMIODebugPrintf_h__

#include "CADebugPrintf.h"

#if (DEBUG || ALWAYS_DO_CMIO_DEBUG_MSG)

	// Undefine these items which were defined in CADebugPrintf.h
	#undef DebugPrintfRtn
	#undef DebugPrintfFile
	#undef DebugPrintfLineEnding
	
	#if	CoreAudio_UseSysLog
		#include <sys/syslog.h>
		#define	DebugPrintfRtn			DebugSysLogPrint
		#define	DebugPrintfFile			LOG_ERR
		#define	DebugPrintfLineEnding	
	#else
		#include <stdio.h>
		#define	DebugPrintfRtn			DebugPrint
		#define DebugPrintfFile			stderr
		#define	DebugPrintfLineEnding	"\n"
	#endif
	

#endif // (DEBUG || ALWAYS_DO_CMIO_DEBUG_MSG)
#endif // !defined(__CMIODebugPrintf_h__)
