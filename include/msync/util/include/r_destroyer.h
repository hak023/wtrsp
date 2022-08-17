
#ifndef _RPDK_SINGLETON_DESTROYER_H_
#define _RPDK_SINGLETON_DESTROYER_H_
#include <stdio.h>
#include <stdlib.h>

////////////////
namespace RED
{
////////////////////
template <class T>
#ifdef WIN32
class DLLPORT Singleton_Destroyer
#else
class Singleton_Destroyer
#endif
{
public:
   Singleton_Destroyer() : m_pT(NULL) { }
#if 0
   ~Singleton_Destroyer() { delete m_pT; }
#endif
   void Register(T *pT) { m_pT = pT; }

private:
   T *m_pT;
};
////////////////////
};//namespace
///////////////////

#endif
