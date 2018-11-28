/*
	    File: CMIO_DALA_Control.cpp
	Abstract: C++ wrapper for CMIOControlID
	 Version: 1.2
	
	Disclaimer: IMPORTANT:  This Apple software is supplied to you by Apple
	Inc. ("Apple") in consideration of your agreement to the following
	terms, and your use, installation, modification or redistribution of
	this Apple software constitutes acceptance of these terms.  If you do
	not agree with these terms, please do not use, install, modify or
	redistribute this Apple software.
	
	In consideration of your agreement to abide by the following terms, and
	subject to these terms, Apple grants you a personal, non-exclusive
	license, under Apple's copyrights in this original Apple software (the
	"Apple Software"), to use, reproduce, modify and redistribute the Apple
	Software, with or without modifications, in source and/or binary forms;
	provided that if you redistribute the Apple Software in its entirety and
	without modifications, you must retain this notice and the following
	text and disclaimers in all such redistributions of the Apple Software.
	Neither the name, trademarks, service marks or logos of Apple Inc. may
	be used to endorse or promote products derived from the Apple Software
	without specific prior written permission from Apple.  Except as
	expressly stated in this notice, no other rights or licenses, express or
	implied, are granted by Apple herein, including but not limited to any
	patent rights that may be infringed by your derivative works or by other
	works in which the Apple Software may be incorporated.
	
	The Apple Software is provided by Apple on an "AS IS" basis.  APPLE
	MAKES NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION
	THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS
	FOR A PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND
	OPERATION ALONE OR IN COMBINATION WITH YOUR PRODUCTS.
	
	IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL
	OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
	SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
	INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION,
	MODIFICATION AND/OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED
	AND WHETHER UNDER THEORY OF CONTRACT, TORT (INCLUDING NEGLIGENCE),
	STRICT LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN ADVISED OF THE
	POSSIBILITY OF SUCH DAMAGE.
	
	Copyright (C) 2012 Apple Inc. All Rights Reserved.
	
*/

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Self Include
#include "CMIO_DALA_Control.h"

// PublicUtility Includes
#include "CMIODebugMacros.h"
#include "CMIO_PropertyAddress.h"

// CA PublicUtility Includes
#include "CAAutoDisposer.h"
#include "CAException.h"

