#ifndef __AL_TCP_IF_H__
#define __AL_TCP_IF_H__

#include "aloxUtil.h"
#include "common_msg.h"
#include "usrpool.h"
#include "buf_traits.h"

enum e_tcpsock_mode{
	etcpserver          = 0x00,
	etcpclient          = 0x01,
	etcpsvrrecv         = 0x02
};

enum e_tcpsock_event{
	esockopened         = 0x00,
	esockclosed         = 0x01,
	esockreconected     = 0x02,
	esockdata           = 0x03
};

struct st_tcp_info{
   enum { e_maxnum_ip=28 };
	e_tcpsock_mode emode;
	unsigned char ucenable_nonblock;
	unsigned char ucenable_order;
	char szip[e_maxnum_ip];
	int nport;
   int nissend_alive;

   int nsendbuf_size;
   int nrecvbuf_size;
};

///defines tcp error event
enum e_tcperr_msgtype
{
   etcpclosed=0xFFE1,
   etcpreconnected=0xFFE2,
   etcperr_usingid_setup=0xFFE3
};
class c_tcp_ping
   : public CUserThread
{
public:
   c_tcp_ping();
   ~c_tcp_ping();
   bool start(char* phdr, void* phandler,int norder);
   void setflag(bool bvalue){
      m_bstart=bvalue;
   }
protected:
   void* ThreadProc();
private:
   void* m_phandler;
   char  m_szhdr[28];
   bool  m_bstart;
   bool  m_border;

};

///class tcp socket
class c_tcpsock 
   : public CUserThread
{
public:
	c_tcpsock();
	virtual ~c_tcpsock()
   {
      if( m_psock ) delete m_psock;
      if( m_ptcpcond ) delete m_ptcpcond;
      if( m_ptcplock ) delete m_ptcplock;
   }
	void set_property(st_tcp_info* pinfo);
	bool start();
	void stop();
	bool can_attachsock();
	void attachtcpsock(CTcpSocket* pSock);
	bool sendmsg(char* pmsg, int nlen);
   st_tcp_info* get_info()
   {
      return &m_info;
   }
   bool is_connected()
   {
      return m_bconnect;
   }
   void user_tcpclose(){
      m_boccured_error=true;
      m_nalive_cnt=0;
   }

	void decide_reconnect();
protected:
   //virtual func.
	virtual void recv_data(char* pmsg,int nlen){}
	virtual void notify_event(void* psock,int ntype){}

	void *ThreadProc();
	void set_connect(bool bconnect);
	bool prepare_sock();
	int  allsendrecv(char* pmsg, int nlen, bool bsend);
	bool recvmsg(char* pmsg, int nlen);
	void gracefulclose();
   //set nonblock
   void set_nonblock(int fd);
   void prt_sockbufsize(int fd);
   void set_sendbufsize(int fd);
   void set_recvbufsize(int fd);
protected:
	bool m_bconnect;	
	bool m_boccured_error;
	bool m_bwaiting;
	st_tcp_info m_info;
	int m_nalive_cnt;
	char m_szlog_hdr[128];
private:
	CCritSec* m_ptcplock;
	CTcpSocket* m_psock;
	CCondition* m_ptcpcond;
   c_tcp_ping* m_pping;
};

///data proc
class c_data_proc
   : public CUserThread
{
public:
   enum { t_buf_size = ((1024*2)+256) };       //max packet size
   enum { t_max_element_size = (1024*5) };     //max buffer pool size, 5000
   enum { t_max_data_queue_size = (t_max_element_size*10) }; //max recv queue size, 50,000 call
   ///defines queue element
   typedef al_buffer_traits<t_buf_size> queue_element;

   ///defines data queue
   typedef CQueue<queue_element*,queue_element> cdata_queue;

   ///defines buffer pool
   typedef cuser_pool< queue_element, t_max_element_size > cbuffer_pool;

public:
   //constructor
   c_data_proc();
   //distructor
   virtual ~c_data_proc();
   ///start thread
   bool start( int nmaxcnt_ses=t_max_element_size, int ninteral_free=1);
   ///stop thread
   void stop();
   ///insert data
   bool insert_data(char* pdata,int nlen);
   void set_id(int nid);
   void get_id(int& nid);
   int  get_tcp_index();
   void set_tcp_index(int idx);
#ifndef BUFFERPOOL_NONE
   void del_buffer(queue_element* pbuf){
      if( m_ninternal_free == 1 ) return;
      m_ppool->del_session(pbuf->ses_id);
   }
   void del_buffer(int nid){
      if( m_ninternal_free == 1 ) return;
      cbuffer_pool::id_type buf_id=nid;
      m_ppool->del_session(buf_id);
   }
#endif
protected:
  virtual void recv_proc(queue_element* pbuf){};
  void* ThreadProc();
protected:
   cdata_queue*   m_pqueue;
   cbuffer_pool*  m_ppool;
   int            m_nmax_pool_cnt;
   //tcp index
   int            m_ntcpidx;
   int            m_ninternal_free;
   //data queue id
   int m_nid;
   //loghdr
   char m_szloghdr[20];
};

