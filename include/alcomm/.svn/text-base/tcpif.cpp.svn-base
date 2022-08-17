#include <errno.h>
#include "tcpif.h"

#define INIT_TCPDATALEN (1024*4)
#define MAX_TCPDATALEN  (1024* 1024 * 1)

#define MAX_ALIVECNT        3 
#define ALIVE_INTERVAL_TIME 999

extern int errno;
/////////////////
c_tcp_ping::c_tcp_ping()
   : m_phandler(NULL),
     m_bstart(false),
     m_border(false)
{
   strcpy(m_szhdr,"[PING]");
}

c_tcp_ping::~c_tcp_ping()
{

}

bool c_tcp_ping::start(char* phdr, void* phandler, int norder)
{
   memset(m_szhdr,0x00,sizeof(m_szhdr));
   strncpy(m_szhdr,phdr,sizeof(m_szhdr)-1);
   m_phandler=phandler;
   if ( norder != 0 ) m_border=true;

   return CUserThread::Start();
}

void* c_tcp_ping::ThreadProc()
{

   SVCComMsgHdr stsnd_msg;
   int header_len=sizeof(SVCComMsgHdr);
   memset(&stsnd_msg,0x00,header_len);

   //set
   stsnd_msg.uiMagicCookie=SVC_MSG_MAGIC_COOKIE;
   stsnd_msg.uiMsgLen=header_len;
   stsnd_msg.uiType=SVC_ALIVE;

   //ordering
   if ( m_border ) stsnd_msg.hton();

   m_bStopped=false;
   bool bret=0;

	while(!DoExit()){
      if(m_bStopped) break;
      SafeSleep(ALIVE_INTERVAL_TIME);
      if(!m_phandler) continue;
      if(!m_bstart) continue;
      if(m_bStopped) break;
      
      //send ping
      bret=((c_tcpsock*)m_phandler)->sendmsg((char*)&stsnd_msg,header_len);
      if(bret){
         ((c_tcpsock*)m_phandler)->decide_reconnect();
		   //LOGGER(CLogger::Info,"%s send SVC_ALIVE %p",m_szhdr,this);
      }
   }
	m_bEscaped=true;
	return NULL;
}


///////////// c_tcp socket
c_tcpsock::c_tcpsock()
   : m_bconnect(false),
     m_boccured_error(false),
     m_bwaiting(false),
     m_nalive_cnt(0),
     m_ptcplock(new CCritSec),
     m_psock(NULL),
     m_ptcpcond(NULL),
     m_pping(NULL)
{
   memset(m_szlog_hdr,0x00,sizeof(m_szlog_hdr));
   memset(&m_info,0x00,sizeof(m_info));
	sprintf(m_szlog_hdr,"[%s]","TSOCK");
}

void c_tcpsock::set_property(st_tcp_info* pinfo){
	memcpy(&m_info,pinfo,sizeof(st_tcp_info));
}
bool c_tcpsock::start()
{
	if ( m_info.emode != etcpserver  ){
      m_pping=new c_tcp_ping;
      m_pping->start(m_szlog_hdr,(void*)this,(int)(m_info.ucenable_order));
	}
	//check the property
	if ( m_info.emode == etcpclient ){
		m_psock=new CTcpSocket;
		if ( strlen(m_info.szip) == 0 ||  m_info.nport <= 0 ) {
			LOGGER(CLogger::Error,"%s IP(%s), Port(%d) was not set",
         m_szlog_hdr,m_info.szip,m_info.nport);
			return false;
		}
	}else if ( m_info.emode == etcpserver ){
		m_psock=new CTcpSocket;
		if ( m_info.nport <= 0 ) {
			LOGGER(CLogger::Error,"%s Port(%d) was not set",m_szlog_hdr, m_info.nport);
			return false;
		}
	}else{
		m_ptcpcond=new CCondition;
		m_ptcpcond->Initialize();
		m_psock=NULL;
	}
  return CUserThread::Start();
}

void c_tcpsock::gracefulclose(){
	if( !m_psock ) return;
	m_ptcplock->Lock();

	int nfd=m_psock->GetSocket();
	if( nfd == INVALID_SOCKET ){ 
		m_ptcplock->Unlock();
		return;
	}
	shutdown(nfd,SHUT_RDWR);
	m_psock->Close();
	if ( m_info.emode == etcpsvrrecv &&
       m_psock ){
		delete m_psock;
		m_psock=NULL;
	}
	m_ptcplock->Unlock();
}

void c_tcpsock::stop()
{
   //if(m_pping) m_pping->Stop();
	if(  m_info.emode != etcpserver  ){
		m_nalive_cnt=0;
      if (  m_info.nissend_alive == 1 )
		   m_pping->setflag(false);
	}
   if ( m_info.emode == etcpserver )
   {
	   if( m_bconnect ) gracefulclose();
   }
	if ( m_info.emode == etcpsvrrecv ){
      m_bStopped=true;
		if( m_bwaiting ) m_ptcpcond->Signal();
	}
   CUserThread::Stop();
}
void c_tcpsock::set_connect(bool bconnect)
{
	if( bconnect ){
		m_bconnect=true;
	}else{
		if( !m_bStopped && m_psock ){ 
			if ( m_psock->GetSocket() != INVALID_SOCKET ){
				notify_event((void*)m_psock,(int)esockclosed);
				gracefulclose();
			}
		}
		m_bconnect=false;
	}	
	m_boccured_error=false;
}
bool c_tcpsock::prepare_sock()
{
	if(m_bStopped) return false;

	bool bRet=true;
	if( m_info.emode == etcpserver ){
		if ( strcmp(m_info.szip,"0.0.0.0") ) {
			bRet=m_psock->PrepareToServer(m_info.nport,m_info.szip);
		}else{
			bRet=m_psock->PrepareToServer(m_info.nport,NULL);
		}
	}else if ( m_info.emode == etcpclient){
		bRet=m_psock->PrepareToClient(m_info.nport,m_info.szip);
	}else {
		LOGGER(CLogger::Info,"%s Waiting......",m_szlog_hdr);
		m_bwaiting=true;
		m_ptcpcond->Wait();
		m_bwaiting=false;
		LOGGER(CLogger::Info,"%s Wake up......",m_szlog_hdr);
		if( m_psock == NULL) bRet=false;
	}
	if( bRet == false ) {
		gracefulclose();
	}
	return bRet;
}

