/*
	    File: CMIO_DALA_Control.h
	Abstract: C++ wrapper for CMIOControlID
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

#if !defined(__CMIO_DALA_Control_h__)
#define __CMIO_DALA_Control_h__

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Public Utility Includes
#include "CMIO_DALA_Object.h"

// System Includes
#include <CoreAudio/CoreAudioTypes.h>
#include <sys/types.h>

namespace CMIO { namespace DALA
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Control
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class Control : public Object
	{
	// Construction/Destruction
	public:
									Control(CMIOObjectID cmioObjectID);
		virtual						~Control();

	// General Operations
	public:
		CMIOObjectPropertyScope	GetPropertyScope() const;
		CMIOObjectPropertyElement	GetPropertyElement() const;
		UInt32						GetVariant() const;
		
	};

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// BooleanControl
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class BooleanControl : public Control
	{
	// Construction/Destruction
	public:
						BooleanControl(CMIOObjectID objectID);
		virtual			~BooleanControl();

	// Attributes
	public:
		bool			GetValue() const;
		void			SetValue(bool value);
	};

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// FeatureControl
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class SelectorControl : public Control
	{
	// Construction/Destruction
	public:
						SelectorControl(CMIOObjectID objectID);
		virtual			~SelectorControl();

	// Attributes
	public:
		UInt32		GetNumberItems() const;

		UInt32		GetCurrentItemID() const ;
		UInt32		GetCurrentItemIndex() const;
	
		void		SetCurrentItemByID(UInt32 inItemID);
		void		SetCurrentItemByIndex(UInt32 inItemIndex);
	
		UInt32		GetItemIDForIndex(UInt32 inItemIndex) const;
		UInt32		GetItemIndexForID(UInt32 inItemID) const;
	
		CFStringRef	CopyItemNameByID(UInt32 inItemID) const;
		CFStringRef	CopyItemNameByIndex(UInt32 inItemIndex) const;
	
		CFStringRef	CopyItemNameByIDWithoutLocalizing(UInt32 inItemID) const;
		CFStringRef	CopyItemNameByIndexWithoutLocalizing(UInt32 inItemIndex) const;
	};

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// FeatureControl
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class FeatureControl : public Control
	{
	// Construction/Destruction
	public:
						FeatureControl(CMIOObjectID objectID);
		virtual			~FeatureControl();

	// Attributes
	public:
		bool			OnOffSettable() const;
		bool			GetOnOff() const;
		void			SetOnOff(bool onOff);
		
		bool			AutomaticManualSettable() const;
		bool			GetAutomaticManual() const;
		void			SetAutomaticManual(bool automaticManual);
		
		bool			AbsoluteNativeSettable() const;
		bool			GetAbsoluteNative() const;
		void			SetAbsoluteNative(bool nativeAbsolute);
		
		bool			HasTune() const;
		bool			GetTune() const;
		void			SetTune(bool tune);
		
		bool			HasNativeValue() const;
		bool			NativeValueSettable() const;
		Float32			GetNativeValue() const;
		void			SetNativeValue(Float32 nativeValue);
		AudioValueRange	GetNativeValueRange() const;

		bool			HasAbsoluteValue() const;
		bool			AbsoluteValueSettable() const;
		Float32			GetAbsoluteValue() const;
		void			SetAbsoluteValue(Float32 absoluteValue);
		AudioValueRange	GetAbsoluteValueRange() const;

		bool			HasNativeToAbsolute() const;
		Float32			ConverNativeValueToAbsoluteValue(Float32 nativeValue) const;

		bool			HasAbsoluteToNative() const;
		Float32			ConverAbsoluteValueToNativeValue(Float32 absoluteValue) const;
		
		bool			HasAbsoluteUnitName() const;
		CFStringRef		CopyAbsoluteUnitName() const;
	};
}}

#endif
