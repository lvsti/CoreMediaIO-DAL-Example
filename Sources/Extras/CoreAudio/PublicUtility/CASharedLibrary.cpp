/*
     File: CASharedLibrary.cpp
 Abstract: CASharedLibrary.h
  Version: 1.1

*/
//=============================================================================
//	Includes
//=============================================================================

#include "CASharedLibrary.h"

//#define	CASharedLibrary_Use_DYLD	1
#if	CASharedLibrary_Use_DYLD
	#include <mach-o/dyld.h>
#else
	#include <dlfcn.h>
	#include <stddef.h>
#endif

//=============================================================================
//	CASharedLibrary
//=============================================================================

void*	CASharedLibrary::LoadLibraryAndGetRoutineAddress(const char* inRoutineName, const char* /*inLibraryName*/, const char* inLibraryPath)
{
	void* theRoutine = 0;

#if	CASharedLibrary_Use_DYLD
	const struct mach_header* theImage = NSAddImage(inLibraryPath, NSADDIMAGE_OPTION_RETURN_ON_ERROR);
	if(theImage != 0)
	{
		NSSymbol theSymbol = NSLookupSymbolInImage(theImage, inRoutineName, NSLOOKUPSYMBOLINIMAGE_OPTION_BIND);
		if(theSymbol != 0)
		{
			theRoutine = NSAddressOfSymbol(theSymbol);
		}
	}
#else
	void* theImage = dlopen(inLibraryPath, RTLD_LAZY);
	if(theImage != NULL)
	{
		//	we assume that all routine names passed here have a leading underscore which gets shaved
		//	off when passed to dlsym
		theRoutine = dlsym(theImage, &(inRoutineName[1]));
	}
	
#endif

	return theRoutine;
}

void*	CASharedLibrary::GetRoutineAddressIfLibraryLoaded(const char* inRoutineName, const char* /*inLibraryName*/, const char* inLibraryPath)
{
	void* theRoutine = 0;
		
#if	CASharedLibrary_Use_DYLD
	const struct mach_header* theImage = NSAddImage(inLibraryPath, NSADDIMAGE_OPTION_RETURN_ONLY_IF_LOADED);
	if(theImage != 0)
	{
		NSSymbol theSymbol = NSLookupSymbolInImage(theImage, inRoutineName, NSLOOKUPSYMBOLINIMAGE_OPTION_BIND);
		if(theSymbol != 0)
		{
			theRoutine = NSAddressOfSymbol(theSymbol);
		}
	}
#else
	void* theImage = dlopen(inLibraryPath, RTLD_LAZY | RTLD_NOLOAD);
	if(theImage != NULL)
	{
		//	we assume that all routine names passed here have a leading underscore which gets shaved
		//	off when passed to dlsym
		theRoutine = dlsym(theImage, &(inRoutineName[1]));
	}
#endif

	return theRoutine;
}