bool c_tcpsock::can_attachsock(){
	if( m_bwaiting ) return true;
	return false;
}
void c_tcpsock::attachtcpsock(CTcpSocket* psock)
{
	m_psock=psock;
	if( m_bwaiting )m_ptcpcond->Signal();
}
void c_tcpsock::decide_reconnect()
{
	if( m_info.emode == etcpserver  ) return;
	if( ++m_nalive_cnt > MAX_ALIVECNT ){
      LOGGER(CLogger::Error,"%s the number of alive exceed(%d). we will close sock",
      m_szlog_hdr,m_nalive_cnt);
		m_boccured_error=true;
		m_nalive_cnt=0;
	}
}
void c_tcpsock::set_nonblock(int fd)
{
   if ( fd == INVALID_SOCKET ) return;
   //tcp sock non-blocking
   int     nflags;
   nflags=fcntl(fd, F_GETFL, 0);
   fcntl(fd, F_SETFL, nflags | O_NONBLOCK);
   LOGGER(CLogger::Critical,"%s set sock. non-blocking",
         m_szlog_hdr);
}

void c_tcpsock::set_sendbufsize(int fd)
{
   if ( fd == INVALID_SOCKET ||
        m_info.nsendbuf_size <= 0 ) return;
   setsockopt(fd, SOL_SOCKET, SO_SNDBUF, &(m_info.nsendbuf_size), sizeof(m_info.nsendbuf_size));
   int ncursize=0;
   int nsizelen=sizeof(ncursize);
   getsockopt(fd,
              SOL_SOCKET,
              SO_SNDBUF, //SO_MAX_MSG_SIZE,
              &ncursize,
              (socklen_t *)&nsizelen);
   LOGGER(CLogger::Critical,"%s set sock. send buf page size (%d=>%d)",
         m_szlog_hdr,m_info.nsendbuf_size,ncursize); 
}
void c_tcpsock::set_recvbufsize(int fd)
{
   if ( fd == INVALID_SOCKET ||
        m_info.nrecvbuf_size <= 0 ) return;
   setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &(m_info.nrecvbuf_size), sizeof(m_info.nrecvbuf_size));
   int ncursize=0;
   int nsizelen=sizeof(ncursize);
   getsockopt(fd,
              SOL_SOCKET,
              SO_RCVBUF, //SO_MAX_MSG_SIZE,
              &ncursize,
              (socklen_t *)&nsizelen);
   LOGGER(CLogger::Critical,"%s set sock. recv buf pagesize (%d=>%d)",
         m_szlog_hdr,m_info.nrecvbuf_size,ncursize); 
}
void c_tcpsock::prt_sockbufsize(int fd)
{

   if ( fd == INVALID_SOCKET ) return;
   int nsndcursize=0;
   int nrcvcursize=0;
   int nsizelen=sizeof(nsndcursize);

   getsockopt(fd,
              SOL_SOCKET,
              SO_SNDBUF, //SO_MAX_MSG_SIZE,
              &nsndcursize,
              (socklen_t *)&nsizelen);

   getsockopt(fd,
              SOL_SOCKET,
              SO_RCVBUF, //SO_MAX_MSG_SIZE,
              &nrcvcursize,
              (socklen_t *)&nsizelen);

   LOGGER(CLogger::Critical,"%s sock page size. send(%d)/recv(%d)",
         m_szlog_hdr,nsndcursize,nrcvcursize);
}
void * c_tcpsock::ThreadProc()
{
	int nsel_value=0;
	unsigned int ndata_len=0;

	unsigned int nmaxdata_len=INIT_TCPDATALEN;
	char *pdata_buf=new char[nmaxdata_len];
   SVCComMsgHdr* pHdr=NULL;
   int header_len=sizeof(SVCComMsgHdr);
   unsigned short ustype;

   m_bStopped=false;
	while(!DoExit()){
		if(m_bStopped) break;

		if( m_boccured_error == true ) {
			if(  m_info.emode != etcpserver ){
            if ( m_info.nissend_alive == 1 ) {
               if(m_pping) m_pping->setflag(false);
            }
				m_nalive_cnt=0;
			}
			LOGGER(CLogger::Error,"%s occured error!. will close",m_szlog_hdr);
			set_connect(false);
         if ( m_info.emode == etcpclient){
		   	CUserThread::SafeSleep(800);
         }
		}

		while( m_bconnect == false ){
			if(m_bStopped) break;
			if ( prepare_sock() == false ){
				CUserThread::SafeSleep(500);
            if ( m_info.nissend_alive == 1 ){
               if(m_pping) m_pping->setflag(false);
            }
				continue;
			}else{
				set_connect(true);
				if( m_info.emode != etcpsvrrecv ){
			  	   notify_event((void*)m_psock,(int)esockreconected);
	   		}
				//timer
				if(  m_info.emode != etcpserver  ){
					m_nalive_cnt=0;
               if ( m_info.nissend_alive == 1 ){
                  if(m_pping) m_pping->setflag(true);
               }
               //tcp non-blocking
               if( m_info.ucenable_nonblock == 0x01){
                  set_nonblock(m_psock->GetSocket());
               }
               //send, recv buffer
               if(m_info.nsendbuf_size > 0 ) set_sendbufsize(m_psock->GetSocket());
               if(m_info.nrecvbuf_size > 0 ) set_recvbufsize(m_psock->GetSocket());
               prt_sockbufsize(m_psock->GetSocket());
				}
				break;
			}	
		}
		if(m_bStopped) break;

		//TCP SERVER
		if (  m_info.emode == etcpserver ){
			CTcpSocket* pSock=NULL;
			pSock=(CTcpSocket*)m_psock->Accept(); //listen
			if(m_bStopped) break;
			if( pSock == NULL ){
				LOGGER(CLogger::Error,"%s socket(Accept) error. we will try reconnect",m_szlog_hdr);
				CUserThread::SafeSleep(100);
				continue;
			}
			notify_event((void*)pSock,(int)esockopened);
			continue;
		}
		if( m_psock == NULL ){
			m_boccured_error=true;
         continue;
		}
#ifdef PI_USING_POLL
		//Received..
		nsel_value=m_psock->Poll(20);
#else
		//Received..
		nsel_value=m_psock->Select(20);
#endif
		if ( nsel_value <= 0 ) {
			if ( nsel_value < 0 ){
				LOGGER(CLogger::Error,"%s socket(select) error. we will try reconnect",m_szlog_hdr);
				m_boccured_error=true;
			}
			continue;
		}
		if( m_nalive_cnt > 0 ) m_nalive_cnt=0;

		//header Received
		ndata_len=0;
		if( recvmsg(pdata_buf,header_len)  == false ){
			continue;
		}
      //ordering
      pHdr=(SVCComMsgHdr*)pdata_buf;

      if ( m_info.ucenable_order != 0 ) pHdr->ntoh();

      ndata_len=pHdr->uiMsgLen;
      ustype=pHdr->uiType;

      ndata_len-=header_len;
      if ( ndata_len >  0 )
      {
         if( (ndata_len+header_len) > MAX_TCPDATALEN ){
            LOGGER(CLogger::Error,
                     "%s recv data size(%d) exceeds 1MB. type(0x%04x). will close socket",
                     m_szlog_hdr,ndata_len,pHdr->uiType);
            m_boccured_error=true;
            continue;
         }
         if( (ndata_len+header_len) > nmaxdata_len ){
            char tmpBuf[header_len+1];
            nmaxdata_len=ndata_len+header_len;
            memcpy(tmpBuf,pdata_buf,header_len);

            delete[] pdata_buf;
            pdata_buf=NULL;
            pdata_buf=new char[nmaxdata_len];	
            memcpy(pdata_buf,tmpBuf,header_len);
         }
         //body
         if( recvmsg(pdata_buf+header_len,ndata_len)  == false ){
            continue;
         }
      }
      if (  ustype == SVC_ALIVE )
      {
         //LOGGER(CLogger::Info,"%s recv SVC_ALIVE",m_szlog_hdr);
         continue;
      }
		recv_data(pdata_buf,ndata_len+header_len);
	}//end while

   //tcp closed
	if( m_bconnect ) gracefulclose();
	m_bEscaped=true;

	return NULL;
}
bool c_tcpsock::sendmsg(char* pmsg, int nlen){
   //check param
	if( m_psock ==NULL || m_bconnect==false  || nlen < 0) return false;
   if(m_boccured_error) return false;
   if( nlen == 0 ) return true;
	if (nlen > MAX_TCPDATALEN ){
		LOGGER(CLogger::Error,"%s send data size(%d) exceeds 1MB", m_szlog_hdr,nlen);
		return false;
	}
	m_ptcplock->Lock();
   if(m_boccured_error){
      m_ptcplock->Unlock();
      return false;
   }
	int nret=allsendrecv(pmsg,nlen,true);
	m_ptcplock->Unlock();
	if( nret != nlen ){
		m_boccured_error=true;
		LOGGER(CLogger::Error,"%s send data (%d/%d).we will close sock.", 
          m_szlog_hdr,nret,nlen);
		return false;
	}
	return true;
}
bool c_tcpsock::recvmsg(char* pmsg, int nlen){
	if( m_psock ==NULL || m_bconnect==false  || nlen < 0) return false;
   if(m_boccured_error) return false;
   if( nlen == 0 ) return true;

   if(m_boccured_error){
      return false;
   }
	int nret=allsendrecv(pmsg,nlen,false);
	if( nret != nlen ){
		m_boccured_error=true;
		LOGGER(CLogger::Error,"%s recv data (%d/%d).we will close sock.",
            m_szlog_hdr,nret,nlen);
		return false;
	}
	return true;
}

