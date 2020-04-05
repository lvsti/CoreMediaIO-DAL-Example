/*
     File: CAVectorUnitTypes.h
 Abstract: Part of CoreAudio Utility Classes
  Version: 1.1

*/
#ifndef __CAVectorUnitTypes_h__
#define __CAVectorUnitTypes_h__

enum {
	kVecUninitialized = -1,
	kVecNone = 0,
	kVecAltivec = 1,
	kVecSSE2 = 100,
	kVecSSE3 = 101,
	kVecAVX1 = 110,
	kVecNeon = 200
};

#endif
