/*
     File: CACFMessagePort.cpp
 Abstract: CACFMessagePort.h
  Version: 1.1

*/
//=============================================================================
//	Includes
//=============================================================================

#include "CACFMessagePort.h"
#include "CADebugMacros.h"
#include "CAException.h"

//=============================================================================
//	CACFLocalMessagePort
//=============================================================================

CACFLocalMessagePort::CACFLocalMessagePort(CFStringRef inName, CFMessagePortCallBack inPortCallBack, CFMessagePortInvalidationCallBack inInvalidationCallBack, void* inUserData)
:
	mMessagePort(NULL),
	mRunLoopSource(NULL),
	mDispatchQueue(NULL)
{
	//	create the CFMessagePort
	CFMessagePortContext theContext = { 0, inUserData, NULL, NULL, NULL };
	mMessagePort = CFMessagePortCreateLocal(NULL, inName, inPortCallBack, &theContext, NULL);
	if(mMessagePort != NULL)
	{
		//	add the invalidation callback, if any
		if(inInvalidationCallBack != NULL)
		{
			CFMessagePortSetInvalidationCallBack(mMessagePort, inInvalidationCallBack);
		}
	}
}

CACFLocalMessagePort::~CACFLocalMessagePort()
{
	if(mRunLoopSource != NULL)
	{
		CFRelease(mRunLoopSource);
	}
	if(mMessagePort != NULL)
	{
		CFMessagePortInvalidate(mMessagePort);
		CFRelease(mMessagePort);
	}
}

CFRunLoopSourceRef	CACFLocalMessagePort::GetRunLoopSource() const
{
	Assert(mDispatchQueue == NULL, "CACFLocalMessagePort::SetDispatchQueue: should have both a run loop source and a dispatch queue");
	if(mRunLoopSource == NULL)
	{
		const_cast<CACFLocalMessagePort*>(this)->mRunLoopSource = CFMessagePortCreateRunLoopSource(NULL, mMessagePort, 0);
	}
	return mRunLoopSource;
}

void	CACFLocalMessagePort::SetDispatchQueue(dispatch_queue_t inDispatchQueue)
{
	Assert(mRunLoopSource == NULL, "CACFLocalMessagePort::SetDispatchQueue: should have both a run loop source and a dispatch queue");
	mDispatchQueue = inDispatchQueue;
	CFMessagePortSetDispatchQueue(mMessagePort, mDispatchQueue);
}

//=============================================================================
//	CACFRemoteMessagePort
//=============================================================================

CACFRemoteMessagePort::CACFRemoteMessagePort(CFStringRef inName, CFMessagePortInvalidationCallBack inInvalidationCallBack)
:
	mMessagePort(NULL),
	mRunLoopSource(NULL),
	mDispatchQueue(NULL)
{
	//	create the CFMessagePort
	mMessagePort = CFMessagePortCreateRemote(NULL, inName);
	if(mMessagePort != NULL)
	{
		//	failure to create a remote port does not need to throw an exception
		//	because it isn't really an error since the port in question may not
		//	exist and this fact requires a more complex response than an excpeption
		//	provides for.
		
		//	add the invalidation callback, if any
		if(inInvalidationCallBack != NULL)
		{
			CFMessagePortSetInvalidationCallBack(mMessagePort, inInvalidationCallBack);
		}
	}
}

CACFRemoteMessagePort::~CACFRemoteMessagePort()
{
	if(mRunLoopSource != NULL)
	{
		CFRelease(mRunLoopSource);
	}
	if(mMessagePort != NULL)
	{
		//CFMessagePortInvalidate(mMessagePort);
		CFRelease(mMessagePort);
	}
}

CFRunLoopSourceRef	CACFRemoteMessagePort::GetRunLoopSource() const
{
	Assert(mDispatchQueue == NULL, "CACFRemoteMessagePort::SetDispatchQueue: should have both a run loop source and a dispatch queue");
	if(mRunLoopSource == NULL)
	{
		const_cast<CACFRemoteMessagePort*>(this)->mRunLoopSource = CFMessagePortCreateRunLoopSource(NULL, mMessagePort, 0);
	}
	return mRunLoopSource;
}

void	CACFRemoteMessagePort::SetDispatchQueue(dispatch_queue_t inDispatchQueue)
{
	Assert(mRunLoopSource == NULL, "CACFRemoteMessagePort::SetDispatchQueue: should have both a run loop source and a dispatch queue");
	mDispatchQueue = inDispatchQueue;
	CFMessagePortSetDispatchQueue(mMessagePort, mDispatchQueue);
}
