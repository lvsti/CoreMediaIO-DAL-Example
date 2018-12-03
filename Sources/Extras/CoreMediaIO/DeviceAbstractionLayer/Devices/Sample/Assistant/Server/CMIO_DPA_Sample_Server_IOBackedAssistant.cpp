/*
	    File: CMIO_DPA_Sample_Server_IOBackedAssistant.cpp
	Abstract: Server which handles all the IPC between the various Sample DAL PlugIn instances.
	 Version: 1.2
	
*/

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Self Include
#include "CMIO_DPA_Sample_Server_IOBackedAssistant.h"

// Internal Includes
#include "CMIO_DPA_Sample_Server_IOBackedDevice.h"

// Public Utility Includes
#include "CMIODebugMacros.h"

// CA Public Utility Includes
#include "CAAutoDisposer.h"
#include "CACFNumber.h"
#include "CACFObject.h"

// System Includes
#include <CoreAudio/HostTime.h>
#include <mach-o/dyld.h>

// Standard Library Includes
#include <algorithm>


#pragma mark -
namespace CMIO { namespace DPA { namespace Sample { namespace Server
{
	#pragma mark Static Globals
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Static Globals
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    IOBackedAssistant* IOBackedAssistant::sInstance = nullptr;

	#pragma mark -
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// IOBackedAssistant()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	IOBackedAssistant::IOBackedAssistant() :
        Assistant(),
		mNotificationPortThread(true),
		mDeviceAddedIterators()
	{
		// Wait for the notification port thread to be running prior to continuing
		while (PTA::NotificationPortThread::kStarting == mNotificationPortThread.GetState())
			pthread_yield_np();

		// Make sure the notification port is not invalid
		ThrowIf(PTA::NotificationPortThread::kInvalid == mNotificationPortThread.GetState(), -1, "Notification thread invalid");
		
		// Initialize the notification for hot plugging of devices.  In addition to handling future hot plug events, this will also set up the devices that are currently plugged in
		InitializeDeviceAddedNotification();
	}
	
    #pragma mark -
    //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    // Instance()
    //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    IOBackedAssistant* IOBackedAssistant::Instance()
    {
        if (!sInstance) {
            sInstance = new IOBackedAssistant();
        }
        
        return sInstance;
    }
    
	#pragma mark -
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// GetDeviceStates()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	kern_return_t IOBackedAssistant::GetDeviceStates(Client client, mach_port_t messagePort, DeviceState** deviceStates, mach_msg_type_number_t* length)
	{
		try
		{
			// Grab the mutex for the Assistant's state
			CAMutex::Locker locker(mStateMutex);
			
			// Add the <client, messagePort> pair to the multimap of clients to notify for future device state changes (added/removed)
			mDeviceStateNotifiers.insert(std::make_pair(client, messagePort));

			// Report the size of the variable length DeviceStates array
			*length = (mach_msg_type_number_t)mDevices.size();

			// Simply return if there are no devices present
			if (mDevices.empty())
				return KERN_SUCCESS;
				
			// Allocate the memory for returning the devices' state
			kern_return_t err =  vm_allocate(mach_task_self(), reinterpret_cast<vm_address_t*>(deviceStates), sizeof(DeviceState) * mDevices.size(), true);
			ThrowIfKernelError(err, CAException(err), "CMIO::DPA::Sample::Server::Assistant::GetDeviceStates couldn't allocate storage for DeviceState[]");
			
			// Populate the devices state array for all the devices currently present
			int index = 0;
			for (Devices::const_iterator i = mDevices.begin() ; i != mDevices.end() ; ++i)
			{
                const IOBackedDevice& device = *(static_cast<IOBackedDevice*>(*i));
                (*deviceStates)[index].mGUID = device.GetDeviceGUID();
                (*deviceStates)[index].mIsIOBacked = true;
				device.GetRegistryPath((*deviceStates)[index].mRegistryPath);
				++index;
			}
		}
		catch (CAException& exception)
		{
			return exception.GetError();
		}

		return KERN_SUCCESS;
	}