///struct queue
struct st_recv_proc
{
   c_data_proc* pqueue;
};
struct st_appid_list
{
   int ntcpidx;
   unsigned short nattach_cnt; 
   unsigned short nwait_flag; 
   void* ptcpserver;
   CCritSec* plock;
};

///tcp server recv session
class c_tcpsvrses
   : public c_tcpsock
{
public:
   ///constructor
   c_tcpsvrses();
   //distructor
   ~c_tcpsvrses();
   //start tcp recv
   bool start(int nidx, int nsend_alive, int nattr, st_recv_proc* precv_proc, 
              int nbaseid,int nmaxid_idx,st_appid_list* plist,int norder);

   //can attach
   bool is_attach();
   //set attach
   void set_attach(CTcpSocket* psock);
   //valid socket
   bool is_valid();
   //set node
   void set_node(char* pip,int nport);
   void set_tcpsendbufsize(int nlen){
      if(nlen > 0 ) m_info.nsendbuf_size=nlen;
   }
   void set_tcprecvbufsize(int nlen){
      if(nlen > 0 ) m_info.nrecvbuf_size=nlen;
   }
protected:
   //recv data
   void recv_data(char* pmsg,int nlen);
   //notify event
   void notify_event(void* psock,int ntype);
protected:
   int            m_nindex; 
   bool           m_benablesock;
   st_recv_proc*  m_pdataqueue;
   int            m_nmaxid_cnt; //id max index

   //tcp atttribute (0:component, 1:app )
   unsigned short m_ustcpattr;
   unsigned short m_usreserved;
   int            m_nbaseid;
   int            m_nid_idx; // id index: 0 ~

   //id buffer
   st_appid_list*  m_papp_idlist;
};

///tcp server
class c_tcpserver
   : public c_tcpsock
{

public:
   ///constructor
   c_tcpserver();
   ///distructor
   virtual ~c_tcpserver();
   ///start
   bool start( char* szip,int nport, int ncnt,int nattr, st_recv_proc* pqueue, 
               c_data_proc* poamqueue, int nbaseid, int nsendalive,int norder);
   //set tcp buffer size
   void set_tcpsendbufsize(int nlen){
      if(nlen > 0 ) m_info.nsendbuf_size=nlen;
   }
   void set_tcprecvbufsize(int nlen){
      if(nlen > 0 ) m_info.nrecvbuf_size=nlen;
   }
   //stop
   void stop();

   ///send message
   bool sendmsg_withtcpindex(int index, char* pmsg, int nlen);
   bool sendmsg_withappid(int nid, char* pmsg, int nlen);
   //check app-id
   bool check_appid(int nid);
   //close sock
   void close_withtcpindex(int index);

   //update close end
   void update_closeproc_end(int nid);

protected:
   //recv data
   void recv_data(char* pmsg,int nlen);
   //notify event
   void notify_event(void* psock,int ntype);
protected:
   int               m_nmax_sescnt;
   c_tcpsvrses*      m_prcv_ses;   
   CCritSec*         m_pevent_lock;
   c_data_proc*      m_pdataqueue;
   int               m_nbaseid;
   st_appid_list*    m_papp_idlist; //to find tcp index with app-id;
};


///tcp client
class c_tcpclient
   : public c_tcpsock
{

public:
   ///constructor
   c_tcpclient();
   ///distructor
   virtual ~c_tcpclient();
   void set_tcpsendbufsize(int nlen){
      if(nlen > 0 ) m_info.nsendbuf_size=nlen;
   }
   void set_tcprecvbufsize(int nlen){
      if(nlen > 0 ) m_info.nrecvbuf_size=nlen;
   }
   ///start
   bool start( char* szip,int nport,c_data_proc* pqueue,int nsendalive,int norder);
protected:
   //recv data
   void recv_data(char* pmsg,int nlen);
   //notify event
   void notify_event(void* psock,int ntype);

protected:
   CCritSec*    m_pevent_lock;
   c_data_proc* m_pdataqueue;
};

#endif
