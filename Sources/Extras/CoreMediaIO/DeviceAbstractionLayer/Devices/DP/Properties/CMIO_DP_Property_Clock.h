/*
	    File: CMIO_DP_Property_Clock.h
	Abstract: Implements the kCMIOStreamPropertyClock property.
	 Version: 1.2

*/

#if !defined(__CMIO_DP_Property_Clock_h__)
#define __CMIO_DP_Property_Clock_h__

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Super Class Includes
#include "CMIO_DP_Property_Base.h"

namespace CMIO { namespace DP
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Types in the CMIO::DP namespace
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class Stream;
}};

namespace CMIO { namespace DP { namespace Property
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Clock
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class Clock : public Base
	{
	// Construction/Destruction
	public:
						Clock(Stream* owningStream) : Base(), mOwningStream(owningStream), mClock(NULL) {}
		virtual			~Clock() { if (NULL != mClock) CFRelease(mClock); mClock = NULL; }
	
	// Property Operations
	public:
		virtual bool	IsActive(const CMIOObjectPropertyAddress& address) const;
		virtual bool	IsPropertySettable(const CMIOObjectPropertyAddress& address) const;
		virtual UInt32	GetPropertyDataSize(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData) const;
		virtual void	GetPropertyData(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData, UInt32 dataSize, UInt32& dataUsed, void* data) const;
		virtual void	SetPropertyData(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData, UInt32 dataSize, const void* data);

		virtual UInt32	GetNumberAddressesImplemented() const;
		virtual void	GetImplementedAddressByIndex(UInt32 index, CMIOObjectPropertyAddress& address) const;
	
	// Property Implementation
	public:
		void			SetClock(CFTypeRef clock) { if (NULL != mClock) { CFRelease(mClock); mClock = NULL; } if (NULL != clock) mClock = CFRetain(clock); }
		CFTypeRef		GetClock() { return mClock; }
	
	private:
		Stream*			mOwningStream;
		CFTypeRef		mClock;
	};
}}}
#endif
