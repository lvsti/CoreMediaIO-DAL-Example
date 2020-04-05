/*
     File: CAAUParameter.h
 Abstract: Part of CoreAudio Utility Classes
  Version: 1.1
*/
#ifndef __CAAUParameter_h__
#define __CAAUParameter_h__

#include <AudioToolbox/AudioUnitUtilities.h>

// ____________________________________________________________________________
//	CAAUParameter
//	complete parameter specification
	/*! @class CAAUParameter */
class CAAUParameter : public AudioUnitParameter {
public:
								/*! @ctor CAAUParameter.0 */
								CAAUParameter();
								/*! @ctor CAAUParameter.1 */
								CAAUParameter(AudioUnit au, AudioUnitParameterID param, AudioUnitScope scope, AudioUnitElement element);
								/*! @ctor CAAUParameter.2 */
								CAAUParameter(AudioUnitParameter &inParam);
								/*! @ctor CAAUParameter.3 */
								CAAUParameter(const CAAUParameter &a);
								/*! @dtor ~CAAUParameter */
								~CAAUParameter();
		
	/*! @method operator <@ */
	bool						operator < (const CAAUParameter &a) const 
								{ 
									return memcmp(this, &a, sizeof(AudioUnitParameter)) < 0; 
								}

	/*! @method operator ==@ */
	bool						operator == (const CAAUParameter &a) const 
								{ 
									return !memcmp(this, &a, sizeof(AudioUnitParameter)); 
								}
	
	/*! @method operator =@ */
	CAAUParameter &				operator = (const CAAUParameter &a);
	
	/*! @method GetValue */
	Float32						GetValue() const;
	/*! @method SetValue */
	void						SetValue(	AUParameterListenerRef			inListener, 
											void *							inObject,
											Float32							inValue) const;
	
	/*! @method GetName */
	CFStringRef					GetName() const { return mParamName; }  
										// borrowed reference!

	/*! @method GetStringFromValueCopy */
	CFStringRef					GetStringFromValueCopy(const Float32 *value = NULL) const;	
										// returns a copy of the name of the current parameter value
										// or null if there is no name associated
										// caller must release
	/*! @method ValuesHaveStrings */
	bool						ValuesHaveStrings () const 
								{ 
									return (mParamInfo.flags & kAudioUnitParameterFlag_ValuesHaveStrings) != 0; 
								}
	
	/*! @method GetValueFromString */
	Float32						GetValueFromString (CFStringRef str) const;					
									// caller must release

	/*! @method ParamInfo */
	const AudioUnitParameterInfo &		
								ParamInfo()	const { return mParamInfo; }

	/*! @method GetParamTag */
	CFStringRef					GetParamTag() const	{ return mParamTag; }
									// this may return null! - 
									// in which case there is no descriptive tag for the parameter

	/*! @method GetParamName */
	CFStringRef					GetParamName (int inIndex) const
									// this can return null if there is no name for the parameter
								{ 
									return (mNamedParams && inIndex < mNumIndexedParams) 
												? (CFStringRef) CFArrayGetValueAtIndex(mNamedParams, inIndex)
												: 0; 
								}
	
	/*! @method GetNumIndexedParams */
	int							GetNumIndexedParams () const { return mNumIndexedParams; }
	
	/*! @method IsIndexedParam */
	bool						IsIndexedParam () const { return mNumIndexedParams != 0; }
	
	/*! @method HasNamedParams */
	bool						HasNamedParams () const { return IsIndexedParam() && mNamedParams; }
	
	/*! @method GetClumpID */
	bool						GetClumpID (UInt32 &outClumpID) const 
								{ 
									if (mParamInfo.flags & kAudioUnitParameterFlag_HasClump) {
										outClumpID = mParamInfo.clumpID;
										return true;
									}
									return false;
								}
								
	/*! @method HasDisplayTransformation */
	bool						HasDisplayTransformation () const 
								{ 
									return GetAudioUnitParameterDisplayType (mParamInfo.flags); 
								}

	/*! @method IsExpert */
	bool						IsExpert () const 
								{ 
									return mParamInfo.flags & kAudioUnitParameterFlag_ExpertMode; 
								}
#if DEBUG
	void						Print () const;
#endif
	
		// these methods are defined in CAPersistence.cpp
		// they will persist and restore only the scope, element and param ID's of the AudioUnitParameter
		// however, this is sufficient to be able to save/restore a CAAUParameter object
	void						Save (CFPropertyListRef &outData) const;
	
	static void					Save (const AudioUnitParameter &inParam, CFPropertyListRef &outData);
	
	static OSStatus				Restore	(const CFPropertyListRef inData, AudioUnitParameter &outParam);

protected:
	// cached parameter info
	/*! @var mParamInfo */
	AudioUnitParameterInfo		mParamInfo;
	/*! @var mParamName */
	CFStringRef					mParamName;
	/*! @var mParamTag */
	CFStringRef					mParamTag;
	/*! @var mNumIndexedParams */
	short						mNumIndexedParams;
	/*! @var mNamedParams */
	CFArrayRef					mNamedParams;
	
private:
	void						Init (AudioUnit au, AudioUnitParameterID param, AudioUnitScope scope, AudioUnitElement element);

};



#endif // __CAAUParameter_h__
