/*
	    File: CMIO_DP_ControlDictionary.cpp
	Abstract: n/a
	 Version: 1.2

*/

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Self Include
#include "CMIO_DP_ControlDictionary.h"

// CA Public Utilility Includes
#include "CACFArray.h"
#include "CACFDictionary.h"
#include "CACFString.h"

// System Includes
#include <IOKit/IOTypes.h>
#include <IOKit/video/IOVideoTypes.h>

namespace
{
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetBoolean
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool GetBoolean(const CACFDictionary& dictionary, const CFStringRef key)
	{
		bool answer = false;
		dictionary.GetBool(key, answer);
		return answer;
	}
	
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetUInt32
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	UInt32 GetUInt32(const CACFDictionary& dictionary, const CFStringRef key)
	{
		UInt32 answer = 0;
		dictionary.GetUInt32(key, answer);
		return answer;
	}
	
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetUInt64
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	UInt64 GetUInt64(const CACFDictionary& dictionary, const CFStringRef key)
	{
		UInt64 answer = 0;
		dictionary.GetUInt64(key, answer);
		return answer;
	}
}

namespace CMIO { namespace DP
{
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Create()
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	CFMutableDictionaryRef ControlDictionary::Create(UInt32 controlID, UInt32 baseClass, UInt32 derivedClass, UInt32 scope, UInt32 element, const CACFString& name, bool isReadOnly, UInt32 variant)
	{
		CACFDictionary answer = CACFDictionary(false);

		if (answer.IsValid())
		{
			// Fill out the info, starting with the control ID
			answer.AddUInt32(CFSTR(kIOVideoControlKey_ControlID), controlID);
			
			// Do the base class
			answer.AddUInt32(CFSTR(kIOVideoControlKey_BaseClass), baseClass);
			
			// Do the class
			answer.AddUInt32(CFSTR(kIOVideoControlKey_Class), derivedClass);
			
			// Do the scope
			answer.AddUInt32(CFSTR(kIOVideoControlKey_Scope), scope);
			
			// Do the element
			answer.AddUInt32(CFSTR(kIOVideoControlKey_Element), element);
			
			// Do the is read only value
			answer.AddBool(CFSTR(kIOVideoControlKey_IsReadOnly), isReadOnly);
			
			// Do the variant
			answer.AddUInt32(CFSTR(kIOVideoControlKey_Variant), variant);
			
			// Do the name
			if (name.IsValid())
			{
				answer.AddString(CFSTR(kIOVideoControlKey_Name), name.GetCFString());
			}
		}

		return (answer.GetCFMutableDictionary());
	}

	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// CreateBooleanControl()
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	CFMutableDictionaryRef ControlDictionary::CreateBooleanControl(UInt32 controlID, UInt32 baseClass, UInt32 derivedClass, UInt32 scope, UInt32 element, bool value, const CACFString& name, bool isReadOnly, UInt32 variant)
	{
		// Do the general fields
		CACFDictionary answer(Create(controlID, baseClass, derivedClass, scope, element, name, isReadOnly, variant), false);
		if (answer.IsValid())
		{
			// Do the value
			answer.AddBool(CFSTR(kIOVideoControlKey_Value), value);
		}

		return (answer.GetCFMutableDictionary());
	}

	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// CreateSelectorControl()
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	CFMutableDictionaryRef ControlDictionary::CreateSelectorControl(UInt32 controlID, UInt32 baseClass, UInt32 derivedClass, UInt32 scope, UInt32 element, UInt32 value, CACFArray& selectorMap, const CACFString& name, bool isReadOnly, UInt32 variant)
	{
		// Do the general fields
		CACFDictionary answer(Create(controlID, baseClass, derivedClass, scope, element, name, isReadOnly, variant), false);
		if (answer.IsValid())
		{
			// Do the value
			answer.AddUInt32(CFSTR(kIOVideoControlKey_Value), value);
			
			// Do the range map
			if (selectorMap.IsValid())
			{
				answer.AddArray(CFSTR(kIOVideoSelectorControlKey_SelectorMap), selectorMap.GetCFArray());
			}
		}

		return (answer.GetCFMutableDictionary());
	}

	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetControlByID()
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	CFMutableDictionaryRef ControlDictionary::GetControlByID(const CACFArray& controlList, UInt32 controlID)
	{
		// Note that this routine require that the returned dictionary not be released
		CACFDictionary	answer;
		CFMutableDictionaryRef returnDict = NULL;
		CFDictionaryRef cfanswer;
		UInt32 count, i;
		
		if (controlList.IsValid())
		{
			count = controlList.GetNumberItems();
			for (i = 0 ; i< count; ++i)
			{
				if (controlList.GetDictionary(i, cfanswer))
				{
					CACFDictionary interimAnswer(cfanswer, false);
					if (interimAnswer.IsValid())
					{
						if (controlID == ControlDictionary::GetControlID(interimAnswer))
						{
							returnDict = interimAnswer.GetCFMutableDictionary();
							break;
						}
					}
				}
			}
		}
		
		return returnDict;
	}

	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetControlID()
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	UInt32 ControlDictionary::GetControlID(const CACFDictionary& dictionary)
	{
		return GetUInt32(dictionary, CFSTR(kIOVideoControlKey_ControlID));
	}

	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// SetControlID()
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void ControlDictionary::SetControlID(CACFDictionary& dictionary, UInt32 controlID)
	{
		dictionary.AddUInt32(CFSTR(kIOVideoControlKey_ControlID), controlID);
	}

	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetBaseClass()
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	UInt32 ControlDictionary::GetBaseClass(const CACFDictionary& dictionary)
	{
		return GetUInt32(dictionary, CFSTR(kIOVideoControlKey_BaseClass));
	}

	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// SetBaseClass()
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void ControlDictionary::SetBaseClass(CACFDictionary& dictionary, UInt32 baseClass)
	{
		dictionary.AddUInt32(CFSTR(kIOVideoControlKey_BaseClass), baseClass);
	}

	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetClass()
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	UInt32 ControlDictionary::GetClass(const CACFDictionary& dictionary)
	{
		return GetUInt32(dictionary, CFSTR(kIOVideoControlKey_Class));
	}

	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// SetClass()
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void ControlDictionary::SetClass(CACFDictionary& dictionary, UInt32 derivedClass)
	{
		dictionary.AddUInt32(CFSTR(kIOVideoControlKey_Class), derivedClass);
	}

	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetScope()
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	UInt32 ControlDictionary::GetScope(const CACFDictionary& dictionary)
	{
		return GetUInt32(dictionary, CFSTR(kIOVideoControlKey_Scope));
	}

	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// SetScope()
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void ControlDictionary::SetScope(CACFDictionary& dictionary, UInt32 scope)
	{
		dictionary.AddUInt32(CFSTR(kIOVideoControlKey_Scope), scope);
	}

	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetElement()
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	UInt32 ControlDictionary::GetElement(const CACFDictionary& dictionary)
	{
		return GetUInt32(dictionary, CFSTR(kIOVideoControlKey_Element));
	}

	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// SetElement()
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void ControlDictionary::SetElement(CACFDictionary& dictionary, UInt32 element)
	{
		dictionary.AddUInt32(CFSTR(kIOVideoControlKey_Element), element);
	}

	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// IsReadOnly()
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool ControlDictionary::IsReadOnly(const CACFDictionary& dictionary)
	{
		return GetBoolean(dictionary, CFSTR(kIOVideoControlKey_IsReadOnly));
	}

	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// SetIsReadOnly()
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void ControlDictionary::SetIsReadOnly(CACFDictionary& dictionary, bool isReadOnly)
	{
		dictionary.AddBool(CFSTR(kIOVideoControlKey_IsReadOnly), isReadOnly);
	}

	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetVariant()
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	UInt32 ControlDictionary::GetVariant(const CACFDictionary& dictionary)
	{
		return GetUInt32(dictionary, CFSTR(kIOVideoControlKey_Variant));
	}

	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// SetVariant()
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void ControlDictionary::SetVariant(CACFDictionary& dictionary, UInt32 variant)
	{
		dictionary.AddUInt32(CFSTR(kIOVideoControlKey_Variant), variant);
	}

	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// CopyName()
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	CFStringRef ControlDictionary::CopyName(const CACFDictionary& dictionary)
	{
		// Get the value from the dictionary and make sure it's a string
		CFStringRef answer;
		dictionary.GetString(CFSTR(kIOVideoControlKey_Name), answer);
		
		return answer;
	}

	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// SetName()
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void ControlDictionary::SetName(CACFDictionary& dictionary, const CACFString& name)
	{
		dictionary.AddString(CFSTR(kIOVideoControlKey_Name), name.GetCFString());
	}

	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetBooleanControlValue()
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool ControlDictionary::GetBooleanControlValue(const CACFDictionary& dictionary)
	{
		return GetBoolean(dictionary, CFSTR(kIOVideoControlKey_Value));
	}

	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// SetBooleanControlValue()
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void ControlDictionary::SetBooleanControlValue(CACFDictionary& dictionary, bool value)
	{
		dictionary.AddBool(CFSTR(kIOVideoControlKey_Value), value);
	}

	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetSelectorControlValue()
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	UInt32 ControlDictionary::GetSelectorControlValue(const CACFDictionary& dictionary)
	{
		return GetUInt32(dictionary, CFSTR(kIOVideoControlKey_Value));
	}

	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// SetSelectorControlValue()
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void ControlDictionary::SetSelectorControlValue(CACFDictionary& dictionary, UInt32 value)
	{
		dictionary.AddUInt32(CFSTR(kIOVideoControlKey_Value), value);
	}

	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// CopySelectorControlSelectorMap()
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	CFArrayRef ControlDictionary::CopySelectorControlSelectorMap(const CACFDictionary& dictionary)
	{
		CFArrayRef answer;
		
		// Get the value from the dictionary and make sure it's an array
		dictionary.GetArray(CFSTR(kIOVideoSelectorControlKey_SelectorMap), answer);
		
		return answer;
	}

	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// SetSelectorControlSelectorMap()
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void ControlDictionary::SetSelectorControlSelectorMap(CACFDictionary& dictionary, CACFArray& selectorMap)
	{
		dictionary.AddArray(CFSTR(kIOVideoSelectorControlKey_SelectorMap), selectorMap.GetCFArray());
	}

	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// CreateSelectorControlSelectorMapItem()
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	CFDictionaryRef ControlDictionary::CreateSelectorControlSelectorMapItem(UInt32 value, const CACFString& name)
	{
		CACFDictionary answer = CACFDictionary(false);
		if (answer.IsValid())
		{
			// Do the value
			answer.AddUInt32(CFSTR(kIOVideoSelectorControlSelectorMapItemKey_Value), value);
			
			// Do the starting name
			answer.AddString(CFSTR(kIOVideoSelectorControlSelectorMapItemKey_Name), name.GetCFString());
		}

		return (answer.GetCFDictionary());
	}

	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// CreateSelectorControlSelectorMapItem()
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	CFDictionaryRef ControlDictionary::CreateSelectorControlSelectorMapItem(UInt32 value, const CACFString& name, UInt32 kind)
	{
		CACFDictionary answer = CACFDictionary(false);
		if (answer.IsValid())
		{
			// Do the value
			answer.AddUInt32(CFSTR(kIOVideoSelectorControlSelectorMapItemKey_Value), value);
			
			// Do the starting name
			answer.AddString(CFSTR(kIOVideoSelectorControlSelectorMapItemKey_Name), name.GetCFString());
			
			// Do the kind
			answer.AddUInt32(CFSTR(kIOVideoSelectorControlSelectorMapItemKey_Kind), kind);
		}

		return (answer.GetCFDictionary());
	}
}}
