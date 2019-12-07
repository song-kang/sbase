/**
 *
 * 文 件 名 : SDecodeMms.cpp
 * 创建日期 : 2015-5-9 23:57
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : 其它协议解码
 * 修改记录 : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-5-9	邵凯田　创建文件
 *
 **/

#include "SDecodeMms.h"

//////////////////////////////////////////////////////////////////////////
// 名    称:  SDecodeMms
// 作    者:  邵凯田
// 创建时间:  2015-5-9 23:57
// 描    述:  ARP
//////////////////////////////////////////////////////////////////////////
SDecodeMms::SDecodeMms()
{
	m_ProtType = SDECODE_PROT_MMS;
	m_LastEvent.m_iProtType = (int)m_ProtType;
	m_pLastSessionStatus = NULL;
	#ifdef SDECODE_OFFLINE
	m_pLastTreeNode = NULL;
	#endif
}

SDecodeMms::~SDecodeMms()
{

}

//////////////////////////////////////////////////////////////////////////
// 描    述:  解码一帧报文
// 作    者:  邵凯田
// 创建时间:  2015-5-10 11:15
// 参数说明:  @soc表示报文时标（秒）
//         :  @nsec表示报文时标（纳秒）
// 返 回 值:  int,0表示解码成功，对应eSDecodeFmtError
//////////////////////////////////////////////////////////////////////////
int SDecodeMms::DecodeFrame(int soc,int nsec)
{
	pThis = this;
	//从本协议的数据开始位置进行解码，下层协议的解码结果已经在预处理信息中(m_pPreprocessInfo)
	BYTE *pBuffer = m_pPreprocessInfo->m_pFrameBuffer;
	int iLen = m_pPreprocessInfo->m_iFrameValidBytes;
	int pos = m_pPreprocessInfo->m_iStartPos;
	int iBytes = iLen - pos;
	int ret=0;
	#ifdef SDECODE_OFFLINE
	m_pLastTreeNode = NULL;
	#endif
	if(m_pPreprocessInfo->m_pSession == NULL || m_pPreprocessInfo->m_pTcp == NULL)
	{
		NEW_MALFORMED_PD(pBuffer,pos,iLen-pos,"no valid session found");
		return SDECODE_FMT_TCP;
	}
	m_pLastSessionStatus = (stuMmsSessionStatus*)m_pLastSession->GetStatusBuf();
	if(m_pLastSessionStatus == NULL)
	{
		m_pLastSessionStatus = new stuMmsSessionStatus();
		memset(m_pLastSessionStatus,0,sizeof(stuMmsSessionStatus));
		m_pLastSession->SetStatusBuf((BYTE*)m_pLastSessionStatus,sizeof(stuMmsSessionStatus));
	}
		
	if(m_pPreprocessInfo->m_iStartPos == m_pPreprocessInfo->m_iFrameValidBytes)
	{
		//无TCP层以上的内容，仅包含了TCP层数据
		//根据TCP层的FLAG产生连接建立、销毁的事件
		if(m_pPreprocessInfo->m_pTcp->IsSetFlagSyn() && m_pEventCallbackFun != NULL)
		{
			//事件:新的连接请求
			m_LastEvent.m_iEventCode = SDECODE_MMS_CONNECT;
			stuSDecodeEvent_MmsSession *pInfo = (stuSDecodeEvent_MmsSession*)m_LastEvent.m_pEventData;
			pInfo->m_iClientIp = m_pLastSession->GetClientIp();
			pInfo->m_iServerIp = m_pLastSession->GetServerIp();
			pInfo->m_iClientPort = ntohs(m_pLastSession->GetClientPort());
			pInfo->m_iServerPort = ntohs(m_pLastSession->GetServerPort());
			m_pEventCallbackFun(m_pEventCallbackParam,&m_LastEvent);
		}
		if(m_pPreprocessInfo->m_pTcp->IsSetFlagFin() && m_pEventCallbackFun != NULL)
		{
			//事件:终止连接请求
			m_LastEvent.m_iEventCode = SDECODE_MMS_DISCONNECT;
			stuSDecodeEvent_MmsSession *pInfo = (stuSDecodeEvent_MmsSession*)m_LastEvent.m_pEventData;
			pInfo->m_iClientIp = m_pLastSession->GetClientIp();
			pInfo->m_iServerIp = m_pLastSession->GetServerIp();
			pInfo->m_iClientPort = ntohs(m_pLastSession->GetClientPort());
			pInfo->m_iServerPort = ntohs(m_pLastSession->GetServerPort());
			m_pEventCallbackFun(m_pEventCallbackParam,&m_LastEvent);
		}
		//无上层协议
		return 0;
	}

again:
	if(m_pLastSessionStatus->m_iTpktLeftByte != 0)
	{
		//有TPKT拼包
		if(m_pLastSessionStatus->m_iTpktLeftByte == iBytes)
		{
			m_pLastSession->AddToDatBuf(SDECODE_BUF_DAT1,pBuffer+pos,iBytes);
			//对完整TPKT解码
			ret = DecodeTpkt(m_pLastSession->GetDatBuf(SDECODE_BUF_DAT1),m_pLastSession->GetDatBufLen(SDECODE_BUF_DAT1));
			if(ret != 0)
			{
				m_pLastSession->RemoveDatBufFront(SDECODE_BUF_DAT1,m_pLastSession->GetDatBufLen(SDECODE_BUF_DAT1));
				m_pLastSessionStatus->m_iTpktLeftByte = 0;
				return ret;
			}

			//清除拼包
			m_pLastSession->RemoveDatBufFront(SDECODE_BUF_DAT1,m_pLastSession->GetDatBufLen(SDECODE_BUF_DAT1));
			m_pLastSessionStatus->m_iTpktLeftByte = 0;
		}
		else if(m_pLastSessionStatus->m_iTpktLeftByte > iBytes)
		{
			m_pLastSession->AddToDatBuf(SDECODE_BUF_DAT1,pBuffer+pos,iBytes);
			m_pLastSessionStatus->m_iTpktLeftByte -= iBytes;
			//报文尚不完整，不解码
		}
		else
		{
			//不大可能出现的情况，报文完整，但后面还跟着下一帧的开头
			m_pLastSession->AddToDatBuf(SDECODE_BUF_DAT1,pBuffer+pos,m_pLastSessionStatus->m_iTpktLeftByte);
			//对完整TPKT解码
			ret = DecodeTpkt(m_pLastSession->GetDatBuf(SDECODE_BUF_DAT1),m_pLastSession->GetDatBufLen(SDECODE_BUF_DAT1));
			if(ret != 0)
			{
				m_pLastSession->RemoveDatBufFront(SDECODE_BUF_DAT1,m_pLastSession->GetDatBufLen(SDECODE_BUF_DAT1));
				m_pLastSessionStatus->m_iTpktLeftByte = 0;
				return ret;
			}
			m_pLastSession->RemoveDatBufFront(SDECODE_BUF_DAT1,m_pLastSession->GetDatBufLen(SDECODE_BUF_DAT1));

			//解下一包TPKT
			pos += m_pLastSessionStatus->m_iTpktLeftByte;
			iBytes -= m_pLastSessionStatus->m_iTpktLeftByte;
			m_pLastSessionStatus->m_iTpktLeftByte = 0;

// 			//将后半截报文记入下一帧的拼接缓冲
// 			m_pLastSession->AddToDatBuf(SDECODE_BUF_DAT1,pBuffer+pos+m_pLastSessionStatus->m_iTpktLeftByte,iBytes-m_pLastSessionStatus->m_iTpktLeftByte);
// 			pos += m_pLastSessionStatus->m_iTpktLeftByte;
// 			iBytes -= m_pLastSessionStatus->m_iTpktLeftByte;
// 			m_pLastSessionStatus->m_iTpktLeftByte = 0;
			goto again;
		}
	}
	else
	{
		//无TPKT拼包
		unsigned short tpkt_len;
		memcpy(&tpkt_len,pBuffer+pos+2,2);
		SKT_SWAP_WORD_REV(tpkt_len);
		if(tpkt_len < 4)
		{
			//清除拼包
			m_pLastSession->RemoveDatBufFront(SDECODE_BUF_DAT1,m_pLastSession->GetDatBufLen(SDECODE_BUF_DAT1));
			m_pLastSessionStatus->m_iTpktLeftByte = 0;
			NEW_MALFORMED_PD(m_pPreprocessInfo->m_pViewBuffer,pBuffer+pos+2-m_pPreprocessInfo->m_pViewBuffer,2,"TPKT length invalid");
			return SDECODE_FMT_TPKT;
		}
		if(tpkt_len == iBytes)
		{
			ret = DecodeTpkt(pBuffer+pos,tpkt_len);
			if(ret != 0)
			{
				m_pLastSession->RemoveDatBufFront(SDECODE_BUF_DAT1,m_pLastSession->GetDatBufLen(SDECODE_BUF_DAT1));
				m_pLastSessionStatus->m_iTpktLeftByte = 0;
				return ret;
			}

		}
		else if(tpkt_len > iBytes)
		{
			m_pLastSession->AddToDatBuf(SDECODE_BUF_DAT1,pBuffer+pos,iBytes);
			m_pLastSessionStatus->m_iTpktLeftByte = tpkt_len - iBytes;
		}
		else
		{
			//当前包有前一个包的结尾，还有新一包的开头
			//先处理头
			if(tpkt_len > 0)
			{
				m_pLastSession->AddToDatBuf(SDECODE_BUF_DAT1,pBuffer+pos,tpkt_len);
			}
			ret = DecodeTpkt(pBuffer+pos,m_pLastSession->GetDatBufLen(SDECODE_BUF_DAT1));
			if(ret != 0)
			{
				m_pLastSession->RemoveDatBufFront(SDECODE_BUF_DAT1,m_pLastSession->GetDatBufLen(SDECODE_BUF_DAT1));
				m_pLastSessionStatus->m_iTpktLeftByte = 0;
				return ret;
			}
			//再处理尾
			m_pLastSession->RemoveDatBufFront(SDECODE_BUF_DAT1,m_pLastSession->GetDatBufLen(SDECODE_BUF_DAT1));
			m_pLastSessionStatus->m_iTpktLeftByte = 0;
			pos += tpkt_len;
			iBytes -= tpkt_len; 
			goto again;
		}
	}

	return ret;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  TPKT解码函数
// 作    者:  邵凯田
// 创建时间:  2015-5-25 19:40
// 参数说明:  @p表示缓冲区地址
//         :  @len表示长度
// 返 回 值:  0表示成功，否则表示失败
//////////////////////////////////////////////////////////////////////////
int SDecodeMms::DecodeTpkt(BYTE *p,int len)
{
#ifdef SDECODE_OFFLINE
	SDecodeTree *pTPKT = NULL;
	if(m_pDecodeTreeRoot != NULL)
		pTPKT = m_pDecodeTreeRoot->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,4,"TPKT");
#endif
	if(*p != 0x03)
	{
		NEW_MALFORMED_PD(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,1,"TPKT version invalid");
		return SDECODE_FMT_TPKT;
	}
	p+=2;
	unsigned short tpkt_len;
	memcpy(&tpkt_len,p,2);
	SKT_SWAP_WORD_REV(tpkt_len);
	if(tpkt_len != len)
	{
		NEW_MALFORMED_PD(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,2,"TPKT length invalid");
		return SDECODE_FMT_TPKT;
	}
#ifdef SDECODE_OFFLINE
	if(pTPKT != NULL)
	{
		pTPKT->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer-2,1,"Version",SString::toFormat("%d",p[-2]).data());
		pTPKT->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer-1,1,"Reserved",SString::toFormat("%d",p[-1]).data());
		pTPKT->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,2,"Length",SString::toFormat("%d",ntohs(*(unsigned short*)p)).data());
	}
#endif
	p+=2;

	//处理下层协议,解码ISO8073(RFC905)COTP
	return Decode8073(p,len-4);
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  8073(COTP)解码函数
// 作    者:  邵凯田
// 创建时间:  2015-5-25 19:41
// 参数说明:  @p表示缓冲区地址
//         :  @len表示长度
// 返 回 值:  0表示成功，否则表示失败
//////////////////////////////////////////////////////////////////////////
int SDecodeMms::Decode8073(BYTE *p,int len)//COTP
{
	int ret = 0;
	BYTE *pBuffer = m_pPreprocessInfo->m_pViewBuffer;
	int pos = p-pBuffer;
	BYTE *pBuf8073 = p;
	int iLen = len;
	/*
	octet    1   2 3 4 ... n   n+1  ...    p  p+1 ...end
			+---+-------------+--------------+-----------+
			| LI| fixed part  | variable part| data field|
			+---+-------------+--------------+-----------+
			<---------------   header ------>
	+-------------------------------------------------------------+
	|                       | Validity within   |       |         |
	|                       |     classes       |  see  |  Code   |
	|                       |-------------------| Clause|         |
	|                       | 0 | 1 | 2 | 3 | 4 |       |         |
	|-----------------------|-------------------|-------|---------|
	|CR Connection Request  | x | x | x | x | x | 13.3  |1110 xxxx| 0x0E
	|-----------------------|---|---|---|---|---|-------|---------|
	|CC Connection Confirm  | x | x | x | x | x | 13.4  |1101 xxxx| 0x0D
	|-----------------------|---|---|---|---|---|-------|---------|
	|DR Disconnect Request  | x | x | x | x | x | 13.5  |1000 0000| 0x08
	|-----------------------|---|---|---|---|---|-------|---------|
	|DC Disconnect Confirm  |   | x | x | x | x | 13.6  |1100 0000| 0x0C
	|-----------------------|---|---|---|---|---|-------|---------|
	|DT Data                | x | x | x | x | x | 13.7  |1111 0000| 0x0F
	|-----------------------|---|---|---|---|---|-------|---------|
	|ED Expedited Data      |   | x | NF| x | x | 13.8  |0001 0000| 0x01
	|-----------------------|---|---|---|---|---|-------|---------|
	|AK Data Acknowledgement|   |NRC| NF| x | x | 13.9  |0110 zzzz| 0x06
	|-----------------------|---|---|---|---|---|-------|---------|
	|EA Expedited Data      |   | x | NF| x | x | 13.10 |0010 0000| 0x02
	|Acknowledgement        |   |   |   |   |   |       |         |
	|-----------------------|---|---|---|---|---|-------|---------|
	|RJ Reject              |   | x |   | x |   | 13.11 |0101 zzzz| 0x05
	|-----------------------|---|---|---|---|---|-------|---------|
	|ER TPDU Error          | x | x | x | x | x | 13.12 |0111 0000| 0x07
	|-----------------------|---|---|---|---|---|-------|---------|
	|                       |   |   |   |   |   |   -   |0000 0000|
	|                       |---|---|---|---|---|-------|---------|
	|not available          |   |   |   |   |   |   -   |0011 0000|
	| (see note)            |---|---|---|---|---|-------|---------|
	|                       |   |   |   |   |   |   -   |1001 xxxx|
	|                       |---|---|---|---|---|-------|---------|
	|                       |   |   |   |   |   |   -   |1010 xxxx|
	+-------------------------------------------------------------+

	Table 8. TPDU code
	*/
	int cotp_len = pBuf8073[0]+1;
	if(cotp_len > iLen)
	{
		NEW_MALFORMED_LEN;
		return SDECODE_FMT_8073;
	}
#ifdef SDECODE_OFFLINE
	SDecodeTree *p8073;
	if(m_pDecodeTreeRoot != NULL)
	{
		p8073 = m_pDecodeTreeRoot->New(pBuffer,pos,cotp_len,"ISO 8073/X.224 COTP Connection-Oriented Transport Protocol");
		p8073->New(pBuffer,pos,1,"Length",SString::toFormat("%d",cotp_len-1).data());
	}
#endif
	pos++;
	unsigned char eot;//last data unit? 1 is last
	unsigned char pdu_type = pBuf8073[1] >> 4;
	unsigned short tpdu_number;
	switch(pdu_type)
	{
	case 0x0E: //CR Connection Request
	case 0x0D: //CC Connection Confirm
		//13.3  Connection Request (CR) TPDU
		//The length of the CR TPDU shall not exceed 128 octets.
#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			if(pdu_type == 0x0E)
				p8073->New(pBuffer,pos,1,"PDU Type","CR Connection Request (0x0E)");
			else
				p8073->New(pBuffer,pos,1,"PDU Type","CC Connection Confirm (0x0D)");
		}
#endif
		if(cotp_len > 129)
		{
			NEW_MALFORMED_LEN;
			return SDECODE_FMT_8073;
		}
// 		The structure of the CR TPDU shall be as follows:
// 		1    2        3        4       5   6    7    8    p  p+1...end
// 		+--+------+---------+---------+---+---+------+-------+---------+
// 		|LI|CR CDT|     DST - REF     |SRC-REF|CLASS |VARIAB.|USER     |
// 		|  |1110  |0000 0000|0000 0000|   |   |OPTION|PART   |DATA     |
// 		+--+------+---------+---------+---+---+------+-------+---------+

		m_pLastSessionStatus->m_iCotpClass = pBuf8073[6]>>4;
		m_pLastSessionStatus->m_iIsExtended = ((pBuf8073[6]&0x02) >> 1);
		break;
	case 0x08: //DR Disconnect Request
// 		The structure of the DR TPDU shall be as follows:
// 		1     2      3     4    5     6     7    8     p   p+1 ...end
// 		+--+---------+----+-----+----+-----+------+--------+----------+
// 		|LI|    DR   | DST-REF. | SRC-REF. |REASON|VARIABLE| USER     |
// 		|  |1000 0001|    |     |    |     |      |  PART  | DATA     |
// 		+--+---------+----+-----+----+-----+------+--------+----------+
		break;
	case 0x0C: //DC Disconnect Confirm 
// 		The structure of DC TPDU shall be as follows:
// 		1       2         3     4     5     6    7        p
// 		+----+-----------+-----+-----+-----+-----+-------+--------+
// 		| LI |    DC     |  DST REF  |  SRC REF  | Variable Part  |
// 		|    | 1100 0000 |     |     |     |     |       |        |
// 		+----+-----------+-----+-----+-----+-----+-------+--------+
		break;
	case 0x0F: //DT Data 
// 		a)  Normal format for Classes 0 and 1
// 
// 		 1       2         3          4       5             ... end
// 		+----+-----------+-----------+------------ - - - - - -------+
// 		| LI |    DT     |  TPDU-NR  | User Data                    |
// 		|    | 1111 0000 |  and EOT  |                              |
// 		+----+-----------+-----------+------------ - - - - - -------+
		if(m_pLastSessionStatus->m_iCotpClass <= 1)
		{
			eot = pBuf8073[2]>>7;
			tpdu_number = pBuf8073[2]&0x7f;
		}
// 		b)  Normal format for Classes 2, 3 and 4
// 
// 		1      2       3   4     5     6       p    p+1       ... end
// 		+----+---------+---+---+-------+-----+-------+----------- - - -+
// 		| LI |   DT    |DST-REF|TPDU-NR|Variable Part|User Data        |
// 		|    |1111 0000|   |   |and EOT|     |       |                 |
// 		+----+---------+---+---+-------+-----+-------+----------- - - -+
		else if(m_pLastSessionStatus->m_iIsExtended == 0)
		{
			eot = pBuf8073[4]>>7;
			tpdu_number = pBuf8073[4]&0x7f;
		}
// 		c)  Extended Format for  use  in  Classes  2,  3  and  4  when
// 		selected during connection establishment.
// 
// 		1      2       3   4   5,6 7,8  9     p  p+1      ... end
// 		+----+---------+---+---+---------+--------+---------- - - -+
// 		| LI |   DT    |DST-REF| TPDU-NR |Variable|User Data       |
// 		|    |1111 0000|   |   | and EOT |  Part  |                |
// 		+----+---------+---+---+---------+--------+---------- - - -+
		else
		{
			eot = pBuf8073[7]>>7;
			tpdu_number = pBuf8073[7]&0x7f;
		}
#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			p8073->New(pBuffer,pos,1,"PDU Type","DT Data (0x0F)");
			p8073->New(pBuffer,pos,1,"TPDU Number",SString::toFormat("%d",tpdu_number).data());
			p8073->New(pBuffer,pos+1,1,"Last Data Unit",SString::toFormat("%s",eot==1?"Yes":"No").data());
		}
#endif
// 		When set to ONE, indicates that the  current  DT
// 		TPDU is the last data unit of a complete DT TPDU
// 		sequence (End of TSDU).  EOT is bit 8 of octet 3
// 		in  class  0  and 1, bit 8 of octet 5 for normal
// 		formats for classes 2, 3 and  4  and  bit  8  of
// 		octet 8 for extended formats;
		if(eot != 1)
		{
			//传输过程未结束
			m_pLastSession->AddToDatBuf(SDECODE_BUF_DAT2,pBuf8073+cotp_len,len-cotp_len);
		}
		else
		{
			int iDatLen;
			if(m_pLastSession->GetDatBufLen(SDECODE_BUF_DAT2) > 0)
			{
				m_pLastSession->AddToDatBuf(SDECODE_BUF_DAT2,pBuf8073+cotp_len,len-cotp_len);
				m_pPreprocessInfo->m_pViewBuffer = pBuffer = m_pLastSession->GetDatBuf(SDECODE_BUF_DAT2);
				m_pPreprocessInfo->m_iViewBytes = iDatLen = m_pLastSession->GetDatBufLen(SDECODE_BUF_DAT2);
			}
			else
			{
				pBuffer = pBuf8073+cotp_len;
				iDatLen = len - cotp_len;
			}			
			//TODO:处理上层协议,pBuffer,iDatLen
			ret = Decode8327(pBuffer,iDatLen);
			//清除缓存数据，并不真正清除，仍可以在界面上引用
			m_pLastSession->RemoveDatBufFront(SDECODE_BUF_DAT2,m_pLastSession->GetDatBufLen(SDECODE_BUF_DAT2));
			if(ret != 0)
				return ret;
		}
		break;
	case 0x01: //ED Expedited Data
		break;
	case 0x06: //AK Data Acknowledgement
		break;
	case 0x02: //EA Expedited Data 
		break;
	case 0x05: //RJ Reject
		break;
	case 0x07: //ER TPDU Error
		break;
	default://not available
		NEW_MALFORMED_PD(pBuffer,pos,1,"PDU type invalid");
		return SDECODE_FMT_8073;
		break;
	}
	return ret;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  解码8327报文
// 作    者:  邵凯田
// 创建时间:  2015-5-11 8:57
// 参数说明:  @pDatBuffer表示8327层有效数据的缓冲区地址
//         :  @iLen表示数据长度
// 返 回 值:  int,0表示解码成功，对应eSDecodeFmtError
//////////////////////////////////////////////////////////////////////////
int SDecodeMms::Decode8327(BYTE *p8327DatBuffer,int iLen)
{
	BYTE cosp_type = p8327DatBuffer[0];
	BYTE cosp_len = p8327DatBuffer[1];
	int ret=0;
	BYTE *p = p8327DatBuffer;
	BYTE* pUserData;
	int iUserDataLen;

#ifdef SDECODE_OFFLINE
	SDecodeTree *p8327;
	if(m_pDecodeTreeRoot != NULL)
	{
		p8327 = m_pDecodeTreeRoot->New(m_pPreprocessInfo->m_pViewBuffer,p8327DatBuffer-m_pPreprocessInfo->m_pViewBuffer,cosp_len+2,"ISO 8327-1 OSI Session Protocol");
	}
#endif
	m_pLastSessionStatus->m_iCospType = cosp_type;
	switch (cosp_type)
	{
	case COSP_SI_CONNECT:
#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			p8327->New(m_pPreprocessInfo->m_pViewBuffer,p8327DatBuffer-m_pPreprocessInfo->m_pViewBuffer,1,"SPDU Type","CONNECT SPDU(13)");
			p8327->New(m_pPreprocessInfo->m_pViewBuffer,p8327DatBuffer+1-m_pPreprocessInfo->m_pViewBuffer,1,"Length",SString::toFormat("%d",cosp_len).data());
		}
#endif
		ret = Decode8327_Con_Acp((char *)p8327DatBuffer,iLen,COSP_SI_CONNECT,pUserData,iUserDataLen);
		if(ret != 0)
			return ret;
		//decode 8823
		Decode8823(pUserData,iUserDataLen);
		//ret = cosp_process_connect(p8327DatBuffer,iLen);
		break;
	case COSP_SI_FINISH:
#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			p8327->New(m_pPreprocessInfo->m_pViewBuffer,p8327DatBuffer-m_pPreprocessInfo->m_pViewBuffer,1,"SPDU Type","FINISH SPDU(9)");
			p8327->New(m_pPreprocessInfo->m_pViewBuffer,p8327DatBuffer+1-m_pPreprocessInfo->m_pViewBuffer,1,"Length",SString::toFormat("%d",cosp_len).data());
		}
