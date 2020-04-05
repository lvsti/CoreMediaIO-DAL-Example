/*
     File: CAVolumeCurve.h
 Abstract: Part of CoreAudio Utility Classes
  Version: 1.1

*/
#if !defined(__CAVolumeCurve_h__)
#define __CAVolumeCurve_h__

//=============================================================================
//	Includes
//=============================================================================

#if !defined(__COREAUDIO_USE_FLAT_INCLUDES__)
	#include <CoreAudio/CoreAudioTypes.h>
#else
	#include <CoreAudioTypes.h>
#endif
#include <map>

//=============================================================================
//	Types
//=============================================================================

struct CARawPoint
{
	SInt32	mMinimum;
	SInt32	mMaximum;
	
	CARawPoint() : mMinimum(0), mMaximum(0) {}
	CARawPoint(const CARawPoint& inPoint) : mMinimum(inPoint.mMinimum), mMaximum(inPoint.mMaximum) {}
	CARawPoint(SInt32 inMinimum, SInt32 inMaximum) : mMinimum(inMinimum), mMaximum(inMaximum) {}
	CARawPoint&	operator=(const CARawPoint& inPoint) { mMinimum = inPoint.mMinimum; mMaximum = inPoint.mMaximum; return *this; }
	
	static bool	Overlap(const CARawPoint& x, const CARawPoint& y) { return (x.mMinimum < y.mMaximum) && (x.mMaximum > y.mMinimum); }
};

inline bool	operator<(const CARawPoint& x, const CARawPoint& y) { return x.mMinimum < y.mMinimum; }
inline bool	operator==(const CARawPoint& x, const CARawPoint& y) { return (x.mMinimum == y.mMinimum) && (x.mMaximum == y.mMaximum); }
inline bool	operator!=(const CARawPoint& x, const CARawPoint& y) { return !(x == y); }
inline bool	operator<=(const CARawPoint& x, const CARawPoint& y) { return (x < y) || (x == y); }
inline bool	operator>=(const CARawPoint& x, const CARawPoint& y) { return !(x < y); }
inline bool	operator>(const CARawPoint& x, const CARawPoint& y) { return !((x < y) || (x == y)); }

struct CADBPoint
{
	Float32	mMinimum;
	Float32	mMaximum;
	
	CADBPoint() : mMinimum(0), mMaximum(0) {}
	CADBPoint(const CADBPoint& inPoint) : mMinimum(inPoint.mMinimum), mMaximum(inPoint.mMaximum) {}
	CADBPoint(Float32 inMinimum, Float32 inMaximum) : mMinimum(inMinimum), mMaximum(inMaximum) {}
	CADBPoint&	operator=(const CADBPoint& inPoint) { mMinimum = inPoint.mMinimum; mMaximum = inPoint.mMaximum; return *this; }
	
	static bool	Overlap(const CADBPoint& x, const CADBPoint& y) { return (x.mMinimum < y.mMaximum) && (x.mMaximum >= y.mMinimum); }
};

inline bool	operator<(const CADBPoint& x, const CADBPoint& y) { return x.mMinimum < y.mMinimum; }
inline bool	operator==(const CADBPoint& x, const CADBPoint& y) { return (x.mMinimum == y.mMinimum) && (x.mMaximum == y.mMaximum); }
inline bool	operator!=(const CADBPoint& x, const CADBPoint& y) { return !(x == y); }
inline bool	operator<=(const CADBPoint& x, const CADBPoint& y) { return (x < y) || (x == y); }
inline bool	operator>=(const CADBPoint& x, const CADBPoint& y) { return !(x < y); }
inline bool	operator>(const CADBPoint& x, const CADBPoint& y) { return !((x < y) || (x == y)); }

//=============================================================================
//	CAVolumeCurve
//=============================================================================

class CAVolumeCurve
{

//	Constants
public:
	enum
	{
					kLinearCurve		= 0,
					kPow1Over3Curve		= 1,
					kPow1Over2Curve		= 2,
					kPow3Over4Curve		= 3,
					kPow3Over2Curve		= 4,
					kPow2Over1Curve		= 5,
					kPow3Over1Curve		= 6,
					kPow4Over1Curve		= 7,
					kPow5Over1Curve		= 8,
					kPow6Over1Curve		= 9,
					kPow7Over1Curve		= 10,
					kPow8Over1Curve		= 11,
					kPow9Over1Curve		= 12,
					kPow10Over1Curve	= 13,
					kPow11Over1Curve	= 14,
					kPow12Over1Curve	= 15
	};

//	Construction/Destruction
public:
					CAVolumeCurve();
	virtual			~CAVolumeCurve();

//	Attributes
public:
	UInt32			GetTag() const			{ return mTag; }
	void			SetTag(UInt32 inTag)	{ mTag = inTag; }
	SInt32			GetMinimumRaw() const;
	SInt32			GetMaximumRaw() const;
	Float32			GetMinimumDB() const;
	Float32			GetMaximumDB() const;
	
	void			SetIsApplyingTransferFunction(bool inIsApplyingTransferFunction)  { mIsApplyingTransferFunction = inIsApplyingTransferFunction; }
	UInt32			GetTransferFunction() const { return mTransferFunction; }
	void			SetTransferFunction(UInt32 inTransferFunction);

//	Operations
public:
	void			AddRange(SInt32 mMinRaw, SInt32 mMaxRaw, Float32 inMinDB, Float32 inMaxDB);
	void			ResetRange();
	bool			CheckForContinuity() const;
	
	SInt32			ConvertDBToRaw(Float32 inDB) const;
	Float32			ConvertRawToDB(SInt32 inRaw) const;
	Float32			ConvertRawToScalar(SInt32 inRaw) const;
	Float32			ConvertDBToScalar(Float32 inDB) const;
	SInt32			ConvertScalarToRaw(Float32 inScalar) const;
	Float32			ConvertScalarToDB(Float32 inScalar) const;

//	Implementation
private:
	typedef	std::map<CARawPoint, CADBPoint>	CurveMap;
	
	UInt32			mTag;
	CurveMap		mCurveMap;
	bool			mIsApplyingTransferFunction;
	UInt32			mTransferFunction;
	Float32			mRawToScalarExponentNumerator;
	Float32			mRawToScalarExponentDenominator;

};

#endif
