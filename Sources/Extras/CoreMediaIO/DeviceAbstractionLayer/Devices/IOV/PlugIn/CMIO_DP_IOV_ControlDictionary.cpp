/*
	    File: CMIO_DP_IOV_ControlDictionary.cpp
	Abstract: User space utility functions that extract items from an IOV control dictionary.
	 Version: 1.2

*/

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Self Include
#include "CMIO_DP_IOV_ControlDictionary.h"

// Public Utility Includes
#include "CMIODebugMacros.h"

// CA Public Utility Includes
#include "CAException.h"
#include "CACFDictionary.h"

// System Includes
#include <IOKit/video/IOVideoTypes.h>

namespace CMIO { namespace DP { namespace IOV
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetControlID()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	UInt32 ControlDictionary::GetControlID(CFDictionaryRef inControlDictionary)
	{
		UInt32 theAnswer = 0;
		CACFDictionary theControlDictionary(inControlDictionary, false);
		bool hasValue = theControlDictionary.GetUInt32(CFSTR(kIOVideoControlKey_ControlID), theAnswer);
		ThrowIf(!hasValue, CAException(kCMIOHardwareIllegalOperationError), "CMIO:DP:IOV::ControlDictionary::GetControlID: couldn't get the control ID");
		return theAnswer;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetBaseClassID()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	CMIOClassID ControlDictionary::GetBaseClassID(CFDictionaryRef inControlDictionary)
	{
		CMIOClassID theAnswer = 0;
		CACFDictionary theControlDictionary(inControlDictionary, false);
		bool hasValue = theControlDictionary.GetUInt32(CFSTR(kIOVideoControlKey_BaseClass), theAnswer);
		ThrowIf(!hasValue, CAException(kCMIOHardwareIllegalOperationError), "CMIO:DP:IOV::ControlDictionary::GetBaseClassID: couldn't get the base class ID");
		return theAnswer;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetClassID()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	CMIOClassID ControlDictionary::GetClassID(CFDictionaryRef inControlDictionary)
	{
		CMIOClassID theAnswer = 0;
		CACFDictionary theControlDictionary(inControlDictionary, false);
		bool hasValue = theControlDictionary.GetUInt32(CFSTR(kIOVideoControlKey_Class), theAnswer);
		ThrowIf(!hasValue, CAException(kCMIOHardwareIllegalOperationError), "CMIO:DP:IOV::ControlDictionary::GetClassID: couldn't get the class ID");
		return theAnswer;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetPropertyScope()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	CMIOObjectPropertyScope ControlDictionary::GetPropertyScope(CFDictionaryRef inControlDictionary)
	{
		CMIOObjectPropertyScope theAnswer = 0;
		CACFDictionary theControlDictionary(inControlDictionary, false);
		bool hasValue = theControlDictionary.GetUInt32(CFSTR(kIOVideoControlKey_Scope), theAnswer);
		ThrowIf(!hasValue, CAException(kCMIOHardwareIllegalOperationError), "CMIO:DP:IOV::ControlDictionary::GetPropertyScope: couldn't get the control scope");
		return theAnswer;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetPropertyElement()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	CMIOObjectPropertyElement ControlDictionary::GetPropertyElement(CFDictionaryRef inControlDictionary)
	{
		CMIOObjectPropertyElement theAnswer = 0;
		CACFDictionary theControlDictionary(inControlDictionary, false);
		bool hasValue = theControlDictionary.GetUInt32(CFSTR(kIOVideoControlKey_Element), theAnswer);
		ThrowIf(!hasValue, CAException(kCMIOHardwareIllegalOperationError), "CMIO:DP:IOV::ControlDictionary::GetPropertyScope: couldn't get the control element");
		return theAnswer;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// IsReadOnly()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool ControlDictionary::IsReadOnly(CFDictionaryRef inControlDictionary)
	{
		bool theAnswer = false;
		CACFDictionary theControlDictionary(inControlDictionary, false);
		theControlDictionary.GetBool(CFSTR(kIOVideoControlKey_IsReadOnly), theAnswer);
		return theAnswer;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetVariant()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	UInt32 ControlDictionary::GetVariant(CFDictionaryRef inControlDictionary)
	{
		UInt32 theAnswer = 0;
		CACFDictionary theControlDictionary(inControlDictionary, false);
		theControlDictionary.GetUInt32(CFSTR(kIOVideoControlKey_Variant), theAnswer);
		return theAnswer;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// CopyName()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	CFStringRef ControlDictionary::CopyName(CFDictionaryRef inControlDictionary)
	{
		CFStringRef theAnswer = NULL;
		CACFDictionary theControlDictionary(inControlDictionary, false);
		theControlDictionary.GetString(CFSTR(kIOVideoControlKey_Name), theAnswer);
		if (NULL != theAnswer)
		{
			CFRetain(theAnswer);
		}
		return theAnswer;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetBooleanControlValue()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool ControlDictionary::GetBooleanControlValue(CFDictionaryRef inControlDictionary)
	{
		bool theAnswer = false;
		CACFDictionary theControlDictionary(inControlDictionary, false);
		bool hasValue = theControlDictionary.GetBool(CFSTR(kIOVideoControlKey_Value), theAnswer);
		ThrowIf(!hasValue, CAException(kCMIOHardwareIllegalOperationError), "CMIO:DP:IOV::ControlDictionary::GetBooleanControlValue: couldn't get the value");
		return theAnswer;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetSelectorControlValue()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	UInt32 ControlDictionary::GetSelectorControlValue(CFDictionaryRef inControlDictionary)
	{
		UInt32 theAnswer = 0;
		CACFDictionary theControlDictionary(inControlDictionary, false);
		bool hasValue = theControlDictionary.GetUInt32(CFSTR(kIOVideoControlKey_Value), theAnswer);
		ThrowIf(!hasValue, CAException(kCMIOHardwareIllegalOperationError), "CMIO:DP:IOV::ControlDictionary::GetSelectorControlValue: couldn't get the value");
		return theAnswer;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetSelectorControlValue()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	CFArrayRef ControlDictionary::CopySelectorControlSelectorMap(CFDictionaryRef inControlDictionary)
	{
		CFArrayRef theAnswer = NULL;
		CACFDictionary theControlDictionary(inControlDictionary, false);
		bool hasValue = theControlDictionary.GetArray(CFSTR(kIOVideoSelectorControlKey_SelectorMap), theAnswer);
		ThrowIf(!hasValue, CAException(kCMIOHardwareIllegalOperationError), "CMIO:DP:IOV::ControlDictionary::CopySelectorControlSelectorMap: couldn't get the selector map");
		if (NULL != theAnswer)
		{
			CFRetain(theAnswer);
		}
		return theAnswer;
	}
}}}
