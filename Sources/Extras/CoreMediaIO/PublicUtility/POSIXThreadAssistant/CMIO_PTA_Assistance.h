/*
	    File: CMIO_PTA_Assistance.h
	Abstract: IMPORTANT IMPLEMENTATION NOTE:  THIS WILL HOPEFULLY BE REPLACED BY CAMutex.h AT SOME POINT IN THE FUTURE
	
				The POSIX Thread Assistant (PTA) consists of various objects to facilitate using the "Acquisition is Initialization" design pattern.
				NOTE: the PTA is sparsely implemented, meaning that it does not attempt to provide wrappers/access to all POSIX Thread features.
	
	 Version: 1.2

*/

#if !defined(__CMIO_PTA_Assistance_h__)
#define __CMIO_PTA_Assistance_h__

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Public Utility Includes
#include "CMIODebugMacros.h"

// CA Public Utility Includes
#include "CMIODebugMacros.h"
#include "CAException.h"

// System Includes
#include <pthread.h>

namespace CMIO { namespace PTA
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Mutex
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class Mutex
	{
	public:
		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// Mutex::Condition
		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		class Condition
		{
			pthread_cond_t mCondition;
			Condition& operator=(Condition& that);										// Don't allow copying
		
		public:
		
			// Creators
			Condition()
			{
				int err = pthread_cond_init(&mCondition, 0);
				ThrowIfError(err, CAException(err), "CMIO::PTA::Mutex::Condition: pthread_cond_init() failed");
			}
			
			~Condition()												{ (void) pthread_cond_destroy(&mCondition); }

			// Conversion operators
			operator pthread_cond_t*()									{ return &mCondition; }
	
			// Accessors
			pthread_cond_t*	Get()										{ return &mCondition; }													
		};
		
		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// Mutex::Attribute
		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		class Attribute
		{
			pthread_mutexattr_t mAttribute;
			Attribute& operator=(Attribute& that);										// Don't allow copying
		
		public:
		
			// Creators
			Attribute()
			{
				int err = pthread_mutexattr_init(&mAttribute);
				ThrowIfError(err, CAException(err), "CMIO::PTA::Mutex::Attribute: pthread_mutexattr_init() failed");
			}
			
			~Attribute()												{ (void) pthread_mutexattr_destroy(&mAttribute); }

			// Conversion operators
			operator pthread_mutexattr_t*()								{ return &mAttribute; }
	
			// Accessors
			pthread_mutexattr_t*	Get()								{ return &mAttribute; }													
		};
		
		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// Mutex::Locker
		//	Helper class to provide stack based locking / unlocking of the mutex
		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		class Locker
		{
			Mutex& mMutex;
			Locker& operator=(Locker& that);										// Don't allow copying
		
		public:
		
			// Creators
			Locker(Mutex& mutex) :
				mMutex(mutex)
			{
				int err = pthread_mutex_lock(mutex);
				ThrowIfError(err, CAException(err), "CMIO::PTA::Mutex::Locker: pthread_mutex_lock() failed");
			}
			
			~Locker()															{ (void) pthread_mutex_unlock(mMutex); }
		};
		
		
		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// Mutex
		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		Mutex(int type = PTHREAD_MUTEX_DEFAULT)
		{
			Attribute attribute;
			int err = pthread_mutexattr_settype(attribute, type);
			ThrowIfError(err, CAException(err), "CMIO::PTA::Mutex: pthread_mutexattr_settype() failed");
				
			err = pthread_mutex_init(&mMutex, attribute);
			ThrowIfError(err, CAException(err), "CMIO::PTA::Mutex: pthread_mutex_init() failed");
		}
		  
		~Mutex()																{ (void) pthread_mutex_destroy(&mMutex); }
			
		// Conversion operators
		operator pthread_mutex_t*()												{ return &mMutex; }

		// Accessors
		pthread_mutex_t*	Get()												{ return &mMutex; }													

	private:
		pthread_mutex_t	mMutex;
		Mutex& operator=(Mutex& that);											// Don't allow copying
	};
}}

#endif
