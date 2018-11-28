/*
	    File: CMIO_CC_608_Scraper.cpp
	Abstract: A utility class to assist with closed caption scraping.
	 Version: 1.2
	
	Disclaimer: IMPORTANT:  This Apple software is supplied to you by Apple
	Inc. ("Apple") in consideration of your agreement to the following
	terms, and your use, installation, modification or redistribution of
	this Apple software constitutes acceptance of these terms.  If you do
	not agree with these terms, please do not use, install, modify or
	redistribute this Apple software.
	
	In consideration of your agreement to abide by the following terms, and
	subject to these terms, Apple grants you a personal, non-exclusive
	license, under Apple's copyrights in this original Apple software (the
	"Apple Software"), to use, reproduce, modify and redistribute the Apple
	Software, with or without modifications, in source and/or binary forms;
	provided that if you redistribute the Apple Software in its entirety and
	without modifications, you must retain this notice and the following
	text and disclaimers in all such redistributions of the Apple Software.
	Neither the name, trademarks, service marks or logos of Apple Inc. may
	be used to endorse or promote products derived from the Apple Software
	without specific prior written permission from Apple.  Except as
	expressly stated in this notice, no other rights or licenses, express or
	implied, are granted by Apple herein, including but not limited to any
	patent rights that may be infringed by your derivative works or by other
	works in which the Apple Software may be incorporated.
	
	The Apple Software is provided by Apple on an "AS IS" basis.  APPLE
	MAKES NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION
	THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS
	FOR A PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND
	OPERATION ALONE OR IN COMBINATION WITH YOUR PRODUCTS.
	
	IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL
	OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
	SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
	INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION,
	MODIFICATION AND/OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED
	AND WHETHER UNDER THEORY OF CONTRACT, TORT (INCLUDING NEGLIGENCE),
	STRICT LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN ADVISED OF THE
	POSSIBILITY OF SUCH DAMAGE.
	
	Copyright (C) 2012 Apple Inc. All Rights Reserved.
	
*/

#define kCMIODebugModuleKey	"CC_608_Scraper.Debug"

#include "CMIODebugMacros.h"

#define module_d_syslog(...) do { if (CMIOModuleDebug( CFSTR( kCMIODebugModuleKey ) )) cmio_debug_syslog(__VA_ARGS__); } while (0)
#define module_debugmsg(...) do { if (CMIOModuleDebug( CFSTR( kCMIODebugModuleKey ) )) cmio_debug_msg(__VA_ARGS__); } while (0)


#include "CMIO_CC_608_Scraper.h"

#include <CoreMediaIO/CMIOUnit.h>


//_____________________________________________________________________________
//

bool		CMIOCC608Scraper::CanSupportFormat(	OSType	pixelFormatType )
{
	return (	( kCMPixelFormat_422YpCbCr8 == pixelFormatType )
			 || ( kCMPixelFormat_422YpCbCr10 == pixelFormatType )
			 || ( kCMPixelFormat_32ARGB == pixelFormatType )
			 || ( kCMPixelFormat_32BGRA == pixelFormatType ) );
}

//_____________________________________________________________________________
//

CMIOCC608Scraper::CMIOCC608Scraper(	OSType	inPixelFormatType ) :
	mPixelFormatType( inPixelFormatType ),
	mNumBitsPerComponent( 0 ),
	mOffsetToFirstCCPixel( 0 ),
	mPixelSkip( 0 ),
	mClockRiseThreshold( 0 ),
	mFindClockMaxLevelProcPtr( NULL ),
	mFindClockMinLevelProcPtr( NULL ),
	mFindClockPeriodProcPtr( NULL ),
	mFindDownwardTransitionProcPtr( NULL ),
	mFindUpwardTransitionProcPtr( NULL ),
	mReadAndCheckSevenBitsProcPtr( NULL )
{
	switch ( mPixelFormatType )
	{
		case kCMPixelFormat_422YpCbCr8:
			mOffsetToFirstCCPixel =				1;
			mPixelSkip =						2;
			mClockRiseThreshold =				0x40;
			mFindClockMaxLevelProcPtr =			Find8BitUnsignedClockMaxLevel;
			mFindClockMinLevelProcPtr =			Find8BitUnsignedClockMinLevel;
			mFindClockPeriodProcPtr =			Find8BitUnsignedClockPeriod;
			mFindDownwardTransitionProcPtr =	Find8BitUnsignedDownwardTransition;
			mFindUpwardTransitionProcPtr =		Find8BitUnsignedUpwardTransition;
			mReadAndCheckSevenBitsProcPtr =		ReadAndCheckSeven8BitUnsignedBits;
		break;
		
		case kCMPixelFormat_422YpCbCr10:
			mOffsetToFirstCCPixel =				0;
			mPixelSkip =						1;
			mClockRiseThreshold =				0x100;
			mFindClockMaxLevelProcPtr =			FindV210ClockMaxLevel;
			mFindClockMinLevelProcPtr =			FindV210ClockMinLevel;
			mFindClockPeriodProcPtr =			FindV210ClockPeriod;
			mFindDownwardTransitionProcPtr =	FindV210DownwardTransition;
			mFindUpwardTransitionProcPtr =		FindV210UpwardTransition;
			mReadAndCheckSevenBitsProcPtr =		ReadAndCheckSevenV210Bits;
		break;
		
		case kCMPixelFormat_32ARGB:
		case kCMPixelFormat_32BGRA:
			// For these two formats, the CC data is in all three channels;  by
			// using an offset to first CC pixel of 1, we will hit either R or G.
			
			mOffsetToFirstCCPixel =				1;
			mPixelSkip =						4;
			mClockRiseThreshold =				0x40;
			mFindClockMaxLevelProcPtr =			Find8BitUnsignedClockMaxLevel;
			mFindClockMinLevelProcPtr =			Find8BitUnsignedClockMinLevel;
			mFindClockPeriodProcPtr =			Find8BitUnsignedClockPeriod;
			mFindDownwardTransitionProcPtr =	Find8BitUnsignedDownwardTransition;
			mFindUpwardTransitionProcPtr =		Find8BitUnsignedUpwardTransition;
			mReadAndCheckSevenBitsProcPtr =		ReadAndCheckSeven8BitUnsignedBits;
		break;
		
		default:
			module_debugmsg( "unsupported pixel type of \'%.4s\'", reinterpret_cast<char *>( &mPixelFormatType ) );
		break;
	}
	
	if ( NULL != mFindClockMaxLevelProcPtr )
	{
		module_debugmsg( "pixel format type = \'%.4s\', offset to first CC pixel = %d, pixel skip = %d, clock rise threshold = %d",
						 reinterpret_cast<char *>( &mPixelFormatType ), (int)mOffsetToFirstCCPixel, (int)mPixelSkip, (int)mClockRiseThreshold );
	}
}

