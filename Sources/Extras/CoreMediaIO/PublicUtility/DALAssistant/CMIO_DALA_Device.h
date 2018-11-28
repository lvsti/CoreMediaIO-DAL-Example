/*
	    File: CMIO_DALA_Device.h
	Abstract: C++ wrapper for CMIODeviceID
	 Version: 1.2
	
	Disclaimer: IMPORTANT:  This Apple software is supplied to you by Apple
	Inc. ("Apple") in consideration of your agreement to the following
	terms, and your use, installation, modification or redistribution of
	this Apple software constitutes acceptance of these terms.  If you do
	not agree with these terms, please do not use, install, modify or
	redistribute this Apple software.
	
	In consideration of your agreement to abide by the following terms, and
	subject to these terms, Apple grants you a personal, non-exclusive
	license, under Apple's copyrights in this original Apple software (the
	"Apple Software"), to use, reproduce, modify and redistribute the Apple
	Software, with or without modifications, in source and/or binary forms;
	provided that if you redistribute the Apple Software in its entirety and
	without modifications, you must retain this notice and the following
	text and disclaimers in all such redistributions of the Apple Software.
	Neither the name, trademarks, service marks or logos of Apple Inc. may
	be used to endorse or promote products derived from the Apple Software
	without specific prior written permission from Apple.  Except as
	expressly stated in this notice, no other rights or licenses, express or
	implied, are granted by Apple herein, including but not limited to any
	patent rights that may be infringed by your derivative works or by other
	works in which the Apple Software may be incorporated.
	
	The Apple Software is provided by Apple on an "AS IS" basis.  APPLE
	MAKES NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION
	THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS
	FOR A PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND
	OPERATION ALONE OR IN COMBINATION WITH YOUR PRODUCTS.
	
	IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL
	OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
	SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
	INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION,
	MODIFICATION AND/OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED
	AND WHETHER UNDER THEORY OF CONTRACT, TORT (INCLUDING NEGLIGENCE),
	STRICT LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN ADVISED OF THE
	POSSIBILITY OF SUCH DAMAGE.
	
	Copyright (C) 2012 Apple Inc. All Rights Reserved.
	
*/

#if !defined(__CMIO_DALA_Device_h__)
#define __CMIO_DALA_Device_h__

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Public Utility Includes
#include "CMIO_DALA_Object.h"

// System Includes
#include <CoreMedia/CMFormatDescription.h>
#include <CoreMediaIO/CMIOHardwareDevice.h>
#include <sys/types.h>

namespace CMIO { namespace DALA
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Device
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class Device : public Object
	{
	// Construction/Destruction
	public:
								Device(CMIODeviceID cmioDeviceID);
		virtual					~Device();

	// General Operations
	public:
		CMIODeviceID			GetCMIODeviceID() const { return GetObjectID(); }
		CFStringRef				CopyUID() const;
		bool					HasModelUID() const;
		CFStringRef				CopyModelUID() const;
		UInt32					GetTransportType() const;
		bool					CanBeDefaultDevice(CMIOObjectPropertyScope scope) const;
		
		bool					HasDevicePlugInStatus() const;
		OSStatus				GetDevicePlugInStatus() const;
		
		bool					IsAlive() const;
		bool					IsRunning() const;
		void					SetIsRunning(bool isRunning);
		bool					IsRunningSomewhere() const;
		
		bool					HogModeIsSettable() const;
		pid_t					GetHogModeOwner() const;
		bool					TakeHogMode();
		void					ReleaseHogMode();
		
		pid_t					GetDeviceMasterOwner() const;
		bool					TakeDeviceMaster();
		void					ReleaseDeviceMaster();

		bool					HasExcludeNonDALAccess() const;
		bool					GetExcludeNonDALAccess() const;
		void					SetExcludeNonDALAccess(bool exclude);
		
		bool					HasVideoDigitizerComponents() const;
		UInt32					GetNumberVideoDigitizerComponents() const;
		void					GetVideoDigitizerComponents(UInt32& ioNumberVideoDigitizerComponents, ComponentDescription* componentDescriptions) const;
		
		bool					CanProcessAVCCommands() const;
		bool					CanProcessRS422Commands() const;
		
		bool					HasSuspendedByUser() const;
		bool					SuspendedByUser() const;
		
	// IO Operations
	public:
		UInt32					GetLatency(CMIOObjectPropertyScope scope) const;
		
	// Stream Operations
	public:
		UInt32					GetNumberStreams(CMIOObjectPropertyScope scope) const;
		void					GetStreams(CMIOObjectPropertyScope scope, UInt32& ioNumberStreams, CMIOStreamID* streamList) const;
		CMIOStreamID			GetStreamByIndex(CMIOObjectPropertyScope scope, UInt32 index) const;
		
		bool					HasSection(CMIOObjectPropertyScope scope) const { return GetNumberStreams(scope) > 0; }
		UInt32					GetTotalNumberChannels(CMIOObjectPropertyScope scope) const;

		void					StartStream(CMIOStreamID streamID);
		void					StopStream(CMIOStreamID streamID);

	// Format Operations
	public:
		void					GetCurrentFormats(CMIOObjectPropertyScope scope, UInt32& ioNumberStreams, CMFormatDescriptionRef* descriptions) const;
		
	// Utility Operations
	public:
		static void				GetNameForTransportType(UInt32 transportType, char* name, UInt32 maximumNameLength);
	};
}}

#endif
