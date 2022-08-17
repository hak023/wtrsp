#ifndef CZIP_H
#define CZIP_H
#include <zlib.h>
#include <string>

#include "easyzlib.h"

class CZip
{
public:
	CZip();
	~CZip();
	static bool m_fnCompress(Bytef * dest, uLongf * destLen, const Bytef * source,
			uLong sourceLen);
	static bool m_fnUncompress(Bytef * dest, uLongf * destLen, const Bytef * source,
			uLong sourceLen);

   static bool m_fnEZCompress(ezbuffer& _rBufDest, const ezbuffer& _rBufSrc);
   static bool m_fnEZUncompress(ezbuffer& _rBufDest, const ezbuffer& _rBufSrc);
};

#endif
