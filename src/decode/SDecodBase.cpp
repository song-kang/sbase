/**
 *
 * �� �� �� : SDecoder.cpp
 * �������� : 2015-5-7 16:36
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : ��̫�����Ľ�����
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-5-7	�ۿ�������ļ�
 *
 **/

#include "SDecodeBase.h"

// 
// char SDecodeFieldViewInfo::GetDatSet_Type(DataBase_pCap * pDBpCap, char * strtype)
// {
// 	char ret;
// 	int bdatalen,blen;
// 
// 	switch (pDBpCap->m_DatSetType)
// 	{
// 	case DatSetType_INTEGER:
// 		if (strtype != NULL) sprintf(strtype,_T("INTEGER"));
// 		ret = GSE_CHTYPE_INT;
// 		break;
// 	case DatSetType_OSTRING:
// 		if (strtype != NULL) sprintf(strtype,_T("OSTRING"));
// 		ret = GSE_CHTYPE_STRING;
// 		break;
// 	case DatSetType_BITSTRING:
// 		if (strtype != NULL) sprintf(strtype,_T("BITSTRING"));
// 		bdatalen = (pDBpCap->m_Data_len - 1) * 8;
// 		blen = *pDBpCap->m_Data.pBYTE;
// 		if ((bdatalen - blen) == 2)
// 			ret = GSE_CHTYPE_DOUBLEBIT;
// 		else
// 			ret = GSE_CHTYPE_STRING;
// 		break;
// 	case DatSetType_UTCTIME:
// 		if (strtype != NULL) sprintf(strtype,_T("UTCTIME"));
// 		ret = GSE_CHTYPE_TIME;
// 		break;
// 	case DatSetType_BOOLEAN:
// 		if (strtype != NULL) sprintf(strtype,_T("BOOLEAN"));
// 		ret = GSE_CHTYPE_BOOL;
// 		break;
// 	case DatSetType_UNSIGNED:
// 		if (strtype != NULL) sprintf(strtype,_T("UNSIGNED"));
// 		ret = GSE_CHTYPE_UNSIGNED;
// 		break;
// 	case DatSetType_FLOAT:
// 		if (strtype != NULL) sprintf(strtype,_T("FLOAT"));
// 		ret = GSE_CHTYPE_FLOAT;
// 		break;
// 	case DatSetType_STRUCT:
// 		if (strtype != NULL) sprintf(strtype,_T("STRUCT"));
// 		ret = GSE_CHTYPE_STRUCT;
// 		break;
// 	default:
// 		if (strtype != NULL) sprintf(strtype,_T("The unknown"));
// 		ret = 0;
// 		break;
// 	}
// 	return ret;
// }
// 
// int SDecodeFieldViewInfo::GetDatSet_Data(DataBase_pCap *pDBpCap, char *datastr)
// {
// 	char tmp[5];
// 	int i,bdatalen,blen;
// 	uint8 j = 0;
// 	uint8 j1, j2;
// 	int idatset = 0;
// 	Event_T event_t;
// 	tm tm_sec;
// 	int ret = GSE_DBP_INVALID;
// 	switch (pDBpCap->m_DatSetType)
// 	{
// 	case DatSetType_INTEGER:
// 		{
// 			switch (pDBpCap->m_Data_len)
// 			{
// 			case 1:
// 				idatset = (int)*pDBpCap->m_Data.pBYTE;
// 				break;
// 			case 2:
// 				idatset = (int)ChangeByteOrder(pDBpCap->m_Data.pUINT16);
// 				break;
// 			case 4:
// 				idatset = (int)ChangeByteOrder(pDBpCap->m_Data.pUINT32);
// 				break;
// 			default:
// 				idatset = 0;
// 				break;
// 			}
// 			sprintf(datastr,"%d",idatset);
// 			break;
// 		}
// 	case DatSetType_OSTRING:
// 		{
// 			sprintf(datastr,"BYTES 0000-%04d:",pDBpCap->m_Data_len);
// 			for (i = 0;i < (int)pDBpCap->m_Data_len;i ++)
// 			{
// 				memset(tmp,0,5);
// 				sprintf(tmp," %02x",pDBpCap->m_Data.pBYTE[i]);
// 				strcat(datastr,tmp);
// 			}
// 			break;
// 		}
// 	case DatSetType_BITSTRING:
// 		{
// 			bdatalen = (pDBpCap->m_Data_len - 1) * 8;
// 			blen = *pDBpCap->m_Data.pBYTE;
// 			sprintf(datastr,"BITS 0000-%04d:",bdatalen - blen);
// 			for (i = 0;i < bdatalen - blen;i ++)
// 			{
// 				j = 0;
// 				j = (pDBpCap->m_Data.pBYTE[1 + i / 8] >> (7 - i % 8)) & 0x01;
// 				if (i == 0) j1 = j;
// 				if (i == 1) j2 = j;
// 				memset(tmp,0,5);
// 				sprintf(tmp," %x",j);
// 				strcat(datastr,tmp);
// 			}
// 			if ((bdatalen - blen) == 2)
// 			{
// 				if (j1 == 0 && j2 == 1)
// 				{
// 					strcat(datastr, _T("(OFF)"));
// 					ret = GSE_DBP_OFF;
// 				}
// 				else if (j1 == 1 && j2 == 0)
// 				{
// 					strcat(datastr, _T("(ON)"));
// 					ret = GSE_DBP_ON;
// 				}
// 				else if (j1 == 0 && j2 == 0)
// 				{
// 					strcat(datastr, _T("(INTERMEDIATE)"));
// 					ret = GSE_DBP_INTERMEDIATE;
// 				}
// 				else if (j1 == 1 && j2 == 1)
// 				{
// 					strcat(datastr, _T("(BAD)"));
// 					ret = GSE_DBP_BAD;
// 				}
// 			}
// 			break;
// 		}
// 	case DatSetType_UTCTIME:
// 		{
// 			event_t.m_sec = ChangeByteOrder(pDBpCap->m_Data.pUINT32);
// 			event_t.m_fraction = pDBpCap->m_Data.pBYTE[4] * 0x010000 + pDBpCap->m_Data.pBYTE[5] * 0x0100 + pDBpCap->m_Data.pBYTE[6];
// 			event_t.m_qflags = pDBpCap->m_Data.pBYTE[7];
// 			if(UtcValsToString(datastr,event_t.m_sec,event_t.m_fraction,event_t.m_qflags))
// 			{
// 				memset(&tm_sec,0,sizeof(tm));
// 				sprintf(datastr,"%04d-%02d-%02d %02d:%02d:%02d.%03d, qflags: 0x%02x",
// 					tm_sec.tm_year + 1900,tm_sec.tm_mon + 1,tm_sec.tm_mday,tm_sec.tm_hour,tm_sec.tm_min,tm_sec.tm_sec,event_t.m_fraction,
// 					event_t.m_qflags);
// 			}
// 			break;
// 		}
// 	case DatSetType_BOOLEAN:
// 		if ((BOOL)*pDBpCap->m_Data.pBOOL)
// 		{
// 			sprintf(datastr,_T("TRUE(ON)"));
// 			ret = GSE_DBP_ON;
// 		}
// 		else
// 		{
// 			sprintf(datastr,_T("FALSE(OFF)"));
// 			ret = GSE_DBP_OFF;
// 		}
// 		break;
// 	case DatSetType_UNSIGNED:
// 		sprintf(datastr,_T("UNSIGNED"));
// 		break;
// 	case DatSetType_FLOAT:
// 		{
// 			float ds = 0;
// 			ChangeByteOrder((unsigned int *)(pDBpCap->m_Data.pBYTE + 1),(unsigned int *)&ds);
// 			sprintf(datastr,"%5.3f",ds);
// 		}
// 		break;
// 	case DatSetType_STRUCT:
// 		// 		sprintf(datastr,"STRUCT");
// 		break;
// 	default:
// 		sprintf(datastr,_T("The unknown"));
// 		break;
// 	}
// 	return ret;
// }

