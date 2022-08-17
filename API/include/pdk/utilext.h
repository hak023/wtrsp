
#ifndef SIP_UTIL_EXTENSION
#define SIP_UTIL_EXTENSION

#include "list.h"

class TParam
{
public:
	const TParam &operator=(const TParam &o);
	CString	strName;
	CString	strValue;
};
inline const TParam &TParam::operator= (const TParam &o)
{
	strName = o.strName;
	strValue= o.strValue;
	return *this;
}

typedef TParam*	PParam;
typedef CArray<TParam,TParam> CParamExt;

PDKINT StringToVal(LPCSTR pszStr, PDKINT nCase = 1/*1=upper,0=noop,-1=lower*/);
/* dont't use function call param */
/* local variable be returned     */
LPCSTR Random(int nLen = 10);
void Random(CString &strRandHex, int nLen = 10);
bool GetToken(LPSTR &pszParam, CString &strToken, LPSTR pszDelim);
bool GetToken(LPSTR &pszParam, CString &strToken, LPSTR pszStart,
			LPSTR pszEnd);
bool ParamParse(LPCSTR pszParam, CParamExt &strParams,
			char *pszDiv, char *pszMid);

#endif // SIP_UTIL_EXTENSION
