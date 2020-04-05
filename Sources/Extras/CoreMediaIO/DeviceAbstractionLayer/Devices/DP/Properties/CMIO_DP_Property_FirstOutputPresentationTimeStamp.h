/*
	    File: CMIO_DP_Property_FirstOutputPresentationTimeStamp.h
	Abstract: Implements the kCMIOStreamPropertyFirstOutputPresentationTimeStamp property.
	 Version: 1.2

*/

#if !defined(__CMIO_DP_Property_FirstOutputPresentationTimeStamp_h__)
#define __CMIO_DP_Property_FirstOutputPresentationTimeStamp_h__

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Super Class Includes
#include "CMIO_DP_Property_Base.h"

// System Includes
#include <CoreMedia/CMTime.h>

namespace CMIO { namespace DP
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Types in the CMIO::DP namespace
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class Stream;
}}

namespace CMIO { namespace DP { namespace Property
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// FirstOutputPresentationTimeStamp
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class FirstOutputPresentationTimeStamp : public Base
	{
	//	Construction/Destruction
	public:
						FirstOutputPresentationTimeStamp(DP::Stream& owningStream) : Base(), mOwningStream(owningStream), mFirstOutputPresentationTimeStamp(kCMTimeInvalid) {}
	virtual				~FirstOutputPresentationTimeStamp() {}
	
	private:
		DP::Stream&		GetOwningStream() const { return mOwningStream; }
		DP::Stream&		mOwningStream;
	
	// Property Operations
	public:
		virtual bool	IsPropertySettable(const CMIOObjectPropertyAddress& address) const { return false; }
		virtual UInt32	GetPropertyDataSize(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData) const;
		virtual void	GetPropertyData(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData, UInt32 dataSize, UInt32& dataUsed, void* data) const;
	
		virtual UInt32	GetNumberAddressesImplemented() const { return 1; }
		virtual void	GetImplementedAddressByIndex(UInt32 index, CMIOObjectPropertyAddress& address) const;
	
	//	Property Implementation
		void			SetFirstOutputPresentationTimeStamp(CMTime FirstOutputPresentationTimeStamp);
		CMTime			GetFirstOutputPresentationTimeStamp() const { return mFirstOutputPresentationTimeStamp; }
	
	private:
		CMTime			mFirstOutputPresentationTimeStamp;
	};
}}}
#endif
