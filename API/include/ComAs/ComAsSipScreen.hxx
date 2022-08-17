
#ifndef COM_AS_SIP_SCREEN_HXX
#define COM_AS_SIP_SCREEN_HXX

#include <string>
#include <map>

#include "eSipUtil/dataFile.hxx"
#include "eSipUtil/lock.hxx"

#include "ComAsSipHelper.hxx"

namespace ComAs
{

struct ComAsSipScrDataInfo;

typedef enum
{
	E_SIP_SCR_RESULT_succ = 0,
	E_SIP_SCR_RESULT_invalid_id,
	E_SIP_SCR_RESULT_invalid_op,
	E_SIP_SCR_RESULT_invalid_main,
	E_SIP_SCR_RESULT_invalid_mainValue,
	E_SIP_SCR_RESULT_invalid_subPart,
	E_SIP_SCR_RESULT_invalid_subRange,
	E_SIP_SCR_RESULT_invalid_subValue1,
	E_SIP_SCR_RESULT_invalid_subValue2,
	E_SIP_SCR_RESULT_invalid_subIdx,
	E_SIP_SCR_RESULT_notInit,
	E_SIP_SCR_RESULT_existId,
	E_SIP_SCR_RESULT_notExistId,
	E_SIP_SCR_RESULT_internalErr,

	E_SIP_SCR_RESULT_NUM,
}ESipScrResult;

typedef enum
{
	E_SIP_SCR_OP_add = 0,
	E_SIP_SCR_OP_del,
	E_SIP_SCR_OP_change,

	E_SIP_SCR_OP_NUM,
}ESipScrOp;

typedef enum
{
	E_SIP_SCR_MAIN_reqUri = 0,
	E_SIP_SCR_MAIN_header,
	E_SIP_SCR_MAIN_rspCode,

	E_SIP_SCR_MAIN_NUM,
}ESipScrMain;

typedef enum
{
	E_SIP_SCR_SUB_PART_full = 0,
	E_SIP_SCR_SUB_PART_part,
	E_SIP_SCR_SUB_PART_user,
	E_SIP_SCR_SUB_PART_userPart,
	E_SIP_SCR_SUB_PART_host,
	E_SIP_SCR_SUB_PART_param,
	E_SIP_SCR_SUB_PART_schemTel,

	E_SIP_SCR_SUB_PART_NUM,
}ESipScrSubPart;

typedef enum
{
	E_SIP_SCR_SUB_RANGE_all = 0,
	E_SIP_SCR_SUB_RANGE_top,
	E_SIP_SCR_SUB_RANGE_bottom,

	E_SIP_SCR_SUB_RANGE_NUM,
}ESipScrSubRange;

const char * g_fnGetSipScrResultStr(int _n);
const char * g_fnGetSipScrOpStr(int _n);
const char * g_fnGetSipScrMainStr(int _n);
const char * g_fnGetSipScrSubPartStr(int _n);
const char * g_fnGetSipScrSubRangeStr(int _n);

int g_fnGetSipScrOpEnum(const char * _pszStr);
int g_fnGetSipScrMainEnum(const char * _pszStr);
int g_fnGetSipScrSubPartEnum(const char * _pszStr);
int g_fnGetSipScrSubRangeEnum(const char * _pszStr);

struct ComAsSipScrEntity
{
	enum
	{
		E_CONST_DATA_NUMS = 9,
		E_CONST_MAX_PRINT_LEN = 1024,
	};

	int m_nId;	
	int m_nSipScrOp;						//ESipScrOp
	
	int m_nSipScrMain;					//ESipScrMain
	std::string m_clsSipScrMainValue;
	
	int m_nSipScrSubPart;				//ESipScrSubPart
	int m_nSipScrSubRange;			//ESipScrSubRange	
	std::string m_clsSipScrSubValue1;
	std::string m_clsSipScrSubValue2;
	int m_nSipScrSubIdx;

	ComAsSipScrEntity();

	void m_fnReset();
	ESipScrResult m_fnValid();
	void m_fnApply(const eSipUtil::list_LineData & _rlistLineData);
	void m_fnApply(const ComAsSipScrEntity & _rstSipSrcEntity);
	void m_fnGetData(eSipUtil::list_LineData & _rlistLineData) const;
	static bool m_fnGetId(const eSipUtil::list_LineData & _rlistLineData, int & _nId);	
	const char * m_fnGetStr(char * _pszStr, unsigned int _unMaxLen, bool _bInit=true) const;
	ComAsSipScrEntity * m_fnClone() const;

	static void m_fnGetId(const ComAsSipScrEntity & _rstEntity, std::string & _rclsOutStr);
	static void m_fnGetOp(const ComAsSipScrEntity & _rstEntity, std::string & _rclsOutStr);
	static void m_fnGetMain(const ComAsSipScrEntity & _rstEntity, std::string & _rclsOutStr);
	static void m_fnGetMainValue(const ComAsSipScrEntity & _rstEntity, std::string & _rclsOutStr);
	static void m_fnGetSubPart(const ComAsSipScrEntity & _rstEntity, std::string & _rclsOutStr);
	static void m_fnGetSubRange(const ComAsSipScrEntity & _rstEntity, std::string & _rclsOutStr);
	static void m_fnGetSubPartValue1(const ComAsSipScrEntity & _rstEntity, std::string & _rclsOutStr);
	static void m_fnGetSubPartValue2(const ComAsSipScrEntity & _rstEntity, std::string & _rclsOutStr);
	static void m_fnGetSubPartIdx(const ComAsSipScrEntity & _rstEntity, std::string & _rclsOutStr);
	
