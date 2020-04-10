/*
	    File: CMIO_DP_Property_Base.h
	Abstract: n/a
	 Version: 1.2

*/

#if !defined(__CMIO_DP_Property_Base_h__)
#define __CMIO_DP_Property_Base_h__

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// System Includes
#include <CoreMediaIO/CMIOHardware.h>

namespace CMIO { namespace DP { namespace Property
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Base
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class Base
	{
	// Construction/Destruction
	public:
						Base();
		virtual			~Base();

		virtual void	Initialize();
		virtual void	Teardown();
		
	private:
						Base(const Base&);
		Base&			operator=(const Base&);

	// Operations
	public:
		virtual bool	IsActive(const CMIOObjectPropertyAddress& address) const;
		virtual bool	IsPropertySettable(const CMIOObjectPropertyAddress& address) const;
		virtual UInt32	GetPropertyDataSize(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData) const;
		virtual void	GetPropertyData(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData, UInt32 dataSize, UInt32& dataUsed, void* data) const;
		virtual void	SetPropertyData(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData, UInt32 dataSize, const void* data);

		virtual UInt32	GetNumberAddressesImplemented() const;
		virtual void	GetImplementedAddressByIndex(UInt32 index, CMIOObjectPropertyAddress& address) const;
	};
}}}
#endif
