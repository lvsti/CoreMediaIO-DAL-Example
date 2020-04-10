/*
	    File: CMIO_DALA_Stream.h
	Abstract: C++ wrapper for CMIOStreamID
	 Version: 1.2

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
