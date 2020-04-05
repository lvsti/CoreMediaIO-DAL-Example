/*
	    File: CMIO_DP_Property_Base.cpp
	Abstract: n/a
	 Version: 1.2

*/

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Self Include
#include "CMIO_DP_Property_Base.h"

namespace CMIO { namespace DP { namespace Property
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Base
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	Base::Base()
	{
	}

	Base::~Base()
	{
	}

	void Base::Initialize()
	{
	}

	void Base::Teardown()
	{
	}

	bool Base::IsActive(const CMIOObjectPropertyAddress& /*address*/) const
	{
		return true;
	}

	bool Base::IsPropertySettable(const CMIOObjectPropertyAddress& /*address*/) const
	{
		return true;
	}

	UInt32 Base::GetPropertyDataSize(const CMIOObjectPropertyAddress& /*address*/, UInt32 /*qualifierDataSize*/, const void* /*qualifierData*/) const
	{
		return 0;
	}

	void Base::GetPropertyData(const CMIOObjectPropertyAddress& /*address*/, UInt32 /*qualifierDataSize*/, const void* /*qualifierData*/, UInt32 /*dataSize*/, UInt32& dataUsed, void* /*data*/) const
	{
		dataUsed = 0;
	}

	void Base::SetPropertyData(const CMIOObjectPropertyAddress& /*address*/, UInt32 /*qualifierDataSize*/, const void* /*qualifierData*/, UInt32 /*dataSize*/, const void* /*data*/)
	{
	}

	UInt32 Base::GetNumberAddressesImplemented() const
	{
		return 0;
	}

	void Base::GetImplementedAddressByIndex(UInt32 /*index*/, CMIOObjectPropertyAddress& /*address*/) const
	{
	}
}}}
