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
	pThis = this;
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
#if 0
	//从本协议的数据开始位置进行解码，下层协议的解码结果已经在预处理信息中(m_pPreprocessInfo)
	BYTE *pBuffer = m_pPreprocessInfo->m_pFrameBuffer;
	int iLen = m_pPreprocessInfo->m_iFrameValidBytes;
	int pos = m_pPreprocessInfo->m_iStartPos;
	if(m_pPreprocessInfo->m_pSession == NULL || m_pPreprocessInfo->m_pTcp == NULL)
	{
		NEW_MALFORMED_PD(pBuffer,pos,iLen-pos,"no valid session found");
		return SDECODE_FMT_TCP;
	}
	stuSDecodeMmsStatus *pStatus = (stuSDecodeMmsStatus*)m_pLastSession->GetStatusBuf();
	if(pStatus == NULL)
	{
		pStatus = new stuSDecodeMmsStatus();
		memset(pStatus,0,sizeof(stuSDecodeMmsStatus));
		m_pLastSession->SetStatusBuf((BYTE*)pStatus,sizeof(stuSDecodeMmsStatus));
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

	//第一步，解码TPKT协议
// 	    0               1               2               3
// 		0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
// 		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// 		| vrsn          | reserved      | packet length                 |
// 		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	if(pBuffer[pos] != 0x03)
	{
		NEW_MALFORMED_PD(pBuffer,pos,iLen-pos,"TPKT version invalid");
		return SDECODE_FMT_TPKT;
	}
	unsigned short tpkt_len;
	memcpy(&tpkt_len,pBuffer+pos+2,2);
	SKT_SWAP_WORD_REV(tpkt_len);
	if(tpkt_len != m_pPreprocessInfo->m_iFrameValidBytes - pos)
	{
		NEW_MALFORMED_PD(pBuffer,pos,iLen-pos,"TPKT length invalid");
		return SDECODE_FMT_TPKT;
	}
#ifdef SDECODE_OFFLINE
	if(m_pDecodeTreeRoot != NULL)
	{
		SDecodeTree *pTPKT = m_pDecodeTreeRoot->New(pBuffer,pos,4,"TPKT");
		pTPKT->New(pBuffer,pos,1,"Version",SString::toFormat("%d",pBuffer[pos]).data());
		pTPKT->New(pBuffer,pos+1,1,"Reserved",SString::toFormat("%d",pBuffer[pos+1]).data());
		pTPKT->New(pBuffer,pos+2,2,"Length",SString::toFormat("%d",ntohs(*(unsigned short*)&pBuffer[pos+2])).data());
	}
#endif
	pos += 4;

	//第二步，解码ISO8073(RFC905)COTP
	m_pPreprocessInfo->m_iStartPos = pos;
	int ret = Decode8073(soc,nsec);

#endif
	return 0;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  解码8073/COTP报文
// 作    者:  邵凯田
// 创建时间:  2015-5-10 21:47
// 参数说明:  @soc表示报文时标（秒）
//         :  @nsec表示报文时标（纳秒）
// 返 回 值:  int,0表示解码成功，对应eSDecodeFmtError
//////////////////////////////////////////////////////////////////////////
int SDecodeMms::Decode8073(int soc,int nsec)
{
#if 0
	int pos = m_pPreprocessInfo->m_iStartPos;
	stuSDecodeMmsStatus *pStatus = (stuSDecodeMmsStatus*)m_pLastSession->GetStatusBuf();
	BYTE *pBuffer = m_pPreprocessInfo->m_pFrameBuffer;
	BYTE *pBuf8073 = pBuffer + pos;
	int iLen = m_pPreprocessInfo->m_iFrameValidBytes;
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
	if(pos + cotp_len > iLen)
	{
		NEW_MALFORMED_PD(pBuffer,pos,iLen-pos,"OSI 8073 length invalid");
		return SDECODE_FMT_8073;
	}
#ifdef SDECODE_OFFLINE
	SDecodeTree *p8073;
	if(m_pDecodeTreeRoot != NULL)
	{
		p8073 = m_pDecodeTreeRoot->New(pBuffer,pos,cotp_len,
			"ISO 8073/X.224 COTP Connection-Oriented Transport Protocol");
	}
#endif
#ifdef SDECODE_OFFLINE
	if(m_pDecodeTreeRoot != NULL)
	{
		p8073->New(pBuffer,pos,1,"Length",SString::toFormat("%d",cotp_len-1).data());
	}
#endif
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
			NEW_MALFORMED_PD(pBuffer,pos,iLen-pos,"OSI 8073 length invalid");
			return SDECODE_FMT_8073;
		}
// 		The structure of the CR TPDU shall be as follows:
// 		1    2        3        4       5   6    7    8    p  p+1...end
// 		+--+------+---------+---------+---+---+------+-------+---------+
// 		|LI|CR CDT|     DST - REF     |SRC-REF|CLASS |VARIAB.|USER     |
// 		|  |1110  |0000 0000|0000 0000|   |   |OPTION|PART   |DATA     |
// 		+--+------+---------+---------+---+---+------+-------+---------+

		pStatus->m_iCotpClass = pBuf8073[6]>>4;
		pStatus->m_iIsExtended = ((pBuf8073[6]&0x02) >> 1);
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
		if(pStatus->m_iCotpClass <= 1)
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
		else if(pStatus->m_iIsExtended == 0)
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
			p8073->New(pBuffer,pos,1,"Last Data Unit",SString::toFormat("%s",eot==1?"Yes":"No").data());
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
			if(m_pLastSession->IsLastFrameS2C())
				m_pLastSession->AddToDatBuf(SDECODE_BUF_S2C,pBuf8073+cotp_len,iLen-pos-cotp_len);
			else
				m_pLastSession->AddToDatBuf(SDECODE_BUF_C2S,pBuf8073+cotp_len,iLen-pos-cotp_len);
		}
		else
		{
			int iDatLen;
			if(m_pLastSession->IsLastFrameS2C())
			{
				if(m_pLastSession->GetDatBufLen(SDECODE_BUF_S2C) > 0)
				{
					m_pLastSession->AddToDatBuf(SDECODE_BUF_S2C,pBuf8073+cotp_len,iLen-pos-cotp_len);
					m_pPreprocessInfo->m_pViewBuffer = pBuffer = m_pLastSession->GetDatBuf(SDECODE_BUF_S2C);
					m_pPreprocessInfo->m_iViewBytes = iDatLen = m_pLastSession->GetDatBufLen(SDECODE_BUF_S2C);
				}
				else
				{
					pBuffer = pBuf8073+cotp_len;
					iDatLen = iLen - pos - cotp_len;
				}
			}
			else
			{
				if(m_pLastSession->GetDatBufLen(SDECODE_BUF_C2S) > 0)
				{
					m_pLastSession->AddToDatBuf(SDECODE_BUF_C2S,pBuf8073+cotp_len,iLen-pos-cotp_len);
					m_pPreprocessInfo->m_pViewBuffer = pBuffer = m_pLastSession->GetDatBuf(SDECODE_BUF_C2S);
					m_pPreprocessInfo->m_iViewBytes = iDatLen = m_pLastSession->GetDatBufLen(SDECODE_BUF_C2S);
				}
				else
				{
					pBuffer = pBuf8073+cotp_len;
					iDatLen = iLen - pos - cotp_len;
				}
			}
			//TODO:处理上层协议,pBuffer,iDatLen
			
			int ret = Decode8327(pBuffer,iDatLen);
			//清除缓存数据，并不真正清除，仍可以在界面上引用
			if(m_pLastSession->IsLastFrameS2C())
				m_pLastSession->RemoveDatBufFront(SDECODE_BUF_S2C,m_pLastSession->GetDatBufLen(SDECODE_BUF_S2C));
			else
				m_pLastSession->RemoveDatBufFront(SDECODE_BUF_C2S,m_pLastSession->GetDatBufLen(SDECODE_BUF_C2S));
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
		NEW_MALFORMED_PD(m_pPreprocessInfo->m_pFrameBuffer,pos,iLen-pos,"PDU type invalid");
		return SDECODE_FMT_8073;
		break;
	}
