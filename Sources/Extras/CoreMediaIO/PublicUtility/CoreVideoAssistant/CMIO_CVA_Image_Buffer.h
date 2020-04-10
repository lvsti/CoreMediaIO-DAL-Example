/*
	    File: CMIO_CVA_Image_Buffer.h
	Abstract: C++ wrapper for CVImageBufferRef
	 Version: 1.2

*/

#if !defined(__CMIO_CVA_Image_Buffer_h__)
#define __CMIO_CVA_Image_Buffer_h__

// Super Class Include
#include "CMIO_CVA_Buffer.h"

// System Includes
#include <CoreVideo/CVImageBuffer.h>

namespace CMIO { namespace CVA { namespace Image
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Buffer
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class Buffer : public CVA::Buffer
	{
	// Construction/Destruction
	public:
							Buffer(CVImageBufferRef ref = NULL, bool retain = false, bool release = true) : CVA::Buffer(ref, retain, release) {}
		virtual				~Buffer() { Reset(); }
		Buffer&				operator=(const Buffer& that) { if (this != &that) this->Reset(that.Get(), true, that.WillRelease());  return *this; }

	// Value Access
	public:
		operator			CVImageBufferRef() const { return CVA::Buffer::Get(); }
		CVImageBufferRef	Get() const { return CVA::Buffer::Get(); }
	};
}}}
#endif