int  c_tcpsock::allsendrecv( char* pmsg, int nlen,bool bsend )
{
   int nret=0;
   bool berror=false;

   //Check parame
   if( m_psock ==NULL || m_bconnect==false  || nlen < 0) return 0; 
   if ( nlen == 0 ) return 0; 
   if(m_boccured_error) return 0;

   int ntotal=0;
   int nremain=nlen;
   int nsel=0;
   int nretry_cnt=0;
   char* szstr="send";
   if( bsend == false) szstr="recv";
	do{	
		if(m_bStopped) break;
		//send
		if ( bsend  ){
         if( m_info.ucenable_nonblock == 0x00 ){
			   nret= m_psock->Send(pmsg+ntotal,nremain);
         }else{
            //non-blocking send
			   //nret= m_psock->Send(pmsg+ntotal,nremain);
			   nret= m_psock->Send(pmsg+ntotal,nremain,MSG_DONTWAIT);
         }
		}else{
			nret= m_psock->Recv(pmsg+ntotal,nremain);
		}
      //check length
   	if (nret <= 0 ){
         if( m_info.ucenable_nonblock == 0x00 ){
            ntotal=nret;
            break;
         //non blocking mode
         }else{
            if ( errno != EWOULDBLOCK && errno != EAGAIN ){
               ntotal=nret;
               break;
            }else{
               nret=0;
            }
         }
   	}

		nretry_cnt++;
		ntotal += nret;
		nremain -= nret;

		if ( ntotal < nlen ){
			LOGGER(CLogger::Api,"%s %s sock warning. retry(%d) data(%d/%d)",
         m_szlog_hdr,szstr,nretry_cnt,ntotal,nlen);
      }

      if(m_boccured_error) break;

		if( bsend == false && (ntotal < nlen) ){
			//recv one more
         nretry_cnt=0;
         berror=false;
			while(1){
				if(m_bStopped) break;
				nretry_cnt++;
#ifdef PI_USING_POLL
		//Received..
      		nsel=m_psock->Poll(20);
#else
		//Received..
	      	nsel=m_psock->Select(20);
#endif
				if( nsel < 0 || nretry_cnt >= 10 || m_boccured_error) {
					berror=true;
					LOGGER(CLogger::Error,"%s %s sock error. retry(%d) data(%d/%d)",
                  m_szlog_hdr,szstr,nretry_cnt,ntotal,nlen);
					break;
				}
				if( nsel == 0 ) continue;
				break;
			}
         if(berror || m_bStopped ) break;
		}else if ( bsend && (ntotal < nlen) ){
         if ( m_info.ucenable_nonblock == 0x01 ) SafeSleep(5);
         if( (nretry_cnt) >= 10 ){
			   LOGGER(CLogger::Error,"%s %s sock error. exceed retry=%d, data(%d/%d)",
                  m_szlog_hdr,szstr,nretry_cnt,ntotal,nlen);
            break;
         }
      }
	}while( ntotal < nlen );
	return ntotal;
}