	#pragma mark -
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// DeviceAdded()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void IOBackedAssistant::DeviceArrived(IOBackedAssistant& assistant, io_iterator_t iterator)
	{
		#if 0
			// Wait forever until the Debugger can attach to the Assistant process
			bool waiting = true;
			while (waiting)
			{
				sleep(1);
			}
		#endif
		
		// Catch all exceptions since this is invoked via a call back and the exception cannot leave this routine 
		try
		{
			// Grab the mutex for the Assistant's state
			CAMutex::Locker locker(assistant.mStateMutex);
			
			// Get the current device count
			UInt32 deviceCount = (UInt32)assistant.mDevices.size();
			
			while (true)
			{
				IOKA::Object registryEntry(IOIteratorNext(iterator));
				if (not registryEntry.IsValid())
					break;
				
				// Make sure the registry entry conforms to an IOVideoDevice
				if (not registryEntry.ConformsTo("IOVideoDevice"))
					continue;

				Device* device = NULL;
				
				// Catch all exceptions so the iterator can be advanced to the next device in the event of any problems
				try
				{
					// Create the new device
					device = new IOBackedDevice(registryEntry, assistant.mNotificationPortThread);
					
					// Add it to the set of discovered devices whose capabilities are known
					assistant.mDevices.insert(device);
				}
				catch (CAException& exception)
				{
					if (NULL != device)
						delete device;
				}
			}
			
			// If any devices were successfully added, notify interested clients that a state change has taken place so they can call UpdateDeviceStates() at their convenience
			if (deviceCount != assistant.mDevices.size())
			{
				// Send out the devices state changed message
				for (ClientNotifiers::iterator i = assistant.mDeviceStateNotifiers.begin() ; i != assistant.mDeviceStateNotifiers.end() ; ++i)
					assistant.SendDeviceStatesChangedMessage((*i).second);

				// All the 'send-once' rights are now used up, so erase everything in the multimap 
				assistant.mDeviceStateNotifiers.erase(assistant.mDeviceStateNotifiers.begin(), assistant.mDeviceStateNotifiers.end());
			}
		}
		catch (...)
		{
		}
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// DeviceRemoved()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void IOBackedAssistant::DeviceRemoved(Device& device)
	{
		// Catch all exceptions since exceptions cannot leave this routine 
		try
		{
			// Grab the mutex for the Assistant's state
			CAMutex::Locker locker(mStateMutex);
			
			// Iterate over the ClientInfoMap and remove this device from each clients' set of streams it was watching
			for (ClientInfoMap::iterator i = mClientInfoMap.begin() ; i != mClientInfoMap.end() ; ++i)
			{
				while (true)
				{
					// See if this client was watching a stream from this device
					StreamSpecifiers::iterator ii = std::find_if((*i).second->mStreamSpecifiers.begin(), (*i).second->mStreamSpecifiers.end(), StreamSpecifier::DeviceEqual(device));
					if (ii == (*i).second->mStreamSpecifiers.end())
						break;
					
					// This client had a stream that corresponded to this device, so erase it
					(void) (*i).second->mStreamSpecifiers.erase(ii);
				}
			}

			// Erase the device from the set of devices which the Assistant knows about
			(void) mDevices.erase(&device);
			
			// Notify interested clients that a state change has taken place so they can call UpdateDeviceStates() at their convenience
			for (ClientNotifiers::iterator i = mDeviceStateNotifiers.begin() ; i != mDeviceStateNotifiers.end() ; ++i)
				SendDeviceStatesChangedMessage((*i).second);

			// All the 'send-once' rights are now used up, so erase everything multimap 
			mDeviceStateNotifiers.erase(mDeviceStateNotifiers.begin(), mDeviceStateNotifiers.end());
		}
		catch (...)
		{
		}
	}

	#pragma mark -
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// InitializeDeviceAddedNotification()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void IOBackedAssistant::InitializeDeviceAddedNotification()
	{
		// Create a matching dictionary to specify that only Sample devices are of interest
		CACFDictionary matchingDictionary(IOServiceMatching("IOVideoSampleDevice"), true);
		ThrowIf(not matchingDictionary.IsValid(), -1, "Assistant::InitializeDeviceAddedNotification: unable to get service matching dictionary");

		// Create the notification
		CreateDeviceAddedNotification(matchingDictionary.GetCFMutableDictionary());
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// CreateDeviceAddedNotification()
	//	Request device added notfication for the device specified in the provided matching dictionary.
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void IOBackedAssistant::CreateDeviceAddedNotification(CFMutableDictionaryRef matchingDictionary)
	{
		// IOServiceAddMatchingNotification 'eats' a matching dictionary, so up the retention count
		CFRetain(matchingDictionary);

		IOKA::Object iterator;
		IOReturn ioReturn = IOServiceAddMatchingNotification(mNotificationPortThread.GetNotificationPort(), kIOMatchedNotification, matchingDictionary, reinterpret_cast<IOServiceMatchingCallback>(DeviceArrived), this, iterator.GetAddress());
		ThrowIfError(ioReturn, CAException(ioReturn), "Assistant::CreateDeviceAddedNotification: IOServiceAddMatchingNotification() failed");
			
		mDeviceAddedIterators.push_back(iterator);
		
		// The iterator is returned unarmed, but full of the devices which matched the dictionary.  So manually invoke the DeviceArrived() routine to add all the devices already present and
		// to arm the iterator for subsequent additions.
		DeviceArrived(*this, iterator);
	}
}}}}
