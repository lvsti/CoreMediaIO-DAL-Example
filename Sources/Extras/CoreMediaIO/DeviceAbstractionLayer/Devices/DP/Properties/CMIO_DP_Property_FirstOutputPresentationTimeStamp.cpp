/*
	    File: CMIO_DP_Property_FirstOutputPresentationTimeStamp.cpp
	Abstract: Implements the kCMIOStreamPropertyFirstOutputPresentationTimeStamp property.
	 Version: 1.2

*/

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Self Include
#include "CMIO_DP_Property_FirstOutputPresentationTimeStamp.h"

// Internal Includes
#include "CMIO_DP_Stream.h"

// Public Utility Includes
#include "CMIODebugMacros.h"

// CA Public Utility Includes
#include "CAException.h"

namespace CMIO { namespace DP { namespace Property
{
	#pragma mark -
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetPropertyDataSize()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	UInt32 FirstOutputPresentationTimeStamp::GetPropertyDataSize(const CMIOObjectPropertyAddress& address, UInt32 /*qualifierDataSize*/, const void* /*qualifierData*/) const
	{
		UInt32 answer = 0;
		
		switch (address.mSelector)
		{
			case kCMIOStreamPropertyFirstOutputPresentationTimeStamp:
				answer = sizeof(CMTime);
				break;
		};
		
		return answer;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetPropertyData()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void FirstOutputPresentationTimeStamp::GetPropertyData(const CMIOObjectPropertyAddress& address, UInt32 /*qualifierDataSize*/, const void* /*qualifierData*/, UInt32 dataSize, UInt32& dataUsed, void* data) const
	{
		switch (address.mSelector)
		{
			case kCMIOStreamPropertyFirstOutputPresentationTimeStamp:
				ThrowIf(dataSize != sizeof(CMTime), CAException(kCMIOHardwareBadPropertySizeError), "CMIO::DP::VDC::Property::FirstOutputPresentationTimeStamp::GetPropertyData: wrong data size for kCMIOStreamPropertyFirstOutputPresentationTimeStamp");
				*static_cast<CMTime*>(data) = mFirstOutputPresentationTimeStamp;
				dataUsed = sizeof(CMTime);
				break;
		};
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetImplementedAddressByIndex()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void FirstOutputPresentationTimeStamp::GetImplementedAddressByIndex(UInt32 index, CMIOObjectPropertyAddress& address) const
	{
		switch(index)
		{
			case 0:
				address.mSelector = kCMIOStreamPropertyFirstOutputPresentationTimeStamp;
				address.mScope = kCMIOObjectPropertyScopeWildcard;
				address.mElement = kCMIOObjectPropertyElementWildcard;
				break;
		};
	}

	#pragma mark -
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// SetFirstOutputPresentationTimeStamp()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void FirstOutputPresentationTimeStamp::SetFirstOutputPresentationTimeStamp(CMTime firstOutputPresentationTimeStamp) 
	{
		// Send out property change notifications if the state is different
		if (CMTimeCompare(firstOutputPresentationTimeStamp, GetFirstOutputPresentationTimeStamp()) != 0)
		{
			mFirstOutputPresentationTimeStamp = firstOutputPresentationTimeStamp;
			PropertyAddress changedAddress(kCMIOStreamPropertyFirstOutputPresentationTimeStamp);
			mOwningStream.PropertiesChanged(1, &changedAddress);
		}
	}
}}}
