/*
     File: CACFString.cpp
 Abstract: CACFString.h
  Version: 1.1

*/
//=============================================================================
//	Includes
//=============================================================================

#include "CACFString.h"

//=============================================================================
//	CACFString
//=============================================================================

UInt32	CACFString::GetStringByteLength(CFStringRef inCFString, CFStringEncoding inEncoding)
{
	CFIndex theAnswer = 0;
	
	if(inCFString != NULL)
	{
		CFRange theRange = { 0, CFStringGetLength(inCFString) };
		CFStringGetBytes(inCFString, theRange, inEncoding, 0, false, NULL, 0x7FFFFFFF, &theAnswer);
	}
	
	return UInt32(theAnswer);
}

void	CACFString::GetCString(CFStringRef inCFString, char* outString, UInt32& ioStringSize, CFStringEncoding inEncoding)
{
	if(ioStringSize > 0)
	{
		if(inCFString != NULL)
		{
			CFIndex theLength = 0;
			CFRange theRange = { 0, CFStringGetLength(inCFString) };
			CFStringGetBytes(inCFString, theRange, inEncoding, 0, false, (UInt8*)outString, static_cast<CFIndex>(ioStringSize - 1), &theLength);
			outString[theLength] = 0;
			ioStringSize = ToUInt32(theLength) + 1;
		}
		else
		{
			outString[0] = 0;
			ioStringSize = 1;
		}
	}
}

void	CACFString::GetUnicodeString(CFStringRef inCFString, UInt16* outString, UInt32& ioStringSize)
{
	if(ioStringSize > 0)
	{
		if(inCFString != NULL)
		{
			CFRange theStringRange = { 0, CFStringGetLength(inCFString) };
			if(static_cast<UInt32>(theStringRange.length) > ioStringSize)
			{
				theStringRange.length = static_cast<CFIndex>(ioStringSize);
			}
			CFStringGetCharacters(inCFString, theStringRange, outString);
			ioStringSize = ToUInt32(theStringRange.length);
		}
		else
		{
			outString[0] = 0;
			ioStringSize = 0;
		}
	}
}
