/*
     File: CACFDictionary.cpp
 Abstract: CACFDictionary.h
  Version: 1.1

*/
//=============================================================================
//	Includes
//=============================================================================

//	Self Include
#include "CACFDictionary.h"

//	PublicUtility Includes
#include "CACFArray.h"
#include "CACFNumber.h"
#include "CACFString.h"

//=============================================================================
//	CACFDictionary
//=============================================================================

bool	CACFDictionary::HasKey(const CFStringRef inKey) const
{
	return CFDictionaryContainsKey(mCFDictionary, inKey) != 0;
}

UInt32	CACFDictionary::Size () const
{
	return mCFDictionary ? ToUInt32(CFDictionaryGetCount(mCFDictionary)) : 0;
}

void	CACFDictionary::GetKeys (const void **keys) const
{
	CFDictionaryGetKeysAndValues(mCFDictionary, keys, NULL);
}

void	CACFDictionary::GetKeysAndValues (const void **keys, const void **values) const
{
	CFDictionaryGetKeysAndValues(mCFDictionary, keys, values);
}

bool	CACFDictionary::GetBool(const CFStringRef inKey, bool& outValue) const
{
	bool theAnswer = false;
	
	CFTypeRef theValue = NULL;
	if(GetCFType(inKey, theValue))
	{
		if((theValue != NULL) && (CFGetTypeID(theValue) == CFBooleanGetTypeID()))
		{
			outValue = CFBooleanGetValue(static_cast<CFBooleanRef>(theValue));
			theAnswer = true;
		}
		else if((theValue != NULL) && (CFGetTypeID(theValue) == CFNumberGetTypeID()))
		{
			SInt32 theNumericValue = 0;
			CFNumberGetValue(static_cast<CFNumberRef>(theValue), kCFNumberSInt32Type, &theNumericValue);
			outValue = theNumericValue != 0;
			theAnswer = true;
		}
	}
	
	return theAnswer;
}

bool	CACFDictionary::GetSInt32(const CFStringRef inKey, SInt32& outValue) const
{
	bool theAnswer = false;
	
	CFTypeRef theValue = NULL;
	if(GetCFType(inKey, theValue))
	{
		if((theValue != NULL) && (CFGetTypeID(theValue) == CFNumberGetTypeID()))
		{
			CFNumberGetValue(static_cast<CFNumberRef>(theValue), kCFNumberSInt32Type, &outValue);
			theAnswer = true;
		}
	}
	
	return theAnswer;
}

bool	CACFDictionary::GetUInt32(const CFStringRef inKey, UInt32& outValue) const
{
	bool theAnswer = false;
	
	CFTypeRef theValue = NULL;
	if(GetCFType(inKey, theValue))
	{
		if((theValue != NULL) && (CFGetTypeID(theValue) == CFNumberGetTypeID()))
		{
			CFNumberGetValue(static_cast<CFNumberRef>(theValue), kCFNumberSInt32Type, &outValue);
			theAnswer = true;
		}
	}
	
	return theAnswer;
}

bool	CACFDictionary::GetSInt64(const CFStringRef inKey, SInt64& outValue) const
{
	bool theAnswer = false;
	
	CFTypeRef theValue = NULL;
	if(GetCFType(inKey, theValue))
	{
		if((theValue != NULL) && (CFGetTypeID(theValue) == CFNumberGetTypeID()))
		{
			CFNumberGetValue(static_cast<CFNumberRef>(theValue), kCFNumberSInt64Type, &outValue);
			theAnswer = true;
		}
	}
	
	return theAnswer;
}

bool	CACFDictionary::GetUInt64(const CFStringRef inKey, UInt64& outValue) const
{
	bool theAnswer = false;
	
	CFTypeRef theValue = NULL;
	if(GetCFType(inKey, theValue))
	{
		if((theValue != NULL) && (CFGetTypeID(theValue) == CFNumberGetTypeID()))
		{
			CFNumberGetValue(static_cast<CFNumberRef>(theValue), kCFNumberSInt64Type, &outValue);
			theAnswer = true;
		}
	}
	
	return theAnswer;
}