namespace CMIO { namespace DALA
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Control()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	Control::Control(CMIOObjectID cmioObjectID) :
		Object(cmioObjectID)
	{
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// ~Control()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	Control::~Control()
	{
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Control::GetPropertyScope()
	//	Get the CMIOObjectPropertyScope IN THE OWNING CMIOObject that contains the control.  (Controls themselves only have kCMIOObjectPropertyScopeGlobal)
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	CMIOObjectPropertyScope Control::GetPropertyScope() const
	{
		// Setup the answer
		CMIOObjectPropertyScope answer = kCMIOObjectPropertyScopeGlobal;
		
		// Setup the property address
		PropertyAddress address(kCMIOControlPropertyScope);
		
		// Make sure the property exists
		if (HasProperty(address))
		{
			// Get the property data
			UInt32 dataUsed = 0;
			GetPropertyData(address, 0, NULL, sizeof(CMIOObjectPropertyScope), dataUsed, &answer);
		}
		
		return answer;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Control::GetPropertyScope()
	//	Get the CMIOObjectPropertyElement IN THE OWNING CMIOObject that contains the control.  (Controls themselves only have master element)
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	CMIOObjectPropertyElement Control::GetPropertyElement() const
	{
		// Setup the answer
		CMIOObjectPropertyElement answer = kCMIOObjectPropertyElementMaster;
		
		// Setup the property address
		PropertyAddress address(kCMIOControlPropertyElement);
		
		// Make sure the property exists
		if (HasProperty(address))
		{
			// Get the property data
			UInt32 dataUsed = 0;
			GetPropertyData(address, 0, NULL, sizeof(CMIOObjectPropertyElement), dataUsed, &answer);
		}
		
		return answer;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Control::GetVariant()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	UInt32 Control::GetVariant() const
	{
		// Setup the answer
		CMIOObjectPropertyElement answer = GetClassID();
		
		// Setup the property address
		PropertyAddress address(kCMIOControlPropertyVariant);
		
		// Make sure the property exists
		if (HasProperty(address))
		{
			// Get the property data
			UInt32 dataUsed = 0;
			GetPropertyData(address, 0, NULL, sizeof(UInt32), dataUsed, &answer);
		}
		
		return answer;
	}

	#pragma mark -
	#pragma mark Boolean Control
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// BooleanControl()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	BooleanControl::BooleanControl(CMIOObjectID cmioObjectID) :
		Control(cmioObjectID)
	{
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// ~BooleanControl()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	BooleanControl::~BooleanControl()
	{
	}

	bool BooleanControl::GetValue() const
	{
		UInt32 value = 0;	// default false
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIOBooleanControlPropertyValue), 0, NULL, sizeof(UInt32), dataUsed, &value);
		return (value ? true : false);
	}
	void BooleanControl::SetValue(bool value)
	{
		UInt32 propvalue = (value ? 1 : 0);
		UInt32 size = sizeof(UInt32);
		SetPropertyData(PropertyAddress(kCMIOBooleanControlPropertyValue), 0, NULL, size, &propvalue);
	}

	#pragma mark -
	#pragma mark Selector Control
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// FeatureControl()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	SelectorControl::SelectorControl(CMIOObjectID cmioObjectID) :
		Control(cmioObjectID)
	{
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// ~FeatureControl()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	SelectorControl::~SelectorControl()
	{
	}

	UInt32 SelectorControl::GetNumberItems() const
	{
		//	figure out the qualifier
		CMIOObjectID IDvalue;
		UInt32 theQualifierSize = 0;
		void* theQualifierData = NULL;
		theQualifierSize = sizeof(CMIOObjectID);
		theQualifierData = &IDvalue;
		
		//	figure out how much space to allocate
		UInt32 theDataSize = GetPropertyDataSize(PropertyAddress(kCMIOSelectorControlPropertyAvailableItems), theQualifierSize, theQualifierData);
		UInt32 theNumberObjectIDs = theDataSize / sizeof(CMIOObjectID);
		return (theNumberObjectIDs);
	}

	UInt32 SelectorControl::GetCurrentItemID() const
	{
		UInt32 itemID = 0xFFFFFFFF;	// Default to the featuring being on
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIOSelectorControlPropertyCurrentItem), 0, NULL, sizeof(UInt32), dataUsed, &itemID);
		return itemID;
	}
	
	UInt32 SelectorControl::GetCurrentItemIndex() const
	{
		return GetItemIndexForID(GetCurrentItemID());
	}

	void SelectorControl::SetCurrentItemByID(UInt32 inItemID)
	{
		UInt32 size = sizeof(UInt32);
		SetPropertyData(PropertyAddress(kCMIOSelectorControlPropertyCurrentItem),0,NULL,size,&inItemID);
	}
	
	void SelectorControl::SetCurrentItemByIndex(UInt32 inItemIndex)
	{
		UInt32 theItemID = GetItemIDForIndex(inItemIndex);
		SetCurrentItemByID(theItemID);
	}

	UInt32 SelectorControl::GetItemIDForIndex(UInt32 inItemIndex) const
	{
		UInt32 answer = 0xFFFFFFFF;	// Default to the featuring being on
		UInt32 numberOfItems = GetNumberItems();
		if (numberOfItems > 0)
		{
			ThrowIf(inItemIndex >= numberOfItems, CAException(kCMIOHardwareIllegalOperationError), "SelectorControl::GetItemIDForIndex: index out of range");			
			CAAutoArrayDelete<UInt32> itemList(numberOfItems);
			UInt32 size = numberOfItems * sizeof(CMIODeviceID);
			UInt32 dataUsed = 0;
			GetPropertyData(PropertyAddress(kCMIOSelectorControlPropertyAvailableItems),0, NULL, size, dataUsed, itemList);
			answer = itemList[inItemIndex];
		}
		return answer;
	}
	
	UInt32 SelectorControl::GetItemIndexForID(UInt32 inItemID) const
	{
		UInt32 answer = 0xFFFFFFFF;	// Default to the featuring being on
		UInt32 numberOfItems = GetNumberItems();
		if (numberOfItems > 0)
		{
			CAAutoArrayDelete<UInt32> itemList(numberOfItems);
			UInt32 size = numberOfItems * sizeof(CMIODeviceID);
			UInt32 dataUsed = 0;
			GetPropertyData(PropertyAddress(kCMIOSelectorControlPropertyAvailableItems),0, NULL, size, dataUsed, itemList);
			for(UInt32 index = 0; index < numberOfItems; ++index)
			{
				if (inItemID == itemList[index])
				{
					answer = index;
					break;
				}
			}
		}
		return answer;
	}

	CFStringRef SelectorControl::CopyItemNameByID(UInt32 inItemID) const
	{
		CFStringRef answer = NULL;
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIOSelectorControlPropertyItemName),sizeof(UInt32), &inItemID, sizeof(CFStringRef), dataUsed, &answer);
		return answer;
	}
	
