
#ifndef ESIPUTIL_DATA_FILE_H
#define ESIPUTIL_DATA_FILE_H

#include <list>

#include "lock.hxx"
#include "ColumnStr.hxx"
#include "JsonK.hxx"
#include "thread.hxx"
#include "fileControl.hxx"

namespace eSipUtil
{

typedef void (* PfuncApplyData)(const list_LineDataAll & _rlistLineAllData, void * _pObject1, void * _pObject2);

typedef enum
{
	E_FILE_FORMAT_column = 0,
	E_FILE_FORMAT_json,

	E_FILE_FORMAT_NUM,
}EFileFormat;

struct DataFileConfig
{
	unsigned int m_unReserveFileLen;
	EFileFormat m_eFileFormat;
	std::string m_clsMgrName;
	std::string m_clsFileName;		//include path
	PfuncApplyData m_pfnCbApplyData;	//when File is changed and loaded
	FileControlCfg m_stFileCtrlCfg;

	/*==== the case of column file ====*/
	ColumnStrConfig m_stColumnStrCfg;

	DataFileConfig()
	{
		m_eFileFormat = E_FILE_FORMAT_column;
		m_pfnCbApplyData = NULL;
	}
};

class DataFileMgr
{
	public :
		DataFileMgr();
		~DataFileMgr();

		bool m_fnInit(const DataFileConfig & _rstDataFileCfg, void * _pApplyObject1=NULL, void * _pApplyObject2=NULL);
		void m_fnWriteFile(const list_LineDataAll & _rlistLineAllData, bool _bUseReload=true);
		inline const DataFileConfig & m_fnGetDataFileCfg() const{ return m_stDataFileCfg; }
		void m_fnReloadLock(){ m_clsReloadLock.m_fnLock(); }
		void m_fnReloadUnLock(){ m_clsReloadLock.m_fnUnlock(); }
		void m_fnSetFileLastModiTime();

	private :
		static void m_fnCbReloadFile(DataFileMgr * _pclsDataFileMgr);

		void m_fnReloadFile();
		/*==== for Column File ====*/
		std::string * m_fnCreateFileColumnStr(const list_LineDataAll & _rlistLineAllData);
		bool m_fnReadFile4ColumnStr(const char * _pData, unsigned int _unDataLen, list_LineDataAll & _rlistLineAllData);
		/*==== for Json File ====*/
		std::string * m_fnCreateFileJsonStr(const list_LineDataAll & _rlistLineAllData);
		bool m_fnReadFile4JsonStr(const char * _pData, unsigned int _unDataLen, list_LineDataAll & _rlistLineAllData);

		bool m_bInit;
		DataFileConfig m_stDataFileCfg;
		eSipUtil::NormalThread< DataFileMgr > m_clsReloadFileThread;
		time_t m_ulFileLastModiTime;
		FileControl m_clsFileCtrl;
		void * m_pApplyCbObject1;
		void * m_pApplyCbObject2;
		eSipUtil::Mutex m_clsReloadLock;

		//FileFormat
		ColumnStr m_clsColumnMgr;
		JsonK m_clsJsonParse;
};

}

#endif

