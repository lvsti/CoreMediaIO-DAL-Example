/*
     File: MatrixMixerVolumes.cpp
 Abstract: MatrixMixerVolumes.h
  Version: 1.1

*/
#include "MatrixMixerVolumes.h"
#include "CAXException.h"

OSStatus	NumberChannels (AudioUnit 		 	au,
							AudioUnitScope		inScope,
							AudioUnitElement	inEl,
							UInt32				&outChans);


OSStatus PrintBuses (FILE* file, const char* str, AudioUnit au, AudioUnitScope inScope)
{
	OSStatus result;
	UInt32 busCount;
	UInt32 theSize = sizeof(busCount);

	ca_require_noerr (result = AudioUnitGetProperty (au, kAudioUnitProperty_ElementCount,	
							inScope, 0, &busCount, &theSize), home);
		
	fprintf (file, "\t%s Elements:\n\t\t", str);
	for (UInt32 i = 0; i < busCount; ++i) {
		Float32 val;
		ca_require_noerr (result = AudioUnitGetParameter (au, kMatrixMixerParam_Enable, inScope, i, &val), home);
		UInt32 numChans;
		ca_require_noerr (result = NumberChannels (au, inScope, i, numChans), home);
		char frameCharStart = (val != 0 ? '[' : '{');
		char frameCharEnd = (val != 0 ? ']' : '}');
		fprintf (file, "%d:%c%d, %c%c  ", (int)i, frameCharStart, (int)numChans, (val != 0 ? 'T' : 'F'), frameCharEnd);
	}
	fprintf (file, "\n");
home:
	return result;
}

void	PrintMatrixMixerVolumes (FILE* file, AudioUnit au)
{
	UInt32 dims[2];
	UInt32 theSize =  sizeof(UInt32) * 2;
	Float32 *theVols = NULL;
	OSStatus result;
	
// this call will fail if the unit is NOT initialized as it would present an incomplete state	
	ca_require_noerr (result = AudioUnitGetProperty (au, kAudioUnitProperty_MatrixDimensions,	
							kAudioUnitScope_Global, 0, dims, &theSize), home);

	theSize = ((dims[0] + 1) * (dims[1] + 1)) * sizeof(Float32);
	
	theVols	= static_cast<Float32*> (malloc (theSize));
	
	ca_require_noerr (result = AudioUnitGetProperty (au, kAudioUnitProperty_MatrixLevels,	
							kAudioUnitScope_Global, 0, theVols, &theSize), home);

home:
	if (result) {
		if (theVols)
			free(theVols);
		return;
	}
	
	theSize /= sizeof(Float32);
	
	unsigned int inputs = dims[0];
	unsigned int outputs = dims[1];

	fprintf (file, "\tInput Channels = %d, Output Channels = %d\n", (int)dims[0], (int)dims[1]);
	PrintBuses (file, "Input", au, kAudioUnitScope_Input);
	PrintBuses (file, "Output", au, kAudioUnitScope_Output);
	fprintf (file, "\tGlobal Volume: %.3f\n", theVols [theSize - 1]);
	for (unsigned int i = 0; i < (inputs + 1); ++i) {
		if (i < inputs) {
			fprintf (file, "\t%.3f   ", theVols[(i + 1) * (outputs + 1) - 1]);
			
			for (unsigned int j = 0; j < outputs; ++j)
				fprintf (file, "(%.3f) ", theVols[(i * (outputs  + 1)) + j]);
		} else {
			fprintf (file, "\t        ");
			for (unsigned int j = 0; j < outputs; ++j)
				fprintf (file, " %.3f  ", theVols[(i * (outputs + 1)) + j]);
		}
		fprintf (file, "\n");
	}

#if 0
	for (unsigned int i = 0; i < theSize; ++i)
		printf ("%f, ", theVols[i]);
#endif
	free(theVols);
}

// Utility routine that gets the number of channels from an audio unit
OSStatus	NumberChannels (AudioUnit 		 	au,
							AudioUnitScope		inScope,
							AudioUnitElement	inEl,
							UInt32				&outChans)
{
	AudioStreamBasicDescription desc;
	UInt32 dataSize = sizeof (AudioStreamBasicDescription);
	OSStatus result = AudioUnitGetProperty (au, kAudioUnitProperty_StreamFormat,
								inScope, inEl, 
								&desc, &dataSize);
	if (!result)
		outChans = desc.mChannelsPerFrame;
	return result;
}
