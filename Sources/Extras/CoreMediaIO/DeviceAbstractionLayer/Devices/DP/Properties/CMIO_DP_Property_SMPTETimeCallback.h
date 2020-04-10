/*
	    File: CMIO_DP_Property_SMPTETimeCallback.h
	Abstract: Implements the kCMIODevicePropertySMPTETimeCallback property which allows a client to provide a callback which can be used to get the current SMPTE timestamp from a device.
	
	 Version: 1.2

*/

#if !defined(__CMIO_DP_Property_SMPTETimeCallback_h__)
#define __CMIO_DP_Property_SMPTETimeCallback_h__

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
	// SMPTETimeCallback
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class SMPTETimeCallback : public Base
	{
	// Construction/Destruction
	public:
										SMPTETimeCallback(Device& owningDevice) : Base(), mOwningDevice(owningDevice) { mSMPTECallback.mGetSMPTETimeProc = NULL; mSMPTECallback.mRefCon = NULL; }
		virtual							~SMPTETimeCallback() {}
	
	private:
		Device&							GetOwningDevice() const { return mOwningDevice; }
		Device&							mOwningDevice;

	// Property Operations
	public:
		virtual bool					IsActive(const CMIOObjectPropertyAddress& address) const;
		virtual bool					IsPropertySettable(const CMIOObjectPropertyAddress& address) const;
		virtual UInt32					GetPropertyDataSize(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData) const;
		virtual void					GetPropertyData(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData, UInt32 dataSize, UInt32& dataUsed, void* data) const;
		virtual void					SetPropertyData(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData, UInt32 dataSize, const void* data);

		virtual UInt32					GetNumberAddressesImplemented() const;
		virtual void					GetImplementedAddressByIndex(UInt32 index, CMIOObjectPropertyAddress& address) const;
	
	// Property Implementation
	public:
		OSStatus						DoCallback(UInt64* frameNumber, Boolean* isDropFrame, UInt32* tolerance) const;
	
	private:
		CMIODeviceSMPTETimeCallback	mSMPTECallback;
	};
}}}
#endif
