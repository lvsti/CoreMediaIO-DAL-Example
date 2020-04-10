/*
     File: CASettingsStorage.h
 Abstract: Part of CoreAudio Utility Classes
  Version: 1.1

*/
#if !defined(__CASettingsStorage_h__)
#define __CASettingsStorage_h__

//==================================================================================================
//	Includes
//==================================================================================================

//	System Includes
#include <CoreAudio/CoreAudioTypes.h>
#include <CoreFoundation/CoreFoundation.h>

//	Stamdard Library Includes
#include <stdio.h>
#include <sys/stat.h>

//==================================================================================================
//	CASettingsStorage
//==================================================================================================

class CASettingsStorage
{

//	Construction/Destruction
public:
							CASettingsStorage(const char* inSettingsFilePath, mode_t inSettingsFileAccessMode = 0, CFPropertyListFormat inSettingsCacheFormat = kCFPropertyListXMLFormat_v1_0, bool inIsSingleProcessOnly = false, bool inIsReadOnly = false);
							~CASettingsStorage();

//	Operations
public:
	UInt32					GetNumberKeys() const;
	void					GetKeys(UInt32 inNumberKeys, UInt32& outNumberKeys, CFStringRef* outKeys) const;

	void					CopyBoolValue(const CFStringRef inKey, bool& outValue, bool inDefaultValue = false) const;
	void					CopySInt32Value(const CFStringRef inKey, SInt32& outValue, SInt32 inDefaultValue = 0) const;
	void					CopyUInt32Value(const CFStringRef inKey, UInt32& outValue, UInt32 inDefaultValue = 0) const;
	void					CopySInt64Value(const CFStringRef inKey, SInt64& outValue, SInt64 inDefaultValue = 0) const;
	void					CopyUInt64Value(const CFStringRef inKey, UInt64& outValue, UInt64 inDefaultValue = 0) const;
	void					CopyFloat32Value(const CFStringRef inKey, Float32& outValue, Float32 inDefaultValue = 0) const;
	void					CopyFloat64Value(const CFStringRef inKey, Float64& outValue, Float64 inDefaultValue = 0) const;
	void					CopyNumberValue(const CFStringRef inKey, CFNumberRef& outValue, CFNumberRef inDefaultValue = NULL) const;	
	void					CopyStringValue(const CFStringRef inKey, CFStringRef& outValue, CFStringRef inDefaultValue = NULL) const;	
	void					CopyArrayValue(const CFStringRef inKey, CFArrayRef& outValue, CFArrayRef inDefaultValue = NULL) const;	
	void					CopyDictionaryValue(const CFStringRef inKey, CFDictionaryRef& outValue, CFDictionaryRef inDefaultValue = NULL) const;	
	void					CopyDataValue(const CFStringRef inKey, CFDataRef& outValue, CFDataRef inDefaultValue = NULL) const;
	void					CopyCFTypeValue(const CFStringRef inKey, CFTypeRef& outValue, CFTypeRef inDefaultValue = NULL) const;
	
	void					SetSInt32Value(const CFStringRef inKey, SInt32 inValue);
	void					SetUInt32Value(const CFStringRef inKey, UInt32 inValue);
	void					SetSInt64Value(const CFStringRef inKey, SInt64 inValue);
	void					SetUInt64Value(const CFStringRef inKey, UInt64 inValue);
	void					SetFloat32Value(const CFStringRef inKey, Float32 inValue);
	void					SetFloat64Value(const CFStringRef inKey, Float64 inValue);
	void					SetNumberValue(const CFStringRef inKey, const CFNumberRef inValue);
	void					SetStringValue(const CFStringRef inKey, const CFStringRef inValue);
	void					SetArrayValue(const CFStringRef inKey, const CFArrayRef inValue);
	void					SetDictionaryValue(const CFStringRef inKey, const CFDictionaryRef inValue);
	void					SetDataValue(const CFStringRef inKey, const CFDataRef inValue);
	void					SetCFTypeValue(const CFStringRef inKey, const CFTypeRef inValue);
	
	void					RemoveValue(const CFStringRef inKey);
	void					RemoveAllValues();
	
	void					SendNotification(const CFStringRef inName, CFDictionaryRef inData = NULL, bool inPostToAllSessions = true) const;
	void					ForceRefresh();

//	Implementation
private:
	void					RefreshSettings();
	void					SaveSettings();

	char*					mSettingsFilePath;
	mode_t					mSettingsFileAccessMode;
	CFMutableDictionaryRef	mSettingsCache;
	CFPropertyListFormat	mSettingsCacheFormat;
	struct timespec			mSettingsCacheTime;
	bool					mSettingsCacheForceRefresh;
	bool					mIsSingleProcessOnly;
	bool					mIsReadOnly;

};

#endif
