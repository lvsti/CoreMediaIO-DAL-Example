/*
	    File: CMIO_CVA_Buffer.h
	Abstract: C++ wrapper for CVBufferRef
	 Version: 1.2

*/

#if !defined(__CMIO_CVA_Buffer_h__)
#define __CMIO_CVA_Buffer_h__

// Public Utility
#include "CMIODebugMacros.h"

// CA Public Utility
#include "CAException.h"

// System Includes
#include <CoreVideo/CVBuffer.h>

namespace CMIO { namespace CVA
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Buffer
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class Buffer
	{
	// Construction/Destruction
	public:
					Buffer(CVBufferRef ref = NULL, bool retain = false, bool release = true) : mRef(NULL)  { Reset(ref, retain, release); }
		virtual		~Buffer() { Reset(); }
		void		Reset(CVBufferRef ref = NULL, bool retain = false, bool release = true) { if (NULL != mRef and mRelease) CFRelease(mRef); if (retain and NULL != ref) CFRetain(ref); mRef = ref; mRelease = release; }
		Buffer&		operator=(const Buffer& that) { if (this != &that) this->Reset(that.Get(), true, that.WillRelease());  return *this; }

	private:
		CVBufferRef	mRef;
		bool		mRelease;

	// Attributes
	public:
		bool		IsValid() const { return mRef != NULL; }
		bool		WillRelease() const { return mRelease; }
		void		ShouldRelease(bool release) { mRelease = release; }

	// Value Access
	public:
		operator	CVBufferRef() const { return mRef; }
		CVBufferRef	Get() const { return mRef; }
	};
}}
#endif
