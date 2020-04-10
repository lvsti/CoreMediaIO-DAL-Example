/*
	    File: CMIO_DP_Property_SuspendedByUser.h
	Abstract: Implements the kCMIODevicePropertySuspendedByUser property.
	 Version: 1.2

*/

#if !defined(__CMIO_DP_Property_SuspendedByUser_h__)
#define __CMIO_DP_Property_SuspendedByUser_h__

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
	class Device;
}}

namespace CMIO { namespace DP { namespace Property
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// SuspendedByUser
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class SuspendedByUser : public Base
	{
	//	Construction/Destruction
	public:
						SuspendedByUser(DP::Device& owningDevice, bool suspendedByUser) : Base(), mOwningDevice(owningDevice), mSuspendedByUser(suspendedByUser) {}
	virtual				~SuspendedByUser() {}
	
	private:
		DP::Device&		GetOwningDevice() const { return mOwningDevice; }
		DP::Device&		mOwningDevice;
	
	// Property Operations
	public:
		virtual bool	IsPropertySettable(const CMIOObjectPropertyAddress& address) const { return false; }
		virtual UInt32	GetPropertyDataSize(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData) const;
		virtual void	GetPropertyData(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData, UInt32 dataSize, UInt32& dataUsed, void* data) const;
	
		virtual UInt32	GetNumberAddressesImplemented() const { return 1; }
		virtual void	GetImplementedAddressByIndex(UInt32 index, CMIOObjectPropertyAddress& address) const;
	
	//	Property Implementation
		void			SetSuspendedByUser(bool suspendedByUser);
		bool			IsSuspendedByUser() const { return mSuspendedByUser; }
	
	private:
		bool			mSuspendedByUser;
	};
}}}
#endif
