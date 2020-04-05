/*
	    File: CMIO_DP_DeviceSettings.h
	Abstract: A place holder for saving and
	 restoring device settings.
	 Version: 1.2

*/

#if !defined(__CMIO_DP_DeviceSettings_h__)
#define __CMIO_DP_DeviceSettings_h__

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// System Includes
#include <CoreFoundation/CoreFoundation.h>

namespace CMIO { namespace DP
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Types in the CMIO::DP namespace
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class Device;

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// DeviceSettings
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class DeviceSettings
	{
	// Types
	public:
		struct ControlInfo
		{
			UInt32					mSelector;
			UInt32					mValueType;
		};

	// Constants
	public:
		enum
		{
									kControlValueTypeBool			= 0,
									kControlValueTypeFloat			= 1,
									kControlValueType4CC			= 2,
									kStandardNumberControlsToSave	= 0
		};

		static const ControlInfo	sStandardControlsToSave[];
		
	// Operations
	public:
		static void					SaveToPrefs(const Device& device, const ControlInfo* controlsToSave, UInt32 numberControlsToSave);
		static CFDictionaryRef		SaveToDictionary(const Device& device, const ControlInfo* controlsToSave, UInt32 numberControlsToSave);
		
		static void					RestoreFromPrefs(Device& device, const ControlInfo* controlsToRestore, UInt32 numberControlsToRestore);
		static void					RestoreFromDictionary(Device& device, const CFDictionaryRef dictionary, const ControlInfo* controlsToRestore, UInt32 numberControlsToRestore);
	};
}}
#endif
