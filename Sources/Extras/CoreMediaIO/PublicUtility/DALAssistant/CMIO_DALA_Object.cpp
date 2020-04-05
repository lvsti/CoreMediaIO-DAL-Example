/*
	    File: CMIO_DALA_Object.cpp
	Abstract: C++ wrapper for CMIOObjectID
	 Version: 1.2

*/

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Self Include
#include "CMIO_DALA_Object.h"

// Public Utility Includes
#include "CMIODebugMacros.h"
#include "CMIO_PropertyAddress.h"

// CA Public Utility Includes
#include "CAAutoDisposer.h"
#include "CAException.h"

namespace CMIO { namespace DALA
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//	Object
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	Object::Object(CMIOObjectID objectID) :
		mObjectID(objectID)
	{
	}

	Object::~Object()
	{
	}

	CMIOObjectID Object::GetObjectID() const
	{
		return mObjectID;
	}

	CMIOClassID Object::GetClassID() const
	{
		//	set up the return value
		CMIOClassID answer = 0;
		
		//	set up the property address
		PropertyAddress address(kCMIOObjectPropertyClass);
		
		//	make sure the property exists
		if (HasProperty(address))
		{
			UInt32 dataUsed = 0;
			GetPropertyData(address, 0, NULL, sizeof(CMIOClassID), dataUsed, &answer);
		}
		
		return answer;
	}

	CMIOObjectID Object::GetOwnerObjectID() const
	{
		//	set up the return value
		CMIOObjectID answer = 0;
		
		//	set up the property address
		PropertyAddress address(kCMIOObjectPropertyOwner);
		
		//	make sure the property exists
		if (HasProperty(address))
		{
			//	get the property data
			UInt32 dataUsed = 0;
			GetPropertyData(address, 0, NULL, sizeof(CMIOObjectID), dataUsed, &answer);
		}
		
		return answer;
	}

	CFStringRef Object::CopyOwningPlugInBundleID() const
	{
		//	set up the return value
		CFStringRef answer = NULL;
		
		//	set up the property address
		PropertyAddress address(kCMIOObjectPropertyCreator);
		
		//	make sure the property exists
		if (HasProperty(address))
		{
			//	get the property data
			UInt32 dataUsed = 0;
			GetPropertyData(address, 0, NULL, sizeof(CFStringRef), dataUsed, &answer);
		}
		
		return answer;
	}

	CFStringRef Object::CopyName() const
	{
		//	set up the return value
		CFStringRef answer = NULL;
		
		//	set up the property address
		PropertyAddress address(kCMIOObjectPropertyName);
		
		//	make sure the property exists
		if (HasProperty(address))
		{
			//	get the property data
			UInt32 dataUsed = 0;
			GetPropertyData(address, 0, NULL, sizeof(CFStringRef), dataUsed, &answer);
		}
		
		return answer;
	}

	CFStringRef Object::CopyManufacturer() const
	{
		//	set up the return value
		CFStringRef answer = NULL;
		
		//	set up the property address
		PropertyAddress address(kCMIOObjectPropertyManufacturer);
		
		//	make sure the property exists
		if (HasProperty(address))
		{
			//	get the property data
			UInt32 dataUsed = 0;
			GetPropertyData(address, 0, NULL, sizeof(CFStringRef), dataUsed, &answer);
		}
		
		return answer;
	}

	CFStringRef Object::CopyNameForElement(CMIOObjectPropertyScope scope, CMIOObjectPropertyElement element) const
	{
		//	set up the return value
		CFStringRef answer = NULL;
		
		//	set up the property address
		PropertyAddress address(kCMIOObjectPropertyElementName, scope, element);
		
		//	make sure the property exists
		if (HasProperty(address))
		{
			//	get the property data
			UInt32 dataUsed = 0;
			GetPropertyData(address, 0, NULL, sizeof(CFStringRef), dataUsed, &answer);
		}
		
		return answer;
	}