//_____________________________________________________________________________
//

CMIOCC608Scraper::~CMIOCC608Scraper()
{
}

//_____________________________________________________________________________
//

OSStatus	CMIOCC608Scraper::Scrape(    const UInt8 *   inCurrentLine,
										UInt8 *         outFieldData )
{
	if ( ( NULL == inCurrentLine ) || ( NULL == outFieldData ) ) { return kCMIOUnitErr_InvalidParameter; }
	if ( NULL == mFindClockMaxLevelProcPtr ) { return kCMIOUnitErr_FormatNotSupported; }
	
	// This code is based upon Figure 8.58, "625-Line Lines 22 and 335 Closed Captioning Timing",
	// from Chapter 8, "NTSC, PAL, and SECAM Overview", of "Video Demystified: A Handbook for the
	// Digital Engineer", Fourth Edition, by Keith Jack (ISBN: 0750678224).
	
	UInt32 pixOffset =	0;
	
	// Try to find where the first clock pulse rises near the top.
	
	if ( !( mFindUpwardTransitionProcPtr( pixOffset, inCurrentLine, mOffsetToFirstCCPixel, mPixelSkip, 3, mClockRiseThreshold ) ) )
	{
		return kCMIOUnitErr_NoData;
	}
	
	// Assume first clock pulse; look for max clock level and then min clock level.
	
	SInt32 maxClockLevel = mFindClockMaxLevelProcPtr( pixOffset, inCurrentLine, mOffsetToFirstCCPixel, mPixelSkip, 3 );
	
	if ( 0x0000 == maxClockLevel )
	{
		return kCMIOUnitErr_NoData;
	}
	
	SInt32 minClockLevel = mFindClockMinLevelProcPtr( pixOffset, inCurrentLine, mOffsetToFirstCCPixel, mPixelSkip, 3 );
	
	if ( 0xFFFF == minClockLevel )
	{
		return kCMIOUnitErr_NoData;
	}
	
	// Determine the thresholds for the clock pulses.
	
	SInt32 clockSwing =		maxClockLevel - minClockLevel;
	SInt32 clockGoingLow =	minClockLevel + ( clockSwing / static_cast<SInt32>( 4 ) );	// 25% of swing.
	SInt32 clockGoingHigh =	clockGoingLow + ( clockSwing / static_cast<SInt32>( 2 ) );	// 75% of swing.
	
	// Determine the clock period.
	
	Float32 clockHighToPeekOffset;
	
	Float32	clockPeriod = mFindClockPeriodProcPtr( pixOffset, inCurrentLine, mOffsetToFirstCCPixel, mPixelSkip, clockGoingLow, clockGoingHigh, clockHighToPeekOffset );
	if (	( static_cast<Float32>( 20.0 ) >= clockPeriod )			// Should be around 26 to 27.
		 || ( static_cast<Float32>( 33.0 ) <= clockPeriod ) )
	{
		return kCMIOUnitErr_NoData;
	}
	
	// Find the start bit, and compute the expected end of the data.
	
	if ( !( mFindUpwardTransitionProcPtr( pixOffset, inCurrentLine, mOffsetToFirstCCPixel, mPixelSkip, 3, clockGoingHigh ) ) )
	{
		return kCMIOUnitErr_NoData;
	}
	
	if ( ( 200 >= pixOffset ) || ( 280 <= pixOffset ) )		// Should be around 240.
	{
		return kCMIOUnitErr_NoData;
	}
	
	Float32 startBitOffset = static_cast<Float32>( pixOffset ) + clockHighToPeekOffset;
	
	UInt32 expectedEndOfDataOffset = static_cast<UInt32>(	startBitOffset
														  + ( static_cast<Float32>( 16 ) * clockPeriod )
														  + ( clockPeriod / static_cast<Float32>( 2 ) ) );
	
	if ( 720 <= expectedEndOfDataOffset )
	{
		return kCMIOUnitErr_NoData;
	}
	
	// OK, determine the data threshold and pick out the bits.
	
	SInt32 dataThreshold = minClockLevel + ( clockSwing / static_cast<SInt32>( 2 ) );	// 50% of swing.
	
	UInt32	d0OffsetToBit0Peak =	static_cast<UInt32>( startBitOffset + clockPeriod );
	UInt8	d0;
	
	if ( !( mReadAndCheckSevenBitsProcPtr( d0OffsetToBit0Peak, inCurrentLine, mOffsetToFirstCCPixel, mPixelSkip, clockPeriod, dataThreshold, d0 ) ) )
	{
		return kCMIOUnitErr_NoData;
	}
	
	UInt32	d1OffsetToBit0Peak = static_cast<UInt32>( startBitOffset + ( static_cast<Float32>( 9 ) * clockPeriod ) );
	UInt8	d1;
	
	if ( !( mReadAndCheckSevenBitsProcPtr( d1OffsetToBit0Peak, inCurrentLine, mOffsetToFirstCCPixel, mPixelSkip, clockPeriod, dataThreshold, d1 ) ) )
	{
		return kCMIOUnitErr_NoData;
	}
	
	// OK, we passed all of the tests!  Return the data bytes!
	
	outFieldData[ 0 ] = d0;
	outFieldData[ 1 ] = d1;
	
	return noErr;
}

