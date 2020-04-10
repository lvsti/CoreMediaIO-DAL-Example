/*
	    File: CMIO_DP_Property_OutputBuffers.cpp
	Abstract: Implements the kCMIOStreamPropertyOutputBufferQueueSize, kCMIOStreamPropertyOutputBuffersRequiredForStartup, kCMIOStreamPropertyOutputBufferUnderrunCount, and
				kCMIOStreamPropertyOutputBufferRepeatCount properties.
	
	 Version: 1.2

*/

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Self Include
#include "CMIO_DP_Property_OutputBuffers.h"

// Internal Includes
#include "CMIO_DP_Stream.h"

// Public Utility Includes
#include "CMIODebugMacros.h"
#include "CMIO_PropertyAddress.h"

// CA Public Utility Includes
#include "CAException.h"

// System Includes
#include <CoreAudio/AudioHardware.h>

namespace CMIO { namespace DP { namespace Property
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// OutputBuffers()
	//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	OutputBuffers::OutputBuffers(Stream& owningStream, UInt32 defaultQueueSize) :
		Base(),
		mOwningStream(owningStream),
		mDefaultQueueSize(defaultQueueSize),
		mDefaultBuffersRequiredForStartup((mDefaultQueueSize + 1) / 2),
		mQueueSize(mDefaultQueueSize),
		mBuffersRequiredForStartup(mDefaultBuffersRequiredForStartup),
		mUnderrunCount(0),
		mRepeatCount(0)
	{
	}