#endif
	return 0;
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
#if 0
	BYTE cosp_type = p8327DatBuffer[0];
	BYTE cosp_len = p8327DatBuffer[1];
	int ret;
	BYTE *p = p8327DatBuffer;
#ifdef SDECODE_OFFLINE
	SDecodeTree *p8327;
	if(m_pDecodeTreeRoot != NULL)
	{
		p8327 = m_pDecodeTreeRoot->New(m_pPreprocessInfo->m_pViewBuffer,p8327DatBuffer-m_pPreprocessInfo->m_pViewBuffer,cosp_len,"ISO 8327-1 OSI Session Protocol");
	}
#endif
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
		ret = cosp_process_connect(p8327DatBuffer,iLen);
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
		ret = cosp_process_disconnect(p8327DatBuffer,iLen);
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
			p += 1 + cosp_len;
			
			cosp_len = p[1];
			p8327 = m_pDecodeTreeRoot->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,cosp_len,"ISO 8327-1 OSI Session Protocol");
			p8327->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,1,"SPDU Type","DATA TRANSFER (DT) SPDU (1)");
			p8327->New(m_pPreprocessInfo->m_pViewBuffer,p+1-m_pPreprocessInfo->m_pViewBuffer,1,"Length",SString::toFormat("%d",cosp_len).data());

		}
#endif
		{
//			COSP_DT	dec_par;
//			off = cosp_process_data(&dec_par,off);
		}
		break;
	default:
		NEW_MALFORMED_PD(m_pPreprocessInfo->m_pViewBuffer,0,0,"COSP Type invalid");
		return SDECODE_FMT_8327;
	}
#endif
	return 0;
}

int SDecodeMms::DecodeAsn1(BYTE *pInBuf,int iLen,BYTE* &pNext,ST_UCHAR &t,ST_UINT& decode_id,ST_UINT &id_bytes,ST_INT &decode_len,ST_UINT &len_bytes)
{return 0;
#if 0
	pNext = pInBuf;
	decode_id = (ST_UINT)asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF)
	{
		NEW_MALFORMED_ID;
		return SDECODE_FMT_MMS;
	}
	t = *p;
	//constr_elmnt = (ST_UCHAR) (t & CONSTR);
	decode_len = (ST_INT)asn1r_head_decode_len(p + id_bytes,&len_bytes);
	pNext += id_bytes + len_bytes+decode_len;
	if (decode_len == -1 || pNext > pInBuf+iLen)
	{
		NEW_MALFORMED_LEN;
		return SDECODE_FMT_MMS;
	}
	return 0;
#endif
}
int SDecodeMms::Decode8823(BYTE *pInDatBuffer,int iLen,BYTE* &pOutDatBuffer,int &iOutLen)
{
#if 0
	int ret = 0;
	BYTE *p,*pNext;
	int off = 0;
	unsigned int info_type;
	ST_UCHAR t;
	ST_UCHAR constr_elmnt;
	ST_UINT decode_id;
	ST_UINT id_bytes;
	ST_INT decode_len;
	ST_UINT len_bytes;

	int invokeId;
	SDecodeTree *pRoot,*pNode2,*pNode3;
	p = pInDatBuffer;
	decode_len=iLen;

	ASN1_DECODE_8823(p,iLen,pNext);
	if(decode_id == 17)
	{
#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			pRoot = m_pDecodeTreeRoot->New(m_pPreprocessInfo->m_pViewBuffer,pInDatBuffer-m_pPreprocessInfo->m_pViewBuffer,
				iLen,"ISO 8823 OSI Presentation Protocol");
			pRoot = pRoot->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,
				iLen,"CP-type");
		}