#pragma mark 8 Bit Unsigned Scraper Routines
//_____________________________________________________________________________
//

SInt32		CMIOCC608Scraper::Find8BitUnsignedClockMaxLevel(	UInt32 &		ioPixOffset,
															const UInt8 *   inCurrentLine,
															UInt32			inOffsetToFirstCCPixel,
															UInt32			inPixelSkip,
															UInt32			inConsecutivePixelsToFind )
{
	const UInt8 *	currYPix =					&( inCurrentLine[ ( ioPixOffset * inPixelSkip ) + inOffsetToFirstCCPixel ] );
	UInt32			numConsecutivePixelsFound =	0;
	bool			transitionFound =			false;
	UInt8			maxClockLevel =				0x00;
	
	UInt8	currYValue;

	while ( ( ioPixOffset < 720 ) && ( !transitionFound ) )
	{
		currYValue = *currYPix;
		
		if ( maxClockLevel > currYValue )
		{
			++numConsecutivePixelsFound;
			
			if ( inConsecutivePixelsToFind == numConsecutivePixelsFound )
			{
				transitionFound = true;
			}
			else
			{
				++ioPixOffset;
				currYPix += inPixelSkip;
			}
		}
		else
		{
			numConsecutivePixelsFound =	0;
			maxClockLevel =				currYValue;
			
			++ioPixOffset;
			currYPix += inPixelSkip;
		}
	}
	
	return ( transitionFound ) ? static_cast<SInt32>( static_cast<UInt32>( maxClockLevel ) ) : 0x0000;
}

//_____________________________________________________________________________
//

SInt32		CMIOCC608Scraper::Find8BitUnsignedClockMinLevel(	UInt32 &		ioPixOffset,
															const UInt8 *   inCurrentLine,
															UInt32			inOffsetToFirstCCPixel,
															UInt32			inPixelSkip,
															UInt32			inConsecutivePixelsToFind )
{
	const UInt8 *	currYPix =					&( inCurrentLine[ ( ioPixOffset * inPixelSkip ) + inOffsetToFirstCCPixel ] );
	UInt32			numConsecutivePixelsFound =	0;
	bool			transitionFound =			false;
	UInt8			minClockLevel =				0xFF;
	
	UInt8	currYValue;

	while ( ( ioPixOffset < 720 ) && ( !transitionFound ) )
	{
		currYValue = *currYPix;
		
		if ( minClockLevel < currYValue )
		{
			++numConsecutivePixelsFound;
			
			if ( inConsecutivePixelsToFind == numConsecutivePixelsFound )
			{
				transitionFound = true;
			}
			else
			{
				++ioPixOffset;
				currYPix += inPixelSkip;
			}
		}
		else
		{
			numConsecutivePixelsFound =	0;
			minClockLevel =				currYValue;
			
			++ioPixOffset;
			currYPix += inPixelSkip;
		}
	}
	
	return ( transitionFound ) ? static_cast<SInt32>( static_cast<UInt32>( minClockLevel ) ) : 0xFFFF;
}

//_____________________________________________________________________________
//