bool	CACFDictionary::GetFloat32FromString(const CFStringRef inKey, Float32& outValue) const
{
	bool theAnswer = false;
	
	CFTypeRef theValue = NULL;
	if(GetCFType(inKey, theValue))
	{
		if((theValue != NULL) && (CFGetTypeID(theValue) == CFStringGetTypeID()))
		{
			outValue = static_cast<Float32>(CFStringGetDoubleValue(static_cast<CFStringRef>(theValue)));
		}
	}
	
	return theAnswer;
}

bool	CACFDictionary::GetUInt32FromString(const CFStringRef inKey, UInt32& outValue) const
{
	bool theAnswer = false;
	
	CFTypeRef theValue = NULL;
	if(GetCFType(inKey, theValue))
	{
		if((theValue != NULL) && (CFGetTypeID(theValue) == CFStringGetTypeID()))
		{
			outValue = CFStringGetIntValue(static_cast<CFStringRef>(theValue));
		}
	}
	
	return theAnswer;
}

bool	CACFDictionary::GetFloat32(const CFStringRef inKey, Float32& outValue) const
{
	bool theAnswer = false;
	
	CFTypeRef theValue = NULL;
	if(GetCFType(inKey, theValue))
	{
		if((theValue != NULL) && (CFGetTypeID(theValue) == CFNumberGetTypeID()))
		{
			CFNumberGetValue(static_cast<CFNumberRef>(theValue), kCFNumberFloat32Type, &outValue);
			theAnswer = true;
		}
	}
	
	return theAnswer;
}

bool	CACFDictionary::GetFloat64(const CFStringRef inKey, Float64& outValue) const
{
	bool theAnswer = false;
	
	CFTypeRef theValue = NULL;
	if(GetCFType(inKey, theValue))
	{
		if((theValue != NULL) && (CFGetTypeID(theValue) == CFNumberGetTypeID()))
		{
			CFNumberGetValue(static_cast<CFNumberRef>(theValue), kCFNumberFloat64Type, &outValue);
			theAnswer = true;
		}
	}
	
	return theAnswer;
}

bool	CACFDictionary::GetFixed32(const CFStringRef inKey, Float32& outValue) const
{
	bool theAnswer = false;
	
	CFTypeRef theValue = NULL;
	if(GetCFType(inKey, theValue))
	{
		if((theValue != NULL) && (CFGetTypeID(theValue) == CFNumberGetTypeID()))
		{
			SInt32 theFixed32 = 0;
			CFNumberGetValue(static_cast<CFNumberRef>(theValue), kCFNumberSInt32Type, &theFixed32);
			
			//	this is a 16.16 value so convert it to a float
			Float32 theSign = theFixed32 < 0 ? -1.0f : 1.0f;
			theFixed32 *= (SInt32)theSign;
			Float32 theWholePart = (theFixed32 & 0x7FFF0000) >> 16;
			Float32 theFractPart = theFixed32 & 0x0000FFFF;
			theFractPart /= 65536.0f;
			outValue = theSign * (theWholePart + theFractPart);
			theAnswer = true;
		}
	}
	
	return theAnswer;
}

bool	CACFDictionary::GetFixed64(const CFStringRef inKey, Float64& outValue) const
{
	bool theAnswer = false;
	
	CFTypeRef theValue = NULL;
	if(GetCFType(inKey, theValue))
	{
		if((theValue != NULL) && (CFGetTypeID(theValue) == CFNumberGetTypeID()))
		{
			SInt64 theFixed64 = 0;
			CFNumberGetValue(static_cast<CFNumberRef>(theValue), kCFNumberSInt64Type, &theFixed64);
			outValue = static_cast<Float64>(theFixed64 >> 32);
			outValue += static_cast<Float64>(theFixed64 & 0x00000000FFFFFFFFLL) / static_cast<Float64>(0x0000000100000000LL);
			theAnswer = true;
		}
	}
	
	return theAnswer;
}

bool	CACFDictionary::Get4CC(const CFStringRef inKey, UInt32& outValue) const
{
	bool theAnswer = false;
	
	CFTypeRef theValue = NULL;
	if(GetCFType(inKey, theValue))
	{
		if((theValue != NULL) && (CFGetTypeID(theValue) == CFNumberGetTypeID()))
		{
			CFNumberGetValue(static_cast<CFNumberRef>(theValue), kCFNumberSInt32Type, &outValue);
			theAnswer = true;
		}
		else if((theValue != NULL) && (CFGetTypeID(theValue) == CFStringGetTypeID()))
		{
			CFStringRef theString = static_cast<CFStringRef>(theValue);
			if(CFStringGetLength(theString) == 4)
			{
				char theCString[5];
				CFStringGetCString(theString, theCString, 5, kCFStringEncodingASCII);
				outValue = CFSwapInt32BigToHost(*reinterpret_cast<UInt32*>(theCString));
			}
		}
	}
	
	return theAnswer;
}

