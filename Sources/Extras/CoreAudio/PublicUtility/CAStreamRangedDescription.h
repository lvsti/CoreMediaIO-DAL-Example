/*
     File: CAStreamRangedDescription.h
 Abstract: Part of CoreAudio Utility Classes
  Version: 1.1

*/
#if !defined(__CAStreamRangedDescription_h__)
#define __CAStreamRangedDescription_h__

//==================================================================================================
//	Includes
//==================================================================================================

//  Super Class Includes
#include <CoreAudio/AudioHardware.h>

//  PublicUtility Includes
#include "CAAudioValueRange.h"
#include "CAStreamBasicDescription.h"

//==================================================================================================
//	CAStreamRangedDescription
//==================================================================================================

class CAStreamRangedDescription
:
	public AudioStreamRangedDescription
{

//	Constants
public:
	static const AudioStreamRangedDescription	sEmpty;

//	Construction/Destruction
public:
								CAStreamRangedDescription()																							{ memset(this, 0, sizeof(AudioStreamRangedDescription)); }
								CAStreamRangedDescription(const CAStreamRangedDescription& inFormat)												{ mFormat = inFormat.mFormat; mSampleRateRange = inFormat.mSampleRateRange; }
								CAStreamRangedDescription(const AudioStreamRangedDescription& inFormat)												{ mFormat = inFormat.mFormat; mSampleRateRange = inFormat.mSampleRateRange; }
								CAStreamRangedDescription(const AudioStreamBasicDescription& inFormat)												{ mFormat = inFormat; mSampleRateRange.mMinimum = inFormat.mSampleRate; mSampleRateRange.mMaximum = inFormat.mSampleRate; }
								CAStreamRangedDescription(const AudioStreamBasicDescription& inFormat, const AudioValueRange& inSampleRateRange)	{ mFormat = inFormat; mSampleRateRange = inSampleRateRange; }
	CAStreamRangedDescription&  operator=(const CAStreamRangedDescription& inFormat)																{ mFormat = inFormat.mFormat; mSampleRateRange = inFormat.mSampleRateRange; return *this; }
	CAStreamRangedDescription&  operator=(const AudioStreamRangedDescription& inFormat)																{ mFormat = inFormat.mFormat; mSampleRateRange = inFormat.mSampleRateRange; return *this; }

	static bool					IsMixable(const AudioStreamRangedDescription& inDescription) { return (inDescription.mFormat.mFormatID == kAudioFormatLinearPCM) && ((inDescription.mFormat.mFormatFlags & kIsNonMixableFlag) == 0); }
#if CoreAudio_Debug
	static void					PrintToLog(const AudioStreamRangedDescription& inDesc);
#endif
	static bool					Sorter(const AudioStreamRangedDescription& x, const AudioStreamRangedDescription& y);

};

inline bool operator<(const AudioStreamRangedDescription& x, const AudioStreamRangedDescription& y)													{	return	(x.mFormat.mFormatID == y.mFormat.mFormatID) ? 
																																								(x.mFormat < y.mFormat) && (x.mSampleRateRange < y.mSampleRateRange) :
																																								(x.mFormat.mFormatID == kAudioFormatLinearPCM) ? false : (x.mFormat.mFormatID < y.mFormat.mFormatID); }
inline bool operator==(const AudioStreamRangedDescription& x, const AudioStreamRangedDescription& y)												{ return (x.mFormat == y.mFormat) && (x.mSampleRateRange == y.mSampleRateRange); }
#if TARGET_OS_MAC || (TARGET_OS_WIN32 && (_MSC_VER > 600))
inline bool	operator!=(const AudioStreamRangedDescription& x, const AudioStreamRangedDescription& y)												{ return !(x == y); }
inline bool	operator<=(const AudioStreamRangedDescription& x, const AudioStreamRangedDescription& y)												{ return (x < y) || (x == y); }
inline bool	operator>=(const AudioStreamRangedDescription& x, const AudioStreamRangedDescription& y)												{ return !(x < y); }
inline bool	operator>(const AudioStreamRangedDescription& x, const AudioStreamRangedDescription& y)													{ return !((x < y) || (x == y)); }
#endif

inline bool operator<(const AudioStreamBasicDescription& x, const AudioStreamRangedDescription& y)													{	return	(x.mFormatID == y.mFormat.mFormatID) ? 
																																								(x < y.mFormat) && (x.mSampleRate < y.mSampleRateRange.mMinimum) :
																																								(x.mFormatID == kAudioFormatLinearPCM) ? false : (x.mFormatID < y.mFormat.mFormatID); }
inline bool operator<(const AudioStreamRangedDescription& x, const AudioStreamBasicDescription& y)													{	return	(x.mFormat.mFormatID == y.mFormatID) ? 
																																								(x.mFormat < y) && (x.mSampleRateRange.mMinimum < y.mSampleRate) :
																																								(x.mFormat.mFormatID == kAudioFormatLinearPCM) ? false : (x.mFormat.mFormatID < y.mFormatID); }
inline bool operator==(const AudioStreamBasicDescription& x, const AudioStreamRangedDescription& y)													{ return (x == y.mFormat) && CAAudioValueRange::ContainsValue(y.mSampleRateRange, x.mSampleRate); }
inline bool operator==(const AudioStreamRangedDescription& x, const AudioStreamBasicDescription& y)													{ return (x.mFormat == y) && CAAudioValueRange::ContainsValue(x.mSampleRateRange, y.mSampleRate); }
#if TARGET_OS_MAC || (TARGET_OS_WIN32 && (_MSC_VER > 600))
inline bool	operator!=(const AudioStreamBasicDescription& x, const AudioStreamRangedDescription& y)													{ return !(x == y); }
inline bool	operator!=(const AudioStreamRangedDescription& x, const AudioStreamBasicDescription& y)													{ return !(x == y); }
inline bool	operator<=(const AudioStreamBasicDescription& x, const AudioStreamRangedDescription& y)													{ return (x < y) || (x == y); }
inline bool	operator<=(const AudioStreamRangedDescription& x, const AudioStreamBasicDescription& y)													{ return (x < y) || (x == y); }
inline bool	operator>=(const AudioStreamBasicDescription& x, const AudioStreamRangedDescription& y)													{ return !(x < y); }
inline bool	operator>=(const AudioStreamRangedDescription& x, const AudioStreamBasicDescription& y)													{ return !(x < y); }
inline bool	operator>(const AudioStreamBasicDescription& x, const AudioStreamRangedDescription& y)													{ return !((x < y) || (x == y)); }
inline bool	operator>(const AudioStreamRangedDescription& x, const AudioStreamBasicDescription& y)													{ return !((x < y) || (x == y)); }
#endif

//	STL Functors
struct	CAStreamRangedDescription_EqualToASBD
:
	public std::unary_function<AudioStreamRangedDescription, bool>
{
			CAStreamRangedDescription_EqualToASBD(const AudioStreamBasicDescription& inFormat)	: mFormat(inFormat) {}
	bool	operator()(const AudioStreamRangedDescription& x) const								{ return mFormat == x; }
	
	AudioStreamBasicDescription	mFormat;
};

struct	CAStreamRangedDescription_ReverseSort
:
	public std::binary_function<AudioStreamRangedDescription, AudioStreamRangedDescription, bool>
{
	bool	operator()(const AudioStreamRangedDescription& x, const AudioStreamRangedDescription& y) const	{ return CAStreamRangedDescription::Sorter(x, y); }
};

#endif
