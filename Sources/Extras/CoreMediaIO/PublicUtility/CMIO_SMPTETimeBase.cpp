/*
	    File: CMIO_SMPTETimeBase.cpp
	Abstract: A utility class to assist with a SMPTE time base.
	 Version: 1.2

*/

#define kCMIODebugModuleKey	"CMIO_SMPTETimeBase.Debug"

#include "CMIODebugMacros.h"

#define module_d_syslog(...) do { if (CMIOModuleDebug( CFSTR( kCMIODebugModuleKey ) )) cmio_debug_syslog(__VA_ARGS__); } while (0)
#define module_debugmsg(...) do { if (CMIOModuleDebug( CFSTR( kCMIODebugModuleKey ) )) cmio_debug_msg(__VA_ARGS__); } while (0)

//_____________________________________________________________________________

#include "CMIO_SMPTETimeBase.h"

namespace CMIO
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// CalculateSMPTECounterFromHMSFs()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	UInt32 SMPTETimeBase::CalculateSMPTECounterFromHMSFs(const SMPTETime& time)
	{
		UInt32 counter;
		
		switch ( time.mType )
		{
			case kSMPTETimeType24:
				counter =
						((UInt32) (time.mFrames))
					+ ( ((UInt32) (time.mSeconds))	* 24L )
					+ ( ((UInt32) (time.mMinutes))	* 24L * 60L )
					+ ( ((UInt32) (time.mHours))	* 24L * 60L * 60L );
				break;
			
			case kSMPTETimeType25:
				counter =
						((UInt32) (time.mFrames))
					+ ( ((UInt32) (time.mSeconds))	* 25L )
					+ ( ((UInt32) (time.mMinutes))	* 25L * 60L )
					+ ( ((UInt32) (time.mHours))	* 25L * 60L * 60L );
				break;
			
			case kSMPTETimeType2997:
			case kSMPTETimeType30:
				counter =
						((UInt32) (time.mFrames))
					+ ( ((UInt32) (time.mSeconds))	* 30L )
					+ ( ((UInt32) (time.mMinutes))	* 30L * 60L )
					+ ( ((UInt32) (time.mHours))	* 30L * 60L * 60L );
				break;
			
			case kSMPTETimeType2997Drop:
			case kSMPTETimeType30Drop:
				{
					// See http://www.phatnav.com/wiki/wiki.phtml?title=SMPTE_time_code.
					
					UInt32 fullMins =	( (UInt32) (time.mMinutes))
									+ ( ((UInt32) (time.mHours)) * 60L );
					
					counter =
								((UInt32) (time.mFrames))
							+ ( ((UInt32) (time.mSeconds))	* 30L )
							+ ( fullMins					* 30L * 60L )
							- ( (fullMins / 10L) * (2L * 9L) )	// Subtract 18 frames for every ten minutes.
							- ( (fullMins % 10L) * 2L );		// Subtract two frames for every remainder
																//	minute that is not a multiple of 10.
				}
				break;
			
			case kSMPTETimeType60:
			case kSMPTETimeType5994:
				counter =
						((UInt32) (time.mFrames))
					+ ( ((UInt32) (time.mSeconds))	* 60L )
					+ ( ((UInt32) (time.mMinutes))	* 60L * 60L )
					+ ( ((UInt32) (time.mHours))	* 60L * 60L * 60L );
				break;
			
			case 8:	// kSMPTETimeType60Drop:
			case 9:	// kSMPTETimeType5994Drop:
				{
					//Adapted from 30 Drop Frame code above.
					
					UInt32 fullMins =	( (UInt32) (time.mMinutes))
									+ ( ((UInt32) (time.mHours)) * 60L );
					
					counter =
								((UInt32) (time.mFrames))
							+ ( ((UInt32) (time.mSeconds))	* 60L )
							+ ( fullMins					* 60L * 60L )
							- ( (fullMins / 10L) * (4L * 9L) )	// Subtract 36 frames for every ten minutes.
							- ( (fullMins % 10L) * 4L );		// Subtract four frames for every remainder
																//	minute that is not a multiple of 10.
				}
				break;
			
			case 10: // kSMPTETimeType50:
				counter =
						((UInt32) (time.mFrames))
					+ ( ((UInt32) (time.mSeconds))	* 50L )
					+ ( ((UInt32) (time.mMinutes))	* 50L * 60L )
					+ ( ((UInt32) (time.mHours))	* 50L * 60L * 60L );
				break;
				
			default:	
				module_debugmsg( "unsupported SMPTETime.mType = %lu", ((unsigned long int) (time.mType)) );
				throw ((OSStatus) (paramErr));
		}
		
		return counter;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// CalculateSMPTE_HMSFsFromCounter()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void SMPTETimeBase::CalculateSMPTE_HMSFsFromCounter(SMPTETime& ioTime, bool inKeepTrackOfDaysAsExtraHours)
	{
		SInt32	counter = ioTime.mCounter;
		
		// Make sure that we are not going to go over one day.  We have to adjust for drop frame where necessary.
		
		SInt32	countsPerFrame;
		SInt32	countsInOneDay;
		SInt32	days;
		
		switch ( ioTime.mType )
		{
			case kSMPTETimeType24:
				countsPerFrame = 24L;
				break;
			
			case kSMPTETimeType25:
				countsPerFrame = 25L;
				break;
			
			case kSMPTETimeType2997:
			case kSMPTETimeType2997Drop:
			case kSMPTETimeType30:
			case kSMPTETimeType30Drop:
				countsPerFrame = 30L;
				break;
			
			case kSMPTETimeType60:
			case kSMPTETimeType5994:
			case 8: // kSMPTETimeType60Drop:
			case 9: // kSMPTETimeType5994Drop:
				countsPerFrame = 60L;
				break;
			
			case 10: // kSMPTETimeType50:
				countsPerFrame = 50L;
				break;
				
			default:
				module_debugmsg( "unsupported SMPTETime.mType = %lu", ((unsigned long int) (ioTime.mType)) );
				throw ((OSStatus) (paramErr));
		}
		
		countsInOneDay = countsPerFrame * 60L * 60L * 24L;
		
		if (	( kSMPTETimeType2997Drop ==	ioTime.mType )
			 || ( kSMPTETimeType30Drop ==	ioTime.mType ) )
		{
			countsInOneDay -= ( 18L * 6L * 24L );	// We drop 18 frames every ten minutes.
		}
		else if (	( 8 /* kSMPTETimeType5994Drop */ ==	ioTime.mType )
				 || ( 9 /* kSMPTETimeType60Drop */ ==	ioTime.mType ) )
		{
			countsInOneDay -= ( 36L * 6L * 24L );	// We drop 36 frames every ten minutes.
		}
		
		days = counter / countsInOneDay;
		counter = counter - ( days * countsInOneDay );
		
		if ( !inKeepTrackOfDaysAsExtraHours )
		{
			days = 0;
		}
		
		// Adjust the counter if we are doing drop-frame, so that we can
		// pretend that its really a true 30FPS when computing the timecode.
		
		if (	( kSMPTETimeType2997Drop ==	ioTime.mType )
			 || ( kSMPTETimeType30Drop ==	ioTime.mType ) )
		{
			SInt32	framesInOneMinute =	30L * 60L;	// Roughly speaking, that is.
			SInt32	ntscFramesInTenMinutes;
			SInt32	numTenMinChunks;
			SInt32	remainderFrames;
			SInt32	minutes;
			SInt32	minuteAdjustment;
			
			// Compute the number of frames in ten minutes of NTSC video (two
			//	frames are dropped every minute that is not divisible by 10).
			
			ntscFramesInTenMinutes = ( framesInOneMinute * 10L ) - ( 2L * 9L );
			
			// Compute number of ten minute chunks.
			
			numTenMinChunks = counter / ntscFramesInTenMinutes;
			
			// Compute the number of frames in the remainder.
			
			remainderFrames = counter - ( numTenMinChunks * ntscFramesInTenMinutes );
			
			// Compute number of minutes that the remainder corresponds to
			// (we drop two frames per minute).
			
			minutes = remainderFrames / ( framesInOneMinute - 2L );
			
			// The above computation of minutes assumes that every minute in
			// the remainder has two frames dropped, but this is not true;
			// the first minute of the remainder has no frames dropped, so
			// we might need to make an adjustment.
			
			if ( minutes == 0 )
			{
				minuteAdjustment = 0;	// The easy case, we are dead-on ten minutes.
			}
			else
			{
				if ( minutes >= 10 )
				{
					minuteAdjustment = 9L * 2L;	// Right before rolling over to next
												//	ten minute chunk, so its 18 frames.
				}
				else
				{
					// This following bit of code is pretty obtuse, but it
					// actually works, and if you stare at it long enough,
					// you can convince yourself that its correct.  It decides
					// whether or not to drop 2 frames for every minute
					// calculated above, or if we need to subtract one from
					// the calculation because we are being affected by the
					// fact that the first minute has no drops.
					
					switch ( remainderFrames % ( framesInOneMinute - 2L ) )
					{
						case 0:
						case 1:
							minuteAdjustment = ( minutes - 1L ) * 2L;
							break;
						
						default:
							minuteAdjustment = minutes * 2L;
					}
				}
			}
			
			// Add 18 frames for each ten minutes, plus 2 frames for each minute beyond,
			//	except for the first minute beyond.
			
			counter += ( ( numTenMinChunks * 2L * 9L ) + minuteAdjustment );
		}
		else if (	( 8 /* kSMPTETimeType5994Drop */ ==	ioTime.mType )
				 || ( 9 /* kSMPTETimeType60Drop */ ==	ioTime.mType ) )
		{
			SInt32	framesInOneMinute =	60L * 60L;	// Roughly speaking, that is.
			SInt32	ntscFramesInTenMinutes;
			SInt32	numTenMinChunks;
			SInt32	remainderFrames;
			SInt32	minutes;
			SInt32	minuteAdjustment;
			
			// Compute the number of frames in ten minutes of NTSC video (four
			//	frames are dropped every minute that is not divisible by 10).
			
			ntscFramesInTenMinutes = ( framesInOneMinute * 10L ) - ( 4L * 9L );
			
			// Compute number of ten minute chunks.
			
			numTenMinChunks = counter / ntscFramesInTenMinutes;
			
			// Compute the number of frames in the remainder.
			
			remainderFrames = counter - ( numTenMinChunks * ntscFramesInTenMinutes );
			
			// Compute number of minutes that the remainder corresponds to
			// (we drop four frames per minute).
			
			minutes = remainderFrames / ( framesInOneMinute - 4L );
			
			// The above computation of minutes assumes that every minute in
			// the remainder has two frames dropped, but this is not true;
			// the first minute of the remainder has no frames dropped, so
			// we might need to make an adjustment.
			
			if ( minutes == 0 )
			{
				minuteAdjustment = 0;	// The easy case, we are dead-on ten minutes.
			}
			else
			{
				if ( minutes >= 10 )
				{
					minuteAdjustment = 9L * 4L;	// Right before rolling over to next
												//	ten minute chunk, so its 36 frames.
				}
				else
				{
					// This following bit of code is pretty obtuse, but it
					// actually works, and if you stare at it long enough,
					// you can convince yourself that its correct.  It decides
					// whether or not to drop 4 frames for every minute
					// calculated above, or if we need to subtract one from
					// the calculation because we are being affected by the
					// fact that the first minute has no drops.
					
					switch ( remainderFrames % ( framesInOneMinute - 4L ) )
					{
						case 0:
						case 1:
						case 2:
						case 3:
							minuteAdjustment = ( minutes - 1L ) * 4L;
							break;
						
						default:
							minuteAdjustment = minutes * 4L;
					}
				}
			}
			
			// Add 36 frames for each ten minutes, plus 4 frames for each minute beyond,
			//	except for the first minute beyond.
			
			counter += ( ( numTenMinChunks * 4L * 9L ) + minuteAdjustment );
		}
		
		// Compute the fields.
		
		ioTime.mFrames	= counter % countsPerFrame;
		ioTime.mSeconds	= ( counter / countsPerFrame ) % 60L;
		ioTime.mMinutes	= ( counter / ( countsPerFrame * 60L ) ) % 60L;
		ioTime.mHours	= ( ( counter / ( countsPerFrame * 60L * 60L ) ) % 24L ) + ( days * 24L );
	}
}
