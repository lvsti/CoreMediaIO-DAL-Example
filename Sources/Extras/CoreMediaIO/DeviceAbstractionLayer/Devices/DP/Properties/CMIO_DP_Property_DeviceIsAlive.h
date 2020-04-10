/*
	    File: CMIO_DP_Property_DeviceIsAlive.h
	Abstract: Implements the kCMIODevicePropertyDeviceIsAlive property.
	 Version: 1.2

*/

#if !defined(__CMIO_DP_Property_DeviceIsAlive_h__)
#define __CMIO_DP_Property_DeviceIsAlive_h__

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
}};

namespace CMIO { namespace DP { namespace Property
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// DeviceIsAlive
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class DeviceIsAlive : public Base
	{
	// Construction/Destruction
	public:
						DeviceIsAlive(DP::Device& owningDevice, bool isAlive = true) : Base(), mOwningDevice(owningDevice), mIsAlive(isAlive) {}
		virtual			~DeviceIsAlive() {}
	
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
		void			SetIsAlive(bool isAlive);
		UInt32			IsAlive() { return mIsAlive; };
	
	private:
		DP::Device&		mOwningDevice;
		bool			mIsAlive;
	};
}}}
#endif
