#ifndef SIP_IF_DEFS
#define SIP_IF_DEFS
#include "Sam_Com.h"
#include "common_msg.h"

#define MAX_SDP_OPT_NUM     28
#define MAX_SIP_OPT_NUM       16

/***************** Flexible Data Struct **********************/
//---------> Flex Control Header
//   1. SIP FLEX
typedef struct
{
	char k_cnt;char d_cnt;char len[2];char key[MAX_SIP_OPT_NUM*2];    
	char kpos[MAX_SIP_OPT_NUM*2];char dpos[MAX_SIP_OPT_NUM*2];     
	int GetCtlSize(){return (MAX_SIP_OPT_NUM*6 + 4);}
}st_sip_ctl;
typedef struct
{
	st_sip_ctl ctl;
	char *data(){return (char *)(this + 1);}
	int GetKey(FX_IN int i){return Fx_Get_Short(&(ctl.key[i*2]));}
	void SetKey(FX_IN int mpos){Fx_Set_Short(mpos, &(ctl.key[ctl.k_cnt*2]));return;}
	int GetKPos(FX_IN int i){return Fx_Get_Short(&(ctl.kpos[i*2]));}
	void SetKPos(FX_IN int mpos){Fx_Set_Short(mpos, &(ctl.kpos[ctl.k_cnt*2]));return;}
	int GetDPos(FX_IN int i){return Fx_Get_Short(&(ctl.dpos[i*2]));}
	void SetDPos(FX_IN int mpos){Fx_Set_Short(mpos, &(ctl.dpos[ctl.d_cnt*2]));return;}
	int GetOptLen(){return Fx_Get_Short(&(ctl.len[0]));}
	void SetOptLen(FX_IN int mlen){Fx_Set_Short(mlen, &(ctl.len[0]));return;}
	int GetKOptCnt(){return (ctl.k_cnt);}
	int GetDOptCnt(){return (ctl.d_cnt);}
	char * GetKOption(FX_IN int i){if(GetKOptCnt() <= i) return FX_NULL;return (char *)&(data()[GetKPos(i)]);}
	char * GetDOption(FX_IN int i){if(GetDOptCnt() <= i) return FX_NULL;return (char *)&(data()[GetDPos(i)]);}
	void SetOption(FX_IN int mkey,FX_IN char * key_str,FX_IN char * op)
	{
		if(GetDOptCnt() == MAX_SIP_OPT_NUM) return;
		if (ctl.d_cnt == 0x00)
		{
			SetKey(mkey);SetKPos(0);
			if(key_str != FX_NULL)
			{memcpy(&(data()[0]),key_str,Fx_Strlen(key_str));SetOptLen((1 + Fx_Strlen(key_str)));}
			else SetOptLen(1);
			ctl.k_cnt = 1;	SetDPos(GetOptLen());
			if(op != FX_NULL)
			{memcpy(&(data()[GetOptLen()]),op,Fx_Strlen(op));SetOptLen((GetOptLen() + Fx_Strlen(op) + 1));}
			else SetOptLen(GetOptLen()+1);
			ctl.d_cnt = 1;
		}
		else
		{
			SetKey(mkey);SetKPos(GetOptLen());
			if(key_str != FX_NULL)
			{memcpy(&(data()[GetOptLen()]),key_str,Fx_Strlen(key_str));SetOptLen((GetOptLen() + Fx_Strlen(key_str) + 1));}
			else SetOptLen((GetOptLen() + 1));
			(ctl.k_cnt) ++;SetDPos(GetOptLen());
			if(op != FX_NULL)
			{memcpy(&(data()[GetOptLen()]),op,Fx_Strlen(op));SetOptLen((GetOptLen() + Fx_Strlen(op) + 1));}
			else SetOptLen((GetOptLen() + 1));
			(ctl.d_cnt) ++;		
		}
		return;
	}
	void SetOption_2(FX_IN int mkey,FX_IN char * key_str,FX_IN char * op,FX_IN int cp_len)
	{
		if(GetDOptCnt() == MAX_SDP_OPT_NUM) return;
		if (ctl.d_cnt == 0x00)
		{
			SetKey(mkey);SetKPos(0);
			if(key_str != FX_NULL)
			{memcpy(&(data()[0]),key_str,Fx_Strlen(key_str));SetOptLen((1 + Fx_Strlen(key_str)));}
			else SetOptLen(1);
			ctl.k_cnt = 1;	SetDPos(GetOptLen());
			if(op != FX_NULL)
			{memcpy(&(data()[GetOptLen()]),op,cp_len);SetOptLen((GetOptLen() + cp_len + 1));}
			else SetOptLen(GetOptLen()+1);
			ctl.d_cnt = 1;
		}
		else
		{
			SetKey(mkey);SetKPos(GetOptLen());
			if(key_str != FX_NULL)
			{memcpy(&(data()[GetOptLen()]),key_str,Fx_Strlen(key_str));SetOptLen((GetOptLen() + Fx_Strlen(key_str) + 1));}
			else SetOptLen((GetOptLen() + 1));
			(ctl.k_cnt) ++;SetDPos(GetOptLen());
			if(op != FX_NULL)
			{memcpy(&(data()[GetOptLen()]),op,cp_len);SetOptLen((GetOptLen() + cp_len + 1));}
			else SetOptLen((GetOptLen() + 1));
			(ctl.d_cnt) ++;		
		}
		return;
	}
	void Fx_Flex_Add(FX_IN int mkey,FX_IN char * key_str,FX_IN const char * op,...)
	{
		char *Cur_Data = FX_NULL;
		if(GetDOptCnt() == MAX_SIP_OPT_NUM) return;
		if (ctl.d_cnt == 0x00)
		{
			SetKey(mkey);SetKPos(0);
			if(key_str != FX_NULL)
			{memcpy(&(data()[0]),key_str,Fx_Strlen(key_str));SetOptLen((1 + Fx_Strlen(key_str)));}
			else SetOptLen(1);
			ctl.k_cnt = 1;	SetDPos(GetOptLen());	Cur_Data = &(data()[GetOptLen()]);
			va_list ap;va_start(ap, op);	vsprintf(Cur_Data, op, ap);	va_end(ap);
			SetOptLen(GetOptLen()+Fx_Strlen(Cur_Data) + 1);	ctl.d_cnt = 1;
		}
		else
		{
			SetKey(mkey);SetKPos(GetOptLen());
			if(key_str != FX_NULL)
			{memcpy(&(data()[GetOptLen()]),key_str,Fx_Strlen(key_str));SetOptLen((GetOptLen() + Fx_Strlen(key_str) + 1));}
			else SetOptLen((GetOptLen() + 1));
			(ctl.k_cnt) ++;SetDPos(GetOptLen());Cur_Data = &(data()[GetOptLen()]);
			va_list ap;va_start(ap, op);	vsprintf(Cur_Data, op, ap);va_end(ap);
			SetOptLen(GetOptLen()+Fx_Strlen(Cur_Data) + 1);	(ctl.d_cnt) ++;		
		}
		return;
	}
	char * Find_1(FX_IN int mType)
	{
		for(int i=0;i<GetKOptCnt();i++){if(GetKey(i) == mType) return GetDOption(i);}
		return FX_NULL;
	}
	char * Find_2(FX_IN char *pkey)
	{
		if(pkey == FX_NULL) return FX_NULL;
		for(int i=0;i<GetKOptCnt();i++)
		{
			if(GetKOption(i) != FX_NULL)
			{if(Fx_StrCmp(GetKOption(i),pkey)) return GetDOption(i);}
		}
		return FX_NULL;
	}
	int GetFlexSize()
	{if(ctl.d_cnt == 0x00) return ctl.GetCtlSize();else return ctl.GetCtlSize() + GetOptLen();}
}st_flexible_sip;
//   2. SDP FLEX
typedef struct
{
	char k_cnt;char d_cnt;char len[2];char key[MAX_SDP_OPT_NUM*2];     
	char kpos[MAX_SDP_OPT_NUM*2];char dpos[MAX_SDP_OPT_NUM*2];       
	int GetCtlSize(){return (MAX_SDP_OPT_NUM*6 + 4);};
}st_sdp_ctl;
typedef struct
{
	st_sdp_ctl ctl;
	char *data(){return (char *)(this + 1);}
	int GetKey(FX_IN int i){return Fx_Get_Short(&(ctl.key[i*2]));}
	void SetKey(FX_IN int mpos){Fx_Set_Short(mpos, &(ctl.key[ctl.k_cnt*2]));return;}
	int GetKPos(FX_IN int i){return Fx_Get_Short(&(ctl.kpos[i*2]));}
	void SetKPos(FX_IN int mpos){Fx_Set_Short(mpos, &(ctl.kpos[ctl.k_cnt*2]));return;}
	int GetDPos(FX_IN int i){return Fx_Get_Short(&(ctl.dpos[i*2]));}
	void SetDPos(FX_IN int mpos){Fx_Set_Short(mpos, &(ctl.dpos[ctl.d_cnt*2]));return;}
	int GetOptLen(){return Fx_Get_Short(&(ctl.len[0]));}
	void SetOptLen(FX_IN int mlen){Fx_Set_Short(mlen, &(ctl.len[0]));return;}
	int GetKOptCnt(){return (ctl.k_cnt);}
	int GetDOptCnt(){return (ctl.d_cnt);}
	char * GetKOption(FX_IN int i){if(GetKOptCnt() <= i) return FX_NULL;return (char *)&(data()[GetKPos(i)]);}
	char * GetDOption(FX_IN int i){if(GetDOptCnt() <= i) return FX_NULL;return (char *)&(data()[GetDPos(i)]);}
	void SetOption(FX_IN int mkey,FX_IN char * key_str,FX_IN char * op)
	{
		if(GetDOptCnt() == MAX_SDP_OPT_NUM) return;
		if (ctl.d_cnt == 0x00)
		{
			SetKey(mkey);SetKPos(0);
			if(key_str != FX_NULL)
			{memcpy(&(data()[0]),key_str,Fx_Strlen(key_str));SetOptLen((1 + Fx_Strlen(key_str)));}
			else SetOptLen(1);
			ctl.k_cnt = 1;	SetDPos(GetOptLen());
			if(op != FX_NULL)
			{memcpy(&(data()[GetOptLen()]),op,Fx_Strlen(op));SetOptLen((GetOptLen() + Fx_Strlen(op) + 1));}
			else SetOptLen(GetOptLen()+1);
			ctl.d_cnt = 1;
		}
		else
		{
			SetKey(mkey);SetKPos(GetOptLen());
			if(key_str != FX_NULL)
			{memcpy(&(data()[GetOptLen()]),key_str,Fx_Strlen(key_str));SetOptLen((GetOptLen() + Fx_Strlen(key_str) + 1));}
			else SetOptLen((GetOptLen() + 1));
			(ctl.k_cnt) ++;SetDPos(GetOptLen());
			if(op != FX_NULL)
			{memcpy(&(data()[GetOptLen()]),op,Fx_Strlen(op));SetOptLen((GetOptLen() + Fx_Strlen(op) + 1));}
			else SetOptLen((GetOptLen() + 1));
			(ctl.d_cnt) ++;		
		}
		return;
	}
	void SetOption_2(FX_IN int mkey,FX_IN char * key_str,FX_IN char * op,FX_IN int cp_len)
	{
		if(GetDOptCnt() == MAX_SDP_OPT_NUM) return;
		if (ctl.d_cnt == 0x00)
		{
			SetKey(mkey);SetKPos(0);
			if(key_str != FX_NULL)
			{memcpy(&(data()[0]),key_str,Fx_Strlen(key_str));SetOptLen((1 + Fx_Strlen(key_str)));}
			else SetOptLen(1);
			ctl.k_cnt = 1;	SetDPos(GetOptLen());
			if(op != FX_NULL)
			{memcpy(&(data()[GetOptLen()]),op,cp_len);SetOptLen((GetOptLen() + cp_len + 1));}
			else SetOptLen(GetOptLen()+1);
			ctl.d_cnt = 1;
		}
		else
		{
			SetKey(mkey);SetKPos(GetOptLen());
			if(key_str != FX_NULL)
			{memcpy(&(data()[GetOptLen()]),key_str,Fx_Strlen(key_str));SetOptLen((GetOptLen() + Fx_Strlen(key_str) + 1));}
			else SetOptLen((GetOptLen() + 1));
			(ctl.k_cnt) ++;SetDPos(GetOptLen());
			if(op != FX_NULL)
			{memcpy(&(data()[GetOptLen()]),op,cp_len);SetOptLen((GetOptLen() + cp_len + 1));}
			else SetOptLen((GetOptLen() + 1));
			(ctl.d_cnt) ++;		
		}
		return;
	}
	void Fx_Flex_Add(FX_IN int mkey,FX_IN char * key_str,FX_IN const char * op,...)
	{
		char *Cur_Data = FX_NULL;
		if(GetDOptCnt() == MAX_SDP_OPT_NUM) return;
		if (ctl.d_cnt == 0x00)
		{
			SetKey(mkey);SetKPos(0);
			if(key_str != FX_NULL)
			{memcpy(&(data()[0]),key_str,Fx_Strlen(key_str));SetOptLen((1 + Fx_Strlen(key_str)));}
			else SetOptLen(1);
			ctl.k_cnt = 1;	SetDPos(GetOptLen());	Cur_Data = &(data()[GetOptLen()]);
			va_list ap;va_start(ap, op);vsprintf(Cur_Data, op, ap);va_end(ap);
			SetOptLen(GetOptLen()+Fx_Strlen(Cur_Data) + 1);	ctl.d_cnt = 1;
		}
		else
		{
			SetKey(mkey);SetKPos(GetOptLen());
			if(key_str != FX_NULL)
			{memcpy(&(data()[GetOptLen()]),key_str,Fx_Strlen(key_str));SetOptLen((GetOptLen() + Fx_Strlen(key_str) + 1));}
			else SetOptLen((GetOptLen() + 1));
			(ctl.k_cnt) ++;SetDPos(GetOptLen());Cur_Data = &(data()[GetOptLen()]);
			va_list ap;va_start(ap, op);	vsprintf(Cur_Data, op, ap);va_end(ap);
			SetOptLen(GetOptLen()+Fx_Strlen(Cur_Data) + 1);	(ctl.d_cnt) ++;		
		}
		return;
	}
	char * Find_1(FX_IN int mType)
	{
		for(int i = 0;i<GetKOptCnt();i++)	{if(GetKey(i) == mType) return GetDOption(i);}
		return FX_NULL;
	}
	char * Find_2(FX_IN char *pkey)
	{
		if(pkey == FX_NULL) return FX_NULL;
		for(int i=0;i<GetKOptCnt();i++)
		{
			if(GetKOption(i) != FX_NULL)
			{if(Fx_StrCmp(GetKOption(i),pkey)) return GetDOption(i);}
		}
		return FX_NULL;
	}
	int GetFlexSize()
	{
		if(ctl.d_cnt == 0x00) return ctl.GetCtlSize();
		else return ctl.GetCtlSize() + GetOptLen();
	}
}st_flexible_sdp;
inline char * FindFlex_Add_3(FX_IN char *ptkey,FX_IN int mpt,FX_IN int mType, FX_IN st_flexible_sdp *flex)
{
	for(int i=0;i<flex->GetDOptCnt(); i++)
	{if(ptkey[i] == mpt){if(flex->GetKey(i) == mType) return flex->GetDOption(i);}}
	return FX_NULL;
}
inline char * FindFlex_Add_4(FX_IN char *ptkey,FX_IN int mpt,FX_IN char *pkey,FX_IN st_flexible_sdp *flex)
{
	if(pkey == FX_NULL) return FX_NULL;
	for(int i=0;i<flex->GetDOptCnt(); i++)
	{if(ptkey[i] == mpt){if(Fx_StrCmp(flex->GetKOption(i),pkey)) return flex->GetDOption(i);}}
	return FX_NULL;
}
/***************** Contents Structures **********************/
typedef struct
{
	char m_cnt;
	char sdp_ip[4];
	char m_num[MAX_SDP_OPT_NUM];                      // Additional Flexible Control Data 
	char pt[MAX_SDP_OPT_NUM];                           // Additional Flexible Control Data
	st_media_descriptor desc[4];
	st_flexible_sdp flex;                   // ex> Flexible Opt String Porcess    
	#ifndef FX_SDP_ADD
	#define FX_SDP_ADD flex.Fx_Flex_Add
	#endif
	int Getkey(FX_IN int i){return flex.GetKey(i);}
	int GetPTkey(FX_IN int i){return (unsigned char)(pt[i]);}     
	void SetPTkey(FX_IN int mpt){pt[flex.GetKOptCnt()] = (char)(mpt);return;}      
	int GetMediakey(FX_IN int i){return m_num[i];}
	void SetMediakey(FX_IN int mpt){m_num[flex.GetKOptCnt()] = (char)(mpt);return;}   
	int GetKPos(FX_IN int i){return flex.GetKPos(i);}
	int GetDPos(FX_IN int i){return flex.GetDPos(i);}
	int GetOptLen(){return flex.GetOptLen();}
	int GetKOptCnt(){return flex.GetKOptCnt();}
	int GetDOptCnt(){return flex.GetDOptCnt();}
	char * GetKOption(FX_IN int i){return flex.GetKOption(i);}
	char * GetDOption(FX_IN int i){return flex.GetDOption(i);}
	int GetStringType(FX_IN int i)
	{
		if(((unsigned char)(pt[i]) == 0xff) &&((unsigned char)(m_num[i]) == 0xff)) return STRING_SDP_COMMON;
		else if(((unsigned char)(pt[i]) == 0xff) &&((unsigned char)(m_num[i]) != 0xff)) return STRING_SDP_MEDIA;
		else if(((unsigned char)(pt[i]) != 0xff) &&((unsigned char)(m_num[i]) == 0xff)) return STRING_SDP_PAYLOAD;
		return STRING_SDP_NONE;
	}
	char * Find_Common_1(FX_IN int mType){return flex.Find_1(mType);}
	char * Find_Common_2(FX_IN char *pkey){return flex.Find_2(pkey);}
	char * Find_Msrp_Attribute(FX_IN int m_arr,FX_IN char *pkey){return FindFlex_Add_4(&m_num[0],m_arr, pkey, &flex);}
	char * Find_Media_1(FX_IN int m_arr,FX_IN int mType){return FindFlex_Add_3(&m_num[0], m_arr,mType, &flex);}
	char * Find_Media_2(FX_IN int m_arr,FX_IN char *pkey){return FindFlex_Add_4(&m_num[0],m_arr, pkey, &flex);}
	char * Find_Media_PT(FX_IN int m_arr,FX_IN int mpt,FX_IN int mType)
	{
		for(int i=0;i<flex.GetDOptCnt(); i++)
		{
			if(pt[i] == mpt && m_num[i] == m_arr && flex.GetKey(i) == mType)
			{
				return flex.GetDOption(i);
			}
		}
		return FX_NULL;
	}
	char * Find_PT_Framesize(FX_IN int mpt){return FindFlex_Add_3(&pt[0], mpt,OPT_SDP_ATTRIBUTE_FRAMESIZE, &flex);}
	char * Find_PT_Framerate(FX_IN int mpt){return FindFlex_Add_3(&pt[0], mpt,OPT_SDP_ATTRIBUTE_FRAMERATE, &flex);}
	char * Find_PT_1(FX_IN int mpt,FX_IN int mType){return FindFlex_Add_3(&pt[0], mpt,mType, &flex);}
	char * Find_PT_2(FX_IN int mpt,FX_IN char *pkey){return FindFlex_Add_4(&pt[0],mpt, pkey, &flex);}
	char * Find_PT_Codec(FX_IN int mpt){return FindFlex_Add_3(&pt[0], mpt,OPT_SDP_ATTRIBUTE_RTPMAP, &flex);}
	char * Find_PT_Fmtp(FX_IN int mpt){return FindFlex_Add_3(&pt[0], mpt,OPT_SDP_ATTRIBUTE_FMTP, &flex);}
	char * Find_Common_UserName(){return flex.Find_1(OPT_SDP_COMMON_O_NAME);}
	char * Find_Common_SessionID(){return flex.Find_1(OPT_SDP_COMMON_O_SESSION);}
	bool IsMultiFrameRate()
	{
		unsigned int ret = 0;
		for(int i = 0;i<flex.GetDOptCnt();i++)
		{if(flex.GetKey(i) == OPT_SDP_ATTRIBUTE_FRAMERATE) ret++;}
		if(ret >= 2) return true;
		else return false;
	}
	bool Find_PT_Fmtp_AMR_Sub(FX_INOUT st_tok_param *m_param,FX_IN int mpt,FX_OUT char *modeset, FX_OUT char *octetalign)
	{
		char * pFmtp = Find_PT_Fmtp(mpt);
		if(pFmtp != FX_NULL)
		{
			m_param->Parse(TOK_PARSE_CHAR,(char*)" =;",pFmtp);
			for(unsigned int i = 0;i<m_param->pos_cnt;i++)
			{
				if(Fx_StrCmp((char*)"mode-set",m_param->GetParam(i))) modeset = m_param->GetParam(i+1);
				else if(Fx_StrCmp((char*)"octet-align",m_param->GetParam(i))) octetalign = m_param->GetParam(i+1);
			}			
			return true;
		}
		else return false;
	}
	int Get_Media_Count(){return m_cnt;}
	int Get_IP(){return Fx_Get_Int(&sdp_ip[0]);}
	void Set_Media_Count(FX_IN int val){m_cnt = (unsigned char)val;return;}
	void Set_IP(FX_IN int val){Fx_Set_Int(val, &sdp_ip[0]);return;}
	void Set_Common_1(FX_IN int mkey,FX_IN char * op)
	{
		if(flex.GetDOptCnt() == MAX_SDP_OPT_NUM) return;
		SetPTkey(0xff);SetMediakey(0xff);flex.SetOption(mkey,FX_NULL,op);return;
	}
	void Set_Common_2(FX_IN char * key_str,FX_IN char * op)
	{
		if(flex.GetDOptCnt() == MAX_SDP_OPT_NUM) return;
		SetPTkey(0xff);SetMediakey(0xff);flex.SetOption(OPT_OTHER,key_str,op);return;
	}
	void Set_Common_3(FX_IN char * key_str,FX_IN char * op,FX_IN unsigned int cp_len)
	{
		if(flex.GetDOptCnt() == MAX_SDP_OPT_NUM) return;
		SetPTkey(0xff);SetMediakey(0xff);flex.SetOption_2(OPT_OTHER,key_str,op,cp_len);return;
	}
	void Set_Common_UserName(FX_IN char * op)
	{
		if(flex.GetDOptCnt() == MAX_SDP_OPT_NUM) return;
		SetPTkey(0xff);SetMediakey(0xff);flex.SetOption(OPT_SDP_COMMON_O_NAME,FX_NULL,op);return;
	}
	void Set_Common_SessionID(FX_IN char * op)
	{
		if(flex.GetDOptCnt() == MAX_SDP_OPT_NUM) return;
		SetPTkey(0xff);SetMediakey(0xff);flex.SetOption(OPT_SDP_COMMON_O_SESSION,FX_NULL,op);return;
	}
	void Set_Msrp_Attribute(FX_IN int m_arr,FX_IN char * key_str, FX_IN char * op)
	{
		if(flex.GetDOptCnt() == MAX_SDP_OPT_NUM) return;
		SetPTkey(0xff);SetMediakey(m_arr);flex.SetOption(OPT_SDP_ATTRIBUTE_OTHER,key_str,op);return;
	}
	void Set_Media_1(FX_IN int m_arr,FX_IN int mkey,FX_IN char * op)
	{
		if(flex.GetDOptCnt() == MAX_SDP_OPT_NUM) return;
		SetPTkey(0xff);SetMediakey(m_arr);flex.SetOption(mkey,FX_NULL,op);return;
	}
	void Set_Media_2(FX_IN int m_arr,FX_IN char * key_str,FX_IN char * op)
	{
		if(flex.GetDOptCnt() == MAX_SDP_OPT_NUM) return;
		SetPTkey(0xff);SetMediakey(m_arr);flex.SetOption(OPT_OTHER,key_str,op);return;
	}
	void Set_Media_PT(FX_IN int m_arr,FX_IN int mpt,FX_IN int mkey,FX_IN char * op)
	{
		if(flex.GetDOptCnt() == MAX_SDP_OPT_NUM) return;
		SetPTkey(mpt);SetMediakey(m_arr);flex.SetOption(mkey,FX_NULL,op);return;
	}
	void Set_PT_Framesize(FX_IN int mpt,FX_IN char * op)
	{
		if(flex.GetDOptCnt() == MAX_SDP_OPT_NUM) return;
		SetPTkey(mpt);SetMediakey(0xff);flex.SetOption(OPT_SDP_ATTRIBUTE_FRAMESIZE,FX_NULL,op);return;
	}
	void Set_PT_Framerate(FX_IN int mpt,FX_IN char * op)
	{
		if(flex.GetDOptCnt() == MAX_SDP_OPT_NUM) return;
		SetPTkey(mpt);SetMediakey(0xff);flex.SetOption(OPT_SDP_ATTRIBUTE_FRAMERATE,FX_NULL,op);return;
	} 
	void Set_PT_1(FX_IN int mpt,FX_IN int mkey,FX_IN char * op)
	{
		if(flex.GetDOptCnt() == MAX_SDP_OPT_NUM) return;
		SetPTkey(mpt);SetMediakey(0xff);flex.SetOption(mkey,FX_NULL,op);return;
	}
	void Set_PT_2(FX_IN int mpt,FX_IN char * key_str,FX_IN char * op)
	{	
		if(flex.GetDOptCnt() == MAX_SDP_OPT_NUM) return;
		SetPTkey(mpt);SetMediakey(0xff);flex.SetOption(OPT_OTHER,key_str,op);return;
	}
	void Set_PT_Codec(FX_IN int mpt,FX_IN char *op)
	{
		if(flex.GetDOptCnt() == MAX_SDP_OPT_NUM) return;
		SetPTkey(mpt);SetMediakey(0xff);flex.SetOption(OPT_SDP_ATTRIBUTE_RTPMAP,FX_NULL,op);return;
	}
	void Set_PT_Fmtp(FX_IN int mpt,FX_IN char *op)
	{
		if(flex.GetDOptCnt() == MAX_SDP_OPT_NUM) return;
		SetPTkey(mpt);SetMediakey(0xff);flex.SetOption(OPT_SDP_ATTRIBUTE_FMTP,FX_NULL,op);return;
	}
	void Set_StrOIP(FX_IN char *strIP){Set_Common_2((char*)"o_v6Ip",strIP);}
	void Set_StrMIP(FX_IN int m_arr,FX_IN char *strIP){Set_Media_2(m_arr,(char*)"m_v6Ip",strIP);}
	char *Get_StrOIP(){return Find_Common_2((char*)"o_v6Ip");}
	char *Get_StrMIP(FX_IN int m_arr){return Find_Media_2(m_arr,(char*)"m_v6Ip");}
	int Getsize(){return (373 + MAX_SDP_OPT_NUM*2  +  flex.GetFlexSize());}                     
	void Set_SDP_Error(FX_IN int m_sdp_error,FX_IN char *strOtherError)
	{
		if(flex.GetDOptCnt() == MAX_SDP_OPT_NUM) return;
		char Temp[24];Temp[0]=0;Fx_StrCat(Temp,"%u",m_sdp_error);
		if(flex.Find_1(OPT_SDP_ERROR) == FX_NULL)
		{
			SetPTkey(0xff);SetMediakey(0xff);
			if(strOtherError)
			{
				flex.SetOption(OPT_SDP_ERROR,Temp,strOtherError);
			}
			else
			{
				flex.SetOption(OPT_SDP_ERROR,Temp,(char*)opt_keyString(m_sdp_error));
			}	
		}
	}
	char *IsSDP_Error(){return flex.Find_1(OPT_SDP_ERROR);}
	int GetSDP_Error_Code()
	{
		if(IsSDP_Error() == FX_NULL) return 0;
		int i = 0;
		for(i = 0;i<flex.GetDOptCnt();i++)
		{
			if(flex.GetKey(i) == OPT_SDP_ERROR)  break;
		}
		unsigned int Temp_Idx = Fx_Atoi(flex.GetKOption(i));
		if(Temp_Idx <= OPT_SDP_ERROR) return 0;
		return Temp_Idx;
	}
	int Get_Msrp_Type(FX_IN int m_arr){return desc[m_arr].Get_Media();}
	int Get_Msrp_SetupMode()
	{
		char *pSetup = Find_Msrp_Attribute(0,(char*)"setup");
		if(Fx_StrStr(pSetup,(char*)"active")) return MSRP_SETUP_ACTIVE;
		else if(Fx_StrStr(pSetup,(char*)"actpass")) return MSRP_SETUP_ACTPASS;
		else if(Fx_StrStr(pSetup,(char*)"passive")) return MSRP_SETUP_PASSIVE;
		return MSRP_SETUP_NONE;	
	}
	char *Get_Msrp_FileSelector(){return Find_Msrp_Attribute(0,(char*)"file-selector");}
	char *Get_Msrp_Path(){return Find_Msrp_Attribute(0,(char*)"path");}
	char *Get_Msrp_FileTransferID(){return Find_Msrp_Attribute(0,(char*)"file-transfer-id");}
	char *Get_Msrp_ContentsType(){return Find_Msrp_Attribute(0,(char*)"accept-types");}
	char *Get_Msrp_WrappedType(){return Find_Msrp_Attribute(0,(char*)"accept-wrapped-types");}
	bool Get_Visited(FX_INOUT char *Resource,FX_IN int src_len, FX_IN int visit_idx,FX_IN int desc_idx,
						FX_OUT char **nod, FX_OUT char **domain,
						FX_OUT bool *bV6, FX_OUT char **IP, FX_OUT int *Port,FX_OUT char **net_type)
	{
		char *pVisit = Find_Media_PT(desc_idx,visit_idx,OPT_SDP_ATTRIBUTE_VISIT_REALM);
		if(pVisit == NULL) return false;
		// a=visited-realm:1 abcX.sktims.net IN IP4 192.0.2.1 1000 net-type nat
		st_tok_param mtok;mtok.Init(10,src_len,Resource);
		if(!mtok.Parse(TOK_PARSE_PATTERN,(char*)" .    ",pVisit)) return false;
		if(mtok.pos_cnt < 7) return false;
		*nod = mtok.GetParam(1);*domain = mtok.GetParam(2);
		if(Fx_StrCmp(mtok.GetParam(4),(char*)"IP6")) *bV6 = true;
		else *bV6 = false;
		*IP = mtok.GetParam(5);*Port = Fx_Atoi(mtok.GetParam(6));
		char *nettype = Fx_StrStrNext(mtok.GetParam(6),(char *)"net-type");
		if(nettype)
		{
			unsigned int pcnt = 0;
			*net_type = Fx_Skip_Trim(nettype,(char*)" ",&pcnt);
		}
		return true;
	}
	void Set_Visited(FX_IN int visit_idx, FX_IN int desc_idx, FX_IN char *node,
					FX_IN char *domain, FX_IN bool bV6, FX_IN char *IP, int Port, FX_IN char *net_type)
	{
		if(flex.GetDOptCnt() == MAX_FLEX_OPT_NUM) return;
		SetPTkey(visit_idx);SetMediakey(desc_idx);
		if(Fx_Strlen(net_type)>0)
		{
			flex.Fx_Flex_Add(OPT_SDP_ATTRIBUTE_VISIT_REALM,FX_NULL,
			"%d %s.%s IN %s %s %d net-type %s",visit_idx+1,node,domain,
			FX_TRUE_SELECT(bV6==true,"IP6","IP4"),IP,Port,net_type);
		}
		else
		{
			flex.Fx_Flex_Add(OPT_SDP_ATTRIBUTE_VISIT_REALM,FX_NULL,
			"%d %s.%s IN %s %s %d",visit_idx+1,node,domain,
			FX_TRUE_SELECT(bV6==true,"IP6","IP4"),IP,Port);
		}
		Incre_Visit_Counting(desc_idx);
	}
	void Incre_Visit_Counting(FX_IN int desc_idx)
	{
		int visit_idx = 0;
		if(!Find_Media_1(desc_idx,OPT_SDP_ATTRIBUTE_VISIT_REALM_CNT))
		{
			char visit_cnt[6];visit_cnt[0]=0;Fx_StrCpy(visit_cnt,(char*)"1    ");
			Set_Media_1(desc_idx,OPT_SDP_ATTRIBUTE_VISIT_REALM_CNT,visit_cnt);
		}
		else
		{
			char *visit_cnt = Find_Media_1(desc_idx,OPT_SDP_ATTRIBUTE_VISIT_REALM_CNT);
			visit_idx = Fx_Atoi(visit_cnt);visit_idx++;
			visit_cnt[0]=0; Fx_StrCat(visit_cnt,"%d",visit_idx);
		}
	}
	int Get_Visit_Cnt(FX_IN int desc_idx)
	{
		return Fx_Atoi(Find_Media_1(desc_idx,OPT_SDP_ATTRIBUTE_VISIT_REALM_CNT));
	}
	char *Find_OMR_M_CkSum(FX_IN int desc_idx)
	{
		return Find_Media_1(desc_idx,OPT_SDP_ATTRIBUTE_OMR_M_CK);
	}
	char *Find_OMR_S_CkSum(FX_IN int desc_idx)
	{
		return Find_Media_1(desc_idx,OPT_SDP_ATTRIBUTE_OMR_S_CK);
	}
	void Set_OMR_M_CkSum(FX_IN int desc_idx,FX_IN char * pcksum)
	{
		Set_Media_1(desc_idx,OPT_SDP_ATTRIBUTE_OMR_M_CK,pcksum);
	}
	void Set_OMR_S_CkSum(FX_IN int desc_idx,FX_IN char * pcksum)
	{
		Set_Media_1(desc_idx,OPT_SDP_ATTRIBUTE_OMR_S_CK,pcksum);
	}
	int valid()
	{
		int i = 0;
		for(i =0;i<m_cnt; i++)
		{
			if(desc[i].media_t == SDPMEDIATYPE_NOTSET) return BUILD_ERR_SDP_MEDIA_TYPE_EMPTY;
			if(desc[i].mode == SDPCONNECTMODE_NOTSET) return BUILD_ERR_SDP_MODE_TYPE_EMPTY;
			if(desc[i].Get_Port() == 0x00) return BUILD_ERR_SDP_PORT_EMPTY;
		}
		for(i=0;i<flex.GetDOptCnt();i++)
		{
			if(((unsigned char)(pt[i]) != 0xff) &&((unsigned char)(m_num[i]) != 0xff))
				return BUILD_ERR_SDP_SUBKEY_OVERLAP;
		}
		return BUILD_ERR_NONE;
	}
	char *Debug(FX_IN char *strDebug)
	{
		int i = 0;int j = 0;st_addressing m_addr;
		Fx_StrCat(strDebug,"\tRTP> Contents Len %d\n",Getsize());
		Fx_StrCat(strDebug,"\tRTP> session Id : %s, user name : %s\n",Find_Common_SessionID(),
																Find_Common_UserName());
		//added by khd for ipv6
		if( Get_IP() )
		{
			Fx_StrCat(strDebug,"\tRTP> O IP : %s, Media Count : %d\n",
															m_addr.N2A(Get_IP()),Get_Media_Count());
		}
		else
			Fx_StrCat(strDebug,"\tRTP> O IP : %s, Media Count : %d\n",Get_StrOIP(),Get_Media_Count());
		for(i=0;i<Get_Media_Count();i++)
		{
			//added by khd for ipv6
			if( Get_StrMIP(i) )
			{
				Fx_StrCat(strDebug,"\tRTP> Media IP : %s:%d, Mode : %s\n",Get_StrMIP(i),desc[i].Get_Port(),
																con_mod_str(desc[i].Get_Mode()));
			}
			else
			{
				Fx_StrCat(strDebug,"\tRTP> Media IP : %s:%d, Mode : %s\n",m_addr.N2A(desc[i].Get_IP()),
												desc[i].Get_Port(),con_mod_str(desc[i].Get_Mode()));
			}
			Fx_StrCat(strDebug,"\tRTP> Media type : %s, Proto Type : %s\n",
				sdp_media_str(desc[i].Get_Media()),Sdp_Proto_Str(desc[i].Get_Proto()));
			Fx_StrCat(strDebug,"\tRTP> ptime : %d, FrameRate : %d\n",desc[i].Get_PTime(),
																		desc[i].Get_FrameRate());
			Fx_StrCat(strDebug,"\tRTP> atr cnt : %d\n",desc[i].Get_Attr_Count());
			for(j= 0;j<desc[i].a_cnt; j++)
			{
				Fx_StrCat(strDebug,"\tRTP> payload : %d, codec : %s, samplerate : %d, bitrate : %d\n",
					desc[i].attri[j].Get_Payload(),Find_PT_Codec(desc[i].attri[j].Get_Payload()),
					desc[i].attri[j].Get_SampleRate(),desc[i].attri[j].Get_BitRate());
			}
		}
		Fx_StrCat(strDebug,"\tRTP> RTP Pos Len %d, Count %d\n",GetOptLen(),GetDOptCnt());
		for(j =0;j<GetDOptCnt();j++)
		{
			if(GetStringType(j) == STRING_SDP_COMMON)
			{
				Fx_StrCat(strDebug,"\tRTP_Opt> %d,Common K[%s][%s], D[%s]\n"
					,j,opt_keyString(Getkey(j)),GetKOption(j),GetDOption(j));
			}
			else if(GetStringType(j) == STRING_SDP_MEDIA)
			{
				Fx_StrCat(strDebug,"\tRTP_Opt> %d,Media, M[%d], K[%s][%s], D[%s]\n"
					,j,GetMediakey(j),opt_keyString(Getkey(j)),GetKOption(j),GetDOption(j));
			}
			else if(GetStringType(j) == STRING_SDP_PAYLOAD)
			{
				Fx_StrCat(strDebug,"\tRTP_Opt> %d,Payload P[%d], K[%s][%s], D[%s]\n"
					,j,GetPTkey(j),opt_keyString(Getkey(j)),GetKOption(j),GetDOption(j));
			}
		}
		return strDebug;
	}
}st_sdp;
/***************** Contents Structures End **********************/

