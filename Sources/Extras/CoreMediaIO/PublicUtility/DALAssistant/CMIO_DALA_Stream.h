/*
	    File: CMIO_DALA_Stream.h
	Abstract: C++ wrapper for CMIOStreamID
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

#if !defined(__CMIO_DALA_Stream_h__)
#define __CMIO_DALA_Stream_h__

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Public Utility Includes
#include "CMIO_DALA_Object.h"

// System Includes
#include <CoreMedia/CMSampleBuffer.h>
#include <CoreMediaIO/CMIOHardwareDevice.h>

namespace CMIO { namespace DALA
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Stream
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class Stream : public Object
	{
	// Construction/Destruction
	public:
								Stream(CMIOStreamID cmioStreamID);
								~Stream();

	// General Operations
	public:
		CMIOStreamID			GetCMIOStreamID() const { return GetObjectID(); }
		CMIODeviceID			GetOwningDevice() const;
		CMIOObjectPropertyScope	GetSection() const;
		UInt32					GetTerminalType() const;
		UInt32					GetStartingDeviceChannel() const;
		UInt32					GetLatency() const;

	// Format Operations
	public:
		bool					CurrentFormatIsSettable() const;
		void					SetCurrentFormat(CMFormatDescriptionRef format);
		void					SetPreferredFormat(CMFormatDescriptionRef format);
		CMFormatDescriptionRef	GetCurrentFormat() const;
		CMFormatDescriptionRef	GetPreferredFormat() const;
		bool					HasAvailableFormats() const;
		bool					HasPreferredFormat() const;
		UInt32					GetNumberAvailableFormats() const;
		CFArrayRef				GetAvailableFormats() const;
		CMFormatDescriptionRef	GetAvailableFormatByIndex(UInt32 index) const;
		
		bool					HasAvailableStillImageFormats() const;
		UInt32					GetNumberStillImageFormats() const;
		CFArrayRef				GetStillImageFormats() const;
		CMFormatDescriptionRef	GetStillImageFormatByIndex(UInt32 index) const;
		CMSampleBufferRef		GetStillImage(CMFormatDescriptionRef description) const;

		bool					HasFrameRate() const;
		bool					HasPreferredFrameRate() const;
		Float64					GetFrameRate() const;
		void					SetFrameRate(Float64 sampleRate);
		Float64					GetPreferredFrameRate() const;
		void					SetPreferredFrameRate(Float64 sampleRate);
		bool					IsValidFrameRate(Float64 sampleRate) const;
		
		bool					HasMinimumFrameRate() const;
		Float64					GetMinimumFrameRate() const;
		void					SetMinimumFrameRate(Float64 sampleRate);
		
		bool					HasFrameRates() const;
		UInt32					GetNumberFrameRates() const;
		void					GetFrameRates(UInt32& ioNumberRates, Float64* rates) const;
		Float64					GetFrameRateByIndex(UInt32 index) const;
		UInt32					GetNumberAllPossibleFrameRates() const;
		void					GetAllPossibleFrameRates(UInt32& ioNumberRates, Float64* rates) const;
		Float64					GetPossibleFrameRateByIndex(UInt32 index) const;
		
	// Deck Operations
	public:
		bool					CanProcessDeckCommands() const;
		CMIOStreamDeck			GetDeck() const;
		Float64					GetDeckTimecode() const;
		UInt32					GetDeckDropness() const;
		UInt32					GetDeckThreaded() const;
		UInt32					GetDeckLocal() const;
		SInt32					GetDeckCueing() const;
		
	// End of Data Operations
	public:
		bool					HasEndOfData() const;
		bool					GetEndOfData() const;
		void					SetEndOfData(bool endOfData);
		
	// Clock Operations
	public:
		bool					HasClock() const;
		CFTypeRef				CopyClock() const;
		
	// Input/Output Buffer Queue Operations
	public:
		CMSimpleQueueRef		CopyBufferQueue(CMIODeviceStreamQueueAlteredProc queueAlteredProc, void* queueAlteredRefCon) const;

	// Output Buffer Queue Operations
	public:
		bool					HasOutputBufferQueueSize() const;
		UInt32					GetOutputBufferQueueSize() const;
		void					SetOutputBufferQueueSize(UInt32 queueSize);
		bool					HasOutputBuffersRequiredForStartup() const;
		UInt32					GetOutputBuffersRequiredForStartup() const;
		void					SetOutputBuffersRequiredForStartup(UInt32 bufferCount);
		UInt32					GetOutputBuffersNeededForDroplessPlayback() const;
		CMIOStreamScheduledOutputNotificationProcAndRefCon GetScheduledOutputNotificationProcAndRefCon() const;
		void					SetScheduledOutputNotificationProcAndRefCon(const CMIOStreamScheduledOutputNotificationProcAndRefCon& scheduledOutputNotificationProcAndRefCon);

	// Utility Operations
	public:
		static void				GetNameForTerminalType(UInt32 terminalType, char* outName, UInt32 maxNameLen);
	};
}}

#endif
