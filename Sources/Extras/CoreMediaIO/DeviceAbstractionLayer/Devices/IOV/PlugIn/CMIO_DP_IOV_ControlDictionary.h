/*
	    File: CMIO_DP_IOV_ControlDictionary.h
	Abstract: User space utility functions that extract items from an IOV control dictionary.
	 Version: 1.2

*/

#if !defined(__CMIO_DP_IOV_ControlDictionary_h__)
#define __CMIO_DP_IOV_ControlDictionary_h__

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// System Includes
#include <CoreFoundation/CoreFoundation.h>
#include <CoreMediaIO/CMIOHardware.h>

namespace CMIO { namespace DP { namespace IOV
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// ControlDictionary
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class ControlDictionary
	{
	public:
		static UInt32						GetControlID(CFDictionaryRef controlDictionary);
		static CMIOClassID					GetBaseClassID(CFDictionaryRef controlDictionary);
		static CMIOClassID					GetClassID(CFDictionaryRef controlDictionary);
		static CMIOObjectPropertyScope		GetPropertyScope(CFDictionaryRef controlDictionary);
		static CMIOObjectPropertyElement	GetPropertyElement(CFDictionaryRef controlDictionary);
		static bool							IsReadOnly(CFDictionaryRef controlDictionary);
		static UInt32						GetVariant(CFDictionaryRef controlDictionary);
		static CFStringRef					CopyName(CFDictionaryRef controlDictionary);

		static bool							GetBooleanControlValue(CFDictionaryRef controlDictionary);

		static UInt32						GetSelectorControlValue(CFDictionaryRef controlDictionary);
		static CFArrayRef					CopySelectorControlSelectorMap(CFDictionaryRef controlDictionary);
	};
}}}
#endif