#endif
		p += id_bytes+len_bytes;
		ASN1_DECODE_8823(p,iLen,pNext);
		
		//mode-selector
		if(decode_id == 0)
		{
#ifdef SDECODE_OFFLINE
			if(m_pDecodeTreeRoot != NULL)
			{
				pNode2 = pRoot->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,decode_len,"mode-selector");
				p += id_bytes+len_bytes;
				ASN1_DECODE_8823(p,iLen,pNext);
				pNode2->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,decode_len,"mode-value",
					SString::toFormat("%d",GetIntWithBufLen(p,decode_len)).data());
			}
#endif
		}
#ifdef SDECODE_OFFLINE	
		else if(decode_id == 1)
		{
			if(m_pDecodeTreeRoot != NULL)
			{
				pNode2 = pRoot->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,decode_len,"x410-mode-parameters");
			}
		}
#endif
		else if(decode_id == 2)
		{
#ifdef SDECODE_OFFLINE
			if(m_pDecodeTreeRoot != NULL)
			{
				pNode2 = pRoot->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,decode_len,"normal-mode-parameters");
			}
#endif
			p += id_bytes+len_bytes;
			ASN1_DECODE_8823(p,iLen,pNext);
			//protocol-version
#ifdef SDECODE_OFFLINE
			if(m_pDecodeTreeRoot != NULL)
			{
				if(decode_id == 0)
					pNode2->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,decode_len,"protocol-version");
				else if(decode_id == 1)
					pNode2->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,decode_len,
					"calling-presentation-selector",SString::toFormat("%d",GetIntWithBufLen(p,decode_len)).data());
				else if(decode_id == 2)
					pNode2->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,decode_len,
					"called-presentation-selector",SString::toFormat("%d",GetIntWithBufLen(p,decode_len)).data());
				else if(decode_id == 4)
				{
					pNode3 = pNode2->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,decode_len,
						"presentation-context-definition-list");
				}
			}
#endif
			ASN1_OUT;
		}
		ASN1_OUT;
	}
	else if(decode_id == 1)
	{
		//user-data
	}

#if 0

	ASN1_IN;
	//CP-type
	if(decode_id == 17)
	{
	#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			pRoot = m_pDecodeTreeRoot->New(m_pPreprocessInfo->m_pViewBuffer,pInDatBuffer-m_pPreprocessInfo->m_pViewBuffer,
				iLen,"ISO 8823 OSI Presentation Protocol");
			pRoot = pRoot->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,
				iLen,"CP-type");
		}
	#endif
		ASN1_IN;
		//mode-selector
		if(decode_id == 0)
		{
	#ifdef SDECODE_OFFLINE
			if(m_pDecodeTreeRoot != NULL)
			{
				ASN1_IN;
				pNode2 = pRoot->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,decode_len,"mode-selector");
				ASN1_IN;
				
				pNode2->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,decode_len,"mode-value",
					SString::toFormat("%d",GetIntWithBufLen(p,decode_len)).data());
				ASN1_OUT;
				ASN1_OUT;
			}
	#endif
		}
	#ifdef SDECODE_OFFLINE	
		else if(decode_id == 1)
		{
			ASN1_IN;
			//x410-mode-parameters

			if(m_pDecodeTreeRoot != NULL)
			{
				pNode2 = pRoot->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,decode_len,"x410-mode-parameters");
			}
			ASN1_OUT;
		}
	#endif
		else if(decode_id == 2)
		{
	#ifdef SDECODE_OFFLINE
			if(m_pDecodeTreeRoot != NULL)
			{
				pNode2 = pRoot->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,decode_len,"normal-mode-parameters");
			}
	#endif
			ASN1_IN;
			//protocol-version
	#ifdef SDECODE_OFFLINE
			if(m_pDecodeTreeRoot != NULL)
			{
				if(decode_id == 0)
					pNode2->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,decode_len,"protocol-version");
				else if(decode_id == 1)
					pNode2->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,decode_len,
					"calling-presentation-selector",SString::toFormat("%d",GetIntWithBufLen(p,decode_len)).data());
				else if(decode_id == 2)
					pNode2->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,decode_len,
					"called-presentation-selector",SString::toFormat("%d",GetIntWithBufLen(p,decode_len)).data());
				else if(decode_id == 4)
				{
					pNode3 = pNode2->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,decode_len,
					"presentation-context-definition-list");
				}
			}
	#endif
			ASN1_OUT;
		}
		ASN1_OUT;
	}
	else if(decode_id == 1)
	{
		//user-data
	}

	
	ASN1_OUT;//CP-type

#endif
#endif
	return 0;
}

int SDecodeMms::cosp_process_connect(BYTE *pDatBuffer,int iLen)
{
#if 0
	int ret = 0;
	unsigned char * p;
	unsigned int plen = 0;
	//COSP_CN_AC	dec_par;
	int off = 0;
	BYTE* pUserData;
	int iUserDataLen;
//	off = ioff;
	p = pDatBuffer;
	plen = iLen;

	ret = cosp_dec_cn_ac((char *)p,plen,COSP_SI_CONNECT,pUserData,iUserDataLen);
	if (ret == SD_SUCCESS)
	{
		BYTE* pUserData8823=NULL;
		int iUserDataLen8823=0;
		
		//ret = cosp_data_8823_cn_ac(pUserData,iUserDataLen,pUserData8823,iUserDataLen8823);
		ret = Decode8823(pUserData,iUserDataLen,pUserData8823,iUserDataLen8823);
		if(ret != 0 || pUserData8823 == NULL)
			return ret;
		BYTE* pUserData8860=NULL;
		int iUserDataLen8860=0;
		ret = cosp_data_8650_1(pUserData8823,iUserDataLen8823,pUserData8860,iUserDataLen8860);
		if(ret != 0 || pUserData8860 == NULL)
			return ret;
		ret = DecodeMms(pUserData8860,iUserDataLen8860);
		if(ret != 0)
			return ret;
		/*邵凯田
		m_Osi_8327_1.m_NextOSIType = OSITYPE_8823_DT;
		off = off + (plen - dec_par.udata_len);
		off = cosp_data_8823_cn_ac(off);
		if (off != -1 && off < m_BufLength)
		{
			off = cosp_data_8650_1(off);
			if (off != -1 && off < m_BufLength && m_Osi_8650_1.m_NextOsiType == OSITYPE_9506)
			{
				off = cosp_data_9506(off);
				ret = off;
			}
			else
			{
				ret = off;
			}
		}
		else
		{
			ret = off;
		}
		*/
	}

	return ret;
#endif
	return 0;
}


