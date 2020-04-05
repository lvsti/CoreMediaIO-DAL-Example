/*
	    File: CMIO_DP_Property_NoData.h
	Abstract: Implements the kCMIOStreamPropertyNoDataTimeoutInMSec, kCMIOStreamPropertyDeviceSyncTimeoutInMSec, and kCMIOStreamPropertyNoDataEventCount properties.
				These deal with a device detecting that no data is coming in (such as a DV device on a section of blank tape).
	
	 Version: 1.2

*/

#if !defined(__CMIO_DP_Property_NoData_h__)
#define __CMIO_DP_Property_NoData_h__

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
	// NoData
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class NoData : public Base
	{
	// Construction/Destruction
	public:
						NoData(DP::Stream& owningStream, UInt32 defaultTimeout) : Base(), mOwningStream(owningStream), mDefaultTimeout(defaultTimeout) { Reset(); }
		virtual			~NoData() {}
	
	private:
		Stream&			GetOwningStream() const { return mOwningStream; }
		Stream&			mOwningStream;

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
		void			Reset() { mTimeout = mDefaultTimeout; mDeviceSyncTimeout = 0; mEventCount = 0; }
		void			BumpNoDataEventCount(bool sendChangeNotifications = true);
	
		UInt32			NoDataTimeoutInMSec() { return mTimeout; };
		void			SetNoDataTimeoutInMSec(UInt32 timeout, bool sendChangeNotifications = true);
		UInt32			DeviceSyncTimeoutInMSec() { return mDeviceSyncTimeout; };
		void			SetDeviceSyncTimeoutInMSec(UInt32 deviceSyncTimeout, bool sendChangeNotifications = true);
		UInt32			NoDataEventCount() { return mEventCount; };
	
	private:
		UInt32			mDefaultTimeout;
		UInt32			mTimeout;
		UInt32			mDeviceSyncTimeout;
		UInt32			mEventCount;
	};
}}}
#endif
