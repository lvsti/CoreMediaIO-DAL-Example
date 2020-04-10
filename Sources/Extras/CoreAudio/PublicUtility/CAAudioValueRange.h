/*
     File: CAAudioValueRange.h
 Abstract: Part of CoreAudio Utility Classes
  Version: 1.1
*/
#if !defined(__CAAudioValueRange_h__)
#define __CAAudioValueRange_h__

//=============================================================================
//	Includes
//=============================================================================

//	System Includes
#if !defined(__COREAUDIO_USE_FLAT_INCLUDES__)
	#include <CoreAudio/CoreAudioTypes.h>
#else
	#include <CoreAudioTypes.h>
#endif
//	Standard Library Includes
#include <functional>
#include <vector>

//=============================================================================
//	CAAudioValueRange
//=============================================================================

struct CAAudioValueRange
:
	public AudioValueRange
{

//	Construction/Destruction
public:
						CAAudioValueRange()																{ mMinimum = 0.0; mMaximum = 0.0; }
						CAAudioValueRange(const AudioValueRange& v)										{ mMinimum = v.mMinimum; mMaximum = v.mMaximum; }
						CAAudioValueRange(Float64 inMinimum, Float64 inMaximum)							{ mMinimum = inMinimum; mMaximum = inMaximum; }

//	Assignment
public:
	CAAudioValueRange&	operator=(const AudioValueRange& v)												{ mMinimum = v.mMinimum; mMaximum = v.mMaximum; return *this; }

//  Operations
public:
	static bool			ContainsValue(const AudioValueRange& inRange, Float64 inValue)					{ return (inValue >= inRange.mMinimum) && (inValue <= inRange.mMaximum); }
	static Float64		BoundValue(const AudioValueRange& inRange, Float64 inValue);
	static Float64		PickCommonSampleRate(const AudioValueRange& inRange);
	static bool			IsStrictlyLessThan(const AudioValueRange& x, const AudioValueRange& y)			{ return x.mMaximum < y.mMinimum; }
	static bool			IsStrictlyGreaterThan(const AudioValueRange& x, const AudioValueRange& y)		{ return x.mMinimum > y.mMaximum; }
	static bool			IsStrictlyContainedBy(const AudioValueRange& x, const AudioValueRange& y)		{ return (x.mMinimum >= y.mMinimum) && (x.mMaximum <= y.mMaximum); }
	static bool			OverlapsLow(const AudioValueRange& x, const AudioValueRange& y)					{ return (x.mMinimum < y.mMinimum) && (x.mMaximum >= y.mMinimum) && (x.mMaximum <= y.mMaximum); }
	static bool			OverlapsHigh(const AudioValueRange& x, const AudioValueRange& y)				{ return (x.mMinimum >= y.mMinimum) && (x.mMinimum <= y.mMaximum) && (x.mMaximum > y.mMaximum); }
	static bool			Intersection(const AudioValueRange& x, const AudioValueRange& y, AudioValueRange& outRange);
	static bool			Union(const AudioValueRange& x, const AudioValueRange& y, AudioValueRange& outRange);

//	STL Helpers
public:
	struct	LessThan
	:
		public std::binary_function<AudioValueRange, AudioValueRange, bool>
	{
		bool	operator()(const AudioValueRange& x, const AudioValueRange& y) const
		{
			return x.mMinimum < y.mMinimum;
		}
	};
	
};

inline bool	operator<(const AudioValueRange& x, const AudioValueRange& y)								{ return x.mMinimum < y.mMinimum; }
inline bool	operator==(const AudioValueRange& x, const AudioValueRange& y)								{ return (x.mMinimum == y.mMinimum) && (x.mMaximum == y.mMaximum); }
inline bool	operator!=(const AudioValueRange& x, const AudioValueRange& y)								{ return !(x == y); }
inline bool	operator<=(const AudioValueRange& x, const AudioValueRange& y)								{ return (x < y) || (x == y); }
inline bool	operator>=(const AudioValueRange& x, const AudioValueRange& y)								{ return !(x < y); }
inline bool	operator>(const AudioValueRange& x, const AudioValueRange& y)								{ return !((x < y) || (x == y)); }

typedef	std::vector<CAAudioValueRange>	CAAudioValueRangeList;
void	CAAudioValueRange_ComputeUnion(const AudioValueRange& inRange, const CAAudioValueRangeList& inRangeList, CAAudioValueRangeList& outUnion);
void	CAAudioValueRange_ComputeIntersection(UInt32 inNumberRangeList1Items, AudioValueRange inRangeList1[], const CAAudioValueRangeList& inRangeList2, CAAudioValueRangeList& outIntersections);

#endif
