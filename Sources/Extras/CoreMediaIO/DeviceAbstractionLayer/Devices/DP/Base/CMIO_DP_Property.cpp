/*
	    File: CMIO_DP_Property.cpp
	Abstract: n/a
	 Version: 1.2

*/

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Self Include
#include "CMIO_DP_Property.h"

namespace CMIO { namespace DP
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Property
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	Property::Property()
	{
	}

	Property::~Property()
	{
	}

	void	Property::Initialize()
	{
	}

	void	Property::Teardown()
	{
	}

	bool	Property::IsActive(const CMIOObjectPropertyAddress& /*inAddress*/) const
	{
		return true;
	}

	bool	Property::IsPropertySettable(const CMIOObjectPropertyAddress& /*inAddress*/) const
	{
		return true;
	}

	UInt32	Property::GetPropertyDataSize(const CMIOObjectPropertyAddress& /*inAddress*/, UInt32 /*inQualifierDataSize*/, const void* /*inQualifierData*/) const
	{
		return 0;
	}

	void	Property::GetPropertyData(const CMIOObjectPropertyAddress& /*inAddress*/, UInt32 /*inQualifierDataSize*/, const void* /*inQualifierData*/, UInt32& ioDataSize, void* /*outData*/) const
	{
		ioDataSize = 0;
	}

	void	Property::SetPropertyData(const CMIOObjectPropertyAddress& /*inAddress*/, UInt32 /*inQualifierDataSize*/, const void* /*inQualifierData*/, UInt32 /*inDataSize*/, const void* /*inData*/)
	{
	}

	UInt32	Property::GetNumberAddressesImplemented() const
	{
		return 0;
	}

	void	Property::GetImplementedAddressByIndex(UInt32 /*inIndex*/, CMIOObjectPropertyAddress& /*outAddress*/) const
	{
	}
}}
