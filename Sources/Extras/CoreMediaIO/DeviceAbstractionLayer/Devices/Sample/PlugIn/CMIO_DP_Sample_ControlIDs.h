/*
	    File: CMIO_DP_Sample_ControlIDs.h
	Abstract: Control ID constants used by the Sample Server and its Assistant.
	 Version: 1.2

*/

#if !defined(__CMIO_DP_Sample_ControlIDs_h__)
#define __CMIO_DP_Sample_ControlIDs_h__

namespace CMIO { namespace DP { namespace Sample
{
	enum
	{
		kProtocolSelectorControlID	= 5
	};
	
	enum
	{
		kProtocolSample				= 1,
		kProtocolSampleBasic		= 2,
		kProtocolSampleAdvanced		= 3
	};
	
}}}

#endif

