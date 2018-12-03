/*
	    File: CMIO_DPA_Sample_Server_IOBackedDevice.cpp
	Abstract: n/a
	 Version: 1.2
 
*/

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Self Include
#include "CMIO_DPA_Sample_Server_IOBackedDevice.h"

// Internal Includes
#include "CMIO_KEXT_Sample_ControlIDs.h"
#include "CMIO_DP_ControlDictionary.h"
#include "CMIO_DP_IOV_ControlDictionary.h"
#include "CMIO_DP_Sample_ControlIDs.h"

// Public Utility Includes
#include "CMIODebugMacros.h"
#include "CMIO_IOKA_Iterator.h"
#include "CMIO_PTA_NotificationPortThread.h"

// CA Public Utility Includes
#include "CACFNumber.h"
#include "CAException.h"
#include "CAHostTimeBase.h"

namespace CMIO { namespace DPA { namespace Sample { namespace Server
{
	#pragma mark -
	#pragma mark Device
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Device()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	IOBackedDevice::IOBackedDevice(IOKA::Object& registryEntry, PTA::NotificationPortThread& notificationPortThread) :
        Device(),
		mRegistryEntry(registryEntry),
        mIOVAPlugIn(IOVA::AllocatePlugIn(mRegistryEntry)),
        mIOVADevice(IOVA::AllocateDevice(mIOVAPlugIn)),
		mNotificationThread(notificationPortThread),
		mPowerNotificationPort(GetNotificationThread().GetRunLoop(), kCFRunLoopDefaultMode, reinterpret_cast<IOServiceInterestCallback>(PowerNotification), this),
		mSleeping(false),
		mRestartStreamOnWake(false)
	{
		// Grab the mutex for the device's state
		CAMutex::Locker locker(mStateMutex);
		
		// Get the registry path for the device
		IOReturn err = IORegistryEntryGetPath(mRegistryEntry, kIOServicePlane, mRegistryPath);
		ThrowIfKernelError(err, CAException(err), "CMIO::DPA::Sample::Server::Device:: IORegistryEntryGetPath() failed")
		
		// Open the underlying IOVideoDevice
		mIOVADevice.Open();
		
		// Add the IOVDevice's notifcation port run loop source to the notificaton thread
		mIOVADevice.AddToRunLoop(GetNotificationThread().GetRunLoop());
		
		// Specify the callback for device notifications
		mIOVADevice.SetNotificationCallback(reinterpret_cast<IOVideoDeviceNotificationCallback>(IOVDeviceNotification), this);
		
		// Discover what streams the device has
		DiscoverStreams();

        InitializeControls();
	}
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// ~IOBackedDevice()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	IOBackedDevice::~IOBackedDevice()
	{
		// Grab the mutex for the device's state
		CAMutex::Locker locker(mStateMutex);
		
		// Remove the IOVDevice's notifcation port run loop source from the notificaton thread
		mIOVADevice.RemoveFromRunLoop(GetNotificationThread().GetRunLoop());
		
		// Close the underlying IOVideoDevice (a safe NOP if it is not open)
		mIOVADevice.Close();
	}

