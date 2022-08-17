
#ifndef ESIPUTIL_SHM_H
#define ESIPUTIL_SHM_H

namespace eSipUtil
{

void * createShmAuto(const char * _pszPathName, size_t _uSize, int & _nShmId);
void * createShmAuto(int _nShmKey, size_t _uSize, int & _nShmId);
void * createShm(const char * _pszPathName, size_t _uSize, bool _bReadOnly, int & _nShmId);
void * createShm(int _nShmKey, size_t _uSize, bool _bReadOnly, int & _nShmId);
bool destroyShm(const char * _pszPathName, void * pShmMem);
bool destroyShm(int _nShmKey, void * pShmMem);

}

#endif

