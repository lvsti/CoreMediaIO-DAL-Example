/*
     File: CABool.h
 Abstract: Part of CoreAudio Utility Classes
  Version: 1.1
*/
#if !defined(__CABool_h__)
#define __CABool_h__

//=============================================================================
//	Includes
//=============================================================================

//	System Includes
#include "CADebugMacros.h"
#include "CAException.h"

//=============================================================================
//	CABool
//
//  This class implements a boolean value that has a third state that marks
//  it as uninitialized. Accessing the value of an instance of this class that
//  is uninitialized will throw an exception.
//=============================================================================

class CABool
{

//	Construction/Destruction
public:
					CABool() : mValue(-1) {}
					CABool(bool inValue) : mValue(inValue ? 1 : 0) {}
					CABool(const CABool& inValue) : mValue(inValue.mValue) {}
					~CABool() {}
					
	CABool&			operator=(bool inValue) { mValue = inValue; return *this; }
	CABool&			operator=(const CABool& inValue) { mValue = inValue.mValue; return *this; }
	
					operator bool() const { ThrowIf(mValue == -1, CAException('nope'), "CABool: uninitialized"); return mValue != 0; }
	bool			IsInitialized() const { return mValue != -1; }
	void			Uninitialize() { mValue = -1; }

private:
	SInt32			mValue;
	
					CABool(const void*);	// prevent accidental instantiation with a pointer via bool constructor
};

#endif
