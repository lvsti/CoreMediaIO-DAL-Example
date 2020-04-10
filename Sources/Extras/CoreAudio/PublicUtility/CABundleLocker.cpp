/*
     File: CABundleLocker.cpp
 Abstract: CABundleLocker.h
  Version: 1.1

*/
#include "CABundleLocker.h"
#include <pthread.h>

/*
some bundle operations are not thread safe, notably CFCopyLocalizedStringFromTableInBundle
*/

static pthread_mutex_t sCABundleLocker = PTHREAD_MUTEX_INITIALIZER;

#define RECURSIVE_LOCK 0

#if RECURSIVE_LOCK
static pthread_once_t sOnce = PTHREAD_ONCE_INIT;

static void InitCABundleLocker()
{
	// have to do this because OS X lacks PTHREAD_MUTEX_RECURSIVE_INITIALIZER_NP
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&sCABundleLocker, &attr);
	pthread_mutexattr_destroy(&attr);
}
#endif

CABundleLocker::CABundleLocker()
{
#if RECURSIVE_LOCK
	pthread_once(&sOnce, InitCABundleLocker);
#endif
	pthread_mutex_lock(&sCABundleLocker);
}

CABundleLocker::~CABundleLocker()
{
	pthread_mutex_unlock(&sCABundleLocker);
}