//////////////////////////////data queue
//constructor
c_data_proc::c_data_proc()
      :  m_pqueue(NULL),
         m_nmax_pool_cnt(t_max_element_size),
         m_ntcpidx(-1),
         m_ninternal_free(1),
         m_nid(0)
{
   strcpy(m_szloghdr,"[DATA_PROC]");
}
//distructor
c_data_proc::~c_data_proc()
{
}
///start thread
bool c_data_proc::start(int nmaxcnt_ses, int ninteral_free)
{
   if ( nmaxcnt_ses <= 0 ) return false;
   m_ninternal_free=ninteral_free;
   // the max queue number per  session
   m_nmax_pool_cnt=nmaxcnt_ses;
   m_pqueue=new  cdata_queue((m_nmax_pool_cnt*10));
#ifndef BUFFERPOOL_NONE
   //create pool
   bool bret=false;
   m_ppool=new cbuffer_pool;
   bret=m_ppool->initialize(m_nmax_pool_cnt);
   if ( !bret ){
      LOGGER(CLogger::Error,"%s fail in init buffer pool",m_szloghdr);
      return false;
   }
#endif
   return CUserThread::Start();
}
///stop thread
void c_data_proc::stop(){
   CUserThread::Stop();
}
void c_data_proc::set_id(int nid)
{
   m_nid=nid;
}
void c_data_proc::get_id(int& nid)
{
   nid=m_nid;
}
int c_data_proc::get_tcp_index()
{
   return m_ntcpidx;
}
void c_data_proc::set_tcp_index(int idx)
{
   m_ntcpidx=idx;
}
///insert data
bool c_data_proc::insert_data(char* pdata,int nlen)
{
#ifndef BUFFERPOOL_NONE
   int nret=0;
   int bret=0;
   //get buffer 
   queue_element* pbuf=NULL;
   nret=m_ppool->get_session(pbuf);
   if(nret < 0 ){
      LOGGER(CLogger::Error,"%s fail get buffer from pool",m_szloghdr);
      return false;
   }
   //buffer is full
   if(nret == 0 ){
      bret=m_ppool->increase_pool(10);
      if(!bret){
         LOGGER(CLogger::Error,"%s fail increase_pool 10",m_szloghdr);
         return false;
      }
      LOGGER(CLogger::Warn,"%s success increase_pool 10",m_szloghdr);
      nret=m_ppool->get_session(pbuf);
      if(nret < 0 ){
         LOGGER(CLogger::Error,"%s fail get buffer from pool",m_szloghdr);
         return false;
      }
   }
   if( pbuf == NULL ){
      LOGGER(CLogger::Error,"%s fail buffer is null",m_szloghdr);
      return false;
   }
   //set data
   pbuf->set_buffer(nlen,pdata);
   //insert queue
   if ( m_pqueue->Put(pbuf) != 0 )
   {
      LOGGER(CLogger::Error,"%s fail insert. may be queue full",m_szloghdr);
      m_ppool->del_session(pbuf->ses_id);
      return false;
   }
#else
   queue_element buffer;
   memset(&buffer,0x00,sizeof(buffer));

   buffer.us_cap_len=(unsigned short) nlen;
   buffer.us_real_len=buffer.us_cap_len;
   buffer.p_data=new char[nlen];
   if( buffer.p_data == NULL ){
      LOGGER(CLogger::Error,"%s fail create buffer",m_szloghdr);
      return false;
   }
   //copy
   memcpy(buffer.p_data,pdata,nlen);
   if ( m_pqueue->Put(&buffer) != 0 )
   {
      LOGGER(CLogger::Error,"%s fail insert. may be queue full",m_szloghdr);
      delete [] buffer.p_data;
      return false;
   }
#endif
   return true;
}
void* c_data_proc::ThreadProc()
{
   int nret=0;
   m_bStopped=false;
   queue_element* pbuf=NULL;
   char* pdata=new char[sizeof(queue_element)+1]; 
   pbuf=(queue_element*)pdata;

   while(!DoExit()){
      if(m_bStopped) break;
      nret=m_pqueue->Get_(pbuf);
      //is empty
      if(nret !=1 ){
//MOD START sleep 20ms-> 1ms by yun 20130725 @modsleep
        //SafeSleep(20); 
         SafeSleep(1); 
//MOD END   sleep 20ms-> 1ms by yun 20130725 @modsleep
         continue;
      }

      //recv proc
      recv_proc(pbuf);
#ifndef BUFFERPOOL_NONE
      //buffer pool
      if( m_ninternal_free == 1 ) m_ppool->del_session(pbuf->ses_id);
#else
      if( m_ninternal_free == 1 ) delete [] pbuf->p_data;
#endif

      if(m_bStopped) break;
   }
   m_bEscaped=true;
   return NULL;   
}

