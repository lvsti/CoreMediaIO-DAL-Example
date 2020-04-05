/*
	    File: CMIO_DP_Property_EndOfData.h
	Abstract: Implements the kCMIOStreamPropertyEndOfData property.
	 Version: 1.2

*/

#if !defined(__CMIO_DP_Property_EndOfData_h__)
#define __CMIO_DP_Property_EndOfData_h__

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
	// EndOfData
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class EndOfData : public Base
	{
	// Construction/Destruction
	public:
						EndOfData(Stream& owningStream) : Base(), mOwningStream(owningStream), mEndOfData(false) {}
		virtual			~EndOfData() {}
	
	private:
		Stream&			GetOwningStream() const { return mOwningStream; }
		Stream&			mOwningStream;
		
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
	public:
		void			SetEndOfData(Boolean endOfData, bool sendChangeNotifications = true);
		UInt32			IsEndOfData() { return mEndOfData; };
	
	private:
		UInt32			mEndOfData;
	};
}}}
#endif