Float32		CMIOCC608Scraper::Find8BitUnsignedClockPeriod(	UInt32 &		ioPixOffset,
															const UInt8 *   inCurrentLine,
															UInt32			inOffsetToFirstCCPixel,
															UInt32			inPixelSkip,
															SInt32			inClockGoingLow,
															SInt32			inClockGoingHigh,
															Float32 &		oClockHighToPeekOffset )
{
	Float32	clockPeriod = 0.0;
	
	Float32 firstClockGoingHighStart;
	Float32 firstClockGoingHighEnd;
	Float32 lastClockGoingHighStart;
	Float32 lastClockGoingHighEnd;
	Float32 peakOfLastPulse;
	Float32 peakOfFirstPulse;
	
	// Look for second of seven clock going high, then going low.
	
	if ( !( Find8BitUnsignedUpwardTransition( ioPixOffset, inCurrentLine, inOffsetToFirstCCPixel, inPixelSkip, 3, inClockGoingHigh ) ) ) { goto bail; }
	
	firstClockGoingHighStart = static_cast<Float32>( ioPixOffset );
	
	if ( !( Find8BitUnsignedDownwardTransition( ioPixOffset, inCurrentLine, inOffsetToFirstCCPixel, inPixelSkip, 3, inClockGoingHigh ) ) ) { goto bail; }
	
	firstClockGoingHighEnd = static_cast<Float32>( ioPixOffset );

	if ( !( Find8BitUnsignedDownwardTransition( ioPixOffset, inCurrentLine, inOffsetToFirstCCPixel, inPixelSkip, 3, inClockGoingLow ) ) ) { goto bail; }
	
	// Find next four clock pulses (pulses 3 through 6).
	
	if ( !( Find8BitUnsignedUpwardTransition( ioPixOffset, inCurrentLine, inOffsetToFirstCCPixel, inPixelSkip, 3, inClockGoingHigh ) ) ) { goto bail; }
	if ( !( Find8BitUnsignedDownwardTransition( ioPixOffset, inCurrentLine, inOffsetToFirstCCPixel, inPixelSkip, 3, inClockGoingLow ) ) ) { goto bail; }
	
	if ( !( Find8BitUnsignedUpwardTransition( ioPixOffset, inCurrentLine, inOffsetToFirstCCPixel, inPixelSkip, 3, inClockGoingHigh ) ) ) { goto bail; }
	if ( !( Find8BitUnsignedDownwardTransition( ioPixOffset, inCurrentLine, inOffsetToFirstCCPixel, inPixelSkip, 3, inClockGoingLow ) ) ) { goto bail; }
	
	if ( !( Find8BitUnsignedUpwardTransition( ioPixOffset, inCurrentLine, inOffsetToFirstCCPixel, inPixelSkip, 3, inClockGoingHigh ) ) ) { goto bail; }
	if ( !( Find8BitUnsignedDownwardTransition( ioPixOffset, inCurrentLine, inOffsetToFirstCCPixel, inPixelSkip, 3, inClockGoingLow ) ) ) { goto bail; }
	
	if ( !( Find8BitUnsignedUpwardTransition( ioPixOffset, inCurrentLine, inOffsetToFirstCCPixel, inPixelSkip, 3, inClockGoingHigh ) ) ) { goto bail; }
	if ( !( Find8BitUnsignedDownwardTransition( ioPixOffset, inCurrentLine, inOffsetToFirstCCPixel, inPixelSkip, 3, inClockGoingLow ) ) ) { goto bail; }
	
	// Look for last of seven clock going high, then going low.
	
	if ( !( Find8BitUnsignedUpwardTransition( ioPixOffset, inCurrentLine, inOffsetToFirstCCPixel, inPixelSkip, 3, inClockGoingHigh ) ) ) { goto bail; }
	
	lastClockGoingHighStart = static_cast<Float32>( ioPixOffset );
	
	if ( !( Find8BitUnsignedDownwardTransition( ioPixOffset, inCurrentLine, inOffsetToFirstCCPixel, inPixelSkip, 3, inClockGoingHigh ) ) ) { goto bail; }
	
	lastClockGoingHighEnd = static_cast<Float32>( ioPixOffset );
	
	if ( !( Find8BitUnsignedDownwardTransition( ioPixOffset, inCurrentLine, inOffsetToFirstCCPixel, inPixelSkip, 3, inClockGoingLow ) ) ) { goto bail; }
	
	// Compute the average clock pulse length, given that we've just found six clock pulses.
	
	peakOfLastPulse =	lastClockGoingHighStart + ( ( lastClockGoingHighEnd - lastClockGoingHighStart ) / static_cast<Float32>( 2.0 ) );
	peakOfFirstPulse =	firstClockGoingHighStart + ( ( firstClockGoingHighEnd - firstClockGoingHighStart ) / static_cast<Float32>( 2.0 ) );
	
	clockPeriod = ( peakOfLastPulse - peakOfFirstPulse ) / static_cast<Float32>( 5.0 );
	
	oClockHighToPeekOffset =	(	( ( firstClockGoingHighEnd - firstClockGoingHighStart ) / static_cast<Float32>( 2.0 ) )
								  + ( ( lastClockGoingHighEnd - lastClockGoingHighStart ) / static_cast<Float32>( 2.0 ) ) )
							  / static_cast<Float32>( 2.0 );
	
bail:
	return clockPeriod;
}

//_____________________________________________________________________________
//