/////tcp server recv session

///constructor
c_tcpsvrses::c_tcpsvrses()
   :  m_nindex(0),
      m_benablesock(false),
      m_pdataqueue(NULL),
      m_nmaxid_cnt(0),
      m_ustcpattr(0),
      m_usreserved(0),
      m_nbaseid(0),
      m_nid_idx(-1),
      m_papp_idlist(NULL)
{

}
//distructor
c_tcpsvrses::~c_tcpsvrses()
{

}
//start tcp recv
bool c_tcpsvrses::start(int nidx, int nsend_alive, int nattr,st_recv_proc* precv_proc,
                        int nbaseid ,int nmaxid_cnt, st_appid_list* plist,int norder )
{
   //set param
   m_nindex=nidx;
   m_nbaseid=nbaseid;

   //set property
   m_info.emode=etcpsvrrecv;
   m_info.nissend_alive=nsend_alive;

   m_info.ucenable_order=(unsigned char)norder;

   //tcp nonblock
   //m_info.ucenable_nonblock=0x01;
   //m_info.nsendbuf_size=(32768*100);
   //m_info.nrecvbuf_size=(32768*100);

   if ( nattr == 0 ){
      sprintf(m_szlog_hdr,"[COMSVR_%02x]",m_nindex);
   }else{
      sprintf(m_szlog_hdr,"[SVCSVR_%02x]",m_nindex);
   }

   //set attribute
   m_ustcpattr=(unsigned short)nattr;

   //set queue
   m_pdataqueue=precv_proc;

   //set ses buf
   m_papp_idlist=plist;
   
   //check max id
   if ( nmaxid_cnt <= 0 ){
      LOGGER(CLogger::Error,"%s fail start. max id is <= 0 ",m_szlog_hdr);
      return false; 
   }
   m_nmaxid_cnt=nmaxid_cnt;

   bool bret=c_tcpsock::start();
   if( !bret){
       LOGGER(CLogger::Error,"%s fail start !!",m_szlog_hdr);
   }
   return bret;
}
//recv data
void c_tcpsvrses::recv_data(char* pmsg,int nlen)
{
   if( nlen <= 0 ) return;
   ///check id
   SVCComMsgHdr* phdr=(SVCComMsgHdr*)pmsg;
   bool bret=false;
   int nid=0;
   //if recv msg is component 
   if( m_ustcpattr == 0 ){
      nid=phdr->uiCompId;
   //recv msg is as
   }else if ( m_ustcpattr == 1 ){
      nid=phdr->uiAsId;
   }else{
      LOGGER(CLogger::Error,"%s fail in recv not define tcp attr(com or as)",m_szlog_hdr);
      return ; 
   }
   //minus baseid
   nid-=m_nbaseid;

   //id check
   if ( nid < 0 || nid >= m_nmaxid_cnt ){
      LOGGER(CLogger::Error,"%s fail recv msg. id(0x%04X) is invalid",
             m_szlog_hdr,(nid+m_nbaseid));
      return ; 
   } 
   //check setupapps
   if ( phdr->uiType == SVC_SETUP_APPS ){
      LOGGER(CLogger::Warn,"%s recv internal SETUP_APPS id=%d/%d", m_szlog_hdr,m_nid_idx,nid);
      if ( m_nid_idx  >= 0 ){
         LOGGER(CLogger::Warn,"%s already id set. id_idx=%d", 
            m_szlog_hdr,m_nid_idx);
         user_tcpclose();
         return;
      }else{
         m_nid_idx=nid;
         //set id buffer
         if( (m_papp_idlist[m_nid_idx].ntcpidx >= 0 && 
             m_papp_idlist[m_nid_idx].nattach_cnt > 0) ||
             m_papp_idlist[m_nid_idx].nwait_flag == 1){

            m_papp_idlist[m_nid_idx].nwait_flag = 1 ;
            (m_papp_idlist[m_nid_idx].plock)->Lock();

            LOGGER(CLogger::Error,"%s start ... already app-id(0x%04X) in use. will close proc",
               m_szlog_hdr,(nid+m_nbaseid));
            //close .. the first tcp client
            c_tcpserver* psvr=NULL;
            psvr=(c_tcpserver*)(m_papp_idlist[m_nid_idx].ptcpserver);
            if( psvr != NULL ){
               LOGGER(CLogger::Error,"%s close the first sock. index(%d)",
                  m_szlog_hdr,m_papp_idlist[m_nid_idx].ntcpidx);
               psvr->close_withtcpindex(m_papp_idlist[m_nid_idx].ntcpidx); 
            }
            int ntrycnt=0;
            while(true){
               SafeSleep(100);
               if( m_papp_idlist[m_nid_idx].ntcpidx == -1 ) break; //if closed end
               if( ntrycnt++ > 100 ) {
                  LOGGER(CLogger::Error,"%s exceed. close proc cnt(%dsec)",m_szlog_hdr,ntrycnt/10);
                  break;
               } 
            }
            LOGGER(CLogger::Error,"%s end .... already app-id(0x%04X) in use. will close proc",
               m_szlog_hdr,(nid+m_nbaseid));

            m_papp_idlist[m_nid_idx].ntcpidx=m_nindex;
            m_papp_idlist[m_nid_idx].nattach_cnt=1;

            (m_papp_idlist[m_nid_idx].plock)->Unlock();
            m_papp_idlist[m_nid_idx].nwait_flag = 0 ;
         }else{
            m_papp_idlist[m_nid_idx].ntcpidx=m_nindex;
            m_papp_idlist[m_nid_idx].nattach_cnt+=1;
         }

      }//else
   }//if ( phdr->uiType == SVC_SETUP_APPS )
   //check index
   if(m_nid_idx != nid  ) {
      LOGGER(CLogger::Warn,"%s id_idx is not equal. 0x%04X/0x%04X",
         m_szlog_hdr,m_nid_idx,nid);
      user_tcpclose();
      return;
   }

   //set tcp index
   c_data_proc* precv=(m_pdataqueue+nid)->pqueue;
   int ntcp_index=precv->get_tcp_index();
   if ( ntcp_index < 0 ){
      precv->set_tcp_index(m_nindex);
   }
   //sync error
   if ( m_papp_idlist[m_nid_idx].ntcpidx < 0 ) {
      m_papp_idlist[m_nid_idx].ntcpidx=m_nindex;
   }
   bret=precv->insert_data(pmsg,nlen);
   if(!bret){
      LOGGER(CLogger::Error,"%s fail insert recv data to queue(id=%d)",
      m_szlog_hdr,nid);
   }

}
//notify event
void c_tcpsvrses::notify_event(void* psock,int ntype)
{
   e_tcpsock_event etype=e_tcpsock_event(ntype);
   if( etype == esockclosed ){

      m_benablesock=false;

      if ( m_nid_idx < 0 || m_nid_idx >= m_nmaxid_cnt ){
         LOGGER(CLogger::Error,"%s fail insert tcp close. id is invalid",
               m_szlog_hdr);
         return;
      }
      //set id buffer
      m_papp_idlist[m_nid_idx].ntcpidx=-2; //recv closed event
      m_papp_idlist[m_nid_idx].nattach_cnt=0;

      //make meassage
      SVCComMsgHdr svc_msg;
      memset(&svc_msg,0x00,sizeof(svc_msg));
      svc_msg.uiMagicCookie=SVC_MSG_MAGIC_COOKIE;
      svc_msg.uiMsgLen=sizeof(SVCComMsgHdr);
      svc_msg.uiType=etcpclosed;
      svc_msg.uiSubType=etcpclosed;
      //if comp attr
      if( m_ustcpattr == 0 ){
         svc_msg.uiCompId=m_nid_idx+m_nbaseid;
      }else{
         svc_msg.uiAsId=m_nid_idx+m_nbaseid;
      }
      c_data_proc* precv=(m_pdataqueue+m_nid_idx)->pqueue;
      bool bret=precv->insert_data((char*)&svc_msg,svc_msg.uiMsgLen);
      if(!bret){
         LOGGER(CLogger::Error,"%s fail insert tcp close data to queue(id=%d)",
         m_szlog_hdr,m_nid_idx);
      }
      //tcp index close
      precv->set_tcp_index(-1);

      //log
      LOGGER(CLogger::Error,"%s idx(%d) is tcp closed",m_szlog_hdr,m_nid_idx);
      //tcp server recv sock closed
      m_nid_idx=-1;
   }
}

