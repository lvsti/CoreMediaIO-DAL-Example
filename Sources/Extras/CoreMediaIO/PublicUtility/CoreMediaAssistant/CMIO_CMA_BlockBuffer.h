/*
	    File: CMIO_CMA_BlockBuffer.h
	Abstract: C++ wrapper for CMBlockBufferRef
	 Version: 1.2

*/

#if !defined(__CMIO_CMA_BlockBuffer_h__)
#define __CMIO_CMA_BlockBuffer_h__

// Public Utility
#include "CMIODebugMacros.h"

// CA Public Utility
#include "CAException.h"

// System Includes
#include <CoreMedia/CMBlockBuffer.h>

// System Includes

namespace CMIO { namespace CMA
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// BlockBuffer
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class BlockBuffer
	{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// BlockBuffer
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Construction/Destruction
	public:
										BlockBuffer(CMBlockBufferRef ref = NULL, bool retain = false, bool release = true) : mRef(NULL)  { Reset(ref, retain, release); }
										~BlockBuffer() { Reset(); }
		void							Reset(CMBlockBufferRef ref = NULL, bool retain = false, bool release = true) { if (NULL != mRef and mRelease) CFRelease(mRef); if (retain and NULL != ref) CFRetain(ref); mRef = ref; mRelease = release; }
		BlockBuffer&					operator=(const BlockBuffer& that) { if (this != &that) this->Reset(that.Get(), true, that.WillRelease());  return *this; }

	// Excecption throwing versions of the CMBlockBufferCreateXXXXX routines for the underlying CMBlockBufferRef
	public:
		static CMBlockBufferRef		CreateEmpty(CFAllocatorRef structureAllocator, uint32_t subBlockCapacity, CMBlockBufferFlags flags)
										{
											CMBlockBufferRef blockBufferRef;
											OSStatus err = CMBlockBufferCreateEmpty(structureAllocator, subBlockCapacity, flags, &blockBufferRef);
											ThrowIfError(err, CAException(err), "CMIO::BlockBuffer::CreateEmpty: CMBlockBufferCreateEmpty() failed");
											return blockBufferRef; 
										}

		static CMBlockBufferRef		CreateWithMemoryBlock(CFAllocatorRef structureAllocator, void *memoryBlock, size_t blockLength, CFAllocatorRef blockAllocator,
												CMBlockBufferCustomBlockSource *customBlockSource, size_t offsetToData, size_t dataLength, CMBlockBufferFlags flags)
										{
											CMBlockBufferRef blockBufferRef;
											OSStatus err = CMBlockBufferCreateWithMemoryBlock(structureAllocator, memoryBlock, blockLength, blockAllocator, customBlockSource, offsetToData, dataLength, flags, &blockBufferRef);
											ThrowIfError(err, CAException(err), "CMIO::BlockBuffer::CreateWithMemoryBlock: CMBlockBufferCreateWithMemoryBlock() failed");
											return blockBufferRef; 
										}

		static CMBlockBufferRef		CreateWithBufferReference(CFAllocatorRef structureAllocator, CMBlockBufferRef targetBuffer, size_t offsetToData, size_t dataLength, CMBlockBufferFlags flags)
										{
											CMBlockBufferRef blockBufferRef;
											OSStatus err = CMBlockBufferCreateWithBufferReference(structureAllocator, targetBuffer, offsetToData, dataLength, flags,  &blockBufferRef);
											ThrowIfError(err, CAException(err), "CMIO::BlockBuffer::CreateWithBufferReference: CMBlockBufferCreateWithBufferReference() failed");
											return blockBufferRef; 
										}

	private:
		CMBlockBufferRef				mRef;
		bool							mRelease;

	// Attributes
	public:
		bool							IsValid() const { return mRef != NULL; }
		bool							WillRelease() const { return mRelease; }
		void							ShouldRelease(bool release) { mRelease = release; }
	
	// Operations
	public:
		UInt8*							GetDataPointer(size_t offset, size_t* lengthAtOffset, size_t* totalLength)
										{
											char* dataPointer = NULL;
											OSStatus err = CMBlockBufferGetDataPointer(mRef, offset, lengthAtOffset, totalLength, &dataPointer);
											ThrowIfError(err, CAException(err), "CMIO::BlockBuffer::GetDataPointer: CMBlockBufferGetDataPointer() failed");
											return (UInt8*)dataPointer;
										}
										
		size_t							GetDataLength() { return CMBlockBufferGetDataLength(mRef); }
		bool							IsRangeContiguous(size_t offset, size_t length) { return CMBlockBufferIsRangeContiguous(mRef, offset, length); }
		
	// Value Access
	public:
		operator						CMBlockBufferRef() const { return mRef; }
		CMBlockBufferRef				Get() const { return mRef; }		
	};
}}
#endif
