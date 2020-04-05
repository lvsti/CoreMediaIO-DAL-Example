/*
     File: CACFMachPort.h
 Abstract: Part of CoreAudio Utility Classes
  Version: 1.1

*/
#if !defined(__CACFMachPort_h__)
#define __CACFMachPort_h__

//==================================================================================================
//	Includes
//==================================================================================================

//	System Includes
#include <CoreFoundation/CFMachPort.h>
#include <mach/mach.h>

//==================================================================================================
//	CACFMachPort
//
//	This class wraps a CFMachPort.
//	
//	Note that when you create a CFMachPort object, CF will attach the run loop source for the the
//	Mach Port that handles Port Death notifications (aka the Invalidation Callback) to the current
//	thread's run loop. This is something over which there is no control, so be sure to create the
//	CFMachPort on the thread on which you want to handle Port Death notificaitons on.
//==================================================================================================

class	CACFMachPort
{

//	Construction/Destruction
public:
						CACFMachPort(CFMachPortCallBack inCallBack, void* inUserData = NULL);
						CACFMachPort(mach_port_t inMachPort, CFMachPortCallBack inCallBack, CFMachPortInvalidationCallBack inInvalidationCallBack, void* inUserData);
	virtual				~CACFMachPort();

//	Attributes
public:
	CFMachPortRef		GetMachPortRef() const { return mMachPort; }
	mach_port_t			GetMachPort() const { return CFMachPortGetPort(mMachPort); }
	CFRunLoopSourceRef	GetRunLoopSource() const { return mRunLoopSource; }

//	Operations
public:
	kern_return_t		ReceiveMessage(UInt32 inMaxMessageSize, mach_msg_header_t* outMessage, mach_msg_timeout_t inTimeOut);

//	Implementation
protected:
	CFMachPortRef		mMachPort;
	CFRunLoopSourceRef	mRunLoopSource;
	bool				mOwnsPort;
};

#endif
