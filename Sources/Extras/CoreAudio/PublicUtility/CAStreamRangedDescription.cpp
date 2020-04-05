/*
     File: CAStreamRangedDescription.cpp
 Abstract: CAStreamRangedDescription.h
  Version: 1.1

*/
//==================================================================================================
//	Includes
//==================================================================================================

//	Self Include
#include "CAStreamRangedDescription.h"
#include "CAMath.h"

//==================================================================================================
//	CAStreamRangedDescription
//==================================================================================================

const AudioStreamRangedDescription	CAStreamRangedDescription::sEmpty = { { 0.0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0.0, 0.0 } };

#if CoreAudio_Debug
#include "CALogMacros.h"

void	CAStreamRangedDescription::PrintToLog(const AudioStreamRangedDescription& inDesc)
{
	PrintFloat		("  Sample Rate:        ", inDesc.mFormat.mSampleRate);
	PrintFloat		("  Max Sample Rate:    ", inDesc.mSampleRateRange.mMaximum);
	PrintFloat		("  Min Sample Rate:    ", inDesc.mSampleRateRange.mMinimum);
	Print4CharCode	("  Format ID:          ", inDesc.mFormat.mFormatID);
	PrintHex		("  Format Flags:       ", inDesc.mFormat.mFormatFlags);
	PrintInt		("  Bytes per Packet:   ", inDesc.mFormat.mBytesPerPacket);
	PrintInt		("  Frames per Packet:  ", inDesc.mFormat.mFramesPerPacket);
	PrintInt		("  Bytes per Frame:    ", inDesc.mFormat.mBytesPerFrame);
	PrintInt		("  Channels per Frame: ", inDesc.mFormat.mChannelsPerFrame);
	PrintInt		("  Bits per Channel:   ", inDesc.mFormat.mBitsPerChannel);
}
#endif

bool	CAStreamRangedDescription::Sorter(const AudioStreamRangedDescription& x, const AudioStreamRangedDescription& y)
{
	bool theAnswer = false;
	bool isDone = false;
	
	//	note that if either side is 0, that field is skipped
	
	//	format ID is the first order sort
	if((!isDone) && ((x.mFormat.mFormatID != 0) && (y.mFormat.mFormatID != 0)))
	{
		if(x.mFormat.mFormatID != y.mFormat.mFormatID)
		{
			//	formats are sorted numerically except that linear
			//	PCM is always first
			if(x.mFormat.mFormatID == kAudioFormatLinearPCM)
			{
				theAnswer = true;
			}
			else if(y.mFormat.mFormatID == kAudioFormatLinearPCM)
			{
				theAnswer = false;
			}
			else
			{
				theAnswer = x.mFormat.mFormatID < y.mFormat.mFormatID;
			}
			isDone = true;
		}
	}
	
	
	//  mixable is always better than non-mixable for linear PCM and should be the second order sort item
	if((!isDone) && ((x.mFormat.mFormatID == kAudioFormatLinearPCM) && (y.mFormat.mFormatID == kAudioFormatLinearPCM)))
	{
		if(((x.mFormat.mFormatFlags & kIsNonMixableFlag) == 0) && ((y.mFormat.mFormatFlags & kIsNonMixableFlag) != 0))
		{
			theAnswer = true;
			isDone = true;
		}
		else if(((x.mFormat.mFormatFlags & kIsNonMixableFlag) != 0) && ((y.mFormat.mFormatFlags & kIsNonMixableFlag) == 0))
		{
			theAnswer = false;
			isDone = true;
		}
	}
	
	//	floating point vs integer for linear PCM only
	if((!isDone) && ((x.mFormat.mFormatID == kAudioFormatLinearPCM) && (y.mFormat.mFormatID == kAudioFormatLinearPCM)))
	{
		if((x.mFormat.mFormatFlags & kAudioFormatFlagIsFloat) != (y.mFormat.mFormatFlags & kAudioFormatFlagIsFloat))
		{
			//	floating point is better than integer
			theAnswer = y.mFormat.mFormatFlags & kAudioFormatFlagIsFloat;
			isDone = true;
		}
	}
	
	//	bit depth
	if((!isDone) && ((x.mFormat.mBitsPerChannel != 0) && (y.mFormat.mBitsPerChannel != 0)))
	{
		if(x.mFormat.mBitsPerChannel != y.mFormat.mBitsPerChannel)
		{
			//	deeper bit depths are higher quality
			theAnswer = x.mFormat.mBitsPerChannel > y.mFormat.mBitsPerChannel;
			isDone = true;
		}
	}
	
	//	sample rate range
	if((!isDone) && fnonzero(x.mSampleRateRange.mMinimum) && fnonzero(x.mSampleRateRange.mMaximum) && fnonzero(y.mSampleRateRange.mMinimum) && fnonzero(y.mSampleRateRange.mMaximum))
	{
		if(x.mSampleRateRange != y.mSampleRateRange)
		{
			//	higher sample rates are higher quality
			theAnswer = x.mSampleRateRange > y.mSampleRateRange;
			isDone = true;
		}
	}
	
	//	sample rate
	if((!isDone) && fnonzero(x.mFormat.mSampleRate) && fnonzero(y.mFormat.mSampleRate))
	{
		if(fnotequal(x.mFormat.mSampleRate, y.mFormat.mSampleRate))
		{
			//	higher sample rates are higher quality
			theAnswer = x.mFormat.mSampleRate > y.mFormat.mSampleRate;
			isDone = true;
		}
	}
	
	//	number of channels
	if((!isDone) && ((x.mFormat.mChannelsPerFrame != 0) && (y.mFormat.mChannelsPerFrame != 0)))
	{
		if(x.mFormat.mChannelsPerFrame != y.mFormat.mChannelsPerFrame)
		{
			//	more channels is higher quality
			theAnswer = x.mFormat.mChannelsPerFrame < y.mFormat.mChannelsPerFrame;
			//	commented out to prevent this from being flagged as a dead store by the static analyzer
			//isDone = true;
		}
	}
	
	return theAnswer;
}

