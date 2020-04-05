/*
	    File: CMIO_DP_ControlDictionary.h
	Abstract: n/a
	 Version: 1.2

*/

#if !defined(__CMIO_DP_ControlDictionary_h__)
#define __CMIO_DP_ControlDictionary_h__

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Super Class Includes
#include "CMIO_DP_Object.h"

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Types in global namespace
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CACFArray;
class CACFDictionary;
class CACFString;

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ControlDictionary
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
namespace CMIO { namespace DP
{
	class ControlDictionary : public Object
	{
	// Construction/Destruction
	public:
		static CFMutableDictionaryRef	Create(UInt32 controlID, UInt32 baseClass, UInt32 derivedClass, UInt32 scope, UInt32 element, const CACFString& name, bool isReadOnly = false, UInt32 variant = 0);
		static CFMutableDictionaryRef	CreateLevelControl(UInt32 controlID, UInt32 baseClass, UInt32 derivedClass, UInt32 scope, UInt32 element, UInt32 value, CACFArray& rangeMap, const CACFString& name , UInt32 dbtoIntegerTransferFunction = 0, bool isReadOnly = false, UInt32 variant = 0);
		static CFMutableDictionaryRef	CreateBooleanControl(UInt32 controlID, UInt32 baseClass, UInt32 derivedClass, UInt32 scope, UInt32 element, bool value, const CACFString& name , bool isReadOnly = false, UInt32 variant = 0);
		static CFMutableDictionaryRef	CreateSelectorControl(UInt32 controlID, UInt32 baseClass, UInt32 derivedClass, UInt32 scope, UInt32 element, UInt32 value, CACFArray& selectorMap, const CACFString& name, bool isReadOnly = false, UInt32 variant = 0);

	// General Attributes
	public:
		static CFMutableDictionaryRef	GetControlByID(const CACFArray& controlList, UInt32 controlID);

		static UInt32					GetControlID(const CACFDictionary& dictionary);
		static void						SetControlID(CACFDictionary& dictionary, UInt32 controlID);

		static UInt32					GetBaseClass(const CACFDictionary& dictionary);
		static void						SetBaseClass(CACFDictionary& dictionary, UInt32 baseClass);

		static UInt32					GetClass(const CACFDictionary& dictionary);
		static void						SetClass(CACFDictionary& dictionary, UInt32 derivedClass);

		static UInt32					GetScope(const CACFDictionary& dictionary);
		static void						SetScope(CACFDictionary& dictionary, UInt32 scope);

		static UInt32					GetElement(const CACFDictionary& dictionary);
		static void						SetElement(CACFDictionary& dictionary, UInt32 element);

		static bool						IsReadOnly(const CACFDictionary& dictionary);
		static void						SetIsReadOnly(CACFDictionary& dictionary, bool isReadOnly);

		static UInt32					GetVariant(const CACFDictionary& dictionary);
		static void						SetVariant(CACFDictionary& dictionary, UInt32 variant);

		static CFStringRef				CopyName(const CACFDictionary& dictionary);
		static void						SetName(CACFDictionary& dictionary, const CACFString& name);

	// Boolean Control Attributes
	public:
		static bool						GetBooleanControlValue(const CACFDictionary& dictionary);
		static void						SetBooleanControlValue(CACFDictionary& dictionary, bool value);

	// Selector Control Attributes
	public:
		static UInt32					GetSelectorControlValue(const CACFDictionary& dictionary);
		static void						SetSelectorControlValue(CACFDictionary& dictionary, UInt32 value);

		static CFArrayRef				CopySelectorControlSelectorMap(const CACFDictionary& dictionary);
		static void						SetSelectorControlSelectorMap(CACFDictionary& dictionary, CACFArray& selectorMap);

	// Selector Control Selector Map Item Support
	public:
		static CFDictionaryRef			CreateSelectorControlSelectorMapItem(UInt32 value, const CACFString& name);
		static CFDictionaryRef			CreateSelectorControlSelectorMapItem(UInt32 value, const CACFString& name, UInt32 kind);
	};
}}

#endif
