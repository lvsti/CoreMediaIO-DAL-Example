/*
	    File: CMIO_DALA_Stream.cpp
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

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Self Include
#include "CMIO_DALA_Stream.h"

// Public Utility Includes
#include "CMIODebugMacros.h"
#include "CMIO_PropertyAddress.h"

// CA Public Utility Includes
#include "CAAutoDisposer.h"
#include "CACFArray.h"
#include "CAException.h"

// System Includes
#include <IOKit/audio/IOAudioTypes.h>

// Standard Library Includes
#include <algorithm>

namespace CMIO { namespace DALA
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Stream
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	Stream::Stream(CMIOStreamID cmioStreamID) :
		Object(cmioStreamID)
	{
	}

	Stream::~Stream()
	{
	}

	CMIODeviceID Stream::GetOwningDevice() const
	{
		CMIODeviceID answer = 0;
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIOObjectPropertyOwner), 0, NULL, sizeof(CMIODeviceID), dataUsed, &answer);
		return answer;
	}

	CMIOObjectPropertyScope Stream::GetSection() const
	{
		UInt32 answer = 0;
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIOStreamPropertyDirection), 0, NULL, sizeof(UInt32), dataUsed, &answer);
		return (answer == 0) ? kCMIODevicePropertyScopeOutput : kCMIODevicePropertyScopeInput;
	}

	UInt32 Stream::GetTerminalType() const
	{
		UInt32 answer = 0;
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIOStreamPropertyTerminalType), 0, NULL, sizeof(UInt32), dataUsed, &answer);
		return answer;
	}

	UInt32 Stream::GetStartingDeviceChannel() const
	{
		UInt32 answer = 0;
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIOStreamPropertyStartingChannel), 0, NULL, sizeof(UInt32), dataUsed, &answer);
		return answer;
	}

	UInt32 Stream::GetLatency() const
	{
		UInt32 answer = 0;
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIOStreamPropertyLatency), 0, NULL, sizeof(UInt32), dataUsed, &answer);
		return answer;
		
	}
    
	#pragma mark -
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// CurrentFormatIsSettable()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool Stream::CurrentFormatIsSettable() const
	{
		return IsPropertySettable(PropertyAddress(kCMIOStreamPropertyFormatDescription));
	}
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// SetCurrentFormat()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void Stream::SetCurrentFormat(CMFormatDescriptionRef format)
	{
		UInt32 size = sizeof(CMFormatDescriptionRef);
		SetPropertyData(PropertyAddress(kCMIOStreamPropertyFormatDescription), 0, NULL, size, &format);
	}
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// SetPreferredFormat()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void Stream::SetPreferredFormat(CMFormatDescriptionRef format)
	{
		UInt32 size = sizeof(CMFormatDescriptionRef);
		SetPropertyData(PropertyAddress(kCMIOStreamPropertyPreferredFormatDescription), 0, NULL, size, &format);
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetCurrentFormat()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	CMFormatDescriptionRef Stream::GetCurrentFormat() const
	{
		CMFormatDescriptionRef format = NULL;
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIOStreamPropertyFormatDescription), 0, NULL, sizeof(CMFormatDescriptionRef), dataUsed, &format);
		return format;
	}
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetPreferredFormat()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	CMFormatDescriptionRef Stream::GetPreferredFormat() const
	{
		CMFormatDescriptionRef format = NULL;
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIOStreamPropertyPreferredFormatDescription), 0, NULL, sizeof(CMFormatDescriptionRef), dataUsed, &format);
		return format;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// HasAvailableFormats()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool Stream::HasAvailableFormats() const
	{
		return HasProperty(PropertyAddress(kCMIOStreamPropertyFormatDescriptions));
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// HasPreferredFormat()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool Stream::HasPreferredFormat() const
	{
		return HasProperty(PropertyAddress(kCMIOStreamPropertyPreferredFormatDescription));
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetNumberAvailableFormats()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	UInt32 Stream::GetNumberAvailableFormats() const
	{
		CACFArray formatDescriptions(GetAvailableFormats(), true);
		return formatDescriptions.GetNumberItems();
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetAvailableFormats()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	CFArrayRef Stream::GetAvailableFormats() const
	{
		UInt32 dataUsed = 0;
		CFArrayRef formatDescriptions = NULL;
		GetPropertyData(PropertyAddress(kCMIOStreamPropertyFormatDescriptions), 0, NULL, sizeof(CFArrayRef), dataUsed, &formatDescriptions);
		return formatDescriptions;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetAvailableFormatByIndex()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	CMFormatDescriptionRef Stream::GetAvailableFormatByIndex(UInt32 index) const
	{
		CACFArray formatDescriptions(GetAvailableFormats(), true);
		ThrowIf(index >= formatDescriptions.GetNumberItems(), CAException(kCMIOHardwareIllegalOperationError), "CMIO::DALA::Stream::GetAvailableFormatByIndex: index out of range");
		CFTypeRef description = NULL;
		(void) formatDescriptions.GetCFType(index, description);
		CFRetain(description);
		return static_cast<CMFormatDescriptionRef>(const_cast<void*>(description));
	}

	#pragma mark -
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// HasAvailableStillImageFormats()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool Stream::HasAvailableStillImageFormats() const
	{
		return HasProperty(PropertyAddress(kCMIOStreamPropertyStillImageFormatDescriptions));
	}
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetNumberAvailableFormats()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	UInt32 Stream::GetNumberStillImageFormats() const
	{
		CACFArray formatDescriptions(GetStillImageFormats(), true);
		return formatDescriptions.GetNumberItems();
	}
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetStillImageFormats()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	CFArrayRef Stream::GetStillImageFormats() const
	{
		UInt32 dataUsed = 0;
		CFArrayRef formatDescriptions = NULL;
		GetPropertyData(PropertyAddress(kCMIOStreamPropertyStillImageFormatDescriptions), 0, NULL, sizeof(CFArrayRef), dataUsed, &formatDescriptions);
		return formatDescriptions;
	}
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetStillImageFormatByIndex()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	CMFormatDescriptionRef Stream::GetStillImageFormatByIndex(UInt32 index) const
	{
		CACFArray formatDescriptions(GetStillImageFormats(), true);
		ThrowIf(index >= formatDescriptions.GetNumberItems(), CAException(kCMIOHardwareIllegalOperationError), "CMIO::DALA::Stream::GetStillImageFormatByIndex: index out of range");
		CFTypeRef description = NULL;
		(void) formatDescriptions.GetCFType(index, description);
		CFRetain(description);
		return static_cast<CMFormatDescriptionRef>(const_cast<void*>(description));
	}
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetStillImage()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	CMSampleBufferRef Stream::GetStillImage(CMFormatDescriptionRef description) const
	{
		CMSampleBufferRef stillImageData = NULL;
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIOStreamPropertyStillImage), sizeof(CMFormatDescriptionRef), &description, sizeof(CMSampleBufferRef), dataUsed, &stillImageData);
		return stillImageData;
	}
	
	#pragma mark -
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// HasFrameRate()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool Stream::HasFrameRate() const
	{
		return HasProperty(PropertyAddress(kCMIOStreamPropertyFrameRate));
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetFrameRate()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	Float64 Stream::GetFrameRate() const
	{
		Float64 answer = 0;
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIOStreamPropertyFrameRate), 0, NULL, sizeof(Float64), dataUsed, &answer);
		return answer;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// SetFrameRate()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void Stream::SetFrameRate(Float64 sampleRate)
	{
		UInt32 size = sizeof(Float64);
		SetPropertyData(PropertyAddress(kCMIOStreamPropertyFrameRate), 0, NULL, size, &sampleRate);
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// HasPreferredFrameRate()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool Stream::HasPreferredFrameRate() const
	{
		return HasProperty(PropertyAddress(kCMIOStreamPropertyPreferredFrameRate));
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetPreferredFrameRate()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	Float64 Stream::GetPreferredFrameRate() const
	{
		Float64 answer = 0;
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIOStreamPropertyPreferredFrameRate), 0, NULL, sizeof(Float64), dataUsed, &answer);
		return answer;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// SetPreferredFrameRate()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void Stream::SetPreferredFrameRate(Float64 sampleRate)
	{
		UInt32 size = sizeof(Float64);
		SetPropertyData(PropertyAddress(kCMIOStreamPropertyPreferredFrameRate), 0, NULL, size, &sampleRate);
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// IsValidFrameRate()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool Stream::IsValidFrameRate(Float64 sampleRate) const
	{
		bool answer = false;
		UInt32 numberRates = GetNumberFrameRates();
		CAAutoArrayDelete<Float64> rates(numberRates);
		GetFrameRates(numberRates, rates);
		for(UInt32 index = 0; !answer && (index < numberRates); ++index)
		{
			answer = (sampleRate == rates[index]);
		}
		return answer;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// HasMinimumFrameRate()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool Stream::HasMinimumFrameRate() const
	{
		return HasProperty(PropertyAddress(kCMIOStreamPropertyMinimumFrameRate));
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetMinimumFrameRate()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	Float64 Stream::GetMinimumFrameRate() const
	{
		Float64 answer = 0;
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIOStreamPropertyMinimumFrameRate), 0, NULL, sizeof(Float64), dataUsed, &answer);
		return answer;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// SetMinimumFrameRate()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void Stream::SetMinimumFrameRate(Float64 sampleRate)
	{
		UInt32 size = sizeof(Float64);
		SetPropertyData(PropertyAddress(kCMIOStreamPropertyMinimumFrameRate), 0, NULL, size, &sampleRate);
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// HasFrameRates()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool Stream::HasFrameRates() const
	{
		return HasProperty(PropertyAddress(kCMIOStreamPropertyFrameRates));
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetNumberFrameRates()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	UInt32 Stream::GetNumberFrameRates() const
	{
		UInt32 size = GetPropertyDataSize(PropertyAddress(kCMIOStreamPropertyFrameRates), 0, NULL);
		return size / sizeof(Float64);
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetFrameRates()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void Stream::GetFrameRates(UInt32& ioNumberRates, Float64* rates) const
	{
		UInt32 size = ioNumberRates * sizeof(Float64);
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIOStreamPropertyFrameRates), 0, NULL, size, dataUsed,rates);
		ioNumberRates = dataUsed / sizeof(Float64);
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetFrameRateByIndex()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	Float64 Stream::GetFrameRateByIndex(UInt32 index) const
	{
		UInt32 numberRates = GetNumberFrameRates();
		ThrowIf(index >= numberRates, CAException(kCMIOHardwareIllegalOperationError), "CMIO::DALA::Stream::GetFrameRateByIndex: index out of range");
		CAAutoArrayDelete<Float64> rates(numberRates);
		GetFrameRates(numberRates, rates);
		return rates[index];
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetNumberAllPossibleFrameRates()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	UInt32 Stream::GetNumberAllPossibleFrameRates() const
	{
		// The idiom of passing a NULL format description is only supported by streams that support
		// preferred frame rates.
		UInt32 result = 0;
		if (HasPreferredFrameRate())
		{
			CMFormatDescriptionRef nullFormat = NULL;  // Triggers returning all possible frame rates
			UInt32 size = GetPropertyDataSize(PropertyAddress(kCMIOStreamPropertyFrameRates), sizeof(nullFormat), &nullFormat);
			result = size / sizeof(Float64);
		}
		return result;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetAllPossibleFrameRates()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void Stream::GetAllPossibleFrameRates(UInt32& ioNumberRates, Float64* rates) const
	{
		UInt32 size = ioNumberRates * sizeof(Float64);
		UInt32 dataUsed = 0;
		CMFormatDescriptionRef nullFormat = NULL;  // Triggers returning all possible frame rates
		GetPropertyData(PropertyAddress(kCMIOStreamPropertyFrameRates), sizeof(nullFormat), &nullFormat, size, dataUsed,rates);
		ioNumberRates = dataUsed / sizeof(Float64);
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetPossibleFrameRateByIndex()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	Float64 Stream::GetPossibleFrameRateByIndex(UInt32 index) const
	{
		UInt32 numberRates = GetNumberAllPossibleFrameRates();
		ThrowIf(index >= numberRates, CAException(kCMIOHardwareIllegalOperationError), "CMIO::DALA::Stream::GetFrameRateByIndex: index out of range");
		CAAutoArrayDelete<Float64> rates(numberRates);
		GetAllPossibleFrameRates(numberRates, rates);
		return rates[index];
	}

	#pragma mark -
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// CanProcessDeckCommands()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool Stream::CanProcessDeckCommands() const
	{
		Boolean answer = 0;
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIOStreamPropertyCanProcessDeckCommand), 0, NULL, sizeof(Boolean), dataUsed, &answer);
		return answer;
	}
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetDeck()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	CMIOStreamDeck Stream::GetDeck() const
	{
		CMIOStreamDeck answer = {0, 0, 0};
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIOStreamPropertyDeck), 0, NULL, sizeof(CMIOStreamDeck), dataUsed, &answer);
		return answer;
	}
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetDeckTimecode()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	Float64 Stream::GetDeckTimecode() const
	{
		Float64 answer;
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIOStreamPropertyDeckFrameNumber), 0, NULL, sizeof(Float64), dataUsed, &answer);
		return answer;
	}
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetDeckDropness()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	UInt32 Stream::GetDeckDropness() const
	{
		UInt32 answer = 0;
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIOStreamPropertyDeckDropness), 0, NULL, sizeof(UInt32), dataUsed, &answer);
		return answer;
	}
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetDeckThreaded()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	UInt32 Stream::GetDeckThreaded() const
	{
		UInt32 answer = 0;
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIOStreamPropertyDeckThreaded), 0, NULL, sizeof(UInt32), dataUsed, &answer);
		return answer;
	}
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetDeckLocal()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	UInt32 Stream::GetDeckLocal() const
	{
		UInt32 answer = 0;
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIOStreamPropertyDeckLocal), 0, NULL, sizeof(UInt32), dataUsed, &answer);
		return answer;
	}
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetDeckCueing()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	SInt32 Stream::GetDeckCueing() const
	{
		SInt32 answer = 0;
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIOStreamPropertyDeckCueing), 0, NULL, sizeof(SInt32), dataUsed, &answer);
		return answer;
	}
		
	#pragma mark -
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// HasEndOfData()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool Stream::HasEndOfData() const
	{
		return HasProperty(PropertyAddress(kCMIOStreamPropertyEndOfData));
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetEndOfData()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool Stream::GetEndOfData() const
	{
		UInt32 endOfData = 0;
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIOStreamPropertyEndOfData), 0, NULL, sizeof(UInt32), dataUsed, &endOfData);
		return endOfData;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// SetEndOfData()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void Stream::SetEndOfData(bool endOfData)
	{
		UInt32 theEndOfData = endOfData ? 1 : 0;
		UInt32 size = sizeof(UInt32);
		SetPropertyData(PropertyAddress(kCMIOStreamPropertyEndOfData), 0, NULL, size, &theEndOfData);
	}

	#pragma mark -
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// HasClock()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool Stream::HasClock() const
	{
		return HasProperty(PropertyAddress(kCMIOStreamPropertyClock));
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// CopyClock()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	CFTypeRef Stream::CopyClock() const
	{
		CFTypeRef clock = NULL;
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIOStreamPropertyClock), 0, NULL, sizeof(CFTypeRef), dataUsed, &clock);
		return clock;
	}
	
	#pragma mark -
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// CopyBufferQueue()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	CMSimpleQueueRef Stream::CopyBufferQueue(CMIODeviceStreamQueueAlteredProc queueAlteredProc, void* queueAlteredRefCon) const
	{
		CMSimpleQueueRef queue = NULL;
		OSStatus err = CMIOStreamCopyBufferQueue(GetObjectID(), queueAlteredProc, queueAlteredRefCon, &queue);
		ThrowIfError(err, CAException(err), "CMIO::DALA::Stream::CopyBufferQueue: CMIOStreamCopyBufferQueue() failed");
		return queue;
	}

	#pragma mark -
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// HasOutputBufferQueueSize()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool Stream::HasOutputBufferQueueSize() const
	{
		return HasProperty(PropertyAddress(kCMIOStreamPropertyOutputBufferQueueSize));
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetOutputBufferQueueSize()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	UInt32 Stream::GetOutputBufferQueueSize() const
	{
		UInt32 queueSize = 0;
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIOStreamPropertyOutputBufferQueueSize), 0, NULL, sizeof(UInt32), dataUsed, &queueSize);
		return queueSize;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// SetOutputBufferQueueSize()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void Stream::SetOutputBufferQueueSize(UInt32 queueSize)
	{
		UInt32 size = sizeof(UInt32);
		SetPropertyData(PropertyAddress(kCMIOStreamPropertyOutputBufferQueueSize), 0, NULL, size, &queueSize);
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// HasOutputBuffersRequiredForStartup()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool Stream::HasOutputBuffersRequiredForStartup() const
	{
		return HasProperty(PropertyAddress(kCMIOStreamPropertyOutputBuffersRequiredForStartup));
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetOutputBuffersRequiredForStartup()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	UInt32 Stream::GetOutputBuffersRequiredForStartup() const
	{
		UInt32 bufferCount = 0;
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIOStreamPropertyOutputBuffersRequiredForStartup), 0, NULL, sizeof(UInt32), dataUsed, &bufferCount);
		return bufferCount;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// SetOutputBuffersRequiredForStartup()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void Stream::SetOutputBuffersRequiredForStartup(UInt32 bufferCount)
	{
		UInt32 size = sizeof(UInt32);
		SetPropertyData(PropertyAddress(kCMIOStreamPropertyOutputBuffersRequiredForStartup), 0, NULL, size, &bufferCount);
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetOutputBuffersNeededForDroplessPlayback()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	UInt32 Stream::GetOutputBuffersNeededForDroplessPlayback() const
	{
		UInt32 answer = 0;
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIOStreamPropertyOutputBuffersNeededForDroplessPlayback), 0, NULL, sizeof(UInt32), dataUsed, &answer);
		return answer;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetScheduledOutputNotificationProcAndRefCon()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	CMIOStreamScheduledOutputNotificationProcAndRefCon Stream::GetScheduledOutputNotificationProcAndRefCon() const
	{
		CMIOStreamScheduledOutputNotificationProcAndRefCon procAndRefCon = { NULL, NULL };
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIOStreamPropertyScheduledOutputNotificationProc), 0, NULL, sizeof(CMIOStreamScheduledOutputNotificationProcAndRefCon), dataUsed, &procAndRefCon);
		return procAndRefCon;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// SetScheduledOutputNotificationProcAndRefCon()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void Stream::SetScheduledOutputNotificationProcAndRefCon(const CMIOStreamScheduledOutputNotificationProcAndRefCon& scheduledOutputNotificationProcAndRefCon)
	{
		UInt32 size = sizeof(CMIOStreamScheduledOutputNotificationProcAndRefCon);
		SetPropertyData(PropertyAddress(kCMIOStreamPropertyScheduledOutputNotificationProc), 0, NULL, size, &scheduledOutputNotificationProcAndRefCon);
	}

	#pragma mark -
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetNameForTerminalType()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void Stream::GetNameForTerminalType(UInt32 terminalType, char* name, UInt32 maxNameLen)
	{
		switch(terminalType)
		{
			//	types that go nowhere
			case OUTPUT_NULL:
				snprintf(name, maxNameLen, "%s", "Null Output");
				break;

			case INPUT_NULL:
				snprintf(name, maxNameLen, "%s", "Null Input");
				break;

			//	Input terminal types
			case INPUT_UNDEFINED:
				snprintf(name, maxNameLen, "%s", "Undefined Input");
				break;

			case INPUT_MICROPHONE:
				snprintf(name, maxNameLen, "%s", "Microphone");
				break;

			case INPUT_DESKTOP_MICROPHONE:
				snprintf(name, maxNameLen, "%s", "Desktop Microphone");
				break;

			case INPUT_PERSONAL_MICROPHONE:
				snprintf(name, maxNameLen, "%s", "Personal Microphone");
				break;

			case INPUT_OMNIDIRECTIONAL_MICROPHONE:
				snprintf(name, maxNameLen, "%s", "Omnidirectional Microphone");
				break;

			case INPUT_MICROPHONE_ARRAY:
				snprintf(name, maxNameLen, "%s", "Microphone Array");
				break;

			case INPUT_PROCESSING_MICROPHONE_ARRAY:
				snprintf(name, maxNameLen, "%s", "Processing Microphone Array");
				break;

			case INPUT_MODEM_AUDIO:
				snprintf(name, maxNameLen, "%s", "Modem");
				break;
			
			//	Output terminal types
			case OUTPUT_UNDEFINED:
				snprintf(name, maxNameLen, "%s", "Undefined Output");
				break;
			
			case OUTPUT_SPEAKER:
				snprintf(name, maxNameLen, "%s", "Speaker");
				break;
			
			case OUTPUT_HEADPHONES:
				snprintf(name, maxNameLen, "%s", "Headphones");
				break;
			
			case OUTPUT_HEAD_MOUNTED_DISPLAY_AUDIO:
				snprintf(name, maxNameLen, "%s", "Head-Monted Display");
				break;
			
			case OUTPUT_DESKTOP_SPEAKER:
				snprintf(name, maxNameLen, "%s", "Desktop Speaker");
				break;
			
			case OUTPUT_ROOM_SPEAKER:
				snprintf(name, maxNameLen, "%s", "Room Speaker");
				break;
			
			case OUTPUT_COMMUNICATION_SPEAKER:
				snprintf(name, maxNameLen, "%s", "Communication Speaker");
				break;
			
			case OUTPUT_LOW_FREQUENCY_EFFECTS_SPEAKER:
				snprintf(name, maxNameLen, "%s", "LFE Speaker");
				break;
			
			//	Bi-directional terminal types
			case BIDIRECTIONAL_UNDEFINED:
				snprintf(name, maxNameLen, "%s", "Undefined Bidirectional");
				break;
			
			case BIDIRECTIONAL_HANDSET:
				snprintf(name, maxNameLen, "%s", "Handset");
				break;
			
			case BIDIRECTIONAL_HEADSET:
				snprintf(name, maxNameLen, "%s", "Headset");
				break;
			
			case BIDIRECTIONAL_SPEAKERPHONE_NO_ECHO_REDX:
				snprintf(name, maxNameLen, "%s", "Speakerphone w/o Echo Reduction");
				break;
			
			case BIDIRECTIONAL_ECHO_SUPPRESSING_SPEAKERPHONE:
				snprintf(name, maxNameLen, "%s", "Speakerphone w/ Echo Suppression");
				break;
			
			case BIDIRECTIONAL_ECHO_CANCELING_SPEAKERPHONE:
				snprintf(name, maxNameLen, "%s", "Speakerphone w/ Echo Cacellation");
				break;
			
			//	Telephony terminal types
			case TELEPHONY_UNDEFINED:
				snprintf(name, maxNameLen, "%s", "Undefined telephony");
				break;
			
			case TELEPHONY_PHONE_LINE:
				snprintf(name, maxNameLen, "%s", "Telephone Line");
				break;
			
			case TELEPHONY_TELEPHONE:
				snprintf(name, maxNameLen, "%s", "Telephone");
				break;
			
			case TELEPHONY_DOWN_LINE_PHONE:
				snprintf(name, maxNameLen, "%s", "Down Line Telephone");
				break;
			
			//	External terminal types
			case EXTERNAL_UNDEFINED:
				snprintf(name, maxNameLen, "%s", "Undefined External");
				break;
			
			case EXTERNAL_ANALOG_CONNECTOR:
				snprintf(name, maxNameLen, "%s", "Analog");
				break;
			
			case EXTERNAL_DIGITAL_AUDIO_INTERFACE:
				snprintf(name, maxNameLen, "%s", "Digital Interface");
				break;
			
			case EXTERNAL_LINE_CONNECTOR:
				snprintf(name, maxNameLen, "%s", "Line");
				break;
			
			case EXTERNAL_LEGACY_AUDIO_CONNECTOR:
				snprintf(name, maxNameLen, "%s", "Legacy");
				break;
			
			case EXTERNAL_SPDIF_INTERFACE:
				snprintf(name, maxNameLen, "%s", "SPDIF");
				break;
			
			case EXTERNAL_1394_DA_STREAM:
				snprintf(name, maxNameLen, "%s", "1394 Digital CMIO");
				break;
			
			case EXTERNAL_1394_DV_STREAM_SOUNDTRACK:
				snprintf(name, maxNameLen, "%s", "1394 Digital Video");
				break;
			
			//	Embedded terminal types
			case EMBEDDED_UNDEFINED:
				snprintf(name, maxNameLen, "%s", "Undefined Embedded");
				break;
			
			case EMBEDDED_LEVEL_CALIBRATION_NOISE_SOURCE:
				snprintf(name, maxNameLen, "%s", "Level Calibration Noise Source");
				break;
			
			case EMBEDDED_EQUALIZATION_NOISE:
				snprintf(name, maxNameLen, "%s", "Equalization Noise");
				break;
			
			case EMBEDDED_CD_PLAYER:
				snprintf(name, maxNameLen, "%s", "CD Player");
				break;
			
			case EMBEDDED_DAT:
				snprintf(name, maxNameLen, "%s", "DAT");
				break;
			
			case EMBEDDED_DCC:
				snprintf(name, maxNameLen, "%s", "DCC");
				break;
			
			case EMBEDDED_MINIDISK:
				snprintf(name, maxNameLen, "%s", "Minidisk");
				break;
			
			case EMBEDDED_ANALOG_TAPE:
				snprintf(name, maxNameLen, "%s", "Analog Tape");
				break;
			
			case EMBEDDED_PHONOGRAPH:
				snprintf(name, maxNameLen, "%s", "Phonograph");
				break;
			
			case EMBEDDED_VCR_AUDIO:
				snprintf(name, maxNameLen, "%s", "VCR");
				break;
			
			case EMBEDDED_VIDEO_DISC_AUDIO:
				snprintf(name, maxNameLen, "%s", "Video Disc");
				break;
			
			case EMBEDDED_DVD_AUDIO:
				snprintf(name, maxNameLen, "%s", "DVD");
				break;
			
			case EMBEDDED_TV_TUNER_AUDIO:
				snprintf(name, maxNameLen, "%s", "TV Tuner");
				break;
			
			case EMBEDDED_SATELLITE_RECEIVER_AUDIO:
				snprintf(name, maxNameLen, "%s", "Satellite Receiver");
				break;
			
			case EMBEDDED_CABLE_TUNER_AUDIO:
				snprintf(name, maxNameLen, "%s", "Cable Tuner");
				break;
			
			case EMBEDDED_DSS_AUDIO:
				snprintf(name, maxNameLen, "%s", "DSS");
				break;
			
			case EMBEDDED_RADIO_RECEIVER:
				snprintf(name, maxNameLen, "%s", "Radio Receiver");
				break;
			
			case EMBEDDED_RADIO_TRANSMITTER:
				snprintf(name, maxNameLen, "%s", "Radio Transmitter");
				break;
			
			case EMBEDDED_MULTITRACK_RECORDER:
				snprintf(name, maxNameLen, "%s", "Mutitrack Recorder");
				break;
			
			case EMBEDDED_SYNTHESIZER:
				snprintf(name, maxNameLen, "%s", "Synthesizer");
				break;
			
			//	Processing terminal types
			case PROCESSOR_UNDEFINED:
				snprintf(name, maxNameLen, "%s", "Undefined Processor");
				break;
			
			case PROCESSOR_GENERAL:
				snprintf(name, maxNameLen, "%s", "General Processor");
				break;
			
			default:
				snprintf(name, maxNameLen, "0x%X", (unsigned int)terminalType);
				break;
			
		};
	}
}}