	CFStringRef SelectorControl::CopyItemNameByIndex(UInt32 inItemIndex) const
	{
		CFStringRef answer = NULL;
		UInt32 numberOfItems = GetNumberItems();
		if (numberOfItems > 0)
		{
			ThrowIf(inItemIndex >= numberOfItems, CAException(kCMIOHardwareIllegalOperationError), "SelectorControl::CopyItemNameByIndex: index out of range");			
			CAAutoArrayDelete<UInt32> itemList(numberOfItems);
			UInt32 size = numberOfItems * sizeof(CMIODeviceID);
			UInt32 dataUsed = 0;
			GetPropertyData(PropertyAddress(kCMIOSelectorControlPropertyAvailableItems),0, NULL, size, dataUsed, itemList);
			answer = CopyItemNameByID(itemList[inItemIndex]);
		}
		return answer;
	}

	CFStringRef SelectorControl::CopyItemNameByIDWithoutLocalizing(UInt32 inItemID) const
	{
		return CopyItemNameByID(inItemID); 
	}
	
	CFStringRef SelectorControl::CopyItemNameByIndexWithoutLocalizing(UInt32 inItemIndex) const
	{
		return CopyItemNameByIndex(inItemIndex);
	}

	#pragma mark -
	#pragma mark Feature Control
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// FeatureControl()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	FeatureControl::FeatureControl(CMIOObjectID cmioObjectID) :
		Control(cmioObjectID)
	{
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// ~FeatureControl()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	FeatureControl::~FeatureControl()
	{
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// FeatureControl::OnOffSettable()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool FeatureControl::OnOffSettable() const
	{
		return IsPropertySettable(PropertyAddress(kCMIOFeatureControlPropertyOnOff));
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// FeatureControl::GetOnOff()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool FeatureControl::GetOnOff() const
	{
		UInt32 onOff = 1;	// Default to the featuring being on
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIOFeatureControlPropertyOnOff), 0, NULL, sizeof(UInt32), dataUsed, &onOff);
		return onOff != 0;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// FeatureControl::SetOnOff()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void FeatureControl::SetOnOff(bool onOff)
	{
		UInt32 value = (onOff ? 1 : 0);
		UInt32 size = sizeof(UInt32);
		SetPropertyData(PropertyAddress(kCMIOFeatureControlPropertyOnOff), 0, NULL, size, &value);
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// FeatureControl::AutomaticManualSettable()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool FeatureControl::AutomaticManualSettable() const
	{
		return IsPropertySettable(PropertyAddress(kCMIOFeatureControlPropertyAutomaticManual));
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// FeatureControl::GetAutomaticManual()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool FeatureControl::GetAutomaticManual() const
	{
		UInt32 automaticManual = 0;	// Default to the featuring being in manual mode
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIOFeatureControlPropertyAutomaticManual), 0, NULL, sizeof(UInt32), dataUsed, &automaticManual);
		return automaticManual != 0;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// FeatureControl::SetAutomaticManual()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void FeatureControl::SetAutomaticManual(bool automaticManual)
	{
		UInt32 value = (automaticManual ? 1 : 0);
		UInt32 size = sizeof(UInt32);
		SetPropertyData(PropertyAddress(kCMIOFeatureControlPropertyAutomaticManual), 0, NULL, size, &value);
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// FeatureControl::AbsoluteNativeSettable()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool FeatureControl::AbsoluteNativeSettable() const
	{
		return IsPropertySettable(PropertyAddress(kCMIOFeatureControlPropertyAbsoluteNative));
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// FeatureControl::GetAbsoluteNative()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool FeatureControl::GetAbsoluteNative() const
	{
		UInt32 absoluteNative = 0;	// Default to the featuring operating with native (i.e. unitless) values
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIOFeatureControlPropertyAbsoluteNative), 0, NULL, sizeof(UInt32), dataUsed, &absoluteNative);
		return absoluteNative != 0;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// FeatureControl::SetAbsoluteNative()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void FeatureControl::SetAbsoluteNative(bool absoluteNative)
	{
		UInt32 value = (absoluteNative ? 1 : 0);
		UInt32 size = sizeof(UInt32);
		SetPropertyData(PropertyAddress(kCMIOFeatureControlPropertyAbsoluteNative), 0, NULL, size, &value);
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// FeatureControl::HasTune()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool FeatureControl::HasTune() const
	{
		return HasProperty(PropertyAddress(kCMIOFeatureControlPropertyTune));
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// FeatureControl::GetTune()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool FeatureControl::GetTune() const
	{
		UInt32 tune = 0;	// Default to the featuring being not being tuned
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIOFeatureControlPropertyTune), 0, NULL, sizeof(UInt32), dataUsed, &tune);
		return tune != 0;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// FeatureControl::SetTune()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void FeatureControl::SetTune(bool tune)
	{
		UInt32 value = (tune ? 1 : 0);
		UInt32 size = sizeof(UInt32);
		SetPropertyData(PropertyAddress(kCMIOFeatureControlPropertyTune), 0, NULL, size, &value);
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// FeatureControl::HasNativeValue()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool FeatureControl::HasNativeValue() const
	{
		return HasProperty(PropertyAddress(kCMIOFeatureControlPropertyNativeValue));
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// FeatureControl::NativeValueSettable()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool FeatureControl::NativeValueSettable() const
	{
		return IsPropertySettable(PropertyAddress(kCMIOFeatureControlPropertyNativeValue));
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// FeatureControl::GetNativeValue()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	Float32 FeatureControl::GetNativeValue() const
	{
		Float32 nativeValue = 0.0;
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIOFeatureControlPropertyNativeValue), 0, NULL, sizeof(Float32), dataUsed, &nativeValue);
		return nativeValue;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// FeatureControl::SetNativeValue()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void FeatureControl::SetNativeValue(Float32 nativeValue)
	{
		UInt32 size = sizeof(Float32);
		SetPropertyData(PropertyAddress(kCMIOFeatureControlPropertyNativeValue), 0, NULL, size, &nativeValue);
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// FeatureControl::GetNativeValueRange()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	AudioValueRange FeatureControl::GetNativeValueRange() const
	{
		AudioValueRange range = { 0.0, 0.0 };
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIOFeatureControlPropertyNativeRange), 0, NULL, sizeof(AudioValueRange), dataUsed, &range);
		return range;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// FeatureControl::HasAbsoluteValue()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool FeatureControl::HasAbsoluteValue() const
	{
		return HasProperty(PropertyAddress(kCMIOFeatureControlPropertyAbsoluteValue));
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// FeatureControl::AbsoluteValueSettable()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool FeatureControl::AbsoluteValueSettable() const
	{
		return IsPropertySettable(PropertyAddress(kCMIOFeatureControlPropertyAbsoluteValue));
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// FeatureControl::GetAbsoluteValue()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	Float32 FeatureControl::GetAbsoluteValue() const
	{
		Float32 absoluteValue = 0.0;
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIOFeatureControlPropertyAbsoluteValue), 0, NULL, sizeof(Float32), dataUsed, &absoluteValue);
		return absoluteValue;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// FeatureControl::SetAbsoluteValue()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void FeatureControl::SetAbsoluteValue(Float32 absoluteValue)
	{
		UInt32 size = sizeof(Float32);
		SetPropertyData(PropertyAddress(kCMIOFeatureControlPropertyAbsoluteValue), 0, NULL, size, &absoluteValue);
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// FeatureControl::GetAbsoluteValueRange()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	AudioValueRange FeatureControl::GetAbsoluteValueRange() const
	{
		AudioValueRange range = { 0.0, 0.0 };
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIOFeatureControlPropertyAbsoluteRange), 0, NULL, sizeof(AudioValueRange), dataUsed, &range);
		return range;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// FeatureControl::HasNativeToAbsolute()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool FeatureControl::HasNativeToAbsolute() const
	{
		return HasProperty(PropertyAddress(kCMIOFeatureControlPropertyConvertNativeToAbsolute));
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// FeatureControl::ConverNativeValueToAbsoluteValue()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	Float32 FeatureControl::ConverNativeValueToAbsoluteValue(Float32 nativeValue) const
	{
		Float32 absoluteValue = 0.0;
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIOFeatureControlPropertyAbsoluteValue), sizeof(nativeValue), &nativeValue, sizeof(Float32), dataUsed, &absoluteValue);
		return absoluteValue;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// FeatureControl::HasAbsoluteToNative()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool FeatureControl::HasAbsoluteToNative() const
	{
		return HasProperty(PropertyAddress(kCMIOFeatureControlPropertyConvertAbsoluteToNative));
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// FeatureControl::ConverAbsoluteValueToNativeValue()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	Float32 FeatureControl::ConverAbsoluteValueToNativeValue(Float32 absoluteValue) const
	{
		Float32 nativeValue = 0.0;
		UInt32 dataUsed = 0;
		GetPropertyData(PropertyAddress(kCMIOFeatureControlPropertyConvertAbsoluteToNative), sizeof(absoluteValue), &absoluteValue, sizeof(Float32), dataUsed, &nativeValue);
		return nativeValue;
	}


}}