int SDecodeMms::cosp_process_disconnect(BYTE *pDatBuffer,int iLen)
{
#if 0
	int ret = 0;
	BYTE * p;
	unsigned int plen = 0;
//	COSP_FN_DN	dec_par;
	int off = 0;
	BYTE* pUserData;
	int iUserDataLen;
	BYTE* pUserData8823;
	int iUserDataLen8823;

	p = pDatBuffer;
	plen = iLen;

//	off = ioff;
	ret = cosp_dec_fn_dn((char *)p,plen,COSP_SI_DISCON,pUserData,iUserDataLen);
	if (ret == SD_SUCCESS)
	{
		ret = Decode8823(pUserData,iUserDataLen,pUserData8823,iUserDataLen8823);
		/*
		m_Osi_8327_1.m_NextOSIType = OSITYPE_8823_DT;
		off = off + (plen - dec_par.udata_len);
		off = cosp_data_8823_cn_ac(off);
		if (off != -1 && off < m_BufLength)
		{
			off = cosp_data_8650_1(off);
			if (off != -1 && off < m_BufLength && m_Osi_8650_1.m_NextOsiType == OSITYPE_9506)
			{
				// 				off = cosp_data_9506(off);
			}
			else
			{
				ret = off;
			}
		}
		else
		{
			ret = off;
		}
		*/
	}

	return ret;
#endif
	return 0;
}


