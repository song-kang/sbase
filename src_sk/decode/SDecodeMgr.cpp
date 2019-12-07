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

#include "SDecodeMgr.h"
#include "SDecodeMms.h"
#include "SDecodeIp.h"
#include "SDecodeOth.h"

//////////////////////////////////////////////////////////////////////////
// ��    ��:  SDecodeMgr
// ��    ��:  �ۿ���
// ����ʱ��:  2015-5-7 16:39
// ��    ��:  �����������ά��һ������ʵ��
//////////////////////////////////////////////////////////////////////////
SDecodeMgr::SDecodeMgr()
{
	memset(m_DecodeProt,0,sizeof(m_DecodeProt));
	m_iLastCheckRmSOC = 0;
	//��ʼ����Э��
	m_DecodeProt[SDECODE_PROT_ETH2]		= new SDecodeEth2();
	m_DecodeProt[SDECODE_PROT_ARP]		= new SDecodeArp();
	m_DecodeProt[SDECODE_PROT_IP]		= new SDecodeIp();
	m_DecodeProt[SDECODE_PROT_TCP]		= new SDecodeTcp();
	m_DecodeProt[SDECODE_PROT_UDP]		= new SDecodeUdp();
	m_DecodeProt[SDECODE_PROT_ICMP]		= new SDecodeIcmp();
	m_DecodeProt[SDECODE_PROT_MMS]		= new SDecodeMms();
	m_pDecodeTreeRoot = NULL;
	int i;
	for(i=0;i<SDECODE_PROT_COUNT;i++)
	{
		if(m_DecodeProt[i] != NULL)
		{
			m_DecodeProt[i]->SetPreprocessInfoPtr(&m_PreInfo);
#ifdef SDECODE_OFFLINE
			m_DecodeProt[i]->m_piCurrFrameIdx = &m_iFrameIndex;
#endif
		}
	}
	m_pEventCallbackFun =  NULL;
	m_pEventCallbackParam = NULL;

}

