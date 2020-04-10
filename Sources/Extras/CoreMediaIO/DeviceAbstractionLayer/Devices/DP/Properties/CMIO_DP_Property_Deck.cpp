/*
	    File: CMIO_DP_Property_Deck.cpp
	Abstract: Implements the kCMIOStreamPropertyCanProcessDeckCommand, kCMIOStreamPropertyDeck, kCMIOStreamPropertyDeckFrameNumber, kCMIOStreamPropertyDeckDropness,
				kCMIOStreamPropertyDeckThreaded, kCMIOStreamPropertyDeckLocal, and kCMIOStreamPropertyDeckCueing properties.
	
	 Version: 1.2

*/

#include "CMIO_DP_Property_Deck.h"

// Internal Includes
#include "CMIO_DP_Stream.h"

// Public Utility Includes
#include "CMIODebugMacros.h"
#include "CMIO_PropertyAddress.h"

// CA Public Utility Includes
#include "CAException.h"

namespace CMIO { namespace DP { namespace Property
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// IsActive()
	//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool Deck::IsActive(const CMIOObjectPropertyAddress& /*address*/) const
	{
		return true;
	}
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// IsPropertySettable()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool Deck::IsPropertySettable(const CMIOObjectPropertyAddress& address) const
	{
		bool answer = false;
		
		switch (address.mSelector)
		{
			case kCMIOStreamPropertyCanProcessDeckCommand:
				answer = false;
				break;
				
			case kCMIOStreamPropertyDeckFrameNumber:
				answer = false;
				break;
				
			case kCMIOStreamPropertyDeck:
				answer = false;
				break;
				
			case kCMIOStreamPropertyDeckDropness:
				answer = false;
				break;
				
			case kCMIOStreamPropertyDeckThreaded:
				answer = false;
				break;
				
			case kCMIOStreamPropertyDeckLocal:
				answer = false;
				break;
				
			case kCMIOStreamPropertyDeckCueing:
				answer = false;
				break;
		};
		
		return answer;
	}
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetPropertyDataSize()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	UInt32 Deck::GetPropertyDataSize(const CMIOObjectPropertyAddress& address, UInt32 /*qualifierDataSize*/, const void* /*qualifierData*/) const
	{
		UInt32 answer = 0;
		
		switch (address.mSelector)
		{
			case kCMIOStreamPropertyCanProcessDeckCommand:
				answer = sizeof(Boolean);
				break;

			case kCMIOStreamPropertyDeckFrameNumber:
				answer = sizeof(Float64);
				break;
				
			case kCMIOStreamPropertyDeck:
				answer = sizeof(CMIOStreamDeck);
				break;
				
			case kCMIOStreamPropertyDeckDropness:
				answer = sizeof(UInt32);
				break;
				
			case kCMIOStreamPropertyDeckThreaded:
				answer = sizeof(UInt32);
				break;
				
			case kCMIOStreamPropertyDeckLocal:
				answer = sizeof(UInt32);
				break;
				
			case kCMIOStreamPropertyDeckCueing:
				answer = sizeof(SInt32);
				break;
		};
		
		return answer;
	}
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetPropertyData()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void Deck::GetPropertyData(const CMIOObjectPropertyAddress& address, UInt32 /*qualifierDataSize*/, const void* /*qualifierData*/, UInt32 dataSize, UInt32& dataUsed, void* data) const
	{
		switch (address.mSelector)
		{
			case kCMIOStreamPropertyCanProcessDeckCommand:
				ThrowIf(dataSize != sizeof(Boolean), CAException(kCMIOHardwareBadPropertySizeError), "CMIO::DP::Property::Deck::GetPropertyData: wrong data size for kCMIOStreamPropertyCanProcessDeckCommand");
				*static_cast<Boolean*>(data) = true;
				dataUsed = sizeof(Boolean);
				break;
				
			case kCMIOStreamPropertyDeck:
				ThrowIf(dataSize != sizeof(CMIOStreamDeck), CAException(kCMIOHardwareBadPropertySizeError), "CMIO::DP::Property::Deck::GetPropertyData: wrong data size for kCMIOStreamPropertyDeck");
				*static_cast<CMIOStreamDeck*>(data) = mUseOneShots ? mStreamDeckOneShot(GetOwningStream()) : mStreamDeck;
				dataUsed = sizeof(CMIOStreamDeck);
				break;
				
			case kCMIOStreamPropertyDeckFrameNumber:
				ThrowIf(dataSize != sizeof(Float64), CAException(kCMIOHardwareBadPropertySizeError), "CMIO::DP::Property::Deck::GetPropertyData: wrong data size for kCMIOStreamPropertyDeckFrameNumber");
				*static_cast<Float64*>(data) = mUseOneShots ? mTimecodeOneShot(GetOwningStream()) : mTimecode;
				dataUsed = sizeof(Float64);
				break;
				
			case kCMIOStreamPropertyDeckDropness:
				ThrowIf(dataSize != sizeof(UInt32), CAException(kCMIOHardwareBadPropertySizeError), "CMIO::DP::Property::Deck::GetPropertyData: wrong data size for kCMIOStreamPropertyDeckDropness");
				*static_cast<UInt32*>(data) = mUseOneShots ? mDropnessOneShot(GetOwningStream()) : mDropness;
				dataUsed = sizeof(UInt32);
				break;
				
			case kCMIOStreamPropertyDeckThreaded:
				ThrowIf(dataSize != sizeof(UInt32), CAException(kCMIOHardwareBadPropertySizeError), "CMIO::DP::Property::Deck::GetPropertyData: wrong data size for kCMIOStreamPropertyDeckThreaded");
				*static_cast<UInt32*>(data) = mUseOneShots ? mThreadedOneShot(GetOwningStream()) : mThreaded;
				dataUsed = sizeof(UInt32);
				break;
				
			case kCMIOStreamPropertyDeckLocal:
				ThrowIf(dataSize != sizeof(UInt32), CAException(kCMIOHardwareBadPropertySizeError), "CMIO::DP::Property::Deck::GetPropertyData: wrong data size for kCMIOStreamPropertyDeckThreaded");
				*static_cast<UInt32*>(data) = mUseOneShots ? mLocalOneShot(GetOwningStream()) : mLocal;
				dataUsed = sizeof(UInt32);
				break;
				
			case kCMIOStreamPropertyDeckCueing:
				ThrowIf(dataSize != sizeof(SInt32), CAException(kCMIOHardwareBadPropertySizeError), "CMIO::DP::Property::Deck::GetPropertyData: wrong data size for kCMIOStreamPropertyDeckCueing");
				*static_cast<SInt32*>(data) = mUseOneShots ? mCueingOneShot(GetOwningStream()) : mCueing;
				dataUsed = sizeof(SInt32);
				break;
		};
	}
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetNumberAddressesImplemented()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	UInt32 Deck::GetNumberAddressesImplemented() const
	{
		return 7;
	}
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetImplementedAddressByIndex()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void Deck::GetImplementedAddressByIndex(UInt32 index, CMIOObjectPropertyAddress& address) const
	{
		switch (index)
		{
			case 0:
				address.mSelector = kCMIOStreamPropertyCanProcessDeckCommand;
				address.mScope = kCMIOObjectPropertyScopeWildcard;
				address.mElement = kCMIOObjectPropertyElementWildcard;
				break;
			case 1:
				address.mSelector = kCMIOStreamPropertyDeckFrameNumber;
				address.mScope = kCMIOObjectPropertyScopeWildcard;
				address.mElement = kCMIOObjectPropertyElementWildcard;
				break;
			case 2:
				address.mSelector = kCMIOStreamPropertyDeck;
				address.mScope = kCMIOObjectPropertyScopeWildcard;
				address.mElement = kCMIOObjectPropertyElementWildcard;
				break;
			case 3:
				address.mSelector = kCMIOStreamPropertyDeckDropness;
				address.mScope = kCMIOObjectPropertyScopeWildcard;
				address.mElement = kCMIOObjectPropertyElementWildcard;
				break;
			case 4:
				address.mSelector = kCMIOStreamPropertyDeckThreaded;
				address.mScope = kCMIOObjectPropertyScopeWildcard;
				address.mElement = kCMIOObjectPropertyElementWildcard;
				break;
			case 5:
				address.mSelector = kCMIOStreamPropertyDeckLocal;
				address.mScope = kCMIOObjectPropertyScopeWildcard;
				address.mElement = kCMIOObjectPropertyElementWildcard;
				break;
			case 6:
				address.mSelector = kCMIOStreamPropertyDeckCueing;
				address.mScope = kCMIOObjectPropertyScopeWildcard;
				address.mElement = kCMIOObjectPropertyElementWildcard;
				break;
		};
	}
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// SetStreamDeck()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void Deck::SetStreamDeck(CMIOStreamDeck streamDeck, bool sendChangeNotifications) 
	{
		mStreamDeck = streamDeck;
		
		if (sendChangeNotifications)
		{
			PropertyAddress changedAddress(kCMIOStreamPropertyDeck);
			GetOwningStream().PropertiesChanged(1, &changedAddress);
		}
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// SetTimecode()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void Deck::SetTimecode(Float64 timecode, bool sendChangeNotifications) 
	{
		mTimecode = timecode;
		
		if (sendChangeNotifications)
		{
			PropertyAddress changedAddress(kCMIOStreamPropertyDeckFrameNumber);
			GetOwningStream().PropertiesChanged(1, &changedAddress);
		}
	}
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// SetCueing()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void Deck::SetCueing(SInt32 cueing, bool sendChangeNotifications) 
	{
		mCueing = cueing;
		if (sendChangeNotifications)
		{
			PropertyAddress changedAddress(kCMIOStreamPropertyDeckCueing);
			GetOwningStream().PropertiesChanged(1, &changedAddress);
		}
	}
}}}
