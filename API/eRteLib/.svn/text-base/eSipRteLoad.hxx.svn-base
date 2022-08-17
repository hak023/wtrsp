/**
 * @file  eSipRteLoad.hxx
 * the sip route load
 *
 * @attention Copyright (c) 2014 POINT-I Co., Ltd, Seoul, South Korea \n\n
 * All rights are reserved.  This is unpublished proprietary source
 * code of POINT-I.\n The copyright notice does not evidence any actual
 * publication of such source code.
 *
 * @par Project : Point-I Service Platform
 * @par Last Changed : 
 * @par 2014.11.12   hc_yun   1.0     first written
 */

#ifndef __ESIP_CORE_SIP_ROUTE_LOAD__
#define __ESIP_CORE_SIP_ROUTE_LOAD__

#include "rutil/RWMutex.hxx"
#include "eSipRteUtil.hxx"

namespace eSIP
{

//template default. get cps
template <unsigned int E_MAX_TIMEINTERVAL=1000, unsigned int E_MAX_PERIOD_SIZE=10 >
class CLoadCalculator
{
public:
   //contstrutor
   CLoadCalculator()
      : m_pLoad(new LoadCtrl),
        m_pLock(new resip::RWMutex){
      m_fnInit();
   }
   //distructor
   virtual ~CLoadCalculator(){
   
   }
   //whether is under the max csp that user defined.
   //bool m_fnGetCalcLoad(unsigned int _uiLoad, unsigned int& _ruiCurCPS,unsigned int _uiMaxLoad){
   bool m_fnGetCalcLoad(unsigned int _uiLoad, unsigned int& _ruiCurCPS,unsigned int _uiMaxLoad, unsigned int& _ruiInCurCPS){

      m_pLock->lock(); //////////////
      unsigned long ulPeriodIndex;
      unsigned int unCurrentIndex;
      unsigned int unRemained;
      unsigned int unRemainedRate; 
      unsigned int unDiff;
      unsigned long ulCurTime;
      unsigned int unTmpLoadOut;
      
      //unsigned int unLoadBits = _uiLoad<<3;
      unsigned int unLoadBits = _uiLoad;

      ulCurTime      = m_fnGetTimeMS();
      ulPeriodIndex  = ulCurTime / m_pLoad->ulPeriodInterval; 
      unRemained     = m_pLoad->ulPeriodInterval - (ulCurTime % m_pLoad->ulPeriodInterval);
      unRemainedRate  = unRemained*100/m_pLoad->ulPeriodInterval;
      unDiff = abs(ulPeriodIndex - m_pLoad->ulPrevPeriodIndex); 
      m_pLoad->ulPrevPeriodIndex = ulPeriodIndex;
      unCurrentIndex = ulPeriodIndex % E_MAX_PERIOD_SIZE; 


      if(unDiff == 0) {
         //do nothing
      } else if(unDiff < E_MAX_PERIOD_SIZE) { //if skip priod  
         while(--unDiff) { // if skip period,   period_sum = 0
            unsigned int unIndex = abs(ulPeriodIndex-unDiff)%E_MAX_PERIOD_SIZE;
            m_pLoad->unIntervalSumIn -= m_pLoad->unarrPeriodSumIn[unIndex];
            m_pLoad->unarrPeriodSumIn[unIndex] = 0;

            m_pLoad->unIntervalSumOut -= m_pLoad->unarrPeriodSumOut[unIndex];
            m_pLoad->unarrPeriodSumOut[unIndex] = 0;
         }

         m_pLoad->unHeadPeriodSumIn =  m_pLoad->unarrPeriodSumIn[unCurrentIndex];
         m_pLoad->unIntervalSumIn  -=  m_pLoad->unarrPeriodSumIn[unCurrentIndex];
         m_pLoad->unarrPeriodSumIn[unCurrentIndex] = 0;

         m_pLoad->unHeadPeriodSumOut =  m_pLoad->unarrPeriodSumOut[unCurrentIndex];
         m_pLoad->unIntervalSumOut  -=  m_pLoad->unarrPeriodSumOut[unCurrentIndex];
         m_pLoad->unarrPeriodSumOut[unCurrentIndex] = 0;
      } else { // if skip all period 
         memset(m_pLoad->unarrPeriodSumIn, 0, sizeof(unsigned int) * E_MAX_PERIOD_SIZE);
         m_pLoad->unIntervalSumIn = 0;
         m_pLoad->unHeadPeriodSumIn = 0;

         memset(m_pLoad->unarrPeriodSumOut, 0, sizeof(unsigned int) * E_MAX_PERIOD_SIZE);
         m_pLoad->unIntervalSumOut = 0;
         m_pLoad->unHeadPeriodSumOut = 0;
      }
      m_pLoad->unarrPeriodSumIn[unCurrentIndex] += unLoadBits;
      m_pLoad->unIntervalSumIn                  += unLoadBits;
      m_pLoad->unSumIn                          += _uiLoad;

      m_pLoad->unCurLoadIn = (m_pLoad->unIntervalSumIn + (m_pLoad->unHeadPeriodSumIn*unRemainedRate/100)) / 1;
      if (m_pLoad->unPeakLoadIn < m_pLoad->unCurLoadIn)
         m_pLoad->unPeakLoadIn=m_pLoad->unCurLoadIn;
  
      //add 
      _ruiInCurCPS=m_pLoad->unCurLoadIn; 

      unTmpLoadOut = (m_pLoad->unIntervalSumOut + unLoadBits + (int)(m_pLoad->unHeadPeriodSumOut * unRemainedRate / 100)) / 1;
#if 1
      if(_uiMaxLoad > 0 && unTmpLoadOut > _uiMaxLoad) { // drop
         m_pLoad->unCurLoadOut = (m_pLoad->unIntervalSumOut + (int)(m_pLoad->unHeadPeriodSumOut * unRemainedRate / 100)) / 1;
         if (m_pLoad->unPeakLoadOut < m_pLoad->unCurLoadOut)
            m_pLoad->unPeakLoadOut=m_pLoad->unCurLoadOut;
         _ruiCurCPS=m_pLoad->unCurLoadOut; 
         m_pLock->unlock(); //////////////
         return false;
      } else  {
         m_pLoad->unCurLoadOut = unTmpLoadOut;
         if (m_pLoad->unPeakLoadOut < m_pLoad->unCurLoadOut)
            m_pLoad->unPeakLoadOut=m_pLoad->unCurLoadOut;
      }
#else
      bool bRet=true;
      if(_uiMaxLoad > 0 && unTmpLoadOut > _uiMaxLoad) { // drop
         bRet=false;
      }
      m_pLoad->unCurLoadOut = unTmpLoadOut;
      if (m_pLoad->unPeakLoadOut < m_pLoad->unCurLoadOut)
         m_pLoad->unPeakLoadOut=m_pLoad->unCurLoadOut;

#endif
      m_pLoad->unSumOut                          += _uiLoad;
      m_pLoad->unIntervalSumOut                  += unLoadBits;
      m_pLoad->unarrPeriodSumOut[unCurrentIndex] += unLoadBits;

      _ruiCurCPS=m_pLoad->unCurLoadOut; 
      m_pLock->unlock(); //******* unlock
      return true;
   }
protected:
   struct LoadCtrl{
      unsigned long        ulStartTime;
      unsigned int         unCurLoadIn;   //current load with input load
      unsigned int         unCurLoadOut;  //current load without input load
      unsigned int         unSumIn;    
      unsigned int         unSumOut;   