bool	CACFDictionary::GetString(const CFStringRef inKey, CFStringRef& outValue) const
{
	bool theAnswer = false;
	
	CFTypeRef theValue = NULL;
	if(GetCFType(inKey, theValue))
	{
		if((theValue != NULL) && (CFGetTypeID(theValue) == CFStringGetTypeID()))
		{
			outValue = static_cast<CFStringRef>(theValue);
			theAnswer = true;
		}
	}
	
	return theAnswer;
}
	
bool	CACFDictionary::GetArray(const CFStringRef inKey, CFArrayRef& outValue) const
{
	bool theAnswer = false;
	
	CFTypeRef theValue = NULL;
	if(GetCFType(inKey, theValue))
	{
		if((theValue != NULL) && (CFGetTypeID(theValue) == CFArrayGetTypeID()))
		{
			outValue = static_cast<CFArrayRef>(theValue);
			theAnswer = true;
		}
	}
	
	return theAnswer;
}
	
bool	CACFDictionary::GetDictionary(const CFStringRef inKey, CFDictionaryRef& outValue) const
{
	bool theAnswer = false;
	
	CFTypeRef theValue = NULL;
	if(GetCFType(inKey, theValue))
	{
		if((theValue != NULL) && (CFGetTypeID(theValue) == CFDictionaryGetTypeID()))
		{
			outValue = static_cast<CFDictionaryRef>(theValue);
			theAnswer = true;
		}
	}
	
	return theAnswer;
}

bool	CACFDictionary::GetData(const CFStringRef inKey, CFDataRef& outValue) const
{
	bool theAnswer = false;
	
	CFTypeRef theValue = NULL;
	if(GetCFType(inKey, theValue))
	{
		if((theValue != NULL) && (CFGetTypeID(theValue) == CFDataGetTypeID()))
		{
			outValue = static_cast<CFDataRef>(theValue);
			theAnswer = true;
		}
	}
	
	return theAnswer;
}

bool	CACFDictionary::GetCFType(const CFStringRef inKey, CFTypeRef& outValue) const
{
	bool theAnswer = false;
	
	if(mCFDictionary != NULL)
	{
		outValue = CFDictionaryGetValue(mCFDictionary, inKey);
		theAnswer = (outValue != NULL);
	}
	
	return theAnswer;
}

bool	CACFDictionary::GetURL(const CFStringRef inKey, CFURLRef& outValue) const
{
	bool theAnswer = false;
	
	CFTypeRef theValue = NULL;
	if(GetCFType(inKey, theValue))
	{
		if((theValue != NULL) && (CFGetTypeID(theValue) == CFURLGetTypeID()))
		{
			outValue = static_cast<CFURLRef>(theValue);
			theAnswer = true;
		}
	}
	
	return theAnswer;
}

bool	CACFDictionary::GetCFTypeWithCStringKey(const char* inKey, CFTypeRef& outValue) const
{
	bool theAnswer = false;
	
	if(mCFDictionary != NULL)
	{
		CACFString theKey(inKey);
		if(theKey.IsValid())
		{
			theAnswer = GetCFType(theKey.GetCFString(), outValue);
		}
	}
	
	return theAnswer;
}

void	CACFDictionary::GetCACFString(const CFStringRef inKey, CACFString& outValue) const
{
	outValue = static_cast<CFStringRef>(NULL);
	CFTypeRef theValue = NULL;
	if(GetCFType(inKey, theValue))
	{
		if((theValue != NULL) && (CFGetTypeID(theValue) == CFStringGetTypeID()))
		{
			outValue = static_cast<CFStringRef>(theValue);
		}
	}
}
	
