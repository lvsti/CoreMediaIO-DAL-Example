/*
	    File: CMIO_DP_Property.h
	Abstract: n/a
	 Version: 1.2

*/

#if !defined(__CMIO_DP_Property_h__)
#define __CMIO_DP_Property_h__

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// System Includes
#include <CoreMediaIO/CMIOHardware.h>

namespace CMIO { namespace DP
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Property
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class Property
	{
	// Construction/Destruction
	public:
						Property();
		virtual			~Property();

		virtual void	Initialize();
		virtual void	Teardown();
		
	private:
						Property(const Property&);
		Property&		operator=(const Property&);

	// Operations
	public:
		virtual bool	IsActive(const CMIOObjectPropertyAddress& inAddress) const;
		virtual bool	IsPropertySettable(const CMIOObjectPropertyAddress& inAddress) const;
		virtual UInt32	GetPropertyDataSize(const CMIOObjectPropertyAddress& inAddress, UInt32 inQualifierDataSize, const void* inQualifierData) const;
		virtual void	GetPropertyData(const CMIOObjectPropertyAddress& inAddress, UInt32 inQualifierDataSize, const void* inQualifierData, UInt32 dataSize, UInt32& dataUsed, void* outData) const;
		virtual void	SetPropertyData(const CMIOObjectPropertyAddress& inAddress, UInt32 inQualifierDataSize, const void* inQualifierData, UInt32 inDataSize, const void* inData);

		virtual UInt32	GetNumberAddressesImplemented() const;
		virtual void	GetImplementedAddressByIndex(UInt32 inIndex, CMIOObjectPropertyAddress& outAddress) const;
	};
}}
#endif
