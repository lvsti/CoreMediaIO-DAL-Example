/*
	    File: IOVideoSampleDevice.h
	Abstract: n/a
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

#if !defined(__IOVideoSampleDevice_h__)
#define __IOVideoSampleDevice_h__

#include "IOVideoSampleStream.h"
#include <IOKit/IOWorkLoop.h>
#include <IOKit/IOTimerEventSource.h>
#include <IOKit/video/IOVideoDevice.h>
#include <IOKit/video/IOVideoStream.h>
#include <IOKit/audio/IOAudioTypes.h>

#if !defined(START_SENDING_ON_TWO_SECOND_BOUNDRY)
	#define START_SENDING_ON_TWO_SECOND_BOUNDRY	1
#endif



struct SampleVideoDeviceControlBuffer
{
	UInt64	vbiTime;
	UInt64	outputTime;
	UInt64	totalFrameCount;
	UInt64	droppedFrameCount;
	UInt64	firstVBITime;
    UInt64  sequenceNumber;
    UInt32  discontinuityFlags;
    IOAudioSMPTETime   smpteTime;
};

class IOVideoSampleDevice : IOVideoDevice
{
								OSDeclareDefaultStructors(IOVideoSampleDevice);

protected:
    IOStreamBufferID			_currentBuffer;
    IOVideoSampleStream*		_outputStream;
    IOVideoSampleStream*		_inputStream;
    
    IOWorkLoop*					_workloop;
    IOTimerEventSource*			_timer;
	char*						mYUVData;
	size_t						mYUVSize;
	char*						mHD720pYUVData;
	size_t						mHD720pYUVSize;
	char*						mHD1080pYUVData;
	size_t						mHD1080pYUVSize;
	bool						mCurrentDirection;
	UInt32						mNumBuffers;
	UInt32						mMaxNumBuffers;
    UInt32                      mTimeInterval;

	IOBufferMemoryDescriptor*	mMemDescFrameBuf;
	IOMemoryMap*				mMemMapFrameBuf;
	UInt8*						mFrameBuf;
	size_t						mFrameBufSize;

	IOBufferMemoryDescriptor*	mMemDescControlBuf;
	IOMemoryMap*				mMemMapControlBuf;
	UInt8*						mControlBuf;
	size_t						mControlBufSize;
	UInt64						mFrameCountFromStart;
	UInt32						mDroppedFrameCount;
    UInt64                      mLastDisplayedSequenceNumber;

    static void					timerFired(OSObject* owner, IOTimerEventSource* timer);
    
    IOReturn					sendOutputFrame(void);
    IOReturn					consumeInputFrame(void);
        
public:
    // IOKit methods
	virtual void				free(void);
	virtual bool				start(IOService* provider);
	virtual void				stop(IOService* provider);

    // IOVideoStream methods
	virtual IOReturn			startStream(IOVideoStream* stream);
    virtual IOReturn			stopStream(IOVideoStream* stream);
    virtual IOReturn			suspendStream(IOVideoStream* stream);
    virtual IOReturn			setControlValue(UInt32 controlID, UInt32 value, UInt32* newValue);
	virtual IOReturn			setStreamFormat(UInt32 streamID, const IOVideoStreamDescription* newStreamFormat);

private:
	static OSDictionary*		createDefaultInputStreamDictionary();
	static OSDictionary*		createDefaultOutputStreamDictionary();

	bool						AddInputStreams();
	bool						AddOutputStreams();

	bool						RemoveInputStreams();
	bool						RemoveOutputStreams();

	bool						ResetInputStreams();
	bool						ResetOutputStreams();

	IOReturn					AllocateFrameBuffers(size_t size);
	IOReturn					AllocateControlBuffers(size_t size);
	void						LoadFrameBuffer(void* buffer, char* src, IOByteCount frameSize, SInt32 number);
    void                        ZeroControlBuffer(void* buffer, IOByteCount bufSize);
    UInt32                      CodecFlagsToFrameRate(UInt32 codecFlags);

	IOVideoStreamDescription	mCurrentInputStreamFormat;
	IOVideoStreamDescription	mCurrentOutputStreamFormat;
	
#if START_SENDING_ON_TWO_SECOND_BOUNDRY
	uint64_t					mFirstInputAtTwoSecondBoundaryHostTimeUptime;
	uint64_t					mHostTimeFudge;
	bool						mWaitingToStart;
	bool						mCalculateHostTimeFudge;
#endif
};

#endif
