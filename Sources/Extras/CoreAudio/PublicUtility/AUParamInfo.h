/*
     File: AUParamInfo.h
 Abstract: Part of CoreAudio Utility Classes
  Version: 1.1
*/
#include <map>
#include <vector>
#include <AudioUnit/AudioUnit.h>
#include "CAAUParameter.h"

/*
	The ParameterMap returned by the Map() method is a map where 
		- the key is the clumpID
		- the value is a ParameterList (vector<CAAUParameter>)
		
	If you have parameters on multiple scopes (or elements within a scope), then you should create one of these 
	for each scope-element pair
*/

class AUParamInfo {

public:
	typedef std::vector <CAAUParameter> ParameterList;
	typedef std::map <UInt32, ParameterList, std::less<UInt32> > ParameterMap;



							AUParamInfo (AudioUnit		inAU, 
									bool				inIncludeExpert, 
									bool				inIncludeReadOnly, 
									AudioUnitScope		inScope = kAudioUnitScope_Global,
									AudioUnitElement	inElement = 0);
									
							~AUParamInfo();
							
	const ParameterMap&		Map () const { return mParams; }
	
	// some convenience methods
	UInt32					NumParams () const { return mNumParams; }
	
	AudioUnitParameterID	ParamID (UInt32 inIndex) const 
							{ 
								if (inIndex < mNumParams) return mParamListID[inIndex]; 
								return 0xFFFFFFFF; 
							}
							 
	UInt32					NumClumps () const { return static_cast<UInt32>(mParams.size()); }

	UInt32					NumParamsForClump (UInt32 inClump) const;
	
			// returns NULL if there's no info for the parameter
	const CAAUParameter*	GetParamInfo (AudioUnitParameterID inParamID) const;
	
	AudioUnitScope			GetScope () const { return mScope; }
	AudioUnitElement		GetElement () const { return mElement; }
	
private:

	AudioUnit				mAU;
	UInt32					mNumParams;
	AudioUnitParameterID *	mParamListID;
	
	ParameterMap			mParams;
	AudioUnitScope			mScope;
	AudioUnitElement		mElement;
		
		// disallow
	AUParamInfo () {}
	AUParamInfo (const AUParamInfo &c) {}
	AUParamInfo& operator= (const AUParamInfo& c) { return *this; }
};