void	CACFDictionary::GetCACFArray(const CFStringRef inKey, CACFArray& outValue) const
{
	outValue = static_cast<CFArrayRef>(NULL);
	CFTypeRef theValue = NULL;
	if(GetCFType(inKey, theValue))
	{
		if((theValue != NULL) && (CFGetTypeID(theValue) == CFArrayGetTypeID()))
		{
			outValue = static_cast<CFArrayRef>(theValue);
		}
	}
}
	
void	CACFDictionary::GetCACFDictionary(const CFStringRef inKey, CACFDictionary& outValue) const
{
	outValue = static_cast<CFDictionaryRef>(NULL);
	CFTypeRef theValue = NULL;
	if(GetCFType(inKey, theValue))
	{
		if((theValue != NULL) && (CFGetTypeID(theValue) == CFDictionaryGetTypeID()))
		{
			outValue = static_cast<CFDictionaryRef>(theValue);
		}
	}
}

bool	CACFDictionary::AddBool(const CFStringRef inKey, bool inValue)
{
	CACFBoolean theValue(inValue);
	return AddCFType(inKey, theValue.GetCFBoolean());
}

bool	CACFDictionary::AddSInt32(const CFStringRef inKey, SInt32 inValue)
{
	CACFNumber theValue(inValue);
	return AddCFType(inKey, theValue.GetCFNumber());
}

bool	CACFDictionary::AddUInt32(const CFStringRef inKey, UInt32 inValue)
{
	CACFNumber theValue(inValue);
	return AddCFType(inKey, theValue.GetCFNumber());
}

bool	CACFDictionary::AddSInt64(const CFStringRef inKey, SInt64 inValue)
{
	CACFNumber theValue(inValue);
	return AddCFType(inKey, theValue.GetCFNumber());
}

bool	CACFDictionary::AddUInt64(const CFStringRef inKey, UInt64 inValue)
{
	CACFNumber theValue(inValue);
	return AddCFType(inKey, theValue.GetCFNumber());
}

bool	CACFDictionary::AddFloat32(const CFStringRef inKey, Float32 inValue)
{
	CACFNumber theValue(inValue);
	return AddCFType(inKey, theValue.GetCFNumber());
}

bool	CACFDictionary::AddFloat64(const CFStringRef inKey, Float64 inValue)
{
	CACFNumber theValue(inValue);
	return AddCFType(inKey, theValue.GetCFNumber());
}

bool	CACFDictionary::AddNumber(const CFStringRef inKey, const CFNumberRef inValue)
{
	return AddCFType(inKey, inValue);
}

bool	CACFDictionary::AddString(const CFStringRef inKey, const CFStringRef inValue)
{
	return AddCFType(inKey, inValue);
}

bool	CACFDictionary::AddArray(const CFStringRef inKey, const CFArrayRef inValue)
{
	return AddCFType(inKey, inValue);
}

bool	CACFDictionary::AddDictionary(const CFStringRef inKey, const CFDictionaryRef inValue)
{
	return AddCFType(inKey, inValue);
}

bool	CACFDictionary::AddData(const CFStringRef inKey, const CFDataRef inValue)
{
	return AddCFType(inKey, inValue);
}

bool	CACFDictionary::AddURL(const CFStringRef inKey, const CFURLRef inValue)
{
	return AddCFType (inKey, inValue);
}

bool	CACFDictionary::AddCFTypeWithCStringKey(const char* inKey, const CFTypeRef inValue)
{
	bool theAnswer = false;
	
	if (inKey)
	{
		CACFString theKey(inKey);
		if(theKey.IsValid())
		{
			theAnswer = AddCFType(theKey.GetCFString(), inValue);
		}
	}
	
	return theAnswer;
}

bool	CACFDictionary::AddCString(const CFStringRef inKey, const char* inValue)
{
	bool theAnswer = false;
	
	if (inValue)
	{
		CACFString theValue(inValue);
		if(theValue.IsValid())
		{
			theAnswer = AddCFType(inKey, theValue.GetCFString());
		}
	}
	return theAnswer;
}

bool	CACFDictionary::AddCFType(const CFStringRef inKey, const CFTypeRef inValue)
{
	bool theAnswer = false;
	
	if(mMutable && (mCFDictionary != NULL) && inValue)
	{
		CFDictionarySetValue(mCFDictionary, inKey, inValue);
		theAnswer = true;
	}
	
	return theAnswer;
}
