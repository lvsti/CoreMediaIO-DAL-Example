/*
	    File: CMIO_DP_PlugIn.h
	Abstract: n/a
	 Version: 1.2

*/

#if !defined(__CMIO_DP_PlugIn_h__)
#define __CMIO_DP_PlugIn_h__

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Super Class Includes
#include "CMIO_DP_Object.h"

// CA Public Utility Includes
#include "CACFObject.h"
#include "CAMutex.h"

// Standard Library Includes
#include <map>


namespace CMIO { namespace DP
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// PlugIn
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class PlugIn : public Object
	{
	// Construction/Destruction
	public:
												PlugIn(CFUUIDRef factoryUUID);
		virtual									~PlugIn();
		
		virtual void							Initialize();
		virtual void							InitializeWithObjectID(CMIOObjectID objectID);
		virtual void							Teardown();

	// Reference Counting
	public:
		virtual UInt32							Retain();
		virtual UInt32							Release();

	// Basic Operations
	public:
//		virtual dispatch_queue_t				GetDispatchQueue() = 0;
		virtual CAMutex*						GetObjectStateMutex() { return &mStateMutex; }
		CAMutex&								GetStateMutex() { return mStateMutex; }
		virtual void							Show() const;

	private:
		CAMutex									mStateMutex;

	// Device Management
	public:
		virtual UInt32							GetNumberDevices() const;
		virtual Object*							GetDeviceByIndex(UInt32 index) const;
		virtual Object*							GetDeviceByObjectID(CMIOObjectID id) const;
		virtual void							AddDevice(Object& device);
		virtual void							RemoveDevice(Object& device);

	protected:
		typedef std::map<CMIOObjectID, Object*> DeviceMap;
		DeviceMap								mDeviceMap;

	// Implementation
	public:
		CMIOHardwarePlugInRef					GetInterface() { return &mInterface; }
		bool									SameFactoryUUID(CFUUIDRef uuid) { return mFactoryUUID.IsEqual(uuid); }

		static PlugIn*							GetObject(CMIOHardwarePlugInRef ref) { PlugIn* p = (PlugIn*)ref; return (PlugIn*)((Byte*)p - ((Byte*)&p->mInterface - (Byte*)p)); }
		
	private:
		CMIOHardwarePlugInInterface*			mInterface;
		CACFUUID								mFactoryUUID;
		UInt32									mRefCount;

		static CMIOHardwarePlugInInterface		sInterface;
	};
}}
#endif
