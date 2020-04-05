/*
	    File: CMIO_CC_608_Scraper.h
	Abstract: A utility class to assist with closed caption scraping.
	 Version: 1.2

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
