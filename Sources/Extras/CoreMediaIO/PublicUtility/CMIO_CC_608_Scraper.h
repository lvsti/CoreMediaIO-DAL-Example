/*
	    File: CMIO_CC_608_Scraper.h
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

#if !defined(__CMIO_CC_608_Scraper__)
#define __CMIO_CC_608_Scraper__

//=============================================================================
//	Includes
//=============================================================================

//=============================================================================
//	CMIOCC608Scraper
//=============================================================================


class CMIOCC608Scraper
{
public:
	static bool		CanSupportFormat(	OSType	pixelFormatType );
	
public:
					CMIOCC608Scraper(	OSType	inPixelFormatType );
					~CMIOCC608Scraper();


public:
    OSStatus        Scrape( const UInt8 *   inCurrentLine,
                            UInt8 *         outFieldData );

private:
	static SInt32	Find8BitUnsignedClockMaxLevel(		UInt32 &		ioPixOffset,
														const UInt8 *   inCurrentLine,
														UInt32			inOffsetToFirstCCPixel,
														UInt32			inPixelSkip,
														UInt32			inConsecutivePixelsToFind );
	
	static SInt32	Find8BitUnsignedClockMinLevel(		UInt32 &		ioPixOffset,
														const UInt8 *   inCurrentLine,
														UInt32			inOffsetToFirstCCPixel,
														UInt32			inPixelSkip,
														UInt32			inConsecutivePixelsToFind );
	
	static Float32	Find8BitUnsignedClockPeriod(		UInt32 &		ioPixOffset,
														const UInt8 *   inCurrentLine,
														UInt32			inOffsetToFirstCCPixel,
														UInt32			inPixelSkip,
														SInt32			inClockGoingLow,
														SInt32			inClockGoingHigh,
														Float32 &		oClockHighToPeekOffset );

	static bool		Find8BitUnsignedDownwardTransition(	UInt32 &		ioPixOffset,
														const UInt8 *   inCurrentLine,
														UInt32			inOffsetToFirstCCPixel,
														UInt32			inPixelSkip,
														UInt32			inConsecutivePixelsToFind,
														SInt32			inThreshold );

	static bool		Find8BitUnsignedUpwardTransition(	UInt32 &		ioPixOffset,
														const UInt8 *   inCurrentLine,
														UInt32			inOffsetToFirstCCPixel,
														UInt32			inPixelSkip,
														UInt32			inConsecutivePixelsToFind,
														SInt32			inThreshold );
	
	static bool		ReadAndCheckSeven8BitUnsignedBits(	UInt32			inOffsetToBit0Peak,
														const UInt8 *   inCurrentLine,
														UInt32			inOffsetToFirstCCPixel,
														UInt32			inPixelSkip,
														Float32			inClockPeriod,
														SInt32			inDataThreshold,
														UInt8 &			oData );
	
	static SInt32	FindV210ClockMaxLevel(				UInt32 &		ioPixOffset,
														const UInt8 *   inCurrentLine,
														UInt32			inOffsetToFirstCCPixel,
														UInt32			inPixelSkip,
														UInt32			inConsecutivePixelsToFind );
	
	static SInt32	FindV210ClockMinLevel(				UInt32 &		ioPixOffset,
														const UInt8 *   inCurrentLine,
														UInt32			inOffsetToFirstCCPixel,
														UInt32			inPixelSkip,
														UInt32			inConsecutivePixelsToFind );
	
	static Float32	FindV210ClockPeriod(				UInt32 &		ioPixOffset,
														const UInt8 *   inCurrentLine,
														UInt32			inOffsetToFirstCCPixel,
														UInt32			inPixelSkip,
														SInt32			inClockGoingLow,
														SInt32			inClockGoingHigh,
														Float32 &		oClockHighToPeekOffset );

	static bool		FindV210DownwardTransition(			UInt32 &		ioPixOffset,
														const UInt8 *   inCurrentLine,
														UInt32			inOffsetToFirstCCPixel,
														UInt32			inPixelSkip,
														UInt32			inConsecutivePixelsToFind,
														SInt32			inThreshold );

	static bool		FindV210UpwardTransition(			UInt32 &		ioPixOffset,
														const UInt8 *   inCurrentLine,
														UInt32			inOffsetToFirstCCPixel,
														UInt32			inPixelSkip,
														UInt32			inConsecutivePixelsToFind,
														SInt32			inThreshold );
	
	static bool		ReadAndCheckSevenV210Bits(			UInt32			inOffsetToBit0Peak,
														const UInt8 *   inCurrentLine,
														UInt32			inOffsetToFirstCCPixel,
														UInt32			inPixelSkip,
														Float32			inClockPeriod,
														SInt32			inDataThreshold,
														UInt8 &			oData );
	
private:
	OSType	mPixelFormatType;
	UInt32	mNumBitsPerComponent;
	UInt32	mOffsetToFirstCCPixel;
	UInt32	mPixelSkip;
	
	SInt32	mClockRiseThreshold;
	
	SInt32 (*mFindClockMaxLevelProcPtr)(		UInt32 &		ioPixOffset,
												const UInt8 *   inCurrentLine,
												UInt32			inOffsetToFirstCCPixel,
												UInt32			inPixelSkip,
												UInt32			inConsecutivePixelsToFind );
	
	SInt32 (*mFindClockMinLevelProcPtr)(		UInt32 &		ioPixOffset,
												const UInt8 *   inCurrentLine,
												UInt32			inOffsetToFirstCCPixel,
												UInt32			inPixelSkip,
												UInt32			inConsecutivePixelsToFind );
	
	Float32 (*mFindClockPeriodProcPtr)(			UInt32 &		ioPixOffset,
												const UInt8 *   inCurrentLine,
												UInt32			inOffsetToFirstCCPixel,
												UInt32			inPixelSkip,
												SInt32			inClockGoingLow,
												SInt32			inClockGoingHigh,
												Float32 &		oClockHighToPeekOffset );
	
	bool (*mFindDownwardTransitionProcPtr)(		UInt32 &		ioPixOffset,
												const UInt8 *   inCurrentLine,
												UInt32			inOffsetToFirstCCPixel,
												UInt32			inPixelSkip,
												UInt32			inConsecutivePixelsToFind,
												SInt32			inThreshold );
	
	bool (*mFindUpwardTransitionProcPtr)(		UInt32 &		ioPixOffset,
												const UInt8 *   inCurrentLine,
												UInt32			inOffsetToFirstCCPixel,
												UInt32			inPixelSkip,
												UInt32			inConsecutivePixelsToFind,
												SInt32			inThreshold );
	
	bool (*mReadAndCheckSevenBitsProcPtr)(		UInt32			inOffsetToBit0Peak,
												const UInt8 *   inCurrentLine,
												UInt32			inOffsetToFirstCCPixel,
												UInt32			inPixelSkip,
												Float32			inClockPeriod,
												SInt32			inDataThreshold,
												UInt8 &			oData );
};

#endif  // __CMIO_CC_608_Scraper__
