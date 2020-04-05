/*
     File: CAReferenceCounted.h
 Abstract: Part of CoreAudio Utility Classes
  Version: 1.1

*/
#ifndef __CAReferenceCounted_h__
#define __CAReferenceCounted_h__

#include "CAAtomic.h"

// base class for reference-counted objects
class CAReferenceCounted {
public:
	CAReferenceCounted() : mRefCount(1) {}
	
	void	retain() { CAAtomicIncrement32(&mRefCount); }
	
	void	release() 
			{ 
				SInt32 rc = CAAtomicDecrement32(&mRefCount);
				if (rc == 0) {
					releaseObject();
				}
			}


	class Retainer {
	public:
		Retainer(CAReferenceCounted *obj) : mObject(obj) { mObject->retain(); }
		~Retainer() { mObject->release(); }
	
	private:
		CAReferenceCounted *	mObject;
	};

protected:
    virtual	~CAReferenceCounted() { }
	
	virtual void releaseObject () 
			{ 
				delete this; 
			}

#if DEBUG
public:
#endif
	SInt32	GetReferenceCount() const { return mRefCount; }
private:
	SInt32		mRefCount;

	CAReferenceCounted(const CAReferenceCounted &a);
	CAReferenceCounted &operator=(const CAReferenceCounted &a);
};


#endif // __CAReferenceCounted_h__