	#pragma mark -
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// IsActive()
	//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool OutputBuffers::IsActive(const CMIOObjectPropertyAddress& /*address*/) const
	{
		return true;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// IsPropertySettable()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool OutputBuffers::IsPropertySettable(const CMIOObjectPropertyAddress& address) const
	{
		bool answer = false;
		
		switch (address.mSelector)
		{
			case kCMIOStreamPropertyOutputBufferQueueSize:
			case kCMIOStreamPropertyOutputBuffersRequiredForStartup:
				answer = true;
				break;
			
			case kCMIOStreamPropertyOutputBufferUnderrunCount:
			case kCMIOStreamPropertyOutputBufferRepeatCount:
				answer = false;
				break;
		};
		
		return answer;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetPropertyDataSize()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	UInt32 OutputBuffers::GetPropertyDataSize(const CMIOObjectPropertyAddress& address, UInt32 /*qualifierDataSize*/, const void* /*qualifierData*/) const
	{
		UInt32 answer = 0;
		
		switch (address.mSelector)
		{
			case kCMIOStreamPropertyOutputBufferQueueSize:
			case kCMIOStreamPropertyOutputBuffersRequiredForStartup:
			case kCMIOStreamPropertyOutputBufferUnderrunCount:
			case kCMIOStreamPropertyOutputBufferRepeatCount:
				answer = sizeof(UInt32);
				break;
		};
		
		return answer;
	}
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetPropertyData()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void OutputBuffers::GetPropertyData(const CMIOObjectPropertyAddress& address, UInt32 /*qualifierDataSize*/, const void* /*qualifierData*/, UInt32 dataSize, UInt32& dataUsed, void* data) const
	{
		switch (address.mSelector)
		{
			case kCMIOStreamPropertyOutputBufferQueueSize:
				ThrowIf(dataSize != sizeof(UInt32), CAException(kAudioHardwareBadPropertySizeError), "CMIO::DP::Property::OutputBuffers::GetData: wrong data size for kCMIOStreamPropertyOutputBufferQueueSize");
				*(static_cast<UInt32*>(data)) = mQueueSize;
				dataUsed = sizeof(UInt32);
				break;
			
			case kCMIOStreamPropertyOutputBuffersRequiredForStartup:
				ThrowIf(dataSize != sizeof(UInt32), CAException(kAudioHardwareBadPropertySizeError), "CMIO::DP::Property::OutputBuffers::GetData: wrong data size for kCMIOStreamPropertyOutputBuffersRequiredForStartup");
				*(static_cast<UInt32*>(data)) = mBuffersRequiredForStartup;
				dataUsed = sizeof(UInt32);
				break;
			
			case kCMIOStreamPropertyOutputBufferUnderrunCount:
				ThrowIf(dataSize != sizeof(UInt32), CAException(kAudioHardwareBadPropertySizeError), "CMIO::DP::Property::OutputBuffers::GetData: wrong data size for kCMIOStreamPropertyOutputBufferUnderrunCount");
				*(static_cast<UInt32*>(data)) = mUnderrunCount;
				dataUsed = sizeof(UInt32);
				break;
			
			case kCMIOStreamPropertyOutputBufferRepeatCount:
				ThrowIf(dataSize != sizeof(UInt32), CAException(kAudioHardwareBadPropertySizeError), "CMIO::DP::Property::OutputBuffers::GetData: wrong data size for kCMIOStreamPropertyOutputBufferRepeatCount");
				*(static_cast<UInt32*>(data)) = mRepeatCount;
				dataUsed = sizeof(UInt32);
				break;
		};
	}
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// SetPropertyData()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void OutputBuffers::SetPropertyData(const CMIOObjectPropertyAddress& address, UInt32 /*qualifierDataSize*/, const void* /*qualifierData*/, UInt32 dataSize, const void* data)
	{
		switch (address.mSelector)
		{
			case kCMIOStreamPropertyOutputBufferQueueSize:
				ThrowIf(dataSize != sizeof(UInt32), CAException(kAudioHardwareBadPropertySizeError), "CMIO::DP::Property::OutputBuffers::SetData: wrong data size for kCMIOStreamPropertyOutputBufferQueueSize");
				mQueueSize = *(static_cast<const UInt32*>(data));
				if (mBuffersRequiredForStartup > mQueueSize)
				{
					mBuffersRequiredForStartup = mQueueSize;
					
					// Send the property changed notification
					PropertyAddress changedAddress(kCMIOStreamPropertyOutputBuffersRequiredForStartup);
					mOwningStream.PropertiesChanged(1, &changedAddress);
				}
				break;
			
			case kCMIOStreamPropertyOutputBuffersRequiredForStartup:
				ThrowIf(dataSize != sizeof(UInt32), CAException(kAudioHardwareBadPropertySizeError), "CMIO::DP::Property::OutputBuffers::SetData: wrong data size for kCMIOStreamPropertyOutputBuffersRequiredForStartup");
				mBuffersRequiredForStartup = *(static_cast<const UInt32*>(data));
				if (mQueueSize < mBuffersRequiredForStartup)
				{
					mQueueSize = mBuffersRequiredForStartup;

					// Send the property changed notification
					PropertyAddress changedAddress(kCMIOStreamPropertyOutputBufferQueueSize);
					mOwningStream.PropertiesChanged(1, &changedAddress);
				}
				break;
		};
	}
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetNumberAddressesImplemented()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	UInt32 OutputBuffers::GetNumberAddressesImplemented() const
	{
		return 4;
	}
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetImplementedAddressByIndex()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void OutputBuffers::GetImplementedAddressByIndex(UInt32 index, CMIOObjectPropertyAddress& address) const
	{
		switch (index)
		{
			case 0:
				address.mSelector = kCMIOStreamPropertyOutputBufferQueueSize;
				address.mScope = kCMIOObjectPropertyScopeWildcard;
				address.mElement = kCMIOObjectPropertyElementWildcard;
				break;
			case 1:
				address.mSelector = kCMIOStreamPropertyOutputBuffersRequiredForStartup;
				address.mScope = kCMIOObjectPropertyScopeWildcard;
				address.mElement = kCMIOObjectPropertyElementWildcard;
				break;
			case 2:
				address.mSelector = kCMIOStreamPropertyOutputBufferUnderrunCount;
				address.mScope = kCMIOObjectPropertyScopeWildcard;
				address.mElement = kCMIOObjectPropertyElementWildcard;
				break;
			case 3:
				address.mSelector = kCMIOStreamPropertyOutputBufferRepeatCount;
				address.mScope = kCMIOObjectPropertyScopeWildcard;
				address.mElement = kCMIOObjectPropertyElementWildcard;
				break;
		};
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// BumpUnderrunCount()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void OutputBuffers::BumpUnderrunCount(bool sendChangeNotifications)
	{
		// Increment the underrun count
		++mUnderrunCount;
		
		if (sendChangeNotifications)
		{
			PropertyAddress address(kCMIOStreamPropertyOutputBufferUnderrunCount);
			mOwningStream.PropertiesChanged(1, &address);
		}
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// BumpRepeatCount()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void OutputBuffers::BumpRepeatCount(bool sendChangeNotifications)
	{
		// Increment the repeat count
		++mRepeatCount;
		
		if (sendChangeNotifications)
		{
			PropertyAddress address(kCMIOStreamPropertyOutputBufferRepeatCount);
			mOwningStream.PropertiesChanged(1, &address);
		}
	}
}}}