#endif
		//		off = cosp_process_finish(off);
		break;
	case COSP_SI_ACCEPT:
#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			p8327->New(m_pPreprocessInfo->m_pViewBuffer,p8327DatBuffer-m_pPreprocessInfo->m_pViewBuffer,1,"SPDU Type","ACCEPT SPDU(14)");
			p8327->New(m_pPreprocessInfo->m_pViewBuffer,p8327DatBuffer+1-m_pPreprocessInfo->m_pViewBuffer,1,"Length",SString::toFormat("%d",cosp_len).data());
		}
#endif
		ret = Decode8327_Con_Acp((char *)p8327DatBuffer,iLen,COSP_SI_ACCEPT,pUserData,iUserDataLen);
		if(ret != 0)
			return ret;
		//decode 8823
		Decode8823(pUserData,iUserDataLen);
		//		off = cosp_process_accept(off);
		break;
	case COSP_SI_REFUSE:
#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			p8327->New(m_pPreprocessInfo->m_pViewBuffer,p8327DatBuffer-m_pPreprocessInfo->m_pViewBuffer,1,"SPDU Type","REFUSE SPDU(12)");
			p8327->New(m_pPreprocessInfo->m_pViewBuffer,p8327DatBuffer+1-m_pPreprocessInfo->m_pViewBuffer,1,"Length",SString::toFormat("%d",cosp_len).data());
		}
#endif
		break;
	case COSP_SI_DISCON:
#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			p8327->New(m_pPreprocessInfo->m_pViewBuffer,p8327DatBuffer-m_pPreprocessInfo->m_pViewBuffer,1,"SPDU Type","DISCON SPDU(10)");
			p8327->New(m_pPreprocessInfo->m_pViewBuffer,p8327DatBuffer+1-m_pPreprocessInfo->m_pViewBuffer,1,"Length",SString::toFormat("%d",cosp_len).data());
		}
#endif
		//ret = cosp_process_disconnect(p8327DatBuffer,iLen);
		//		off = cosp_process_disconnect(off);
		break;
	case COSP_SI_ABORT:
#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			p8327->New(m_pPreprocessInfo->m_pViewBuffer,p8327DatBuffer-m_pPreprocessInfo->m_pViewBuffer,1,"SPDU Type","ABORT SPDU(25)");
			p8327->New(m_pPreprocessInfo->m_pViewBuffer,p8327DatBuffer+1-m_pPreprocessInfo->m_pViewBuffer,1,"Length",SString::toFormat("%d",cosp_len).data());
		}
#endif
		break;
	case COSP_SI_AB_ACCEPT:
#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			p8327->New(m_pPreprocessInfo->m_pViewBuffer,p8327DatBuffer-m_pPreprocessInfo->m_pViewBuffer,1,"SPDU Type","AB_ACCEPT SPDU(26)");
			p8327->New(m_pPreprocessInfo->m_pViewBuffer,p8327DatBuffer+1-m_pPreprocessInfo->m_pViewBuffer,1,"Length",SString::toFormat("%d",cosp_len).data());
		}
#endif
		break;
	case COSP_SI_DATA:
#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			p8327->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,1,"SPDU Type","Give tokens PDU(1)");
			p8327->New(m_pPreprocessInfo->m_pViewBuffer,p+1-m_pPreprocessInfo->m_pViewBuffer,1,"Length",SString::toFormat("%d",cosp_len).data());
			
			cosp_len = p[cosp_len+3];
			p8327 = m_pDecodeTreeRoot->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,cosp_len+2,"ISO 8327-1 OSI Session Protocol");
			p8327->New(m_pPreprocessInfo->m_pViewBuffer,p+cosp_len+2-m_pPreprocessInfo->m_pViewBuffer,1,"SPDU Type","DATA TRANSFER (DT) SPDU (1)");
			p8327->New(m_pPreprocessInfo->m_pViewBuffer,p+cosp_len+3-m_pPreprocessInfo->m_pViewBuffer,1,"Length",SString::toFormat("%d",cosp_len).data());

		}
#endif
		p+=p[1]+p[3]+4;
		if(p >= p8327DatBuffer+iLen)
		{
			if(p == p8327DatBuffer+iLen)
				return 0;
			else
			{
				NEW_MALFORMED;
				return SDECODE_FMT_8327;
			}
		}
		ret = Decode8823(p,p8327DatBuffer+iLen-p);
		break;
	default:
		NEW_MALFORMED_PD(m_pPreprocessInfo->m_pViewBuffer,0,0,"COSP Type invalid");
		return SDECODE_FMT_8327;
	}
	return ret;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  8327协议取长度及长度占用字节数
