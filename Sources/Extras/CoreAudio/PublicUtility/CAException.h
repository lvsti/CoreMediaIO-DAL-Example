/*
     File: CAException.h
 Abstract: Part of CoreAudio Utility Classes
  Version: 1.1

*/
#if !defined(__CAException_h__)
#define __CAException_h__

//=============================================================================
//	Includes
//=============================================================================

#if !defined(__COREAUDIO_USE_FLAT_INCLUDES__)
	#include <CoreAudio/CoreAudioTypes.h>
#else
	#include "CoreAudioTypes.h"
#endif

//=============================================================================
//	CAException
//=============================================================================

class CAException
{

public:
					CAException(OSStatus inError) : mError(inError) {}
					CAException(const CAException& inException) : mError(inException.mError) {}
	CAException&	operator=(const CAException& inException) { mError = inException.mError; return *this; }
					~CAException() {}

	OSStatus		GetError() const { return mError; }
	
protected:
	OSStatus		mError;
};

#define	CATry								try{
#define CACatch								} catch(...) {}
#define	CASwallowException(inExpression)	try { inExpression; } catch(...) {}

#endif
