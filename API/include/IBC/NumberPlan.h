#ifndef NUMBER_PLAN_H
#define NUMBER_PLAN_H
#include "eSipUtil/string.hxx"
class NumberPlan
{
	public:
		/*
			핸펀    01x
			서울    02
			경기    031
			인천    032
			강원    033
			충남    041
			대전    042
			충북    043
			세종    044
			부산    051
			울산    052
			대구    053
			경북    054
			경남    055
			전남    061
			광주    062
			전북    063
			제주    064
			유선    070
		*/
		NumberPlan();
		~NumberPlan();
		// +82-10-1234-5678
		// hlr npa = 010
		// hlr pfx = 1234
		// npdb number = 01012345678
		// hlr number = 821012345678
		bool m_fnDecode(const char * _pszNumber);
		const char * m_fnDebug(eSipUtil::KString &_rclsDebug);
		char * m_fnGetNpdbNumber(eSipUtil::KString &_rclsDebug);
		char * m_fnGetHlrNumber(eSipUtil::KString &_rclsDebug);
		char * m_fnGetTelNumber(eSipUtil::KString &_rclsDebug);
		char * m_fnGetSipNumber(eSipUtil::KString &_rclsDebug);
		const char * m_fnGetHlrPrefix();
		const char * m_fnGetHlrNpa();
		void m_fnClear();
	private:
		bool m_fnParseNationalCode();
		bool m_fnParseAreaCode();
		bool m_fnParsePfxCode();
		bool m_fnParsePrivateCode();
		unsigned int m_fnGetRemainDigit();
		bool m_fnIsDigit(char _ucInput);
	public:
		eSipUtil::KString m_clsNationalCode;
		eSipUtil::KString m_clsAreaCode;
		eSipUtil::KString m_clsPfxCode;
		eSipUtil::KString m_clsPrivateCode;
		bool m_bExistNcCode;
	private:
		eSipUtil::KString m_clsRawData;
		unsigned int m_unPos;
};
void g_fnTestNumberPlan();
#endif
