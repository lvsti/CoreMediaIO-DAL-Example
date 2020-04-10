/*
	    File: CMIO_DP_Property_ScheduledOutputNotificationProc.h
	Abstract: Implements the kCMIOStreamPropertyScheduledOutputNotificationProc property.
	 Version: 1.2

*/

#if !defined(__CMIO_DP_Property_ScheduledOutputNotificationProc_h__)
#define __CMIO_DP_Property_ScheduledOutputNotificationProc_h__

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Super Class Includes
#include "CMIO_DP_Property_Base.h"

// System Includes
#include <CoreMediaIO/CMIOHardwareStream.h>

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
	// ScheduledOutputNotificationProc
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class ScheduledOutputNotificationProc : public Base
	{
	//	Construction/Destruction
	public:
						ScheduledOutputNotificationProc(DP::Stream& owningStream) : Base(), mOwningStream(owningStream) { bzero(&mScheduledOutputNotificationProc, sizeof(mScheduledOutputNotificationProc)); }
	virtual				~ScheduledOutputNotificationProc() {}
	
	private:
		DP::Stream&		GetOwningStream() const { return mOwningStream; }
		DP::Stream&		mOwningStream;
	
	// Property Operations
	public:
		virtual bool	IsPropertySettable(const CMIOObjectPropertyAddress& address) const { return true; }
		virtual UInt32	GetPropertyDataSize(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData) const;
		virtual void	GetPropertyData(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData, UInt32 dataSize, UInt32& dataUsed, void* data) const;
		virtual void	SetPropertyData(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData, UInt32 dataSize, const void* data);
	
		virtual UInt32	GetNumberAddressesImplemented() const { return 1; }
		virtual void	GetImplementedAddressByIndex(UInt32 index, CMIOObjectPropertyAddress& address) const;
	
	//	Property Implementation
		void			SendNotification(UInt64 sequenceNumberOfBufferThatWasOutput, UInt64 outputHostTime);
	
	private:
		CMIOStreamScheduledOutputNotificationProcAndRefCon	mScheduledOutputNotificationProc;
	};
}}}
#endif
