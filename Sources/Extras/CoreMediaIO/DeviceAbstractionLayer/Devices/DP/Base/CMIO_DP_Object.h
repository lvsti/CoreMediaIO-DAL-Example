/*
	    File: CMIO_DP_Object.h
	Abstract: n/a
	 Version: 1.2

*/

#if !defined(__CMIO_DP_Object_h__)
#define __CMIO_DP_Object_h__

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// System Includes
#include <CoreMediaIO/CMIOHardwarePlugIn.h>

// Public Utility Includes
#include "CMIO_PropertyAddress.h"

// Standard Library Includes
#include <vector>

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Types in the global namespace
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CAMutex;

namespace CMIO { namespace DP
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Types in the CMIO::DP namespace
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class PlugIn;
	class Device;
	class Stream;
}}

namespace CMIO { namespace DP { namespace Property
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Types in the CMIO::DP::Property namespace
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class Base;
}}}


namespace CMIO { namespace DP
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Object
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class Object
	{
	// Construction/Destruction
	public:
								Object(CMIOObjectID objectID, CMIOClassID classID, PlugIn& plugIn);
		virtual					~Object();
		
		virtual void			Initialize();
		virtual void			Teardown();

	private:
								Object(const Object&);
		Object&					operator=(const Object&);

	// Attributes
	public:
		CMIOObjectID			GetObjectID() const { return mObjectID; }
		void					SetObjectID(CMIOObjectID objectID);
		CMIOClassID				GetClassID() const { return mClassID; }
		PlugIn&					GetPlugIn() const { return mPlugIn; }

		static bool				IsSubClass(CMIOClassID classID, CMIOClassID baseClassID);

	protected:
		CMIOObjectID			mObjectID;
		CMIOClassID				mClassID;
		PlugIn&					mPlugIn;
		
	// Operations
	public:
		virtual CAMutex*		GetObjectStateMutex();
		virtual void			Show() const;
		
		static Object*			GetObjectByID(CMIOObjectID objectID);
		static Device*			GetDeviceByID(CMIOObjectID objectID);
		static Stream*			GetStreamByID(CMIOObjectID objectID);

		static CAMutex*			GetObjectStateMutexByID(CMIOObjectID objectID);
		static void				SetObjectStateMutexForID(CMIOObjectID objectID, CAMutex* mutex);
		
	// Property Access
	public:
		virtual bool			HasProperty(const CMIOObjectPropertyAddress& address) const;
		virtual bool			IsPropertySettable(const CMIOObjectPropertyAddress& address) const;
		virtual UInt32			GetPropertyDataSize(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData) const;
		virtual void			GetPropertyData(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData, UInt32 dataSize, UInt32& dataUsed, void* data) const;
		virtual void			SetPropertyData(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData, UInt32 dataSize, const void* data);
		virtual void			PropertiesChanged(UInt32 numberAddresses, const CMIOObjectPropertyAddress addresses[]) const;

	protected:
		virtual void			PropertyListenerAdded(const CMIOObjectPropertyAddress& address);
		virtual void			PropertyListenerRemoved(const CMIOObjectPropertyAddress& address);

	// Property Management
	public:
		void					AddProperty(Property::Base* property);
		void					RemoveProperty(Property::Base* property);

	private:
		typedef std::pair<PropertyAddress, Property::Base*>	PropertyMapItem;
		typedef std::vector<PropertyMapItem>				PropertyMap;
		
		PropertyMap::iterator	FindPropertyByAddress(const CMIOObjectPropertyAddress& address);
		Property::Base*			FindActivePropertyByAddress(const CMIOObjectPropertyAddress& address) const;
		
		PropertyMap				mPropertyMap;
	};
}}
#endif
