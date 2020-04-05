/*
     File: CACFArray.cpp
 Abstract: CACFArray.h
  Version: 1.1

*/
//=============================================================================
//	Includes
//=============================================================================

//	Self Include
#include "CACFArray.h"

//	PublicUtility Includes
#include "CACFDictionary.h"
#include "CACFNumber.h"
#include "CACFString.h"

//=============================================================================
//	CACFArray
//=============================================================================

bool	CACFArray::HasItem(const void* inItem) const
{
	bool theAnswer = false;
	if(mCFArray != NULL)
	{
		CFRange theRange = { 0, CFArrayGetCount(mCFArray)};
		theAnswer = CFArrayContainsValue(mCFArray, theRange, inItem);
	}
	return theAnswer;
}

bool	CACFArray::GetIndexOfItem(const void* inItem, UInt32& outIndex) const
{
	bool theAnswer = false;
	outIndex = 0;
	if(mCFArray != NULL)
	{
		CFRange theRange = { 0, CFArrayGetCount(mCFArray)};
		CFIndex theIndex = CFArrayGetFirstIndexOfValue(mCFArray, theRange, inItem);
		if(theIndex != -1)
		{
			theAnswer = true;
			outIndex = ToUInt32(theIndex);
		}
	}
	return theAnswer;
}