bool		CMIOCC608Scraper::Find8BitUnsignedDownwardTransition(	UInt32 &		ioPixOffset,
																	const UInt8 *   inCurrentLine,
																	UInt32			inOffsetToFirstCCPixel,
																	UInt32			inPixelSkip,
																	UInt32			inConsecutivePixelsToFind,
																	SInt32			inThreshold )
{
	const UInt8 *	currYPix =					&( inCurrentLine[ ( ioPixOffset * inPixelSkip ) + inOffsetToFirstCCPixel ] );
	UInt32			numConsecutivePixelsFound =	0;
	bool			transitionFound =			false;
	UInt8			threshold =					static_cast<UInt8>( inThreshold );

	while ( ( ioPixOffset < 720 ) && ( !transitionFound ) )
	{
		if ( *currYPix <= threshold )
		{
			++numConsecutivePixelsFound;
			
			if ( inConsecutivePixelsToFind == numConsecutivePixelsFound )
			{
				transitionFound = true;
			}
			else
			{
				++ioPixOffset;
				currYPix += inPixelSkip;
			}
		}
		else
		{
			numConsecutivePixelsFound = 0;
			
			++ioPixOffset;
			currYPix += inPixelSkip;
		}
	}
	
	return transitionFound;
}

//_____________________________________________________________________________
//

bool		CMIOCC608Scraper::Find8BitUnsignedUpwardTransition(	UInt32 &		ioPixOffset,
																const UInt8 *   inCurrentLine,
																UInt32			inOffsetToFirstCCPixel,
																UInt32			inPixelSkip,
																UInt32			inConsecutivePixelsToFind,
																SInt32			inThreshold )
{
	const UInt8 *	currYPix =					&( inCurrentLine[ ( ioPixOffset * inPixelSkip ) + inOffsetToFirstCCPixel ] );
	UInt32			numConsecutivePixelsFound =	0;
	bool			transitionFound =			false;
	UInt8			threshold =					static_cast<UInt8>( inThreshold );

	while ( ( ioPixOffset < 720 ) && ( !transitionFound ) )
	{
		if ( *currYPix >= threshold )
		{
			++numConsecutivePixelsFound;
			
			if ( inConsecutivePixelsToFind == numConsecutivePixelsFound )
			{
				transitionFound = true;
			}
			else
			{
				++ioPixOffset;
				currYPix += inPixelSkip;
			}
		}
		else
		{
			numConsecutivePixelsFound = 0;
			
			++ioPixOffset;
			currYPix += inPixelSkip;
		}
	}
	
	return transitionFound;
}

//_____________________________________________________________________________
//

bool		CMIOCC608Scraper::ReadAndCheckSeven8BitUnsignedBits(	UInt32			inOffsetToBit0Peak,
																const UInt8 *   inCurrentLine,
																UInt32			inOffsetToFirstCCPixel,
																UInt32			inPixelSkip,
																Float32			inClockPeriod,
																SInt32			inDataThreshold,
																UInt8 &			oData )
{
	const UInt8 *	bit0Peak =	&( inCurrentLine[ ( inOffsetToBit0Peak * inPixelSkip ) + inOffsetToFirstCCPixel ] );
	UInt8			data =		0;
	UInt8			parity =	0;
	UInt8			threshold =	static_cast<UInt8>( inDataThreshold );
	
	if ( threshold < bit0Peak[ 0 ] )
	{
		data =		0x01;
		parity =	0x1;
	}
	
	if ( threshold < bit0Peak[ static_cast<UInt32>( inClockPeriod ) * inPixelSkip ] )
	{
		data |=		0x02;
		parity =	parity ^ 0x1;
	}
	
	if ( threshold < bit0Peak[ static_cast<UInt32>( static_cast<Float32>( 2 ) * inClockPeriod ) * inPixelSkip ] )
	{
		data |=		0x04;
		parity =	parity ^ 0x1;
	}
	
	if ( threshold < bit0Peak[ static_cast<UInt32>( static_cast<Float32>( 3 ) * inClockPeriod ) * inPixelSkip ] )
	{
		data |=		0x08;
		parity =	parity ^ 0x1;
	}
	
	if ( threshold < bit0Peak[ static_cast<UInt32>( static_cast<Float32>( 4 ) * inClockPeriod ) * inPixelSkip ] )
	{
		data |=		0x10;
		parity =	parity ^ 0x1;
	}
	
	if ( threshold < bit0Peak[ static_cast<UInt32>( static_cast<Float32>( 5 ) * inClockPeriod ) * inPixelSkip ] )
	{
		data |=		0x20;
		parity =	parity ^ 0x1;
	}
	
	if ( threshold < bit0Peak[ static_cast<UInt32>( static_cast<Float32>( 6 ) * inClockPeriod ) * inPixelSkip ] )
	{
		data |=		0x40;
		parity =	parity ^ 0x1;
	}
	
	if ( threshold < bit0Peak[ static_cast<UInt32>( static_cast<Float32>( 7 ) * inClockPeriod ) * inPixelSkip ] )
	{
		if ( 0 != parity )
		{
			module_debugmsg( "parity error" );
			return false;
		}
	}
	else
	{
		if ( 0 == parity )
		{
			module_debugmsg( "parity error" );
			return false;
		}
	}
	
	oData = data;
	return true;
}

