/*
	    File: CMIO_DP_Sample_Property_HogMode.h
	Abstract: Implements the kCMIODevicePropertyHogMode property with a non-settable value of -1, meaning that the device can be used by any process.
	
	 Version: 1.2

*/

#if !defined(__CMIO_DP_Sample_Property_HogMode_h__)
#define __CMIO_DP_Sample_Property_HogMode_h__

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Super Class Includes
#include "CMIO_DP_Property_Base.h"

namespace CMIO { namespace DP { namespace Sample { namespace Property
{
	class Device;
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// HogMode
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class HogMode : public CMIO::DP::Property::Base
	{
	//	Construction/Destruction
	public:
						HogMode() : CMIO::DP::Property::Base() {}
		virtual			~HogMode() {}
	
	// Property Operations
	public:
		virtual bool	IsPropertySettable(const CMIOObjectPropertyAddress& address) const { return false; }
		virtual UInt32	GetPropertyDataSize(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData) const;
		virtual void	GetPropertyData(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData, UInt32 dataSize, UInt32& dataUsed, void* data) const;

		virtual UInt32	GetNumberAddressesImplemented() const { return 1; }
		virtual void	GetImplementedAddressByIndex(UInt32 index, CMIOObjectPropertyAddress& address) const;
	};
}}}}
#endif