int SDecodeMms::cosp_dec_fn_dn(char *spdu_buf, ST_UINT spdu_len,	ST_UCHAR spdu_type, BYTE* &pUserData,int &iUserDataLen)
{
#if 0
	int		ret;
	ST_UCHAR	*dec_buf;
	ST_UINT	dec_len;
	ST_UCHAR	param_code;
	ST_UINT	param_len;
	ST_UINT	len_bytes;	/* number of bytes for encoded len (1 or 3) */
	ST_UINT	idx;
	ST_UINT off = m_pPreprocessInfo->m_iViewBytes - spdu_len;
	unsigned char * p = (unsigned char *)spdu_buf;
	pUserData = NULL;
	iUserDataLen = 0;

#ifdef SDECODE_OFFLINE
	SDecodeTree *p8327 = m_pDecodeTreeRoot->GetChildsPtr()->getTail();
	if(m_pDecodeTreeRoot != NULL)
	{
		//p8327->New(pDatBuffer,0,1,"SPDU Type","CONNECT SPDU(13)");
	}
#endif
	ret = SD_SUCCESS;

	dec_buf = (ST_UCHAR *) spdu_buf;
	dec_len = 0;
	idx = 0;

	/*-----------------------------------------------------*/
	/* set defaults in case params are not present in SPDU */
	/*-----------------------------------------------------*/

// 	memset (dec_par, 0, sizeof (COSP_FN_DN));
// 	if (spdu_type == COSP_SI_FINISH)
// 		dec_par->disconnect        = 1;	/* default disconnect transport	*/

	/* in addition memset is equivalent to following con params settings	*/
	/* dec_par->udata_len      = 0;	Length of User Data		*/
	/* dec_par->udata_ptr      = 0;       Pointer to User Data (points into spdu_buf!)	*/

	/* we will use the first while loop to avoid excesive nesting of if...else... */

	while (SD_TRUE)
	{
		/* NOTE: we should never return back from bottom of the loop here!!! */

		/* the decoded len should be comparable with spdu_len passed to this func */
		dec_len = (ST_UINT) cosp_dec_len (&dec_buf[idx+1], &len_bytes);
		if (dec_len + 1+len_bytes != spdu_len)
		{
			NEW_MALFORMED_D("length invalid");
			return SDECODE_FMT_8327;
		}
// 		InitDataBasepCap(&m_Osi_8327_1.m_ilen,p,DATATYPE_UCHAR,off,off + 1 + len_bytes + dec_len,1 + len_bytes + dec_len);
// 		InitDataBasepCap(&m_Osi_8327_1.m_spdu_type,p,DATATYPE_UCHAR,off,off + 1,1);
// 		InitDataBasepCap(&m_Osi_8327_1.m_Length,p + 1,DATATYPE_UCHAR,off + 1,off + 1 + len_bytes,len_bytes);
// 		m_Osi_8327_1.m_iLength = (unsigned short)dec_len;
// 		m_Osi_8327_1.m_bOsi8327_1 = TRUE;

		idx += 1+len_bytes;		/* 1 for FN/DN SPDU code */

		/* decode all FINISH / DISCONNECT parameters */
		while (dec_len > 0)
		{
			param_code = dec_buf [idx];
			param_len  = (ST_UINT)cosp_dec_len (&dec_buf [idx+1], &len_bytes);

			/* signal error if param_len is greater then decoding length left */
			if (param_len + 1+len_bytes > dec_len)
			{
				NEW_MALFORMED_D("length invalid");
				return SDECODE_FMT_8327;
			}
			dec_len -= 1+len_bytes;
			idx     += 1+len_bytes;

			/* do not try to decode if param_len is 0 (param is empty)	*/
			if (param_len == 0)
				continue;

			switch (param_code)
			{
			case  17:	/* PI - Transport Disconnect */
				/* this param is valid only in FINISH SPDU */
#ifdef SDECODE_OFFLINE
				if(m_pDecodeTreeRoot != NULL)
				{
					SDecodeTree *p = p8327->New(m_pPreprocessInfo->m_pViewBuffer,dec_buf+idx-m_pPreprocessInfo->m_pViewBuffer,1,"Transport Disconnect");
					p->New(m_pPreprocessInfo->m_pViewBuffer,dec_buf+idx-len_bytes-1-m_pPreprocessInfo->m_pViewBuffer,1,"Parameter Type","Transport Disconnect");
					p->New(m_pPreprocessInfo->m_pViewBuffer,dec_buf+idx-len_bytes-m_pPreprocessInfo->m_pViewBuffer,len_bytes,"Parameter Length",
						SString::toFormat("%d",GetIntWithBufLen(&dec_buf[idx]-len_bytes,len_bytes)).data());
				}
#endif
				if (spdu_type == COSP_SI_FINISH)
				{
//					dec_par->disconnect = (ST_BOOLEAN)(dec_buf [idx] & 0x01);
					if ((ST_BOOLEAN)(dec_buf [idx] & 0x01) != 1)
					{
						NEW_MALFORMED;
						return SDECODE_FMT_8327;
					}
				}
				else
				{
					NEW_MALFORMED;
					return SDECODE_FMT_8327;
				}
				break;

			case  193:	/* PGI - User Data group */
				pUserData = (BYTE*)&dec_buf[idx];
				iUserDataLen = GetIntWithBufLen(&dec_buf[idx]-len_bytes,len_bytes);
#ifdef SDECODE_OFFLINE
				if(m_pDecodeTreeRoot != NULL)
				{
					SDecodeTree *p = p8327->New(m_pPreprocessInfo->m_pViewBuffer,dec_buf+idx-m_pPreprocessInfo->m_pViewBuffer,1,"Session User Data");
					p->New(m_pPreprocessInfo->m_pViewBuffer,dec_buf+idx-len_bytes-1-m_pPreprocessInfo->m_pViewBuffer,1,"Parameter Type","Session User Data");
					p->New(m_pPreprocessInfo->m_pViewBuffer,dec_buf+idx-len_bytes-m_pPreprocessInfo->m_pViewBuffer,len_bytes,"Parameter Length",
						SString::toFormat("%d",iUserDataLen).data());
				}
#endif
				break;

			default:
				NEW_MALFORMED;
				return SDECODE_FMT_8327;
				// 				COSP_LOG_ERR1 ("COSP-ERROR: Decoding FN/DN: invalid PI code=%d", param_code);
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

	/* validate the received SPDU */
	if (ret == SD_SUCCESS)
	{
		if (iUserDataLen == 0)
		{
			NEW_MALFORMED;
			return SDECODE_FMT_8327;
		}
	}

	return ret;
#endif
	return 0;
}

ST_UINT16 SDecodeMms::cosp_dec_len(ST_UCHAR *buf, ST_UINT *bytes)
{
	ST_UINT16   len;

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


int SDecodeMms::cosp_dec_cn_ac(char *spdu_buf, ST_UINT spdu_len, ST_UCHAR spdu_type, BYTE* &pUserData,int &iUserDataLen)
{
#if 0
	int		ret;
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
	SDecodeTree *p8327 = m_pDecodeTreeRoot->GetChildsPtr()->getTail();
	if(m_pDecodeTreeRoot != NULL)
	{
		//p8327->New(pDatBuffer,0,1,"SPDU Type","CONNECT SPDU(13)");
	}
#endif
	ret = SD_SUCCESS;

	dec_buf = (ST_UCHAR *) spdu_buf;
	dec_len = 0;
	idx = 0;

	/*-----------------------------------------------------*/
	/* set defaults in case params are not present in SPDU */
	/*-----------------------------------------------------*/

// 	memset (dec_par, 0, sizeof (COSP_CN_AC));
// 	dec_par->ver_num        = COSP_VER1;		/*!default ver is Version1 */
// 	dec_par->ses_urequir [0]= 0x03;		/* bits 9-16: 9 and 10 set */
// 	dec_par->ses_urequir [1]= 0x49;		/* bits 1-8: 1,4 and 7 set */

	/* in addition memset is equivalent to following con params settings */
	/* dec_par->prot_option         = 0;   rcv extended concatination not supported	*/
	/* dec_par->initiator_tsdu_size = 0;   SSDU segmenting from initiator not supported	*/
	/* dec_par->responder_tsdu_size = 0;   SSDU segmenting from responder not supported	*/
	/* dec_par->loc_ssel [0]        = 0;   called SSEL is NULL				*/
	/* dec_par->rem_ssel [0]        = 0;   calling SSEL is NULL				*/
	/* dec_par->udata_len           = 0;   User Data length  				*/
	/* dec_par->udata_ptr           = 0;   User Data pointer (points into spdu_buf!)	*/

	/* we will use the first while loop to avoid excesive nesting of if...else... */

	while (SD_TRUE)
	{
		/* NOTE: we should never return back from bottom of the loop here!!! */

		/* the decoded len should be comparable with spdu_len passed to this func */
		dec_len = (ST_UINT) cosp_dec_len (&dec_buf[idx+1], &len_bytes);
		if (dec_len + 1+len_bytes != spdu_len)
		{
			NEW_MALFORMED;
			return SDECODE_FMT_8327;
		}
		//MEMO:  [2015-5-11 10:41 邵凯田]
		/*
		InitFieldViewInfo(&m_Osi_8327_1.m_ilen,p,DATATYPE_UCHAR,off,off + 1 + len_bytes + dec_len,1 + len_bytes + dec_len);
		InitFieldViewInfo(&m_Osi_8327_1.m_spdu_type,p,DATATYPE_UCHAR,off,off + 1,1);
		InitFieldViewInfo(&m_Osi_8327_1.m_Length,p + 1,DATATYPE_UCHAR,off + 1,off + 1 + len_bytes,len_bytes);
		m_Osi_8327_1.m_iLength = (unsigned short)dec_len;
		m_Osi_8327_1.m_bOsi8327_1 = TRUE;
		*/
		idx += 1+len_bytes;		/* 1 for SPDU code */

		/* decode all connect parameters */
		while (dec_len > 0)
		{
			param_code = dec_buf [idx];
			param_len  = (ST_UINT) cosp_dec_len (&dec_buf [idx+1], &len_bytes);

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
				if(m_pDecodeTreeRoot != NULL)
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
				if(m_pDecodeTreeRoot != NULL)
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
					param_len  = (ST_UINT) cosp_dec_len (&dec_buf [idx+1], &len_bytes);

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
						if(m_pDecodeTreeRoot != NULL)
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
							if(m_pDecodeTreeRoot != NULL)
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
					if(m_pDecodeTreeRoot != NULL)
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
					if(m_pDecodeTreeRoot != NULL)
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
						if(m_pDecodeTreeRoot != NULL)
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
						if(m_pDecodeTreeRoot != NULL)
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
					if(m_pDecodeTreeRoot != NULL)
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
					if(m_pDecodeTreeRoot != NULL)
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
#endif
	return 0;
}


ST_INT SDecodeMms::asn1r_head_decode_len(ST_UCHAR *buf, ST_UINT *bytes)
{
	ST_INT head_len = 0;
	ST_UCHAR c;
	ST_UCHAR *p;

	p = buf;

	if ((c = *(p ++)) & 0x80)
	{
		if (c &= 0x7F)
		{
			*bytes = c+1;
			return GetIntWithBufLen(p,c);
		}
		else
		{
			head_len = -1;
			return head_len;
		}
	}
	else
	{
		head_len = (ST_INT)c;
		*bytes = 1;
	}

	return head_len;
}

ST_UINT16 SDecodeMms::asn1r_head_decode_id(ST_UCHAR *buf, ST_UINT *bytes)
{
	ST_UINT16 head_id = 0;
	ST_UCHAR c;
	ST_UINT16 ui16;
	ST_UCHAR *p;

	p = buf;
	c = *(p ++);

	if ((c &= 0x1F) == 0x1F)
	{
		if ((c = *(p++)) & 0x80)
		{
			if (*p & 0x80)
			{
				head_id = 0xFFFF;
				return (head_id);
			}
			ui16 = (ST_UINT16) (c & 0x7F) << 7;
			ui16 |= (*(p++) & 0x7F);
			head_id = ui16;
			*bytes = 3;
		}
		else
		{
			head_id = (ST_UINT16) c;
			*bytes = 2;
		}
	}
	else
	{
		head_id = (ST_UINT16) c;
		*bytes = 1;
	}
	return head_id;
}


int SDecodeMms::cosp_data_8823_cn_ac(BYTE *p8823Buffer,int iLen, BYTE* &pUserData,int &iUserDataLen)
{
#if 0
	int ret = 0;
	unsigned char * p;
	int off = 0;
	int len = 0;
	ST_UCHAR c;
	ST_UCHAR constr_elmnt;
	ST_UINT decode_id;
	ST_UINT id_bytes;
	ST_INT decode_len;
	ST_UINT len_bytes;
	pUserData =  NULL;
	iUserDataLen = 0;
//	off = ioff;
//	p = m_pBuf + off;
	p = p8823Buffer;
//	plen = iLen;

	decode_id = (ST_UINT)asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF)
	{
		NEW_MALFORMED_ID;
		return SDECODE_FMT_8823;
	}
	decode_len = (ST_INT)asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1)
	{
		NEW_MALFORMED_LEN;
		return SDECODE_FMT_8823;
	}
#ifdef SDECODE_OFFLINE
	SDecodeTree *p8823;
	if(m_pDecodeTreeRoot != NULL)
	{
		p8823 = m_pDecodeTreeRoot->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,
			id_bytes + len_bytes + decode_len,"ISO 8823 OSI Presentation Protocol");
		p8823 = p8823->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,
			id_bytes + len_bytes + decode_len,"CP-type");
	}
#endif

	while(off < iLen)
	{
		decode_id = (ST_UINT)asn1r_head_decode_id(p,&id_bytes);
		if (decode_id == 0xFFFF)
		{
			NEW_MALFORMED_ID;
			return SDECODE_FMT_8823;
		}
		c = *p;
		constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
		decode_len = (ST_INT)asn1r_head_decode_len(p + id_bytes,&len_bytes);
		if (decode_len == -1)
		{
			NEW_MALFORMED_LEN;
			return SDECODE_FMT_8823;
		}
		if (constr_elmnt)
		{
			p += id_bytes + len_bytes;
			off += id_bytes + len_bytes;
		}
		else
		{
			p += id_bytes + len_bytes + decode_len;
			off += id_bytes + len_bytes + decode_len;
		}
		if(c == 0x61)
		{
			break;
		}
	}
	if (off >= iLen)
	{
		NEW_MALFORMED_LEN;
		return SDECODE_FMT_8823;
	}

// 	InitDataBasepCap(&m_Osi_8823_DT.m_UserData,p,DATATYPE_UCHAR,off,off + decode_len,decode_len);
// 	InitDataBasepCap(&m_Osi_8823_DT.m_EncodedData,p,DATATYPE_UCHAR,off,off + decode_len,decode_len);

	decode_id = (ST_UINT)asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF)
	{
		NEW_MALFORMED_ID;
		return SDECODE_FMT_8823;
	}
	decode_len = (ST_INT)asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1)
	{
		NEW_MALFORMED_LEN;
		return SDECODE_FMT_8823;
	}

	p += id_bytes + len_bytes;
	off += id_bytes + len_bytes;
	len += id_bytes + len_bytes;

//	InitDataBasepCap(&m_Osi_8823_DT.m_ItemData,p,DATATYPE_UCHAR,off,off + decode_len,decode_len);

	decode_id = (ST_UINT)asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF)
	{
		NEW_MALFORMED_ID;
		return SDECODE_FMT_8823;
	}
	decode_len = (ST_INT)asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1)
	{
		NEW_MALFORMED_LEN;
		return SDECODE_FMT_8823;
	}

	p += id_bytes + len_bytes;
	off += id_bytes + len_bytes;
	len += id_bytes + len_bytes;
