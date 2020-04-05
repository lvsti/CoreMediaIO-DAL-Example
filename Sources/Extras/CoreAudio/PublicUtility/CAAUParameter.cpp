/*
     File: CAAUParameter.cpp
 Abstract: CAAUParameter.h
  Version: 1.1
*/
#include "CAAUParameter.h"

CAAUParameter::CAAUParameter() 
{
	memset(this, 0, sizeof(CAAUParameter));
}

CAAUParameter::CAAUParameter(AudioUnit au, AudioUnitParameterID param, AudioUnitScope scope, AudioUnitElement element)
{
	memset(this, 0, sizeof(CAAUParameter));
	Init (au, param, scope, element);
}

CAAUParameter::CAAUParameter (AudioUnitParameter &inParam)
{
	memset(this, 0, sizeof(CAAUParameter));
	Init (inParam.mAudioUnit, inParam.mParameterID, inParam.mScope, inParam.mElement);
}

CAAUParameter::CAAUParameter(const CAAUParameter &a) 
{
	memset(this, 0, sizeof(CAAUParameter));
	*this = a;
}

CAAUParameter &	CAAUParameter::operator = (const CAAUParameter &a)
{
	if (mParamName) CFRelease(mParamName);
	if (mParamTag) CFRelease(mParamTag);
	if (mNamedParams) CFRelease(mNamedParams);
	
	memcpy(this, &a, sizeof(CAAUParameter));

	if (mParamName) CFRetain(mParamName);
	if (mParamTag) CFRetain(mParamTag);
	if (mNamedParams) CFRetain(mNamedParams);
	
	return *this;
}

CAAUParameter::~CAAUParameter()
{
	if (mParamName) CFRelease(mParamName);
	if (mParamTag) CFRelease(mParamTag);
	if (mNamedParams) CFRelease (mNamedParams);
}

void		CAAUParameter::Init (AudioUnit au, AudioUnitParameterID param, AudioUnitScope scope, AudioUnitElement element)
{
	mAudioUnit = au;
	mParameterID = param;
	mScope = scope;
	mElement = element;
	
	UInt32 propertySize = sizeof(mParamInfo);
	OSStatus err = AudioUnitGetProperty(au, kAudioUnitProperty_ParameterInfo,
			scope, param, &mParamInfo, &propertySize);
	if (err)
		memset(&mParamInfo, 0, sizeof(mParamInfo));
	if (mParamInfo.flags & kAudioUnitParameterFlag_HasCFNameString) {
		mParamName = mParamInfo.cfNameString;
		if (!(mParamInfo.flags & kAudioUnitParameterFlag_CFNameRelease)) 
			CFRetain (mParamName);
	} else
		mParamName = CFStringCreateWithCString(NULL, mParamInfo.name, kCFStringEncodingUTF8);
	
	const char* str = 0;
	switch (mParamInfo.unit)
	{
		case kAudioUnitParameterUnit_Boolean:
			str = "T/F";
			break;
		case kAudioUnitParameterUnit_Percent:
		case kAudioUnitParameterUnit_EqualPowerCrossfade:
			str = "%";
			break;
		case kAudioUnitParameterUnit_Seconds:
			str = "Secs";
			break;
		case kAudioUnitParameterUnit_SampleFrames:
			str = "Samps";
			break;
		case kAudioUnitParameterUnit_Phase:
		case kAudioUnitParameterUnit_Degrees:
			str = "Degr.";
			break;
		case kAudioUnitParameterUnit_Hertz:
			str = "Hz";
			break;
		case kAudioUnitParameterUnit_Cents:
		case kAudioUnitParameterUnit_AbsoluteCents:
			str = "Cents";
			break;
		case kAudioUnitParameterUnit_RelativeSemiTones:
			str = "S-T";
			break;
		case kAudioUnitParameterUnit_MIDINoteNumber:
		case kAudioUnitParameterUnit_MIDIController:
			str = "MIDI";
				//these are inclusive, so add one value here
			mNumIndexedParams = short(mParamInfo.maxValue+1 - mParamInfo.minValue);
			break;
		case kAudioUnitParameterUnit_Decibels:
			str = "dB";
			break;
		case kAudioUnitParameterUnit_MixerFaderCurve1:
		case kAudioUnitParameterUnit_LinearGain:
			str = "Gain";
			break;
		case kAudioUnitParameterUnit_Pan:
			str = "L/R";
			break;
		case kAudioUnitParameterUnit_Meters:
			str = "Mtrs";
			break;
		case kAudioUnitParameterUnit_Octaves:
			str = "8ve";
			break;
		case kAudioUnitParameterUnit_BPM:
			str = "BPM";
			break;
		case kAudioUnitParameterUnit_Beats:
			str = "Beats";
			break;
		case kAudioUnitParameterUnit_Milliseconds:
			str = "msecs";
			break;
		case kAudioUnitParameterUnit_Ratio:
			str = "Ratio";
			break;
		case kAudioUnitParameterUnit_Indexed:
			{
				propertySize = sizeof(mNamedParams);
				err = AudioUnitGetProperty (au, 
									kAudioUnitProperty_ParameterValueStrings,
									scope, 
									param, 
									&mNamedParams, 
									&propertySize);
				if (!err && mNamedParams) {
					mNumIndexedParams = CFArrayGetCount(mNamedParams);
				} else {
						//these are inclusive, so add one value here
					mNumIndexedParams = short(mParamInfo.maxValue+1 - mParamInfo.minValue);
				}
				str = NULL;
			}
			break;
		case kAudioUnitParameterUnit_CustomUnit:
		{
			CFStringRef unitName = mParamInfo.unitName;
			static char paramStr[256];
			CFStringGetCString (unitName, paramStr, 256, kCFStringEncodingUTF8);
			if (mParamInfo.flags & kAudioUnitParameterFlag_CFNameRelease)
				CFRelease (unitName);
			str = paramStr;
			break;
		}
		case kAudioUnitParameterUnit_Generic:
		case kAudioUnitParameterUnit_Rate:
		default:
			str = NULL;
			break;
	}
	
	if (str)
		mParamTag = CFStringCreateWithCString(NULL, str, kCFStringEncodingUTF8);
	else
		mParamTag = NULL;
}


