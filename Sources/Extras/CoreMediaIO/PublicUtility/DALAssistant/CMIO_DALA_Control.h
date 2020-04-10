/*
	    File: CMIO_DALA_Control.h
	Abstract: C++ wrapper for CMIOControlID
	 Version: 1.2

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