#ifdef SDECODE_OFFLINE
	SDecodeTree *pUsrNode;
	if(m_pDecodeTreeRoot != NULL)
	{
		pUsrNode = p8823->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,id_bytes + len_bytes,
			"user-data");
		pUsrNode->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,id_bytes,
			"presentation-context-identifier",SString::toFormat("%d",*p).data());
	}
#endif

//	InitDataBasepCap(&m_Osi_8823_DT.m_context_identifier,p,DATATYPE_BYTE,off,off + decode_len,decode_len);
	p += decode_len;
	off += decode_len;
	len += decode_len;

	decode_id = (ST_UINT)asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF)
	{
		NEW_MALFORMED_ID;
		return SDECODE_FMT_8823;
	}
	decode_len = (ST_INT)asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1)
	{
		NEW_MALFORMED_LEN;
		return SDECODE_FMT_8823;
	}

	p += id_bytes + len_bytes;
	off += id_bytes + len_bytes;
	len += id_bytes + len_bytes;

	//下层协议的数据
	pUserData = p;
	iUserDataLen = GetIntWithBufLen(p-len_bytes,len_bytes);

#ifdef SDECODE_OFFLINE
	if(m_pDecodeTreeRoot != NULL)
	{
		pUsrNode->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,iUserDataLen,
			"presentation-data-values");
	}