#pragma mark 10 Bit Unsigned YUV v210 Scraper Routines

// Constants and a macro to help us create the 10 bit Y value for a given offset.
// Taken from http://developer.apple.com/quicktime/icefloe/dispatch019.html#v210.
// We access these by dividing the line into chunks of 3 tuples (each touple is
// encoded into 4 bytes), and then figuring out the offset to the Y pixel in the
// given tuple, and then build the 10 bit Y value by picking things out of the
// different bytes.

static const UInt32 kV210YByte0Offsets[ 3 ] =		{    1,    4,    6 };

static const UInt32	kV210YByte0Mask[ 3 ] =			{ 0xFC, 0xFF, 0xF0 };
static const UInt32 kV210YByte0RightShifts[ 3 ] =	{    2,    0,    4 };
static const UInt32	kV210YByte1Mask[ 3 ] =			{ 0x0F, 0x03, 0x3F };
static const UInt32 kV210YByte1LeftShifts[ 3 ] =	{    6,    8,    4 };

#define ExtractV210YValue( _inOffset, _inCurrentLine, _outYValue )	\
	{	\
		UInt32 v210Chunk =		( _inOffset / 3 );				\
		UInt32 indexInChunk =	_inOffset - ( v210Chunk * 3 );	\
		UInt32 yByte0Offset =	( v210Chunk * 8 ) + kV210YByte0Offsets[ indexInChunk ];	\
		\
		_outYValue = (	( ( static_cast<UInt32>( _inCurrentLine[ yByte0Offset ] )     & kV210YByte0Mask[ indexInChunk ] ) >> kV210YByte0RightShifts[ indexInChunk ] )	\
					  | ( ( static_cast<UInt32>( _inCurrentLine[ yByte0Offset + 1 ] ) & kV210YByte1Mask[ indexInChunk ] ) << kV210YByte1LeftShifts[ indexInChunk ] ) );	\
	}

//_____________________________________________________________________________
//

SInt32		CMIOCC608Scraper::FindV210ClockMaxLevel(	UInt32 &		ioPixOffset,
													const UInt8 *   inCurrentLine,
													UInt32			inOffsetToFirstCCPixel,
													UInt32			inPixelSkip,
													UInt32			inConsecutivePixelsToFind )
{
	UInt32	numConsecutivePixelsFound =	0;
	bool	transitionFound =			false;
	UInt32	maxClockLevel =				0x0000;
	
	UInt32	currYValue;

	while ( ( ioPixOffset < 720 ) && ( !transitionFound ) )
	{
		ExtractV210YValue( ioPixOffset, inCurrentLine, currYValue );
		
		if ( maxClockLevel > currYValue )
		{
			++numConsecutivePixelsFound;
			
			if ( inConsecutivePixelsToFind == numConsecutivePixelsFound )
			{
				transitionFound = true;
			}
			else
			{
				++ioPixOffset;
			}
		}
		else
		{
			numConsecutivePixelsFound =	0;
			maxClockLevel =				currYValue;
			
			++ioPixOffset;
		}
	}
	
	return ( transitionFound ) ? static_cast<SInt32>( maxClockLevel ) : 0x0000;
}

//_____________________________________________________________________________
//

SInt32		CMIOCC608Scraper::FindV210ClockMinLevel(	UInt32 &		ioPixOffset,
													const UInt8 *   inCurrentLine,
													UInt32			inOffsetToFirstCCPixel,
													UInt32			inPixelSkip,
													UInt32			inConsecutivePixelsToFind )
{
	UInt32	numConsecutivePixelsFound =	0;
	bool	transitionFound =			false;
	UInt32	minClockLevel =				0xFFFF;
	
	UInt32	currYValue;

	while ( ( ioPixOffset < 720 ) && ( !transitionFound ) )
	{
		ExtractV210YValue( ioPixOffset, inCurrentLine, currYValue );
		
		if ( minClockLevel < currYValue )
		{
			++numConsecutivePixelsFound;
			
			if ( inConsecutivePixelsToFind == numConsecutivePixelsFound )
			{
				transitionFound = true;
			}
			else
			{
				++ioPixOffset;
			}
		}
		else
		{
			numConsecutivePixelsFound =	0;
			minClockLevel =				currYValue;
			
			++ioPixOffset;
		}
	}
	
	return ( transitionFound ) ? static_cast<SInt32>( minClockLevel ) : 0xFFFF;
}

//_____________________________________________________________________________
//