const char* SDecode_GetProtocolName(eSDecodeProtType idx)
{
	static char invalid[32];
	if(idx < SDECODE_PROT_UNKNOWN || idx >= SDECODE_PROT_COUNT)
	{
		sprintf(invalid,"invalid:%d",(int)idx);
		return invalid;
	}
	return g_sDecodeProtocolName[(int)idx];
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  SDecodeTcpSession
// ��    ��:  �ۿ���
// ����ʱ��:  2015-5-7 17:14
// ��    ��:  TCPЭ�飨������TCP���ϲ�Э�飩�ĻỰ��
//////////////////////////////////////////////////////////////////////////
SDecodeTcpSession::SDecodeTcpSession()
{
	m_iTcpStatus		= SDECODE_TCP_UNKNOWN;
	m_iConnectSOC		= 0;//�������ӵ�ʱ�䣨�����룩
	m_iDisconnectSOC	= 0;//�������ӵ�ʱ�䣨�����룩
	m_pStatusData		= NULL;//״̬���ݣ�����ʱ�ͷ�
	m_iStatusLen		= 0;//״̬���ݳ���
	memset(m_pBufferData,0,sizeof(m_pBufferData));
	memset(m_iBufferLen,0,sizeof(m_iBufferLen));
	memset(m_iBufferMaxSize,0,sizeof(m_iBufferMaxSize));
	m_ComplexSet.setAutoDelete(true);
	m_bUnknownSC		= true;//�Ƿ�ȷ��˭�Ƿ���ˣ�˭�ǿͻ��ˣ����δ�õ��������ֱ���ʱΪtrue����ʾ����˺Ϳͻ��˿��ܲ���ȷ���µģ�
	m_bLastFrameS2C		= false;//���һ�α��ĵķ����Ƿ�ΪS->C?
	m_iTotalFrames		= 0;
	m_iTotalBytes		= 0;
	m_iTotalFrames_G	= 0;
	m_iTotalBytes_G		= 0;
	m_iLastSOC			= 0;
	m_iNoComplexCnt		= 0;
	m_pLastComplexSet	= NULL;
	m_pFreeSessionstatusFun = NULL;
}

SDecodeTcpSession::~SDecodeTcpSession()
{
	int i;
	for(i=0;i<SDECODE_BUF_COUNT;i++)
	{
		if(m_pBufferData[i] != NULL)
		{
			m_iBufferLen[i] = 0;
			delete[] m_pBufferData[i];
			m_pBufferData[i] = NULL;
		}
	}
// 	if(m_pBufferDataC2S != NULL)
// 	{
// 		m_iBufferLenC2S = 0;
// 		delete[] m_pBufferDataC2S;
// 		m_pBufferDataC2S = NULL;
// 	}
// 	if(m_pBufferDataS2C != NULL)
// 	{
// 		m_iBufferLenS2C = 0;
// 		delete[] m_pBufferDataS2C;
// 		m_pBufferDataC2S = NULL;
// 	}
	if(m_pStatusData != NULL)
	{
		m_iStatusLen = 0;
		if(m_pFreeSessionstatusFun != NULL)
			m_pFreeSessionstatusFun(m_pStatusData);
		delete[] m_pStatusData;
		m_pStatusData = NULL;
	}
}


//////////////////////////////////////////////////////////////////////////
// ��    ��:  ���C2S�������ݵ����ݻ�����
// ��    ��:  �ۿ���
// ����ʱ��:  2015-5-10 22:59
// ����˵��:  @p��������
//         :  @l���ݳ���
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SDecodeTcpSession::AddToDatBuf(eSDecodeBufType buf_type,BYTE* p,int l)
{
	if(m_pBufferData[buf_type] == NULL)
	{
		m_pBufferData[buf_type] = new BYTE[l];
		memcpy(m_pBufferData[buf_type],p,l);
		m_iBufferLen[buf_type] = m_iBufferMaxSize[buf_type] = l;
	}
	else
	{
		int size = (m_iBufferLen[buf_type] + l);
		if(size > m_iBufferMaxSize[buf_type])
		{
			size = size << 1;
			BYTE *pNew = new BYTE[size];
			memcpy(pNew,m_pBufferData[buf_type],m_iBufferLen[buf_type]);
			m_iBufferMaxSize[buf_type] = size;
			BYTE *pOld = m_pBufferData[buf_type];
			m_pBufferData[buf_type] = pNew;
			delete[] pOld;
		}
		memcpy(m_pBufferData[buf_type]+m_iBufferLen[buf_type],p,l);
		m_iBufferLen[buf_type] += l;
	}
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ɾ��C2S������ǰ��ָ�����ֽ�
// ��    ��:  �ۿ���
// ����ʱ��:  2015-5-10 23:27
// ����˵��:  @sizeΪ��ɾ���ĳ���
// �� �� ֵ:  true�ɹ���falseʧ��
//////////////////////////////////////////////////////////////////////////
bool SDecodeTcpSession::RemoveDatBufFront(eSDecodeBufType buf_type,int size)
{
	if(size <= 0 || size > m_iBufferLen[buf_type])
		return false;
	if(size == m_iBufferLen[buf_type])
	{
		m_iBufferLen[buf_type] = 0;
		return true;
	}
	memmove(m_pBufferData[buf_type],m_pBufferData[buf_type]+size,m_iBufferLen[buf_type]-size);
	m_iBufferLen[buf_type] -= size;
	return true;
}

#ifdef SDECODE_OFFLINE
SDecodeComplexFrameSet* SDecodeTcpSession::SearchComplexSet(int idx)//�������ϱ��ļ�
{
	unsigned long pos;
	SDecodeComplexFrameSet *p = m_ComplexSet.FetchFirst(pos);
	while(p)
	{
		if(p->SearchIdx(idx))
			return p;
		p = m_ComplexSet.FetchNext(pos);
	}
	return NULL;
}
SDecodeComplexFrameSet* SDecodeTcpSession::NewComplexSet(int idx)//�½����ϱ��ļ�
{
	SDecodeComplexFrameSet *p = new SDecodeComplexFrameSet();
	p->m_iMaxIdx = p->m_iMinIdx = idx;
	p->m_FrameIdx.append(idx);
	m_ComplexSet.append(p);
	return p;
}
#endif

//////////////////////////////////////////////////////////////////////////
// ��    ��:  SDecodeTree
// ��    ��:  �ۿ���
// ����ʱ��:  2015-5-7 17:38
// ��    ��:  ����Ľ����
//////////////////////////////////////////////////////////////////////////
SDecodeTree::SDecodeTree()
{
	m_Childs.setShared(false);
	m_Childs.setAutoDelete(false);
}

SDecodeTree::~SDecodeTree()
{
	m_Childs.setAutoDelete(false);
	m_Childs.clear();
}

void SDecodeTree::ClearChild()
{
	unsigned long pos;
	SDecodeTree *p0,*p = m_Childs.FetchFirst(pos);
	while(p)
	{
		if(p->m_Childs.count() > 0)
			p->ClearChild();
		p0 = m_Childs.FetchNext(pos);
		m_Childs.remove(p);
		m_pTempTree->append(p);
		p = p0;
	}
}

SDecodeTree* SDecodeTree::New(BYTE *pBuf,int iOffset,int iBytes,const char* sName,const char* sVal,const char* sRemark)
{
// 	if(pBuf == m_pFrameBuffer &&  m_iDataOffset	== iOffset && m_iDataBytes	== iBytes)
// 	{
// 		//�뱾�ڵ���ͬ���������
// 		return this;
// 	}
	if(iBytes < 0)
		iBytes = 0;
	SDecodeTree *p = m_pTempTree->at(0);
	if(p == NULL)
	{
		p = new SDecodeTree();
		p->m_pTempTree = m_pTempTree;
	}
	else
		m_pTempTree->remove(0);
	m_Childs.append(p);
	p->m_sNodeName		= sName;
	p->m_sNodeValue		= sVal;
	p->m_sNodeRemark	= sRemark;
	p->m_pFrameBuffer	= pBuf;
	p->m_iDataOffset	= iOffset;
	p->m_iDataBytes		= iBytes;
	return p;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  SDecodeBase
// ��    ��:  �ۿ���
// ����ʱ��:  2015-5-7 17:00
// ��    ��:  ����Э�����
//////////////////////////////////////////////////////////////////////////
SDecodeBase::SDecodeBase()
{
	m_LastEvent.m_iProtType = m_ProtType = SDECODE_PROT_UNKNOWN;//Ӧ�������๹��ʱָ����Ӧ������
	m_TcpSessions.setAutoDelete(false);
	m_TcpSessions.setShared(false);//Լ������ʵ��Ϊ���л������������ڲ������
	m_SessionsList.setAutoDelete(false);
	m_SessionsList.setShared(false);
	m_RemovedSessions.setAutoDelete(true);
	m_RemovedSessions.setShared(false);
	m_TcpSessions.SetHashSize(512);//ɢ�д�С
	m_pDecodeTreeRoot = NULL;
	m_pEventCallbackFun =  NULL;
	m_pEventCallbackParam = NULL;
	m_pLastSession = NULL;
}

SDecodeBase::~SDecodeBase()
{

}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡTCP�Ự��ǵ�ɢ��ֵ
// ��    ��:  �ۿ���
// ����ʱ��:  2015-5-8 18:58
// ����˵��:  
// �� �� ֵ:  
//////////////////////////////////////////////////////////////////////////
int SDecodeBase::TcpSession_Key(void *cbParam,void *pItem)
{
	return ((SDecodeTcpSession*)pItem)->m_iHashKey;
	/*
	SDecodeBase *pThis = (SDecodeBase*)cbParam;
	SDecodeTcpSession *pSession = (SDecodeTcpSession*)pItem;
	return (int)(((SDECODE_IPV4*)&pSession->m_iServerIp)->GetIpSector4()+((SDECODE_IPV4*)&pSession->m_iClientIp)->GetIpSector4()+pSession->m_iServerPort+pSession->m_iClientPort);
	*/
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ���ȱȽ������Ự�Ƿ���ͬ
// ��    ��:  �ۿ���
// ����ʱ��:  2015-5-8 18:58
// ����˵��:  
// �� �� ֵ:  
//////////////////////////////////////////////////////////////////////////
bool SDecodeBase::TcpSession_Compare(void *cbParam,void *pItem1,void *pItem2)
{
	SDecodeBase *pThis = (SDecodeBase*)cbParam;
	SDecodeTcpSession *pSession1 = (SDecodeTcpSession*)pItem1;
	SDecodeTcpSession *pSession2 = (SDecodeTcpSession*)pItem2;
	return memcmp(&pSession1->m_iServerIp,&pSession2->m_iServerIp,12) == 0;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ����TCP�Ự��Ϣ
// ��    ��:  �ۿ���
// ����ʱ��:  2015-5-8 18:35
// ����˵��:  @socΪ����ʱ��(������)
//         :  @bCreateNew��ʾ�粻����ʱ�Ƿ񴴽��µĻỰʵ��,true��ʾ������false��ʾ������
// �� �� ֵ:  SDecodeTcpSession*Ϊ�Ựָ�룬NULL��ʾ������
//////////////////////////////////////////////////////////////////////////
SDecodeTcpSession* SDecodeBase::SearchTcpSession(int soc,bool bCreateNew)
{
	unsigned long pos;
	SDECODE_IP_HEADER *pIp = m_pPreprocessInfo->m_pIp;
	SDECODE_TCP_HEADER *pTcp = m_pPreprocessInfo->m_pTcp;
	//MEMO: ɢ�м�ֵ��Ϊʹɢ�о��ȣ��˿ں�תΪ�����ֽ�˳����㣬����ȷ�Ƚ�ʱ��ֱ��ʹ�������ֽ�˳��Ƚ� [2015-5-9 21:51 �ۿ���]
	int iSessionKey =	((SDECODE_IPV4*)&pIp->src_ip)->GetIpSector4()+((SDECODE_IPV4*)&pIp->dst_ip)->GetIpSector4()+
						ntohs(pTcp->src_port)+ntohs(pTcp->dst_port);
	SDecodeTcpSession *p = m_TcpSessions.FetchFirst(iSessionKey,pos);
	while(p)
	{
		if(p->m_iServerIp == pIp->src_ip && p->m_iClientIp == pIp->dst_ip && p->m_iServerPort == pTcp->src_port && p->m_iClientPort == pTcp->dst_port)
		{
			//S->C
			p->m_bLastFrameS2C = true;
			return p;
		}
		else if(p->m_iServerIp == pIp->dst_ip && p->m_iClientIp == pIp->src_ip && p->m_iServerPort == pTcp->dst_port && p->m_iClientPort == pTcp->src_port)
		{
			//C->S
			p->m_bLastFrameS2C = false;
			return p;
		}
		p = m_TcpSessions.FetchNext(iSessionKey,pos);
	}
	if(p == NULL)
	{
		if(!bCreateNew)
			return NULL;//MEMO: �������»Ự [2015-5-9 21:54 �ۿ���]
		p = new SDecodeTcpSession();
		p->m_iLastClientSeqNo = 0;
		p->m_iLastServerSeqNo = 0;
		p->m_iConnectSOC = soc;
		p->m_iHashKey = iSessionKey;
		if(pTcp->IsSetFlagSyn())
		{
			//���������Է��ͷ�Ϊ�ͻ��ˣ����շ�Ϊ����ˣ�C->S
			p->m_bUnknownSC = false;
			p->m_bLastFrameS2C = false;
			p->m_iServerIp = pIp->dst_ip;
			p->m_iClientIp = pIp->src_ip;
			p->m_iServerPort = pTcp->dst_port;
			p->m_iClientPort = pTcp->src_port;
			p->m_iTcpStatus = SDECODE_TCP_CONNECTING;
		}
		else
		{
			//��һ��δ�յ������������Զ˿ں�С��Ϊ�����
			p->m_bUnknownSC = true;
			if(ntohs(pTcp->src_port) < ntohs(pTcp->dst_port))
			{
				//���ͷ�Ϊ�����
				p->m_bLastFrameS2C = true;
				p->m_iServerIp = pIp->src_ip;
				p->m_iClientIp = pIp->dst_ip;
				p->m_iServerPort = pTcp->src_port;
				p->m_iClientPort = pTcp->dst_port;

			}
			else
			{
				//���շ�Ϊ�����
				p->m_bLastFrameS2C = false;
				p->m_iServerIp = pIp->dst_ip;
				p->m_iClientIp = pIp->src_ip;
				p->m_iServerPort = pTcp->dst_port;
				p->m_iClientPort = pTcp->src_port;
			}
		}
		//��ӵ��Ự�б�
		m_TcpSessions.append(p);
		m_SessionsList.append(p);
	}
	else
	{
		if(p->m_iTcpStatus >= SDECODE_TCP_DISCONNECTING)
		{
			if(pTcp->IsSetFlagAck())
			{
				//��Ϊͬ���ĻỰ���ؽ���
				p->m_iTcpStatus = SDECODE_TCP_CONNECTING;
				p->m_iDisconnectSOC = 0;
			}
			else
			{
				//�������ӹرչ����е��������ģ�һ��Ϊ�Ͽ������ֹ��̣������������е����ӶϿ�ʱ�������ŵ����ֶϿ��ģ�
				//ά�ֻỰ���رյ�״̬
			}
		}
	}
	if(p->m_iTcpStatus == 1)
	{
		//connecting
		if(p->m_iTotalFrames > 0 && pTcp->IsSetFlagAck())
		{
			p->m_iTcpStatus = SDECODE_TCP_CONNECTED;
		}
	}
	if(pTcp->IsSetFlagFin())
	{
		p->m_iDisconnectSOC = soc;
		p->m_iTcpStatus = SDECODE_TCP_DISCONNECTING;
	}
	else if(p->m_iTcpStatus == SDECODE_TCP_DISCONNECTING && pTcp->IsSetFlagAck())
	{
		p->m_iTcpStatus = SDECODE_TCP_DISCONNECTED;
	}
	p->m_iTotalFrames ++;
	if((p->m_iTotalFrames & 0x40000000) != 0)
	{
		p->m_iTotalFrames_G ++ ;
		p->m_iTotalFrames = 0;
	}
	p->m_iTotalBytes += m_pPreprocessInfo->m_iFrameBytes;
	if((p->m_iTotalBytes & 0x40000000) != 0)
	{
		p->m_iTotalBytes_G ++ ;
		p->m_iTotalBytes -= 0x40000000;
	}
	p->m_iLastSOC = soc;
	m_pLastSession = p;
	return p;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ����һ֡����
// ��    ��:  �ۿ���
// ����ʱ��:  2015-5-9 22:27
// ����˵��:  @soc��ʾ����ʱ�꣨�룩
//         :  @nsec��ʾ����ʱ�꣨���룩
// �� �� ֵ:  int,0��ʾ����ɹ�����ӦeSDecodeFmtError
//////////////////////////////////////////////////////////////////////////
int SDecodeBase::DecodeFrame(int soc,int nsec)
{
	return 0;
}

void SDecodeBase::AfterDecode()
{
#ifdef SDECODE_OFFLINE
	if(m_pLastSession != NULL)
	{
		if(m_pLastSession->m_iNoComplexCnt > 0)
		{
			if(m_pLastSession->m_pLastComplexSet != NULL)
			{
				if(m_pLastSession->m_pLastComplexSet->m_FrameIdx.count() <= 1)
					m_pLastSession->m_ComplexSet.remove(m_pLastSession->m_pLastComplexSet);//ֻ��һ�����ϱ��ĵ��������Ϊ����֡���ķ��˶������㣬�����д���㡢��ʾ�㡢Ӧ�ò���������������������Ϊ���������ĸ��ϱ���
				m_pLastSession->m_pLastComplexSet = NULL;
			}
		}
		else
		{
			if(m_pLastSession->m_pLastComplexSet == NULL)
				m_pLastSession->m_pLastComplexSet = m_pLastSession->NewComplexSet(*m_piCurrFrameIdx);
			else
				m_pLastSession->m_pLastComplexSet->AddIdx(*m_piCurrFrameIdx);
		}
	}
#endif
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ���������Ч��TCP����
// ��    ��:  �ۿ���
// ����ʱ��:  2015-5-10 12:56
// ����˵��:  @soc��ʾ��ǰʱ��
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SDecodeBase::RemoveInvalidTcpSession(int soc)
{
	//ѡ������ͷ��ϴ�ѡȡ������ɾ���ĻỰ
	m_RemovedSessions.clear();

	//��ѡȡ�´���ɾ���ĻỰ������ؽ��д��л���������
	unsigned long pos;
	SDecodeTcpSession *pRm;
	SDecodeTcpSession *pSession = m_SessionsList.FetchFirst(pos);
	while(pSession)
	{
		pRm = NULL;
		if(pSession->m_iTcpStatus >= SDECODE_TCP_DISCONNECTING)
			pRm = pSession;
		else if(pSession->m_iLastSOC > 0 && abs(soc - pSession->m_iLastSOC) > 300)//5��������û�����ݽ�����Ϊ�����ѶϿ�
			pRm = pSession;
		pSession = m_SessionsList.FetchNext(pos);
		if(pRm != NULL)
		{
			m_TcpSessions.remove(pRm);
			m_SessionsList.remove(pRm);
			m_RemovedSessions.append(pRm);
		}
	}
}


void SDecodeBase::InitFieldViewInfo(SDecodeFieldViewInfo * pViewInfo, unsigned char * pbuf, unsigned char datatype, unsigned short dataoff, unsigned short dataend, unsigned short datalen, unsigned char datsettype, bool inval)
{
	memset(pViewInfo,0,sizeof(SDecodeFieldViewInfo));
	pViewInfo->m_DatSetType = datsettype;
	pViewInfo->m_Data_off = dataoff;
	pViewInfo->m_Data_End = dataend;
	pViewInfo->m_Data_len = datalen;
	pViewInfo->m_DataType = datatype;
	if (inval)
	{
		pViewInfo->m_Data.pBYTE = NULL;
		return;
	}
	switch (pViewInfo->m_DataType)
	{
	case DATATYPE_UCHAR:
		pViewInfo->m_Data.pUCHAR = (unsigned char *)pbuf;
		break;
	case DATATYPE_UINT32:
		pViewInfo->m_Data.pUINT32 = (unsigned int *)pbuf;
		if (pViewInfo->m_Data_len == 2)
		{
			pViewInfo->m_DataType = DATATYPE_UINT16;
		}
		else if (pViewInfo->m_Data_len == 1)
		{
			pViewInfo->m_DataType = DATATYPE_BYTE;
		}
		break;
	case DATATYPE_UINT16:
		pViewInfo->m_Data.pUINT16 = (unsigned short *)pbuf;
		if (pViewInfo->m_Data_len == 1)
		{
			pViewInfo->m_DataType = DATATYPE_BYTE;
		}
		break;
	case DATATYPE_INT32:
		pViewInfo->m_Data.pINT32 = (int *)pbuf;
		if (pViewInfo->m_Data_len == 2)
		{
			pViewInfo->m_DataType = DATATYPE_INT16;
		}
		else if (pViewInfo->m_Data_len == 1)
		{
			pViewInfo->m_DataType = DATATYPE_BYTE;
		}
		break;
	case DATATYPE_INT16:
		pViewInfo->m_Data.pINT16 = (short *)pbuf;
		if (pViewInfo->m_Data_len == 1)
		{
			pViewInfo->m_DataType = DATATYPE_BYTE;
		}
		break;
	case DATATYPE_FLOAT:
		pViewInfo->m_Data.pFLOAT = (float *)pbuf;
		break;
	case DATATYPE_BOOL:
		pViewInfo->m_Data.pBOOL = (unsigned char *)pbuf;
		break;
	case DATATYPE_BYTE:
		pViewInfo->m_Data.pBYTE = (unsigned char *)pbuf;
		break;
	case DATATYPE_T:
		pViewInfo->m_Data.pBYTE = (unsigned char *)pbuf;
		break;
	case DATATYPE_SVDATA:
		pViewInfo->m_Data.pBYTE = (unsigned char *)pbuf;
		break;
	case DATATYPE_ASNLEN:
		pViewInfo->m_Data.pBYTE = (unsigned char *)pbuf;
		break;
	default:
		// 		pViewInfo->m_Invalidity = true;
		break;
	}
}

