/*
	    File: CMIO_IOKA_VirtualRange.h
	Abstract: CMIO::IOKA::VirtualRange extends the wraps IOVirualRange structure to C++ provide constructors.
	 Version: 1.2

*/

/*
	File:		CMIO_IOKA_VirtualRange.h

	Contains:	CMIO::IOKA::VirtualRange extends the wraps IOVirualRange structure to C++ provide constructors.

		$Log$
*/

#if !defined(__CMIO_IOKA_VirtualRange_h__)
#define __CMIO_IOKA_VirtualRange_h__

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// System Includes
#include <IOKit/IOTypes.h>

namespace CMIO { namespace IOKA
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// VirtualRange
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	struct VirtualRange : public IOVirtualRange
	{
	// Construction/Destruction
	public:
		VirtualRange() : IOVirtualRange() { address = IO_OBJECT_NULL; length = 0; }
		VirtualRange(IOVirtualAddress virtualAddress, IOByteCount byteCount) : IOVirtualRange() { address = virtualAddress; length = byteCount; }
	};

}}
#endif