Float32		CMIOCC608Scraper::FindV210ClockPeriod(	UInt32 &		ioPixOffset,
													const UInt8 *   inCurrentLine,
													UInt32			inOffsetToFirstCCPixel,
													UInt32			inPixelSkip,
													SInt32			inClockGoingLow,
													SInt32			inClockGoingHigh,
													Float32 &		oClockHighToPeekOffset )
{
	Float32	clockPeriod = 0.0;
	
	Float32 firstClockGoingHighStart;
	Float32 firstClockGoingHighEnd;
	Float32 lastClockGoingHighStart;
	Float32 lastClockGoingHighEnd;
	Float32 peakOfLastPulse;
	Float32 peakOfFirstPulse;
	
	// Look for second of seven clock going high, then going low.
	
	if ( !( FindV210UpwardTransition( ioPixOffset, inCurrentLine, inOffsetToFirstCCPixel, inPixelSkip, 3, inClockGoingHigh ) ) ) { goto bail; }
	
	firstClockGoingHighStart = static_cast<Float32>( ioPixOffset );
	
	if ( !( FindV210DownwardTransition( ioPixOffset, inCurrentLine, inOffsetToFirstCCPixel, inPixelSkip, 3, inClockGoingHigh ) ) ) { goto bail; }
	
	firstClockGoingHighEnd = static_cast<Float32>( ioPixOffset );

	if ( !( FindV210DownwardTransition( ioPixOffset, inCurrentLine, inOffsetToFirstCCPixel, inPixelSkip, 3, inClockGoingLow ) ) ) { goto bail; }
	
	// Find next four clock pulses (pulses 3 through 6).
	
	if ( !( FindV210UpwardTransition( ioPixOffset, inCurrentLine, inOffsetToFirstCCPixel, inPixelSkip, 3, inClockGoingHigh ) ) ) { goto bail; }
	if ( !( FindV210DownwardTransition( ioPixOffset, inCurrentLine, inOffsetToFirstCCPixel, inPixelSkip, 3, inClockGoingLow ) ) ) { goto bail; }
	
	if ( !( FindV210UpwardTransition( ioPixOffset, inCurrentLine, inOffsetToFirstCCPixel, inPixelSkip, 3, inClockGoingHigh ) ) ) { goto bail; }
	if ( !( FindV210DownwardTransition( ioPixOffset, inCurrentLine, inOffsetToFirstCCPixel, inPixelSkip, 3, inClockGoingLow ) ) ) { goto bail; }
	
	if ( !( FindV210UpwardTransition( ioPixOffset, inCurrentLine, inOffsetToFirstCCPixel, inPixelSkip, 3, inClockGoingHigh ) ) ) { goto bail; }
	if ( !( FindV210DownwardTransition( ioPixOffset, inCurrentLine, inOffsetToFirstCCPixel, inPixelSkip, 3, inClockGoingLow ) ) ) { goto bail; }
	
	if ( !( FindV210UpwardTransition( ioPixOffset, inCurrentLine, inOffsetToFirstCCPixel, inPixelSkip, 3, inClockGoingHigh ) ) ) { goto bail; }
	if ( !( FindV210DownwardTransition( ioPixOffset, inCurrentLine, inOffsetToFirstCCPixel, inPixelSkip, 3, inClockGoingLow ) ) ) { goto bail; }
	
	// Look for last of seven clock going high, then going low.
	
	if ( !( FindV210UpwardTransition( ioPixOffset, inCurrentLine, inOffsetToFirstCCPixel, inPixelSkip, 3, inClockGoingHigh ) ) ) { goto bail; }
	
	lastClockGoingHighStart = static_cast<Float32>( ioPixOffset );
	
	if ( !( FindV210DownwardTransition( ioPixOffset, inCurrentLine, inOffsetToFirstCCPixel, inPixelSkip, 3, inClockGoingHigh ) ) ) { goto bail; }
	
	lastClockGoingHighEnd = static_cast<Float32>( ioPixOffset );
	
	if ( !( FindV210DownwardTransition( ioPixOffset, inCurrentLine, inOffsetToFirstCCPixel, inPixelSkip, 3, inClockGoingLow ) ) ) { goto bail; }
	
	// Compute the average clock pulse length, given that we've just found six clock pulses.
	
	peakOfLastPulse =	lastClockGoingHighStart + ( ( lastClockGoingHighEnd - lastClockGoingHighStart ) / static_cast<Float32>( 2.0 ) );
	peakOfFirstPulse =	firstClockGoingHighStart + ( ( firstClockGoingHighEnd - firstClockGoingHighStart ) / static_cast<Float32>( 2.0 ) );
	
	clockPeriod = ( peakOfLastPulse - peakOfFirstPulse ) / static_cast<Float32>( 5.0 );
	
	oClockHighToPeekOffset =	(	( ( firstClockGoingHighEnd - firstClockGoingHighStart ) / static_cast<Float32>( 2.0 ) )
								  + ( ( lastClockGoingHighEnd - lastClockGoingHighStart ) / static_cast<Float32>( 2.0 ) ) )
							  / static_cast<Float32>( 2.0 );
	
bail:
	return clockPeriod;
}

//_____________________________________________________________________________
//

bool		CMIOCC608Scraper::FindV210DownwardTransition(	UInt32 &		ioPixOffset,
															const UInt8 *   inCurrentLine,
															UInt32			inOffsetToFirstCCPixel,
															UInt32			inPixelSkip,
															UInt32			inConsecutivePixelsToFind,
															SInt32			inThreshold )
{
	UInt32	numConsecutivePixelsFound =	0;
	bool	transitionFound =			false;
	UInt32	threshold =					static_cast<UInt32>( inThreshold );
	
	UInt32	currYValue;

	while ( ( ioPixOffset < 720 ) && ( !transitionFound ) )
	{
		ExtractV210YValue( ioPixOffset, inCurrentLine, currYValue );
		
		if ( currYValue <= threshold )
		{
			++numConsecutivePixelsFound;
			
			if ( inConsecutivePixelsToFind == numConsecutivePixelsFound )
			{
				transitionFound = true;
			}
			else
			{
				++ioPixOffset;
			}
		}
		else
		{
			numConsecutivePixelsFound = 0;
			
			++ioPixOffset;
		}
	}
	
	return transitionFound;
}

