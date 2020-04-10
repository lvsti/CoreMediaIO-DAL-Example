/*
     File: CAAudioTimeStamp.cpp
 Abstract: CAAudioTimeStamp.h
  Version: 1.1
*/
//=============================================================================
//	Includes
//=============================================================================

#include "CAAudioTimeStamp.h"

//=============================================================================
//	CAAudioTimeStamp
//=============================================================================

const AudioTimeStamp	CAAudioTimeStamp::kZero = { 0.0, 0, 0.0, 0, { 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 0, 0 };

bool	operator<(const AudioTimeStamp& x, const AudioTimeStamp& y)
{
	bool	isLessThan = false;
	bool	isDone = false;
	
	//	check the sample time
	if(!isDone)
	{
		if((x.mFlags & kAudioTimeStampSampleTimeValid) && (y.mFlags & kAudioTimeStampSampleTimeValid))
		{
			isLessThan = x.mSampleTime < y.mSampleTime;
			isDone = true;
		}
	}
	
	//	check the host time
	if(!isDone)
	{
		if((x.mFlags & kAudioTimeStampHostTimeValid) && (y.mFlags & kAudioTimeStampHostTimeValid))
		{
			isLessThan = x.mHostTime < y.mHostTime;
			isDone = true;
		}
	}
	
	//	check the word clock time
	if(!isDone)
	{
		if((x.mFlags & kAudioTimeStampWordClockTimeValid) && (y.mFlags & kAudioTimeStampWordClockTimeValid))
		{
			isLessThan = x.mWordClockTime < y.mWordClockTime;
			//	commented out to prevent this from being flagged as a dead store by the static analyzer
			//isDone = true;
		}
	}
	
	return isLessThan;
}

bool	operator==(const AudioTimeStamp& x, const AudioTimeStamp& y)
{
	bool	isEqual = false;
	bool	isDone = false;
	
	//	check the sample time
	if(!isDone)
	{
		if((x.mFlags & kAudioTimeStampSampleTimeValid) && (y.mFlags & kAudioTimeStampSampleTimeValid))
		{
			isEqual = x.mSampleTime == y.mSampleTime;
			isDone = true;
		}
	}
	
	//	check the host time
	if(!isDone)
	{
		if((x.mFlags & kAudioTimeStampHostTimeValid) && (y.mFlags & kAudioTimeStampHostTimeValid))
		{
			isEqual = x.mHostTime == y.mHostTime;
			isDone = true;
		}
	}
	
	//	check the word clock time
	if(!isDone)
	{
		if((x.mFlags & kAudioTimeStampWordClockTimeValid) && (y.mFlags & kAudioTimeStampWordClockTimeValid))
		{
			isEqual = x.mWordClockTime == y.mWordClockTime;
			//	commented out to prevent this from being flagged as a dead store by the static analyzer
			//isDone = true;
		}
	}
	
	return isEqual;
}
