/*
	    File: CMIO_CVA_Pixel_Buffer.h
	Abstract: C++ wrapper for CVPixelBufferRef
	 Version: 1.2

*/

#if !defined(__CMIO_CVA_Pixel_Buffer_h__)
#define __CMIO_CVA_Pixel_Buffer_h__

// Super Class Include
#include "CMIO_CVA_Image_Buffer.h"

// CA Public Utility
#include "CAException.h"

// System Includes
#include <CoreVideo/CVPixelBuffer.h>

namespace CMIO { namespace CVA { namespace Pixel
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Buffer
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class Buffer : public Image::Buffer
	{
	// Construction/Destruction
	public:
									Buffer(CVPixelBufferRef ref = NULL, bool retain = false, bool release = true) : Image::Buffer(ref, retain, release) {}
		virtual						~Buffer() { Reset(); }
		Buffer&						operator=(const Buffer& that) { if (this != &that) this->Reset(that.Get(), true, that.WillRelease());  return *this; }

	public:
		static CVPixelBufferRef		CreateWithBytes(CFAllocatorRef allocator, size_t width, size_t height, OSType pixelFormatType, void *baseAddress, size_t bytesPerRow,CVPixelBufferReleaseBytesCallback releaseCallback, void *releaseRefCon, CFDictionaryRef pixelBufferAttributes)
									{
										CVPixelBufferRef pixelBufferRef;
										CVReturn err = CVPixelBufferCreateWithBytes(allocator, width, height, pixelFormatType, baseAddress,bytesPerRow, releaseCallback, releaseRefCon, pixelBufferAttributes, &pixelBufferRef);
										ThrowIfError(err, CAException(err), "CMIO::CVA::Pixel::Buffer::CreateWithBytes: CVPixelBufferCreateWithBytes() failed");
										return pixelBufferRef; 
									}
	
		static CVPixelBufferRef		Create(CFAllocatorRef allocator, size_t width, size_t height, OSType pixelFormatType, CFDictionaryRef pixelBufferAttributes)
									{
										CVPixelBufferRef pixelBufferRef;
										CVReturn err = CVPixelBufferCreate(allocator, width, height, pixelFormatType, pixelBufferAttributes, &pixelBufferRef);
										ThrowIfError(err, CAException(err), "CMIO::PixelBuffer::Create: CVPixelBufferCreate() failed");
										return pixelBufferRef; 
									}

		static CVPixelBufferRef		CreateFromIOSurface(CFAllocatorRef allocator, IOSurfaceRef surface, CFDictionaryRef pixelBufferAttributes)
									{
										CVPixelBufferRef pixelBufferRef;
										CVReturn err = CVPixelBufferCreateWithIOSurface(allocator, surface, pixelBufferAttributes, &pixelBufferRef);
										ThrowIfError(err, CAException(err), "CMIO::CVA::Pixel::Buffer::CreateFromIOSurface: CVPixelBufferCreateWithIOSurface() failed");
										return pixelBufferRef; 
									}

	// Value Access
	public:
		operator					CVPixelBufferRef() const { return Image::Buffer::Get(); }
		CVPixelBufferRef			Get() const { return Image::Buffer::Get(); }

	// Operations
	public:
		void						LockBaseAddress(CVOptionFlags lockFlags) { CVReturn err = CVPixelBufferLockBaseAddress(Get(), lockFlags); ThrowIfError(err, CAException(err), "CMIO::CVA::Pixel::Buffer::LockBaseAddres: CVPixelBufferLockBaseAddress() failed"); }
		void						UnlockBaseAddress(CVOptionFlags unlockFlags) { CVReturn err = CVPixelBufferUnlockBaseAddress(Get(), unlockFlags); ThrowIfError(err, CAException(err), "CMIO::CVA::Pixel::Buffer::UnlockBaseAddress: CVPixelBufferUnlockBaseAddress() failed"); }

	// Helper class to manage locking and then unlocking automatically when going out of scope
	public:
		class BaseAddressLocker
		{
			// Construction/Destruction
			public:
									BaseAddressLocker(Buffer& buffer, CVOptionFlags lockFlags) : mBuffer(buffer), mLockFlags(lockFlags) { mBuffer.LockBaseAddress(mLockFlags); }
									~BaseAddressLocker() { mBuffer.UnlockBaseAddress(mLockFlags); }
			
			private:
									BaseAddressLocker(const BaseAddressLocker&);
				BaseAddressLocker&	operator=(const BaseAddressLocker&);
					
			//	Implementation
			private:
				Buffer&				mBuffer;
				CVOptionFlags		mLockFlags;
		};
	};
}}}
#endif
