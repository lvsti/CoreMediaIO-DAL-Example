/*
	    File: CMIO_IOKA_Object.h
	Abstract: The IOKit Assisistant (IOKA) consists of various objects to facilitate using the "Acquisition is Initialization" design pattern.
				NOTE: the IOKA is sparsely implemented, meaning that it does not attempt to provide wrappers/access to all IOKit features, but just those needed in the CMIO namespace.
	
	 Version: 1.2

*/

#if !defined(__CMIO_IOKA_Object_h__)
#define __CMIO_IOKA_Object_h__

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
	// Object
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class Object
	{
	// Construction/Destruction
	public:
						Object(const Object& object) : mReference(IO_OBJECT_NULL) { Reset(object); }
		explicit		Object(io_object_t reference = IO_OBJECT_NULL) : mReference(IO_OBJECT_NULL) { Reset(reference); }
						~Object() { if (IO_OBJECT_NULL != mReference) (void) IOObjectRelease(mReference); }
		Object&			operator=(const Object& that) { if (this != &that) this->Reset(that); return *this; }
		  
		void			Reset(io_object_t reference = IO_OBJECT_NULL) { if (IO_OBJECT_NULL != mReference) (void) IOObjectRelease(mReference); mReference = reference; }
		void			Reset(const Object& object)
						{
							if (IO_OBJECT_NULL != mReference)
							{
								(void) IOObjectRelease(mReference);
								mReference = IO_OBJECT_NULL;
							}
							
							if (IO_OBJECT_NULL != object.Get())
							{
								IOReturn ioReturn = IOObjectRetain(object.Get());
								ThrowIfError(ioReturn, CAException(ioReturn), "CMIO::IOKA::Object::Reset: IOObjectRetain() failed");
								mReference = object.Get();
							}
						}

	private:
		io_object_t		mReference;

	// Attributes
	public:
		bool			IsValid() const { return IO_OBJECT_NULL !=  mReference; }
		bool			ConformsTo(const io_name_t className) { return IOObjectConformsTo(mReference, className); }

	// Value Access
	public:
		operator		io_object_t() const { return mReference; }
		io_object_t		Get() const { return mReference; }													
		io_object_t*	GetAddress() { return &mReference; }													
	};

}}
#endif

