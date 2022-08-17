
#ifndef COM_AS_SOLUTION_HXX
#define COM_AS_SOLUTION_HXX

#include <string>

#include "ComAsExtAsIf.hxx"
#include "ComAsSession.hxx"

namespace ComAs
{

typedef enum
{
	E_COM_AS_SOLUTION_vic = 0,
	E_COM_AS_SOLUTION_mgc,

	E_COM_AS_SOLUTION_num,
}EComAsSolutionType;

const char * g_fnGetSolutionTypeStr(int _n);

class SolutionVic
{
	public :
		static const SolutionExtAsCallback & m_fnGetSolutionExtAsCallback(){ return m_stVicExtAsCallback; }
		static void m_fnSetExtAsIfMgr(const ExtAsIfMgr * _pclsExtAsIfMgr){ m_pclsExtAsIfMgr = _pclsExtAsIfMgr; }		
		
		static bool m_fnEncodeVic4CustomSipEvent(const ExtAsIfMsgSipInfo & _rstExtAsIfSipInfo, void * _pExtAsIfPacket);
		static bool m_fnDecodeVic4CustomSipEvent(const void * _pExtAsIfPacket, ExtAsIfMsgSipInfo & _rstExtAsIfSipInfo);
		static bool m_fnDecodeVic4InitCustomSipEvent(const void * _pExtAsIfPacket, ExtAsIfMsgSipInfo & _rstExtAsIfSipInfo, eSIP::SetSessionInfo & _rstSetSessionInfo);

		//SNodeType
		static bool m_fnGetSNodeTypeFromSess(const ExtAsIfMsgSipInfo & _rstExtAsIfSipInfo, std::string & _rclsCustomValue);
		static bool m_fnSetSNodeTypeToSess(const char * _pszCustomValue, ExtAsIfMsgSipInfo & _rstExtAsIfSipInfo);
		static bool m_fnSetSNodeTypeToInitSess(const char * _pszCustomValue, void * _pInitSessData);

		//SNodeName
		static bool m_fnGetSNodeNameFromSess(const ExtAsIfMsgSipInfo & _rstExtAsIfSipInfo, std::string & _rclsCustomValue);

	private :
		static void m_fnCbSetSessData4Init(eDum::Session & _rclsSession, const void * _pInitSessData);		
		
		static const SolutionExtAsCallback m_stVicExtAsCallback;
		static const ExtAsIfMgr * m_pclsExtAsIfMgr;
		
};

class SolutionMgc
{
	public :
		static const SolutionExtAsCallback & m_fnGetSolutionExtAsCallback(){ return m_stSolutionExtAsCallback; }
		static void m_fnSetExtAsIfMgr(const ExtAsIfMgr * _pclsExtAsIfMgr){ m_pclsExtAsIfMgr = _pclsExtAsIfMgr; }		
		
		static bool m_fnEncodeSolCustomSipEvent(const ExtAsIfMsgSipInfo & _rstExtAsIfSipInfo, void * _pExtAsIfPacket);
		static bool m_fnDecodeSolCustomSipEvent(const void * _pExtAsIfPacket, ExtAsIfMsgSipInfo & _rstExtAsIfSipInfo);
		static bool m_fnDecodeSolInitCustomSipEvent(const void * _pExtAsIfPacket, ExtAsIfMsgSipInfo & _rstExtAsIfSipInfo, eSIP::SetSessionInfo & _rstSetSessionInfo);

	private :
		static void m_fnCbSetSessData4Init(eDum::Session & _rclsSession, const void * _pInitSessData);		
		
		static const SolutionExtAsCallback m_stSolutionExtAsCallback;
		static const ExtAsIfMgr * m_pclsExtAsIfMgr;
		
};

}

#endif

