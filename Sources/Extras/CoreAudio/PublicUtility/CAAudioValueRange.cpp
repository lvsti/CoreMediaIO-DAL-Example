/*
     File: CAAudioValueRange.cpp
 Abstract: CAAudioValueRange.h
  Version: 1.1
*/
//==================================================================================================
//	Includes
//==================================================================================================

//	Self Include
#include "CAAudioValueRange.h"

//	Standard Library
#include <algorithm>

//==================================================================================================
//	CAAudioValueRange
//==================================================================================================

Float64	CAAudioValueRange::BoundValue(const AudioValueRange& inRange, Float64 inValue)
{
	if (inValue <= inRange.mMinimum)
	{
		return inRange.mMinimum;
	}
	else if (inValue >= inRange.mMaximum)
	{
		return inRange.mMaximum;
	}
	else
	{
		return inValue;
	}
}

Float64 CAAudioValueRange::PickCommonSampleRate(const AudioValueRange& inRange)
{
	//  This routine will pick a "common" sample rate from the give range of rates or the maximum
	//  if no common rates can be found. It assumes that inRange contains a continuous range of
	//  sample rates.
	Float64 theAnswer = inRange.mMaximum;
	
	if(ContainsValue(inRange, 44100.0))
	{
		theAnswer = 44100.0;
	}
	else if(ContainsValue(inRange, 48000.0))
	{
		theAnswer = 48000.0;
	}
	else if(ContainsValue(inRange, 96000.0))
	{
		theAnswer = 96000.0;
	}
	else if(ContainsValue(inRange, 88200.0))
	{
		theAnswer = 88200.0;
	}
	else if(ContainsValue(inRange, 64000.0))
	{
		theAnswer = 64000.0;
	}
	else if(ContainsValue(inRange, 32000.0))
	{
		theAnswer = 32000.0;
	}
	else if(ContainsValue(inRange, 24000.0))
	{
		theAnswer = 24000.0;
	}
	else if(ContainsValue(inRange, 22050.0))
	{
		theAnswer = 22050.0;
	}
	else if(ContainsValue(inRange, 16000.0))
	{
		theAnswer = 16000.0;
	}
	else if(ContainsValue(inRange, 12000.0))
	{
		theAnswer = 12000.0;
	}
	else if(ContainsValue(inRange, 11025.0))
	{
		theAnswer = 11025.0;
	}
	else if(ContainsValue(inRange, 8000.0))
	{
		theAnswer = 8000.0;
	}
	
	return theAnswer;
}

bool	CAAudioValueRange::Intersection(const AudioValueRange& x, const AudioValueRange& y, AudioValueRange& outRange)
{
	bool isNonEmpty;
	if(!IsStrictlyLessThan(x, y) && !IsStrictlyGreaterThan(x, y))
	{
		outRange.mMinimum = std::max(x.mMinimum, y.mMinimum);
		outRange.mMaximum = std::min(x.mMaximum, y.mMaximum);
		isNonEmpty = true;
	}
	else
	{
		outRange.mMinimum = 0;
		outRange.mMaximum = 0;
		isNonEmpty = false;
	}
	return isNonEmpty;
}

bool	CAAudioValueRange::Union(const AudioValueRange& x, const AudioValueRange& y, AudioValueRange& outRange)
{
	bool isDisjoint;
	if(!IsStrictlyLessThan(x, y) && !IsStrictlyGreaterThan(x, y))
	{
		outRange.mMinimum = std::min(x.mMinimum, y.mMinimum);
		outRange.mMaximum = std::max(x.mMaximum, y.mMaximum);
		isDisjoint = false;
	}
	else
	{
		outRange.mMinimum = 0;
		outRange.mMaximum = 0;
		isDisjoint = true;
	}
	return isDisjoint;
}

void	CAAudioValueRange_ComputeUnion(const AudioValueRange& inRange, const CAAudioValueRangeList& inRangeList, CAAudioValueRangeList& outUnion)
{
	//	this method assumes that the ranges in inRangeList are disjoint and that they are sorted from low to high and
	outUnion.clear();
	
	//	start at the beginning of inRangeList
	CAAudioValueRangeList::const_iterator theIterator = inRangeList.begin();
	
	//	iterate through inRangeList and add all the ranges that are strictly less than inRange
	while((theIterator != inRangeList.end()) && CAAudioValueRange::IsStrictlyLessThan(*theIterator, inRange))
	{
		//	put this range in the union
		outUnion.push_back(*theIterator);
		
		//	go to the next one
		std::advance(theIterator, 1);
	}
	
	if(theIterator != inRangeList.end())
	{
		if(!CAAudioValueRange::IsStrictlyGreaterThan(*theIterator, inRange))
		{
			//	inRange intersects the range that theIterator points at, but might actually intersect several contiguous ranges
			
			//	initialize the starting point, noting that we can skip the current one since we already know it's in the intersection
			CAAudioValueRangeList::const_iterator theGreaterIterator = theIterator;
			std::advance(theGreaterIterator, 1);
			
			//	iterate until we find a range that is strictly greater than inRange
			while((theGreaterIterator != inRangeList.end()) && !CAAudioValueRange::IsStrictlyGreaterThan(*theGreaterIterator, inRange))
			{
				//	go to the next one
				std::advance(theGreaterIterator, 1);
			}
			
			//	theGreaterIterator now points at either one past the highest range in the intersection or the end of the vector
			//	Either way, we have to adjust it to point at the true highest range in the intersection
			std::advance(theGreaterIterator, -1);
			
			//	now theIterator points at the lowest range in the intersection and theGreaterIterator points at the highest
			//	so we can compute the coagulated range
			AudioValueRange theCoagulation;
			theCoagulation.mMinimum = std::min(theIterator->mMinimum, inRange.mMinimum);
			theCoagulation.mMaximum = std::max(theGreaterIterator->mMaximum, inRange.mMaximum);
			
			//	add the coagulation to the union
			outUnion.push_back(theCoagulation);
			
			//	adjust theIterator to point at the next range for processing
			theIterator = theGreaterIterator;
			std::advance(theIterator, 1);
		}
		else
		{
			//	the range theIterator points at is strictly greater than inRange, so insert inRange in front of it and we're done
			outUnion.push_back(inRange);
		}
			
		//	we need to now copy the remaining higher ranges in inRangeList into the union
		while(theIterator != inRangeList.end())
		{
			//	put this range in the union
			outUnion.push_back(*theIterator);
			
			//	go to the next one
			std::advance(theIterator, 1);
		}
	}
	else
	{
		//	inRange is larger than all of the ranges in inRangeList, so just add it onto the end of the union and we're done
		//	This is also the case if inRangeList is empty
		outUnion.push_back(inRange);
	}
}

void	CAAudioValueRange_ComputeIntersection(UInt32 inNumberRangeList1Items, AudioValueRange inRangeList1[], const CAAudioValueRangeList& inRangeList2, CAAudioValueRangeList& outIntersections)
{
	outIntersections.clear();
	for(UInt32 theRangeList1Index = 0; theRangeList1Index < inNumberRangeList1Items; ++theRangeList1Index)
	{
		for(CAAudioValueRangeList::const_iterator theRangeList2Iterator = inRangeList2.begin(); theRangeList2Iterator != inRangeList2.end(); std::advance(theRangeList2Iterator, 1))
		{
			AudioValueRange theIntersection;
			if(CAAudioValueRange::Intersection(inRangeList1[theRangeList1Index], *theRangeList2Iterator, theIntersection))
			{
				outIntersections.push_back(theIntersection);
			}
		}
	}
}