	CFStringRef Object::CopyCategoryNameForElement(CMIOObjectPropertyScope scope, CMIOObjectPropertyElement element) const
	{
		//	set up the return value
		CFStringRef answer = NULL;
		
		//	set up the property address
		PropertyAddress address(kCMIOObjectPropertyElementCategoryName, scope, element);
		
		//	make sure the property exists
		if (HasProperty(address))
		{
			//	get the property data
			UInt32 dataUsed = 0;
			GetPropertyData(address, 0, NULL, sizeof(CFStringRef), dataUsed, &answer);
		}
		
		return answer;
	}

	CFStringRef Object::CopyNumberNameForElement(CMIOObjectPropertyScope scope, CMIOObjectPropertyElement element) const
	{
		//	set up the return value
		CFStringRef answer = NULL;
		
		//	set up the property address
		PropertyAddress address(kCMIOObjectPropertyElementNumberName, scope, element);
		
		//	make sure the property exists
		if (HasProperty(address))
		{
			//	get the property data
			UInt32 dataUsed = 0;
			GetPropertyData(address, 0, NULL, sizeof(CFStringRef), dataUsed, &answer);
		}
		
		return answer;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// IsSubClass()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool Object::IsSubClass(CMIOClassID classID, CMIOClassID baseClassID)
	{
		bool answer = false;
		
		switch(baseClassID)
		{
			case kCMIOObjectClassID:
			{
				// All classes are subclasses of CMIOObject
				answer = true;
			}
			break;
			
			case kCMIOControlClassID:
			{
				switch(classID)
				{
					case kCMIOControlClassID:
					case kCMIOBooleanControlClassID:
					case kCMIOSelectorControlClassID:
					case kCMIOFeatureControlClassID:
					case kCMIOJackControlClassID:
					case kCMIODirectionControlClassID:
					case kCMIODataSourceControlClassID:
					case kCMIODataDestinationControlClassID:
					case kCMIOBlackLevelControlClassID:
					case kCMIOWhiteLevelControlClassID:
					case kCMIOHueControlClassID:
					case kCMIOSaturationControlClassID:
					case kCMIOContrastControlClassID:
					case kCMIOSharpnessControlClassID:
					case kCMIOBrightnessControlClassID:
					case kCMIOGainControlClassID:
					case kCMIOIrisControlClassID:
					case kCMIOShutterControlClassID:
					case kCMIOExposureControlClassID:
					case kCMIOWhiteBalanceUControlClassID:
					case kCMIOWhiteBalanceVControlClassID:
					case kCMIOGammaControlClassID:
					case kCMIOTemperatureControlClassID:
					case kCMIOZoomControlClassID:
					case kCMIOFocusControlClassID:
					case kCMIOPanControlClassID:
					case kCMIOTiltControlClassID:
					case kCMIOOpticalFilterClassID:
					case kCMIOBacklightCompensationControlClassID:
					case kCMIOPowerLineFrequencyControlClassID:
					case kCMIONoiseReductionControlClassID:
					{
						answer = true;
					}
					break;
				};
			}
			break;
						
			case kCMIOBooleanControlClassID:
			{
				switch(classID)
				{
					case kCMIOBooleanControlClassID:
					case kCMIOJackControlClassID:
					case kCMIODirectionControlClassID:
					{
						answer = true;
					}
					break;
				};
			}
			break;
			
			case kCMIOSelectorControlClassID:
			{
				switch(classID)
				{
					case kCMIOSelectorControlClassID:
					case kCMIODataSourceControlClassID:
					case kCMIODataDestinationControlClassID:
					{
						answer = true;
					}
					break;
				};
			}
			break;
			
			case kCMIOFeatureControlClassID:
			{
				switch(classID)
				{
					case kCMIOFeatureControlClassID:
					case kCMIOBlackLevelControlClassID:
					case kCMIOWhiteLevelControlClassID:
					case kCMIOHueControlClassID:
					case kCMIOSaturationControlClassID:
					case kCMIOContrastControlClassID:
					case kCMIOSharpnessControlClassID:
					case kCMIOBrightnessControlClassID:
					case kCMIOGainControlClassID:
					case kCMIOIrisControlClassID:
					case kCMIOShutterControlClassID:
					case kCMIOExposureControlClassID:
					case kCMIOWhiteBalanceUControlClassID:
					case kCMIOWhiteBalanceVControlClassID:
					case kCMIOGammaControlClassID:
					case kCMIOTemperatureControlClassID:
					case kCMIOZoomControlClassID:
					case kCMIOFocusControlClassID:
					case kCMIOPanControlClassID:
					case kCMIOTiltControlClassID:
					case kCMIOOpticalFilterClassID:
					case kCMIOBacklightCompensationControlClassID:
					case kCMIOPowerLineFrequencyControlClassID:
					case kCMIONoiseReductionControlClassID:
					{
						answer = true;
					}
					break;
				};
			}
			break;
			
			case kCMIODeviceClassID:
			{
				switch(classID)
				{
					case kCMIODeviceClassID:
					{
						answer = true;
					}
					break;
				};
			}
			break;
			
			// Leaf classes
			case kCMIOJackControlClassID:
			case kCMIODirectionControlClassID:
			case kCMIODataSourceControlClassID:
			case kCMIODataDestinationControlClassID:
			case kCMIOBlackLevelControlClassID:
			case kCMIOWhiteLevelControlClassID:
			case kCMIOHueControlClassID:
			case kCMIOSaturationControlClassID:
			case kCMIOContrastControlClassID:
			case kCMIOSharpnessControlClassID:
			case kCMIOBrightnessControlClassID:
			case kCMIOGainControlClassID:
			case kCMIOIrisControlClassID:
			case kCMIOShutterControlClassID:
			case kCMIOExposureControlClassID:
			case kCMIOWhiteBalanceUControlClassID:
			case kCMIOWhiteBalanceVControlClassID:
			case kCMIOGammaControlClassID:
			case kCMIOTemperatureControlClassID:
			case kCMIOZoomControlClassID:
			case kCMIOFocusControlClassID:
			case kCMIOPanControlClassID:
			case kCMIOTiltControlClassID:
			case kCMIOOpticalFilterClassID:
			case kCMIOBacklightCompensationControlClassID:
			case kCMIOPowerLineFrequencyControlClassID:
			case kCMIONoiseReductionControlClassID:
			case kCMIOSystemObjectClassID:
			case kCMIOPlugInClassID:
			case kCMIOStreamClassID:
			{
				answer = classID == baseClassID;
			}
			break;
			
			default:
			{
				#if CMIO_Debug
					char classIDString[5] = CA4CCToCString(baseClassID);
					DebugMessage("CMIO::DALA::Object::IsSubClass: unknown base class '%s'", classIDString);
				#endif
				answer = classID == baseClassID;
			}
			break;
			
		};
		
		return answer;
	}

	UInt32 Object::GetNumberOwnedObjects(CMIOClassID classID) const
	{
		//	set up the return value
		UInt32 answer = 0;
		
		//	set up the property address
		PropertyAddress address(kCMIOObjectPropertyOwnedObjects);
		
		//	figure out the qualifier
		UInt32 qualifierSize = 0;
		void* qualifierData = NULL;
		if (classID != 0)
		{
			qualifierSize = sizeof(CMIOObjectID);
			qualifierData = &classID;
		}
		
		//	get the property data size
		answer = GetPropertyDataSize(address, qualifierSize, qualifierData);
		
		//	calculate the number of object IDs
		answer /= SizeOf32(CMIOObjectID);
		
		return answer;
	}

	void Object::GetAllOwnedObjects(CMIOClassID classID, UInt32& ioNumberObjects, CMIOObjectID* ioObjectIDs) const
	{
		//	set up the property address
		PropertyAddress address(kCMIOObjectPropertyOwnedObjects);
		
		//	figure out the qualifier
		UInt32 qualifierSize = 0;
		void* qualifierData = NULL;
		if (classID != 0)
		{
			qualifierSize = sizeof(CMIOObjectID);
			qualifierData = &classID;
		}
		
		//	get the property data
		UInt32 dataSize = ioNumberObjects * SizeOf32(CMIOClassID);
		UInt32 dataUsed = 0;
		GetPropertyData(address, qualifierSize, qualifierData, dataSize, dataUsed, ioObjectIDs);
		
		//	set the number of object IDs being returned
		ioNumberObjects = dataUsed / SizeOf32(CMIOObjectID);
	}

	CMIOObjectID Object::GetOwnedObjectByIndex(CMIOClassID classID, UInt32 index)
	{
		//	set up the property address
		PropertyAddress address(kCMIOObjectPropertyOwnedObjects);
		
		//	figure out the qualifier
		UInt32 qualifierSize = 0;
		void* qualifierData = NULL;
		if (classID != 0)
		{
			qualifierSize = sizeof(CMIOObjectID);
			qualifierData = &classID;
		}
		
		//	figure out how much space to allocate
		UInt32 dataSize = GetPropertyDataSize(address, qualifierSize, qualifierData);
		UInt32 numberObjectIDs = dataSize / SizeOf32(CMIOObjectID);
		
		//	set up the return value
		CMIOObjectID answer = 0;
		
		//	maker sure the index is in range
		if (index < numberObjectIDs)
		{
			//	allocate it
			CAAutoArrayDelete<CMIOObjectID> objectList(dataSize / sizeof(CMIOObjectID));
			
			//	get the property data
			UInt32 dataUsed = 0;
			GetPropertyData(address, qualifierSize, qualifierData, dataSize, dataUsed, objectList);
			
			//	get the return value
			answer = objectList[index];
		}
		
		return answer;
	}

	bool Object::HasProperty(const CMIOObjectPropertyAddress& address) const
	{
		return CMIOObjectHasProperty(mObjectID, &address);
	}

	bool Object::IsPropertySettable(const CMIOObjectPropertyAddress& address) const
	{
		Boolean isSettable = false;
		OSStatus error = CMIOObjectIsPropertySettable(mObjectID, &address, &isSettable);
		ThrowIfError(error, CAException(error), "Object::IsPropertySettable: got an error getting info about a property");
		return isSettable != 0;
	}

	UInt32 Object::GetPropertyDataSize(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData) const
	{
		UInt32 dataSize = 0;
		OSStatus error = CMIOObjectGetPropertyDataSize(mObjectID, &address, qualifierDataSize, qualifierData, &dataSize);
		ThrowIfError(error, CAException(error), "Object::GetPropertyDataSize: got an error getting the property data size");
		return dataSize;
	}

	void Object::GetPropertyData(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData, UInt32 dataSize, UInt32& dataUsed, void* data) const
	{
		OSStatus error = CMIOObjectGetPropertyData(mObjectID, &address, qualifierDataSize, qualifierData, dataSize, &dataUsed, data);
		ThrowIfError(error, CAException(error), "Object::GetPropertyData: got an error getting the property data");
	}

	void Object::SetPropertyData(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData, UInt32 dataSize, const void* data)
	{
		OSStatus error = CMIOObjectSetPropertyData(mObjectID, &address, qualifierDataSize, qualifierData, dataSize, data);
		ThrowIfError(error, CAException(error), "Object::SetPropertyData: got an error setting the property data");
	}

	void Object::AddPropertyListener(const CMIOObjectPropertyAddress& address, CMIOObjectPropertyListenerProc listenerProc, void* clientData)
	{
		OSStatus error = CMIOObjectAddPropertyListener(mObjectID, &address, listenerProc, clientData);
		ThrowIfError(error, CAException(error), "Object::AddPropertyListener: got an error adding a property listener");
	}

	void Object::RemovePropertyListener(const CMIOObjectPropertyAddress& address, CMIOObjectPropertyListenerProc listenerProc, void* clientData)
	{
		OSStatus error = CMIOObjectRemovePropertyListener(mObjectID, &address, listenerProc, clientData);
		ThrowIfError(error, CAException(error), "Object::RemovePropertyListener: got an error removing a property listener");
	}
}}
