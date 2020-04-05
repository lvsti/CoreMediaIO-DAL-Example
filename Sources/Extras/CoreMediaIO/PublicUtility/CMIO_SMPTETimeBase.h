/*
	    File: CMIO_SMPTETimeBase.h
	Abstract: A utility class to assist with a SMPTE time base.
	 Version: 1.2

*/

#if !defined(__CMIO_SMPTETimeBase_h__)
#define __CMIO_SMPTETimeBase_h__

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#include <CoreAudio/CoreAudioTypes.h>

namespace CMIO
{
	typedef UInt32 SMPTE_HMSF;  // nibbles: hhmmssff

	class SMPTETimeBase
	{
	public:
		static UInt32	CalculateSMPTECounterFromHMSFs(const SMPTETime& time);
		static void		CalculateSMPTE_HMSFsFromCounter(SMPTETime& ioTime, bool inKeepTrackOfDaysAsExtraHours);
	};
}
#endif