// 作    者:  邵凯田
// 创建时间:  2015-5-25 19:42
// 参数说明:  @buf为长度缓冲首地址
//         :  @bytes表示长度占用字节数的指针，输出参数
// 返 回 值:  WORD型长度
//////////////////////////////////////////////////////////////////////////
WORD SDecodeMms::Decode8327_GetLen(BYTE *buf, ST_UINT *bytes)
{
	WORD   len;
	if (buf [0] == 0xFF)
	{
		/* len >254      high order byte          low order byte		*/
		len  = ((ST_UINT16) buf [1] << 8) | (ST_UINT16) buf [2];
		*bytes = 3;
	}
	else
	{
		/* len <=254 */
		len  = (ST_UINT16) buf [0];
		*bytes = 1;
	}

	return len;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  8327连接、接受报文解码
// 作    者:  邵凯田
// 创建时间:  2015-5-25 19:43
// 参数说明:  
// 返 回 值:  
//////////////////////////////////////////////////////////////////////////
int SDecodeMms::Decode8327_Con_Acp(char *spdu_buf, ST_UINT spdu_len, ST_UCHAR spdu_type, BYTE* &pUserData,int &iUserDataLen)
{
	int		ret=0;
	ST_UCHAR *dec_buf;
	ST_UINT	dec_len;
	ST_UCHAR		param_code;
	ST_UINT	param_len;
	ST_UINT	group_len;
	ST_UINT	len_bytes;	/* number of bytes for encoded len (1 or 3) */
	ST_UINT	idx;
	ST_UINT off = m_pPreprocessInfo->m_iViewBytes - spdu_len;
	unsigned char * p = (unsigned char *)spdu_buf;
	pUserData = NULL;
	iUserDataLen = 0;

#ifdef SDECODE_OFFLINE
	SDecodeTree *p8327 = NULL;
	if(m_pDecodeTreeRoot != NULL)
	{
		p8327 = m_pDecodeTreeRoot->GetChildsPtr()->getTail();
	}
#endif
	ret = SD_SUCCESS;

	dec_buf = (ST_UCHAR *) spdu_buf;
	dec_len = 0;
	idx = 0;

	while (SD_TRUE)
	{
		/* NOTE: we should never return back from bottom of the loop here!!! */

		/* the decoded len should be comparable with spdu_len passed to this func */
		dec_len = (ST_UINT) Decode8327_GetLen (&dec_buf[idx+1], &len_bytes);
		if (dec_len + 1+len_bytes != spdu_len)
		{
			NEW_MALFORMED;
			return SDECODE_FMT_8327;
		}
		idx += 1+len_bytes;		/* 1 for SPDU code */
		/* decode all connect parameters */
		while (dec_len > 0)
		{
			param_code = dec_buf [idx];
			param_len  = (ST_UINT) Decode8327_GetLen (&dec_buf [idx+1], &len_bytes);

			/* signal error if param_len is greater then decoding length left */
			if (param_len + 1+len_bytes > dec_len)
			{
				NEW_MALFORMED_D("signal error if param_len is greater then decoding length left");
				return SDECODE_FMT_8327;
			}
			dec_len -= 1+len_bytes;
			idx     += 1+len_bytes;

			/* do not try to decode if param_len is 0 (param is empty)	*/
			if (param_len == 0)
				continue;

			switch (param_code)
			{
			case  1:		/* PGI - Connection Identifier group 	*/
#ifdef SDECODE_OFFLINE
				if(p8327 != NULL)
				{
					SDecodeTree *p = p8327->New(m_pPreprocessInfo->m_pViewBuffer,dec_buf+idx-m_pPreprocessInfo->m_pViewBuffer,1,"Connection Identifier");
					p->New(m_pPreprocessInfo->m_pViewBuffer,dec_buf+idx-len_bytes-1-m_pPreprocessInfo->m_pViewBuffer,1,"Parameter Type","Connection Identifier");
					p->New(m_pPreprocessInfo->m_pViewBuffer,dec_buf+idx-len_bytes-m_pPreprocessInfo->m_pViewBuffer,len_bytes,"Parameter Length",
						SString::toFormat("%d",GetIntWithBufLen(&dec_buf[idx]-len_bytes,len_bytes)).data());
				}
#endif

				/* we will ignore this group (skip all decoding) */
				break;

			case  5:		/* PGI - Connect/Accept Item group	*/
			{				
#ifdef SDECODE_OFFLINE
				SDecodeTree *pConAcc;
				if(p8327 != NULL)
				{
					pConAcc = p8327->New(m_pPreprocessInfo->m_pViewBuffer,dec_buf+idx-m_pPreprocessInfo->m_pViewBuffer,param_len,"Connect Accept Item");
					pConAcc->New(m_pPreprocessInfo->m_pViewBuffer,dec_buf+idx-len_bytes-1-m_pPreprocessInfo->m_pViewBuffer,1,"Parameter Type","Connect Accept Item");
					pConAcc->New(m_pPreprocessInfo->m_pViewBuffer,dec_buf+idx-len_bytes-m_pPreprocessInfo->m_pViewBuffer,len_bytes,"Parameter Length",
						SString::toFormat("%d",GetIntWithBufLen(&dec_buf[idx]-len_bytes,len_bytes)).data());
				}
#endif
				group_len = param_len;
				while (group_len > 0)
				{
					param_code = dec_buf [idx];
					param_len  = (ST_UINT) Decode8327_GetLen (&dec_buf [idx+1], &len_bytes);

					/* signal error if param_len is greater then decoding length left */
					if (param_len + 1+len_bytes > group_len)
					{
						NEW_MALFORMED;
						return SDECODE_FMT_8327;
					}
					dec_len   -= 1+len_bytes;
					group_len -= 1+len_bytes;
					idx       += 1+len_bytes;

					/* do not try to decode if param_len is 0 (param is empty)	*/
					if (param_len == 0)
						continue;

					switch (param_code)
					{
					case  19:			/* PI - Protocol Options	*/
						if (param_len != 1)
						{
							NEW_MALFORMED;
							return SDECODE_FMT_8327;
						}
#ifdef SDECODE_OFFLINE
						SDecodeTree *pOpt;
						if(p8327 != NULL)
						{
							pOpt = pConAcc->New(m_pPreprocessInfo->m_pViewBuffer,dec_buf+idx-len_bytes-1-m_pPreprocessInfo->m_pViewBuffer,1,"Protocol Option");
							pOpt->New(m_pPreprocessInfo->m_pViewBuffer,dec_buf+idx-len_bytes-1-m_pPreprocessInfo->m_pViewBuffer,1,"Parameter Type","Protocol Option");
							pOpt->New(m_pPreprocessInfo->m_pViewBuffer,dec_buf+idx-len_bytes-m_pPreprocessInfo->m_pViewBuffer,len_bytes,"Parameter Length",
								SString::toFormat("%d",GetIntWithBufLen(&dec_buf[idx]-len_bytes,len_bytes)).data());
							pOpt->New(m_pPreprocessInfo->m_pViewBuffer,dec_buf+idx-m_pPreprocessInfo->m_pViewBuffer,1,"Flags",SString::toFormat("0x%2X",dec_buf [idx]).data());
						}
#endif
						break;

					case  21:			/* PI - TSDU Max Size		*/
						if (param_len == 4)
						{
#ifdef SDECODE_OFFLINE
							SDecodeTree *pOpt;
							if(m_pDecodeTreeRoot != NULL)
							{
								pOpt = pConAcc->New(m_pPreprocessInfo->m_pViewBuffer,dec_buf+idx-len_bytes-1-m_pPreprocessInfo->m_pViewBuffer,1,"TSDU Max Size");
								pOpt->New(m_pPreprocessInfo->m_pViewBuffer,dec_buf+idx-len_bytes-1-m_pPreprocessInfo->m_pViewBuffer,1,"Parameter Type","TSDU Max Size");
								pOpt->New(m_pPreprocessInfo->m_pViewBuffer,dec_buf+idx-len_bytes-m_pPreprocessInfo->m_pViewBuffer,len_bytes,"Parameter Length",
									SString::toFormat("%d",GetIntWithBufLen(&dec_buf[idx]-len_bytes,len_bytes)).data());
								pOpt->New(m_pPreprocessInfo->m_pViewBuffer,dec_buf+idx-m_pPreprocessInfo->m_pViewBuffer,2,"Initiator TSDU Size",
									SString::toFormat("%d",ntohs(*(unsigned short*)(dec_buf+idx))).data());
								pOpt->New(m_pPreprocessInfo->m_pViewBuffer,dec_buf+idx+2-m_pPreprocessInfo->m_pViewBuffer,2,"Responder TSDU Size",
									SString::toFormat("%d",ntohs(*(unsigned short*)(dec_buf+idx+2))).data());
							}
#endif
						}
						else
						{
							NEW_MALFORMED;
							return SDECODE_FMT_8327;
						}
						break;

					case  22:			/* PI - Version Number		*/
						if (param_len == 1)
						{
//							dec_par->ver_num = dec_buf[idx];
#ifdef SDECODE_OFFLINE
							SDecodeTree *pOpt;
							if(p8327 != NULL)
							{
								pOpt = pConAcc->New(m_pPreprocessInfo->m_pViewBuffer,dec_buf+idx-len_bytes-1-m_pPreprocessInfo->m_pViewBuffer,1,"Version Number");
								pOpt->New(m_pPreprocessInfo->m_pViewBuffer,dec_buf+idx-len_bytes-1-m_pPreprocessInfo->m_pViewBuffer,1,"Parameter Type","Version Number");
								pOpt->New(m_pPreprocessInfo->m_pViewBuffer,dec_buf+idx-len_bytes-m_pPreprocessInfo->m_pViewBuffer,len_bytes,"Parameter Length",
									SString::toFormat("%d",GetIntWithBufLen(&dec_buf[idx]-len_bytes,len_bytes)).data());
								SDecodeTree *p = pOpt->New(m_pPreprocessInfo->m_pViewBuffer,dec_buf+idx-m_pPreprocessInfo->m_pViewBuffer,2,"Flags",
									SString::toFormat("0x%2X",dec_buf[idx]).data());
								p->New(m_pPreprocessInfo->m_pViewBuffer,dec_buf+idx-m_pPreprocessInfo->m_pViewBuffer,1,"Protocol Version 2",
									(dec_buf[idx]&0x02) == 0?"False":"True");
								p->New(m_pPreprocessInfo->m_pViewBuffer,dec_buf+idx-m_pPreprocessInfo->m_pViewBuffer,1,"Protocol Version 1",
									(dec_buf[idx]&0x01) == 0?"False":"True");
							}
#endif
						}
						else
						{
							NEW_MALFORMED;
							return SDECODE_FMT_8327;
						}
						break;

					case  23:			/* PI - Initial Serial Number	*/
						/* we will ignore this param (skip all decoding) */
						break;

					case  26:			/* PI - Token Setting Item	*/
						/* we will ignore this param (skip all decoding) */
						break;

					default:
						NEW_MALFORMED;
						return SDECODE_FMT_8327;
					} /* end of switch */

					if (ret == SD_SUCCESS)
					{
						/* adjust processing vars by the length of param processed */
						dec_len   -= param_len;
						group_len -= param_len;
						idx       += param_len;
					}
					else
						break;	/* error, exit this loop */
				} /* end of while (group_len > 0) */

				param_len = 0;	/* dec_len & idx already adjusted */
				break;
			}
			case  16:			/* PI - Token Item	*/
				if (spdu_type == COSP_SI_ACCEPT)
					/* we will ignore this param (skip all decoding) */
					;
				else
				{
					NEW_MALFORMED;
					return SDECODE_FMT_8327;
				}
				break;

			case  20:	/* PI -  Session User Requirements param */
				if (param_len == 2)
				{
#ifdef SDECODE_OFFLINE
					if(p8327 != NULL)
					{
						SDecodeTree *p = p8327->New(m_pPreprocessInfo->m_pViewBuffer,dec_buf+idx-m_pPreprocessInfo->m_pViewBuffer,1,"Session User Requirements");
						p->New(m_pPreprocessInfo->m_pViewBuffer,dec_buf+idx-len_bytes-1-m_pPreprocessInfo->m_pViewBuffer,1,"Parameter Type","Session User Requirements");
						p->New(m_pPreprocessInfo->m_pViewBuffer,dec_buf+idx-len_bytes-m_pPreprocessInfo->m_pViewBuffer,len_bytes,"Parameter Length",
							SString::toFormat("%d",GetIntWithBufLen(&dec_buf[idx]-len_bytes,len_bytes)).data());
						p->New(m_pPreprocessInfo->m_pViewBuffer,dec_buf+idx-m_pPreprocessInfo->m_pViewBuffer,2,"Flags",
								SString::toFormat("0x%04X",ntohs(*(unsigned short*)&dec_buf[idx])).data());
					}
#endif
				}
				else
				{
					NEW_MALFORMED;
					return SDECODE_FMT_8327;
				}
				break;

			case  51:	/* PI -  Calling Session Selector param */
				if (param_len <= MAX_SSEL_LEN) /* 16 */
				{
#ifdef SDECODE_OFFLINE
					if(p8327 != NULL)
					{
						SDecodeTree *p = p8327->New(m_pPreprocessInfo->m_pViewBuffer,dec_buf+idx-m_pPreprocessInfo->m_pViewBuffer,1,"Calling Session Selector");
						p->New(m_pPreprocessInfo->m_pViewBuffer,dec_buf+idx-len_bytes-1-m_pPreprocessInfo->m_pViewBuffer,1,"Parameter Type","Calling Session Selector");
						p->New(m_pPreprocessInfo->m_pViewBuffer,dec_buf+idx-len_bytes-m_pPreprocessInfo->m_pViewBuffer,len_bytes,"Parameter Length",
							SString::toFormat("%d",GetIntWithBufLen(&dec_buf[idx]-len_bytes,len_bytes)).data());
						p->New(m_pPreprocessInfo->m_pViewBuffer,dec_buf+idx-m_pPreprocessInfo->m_pViewBuffer,param_len,"Calling Session Selector",
							SString::toFormat("%04X",ntohs(*(unsigned short*)&dec_buf[idx])).data());
					}
#endif
				}
				else
				{
					NEW_MALFORMED;
					return SDECODE_FMT_8327;
				}
				break;

			case  52:	/* PI - Called(CN)/Responding(AC) Session Selector param */
				if (param_len <= MAX_SSEL_LEN) /* 16 */
				{
					if (spdu_type == COSP_SI_CONNECT)
					{
#ifdef SDECODE_OFFLINE
						if(p8327 != NULL)
						{
							SDecodeTree *p = p8327->New(m_pPreprocessInfo->m_pViewBuffer,dec_buf+idx-m_pPreprocessInfo->m_pViewBuffer,1,"Called Session Selector");
							p->New(m_pPreprocessInfo->m_pViewBuffer,dec_buf+idx-len_bytes-1-m_pPreprocessInfo->m_pViewBuffer,1,"Parameter Type","Called Session Selector");
							p->New(m_pPreprocessInfo->m_pViewBuffer,dec_buf+idx-len_bytes-m_pPreprocessInfo->m_pViewBuffer,len_bytes,"Parameter Length",
								SString::toFormat("%d",GetIntWithBufLen(&dec_buf[idx]-len_bytes,len_bytes)).data());
							p->New(m_pPreprocessInfo->m_pViewBuffer,dec_buf+idx-m_pPreprocessInfo->m_pViewBuffer,param_len,"Called Session Selector",
								SString::toFormat("%04X",ntohs(*(unsigned short*)&dec_buf[idx])).data());
						}
#endif
					}
					else
					{
#ifdef SDECODE_OFFLINE
						if(p8327 != NULL)
						{
							SDecodeTree *p = p8327->New(m_pPreprocessInfo->m_pViewBuffer,dec_buf+idx-m_pPreprocessInfo->m_pViewBuffer,1,"Responding Session Selector");
							p->New(m_pPreprocessInfo->m_pViewBuffer,dec_buf+idx-len_bytes-1-m_pPreprocessInfo->m_pViewBuffer,1,"Parameter Type","Responding Session Selector");
							p->New(m_pPreprocessInfo->m_pViewBuffer,dec_buf+idx-len_bytes-m_pPreprocessInfo->m_pViewBuffer,len_bytes,"Parameter Length",
								SString::toFormat("%d",GetIntWithBufLen(&dec_buf[idx]-len_bytes,len_bytes)).data());
							p->New(m_pPreprocessInfo->m_pViewBuffer,dec_buf+idx-m_pPreprocessInfo->m_pViewBuffer,param_len,"Responding Session Selector",
								SString::toFormat("%04X",ntohs(*(unsigned short*)&dec_buf[idx])).data());
						}
#endif
					}
				}
				else
				{
					NEW_MALFORMED;
					return SDECODE_FMT_8327;
				}
				break;

			case  193:	/* PGI - User Data - used only if len <=512 in CONNECT and	*/
				/* len up to 65539 total SPDU size in ACCEPT (ISO 8327/DAD2)	*/
				if ((spdu_type == COSP_SI_CONNECT && param_len <= 512) ||
					(spdu_type == COSP_SI_ACCEPT  && param_len <= COSP_MAX_UDATA))
				{
					pUserData = (BYTE*)&dec_buf[idx];
					iUserDataLen = GetIntWithBufLen(&dec_buf[idx]-len_bytes,len_bytes);
#ifdef SDECODE_OFFLINE
					if(p8327 != NULL)
					{
						SDecodeTree *p = p8327->New(m_pPreprocessInfo->m_pViewBuffer,dec_buf+idx-m_pPreprocessInfo->m_pViewBuffer,1,"User Data");
						p->New(m_pPreprocessInfo->m_pViewBuffer,dec_buf+idx-len_bytes-1-m_pPreprocessInfo->m_pViewBuffer,1,"Parameter Type","User Data");
						p->New(m_pPreprocessInfo->m_pViewBuffer,dec_buf+idx-len_bytes-m_pPreprocessInfo->m_pViewBuffer,len_bytes,"Parameter Length",
							SString::toFormat("%d",iUserDataLen).data());
					}
#endif
				}
				else
				{
					NEW_MALFORMED;
					return SDECODE_FMT_8327;
				}
				break;

			case  194:	/* PGI - Extended User Data - used if len>512	*/
				if (param_len <= COSP_MAX_UDATA_CON)	/* and len<=10240 	*/
				{
#ifdef SDECODE_OFFLINE
					if(p8327 != NULL)
					{
						SDecodeTree *p = p8327->New(m_pPreprocessInfo->m_pViewBuffer,dec_buf+idx-m_pPreprocessInfo->m_pViewBuffer,1,"Extended User Data");
						p->New(m_pPreprocessInfo->m_pViewBuffer,dec_buf+idx-len_bytes-1-m_pPreprocessInfo->m_pViewBuffer,1,"Parameter Type","Extended User Data");
						p->New(m_pPreprocessInfo->m_pViewBuffer,dec_buf+idx-len_bytes-m_pPreprocessInfo->m_pViewBuffer,len_bytes,"Parameter Length",
							SString::toFormat("%d",GetIntWithBufLen(&dec_buf[idx]-len_bytes,len_bytes)).data());
					}
#endif
				}
				else
				{
					NEW_MALFORMED;
					return SDECODE_FMT_8327;
				}
				break;

			default:
				NEW_MALFORMED;
				return SDECODE_FMT_8327;
			} /* end of switch */

			/* adjust processing vars by the length of param processed */
			if (ret == SD_SUCCESS)
			{
				dec_len -= param_len;
				idx     += param_len;
			}
			else
				break;
		} /* end while len to decode */

		break;	/* this is end of decoding, exit loop unconditionally */
	}  /* end of while (SD_TRUE) loop */

	//	if (ret == SD_SUCCESS)
	/* validate the received SPDU */
	// 	ret = cosp_validate_cn_ac (dec_par, spdu_type);

	return (ret);
}



//////////////////////////////////////////////////////////////////////////
// 描    述:  8823解码
// 作    者:  邵凯田
// 创建时间:  2015-5-25 19:43
// 参数说明:  @pDatbuffer表示缓冲区地址
//         :  @iLen表示长度
// 返 回 值:  0表示成功，否则表示失败
//////////////////////////////////////////////////////////////////////////
int SDecodeMms::Decode8823(BYTE *pDatBuffer,int iLen)
{
	SDecodeMms *pThis = this;
	int ret=0;
	m_iFmtErrorNo = SDECODE_FMT_8823;
#ifdef SDECODE_OFFLINE
	SDecodeTree *pTreeNode=NULL;
	if(pThis->m_pDecodeTreeRoot != NULL)
	{
		pThis->m_pLastTreeNode = pTreeNode = pThis->m_pDecodeTreeRoot->New(pThis->m_pPreprocessInfo->m_pViewBuffer,
			pDatBuffer-pThis->m_pPreprocessInfo->m_pViewBuffer,iLen,"ISO 8823 OSI Presentation Protocol");
	}
#endif
	ret = DecodeAsn1Ber(pDatBuffer,iLen,Callback_8823,this);
	return ret;
}

int SDecodeMms::Callback_8823(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
// 	SDecodeMms *pThis = (SDecodeMms*)cbParam;
// 	int ret = 0;

	if(pPdu->tag.id == 0x11)
	{
		if(pThis->m_pLastSessionStatus->m_iCospType == COSP_SI_CONNECT)
		{
			//CP-type
			DECODE_TREE_SON("CP-type","","");
			ret = pThis->DecodeAsn1Ber(pPdu->val,pPdu->len,Callback_8823_CP_type,pThis);
		}
		else if(pThis->m_pLastSessionStatus->m_iCospType == COSP_SI_ACCEPT)
		{
			//CPA-PPDU
			DECODE_TREE_SON("CPA-PPDU","","");
			ret = pThis->DecodeAsn1Ber(pPdu->val,pPdu->len,Callback_8823_CPA_PPDU,pThis);
		}
	}
	else if(pPdu->tag.id == 0x01)
	{
		//CPA-PPDU
		DECODE_TREE_SON("User-data","","");
		ret = pThis->DecodeAsn1Ber(pPdu->val,pPdu->len,Callback_8823_user_data,pThis);
	}
	DECODE_CALLBACK_RET;
}


int SDecodeMms::Callback_8823_CP_type(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	if(pPdu->tag.id == 0)
	{//mode-selector
		DECODE_TREE_SON("mode-selector","","");
		ret = pThis->DecodeAsn1Ber(pPdu->val,pPdu->len,Callback_8823_mode_selector,pThis);
	}
	else if(pPdu->tag.id == 1)
	{//x410-mode-parameters
		DECODE_TREE_SON("x410-mode-parameters","","");
	}
	else if(pPdu->tag.id == 2)
	{//normal-mode-parameters
		DECODE_TREE_SON("normal-mode-parameters","","");
		ret = pThis->DecodeAsn1Ber(pPdu->val,pPdu->len,Callback_8823_normal_mode_parameters,pThis);
	}
	DECODE_CALLBACK_RET;
}

int SDecodeMms::Callback_8823_CPA_PPDU(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	if(pPdu->tag.id == 0)
	{//mode-selector
		DECODE_TREE_SON("mode-selector","","");
		ret = pThis->DecodeAsn1Ber(pPdu->val,pPdu->len,Callback_8823_mode_selector,pThis);
	}
	else if(pPdu->tag.id == 1)
	{//x410-mode-parameters
		DECODE_TREE_SON("x410-mode-parameters","","");
	}
	else if(pPdu->tag.id == 2)
	{//normal-mode-parameters
		DECODE_TREE_SON("normal-mode-parameters","","");
		ret = pThis->DecodeAsn1Ber(pPdu->val,pPdu->len,Callback_8823_CPA_normal_mode_parameters,pThis);
	}
	DECODE_CALLBACK_RET;
}

int SDecodeMms::Callback_8823_mode_selector(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	DECODE_TREE_SON("mode-value",pPdu->val[0] == 0?"x410-1984-mode(0)":"normal-mode(1)","");
	DECODE_CALLBACK_RET;
}

int SDecodeMms::Callback_8823_normal_mode_parameters(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	switch(pPdu->tag.id)
	{
// 	normal-mode-parameters	[2] IMPLICIT SEQUENCE 
//	{protocol-version
// 		[0] IMPLICIT Protocol-version DEFAULT {version-1},
	case 0:
		DECODE_TREE_SON("protocol-version","","");
		break;
// 		calling-presentation-selector
// 		[1] IMPLICIT Calling-presentation-selector
// 		OPTIONAL,
	case 1:
		if(pPdu->tag.fmt == 0)
		{
			DECODE_TREE_SON("calling-presentation-selector","","");
		}
		else
		{
			DECODE_TREE_SON("User-data","","");
			ret = pThis->DecodeAsn1Ber(pPdu->val,pPdu->len,Callback_8823_user_data,pThis);
		}
		break;
// 		called-presentation-selector
// 		[2] IMPLICIT Called-presentation-selector OPTIONAL,
	case 2:
		DECODE_TREE_SON("called-presentation-selector","","");
		break;
// 		presentation-context-definition-list
// 		[4] IMPLICIT Presentation-context-definition-list
// 		OPTIONAL,
	case 4:
		DECODE_TREE_SON("presentation-context-definition-list","","");
		break;
// 		default-context-name
// 		[6] IMPLICIT Default-context-name OPTIONAL,
	case 6:
		DECODE_TREE_SON("default-context-name","","");
		break;
// 		presentation-requirements
// 		[8] IMPLICIT Presentation-requirements OPTIONAL,
	case 8:
		DECODE_TREE_SON("presentation-requirements","","");
		break;
// 		user-session-requirements
// 		[9] IMPLICIT User-session-requirements OPTIONAL-- shall not be present if equal to the Revised session
// 		-- requirements parameter --,
// 		protocol-options
	case 9:
		DECODE_TREE_SON("user-session-requirements","","");
		break;
// 		protocol-options
// 		[11]  Protocol-options DEFAULT {},
// 		--  shall be absent if no options are offered
// 		initiators-nominated-context
	case 11:
		DECODE_TREE_SON("protocol-options","","");
		break;
// 		initiators-nominated-context
// 		[12]  Presentation-context-identifier OPTIONAL,
// 		--  shall only be present if nominated-context is
// 		--  offered in protocol-options
// 		extensions
	case 12:
		DECODE_TREE_SON("initiators-nominated-context","","");
		break;
// 		extensions
// 		[14]  SEQUENCE {...} OPTIONAL,
// 		--  to allow for future extensions
	case 14:
		DECODE_TREE_SON("extensions","","");
		break;
// 		user-data
// 		User-data OPTIONAL} OPTIONAL
	}
	
	DECODE_CALLBACK_RET;
}


int SDecodeMms::Callback_8823_CPA_normal_mode_parameters(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	switch(pPdu->tag.id)
	{
// 			protocol-version
// 			[0] IMPLICIT Protocol-version DEFAULT {version-1},
	case 0:
		DECODE_TREE_SON("protocol-version","","");
		break;
// 			responding-presentation-selector
// 			[3] IMPLICIT Responding-presentation-selector
// 			OPTIONAL,
	case 3:
		DECODE_TREE_SON("responding-presentation-selector","","");
		break;
// 			presentation-context-definition-result-list
// 			[5] IMPLICIT Presentation-context-definition-result-list
// 			OPTIONAL,
	case 5:
		DECODE_TREE_SON("presentation-context-definition-result-list","","");
		break;
// 			presentation-requirements
// 			[8] IMPLICIT Presentation-requirements OPTIONAL,
	case 8:
		DECODE_TREE_SON("presentation-requirements","","");
		break;
// 			user-session-requirements
// 			[9] IMPLICIT User-session-requirements OPTIONAL,
	case 9:
		DECODE_TREE_SON("user-session-requirements","","");
		break;
// 			--  shall not be present if equal to the Revised session
// 			--  requirements parameter 
// 			protocol-options
// 			[11]  Protocol-options DEFAULT {},
	case 11:
		DECODE_TREE_SON("protocol-options","","");
		break;
// 			--  shall be absent if no options are selected
// 			responders-nominated-context
// 			[13]  Presentation-context-identifier OPTIONAL,
	case 13:
		DECODE_TREE_SON("responders-nominated-context","","");
		break;
// 			--  shall only be present if nominated-context is 
// 			--  selected in protocol-options
// 			user-data
// 			User-data OPTIONAL} OPTIONAL
	case 1:
		if(pPdu->tag.fmt == 1)
		{
			DECODE_TREE_SON("User-data","","");
			ret = pThis->DecodeAsn1Ber(pPdu->val,pPdu->len,Callback_8823_user_data,pThis);
		}
		break;

	}

	DECODE_CALLBACK_RET;
}

int SDecodeMms::Callback_8823_user_data(void* cbParam, stuAsn1Pdu *pPdu)
{
	//simply-encoded-data  [APPLICATION 0] IMPLICIT Simply-encoded-data,
	//fully-encoded-data   [APPLICATION 1] IMPLICIT Fully-encoded-data,
	DECODE_CALLBACK_INTO;
	if(pPdu->tag.id == 0)
	{
		DECODE_TREE_SON("Simply-encoded-data","","");
	}
	else
	{
		DECODE_TREE_SON("Fully-encoded-data","","");
		DECODE_TREE_SON("PDV-list","","");
		ret = pThis->DecodeAsn1Ber(pPdu->val,pPdu->len,Callback_8823_PDV_list,pThis);
	}
	
	DECODE_CALLBACK_RET;
}

int SDecodeMms::Callback_8823_PDV_list(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	if(pPdu->tag.id == 2)
	{
		//Presentation-context-identifier
		pThis->m_pLastSessionStatus->m_i8823ContextId = pPdu->val[0];
		#ifdef SDECODE_OFFLINE
		DECODE_TREE_SON("Presentation-context-identifier",
			SString::toFormat("%d%s",pPdu->val[0],pPdu->val[0]==1?" (id-as-acse)":(pPdu->val[0]==3?" (mms-abstract-syntax-version1(1))":"")).data(),"");
		#endif
	}
	else if(pPdu->tag.id == 0 && pPdu->tag.fmt == 1)
	{
		DECODE_TREE_SON("presentation-data-values","single-ASN1-type(0)","");
		if(pThis->m_pLastSessionStatus->m_i8823ContextId == 1)
		{
			//id-as-acse
			ret = pThis->Decode8650(pPdu->val,pPdu->len);
		}
		else if(pThis->m_pLastSessionStatus->m_i8823ContextId == 3)
		{
			//mms
			ret = pThis->DecodeMms(pPdu->val,pPdu->len);
		}
	}
	
	DECODE_CALLBACK_RET;
}


int SDecodeMms::Decode8650(BYTE *pDatBuffer,int iLen)
{
	SDecodeMms *pThis = this;
	m_iFmtErrorNo = SDECODE_FMT_8650;
	int ret=0;
#ifdef SDECODE_OFFLINE
	SDecodeTree *pTreeNode=NULL;
	if(pThis->m_pDecodeTreeRoot != NULL)
	{
		pThis->m_pLastTreeNode = pTreeNode = pThis->m_pDecodeTreeRoot->New(pThis->m_pPreprocessInfo->m_pViewBuffer,
			pDatBuffer-pThis->m_pPreprocessInfo->m_pViewBuffer,iLen,"ISO 8650-1 OSI Association Control Service");
	}
#endif
	ret = pThis->DecodeAsn1Ber(pDatBuffer,iLen,Callback_8650,pThis);
	return ret;
}

int SDecodeMms::Callback_8650(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	switch (pPdu->tag.id)
	{
	case 0:
		DECODE_TREE_SON("aarq","","");
		ret = pThis->DecodeAsn1Ber(pPdu->val,pPdu->len,Callback_8650_aarq,pThis);
		break;
	case 1:
		DECODE_TREE_SON("aare","","");
		ret = pThis->DecodeAsn1Ber(pPdu->val,pPdu->len,Callback_8650_aare,pThis);
		break;
	case 2:
		DECODE_TREE_SON("rlrq","","");
		ret = pThis->DecodeAsn1Ber(pPdu->val,pPdu->len,Callback_8650_rlrq,pThis);
		break;
	case 3:
		DECODE_TREE_SON("rlre","","");
		ret = pThis->DecodeAsn1Ber(pPdu->val,pPdu->len,Callback_8650_rlre,pThis);
		break;
	default:
		break;
	}
	DECODE_CALLBACK_RET;
}

int SDecodeMms::Callback_8650_aarq(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	switch(pPdu->tag.id)
	{
// 		protocol-version
// 		[0] IMPLICIT BIT STRING {version1(0)} DEFAULT {version1},
	case 0:
		#ifdef SDECODE_OFFLINE
		DECODE_TREE_SON("protocol-version",SString::toFormat("%02X%s",pPdu->len>1?pPdu->val[1]:pPdu->val[0],((pPdu->len>1?pPdu->val[1]:pPdu->val[0])&0x80)!=0?" (version1)":"").data(),"");
		#endif
		break;
// 		aSO-context-name                  [1]  ASO-context-name,
	case 1:
		DECODE_TREE_SON("aSO-context-name","","");
		break;
// 		called-AP-title                   [2]  AP-title OPTIONAL,
	case 2:
		DECODE_TREE_SON("called-AP-title","","");
		break;
// 		called-AE-qualifier               [3]  AE-qualifier OPTIONAL,
	case 3:
		DECODE_TREE_SON("called-AE-qualifier","","");
		break;
// 		called-AP-invocation-identifier   [4]  AP-invocation-identifier OPTIONAL,
	case 4:
		DECODE_TREE_SON("called-AP-invocation-identifier","","");
		break;
// 		called-AE-invocation-identifier   [5]  AE-invocation-identifier OPTIONAL,
	case 5:
		DECODE_TREE_SON("called-AE-invocation-identifier","","");
		break;
// 		calling-AP-title                  [6]  AP-title OPTIONAL,
	case 6:
		DECODE_TREE_SON("calling-AP-title","","");
		break;
// 		calling-AE-qualifier              [7]  AE-qualifier OPTIONAL,
	case 7:
		DECODE_TREE_SON("calling-AE-qualifier","","");
		break;
// 		calling-AP-invocation-identifier  [8]  AP-invocation-identifier OPTIONAL,
	case 8:
		DECODE_TREE_SON("calling-AP-invocation-identifier","","");
		break;
// 		calling-AE-invocation-identifier  [9]  AE-invocation-identifier OPTIONAL,
	case 9:
		DECODE_TREE_SON("calling-AE-invocation-identifier","","");
		break;
// 		--  The following field shall not be present if only the Kernel is used.
// 		sender-acse-requirements          [10] IMPLICIT ACSE-requirements OPTIONAL,
	case 10:
		DECODE_TREE_SON("sender-acse-requirements","","");
		break;
// 		--  The following field shall only be present if the Authentication functional unit is selected.
// 		mechanism-name                    [11] IMPLICIT Mechanism-name OPTIONAL,
	case 11:
		DECODE_TREE_SON("mechanism-name","","");
		break;
// 		--  The following field shall only be present if the Authentication functional unit is selected.
// 		calling-authentication-value      [12] EXPLICIT Authentication-value OPTIONAL,
	case 12:
		DECODE_TREE_SON("calling-authentication-value","","");
		break;
// 		aSO-context-name-list
// 		[13] IMPLICIT ASO-context-name-list OPTIONAL,
// 		--  The above field shall only be present if the Application Context Negotiation functional unit is selected.
// 		implementation-information        [29] IMPLICIT Implementation-data OPTIONAL,
	case 13:
		DECODE_TREE_SON("aSO-context-name-list","","");
		break;
// 		...,
// 		--  Extensions for higher level association FU
// 		p-context-definition-list         [14]  Syntactic-context-list OPTIONAL,
	case 14:
		DECODE_TREE_SON("p-context-definition-list","","");
		break;
// 		called-asoi-tag                   [15] IMPLICIT ASOI-tag OPTIONAL,
	case 15:
		DECODE_TREE_SON("called-asoi-tag","","");
		break;
// 		calling-asoi-tag                  [16] IMPLICIT ASOI-tag OPTIONAL,
	case 16:
		DECODE_TREE_SON("calling-asoi-tag","","");
		break;
// 		--  End of extensions for higher level association FU
// 		--  ...,
// 		user-information                  [30] IMPLICIT Association-data OPTIONAL
	case 30:
		DECODE_TREE_SON("user-information","","");
		ret = pThis->DecodeAsn1Ber(pPdu->val,pPdu->len,Callback_8650_Association_data,pThis);
		break;
	}
	DECODE_CALLBACK_RET;
}

int SDecodeMms::Callback_8650_Association_data(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	DECODE_TREE_SON("Association-data","","");
	ret = pThis->DecodeAsn1Ber(pPdu->val,pPdu->len,Callback_8650_EXTERNALt,pThis);
	DECODE_CALLBACK_RET;
}

int SDecodeMms::Callback_8650_EXTERNALt(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	if(pPdu->tag.id == 2)
	{
		#ifdef SDECODE_OFFLINE
		DECODE_TREE_SON("indirect-reference",SString::toFormat("%d",pThis->GetIntWithBufLen(pPdu->val,pPdu->len)).data(),"");
		#endif
	}
	else if(pPdu->tag.fmt == 1)
	{
		//encoding
		if(pPdu->tag.id == 0)
		{
			DECODE_TREE_SON("encoding","single-ASN1-type (0)","");
			ret = pThis->DecodeMms(pPdu->val,pPdu->len);
		}
		else if(pPdu->tag.id == 1)
		{
			DECODE_TREE_SON("encoding","octet-aligned (1)","");
		}
		else if(pPdu->tag.id == 2)
		{
			DECODE_TREE_SON("encoding","arbitrary (2)","");
		}
	}
// 	direct-reference  OBJECT IDENTIFIER OPTIONAL,
// 		indirect-reference  INTEGER OPTIONAL,
// 		data-value-descriptor  ObjectDescriptor  OPTIONAL,
// 		encoding  CHOICE
// 	{single-ASN1-type  [0] ANY,
// 	octet-aligned     [1] IMPLICIT OCTET STRING,
// 	arbitrary         [2] IMPLICIT BIT STRING}

	DECODE_CALLBACK_RET;
}

int SDecodeMms::Callback_8650_aare(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;

	switch(pPdu->tag.id)
	{
// 		protocol-version
// 			[0] IMPLICIT BIT STRING {version1(0)} DEFAULT {version1},
	case 0:
#ifdef SDECODE_OFFLINE
		DECODE_TREE_SON("protocol-version",SString::toFormat("%02X%s",pPdu->len>1?pPdu->val[1]:pPdu->val[0],((pPdu->len>1?pPdu->val[1]:pPdu->val[0])&0x80)!=0?" (version1)":"").data(),"");
#endif
		break;
// 			aSO-context-name                     [1]  ASO-context-name,
	case 1:
		DECODE_TREE_SON("aSO-context-name","","");
		break;
// 			result                               [2]  Associate-result,
	case 2:
		DECODE_TREE_SON("result","","");
		break;
// 			result-source-diagnostic             [3]  Associate-source-diagnostic,
	case 3:
		DECODE_TREE_SON("result-source-diagnostic","","");
		break;
// 			responding-AP-title                  [4]  AP-title OPTIONAL,
	case 4:
		DECODE_TREE_SON("responding-AP-title","","");
		break;
// 			responding-AE-qualifier              [5]  AE-qualifier OPTIONAL,
	case 5:
		DECODE_TREE_SON("responding-AE-qualifier","","");
		break;
// 			responding-AP-invocation-identifier  [6]  AP-invocation-identifier OPTIONAL,
	case 6:
		DECODE_TREE_SON("responding-AP-invocation-identifier","","");
		break;
// 			responding-AE-invocation-identifier  [7]  AE-invocation-identifier OPTIONAL,
	case 7:
		DECODE_TREE_SON("responding-AE-invocation-identifier","","");
		break;
// 			--  The following field shall not be present if only the Kernel is used.
// 			responder-acse-requirements          [8] IMPLICIT ACSE-requirements OPTIONAL,
// 			--  The following field shall only be present if the Authentication functional unit is selected.
// 			mechanism-name                       [9] IMPLICIT Mechanism-name OPTIONAL,
// 			--  This following field shall only be present if the Authentication functional unit is selected.
// 			responding-authentication-value
// 			[10] EXPLICIT Authentication-value OPTIONAL,
	case 10:
		DECODE_TREE_SON("responding-authentication-value","","");
		break;
// 			aSO-context-name-list
// 			[11] IMPLICIT ASO-context-name-list OPTIONAL,
	case 11:
		DECODE_TREE_SON("aSO-context-name-list","","");
		break;
// 			--  The above field shall only be present if the Application Context Negotiation functional unit is selected.
// 			implementation-information
// 			[29] IMPLICIT Implementation-data OPTIONAL,
	case 29:
		DECODE_TREE_SON("implementation-information","","");
		break;
// 			...,
// 			--  Extensions for higher level association FU
// 			p-context-result-list
// 			[12] IMPLICIT P-context-result-list OPTIONAL,
	case 12:
		DECODE_TREE_SON("p-context-result-list","","");
		break;
// 			called-asoi-tag                      [13] IMPLICIT ASOI-tag OPTIONAL,
	case 13:
		DECODE_TREE_SON("called-asoi-tag","","");
		break;
// 			calling-asoi-tag                     [14] IMPLICIT ASOI-tag OPTIONAL,
	case 14:
		DECODE_TREE_SON("calling-asoi-tag","","");
		break;
// 			--  End of extensions for higher level association FU
// 			--  ...,
// 			user-information                     [30] IMPLICIT Association-data OPTIONAL
	case 30:
		DECODE_TREE_SON("user-information","","");
		ret = pThis->DecodeAsn1Ber(pPdu->val,pPdu->len,Callback_8650_Association_data,pThis);
		break;

	}

	DECODE_CALLBACK_RET;
}

int SDecodeMms::Callback_8650_rlrq(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	DECODE_CALLBACK_RET;
}

int SDecodeMms::Callback_8650_rlre(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	DECODE_CALLBACK_RET;
}

int SDecodeMms::DecodeMms(BYTE *pDatBuffer,int iLen)
{
	SDecodeMms *pThis = this;
	m_iFmtErrorNo = SDECODE_FMT_MMS;
	int ret=0;
#ifdef SDECODE_OFFLINE
	SDecodeTree *pTreeNode=NULL;
	if(pThis->m_pDecodeTreeRoot != NULL)
	{
		pThis->m_pLastTreeNode = pTreeNode = pThis->m_pDecodeTreeRoot->New(pThis->m_pPreprocessInfo->m_pViewBuffer,
			pDatBuffer-pThis->m_pPreprocessInfo->m_pViewBuffer,iLen,"MMS");
	}
#endif
	stuAsn1Pdu pdu;
	int offset = 0;
	offset=get_ber_identifier(pDatBuffer, offset, &pdu.tag.cls, &pdu.tag.fmt, &pdu.tag.id);
	if(offset < 0)
		return m_iFmtErrorNo;
	offset=get_ber_length(pDatBuffer, offset, &pdu.len, /*&pdu.ind*/NULL);
	if(offset < 0)
		return m_iFmtErrorNo;
	pdu.val = pDatBuffer;//+offset;
	pdu.len = iLen;//-offset;
	ret = Callback_MMS_pdu(pThis,&pdu);
	//ret = pThis->DecodeAsn1Ber(pDatBuffer,iLen,Callback_MMS_pdu,pThis);
	return ret;
}

#include "SDecodeMmsDeep.cpp"
//int dissect_mms_MMSpdu(void* cbParam, stuAsn1Pdu *pPdu);
int SDecodeMms::Callback_MMS_pdu(void* cbParam, stuAsn1Pdu *pPdu)
{
	return dissect_mms_MMSpdu(cbParam,pPdu);
#if 0
	DECODE_CALLBACK_INTO;
	switch(pPdu->tag.id)
	{
	// 	MMSpdu ::= CHOICE {
	// 		confirmed-RequestPDU     [0] IMPLICIT Confirmed-RequestPDU,
	case 0:
		DECODE_TREE_SON("confirmed-RequestPDU","","");
		ret = pThis->DecodeAsn1Ber(pPdu->val,pPdu->len,Callback_MMS_confirmed_RequestPDU,pThis);
		break;
	// 			confirmed-ResponsePDU    [1] IMPLICIT Confirmed-ResponsePDU,
	case 1:
		DECODE_TREE_SON("confirmed-ResponsePDU","","");
		ret = pThis->DecodeAsn1Ber(pPdu->val,pPdu->len,Callback_MMS_confirmed_ResponsePDU,pThis);
		break;
	// 			confirmed-ErrorPDU  [2] IMPLICIT Confirmed-ErrorPDU,
	case 2:
		DECODE_TREE_SON("confirmed-ErrorPDU","","");
		ret = pThis->DecodeAsn1Ber(pPdu->val,pPdu->len,Callback_MMS_confirmed_ErrorPDU,pThis);
		break;
	// 			unconfirmed-PDU     [3] IMPLICIT Unconfirmed-PDU,
	case 3:
		DECODE_TREE_SON("unconfirmed-PDU","","");
		ret = pThis->DecodeAsn1Ber(pPdu->val,pPdu->len,Callback_MMS_unconfirmed_PDU,pThis);
		break;
	// 			rejectPDU [4] IMPLICIT RejectPDU,
	case 4:
		DECODE_TREE_SON("rejectPDU","","");
		ret = pThis->DecodeAsn1Ber(pPdu->val,pPdu->len,Callback_MMS_rejectPDU,pThis);
		break;
	// 			cancel-RequestPDU   [5] IMPLICIT Cancel-RequestPDU,
	case 5:
		DECODE_TREE_SON("cancel-RequestPDU","","");
		ret = pThis->DecodeAsn1Ber(pPdu->val,pPdu->len,Callback_MMS_cancel_RequestPDU,pThis);
		break;
	// 			cancel-ResponsePDU  [6] IMPLICIT Cancel-ResponsePDU,
	case 6:
		DECODE_TREE_SON("cancel-ResponsePDU","","");
		ret = pThis->DecodeAsn1Ber(pPdu->val,pPdu->len,Callback_MMS_cancel_ResponsePDU,pThis);
		break;
	// 			cancel-ErrorPDU     [7] IMPLICIT Cancel-ErrorPDU,
	case 7:
		DECODE_TREE_SON("cancel-ErrorPDU","","");
		ret = pThis->DecodeAsn1Ber(pPdu->val,pPdu->len,Callback_MMS_cancel_ErrorPDU,pThis);
		break;
	// 			initiate-RequestPDU [8] IMPLICIT Initiate-RequestPDU,
	case 8:
		DECODE_TREE_SON("initiate-RequestPDU","","");
		ret = pThis->DecodeAsn1Ber(pPdu->val,pPdu->len,Callback_MMS_initiate_RequestPDU,pThis);
		break;
	// 			initiate-ResponsePDU     [9] IMPLICIT Initiate-ResponsePDU,
	case 9:
		DECODE_TREE_SON("initiate-ResponsePDU","","");
		ret = pThis->DecodeAsn1Ber(pPdu->val,pPdu->len,Callback_MMS_initiate_ResponsePDU,pThis);
		break;
	// 			initiate-ErrorPDU   [10] IMPLICIT Initiate-ErrorPDU,
	case 10:
		DECODE_TREE_SON("initiate-ErrorPDU","","");
		ret = pThis->DecodeAsn1Ber(pPdu->val,pPdu->len,Callback_MMS_initiate_ErrorPDU,pThis);
		break;
	// 			conclude-RequestPDU [11] IMPLICIT Conclude-RequestPDU,
	case 11:
		DECODE_TREE_SON("conclude-RequestPDU","","");
		ret = pThis->DecodeAsn1Ber(pPdu->val,pPdu->len,Callback_MMS_conclude_RequestPDU,pThis);
		break;
	// 			conclude-ResponsePDU     [12] IMPLICIT Conclude-ResponsePDU,
	case 12:
		DECODE_TREE_SON("conclude-ResponsePDU","","");
		ret = pThis->DecodeAsn1Ber(pPdu->val,pPdu->len,Callback_MMS_conclude_ResponsePDU,pThis);
		break;
	// 			conclude-ErrorPDU   [13] IMPLICIT Conclude-ErrorPDU
	case 13:
		DECODE_TREE_SON("conclude-ErrorPDU","","");
		ret = pThis->DecodeAsn1Ber(pPdu->val,pPdu->len,Callback_MMS_conclude_ErrorPDU,pThis);
		break;
	// 	} 

	}

	DECODE_CALLBACK_RET;
#endif
}

#if 0

static const stuDecodeIdName mms_ConfirmedServiceRequest_vals[] = {
	{   0, "status" },
	{   1, "getNameList" },
	{   2, "identify" },
	{   3, "rename" },
	{   4, "read" },
	{   5, "write" },
	{   6, "getVariableAccessAttributes" },
	{   7, "defineNamedVariable" },
	{   8, "defineScatteredAccess" },
	{   9, "getScatteredAccessAttributes" },
	{  10, "deleteVariableAccess" },
	{  11, "defineNamedVariableList" },
	{  12, "getNamedVariableListAttributes" },
	{  13, "deleteNamedVariableList" },
	{  14, "defineNamedType" },
	{  15, "getNamedTypeAttributes" },
	{  16, "deleteNamedType" },
	{  17, "input" },
	{  18, "output" },
	{  19, "takeControl" },
	{  20, "relinquishControl" },
	{  21, "defineSemaphore" },
	{  22, "deleteSemaphore" },
	{  23, "reportSemaphoreStatus" },
	{  24, "reportPoolSemaphoreStatus" },
	{  25, "reportSemaphoreEntryStatus" },
	{  26, "initiateDownloadSequence" },
	{  27, "downloadSegment" },
	{  28, "terminateDownloadSequence" },
	{  29, "initiateUploadSequence" },
	{  30, "uploadSegment" },
	{  31, "terminateUploadSequence" },
	{  32, "requestDomainDownload" },
	{  33, "requestDomainUpload" },
	{  34, "loadDomainContent" },
	{  35, "storeDomainContent" },
	{  36, "deleteDomain" },
	{  37, "getDomainAttributes" },
	{  38, "createProgramInvocation" },
	{  39, "deleteProgramInvocation" },
	{  40, "start" },
	{  41, "stop" },
	{  42, "resume" },
	{  43, "reset" },
	{  44, "kill" },
	{  45, "getProgramInvocationAttributes" },
	{  46, "obtainFile" },
	{  47, "defineEventCondition" },
	{  48, "deleteEventCondition" },
	{  49, "getEventConditionAttributes" },
	{  50, "reportEventConditionStatus" },
	{  51, "alterEventConditionMonitoring" },
	{  52, "triggerEvent" },
	{  53, "defineEventAction" },
	{  54, "deleteEventAction" },
	{  55, "getEventActionAttributes" },
	{  56, "reportEventActionStatus" },
	{  57, "defineEventEnrollment" },
	{  58, "deleteEventEnrollment" },
	{  59, "alterEventEnrollment" },
	{  60, "reportEventEnrollmentStatus" },
	{  61, "getEventEnrollmentAttributes" },
	{  62, "acknowledgeEventNotification" },
	{  63, "getAlarmSummary" },
	{  64, "getAlarmEnrollmentSummary" },
	{  65, "readJournal" },
	{  66, "writeJournal" },
	{  67, "initializeJournal" },
	{  68, "reportJournalStatus" },
	{  69, "createJournal" },
	{  70, "deleteJournal" },
	{  71, "getCapabilityList" },
	{  72, "fileOpen" },
	{  73, "fileRead" },
	{  74, "fileClose" },
	{  75, "fileRename" },
	{  76, "fileDelete" },
	{  77, "fileDirectory" },
	{ 0, NULL }
};


static const stuDecodeIdName mms_ConfirmedServiceResponse_vals[] = {
	{   0, "status" },
	{   1, "getNameList" },
	{   2, "identify" },
	{   3, "rename" },
	{   4, "read" },
	{   5, "write" },
	{   6, "getVariableAccessAttributes" },
	{   7, "defineNamedVariable" },
	{   8, "defineScatteredAccess" },
	{   9, "getScatteredAccessAttributes" },
	{  10, "deleteVariableAccess" },
	{  11, "defineNamedVariableList" },
	{  12, "getNamedVariableListAttributes" },
	{  13, "deleteNamedVariableList" },
	{  14, "defineNamedType" },
	{  15, "getNamedTypeAttributes" },
	{  16, "deleteNamedType" },
	{  17, "input" },
	{  18, "output" },
	{  19, "takeControl" },
	{  20, "relinquishControl" },
	{  21, "defineSemaphore" },
	{  22, "deleteSemaphore" },
	{  23, "reportSemaphoreStatus" },
	{  24, "reportPoolSemaphoreStatus" },
	{  25, "reportSemaphoreEntryStatus" },
	{  26, "initiateDownloadSequence" },
	{  27, "downloadSegment" },
	{  28, "terminateDownloadSequence" },
	{  29, "initiateUploadSequence" },
	{  30, "uploadSegment" },
	{  31, "terminateUploadSequence" },
	{  32, "requestDomainDownLoad" },
	{  33, "requestDomainUpload" },
	{  34, "loadDomainContent" },
	{  35, "storeDomainContent" },
	{  36, "deleteDomain" },
	{  37, "getDomainAttributes" },
	{  38, "createProgramInvocation" },
	{  39, "deleteProgramInvocation" },
	{  40, "start" },
	{  41, "stop" },
	{  42, "resume" },
	{  43, "reset" },
	{  44, "kill" },
	{  45, "getProgramInvocationAttributes" },
	{  46, "obtainFile" },
	{  72, "fileOpen" },
	{  47, "defineEventCondition" },
	{  48, "deleteEventCondition" },
	{  49, "getEventConditionAttributes" },
	{  50, "reportEventConditionStatus" },
	{  51, "alterEventConditionMonitoring" },
	{  52, "triggerEvent" },
	{  53, "defineEventAction" },
	{  54, "deleteEventAction" },
	{  55, "getEventActionAttributes" },
	{  56, "reportActionStatus" },
	{  57, "defineEventEnrollment" },
	{  58, "deleteEventEnrollment" },
	{  59, "alterEventEnrollment" },
	{  60, "reportEventEnrollmentStatus" },
	{  61, "getEventEnrollmentAttributes" },
	{  62, "acknowledgeEventNotification" },
	{  63, "getAlarmSummary" },
	{  64, "getAlarmEnrollmentSummary" },
	{  65, "readJournal" },
	{  66, "writeJournal" },
	{  67, "initializeJournal" },
	{  68, "reportJournalStatus" },
	{  69, "createJournal" },
	{  70, "deleteJournal" },
	{  71, "getCapabilityList" },
	{  73, "fileRead" },
	{  74, "fileClose" },
	{  75, "fileRename" },
	{  76, "fileDelete" },
	{  77, "fileDirectory" },
	{ 0, NULL }
};
int SDecodeMms::Callback_MMS_confirmed_RequestPDU(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	if(pPdu->tag.fmt == 0 && pPdu->tag.cls == 0)
	{
		if(pPdu->tag.id == 2)
		{
			//invokeID
			pThis->m_pLastSessionStatus->m_iMmsInvokeID = pThis->GetIntWithBufLen(pPdu->val,pPdu->len);
			#ifdef SDECODE_OFFLINE
			DECODE_TREE_SON("invokeID",SString::toFormat("%d",pThis->m_pLastSessionStatus->m_iMmsInvokeID).data(),"");
			#endif
		}
	}
	else
	{
		pThis->m_pLastSessionStatus->m_iMmsConfirmedServiceRequestId = pPdu->tag.id;
		//confirmedServiceRequest
		#ifdef SDECODE_OFFLINE
		DECODE_TREE_SON("confirmedServiceRequest",pPdu->tag.id<=77?SString::toFormat("%s (%d)",mms_ConfirmedServiceRequest_vals[pPdu->tag.id].name,pPdu->tag.id).data():SString::toFormat("%d",pPdu->tag.id).data(),"");
		#endif
// 			Callback_MMS_confirmedServiceRequest(pThis,pPdu,)
// 			ret = pThis->DecodeAsn1Ber(pPdu->val,pPdu->len,Callback_MMS_confirmedServiceRequest,pThis);
		switch(pPdu->tag.id)
		{
			// 		ConfirmedServiceRequest ::= CHOICE {
			// 			status    
			// 				[0] IMPLICIT Status-Request
		case 0:
			DECODE_TREE_SON("status","","");
			break;
			// 				,    getNameList    
			// 				[1] IMPLICIT GetNameList-Request
		case 1:
			DECODE_TREE_SON("getNameList","","");
			break;
			// 				,    identify  
			// 				[2] IMPLICIT Identify-Request
		case 2:
			DECODE_TREE_SON("identify","","");
			break;
			// 				,    rename    
			// 				[3] IMPLICIT Rename-Request
		case 3:
			DECODE_TREE_SON("rename","","");
			break;
			// 				,    read 
			// 				[4] IMPLICIT Read-Request
		case 4:
			DECODE_TREE_SON("read","","");
			break;
			// 				,    write     
			// 				[5] IMPLICIT Write-Request
		case 5:
			DECODE_TREE_SON("write","","");
			break;
			// 				,    getVariableAccessAttributes   
			// 				[6] GetVariableAccessAttributes-Request
		case 6:
			DECODE_TREE_SON("getVariableAccessAttributes","","");
			break;
			// 				,    defineNamedVariable 
			// 				[7] IMPLICIT DefineNamedVariable-Request
		case 7:
			DECODE_TREE_SON("defineNamedVariable","","");
			break;
			// 				--   [8] is reserved for a service defined in Annex E  
			// 				,    defineScatteredAccess    
			// 				[8] IMPLICIT DefineScatteredAccess-Request
		case 8:
			DECODE_TREE_SON("defineScatteredAccess","","");
			break;
			// 				--   [9] is reserved for a service defined in Annex E
			// 				,    getScatteredAccessAttributes  
			// 				[9] GetScatteredAccessAttributes-Request
		case 9:
			DECODE_TREE_SON("getScatteredAccessAttributes","","");
			break;
			// 				,    deleteVariableAccess     
			// 				[10] IMPLICIT DeleteVariableAccess-Request
		case 10:
			DECODE_TREE_SON("deleteVariableAccess","","");
			break;
			// 				,    defineNamedVariableList  
			// 				[11] IMPLICIT DefineNamedVariableList-Request
		case 11:
			DECODE_TREE_SON("defineNamedVariableList","","");
			break;
			// 				,    getNamedVariableListAttributes     
			// 				[12] GetNamedVariableListAttributes-Request
		case 12:
			DECODE_TREE_SON("getNamedVariableListAttributes","","");
			break;
			// 				,    deleteNamedVariableList  
			// 				[13] IMPLICIT DeleteNamedVariableList-Request
		case 13:
			DECODE_TREE_SON("deleteNamedVariableList","","");
			break;
			// 				,    defineNamedType     
			// 				[14] IMPLICIT DefineNamedType-Request
		case 14:
			DECODE_TREE_SON("defineNamedType","","");
			break;
			// 				,    getNamedTypeAttributes   
			// 				[15] GetNamedTypeAttributes-Request
		case 15:
			DECODE_TREE_SON("getNamedTypeAttributes","","");
			break;
			// 				,    deleteNamedType     
			// 				[16] IMPLICIT DeleteNamedType-Request
		case 16:
			DECODE_TREE_SON("deleteNamedType","","");
			break;
			// 				,    input     
			// 				[17] IMPLICIT Input-Request
		case 17:
			DECODE_TREE_SON("input","","");
			break;
			// 				,    output    
			// 				[18] IMPLICIT Output-Request
		case 18:
			DECODE_TREE_SON("output","","");
			break;
			// 				,    takeControl    
			// 				[19] IMPLICIT TakeControl-Request
		case 19:
			DECODE_TREE_SON("takeControl","","");
			break;
			// 				,    relinquishControl   
			// 				[20] IMPLICIT RelinquishControl-Request
		case 20:
			DECODE_TREE_SON("relinquishControl","","");
			break;
			// 				,    defineSemaphore     
			// 				[21] IMPLICIT DefineSemaphore-Request
		case 21:
			DECODE_TREE_SON("defineSemaphore","","");
			break;
			// 				,    deleteSemaphore     
			// 				[22] DeleteSemaphore-Request
		case 22:
			DECODE_TREE_SON("deleteSemaphore","","");
			break;
			// 				,    reportSemaphoreStatus    
			// 				[23] ReportSemaphoreStatus-Request
		case 23:
			DECODE_TREE_SON("reportSemaphoreStatus","","");
			break;
			// 				,    reportPoolSemaphoreStatus     
			// 				[24] IMPLICIT ReportPoolSemaphoreStatus-Request
		case 24:
			DECODE_TREE_SON("reportPoolSemaphoreStatus","","");
			break;
			// 				,    reportSemaphoreEntryStatus    
			// 				[25] IMPLICIT ReportSemaphoreEntryStatus-Request
		case 25:
			DECODE_TREE_SON("reportSemaphoreEntryStatus","","");
			break;
			// 				,    initiateDownloadSequence 
			// 				[26] IMPLICIT InitiateDownloadSequence-Request,
		case 26:
			DECODE_TREE_SON("initiateDownloadSequence","","");
			break;
			// 				downloadSegment     
			// 				[27] IMPLICIT DownloadSegment-Request,
		case 27:
			DECODE_TREE_SON("downloadSegment","","");
			break;
			// 				terminateDownloadSequence     
			// 				[28] IMPLICIT TerminateDownloadSequence-Request
		case 28:
			DECODE_TREE_SON("terminateDownloadSequence","","");
			break;
			// 				,    initiateUploadSequence   
			// 				[29] IMPLICIT InitiateUploadSequence-Request,
		case 29:
			DECODE_TREE_SON("initiateUploadSequence","","");
			break;
			// 				uploadSegment  
			// 				[30] IMPLICIT UploadSegment-Request,
		case 30:
			DECODE_TREE_SON("uploadSegment","","");
			break;
			// 				terminateUploadSequence  
			// 				[31] IMPLICIT TerminateUploadSequence-Request
		case 31:
			DECODE_TREE_SON("terminateUploadSequence","","");
			break;
			// 				,    requestDomainDownload    
			// 				[32] IMPLICIT RequestDomainDownload-Request
		case 32:
			DECODE_TREE_SON("requestDomainDownload","","");
			break;
			// 				,    requestDomainUpload 
			// 				[33] IMPLICIT RequestDomainUpload-Request
		case 33:
			DECODE_TREE_SON("requestDomainUpload","","");
			break;
			// 				,    loadDomainContent   
			// 				[34] IMPLICIT LoadDomainContent-Request
		case 34:
			DECODE_TREE_SON("loadDomainContent","","");
			break;
			// 				,    storeDomainContent  
			// 				[35] IMPLICIT StoreDomainContent-Request
		case 35:
			DECODE_TREE_SON("storeDomainContent","","");
			break;
			// 				,    deleteDomain   
			// 				[36] IMPLICIT DeleteDomain-Request
		case 36:
			DECODE_TREE_SON("deleteDomain","","");
			break;
			// 				,    getDomainAttributes 
			// 				[37] IMPLICIT GetDomainAttributes-Request
		case 37:
			DECODE_TREE_SON("getDomainAttributes","","");
			break;
			// 				,    createProgramInvocation  
			// 				[38] IMPLICIT CreateProgramInvocation-Request
		case 38:
			DECODE_TREE_SON("createProgramInvocation","","");
			break;
			// 				,    deleteProgramInvocation  
			// 				[39] IMPLICIT DeleteProgramInvocation-Request
		case 39:
			DECODE_TREE_SON("deleteProgramInvocation","","");
			break;
			// 				,    start     
			// 				[40] IMPLICIT Start-Request
		case 40:
			DECODE_TREE_SON("start","","");
			break;
			// 				,    stop 
			// 				[41] IMPLICIT Stop-Request
		case 41:
			DECODE_TREE_SON("stop","","");
			break;
			// 				,    resume    
			// 				[42] IMPLICIT Resume-Request
		case 42:
			DECODE_TREE_SON("resume","","");
			break;
			// 				,    reset     
			// 				[43] IMPLICIT Reset-Request
		case 43:
			DECODE_TREE_SON("reset","","");
			break;
			// 				,    kill 
			// 				[44] IMPLICIT Kill-Request
		case 44:
			DECODE_TREE_SON("kill","","");
			break;
			// 				,    getProgramInvocationAttributes     
			// 				[45] IMPLICIT GetProgramInvocationAttributes-Request
		case 45:
			DECODE_TREE_SON("getProgramInvocationAttributes","","");
			break;
			// 				,    obtainFile     
			// 				[46] IMPLICIT ObtainFile-Request
		case 46:
			DECODE_TREE_SON("obtainFile","","");
			break;
			// 				,    defineEventCondition     
			// 				[47] IMPLICIT DefineEventCondition-Request
		case 47:
			DECODE_TREE_SON("defineEventCondition","","");
			break;
			// 				,    deleteEventCondition     
			// 				[48] DeleteEventCondition-Request
		case 48:
			DECODE_TREE_SON("deleteEventCondition","","");
			break;
			// 				,    getEventConditionAttributes
			// 				[49] GetEventConditionAttributes-Request
		case 49:
			DECODE_TREE_SON("getEventConditionAttributes","","");
			break;
			// 				,    reportEventConditionStatus
			// 				[50] ReportEventConditionStatus-Request
		case 50:
			DECODE_TREE_SON("reportEventConditionStatus","","");
			break;
			// 				,    alterEventConditionMonitoring
			// 				[51] IMPLICIT AlterEventConditionMonitoring-Request
		case 51:
			DECODE_TREE_SON("alterEventConditionMonitoring","","");
			break;
			// 				,    triggerEvent   
			// 				[52] IMPLICIT TriggerEvent-Request
		case 52:
			DECODE_TREE_SON("triggerEvent","","");
			break;
			// 				,    defineEventAction   
			// 				[53] IMPLICIT DefineEventAction-Request
		case 53:
			DECODE_TREE_SON("defineEventAction","","");
			break;
			// 				,    deleteEventAction   
			// 				[54] DeleteEventAction-Request
		case 54:
			DECODE_TREE_SON("deleteEventAction","","");
			break;
			// 				,    getEventActionAttributes 
			// 				[55] GetEventActionAttributes-Request
		case 55:
			DECODE_TREE_SON("getEventActionAttributes","","");
			break;
			// 				,    reportEventActionStatus  
			// 				[56] ReportEventActionStatus-Request
		case 56:
			DECODE_TREE_SON("reportEventActionStatus","","");
			break;
			// 				,    defineEventEnrollment    
			// 				[57] IMPLICIT DefineEventEnrollment-Request
		case 57:
			DECODE_TREE_SON("defineEventEnrollment","","");
			break;
			// 				,    deleteEventEnrollment    
			// 				[58] DeleteEventEnrollment-Request
		case 58:
			DECODE_TREE_SON("deleteEventEnrollment","","");
			break;
			// 				,    alterEventEnrollment     
			// 				[59] IMPLICIT AlterEventEnrollment-Request
		case 59:
			DECODE_TREE_SON("alterEventEnrollment","","");
			break;
			// 				,    reportEventEnrollmentStatus
			// 				[60] ReportEventEnrollmentStatus-Request
		case 60:
			DECODE_TREE_SON("reportEventEnrollmentStatus","","");
			break;
			// 				,    getEventEnrollmentAttributes
			// 				[61] IMPLICIT GetEventEnrollmentAttributes-Request
		case 61:
			DECODE_TREE_SON("getEventEnrollmentAttributes","","");
			break;
			// 				,    acknowledgeEventNotification
			// 				[62] IMPLICIT AcknowledgeEventNotification-Request
		case 62:
			DECODE_TREE_SON("acknowledgeEventNotification","","");
			break;
			// 				,    getAlarmSummary     
			// 				[63] IMPLICIT GetAlarmSummary-Request
		case 63:
			DECODE_TREE_SON("getAlarmSummary","","");
			break;
			// 				,    getAlarmEnrollmentSummary     
			// 				[64] IMPLICIT GetAlarmEnrollmentSummary-Request
		case 64:
			DECODE_TREE_SON("getAlarmEnrollmentSummary","","");
			break;
			// 				,    readJournal    
			// 				[65] IMPLICIT ReadJournal-Request
		case 65:
			DECODE_TREE_SON("readJournal","","");
			break;
			// 				,    writeJournal   
			// 				[66] IMPLICIT WriteJournal-Request
		case 66:
			DECODE_TREE_SON("writeJournal","","");
			break;
			// 				,    initializeJournal   
			// 				[67] IMPLICIT InitializeJournal-Request
		case 67:
			DECODE_TREE_SON("initializeJournal","","");
			break;
			// 				,    reportJournalStatus 
			// 				[68] ReportJournalStatus-Request
		case 68:
			DECODE_TREE_SON("reportJournalStatus","","");
			break;
			// 				,    createJournal  
			// 				[69] IMPLICIT CreateJournal-Request
		case 69:
			DECODE_TREE_SON("createJournal","","");
			break;
			// 				,    deleteJournal  
			// 				[70] IMPLICIT DeleteJournal-Request
		case 70:
			DECODE_TREE_SON("deleteJournal","","");
			break;
			// 				,    getCapabilityList   
			// 				[71] IMPLICIT GetCapabilityList-Request
		case 71:
			DECODE_TREE_SON("getCapabilityList","","");
			break;
			// 				-- choices [72] through [77] are reserved for use by services
			// 				-- defined in annex D   
			// 				,    fileOpen  
			// 				[72] IMPLICIT FileOpen-Request
		case 72:
			DECODE_TREE_SON("fileOpen","","");
			break;
			// 				,    fileRead  
			// 				[73] IMPLICIT FileRead-Request
		case 73:
			DECODE_TREE_SON("fileRead","","");
			break;
			// 				,    fileClose 
			// 				[74] IMPLICIT FileClose-Request    
		case 74:
			DECODE_TREE_SON("fileClose","","");
			break;
			// 				,    fileRename     
			// 				[75] IMPLICIT FileRename-Request
		case 75:
			DECODE_TREE_SON("fileRename","","");
			break;
			// 				,    fileDelete     
			// 				[76] IMPLICIT FileDelete-Request
		case 76:
			DECODE_TREE_SON("fileDelete","","");
			break;
			// 				,    fileDirectory  
			// 				[77] IMPLICIT FileDirectory-Request
		case 77:
			DECODE_TREE_SON("fileDirectory","","");
			break;
			// 				,...
			// 				,    additionalService   
			// 				[78] AdditionalService-Request
		case 78:
			DECODE_TREE_SON("additionalService","","");
			break;
			// 				-- choice [79] is reserved
			// 				,    getDataExchangeAttributes     
			// 				[80] GetDataExchangeAttributes-Request
		case 80:
			DECODE_TREE_SON("getDataExchangeAttributes","","");
			break;
			// 				-- Shall not appear in minor version 1
			// 				,    exchangeData   
			// 				[81] IMPLICIT ExchangeData-Request
		case 81:
			DECODE_TREE_SON("exchangeData","","");
			break;
			// 				-- Shall not appear in minor version 1
			// 				,    defineAccessControlList  
			// 				[82] IMPLICIT DefineAccessControlList-Request
		case 82:
			DECODE_TREE_SON("defineAccessControlList","","");
			break;
			// 				-- Shall not appear in minor version 1 or 2
			// 				,    getAccessControlListAttributes
			// 				[83] GetAccessControlListAttributes-Request
		case 83:
			DECODE_TREE_SON("getAccessControlListAttributes","","");
			break;
			// 				-- Shall not appear in minor version 1 or 2
			// 				,    reportAccessControlledObjects 
			// 				[84] IMPLICIT ReportAccessControlledObjects-Request
		case 84:
			DECODE_TREE_SON("reportAccessControlledObjects","","");
			break;
			// 				-- Shall not appear in minor version 1 or 2
			// 				,    deleteAccessControlList  
			// 				[85] IMPLICIT DeleteAccessControlList-Request
		case 85:
			DECODE_TREE_SON("deleteAccessControlList","","");
			break;
			// 				-- Shall not appear in minor version 1 or 2
			// 				,    changeAccessControl 
			// 				[86] IMPLICIT ChangeAccessControl-Request
		case 86:
			DECODE_TREE_SON("changeAccessControl","","");
			break;
			// 				-- Shall not appear in minor version 1 or 2
			// 				,...

		}
	}
	DECODE_CALLBACK_RET;
}

int SDecodeMms::Callback_MMS_confirmed_ResponsePDU(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	if(pPdu->tag.fmt == 0 && pPdu->tag.cls == 0)
	{
		if(pPdu->tag.id == 2)
		{
			//invokeID
			pThis->m_pLastSessionStatus->m_iMmsInvokeID = pThis->GetIntWithBufLen(pPdu->val,pPdu->len);
#ifdef SDECODE_OFFLINE
			DECODE_TREE_SON("invokeID",SString::toFormat("%d",pThis->m_pLastSessionStatus->m_iMmsInvokeID).data(),"");
#endif
		}
	}
	else
	{
		pThis->m_pLastSessionStatus->m_iMmsConfirmedServiceReponseId = pPdu->tag.id;
		//confirmedServiceResponse
#ifdef SDECODE_OFFLINE
		DECODE_TREE_SON("confirmedServiceResponse",pPdu->tag.id<=77?SString::toFormat("%s (%d)",mms_ConfirmedServiceResponse_vals[pPdu->tag.id].name,pPdu->tag.id).data():SString::toFormat("%d",pPdu->tag.id).data(),"");
#endif
		switch(pPdu->tag.id)
		{
			// 		ConfirmedServiceResponse ::= CHOICE {
			// 			status    
			// 				[0] IMPLICIT Status-Response
		case 0:
			DECODE_TREE_SON("status","","");
			break;
			// 				,    getNameList    
			// 				[1] IMPLICIT GetNameList-Response
		case 1:
			DECODE_TREE_SON("getNameList","","");
			break;
			// 				,    identify  
			// 				[2] IMPLICIT Identify-Response
		case 2:
			DECODE_TREE_SON("identify","","");
			break;
			// 				,    rename    
			// 				[3] IMPLICIT Rename-Response
		case 3:
			DECODE_TREE_SON("rename","","");
			break;
			// 				,    read 
			// 				[4] IMPLICIT Read-Response
		case 4:
			DECODE_TREE_SON("read","","");
			break;
			// 				,    write     
			// 				[5] IMPLICIT Write-Response
		case 5:
			DECODE_TREE_SON("write","","");
			break;
			// 				,    getVariableAccessAttributes   
			// 				[6] GetVariableAccessAttributes-Response
		case 6:
			DECODE_TREE_SON("getVariableAccessAttributes","","");
			break;
			// 				,    defineNamedVariable 
			// 				[7] IMPLICIT DefineNamedVariable-Response
		case 7:
			DECODE_TREE_SON("defineNamedVariable","","");
			break;
			// 				--   [8] is reserved for a service defined in Annex E  
			// 				,    defineScatteredAccess    
			// 				[8] IMPLICIT DefineScatteredAccess-Response
		case 8:
			DECODE_TREE_SON("defineScatteredAccess","","");
			break;
			// 				--   [9] is reserved for a service defined in Annex E
			// 				,    getScatteredAccessAttributes  
			// 				[9] GetScatteredAccessAttributes-Response
		case 9:
			DECODE_TREE_SON("getScatteredAccessAttributes","","");
			break;
			// 				,    deleteVariableAccess     
			// 				[10] IMPLICIT DeleteVariableAccess-Response
		case 10:
			DECODE_TREE_SON("deleteVariableAccess","","");
			break;
			// 				,    defineNamedVariableList  
			// 				[11] IMPLICIT DefineNamedVariableList-Response
		case 11:
			DECODE_TREE_SON("defineNamedVariableList","","");
			break;
			// 				,    getNamedVariableListAttributes     
			// 				[12] GetNamedVariableListAttributes-Response
		case 12:
			DECODE_TREE_SON("getNamedVariableListAttributes","","");
			break;
			// 				,    deleteNamedVariableList  
			// 				[13] IMPLICIT DeleteNamedVariableList-Response
		case 13:
			DECODE_TREE_SON("deleteNamedVariableList","","");
			break;
			// 				,    defineNamedType     
			// 				[14] IMPLICIT DefineNamedType-Response
		case 14:
			DECODE_TREE_SON("defineNamedType","","");
			break;
			// 				,    getNamedTypeAttributes   
			// 				[15] GetNamedTypeAttributes-Response
		case 15:
			DECODE_TREE_SON("getNamedTypeAttributes","","");
			break;
			// 				,    deleteNamedType     
			// 				[16] IMPLICIT DeleteNamedType-Response
		case 16:
			DECODE_TREE_SON("deleteNamedType","","");
			break;
			// 				,    input     
			// 				[17] IMPLICIT Input-Response
		case 17:
			DECODE_TREE_SON("input","","");
			break;
			// 				,    output    
			// 				[18] IMPLICIT Output-Response
		case 18:
			DECODE_TREE_SON("output","","");
			break;
			// 				,    takeControl    
			// 				[19] IMPLICIT TakeControl-Response
		case 19:
			DECODE_TREE_SON("takeControl","","");
			break;
			// 				,    relinquishControl   
			// 				[20] IMPLICIT RelinquishControl-Response
		case 20:
			DECODE_TREE_SON("relinquishControl","","");
			break;
			// 				,    defineSemaphore     
			// 				[21] IMPLICIT DefineSemaphore-Response
		case 21:
			DECODE_TREE_SON("defineSemaphore","","");
			break;
			// 				,    deleteSemaphore     
			// 				[22] DeleteSemaphore-Response
		case 22:
			DECODE_TREE_SON("deleteSemaphore","","");
			break;
			// 				,    reportSemaphoreStatus    
			// 				[23] ReportSemaphoreStatus-Response
		case 23:
			DECODE_TREE_SON("reportSemaphoreStatus","","");
			break;
			// 				,    reportPoolSemaphoreStatus     
			// 				[24] IMPLICIT ReportPoolSemaphoreStatus-Response
		case 24:
			DECODE_TREE_SON("reportPoolSemaphoreStatus","","");
			break;
			// 				,    reportSemaphoreEntryStatus    
			// 				[25] IMPLICIT ReportSemaphoreEntryStatus-Response
		case 25:
			DECODE_TREE_SON("reportSemaphoreEntryStatus","","");
			break;
			// 				,    initiateDownloadSequence 
			// 				[26] IMPLICIT InitiateDownloadSequence-Response,
		case 26:
			DECODE_TREE_SON("initiateDownloadSequence","","");
			break;
			// 				downloadSegment     
			// 				[27] IMPLICIT DownloadSegment-Response,
		case 27:
			DECODE_TREE_SON("downloadSegment","","");
			break;
			// 				terminateDownloadSequence     
			// 				[28] IMPLICIT TerminateDownloadSequence-Response
		case 28:
			DECODE_TREE_SON("terminateDownloadSequence","","");
			break;
			// 				,    initiateUploadSequence   
			// 				[29] IMPLICIT InitiateUploadSequence-Response,
		case 29:
			DECODE_TREE_SON("initiateUploadSequence","","");
			break;
			// 				uploadSegment  
			// 				[30] IMPLICIT UploadSegment-Response,
		case 30:
			DECODE_TREE_SON("uploadSegment","","");
			break;
			// 				terminateUploadSequence  
			// 				[31] IMPLICIT TerminateUploadSequence-Response
		case 31:
			DECODE_TREE_SON("terminateUploadSequence","","");
			break;
			// 				,    ResponseDomainDownload    
			// 				[32] IMPLICIT ResponseDomainDownload-Response
		case 32:
			DECODE_TREE_SON("ResponseDomainDownload","","");
			break;
			// 				,    ResponseDomainUpload 
			// 				[33] IMPLICIT ResponseDomainUpload-Response
		case 33:
			DECODE_TREE_SON("ResponseDomainUpload","","");
			break;
			// 				,    loadDomainContent   
			// 				[34] IMPLICIT LoadDomainContent-Response
		case 34:
			DECODE_TREE_SON("loadDomainContent","","");
			break;
			// 				,    storeDomainContent  
			// 				[35] IMPLICIT StoreDomainContent-Response
		case 35:
			DECODE_TREE_SON("storeDomainContent","","");
			break;
			// 				,    deleteDomain   
			// 				[36] IMPLICIT DeleteDomain-Response
		case 36:
			DECODE_TREE_SON("deleteDomain","","");
			break;
			// 				,    getDomainAttributes 
			// 				[37] IMPLICIT GetDomainAttributes-Response
		case 37:
			DECODE_TREE_SON("getDomainAttributes","","");
			break;
			// 				,    createProgramInvocation  
			// 				[38] IMPLICIT CreateProgramInvocation-Response
		case 38:
			DECODE_TREE_SON("createProgramInvocation","","");
			break;
			// 				,    deleteProgramInvocation  
			// 				[39] IMPLICIT DeleteProgramInvocation-Response
		case 39:
			DECODE_TREE_SON("deleteProgramInvocation","","");
			break;
			// 				,    start     
			// 				[40] IMPLICIT Start-Response
		case 40:
			DECODE_TREE_SON("start","","");
			break;
			// 				,    stop 
			// 				[41] IMPLICIT Stop-Response
		case 41:
			DECODE_TREE_SON("stop","","");
			break;
			// 				,    resume    
			// 				[42] IMPLICIT Resume-Response
		case 42:
			DECODE_TREE_SON("resume","","");
			break;
			// 				,    reset     
			// 				[43] IMPLICIT Reset-Response
		case 43:
			DECODE_TREE_SON("reset","","");
			break;
			// 				,    kill 
			// 				[44] IMPLICIT Kill-Response
		case 44:
			DECODE_TREE_SON("kill","","");
			break;
			// 				,    getProgramInvocationAttributes     
			// 				[45] IMPLICIT GetProgramInvocationAttributes-Response
		case 45:
			DECODE_TREE_SON("getProgramInvocationAttributes","","");
			break;
			// 				,    obtainFile     
			// 				[46] IMPLICIT ObtainFile-Response
		case 46:
			DECODE_TREE_SON("obtainFile","","");
			break;
			// 				,    defineEventCondition     
			// 				[47] IMPLICIT DefineEventCondition-Response
		case 47:
			DECODE_TREE_SON("defineEventCondition","","");
			break;
			// 				,    deleteEventCondition     
			// 				[48] DeleteEventCondition-Response
		case 48:
			DECODE_TREE_SON("deleteEventCondition","","");
			break;
			// 				,    getEventConditionAttributes
			// 				[49] GetEventConditionAttributes-Response
		case 49:
			DECODE_TREE_SON("getEventConditionAttributes","","");
			break;
			// 				,    reportEventConditionStatus
			// 				[50] ReportEventConditionStatus-Response
		case 50:
			DECODE_TREE_SON("reportEventConditionStatus","","");
			break;
			// 				,    alterEventConditionMonitoring
			// 				[51] IMPLICIT AlterEventConditionMonitoring-Response
		case 51:
			DECODE_TREE_SON("alterEventConditionMonitoring","","");
			break;
			// 				,    triggerEvent   
			// 				[52] IMPLICIT TriggerEvent-Response
		case 52:
			DECODE_TREE_SON("triggerEvent","","");
			break;
			// 				,    defineEventAction   
			// 				[53] IMPLICIT DefineEventAction-Response
		case 53:
			DECODE_TREE_SON("defineEventAction","","");
			break;
			// 				,    deleteEventAction   
			// 				[54] DeleteEventAction-Response
		case 54:
			DECODE_TREE_SON("deleteEventAction","","");
			break;
			// 				,    getEventActionAttributes 
			// 				[55] GetEventActionAttributes-Response
		case 55:
			DECODE_TREE_SON("getEventActionAttributes","","");
			break;
			// 				,    reportEventActionStatus  
			// 				[56] ReportEventActionStatus-Response
		case 56:
			DECODE_TREE_SON("reportEventActionStatus","","");
			break;
			// 				,    defineEventEnrollment    
			// 				[57] IMPLICIT DefineEventEnrollment-Response
		case 57:
			DECODE_TREE_SON("defineEventEnrollment","","");
			break;
			// 				,    deleteEventEnrollment    
			// 				[58] DeleteEventEnrollment-Response
		case 58:
			DECODE_TREE_SON("deleteEventEnrollment","","");
			break;
			// 				,    alterEventEnrollment     
			// 				[59] IMPLICIT AlterEventEnrollment-Response
		case 59:
			DECODE_TREE_SON("alterEventEnrollment","","");
			break;
			// 				,    reportEventEnrollmentStatus
			// 				[60] ReportEventEnrollmentStatus-Response
		case 60:
			DECODE_TREE_SON("reportEventEnrollmentStatus","","");
			break;
			// 				,    getEventEnrollmentAttributes
			// 				[61] IMPLICIT GetEventEnrollmentAttributes-Response
		case 61:
			DECODE_TREE_SON("getEventEnrollmentAttributes","","");
			break;
			// 				,    acknowledgeEventNotification
			// 				[62] IMPLICIT AcknowledgeEventNotification-Response
		case 62:
			DECODE_TREE_SON("acknowledgeEventNotification","","");
			break;
			// 				,    getAlarmSummary     
			// 				[63] IMPLICIT GetAlarmSummary-Response
		case 63:
			DECODE_TREE_SON("getAlarmSummary","","");
			break;
			// 				,    getAlarmEnrollmentSummary     
			// 				[64] IMPLICIT GetAlarmEnrollmentSummary-Response
		case 64:
			DECODE_TREE_SON("getAlarmEnrollmentSummary","","");
			break;
			// 				,    readJournal    
			// 				[65] IMPLICIT ReadJournal-Response
		case 65:
			DECODE_TREE_SON("readJournal","","");
			break;
			// 				,    writeJournal   
			// 				[66] IMPLICIT WriteJournal-Response
		case 66:
			DECODE_TREE_SON("writeJournal","","");
			break;
			// 				,    initializeJournal   
			// 				[67] IMPLICIT InitializeJournal-Response
		case 67:
			DECODE_TREE_SON("initializeJournal","","");
			break;
			// 				,    reportJournalStatus 
			// 				[68] ReportJournalStatus-Response
		case 68:
			DECODE_TREE_SON("reportJournalStatus","","");
			break;
			// 				,    createJournal  
			// 				[69] IMPLICIT CreateJournal-Response
		case 69:
			DECODE_TREE_SON("createJournal","","");
			break;
			// 				,    deleteJournal  
			// 				[70] IMPLICIT DeleteJournal-Response
		case 70:
			DECODE_TREE_SON("deleteJournal","","");
			break;
			// 				,    getCapabilityList   
			// 				[71] IMPLICIT GetCapabilityList-Response
		case 71:
			DECODE_TREE_SON("getCapabilityList","","");
			break;
			// 				-- choices [72] through [77] are reserved for use by services
			// 				-- defined in annex D   
			// 				,    fileOpen  
			// 				[72] IMPLICIT FileOpen-Response
		case 72:
			DECODE_TREE_SON("fileOpen","","");
			break;
			// 				,    fileRead  
			// 				[73] IMPLICIT FileRead-Response
		case 73:
			DECODE_TREE_SON("fileRead","","");
			break;
			// 				,    fileClose 
			// 				[74] IMPLICIT FileClose-Response    
		case 74:
			DECODE_TREE_SON("fileClose","","");
			break;
			// 				,    fileRename     
			// 				[75] IMPLICIT FileRename-Response
		case 75:
			DECODE_TREE_SON("fileRename","","");
			break;
			// 				,    fileDelete     
			// 				[76] IMPLICIT FileDelete-Response
		case 76:
			DECODE_TREE_SON("fileDelete","","");
			break;
			// 				,    fileDirectory  
			// 				[77] IMPLICIT FileDirectory-Response
		case 77:
			DECODE_TREE_SON("fileDirectory","","");
			break;
			// 				,...
			// 				,    additionalService   
			// 				[78] AdditionalService-Response
		case 78:
			DECODE_TREE_SON("additionalService","","");
			break;
			// 				-- choice [79] is reserved
			// 				,    getDataExchangeAttributes     
			// 				[80] GetDataExchangeAttributes-Response
		case 80:
			DECODE_TREE_SON("getDataExchangeAttributes","","");
			break;
			// 				-- Shall not appear in minor version 1
			// 				,    exchangeData   
			// 				[81] IMPLICIT ExchangeData-Response
		case 81:
			DECODE_TREE_SON("exchangeData","","");
			break;
			// 				-- Shall not appear in minor version 1
			// 				,    defineAccessControlList  
			// 				[82] IMPLICIT DefineAccessControlList-Response
		case 82:
			DECODE_TREE_SON("defineAccessControlList","","");
			break;
			// 				-- Shall not appear in minor version 1 or 2
			// 				,    getAccessControlListAttributes
			// 				[83] GetAccessControlListAttributes-Response
		case 83:
			DECODE_TREE_SON("getAccessControlListAttributes","","");
			break;
			// 				-- Shall not appear in minor version 1 or 2
			// 				,    reportAccessControlledObjects 
			// 				[84] IMPLICIT ReportAccessControlledObjects-Response
		case 84:
			DECODE_TREE_SON("reportAccessControlledObjects","","");
			break;
			// 				-- Shall not appear in minor version 1 or 2
			// 				,    deleteAccessControlList  
			// 				[85] IMPLICIT DeleteAccessControlList-Response
		case 85:
			DECODE_TREE_SON("deleteAccessControlList","","");
			break;
			// 				-- Shall not appear in minor version 1 or 2
			// 				,    changeAccessControl 
			// 				[86] IMPLICIT ChangeAccessControl-Response
		case 86:
			DECODE_TREE_SON("changeAccessControl","","");
			break;
			// 				-- Shall not appear in minor version 1 or 2
			// 				,...

		}
	}
	DECODE_CALLBACK_RET;
}

int SDecodeMms::Callback_MMS_confirmed_ErrorPDU(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;

	DECODE_CALLBACK_RET;
}

int SDecodeMms::Callback_MMS_unconfirmed_PDU(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;

	DECODE_CALLBACK_RET;
}

int SDecodeMms::Callback_MMS_rejectPDU(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;

	DECODE_CALLBACK_RET;
}

int SDecodeMms::Callback_MMS_cancel_RequestPDU(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;

	DECODE_CALLBACK_RET;
}

int SDecodeMms::Callback_MMS_cancel_ResponsePDU(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;

	DECODE_CALLBACK_RET;
}

int SDecodeMms::Callback_MMS_cancel_ErrorPDU(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;

	DECODE_CALLBACK_RET;
}

int SDecodeMms::Callback_MMS_initiate_RequestPDU(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;

// 	Initiate-RequestPDU ::= SEQUENCE {
// 		localDetailCalling  [0] IMPLICIT Integer32 OPTIONAL,
// 			proposedMaxServOutstandingCalling  [1] IMPLICIT Integer16,
// 			proposedMaxServOutstandingCalled   [2] IMPLICIT Integer16,
// 			proposedDataStructureNestingLevel  [3] IMPLICIT Integer8 OPTIONAL,

#ifdef SDECODE_OFFLINE
	switch(pPdu->tag.id)
	{
	case 0:
		DECODE_TREE_SON("localDetailCalling",SString::toFormat("%d",pThis->GetIntWithBufLen(pPdu->val,pPdu->len)).data(),"");
		break;
	case 1:
		DECODE_TREE_SON("proposedMaxServOutstandingCalling",SString::toFormat("%d",pThis->GetIntWithBufLen(pPdu->val,pPdu->len)).data(),"");
		break;
	case 2:
		DECODE_TREE_SON("proposedMaxServOutstandingCalled",SString::toFormat("%d",pThis->GetIntWithBufLen(pPdu->val,pPdu->len)).data(),"");
		break;
	case 3:
		DECODE_TREE_SON("proposedDataStructureNestingLevel",SString::toFormat("%d",pThis->GetIntWithBufLen(pPdu->val,pPdu->len)).data(),"");
		break;
	case 4:
		DECODE_TREE_SON("initRequestDetail","","");
		ret = pThis->DecodeAsn1Ber(pPdu->val,pPdu->len,Callback_MMS_initRequestDetail,pThis);
		break;
	}
#endif


	DECODE_CALLBACK_RET;
}

int SDecodeMms::Callback_MMS_initRequestDetail(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
#ifdef SDECODE_OFFLINE
	switch(pPdu->tag.id)
	{
	case 0:
		DECODE_TREE_SON("proposedVersionNumber",SString::toFormat("%d",pThis->GetIntWithBufLen(pPdu->val,pPdu->len)).data(),"");
		break;
	case 1:
		{
			int padding = 0;
			if(pPdu->len>2)
			{
				padding=pPdu->len-2;
				pPdu->len -= padding;
				pPdu->val += padding;
			}
			DECODE_TREE_SON("proposedParameterCBB",SString::toFormat("%X",pThis->GetIntWithBufLen(pPdu->val,pPdu->len)).data(),"");
			ret = Callback_MMS_ParameterSupportOptions(pThis,pPdu);
			if(padding > 0)
			{
				pPdu->len += padding;
			}
		}
		break;
	case 2:
		{
			int padding = 0;
			if(pPdu->len>11)
			{
				padding=pPdu->len-11;
				pPdu->len -= padding;
				pPdu->val += padding;
			}
			DECODE_TREE_SON("servicesSupportedCalling","","");
			ret = Callback_MMS_ServiceSupportOptions(pThis,pPdu);
			if(padding > 0)
			{
				pPdu->len += padding;
			}
		}
		break;
	}
	// initRequestDetail   [4] IMPLICIT SEQUENCE {
	// proposedVersionNumber    [0] IMPLICIT Integer16,
	// proposedParameterCBB     [1] IMPLICIT ParameterSupportOptions,
	// servicesSupportedCalling [2] IMPLICIT ServiceSupportOptions ,
	// ...
	// ,    additionalSupportedCalling    [3] IMPLICIT AdditionalSupportOptions
	// ,    additionalCbbSupportedCalling [4] IMPLICIT AdditionalCBBOptions,
	// privilegeClassIdentityCalling [5] IMPLICIT VisibleString
	// }
	// } 
#endif
	DECODE_CALLBACK_RET;
}

int SDecodeMms::Callback_MMS_ParameterSupportOptions(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	if(pPdu->len != 2)
	{
		return pThis->m_iFmtErrorNo;
	}
	BYTE b;
	b = pPdu->val[0];
	DECODE_TREE_POS(0,1,(b&0x80)?"1... ....":"0... ...."" =  str1",(b&0x80)?"True":"False","");
	DECODE_TREE_POS(0,1,(b&0x40)?".1.. ....":".0.. ...."" =  str2",(b&0x40)?"True":"False","");
	DECODE_TREE_POS(0,1,(b&0x20)?"..1. ....":"..0. ...."" =  vnam",(b&0x20)?"True":"False","");
	DECODE_TREE_POS(0,1,(b&0x10)?"...1 ....":"...0 ...."" =  valt",(b&0x10)?"True":"False","");
	DECODE_TREE_POS(0,1,(b&0x08)?".... 1...":".... 0..."" =  vadr",(b&0x08)?"True":"False","");
	DECODE_TREE_POS(0,1,(b&0x04)?".... .1..":".... .0.."" =  vsca",(b&0x04)?"True":"False","");
	DECODE_TREE_POS(0,1,(b&0x02)?".... ..1.":".... ..0."" =  tpy", (b&0x02)?"True":"False","");
	DECODE_TREE_POS(0,1,(b&0x01)?".... ...1":".... ...0"" =  vlis",(b&0x01)?"True":"False","");
	b = pPdu->val[1];
// 	DECODE_CALLBACK_TREE_POS(1,1,(b&0x80)?"1... ....":"0... ...."" =  res",(b&0x80)?"True":"False","");
// 	DECODE_CALLBACK_TREE_POS(1,1,(b&0x40)?".1.. ....":".0.. ...."" =  res",(b&0x40)?"True":"False","");
	DECODE_TREE_POS(1,1,(b&0x20)?"..1. ....":"..0. ...."" =  cei",(b&0x20)?"True":"False","");
	DECODE_TREE_POS(1,1,(b&0x10)?"...1 ....":"...0 ...."" =  aco",(b&0x10)?"True":"False","");
	DECODE_TREE_POS(1,1,(b&0x08)?".... 1...":".... 0..."" =  sem",(b&0x08)?"True":"False","");
	DECODE_TREE_POS(1,1,(b&0x04)?".... .1..":".... .0.."" =  csr",(b&0x04)?"True":"False","");
	DECODE_TREE_POS(1,1,(b&0x02)?".... ..1.":".... ..0."" =  csnc", (b&0x02)?"True":"False","");
	DECODE_TREE_POS(1,1,(b&0x01)?".... ...1":".... ...0"" =  csplc",(b&0x01)?"True":"False","");
	DECODE_CALLBACK_RET;
}

int SDecodeMms::Callback_MMS_ServiceSupportOptions(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	if(pPdu->len != 11)
	{
		return pThis->m_iFmtErrorNo;
	}
	BYTE b;
	b = pPdu->val[0];
	DECODE_TREE_POS(0,1,(b&0x80)?"1... ....":"0... ...."" =  status",				(b&0x80)?"True":"False","");
	DECODE_TREE_POS(0,1,(b&0x40)?".1.. ....":".0.. ...."" =  getNameList",		(b&0x40)?"True":"False","");
	DECODE_TREE_POS(0,1,(b&0x20)?"..1. ....":"..0. ...."" =  identify",			(b&0x20)?"True":"False","");
	DECODE_TREE_POS(0,1,(b&0x10)?"...1 ....":"...0 ...."" =  rename",				(b&0x10)?"True":"False","");
	DECODE_TREE_POS(0,1,(b&0x08)?".... 1...":".... 0..."" =  read",				(b&0x08)?"True":"False","");
	DECODE_TREE_POS(0,1,(b&0x04)?".... .1..":".... .0.."" =  write",				(b&0x04)?"True":"False","");
	DECODE_TREE_POS(0,1,(b&0x02)?".... ..1.":".... ..0."" =  getVariableAccessAttributes", (b&0x02)?"True":"False","");
	DECODE_TREE_POS(0,1,(b&0x01)?".... ...1":".... ...0"" =  defineNamedVariable",(b&0x01)?"True":"False","");

	b = pPdu->val[1];
	DECODE_TREE_POS(1,1,(b&0x80)?"1... ....":"0... ...."" =  defineScatteredAccess",				(b&0x80)?"True":"False","");
	DECODE_TREE_POS(1,1,(b&0x40)?".1.. ....":".0.. ...."" =  getScatteredAccessAttributes",		(b&0x40)?"True":"False","");
	DECODE_TREE_POS(1,1,(b&0x20)?"..1. ....":"..0. ...."" =  deleteVariableAccess",			(b&0x20)?"True":"False","");
	DECODE_TREE_POS(1,1,(b&0x10)?"...1 ....":"...0 ...."" =  defineNamedVariableList",				(b&0x10)?"True":"False","");
	DECODE_TREE_POS(1,1,(b&0x08)?".... 1...":".... 0..."" =  getNamedVariableListAttributes",				(b&0x08)?"True":"False","");
	DECODE_TREE_POS(1,1,(b&0x04)?".... .1..":".... .0.."" =  deleteNamedVariableList",				(b&0x04)?"True":"False","");
	DECODE_TREE_POS(1,1,(b&0x02)?".... ..1.":".... ..0."" =  defineNamedType", (b&0x02)?"True":"False","");
	DECODE_TREE_POS(1,1,(b&0x01)?".... ...1":".... ...0"" =  getNamedTypeAttributes",(b&0x01)?"True":"False","");

	b = pPdu->val[2];
	DECODE_TREE_POS(2,1,(b&0x80)?"1... ....":"0... ...."" =  deleteNamedType",				(b&0x80)?"True":"False","");
	DECODE_TREE_POS(2,1,(b&0x40)?".1.. ....":".0.. ...."" =  input",		(b&0x40)?"True":"False","");
	DECODE_TREE_POS(2,1,(b&0x20)?"..1. ....":"..0. ...."" =  output",			(b&0x20)?"True":"False","");
	DECODE_TREE_POS(2,1,(b&0x10)?"...1 ....":"...0 ...."" =  takeControl",				(b&0x10)?"True":"False","");
	DECODE_TREE_POS(2,1,(b&0x08)?".... 1...":".... 0..."" =  relinquishControl",				(b&0x08)?"True":"False","");
	DECODE_TREE_POS(2,1,(b&0x04)?".... .1..":".... .0.."" =  defineSemaphore",				(b&0x04)?"True":"False","");
	DECODE_TREE_POS(2,1,(b&0x02)?".... ..1.":".... ..0."" =  deleteSemaphore", (b&0x02)?"True":"False","");
	DECODE_TREE_POS(2,1,(b&0x01)?".... ...1":".... ...0"" =  reportSemaphoreStatus",(b&0x01)?"True":"False","");

	b = pPdu->val[3];
	DECODE_TREE_POS(3,1,(b&0x80)?"1... ....":"0... ...."" =  reportPoolSemaphoreStatus",				(b&0x80)?"True":"False","");
	DECODE_TREE_POS(3,1,(b&0x40)?".1.. ....":".0.. ...."" =  reportSemaphoreEntryStatus",		(b&0x40)?"True":"False","");
	DECODE_TREE_POS(3,1,(b&0x20)?"..1. ....":"..0. ...."" =  initiateDownloadSequence",			(b&0x20)?"True":"False","");
	DECODE_TREE_POS(3,1,(b&0x10)?"...1 ....":"...0 ...."" =  downloadSegment",				(b&0x10)?"True":"False","");
	DECODE_TREE_POS(3,1,(b&0x08)?".... 1...":".... 0..."" =  terminateDownloadSequence",				(b&0x08)?"True":"False","");
	DECODE_TREE_POS(3,1,(b&0x04)?".... .1..":".... .0.."" =  initiateUploadSequence",				(b&0x04)?"True":"False","");
	DECODE_TREE_POS(3,1,(b&0x02)?".... ..1.":".... ..0."" =  uploadSegment", (b&0x02)?"True":"False","");
	DECODE_TREE_POS(3,1,(b&0x01)?".... ...1":".... ...0"" =  terminateUploadSequence",(b&0x01)?"True":"False","");

	b = pPdu->val[4];
	DECODE_TREE_POS(4,1,(b&0x80)?"1... ....":"0... ...."" =  requestDomainDownload",				(b&0x80)?"True":"False","");
	DECODE_TREE_POS(4,1,(b&0x40)?".1.. ....":".0.. ...."" =  requestDomainUpload",		(b&0x40)?"True":"False","");
	DECODE_TREE_POS(4,1,(b&0x20)?"..1. ....":"..0. ...."" =  loadDomainContent",			(b&0x20)?"True":"False","");
	DECODE_TREE_POS(4,1,(b&0x10)?"...1 ....":"...0 ...."" =  storeDomainContent",				(b&0x10)?"True":"False","");
	DECODE_TREE_POS(4,1,(b&0x08)?".... 1...":".... 0..."" =  deleteDomain",				(b&0x08)?"True":"False","");
	DECODE_TREE_POS(4,1,(b&0x04)?".... .1..":".... .0.."" =  getDomainAttributes",				(b&0x04)?"True":"False","");
	DECODE_TREE_POS(4,1,(b&0x02)?".... ..1.":".... ..0."" =  createProgramInvocation", (b&0x02)?"True":"False","");
	DECODE_TREE_POS(4,1,(b&0x01)?".... ...1":".... ...0"" =  deleteProgramInvocation",(b&0x01)?"True":"False","");

	b = pPdu->val[5];
	DECODE_TREE_POS(5,1,(b&0x80)?"1... ....":"0... ...."" =  start",				(b&0x80)?"True":"False","");
	DECODE_TREE_POS(5,1,(b&0x40)?".1.. ....":".0.. ...."" =  stop",		(b&0x40)?"True":"False","");
	DECODE_TREE_POS(5,1,(b&0x20)?"..1. ....":"..0. ...."" =  resume",			(b&0x20)?"True":"False","");
	DECODE_TREE_POS(5,1,(b&0x10)?"...1 ....":"...0 ...."" =  reset",				(b&0x10)?"True":"False","");
	DECODE_TREE_POS(5,1,(b&0x08)?".... 1...":".... 0..."" =  kill",				(b&0x08)?"True":"False","");
	DECODE_TREE_POS(5,1,(b&0x04)?".... .1..":".... .0.."" =  getProgramInvocationAttributes",				(b&0x04)?"True":"False","");
	DECODE_TREE_POS(5,1,(b&0x02)?".... ..1.":".... ..0."" =  obtainFile", (b&0x02)?"True":"False","");
	DECODE_TREE_POS(5,1,(b&0x01)?".... ...1":".... ...0"" =  defineEventCondition",(b&0x01)?"True":"False","");

	b = pPdu->val[6];
	DECODE_TREE_POS(6,1,(b&0x80)?"1... ....":"0... ...."" =  deleteEventCondition",				(b&0x80)?"True":"False","");
	DECODE_TREE_POS(6,1,(b&0x40)?".1.. ....":".0.. ...."" =  getEventConditionAttributes",		(b&0x40)?"True":"False","");
	DECODE_TREE_POS(6,1,(b&0x20)?"..1. ....":"..0. ...."" =  reportEventConditionStatus",			(b&0x20)?"True":"False","");
	DECODE_TREE_POS(6,1,(b&0x10)?"...1 ....":"...0 ...."" =  alterEventConditionMonitoring",				(b&0x10)?"True":"False","");
	DECODE_TREE_POS(6,1,(b&0x08)?".... 1...":".... 0..."" =  triggerEvent",				(b&0x08)?"True":"False","");
	DECODE_TREE_POS(6,1,(b&0x04)?".... .1..":".... .0.."" =  defineEventAction",				(b&0x04)?"True":"False","");
	DECODE_TREE_POS(6,1,(b&0x02)?".... ..1.":".... ..0."" =  deleteEventAction", (b&0x02)?"True":"False","");
	DECODE_TREE_POS(6,1,(b&0x01)?".... ...1":".... ...0"" =  getEventActionAttributes",(b&0x01)?"True":"False","");

	b = pPdu->val[7];
	DECODE_TREE_POS(7,1,(b&0x80)?"1... ....":"0... ...."" =  reportEventActionStatus",				(b&0x80)?"True":"False","");
	DECODE_TREE_POS(7,1,(b&0x40)?".1.. ....":".0.. ...."" =  defineEventEnrollment",		(b&0x40)?"True":"False","");
	DECODE_TREE_POS(7,1,(b&0x20)?"..1. ....":"..0. ...."" =  deleteEventEnrollment",			(b&0x20)?"True":"False","");
	DECODE_TREE_POS(7,1,(b&0x10)?"...1 ....":"...0 ...."" =  alterEventEnrollment",				(b&0x10)?"True":"False","");
	DECODE_TREE_POS(7,1,(b&0x08)?".... 1...":".... 0..."" =  reportEventEnrollmentStatus",				(b&0x08)?"True":"False","");
	DECODE_TREE_POS(7,1,(b&0x04)?".... .1..":".... .0.."" =  getEventEnrollmentAttributes",				(b&0x04)?"True":"False","");
	DECODE_TREE_POS(7,1,(b&0x02)?".... ..1.":".... ..0."" =  acknowledgeEventNotification", (b&0x02)?"True":"False","");
	DECODE_TREE_POS(7,1,(b&0x01)?".... ...1":".... ...0"" =  getAlarmSummary",(b&0x01)?"True":"False","");

	b = pPdu->val[8];
	DECODE_TREE_POS(8,1,(b&0x80)?"1... ....":"0... ...."" =  getAlarmEnrollmentSummary",				(b&0x80)?"True":"False","");
	DECODE_TREE_POS(8,1,(b&0x40)?".1.. ....":".0.. ...."" =  readJournal",		(b&0x40)?"True":"False","");
	DECODE_TREE_POS(8,1,(b&0x20)?"..1. ....":"..0. ...."" =  writeJournal",			(b&0x20)?"True":"False","");
	DECODE_TREE_POS(8,1,(b&0x10)?"...1 ....":"...0 ...."" =  initializeJournal",				(b&0x10)?"True":"False","");
	DECODE_TREE_POS(8,1,(b&0x08)?".... 1...":".... 0..."" =  reportJournalStatus",				(b&0x08)?"True":"False","");
	DECODE_TREE_POS(8,1,(b&0x04)?".... .1..":".... .0.."" =  createJournal",				(b&0x04)?"True":"False","");
	DECODE_TREE_POS(8,1,(b&0x02)?".... ..1.":".... ..0."" =  deleteJournal", (b&0x02)?"True":"False","");
	DECODE_TREE_POS(8,1,(b&0x01)?".... ...1":".... ...0"" =  getCapabilityList",(b&0x01)?"True":"False","");

	b = pPdu->val[9];
	DECODE_TREE_POS(9,1,(b&0x80)?"1... ....":"0... ...."" =  fileOpen",				(b&0x80)?"True":"False","");
	DECODE_TREE_POS(9,1,(b&0x40)?".1.. ....":".0.. ...."" =  fileRead",		(b&0x40)?"True":"False","");
	DECODE_TREE_POS(9,1,(b&0x20)?"..1. ....":"..0. ...."" =  fileClose",			(b&0x20)?"True":"False","");
	DECODE_TREE_POS(9,1,(b&0x10)?"...1 ....":"...0 ...."" =  fileRename",				(b&0x10)?"True":"False","");
	DECODE_TREE_POS(9,1,(b&0x08)?".... 1...":".... 0..."" =  fileDelete",				(b&0x08)?"True":"False","");
	DECODE_TREE_POS(9,1,(b&0x04)?".... .1..":".... .0.."" =  fileDirectory",				(b&0x04)?"True":"False","");
	DECODE_TREE_POS(9,1,(b&0x02)?".... ..1.":".... ..0."" =  unsolicitedStatus", (b&0x02)?"True":"False","");
	DECODE_TREE_POS(9,1,(b&0x01)?".... ...1":".... ...0"" =  informationReport",(b&0x01)?"True":"False","");

	b = pPdu->val[10];
	DECODE_TREE_POS(10,1,(b&0x80)?"1... ....":"0... ...."" =  eventNotification",				(b&0x80)?"True":"False","");
	DECODE_TREE_POS(10,1,(b&0x40)?".1.. ....":".0.. ...."" =  attachToEventCondition",		(b&0x40)?"True":"False","");
	DECODE_TREE_POS(10,1,(b&0x20)?"..1. ....":"..0. ...."" =  attachToSemaphore",			(b&0x20)?"True":"False","");
	DECODE_TREE_POS(10,1,(b&0x10)?"...1 ....":"...0 ...."" =  conclude",				(b&0x10)?"True":"False","");
	DECODE_TREE_POS(10,1,(b&0x08)?".... 1...":".... 0..."" =  cancel",				(b&0x08)?"True":"False","");
	DECODE_TREE_POS(10,1,(b&0x04)?".... .1..":".... .0.."" =  getDataExchangeAttributes",				(b&0x04)?"True":"False","");
	DECODE_TREE_POS(10,1,(b&0x02)?".... ..1.":".... ..0."" =  exchangeData", (b&0x02)?"True":"False","");
	DECODE_TREE_POS(10,1,(b&0x01)?".... ...1":".... ...0"" =  defineAccessControlList",(b&0x01)?"True":"False","");

	DECODE_CALLBACK_RET;
}

int SDecodeMms::Callback_MMS_initiate_ResponsePDU(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;

	DECODE_CALLBACK_RET;
}

int SDecodeMms::Callback_MMS_initiate_ErrorPDU(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;

	DECODE_CALLBACK_RET;
}

int SDecodeMms::Callback_MMS_conclude_RequestPDU(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;

	DECODE_CALLBACK_RET;
}

int SDecodeMms::Callback_MMS_conclude_ResponsePDU(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;

	DECODE_CALLBACK_RET;
}

int SDecodeMms::Callback_MMS_conclude_ErrorPDU(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;

	DECODE_CALLBACK_RET;
}

int SDecodeMms::Callback_MMS_confirmedServiceRequest(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;

	DECODE_CALLBACK_RET;
}

int SDecodeMms::Callback_MMS_GetNameList_Request(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
	
// 	  GetNameList-Request ::= SEQUENCE {
	switch(pPdu->tag.id)
	{
//       objectClass    [0] ObjectClass,
	case 0:
		DECODE_TREE("ObjectClass","","");
		DECODE_CHILD(Callback_MMS_ObjectClass);
		break;
//       objectScope    [1] CHOICE {
//       vmdSpecific    [0] IMPLICIT NULL,
//       domainSpecific [1] IMPLICIT Identifier,
//       aaSpecific     [2] IMPLICIT NULL },
	case 1:
		DECODE_TREE("objectScope","","");
		break;
//       continueAfter  [2] IMPLICIT Identifier OPTIONAL } 
	case 2:
		DECODE_TREE("continueAfter","","");
		break;
	}
	DECODE_CALLBACK_RET;
}

int SDecodeMms::Callback_MMS_ObjectClass(void* cbParam, stuAsn1Pdu *pPdu)
{
	DECODE_CALLBACK_INTO;
// 	  ObjectClass ::=    CHOICE {
	switch(pPdu->tag.id)
	{
	case 0:
		DECODE_TREE("namedVariable","","");
		break;
	case 1:
		DECODE_TREE("scatteredAccess","","");
		break;
	case 2:
		DECODE_TREE("namedVariableList","","");
		break;
	case 3:
		DECODE_TREE("namedType","","");
		break;
	case 4:
		DECODE_TREE("semaphore","","");
		break;
	case 5:
		DECODE_TREE("eventCondition","","");
		break;
	case 6:
		DECODE_TREE("eventAction","","");
		break;
	case 7:
		DECODE_TREE("eventEnrollment","","");
		break;
	case 8:
		DECODE_TREE("journal","","");
		break;
	case 9:
		DECODE_TREE("domain","","");
		break;
	case 10:
		DECODE_TREE("programInvocation","","");
		break;
	case 11:
		DECODE_TREE("operatorStation","","");
		break;
	case 12:
		DECODE_TREE("dataExchange","","");
		break;
	case 13:
		DECODE_TREE("accessControlList","","");
		break;
	}
//       basicObjectClass    [0] IMPLICIT INTEGER {
//       namedVariable  (0)
//       -- value 1 is reserved for definition in Annex E
//  ,    scatteredAccess     (1)
//  ,    namedVariableList   (2)
//  ,    namedType (3)
//  ,    semaphore (4),
//       eventCondition (5),
//       eventAction    (6),
//       eventEnrollment     (7),
//       journal   (8),
//       domain    (9),
//       programInvocation   (10),
//       operatorStation     (11),
//       dataExchange   (12),
//       -- Shall not appear in minor version 1
//       accessControlList   (13)
//     -- Shall not appear in minor version 1 or 2
//       } (0..13),
//       ...
//  ,    csObjectClass  [1] IMPLICIT INTEGER {
//       eventConditionList  (0),
//       unitControl    (1) } (0..1)
//       } 

	DECODE_CALLBACK_RET;
}
#endif 




int get_ber_identifier(unsigned char *buf,int offset,char *cls,bool *pc,int *tag)
{
	unsigned char c;
	unsigned short ui16;
	unsigned char *p;

	p = buf+offset;
	c = *(p ++);
	if(cls != NULL)
		*cls = c>>6;
	if(pc != NULL)
		*pc = ((c>>5)&1);

	if ((c &= 0x1F) == 0x1F)
	{
		if ((c = *(p++)) & 0x80)
		{
			if (*p & 0x80)
			{
				return -1;
			}
			if(tag != NULL)
			{
				ui16 = (unsigned short) (c & 0x7F) << 7;
				ui16 |= (*(p++) & 0x7F);
				*tag = ui16;
			}
			offset += 3;
		}
		else
		{
			if(tag != NULL)
				*tag = c;
			offset = 2;
		}
	}
	else
	{
		if(tag != NULL)
			*tag = c;
		offset ++;
	}
	return offset;
}

int get_ber_length(unsigned char *buf, int offset, unsigned int *len, bool *idx)
{
	int head_len = 0;
	unsigned char c;
	unsigned char *p;

	p = buf+offset;

	if ((c = *(p ++)) & 0x80)
	{
		if (c &= 0x7F)
		{
			offset += c+1;
			if(len != NULL)
			{
				register int v=0;
				if(c>4)
					return -1;
				memcpy(((BYTE*)&v)+4-c,p,c);
				*len = ntohl(v);
			}
		}
		else
			return -1;
	}
	else
	{
		if(len != NULL)
			*len = (int)c;
		offset ++;
	}
	if(idx != NULL)
		*idx = false;
	return offset;
}
int dissect_ber_identifier(stuAsn1Pdu *pPdu, void *cbParam, BYTE *pBuf, int offset, char *mClass, bool *pc, int *tag)
{
	int old_offset = offset;
	char tmp_class;
	bool tmp_pc;
	int tmp_tag;
	SDecodeMms *pThis = (SDecodeMms *)cbParam;
	offset = get_ber_identifier(pBuf, offset, &tmp_class, &tmp_pc, &tmp_tag);
	if(offset < 0)
	{
		NEW_MALFORMED_ID;
		return -1;
	}
	pPdu->tag.cls = tmp_class;
	pPdu->tag.fmt = tmp_pc;
	pPdu->tag.id = tmp_tag;

	if(mClass)
		*mClass = tmp_class;
	if(pc)
		*pc = tmp_pc;
	if(tag)
		*tag = tmp_tag;

	return offset;
}

int	dissect_ber_length(stuAsn1Pdu *pPdu, void *cbParam, BYTE *pBuf, int offset, unsigned int *length, bool *ind)
{
	int old_offset = offset;
	unsigned int tmp_length;
	bool tmp_ind;
	SDecodeMms *pThis = (SDecodeMms *)cbParam;
	offset = get_ber_length(pBuf, offset, &tmp_length, &tmp_ind);
	if(offset < 0)
	{
		NEW_MALFORMED_LEN;
		return -1;
	}
	pPdu->len = tmp_length;
	pPdu->val = pBuf + offset;
	/*
	if(show_internal_ber_fields)
	{
		if(tmp_ind){
			proto_tree_add_text(tree, tvb, old_offset, 1, "Length: Indefinite length %d", tmp_length);
		} else {
			proto_tree_add_uint(tree, hf_ber_length, tvb, old_offset, offset - old_offset, tmp_length);
		}
	}*/
	if(length)
		*length = tmp_length;
	if(ind)
		*ind = tmp_ind;

	return offset;
}

#define DECODE_HEAD \
/*id*/ \
c = *(p ++); \
pdu.tag.cls = c>>6; \
pdu.tag.fmt = (c>>5)&1; \
if ((c &= 0x1F) == 0x1F) \
{ \
	if ((c = *(p++)) & 0x80) \
	{ \
		if (*p & 0x80) \
			return pThis->m_iFmtErrorNo; \
		ST_UINT16 ui16 = (ST_UINT16) (c & 0x7F) << 7; \
		ui16 |= (*(p++) & 0x7F); \
		pdu.tag.id =(WORD) ui16; \
	} \
	else \
	{ \
		pdu.tag.id = (WORD) c; \
	} \
} \
else \
{ \
	pdu.tag.id = (WORD) c; \
} \
	 \
/*length*/ \
if ((c = *(p ++)) & 0x80) \
{ \
	if (c &= 0x7F) \
	{ \
		if(c>4) \
		{ \
			return pThis->m_iFmtErrorNo; \
		} \
		register int v=0; \
		memcpy(((BYTE*)&v)+4-c,p,c); \
		p += c; \
		pdu.len = ntohl(v); \
	} \
	else \
	{ \
		return pThis->m_iFmtErrorNo; \
	} \
} \
else \
{ \
	pdu.len = (ST_INT)c; \
} \
pdu.val = p; 

int dissect_ber_sequence(void *cbParam,stuAsn1Pdu *pPdu,const ber_sequence_t *seq)
{
	DECODE_CALLBACK_INTO;
	ST_UCHAR c;
	stuAsn1Pdu pdu,*pPdu0=pPdu;
	BYTE *pInBuf = pPdu->val;
	int iLen = pPdu->len;
	BYTE *p = pInBuf;
	pPdu = &pdu;

	char classx;
	bool pcx, ind = false, ind_field, imp_tag=false;
	int tagx;
	unsigned int lenx;
// 	proto_tree *tree = parent_tree;
// 	proto_item *item = NULL;
// 	proto_item *cause;
	int end_offset = 0;
	int s_offset;
	int hoffset;
	int length_remaining;
//	tvbuff_t *next_tvb;
	int offset = 0;
	bool implicit_tag = pPdu0->implicit_tag;
	s_offset = offset;
	hoffset = offset;
	BYTE *tvb = pPdu0->val;
	BYTE *next_tvb = 0;
	int next_tvb_len = 0;
	if(!implicit_tag)
	{
		offset = get_ber_identifier(tvb, offset, NULL, NULL, NULL);
		offset = get_ber_length(tvb, offset, &lenx, NULL);
	}
	else
	{
		/* was implicit tag so just use the length of the tvb */
		lenx=pPdu0->len;
		end_offset=offset+lenx;
	}
	offset = hoffset;

	if(!implicit_tag)
	{
		/* first we must read the sequence header */
		offset = dissect_ber_identifier(&pdu,pThis,tvb,offset, &classx, &pcx, &tagx);
		offset = dissect_ber_length(&pdu,pThis,tvb,offset, &lenx, &ind);
		if(ind)
		{
		/*  Fixed the length is correctly returned from dissect ber_length
		  end_offset = tvb_length(tvb);*/
		  end_offset = offset + lenx -2;
		} 
		else
		{
		  end_offset = offset + lenx;
		}

		/* sanity check: we only handle Constructed Universal Sequences */
		if((classx!=BER_CLASS_APP)&&(classx!=BER_CLASS_PRI))
		if((!pcx)
		||(!implicit_tag&&((classx!=BER_CLASS_UNI)
					||(tagx!=BER_UNI_TAG_SEQUENCE)))) {
			ret = pThis->m_iFmtErrorNo;
			DECODE_CALLBACK_RET;
		}
	}
	/* loop over all entries until we reach the end of the sequence */
	while (offset >=0 && offset < end_offset)
	{
		char mClass;
		bool pc;
		int tag;
		unsigned int len;
		int eoffset, count;

		/*if(ind){  this sequence was of indefinite length, if this is implicit indefinite impossible maybe
					but ber dissector uses this to eat the tag length then pass into here... EOC still on there...*/
			if(offset + 2 >= (int)pPdu0->len)// (tvb_get_guint8(tvb, offset)==0)&&(tvb_get_guint8(tvb, offset+1)==0))
			{
				DECODE_TREE("0 items","","");
				DECODE_CALLBACK_RET;
			}
		/*}*/
		hoffset = offset;
		/* read header and len for next field */
		offset = get_ber_identifier(tvb, offset, &mClass, &pc, &tag);
		offset = get_ber_length(tvb, offset, &len, &ind_field);
		eoffset = offset + len;
        /* Make sure we move forward */
		if (eoffset <= hoffset)
		{
			ret = pThis->m_iFmtErrorNo;
			DECODE_CALLBACK_RET;
		}

ber_sequence_try_again:
		/* have we run out of known entries in the sequence ?*/
		if(!seq->func) 
		{
			/* it was not,  move to the next one and try again */
			offset = dissect_ber_identifier(&pdu,pThis,tvb, hoffset, NULL, NULL, NULL);
			offset = dissect_ber_length(&pdu,pThis,tvb, offset, NULL, NULL);
			DECODE_TREE("BER Error: This field lies beyond the end of the known sequence definition.","","");
			offset = eoffset;
			continue;
		}

		/* Verify that this one is the one we want.
		 * Skip check completely if mClass==ANY
		 * of if NOCHKTAG is set
		 */
/* XXX Bug in asn2eth,
 * for   scope            [7]  Scope OPTIONAL,
 * it generates
 *   { BER_CLASS_CON, 7, BER_FLAGS_OPTIONAL|BER_FLAGS_NOTCHKTAG, dissect_scope },
 * and there should not be a NOTCHKTAG here
 */
		if( ((seq->ber_class==BER_CLASS_CON)||(seq->ber_class==BER_CLASS_APP)||(seq->ber_class==BER_CLASS_PRI)) && (!(seq->flags&BER_FLAGS_NOOWNTAG)) )
		{
		  if( (seq->ber_class!=BER_CLASS_ANY)
		  &&  (seq->tag!=-1)
		  &&( (seq->ber_class!=mClass)
		    ||(seq->tag!=tag) ) ){
			/* it was not,  move to the next one and try again */
			if(seq->flags&BER_FLAGS_OPTIONAL){
				/* well this one was optional so just skip to the next one and try again. */
				seq++;
				goto ber_sequence_try_again;
			}
			offset = dissect_ber_identifier(&pdu,pThis,tvb, hoffset, NULL, NULL, NULL);
			offset = dissect_ber_length(&pdu,pThis,tvb, offset, NULL, NULL);
			if( seq->ber_class == BER_CLASS_UNI)
			{ 
				DECODE_TREE("BER Error: Wrong field in SEQUENCE","","");
			}else{
				DECODE_TREE("BER Error: Wrong field in SEQUENCE","","");
			}
			seq++;
			offset=eoffset;
			continue;
		  }
	        } else if(!(seq->flags & BER_FLAGS_NOTCHKTAG)) {
		  if( (seq->ber_class!=BER_CLASS_ANY)
		  &&  (seq->tag!=-1)
		  &&( (seq->ber_class!=mClass)
		    ||(seq->tag!=tag) ) ){
			/* it was not,  move to the next one and try again */
			if(seq->flags&BER_FLAGS_OPTIONAL){
				/* well this one was optional so just skip to the next one and try again. */
				seq++;
				goto ber_sequence_try_again;
			}

			offset = dissect_ber_identifier(&pdu,pThis, tvb, hoffset, NULL, NULL, NULL);
			offset = dissect_ber_length(&pdu,pThis, tvb, offset, NULL, NULL);
			if( seq->ber_class == BER_CLASS_UNI)
			{
				DECODE_TREE("BER Error: Wrong field in SEQUENCE","","");
			}else{
				DECODE_TREE("BER Error: Wrong field in SEQUENCE","","");
			}
			seq++;
			offset=eoffset;
			continue;
		  }
		}

		if(!(seq->flags & BER_FLAGS_NOOWNTAG) )
		{
			/* dissect header and len for field */
			if(ind_field && (len == 2)){
				/* This is a Zero length field */
				next_tvb = tvb+offset;// tvb_new_subset(tvb, offset, len, len);
				next_tvb_len = len;
				hoffset = eoffset;
			}else{
				hoffset = dissect_ber_identifier(&pdu,pThis, tvb, hoffset, NULL, NULL, NULL);
				hoffset = dissect_ber_length(&pdu,pThis, tvb, hoffset, NULL, NULL);
				length_remaining=pPdu0->len - hoffset;//tvb_length_remaining(tvb, hoffset);
				if (length_remaining>eoffset-hoffset-(2*ind_field))
					length_remaining=eoffset-hoffset-(2*ind_field);
				if(length_remaining > 0)
					next_tvb = tvb+hoffset;
				else
				{
					next_tvb = tvb+pPdu0->len+length_remaining;
					length_remaining = -length_remaining;
				}
				next_tvb_len = length_remaining;
				//next_tvb = tvb_new_subset(tvb, hoffset, length_remaining, eoffset-hoffset-(2*ind_field));
			}
		}
		else {
			length_remaining=pPdu0->len - hoffset;//(tvb, hoffset);
			if (length_remaining>eoffset-hoffset)
				length_remaining=eoffset-hoffset;
			if(length_remaining > 0)
				next_tvb = tvb+hoffset;
			else
			{
				next_tvb = tvb+pPdu0->len+length_remaining;
				length_remaining = -length_remaining;
			}
			next_tvb_len = length_remaining;
			//next_tvb = tvb_new_subset(tvb, hoffset, length_remaining, eoffset-hoffset);
		}

		/* call the dissector for this field */
		/*if 	((eoffset-hoffset)>length_remaining) {*/
			/* If the field is indefinite (i.e. we dont know the
			 * length) of if the tvb is short, then just
			 * give it all of the tvb and hope for the best.
			 */
			/*next_tvb = tvb_new_subset_remaining(tvb, hoffset);*/
		/*} else {*/

		/*}*/

		if (next_tvb == NULL) 
		{
			/* Assume that we have a malformed packet. */
			ret =  pThis->m_iFmtErrorNo;
			DECODE_CALLBACK_RET;
		}
		imp_tag=false;
		if (seq->flags & BER_FLAGS_IMPLTAG)
		{
			imp_tag = true;
		}

		pdu.implicit_tag = imp_tag;
		pdu.val = next_tvb;
		pdu.len = next_tvb_len;
		DECODE_TREE(seq->strptr,"","");
		ret=seq->func(pThis, &pdu);

		/* if it was optional and no bytes were eaten and it was */
		/* supposed to (len<>0), just try again. */
		if((len!=0)&&ret != 0/*&&(count==0)*/&&(seq->flags&BER_FLAGS_OPTIONAL))
		{
			seq++;
			goto ber_sequence_try_again;
		/* move the offset to the beginning of the next sequenced item */
		}
		offset = eoffset;
		if(!(seq->flags & BER_FLAGS_NOOWNTAG) )
		{
			/* if we stripped the tag and length we should also strip the EOC is ind_len 
			 * Unless its a zero length field (len = 2)
			 */
			if((ind_field == 1)&&(len>2))
			{
				/* skip over EOC */
				DECODE_ADDVALUE("SEQ FIELD EOC");
			}
		}
		seq++;
	}
	if (offset < 0) 
	{
		ret = 0;
		DECODE_CALLBACK_RET;
	}
	/* if we didnt end up at exactly offset, then we ate too many bytes */
	if(offset != end_offset)
	{
		DECODE_TREE("BER Error: too many bytes in Sequence","","");
	}
	if(ind)
	{
		/*  need to eat this EOC
		end_offset = tvb_length(tvb);*/
		end_offset += 2;
		DECODE_ADDVALUE("SEQ EOC");
	}
	return end_offset;


	DECODE_CALLBACK_RET;
}

int dissect_ber_choice(void *cbParam,stuAsn1Pdu *pPdu,const ber_choice_t *choice)
{
	DECODE_CALLBACK_INTO;
	ST_UCHAR c;
	stuAsn1Pdu pdu,*pPdu0=pPdu;
	BYTE *pInBuf = pPdu->val;
	int iLen = pPdu->len;
	BYTE *p = pInBuf;
	pPdu = &pdu;

	int offset = 0;
	bool implicit_tag = pPdu0->implicit_tag;
	char       ber_class;
    bool    pc, ind, imp_tag = false;
    int      tag;
    unsigned int     len;
//     proto_tree *tree = parent_tree;
//     proto_item *item = NULL;
    int         end_offset, start_offset, count;
    int         hoffset = offset;
    int        length, length_remaining;
//    tvbuff_t   *next_tvb;
    bool    first_pass;
//    header_field_info  *hfinfo;
    const ber_choice_t *ch;
	int *branch_taken = NULL;
	BYTE *tvb = pPdu0->val;
	BYTE *next_tvb = 0;
	int next_tvb_len = 0;

    start_offset = offset;

    if (branch_taken) {
        *branch_taken = -1;
    }

    if (pPdu0->len == 0)
	{
		DECODE_ADDVALUE("empty choice");
		DECODE_CALLBACK_RET;
    }

    /* read header and len for choice field */
    offset = get_ber_identifier(tvb, offset, &ber_class, &pc, &tag);
    offset = get_ber_length(tvb, offset, &len, &ind);
    end_offset = offset + len ;

    /* loop over all entries until we find the right choice or
       run out of entries */
    ch = choice;
    first_pass = TRUE;
    while (ch->func || first_pass)
	{
        if (branch_taken) {
            (*branch_taken)++;
        }
        /* we reset for a second pass when we will look for choices */
        if (!ch->func) {
            first_pass = FALSE;
            ch = choice; /* reset to the beginning */
            if (branch_taken) {
                *branch_taken = -1;
            }
            continue;
        }

        if ( (first_pass
           && (((ch->ber_class == ber_class) && (ch->tag == tag))
            || ((ch->ber_class == ber_class) && (ch->tag == -1) && (ch->flags & BER_FLAGS_NOOWNTAG))))
          || (!first_pass && (((ch->ber_class == BER_CLASS_ANY) && (ch->tag == -1)))) /* we failed on the first pass so now try any choices */
        ) {
            if (!(ch->flags & BER_FLAGS_NOOWNTAG))
			{
                /* dissect header and len for field */
                hoffset = dissect_ber_identifier(&pdu,pThis, tvb, start_offset, NULL, NULL, NULL);
                hoffset = dissect_ber_length(&pdu,pThis, tvb, hoffset, NULL, NULL);
                start_offset = hoffset;
                if (ind) {
                    length = len - 2;
                } else {
                    length = len;
                }
            } else {
                length = end_offset- hoffset;
            }
            length_remaining = pPdu0->len-hoffset;// tvb_length_remaining(tvb, hoffset);
            if (length_remaining>length)
                length_remaining = length;

            next_tvb = tvb+hoffset;//tvb_new_subset(tvb, hoffset, length_remaining, length);
			next_tvb_len = length_remaining;
            if (next_tvb == NULL)
			{
                ret = pThis->m_iFmtErrorNo;
				DECODE_CALLBACK_RET;
            }
            imp_tag = false;
            if ((ch->flags & BER_FLAGS_IMPLTAG))
                imp_tag = true;
			pdu.implicit_tag = imp_tag;
			pdu.val = next_tvb;
			pdu.len = next_tvb_len;
			DECODE_TREE(ch->strptr,"","");
			ret = ch->func(pThis, &pdu);
            //count = ch->func(imp_tag, next_tvb, 0, actx, tree, *ch->p_id);
            if (ret != 0 && /*(count == 0) && */(((ch->ber_class == ber_class) && (ch->tag == -1) && (ch->flags & BER_FLAGS_NOOWNTAG)) || !first_pass))
			{
                /* wrong one, break and try again */
                ch++;
                continue;
            }
            if (!(ch->flags & BER_FLAGS_NOOWNTAG))
			{
                if (ind) 
				{
					DECODE_ADDVALUE("CHOICE EOC");
                }
            }
            ret = 0;
			DECODE_CALLBACK_RET;
        }
        ch++;
    }
    if (branch_taken)
	{
        /* none of the branches were taken so set the param
           back to -1 */
        *branch_taken = -1;
    }

	DECODE_CALLBACK_RET;
}

static const char* g_bit_name_mask[]={
	"%d... .... = %s",
	".%d.. .... = %s",
	"..%d. .... = %s",
	"...%d .... = %s",
	".... %d... = %s",
	".... .%d.. = %s",
	".... ..%d. = %s",
	".... ...%d = %s"
};

int dissect_ber_bitstring(void *cbParam,stuAsn1Pdu *pPdu,const asn_namedbit *seq)
{
	DECODE_CALLBACK_INTO;
	ST_UCHAR c;
	stuAsn1Pdu pdu,*pPdu0 = pPdu;
	BYTE *pInBuf = pPdu->val;
	int iLen = pPdu->len;
	BYTE *p = pInBuf;
	const asn_namedbit *nb = seq;
	memcpy(&pdu,pPdu0,sizeof(pdu));
	pPdu = &pdu;
	if(pPdu0->implicit_tag == false)
	{
		DECODE_HEAD;
	}
// 	if (!pPdu->implicit_tag && (pPdu->tag.cls != BER_CLASS_APP))
// 	{
// 		if ( (pPdu->tag.cls != BER_CLASS_UNI)
// 			|| (pPdu->tag.id != BER_UNI_TAG_BITSTRING) )
// 		{
// 			ret = pThis->m_iFmtErrorNo;
// 			DECODE_CALLBACK_RET;
// 		}
// 	}
	if(pdu.tag.fmt)
	{
		//TODO
	}
	else
	{
		if(*p == 0 && pdu.len == 1)
		{
			//empty
			DECODE_TREE("BitString Empty","","");
		}
		else
		{
			//padding
			iLen = pdu.len;
			pdu.len = 1;
			if(*p <= 7)
			{
				DECODE_TREE("Padding",SString::toFormat("%d",*p).data(),"");
			}
			else
			{
				DECODE_TREE("Illegal Padding(0 .. 7)",SString::toFormat("%d",*p).data(),"");
			}
			pdu.len = iLen;
		}
	}
	p++;
	pdu.val = p;
	pdu.len --;
	iLen = pdu.len;
	if(nb == 0)
	{
		if(iLen > 10)
		{
			DECODE_TREE("bit-string",SString::toFormat("%s...",SString::HexToStr(p,10,false).data()).data(),"");
		}
		else
		{
			DECODE_TREE("bit-string",SString::HexToStr(p,iLen,false).data(),"");
		}
		DECODE_CALLBACK_RET;
	}
	pdu.len = 1;
	while(nb->tstr != NULL && (int)(nb->bit/8) < iLen)
	{
		pdu.val = &p[nb->bit/8];
		if((*pdu.val & (0x80 >> (nb->bit & 0x07))) != 0)
		{
			//true
			DECODE_TREE(SString::toFormat(g_bit_name_mask[nb->bit & 0x07],1,nb->tstr==NULL?"":nb->tstr).data(),"True","");
		}
		else
		{
			//false
			DECODE_TREE(SString::toFormat(g_bit_name_mask[nb->bit & 0x07],0,nb->fstr==NULL?(nb->tstr==NULL?"":nb->tstr):nb->fstr).data(),"False","");
		}
		nb++;
	}
	DECODE_CALLBACK_RET;
}

int dissect_ber_integer(void *cbParam,stuAsn1Pdu *pPdu,const value_string *vs)
{
	DECODE_CALLBACK_INTO;
	ST_UCHAR c;
	stuAsn1Pdu pdu,*pPdu0 = pPdu;
	BYTE *pInBuf = pPdu->val;
	int iLen = pPdu->len;
	BYTE *p = pInBuf;
	memcpy(&pdu,pPdu0,sizeof(pdu));
	pPdu = &pdu;
	if(pPdu0->implicit_tag == false)
	{
		DECODE_HEAD;
	}

	c = pPdu->len;
	if(c <= 4)
	{
		register int v=0; 
		memcpy(((BYTE*)&v)+4-c,pPdu->val,c); 
		v = ntohl(v); 
		if(vs != NULL)
		{
			while(vs->strptr)
			{
				if(vs->value == v)
				{
					DECODE_ADDVALUE(SString::toFormat("%s (%d)",vs->strptr,v).data());
					break;
				}
				vs ++;
			}
		}
		if(vs == NULL)
			DECODE_ADDVALUE(SString::toFormat("%d",v).data());
	}

	DECODE_CALLBACK_RET;
}

int dissect_ber_restricted_string(void *cbParam,stuAsn1Pdu *pPdu,void *seq)
{
	DECODE_CALLBACK_INTO;
	ST_UCHAR c;
	stuAsn1Pdu pdu,*pPdu0 = pPdu;
	BYTE *pInBuf = pPdu->val;
	int iLen = pPdu->len;
	BYTE *p = pInBuf;
	memcpy(&pdu,pPdu0,sizeof(pdu));
	pPdu = &pdu;
	if(pPdu0->implicit_tag == false)
	{
		DECODE_HEAD;
	}

	DECODE_ADDVALUE(SString::toString((char*)pPdu->val,pPdu->len).data());
	DECODE_CALLBACK_RET;
}

int dissect_ber_boolean(void *cbParam,stuAsn1Pdu *pPdu,void *seq)
{
	DECODE_CALLBACK_INTO;
	ST_UCHAR c;
	stuAsn1Pdu pdu,*pPdu0 = pPdu;
	BYTE *pInBuf = pPdu->val;
	int iLen = pPdu->len;
	BYTE *p = pInBuf;
	memcpy(&pdu,pPdu0,sizeof(pdu));
	pPdu = &pdu;
	if(pPdu0->implicit_tag == false)
	{
		DECODE_HEAD;
	}


#ifdef SDECODE_OFFLINE
	DECODE_ADDVALUE(*pPdu->val?"True":"False");
#endif
	DECODE_CALLBACK_RET;
}

int dissect_ber_sequence_of(void *cbParam,stuAsn1Pdu *pPdu,const ber_sequence_t *seq)
{
	DECODE_CALLBACK_INTO;
	ST_UCHAR c;
	stuAsn1Pdu pdu,*pPdu0=pPdu;
	BYTE *pInBuf = pPdu->val;
	int iLen = pPdu->len;
	BYTE *p = pInBuf;
	pPdu = &pdu;

    char              classx;
    bool           pcx, ind = FALSE, ind_field;
    int             tagx;
    unsigned int            lenx;

//     proto_tree        *tree     = parent_tree;
//     proto_item        *item     = NULL;
//     proto_item        *causex;
    int                cnt, hoffsetx, end_offset;
//    header_field_info *hfi;
    int               length_remaining;
//    tvbuff_t          *next_tvb;
	BYTE *tvb = pPdu0->val;
	BYTE *next_tvb = 0;
	int next_tvb_len = 0;
	int offset = 0;
	bool implicit_tag = pPdu0->implicit_tag;
    if (!implicit_tag)
	{
        hoffsetx = offset;
        /* first we must read the sequence header */
        offset = dissect_ber_identifier(&pdu,pThis, tvb, offset, &classx, &pcx, &tagx);
        offset = dissect_ber_length(&pdu,pThis, tvb, offset, &lenx, &ind);
        if (ind)
		{
            /* if the length is indefinite we don't really know (yet) where the
             * object ends so assume it spans the rest of the tvb for now.
             */
            end_offset = offset + lenx;
        } else {
            end_offset = offset + lenx;
        }

        /* sanity check: we only handle Constructed Universal Sequences */
        if ((classx != BER_CLASS_APP) && (classx != BER_CLASS_PRI))
		{
            if (!pcx
             || (!implicit_tag && ((classx != BER_CLASS_UNI)
                                /*del by skt || (tagx != type)*/)))
			{
				NEW_MALFORMED_D("sequence of expected");
                DECODE_CALLBACK_RET;
            }
        }
    } else {
        /* the tvb length should be correct now nope we could be coming from an implicit choice or sequence, thus we
        read the items we match and return the length*/
        lenx = pPdu0->len - offset;// tvb_length_remaining(tvb, offset);
        end_offset = offset + lenx;
    }

    /* count number of items */
    cnt = 0;
    hoffsetx = offset;
    /* only count the number of items IFF we have the full blob,
     * else this will just generate a [short frame] before we even start
     * dissecting a single item.
     */
    /* XXX Do we really need to count them at all ?  ronnie */
    if(0)// (pPdu0->len - offset/* tvb_length_remaining(tvb, offset)*/ == tvb_reported_length_remaining(tvb, offset))
	{
        while (offset < end_offset)
		{
            unsigned int len;
            int    s_offset;

            s_offset = offset;

            /*if (ind) {  this sequence of was of indefinite length, if this is implicit indefinite impossible maybe
              but ber dissector uses this to eat the tag length then pass into here... EOC still on there...*/
                if (offset + 2 >= (int)pPdu0->len)// (tvb_get_guint8(tvb, offset) == 0) && (tvb_get_guint8(tvb, offset+1) == 0))
				{
                    break;
                }
            /* } */

            /* read header and len for next field */
            offset = get_ber_identifier(tvb, offset, NULL, NULL, NULL);
            offset = get_ber_length(tvb, offset, &len, &ind);
            /* best place to get real length of implicit sequence of or set of is here... */
            /* adjust end_offset if we find somthing that doesnt match */
            offset += len;
            //del by skt cnt++;
            if (offset <= s_offset)
			{
				NEW_MALFORMED;
				ret = pThis->m_iFmtErrorNo;
				DECODE_CALLBACK_RET;
			}
        }
    }
    offset = hoffsetx;


    /* loop over all entries until we reach the end of the sequence */
    while (offset < end_offset)
	{
        char       ber_class;
        bool    pc;
        int      tag;
        unsigned int     len;
        int         eoffset;
        int         hoffset;
        //proto_item *cause;
        bool    imp_tag;

        hoffset = offset;
        /*if (ind) {  this sequence was of indefinite length, if this is implicit indefinite impossible maybe
          but ber dissector uses this to eat the tag length then pass into here... EOC still on there...*/
            if (offset + 2 >= (int)pPdu0->len)// (tvb_get_guint8(tvb, offset) == 0) && (tvb_get_guint8(tvb, offset+1) == 0))
			{
				DECODE_ADDVALUE("SEQ OF EOC");
				ret = 0;
				DECODE_CALLBACK_RET;
            }
        /*}*/
        /* read header and len for next field */
        offset  = get_ber_identifier(tvb, offset, &ber_class, &pc, &tag);
        offset  = get_ber_length(tvb, offset, &len, &ind_field);
        eoffset = offset + len;
                /* Make sure we move forward */
        if (eoffset <= hoffset)
		{
			ret = pThis->m_iFmtErrorNo;
			DECODE_CALLBACK_RET;
		}

        if ((ber_class == BER_CLASS_UNI) && (tag == BER_UNI_TAG_EOC))
		{
            /* This is a zero length sequence of*/
            hoffset = dissect_ber_identifier(&pdu, pThis, tvb, hoffset, NULL, NULL, NULL);
            dissect_ber_length(&pdu,pThis, tvb, hoffset, NULL, NULL);
            DECODE_CALLBACK_RET;
        }
        /* verify that this one is the one we want */
        /* ahup if we are implicit then we return to the upper layer how much we have used */
        if (seq->ber_class != BER_CLASS_ANY)
		{
          if ((seq->ber_class != ber_class)
           || (seq->tag != tag) )
		  {
            if (!(seq->flags & BER_FLAGS_NOTCHKTAG)) {
				DECODE_TREE("Wrong field in SQ OF","","");
                offset = eoffset;
                continue;
                /* wrong.... */
            }
          }
        }

        if (!(seq->flags & BER_FLAGS_NOOWNTAG) && !(seq->flags & BER_FLAGS_IMPLTAG)) {
            /* dissect header and len for field */
            hoffset = dissect_ber_identifier(&pdu,pThis, tvb, hoffset, NULL, NULL, NULL);
            hoffset = dissect_ber_length(&pdu,pThis, tvb, hoffset, NULL, NULL);
        }
        if ((seq->flags == BER_FLAGS_IMPLTAG) && (seq->ber_class == BER_CLASS_CON)) {
            /* Constructed sequence of with a tag */
            /* dissect header and len for field */
            hoffset = dissect_ber_identifier(&pdu,pThis, tvb, hoffset, NULL, NULL, NULL);
            hoffset = dissect_ber_length(&pdu,pThis, tvb, hoffset, NULL, NULL);
            /* Function has IMPLICIT TAG */
        }

        length_remaining = pPdu0->len - hoffset;// tvb_length_remaining(tvb, hoffset);
        if (length_remaining>eoffset-hoffset)
            length_remaining = eoffset-hoffset;
        //next_tvb = tvb_new_subset(tvb, hoffset, length_remaining, length_remaining);
		next_tvb = tvb + hoffset;
		next_tvb_len = length_remaining;

        imp_tag = false;
        if (seq->flags == BER_FLAGS_IMPLTAG)
            imp_tag = true;
        /* call the dissector for this field */
		pdu.implicit_tag = imp_tag;
		pdu.val = next_tvb;
		pdu.len = next_tvb_len;
		DECODE_TREE(seq->strptr,"","");
		ret = seq->func(pThis,&pdu);
        //seq->func(imp_tag, next_tvb, 0, actx, tree, *seq->p_id);
        /* hold on if we are implicit and the result is zero, i.e. the item in the sequence of
           doesnt match the next item, thus this implicit sequence is over, return the number of bytes
           we have eaten to allow the possible upper sequence continue... */
        cnt++; /* rubbish*/
        offset = eoffset;
    }

    /* if we didnt end up at exactly offset, then we ate too many bytes */
    if (offset != end_offset)
	{
		DECODE_ADDVALUE("BER Error:too many byte");
    }

	DECODE_ADDVALUE(SString::toFormat("%d items",cnt).data());
	DECODE_CALLBACK_RET;
}

int dissect_ber_null(void *cbParam,stuAsn1Pdu *pPdu,void *seq)
{
	DECODE_CALLBACK_INTO;
	DECODE_ADDVALUE("Null");
	DECODE_CALLBACK_RET;
}

int dissect_ber_FloatingAndDouble(void *cbParam,stuAsn1Pdu *pPdu,void *seq)
//int dissect_ber_FloatingAndDouble( gboolean implicit_tag, asn1_ctx_t *actx, proto_tree *tree, tvbuff_t *tvb, int offset, gint hf_id, guint32 *value )
{
	DECODE_CALLBACK_INTO;
	unsigned char *mPtr = NULL;
	char mBuf[100];
	unsigned char  *mTmpStr = NULL;

	mBuf[0] = 0;

	mPtr = pPdu->val;

	if( mPtr[0] == (unsigned char)'\x08' && pPdu->len >= 5)
	{//float
		float mTmpVal = 0;
		mTmpStr = (unsigned char  *) &mTmpVal;

		mTmpStr[0] = mPtr[4];
		mTmpStr[1] = mPtr[3];
		mTmpStr[2] = mPtr[2];
		mTmpStr[3] = mPtr[1];
		
		sprintf(mBuf, "%f", mTmpVal);
		DECODE_ADDVALUE(mBuf);
	}
	else
	{

		if( mPtr[0] == (unsigned char)'\x0B' && pPdu->len >= 9)
		{//double
			double mTmpVal = 0;
			mTmpStr = (unsigned char  *) &mTmpVal;
			mTmpStr[0] = mPtr[8];
			mTmpStr[1] = mPtr[7];
			mTmpStr[2] = mPtr[6];
			mTmpStr[3] = mPtr[5];
			mTmpStr[4] = mPtr[4];
			mTmpStr[5] = mPtr[3];
			mTmpStr[6] = mPtr[2];
			mTmpStr[7] = mPtr[1];

			sprintf(mBuf, "%f", mTmpVal);
			DECODE_ADDVALUE(mBuf);
		}
		else
		{
			strcpy( mBuf, "Unknow.");
		}
	}
	DECODE_CALLBACK_RET;
}

int dissect_ber_octet_string(void *cbParam,stuAsn1Pdu *pPdu,void *seq)
{
	DECODE_CALLBACK_INTO;
	ST_UCHAR c;
	stuAsn1Pdu pdu,*pPdu0 = pPdu;
	BYTE *pInBuf = pPdu->val;
	int iLen = pPdu->len;
	BYTE *p = pInBuf;
	memcpy(&pdu,pPdu0,sizeof(pdu));
	pPdu = &pdu;
	if(pPdu0->implicit_tag == false)
	{
		DECODE_HEAD;
	}


#ifdef SDECODE_OFFLINE
	DECODE_ADDVALUE(SString::toFormat("%s%s",SString::HexToStr(pPdu->val,pPdu->len>16?16:pPdu->len,false).data(),pPdu->len>16?"...":"").data());
#endif
	DECODE_CALLBACK_RET;
}
int dissect_ber_object_identifier(void *cbParam,stuAsn1Pdu *pPdu,void *seq)
{
	return 0;
}
int dissect_ber_GeneralizedTime(void *cbParam,stuAsn1Pdu *pPdu,void *seq)
{
	DECODE_CALLBACK_INTO;
	ST_UCHAR c;
	stuAsn1Pdu pdu,*pPdu0 = pPdu;
	BYTE *pInBuf = pPdu->val;
	int iLen = pPdu->len;
	BYTE *p = pInBuf;
	memcpy(&pdu,pPdu0,sizeof(pdu));
	pPdu = &pdu;
	if(pPdu0->implicit_tag == false)
	{
		DECODE_HEAD;
	}


	if ((pPdu->len < 14) || (pPdu->len > 23)) 
	{
		DECODE_TREE(SString::toFormat("GeneralizedTime invalid length: %d",pPdu->len).data(),"","");
		ret = pThis->m_iFmtErrorNo;
		DECODE_CALLBACK_RET;
	}
	int           tmp_int;
	char *tmpstr = (char*)pPdu->val;
	SString str;
	char         *strptr;
	char          first_delim[2];
	int           first_digits;
	char          second_delim[2];
	int           second_digits;
	str = SString::toFormat("%.4s-%.2s-%.2s %.2s:%.2s:%.2s",tmpstr, tmpstr+4, tmpstr+6, tmpstr+8,tmpstr+10, tmpstr+12);
	first_delim[0]  = 0;
	second_delim[0] = 0;
	ret = sscanf( tmpstr, "%14d%1[.,+-Z]%4d%1[+-Z]%4d", &tmp_int, first_delim, &first_digits, second_delim, &second_digits);
	/* tmp_int does not contain valid value bacause of overflow but we use it just for format checking */
	if (ret < 1) 
	{
		DECODE_TREE(SString::toFormat("GeneralizedTime invalid format: %s",SString::toString(tmpstr,pPdu->len).data()).data(),"","");
		ret = pThis->m_iFmtErrorNo;
		DECODE_CALLBACK_RET;
	}
	ret = 0;
	switch (first_delim[0])
	{
	case '.':
	case ',':
		str += SString::toFormat("%c%.3d", first_delim[0], first_digits);
		switch (second_delim[0]) 
		{
		case '+':
		case '-':
			str += SString::toFormat(" (UTC%c%.4d)", second_delim[0], second_digits);
			break;
		case 'Z':
			str += SString::toFormat(" (UTC)");
			break;
		case 0:
			break;
		default:
			/* handle the malformed field */
			break;
		}
		break;
	case '+':
	case '-':
		str += SString::toFormat(" (UTC%c%.4d)", first_delim[0], first_digits);
		break;
	case 'Z':
		str += SString::toFormat(" (UTC)");
		break;
	case 0:
		break;
	default:
		/* handle the malformed field */
		break;
	}
	DECODE_ADDVALUE(str.data());

	DECODE_CALLBACK_RET;
}

int dissect_ber_tagged_type(void *cbParam,stuAsn1Pdu *pPdu,const ber_sequence_t *seq)
{
	DECODE_CALLBACK_INTO;
	DECODE_CALLBACK_RET;
}

#define             G_GNUC_EXTENSION
#ifdef WIN32
G_GNUC_EXTENSION typedef __int64 gint64;
G_GNUC_EXTENSION typedef unsigned __int64 guint64;
#define G_GINT64_CONSTANT(val)	(G_GNUC_EXTENSION (val))
#define G_GUINT64_CONSTANT(val)	(G_GNUC_EXTENSION (val))
#else
G_GNUC_EXTENSION typedef signed long long gint64;
G_GNUC_EXTENSION typedef unsigned long long guint64;
#define G_GINT64_CONSTANT(val)	(G_GNUC_EXTENSION (val##LL))
#define G_GUINT64_CONSTANT(val)	(G_GNUC_EXTENSION (val##ULL))
#endif

int dissect_ber_utctime(void *cbParam,stuAsn1Pdu *pPdu,void *seq)
{
	DECODE_CALLBACK_INTO;
	ST_UCHAR c;
	stuAsn1Pdu pdu,*pPdu0 = pPdu;
	BYTE *pInBuf = pPdu->val;
	int iLen = pPdu->len;
	BYTE *p = pInBuf;
	memcpy(&pdu,pPdu0,sizeof(pdu));
	pPdu = &pdu;
	if(pPdu0->implicit_tag == false)
	{
		DECODE_HEAD;
	}
	if(pPdu->len != 8)
	{
		DECODE_ADDVALUE("Invalid Utc time");
		DECODE_CALLBACK_RET;
	}
	register int v=0;
	memcpy(((BYTE*)&v),pPdu->val,4);
	int seconds = ntohl(v);
	v = 0;
	memcpy(((BYTE*)&v)+1,pPdu->val+4,3);
	int fraction = ntohl(v);
	unsigned int nanoseconds = (unsigned int)( ((guint64)fraction * G_GUINT64_CONSTANT(1000000000)) / G_GUINT64_CONSTANT(0x100000000) ) ;
	int ms = nanoseconds/1000000;
	int us = (nanoseconds/1000)%1000;
	int ns = nanoseconds%1000;
	SString tstr = SString::toFormat("%s.%03d",SDateTime::makeDateTime((time_t)seconds,ms,us).toString("yyyy-MM-dd hh:mm:ss.zzz.uuu").data(),ns);
	DECODE_ADDVALUE(tstr.data());
	DECODE_CALLBACK_RET;
}

#define	PLURALIZE(n)	(((n) > 1) ? "s" : "")
#define	COMMA(do_it)	((do_it) ? ", " : "")
int dissect_ber_timeofday(void *cbParam,stuAsn1Pdu *pPdu,void *seq)
{
	DECODE_CALLBACK_INTO;
	ST_UCHAR c;
	stuAsn1Pdu pdu,*pPdu0 = pPdu;
	BYTE *pInBuf = pPdu->val;
	int iLen = pPdu->len;
	BYTE *p = pInBuf;
	memcpy(&pdu,pPdu0,sizeof(pdu));
	pPdu = &pdu;
	if(pPdu0->implicit_tag == false)
	{
		DECODE_HEAD;
	}
	bool do_comma=false;
	const char *msign = "";
	int len = pPdu->len;
	int milliseconds;
	int msecs;
	SString sTime;
	unsigned short days;
// 	gchar *	ptime;
// 	nstime_t ts;
	if(len == 4)
	{
		register int v=0;
		bool is_nsecs = false;
		memcpy(((BYTE*)&v),pPdu->val,4);
		milliseconds = ntohl(v);
		if(milliseconds == 0)
			sTime = "0 seconds";
		else
		{
			if(milliseconds<0){
				/* oops we got passed a negative time */
				milliseconds= -milliseconds;
				msecs = milliseconds % 1000;
				milliseconds /= 1000;
				milliseconds= -milliseconds;
			} else {
				msecs = milliseconds % 1000;
				milliseconds /= 1000;
			}
			if(milliseconds < 0)
			{
				milliseconds = -milliseconds;
				msign = "-";
			}
			int secs = milliseconds % 60;
			milliseconds /= 60;
			int mins = milliseconds % 60;
			milliseconds /= 60;
			int hours = milliseconds % 24;
			milliseconds /= 24;

			if (milliseconds != 0) {
				sTime += SString::toFormat("%s%u day%s", msign, milliseconds, PLURALIZE(milliseconds));
				do_comma = TRUE;
				msign="";
			}
			if (hours != 0) {
				sTime += SString::toFormat("%s%s%u hour%s", COMMA(do_comma), msign, hours, PLURALIZE(hours));
				do_comma = true;
				msign="";
			}
			if (mins != 0) {
				sTime += SString::toFormat("%s%s%u minute%s", COMMA(do_comma), msign, mins, PLURALIZE(mins));
				do_comma = true;
				msign="";
			}
			if (secs != 0 || msecs != 0) {
				if (msecs != 0) {
					if (is_nsecs)
						sTime += SString::toFormat( "%s%s%u.%09u seconds", COMMA(do_comma), msign, secs, msecs);
					else
						sTime += SString::toFormat( "%s%s%u.%03u seconds", COMMA(do_comma), msign, secs, msecs);
				} else
					sTime += SString::toFormat( "%s%s%u second%s", COMMA(do_comma), msign, secs, PLURALIZE(secs));
			}
		}
	}
	else if(len == 6)
	{
		register int v=0;
		bool is_nsecs = false;
		memcpy(((BYTE*)&v),pPdu->val,4);
		milliseconds = ntohl(v);
		v= 0 ;
		memcpy(((BYTE*)&v)+2,pPdu->val+4,2);
		days = (unsigned short)ntohl(v);

		/* 5113 days between 01-01-1970 and 01-01-1984 */
		/* 86400 seconds in one day */

		int secs = (days + 5113) * 86400 + milliseconds / 1000;
		int nanoseconds = (milliseconds % 1000) * 1000000U;
		int ms = nanoseconds/1000000;
		int us = (nanoseconds/1000)%1000;
		int ns = nanoseconds%1000;
		sTime = SString::toFormat("%s.%03d",SDateTime::makeDateTime((time_t)secs,ms,us).toString("yyyy-MM-dd hh:mm:ss.zzz.uuu").data(),ns);
	}
	else
	{
		sTime = "Error Time";
	}
	DECODE_ADDVALUE(sTime.data());
	DECODE_CALLBACK_RET;
}