/***************** SIP Data Struct **********************/
typedef struct
{
	/**************** Fixed Data Area *******************/
	char sip_trans_mod;       // ex>   SIPA_CALL_TRANS_MODE
	char sip_content;           // ex>   CONTENT_TYPE_EMPTY
	char og;                         // ex> 0 = incoming, 1 = outgoing
	char sip_rsp[2];     // ex> 180, 200, 488, 0(req)
	char sip_pair[2];    //ex>   SIPA_METHOD_INVITE
	char sip_ip[4];         // ex> snd/rcv ip
	char sip_port[4];     // ex> snd/rcv port
	st_flexible_sip flex;                // ex> Flexible Opt String Porcess
	/**************** Flexible Data Area ****************/
	char data[2048];
	int Getkey(FX_IN int i){return flex.GetKey(i);}
	int GetKPos(FX_IN int i){return flex.GetKPos(i);}
	int GetDPos(FX_IN int i){return flex.GetDPos(i);}
	int GetOptLen(){return flex.GetOptLen();}
	int GetOptCnt(){return flex.GetDOptCnt();}
	char * GetKOption(FX_IN int i){return flex.GetKOption(i);}
	char * GetDOption(FX_IN int i){return flex.GetDOption(i);}
	void SetOption_1(FX_IN int e_num,FX_IN char * op){flex.SetOption(e_num,FX_NULL,op);return;}
	void SetOption_2(FX_IN char *key,FX_IN char * op){flex.SetOption(OPT_OTHER,key,op);return;}
	char * Find_1(FX_IN int e_num){return flex.Find_1(e_num);}
	char * Find_2(FX_IN char * pkey){return flex.Find_2(pkey);}
	int Get_Rsp(){return Fx_Get_Short(&sip_rsp[0]);}
	void Set_Rsp(FX_IN int rsp){Fx_Set_Short(rsp,&sip_rsp[0]);return;}
	int Get_Pair(){return Fx_Get_Short(&sip_pair[0]);}
	void Set_Pair(FX_IN int pair){Fx_Set_Short(pair,&sip_pair[0]);return;}
	int Get_IP(){return Fx_Get_Int(&sip_ip[0]);}
	void Set_IP(FX_IN int mIP){Fx_Set_Int(mIP,&sip_ip[0]);return;}
	int Get_Port(){return Fx_Get_Int(&sip_port[0]);}
	void Set_Port(FX_IN int mport){Fx_Set_Int(mport,&sip_port[0]);return;}
	char * Get_Caller(){return flex.Find_1(OPT_SIP_CALLER);}
	char * Get_Callee(){return flex.Find_1(OPT_SIP_CALLEE);}
	char * Get_Uri(){return flex.Find_1(OPT_SIP_REQ_URI);}
	char *Get_UserAgent(){return flex.Find_1(OPT_SIP_USER_AGENT);}
	void Set_Caller(FX_IN char *op){flex.SetOption(OPT_SIP_CALLER,FX_NULL,op);return;}
	void Set_Callee(FX_IN char *op){flex.SetOption(OPT_SIP_CALLEE,FX_NULL,op);return;}
	void Set_Uri(FX_IN char *op){flex.SetOption(OPT_SIP_REQ_URI,FX_NULL,op);return;}
	void Set_UserAgent(FX_IN char *op){flex.SetOption(OPT_SIP_USER_AGENT,FX_NULL,op);return;}
	int GetContentsLen()
	{
		if((int)sip_content == CONTENT_TYPE_EMPTY) return 0x00;    
		else if(flex.GetDOptCnt() == 0x00){return Fx_Get_Short(&data[0]);}
		else{return Fx_Get_Short(&(data[GetOptLen()]));}
	}
	void SetContentsLen(FX_IN int len)
	{
		if(flex.GetDOptCnt()== 0x00){Fx_Set_Short(len, &data[0]);}
		else{Fx_Set_Short(len,&data[GetOptLen()]);}
		return;
	}
	void * GetContents()
	{
		if(flex.GetDOptCnt() == 0x00){return (void *)(&(data[2]));}
		else{return (void *)(&(data[GetOptLen() + 2]));}
	}
	int Getsize()
	{
		if(sip_content != CONTENT_TYPE_EMPTY){return (15 + flex.GetFlexSize() + 2 + GetContentsLen());}
		else {return (15 + flex.GetFlexSize());}
	}
}st_sip_msg;
typedef struct
{
	SVCComMsgHdr hdr;
	st_sip_msg sip_msg;
	#define FX_SIP_ADD sip_msg.flex.Fx_Flex_Add

	/**************** Common HDR Functions *******************/
	void Initailize()
	{
		memset(&hdr,0x00,sizeof(SVCComMsgHdr));
		hdr.uiMagicCookie = SVC_MSG_MAGIC_COOKIE; 
		memset(&sip_msg,0x00,sizeof(st_sip_msg));return;
	}
	/*<<< Get Functions >>> */
	int Getsize(){return hdr.uiMsgLen;}
	int GetCmd(){return hdr.uiType;}
	int GetSubCmd(){return hdr.uiSubType;}
	int GetSession(){return hdr.uiCompSesId;}
	int Get_ExeID(){return (int)((hdr.uiCompSesId)/100000);}
	int Get_RvID(){return (int)((hdr.uiCompSesId)%100000);}
	int GetComId(){return hdr.uiCompId;}
	int GetReason(){return hdr.uiReasonCode;}
	/*<<< Set Functions >>> */
	int Setsize(){hdr.uiMsgLen = sizeof(SVCComMsgHdr) + sip_msg.Getsize();return hdr.uiMsgLen;}
	void SetCmd(FX_IN unsigned int cmd){ hdr.uiType = cmd;return;}
	void SetSubCmd(FX_IN unsigned int cmd){ hdr.uiSubType= cmd;return;}
	void SetSession(FX_IN unsigned int ses){hdr.uiCompSesId = ses;return;}
	void SetComId(FX_IN unsigned int id){hdr.uiCompId = id;return;}
	void SetReason(FX_IN unsigned int reason){hdr.uiReasonCode = reason;return;}
	
	/**************** SIP HDR Functions *******************/
	/*<<< Get Functions >>> */
	int GetRsp(){return sip_msg.Get_Rsp();}
	int GetPair(){return sip_msg.Get_Pair();}
	char GetTransMode(){return (unsigned char)(sip_msg.sip_trans_mod);}    
	char GetConType(){return (unsigned char)(sip_msg.sip_content);}    
	char GetOg(){return (unsigned char)(sip_msg.og);}    
	int GetIp(){return sip_msg.Get_IP();}
	int GetPort(){return sip_msg.Get_Port();}
	/*<<< Set Functions >>> */
	void SetRsp(FX_IN unsigned int rsp){sip_msg.Set_Rsp(rsp);return;}
	void SetPair(FX_IN unsigned int pair){sip_msg.Set_Pair(pair);return;}
	void SetTransMode(FX_IN char m){sip_msg.sip_trans_mod = m;return;}
	void SetConType(FX_IN char ctype){sip_msg.sip_content = ctype;return;}
	void SetOg(FX_IN char og){sip_msg.og = og;return;}
	void SetIp(FX_IN int mIp){sip_msg.Set_IP(mIp);}
	void SetPort(FX_IN int mport){sip_msg.Set_Port(mport);}

	/**************** SIP Optional Data Functions *******************/
	/*<<< Get Functions >>> */
	int GetContentsLen(){return sip_msg.GetContentsLen();}
	void * GetContents(){	return sip_msg.GetContents();}
	char * GetKOption(FX_IN int i){return sip_msg.GetKOption(i);}
	char * GetDOption(FX_IN int i){return sip_msg.GetDOption(i);}
	int GetOptCnt(){return sip_msg.GetOptCnt();}
	int GetKey(FX_IN int mkey){return sip_msg.Getkey(mkey);}
	int GetKOptPos(FX_IN int mpos){return sip_msg.GetKPos(mpos);}
	int GetDOptPos(FX_IN int mpos){return sip_msg.GetDPos(mpos);}
	int GetOptLen(){return sip_msg.GetOptLen();}
	char * Find_1(FX_IN int mkey){return sip_msg.Find_1(mkey);}
	char * Find_2(FX_IN char * pkey){return sip_msg.Find_2(pkey);}
	char * Get_Uri(){return sip_msg.Get_Uri();}
	char * Get_Caller(){return sip_msg.Get_Caller();}
	char * Get_PUri(){return Find_2((char*)"P-Asserted-Identity");}
	char * Get_Callee(){return sip_msg.Get_Callee();}
	char *Get_UserAgent(){return sip_msg.Get_UserAgent();}
	char * Get_Number_Plan(FX_IN char * prefix [ ],FX_INOUT st_call_number_plan *pPlan)
	{
		pPlan->Init();
		char *pUri = Get_PUri();
		char *pConv = FX_NULL;
		if(!pUri) pUri = Get_Caller();
		if(!pUri) return FX_NULL;
		pConv = pPlan->conversion_Uri(prefix, pUri);
		return pConv;
	}
	char * Get_Request_Number(FX_IN char * prefix [],FX_INOUT st_request_uri_plan *pPlan)
	{
		char * pRequest_Line = Find_1(OPT_SIP_REQ_URI);
		if(pRequest_Line == FX_NULL) return FX_NULL;
		else return pPlan->Init(pRequest_Line,prefix);
	}
	char *GetV6Ip(){return sip_msg.Find_2((char*)"V6Ip");}
	char *GetLocalIP(){return sip_msg.Find_2((char*)"Local_IP");}
	int GetLocalPort(){return Fx_Atoi(sip_msg.Find_2((char*)"Local_Port"));}
	/*<<< Set Functions >>> */
	void Set_Uri(FX_IN char *op){sip_msg.Set_Uri(op);}
	void Set_Caller(FX_IN char *op){sip_msg.Set_Caller(op);}
	void Set_PUri(FX_IN char *op){SetOption_2((char*)"P-Asserted-Identity",op);}
	void Set_Callee(FX_IN char *op){sip_msg.Set_Callee(op);}
	void Set_UserAgent(FX_IN char *op){sip_msg.Set_UserAgent(op);}
	void SetOption_1(FX_IN int e_num, FX_IN char * op){sip_msg.SetOption_1(e_num,op);return;}
	void SetOption_2(FX_IN char *key, FX_IN char * op){sip_msg.SetOption_2(key,op);return;}
	void SetContentsLen(FX_IN int len){sip_msg.SetContentsLen(len);return;}
	void SetV6Ip(FX_IN char *strIP){sip_msg.SetOption_2((char*)"V6Ip",strIP);}
	void SetLocalIP(FX_IN char *strIP){sip_msg.SetOption_2((char*)"Local_IP",strIP);}
	void SetLocalPort(FX_IN int mPort){sip_msg.flex.Fx_Flex_Add(OPT_OTHER,(char*)"Local_Port","%u",mPort);}

	/**************** Packet Validation Functions *******************/
	void build_err(int reason)
	{
		hdr.uiMagicCookie = SVC_MSG_MAGIC_COOKIE; 
		hdr.uiSubType = hdr.uiType;
		SetCmd(SIPA_METHOD_ERROR);
		hdr.uiReasonCode = reason;
		hdr.uiMsgLen = sizeof(SVCComMsgHdr) + 15;
	}
	int valid()
	{
		int i = 0;
		/******** HDR Check *********************/
		if (hdr.uiMagicCookie != SVC_MSG_MAGIC_COOKIE)     // initialize error
		{
			build_err(BUILD_ERR_INIT_FAIL);
			return BUILD_ERR_INIT_FAIL;
		}
		if (hdr.uiMsgLen == 0x00)                                      // common total length empty
		{
			build_err(BUILD_ERR_LENGTH_EMPTY);
			return BUILD_ERR_LENGTH_EMPTY;      
		}
		if (hdr.uiMsgLen > 2048)
		{
			build_err(BUILD_ERR_OVER_SIZE_TOTAL);
			return BUILD_ERR_OVER_SIZE_TOTAL;
		}
		if (sip_msg.Get_Pair() == 0x00)                                // pair empty
		{
			build_err(BUILD_ERR_PAIR_EMPTY);
			return BUILD_ERR_PAIR_EMPTY;
		}
		if ((hdr.uiType != SIPA_METHOD_RSP) && (sip_msg.Get_Rsp() != 0x00))    // request set error
		{
			build_err(BUILD_ERR_REQUEST_ILLEGAL);
			return BUILD_ERR_REQUEST_ILLEGAL;
		}
		//added by khd for ipv6
		if((sip_msg.og == 1) && ( ((sip_msg.Get_IP() == 0x00) && (sip_msg.Find_2((char*)"V6Ip") == 0x00)) 
										|| (sip_msg.Get_Port() == 0x00)))     // outgoing : ip-port empty
		{
			build_err(BUILD_ERR_OG_TRANSPORT_EMPTY);
			return BUILD_ERR_OG_TRANSPORT_EMPTY;
		}
		if((sip_msg.og == 1) && (hdr.uiType == SIPA_METHOD_RSP))     // outgoing : METHOD FAIL
		{
			build_err(BUILD_ERR_OG_METHOD);
			return BUILD_ERR_OG_METHOD;
		}
		if ((sip_msg.sip_content != CONTENT_TYPE_EMPTY) && (sip_msg.GetContentsLen() == 0x00)) // contents Length empty
		{
			build_err(BUILD_ERR_CONTENT_LENGTH_EMPTY);
			return BUILD_ERR_CONTENT_LENGTH_EMPTY;
		}
		/******** SIP Opt String Check *********************/
		if ((sip_msg.og == 1) && ((sip_msg.Find_1(OPT_SIP_CALLEE) == FX_NULL) 
									|| (sip_msg.Find_1(OPT_SIP_CALLER) == FX_NULL) )) // og empty string
		{
			build_err(BUILD_ERR_OG_OPT_EMPTY);
			return BUILD_ERR_OG_OPT_EMPTY;
		}
		if((sip_msg.Find_1(OPT_SIP_SESSION_EXPIRES) == FX_NULL)
			&& (sip_msg.Find_1(OPT_SIP_MINSE) == FX_NULL)
			&& (sip_msg.Find_1(OPT_SIP_REFRESSHER) == FX_NULL))
		{
			// no error Empty Session Timer
		}
		else if((sip_msg.Find_1(OPT_SIP_SESSION_EXPIRES) != FX_NULL)
			&& (sip_msg.Find_1(OPT_SIP_MINSE) != FX_NULL)
			&& (sip_msg.Find_1(OPT_SIP_REFRESSHER) != FX_NULL))
		{
			// no error Session Timer
		}
		else
		{
			return BUILD_ERR_SESSION_HDR_EMPTY;// lack of Session Param
		}
		if (hdr.uiMagicCookie == SVC_MSG_MAGIC_COOKIE)
		{
			if ((sip_msg.GetOptCnt() > 0) && (sip_msg.data[sip_msg.GetOptLen()-1] != 0x00))
			{
				build_err(BUILD_ERR_OVERLAP);
				return BUILD_ERR_OVERLAP;
			}
			for (i = 0;i<sip_msg.GetOptCnt();i++)
			{
				if(sip_msg.data[sip_msg.GetDPos(i)-1] != 0x00)
				{
					build_err(BUILD_ERR_OVERLAP);
					return BUILD_ERR_OVERLAP;
				}
			}
		}
		/******** SIP Contents Data Check *********************/
		if (((sip_msg.sip_content != CONTENT_TYPE_EMPTY) && (sip_msg.GetOptCnt() > 0)) &&
			((sip_msg.flex.GetFlexSize() + sip_msg.GetContentsLen()) > 2048))
		{
			build_err(BUILD_ERR_OVER_SIZE_CONTENTS);
			return BUILD_ERR_OVER_SIZE_CONTENTS;
		}
		if(sip_msg.sip_content == CONTENT_TYPE_APP_SDP)
		{
			int err_code = 0;
			st_sdp *pSdp = (st_sdp *)sip_msg.GetContents();
			err_code = pSdp->valid();
			if(err_code != BUILD_ERR_NONE)
			{
				build_err(err_code);
				return hdr.uiReasonCode;
			}
		}
		return BUILD_ERR_NONE;
	}
	char *Debug(FX_IN char *strDebug)
	{
		int i = 0;st_addressing m_addr;
		Fx_StrCat(strDebug,"\t[%08d] Msg Len = %d, Content Len = %d\n",GetSession(),
													Getsize(),GetContentsLen());
		if(GetCmd() == SIPA_METHOD_INVITE)
		{
			//added by khd for ipv6
			if( GetIp() )
			{
				Fx_StrCat(strDebug,"\tSIP> Target : IP= %s, Port = %d\n",m_addr.N2A(GetIp()),
																			GetPort());
			}
			else
			{
				if (GetOptCnt() > 0)
				{
					Fx_StrCat(strDebug,"\tSIP> Target : IP= %s, Port = %d\n",
															GetV6Ip(),GetPort());
				}
			}
			Fx_StrCat(strDebug,"\tSIP> Caller : %s, PURI : %s\n",Get_Caller(),Get_PUri());
		}
		if (GetOptCnt() > 0)
		{
			Fx_StrCat(strDebug,"\tSIP> Pos Len %d, Count %d\n",GetOptLen(),GetOptCnt());
			for(i = 0;i<GetOptCnt(); i++)
			{
				Fx_StrCat(strDebug,"\tSIP_Opt> %d, K[%d][%s][%s], D[%d][%s]\n"
					,i,GetKOptPos(i),opt_keyString(GetKey(i)),GetKOption(i),
					GetDOptPos(i),GetDOption(i));
			}
		}
		return strDebug;
	}
}SVC_Sip_Msg;
typedef struct
{
	SVCComMsgHdr hdr;
	SVCSetupApp   ping;
	int enable;
	char cur_call_av[4];
	char cur_trans_av[4];
	char cur_msg_av[4];
	unsigned int Get_CurCallAv(){return Fx_Get_Int(&cur_call_av[0]);}
	unsigned int Get_CurTransAv(){return Fx_Get_Int(&cur_trans_av[0]);}
	unsigned int Get_CurMsgAv(){return Fx_Get_Int(&cur_msg_av[0]);}
	void Set_CurCallAv(unsigned int m_cur){Fx_Set_Int(m_cur,&cur_call_av[0]);}
	void Set_CurTransAv(unsigned int m_cur){Fx_Set_Int(m_cur,&cur_trans_av[0]);}
	void Set_CurMsgAv(unsigned int m_cur){Fx_Set_Int(m_cur,&cur_msg_av[0]);}
}st_ping;
typedef struct
{
	unsigned char media_cnt;
	unsigned char map_cnt;
	unsigned char media[4];     // ex> media[0] == SDPMEDIATYPE_AUDIO
	unsigned char map[4];       // ex> map[0] == PAYLOAD_G723_AUDIO
}st_Nego_Result;