      unsigned long        ulInterval;            //must be multiple of E_MAX_PERIOD_SIZE
      unsigned long        ulPeriodInterval;
      unsigned long        ulPrevPeriodIndex;
      //unsigned int         unMaxLoad;           // limit,,, rename,,,
      
      unsigned int         unPeakLoadIn;
      unsigned int         unIntervalSumIn; // 
      unsigned int         unHeadPeriodSumIn; // unRemainPeriodValue
      unsigned int         unarrPeriodSumIn[E_MAX_PERIOD_SIZE];

      unsigned int         unPeakLoadOut;
      unsigned int         unIntervalSumOut; // 
      unsigned int         unHeadPeriodSumOut; // unRemainPeriodValue
      unsigned int         unarrPeriodSumOut[E_MAX_PERIOD_SIZE];
   };
protected:
   LoadCtrl*          m_pLoad;
   resip::RWMutex*    m_pLock;
protected:
   void m_fnInit()
   {
      //m_pLoad->unMaxLoad         = unMaxLoad;
      m_pLoad->ulInterval        = E_MAX_TIMEINTERVAL;

      m_pLoad->ulPeriodInterval  = m_pLoad->ulInterval / E_MAX_PERIOD_SIZE;
      m_pLoad->ulPrevPeriodIndex = 0;

      m_pLoad->unSumIn           = 0;
      m_pLoad->unIntervalSumIn   = 0;
      m_pLoad->unHeadPeriodSumIn = 0;
      memset(m_pLoad->unarrPeriodSumIn, 0, sizeof(unsigned int) * E_MAX_PERIOD_SIZE);

      // *** current load with input load
      m_pLoad->unCurLoadIn       = 0;
      m_pLoad->unPeakLoadIn      = 0;

      m_pLoad->unSumOut           = 0;
      m_pLoad->unIntervalSumOut   = 0;
      m_pLoad->unHeadPeriodSumOut = 0;
      memset(m_pLoad->unarrPeriodSumOut, 0, sizeof(unsigned int) * E_MAX_PERIOD_SIZE);

      // *** current load without input load
      m_pLoad->unCurLoadOut       = 0;
      m_pLoad->unPeakLoadOut      = 0;
      m_pLoad->ulStartTime        = m_fnGetTimeMS(); //current time

   }
   unsigned long m_fnGetTimeMS(){
      struct timeval tv;
      gettimeofday(&tv, NULL);
      return  ((unsigned long)(tv.tv_sec) * 1000) + ((unsigned long)tv.tv_usec/1000);
   }
};


}; //end namespace

#endif

