/*
     File: CABundleLocker.h
 Abstract: Part of CoreAudio Utility Classes
  Version: 1.1

*/
#ifndef _CABundleLocker_h_
#define _CABundleLocker_h_

#include <TargetConditionals.h>
 
/*
some bundle operations are not thread safe, notably CFCopyLocalizedStringFromTableInBundle
*/

class CABundleLocker
{
public:

#if TARGET_OS_MAC
	CABundleLocker();
	~CABundleLocker();
#else
	CABundleLocker() {}
	~CABundleLocker() {}
#endif
};

#endif
