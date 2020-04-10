/*
	    File: CMIO_DP_Property_DeviceMaster.h
	Abstract: Implements kCMIODevicePropertyDeviceMaster property.
	 Version: 1.2

*/

#if !defined(__CMIO_DP_Property_DeviceMaster_h__)
#define __CMIO_DP_Property_DeviceMaster_h__

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Super Class Includes
#include "CMIO_DP_Property_Base.h"

// CA Public Utility Includes
#include "CAProcess.h"

// System Includes
#include <sys/types.h>

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
	// DeviceMaster
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class DeviceMaster : public Base
	{
	// Construction/Destruction
	public:
						DeviceMaster(DP::Device& owningDevice, pid_t masterPID) : Base(), mOwningDevice(owningDevice), mMasterPID(masterPID) {}
		virtual			~DeviceMaster() {}

	private:
		Device&			GetOwningDevice() const { return mOwningDevice; }
		Device&			mOwningDevice;

	// Property Operations
	public:
		virtual bool	IsActive(const CMIOObjectPropertyAddress& address) const;
		virtual bool	IsPropertySettable(const CMIOObjectPropertyAddress& address) const;
		virtual UInt32	GetPropertyDataSize(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData) const;
		virtual void	GetPropertyData(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData, UInt32 dataSize, UInt32& dataUsed, void* data) const;
		virtual void	SetPropertyData(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData, UInt32 dataSize, const void* data);

		virtual UInt32	GetNumberAddressesImplemented() const;
		virtual void	GetImplementedAddressByIndex(UInt32 index, CMIOObjectPropertyAddress& address) const;
	
	//	Property Implementation
	public:
		pid_t			GetMaster() const { return mMasterPID; }
		void			SetMaster(pid_t masterPID, bool sendChangeNotifications = true);

		bool			IsFree() const { return (-1 == mMasterPID); }
		bool			CurrentProcessIsMaster() const { return (mMasterPID == CAProcess::GetPID()); }
		bool			CurrentProcessIsMasterOrIsFree() const { return (CurrentProcessIsMaster() or IsFree()); }

	private:
		pid_t			mMasterPID;
	};
}}}
#endif
