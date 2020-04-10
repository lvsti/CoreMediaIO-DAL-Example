/*
	    File: CMIO_BitField.h
	Abstract: C bit fields are not strictly defined by ANSI, hence they vary between compiler implementations.  Some implementations start at the most significant bit and work towards the
				least significant, whereas others start at the least significant bit and work towards the most significant.  By using the CMIO::BitField32 class, the implementation is
				explicitly controlled.
	
	 Version: 1.2

*/

#if !defined(__CMIO_BitField_h__)
#define __CMIO_BitField_h__

#include <CoreFoundation/CFBase.h>

namespace CMIO
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// BitField32
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class BitField32
	{ 
	// Construction/Destruction
	public:
					BitField32() : mBits(0) {}
					BitField32(UInt32 initialData) : mBits(initialData) {}

	private:
		UInt32		mBits;
		
	// Operations
	public:
		void		Set(UInt32 setData, UInt32 startPosition, UInt32 width) { UInt32 alignedSetData = setData << startPosition; UInt32 alignedFieldMask = ((1 << width) - 1) << startPosition; mBits = (alignedSetData & alignedFieldMask) | (mBits & ~alignedFieldMask); }

	// Value Access
	public:
		operator	UInt32() const { return mBits; }
		UInt32		Get(UInt32 startPosition, UInt32 width) const { return (mBits >> startPosition) & ((1 << width) - 1); }
	};
}
#endif