bool	CACFArray::GetBool(UInt32 inIndex, bool& outValue) const
{
	bool theAnswer = false;
	
	CFTypeRef theValue = NULL;
	if(GetCFType(inIndex, theValue))
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

bool	CACFArray::GetSInt32(UInt32 inIndex, SInt32& outItem) const
{
	bool theAnswer = false;
	
	CFTypeRef theItem = NULL;
	if(GetCFType(inIndex, theItem))
	{
		if((theItem != NULL) && (CFGetTypeID(theItem) == CFNumberGetTypeID()))
		{
			CFNumberGetValue(static_cast<CFNumberRef>(theItem), kCFNumberSInt32Type, &outItem);
			theAnswer = true;
		}
	}
	
	return theAnswer;
}

bool	CACFArray::GetUInt32(UInt32 inIndex, UInt32& outItem) const
{
	bool theAnswer = false;
	
	CFTypeRef theItem = NULL;
	if(GetCFType(inIndex, theItem))
	{
		if((theItem != NULL) && (CFGetTypeID(theItem) == CFNumberGetTypeID()))
		{
			CFNumberGetValue(static_cast<CFNumberRef>(theItem), kCFNumberSInt32Type, &outItem);
			theAnswer = true;
		}
	}
	
	return theAnswer;
}

bool	CACFArray::GetSInt64(UInt32 inIndex, SInt64& outItem) const
{
	bool theAnswer = false;
	
	CFTypeRef theItem = NULL;
	if(GetCFType(inIndex, theItem))
	{
		if((theItem != NULL) && (CFGetTypeID(theItem) == CFNumberGetTypeID()))
		{
			CFNumberGetValue(static_cast<CFNumberRef>(theItem), kCFNumberSInt64Type, &outItem);
			theAnswer = true;
		}
	}
	
	return theAnswer;
}

bool	CACFArray::GetUInt64(UInt32 inIndex, UInt64& outItem) const
{
	bool theAnswer = false;
	
	CFTypeRef theItem = NULL;
	if(GetCFType(inIndex, theItem))
	{
		if((theItem != NULL) && (CFGetTypeID(theItem) == CFNumberGetTypeID()))
		{
			CFNumberGetValue(static_cast<CFNumberRef>(theItem), kCFNumberSInt64Type, &outItem);
			theAnswer = true;
		}
	}
	
	return theAnswer;
}

bool	CACFArray::GetFloat32(UInt32 inIndex, Float32& outItem) const
{
	bool theAnswer = false;
	
	CFTypeRef theItem = NULL;
	if(GetCFType(inIndex, theItem))
	{
		if((theItem != NULL) && (CFGetTypeID(theItem) == CFNumberGetTypeID()))
		{
			CFNumberGetValue(static_cast<CFNumberRef>(theItem), kCFNumberFloat32Type, &outItem);
			theAnswer = true;
		}
	}
	
	return theAnswer;
}

bool	CACFArray::GetFloat64(UInt32 inIndex, Float64& outItem) const
{
	bool theAnswer = false;
	
	CFTypeRef theItem = NULL;
	if(GetCFType(inIndex, theItem))
	{
		if((theItem != NULL) && (CFGetTypeID(theItem) == CFNumberGetTypeID()))
		{
			CFNumberGetValue(static_cast<CFNumberRef>(theItem), kCFNumberFloat64Type, &outItem);
			theAnswer = true;
		}
	}
	
	return theAnswer;
}

bool	CACFArray::Get4CC(UInt32 inIndex, UInt32& outValue) const
{
	bool theAnswer = false;
	
	CFTypeRef theValue = NULL;
	if(GetCFType(inIndex, theValue))
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

bool	CACFArray::GetString(UInt32 inIndex, CFStringRef& outItem) const
{
	bool theAnswer = false;
	
	CFTypeRef theItem = NULL;
	if(GetCFType(inIndex, theItem))
	{
		if((theItem != NULL) && (CFGetTypeID(theItem) == CFStringGetTypeID()))
		{
			outItem = static_cast<CFStringRef>(theItem);
			theAnswer = true;
		}
	}
	
	return theAnswer;
}

bool	CACFArray::GetArray(UInt32 inIndex, CFArrayRef& outItem) const
{
	bool theAnswer = false;
	
	CFTypeRef theItem = NULL;
	if(GetCFType(inIndex, theItem))
	{
		if((theItem != NULL) && (CFGetTypeID(theItem) == CFArrayGetTypeID()))
		{
			outItem = static_cast<CFArrayRef>(theItem);
			theAnswer = true;
		}
	}
	
	return theAnswer;
}

bool	CACFArray::GetDictionary(UInt32 inIndex, CFDictionaryRef& outItem) const
{
	bool theAnswer = false;
	
	CFTypeRef theItem = NULL;
	if(GetCFType(inIndex, theItem))
	{
		if((theItem != NULL) && (CFGetTypeID(theItem) == CFDictionaryGetTypeID()))
		{
			outItem = static_cast<CFDictionaryRef>(theItem);
			theAnswer = true;
		}
	}
	
	return theAnswer;
}

bool	CACFArray::GetData(UInt32 inIndex, CFDataRef& outItem) const
{
	bool theAnswer = false;
	
	CFTypeRef theItem = NULL;
	if(GetCFType(inIndex, theItem))
	{
		if((theItem != NULL) && (CFGetTypeID(theItem) == CFDataGetTypeID()))
		{
			outItem = static_cast<CFDataRef>(theItem);
			theAnswer = true;
		}
	}
	
	return theAnswer;
}

bool	CACFArray::GetUUID(UInt32 inIndex, CFUUIDRef& outItem) const
{
	bool theAnswer = false;
	
	CFTypeRef theItem = NULL;
	if(GetCFType(inIndex, theItem))
	{
		if((theItem != NULL) && (CFGetTypeID(theItem) == CFUUIDGetTypeID()))
		{
			outItem = static_cast<CFUUIDRef>(theItem);
			theAnswer = true;
		}
	}
	
	return theAnswer;
}

bool	CACFArray::GetCFType(UInt32 inIndex, CFTypeRef& outItem) const
{
	bool theAnswer = false;
	
	if((mCFArray != NULL) && (inIndex < GetNumberItems()))
	{
		outItem = CFArrayGetValueAtIndex(mCFArray, static_cast<CFIndex>(inIndex));
		theAnswer = outItem != NULL;
	}
	
	return theAnswer;
}

void	CACFArray::GetCACFString(UInt32 inIndex, CACFString& outItem) const
{
	outItem = static_cast<CFStringRef>(NULL);
	CFTypeRef theItem = NULL;
	if(GetCFType(inIndex, theItem))
	{
		if((theItem != NULL) && (CFGetTypeID(theItem) == CFStringGetTypeID()))
		{
			outItem = static_cast<CFStringRef>(theItem);
		}
	}
}

void	CACFArray::GetCACFArray(UInt32 inIndex, CACFArray& outItem) const
{
	outItem = static_cast<CFArrayRef>(NULL);
	CFTypeRef theItem = NULL;
	if(GetCFType(inIndex, theItem))
	{
		if((theItem != NULL) && (CFGetTypeID(theItem) == CFArrayGetTypeID()))
		{
			outItem = static_cast<CFArrayRef>(theItem);
		}
	}
}

void	CACFArray::GetCACFDictionary(UInt32 inIndex, CACFDictionary& outItem) const
{
	outItem = static_cast<CFDictionaryRef>(NULL);
	CFTypeRef theItem = NULL;
	if(GetCFType(inIndex, theItem))
	{
		if((theItem != NULL) && (CFGetTypeID(theItem) == CFDictionaryGetTypeID()))
		{
			outItem = static_cast<CFDictionaryRef>(theItem);
		}
	}
}

bool	CACFArray::AppendBool(bool inItem)
{
	bool theAnswer = false;
	
	if((mCFArray != NULL) && mMutable)
	{
		CACFBoolean theItem(inItem);
		if(theItem.IsValid())
		{
			theAnswer = AppendCFType(theItem.GetCFBoolean());
		}
	}
	
	return theAnswer;
}

bool	CACFArray::AppendSInt32(SInt32 inItem)
{
	bool theAnswer = false;
	
	if((mCFArray != NULL) && mMutable)
	{
		CACFNumber theItem(inItem);
		if(theItem.IsValid())
		{
			theAnswer = AppendCFType(theItem.GetCFNumber());
		}
	}
	
	return theAnswer;
}

bool	CACFArray::AppendUInt32(UInt32 inItem)
{
	bool theAnswer = false;
	
	if((mCFArray != NULL) && mMutable)
	{
		CACFNumber theItem(inItem);
		if(theItem.IsValid())
		{
			theAnswer = AppendCFType(theItem.GetCFNumber());
		}
	}
	
	return theAnswer;
}

bool	CACFArray::AppendSInt64(SInt64 inItem)
{
	bool theAnswer = false;
	
	if((mCFArray != NULL) && mMutable)
	{
		CACFNumber theItem(inItem);
		if(theItem.IsValid())
		{
			theAnswer = AppendCFType(theItem.GetCFNumber());
		}
	}
	
	return theAnswer;
}

bool	CACFArray::AppendUInt64(UInt64 inItem)
{
	bool theAnswer = false;
	
	if((mCFArray != NULL) && mMutable)
	{
		CACFNumber theItem(inItem);
		if(theItem.IsValid())
		{
			theAnswer = AppendCFType(theItem.GetCFNumber());
		}
	}
	
	return theAnswer;
}

bool	CACFArray::AppendFloat32(Float32 inItem)
{
	bool theAnswer = false;
	
	if((mCFArray != NULL) && mMutable)
	{
		CACFNumber theItem(inItem);
		if(theItem.IsValid())
		{
			theAnswer = AppendCFType(theItem.GetCFNumber());
		}
	}
	
	return theAnswer;
}

bool	CACFArray::AppendFloat64(Float64 inItem)
{
	bool theAnswer = false;
	
	if((mCFArray != NULL) && mMutable)
	{
		CACFNumber theItem(inItem);
		if(theItem.IsValid())
		{
			theAnswer = AppendCFType(theItem.GetCFNumber());
		}
	}
	
	return theAnswer;
}

bool	CACFArray::AppendString(const CFStringRef inItem)
{
	return AppendCFType(inItem);
}

bool	CACFArray::AppendArray(const CFArrayRef inItem)
{
	return AppendCFType(inItem);
}

bool	CACFArray::AppendDictionary(const CFDictionaryRef inItem)
{
	return AppendCFType(inItem);
}

bool	CACFArray::AppendData(const CFDataRef inItem)
{
	return AppendCFType(inItem);
}

bool	CACFArray::AppendCFType(const CFTypeRef inItem)
{
	bool theAnswer = false;
	
	if((mCFArray != NULL) && mMutable)
	{
		CFArrayAppendValue(mCFArray, inItem);
		theAnswer = true;
	}
	
	return theAnswer;
}

bool	CACFArray::InsertBool(UInt32 inIndex, bool inItem)
{
	bool theAnswer = false;
	
	if((mCFArray != NULL) && mMutable)
	{
		CACFBoolean theItem(inItem);
		if(theItem.IsValid())
		{
			theAnswer = InsertCFType(inIndex, theItem.GetCFBoolean());
		}
	}
	
	return theAnswer;
}

bool	CACFArray::InsertSInt32(UInt32 inIndex, SInt32 inItem)
{
	bool theAnswer = false;
	
	if((mCFArray != NULL) && mMutable)
	{
		CACFNumber theItem(inItem);
		if(theItem.IsValid())
		{
			theAnswer = InsertCFType(inIndex, theItem.GetCFNumber());
		}
	}
	
	return theAnswer;
}

bool	CACFArray::InsertUInt32(UInt32 inIndex, UInt32 inItem)
{
	bool theAnswer = false;
	
	if((mCFArray != NULL) && mMutable)
	{
		CACFNumber theItem(inItem);
		if(theItem.IsValid())
		{
			theAnswer = InsertCFType(inIndex, theItem.GetCFNumber());
		}
	}
	
	return theAnswer;
}

bool	CACFArray::InsertSInt64(UInt32 inIndex, SInt64 inItem)
{
	bool theAnswer = false;
	
	if((mCFArray != NULL) && mMutable)
	{
		CACFNumber theItem(inItem);
		if(theItem.IsValid())
		{
			theAnswer = InsertCFType(inIndex, theItem.GetCFNumber());
		}
	}
	
	return theAnswer;
}

bool	CACFArray::InsertUInt64(UInt32 inIndex, UInt64 inItem)
{
	bool theAnswer = false;
	
	if((mCFArray != NULL) && mMutable)
	{
		CACFNumber theItem(inItem);
		if(theItem.IsValid())
		{
			theAnswer = InsertCFType(inIndex, theItem.GetCFNumber());
		}
	}
	
	return theAnswer;
}

bool	CACFArray::InsertFloat32(UInt32 inIndex, Float32 inItem)
{
	bool theAnswer = false;
	
	if((mCFArray != NULL) && mMutable)
	{
		CACFNumber theItem(inItem);
		if(theItem.IsValid())
		{
			theAnswer = InsertCFType(inIndex, theItem.GetCFNumber());
		}
	}
	
	return theAnswer;
}

bool	CACFArray::InsertFloat64(UInt32 inIndex, Float64 inItem)
{
	bool theAnswer = false;
	
	if((mCFArray != NULL) && mMutable)
	{
		CACFNumber theItem(inItem);
		if(theItem.IsValid())
		{
			theAnswer = InsertCFType(inIndex, theItem.GetCFNumber());
		}
	}
	
	return theAnswer;
}

bool	CACFArray::InsertString(UInt32 inIndex, const CFStringRef inItem)
{
	return InsertCFType(inIndex, inItem);
}

bool	CACFArray::InsertArray(UInt32 inIndex, const CFArrayRef inItem)
{
	return InsertCFType(inIndex, inItem);
}

bool	CACFArray::InsertDictionary(UInt32 inIndex, const CFDictionaryRef inItem)
{
	return InsertCFType(inIndex, inItem);
}

bool	CACFArray::InsertData(UInt32 inIndex, const CFDataRef inItem)
{
	return InsertCFType(inIndex, inItem);
}

bool	CACFArray::InsertCFType(UInt32 inIndex, const CFTypeRef inItem)
{
	bool theAnswer = false;
	
	if((mCFArray != NULL) && mMutable)
	{
		if(inIndex < GetNumberItems())
		{
			CFArrayInsertValueAtIndex(mCFArray, static_cast<CFIndex>(inIndex), inItem);
		}
		else
		{
			CFArrayAppendValue(mCFArray, inItem);
		}
		theAnswer = true;
	}
	
	return theAnswer;
}

bool	CACFArray::SetBool(UInt32 inIndex, bool inItem)
{
	bool theAnswer = false;
	
	if((mCFArray != NULL) && mMutable && (inIndex <= GetNumberItems()))
	{
		CACFBoolean theItem(inItem);
		if(theItem.IsValid())
		{
			theAnswer = SetCFType(inIndex, theItem.GetCFBoolean());
		}
	}
	
	return theAnswer;
}

bool	CACFArray::SetSInt32(UInt32 inIndex, SInt32 inItem)
{
	bool theAnswer = false;
	
	if((mCFArray != NULL) && mMutable && (inIndex <= GetNumberItems()))
	{
		CACFNumber theItem(inItem);
		if(theItem.IsValid())
		{
			theAnswer = SetCFType(inIndex, theItem.GetCFNumber());
		}
	}
	
	return theAnswer;
}

bool	CACFArray::SetUInt32(UInt32 inIndex, UInt32 inItem)
{
	bool theAnswer = false;
	
	if((mCFArray != NULL) && mMutable && (inIndex <= GetNumberItems()))
	{
		CACFNumber theItem(inItem);
		if(theItem.IsValid())
		{
			theAnswer = SetCFType(inIndex, theItem.GetCFNumber());
		}
	}
	
	return theAnswer;
}

bool	CACFArray::SetSInt64(UInt32 inIndex, SInt64 inItem)
{
	bool theAnswer = false;
	
	if((mCFArray != NULL) && mMutable && (inIndex <= GetNumberItems()))
	{
		CACFNumber theItem(inItem);
		if(theItem.IsValid())
		{
			theAnswer = SetCFType(inIndex, theItem.GetCFNumber());
		}
	}
	
	return theAnswer;
}

bool	CACFArray::SetUInt64(UInt32 inIndex, UInt64 inItem)
{
	bool theAnswer = false;
	
	if((mCFArray != NULL) && mMutable && (inIndex <= GetNumberItems()))
	{
		CACFNumber theItem(inItem);
		if(theItem.IsValid())
		{
			theAnswer = SetCFType(inIndex, theItem.GetCFNumber());
		}
	}
	
	return theAnswer;
}

bool	CACFArray::SetFloat32(UInt32 inIndex, Float32 inItem)
{
	bool theAnswer = false;
	
	if((mCFArray != NULL) && mMutable && (inIndex <= GetNumberItems()))
	{
		CACFNumber theItem(inItem);
		if(theItem.IsValid())
		{
			theAnswer = SetCFType(inIndex, theItem.GetCFNumber());
		}
	}
	
	return theAnswer;
}

bool	CACFArray::SetFloat64(UInt32 inIndex, Float64 inItem)
{
	bool theAnswer = false;
	
	if((mCFArray != NULL) && mMutable && (inIndex <= GetNumberItems()))
	{
		CACFNumber theItem(inItem);
		if(theItem.IsValid())
		{
			theAnswer = SetCFType(inIndex, theItem.GetCFNumber());
		}
	}
	
	return theAnswer;
}

bool	CACFArray::SetString(UInt32 inIndex, const CFStringRef inItem)
{
	return SetCFType(inIndex, inItem);
}

bool	CACFArray::SetArray(UInt32 inIndex, const CFArrayRef inItem)
{
	return SetCFType(inIndex, inItem);
}

bool	CACFArray::SetDictionary(UInt32 inIndex, const CFDictionaryRef inItem)
{
	return SetCFType(inIndex, inItem);
}

bool	CACFArray::SetData(UInt32 inIndex, const CFDataRef inItem)
{
	return SetCFType(inIndex, inItem);
}

bool	CACFArray::SetCFType(UInt32 inIndex, const CFTypeRef inItem)
{
	bool theAnswer = false;
	
	if((mCFArray != NULL) && mMutable && (inIndex <= GetNumberItems()))
	{
		CFArraySetValueAtIndex(mCFArray, static_cast<CFIndex>(inIndex), inItem);
		theAnswer = true;
	}
	
	return theAnswer;
}
