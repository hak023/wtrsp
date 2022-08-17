#include "CZip.h"
#include "main.h"

using namespace eSipUtil;

CZip::CZip(){}
CZip::~CZip(){}
bool CZip::m_fnCompress(Bytef * dest, uLongf * destLen, const Bytef * source,
		uLong sourceLen)
{
	int ret = compress(dest, destLen, source, sourceLen);
	if (ret == Z_OK)
	{
		IFLOG(E_LOG_INFO, "Compress Success");
		return true;
	}
	else if (ret == Z_BUF_ERROR)
	{
		IFLOG(E_LOG_ERR,
				"Compress Fail[The buffer dest was not large enough to hold the compressed data]");
		return false;
	}
	else if (ret == Z_MEM_ERROR)
	{
		IFLOG(E_LOG_ERR, "Compress Fail[Insufficient memory]");
		return false;
	}
	else
	{
		IFLOG(E_LOG_ERR, "Compress Fail[Unknown error][code:%d]", ret);
		return false;
	}
}
bool CZip::m_fnEZCompress(ezbuffer& _rBufDest, const ezbuffer& _rBufSrc)
{
   int ret = ezcompress(_rBufDest, _rBufSrc);
   if (ret == Z_OK)
   {
      IFLOG(E_LOG_DEBUG, "EZCompress Success");
      return true;
   }
   else if (ret == Z_BUF_ERROR)
   {
      IFLOG(E_LOG_ERR,
            "EZCompress Fail[The buffer dest was not large enough to hold the compressed data]");
      return false;
   }
   else if (ret == Z_MEM_ERROR)
   {
      IFLOG(E_LOG_ERR, "EZCompress Fail[Insufficient memory]");
      return false;
   }
   else
   {
      IFLOG(E_LOG_ERR, "EZCompress Fail[Unknown error][code:%d]", ret);
      return false;
   }
}
bool CZip::m_fnUncompress(Bytef * dest, uLongf * destLen, const Bytef * source,
		uLong sourceLen)
{
	int ret = uncompress(dest, destLen, source, sourceLen);
	if (ret == Z_OK)
	{
		IFLOG(E_LOG_INFO, "Uncompress Success");
		return true;
	}
	else if (ret == Z_DATA_ERROR)
	{
		IFLOG(E_LOG_ERR,
				"Uncompress Fail[The compressed data (referenced by source) was corrupted]");
		return false;
	}
	else if (ret == Z_MEM_ERROR)
	{
		IFLOG(E_LOG_ERR, "Uncompress Fail[Insufficient memory]");
		return false;
	}
	else if (ret == Z_BUF_ERROR)
	{
		IFLOG(E_LOG_ERR,
				"Uncompress Fail[The buffer dest was not large enough to hold the uncompressed data]");
		return false;
	}
	else
	{
		IFLOG(E_LOG_ERR, "Uncompress Fail[Unknown error][code:%d]", ret);
		return false;
	}
}
bool CZip::m_fnEZUncompress(ezbuffer& _rBufDest, const ezbuffer& _rBufSrc)
{
   int ret = ezuncompress(_rBufDest, _rBufSrc);
   if (ret == Z_OK)
   {
      IFLOG(E_LOG_DEBUG, "EZUncompress Success");
      return true;
   }
   else if (ret == Z_DATA_ERROR)
   {
      IFLOG(E_LOG_ERR,
            "EZUncompress Fail[The compressed data (referenced by source) was corrupted]");
      return false;
   }
   else if (ret == Z_MEM_ERROR)
   {
      IFLOG(E_LOG_ERR, "EZUncompress Fail[Insufficient memory]");
      return false;
   }
   else if (ret == Z_BUF_ERROR)
   {
      IFLOG(E_LOG_ERR,
            "EZUncompress Fail[The buffer dest was not large enough to hold the uncompressed data]");
      return false;
   }
   else
   {
      IFLOG(E_LOG_ERR, "EZUncompress Fail[Unknown error][code:%d]", ret);
      return false;
   }
}
