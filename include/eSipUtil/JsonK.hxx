#ifndef JSONK_HXX
#define JSONK_HXX
#include "StlMap.hxx"
#include "string.hxx"
#include "json.hxx"
namespace eSipUtil
{
/******************************** Test Functions *****************************************/
void g_fnJsonTest();
/*********************************** Json Object ****************************************/
class JsonObject : public StlObject
{ 
	public:
		JsonObject();
		virtual ~JsonObject();
		JsonObject & operator=(JsonObject & _rclsSrc);
		void m_fnBuild(KString & _rclsBuild, unsigned int _unDepth);
		KString & m_fnFindVal(const char * _pszKey);
		//==========> Json Child Object List Functions 
		unsigned int m_fnGetNum(){return m_listJson.m_fnGetSize();}
		JsonObject *m_fnAddObj(json_type _eT,const char * _pszKey, KString _clsVal);
		JsonObject * m_fnFindObj(const char * _pszKey){return (JsonObject*)m_listJson.m_fnFindNode(_pszKey);}
		bool m_fnDelObj(const char * _pszKey){return m_listJson.m_fnDelNode(_pszKey);}
		JsonObject * m_fnBegin(ListItr_t & _rstItor){return (JsonObject*)m_listJson.m_fnBegin(_rstItor);}
		JsonObject * m_fnNext(ListItr_t & _rstItor){return (JsonObject*)m_listJson.m_fnNext(_rstItor);}
		void m_fnClear();
		json_type m_eType;
		KString m_clsJKey;
		KString m_clsJData;
		JsonObject *m_pclsOwner;
	protected:
		JsonObject * m_fnAddObjInternal(json_type _eT,const char *_pszKey);
		StlList m_listJson;
	private:
		static void m_fnCatDepth(KString &_rclsBuff,unsigned int _unDepth);
		KString m_clsNull;
};
/******************************** JSON ************************************************/
class JsonK
{
	public:
		JsonK();
		virtual ~JsonK();
		JsonK & operator=(JsonK & _rclsSrc);
		//==========> Parse Json Function
		bool m_fnParseJson(const char *_pszRaw);
		//==========> Json Root Object List Functions 
		unsigned int m_fnGetNum(){return m_listJson.m_fnGetSize();}
		JsonObject *m_fnAddObj(json_type _eT,const char * _pszKey,KString _clsVal);
		JsonObject * m_fnFindObj(const char * _pszKey){return (JsonObject*)m_listJson.m_fnFindNode(_pszKey);}
		bool m_fnDelObj(const char * _pszKey){return m_listJson.m_fnDelNode(_pszKey);}
		JsonObject * m_fnBegin(ListItr_t & _rstItor){return (JsonObject*)m_listJson.m_fnBegin(_rstItor);}
		JsonObject * m_fnNext(ListItr_t & _rstItor){return (JsonObject*)m_listJson.m_fnNext(_rstItor);}
		//==========> Build Function
		char * m_fnBuild();
		char * m_fnBuild(KString & _szBuild);
		//==========> Etc
		KString & m_fnFindVal(const char * _pszKey);
		void m_fnClear();
		static const char * m_fnGetStringJsonT(json_type _eT);
		static void m_fnBuildItem(KString &_rclsBuild,const char * _pszKey,KString _clsVal);
		static void m_fnCatDepth(KString &_rclsBuff,unsigned int _unDepth);
	protected:
		JsonObject * m_fnAddObjInternal(json_type _eT,const char *_pszKey);
		StlList m_listJson;
	private:
		void m_fnExtractJson(json_object *obj,unsigned int _unDepth,bool * _pbError);
		KString m_clsRaw;
		KString m_clsNull;
		JsonObject *m_pclsCurObj;
};
}
#endif