Float32		CAAUParameter::GetValue() const
{
	Float32 value = 0.;
	//OSStatus err = 
	AudioUnitGetParameter(mAudioUnit, mParameterID, mScope, mElement, &value);
	return value;
}

CFStringRef CreateLocalizedStringForParameterValue ( double					inParameterValue,
													 const CAAUParameter *	inParameter,
													 UInt32					inDigits,
													 UInt32					minDigits) {	
	if (!inParameter) return nil;

	AudioUnitParameterInfo info = inParameter->ParamInfo();
	int pow10;

	switch (info.unit) {
		case kAudioUnitParameterUnit_Hertz:
			// number of significant digits based on value
			pow10 = int(log10(fmax(inParameterValue, .000001)));
			break;
		default:
			// number of significant digits based on parameter range
			pow10 = int(log10(fmax(double(info.maxValue - info.minValue), .000001)));
			break;
	}

	// pow10	range			nDigitsAfterDecimal
	//	-2		.0100-.0999		4
	//	-1		.100-.999		3
	//	0		1.00-9.99		2
	//	1		10.0-99.9		1
	//	2		100-999			0
	//	3		1000-9990		-1
	//	4		10000-99900		-2
	
	int nDigitsAfterDecimal = inDigits - (pow10 + 1);
	if (nDigitsAfterDecimal < 0)
		nDigitsAfterDecimal = 0;	// the least number of digits possible is zero

	if (info.flags & kAudioUnitParameterFlag_IsHighResolution)
		nDigitsAfterDecimal = 4;
	
	CFLocaleRef currentLocale = CFLocaleCopyCurrent(); 
	CFNumberFormatterRef numberFormatter = CFNumberFormatterCreate (NULL, currentLocale, kCFNumberFormatterDecimalStyle);
	
	CFNumberRef maxFractionDigits = CFNumberCreate (NULL, kCFNumberIntType, &nDigitsAfterDecimal);
	
	if (nDigitsAfterDecimal > 0)
		nDigitsAfterDecimal = minDigits;
		
	CFNumberRef minFractionDigits = CFNumberCreate (NULL, kCFNumberIntType, &nDigitsAfterDecimal); 

	CFNumberFormatterSetProperty (numberFormatter, kCFNumberFormatterMinFractionDigits, minFractionDigits); 
	CFNumberFormatterSetProperty (numberFormatter, kCFNumberFormatterMaxFractionDigits, maxFractionDigits); 
	CFStringRef formattedNumberString = CFNumberFormatterCreateStringWithValue (NULL, numberFormatter, kCFNumberDoubleType, &inParameterValue); 

	CFRelease(currentLocale); 
	CFRelease(numberFormatter); 
	CFRelease(maxFractionDigits);
	CFRelease(minFractionDigits);

	return formattedNumberString;
}

