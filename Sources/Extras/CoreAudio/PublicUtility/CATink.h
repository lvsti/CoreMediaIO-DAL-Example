/*
     File: CATink.h
 Abstract: Part of CoreAudio Utility Classes
  Version: 1.1

*/
#if !defined(__CATink_h__)
#define __CATink_h__

//=============================================================================
//	Includes
//=============================================================================

//	System Includes
#if !defined(__COREAUDIO_USE_FLAT_INCLUDES__)
	#include <CoreServices/CoreServices.h>
#else
	#include <CoreServices.h>
#endif


//=============================================================================
//	CATink
//
//	A Tink is a small jump island that can make one function appear as if it
//	has many addresses. Note that Tinks are not supported for 64 bit environments
//	as the APIs that made tinks useful have been replaced by APIs that do away
//	with all the reasons for using a tink.
//=============================================================================

template <class F>
class CATink
{

public:
	CATink(F proc) { Set(proc); }
	
	~CATink() { Set((F)0xDEADDEAD); }		// jump to an obviously bad (odd) address if accessed after destruction

#if TARGET_CPU_PPC
	void Set(F proc)
	{
		/*
			lis		r11,0x1234
		00000000: 3D601234  lis        r11,4660
			ori		r11,r11,0x5678
		00000004: 616B5678  ori        r11,r11,$5678
			mtctr	r11
		00000008: 7D6903A6  mtctr      r11
			bctr
		0000000C: 4E800420  bctr
		*/
		UInt32 p = UInt32(proc);
		mCode[0] = 0x3D600000 | (p >> 16);
		mCode[1] = 0x616B0000 | (p & 0xFFFF);
		mCode[2] = 0x7D6903A6;
		mCode[3] = 0x4E800420;
		MakeDataExecutable(mCode, sizeof(mCode));
	}
	
	operator F () { return F(mCode); }

private:
	UInt32		mCode[4];
#elif TARGET_CPU_X86
	void Set(F proc)
	{
		/*
			<tink>:   push   $0x12345678
			<tink+5>: ret    
			<tink>:   0x34567868
			<tink+4>: 0x00e8c312
		*/
		UInt32 p = UInt32(proc);
		mCode[0] = ((p & 0xFFFFFF) << 8) | 0x00000068;
		mCode[1] = 0xCCCCC300 | (p >> 24);
		MakeDataExecutable(mCode, sizeof(mCode));
	}

	operator F () { return F(&mCode[0]); }

private:
	UInt32		mCode[2];
#else
	#warning: CPU not directly supported by CATink
	
	// For other CPU's, just wrap the function pointer for now.
	// this bypasses what we're trying to accomplish with the Tink
	// (multiple unique "instances" of a function), but it will at
	// least compile and run.

	void Set(F proc) { mProcPtr = proc; }
	
	operator F () { return mProcPtr; }
private:
	F			mProcPtr;
#endif

//	Tinks cannot be allocated in STL Containers, so we make a few key methods private
private:
	CATink(){}
	CATink(const CATink&){}
	CATink& operator=(const CATink&){ return *this;}
};

#endif
