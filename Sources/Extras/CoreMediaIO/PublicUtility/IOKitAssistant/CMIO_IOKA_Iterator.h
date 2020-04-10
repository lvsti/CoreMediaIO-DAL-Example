/*
	    File: CMIO_IOKA_Iterator.h
	Abstract: The IOKit Assisistant (IOKA) consists of various objects to facilitate using the "Acquisition is Initialization" design pattern.
				NOTE: the IOKA is sparsely implemented, meaning that it does not attempt to provide wrappers/access to all IOKit features, but just those needed in the CMIO namespace.
	
	 Version: 1.2

*/

#if !defined(__CMIO_IOKA_Iterator_h__)
#define __CMIO_IOKA_Iterator_h__

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Public Utility Includes
#include "CMIODebugMacros.h"

// CA Public Utility Includes
#include "CAException.h"

// System Includes
#include <IOKit/IOKitLib.h>

namespace CMIO { namespace IOKA
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Iterator
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class Iterator
	{
	//	Construction/Destruction
	public:
						Iterator() : mIOIterator(IO_OBJECT_NULL), mWillRelease(true), mLastKernelError(0) {}
						Iterator(io_iterator_t inIOIterator, bool willRelease = true) : mIOIterator(inIOIterator), mWillRelease(willRelease), mLastKernelError(0) {}
						Iterator(io_object_t parent, const io_name_t plane) : mIOIterator(IO_OBJECT_NULL), mWillRelease(true), mLastKernelError(0) { mLastKernelError = IORegistryEntryGetChildIterator(parent, plane, &mIOIterator); }
						Iterator(io_object_t child, const io_name_t plane, bool /*getParent*/) : mIOIterator(IO_OBJECT_NULL), mWillRelease(true), mLastKernelError(0) { mLastKernelError = IORegistryEntryGetParentIterator(child, plane, &mIOIterator); }
						~Iterator() { Release(); }
						Iterator(const Iterator& iterator) : mIOIterator(iterator.mIOIterator), mWillRelease(iterator.mWillRelease), mLastKernelError(0) { Retain(); }
		Iterator&		operator=(const Iterator& iterator) { Release(); mIOIterator = iterator.mIOIterator; mWillRelease = iterator.mWillRelease; Retain(); mLastKernelError = 0; return *this; }
		Iterator&		operator=(io_iterator_t inIOIterator) { Release(); mIOIterator = inIOIterator; mWillRelease = true; return *this; }

	private:
		void			Retain() { if (mWillRelease && (mIOIterator != IO_OBJECT_NULL)) { IOObjectRetain(mIOIterator); } }
		void			Release() { if (mWillRelease && (mIOIterator != IO_OBJECT_NULL)) { IOObjectRelease(mIOIterator); mIOIterator = IO_OBJECT_NULL; } }
		
		io_iterator_t   mIOIterator;
		bool			mWillRelease;
		kern_return_t   mLastKernelError;

	//	Operations
	public:
		void			AllowRelease() { mWillRelease = true; }
		void			DontAllowRelease() { mWillRelease = false; }
		bool			IsValid() const { return mIOIterator != IO_OBJECT_NULL; }
		bool			IsEqual(io_iterator_t inIOIterator) { return IOObjectIsEqualTo(inIOIterator, mIOIterator); }
		io_object_t		Next() { return IOIteratorNext(mIOIterator); }

	//	Value Access
	public:
		io_iterator_t   GetIOIterator() const { return mIOIterator; }
		io_iterator_t   CopyIOIterator() const { if (mIOIterator != IO_OBJECT_NULL) { IOObjectRetain(mIOIterator); } return mIOIterator; }
		kern_return_t   GetLastKernelError() const { return mLastKernelError; }

	};
}}
#endif

