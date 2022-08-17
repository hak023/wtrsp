#ifndef _IBC_ERROR_TABLE_H
#define _IBC_ERROR_TABLE_H
#include "eRule/RuleInterface.h"
#include "eRule/CStatLog.h"
#include "eRule/CRUPools.h"

#include "eRte/eSipRteApi.hxx"
#include "eRte/eSipRteErr.hxx"

#include "eSipErr.hxx"
#include "eRte/IBCErrorDef.hxx"

typedef enum
{
	E_IBC_ERROR_TYPE_NONE,
	E_IBC_ERROR_TYPE_CHARGE,
	E_IBC_ERROR_TYPE_CM,
	E_IBC_ERROR_TYPE_EMP
}EIBCErrorType_t;
typedef struct IBCErrorElement_t
{
	EIBCErrorType_t m_eType;
	EIBCErrors_t m_eErrorCode;
	const char * m_pszEnumName;
	const char * m_pszDesc;
	const char * m_pszAction;
}IBCErrorElement_t;
extern IBCErrorElement_t g_arrIBCError[];
class IBCErrorInfo : public RUObject
{
	public:
		IBCErrorInfo(){m_pstElement = NULL;}
		~IBCErrorInfo(){}
		IBCErrorElement_t * m_pstElement;
};
class IBCErrors
{
	public:
		IBCErrors();
		~IBCErrors();
		static IBCErrors * fnGetInstance();
		static void fnFinalize();
		IBCErrorElement_t * fnFind(EIBCErrors_t _eErr);
	private:
		void fnInit();
		bool fnAddTable(EIBCErrors_t _eErr, IBCErrorElement_t * _pstElement);
		static IBCErrors * m_clsErros;
		RUMap m_mapErros;
};
/********************************** Core Error Conversion **********************************/
typedef struct StackError_t
{
	eSIP::ECoreError_t m_eCoreErr;
	const char * m_pszString;
	EIBCErrors_t m_eConv;
}IBCCoreError_t;
extern StackError_t g_arrStackErr[eSIP::E_CORE_ERR_NUM+1];
#endif
