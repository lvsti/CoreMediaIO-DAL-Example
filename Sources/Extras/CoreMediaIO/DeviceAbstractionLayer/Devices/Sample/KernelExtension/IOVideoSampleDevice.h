/*
	    File: IOVideoSampleDevice.h
	Abstract: n/a
	 Version: 1.2

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
	virtual void				free(void) override;
	virtual bool				start(IOService* provider) override;
	virtual void				stop(IOService* provider) override;

    // IOVideoStream methods
	virtual IOReturn			startStream(IOVideoStream* stream) override;
    virtual IOReturn			stopStream(IOVideoStream* stream) override;
    virtual IOReturn			suspendStream(IOVideoStream* stream) override;
    virtual IOReturn			setControlValue(UInt32 controlID, UInt32 value, UInt32* newValue) override;
	virtual IOReturn			setStreamFormat(UInt32 streamID, const IOVideoStreamDescription* newStreamFormat) override;

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
