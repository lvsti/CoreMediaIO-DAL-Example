/*
     File: CATokenMap.h
 Abstract: Part of CoreAudio Utility Classes
  Version: 1.1

*/
#if !defined(__CATokenMap_h__)
#define __CATokenMap_h__

//=============================================================================
//	Includes
//=============================================================================

//  System Includes
#if !defined(__COREAUDIO_USE_FLAT_INCLUDES__)
	#include <CoreAudio/CoreAudioTypes.h>
#else
	#include <CoreAudioTypes.h>
#endif

//  PublicUtility Includes
#include "CAMutex.h"

//  Standard Library Includes
#include <map>
#include <iterator>

//=============================================================================
//	CATokenMap
//=============================================================================

template <class T>
class	CATokenMap
{

//	Types
private:
	typedef std::map<UInt32, T*>	TokenMap;

//	Construction/Destruction
public:
			CATokenMap() : mTokenMap(), mNextToken(256), mTokenMapMutex("CATokenMap Mutex") {}
			~CATokenMap() {}

//	Operations
public:
	bool	Lock()
	{
		return mTokenMapMutex.Lock();
	}
	
	void	Unlock()
	{
		mTokenMapMutex.Unlock();
	}

	UInt32	GetToken(T* inObject) const
	{
		CAMutex::Locker theLocker(const_cast<CAMutex&>(mTokenMapMutex));
		UInt32 theAnswer = 0;
		typename TokenMap::const_iterator i = mTokenMap.begin();
		while((theAnswer == 0) && (i != mTokenMap.end()))
		{
			if(i->second == inObject)
			{
				theAnswer = i->first;
			}
			std::advance(i, 1);
		}
		return theAnswer;
	}
	
	T*		GetObject(UInt32 inToken) const
	{
		CAMutex::Locker theLocker(const_cast<CAMutex&>(mTokenMapMutex));
		typename TokenMap::const_iterator i = mTokenMap.find(inToken);
		return i != mTokenMap.end() ? i->second : NULL;
	}

	T*		GetObject(const void* inToken) const
	{
		#if __LP64__
			return GetObject((UInt32)((UInt64)inToken));
		#else
			return GetObject((UInt32)inToken);
		#endif
	}
	
	UInt32	GetNumberObjects() const
	{
		CAMutex::Locker theLocker(const_cast<CAMutex&>(mTokenMapMutex));
		return static_cast<UInt32>(mTokenMap.size());
	}
	
	T*		GetObjectByIndex(UInt32 inIndex) const
	{
		T* theAnswer = NULL;
		CAMutex::Locker theLocker(const_cast<CAMutex&>(mTokenMapMutex));
		if(inIndex < mTokenMap.size())
		{
			typename TokenMap::const_iterator i = mTokenMap.begin();
			std::advance(i, inIndex);
			theAnswer = (i != mTokenMap.end()) ? i->second : NULL;
		}
		return theAnswer;
	}
	
	void	AddMapping(UInt32 inToken, T* inObject)
	{
		CAMutex::Locker theLocker(mTokenMapMutex);
		typename TokenMap::iterator i = mTokenMap.find(inToken);
		if(i != mTokenMap.end())
		{
			i->second = inObject;
		}
		else
		{
			mTokenMap.insert(typename TokenMap::value_type(inToken, inObject));
		}
	}
	
	void	RemoveMapping(UInt32 inToken, T* /*inObject*/)
	{
		CAMutex::Locker theLocker(mTokenMapMutex);
		typename TokenMap::iterator i = mTokenMap.find(inToken);
		if(i != mTokenMap.end())
		{
			mTokenMap.erase(i);
		}
	}
	
	UInt32	GetNextToken()
	{
		return mNextToken++;
	}
	
	UInt32	MapObject(T* inObject)
	{
		CAMutex::Locker theLocker(mTokenMapMutex);
		UInt32 theToken = GetNextToken();
		mTokenMap.insert(typename TokenMap::value_type(theToken, inObject));
		return theToken;
	}
	
	void	UnmapObject(T* inObject)
	{
		CAMutex::Locker theLocker(mTokenMapMutex);
		bool isDone = false;
		typename TokenMap::iterator i = mTokenMap.begin();
		while(!isDone && (i != mTokenMap.end()))
		{
			if(i->second == inObject)
			{
				mTokenMap.erase(i);
				isDone = true;
			}
			else
			{
				std::advance(i, 1);
			}
		}
	}

//	Implementation
private:	
	TokenMap	mTokenMap;
	UInt32		mNextToken;
	CAMutex		mTokenMapMutex;

};

#endif
