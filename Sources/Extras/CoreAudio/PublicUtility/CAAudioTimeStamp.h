/*
     File: CAAudioTimeStamp.h
 Abstract: Part of CoreAudio Utility Classes
  Version: 1.1
*/
#if !defined(__CAAudioTimeStamp_h__)
#define __CAAudioTimeStamp_h__

//=============================================================================
//	Includes
//=============================================================================

#if !defined(__COREAUDIO_USE_FLAT_INCLUDES__)
	#include <CoreAudio/CoreAudioTypes.h>
#else
	#include <CoreAudioTypes.h>
#endif

#include <string.h>

//=============================================================================
//	CAAudioTimeStamp
//=============================================================================

struct CAAudioTimeStamp
:
	public AudioTimeStamp
{

//	Construction/Destruction
public:
						CAAudioTimeStamp()																{ memset(this, 0, sizeof(AudioTimeStamp)); }
						CAAudioTimeStamp(const AudioTimeStamp& v)										{ memcpy(this, &v, sizeof(AudioTimeStamp)); }
						CAAudioTimeStamp(Float64 inSampleTime)											{ memset(this, 0, sizeof(AudioTimeStamp)); mSampleTime = inSampleTime; mFlags = kAudioTimeStampSampleTimeValid; }
						CAAudioTimeStamp(UInt64 inHostTime)												{ memset(this, 0, sizeof(AudioTimeStamp)); mHostTime = inHostTime; mFlags = kAudioTimeStampHostTimeValid; }
						CAAudioTimeStamp(Float64 inSampleTime, UInt64 inHostTime)						{ memset(this, 0, sizeof(AudioTimeStamp)); mSampleTime = inSampleTime; mHostTime = inHostTime; mFlags = kAudioTimeStampSampleTimeValid | kAudioTimeStampHostTimeValid; }
						CAAudioTimeStamp(Float64 inSampleTime, UInt64 inHostTime, Float64 inRateScalar)	{ memset(this, 0, sizeof(AudioTimeStamp)); mSampleTime = inSampleTime; mHostTime = inHostTime; mRateScalar = inRateScalar; mFlags = kAudioTimeStampSampleTimeValid | kAudioTimeStampHostTimeValid | kAudioTimeStampRateScalarValid; }

//	Assignment
public:
	CAAudioTimeStamp&	operator=(const AudioTimeStamp& v) { memcpy(this, &v, sizeof(AudioTimeStamp)); return *this; }

//	Constants
public:
	static const AudioTimeStamp	kZero;

};

bool		operator<(const AudioTimeStamp& x, const AudioTimeStamp& y);
bool		operator==(const AudioTimeStamp& x, const AudioTimeStamp& y);
inline bool	operator!=(const AudioTimeStamp& x, const AudioTimeStamp& y) { return !(x == y); }
inline bool	operator<=(const AudioTimeStamp& x, const AudioTimeStamp& y) { return (x < y) || (x == y); }
inline bool	operator>=(const AudioTimeStamp& x, const AudioTimeStamp& y) { return !(x < y); }
inline bool	operator>(const AudioTimeStamp& x, const AudioTimeStamp& y) { return !((x < y) || (x == y)); }

#endif