bool c_tcpsvrses::is_attach(){
   if ( m_benablesock ) return false;
   return can_attachsock();
}

void c_tcpsvrses::set_attach(CTcpSocket* psock)
{
   attachtcpsock(psock);
   m_benablesock=true;
}
bool c_tcpsvrses::is_valid()
{
   return m_benablesock;
}

void c_tcpsvrses::set_node(char* pip,int nport)
{
   strcpy(m_info.szip,pip);
   m_info.nport=nport;
}

////////tcp server session

///constructor
c_tcpserver::c_tcpserver()
   : m_nmax_sescnt(0),
     m_prcv_ses(NULL),
     m_pevent_lock(new CCritSec),
     m_pdataqueue(NULL),
     m_nbaseid(-1),
     m_papp_idlist(NULL)
{

}
///distructor
c_tcpserver::~c_tcpserver()
{

}
///start
bool c_tcpserver::start( char* szip,int nport, int ncnt,int nattr,
                        st_recv_proc* pqueue, c_data_proc* poamqueue, 
                        int nbaseid, int nsendalive, int norder)
{
   //set max session
   m_nmax_sescnt=ncnt;
   //set mode
   strcpy(m_info.szip,szip);
   m_info.nport=nport;
   m_info.nissend_alive=nsendalive;
   m_info.emode=etcpserver;


   m_info.ucenable_order=(unsigned char)norder;

   m_prcv_ses=new c_tcpsvrses[m_nmax_sescnt];

   //id buf
   m_papp_idlist=new st_appid_list[m_nmax_sescnt];
   for( int i=0; i < m_nmax_sescnt; i++)
   {
      //init list 
      m_papp_idlist[i].ntcpidx=-1;
      m_papp_idlist[i].nattach_cnt=0;
      m_papp_idlist[i].nwait_flag=0;
      m_papp_idlist[i].ptcpserver=(void*)this;
      m_papp_idlist[i].plock=m_pevent_lock;

      (m_prcv_ses+i)->start(i,nsendalive,nattr,pqueue,
                            nbaseid,m_nmax_sescnt,m_papp_idlist,norder);
      (m_prcv_ses+i)->set_node(szip,nport);
      //send, recv buffer
      if(m_info.nsendbuf_size > 0 ) (m_prcv_ses+i)->set_tcpsendbufsize(m_info.nsendbuf_size);
      if(m_info.nrecvbuf_size > 0 ) (m_prcv_ses+i)->set_tcprecvbufsize(m_info.nrecvbuf_size);
   }
   //set dataqueue
   m_pdataqueue=poamqueue;
   m_nbaseid=nbaseid;
   

   bool bret=c_tcpsock::start();
   if(!bret){
      LOGGER(CLogger::Error,"%s fail start !!",m_szlog_hdr);
   }
   return bret;
}
///stop
void c_tcpserver::stop()
{
   //stop
   for( int i=0; i < m_nmax_sescnt; i++)
   {
      (m_prcv_ses+i)->stop();
   }
   //tcp sock stop
   c_tcpsock::stop();
   LOGGER(CLogger::Info,"%s stop tcp sock",m_szlog_hdr);
}

