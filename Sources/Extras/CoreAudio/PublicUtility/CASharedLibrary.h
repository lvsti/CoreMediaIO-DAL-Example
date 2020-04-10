/*
     File: CASharedLibrary.h
 Abstract: Part of CoreAudio Utility Classes
  Version: 1.1

*/
#if !defined(__CASharedLibrary_h__)
#define __CASharedLibrary_h__

//=============================================================================
//	CASharedLibrary
//=============================================================================

class CASharedLibrary
{

//	Symbol Operations
public:
	static void*	LoadLibraryAndGetRoutineAddress(const char* inRoutineName, const char* inLibraryName, const char* inLibraryPath);
	static void*	GetRoutineAddressIfLibraryLoaded(const char* inRoutineName, const char* inLibraryName, const char* inLibraryPath);

};

#endif
