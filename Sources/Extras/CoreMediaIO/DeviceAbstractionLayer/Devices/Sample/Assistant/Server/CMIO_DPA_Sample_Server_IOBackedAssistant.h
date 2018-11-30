/*
	    File: CMIO_DPA_Sample_Server_IOBackedAssistant.h
	Abstract: Server which handles all the IPC between the various Sample DAL PlugIn instances.
	 Version: 1.2
	
*/

#if !defined(__CMIO_DPA_Sample_Server_IOBackedAssistant_h__)
#define __CMIO_DPA_Sample_Server_IOBackedAssistant_h__

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "CMIO_DPA_Sample_Server_Assistant.h"

// Public Utility Includes
#include "CMIO_IOKA_Object.h"
#include "CMIO_PTA_NotificationPortThread.h"

// Standard Library Includes
#include <vector>

namespace CMIO { namespace DPA { namespace Sample { namespace Server
{
    class IOBackedAssistant: public Assistant
	{
	// Construction/Destruction
	public:
		static IOBackedAssistant*			Instance();

	public:
									IOBackedAssistant();
        static IOBackedAssistant*           sInstance;

    public:
        virtual kern_return_t               GetDeviceStates(Client client, mach_port_t messagePort, DeviceState** deviceStates, mach_msg_type_number_t* length) override;
        
    // Device Mangagement
	public:
		static void					DeviceArrived(IOBackedAssistant& assistant, io_iterator_t iterator);
		void						DeviceRemoved(Device& device);

	private:
		void						InitializeDeviceAddedNotification();
		void						CreateDeviceAddedNotification(CFMutableDictionaryRef matchingDictionary);

		typedef std::vector<IOKA::Object>			NotificationIterators;

		PTA::NotificationPortThread	mNotificationPortThread;				// Thread for getting IOKit notifications on
		NotificationIterators		mDeviceAddedIterators;					// A "kIOMatchedNotification" notifcation iterator for each "matching dictionary"
	};
}}}}
#endif
