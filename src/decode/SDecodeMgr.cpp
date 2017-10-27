/**
 *
 * 文 件 名 : SDecoder.cpp
 * 创建日期 : 2015-5-7 16:36
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : 以太网报文解码器
 * 修改记录 : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-5-7	邵凯田　创建文件
 *
 **/

#include "SDecodeMgr.h"
#include "SDecodeMms.h"
#include "SDecodeIp.h"
#include "SDecodeOth.h"

//////////////////////////////////////////////////////////////////////////
// 名    称:  SDecodeMgr
// 作    者:  邵凯田
// 创建时间:  2015-5-7 16:39
// 描    述:  解码管理器，维护一个解码实例
//////////////////////////////////////////////////////////////////////////
SDecodeMgr::SDecodeMgr()
{
	memset(m_DecodeProt,0,sizeof(m_DecodeProt));
	m_iLastCheckRmSOC = 0;
	//初始解码协议
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
// 描    述:  开始解码管理器
// 作    者:  邵凯田
// 创建时间:  2015-5-7 16:43
// 参数说明:  @bRemoveTcpSessionWhenDiscon表示是否在TCP会话断开以后销毁会话实例（离线分析时为false,在线分析时为true）
//         :  @sAttr为初始化属性字符串，使用指示开启哪些解码协议（后续定义）
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
bool SDecodeMgr::Start(bool bRemoveTcpSessionWhenDiscon,SString sAttr)
{
	//m_bRemoveTcpSessionWhenDiscon = bRemoveTcpSessionWhenDiscon;
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  停止解码，清除所有暂存信息、会话
// 作    者:  邵凯田
// 创建时间:  2015-5-7 16:48
// 参数说明:  void
// 返 回 值:  true表示成功，false表示失败
//////////////////////////////////////////////////////////////////////////
bool SDecodeMgr::Stop()
{
	return true;
}


//////////////////////////////////////////////////////////////////////////
// 描    述:  预解码，获得报文的类型，以及其对应协议有效数据的起始位置和长度
// 作    者:  邵凯田
// 创建时间:  2015-5-8 14:10
// 参数说明:  @soc表示当前报文的时标
//         :  @pBuffer表示报文缓冲区
//         :  @iLen表示报文长度
//         :  @iLen表示报文长度
// 返 回 值:  eSDecodeProtType，对应的协议类型
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
		p = pEth2->New(pBuffer,0,6,"Destination",SString::toFormat("%02X:%02X:%02X:%02X:%02X:%02X",pBuffer[0],pBuffer[1],pBuffer[2],pBuffer[3],pBuffer[4],pBuffer[5]).data(),"目标MAC地址");
		p = pEth2->New(pBuffer,6,6,"Source",SString::toFormat("%02X:%02X:%02X:%02X:%02X:%02X",pBuffer[6],pBuffer[7],pBuffer[8],pBuffer[9],pBuffer[10],pBuffer[11]).data(),"源MAC地址");
	}
#endif
#ifdef _SKT_NUMBER_SWAP
	if(mac_type == 0x0081)
#else
	if(mac_type == 0x8100)
#endif
	{
		//含VLAN
		m_PreInfo.m_pVlan = (SDECODE_VLAN_HEADER*)(&pBuffer[pos]);
		mac_type = m_PreInfo.m_pVlan->prot;
#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			pEth2->New(pBuffer,12,2,"Type","VLAN","");
			p = m_pDecodeTreeRoot->New(pBuffer,pos,4,"802.1Q Virtual LAN","","");
			p->New(pBuffer,pos,2,"Priority",SString::toFormat("%d",m_PreInfo.m_pVlan->GetPriority()).data(),"优先级");
			p->New(pBuffer,pos,2,"CFI",SString::toFormat("%d",m_PreInfo.m_pVlan->GetCFI()).data(),"");
			p->New(pBuffer,pos,2,"ID",SString::toFormat("%d",m_PreInfo.m_pVlan->GetVLanNum()).data(),"VLAN号");
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
	case 0x0008://IP协议
		{
			SDECODE_IP_HEADER *pIp = (SDECODE_IP_HEADER*)&pBuffer[pos];
			m_PreInfo.m_pIp = pIp;
			//IP层长度校验
			rest_len = iLen - pos;
			if(rest_len < sizeof(SDECODE_IP_HEADER))
			{
#ifdef SDECODE_OFFLINE
				if(m_pDecodeTreeRoot != NULL)
				{
					m_pDecodeTreeRoot->NewMalformed(pBuffer,pos,iLen-pos);
				}
#endif
				m_PreInfo.m_iFmtErrorCode = SDECODE_FMT_IP;//IP层长度不正确
				break;
			}
#ifdef SDECODE_OFFLINE
			if(m_pDecodeTreeRoot != NULL)
			{
				p = pEth2->New(pBuffer,12,2,"Type","IP","");
				p = m_pDecodeTreeRoot->New(pBuffer,pos,pIp->GetIpHeadLen(),"Internet Protocol","","");
				p2 = p->New(pBuffer,pos,1,"Version",SString::toFormat("%d",pIp->GetIpVer()).data(),"版本号");
				p2 = p->New(pBuffer,pos+1,1,"Header length",SString::toFormat("%d",pIp->GetIpHeadLen()).data(),"IP头部长度");
				p2 = p->New(pBuffer,pos+2,2,"Differentiated Services Field",SString::toFormat("%d",pIp->tos ).data(),"");
				p2 = p->New(pBuffer,pos+4,2,"Total Length",SString::toFormat("%d",pIp->len ).data(),"总长度");
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
					m_PreInfo.m_iFmtErrorCode = SDECODE_FMT_IP;//IP层长度不正确
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
					m_PreInfo.m_iFmtErrorCode = SDECODE_FMT_TCP;//TCP层长度不正确
					break;
				}
#ifdef SDECODE_OFFLINE
				if(m_pDecodeTreeRoot != NULL)
				{
					p = m_pDecodeTreeRoot->New(pBuffer,pos,pTcp->GetTcpHeadLen(),"Transmission Control Protocol ",
						SString::toFormat("Src Port: %d, Dst Port: %d, Seq: %d, Ack: %d, Len: %d",ntohs(pTcp->src_port),ntohs(pTcp->dst_port),ntohl(pTcp->seq_num),ntohl(pTcp->ack_num),rest_len ).data(),"");
					p2 = p->New(pBuffer,pos+0,2,"Source port",SString::toFormat("%d",ntohs(pTcp->src_port)).data(),"源端口号");
					p2 = p->New(pBuffer,pos+2,2,"Destination port",SString::toFormat("%d",ntohs(pTcp->dst_port)).data(),"目标端口号");
					p2 = p->New(pBuffer,pos+4,4,"Sequence number",SString::toFormat("%d",ntohl(pTcp->seq_num)).data(),"");
					p2 = p->New(pBuffer,pos+8,4,"Acknowledgment number",SString::toFormat("%d",ntohl(pTcp->ack_num)).data(),"");
					p2 = p->New(pBuffer,pos+12,1,"Header length",SString::toFormat("%d",pTcp->GetTcpHeadLen()).data(),"");
					p2 = p->New(pBuffer,pos+12,2,"Flags",SString::toFormat("0x%03X",pTcp->GetFlag()).data(),"");
					p2 = p->New(pBuffer,pos+14,2,"Window size",SString::toFormat("%d",pTcp->GetWindowSize()).data(),"");
					p2 = p->New(pBuffer,pos+16,2,"Checksum",SString::toFormat("0x%04X",ntohs(pTcp->chk_sum)).data(),"");
				}
#endif
				//MMS协议识别
#ifdef _SKT_NUMBER_SWAP
				if(pTcp->src_port == 102 || pTcp->dst_port == 102)
#else
				if(pTcp->src_port == 0x6600 || pTcp->dst_port == 0x6600)
#endif
				{
					pos += pTcp->GetTcpHeadLen();
					prot_type =  SDECODE_PROT_MMS;
				}

				//104协议识别
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
					m_PreInfo.m_iFmtErrorCode = SDECODE_FMT_UDP;//UDP层长度不正确
				}
				//SNTP协议识别
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
	case 0x0608://ARP协议
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
		//MEMO: 需要考虑9-1与9-2区分 [2015-5-8 15:19 邵凯田]
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
	default://认为是EthernetII
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
		//TCP连接，查找TCP会话
		SDecodeBase *pDecode = m_DecodeProt[prot_type];
		if(pDecode != NULL)
		{
			pDecode->m_pLastSession = m_PreInfo.m_pSession = pDecode->SearchTcpSession(soc,true);
			if(pDecode->m_pLastSession != NULL)
			{
				pDecode->m_pLastSession->m_iNoComplexCnt ++;//没有复合报文的数量+1，如果有复合报文如协议实例负责清0
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
// 描    述:  添加一帧报文
// 作    者:  邵凯田
// 创建时间:  2015-5-7 16:51
// 参数说明:  @soc表示报文时标（秒）
//         :  @nsec表示报文时标（纳秒）
//         :  @pBuffer表示报文缓冲区地址
//         :  @iLen表示报文实际长度
// 返 回 值:  int,对应eSDecodeFmtError,0表示解码成功
//////////////////////////////////////////////////////////////////////////
int SDecodeMgr::AddFrame(int soc,int nsec,BYTE *pBuffer,int iLen)
{
	if(m_pDecodeTreeRoot != NULL)
	{
		//清除上次的结果树
		m_pDecodeTreeRoot->ClearChild();
	}
	eSDecodeProtType iProtType = PreDecode(soc,pBuffer,iLen);
	if(m_DecodeProt[iProtType] == NULL || m_PreInfo.m_iFmtErrorCode != SDECODE_FMT_NOERROR)
		return m_PreInfo.m_iFmtErrorCode;
	int ret = m_DecodeProt[iProtType]->DecodeFrame(soc,nsec);
	m_DecodeProt[iProtType]->AfterDecode();

	//在线分析模式下，每5秒检查一次是否会话删除
#ifndef SDECODE_OFFLINE
	if(/*m_bRemoveTcpSessionWhenDiscon && */abs(soc-m_iLastCheckRmSOC) > 5)
		CheckForSessionRemove(soc);
#endif
	return ret;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  当报文接收超时的时候，调用该函数进行链路状态的更新
// 作    者:  邵凯田
// 创建时间:  2015-5-10 12:44
// 参数说明:  @soc为预计的时间（世纪秒），不需要太精确，满足秒级精度即可
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
#ifndef SDECODE_OFFLINE
void SDecodeMgr::FrameTimeout(int soc)
{
	//在线分析模式下，每5秒检查一次是否会话删除

	if(/*m_bRemoveTcpSessionWhenDiscon && */abs(soc-m_iLastCheckRmSOC) > 5)
		CheckForSessionRemove(soc);
}
#endif
//////////////////////////////////////////////////////////////////////////
// 描    述:  设置解码结果树的根节点指针，默认为NULL（表示解码时不需要生成解码树）
// 作    者:  邵凯田
// 创建时间:  2015-5-10 14:06
// 参数说明:  @p树的根节点，每次解码时将清除其中的内容，并有新的结果构建结果树
// 返 回 值:  void
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
// 描    述:  设置事件处理的回调函数
// 作    者:  邵凯田
// 创建时间:  2015-5-10 14:20
// 参数说明:  @OnDecodeEvent回调函数【 void (*OnDecodeEvent)(void* cbParam,stuSDecodeEvent *pEvent)  】
//         :  @cbParam回调时的第一个参数（通常用于回调函数区分不同的解码实例)
// 返 回 值:  void
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
// 描    述:  定期检查是否有需要删除的TCP会话
// 作    者:  邵凯田
// 创建时间:  2015-5-10 13:17
// 参数说明:  @soc为当前时间
// 返 回 值:  void
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