SDecodeMgr::~SDecodeMgr()
{

}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ʼ���������
// ��    ��:  �ۿ���
// ����ʱ��:  2015-5-7 16:43
// ����˵��:  @bRemoveTcpSessionWhenDiscon��ʾ�Ƿ���TCP�Ự�Ͽ��Ժ����ٻỰʵ�������߷���ʱΪfalse,���߷���ʱΪtrue��
//         :  @sAttrΪ��ʼ�������ַ�����ʹ��ָʾ������Щ����Э�飨�������壩
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
bool SDecodeMgr::Start(bool bRemoveTcpSessionWhenDiscon,SString sAttr)
{
	//m_bRemoveTcpSessionWhenDiscon = bRemoveTcpSessionWhenDiscon;
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ֹͣ���룬��������ݴ���Ϣ���Ự
// ��    ��:  �ۿ���
// ����ʱ��:  2015-5-7 16:48
// ����˵��:  void
// �� �� ֵ:  true��ʾ�ɹ���false��ʾʧ��
//////////////////////////////////////////////////////////////////////////
bool SDecodeMgr::Stop()
{
	return true;
}


//////////////////////////////////////////////////////////////////////////
// ��    ��:  Ԥ���룬��ñ��ĵ����ͣ��Լ����ӦЭ����Ч���ݵ���ʼλ�úͳ���
// ��    ��:  �ۿ���
// ����ʱ��:  2015-5-8 14:10
// ����˵��:  @soc��ʾ��ǰ���ĵ�ʱ��
//         :  @pBuffer��ʾ���Ļ�����
//         :  @iLen��ʾ���ĳ���
//         :  @iLen��ʾ���ĳ���
// �� �� ֵ:  eSDecodeProtType����Ӧ��Э������
//////////////////////////////////////////////////////////////////////////
eSDecodeProtType SDecodeMgr::PreDecode(int soc,BYTE *pBuffer,int iLen)
{
	SDECODE_MAC_HEADER *pMac = (SDECODE_MAC_HEADER*)pBuffer;
	unsigned short mac_type = pMac->type;
	int pos=sizeof(SDECODE_MAC_HEADER);
	int rest_len;
	eSDecodeProtType prot_type;
	memset(&m_PreInfo,0,sizeof(m_PreInfo));
	m_PreInfo.m_pViewBuffer = pBuffer;
	m_PreInfo.m_pFrameBuffer = pBuffer;
	m_PreInfo.m_iViewBytes = m_PreInfo.m_iFrameValidBytes = m_PreInfo.m_iFrameBytes = iLen;
#ifdef SDECODE_OFFLINE
	SDecodeTree *pEth2,*p,*p2;
	if(m_pDecodeTreeRoot != NULL)
	{
		m_pDecodeTreeRoot->m_sNodeName = "Frame";
		m_pDecodeTreeRoot->m_sNodeValue.sprintf("%d",m_iFrameIndex);
		m_pDecodeTreeRoot->m_pFrameBuffer = pBuffer;
		m_pDecodeTreeRoot->m_iDataOffset = 0;
		m_pDecodeTreeRoot->m_iDataBytes = iLen;
		pEth2 = m_pDecodeTreeRoot->New(pBuffer,0,14,"Ethernet II","","");
		p = pEth2->New(pBuffer,0,6,"Destination",SString::toFormat("%02X:%02X:%02X:%02X:%02X:%02X",pBuffer[0],pBuffer[1],pBuffer[2],pBuffer[3],pBuffer[4],pBuffer[5]).data(),"Ŀ��MAC��ַ");
		p = pEth2->New(pBuffer,6,6,"Source",SString::toFormat("%02X:%02X:%02X:%02X:%02X:%02X",pBuffer[6],pBuffer[7],pBuffer[8],pBuffer[9],pBuffer[10],pBuffer[11]).data(),"ԴMAC��ַ");
	}
#endif
#ifdef _SKT_NUMBER_SWAP
	if(mac_type == 0x0081)
#else
	if(mac_type == 0x8100)
#endif
	{
		//��VLAN
		m_PreInfo.m_pVlan = (SDECODE_VLAN_HEADER*)(&pBuffer[pos]);
		mac_type = m_PreInfo.m_pVlan->prot;
#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			pEth2->New(pBuffer,12,2,"Type","VLAN","");
			p = m_pDecodeTreeRoot->New(pBuffer,pos,4,"802.1Q Virtual LAN","","");
			p->New(pBuffer,pos,2,"Priority",SString::toFormat("%d",m_PreInfo.m_pVlan->GetPriority()).data(),"���ȼ�");
			p->New(pBuffer,pos,2,"CFI",SString::toFormat("%d",m_PreInfo.m_pVlan->GetCFI()).data(),"");
			p->New(pBuffer,pos,2,"ID",SString::toFormat("%d",m_PreInfo.m_pVlan->GetVLanNum()).data(),"VLAN��");
		}
#endif
		pos += 4;
	}
#ifdef _SKT_NUMBER_SWAP
	switch(ntohs(mac_type))
#else
	switch(mac_type)
#endif
	{
	case 0x0008://IPЭ��
		{
			SDECODE_IP_HEADER *pIp = (SDECODE_IP_HEADER*)&pBuffer[pos];
			m_PreInfo.m_pIp = pIp;
			//IP�㳤��У��
			rest_len = iLen - pos;
			if(rest_len < sizeof(SDECODE_IP_HEADER))
			{
#ifdef SDECODE_OFFLINE
				if(m_pDecodeTreeRoot != NULL)
				{
					m_pDecodeTreeRoot->NewMalformed(pBuffer,pos,iLen-pos);
				}
#endif
				m_PreInfo.m_iFmtErrorCode = SDECODE_FMT_IP;//IP�㳤�Ȳ���ȷ
				break;
			}
#ifdef SDECODE_OFFLINE
			if(m_pDecodeTreeRoot != NULL)
			{
				p = pEth2->New(pBuffer,12,2,"Type","IP","");
				p = m_pDecodeTreeRoot->New(pBuffer,pos,pIp->GetIpHeadLen(),"Internet Protocol","","");
				p2 = p->New(pBuffer,pos,1,"Version",SString::toFormat("%d",pIp->GetIpVer()).data(),"�汾��");
				p2 = p->New(pBuffer,pos+1,1,"Header length",SString::toFormat("%d",pIp->GetIpHeadLen()).data(),"IPͷ������");
				p2 = p->New(pBuffer,pos+2,2,"Differentiated Services Field",SString::toFormat("%d",pIp->tos ).data(),"");
				p2 = p->New(pBuffer,pos+4,2,"Total Length",SString::toFormat("%d",pIp->len ).data(),"�ܳ���");
				p2 = p->New(pBuffer,pos+6,2,"Identification",SString::toFormat("%d",pIp->id).data(),"");
				//p2 = SDecodeTree::New(p,"Flags",SString::toFormat("%d",pIp->id).data(),"");
				//...
				p2 = p->New(pBuffer,pos+12,4,"Source",pIp->GetSrcIpStr().data(),"");
				p2 = p->New(pBuffer,pos+16,4,"Destination",pIp->GetDstIpStr().data(),"");
			}
#endif
			int ip_len = ntohs(pIp->len);
			if(rest_len != ip_len)
			{
				if(ip_len > rest_len )
				{
#ifdef SDECODE_OFFLINE
					if(m_pDecodeTreeRoot != NULL)
					{
						m_pDecodeTreeRoot->NewMalformed(pBuffer,pos,iLen-pos);
					}
#endif
					m_PreInfo.m_iFmtErrorCode = SDECODE_FMT_IP;//IP�㳤�Ȳ���ȷ
					break;
				}
				else
					m_PreInfo.m_iFrameValidBytes -= (rest_len - ip_len);
			}
			if(pIp->ptl == 0x06)
			{
				//tcp
				pos += pIp->GetIpHeadLen();
				SDECODE_TCP_HEADER *pTcp = (SDECODE_TCP_HEADER*)&pBuffer[pos];
				m_PreInfo.m_pTcp = pTcp;
				rest_len = iLen - pos;
				if(rest_len < sizeof(SDECODE_TCP_HEADER))
				{
#ifdef SDECODE_OFFLINE
					if(m_pDecodeTreeRoot != NULL)
					{
						m_pDecodeTreeRoot->NewMalformed(pBuffer,pos,iLen-pos);
					}
#endif
					m_PreInfo.m_iFmtErrorCode = SDECODE_FMT_TCP;//TCP�㳤�Ȳ���ȷ
					break;
				}
#ifdef SDECODE_OFFLINE
				if(m_pDecodeTreeRoot != NULL)
				{
					p = m_pDecodeTreeRoot->New(pBuffer,pos,pTcp->GetTcpHeadLen(),"Transmission Control Protocol ",
						SString::toFormat("Src Port: %d, Dst Port: %d, Seq: %d, Ack: %d, Len: %d",ntohs(pTcp->src_port),ntohs(pTcp->dst_port),ntohl(pTcp->seq_num),ntohl(pTcp->ack_num),rest_len ).data(),"");
					p2 = p->New(pBuffer,pos+0,2,"Source port",SString::toFormat("%d",ntohs(pTcp->src_port)).data(),"Դ�˿ں�");
					p2 = p->New(pBuffer,pos+2,2,"Destination port",SString::toFormat("%d",ntohs(pTcp->dst_port)).data(),"Ŀ��˿ں�");
					p2 = p->New(pBuffer,pos+4,4,"Sequence number",SString::toFormat("%d",ntohl(pTcp->seq_num)).data(),"");
					p2 = p->New(pBuffer,pos+8,4,"Acknowledgment number",SString::toFormat("%d",ntohl(pTcp->ack_num)).data(),"");
					p2 = p->New(pBuffer,pos+12,1,"Header length",SString::toFormat("%d",pTcp->GetTcpHeadLen()).data(),"");
					p2 = p->New(pBuffer,pos+12,2,"Flags",SString::toFormat("0x%03X",pTcp->GetFlag()).data(),"");
					p2 = p->New(pBuffer,pos+14,2,"Window size",SString::toFormat("%d",pTcp->GetWindowSize()).data(),"");
					p2 = p->New(pBuffer,pos+16,2,"Checksum",SString::toFormat("0x%04X",ntohs(pTcp->chk_sum)).data(),"");
				}
#endif
				//MMSЭ��ʶ��
#ifdef _SKT_NUMBER_SWAP
				if(pTcp->src_port == 102 || pTcp->dst_port == 102)
#else
				if(pTcp->src_port == 0x6600 || pTcp->dst_port == 0x6600)
#endif
				{
					pos += pTcp->GetTcpHeadLen();
					prot_type =  SDECODE_PROT_MMS;
				}

				//104Э��ʶ��
#ifdef _SKT_NUMBER_SWAP
				else if(pTcp->src_port == 2404 || pTcp->dst_port == 2404)
#else
				else if(pTcp->src_port == 0x6409 || pTcp->dst_port == 0x6409)
#endif
				{				
					pos += pTcp->GetTcpHeadLen();
					prot_type = SDECODE_PROT_IEC104;
				}
				else
				{
					prot_type = SDECODE_PROT_TCP;
				}
			}
			else if(pIp->ptl == 0x11)
			{
				//udp
				pos += pIp->GetIpHeadLen();
				SDECODE_UDP_HEADER *pUdp = (SDECODE_UDP_HEADER*)&pBuffer[pos];
				m_PreInfo.m_pUdp = pUdp;
				rest_len = iLen - pos;
				if(rest_len < sizeof(SDECODE_UDP_HEADER))
				{
#ifdef SDECODE_OFFLINE
					if(m_pDecodeTreeRoot != NULL)
					{
						m_pDecodeTreeRoot->NewMalformed(pBuffer,pos,iLen-pos);
					}
#endif
					m_PreInfo.m_iFmtErrorCode = SDECODE_FMT_UDP;//UDP�㳤�Ȳ���ȷ
				}
				//SNTPЭ��ʶ��
#ifdef _SKT_NUMBER_SWAP
				if(pUdp->src_port == 123 || pUdp->dst_port == 123)
#else
				if(pUdp->src_port == 0x7B00 || pUdp->dst_port == 0x7B00)
#endif
				{				
					pos += sizeof(SDECODE_UDP_HEADER);
					prot_type = SDECODE_PROT_SNTP;
				}
				else
				{
					prot_type = SDECODE_PROT_UDP;
				}
			}
			else if(pIp->ptl == 0x01)
			{
				//ICMP
				pos += pIp->GetIpHeadLen();
				prot_type = SDECODE_PROT_ICMP;
			}
			else
			{
				prot_type = SDECODE_PROT_IP;
			}
		}
		break;
	case 0x0608://ARPЭ��
		prot_type = SDECODE_PROT_ARP;
#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			p = pEth2->New(pBuffer,12,2,"Type","ARP","");
			p = m_pDecodeTreeRoot->New(pBuffer,14,iLen-14,"Address Resolution Protocol","","");
		}
#endif
		break;
	case 0xB888://GOOSE
		prot_type = SDECODE_PROT_GOOSE;
#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			p = pEth2->New(pBuffer,12,2,"Type","GOOSE","");
			p = m_pDecodeTreeRoot->New(pBuffer,14,iLen-14,"GOOSE","","");
		}
