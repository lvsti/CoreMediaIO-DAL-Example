/*
	    File: CMIO_IOSA_Assistance.h
	Abstract: The IOStream Assisistant (IOSA) consists of various objects to facilitate using the "Acquisition is Initialization" design pattern.
				NOTE: the IOSA is sparsely implemented, meaning that it does not attempt to provide wrappers/access to all IOStream features, but just those needed by the IOStream plugin.
	
	 Version: 1.2

*/

#if !defined(__CMIO_IOSA_Assistance_h__)
#define __CMIO_IOSA_Assistance_h__

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// CA Public Utility Includes
#include "CMIODebugMacros.h"
#include "CAException.h"

// System Includes
#include <IOKit/stream/IOStreamLib.h>

namespace CMIO { namespace IOSA
{

    IOCFPlugInInterface** 			AllocatePlugIn(io_service_t service);
    IOStreamRef                     AllocateStream(IOCFPlugInInterface** plugIn);

    //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    // PlugIn
    //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    class PlugIn
    {
	// Construction/Destruction
	public:
		explicit				PlugIn(IOCFPlugInInterface** plugIn) : mPlugin(NULL) { Reset(plugIn); }
								~PlugIn() { Reset(NULL); }
		void					Reset(IOCFPlugInInterface** plugIn = NULL) { if (NULL != mPlugin) IODestroyPlugInInterface(mPlugin); mPlugin = plugIn; }
        
	private:
		PlugIn&					operator=(PlugIn& that); // Don't allow copying
		IOCFPlugInInterface**	mPlugin;
		
	// Value Access
	public:
		operator				IOCFPlugInInterface**()	const { return mPlugin; }
		IOCFPlugInInterface**	Get() const { return mPlugin; }													
    };
    
    //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Stream
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class Stream
	{
	// Construction/Destruction
	public:
					Stream(IOStreamRef stream) : mStream(NULL), mOpened(false) { Reset(stream); }
					~Stream() { Reset(); }
		void		Reset(IOStreamRef stream = NULL, CFRunLoopRef runLoop = NULL)
					{
						if (NULL != mStream)
						{
							Close();
							(**mStream).Release(mStream);
							mStream = NULL;
						} 
						
						if (NULL != stream)
						{
							mStream = stream;
						}
					}
        
	private:
		Stream&		operator=(Stream& that); // Don't allow copying
		IOStreamRef	mStream;
		bool		mOpened;
		
	// Attributes
	public:
		bool		IsValid() const { return NULL != mStream; }

   // Operations
	public:
		void		Open() { IOReturn ioReturn = (**mStream).Open(mStream, 0); ThrowIfKernelError(ioReturn, CAException(ioReturn), "CMIO::IOSA::Stream::Open() failed"); mOpened = true; }
		void		Close() { if (not mOpened) return; IOReturn ioReturn = (**mStream).Close(mStream); DebugMessageIfError(ioReturn, "CMIO::IOSA::Stream::Close() failed"); mOpened = false; }
         void		AddToRunLoop(CFRunLoopRef runLoop) { IOReturn ioReturn = (**mStream).AddToRunLoop(mStream, runLoop); ThrowIfKernelError(ioReturn, CAException(ioReturn), "CMIO::IOSA::Stream::AddToRunLoop() failed"); }
        void		RemoveFromRunLoop(CFRunLoopRef runLoop) { IOReturn ioReturn = (**mStream).RemoveFromRunLoop(mStream, runLoop); ThrowIfKernelError(ioReturn, CAException(ioReturn), "CMIO::IOSA::Stream::RemoveFromRunLoop() failed"); }
        void		SetOutputCallback(IOStreamOutputCallback outputCallback, void* context) { IOReturn ioReturn = (**mStream).SetOutputCallback(mStream, outputCallback, context); ThrowIfKernelError(ioReturn, CAException(ioReturn), "CMIO::IOSA::Stream::SetOutputCallback() failed"); }
		void		Start() { IOReturn ioReturn = (**mStream).StartStream(mStream); ThrowIfKernelError(ioReturn, CAException(ioReturn), "CMIO::IOSA::Stream::Start() failed"); }
		void		Stop() { IOReturn ioReturn = (**mStream).StopStream(mStream); DebugMessageIfError(ioReturn, "CMIO::IOSA::Stream::Stop() failed"); }
		void*		GetDataBuffer(IOStreamBufferID bufferID) { return (**mStream).GetDataBuffer(mStream, bufferID); }
		void*		GetControlBuffer(IOStreamBufferID bufferID) { return (**mStream).GetControlBuffer(mStream, bufferID); }

	// Value Access
	public:
		operator	IOStreamRef() const { return mStream; }
		IOStreamRef	Get() const { return mStream; }													
		bool		Opened() const { return mOpened; }													
	};
}}

#endif
