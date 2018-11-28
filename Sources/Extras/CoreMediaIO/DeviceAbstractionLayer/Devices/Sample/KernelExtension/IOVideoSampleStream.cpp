/*
	    File: IOVideoSampleStream.cpp
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

// Self Include
#include "IOVideoSampleStream.h"

// Internal Includes
#include "IOVideoSampleDevice.h"

// System Includes
#include <IOKit/usb/IOUSBLog.h>

#define super IOVideoStream
OSDefineMetaClassAndStructors( IOVideoSampleStream, IOVideoStream)

IOVideoSampleStream* IOVideoSampleStream::withBuffers(OSArray* buffers, IOStreamMode mode, IOItemCount queueLength, OSDictionary* properties)
{
    USBLog(1, "IOVideoSampleStream::withBuffers\n");
    IOVideoSampleStream* stream = new IOVideoSampleStream;
    
    if (stream)
	{
        if (stream->initWithBuffers(buffers, mode, queueLength, properties))
            return stream;
        
        stream->release();
    }
	
    return NULL;
}

bool IOVideoSampleStream::initWithBuffers(OSArray* buffers, IOStreamMode mode, IOItemCount queueLength, OSDictionary* properties)
{
    USBLog(1, "IOVideoSampleStream::initWithBuffers\n");
	
	OSDictionary* props = (OSDictionary*) properties->copyCollection();
	
    if (!super::initWithBuffers(buffers, mode, queueLength, props))
		return false;
    
    mStreamMode = mode;
	OSNumber* theStreamID = OSDynamicCast(OSNumber, properties->getObject(kIOVideoStreamKey_StreamID));
	if (NULL != theStreamID)
		setProperty(kIOVideoStreamKey_StreamID, (OSObject*)theStreamID);
	
	if (props)
		props->release();
	
	_freeBuffers = OSArray::withArray(buffers);
	if (!_freeBuffers)
		return false;

	_filledBuffers = OSArray::withCapacity(_freeBuffers->getCapacity());
	if (!_filledBuffers)
		return false;
		
    return true;
}

void IOVideoSampleStream::free(void)
{
    USBLog(1, "IOVideoSampleStream::free\n");
	
	if (_freeBuffers)
	{
		_freeBuffers->release();
		_freeBuffers = 0;
	}
	
	if (_filledBuffers)
	{
		_filledBuffers->release();
		_filledBuffers = 0;
	}

	super::free();
}

bool IOVideoSampleStream::handleOpen(IOService* forClient, IOOptionBits options, void* arg)
{
    USBLog(1, "IOVideoSampleStream::handleOpen\n");

	if (!super::handleOpen(forClient, options, arg))
	{
		USBLog(1, "super::handleOpen failed\n");
		return false;
	}
	
	// free all buffers
	_filledBuffers->flushCollection();
	_freeBuffers->flushCollection();
	_freeBuffers->merge(_buffers);
	
	return true;
}

void IOVideoSampleStream::handleClose(IOService* forClient, IOOptionBits options)
{
    USBLog(1, "IOVideoSampleStream::handleClose\n");
	
	super::handleClose(forClient, options);
	
	// free all buffers
	_filledBuffers->flushCollection();
	_freeBuffers->flushCollection();
	_freeBuffers->merge(_buffers);
}

void IOVideoSampleStream::inputCallback(UInt32 token)
{
    USBLog(1, "IOVideoSampleStream::inputCallback(%d)\n", (int)token);

	IOReturn result;
	if (mStreamMode == kIOStreamModeInput)
	{
		do
		{
			IOStreamBufferQueueEntry entry;
			result = dequeueInputEntry(&entry);
			if (result == kIOReturnSuccess)
			{
				IOStreamBuffer *buf = getBufferWithID(entry.bufferID);
				if (NULL == buf)
				{
					USBLog(1, "IOVideoSampleStream::inputCallback Invalid Buffer ID %d\n", (int)entry.bufferID);
					break;
				}
				
				_filledBuffers->setObject(buf);
			}
		} while (result == kIOReturnSuccess);
	}
}

void IOVideoSampleStream::inputSyncCallback(UInt32 token)
{
    USBLog(1, "IOVideoSampleStream::inputSyncCallback(%d)\n", (int)token);
}

IOStreamBuffer* IOVideoSampleStream::getFilledBuffer()
{
	IOStreamBuffer* streamBuffer = OSDynamicCast(IOStreamBuffer, _filledBuffers->getObject(0));
	USBLog(1, "IOVideoSampleStream::getFilledBuffer buffer=%llx", (long long unsigned int)streamBuffer);
	
	if (NULL != streamBuffer)
		_filledBuffers->removeObject (0);

	return streamBuffer;
}

void IOVideoSampleStream::returnBufferToFreeQueue(IOStreamBuffer* buffer)
{
	if (NULL == buffer)
	{
		USBLog(1, "IOVideoSampleStream::returnBufferToFreeQueue - NULL == returnBufferToFreeQueue\n");
		return;
	}
	
	_freeBuffers->setObject(buffer);
}

void IOVideoSampleStream::postFreeInputBuffer(UInt64 vbiTime, UInt64 outputTime, UInt64 totalFrameCount, UInt64 droppedFrameCount,UInt64 lastDisplayedSequenceNumber)
{
	IOStreamBuffer* buf = OSDynamicCast(IOStreamBuffer, _freeBuffers->getObject(0));
	if (NULL == buf)
	{
		USBLog(1, "IOVideoSampleStream::postFreeInputBuffer - no free buffers\n");
		if ((getOutputQueue())->entryCount > 0)
		{
			//all the free buffers are in the queue already so just alert the host
			(void) sendOutputNotification();
		}
		return;
	}
	
	_freeBuffers->removeObject(0);

	IOMemoryDescriptor *ctrlDescriptor = OSDynamicCast(IOMemoryDescriptor, buf->getControlBuffer());
	if (NULL != ctrlDescriptor)
	{
		USBLog(1, "IOVideoSampleStream got control buffer descriptor\n");
		SampleVideoDeviceControlBuffer theBuffControl, readBackControl;
		USBLog(1, "IOVideoSampleStream::postFreeInputBuffer - passed in vbiTime = %lld outputTime = %lld framecount = %lld droppedframecount = %lld lastDisplayedSequenceNumber = %lld  \n", vbiTime, outputTime, totalFrameCount, droppedFrameCount, lastDisplayedSequenceNumber);

		
		theBuffControl.vbiTime = vbiTime; 
		theBuffControl.outputTime = outputTime; 
		theBuffControl.totalFrameCount = totalFrameCount; 
		theBuffControl.droppedFrameCount = droppedFrameCount; 
		theBuffControl.firstVBITime = 0;
		theBuffControl.sequenceNumber = lastDisplayedSequenceNumber;
		theBuffControl.discontinuityFlags = 0;
		
		(void) ctrlDescriptor->prepare();
		
        ctrlDescriptor->writeBytes(0, &theBuffControl, buf->getControlBuffer()->getLength());
		ctrlDescriptor->readBytes(0, &readBackControl, buf->getControlBuffer()->getLength());
		USBLog(1, "IOVideoSampleStream::postFreeInputBuffer - control buffer info vbiTime = %lld outputTime = %lld framecount = %lld droppedframecount = %lld  sequencenumber = %lld\n", readBackControl.vbiTime, readBackControl.outputTime, readBackControl.totalFrameCount, readBackControl.droppedFrameCount,readBackControl.sequenceNumber);
		(void) ctrlDescriptor->complete();
	}
		
	IOReturn result = enqueueOutputBuffer(buf, 0, buf->getDataBuffer()->getLength(), 0, buf->getControlBuffer()->getLength()); 
	if (result != kIOReturnSuccess)
	{
		USBLog(1, "IOVideoSampleStream::postFreeInputBuffer help enqueueOutputBuffer failed! (%x)\n", result);
		return;
	}
	
	result = sendOutputNotification();
	if (result != kIOReturnSuccess)
	{
		USBLog(1, "IOVideoSampleStream::postFreeInputBuffer help sendOutputNotification failed! (%x)\n", result);
		return;
	}
}