#endif
		break;
	case 0xBA88://SV
		prot_type = SDECODE_PROT_SV92;//SDECODE_PROT_SV91
#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			p = pEth2->New(pBuffer,12,2,"Type","SMV","");
			p = m_pDecodeTreeRoot->New(pBuffer,14,iLen-14,"SMV","","");
		}
#endif
		//MEMO: ��Ҫ����9-1��9-2���� [2015-5-8 15:19 �ۿ���]
		break;
	case 0xF788://PTP
		prot_type = SDECODE_PROT_PTP;
#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			p = pEth2->New(pBuffer,12,2,"Type","PTP","");
			p = m_pDecodeTreeRoot->New(pBuffer,14,iLen-14,"PTP","","");
		}
#endif
		break;
	default://��Ϊ��EthernetII
		prot_type = SDECODE_PROT_ETH2;
#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			p = pEth2->New(pBuffer,12,2,"Type",SString::toFormat("Unknown:%d",ntohs(mac_type)).data(),"");
		}
#endif
		break;
	}
	if(m_PreInfo.m_iFmtErrorCode != SDECODE_FMT_NOERROR)
		return prot_type;

	m_PreInfo.m_iDecodeType = prot_type;
	m_PreInfo.m_iStartPos = pos;
	if(m_PreInfo.m_pTcp != NULL)
	{
		//TCP���ӣ�����TCP�Ự
		SDecodeBase *pDecode = m_DecodeProt[prot_type];
		if(pDecode != NULL)
		{
			pDecode->m_pLastSession = m_PreInfo.m_pSession = pDecode->SearchTcpSession(soc,true);
			if(pDecode->m_pLastSession != NULL)
			{
				pDecode->m_pLastSession->m_iNoComplexCnt ++;//û�и��ϱ��ĵ�����+1������и��ϱ�����Э��ʵ��������0
			}
		}
		else
			m_PreInfo.m_pSession = NULL;
	}
	else
		m_PreInfo.m_pSession = NULL;

	return prot_type;
}


