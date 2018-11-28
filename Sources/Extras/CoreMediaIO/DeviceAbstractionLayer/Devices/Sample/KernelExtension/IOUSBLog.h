//
//  IOUSBLog.h
//  Sample
//
//  Created by Tamás Lustyik on 2018. 11. 28..
//

#ifndef IOUSBLog_h
#define IOUSBLog_h

// contents of this file originate from deprecated header <IOKit/usb/IOUSBLog.h>

// Possible Debug levels. If DEBUG_LEVEL is set to DEBUG_LEVEL_PRODUCTION, all debug logs will be
// stripped of the final code.
#define    DEBUG_LEVEL_PRODUCTION        0
#define    DEBUG_LEVEL_DEVELOPMENT        1
#define    DEBUG_LEVEL_ALPHA            2
#define    DEBUG_LEVEL_BETA            3
#define    DEBUG_LEVEL_FINAL            DEBUG_LEVEL_PRODUCTION

// Allow clients to define their own debug level.

#if ( !defined( DEBUG_LEVEL ) )
#define    DEBUG_LEVEL            DEBUG_LEVEL_FINAL
#endif


#ifdef KERNEL

#if DEBUG_LEVEL != DEBUG_LEVEL_PRODUCTION
    // Info Debug levels.
    typedef UInt32        KernelDebugLevel;

    // Info Debug Output Types.
    typedef UInt32		KernelDebuggingOutputType;
    enum
    {
        kKernelDebugOutputIOLogType		= 0x00000001,
        kKernelDebugOutputKextLoggerType	= 0x00000002
    };

    extern "C" {
        void            KernelDebugEnable( bool enable );
        void            KernelDebugSetLevel( KernelDebugLevel inLevel );
        void            KernelDebugSetOutputType( KernelDebuggingOutputType inType );
        void            KernelDebugLogInternal( KernelDebugLevel inLevel, UInt32 inTag, char const *inFormatString, ... )  __attribute__ ((format(printf,3,4)));;
    }

	#define	KernelDebugLogTag( LEVEL, TAG, ARGS... )		KernelDebugLogInternal( ( LEVEL ), ( TAG ),  ## ARGS )
#else
	#define	KernelDebugLogTag( LEVEL, TAG, ARGS... )
#endif

#define USBLog( LEVEL, ARGS...)        KernelDebugLogTag( LEVEL, 'USBF', ## ARGS )

#else

#define USBLog( LEVEL, ARGS...)

#endif // KERNEL

#endif /* IOUSBLog_h */