	#pragma mark -
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//  DiscoverStreams()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void IOBackedDevice::DiscoverStreams()
	{
		// Grab the mutex for the Device's state
		CAMutex::Locker locker(mStateMutex);

		// Fetch the input streams list
		CACFArray inputStreamList(static_cast<CFArrayRef>(IORegistryEntryCreateCFProperty(mRegistryEntry, CFSTR(kIOVideoDeviceKey_InputStreamList), NULL, 0)), true);
		
		// Iterate through the input streams, locate their registry entries, and add them to the mInputStreams map
		UInt32 streamCount = inputStreamList.GetNumberItems();
		for (UInt32 index = 0 ; index < streamCount ; ++index)
		{
			// Get the stream dictionary
			CFDictionaryRef streamDictionary = NULL;
			if (inputStreamList.GetDictionary(index, streamDictionary))
			{
				// Get the streamID from the the dictionary
				UInt32 streamID = CACFNumber(static_cast<CFNumberRef>(CFDictionaryGetValue(streamDictionary, CFSTR(kIOVideoStreamKey_StreamID))), false).GetSInt32();

				// Iterate over all the device's streams and find the registry entry for this stream
				IOKA::Iterator iterator(mRegistryEntry, kIOServicePlane);
				
				while (true)
				{
					IOKA::Object registryEntry(iterator.Next());
					if (not registryEntry.IsValid())
						break;
					
					// Make sure the registry entry conforms to an IOVideoStream
					if (not registryEntry.ConformsTo("IOVideoStream"))
						continue;
				
					// Make sure the streamIDs are the same
					if (streamID != CACFNumber(static_cast<CFNumberRef>(IORegistryEntryCreateCFProperty(registryEntry, CFSTR(kIOVideoStreamKey_StreamID), NULL, 0))).GetSInt32())
						continue;
					
					// Add the stream to the map of input streams the device tracks
					mInputStreams[streamID] = new IOBackedStream(this, registryEntry, streamDictionary, kCMIODevicePropertyScopeInput);
					
					// break out of the while loop since the stream was located
					break;
				}
			
			}
		}

		// Fetch the output streams list
		CACFArray outputStreamList(static_cast<CFArrayRef>(IORegistryEntryCreateCFProperty(mRegistryEntry, CFSTR(kIOVideoDeviceKey_OutputStreamList), NULL, 0)), true);

		// Iterate through the output streams, locate their registry entries, and add them to the mOutputStreams map
		streamCount = outputStreamList.GetNumberItems();
		for (UInt32 index = 0 ; index < streamCount ; ++index)
		{
			// Get the stream dictionary
			CFDictionaryRef streamDictionary = NULL;
			if (outputStreamList.GetDictionary(index, streamDictionary))
			{
				// Get the streamID from the the dictionary
				UInt32 streamID = CACFNumber(static_cast<CFNumberRef>(CFDictionaryGetValue(streamDictionary, CFSTR(kIOVideoStreamKey_StreamID))), false).GetSInt32();

				// Iterate over all the device's streams and find the registry entry for this stream
				IOKA::Iterator iterator(mRegistryEntry, kIOServicePlane);
				
				while (true)
				{
					IOKA::Object registryEntry(iterator.Next());
					if (not registryEntry.IsValid())
						break;
					
					// Make sure the registry entry conforms to an IOVideoStream
					if (not registryEntry.ConformsTo("IOVideoStream"))
						continue;
				
					// Make sure the streamIDs are the same
					if (streamID != CACFNumber(static_cast<CFNumberRef>(IORegistryEntryCreateCFProperty(registryEntry, CFSTR(kIOVideoStreamKey_StreamID), NULL, 0))).GetSInt32())
						continue;
					
					// Add the stream to the map of output streams the device tracks
					mOutputStreams[streamID] = new IOBackedStream(this, registryEntry, streamDictionary, kCMIODevicePropertyScopeOutput);
					
					// break out of the while loop since the stream was located
					break;
				}
			}
		}
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//  SetControl()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void IOBackedDevice::SetControl(Client client, UInt32 controlID, UInt32 value, UInt32* newValue)
	{
		// Make sure the request is for a known control
		ThrowIf(mControls.end() == mControls.find(controlID), CAException(kCMIOHardwareUnsupportedOperationError), "Device::SetControl: unknown control ID");

		// Make sure the device is allowed to be changed by this client
		ThrowIf(not ClientIsDeviceMasterOrIsFree(client), CAException(kCMIODevicePermissionsError), "Device::SetControl: client not allowed to set controls on device");

		// Grab the mutex for the Device's state
		CAMutex::Locker locker(mStateMutex);
		
		switch (controlID)
		{
			case KEXT::Sample::kDirectionControlID:
				{
					RemoveStreams();
					mIOVADevice.SetControl(controlID, value, newValue);
					DiscoverStreams();
				}
				break;
			
			case KEXT::Sample::kInputSourceSelectorControlID:
				{
					mIOVADevice.SetControl(controlID, value, newValue);
				}
				break;
				
			case CMIO::DP::Sample::kProtocolSelectorControlID:
				{
					if (mControlsList.IsValid())
					{
						CFMutableDictionaryRef theControlDictionary = CMIO::DP::ControlDictionary::GetControlByID(mControlsList, controlID);
						if (NULL != theControlDictionary)
						{
							UInt32	currentSelection;
							//	set the new value in the registry
							CACFDictionary theCAControlDictionary(theControlDictionary, false);
							currentSelection = CMIO::DP::ControlDictionary::GetSelectorControlValue(theCAControlDictionary);
							if (currentSelection != value)
							{
								CMIO::DP::ControlDictionary::SetSelectorControlValue(theCAControlDictionary, value);
								
								//	set the new value as the return value
								if (NULL != newValue)
								{
									*newValue = value;
								}
								mControls[controlID].mValueShadowTime = CAHostTimeBase::GetTheCurrentTime();
								mControls[controlID].mRangeShadowTime = CAHostTimeBase::GetTheCurrentTime();
							}
						}
					}
				}
				break;
		}
	}

	#pragma mark -
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// InitializeControls()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void IOBackedDevice::InitializeControls()
	{
		{
			// Get the kext control list from the IORegistry
			CACFArray controlList = CACFArray(static_cast<CFArrayRef>(IORegistryEntryCreateCFProperty(mRegistryEntry, CFSTR(kIOVideoDeviceKey_ControlList), NULL, 0)), true);

			// Don't do anything if the list is not valid
			if (not controlList.IsValid())
				return;

			// Use "now" as the shadow time for when the controls were last changed
			UInt64 shadowTime = CAHostTimeBase::GetTheCurrentTime();

			// Iterate over the controls
			UInt32 controlCount = controlList.GetNumberItems();
			for (UInt32 index = 0 ; index < controlCount ; ++index)
			{
				// Get the control dictionary:DP::ControlDictionary::S
				CFDictionaryRef controlDictionary = NULL;
				if (controlList.GetDictionary(index, controlDictionary))
				{
					// Extract the control ID
					UInt32 controlID = DP::IOV::ControlDictionary::GetControlID(controlDictionary);
					
					// Make an entry for it in the map of Controls
					mControls[controlID].mValueShadowTime = shadowTime;
					mControls[controlID].mRangeShadowTime = shadowTime;
				}
			}
		}
		
		//Create the plugin based controls
		{
			CACFArray			theSourceSelectorMap;
			CFDictionaryRef		theSelectorItem = NULL;
			CFDictionaryRef		theSelectorControl = NULL;
			theSourceSelectorMap = CACFArray(3, false);
			//		CFArray::withCapacity(3);
			if (!theSourceSelectorMap.IsValid())
			{
				DebugMessage("Device::InitializeControls: couldn't allocate the source selector map array");
			}
			
			theSelectorItem = CMIO::DP::ControlDictionary::CreateSelectorControlSelectorMapItem((UInt32)CMIO::DP::Sample::kProtocolSample, CACFString(CFSTR("Sample")));
			theSourceSelectorMap.AppendDictionary(theSelectorItem);
			theSelectorItem = CMIO::DP::ControlDictionary::CreateSelectorControlSelectorMapItem((UInt32)CMIO::DP::Sample::kProtocolSampleBasic, CACFString(CFSTR("Sample Basic")));
			theSourceSelectorMap.AppendDictionary(theSelectorItem);
			theSelectorItem = CMIO::DP::ControlDictionary::CreateSelectorControlSelectorMapItem((UInt32)CMIO::DP::Sample::kProtocolSampleAdvanced, CACFString(CFSTR("Sample Advanced")));
			theSourceSelectorMap.AppendDictionary(theSelectorItem);
			
			//	create a custom play through boolean control
			theSelectorControl = CMIO::DP::ControlDictionary::CreateSelectorControl(CMIO::DP::Sample::kProtocolSelectorControlID, kCMIOSelectorControlClassID, kCMIOSelectorControlClassID, kCMIOObjectPropertyScopeGlobal, kCMIOObjectPropertyElementMaster, 1, theSourceSelectorMap, CACFString(CFSTR("Protocol")), false, 0);
			if (NULL == theSelectorControl)
			{
				DebugMessage("Device::InitializeControls: couldn't allocate the theSelectorControl control");
			}
			
			mControlsList.AppendDictionary(theSelectorControl);
			
			// Use "now" as the shadow time for when the controls were last changed
			UInt64 shadowTime = CAHostTimeBase::GetTheCurrentTime();
			
			// Iterate over the controls
			UInt32 controlCount = mControlsList.GetNumberItems();
			for (UInt32 index = 0 ; index < controlCount ; ++index)
			{
				// Get the control dictionary
				CFDictionaryRef controlDictionary = NULL;
				if (mControlsList.GetDictionary(index, controlDictionary))
				{
					CFShow(controlDictionary);
					// Extract the control ID
					CACFDictionary caControlDictionary(controlDictionary, false);
					UInt32 controlID = CMIO::DP::ControlDictionary::GetControlID(caControlDictionary);
					
					// Make an entry for it in the map of Controls
					// Make an entry for it in the map of Controls
					mControls[controlID].mValueShadowTime = shadowTime;
					mControls[controlID].mRangeShadowTime = shadowTime;
				}
			}
		}		
	}
	
    #pragma mark -
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Sleep()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void IOBackedDevice::Sleep()
	{
		// This shoud stop all the streams that are currently running
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Wake()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void IOBackedDevice::Wake()
	{
		// This should start all the streams were stopped by sleep
	}


	#pragma mark -
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// IOVDeviceNotification()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void IOBackedDevice::IOVDeviceNotification(IOVideoDeviceRef /*deviceRef*/, IOBackedDevice& device, const IOVideoDeviceNotificationMessage& message)
	{
		// Grab the mutex for the device's state
		CAMutex::Locker locker(device.mStateMutex);
		
		// Assume this notification is not about a control change
		bool sendControlStatesChangedMessage = false;
		
		// Record the shadow time in the event a control as been altered
		UInt64 shadowTime = CAHostTimeBase::GetTheCurrentTime();

		#if Log_HardareNotifications
			DebugMessage("Device::IOVDeviceNotification: received %lu messages", message.mNumberNotifications);
		#endif
		
		// A message can contain many notifications, so iterate through them
		for (UInt32 notificationindex = 0 ; notificationindex < message.mNumberNotifications ; ++notificationindex)
		{
			#if Log_HardareNotifications
				char notificationIDString[] = CA4CCToCString(message.mNotifications[notificationindex].mNotificationID);
				DebugMessage("Device::IOVDeviceNotification: Handling notification %lu: Object ID: %lu Notification ID: '%s' (%lu, %lu, %qd, %qd)", notificationindex, message.mNotifications[notificationindex].mObjectID, notificationIDString, message.mNotifications[notificationindex].mNotificationArgument1, message.mNotifications[notificationindex].mNotificationArgument2, message.mNotifications[notificationindex].mNotificationArgument3, message.mNotifications[notificationindex].mNotificationArgument4);
			#endif
			
			// Figure out what object this notification is for
			if (0 == message.mNotifications[notificationindex].mObjectID)
			{
				#if Log_HardareNotifications
					DebugMessage("Device::IOVDeviceNotification: Notification %lu is a device notification", notificationIndex);
				#endif
				
				// This is a device level notification
				device.DeviceNotification(message.mNotifications[notificationindex]);
			}
			else
			{
				// Check to see if this object is a stream
				Stream* stream = device.GetStreamByStreamID(message.mNotifications[notificationindex].mObjectID);
				if (NULL != stream)
				{
					device.StreamNotification(message.mNotifications[notificationindex], *stream);
				}
				else
				{
					// Check to see if this object is a control
					Controls::const_iterator i = device.mControls.find(message.mNotifications[notificationindex].mObjectID);
					if (i != device.mControls.end())
					{
						device.ControlNotification(message.mNotifications[notificationindex], shadowTime);
						
						// Since some control was altered, remember that a "controls changed" message will have to be sent
						sendControlStatesChangedMessage = true;
					}
				}
			}
		}
		
		if (sendControlStatesChangedMessage)
			device.SendControlStatesChangedMessage();
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// DeviceNotification()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void IOBackedDevice::DeviceNotification(const IOVideoDeviceNotification& notification)
	{
		switch (notification.mNotificationID)
		{
			default:
				DebugMessage("Device::HandleDeviceNotification: unusual notification type");
				break;
		};
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// StreamNotification()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void IOBackedDevice::StreamNotification(const IOVideoDeviceNotification& /*notification*/, Stream& /*stream*/)
	{
	}
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// ControlNotification()
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void IOBackedDevice::ControlNotification(const IOVideoDeviceNotification& notification, UInt64 shadowTime)
	{
		switch (notification.mNotificationID)
		{
			case kIOVideoDeviceNotificationID_ControlValueChanged:
				mControls[notification.mObjectID].mValueShadowTime = shadowTime;
				break;
			
			case kIOVideoDeviceNotificationID_ControlRangeChanged:
				mControls[notification.mObjectID].mRangeShadowTime = shadowTime;
				break;
						
			default:
				DebugMessage("Device::ControlNotification: unusual notification");
				break;
		};
	}
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// PowerNotification()
	//	Which notifications are handled (and how they are handled) can vary based on the device.
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void IOBackedDevice::PowerNotification(IOBackedDevice& device, io_service_t /*unused*/, natural_t messageType, void* message)
	{
		// Catch all exceptions since this is invoked via a call back and the exception cannot leave this routine 
		try
		{
			switch (messageType)
			{
				case kIOMessageCanSystemSleep:
					(void) IOAllowPowerChange(device.mPowerNotificationPort.GetRootPowerDomain(), reinterpret_cast<long>(message));
					break;
					
				case kIOMessageSystemWillSleep:
					device.Sleep();
					(void) IOAllowPowerChange(device.mPowerNotificationPort.GetRootPowerDomain(), reinterpret_cast<long>(message));
					break;
					
				case kIOMessageSystemHasPoweredOn:
					device.Wake();
					break;
					
				default:
					break;
			}
		}
		catch (...)
		{
		}
	}
}}}}
