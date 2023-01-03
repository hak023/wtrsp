#ifndef TARGET_CONTENT_H
#define TARGET_CONTENT_H
#include "StlMap.hxx"
#include "AppIf.h"

using namespace eSipUtil;

typedef enum
{
	E_TC_FFMPEG = 0,
	E_TC_MCONV,
	E_TC_VOX
} ETcType_t;
inline const char * g_fnGetTranscodingType(ETcType_t _eTcType)
{
	if(_eTcType == E_TC_FFMPEG) return "FCONV";
	else if(_eTcType == E_TC_MCONV) return "MCONV";
	else if(_eTcType == E_TC_VOX) return "VOX";
	else return "UNKNOWN";
}

class AudioCodec
{
public:
	AudioCodec()
	{
	}
	~AudioCodec()
	{
	}
	KString m_clsBitPerSample;
	KString m_clsBitRate;
	KString m_clsChannel;
	KString m_clsID;
	KString m_clsSampleRate;

	char * m_fnDebug(eSipUtil::KString & _rclsDebug)
	{
		_rclsDebug << "[AudioCodec]\n";
		_rclsDebug << "BitPerSample=" << m_clsBitPerSample << ", BitRate="
				<< m_clsBitRate << ", Channel=" << m_clsChannel << ", ID="
				<< m_clsID << ", SampleRate=" << m_clsSampleRate << "\n";
		return (KSTR) _rclsDebug;
	}
};

class VideoCodec
{
public:
	VideoCodec()
	{
	}
	~VideoCodec()
	{
	}
	KString m_clsBitRate;
	KString m_clsHeight;
	KString m_clsID;
	KString m_clsMaxFrameRate;
	KString m_clsWidth;

	char * m_fnDebug(eSipUtil::KString & _rclsDebug)
	{
		_rclsDebug << "[VideoCodec]\n";
		_rclsDebug << "BitRate=" << m_clsBitRate << ", Height=" << m_clsHeight
				<< ", ID=" << m_clsID << ", MaxFrameRate=" << m_clsMaxFrameRate
				<< ", Width=" << m_clsWidth << "\n";
		return (KSTR) _rclsDebug;
	}
};

class Container
{
public:
	Container()
	{
	}
	~Container()
	{
	}
	KString m_clsID;
	AudioCodec clsAudioCodec;
	VideoCodec clsVideoCodec;

	char * m_fnDebug(eSipUtil::KString & _rclsDebug)
	{
		_rclsDebug << "[Container]\n";
		_rclsDebug << "ID=" << m_clsID << "\n";
		clsAudioCodec.m_fnDebug(_rclsDebug);
		clsVideoCodec.m_fnDebug(_rclsDebug);
		return (KSTR) _rclsDebug;
	}
};

class TargetContent: public eSipUtil::StlObject
{
public:
	TargetContent()
	{
	}
	~TargetContent()
	{
	}
	char * m_fnDebug(eSipUtil::KString & _rclsDebug)
	{
		_rclsDebug << "[TargetContent]\n";
		_rclsDebug << "File=" << m_clsFile << ", ID=" << m_clsID << ", NASCode="
				<< m_clsNasCode << ", Path=" << m_clsPath << ", Size=" << m_clsSize << ", FilePath=" << m_clsFilePath
				<< ", TempFile=" << m_clsTempFile <<", OutputFile=" << m_clsOutputFile << "\n";
		m_clsContainer.m_fnDebug(_rclsDebug);
		return (KSTR) _rclsDebug;
	}

	KString m_clsBinaryData;
	KString m_clsFile;
	KString m_clsID;
	KString m_clsNasCode;
	KString m_clsPath;
	KString m_clsSize;
	KString m_clsFilePath; 	// Path+File
	KString m_clsTempFile;	// For Vox Type
	KString m_clsOutputFile;
	Container m_clsContainer;
	KString m_clsTcCommand;
	ETcType_t m_clsTcType;
	ETrssCodeSet_t m_eTcSt;
};
#endif

