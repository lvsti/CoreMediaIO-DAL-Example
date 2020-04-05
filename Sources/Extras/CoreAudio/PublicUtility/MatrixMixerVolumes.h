/*
     File: MatrixMixerVolumes.h
 Abstract: Part of CoreAudio Utility Classes
  Version: 1.1

*/
#ifndef __MatrixMixerVolumes_h__
#define __MatrixMixerVolumes_h__

#if !defined(__COREAUDIO_USE_FLAT_INCLUDES__)
	#include <AudioUnit/AudioUnit.h>
#else
	#include <AudioUnit.h>
#endif

#if defined(__cplusplus)
extern "C"
{
#endif

// prints the matrix mixer volumes of a specific audio unit to the given file
void	 PrintMatrixMixerVolumes (FILE* file, AudioUnit au);				

// prints the mixer volumes for the specific scope of the audio unit
// results will be printed to the speficied file "file" with identifiying string tag "str"		
OSStatus PrintBuses (FILE* file, char* str, AudioUnit au, AudioUnitScope inScope);
#if defined(__cplusplus)
}
#endif

#endif
