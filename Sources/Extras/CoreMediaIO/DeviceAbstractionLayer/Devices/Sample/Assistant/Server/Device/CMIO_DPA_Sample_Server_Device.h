/*
	    File: CMIO_DPA_Sample_Server_Device.h
	Abstract: n/a
	 Version: 1.2

*/

#if !defined(__CMIO_DPA_Sample_Server_Device_h__)
#define __CMIO_DPA_Sample_Server_Device_h__

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Internal Includes
#include "CMIO_DPA_Sample_Server_Common.h"
#include "CMIO_DPA_Sample_Server_IOBackedStream.h"
#include "CMIO_DPA_Sample_Shared.h"

// CA Public Utility Includes
#include "CACFArray.h"
#include "CACFDictionary.h"
#include "CAGuard.h"

// System Includes
#include <CoreMedia/CMTime.h>
#include <CoreMediaIO/CMIOSampleBuffer.h>

// Standard Library Includes
#include <map>

namespace CMIO { namespace DPA { namespace Sample { namespace Server
{
	class Device
	{
	public:
		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// Device::GUIDEqual
		//	A unary predicate object which reports if the Device's GUID equals the specified GUID.
		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		class GUIDEqual
		{
		public:
					GUIDEqual(UInt64 guid) : mGUID(guid) {};
			bool	operator()(Device* device) const { return device->GetDeviceGUID() == mGUID; }
		
		private:
					UInt64 mGUID;
		};

	public:
		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// Device::StreamIsActive
		//	A unary predicate object which reports true if the FormatPair.second.first equals the specified CMFormatDescriptionRef.
		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		class StreamIsActive
		{
		public:
					StreamIsActive()  {};
			bool	operator()(const std::pair<UInt32, Stream*>& pair) const { return pair.second->Streaming(); }
		};

	#pragma mark -
	#pragma mark Device
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Device
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Construction/Destruction
	public:
										Device();
		virtual							~Device();
	
	private:
		Device&							operator=(Device& that); // Don't allow copying
		
	protected:
		CAMutex							mStateMutex;				// Controls access to device's state

	// Device 'Guaranteed" Unique ID
	public:
		UInt64							GetDeviceGUID() const { return mDeviceGUID; }

 	protected:
		static UInt64					mGUIDGenerator;				// A static member that is incremented in for each Device constructed to provide mDeviceGUID
		UInt64							mDeviceGUID;				// A psuedo-GUID to act as the suffix for supplying the kCMIODevicePropertyDeviceUID

	// Client Management
	public:
		void							ClientDied(Client client);

	// Stream Management
	public:
		void							StartStream(Client client = MACH_PORT_NULL, mach_port_t messagePort = MACH_PORT_NULL, CMIOObjectPropertyScope scope = kCMIOObjectPropertyScopeWildcard, CMIOObjectPropertyElement element = kCMIOObjectPropertyElementWildcard, UInt32 initialDiscontinuityFlags = kCMIOSampleBufferNoDiscontinuities);
		void							StopStream(Client client, CMIOObjectPropertyScope scope, CMIOObjectPropertyElement element);
	
	protected:
		typedef std::map<UInt32, Stream*> StreamMap;
		typedef std::map<Client, ClientStream*> ClientStreamMap;
		
		void							RemoveStreams();
		Stream&							GetStreamByScopeAndElement(CMIOObjectPropertyScope scope, CMIOObjectPropertyElement element);		
		Stream*							GetStreamByStreamID(UInt32 streamID);		
		bool							AnyStreamRunning() const;
		
		StreamMap						mInputStreams;
		StreamMap						mOutputStreams;
		
	// Deck
	public:
		void							StartDeckThreads(Client client, CMIOObjectPropertyScope scope, CMIOObjectPropertyElement element);
		void							StopDeckThreads(Client client, CMIOObjectPropertyScope scope, CMIOObjectPropertyElement element);
		void							DeckPlay(Client client, CMIOObjectPropertyScope scope, CMIOObjectPropertyElement element);
		void							DeckStop(Client client, CMIOObjectPropertyScope scope, CMIOObjectPropertyElement element);
		void							DeckJog(Client client, CMIOObjectPropertyScope scope, CMIOObjectPropertyElement element, SInt32 speed);
		void							DeckCueTo(Client client, CMIOObjectPropertyScope scope, CMIOObjectPropertyElement element, Float64 requestedTimecode, Boolean playOnCue);

	// Capabilities
	public:
		bool							CapabilitiesDiscovered() const { return mCapabilitiesDiscovered; }
		void							DiscoverCapabilities();

	protected:
		bool							mCapabilitiesDiscovered;

		typedef std::multimap<Client, mach_port_t> ClientNotifiers;					// Clients Mach ports to message in response to various events

	// Properties
	public:
		void							GetProperties(Client client, mach_port_t messagePort, UInt64 time, const PropertyAddress& matchAddress, PropertyAddress** addresses, mach_msg_type_number_t* length);
		void							GetPropertyState(const PropertyAddress& address, UInt8* qualifier, mach_msg_type_number_t qualifierLength, UInt8** data, mach_msg_type_number_t* length);
		void							SetPropertyState(Client client, bool sendChangedNotifications, const PropertyAddress& address, UInt8* qualifier, mach_msg_type_number_t qualifierLength, Byte* data, mach_msg_type_number_t length);
		void							SendPropertyStatesChangedMessage();

		void							GetControlList(UInt8** data, mach_msg_type_number_t* length);

	protected:
		virtual void					InitializeProperties();
		bool							ClientIsDeviceMasterOrIsFree(Client client) const { return (-1 == mDeviceMaster or client == mDeviceMasterClient); }
		
	private:
		Properties						mProperties;
		ClientNotifiers					mPropertyStateNotifiers;
		CAMutex							mPropertyStateNotifiersMutex;

		pid_t							mHogModeOwner;						// -1 unless being accessed through means other than the DAL (e.g., a digitizer)
		pid_t							mDeviceMaster;						// Process which owns the mastership of the device (or -1 if free)
		Client							mDeviceMasterClient;				// Client which owns the mastership of the device (or MACH_PORT_NULL if free)
		bool							mExcludeNonDALAccess;				// If 1, grab exclusive access to the device to prevent non-DAL access (e.g., a QuickTime Video Digitizer)
		Client							mExcludeNonDALAccessClient;			// Client which set mExcludeNonDALAccess to 1 (MACH_PORT_NULL if currently 0) 
		bool							mForceDiscontinuity;				// True if instructed to force a discontinuity (kCMIODevicePropertyClientSyncDiscontinuity)

	// Controls
	public:
		void							GetControls(Client client, mach_port_t messagePort, UInt64 time, ControlChanges** controlChanges, mach_msg_type_number_t* length);
		virtual void					SetControl(Client client, UInt32 controlID, UInt32 value, UInt32* newValue);
		
		kern_return_t					RS422Command(const UInt8 *command, UInt32 commandLength, UInt8 *response, UInt32 *responseLength);

	protected:
		void							SendControlStatesChangedMessage();

		CACFArray						mControlsList;
		Controls						mControls;
		ClientNotifiers					mControlStateNotifiers;				// Clients to notify when controls changed
		CAMutex							mControlStateNotifiersMutex;
	};
}}}}
#endif
