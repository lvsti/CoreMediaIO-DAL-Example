/*
	    File: IOVideoSampleStream.h
	Abstract: n/a
	 Version: 1.2

*/

#if !defined(__IOVideoSampleStream_h__)
#define __IOVideoSampleStream_h__

// System Includes
#include <IOKit/video/IOVideoStream.h>

class IOVideoSampleStream : public IOVideoStream
{
	OSDeclareDefaultStructors(IOVideoSampleStream);

protected:
	OSArray*					_freeBuffers;
	OSArray*					_filledBuffers;
	
public:
	virtual void				free(void) override;
	
	static IOVideoSampleStream*	withBuffers(OSArray* buffers, IOStreamMode mode = kIOStreamModeOutput, IOItemCount queueLength = 0, OSDictionary* properties = 0);
	
	virtual bool				initWithBuffers(OSArray* buffers, IOStreamMode mode = kIOStreamModeOutput, IOItemCount queueLength = 0, OSDictionary* properties = 0) override;
	
	virtual bool				handleOpen(IOService* forClient, IOOptionBits options, void* arg) override;
	virtual void				handleClose(IOService* forClient, IOOptionBits options = 0) override;
	
	virtual void				inputCallback(UInt32 token) override;
	virtual void				inputSyncCallback(UInt32 token) override;
	
	IOStreamBuffer*				getFilledBuffer();
	void						returnBufferToFreeQueue(IOStreamBuffer* buffer);
	void						postFreeInputBuffer(UInt64 vbiTime, UInt64 outputTime, UInt64 totalFrameCount, UInt64 droppedFrameCount,UInt64 lastDisplayedSequenceNumber);
};

#endif
