/*
	    File: CMIO_CMA_Time.h
	Abstract: Helper items for CMTime
	 Version: 1.2

*/

#if !defined(__CMIO_CMA_Time_h__)
#define __CMIO_CMA_Time_h__

// Public Utility
#include "CMIODebugMacros.h"

// CA Public Utility
#include "CAException.h"

// System Includes
#include <CoreMedia/CMTime.h>

// System Includes

namespace CMIO { namespace CMA
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Time
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class Time
	{
	public:
		static inline void		ConformTimescale(CMTime& time, int32_t timescale)
								{
									if (time.timescale != timescale)
										time = CMTimeConvertScale(time, timescale, kCMTimeRoundingMethod_Default);
								}
	};
}}
#endif
