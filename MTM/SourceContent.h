#ifndef SOURCE_CONTENT_H
#define SOURCE_CONTENT_H
#include "StlMap.hxx"
#include "MainConfig.h"

using namespace eSipUtil;

class Metadata
{
public:
	Metadata()
	{
	}
	~Metadata()
	{
	}
	KString m_clsAlbum;
	KString m_clsArtist;
	KString m_clsCopyright;
	KString m_clsDate;
	KString m_clsGenre;
	KString m_clsImageData;
	KString m_clsImageType;
	KString m_clsPlayDevice;
	KString m_clsPlayTime;
	KString m_clsSite;
	KString m_clsTitle;
	KString m_clsTrack;
	KString m_clsYear;

	char * m_fnDebug(eSipUtil::KString & _rclsDebug)
	{
		_rclsDebug << "[Metadata]\n";
		_rclsDebug << "Album=" << m_clsAlbum << ", Artist="
				<< m_clsArtist << ", Copyright=" << m_clsCopyright << ", Date="
				<< m_clsDate << ", Genre=" << m_clsGenre << ", ImageData=" << m_clsImageData
				<< ", ImageType=" << m_clsImageType << ", PlayDevice=" << m_clsPlayDevice << ", PlayTime="
				<< m_clsPlayTime << ", Site=" << m_clsSite << ", Title=" << m_clsTitle
				<< ", Track=" << m_clsTrack << ", Year=" << m_clsYear << "\n";

		return (KSTR) _rclsDebug;
	}
	const char* m_fnGetAlbum()
	{
		if (m_clsAlbum.m_unRealLen == 0)
			return "";
		return (KCSTR) m_clsAlbum;
	}

	const char* m_fnGetArtist()
	{
		if (m_clsArtist.m_unRealLen == 0)
			return "";
		return (KCSTR) m_clsArtist;
	}

	const char* m_fnGetCopyright()
	{
		if (m_clsCopyright.m_unRealLen == 0)
			return "";
		return (KCSTR) m_clsCopyright;
	}

	const char* m_fnGetDate()
	{
		if (m_clsDate.m_unRealLen == 0)
			return "";
		return (KCSTR) m_clsDate;
	}

	const char* m_fnGetGenre()
	{
		if (m_clsGenre.m_unRealLen == 0)
			return "";
		return (KCSTR) m_clsGenre;
	}

	const char* m_fnGetImageData()
	{
		if (m_clsImageData.m_unRealLen == 0)
			return "";
		return (KCSTR) m_clsImageData;
	}

	const char* m_fnGetImageType()
	{
		if (m_clsImageType.m_unRealLen == 0)
			return "";
		return (KCSTR) m_clsImageType;
	}

	const char* m_fnGetPlayDevice()
	{
		if (m_clsPlayDevice.m_unRealLen == 0)
			return "";
		return (KCSTR) m_clsPlayDevice;
	}

	const char* m_fnGetPlayTime()
	{
		if (m_clsPlayTime.m_unRealLen == 0)
			return "";
		return (KCSTR) m_clsPlayTime;
	}

	const char* m_fnGetSite()
	{
		if (m_clsSite.m_unRealLen == 0)
			return "";
		return (KCSTR) m_clsSite;
	}

	const char* m_fnGetTitle()
	{
		if (m_clsTitle.m_unRealLen == 0)
			return "";
		return (KCSTR) m_clsTitle;
	}

	const char* m_fnGetTrack()
	{
		if (m_clsTrack.m_unRealLen == 0)
			return "";
		return (KCSTR) m_clsTrack;
	}

	const char* m_fnGetYear()
	{
		if (m_clsYear.m_unRealLen == 0)
			return "";
		return (KCSTR) m_clsYear;
	}
};

class SourceContent: public eSipUtil::StlObject
{
public:
	SourceContent()
	{
	}
	~SourceContent()
	{
	}
	char * m_fnDebug(eSipUtil::KString & _rclsDebug)
	{
		_rclsDebug << "[SourceContent]\n";
		_rclsDebug << "BinaryData=" << m_clsBinaryData << ", File=" << m_clsFile << ", Container ID=" << m_clsID << ", NASCode="
				<< m_clsNasCode << ", Path=" << m_clsPath << ", FilePath=" << m_clsFilePath
				<< "\n";
		m_clsMetadata.m_fnDebug(_rclsDebug);
		return (KSTR) _rclsDebug;
	}

	KString m_clsBinaryData;
	KString m_clsFile;
	KString m_clsID;        // ContainerID
	KString m_clsNasCode;
	KString m_clsPath;
	KString m_clsFilePath; 	// Path+File
	Metadata m_clsMetadata;
};
#endif

