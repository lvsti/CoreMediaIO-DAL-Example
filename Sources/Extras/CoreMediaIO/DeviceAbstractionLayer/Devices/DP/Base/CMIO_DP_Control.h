/*
	    File: CMIO_DP_Control.h
	Abstract: n/a
	 Version: 1.2

*/

#if !defined(__CMIO_DP_Control_h__)
#define __CMIO_DP_Control_h__

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Super Class Includes
#include "CMIO_DP_Object.h"

// Internal Includes
#include "CMIO_DP_Property_Base.h"

namespace CMIO { namespace DP
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Types in the CMIO::DP namespace
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class Device;

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Control
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class Control : public Object
	{
	// Construction/Destruction
	public:
											Control(CMIOObjectID objectID, CMIOClassID classID, PlugIn& plugIn, Device& owningDevice);
		virtual								~Control();

	// Attributes
	public:
		virtual CMIOClassID					GetBaseClassID() const;
		virtual CAMutex*					GetObjectStateMutex();
		virtual void						Show() const;
		bool								GetMark() const { return mMark; }
		void								SetMark(bool mark) { mMark = mark; }
		virtual CFStringRef					CopyName() const;
		virtual CFStringRef					CopyManufacturerName() const;
		virtual CMIOObjectPropertyScope		GetPropertyScope() const = 0;
		virtual CMIOObjectPropertyElement	GetPropertyElement() const = 0;
		virtual void*						GetImplementationObject() const;
		virtual UInt32						GetVariant() const;
		virtual bool						IsReadOnly() const;
		
	// Property Access
	public:
		virtual bool						HasProperty(const CMIOObjectPropertyAddress& address) const;
		virtual bool						IsPropertySettable(const CMIOObjectPropertyAddress& address) const;
		virtual UInt32						GetPropertyDataSize(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData) const;
		virtual void						GetPropertyData(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData, UInt32 dataSize, UInt32& dataUsed, void* data) const;
		virtual void						SetPropertyData(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData, UInt32 dataSize, const void* data);

	// Implementation
	protected:
		Device&								mOwningDevice;
		bool								mMark;
	};

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// BooleanControl
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class BooleanControl : public Control
	{
	// Construction/Destruction
	public:
							BooleanControl(CMIOObjectID objectID, CMIOClassID classID, PlugIn& plugIn, Device& owningDevice);
		virtual				~BooleanControl();

	// Attributes
	public:
		virtual CMIOClassID	GetBaseClassID() const;
		virtual bool		GetValue() const = 0;
		virtual void		SetValue(bool value) = 0;

	// Property Access
	public:
		virtual bool		HasProperty(const CMIOObjectPropertyAddress& address) const;
		virtual bool		IsPropertySettable(const CMIOObjectPropertyAddress& address) const;
		virtual UInt32		GetPropertyDataSize(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData) const;
		virtual void		GetPropertyData(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData, UInt32 dataSize, UInt32& dataUsed, void* data) const;
		virtual void		SetPropertyData(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData, UInt32 dataSize, const void* data);
 
 		virtual void		ValueChanged() const;
	};

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// SelectorControl
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class SelectorControl : public Control
	{
	// Construction/Destruction
	public:
							SelectorControl(CMIOObjectID objectID, CMIOClassID classID, PlugIn& plugIn, Device& owningDevice);
		virtual				~SelectorControl();

	// Attributes
	public:
		virtual CMIOClassID	GetBaseClassID() const;
		virtual UInt32		GetNumberItems() const = 0;

		virtual UInt32		GetCurrentItemID() const = 0;
		virtual UInt32		GetCurrentItemIndex() const = 0;
		
		virtual void		SetCurrentItemByID(UInt32 itemID) = 0;
		virtual void		SetCurrentItemByIndex(UInt32 itemIndex) = 0;
		
		virtual UInt32		GetItemIDForIndex(UInt32 itemIndex) const = 0;
		virtual UInt32		GetItemIndexForID(UInt32 itemID) const = 0;
		
		virtual CFStringRef	CopyItemNameByID(UInt32 itemID) const = 0;
		virtual CFStringRef	CopyItemNameByIndex(UInt32 itemIndex) const = 0;
		
		virtual CFStringRef	CopyItemNameByIDWithoutLocalizing(UInt32 itemID) const = 0;
		virtual CFStringRef	CopyItemNameByIndexWithoutLocalizing(UInt32 itemIndex) const = 0;
		
		virtual UInt32		GetItemKindByID(UInt32 itemID) const = 0;
		virtual UInt32		GetItemKindByIndex(UInt32 itemIndex) const = 0;

		virtual void		CurrentItemChanged() const;
		virtual void		AvailableItemsChanged() const;
		virtual void		ItemNameChanged() const;

	// Property Access
	public:
		virtual bool		HasProperty(const CMIOObjectPropertyAddress& address) const;
		virtual bool		IsPropertySettable(const CMIOObjectPropertyAddress& address) const;
		virtual UInt32		GetPropertyDataSize(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData) const;
		virtual void		GetPropertyData(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData, UInt32 dataSize, UInt32& dataUsed, void* data) const;
		virtual void		SetPropertyData(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData, UInt32 dataSize, const void* data);
	};

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// FeatureControl
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class FeatureControl : public Control
	{
	// Construction/Destruction
	public:
							FeatureControl(CMIOObjectID objectID, CMIOClassID classID, PlugIn& plugIn, Device& owningDevice);
		virtual				~FeatureControl();

	// Attributes
	public:
		virtual CMIOClassID	GetBaseClassID() const;
		virtual CFStringRef	CopyName() const;

		virtual bool		OnOffSettable() const = 0;
		virtual UInt32		GetOnOff() const = 0;
		virtual void		SetOnOff(UInt32 onOff) = 0;
		
		virtual bool		AutomaticManualSettable() const = 0;
		virtual UInt32		GetAutomaticManual() const = 0;
		virtual void		SetAutomaticManual(UInt32 automaticManual) = 0;
		
		virtual bool		AbsoluteNativeSettable() const = 0;
		virtual UInt32		GetAbsoluteNative() const = 0;
		virtual void		SetAbsoluteNative(UInt32 nativeAbsolute) = 0;
		
		virtual bool		HasTune() const = 0;
		virtual UInt32		GetTune() const = 0;
		virtual void		SetTune(UInt32 tune) = 0;

		virtual Float32		GetNativeValue() const = 0;
		virtual void		SetNativeValue(Float32 nativeValue) = 0;
		virtual Float32		GetMinimumNativeValue() const = 0;
		virtual Float32		GetMaximumNativeValue() const = 0;

		virtual Float32		GetAbsoluteValue() const = 0;
		virtual void		SetAbsoluteValue(Float32 absoluteValue) = 0;
		virtual Float32		GetMinimumAbsoluteValue() const = 0;
		virtual Float32		GetMaximumAbsoluteValue() const = 0;

		virtual bool		HasNativeToAbsolute() const = 0;
		virtual Float32		ConverNativeValueToAbsoluteValue(Float32 nativeValue) const = 0;
		
		virtual bool		HasAbsoluteToNative() const = 0;
		virtual Float32		ConverAbsoluteValueToNativeValue(Float32 absoluteValue) const = 0;
		
		virtual CFStringRef	CopyAbsoluteUnitName() const;

	// Property Access
	public:
		virtual bool		HasProperty(const CMIOObjectPropertyAddress& address) const;
		virtual bool		IsPropertySettable(const CMIOObjectPropertyAddress& address) const;
		virtual UInt32		GetPropertyDataSize(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData) const;
		virtual void		GetPropertyData(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData, UInt32 dataSize, UInt32& dataUsed, void* data) const;
		virtual void		SetPropertyData(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData, UInt32 dataSize, const void* data);
	};
}}
#endif
