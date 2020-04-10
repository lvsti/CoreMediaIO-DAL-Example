/*
     File: CAAudioFileFormats.h
 Abstract: Part of CoreAudio Utility Classes
  Version: 1.1
*/
#ifndef __CAAudioFileFormats_h__
#define __CAAudioFileFormats_h__

#if !defined(__COREAUDIO_USE_FLAT_INCLUDES__)
	#include <AudioToolbox/AudioToolbox.h>
#else
	#include <AudioToolbox.h>
#endif
#include "CAStreamBasicDescription.h"

class CAAudioFileFormats {
public:
	enum { noErr = 0 };

	struct DataFormatInfo {
		DataFormatInfo() : mVariants(NULL) { }
		~DataFormatInfo() { delete[] mVariants; }
		
		UInt32							mFormatID;
		int								mNumVariants;
		AudioStreamBasicDescription *	mVariants;
		bool							mReadable;
		bool							mWritable;
		bool							mEitherEndianPCM;

#if DEBUG
		void	DebugPrint();
#endif
	};
	
	struct FileFormatInfo {
		FileFormatInfo() : mFileTypeName(NULL), mExtensions(NULL), mDataFormats(NULL) { }
		~FileFormatInfo() {
			delete[] mDataFormats;
			if (mFileTypeName)
				CFRelease(mFileTypeName);
			if (mExtensions)
				CFRelease(mExtensions);
		}
		
		AudioFileTypeID					mFileTypeID;
		CFStringRef						mFileTypeName;
		CFArrayRef						mExtensions;
		int								mNumDataFormats;
		DataFormatInfo *				mDataFormats;		// NULL until loaded!
		
		int		NumberOfExtensions() { return static_cast<int>(mExtensions ? CFArrayGetCount(mExtensions) : 0); }
		char *	GetExtension(int index, char *buf, int buflen) {
					CFStringRef cfext = (CFStringRef)CFArrayGetValueAtIndex(mExtensions, index);
					CFStringGetCString(cfext, buf, buflen, kCFStringEncodingUTF8);
					return buf;
				}
		bool	MatchExtension(CFStringRef testExt) {	// testExt should not include "."
					CFIndex n = NumberOfExtensions();
					for (CFIndex i = 0; i < n; ++i) {
						CFStringRef ext = (CFStringRef)CFArrayGetValueAtIndex(mExtensions, i);
						if (CFStringCompare(ext, testExt, kCFCompareCaseInsensitive) == kCFCompareEqualTo)
							return true;
					}
					return false;
				}
		bool	AnyWritableFormats();
		void	LoadDataFormats();
		
#if DEBUG
		void	DebugPrint();
#endif
	};
	
private:	// use Instance()
	CAAudioFileFormats(bool loadDataFormats);
	~CAAudioFileFormats();
public:
	
	bool	InferDataFormatFromFileFormat(AudioFileTypeID filetype, CAStreamBasicDescription &fmt);
	
	bool	InferFileFormatFromFilename(const char *filename, AudioFileTypeID &filetype);

	bool	InferFileFormatFromFilename(CFStringRef filename, AudioFileTypeID &filetype);

	bool	InferFileFormatFromDataFormat(const CAStreamBasicDescription &fmt, AudioFileTypeID &filetype);

	bool	IsKnownDataFormat(UInt32 dataFormat);
	
#if DEBUG
	void	DebugPrint();
#endif

	int					mNumFileFormats;
	FileFormatInfo	*	mFileFormats;
	
	FileFormatInfo *	FindFileFormat(UInt32 formatID);

	static CAAudioFileFormats *	Instance(bool loadDataFormats=true);

private:	
	static CAAudioFileFormats *	sInstance;
};

char *	OSTypeToStr(char *buf, size_t bufsize, UInt32 t);
int		StrToOSType(const char *str, UInt32 &t);

#endif // __CAAudioFileFormats_h__