//recv data
void c_tcpserver::recv_data(char* pmsg,int nlen)
{

}

//notify event
void c_tcpserver::notify_event(void* psock,int ntype)
{
   e_tcpsock_event etype=e_tcpsock_event(ntype);
   m_pevent_lock->Lock();
   do{
      //tcp server sock connected
      if( etype == esockreconected ){
         LOGGER(CLogger::Warn,"%s server sock(%p) connected",m_szlog_hdr,psock);
         //make meassage
         SVCComMsgHdr svc_msg;
         memset(&svc_msg,0x00,sizeof(svc_msg));
         svc_msg.uiMagicCookie=SVC_MSG_MAGIC_COOKIE;
         svc_msg.uiMsgLen=sizeof(SVCComMsgHdr);
         svc_msg.uiType=etcpreconnected;
         svc_msg.uiSubType=etcpreconnected;
         bool bret=m_pdataqueue->insert_data((char*)&svc_msg,svc_msg.uiMsgLen);
         if(!bret){
            LOGGER(CLogger::Error,"%s fail insert tcp server connect",
            m_szlog_hdr);
         }
         break;
      //tcp server sock closed
      }else if ( etype == esockclosed ){
         LOGGER(CLogger::Warn,"%s server sock closed(%p)",m_szlog_hdr,psock);
         //make meassage
         SVCComMsgHdr svc_msg;
         memset(&svc_msg,0x00,sizeof(svc_msg));
         svc_msg.uiMagicCookie=SVC_MSG_MAGIC_COOKIE;
         svc_msg.uiMsgLen=sizeof(SVCComMsgHdr);
         svc_msg.uiType=etcpclosed;
         svc_msg.uiSubType=etcpclosed;
         bool bret=m_pdataqueue->insert_data((char*)&svc_msg,svc_msg.uiMsgLen);
         if(!bret){
            LOGGER(CLogger::Error,"%s fail insert tcp server close",
            m_szlog_hdr);
         }
         break;
      }else if ( etype == esockopened ){
         bool batthached=false;
         int ncnt=3;
         while(ncnt-- > 0 )
         {
            batthached=false;
            for( int i=0 ; i < m_nmax_sescnt; i++)
            {
               if( (m_prcv_ses+i)->is_attach() )
               {
                  (m_prcv_ses+i)->set_attach((CTcpSocket*)psock);
                  batthached=true;
                  break;
               }
            }
            if( batthached ) break;
            SafeSleep(1000);
            LOGGER(CLogger::Warn,"%s search attach sock(%p), cnt(%d)",
                  m_szlog_hdr,psock,ncnt);
         }
         if( batthached )
         {
            LOGGER(CLogger::Info,"%s success sock opened & attach(%p)",
            m_szlog_hdr,psock);
         }else{
            LOGGER(CLogger::Error,"%s fail in search attach sock(%p), cnt(%d)",
                  m_szlog_hdr,psock,m_nmax_sescnt);
            ((CTcpSocket*)psock)->Close();
            delete (CTcpSocket*) psock;
            psock=NULL;
         }
      } 
   }while(0);
   m_pevent_lock->Unlock();
}
bool c_tcpserver::sendmsg_withtcpindex(int index, char* pmsg, int nlen)
{
   if ( index >= m_nmax_sescnt || index < 0  )
   {
      LOGGER(CLogger::Error,"%s fail in send msg. exceed tcp index (%d/%d)",
      m_szlog_hdr,index,m_nmax_sescnt);
      return false;
   }
   return (m_prcv_ses+index)->sendmsg(pmsg,nlen);
}