//////////////////////////////////////////////////////////////////////////
// ��    ��:  ���һ֡����
// ��    ��:  �ۿ���
// ����ʱ��:  2015-5-7 16:51
// ����˵��:  @soc��ʾ����ʱ�꣨�룩
//         :  @nsec��ʾ����ʱ�꣨���룩
//         :  @pBuffer��ʾ���Ļ�������ַ
//         :  @iLen��ʾ����ʵ�ʳ���
// �� �� ֵ:  int,��ӦeSDecodeFmtError,0��ʾ����ɹ�
//////////////////////////////////////////////////////////////////////////
int SDecodeMgr::AddFrame(int soc,int nsec,BYTE *pBuffer,int iLen)
{
	if(m_pDecodeTreeRoot != NULL)
	{
		//����ϴεĽ����
		m_pDecodeTreeRoot->ClearChild();
	}
	eSDecodeProtType iProtType = PreDecode(soc,pBuffer,iLen);
	if(m_DecodeProt[iProtType] == NULL || m_PreInfo.m_iFmtErrorCode != SDECODE_FMT_NOERROR)
		return m_PreInfo.m_iFmtErrorCode;
	int ret = m_DecodeProt[iProtType]->DecodeFrame(soc,nsec);
	m_DecodeProt[iProtType]->AfterDecode();

	//���߷���ģʽ�£�ÿ5����һ���Ƿ�Ựɾ��
#ifndef SDECODE_OFFLINE
	if(/*m_bRemoveTcpSessionWhenDiscon && */abs(soc-m_iLastCheckRmSOC) > 5)
		CheckForSessionRemove(soc);
#endif
	return ret;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �����Ľ��ճ�ʱ��ʱ�򣬵��øú���������·״̬�ĸ���
// ��    ��:  �ۿ���
// ����ʱ��:  2015-5-10 12:44
// ����˵��:  @socΪԤ�Ƶ�ʱ�䣨�����룩������Ҫ̫��ȷ�������뼶���ȼ���
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
#ifndef SDECODE_OFFLINE
void SDecodeMgr::FrameTimeout(int soc)
{
	//���߷���ģʽ�£�ÿ5����һ���Ƿ�Ựɾ��

	if(/*m_bRemoveTcpSessionWhenDiscon && */abs(soc-m_iLastCheckRmSOC) > 5)
		CheckForSessionRemove(soc);
}
#endif
//////////////////////////////////////////////////////////////////////////
// ��    ��:  ���ý��������ĸ��ڵ�ָ�룬Ĭ��ΪNULL����ʾ����ʱ����Ҫ���ɽ�������
// ��    ��:  �ۿ���
// ����ʱ��:  2015-5-10 14:06
// ����˵��:  @p���ĸ��ڵ㣬ÿ�ν���ʱ��������е����ݣ������µĽ�����������
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
#ifdef SDECODE_OFFLINE
void SDecodeMgr::SetDecodeTreeRootPtr(SDecodeTree *p)
{
	p->m_pTempTree = &m_TempTree;
	m_pDecodeTreeRoot = p;
	int i;
	for(i=0;i<SDECODE_PROT_COUNT;i++)
	{
		if(m_DecodeProt[i] != NULL)
			m_DecodeProt[i]->SetDecodeTreeRootPtr(p);
	}
}
#endif

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �����¼�����Ļص�����
// ��    ��:  �ۿ���
// ����ʱ��:  2015-5-10 14:20
// ����˵��:  @OnDecodeEvent�ص������� void (*OnDecodeEvent)(void* cbParam,stuSDecodeEvent *pEvent)  ��
//         :  @cbParam�ص�ʱ�ĵ�һ��������ͨ�����ڻص��������ֲ�ͬ�Ľ���ʵ��)
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SDecodeMgr::SetEventCallback(OnDecodeEvent pFun,void *cbParam)
{
	m_pEventCallbackFun = pFun;
	m_pEventCallbackParam = cbParam;
	int i;
	for(i=0;i<SDECODE_PROT_COUNT;i++)
	{
		if(m_DecodeProt[i] != NULL)
			m_DecodeProt[i]->SetEventCallback(pFun,cbParam);
	}

}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ���ڼ���Ƿ�����Ҫɾ����TCP�Ự
// ��    ��:  �ۿ���
// ����ʱ��:  2015-5-10 13:17
// ����˵��:  @socΪ��ǰʱ��
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SDecodeMgr::CheckForSessionRemove(int soc)
{
	int i;
	for(i=0;i<SDECODE_PROT_COUNT;i++)
	{
		if(m_DecodeProt[i] != NULL)
			m_DecodeProt[i]->RemoveInvalidTcpSession(soc);
	}
	m_iLastCheckRmSOC = soc;
}