#endif

// 	m_Osi_8823_DT.m_iData_Value_Type = decode_id;
// 	InitDataBasepCap(&m_Osi_8823_DT.m_Data_Value,p,DATATYPE_UCHAR,off,off + decode_len,decode_len);
// 	m_Osi_8823_DT.m_bOsi8823_DT = TRUE;
// 
// 	m_Osi_8823_DT.m_NextOsiType = OSITYPE_8650_1;

//	ret = off;
#endif
	return 0;
}


int SDecodeMms::cosp_data_8650_1(BYTE *p8650Buffer,int iLen, BYTE* &pUserData,int &iUserDataLen)
{
#if 0
	int ret = 0;
	unsigned char * p;
	int off = 0;
	int ilen = 0;
	ST_UCHAR c;
	ST_UCHAR constr_elmnt;
	ST_UINT decode_id;
	ST_UINT id_bytes;
	ST_INT decode_len;
	ST_UINT len_bytes;

	//off = 0;
	p = p8650Buffer;

	c = *p;
	decode_id = (ST_UINT)asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF)
	{
		NEW_MALFORMED_ID;
		return SDECODE_FMT_8650;
	}
	decode_len = (ST_INT)asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1)
	{
		NEW_MALFORMED_LEN;
		return SDECODE_FMT_8650;
	}
#ifdef SDECODE_OFFLINE
	SDecodeTree *p8650;
	if(m_pDecodeTreeRoot != NULL)
	{
		p8650 = m_pDecodeTreeRoot->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,
			iLen,"ISO 8650 OSI Association Control Service");
	}
#endif

	switch (c)
	{
	case 0x60:
#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			p8650 = p8650->New(m_pPreprocessInfo->m_pViewBuffer,p+id_bytes+len_bytes-m_pPreprocessInfo->m_pViewBuffer,decode_len,"aarq");
		}
#endif
		break;
	case 0x61:
#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			p8650 = p8650->New(m_pPreprocessInfo->m_pViewBuffer,p+id_bytes+len_bytes-m_pPreprocessInfo->m_pViewBuffer,decode_len,"aare");
		}
#endif
		break;
	case 0x62:
#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			p8650 = p8650->New(m_pPreprocessInfo->m_pViewBuffer,p+id_bytes+len_bytes-m_pPreprocessInfo->m_pViewBuffer,decode_len,"rlrq");
		}
#endif
		break;
	case 0x63:
#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			p8650 = p8650->New(m_pPreprocessInfo->m_pViewBuffer,p+id_bytes+len_bytes-m_pPreprocessInfo->m_pViewBuffer,decode_len,"rlre");
		}
#endif
		break;
	default:
		break;
	}
	p += id_bytes + len_bytes;
	off += id_bytes + len_bytes;

	if (c == 0x62 || c == 0x63)
	{
		decode_id = (ST_UINT)asn1r_head_decode_id(p,&id_bytes);
		if (decode_id == 0xFFFF)
		{
			NEW_MALFORMED_ID;
			return SDECODE_FMT_8650;
		}
		c = *p;
		constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
		decode_len = (ST_INT)asn1r_head_decode_len(p + id_bytes,&len_bytes);
		if (decode_len == -1)
		{
			NEW_MALFORMED_LEN;
			return SDECODE_FMT_8650;
		}
		ilen = id_bytes + len_bytes;
#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			p8650 = p8650->New(m_pPreprocessInfo->m_pViewBuffer,p+ilen-m_pPreprocessInfo->m_pViewBuffer,decode_len,"Reason");
		}
#endif
	}

	while(off < iLen)
	{
		decode_id = (ST_UINT)asn1r_head_decode_id(p,&id_bytes);
		if (decode_id == 0xFFFF)
		{
			NEW_MALFORMED_ID;
			return SDECODE_FMT_8650;
		}
		c = *p;
		constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
		decode_len = (ST_INT)asn1r_head_decode_len(p + id_bytes,&len_bytes);
		if (decode_len == -1)
		{
			NEW_MALFORMED_LEN;
			return SDECODE_FMT_8650;
		}
		if (constr_elmnt)
		{
			p += id_bytes + len_bytes;
			off += id_bytes + len_bytes;
		}
		else
		{
			p += id_bytes + len_bytes + decode_len;
			off += id_bytes + len_bytes + decode_len;
		}
		if(c == 0xa0)
		{
			pUserData = p;
			iUserDataLen = decode_len;
#ifdef SDECODE_OFFLINE
			if(m_pDecodeTreeRoot != NULL)
			{
				p8650->New(m_pPreprocessInfo->m_pViewBuffer,pUserData-m_pPreprocessInfo->m_pViewBuffer,iUserDataLen,
					"MMS Data");
			}
#endif
			//m_Osi_8650_1.m_NextOsiType = OSITYPE_9506;
			break;
		}
	}
	//m_Osi_8650_1.m_bOsi8650_1 = TRUE;
	//ret = off;
	if (off >= iLen)
	{
		NEW_MALFORMED_LEN;
		return SDECODE_FMT_8650;
	}
