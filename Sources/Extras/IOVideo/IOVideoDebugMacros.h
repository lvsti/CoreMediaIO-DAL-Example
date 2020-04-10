/*
	    File: IOVideoDebugMacros.h
	Abstract: Macros
				These macros are only slightly different from the ones in CMIODebugMacros.h. We split them out here for two reasons.
				First, IOLog doesn't have a file argument. This makes it incompatible with CMIODebugPrintf.h. Second, we want to be darn sure about what code we compile into a kext.
	
	 Version: 1.2

*/

#if !defined(__IOVideoDeviceDebugMacros_h__)
#define __IOVideoDeviceDebugMacros_h__

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include <IOKit/IOLib.h>

#if __BIG_ENDIAN__
	#define	IOVD_4CCToCString(the4CC)	{ ((char*)&the4CC)[0], ((char*)&the4CC)[1], ((char*)&the4CC)[2], ((char*)&the4CC)[3], 0 }
#else
	#define	IOVD_4CCToCString(the4CC)	{ ((char*)&the4CC)[3], ((char*)&the4CC)[2], ((char*)&the4CC)[1], ((char*)&the4CC)[0], 0 }
#endif

#if VideoDevice_Debug

#pragma mark Debug Macros

#define	DebugMessage(message, ...)	IOLog(message, ## __VA_ARGS__)

#define	Assert(inCondition, inMessage)													\
			if (!(inCondition))															\
			{																			\
				DebugMessage(inMessage);												\
			}

#define	AssertNoError(inError, inMessage)												\
			{																			\
				SInt32 __Err = (inError);												\
				if (__Err != 0)															\
				{																		\
					char __4CC[5] = IOVD_4CCToCString(__Err);								\
					DebugMessageN2(inMessage ", Error: %d (%s)", (int)__Err, __4CC);		\
				}																		\
			}

#define	AssertNoKernelError(inError, inMessage)											\
			{																			\
				unsigned int __Err = (unsigned int)(inError);							\
				if (__Err != 0)															\
				{																		\
					DebugMessageN1(inMessage ", Error: 0x%X", __Err);					\
				}																		\
			}

#define	FailIf(inCondition, inHandler, inMessage)										\
			if (inCondition)																\
			{																			\
				DebugMessage(inMessage);												\
				goto inHandler;															\
			}

#define	FailIfNULL(inPointer, inHandler, inMessage)										\
			if ((inPointer) == NULL)														\
			{																			\
				DebugMessage(inMessage);												\
				goto inHandler;															\
			}

#define	FailIfError(inError, inHandler, inMessage)										\
			{																			\
				SInt32 __Err = (inError);												\
				if (__Err != 0)															\
				{																		\
					DebugMessageN2(inMessage ", Error: %ld (0x%X)", __Err, (unsigned int)__Err);		\
					goto inHandler;														\
				}																		\
			}

#define	FailIfWithAction(inCondition, inAction, inHandler, inMessage)					\
			if (inCondition)																\
			{																			\
				DebugMessage(inMessage);												\
				{ inAction; }															\
				goto inHandler;															\
			}

#define	FailIfNULLWithAction(inPointer, inAction, inHandler, inMessage)					\
			if ((inPointer) == NULL)														\
			{																			\
				DebugMessage(inMessage);												\
				{ inAction; }															\
				goto inHandler;															\
			}

#define	FailIfErrorWithAction(inError, inAction, inHandler, inMessage)					\
			{																			\
				SInt32 __Err = (inError);												\
				if (__Err != 0)															\
				{																		\
					DebugMessageN2(inMessage ", Error: %ld (0x%X)", __Err, __Err);		\
					{ inAction; }														\
					goto inHandler;														\
				}																		\
			}

#else

#pragma mark Release Macros

#define	DebugMessage(message, ...)

#define	Assert(inCondition, inMessage)													\
			if (!(inCondition))															\
			{																			\
			}

#define	AssertNoError(inError, inMessage)												\
			{																			\
				SInt32 __Err = (inError);												\
				if (__Err != 0)															\
				{																		\
				}																		\
			}

#define	AssertNoKernelError(inError, inMessage)											\
			{																			\
				unsigned int __Err = (unsigned int)(inError);							\
				if (__Err != 0)															\
				{																		\
				}																		\
			}

#define	FailIf(inCondition, inHandler, inMessage)										\
			if (inCondition)																\
			{																			\
				goto inHandler;															\
			}

#define	FailIfNULL(inPointer, inHandler, inMessage)										\
			if ((inPointer) == NULL)														\
			{																			\
				goto inHandler;															\
			}

#define	FailIfError(inError, inHandler, inMessage)										\
			if ((inError) != 0)															\
			{																			\
				goto inHandler;															\
			}

#define	FailIfWithAction(inCondition, inAction, inHandler, inMessage)					\
			if (inCondition)																\
			{																			\
				{ inAction; }															\
				goto inHandler;															\
			}

#define	FailIfNULLWithAction(inPointer, inAction, inHandler, inMessage)					\
			if ((inPointer) == NULL)														\
			{																			\
				{ inAction; }															\
				goto inHandler;															\
			}

#define	FailIfErrorWithAction(inError, inAction, inHandler, inMessage)					\
			if ((inError) != 0)															\
			{																			\
				{ inAction; }															\
				goto inHandler;															\
			}

#endif

#endif