inline void SIP_DebugString(char *strDebug, SVC_Sip_Msg * msg_data)
{
	if(strDebug == NULL) return;strDebug[0] = 0x00;
	Fx_StrCat(strDebug,"\r\n\t====== [%08d] SVC_SipMsg Info ========\n",msg_data->GetSession());
	msg_data->Debug(strDebug);
	if(msg_data->GetContentsLen() == 0) return;
	if(msg_data->GetConType() == CONTENT_TYPE_APP_SDP)
	{
		st_sdp *pRtp = (st_sdp *)msg_data->GetContents();
		pRtp->Debug(strDebug);
	}
	else if (msg_data->GetConType() == CONTENT_TYPE_TEXT)
	{
		char *pCon = (char *)msg_data->GetContents();
		Fx_StrCat(strDebug,"\tContents Data \n%s\n",pCon);
	}
	else if(msg_data->GetConType() != CONTENT_TYPE_EMPTY)
	{
		char *pCon = (char *)msg_data->GetContents();
		Fx_StrCat(strDebug,"\tContents Data \n%s\n",pCon);
	}
	Fx_StrCat(strDebug,"\r\n\t================================\n");
}
inline char *SIP_RawDump(FX_OUT char *Resource,FX_IN void *pRawData,FX_IN int Len)
{
	Resource[0]=0;char *pData = (char*)pRawData;int j=0;int i=0;
	Fx_StrCat(Resource,"\n===================== dump =====================\n");
	Fx_StrCat(Resource,"%-16s : %02x %02x %02x %02x, ","uiMagicCookie",
		(unsigned char)(*(&pData[0])),(unsigned char)(*(&pData[1])),(unsigned char)(*(&pData[2])),(unsigned char)(*(&pData[3])));
	Fx_StrCat(Resource,"%-16s : %02x %02x %02x %02x\n","uiMsgLen",
		(unsigned char)(*(&pData[4])),(unsigned char)(*(&pData[5])),(unsigned char)(*(&pData[6])),(unsigned char)(*(&pData[7])));
	Fx_StrCat(Resource,"%-16s : %02x %02x %02x %02x, ","uiType",
		(unsigned char)(*(&pData[8])),(unsigned char)(*(&pData[9])),(unsigned char)(*(&pData[10])),(unsigned char)(*(&pData[11])));
	Fx_StrCat(Resource,"%-16s : %02x %02x %02x %02x\n","uiSubType",
		(unsigned char)(*(&pData[12])),(unsigned char)(*(&pData[13])),(unsigned char)(*(&pData[14])),(unsigned char)(*(&pData[15])));
	Fx_StrCat(Resource,"%-16s : %02x %02x %02x %02x, ","uiCompId",
		(unsigned char)(*(&pData[16])),(unsigned char)(*(&pData[17])),(unsigned char)(*(&pData[18])),(unsigned char)(*(&pData[19])));
	Fx_StrCat(Resource,"%-16s : %02x %02x %02x %02x\n","uiCompSesId",
		(unsigned char)(*(&pData[20])),(unsigned char)(*(&pData[21])),(unsigned char)(*(&pData[22])),(unsigned char)(*(&pData[23])));
	Fx_StrCat(Resource,"%-16s : %02x %02x %02x %02x, ","uiAsId",
		(unsigned char)(*(&pData[24])),(unsigned char)(*(&pData[25])),(unsigned char)(*(&pData[26])),(unsigned char)(*(&pData[27])));
	Fx_StrCat(Resource,"%-16s : %02x %02x %02x %02x\n","uiAsSesId",
		(unsigned char)(*(&pData[28])),(unsigned char)(*(&pData[29])),(unsigned char)(*(&pData[30])),(unsigned char)(*(&pData[31])));
	Fx_StrCat(Resource,"%-16s :\n","szSesDesc");
	for(j=0;j<32;j++) Fx_StrCat(Resource,"%02x ",(unsigned char)(*(&pData[j+32])));   // 63
	Fx_StrCat(Resource,"\n%-16s : %02x %02x %02x %02x\n","uiReasonCode",
		(unsigned char)(*(&pData[64])),(unsigned char)(*(&pData[65])),(unsigned char)(*(&pData[66])),(unsigned char)(*(&pData[67])));
	Fx_StrCat(Resource,"%-16s : ","Reserved");
	for(j=0;j<24;j++) Fx_StrCat(Resource,"%02x ",(unsigned char)(*(&pData[j+68])));   // 91	
	Fx_StrCat(Resource,"\n%-16s : %u\n","PK Sceq", Fx_Get_Int(&(pData[68])));
	Fx_StrCat(Resource,"\n===================== HEX ======================\n");
	for(j=0,i=0;i<Len;i++)
	{
		j = i+1;
		if((j%32 == 0) && (j != 1)) Fx_StrCat(Resource,"%02x \n",(unsigned char)(*(&pData[i])));
		else Fx_StrCat(Resource,"%02x ",(unsigned char)(*(&pData[i])));
	}
	Fx_StrCat(Resource,"\n================================================\n\n");
	return Resource;
}
inline char *Trace_Call_Info(FX_OUT char *Resource,FX_IN SVC_Sip_Msg *pSip)
{
	Resource[0]=0;
	char *strTasname = pSip->Find_2((char*)"TASName");
	char *strTastype = pSip->Find_2((char*)"TASType");
	char *strCallID = pSip->Find_2((char*)"CallString");
	char *strCaller = pSip->Find_2((char*)"TraceCaller");
	char *strCalled = pSip->Find_2((char*)"TraceCalled");
	Fx_StrCat(Resource,"\n================ Call Trace Info ===========\n");
	Fx_StrCat(Resource,"%-16s : %-16s, %-16s : %-16s\n","TASName",strTasname,"TASType",strTastype);
	Fx_StrCat(Resource,"%-16s : %s\n","Call-ID",strCallID);
	Fx_StrCat(Resource,"%-16s : %-16s, %-16s : %-16s\n","Caller",strCaller,"Called",strCalled);
	Fx_StrCat(Resource,"==========================================\n");
	return Resource;
}
inline void New_Nego_SDP(FX_IN st_sdp *offer, FX_IN st_sdp *answer,FX_OUT st_Nego_Result *pResult)
{
	int i,j,k,m;	
	for(i=0;i<offer->Get_Media_Count(); i++)
	{
		for(j=0;j<answer->Get_Media_Count();j++)
		{
			if(offer->desc[i].Get_Media() == answer->desc[j].Get_Media())
			{
				////ex> SDPMEDIATYPE_AUDIO
				//printf("[SDP Nego] Media Match %s",sdp_media_str(a_Sdp->desc[j].Get_Media()));   
				pResult->media[pResult->media_cnt] =  (unsigned char)(answer->desc[j].Get_Media());
				(pResult->media_cnt)++;
				for(k=0;k<offer->desc[i].Get_Attr_Count();k++)
				{
					for(m=0;m<answer->desc[j].Get_Attr_Count();m++)
					{
						if(offer->desc[i].attri[k].Get_Payload() == answer->desc[j].attri[m].Get_Payload())
						{
							//printf("[SDP Nego] PayLoad Match %d",a_Sdp->desc[j].attri[m].Get_Payload());  
							pResult->map[pResult->map_cnt]=(unsigned char)(answer->desc[j].attri[m].Get_Payload());
							(pResult->map_cnt)++;
							goto jump;
						}
					}
				}
			}
			jump:
				continue;
		}
	}
	//for(i =0;i<pResult->media_cnt;i++)
	//	printf("[SDP Nego] Result>>> [%d] media %s, payload %d",
	//					i,sdp_media_str(pResult->media[i]),pResult->map[i] );
}
inline void TestMsg_Package(FX_IN SVC_Sip_Msg *msg)
{
	/********************************************/
	/**************** Pack *********************/
	/********************************************/
	
	/*********** HDR Set ********************/
	msg->Initailize();
	msg->SetComId(0x12345);
	msg->SetConType(CONTENT_TYPE_APP_SDP);
	msg->SetOg(1);
	msg->SetCmd(SIPA_METHOD_INVITE);
	msg->SetSession(0x1234);
	msg->SetTransMode(SIPA_CALL_TRANS_MODE);
	msg->SetPair(SIPA_METHOD_INVITE);
	msg->SetRsp(0);

	/*********** Optional Set ********************/
	msg->Set_Callee((char*)"07087186000");
	msg->Set_Caller((char*)"01065882310");
	msg->SetOption_2((char*)"User-Agent",(char*)"LGT xx Phone");
	msg->SetOption_2((char*)"others",(char*)"Optional String");

	/*********** Contents Set ********************/
	st_sdp * pSdp = (st_sdp *)msg->GetContents();    
	// >>>>>>> SDP Contents Data Set
	// ip, session name, descriptor(attribute, paylod type... etc)
	// >>>>>>>  Contents Optional Set
	pSdp->Set_Common_SessionID((char*)"123456t77");
	pSdp->Set_Common_UserName((char*)"LGT FeelRing");
	pSdp->Set_Media_Count(2);
	pSdp->desc[0].Set_Attr_Count(1);
	pSdp->desc[0].attri[0].Set_BitRate(1234);
	pSdp->desc[1].Set_Attr_Count(1);
	pSdp->desc[1].attri[0].Set_BitRate(3456l);
	pSdp->Set_PT_1(PAYLOAD_G723_AUDIO,OPT_SDP_ATTRIBUTE_CAT,(char*)"p = pixel 1000");
	pSdp->Set_PT_1(PAYLOAD_AMR_RFC,OPT_SDP_COMMON_B,(char*)"t = 1000ms");
	pSdp->Set_PT_2(PAYLOAD_H263_VIDEO,(char*)"pixelothers" ,(char*)"56/24");
	// >>>>>>>  Contents Length Set
	msg->SetContentsLen(pSdp->Getsize());
		
	/*********** Final Msg Length Set ********************/
	int len = msg->Setsize();
	printf("Packet Len : %d\r\n",len);
	// Send_TCP(msg,len);

	/********************************************/
	/**************** Unpack *********************/
	/********************************************/
	char temp[sizeof(SVC_Sip_Msg)];
	memset(temp,0x00,len);
	memcpy(temp,msg,len);
	SVC_Sip_Msg *msg2 = (SVC_Sip_Msg *)temp;
	int sz = msg2->Getsize();
	int comid = msg2->GetComId();
	int cmd = msg2->GetCmd();
	int Sess = msg2->GetSession();
	int Sip_Pair = msg2->GetPair();
	printf("%d %d %d %d %d\r\n",sz,comid,cmd,Sess,Sip_Pair);
	printf("Caller : %s\r\n",msg2->Get_Caller());
	printf("Callee : %s\r\n",msg2->Get_Callee());
	printf("Agent : %s\r\n",msg2->Find_2((char*)"User-Agent"));
	int i = 0;
	for(i = 0;i<msg2->GetOptCnt(); i++)
	{
		printf(" %d, keypos[%d], keyenum[%s], keystr: %s : Data : %s"
			,i,msg2->GetKOptPos(i),opt_keyString(msg2->GetKey(i)),msg2->GetKOption(i),msg2->GetDOption(i));
	}	
	if(msg2->GetConType() == CONTENT_TYPE_APP_SDP)
	{
		st_sdp *mSdp = (st_sdp *)msg2->GetContents();
		printf("session Id : %s\r\n",mSdp->Find_Common_SessionID());
		printf("RTP Pos Len %d\r\n",mSdp->GetOptLen());
		printf("Optional Attribute String  %s\r\n",
									mSdp->Find_PT_1(PAYLOAD_G723_AUDIO,OPT_SDP_ATTRIBUTE_CAT));
		int j = 0;
		for(j =0;j<mSdp->GetDOptCnt();j++)
			{
			if(mSdp->GetStringType(j) == STRING_SDP_COMMON)
			{
				printf("[New_SIPA] RTP_Opt> %d,Common K[%s][%s], D[%s]"
					,j,opt_keyString(mSdp->Getkey(j)),mSdp->GetKOption(j),mSdp->GetDOption(j));
			}
			else if(mSdp->GetStringType(j) == STRING_SDP_MEDIA)
			{
				printf("[New_SIPA] RTP_Opt> %d,Media, M[%d], K[%s][%s], D[%s]"
					,j,mSdp->GetMediakey(j),opt_keyString(mSdp->Getkey(j)),
					mSdp->GetKOption(j),mSdp->GetDOption(j));
			}
			else if(mSdp->GetStringType(j) == STRING_SDP_PAYLOAD)
			{
				printf("[New_SIPA] RTP_Opt> %d,Payload P[%d], K[%s][%s], D[%s]"
					,j,mSdp->GetPTkey(j),opt_keyString(mSdp->Getkey(j)),mSdp->GetKOption(j),
					mSdp->GetDOption(j));
			}
		}
	}
	
}
inline void Test_Parse_SDP(st_sdp * pSdp)
{
	for(int i = 0;i<pSdp->Get_Media_Count();i++)
	{
		int m_rate = pSdp->desc[i].Get_FrameRate();
		for(int k = 0;k<pSdp->desc[i].Get_Attr_Count();k++)
		{
			char *ptFramesize = pSdp->Find_PT_Framesize(pSdp->desc[i].attri[k].Get_Payload());
			printf("[New_SIPA] > %d, %s\r\n",m_rate,ptFramesize);
		}
	}
	// Set Functions...
	pSdp->desc[1].Set_FrameRate(15);
	pSdp->Set_PT_Framesize(104, (char*)"Frame size String");
}

