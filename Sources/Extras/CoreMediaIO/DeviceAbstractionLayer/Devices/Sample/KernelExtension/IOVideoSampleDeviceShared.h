/*
	    File: IOVideoSampleDeviceShared.h
	Abstract: n/a
	 Version: 1.2

*/

#if !defined(__IOVideoSampleDeviceShared_h__)
#define __IOVideoSampleDeviceShared_h__

/*!
 @enum		CodecFlags.
 @discussion .
 */
enum
{
	kSampleCodecNoFlags                                 = 0,			/*! @constant kSampleCodecNoFlags							no rates supported. */
	
	kSampleCodecFlags_24fps                             = (1L << 0),	/*! @constant kSampleCodecFlags_24fps		0x00000001  framerate 24fps. */
	kSampleCodecFlags_25fps                             = (1L << 1),	/*! @constant kSampleCodecFlags_25fps       0x00000002  framerate 25fps. */
	kSampleCodecFlags_30fps                             = (1L << 2),	/*! @constant kSampleCodecFlags_30fps		0x00000004  framerate 30fps. */
	kSampleCodecFlags_50fps                             = (1L << 3),	/*! @constant kSampleCodecFlags_50fps		0x00000008  framerate 50fps. */
	kSampleCodecFlags_60fps                             = (1L << 4),	/*! @constant kSampleCodecFlags_60fps		0x00000010  framerate 60fps. */
    
	kSampleCodecFlags_1001_1000_adjust                  = (1L << 16),	/*! @constant kSampleCodecFlags_1001_1000_adjust	0x00010000  multiply framerate by 1000/1001. */
    kSampleCodecFlags_psf_frame                         = (1L << 17)    /*! @constant kSampleCodecFlags_psf_frame			0x00020000  frame is psf. */
};

enum FrameType
{
	kYUV422_720x480,
	kYUV422_720x486,
	kYUV422_720x576,
	kYUV422_1280x720,
	kYUV422_1920x1080,
	kYUV422_10_720x480,
	kYUV422_10_720x486,
	kYUV422_10_720x576,
	kYUV422_10_1280x720,
	kYUV422_10_1920x1080,
	kFrameTypePad = 0xFFFFFFFFUL
};

#endif
