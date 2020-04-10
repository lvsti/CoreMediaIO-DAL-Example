/*
	    File: CMIO_DP_Property_HogMode.h
	Abstract: Implements the kCMIODevicePropertyHogMode property with a non-settable value of -1 (the device can be used by any process).
				However, it does allow the property to be set internally to a valid PID, in the event it is necessary to signify that the device is in use by other software such as a
				QuickTime video digitizer.
				
				NOTE:  This has NO RELATION to the CMIO::DP::HogMode object which is implemented in terms of a global preference and uses CFDistributedNotificationCenter for notifications.
				That object is intended for devices which are not shareable across processes.
	
	 Version: 1.2

*/

#if !defined(__CMIO_DP_Property_HogMode_h__)
#define __CMIO_DP_Property_HogMode_h__

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Super Class Includes
#include "CMIO_DP_Property_Base.h"

// CA Public Utility Includes
#include "CACFMachPort.h"

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
	// HogMode
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class HogMode : public DP::Property::Base
	{
	//	Construction/Destruction
	public:
						HogMode(Device& owningDevice, pid_t owner) : DP::Property::Base(), mOwningDevice(owningDevice), mOwner(owner) {}
		virtual			~HogMode() {}

	private:
		Device&			GetOwningDevice() const { return mOwningDevice; }
		Device&			mOwningDevice;
	
	// Property Operations
	public:
		virtual bool	IsPropertySettable(const CMIOObjectPropertyAddress& address) const { return false; }
		virtual UInt32	GetPropertyDataSize(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData) const;
		virtual void	GetPropertyData(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData, UInt32 dataSize, UInt32& dataUsed, void* data) const;

		virtual UInt32	GetNumberAddressesImplemented() const { return 1; }
		virtual void	GetImplementedAddressByIndex(UInt32 index, CMIOObjectPropertyAddress& address) const;
	
	//	Property Implementation
		void			SetHogMode(pid_t owner, bool sendChangeNotifications = true);
		bool			IsFree() const { return  -1 == mOwner; }
		
	private:
		pid_t			mOwner;
	};
}}}
#endif
