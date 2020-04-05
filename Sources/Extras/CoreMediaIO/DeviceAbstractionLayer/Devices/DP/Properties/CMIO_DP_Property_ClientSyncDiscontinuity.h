/*
	    File: CMIO_DP_Property_ClientSyncDiscontinuity.h
	Abstract: Implements the kCMIODevicePropertyClientSyncDiscontinuity property which allows a client to force a discontinuity into a stream in order to synchronize with data coming
				from a CMIO processing graph.
	
	 Version: 1.2

*/

#if !defined(__CMIO_DP_Property_ClientSyncDiscontinuity_h__)
#define __CMIO_DP_Property_ClientSyncDiscontinuity_h__

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
	// ClientSyncDiscontinuity
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class ClientSyncDiscontinuity : public Base
	{
	// Construction/Destruction
	public:
						ClientSyncDiscontinuity(Device& owningDevice) : mOwningDevice(owningDevice), mForceDiscontinuity(false) {}
		virtual			~ClientSyncDiscontinuity() {}
	
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
	
	// Property Implementation
	public:
		Boolean			GetForceDiscontinuity() { return mForceDiscontinuity; };
		void			SetForceDiscontinuity(Boolean forceDiscontinuity, bool sendChangeNotifications = true);
	
	private:
		Boolean			mForceDiscontinuity;
	};
}}}
#endif