#endif
	return 0;
}


int SDecodeMms::cosp_data_9506(BYTE *p9506Buffer,int iLen)
{
#if 0
	int ret = 0;
	BYTE * p;
	int off = 0;
	ST_UCHAR c;
	ST_UCHAR constr_elmnt;
	ST_UINT decode_id;
	ST_UINT id_bytes;
	ST_INT decode_len;
	ST_UINT len_bytes;
	int invokeId;
	int endoff = 0;

	//off = ioff;
	p = p9506Buffer;

	c = *p;
	decode_id = (ST_UINT)asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF)
	{
		NEW_MALFORMED_ID;
		return SDECODE_FMT_MMS;	
	}
	decode_len = (ST_INT)asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1)
	{
		NEW_MALFORMED_LEN;
		return SDECODE_FMT_MMS;	
	}
#ifdef SDECODE_OFFLINE
	SDecodeTree *pMMS;
	if(m_pDecodeTreeRoot != NULL)
	{
		pMMS = m_pDecodeTreeRoot->New(m_pPreprocessInfo->m_pViewBuffer,p9506Buffer-m_pPreprocessInfo->m_pViewBuffer,
			iLen,"ISO 9506 MMS");
	}
#endif
// 	m_Osi_9506.m_osi9506_type = c;
// 	m_9506Type = c;
	InitDataBasepCap(&m_Osi_9506.m_9506type,p,DATATYPE_UCHAR,off,off + id_bytes + len_bytes,id_bytes + len_bytes);
	InitDataBasepCap(&m_Osi_9506.m_ilen,p,DATATYPE_UCHAR,off,off + id_bytes + len_bytes + decode_len,id_bytes + len_bytes + decode_len);
	p += id_bytes + len_bytes;
	off += id_bytes + len_bytes;
	endoff = off + id_bytes + len_bytes + decode_len;

	switch (c)
	{
	case 0xa0:
	case 0xa1:
		{
#ifdef SDECODE_OFFLINE
			if(m_pDecodeTreeRoot != NULL)
			{
				pMMS = pMMS->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,
					decode_len,c==0xa0?"Confirmed-RequestPDU":"Confirmed-ResponsePDU");
			}
#endif
			decode_id = (ST_UINT)asn1r_head_decode_id(p,&id_bytes);
			if (decode_id == 0xFFFF)
			{
				NEW_MALFORMED_ID;
				return SDECODE_FMT_MMS;	
			}
			c = *p;
			constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
			decode_len = (ST_INT)asn1r_head_decode_len(p + id_bytes,&len_bytes);
			if (decode_len == -1)
			{
				NEW_MALFORMED_LEN;
				return SDECODE_FMT_MMS;	
			}
			p += id_bytes + len_bytes;
			off += id_bytes + len_bytes;
			if (!constr_elmnt)
			{
				invokeId = GetIntWithBufLen(p,decode_len);
#ifdef SDECODE_OFFLINE
				if(m_pDecodeTreeRoot != NULL)
				{
					pMMS->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,
						decode_len,"invokeID",SString::toFormat("%d",invokeId).data());
				}
#endif
				//InitDataBasepCap(&m_Osi_9506.m_InvokeID,p,DATATYPE_UINT32,off - id_bytes - len_bytes,off + decode_len,decode_len);
				//m_iInvokeID = GetData_Val(&m_Osi_9506.m_InvokeID);
				p += decode_len;
				off += decode_len;
			}

			decode_id = (ST_UINT)asn1r_head_decode_id(p,&id_bytes);
			if (decode_id == 0xFFFF)
			{
				NEW_MALFORMED_ID;
				return SDECODE_FMT_MMS;	
			}
			decode_len = (ST_INT)asn1r_head_decode_len(p + id_bytes,&len_bytes);
			if (decode_len == -1)
			{
				NEW_MALFORMED_LEN;
				return SDECODE_FMT_MMS;	
			}
			InitDataBasepCap(&m_Osi_9506.m_MmsInfoType,p,DATATYPE_UCHAR,off,off + id_bytes + len_bytes,id_bytes);
			if (id_bytes == 1)
			{
				m_Osi_9506.m_mmsinfo_type = *p;
				m_MmsType = *p;
			}
			else
			{
				m_Osi_9506.m_mmsinfo_type = *(p + 1);
				m_MmsType = *(p + 1);
			}
			InitDataBasepCap(&m_Osi_9506.m_InfoData,p,DATATYPE_UCHAR,off,off + id_bytes + len_bytes + decode_len,id_bytes + len_bytes + decode_len);
			//            InitDataBasepCap(&m_Osi_9506.m_InfoData,p,DATATYPE_UCHAR,off,m_BufLength,m_BufLength - off);
			if (m_Osi_9506.m_osi9506_type == 0xa0 &&
				(m_Osi_9506.m_mmsinfo_type == 0xa4 || m_Osi_9506.m_mmsinfo_type == 0xa5))
			{
				GetObjectItemID_Type(p);
			}
			p += id_bytes + len_bytes;
			off += id_bytes + len_bytes;
			p += decode_len;
			off += decode_len;
			//            p += 1;
			//            off += 1;

		}
		break;
	case 0xA3:
		InitDataBasepCap(&m_Osi_9506.m_InfoData,p,DATATYPE_UCHAR,off,m_BufLength,m_BufLength - off);
		break;
	default:
		break;
	}


	m_Osi_9506.m_bOsi9506 = TRUE;
	ret = endoff;
	return ret;
#endif
	return 0;
}