	static void m_fnSetId(const char * _pszStr, ComAsSipScrEntity & _rstEntity);
	static void m_fnSetOp(const char * _pszStr, ComAsSipScrEntity & _rstEntity);
	static void m_fnSetMain(const char * _pszStr, ComAsSipScrEntity & _rstEntity);
	static void m_fnSetMainValue(const char * _pszStr, ComAsSipScrEntity & _rstEntity);	
	static void m_fnSetSubPart(const char * _pszStr, ComAsSipScrEntity & _rstEntity);
	static void m_fnSetSubRange(const char * _pszStr, ComAsSipScrEntity & _rstEntity);
	static void m_fnSetSubPartValue1(const char * _pszStr, ComAsSipScrEntity & _rstEntity);
	static void m_fnSetSubPartValue2(const char * _pszStr, ComAsSipScrEntity & _rstEntity);
	static void m_fnSetSubPartIdx(const char * _pszStr, ComAsSipScrEntity & _rstEntity);
};

typedef std::map< int, ComAsSipScrEntity > Map_Scr;		//key : id of screen

struct SipScrAllInfo
{
	ComAsSipScrEntity * m_pclsSipScrEntity;

	SipScrAllInfo()
	{
		m_pclsSipScrEntity = NULL;
	}

	~SipScrAllInfo()
	{
		delete m_pclsSipScrEntity;
	}
};
typedef std::list< SipScrAllInfo > list_SipScrAllInfo;

class ComAsSipScrMgr
{
	public :
		static ComAsSipScrMgr & getInstance();
		~ComAsSipScrMgr();

		bool m_fnInit();
		
		ESipScrResult m_fnAddScr(const eSipUtil::list_LineData & _rlistScrData, bool _bLock=true, bool _bFileWrite=true);
		ESipScrResult m_fnDelScr(const eSipUtil::list_LineData & _rlistScrData);
		ESipScrResult m_fnChgScr(const eSipUtil::list_LineData & _rlistScrData);
		ESipScrResult m_fnFindScr(eSipUtil::list_LineData & _rlistScrData) const;

		ESipScrResult m_fnAddScr(const ComAsSipScrEntity & _rstSipScrEntity);
		ESipScrResult m_fnDelScr(int _nId);
		ESipScrResult m_fnChgScr(const ComAsSipScrEntity & _rstSipScrEntity);
		ESipScrResult m_fnFindScr(ComAsSipScrEntity & _rstSipScrEntity) const;
		ESipScrResult m_fnFindScrAll(list_SipScrAllInfo & _rlistSipScrAllInfo) const;
		
		bool m_fnIsScr(int _nScrId) const;
		bool m_fnExecuteScr(int _nId, resip::SipMessage & _rclsSipMsg) const;

#if 0
		void m_fnTestInputScr();
#endif

	private :
		static void m_fnCbApplyScrData(const eSipUtil::list_LineDataAll & _rlistScrDataAll, void * _pObject1, void * _pObject2);
		
		ComAsSipScrMgr();

