
#ifndef ESIPUTIL_FILE_CONTROL_H
#define ESIPUTIL_FILE_CONTROL_H

#include <string>

#include "queueThread.hxx"

namespace eSipUtil
{

class DataFileMgr;

struct FileControlCfg
{
	std::string m_clsName;	//name of mgr
	bool m_bAsyncWrite;
	unsigned int m_unMaxQueueSize;
	bool m_bDiskWrite;

	FileControlCfg()
	{
		m_bAsyncWrite = true;
		m_unMaxQueueSize = 1000;
		m_bDiskWrite = false;
	}
};

class FileControl
{
	public :
		FileControl();
		~FileControl();
		
		bool m_fnInit(const FileControlCfg & _rstFileControlCfg, DataFileMgr * _pclsDataFileMgr=NULL);
		// Global Internal Instance Function Call
		static bool m_fnWrite(const char * _pszFileName, const char * _pszData,unsigned int _unLen);
		//_pclsWriteStr must be create by new() and caller must not delete _pclsWriteStr exception returned value is false
		bool m_fnWriteFile(const char * _pszFileName, std::string * _pclsWriteStr, bool _bUseReload4DataFile=true);
		//_pWriteData must be create by malloc() and caller must not delete _pWriteData exception returned value is false
		bool m_fnWriteFile(const char * _pszFileName, char * _pWriteData, unsigned int _unWriteLen, bool _bUseReload4DataFile=true);
		//return : null terminated buffer created by malloc()
		bool m_fnWriteFile_(const char * _pszFileName, const char * _pszData, unsigned int _unLen);  // copy value
		char * m_fnReadFile(const char * _pszFileName);

	private :		
		struct FileWriteInfo
		{
			bool m_bUseReload4DataFile;
			std::string m_clsFileName;

			/*==== for string ====*/
			std::string * m_pclsWriteStr;

			/*==== for non-string ====*/
			char * m_pWriteData;
			unsigned int m_unWriteLen;

			FileWriteInfo()
			{
				m_bUseReload4DataFile = true;
				m_pclsWriteStr = NULL;
				m_pWriteData = NULL;
				m_unWriteLen = 0;
			}

			~FileWriteInfo()
			{
				delete m_pclsWriteStr;
				free(m_pWriteData);
			}
		};

		static void m_fnCbFileWrite(FileWriteInfo * _pstFileWriteInfo, void * _pObject);

		bool m_bInit;
		FileControlCfg m_stFileControlCfg;
		QueueThread< FileWriteInfo > * m_pclsFileWriteThread;
		DataFileMgr * m_pclsDataFileMgr;

};

}

#endif

