/*
	    File: CMIO_DP_Property_ScheduledOutputNotificationProc.cpp
	Abstract: Implements the kCMIOStreamPropertyScheduledOutputNotificationProc property.
	 Version: 1.2

*/

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Self Include
#include "CMIO_DP_Property_ScheduledOutputNotificationProc.h"

// Internal Includes
#include "CMIO_DP_Stream.h"

// Public Utility Includes
#include "CMIODebugMacros.h"

// CA Public Utility Includes
#include "CAException.h"
#include "CAMutex.h"

namespace CMIO { namespace DP { namespace Property
{
	#pragma mark -
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetPropertyDataSize()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	UInt32 ScheduledOutputNotificationProc::GetPropertyDataSize(const CMIOObjectPropertyAddress& address, UInt32 /*qualifierDataSize*/, const void* /*qualifierData*/) const
	{
		UInt32 answer = 0;
		
		switch (address.mSelector)
		{
			case kCMIOStreamPropertyScheduledOutputNotificationProc:
				answer = sizeof(CMIOStreamScheduledOutputNotificationProcAndRefCon);
				break;
		};
		
		return answer;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetPropertyData()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void ScheduledOutputNotificationProc::GetPropertyData(const CMIOObjectPropertyAddress& address, UInt32 /*qualifierDataSize*/, const void* /*qualifierData*/, UInt32 dataSize, UInt32& dataUsed, void* data) const
	{
		switch (address.mSelector)
		{
			case kCMIOStreamPropertyScheduledOutputNotificationProc:
				ThrowIf(dataSize != sizeof(CMIOStreamScheduledOutputNotificationProcAndRefCon), CAException(kCMIOHardwareBadPropertySizeError), "CMIO::DP::VDC::Property::ScheduledOutputNotificationProc::GetPropertyData: wrong data size for kCMIOStreamPropertyScheduledOutputNotificationProc");
				*static_cast<CMIOStreamScheduledOutputNotificationProcAndRefCon*>(data) = mScheduledOutputNotificationProc;
				dataUsed = sizeof(CMIOStreamScheduledOutputNotificationProcAndRefCon);
				break;
		};
	}
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// SetPropertyData()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void ScheduledOutputNotificationProc::SetPropertyData(const CMIOObjectPropertyAddress& address, UInt32 /*qualifierDataSize*/, const void* /*qualifierData*/, UInt32 dataSize, const void* data)
	{
		switch (address.mSelector)
		{
			case kCMIOStreamPropertyScheduledOutputNotificationProc:
				ThrowIf(dataSize != sizeof(CMIOStreamScheduledOutputNotificationProcAndRefCon), CAException(kCMIOHardwareBadPropertySizeError), "CMIO::DP::ScheduledOutputNotificationProc::SetPropertyData: wrong data size for kCMIOStreamPropertyScheduledOutputNotificationProc");
				CMIOStreamScheduledOutputNotificationProcAndRefCon scheduledOutputNotificationProc = *(static_cast<const CMIOStreamScheduledOutputNotificationProcAndRefCon*>(data));
				
				if (	(mScheduledOutputNotificationProc.scheduledOutputNotificationProc != scheduledOutputNotificationProc.scheduledOutputNotificationProc)
					 || (mScheduledOutputNotificationProc.scheduledOutputNotificationRefCon != scheduledOutputNotificationProc.scheduledOutputNotificationRefCon))
				{
					mScheduledOutputNotificationProc = scheduledOutputNotificationProc;
					
					// Signal that device master changed
					PropertyAddress changedAddress(kCMIOStreamPropertyScheduledOutputNotificationProc);
					GetOwningStream().PropertiesChanged(1, &changedAddress);
				}
				break;
		};
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetImplementedAddressByIndex()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void ScheduledOutputNotificationProc::GetImplementedAddressByIndex(UInt32 index, CMIOObjectPropertyAddress& address) const
	{
		switch(index)
		{
			case 0:
				address.mSelector = kCMIOStreamPropertyScheduledOutputNotificationProc;
				address.mScope = kCMIOObjectPropertyScopeWildcard;
				address.mElement = kCMIOObjectPropertyElementWildcard;
				break;
		};
	}

	#pragma mark -
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// SendNotification()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void ScheduledOutputNotificationProc::SendNotification(UInt64 sequenceNumberOfBufferThatWasOutput, UInt64 outputHostTime) 
	{
		CAMutex*	objectStateMutex = mOwningStream.GetObjectStateMutex();
		bool		objectStateMutexNeedsUnlocking = false;
		
		try
		{
			if (objectStateMutex != NULL)
				objectStateMutexNeedsUnlocking = objectStateMutex->Lock();
			
			if (mScheduledOutputNotificationProc.scheduledOutputNotificationProc)
				(mScheduledOutputNotificationProc.scheduledOutputNotificationProc)(sequenceNumberOfBufferThatWasOutput, outputHostTime, mScheduledOutputNotificationProc.scheduledOutputNotificationRefCon);
		}
		catch (...)
		{
		}
		
		if (objectStateMutexNeedsUnlocking)
			objectStateMutex->Unlock();
	}
}}}
