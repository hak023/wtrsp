
#ifndef ESIPUTIL_MEM_H
#define ESIPUTIL_MEM_H

namespace eSipUtil
{

const char * memstr(const char * _pSrcData, const char * _pszFindStr, int _nSrcDataLen);
const char * memstr4Reverse(const char * _pSrcData, const char * _pszFindStr, int _nSrcDataLen);
const char * memistr(const char * _pSrcData, const char * _pszFindStr, int _nSrcDataLen);
const char * memistr4Reverse(const char * _pSrcData, const char * _pszFindStr, int _nSrcDataLen);

}

#endif

