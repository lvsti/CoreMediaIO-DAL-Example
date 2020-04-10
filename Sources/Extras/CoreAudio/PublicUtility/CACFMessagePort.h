/*
     File: CACFMessagePort.h
 Abstract: Part of CoreAudio Utility Classes
  Version: 1.1

*/
#if !defined(__CACFMessagePort_h__)
#define __CACFMessagePort_h__

//=============================================================================
//	Includes
//=============================================================================

#if !defined(__COREAUDIO_USE_FLAT_INCLUDES__)
	#include <CoreFoundation/CFMessagePort.h>
#else
	#include <CFMessagePort.h>
#endif

//=============================================================================
//	CACFLocalMessagePort
//=============================================================================

class CACFLocalMessagePort
{

//	Construction/Destruction
public:
						CACFLocalMessagePort(CFStringRef inName, CFMessagePortCallBack inPortCallBack, CFMessagePortInvalidationCallBack inInvalidationCallBack, void* inUserData = NULL);
	virtual				~CACFLocalMessagePort();

//	Attributes
public:
	bool				IsValid() const { return mMessagePort != NULL; }
	CFMessagePortRef	GetMessagePortRef() const { return mMessagePort; }
	CFRunLoopSourceRef	GetRunLoopSource() const;
	void				SetDispatchQueue(dispatch_queue_t inDispatchQueue);

//	Implementation
protected:
	CFMessagePortRef	mMessagePort;
	CFRunLoopSourceRef	mRunLoopSource;
	dispatch_queue_t	mDispatchQueue;

};

//=============================================================================
//	CACFRemoteMessagePort
//=============================================================================

class CACFRemoteMessagePort
{

//	Construction/Destruction
public:
						CACFRemoteMessagePort(CFStringRef inName, CFMessagePortInvalidationCallBack inInvalidationCallBack);
	virtual				~CACFRemoteMessagePort();

//	Attributes
public:
	bool				IsValid() const { return mMessagePort != NULL; }
	CFMessagePortRef	GetMessagePortRef() const { return mMessagePort; }
	CFRunLoopSourceRef	GetRunLoopSource() const;
	void				SetDispatchQueue(dispatch_queue_t inDispatchQueue);

//	Operations
public:
	SInt32				SendRequest(SInt32 inMessageID, CFDataRef inData, CFTimeInterval inSendTimeout, CFTimeInterval inReceiveTimout) const { return CFMessagePortSendRequest(mMessagePort, inMessageID, inData, inSendTimeout, inReceiveTimout, NULL, NULL); }
	SInt32				SendRequest(SInt32 inMessageID, CFDataRef inData, CFTimeInterval inSendTimeout, CFTimeInterval inReceiveTimout, CFStringRef inReplyMode, CFDataRef& outReturnData) const { return CFMessagePortSendRequest(mMessagePort, inMessageID, inData, inSendTimeout, inReceiveTimout, inReplyMode, &outReturnData); }

//	Implementation
protected:
	CFMessagePortRef	mMessagePort;
	CFRunLoopSourceRef	mRunLoopSource;
	dispatch_queue_t	mDispatchQueue;

};

#endif