		void m_fnWriteFileFromMem() const;
		//add
		bool m_fnExecAdd(const ComAsSipScrEntity & _rstSipScrEntity, resip::SipMessage & _rclsSipMsg) const;
		bool m_fnExecAddReqUriFull(const ComAsSipScrEntity & _rstSipScrEntity, resip::SipMessage & _rclsSipMsg) const;
		bool m_fnExecAddReqUriPart(const ComAsSipScrEntity & _rstSipScrEntity, resip::SipMessage & _rclsSipMsg) const;
		bool m_fnExecAddReqUriUsr(const ComAsSipScrEntity & _rstSipScrEntity, resip::SipMessage & _rclsSipMsg) const;
		bool m_fnExecAddReqUriUsrPart(const ComAsSipScrEntity & _rstSipScrEntity, resip::SipMessage & _rclsSipMsg) const;	
		bool m_fnExecAddReqUriHost(const ComAsSipScrEntity & _rstSipScrEntity, resip::SipMessage & _rclsSipMsg) const;	
		bool m_fnExecAddReqUriParam(const ComAsSipScrEntity & _rstSipScrEntity, resip::SipMessage & _rclsSipMsg) const;
		bool m_fnExecAddHdrFull(const ComAsSipScrEntity & _rstSipScrEntity, resip::SipMessage & _rclsSipMsg) const;
		bool m_fnExecAddHdrPart(const ComAsSipScrEntity & _rstSipScrEntity, resip::SipMessage & _rclsSipMsg) const;
		bool m_fnExecAddHdrUriUsr(const ComAsSipScrEntity & _rstSipScrEntity, resip::SipMessage & _rclsSipMsg) const;
		bool m_fnExecAddHdrUriUsrPart(const ComAsSipScrEntity & _rstSipScrEntity, resip::SipMessage & _rclsSipMsg) const;
		bool m_fnExecAddHdrUriHost(const ComAsSipScrEntity & _rstSipScrEntity, resip::SipMessage & _rclsSipMsg) const;
		bool m_fnExecAddHdrParam(const ComAsSipScrEntity & _rstSipScrEntity, resip::SipMessage & _rclsSipMsg) const;
		//del
		bool m_fnExecDel(const ComAsSipScrEntity & _rstSipScrEntity, resip::SipMessage & _rclsSipMsg) const;
		bool m_fnExecDelReqUriPart(const ComAsSipScrEntity & _rstSipScrEntity, resip::SipMessage & _rclsSipMsg) const;
		bool m_fnExecDelReqUriUsr(const ComAsSipScrEntity & _rstSipScrEntity, resip::SipMessage & _rclsSipMsg) const;
		bool m_fnExecDelReqUriUsrPart(const ComAsSipScrEntity & _rstSipScrEntity, resip::SipMessage & _rclsSipMsg) const;
		bool m_fnExecDelReqUriHost(const ComAsSipScrEntity & _rstSipScrEntity, resip::SipMessage & _rclsSipMsg) const;
		bool m_fnExecDelReqUriParam(const ComAsSipScrEntity & _rstSipScrEntity, resip::SipMessage & _rclsSipMsg) const;
		bool m_fnExecDelHdrFull(const ComAsSipScrEntity & _rstSipScrEntity, resip::SipMessage & _rclsSipMsg) const;
		bool m_fnExecDelHdrPart(const ComAsSipScrEntity & _rstSipScrEntity, resip::SipMessage & _rclsSipMsg) const;
		bool m_fnExecDelHdrUriUsr(const ComAsSipScrEntity & _rstSipScrEntity, resip::SipMessage & _rclsSipMsg) const;
		bool m_fnExecDelHdrUriUsrPart(const ComAsSipScrEntity & _rstSipScrEntity, resip::SipMessage & _rclsSipMsg) const;
		bool m_fnExecDelHdrUriHost(const ComAsSipScrEntity & _rstSipScrEntity, resip::SipMessage & _rclsSipMsg) const;
		bool m_fnExecDelHdrParam(const ComAsSipScrEntity & _rstSipScrEntity, resip::SipMessage & _rclsSipMsg) const;
		//chg
		bool m_fnExecChg(const ComAsSipScrEntity & _rstSipScrEntity, resip::SipMessage & _rclsSipMsg) const;
		bool m_fnExecChgReqUriFull(const ComAsSipScrEntity & _rstSipScrEntity, resip::SipMessage & _rclsSipMsg) const;
		bool m_fnExecChgReqUriPart(const ComAsSipScrEntity & _rstSipScrEntity, resip::SipMessage & _rclsSipMsg) const;
		bool m_fnExecChgReqUriUsr(const ComAsSipScrEntity & _rstSipScrEntity, resip::SipMessage & _rclsSipMsg) const;
		bool m_fnExecChgReqUriUsrPart(const ComAsSipScrEntity & _rstSipScrEntity, resip::SipMessage & _rclsSipMsg) const;
		bool m_fnExecChgReqUriHost(const ComAsSipScrEntity & _rstSipScrEntity, resip::SipMessage & _rclsSipMsg) const;
		bool m_fnExecChgReqUriParam(const ComAsSipScrEntity & _rstSipScrEntity, resip::SipMessage & _rclsSipMsg) const;
		bool m_fnExecChgHdrFull(const ComAsSipScrEntity & _rstSipScrEntity, resip::SipMessage & _rclsSipMsg) const;
		bool m_fnExecChgHdrPart(const ComAsSipScrEntity & _rstSipScrEntity, resip::SipMessage & _rclsSipMsg) const;
		bool m_fnExecChgHdrUriUsr(const ComAsSipScrEntity & _rstSipScrEntity, resip::SipMessage & _rclsSipMsg) const;
		bool m_fnExecChgHdrUriUsrPart(const ComAsSipScrEntity & _rstSipScrEntity, resip::SipMessage & _rclsSipMsg) const;
		bool m_fnExecChgHdrUriHost(const ComAsSipScrEntity & _rstSipScrEntity, resip::SipMessage & _rclsSipMsg) const;
		bool m_fnExecChgHdrParam(const ComAsSipScrEntity & _rstSipScrEntity, resip::SipMessage & _rclsSipMsg) const;
		bool m_fnExecChgSchem(const ComAsSipScrEntity & _rstSipScrEntity, resip::SipMessage & _rclsSipMsg) const;
		bool m_fnExecChgRspCode(const ComAsSipScrEntity & _rstSipScrEntity, resip::SipMessage & _rclsSipMsg) const;

		bool m_bInit;
		mutable eSipUtil::RwMutex m_clsMapRwMutex;
		Map_Scr m_mapScrData;
		mutable eSipUtil::DataFileMgr m_stScrDataFile;
};

}

#endif

