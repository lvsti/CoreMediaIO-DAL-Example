/*
	    File: CMIO_KEXT_Sample_ControlIDs.h
	Abstract: Control ID constants used by the Sample kext and its Assistant
	 Version: 1.2
*/

#if !defined(__CMIO_KEXT_Sample_ControlIDs_h__)
#define __CMIO_KEXT_Sample_ControlIDs_h__

namespace CMIO { namespace KEXT { namespace Sample
{
	enum
	{
		kDirectionControlID				= 2,
		kInputSourceSelectorControlID	= 3,
		kInputChannelSelectorControlID	= 4
	};

	enum
	{
		kSampleSourceTuner				= 0,
		kSampleSourceComposite			= 1,
		kSampleSourceSVideo				= 2,
	};
}}}

#endif