//_____________________________________________________________________________
//

bool		CMIOCC608Scraper::FindV210UpwardTransition(	UInt32 &		ioPixOffset,
														const UInt8 *   inCurrentLine,
														UInt32			inOffsetToFirstCCPixel,
														UInt32			inPixelSkip,
														UInt32			inConsecutivePixelsToFind,
														SInt32			inThreshold )
{
	UInt32	numConsecutivePixelsFound =	0;
	bool	transitionFound =			false;
	UInt32	threshold =					static_cast<UInt32>( inThreshold );
	
	UInt32	currYValue;

	while ( ( ioPixOffset < 720 ) && ( !transitionFound ) )
	{
		ExtractV210YValue( ioPixOffset, inCurrentLine, currYValue );
		
		if ( currYValue >= threshold )
		{
			++numConsecutivePixelsFound;
			
			if ( inConsecutivePixelsToFind == numConsecutivePixelsFound )
			{
				transitionFound = true;
			}
			else
			{
				++ioPixOffset;
			}
		}
		else
		{
			numConsecutivePixelsFound = 0;
			
			++ioPixOffset;
		}
	}
	
	return transitionFound;
}

//_____________________________________________________________________________
//

bool		CMIOCC608Scraper::ReadAndCheckSevenV210Bits(	UInt32			inOffsetToBit0Peak,
														const UInt8 *   inCurrentLine,
														UInt32			inOffsetToFirstCCPixel,
														UInt32			inPixelSkip,
														Float32			inClockPeriod,
														SInt32			inDataThreshold,
														UInt8 &			oData )
{
	UInt8	data =		0;
	UInt8	parity =	0;
	UInt32	threshold =	static_cast<UInt32>( inDataThreshold );
	
	UInt32	offsetToPeak;
	UInt32	currYValue;
	
	ExtractV210YValue( inOffsetToBit0Peak, inCurrentLine, currYValue );
		
	if ( threshold < currYValue )
	{
		data =		0x01;
		parity =	0x1;
	}
	
	offsetToPeak = inOffsetToBit0Peak + ( static_cast<UInt32>( inClockPeriod ) * inPixelSkip );
	ExtractV210YValue( offsetToPeak, inCurrentLine, currYValue );
		
	if ( threshold < currYValue )
	{
		data |=		0x02;
		parity =	parity ^ 0x1;
	}
	
	offsetToPeak = inOffsetToBit0Peak + ( static_cast<UInt32>( static_cast<Float32>( 2 ) * inClockPeriod ) * inPixelSkip );
	ExtractV210YValue( offsetToPeak, inCurrentLine, currYValue );
		
	if ( threshold < currYValue )
	{
		data |=		0x04;
		parity =	parity ^ 0x1;
	}
	
	offsetToPeak = inOffsetToBit0Peak + ( static_cast<UInt32>( static_cast<Float32>( 3 ) * inClockPeriod ) * inPixelSkip );
	ExtractV210YValue( offsetToPeak, inCurrentLine, currYValue );
		
	if ( threshold < currYValue )
	{
		data |=		0x08;
		parity =	parity ^ 0x1;
	}
	
	offsetToPeak = inOffsetToBit0Peak + ( static_cast<UInt32>( static_cast<Float32>( 4 ) * inClockPeriod ) * inPixelSkip );
	ExtractV210YValue( offsetToPeak, inCurrentLine, currYValue );
		
	if ( threshold < currYValue )
	{
		data |=		0x10;
		parity =	parity ^ 0x1;
	}
	
	offsetToPeak = inOffsetToBit0Peak + ( static_cast<UInt32>( static_cast<Float32>( 5 ) * inClockPeriod ) * inPixelSkip );
	ExtractV210YValue( offsetToPeak, inCurrentLine, currYValue );
		
	if ( threshold < currYValue )
	{
		data |=		0x20;
		parity =	parity ^ 0x1;
	}
	
	offsetToPeak = inOffsetToBit0Peak + ( static_cast<UInt32>( static_cast<Float32>( 6 ) * inClockPeriod ) * inPixelSkip );
	ExtractV210YValue( offsetToPeak, inCurrentLine, currYValue );
		
	if ( threshold < currYValue )
	{
		data |=		0x40;
		parity =	parity ^ 0x1;
	}
	
	offsetToPeak = inOffsetToBit0Peak + ( static_cast<UInt32>( static_cast<Float32>( 7 ) * inClockPeriod ) * inPixelSkip );
	ExtractV210YValue( offsetToPeak, inCurrentLine, currYValue );
		
	if ( threshold < currYValue )
	{
		if ( 0 != parity )
		{
			module_debugmsg( "parity error" );
			return false;
		}
	}
	else
	{
		if ( 0 == parity )
		{
			module_debugmsg( "parity error" );
			return false;
		}
	}
	
	oData = data;
	return true;
}