typedef enum
{
	SIPA_REP_NONE,
	SIPA_REP_INIT,	
	SIPA_REP_SIPMSG,
	SIPA_REP_STORE,
	SIPA_REP_DEL,
	SIPA_REP_RSP,
	SIPA_REP_REINIT,
	SIPA_REP_SWITCH,
	SIPA_REP_SETUP,
	SIPA_REP_ALIVE,
	SIPA_REP_CONNECT,        
	SIPA_REP_CLOSE, 
}e_rep_cmd;
typedef enum
{
	SIPA_REP_HDR_NONE,
	SIPA_REP_HDR_RVRAWDATA,
}e_rep_hdr;
typedef struct _hdr_stack
{
	char body_len[2];
	char bExist;
	char bclone;
	char connect_time[4];
	char sip_session_id[4];
	char sip_clone_pos[4];
	char as_session_id[4];
	char sip_comp_id[4];
	char as_comp_id[4];
	int Get_Len_Size(){return 28;}
}_hdr_stack;
typedef struct
{
	_hdr_stack hdr_stack;
	char data[1024];
	void Init(){hdr_stack.bExist=0;memset(&hdr_stack,0x00,hdr_stack.Get_Len_Size());}
	void Del(){hdr_stack.bExist = 0;}
	int Get_Clone(){return hdr_stack.bclone;}
	int Get_Exist(){return hdr_stack.bExist;}
	void Set_SndType(char mType){data[1022] = mType;}
	void Incre_Retry(){char temp = data[1023];temp++;data[1023]=temp;}
	void Set_Retry(char mval){data[1023] = mval;}
	int Get_SndType(){return (int)((unsigned char)(*(&data[1022])));}
	int Get_Retry(){return (int)((unsigned char)(*(&data[1023])));}
	int Get_Connect_Time(){return Fx_Get_Int(&(hdr_stack.connect_time[0]));}
	int Get_Sip_SessionID(){return Fx_Get_Int(&(hdr_stack.sip_session_id[0]));}
	int Get_Sip_ClonePos(){return Fx_Get_Int(&(hdr_stack.sip_clone_pos[0]));}
	int Get_As_SessionID(){return Fx_Get_Int(&(hdr_stack.as_session_id[0]));}
	int Get_Sip_CompID(){return Fx_Get_Int(&(hdr_stack.sip_comp_id[0]));}
	int Get_As_CompID(){return Fx_Get_Int(&(hdr_stack.as_comp_id[0]));}
	int Get_Exe_ID()
	{
		int rt_sess = Fx_Get_Int(&hdr_stack.sip_session_id[0]);
		return (int)(((unsigned int)rt_sess)/100000);
	}
	int Get_Sip_RvID()
	{
		int rt_sess = Fx_Get_Int(&hdr_stack.sip_session_id[0]);
		return (int)(((unsigned int)rt_sess)%100000);
	}
	char *Get_Body(){return data;}
	int Get_Body_Len(){return Fx_Get_Short(&(hdr_stack.body_len[0]));}
	int Get_Len(){return hdr_stack.Get_Len_Size() + Get_Body_Len();}
	int Get_Len_Default(){return hdr_stack.Get_Len_Size();}
	int Get_Len_Size(){return hdr_stack.Get_Len_Size() + 1024;}
	void Set_Exist(){hdr_stack.bExist = 1;}
	void Set_Clone(){hdr_stack.bclone = 1;}
	void Set_Body_Len(int mlen){Fx_Set_Short(mlen,&(hdr_stack.body_len[0]));}
	void Set_Connect_Time(int mTime){Fx_Set_Int(mTime,&(hdr_stack.connect_time[0]));}
	void Set_Sip_SessionID(int mSes){Fx_Set_Int(mSes,&(hdr_stack.sip_session_id[0]));}
	void Set_Sip_ClonePos(int mPos){Fx_Set_Int(mPos,&(hdr_stack.sip_clone_pos[0]));}
	void Set_As_SessionID(int mSes){Fx_Set_Int(mSes,&(hdr_stack.as_session_id[0]));}
	void Set_Sip_CompID(int mComp){Fx_Set_Int(mComp,&(hdr_stack.sip_comp_id[0]));}
	void Set_As_CompID(int mComp){Fx_Set_Int(mComp,&(hdr_stack.as_comp_id[0]));}
	void Set_Body(char * mBody, int mlen)
	{
		if(mBody == NULL || mlen ==0){memset(&(hdr_stack.body_len[0]),0x00,2);memset(data,0x00,1024);}
		else	{memcpy(&data[0],mBody,mlen);Fx_Set_Short(mlen,&hdr_stack.body_len[0]);}
	}
	void Copy_Stack_Rep(void *Out){memset(Out,0x00,Get_Len_Size());memcpy(Out,&hdr_stack,Get_Len_Size());}
	void Store_Stack_Rep(void *In){Init();memcpy(&hdr_stack,In,Get_Len_Size());}
	void GetDebug(char *buff)
	{
		buff[0] = 0;
		Fx_StrCat(buff,"Len : %d\r\n",Get_Len());
		Fx_StrCat(buff,"clone: %d\r\n",Get_Clone());
		Fx_StrCat(buff,"Sip Comp : %d, Sip Session : %d\r\n",Get_Sip_CompID(),Get_Sip_SessionID());
		Fx_StrCat(buff,"Sip Clon Pos : %d,",Get_Sip_ClonePos());
		Fx_StrCat(buff,"As Comp : %d, As Session : %d\r\n",Get_As_CompID(),Get_As_SessionID());
	}
}st_RepStack;
typedef struct _hdr_pk
{
	char len[2];
	char pk_seq[4];
	char cmd[2];
	char subcmd[4];
	char subpair[4];
	char subrsp[2];
	char realpos[4];
	int Get_Len(){return Fx_Get_Short(&(len[0]));}
	int Get_PkSeq(){return Fx_Get_Int(&(pk_seq[0]));}
	int Get_Cmd(){return Fx_Get_Short(&(cmd[0]));}
	int Get_SubCmd(){return Fx_Get_Int(&(subcmd[0]));}
	int Get_SubPair(){return Fx_Get_Int(&(subpair[0]));}
	int Get_SubRsp(){return Fx_Get_Short(&(subrsp[0]));}
	int Get_RealPos(){return Fx_Get_Int(&(realpos[0]));}
	void Set_Len(int mlen){Fx_Set_Short(mlen,&(len[0]));}
	void Set_PkSeq(int mSeq){Fx_Set_Int(mSeq,&(pk_seq[0]));}
	void Set_Cmd(int mCmd){Fx_Set_Short(mCmd,&(cmd[0]));}
	void Set_SubCmd(int mSubCmd){Fx_Set_Int(mSubCmd,&(subcmd[0]));}
	void Set_SubPair(int mPair){Fx_Set_Int(mPair,&(subpair[0]));}
	void Set_SubRsp(int mRsp){Fx_Set_Short(mRsp,&(subrsp[0]));}
	void Set_RealPos(int mPos){Fx_Set_Int(mPos,&(realpos[0]));}
	int Get_Len_Size(){return 22;}
}_hdr_pk;
typedef struct
{
	_hdr_pk hdr_pk;
	st_RepStack RepStack;
	void Init(){memset(&hdr_pk,0x00,hdr_pk.Get_Len_Size());memset(&RepStack,0x00,RepStack.Get_Len_Size());}
	void BuildCloneSipMsg(SVC_Sip_Msg *pSip,char *StrBody,bool mClone)
	{
		if(Get_Cmd() == SIPA_REP_STORE) return;
		RepStack.Set_Clone();
		Set_Cmd(SIPA_REP_SIPMSG);
		Set_SubCmd(pSip->GetCmd());
		Set_SubPair(pSip->GetPair());
		Set_SubRsp(pSip->GetRsp());
		RepStack.Set_Sip_SessionID(pSip->GetSession());
		RepStack.Set_As_SessionID(pSip->hdr.uiAsSesId);
		RepStack.Set_Sip_CompID(pSip->GetComId());
		RepStack.Set_As_CompID(pSip->hdr.uiCompSesId);
		RepStack.Set_Body(StrBody,Fx_Strlen(StrBody));
		Set_Length();
	}
	void Copy_Stack_Rep(void *Out){RepStack.Copy_Stack_Rep(Out);}
	void Store_Stack_Rep(void *In){RepStack.Store_Stack_Rep(In);}
	void Copy_Hdr_Packet(void *reppk){memcpy(reppk,&hdr_pk,Get_DefaultLength());}
	void Set_Command(int mseq,int mcmd, int msubcmd,int mpair, int mrsp)
	{
		hdr_pk.Set_PkSeq(mseq);hdr_pk.Set_Cmd(mcmd);
		hdr_pk.Set_SubCmd(msubcmd);hdr_pk.Set_SubPair(mpair);
		hdr_pk.Set_SubRsp(mrsp);
	}
	int Get_DefaultLength(){return  hdr_pk.Get_Len_Size() + RepStack.Get_Len_Default();}
	void Set_DefaultLength(){hdr_pk.Set_Len(hdr_pk.Get_Len_Size() + RepStack.Get_Len_Default());}
	int Get_FullSize(){return hdr_pk.Get_Len_Size() + RepStack.Get_Len_Size();}
	void Set_Length(){hdr_pk.Set_Len(hdr_pk.Get_Len_Size()+ RepStack.Get_Len());} 
	int Get_Length(){return hdr_pk.Get_Len();}
	void Set_Cmd(int mCmd){hdr_pk.Set_Cmd(mCmd);}
	int Get_Cmd(){return hdr_pk.Get_Cmd();}
	void Set_SubCmd(int mCmd){hdr_pk.Set_SubCmd(mCmd);}
	int Get_SubCmd(){return hdr_pk.Get_SubCmd();}
	void Set_SubPair(int mCmd){hdr_pk.Set_SubCmd(mCmd);}
	int Get_SubPair(){return hdr_pk.Get_SubPair();}
	void Set_SubRsp(int mCmd){hdr_pk.Set_SubRsp(mCmd);}
	int Get_SubRsp(){return hdr_pk.Get_SubRsp();}
	int Get_Exe_ID(){return RepStack.Get_Exe_ID();}
	int Get_Sip_RvID(){return RepStack.Get_Sip_RvID();}
	int Get_Clone(){return RepStack.Get_Clone();}
	int Get_Sip_SessionID(){return RepStack.Get_Sip_SessionID();}
	int Get_Sip_ClonePos(){return RepStack.Get_Sip_ClonePos();}
	int Get_Rep_RealPos(){return hdr_pk.Get_RealPos();}
	void Set_Rep_RealPos(int mpos){hdr_pk.Set_RealPos(mpos);}
	int Get_As_SessionID(){return RepStack.Get_As_SessionID();}
	int Get_Sip_CompID(){return RepStack.Get_Sip_CompID();}
	int Get_As_CompID(){return RepStack.Get_As_CompID();}
	char *GetCmdString(){return (char*)CmdString(Get_Cmd());}
	char *GetSubCmdString(){return (char*)Sipa_Method_Str(Get_SubCmd());}
	const char *CmdString(int mCmd)
	{
		switch(mCmd)
		{
			case SIPA_REP_NONE: return "SIPA_REP_NONE";
			case SIPA_REP_INIT: return "SIPA_REP_INIT";
			case SIPA_REP_SIPMSG: return "SIPA_REP_SIPMSG";
			case SIPA_REP_STORE: return "SIPA_REP_STORE";
			case SIPA_REP_DEL: return "SIPA_REP_DEL";
			case SIPA_REP_RSP: return "SIPA_REP_RSP";
			case SIPA_REP_REINIT: return "SIPA_REP_REINIT";
			case SIPA_REP_SWITCH: return "SIPA_REP_SWITCH";
			case SIPA_REP_SETUP: return "SIPA_REP_SETUP";
			case SIPA_REP_ALIVE: return "SIPA_REP_ALIVE";
			case SIPA_REP_CONNECT: return "SIPA_REP_CONNECT";
			case SIPA_REP_CLOSE: return "SIPA_REP_CLOSE";
			default: return "SIPA_REP_UNKNOWN";
		}
	}
	const char *GetHdrString(int mHdr)
	{
		switch(mHdr)
		{
			case SIPA_REP_HDR_NONE: return "SIPA_REP_HDR_NONE";
			case SIPA_REP_HDR_RVRAWDATA: return "SIPA_REP_HDR_RVRAWDATA";
			default: return "SIPA_REP_UNKNOWN";
		}
	}
	void GetDebug(char *buff)
	{
		buff[0] = 0;
		Fx_StrCat(buff,"\n=======================\n");
		Fx_StrCat(buff,"Len : %d, body : %d\n",Get_Length(),Get_Length());
		Fx_StrCat(buff,"clone: %d, Cmd: %s\n",Get_Clone(),GetCmdString());
		Fx_StrCat(buff,"Sip Comp : %d, Sip Session : %d\n",Get_Sip_CompID(),Get_Sip_SessionID());
		Fx_StrCat(buff,"Sip Clon Pos : %d,",Get_Sip_ClonePos());
		Fx_StrCat(buff,"As Comp : %d, As Session : %d\n",Get_As_CompID(),Get_As_SessionID());
		Fx_StrCat(buff,"%s\n=======================\n",RepStack.Get_Body());
	}
}st_rep_data;

#endif //SIP_IF_DEFS

