/*
     File: CAComponentDescription.cpp
 Abstract: CAComponentDescription.h
  Version: 1.1

*/
#include "CAComponentDescription.h"
#include "CAStreamBasicDescription.h"
#include <ctype.h>

void CAShowComponentDescription(const AudioComponentDescription *desc)
{
	CAComponentDescription::_CAShowComponentDescription (desc, stdout);
}

void 	CAComponentDescription::_CAShowComponentDescription(const AudioComponentDescription *desc, FILE* file)
{
	if (desc)
	{
		char str[24];
		fprintf (file, "AudioComponentDescription: %s - ", CAStringForOSType(desc->componentType, str, sizeof(str)));
		fprintf (file, "%s - ", CAStringForOSType(desc->componentSubType, str, sizeof(str)));
		fprintf (file, "%s", CAStringForOSType(desc->componentManufacturer, str, sizeof(str)));		
		fprintf (file, ", 0x%X, 0x%X\n", (int)desc->componentFlags, (int)desc->componentFlagsMask);
	}
}

CAComponentDescription::CAComponentDescription (OSType inType, OSType inSubtype, OSType inManu)
{
	componentType = inType;
	componentSubType = inSubtype;
	componentManufacturer = inManu;
	componentFlags = 0;
	componentFlagsMask = 0;
}

bool	CAComponentDescription::IsAU () const 
{ 
	bool flag = IsEffect() || IsMusicDevice() || IsOffline();
	if (flag) return true;
	
	switch (componentType) {
		case kAudioUnitType_Output:
		case kAudioUnitType_FormatConverter:
		case kAudioUnitType_Mixer:
			return true;
	}
	return false;
}

inline bool _MatchTest (const OSType &inTypeA, const OSType &inTypeB)
{
	return ((inTypeA == inTypeB) || (!inTypeA && !inTypeB) || (inTypeA && !inTypeB) || (!inTypeA && inTypeB)); 
}

bool	CAComponentDescription::Matches (const AudioComponentDescription &desc) const
{
	bool matches = false;
		
		// see if the type matches
	matches = _MatchTest (componentType, desc.componentType);
	
	if (matches)
		matches = _MatchTest (componentSubType, desc.componentSubType);
	
	if (matches)
		matches = _MatchTest (componentManufacturer, desc.componentManufacturer);
	
	return matches;
}
