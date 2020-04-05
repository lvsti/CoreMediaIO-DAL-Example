/*
     File: AUParamInfo.cpp
 Abstract: AUParamInfo.h
  Version: 1.1
*/
#include "AUParamInfo.h"
#include "CAXException.h"

AUParamInfo::AUParamInfo (AudioUnit				inAU, 
							bool				inIncludeExpert, 
							bool				inIncludeReadOnly,
							AudioUnitScope		inScope,
							AudioUnitElement	inElement)
	: mAU (inAU),
	  mNumParams (0),
	  mParamListID(NULL),
	  mScope (inScope),
	  mElement (inElement)
{
	UInt32 size;
	OSStatus result = AudioUnitGetPropertyInfo(mAU, kAudioUnitProperty_ParameterList, inScope, mElement, &size, NULL);
		if (size == 0 || result) return;
	
	int nparams = size / sizeof(AudioUnitPropertyID);
	mParamListID = new AudioUnitParameterID[nparams];

	memset (mParamListID, 0xFF, size);

	AudioUnitParameterID *paramList = new AudioUnitParameterID[nparams];
	
	result = AudioUnitGetProperty(mAU, kAudioUnitProperty_ParameterList, mScope, mElement, paramList, &size);
	if (result) {
		delete [] mParamListID;
		delete [] paramList;
		mParamListID = NULL;
		return;
	}
	
	ParameterMap params;
	for (int i = 0; i < nparams; ++i) 
	{
		CAAUParameter auvp (mAU, paramList[i], mScope, mElement); // took out only using global scope in CAAUParameter creation
		const AudioUnitParameterInfo &paramInfo = auvp.ParamInfo();
			
		//	don't include if parameter can't be read or written
		if (!(paramInfo.flags & kAudioUnitParameterFlag_IsWritable) 
			&& !(paramInfo.flags & kAudioUnitParameterFlag_IsReadable))
			continue;

		// only include if expert params wanted
		if (!inIncludeExpert && auvp.IsExpert())
			continue;
		
		// only include if read only params are wanted
		if (!(paramInfo.flags & kAudioUnitParameterFlag_IsWritable) 
			&& (paramInfo.flags & kAudioUnitParameterFlag_IsReadable))
		{	
			if (!inIncludeReadOnly)
				continue;
		}
		
		mParamListID[mNumParams] = paramList[i];
		mNumParams++;
		
		// ok - if we're here, then we have a parameter we are going to display.
		UInt32 clump = 0;
		auvp.GetClumpID (clump);
		mParams[clump].push_back (auvp);
	}

	delete [] paramList;
}

AUParamInfo::~AUParamInfo()
{
	delete [] mParamListID;
}

UInt32			AUParamInfo::NumParamsForClump (UInt32 inClump) const
{ 
	ParameterMap::const_iterator it = mParams.find(inClump);
	if (it != mParams.end()) 
		return static_cast<UInt32>((*it).second.size());
	return 0;
}

const CAAUParameter*	AUParamInfo::GetParamInfo (AudioUnitParameterID inParamID) const
{
	for (ParameterMap::const_iterator it = mParams.begin(); it != mParams.end(); ++it) {
		const ParameterList &list = (*it).second;
		for (ParameterList::const_iterator iter = list.begin(); iter != list.end(); ++iter) {
			if (inParamID == (*iter).mParameterID) {
				return &(*iter);
			}
		}
	}
	return NULL;
}