bool c_tcpserver::sendmsg_withappid(int nid, char* pmsg, int nlen)
{
   int index=nid-m_nbaseid;
   if ( index >= m_nmax_sescnt || index < 0  )
   {
      LOGGER(CLogger::Error,"%s fail in send msg. exceed max appid(%d/%d)",
      m_szlog_hdr,nid,m_nmax_sescnt);
      return false;
   }
   return sendmsg_withtcpindex(m_papp_idlist[index].ntcpidx,pmsg,nlen);
}

void c_tcpserver::update_closeproc_end(int nid)
{
   int index=nid-m_nbaseid;
   if ( index >= m_nmax_sescnt || index < 0  )
   {
      LOGGER(CLogger::Error,"%s fail in update. exceed max appid(%d/%d)",
      m_szlog_hdr,nid,m_nmax_sescnt);
      return ;
   }
   //sync error
   if ( m_papp_idlist[index].ntcpidx < 0 ){
      m_papp_idlist[index].ntcpidx=-1;
      m_papp_idlist[index].nattach_cnt=0;
   }
}
bool c_tcpserver::check_appid(int nid)
{
   int index=nid-m_nbaseid;
   if ( index >= m_nmax_sescnt || index < 0  )
   {
      LOGGER(CLogger::Error,"%s fail in set tcpindex.exceed max appid(%d/%d)",
      m_szlog_hdr,nid,m_nmax_sescnt);
      return false;
   }
   if ( m_papp_idlist[index].nattach_cnt != 1 ){
      LOGGER(CLogger::Error,"%s attach count is invalid. id=%d,cnt=%d",
      m_szlog_hdr,nid,m_papp_idlist[index].nattach_cnt);
      return false;
   }
   if ( m_papp_idlist[index].ntcpidx < 0 )
   {
      LOGGER(CLogger::Error,"%s tcp index(%d) disable",
      m_szlog_hdr,m_papp_idlist[index].ntcpidx);
      return false;
   }
   return true;
}


void c_tcpserver::close_withtcpindex(int index)
{
   if ( index >= m_nmax_sescnt || index < 0  )
   {
      LOGGER(CLogger::Error,"%s fail in send msg. exceed tcp index (%d/%d)",
      m_szlog_hdr,index,m_nmax_sescnt);
      return;
   }
   (m_prcv_ses+index)->user_tcpclose();
}

///////tcp client

///constructor
c_tcpclient::c_tcpclient()
   : m_pevent_lock(new CCritSec),
     m_pdataqueue(NULL)
{

}
///distructor
c_tcpclient::~c_tcpclient()
{

}
///start
bool c_tcpclient::start( char* szip,int nport,c_data_proc* pqueue,int nsendalive,int norder)
{
   //set mode
   strcpy(m_info.szip,szip);
   m_info.nport=nport;
   m_info.nissend_alive=nsendalive;
   m_info.emode=etcpclient;

   m_info.ucenable_order=(unsigned char)norder;
   //tcp nonblock
   //m_info.ucenable_nonblock=0x01;
   //m_info.nsendbuf_size=(32768*100);
   //m_info.nrecvbuf_size=(32768*100);

   m_pdataqueue=pqueue;
   bool bret=c_tcpsock::start();
   if(!bret){
      LOGGER(CLogger::Error,"%s fail start !!",m_szlog_hdr);
   }
   return bret;
}
//recv data
void c_tcpclient::recv_data(char* pmsg,int nlen)
{
   if( nlen <= 0) return;
   bool bret=m_pdataqueue->insert_data(pmsg,nlen);
   if(!bret){
       LOGGER(CLogger::Error,"%s fail insert recv data to queue",m_szlog_hdr);
   }
}
//notify event
void c_tcpclient::notify_event(void* psock,int ntype)
{
   e_tcpsock_event etype=e_tcpsock_event(ntype);
   m_pevent_lock->Lock();
   do{
      //tcp server sock connected
      if( etype == esockreconected ){
         LOGGER(CLogger::Warn,"%s client sock(%p) connected",m_szlog_hdr,psock);
         //make meassage
         SVCComMsgHdr svc_msg;
         memset(&svc_msg,0x00,sizeof(svc_msg));
         svc_msg.uiMagicCookie=SVC_MSG_MAGIC_COOKIE;
         svc_msg.uiMsgLen=sizeof(SVCComMsgHdr);
         svc_msg.uiType=etcpreconnected;
         svc_msg.uiSubType=etcpreconnected;
         bool bret=m_pdataqueue->insert_data((char*)&svc_msg,svc_msg.uiMsgLen);
         if(!bret){
            LOGGER(CLogger::Error,"%s fail insert tcp client connect",
            m_szlog_hdr);
         }
         break;
      //tcp server sock closed
      }else if ( etype == esockclosed ){
         LOGGER(CLogger::Warn,"%s client sock closed(%p)",m_szlog_hdr,psock);
         //make meassage
         SVCComMsgHdr svc_msg;
         memset(&svc_msg,0x00,sizeof(svc_msg));
         svc_msg.uiMagicCookie=SVC_MSG_MAGIC_COOKIE;
         svc_msg.uiMsgLen=sizeof(SVCComMsgHdr);
         svc_msg.uiType=etcpclosed;
         svc_msg.uiSubType=etcpclosed;
         bool bret=m_pdataqueue->insert_data((char*)&svc_msg,svc_msg.uiMsgLen);
         if(!bret){
            LOGGER(CLogger::Error,"%s fail insert tcp client close",
            m_szlog_hdr);
         }
         break;
      }
   }while(0);
   m_pevent_lock->Unlock();
}




#undef INIT_TCPDATALEN 
#undef MAX_TCPDATALEN

#undef MAX_ALIVECNT
#undef ALIVE_INTERVAL_TIME
