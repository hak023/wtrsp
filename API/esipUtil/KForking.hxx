#ifndef _KFORKING_HXX
#define _KFORKING_HXX
#include "thread.hxx"
#include "string.hxx"
namespace eSipUtil
{
class KForking;
typedef void (*PFuncKillBackGround_t)(int _nT);   // kill -SIGUSR1 pid
typedef void (*PFuncNotifyChildTerminated_t)(KForking * _pclsObj, char _cExitCode);
typedef enum
{
	E_FORKING_NONE=0,
	E_FORKING_ONCE,
	E_FORKING_PERIODIC,
	E_FORKING_MAX
}EForkingMode_t;
class KForking
{
	public :
		KForking();
		~KForking();
		//==========> Init Child Process
		void m_fnInit(EForkingMode_t _eT, PFuncNotifyChildTerminated_t _pfnTerm, const char * _pszArgs);
		//==========> Start Child Process
		void m_fnStart();
		//==========> Stop Child Process
		void m_fnStop();
		//==========> Get Child Process Name
		const char * m_fnGetChildName();
		//==========> BackGround Mode Function
		static void m_fnBackGroundMode( PFuncKillBackGround_t _pfn);
		/*
			Example
			static s_fnCbkBackGroundKill(int _nT)
			{
				exit(_nT);
			}
			int main(int argc, char ** argv)
			{
				bool bBackGround = false;
				for(int i=0;i<argc;i++)
				{
					if(strcmp(argv[i],"bg")==0) {bBackGround = true;break;}
				}
				if(bBackGround) KForking::m_fnBackGroundMode(s_fnCbkBackGroundKill);
				... user code ....
				main_loop
			}
		*/
	private :
		static void m_fnCbkProc(KForking * _pObject);
		NormalThread< KForking > m_clsThread;
		bool m_bChildStarted;
		int m_nStatus;
		int m_nChildRet;
		pid_t m_Pid;
		pid_t m_Pidck;
		KString m_clsBuff;
		TokStrings m_clsArgs;
		char **m_argv;
		EForkingMode_t m_eMod;
		PFuncNotifyChildTerminated_t m_pfnTerminated;
};


}
#endif