CFStringRef CreateLocalizedStringForParameterValue ( double					inParameterValue,
													 const CAAUParameter *	inParameter,
													 UInt32					inDigits) {	
	return CreateLocalizedStringForParameterValue (inParameterValue, inParameter, inDigits, 1);
}

double ValueForLocalizedParameterString (CFStringRef string, const CAAUParameter * inParameter) {
	CFLocaleRef currentLocale = CFLocaleCopyCurrent(); 
	CFNumberFormatterRef numberFormatter = CFNumberFormatterCreate (NULL, currentLocale, kCFNumberFormatterDecimalStyle);

	double value = 0;
	Boolean worked = CFNumberFormatterGetValueFromString (numberFormatter, string, NULL, kCFNumberDoubleType, &value);
	
	CFRelease(currentLocale);
	CFRelease(numberFormatter);
	
	if (worked)
		return value;
	else {
		AudioUnitParameterInfo info = inParameter->ParamInfo();
		return info.defaultValue;
	}
}

CFStringRef CAAUParameter::GetStringFromValueCopy(const Float32 *value) const
{
	if (HasNamedParams())
	{
		Float32 val = (value == NULL ? GetValue() : *value);
		int index = int(mParamInfo.minValue) + int(val);
		CFStringRef str = GetParamName (index);
		if (str) {
			CFRetain (str);
			return str;
		}
	}
	else if (ValuesHaveStrings()) 
	{
		AudioUnitParameterStringFromValue stringValue;
		stringValue.inParamID = mParameterID;
		stringValue.inValue = value;
		stringValue.outString = NULL;
		UInt32 propertySize = sizeof(stringValue);
		
		OSStatus err = AudioUnitGetProperty (mAudioUnit, 
											kAudioUnitProperty_ParameterStringFromValue,
											mScope, 
											0, 
											&stringValue, 
											&propertySize);
		
		if (!err && stringValue.outString != NULL)
			return stringValue.outString;
	}
	
	Float32 val = (value == NULL ? GetValue() : *value);
	AudioUnitParameterUnit unit = this->ParamInfo().unit;
	if (unit ==  kAudioUnitParameterUnit_Cents || unit == kAudioUnitParameterUnit_AbsoluteCents)
		return CreateLocalizedStringForParameterValue(val, this, 4, 0);
	else
		return CreateLocalizedStringForParameterValue(val, this, 4);
}

Float32 CAAUParameter::GetValueFromString(CFStringRef str) const
{
	if (ValuesHaveStrings()) 
	{
		AudioUnitParameterValueFromString valueString;
		valueString.inParamID = mParameterID;
		valueString.inString = str;
		UInt32 propertySize = sizeof(valueString);
		
		OSStatus err = AudioUnitGetProperty (mAudioUnit, 
										kAudioUnitProperty_ParameterValueFromString,
										mScope, 
										0, 
										&valueString, 
										&propertySize);
										
		if (!err) {
			return valueString.outValue;
		}
	}
	
	return (Float32) ValueForLocalizedParameterString(str, this);
}

void		CAAUParameter::SetValue(	AUParameterListenerRef		inListener, 
									void *							inObject,
									Float32							inValue) const
{
    // clip inValue as: maxValue >= inValue >= minValue before setting
    Float32 valueToSet = inValue;
    if (valueToSet > mParamInfo.maxValue)
        valueToSet = mParamInfo.maxValue;
    if (valueToSet < mParamInfo.minValue)
        valueToSet = mParamInfo.minValue;
    
	AUParameterSet(inListener, inObject, this, valueToSet, 0);
}

#if DEBUG
void	CAAUParameter::Print() const
{
	UInt32 clump = 0;
	GetClumpID (clump);
	
	UInt32 len = static_cast<UInt32>(CFStringGetLength(mParamName));
	char* chars = (char*)malloc (len * 2); // give us plenty of room for unichar chars
	if (!CFStringGetCString (mParamName, chars, len * 2, kCFStringEncodingUTF8))
		chars[0] = 0;
	
	printf ("ID: %ld, Clump: %u, Name: %s\n", (long unsigned int) mParameterID, (unsigned int) clump, chars);
	free (chars);
}
#endif
