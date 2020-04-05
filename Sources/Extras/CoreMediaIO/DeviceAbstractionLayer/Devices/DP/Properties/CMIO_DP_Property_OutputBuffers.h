/*
	    File: CMIO_DP_Property_OutputBuffers.h
	Abstract: Implements the kCMIOStreamPropertyOutputBufferQueueSize, kCMIOStreamPropertyOutputBuffersRequiredForStartup, kCMIOStreamPropertyOutputBufferUnderrunCount, and
				kCMIOStreamPropertyOutputBufferRepeatCount properties.
	
	 Version: 1.2

*/

#if !defined(__CMIO_DP_Property_OutputBuffers_h__)
#define __CMIO_DP_Property_OutputBuffers_h__

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Super Class Includes
#include "CMIO_DP_Property_Base.h"

namespace CMIO { namespace DP
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Types in the CMIO::DP namespace
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class Stream;
}};

namespace CMIO { namespace DP { namespace Property
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// OutputBuffers
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class OutputBuffers : public Base
	{
	// Construction/Destruction
	public:
						OutputBuffers(Stream& owningStream, UInt32 defaultQueueSize);
		virtual			~OutputBuffers() {}
	
	// Property Operations
	public:
		virtual bool	IsActive(const CMIOObjectPropertyAddress& address) const;
		virtual bool	IsPropertySettable(const CMIOObjectPropertyAddress& address) const;
		virtual UInt32	GetPropertyDataSize(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData) const;
		virtual void	GetPropertyData(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData, UInt32 dataSize, UInt32& dataUsed, void* data) const;
		virtual void	SetPropertyData(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData, UInt32 dataSize, const void* data);

		virtual UInt32	GetNumberAddressesImplemented() const;
		virtual void	GetImplementedAddressByIndex(UInt32 index, CMIOObjectPropertyAddress& address) const;
	
	// Property Implementation
		void			Reset()						{ mQueueSize = mDefaultQueueSize; mBuffersRequiredForStartup = mDefaultBuffersRequiredForStartup; mUnderrunCount = 0; mRepeatCount = 0; }
		void			BumpUnderrunCount(bool sendChangeNotifications = true);
		void			BumpRepeatCount(bool sendChangeNotifications = true);
	
		UInt32			QueueSize()					{ return mQueueSize; };
		UInt32			BuffersRequiredForStartup()	{ return mBuffersRequiredForStartup; };
		UInt32			UnderrunCount()				{ return mUnderrunCount; };
		UInt32			RepeatCount()				{ return mRepeatCount; };
	
	private:
		Stream&			GetOwningStream() const { return mOwningStream; }
		Stream&			mOwningStream;
		
		UInt32			mDefaultQueueSize;
		UInt32			mDefaultBuffersRequiredForStartup;
		UInt32			mQueueSize;
		UInt32			mBuffersRequiredForStartup;
		UInt32			mUnderrunCount;
		UInt32			mRepeatCount;
	};
}}}
#endif
