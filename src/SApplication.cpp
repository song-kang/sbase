/**
 *
 * �� �� �� : SApplication.cpp
 * �������� : 2009-8-20 13:36
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: 2011/08/31 08:43:32 $
 * ��ǰ�汾 : $Revision: 1.1 $
 * �������� : 
 * �޸ļ�¼ : 
 *            $Log: SApplication.cpp,v $
 *            Revision 1.1  2011/08/31 08:43:32  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.11  2011/05/09 11:08:27  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.10  2011/04/27 02:58:48  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.9  2011/04/23 10:53:52  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.8  2011/04/22 06:32:33  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.7  2011/04/21 02:14:16  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.6  2011/04/11 12:08:34  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.5  2011/04/04 08:20:57  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.4  2011/04/03 07:33:12  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2011/03/29 07:07:00  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2011/03/03 12:07:39  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2011/02/25 03:12:05  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.6  2011/01/17 01:20:49  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.5  2010/12/20 08:37:09  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.4  2010/11/19 02:03:22  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2010/11/17 02:19:15  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2010/11/16 08:08:55  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2010/08/09 03:04:39  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2009/09/15 02:14:09  shaokt
 *            ͨѸ��¼����װ����Ŀ�״��ύ
 *
 *
 **/


#include "SApplication.h"

//////////////////////////////////////////////////////////////////////////
// History
// 1.0.0	2011-xx-xx  �ۿ��Ӧ�ó�������
// 2.0.0	2013-08-xx  �ۿ��Ӧ�ó���������������
//////////////////////////////////////////////////////////////////////////

#define SKTBASE_VERSION "2.0.0"

#ifndef _WITHOUT_AGENT_CHANNEL_

CSpAgentClient::CSpAgentClient()
{
	SetStartWord(0xeb57);
	m_pApp = NULL;
	m_iBeginSOC = (DWORD)SDateTime::getNowSoc();
	m_bAgentUplink = false;
	m_RecvPackage_Processed.setAutoDelete(false);
	m_RecvPackage_Processed.setShared(true);
}

CSpAgentClient::~CSpAgentClient()
{
	m_RecvPackage_Processed.setAutoDelete(true);
	m_RecvPackage_Processed.clear();
}

bool CSpAgentClient::StartAgent(bool bBlock)
{
	if(bBlock)
	{
		Start(m_sAgentIp,m_iAgentPort);
		while(!this->IsQuit())
		{
			SApi::UsSleep(1000000);
		}
	}
	else
	{
		Start(m_sAgentIp,m_iAgentPort);
	}
	return true;
}

bool CSpAgentClient::StopAgent()
{
	return STcpClientBase::StopAndWait(3);
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  �Է�����ʽ�����ͻ��ˣ�������Connect����ͬʱʹ��
// ��    ��:  �ۿ���
// ����ʱ��:  2016-2-3 09:52
// ����˵��:  @ip��ʾ�����IP��ַ��@port��ʾ�˿�
// �� �� ֵ:  true��ʾ���������ɹ�,false��ʾ��������ʧ��
//////////////////////////////////////////////////////////////////////////
bool CSpAgentClient::Start(SString ip,int port)
{
	SString sLogin;
// 	if(m_pApp->m_iUnitId == 0)
// 	{
// 		SString ips = SSocket::GetLocalIp().c_str();
// 		int i,cnt = SString::GetAttributeCount(ips,",");
// 		for(i=1;i<=cnt;i++)
// 		{
// 			m_pApp->m_iUnitId ^= SSocket::IpStrToIpv4(SString::GetIdAttribute(i,ips,",").data());
// 		}
// 	}
	sLogin.sprintf("act=login;soft_type=%s;",m_pApp->m_sSoftwareType.data());
	SetLoginInfo(sLogin);
	if(m_pApp->m_iUnitId == 0)
	{
		LOGERROR("δ���õ�Ԫ��ʶ!");
		return false;
	}
	m_SelfProcId.process_id.m_iUnitId = m_pApp->m_iUnitId;
	m_SelfProcId.process_id.SetApplicationId((WORD)m_pApp->m_iApplicationId,(WORD)m_pApp->m_iAppInstId);

	SKT_CREATE_THREAD(ThreadAgentMsgProc,this);
	return STcpClientBase::Start(ip,port);
}


//////////////////////////////////////////////////////////////////////////
// ��    ��:  ͨѶ���ݰ�����
// ��    ��:  �ۿ���
// ����ʱ��:  2016-2-4 17:29
// ����˵��:  @pPackageΪԴ���ݰ�
//         :  @pMsgHead
// �� �� ֵ:  true��ʾ���Ϸ���false��ʾ���Ϸ�
//////////////////////////////////////////////////////////////////////////
bool CSpAgentClient::PackageExplain(stuSTcpPackage *pPackage,WORD &wMsgType, stuSpUnitAgentMsgHead* &pMsgHead,BYTE* &pBuffer,int &iLength)
{
	if(pPackage == NULL || pPackage->m_iAsduLen < (int)sizeof(stuSpUnitAgentMsgHead))
		return false;
	wMsgType = pPackage->m_wFrameType;
	pMsgHead = (stuSpUnitAgentMsgHead*)pPackage->m_pAsduBuffer;
	pMsgHead->Swap();
	iLength = pPackage->m_iAsduLen - (int)sizeof(stuSpUnitAgentMsgHead);
	if(iLength == 0)
		pBuffer = NULL;
	else
		pBuffer = (BYTE*)(pMsgHead+1);
	return true;
}


//////////////////////////////////////////////////////////////////////////
// ��    ��:  ����ͨѶ���ݰ�
// ��    ��:  �ۿ���
// ����ʱ��:  2016-2-4 18:18
// ����˵��:  @pMsgHeadΪ��Ϣͷ
//         :  @pBufferΪ��ϢЯ���Ķ������������ݣ�NULL��ʾ�޶���������
//         :  @iLengthΪ���������ݳ���
// �� �� ֵ:  stuSTcpPackage*
//////////////////////////////////////////////////////////////////////////
stuSTcpPackage* CSpAgentClient::PackageBuild(WORD wMsgType,stuSpUnitAgentMsgHead *pMsgHead,BYTE* pBuffer/*=NULL*/,int iLength/*=0*/)
{
	BYTE *pBuf = new BYTE[iLength+(int)sizeof(stuSpUnitAgentMsgHead)];
	memcpy(pBuf,pMsgHead,(int)sizeof(stuSpUnitAgentMsgHead));
	((stuSpUnitAgentMsgHead*)pBuf)->Swap();
	if(pBuffer != NULL && iLength > 0)
	{
		memcpy(pBuf+(int)sizeof(stuSpUnitAgentMsgHead),pBuffer,iLength);
	}
	return NewPackage(wMsgType,pBuf,iLength+(int)sizeof(stuSpUnitAgentMsgHead),false);
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ��¼�Ϸ���˺�Ļص�
// ��    ��:  �ۿ���
// ����ʱ��:  2016-2-2 22:25
// ����˵��:  void
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void CSpAgentClient::OnLogin()
{
	if(m_pApp == NULL)
		return;
	if(m_bAgentUplink)//m_pApp->m_iApplicationId == SP_UA_APPNO_AGENT)
	{
		//�Դ�����ݵ�¼��һ������
		stuSpUnitAgentUnitInfo unit;
		memset(&unit,0,(int)sizeof(unit));
		unit.m_iUnitId = m_pApp->m_iUnitId;
		SString::strncpy(unit.m_sSoftType,(int)sizeof(unit.m_sSoftType),m_pApp->m_sSoftwareType.data());
		SString::strncpy(unit.m_sUnitName,(int)sizeof(unit.m_sUnitName),m_pApp->m_sUnitName.data());
		SString::strncpy(unit.m_sUnitDesc,(int)sizeof(unit.m_sUnitDesc),m_pApp->m_sUnitDesc.data());
		SString sLocalIps = SSocket::GetLocalIp();
		int i,cnt = SString::GetAttributeCount(sLocalIps,",");
		for(i=1;i<=cnt&&i<=8;i++)
		{
			unit.m_iUnitIp[i-1] = SSocket::IpStrToIpv4(SString::GetIdAttribute(i,sLocalIps,",").data());
		}
// 		if(unit.m_iUnitId == 0)
// 			unit.m_iUnitId = m_pApp->m_iUnitId = unit.m_iUnitIp[0];
		AddToSend(NewPackage(SP_UA_MSG_REG_AGENT,(BYTE*)&unit,(int)sizeof(unit)));

	}
	else
	{
		//������������ݵ�¼����������д��ǰ������Ϣ
		memset(&m_SelfProcId,0,(int)sizeof(m_SelfProcId));	
		m_SelfProcId.process_id.m_iUnitId = m_pApp->m_iUnitId;
		m_SelfProcId.process_id.SetApplicationId((WORD)m_pApp->m_iApplicationId,(WORD)m_pApp->m_iAppInstId);
		m_SelfProcId.pid = SApi::GetPid();
		SString::strncpy(m_SelfProcId.name,(int)sizeof(m_SelfProcId.name),m_pApp->GetModule().data());
		SString::strncpy(m_SelfProcId.version,(int)sizeof(m_SelfProcId.version),m_pApp->GetModuleVer().data());
		SString::strncpy(m_SelfProcId.desc,(int)sizeof(m_SelfProcId.desc),m_pApp->GetModuleDesc().data());
		SString::strncpy(m_SelfProcId.crc,(int)sizeof(m_SelfProcId.crc),m_pApp->GetModuleCrc().data());
		SString::strncpy(m_SelfProcId.build_time,(int)sizeof(m_SelfProcId.build_time),m_pApp->GetModuleTime().data());
		m_SelfProcId.begin_time = m_iBeginSOC;
		AddToSend(NewPackage(SP_UA_MSG_REGISTER,(BYTE*)&m_SelfProcId,(int)sizeof(m_SelfProcId)));

	}
}


////////////////////////////////////////////////////////////////////////
// ��    ��:  �Ự�߳̽��յ����ĺ�Ļص��麯����������ͨ���˺�������ʱ��Ϣ
// ��    ��:  �ۿ���
// ����ʱ��:  2016-2-3 09:35
// ����˵��:  @pSessionΪ�Ựʵ��ָ�룬����ͨ��ָ����Զ˷�������
//            @pPackageΪ�ս��յ������ݰ�
// �� �� ֵ:  true��ʾ�Ѿ�������ϣ��˱��Ŀ����ͷ��ˣ�false��ʾδ�����˱���Ӧ������ն���
//////////////////////////////////////////////////////////////////////////
bool CSpAgentClient::OnRecvFrame(stuSTcpPackage *pPackage)
{
	if(pPackage == NULL || pPackage->m_iAsduLen < (int)sizeof(stuSpUnitAgentMsgHead))
		return true;
	if(pPackage->m_wFrameType == SP_UA_MSG_SEND_OK || pPackage->m_wFrameType == SP_UA_MSG_SEND_ERR)
	{
		m_RecvPackage_Processed.append(NewPackage(pPackage));
		return true;
	}
	return false;
// 	stuSpUnitAgentMsgHead *pMsg = (stuSpUnitAgentMsgHead*)pPackage->m_pAsduBuffer;
// 	int iAsduLen = pPackage->m_iAsduLen - sizeof(stuSpUnitAgentMsgHead);
// 	BYTE *pAsdu = iAsduLen==0?NULL:((BYTE*)(pMsg+1));
// 	bool bProcess = ProcessAgentMsg(pPackage->m_wFrameType,pMsg,pAsdu,iAsduLen);
// 	if(bProcess == false && pPackage->m_wFrameType == SP_UA_MSG_PROC_TXT_CMD)
// 	{
// 		BYTE buf[128]={0};
// 		stuSpUnitAgentMsgHead *pHead = (stuSpUnitAgentMsgHead*)buf;
// 		memcpy(&pHead->m_DstId,&pMsg->m_SrcId,sizeof(pMsg->m_SrcId));
// 		memcpy(&pHead->m_SrcId,&pMsg->m_DstId,sizeof(pMsg->m_DstId));
// 		pHead->m_dwMsgSn = pMsg->m_dwMsgSn;
// 		pHead->m_dwParamReault = 0;
// 		char* pStr = (char*)(&buf[sizeof(stuSpUnitAgentMsgHead)]);
// 		strcpy(pStr,"The message 'cmd' is unsuported on this application!\r\n");
// 		AddToSend(NewPackage(SP_UA_MSG_PROC_TXT_CMD,(BYTE*)buf,sizeof(stuSpUnitAgentMsgHead)+strlen(pStr)+1));
// 		bProcess = true;
// 	}
// 	return bProcess;
}

void* CSpAgentClient::ThreadAgentMsgProc(void *lp)
{
	CSpAgentClient *pThis = (CSpAgentClient*)lp;
	stuSTcpPackage *pPackage;
	pThis->BeginThread();
	while(!pThis->IsQuit())
	{
		pThis->GetRecvPackagePtr()->lock();
		pPackage = pThis->GetRecvPackagePtr()->at(0,false);
		if(pPackage == NULL)
		{
			pThis->GetRecvPackagePtr()->unlock();
			SApi::UsSleep(1000);
			continue;
		}
		pThis->GetRecvPackagePtr()->remove(pPackage,false);
		pThis->GetRecvPackagePtr()->unlock();
		stuSpUnitAgentMsgHead *pMsg = (stuSpUnitAgentMsgHead*)pPackage->m_pAsduBuffer;
		pMsg->Swap();
		int iAsduLen = pPackage->m_iAsduLen - (int)sizeof(stuSpUnitAgentMsgHead);
		BYTE *pAsdu = iAsduLen==0?NULL:((BYTE*)(pMsg+1));
		if(pPackage->m_wFrameType == SP_UA_MSG_APP_STOP)
		{
			LOGWARN("�յ�Ӧ�ý����˳�����!");
			SApi::UsSleep(10000);
			pThis->STcpClientBase::Stop();
			pThis->StopAgent();
			pThis->STcpClientBase::StopAndWait(3);
			pThis->m_pApp->Stop();
			pThis->m_pApp->StopAndWait(10);
			delete pPackage;
		}
		else if(pPackage->m_wFrameType == SP_UA_MSG_PROC_TXT_CMD_REQ)
		{
			if(pAsdu != NULL)
			{
				pAsdu[iAsduLen-1] = '\0';
				SString sCmd = (char*)pAsdu;
				SString sResult;
				if(sCmd == "cmd")
					sCmd = "";
				else if(sCmd.length()>=4)
					sCmd = sCmd.mid(4);
				if(!pThis->ProcessTxtCmd(sCmd,sResult))
					sResult = "The message 'cmd' is unsuported on this application!\r\n";
				BYTE *buf = new BYTE[(int)sizeof(stuSpUnitAgentMsgHead)+sResult.length()+1];
				stuSpUnitAgentMsgHead *pHead = (stuSpUnitAgentMsgHead*)buf;
				memcpy(&pHead->m_DstId,&pMsg->m_SrcId,(int)sizeof(pMsg->m_SrcId));
				memcpy(&pHead->m_SrcId,&pMsg->m_DstId,(int)sizeof(pMsg->m_DstId));
				pHead->m_dwMsgSn = pMsg->m_dwMsgSn;
				pHead->m_dwParamReault = 0;
				char* pStr = (char*)(&buf[(int)sizeof(stuSpUnitAgentMsgHead)]);
				strcpy(pStr,sResult.data());
				pThis->SendAgentMsg(&pMsg->m_SrcId,SP_UA_MSG_PROC_TXT_CMD_ACK,pMsg->m_dwMsgSn,0,(BYTE*)sResult.data(),sResult.length()+1);
				//pThis->AddToSend(pThis->NewPackage(SP_UA_MSG_PROC_TXT_CMD_ACK,(BYTE*)buf,sizeof(stuSpUnitAgentMsgHead)+sResult.length()+1));
				delete[] buf;
			}
			delete pPackage;
		}
		else
		{
			bool bProcess = pThis->ProcessAgentMsg(pPackage->m_wFrameType,pMsg,pPackage->m_sHead,pAsdu,iAsduLen);
			if(bProcess)
				delete pPackage;		
			else
			{
				pThis->m_RecvPackage_Processed.append(pPackage);
				while(pThis->m_RecvPackage_Processed.count() > 1000)
				{
					pPackage = pThis->m_RecvPackage_Processed[0];
					pThis->m_RecvPackage_Processed.remove(0);
					delete pPackage;
				}
			}
		}
	}
	pThis->EndThread();
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡΨһ����Ϣ���
// ��    ��:  �ۿ���
// ����ʱ��:  2016-2-3 15:35
// ����˵��:  void
// �� �� ֵ:  WORD
//////////////////////////////////////////////////////////////////////////
DWORD CSpAgentClient::NewMsgSn()
{
	static DWORD sn = 0;
	sn ++;
	return sn;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ���еĴ���Ԫ�б�
// ��    ��:  �ۿ���
// ����ʱ��:  2016-2-3 17:31
// ����˵��:  @unitsΪ���÷��صĵ�Ԫ��Ϣ
// �� �� ֵ:  ���ص�Ԫ����,<0��ʾʧ��
//////////////////////////////////////////////////////////////////////////
int CSpAgentClient::GetUnitInfo(SPtrList<stuSpUnitAgentUnitInfo> &units)
{
	stuSpUnitAgentProcessId id;
	id.m_iUnitId = m_SelfProcId.process_id.m_iUnitId;
	id.SetApplicationId(SP_UA_APPNO_AGENT);
	stuSTcpPackage *pPackage=NULL;
	if(!SendAgentMsgAndRecv(&id,SP_UA_MSG_REQ_GET_UNIT,NewMsgSn(),0,NULL,0,NULL,SP_UA_MSG_RES_GET_UNIT,pPackage,3))
	{
		LOGERROR("ȡ����Ԫʱʧ��!");
		return -1;
	}
	BYTE *pAsdu;
	int iAsduLen;
	WORD wMsgType;
	stuSpUnitAgentMsgHead *pMsgHead;
	if(!PackageExplain(pPackage,wMsgType,pMsgHead,pAsdu,iAsduLen))
	{
		delete pPackage;
		return -2;
	}
	if(iAsduLen%((int)sizeof(stuSpUnitAgentUnitInfo)) != 0)
	{
		delete pPackage;
		return -3;
	}
	int cnt = iAsduLen/(int)sizeof(stuSpUnitAgentUnitInfo);
	stuSpUnitAgentUnitInfo *p0 = (stuSpUnitAgentUnitInfo*)pAsdu;
	units.clear();
	units.setAutoDelete(true);
	for(int i=0;i<cnt;i++)
	{
		stuSpUnitAgentUnitInfo *p = new stuSpUnitAgentUnitInfo();
		memcpy(p,p0++,(int)sizeof(stuSpUnitAgentUnitInfo));
		units.append(p);
	}
	delete pPackage;
	return units.count();
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡָ����Ԫ�Ŷ�Ӧ�Ľ�����Ϣ
// ��    ��:  �ۿ���
// ����ʱ��:  2016-2-12 8:53
// ����˵��:  @iUnitId��ʾ���̱�ʶ��0��ʾ��ǰ��Ԫ
//         :  @ProcInfosΪ���÷��صĽ�����Ϣ�б�
// �� �� ֵ:  >=0��ʾ��������,<0��ʾʧ��
//////////////////////////////////////////////////////////////////////////
int CSpAgentClient::GetProcessInfo(int iUnitId,SPtrList<stuSpUnitAgentProcInfo> &ProcInfos)
{
	stuSpUnitAgentProcessId id;
	id.m_iUnitId = iUnitId==0?m_pApp->m_iUnitId:iUnitId;
	id.SetApplicationId(SP_UA_APPNO_AGENT);	
	stuSTcpPackage *pPackage=NULL;
	if(!SendAgentMsgAndRecv(&id,SP_UA_MSG_REQ_GET_PROCINFO,NewMsgSn(),0,NULL,0,NULL,SP_UA_MSG_RES_GET_PROCINFO,pPackage,3))
	{
		LOGERROR("ȡ�����б�ʱʧ��!");
		return -1;
	}
	BYTE *pAsdu;
	int iAsduLen;
	WORD wMsgType;
	stuSpUnitAgentMsgHead *pMsgHead;
	if(!PackageExplain(pPackage,wMsgType,pMsgHead,pAsdu,iAsduLen))
	{
		delete pPackage;
		return -2;
	}
	if(iAsduLen%(int)sizeof(stuSpUnitAgentProcInfo) != 0)
	{
		delete pPackage;
		return -3;
	}
	int cnt = iAsduLen/(int)sizeof(stuSpUnitAgentProcInfo);
	stuSpUnitAgentProcInfo *p0 = (stuSpUnitAgentProcInfo*)pAsdu;
	ProcInfos.clear();
	ProcInfos.setAutoDelete(true);
	for(int i=0;i<cnt;i++)
	{
		stuSpUnitAgentProcInfo *p = new stuSpUnitAgentProcInfo();
		memcpy(p,p0++,(int)sizeof(stuSpUnitAgentProcInfo));
		ProcInfos.append(p);
	}
	delete pPackage;
	return ProcInfos.count();
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡӦ�úż�ʵ���Ŷ�Ӧ�Ľ��̱�ʶ��ֻ���ص�һ��ƥ����̱�ʶ
// ��    ��:  �ۿ���
// ����ʱ��:  2016-2-3 17:11
// ����˵��:  @wAppId��ʾӦ�ú�
//         :  @wAppInstId��ʾӦ��ʵ���ţ����Ƕ�ʵ��Ӧ��ʱ��0
//         :  @pProcIdsΪ��ʶָ�룬��ȡ���ı�ʶ��д���ָ���Ӧ���ڴ���
// �� �� ֵ:  ���ؽ��̱�ʶ����,<0��ʾʧ��
//////////////////////////////////////////////////////////////////////////
int CSpAgentClient::GetProcessId(WORD wAppId,WORD wAppInstId,SPtrList<stuSpUnitAgentProcessId> &ProcIds)
{
	stuSpUnitAgentProcessId id,id2;
	id.m_iUnitId = 0;//���е�Ԫ�㲥
	id.SetApplicationId(SP_UA_APPNO_AGENT);
	id2.m_iUnitId = 0;
	id2.SetApplicationId(wAppId,wAppInstId);
	id2.Swap();
	stuSTcpPackage *pPackage=NULL;
	if(!SendAgentMsgAndRecv(&id,SP_UA_MSG_REQ_GET_PROCID,NewMsgSn(),0,(BYTE*)&id2,(int)sizeof(id2),NULL,SP_UA_MSG_RES_GET_PROCID,pPackage,3))
	{
		LOGERROR("ȡ���̱�ʶʱʧ��!");
		return -1;
	}
	BYTE *pAsdu;
	int iAsduLen;
	WORD wMsgType;
	stuSpUnitAgentMsgHead *pMsgHead;
	if(!PackageExplain(pPackage,wMsgType,pMsgHead,pAsdu,iAsduLen))
	{
		delete pPackage;
		return -2;
	}
	if(iAsduLen%(int)sizeof(stuSpUnitAgentProcessId) != 0)
	{
		delete pPackage;
		return -3;
	}
	int cnt = iAsduLen/(int)sizeof(stuSpUnitAgentProcessId);
	stuSpUnitAgentProcessId *p0 = (stuSpUnitAgentProcessId*)pAsdu;
	ProcIds.clear();
	ProcIds.setAutoDelete(true);
	for(int i=0;i<cnt;i++)
	{
		stuSpUnitAgentProcessId *p = new stuSpUnitAgentProcessId();
		memcpy(p,p0++,(int)sizeof(stuSpUnitAgentProcessId));
		ProcIds.append(p);
	}
	delete pPackage;
	return ProcIds.count();
}


//////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡָ����Ԫ���ļ��б�
// ��    ��:  �ۿ���
// ����ʱ��:  2016-2-20 13:28
// ����˵��:  @iUnitIdΪ��Ԫ��ʶ
//         :  @sPathΪ����ȡ��Ŀ¼��������{@APP_PATH@}��ʾӦ�ó����Ŀ¼
//         :  @sFilterΪ����������������Ϊ�ջ�*
//         :  @itemsΪ���ص��ļ���Ŀ¼�����
//         :  @��ʾ��ʱʱ�䣬��λΪ��
// �� �� ֵ:  >=0��ʾ��Ŀ������<0��ʾʧ��
//////////////////////////////////////////////////////////////////////////
int CSpAgentClient::GetUnitDir(int iUnitId,SString sPath,SString sFilter,SPtrList<stuSpUnitAgentLsRes> &items,int iTimeout_s/*=5*/)
{
	stuSpUnitAgentProcessId id;
	id.m_iUnitId = iUnitId;
	id.SetApplicationId(SP_UA_APPNO_AGENT);
	stuSpUnitAgentLsReq req;
	memset(&req,0,(int)sizeof(req));
	SString::strncpy(req.path,(int)sizeof(req.path),sPath.data());
	SString::strncpy(req.filter,(int)sizeof(req.filter),sFilter.data());
	stuSTcpPackage *pPackage=NULL;
	if(!SendAgentMsgAndRecv(&id,SP_UA_MSG_REQ_LS,NewMsgSn(),0,(BYTE*)&req,(int)sizeof(req),NULL,SP_UA_MSG_RES_LS,pPackage,iTimeout_s))
	{
		LOGERROR("ȡĿ¼[%s]����ʧ��!",sPath.data());
		return -1;
	}
	items.clear();
	items.setAutoDelete(true);
	WORD wMsgType;
	stuSpUnitAgentMsgHead *pMsgHead;
	BYTE *pAsdu;
	int iAsduLen;
	if(!PackageExplain(pPackage,wMsgType,pMsgHead,pAsdu,iAsduLen))
	{
		if(pPackage != NULL)
			delete pPackage;
		LOGERROR("ȡĿ¼[%s]����ʧ��!",sPath.data());
		return -2;
	}
	stuSpUnitAgentLsRes *pRes = (stuSpUnitAgentLsRes*)pAsdu;
	int i,cnt = iAsduLen/(int)sizeof(stuSpUnitAgentLsRes);
	for(i=0;i<cnt;i++)
	{
		stuSpUnitAgentLsRes *p = new stuSpUnitAgentLsRes();
		memcpy(p,pRes+i,(int)sizeof(stuSpUnitAgentLsRes));
		items.append(p);
	}
	if(pPackage != NULL)
		delete pPackage;
	return items.count();
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡָ����Ԫ���ļ����ݣ������浽�����ļ�
// ��    ��:  �ۿ���
// ����ʱ��:  2016-2-20 13:30
// ����˵��:  @iUnitIdΪ��Ԫ��ʶ
//         :  @sPathFile��ʾ�����ļ���ȫ·��������{@APP_PATH@}��ʾӦ�ó����Ŀ¼
//         :  @sLocalPathFile��ʾ���ر����ȫ·��
//         :  @��ʾ��ʱʱ�䣬��λΪ��
// �� �� ֵ:  >=0��ʾ�ļ��Ĵ�С,<0��ʾʧ��
//////////////////////////////////////////////////////////////////////////
int CSpAgentClient::GetUnitFile(int iUnitId,SString sPathFile, SString sLocalPathFile,int iTimeout_s/*=5*/)
{
	SFile file(sLocalPathFile+".tmp");
	if(!file.open(IO_Truncate))
	{
		LOGWARN("ȡ��Ԫ�ļ�ʱд������ʱ�ļ�ʧ��!file=%s.tmp",sLocalPathFile.data());
		return -1;
	}
	stuSpUnitAgentProcessId id;
	id.m_iUnitId = iUnitId;
	id.SetApplicationId(SP_UA_APPNO_AGENT);
	stuSpUnitAgentGetFileReq req;
	memset(&req,0,(int)sizeof(req));
	DWORD dwMsgSn = NewMsgSn();
	SString::strncpy(req.path_file,(int)sizeof(req.path_file),sPathFile.data());
	stuSTcpPackage *pPackage=NULL;
	if(!SendAgentMsg(&id,SP_UA_MSG_REQ_GET_FILE,dwMsgSn,0,(BYTE*)&req,(int)sizeof(req)))
	{
		LOGERROR("ȡ�ļ�[%s]����ʧ��!������Ϣʧ��!",sPathFile.data());
		return -2;
	}
	int last_soc = (int)SDateTime::getNowSoc();
	int soc = last_soc;
	int file_size = -1;
	WORD wMsgType;
	stuSpUnitAgentMsgHead *pMsgHead;
	BYTE *pAsdu;
	int iAsduLen;
	int last_sn=0;
	while(abs(soc-last_soc) <= iTimeout_s)
	{
		pPackage = SearchRecvPackage_Processed(dwMsgSn,SP_UA_MSG_RES_GET_FILE,0,true);//ֻ���Ѵ�����ȡ������˳�����
		if(pPackage == NULL)
		{
			soc = (int)SDateTime::getNowSoc();
			SApi::UsSleep(10000);
			continue;
		}
		if(!PackageExplain(pPackage,wMsgType,pMsgHead,pAsdu,iAsduLen))
		{
			delete pPackage;
			continue;
		}
		
		last_soc = soc;
		if(file_size < 0)
			file_size = SString::GetAttributeValueI(pPackage->m_sHead,"filesize");
		if(iAsduLen > 0)
		{
			file.writeBlock(pAsdu,iAsduLen);			
		}
		if(pMsgHead->m_dwParamReault == 0)
		{
			if(last_sn == 0)
				last_sn = last_sn;
			//�ļ�����
			file.close();
			int real_size = file.size();
			if(real_size != file_size)
			{
				LOGWARN("ȡ�ļ�ʧ�ܣ��ļ���С��֤����!");
				file.remove();
				if(pPackage != NULL)
					delete pPackage;
				return -3;
			}
			//��֤ͨ��
			SFile::remove(sLocalPathFile);
			SFile::rename(sLocalPathFile+".tmp",sLocalPathFile);
			break;
		}
		else if((int)pMsgHead->m_dwParamReault != last_sn+1)
		{
			LOGWARN("�����ļ�ʱ��Ų�����,sn=%d,last_sn=%d",pMsgHead->m_dwParamReault,last_sn);
		}
		last_sn = pMsgHead->m_dwParamReault;
		if(pPackage != NULL)
			delete pPackage;
	}
	return file_size;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �ӱ����ļ���ȡ�ļ����ݲ��ϴ���ָ����Ԫ��ָ��Ŀ¼�ļ�λ��
// ��    ��:  �ۿ���
// ����ʱ��:  2016-2-20 13:31
// ����˵��:  @iUnitId��ʾ��Ԫ��ʶ 
//         :  @sPathFileΪĿ���ļ���ȫ·��������{@APP_PATH@}��ʾӦ�ó����Ŀ¼
//         :  @��ʾ��ʱʱ�䣬��λΪ��
//         :  @sLocalPathFile��ʾ����Դ�ļ���ȫ·��
// �� �� ֵ:  >=0��ʾ�ϴ��ɹ���<0��ʾ�ϴ�ʧ��
//////////////////////////////////////////////////////////////////////////
int CSpAgentClient::PutUnitFile(int iUnitId,SString sPathFile,SString sLocalPathFile,int iTimeout_s/*=5*/)
{
	SFile file(sLocalPathFile);
	if(!file.open(IO_ReadOnly))
	{
		LOGWARN("�浥Ԫ�ļ�ʱ�������ļ�ʧ��!file=%s",sLocalPathFile.data());
		return -1;
	}
	stuSpUnitAgentProcessId id;
	id.m_iUnitId = iUnitId;
	id.SetApplicationId(SP_UA_APPNO_AGENT);
	SString sReqHeadStr;
	int file_size = file.size();
	int size;
	DWORD dwMsgSn = NewMsgSn();
	if(file_size == 0)
	{
		sReqHeadStr.sprintf("filename=%s;filesize=%d;pice=%d;cont=%d;",sPathFile.data(),0,1,0);
		if(!SendAgentMsg(&id,SP_UA_MSG_REQ_PUT_FILE,dwMsgSn,0,NULL,0,&sReqHeadStr))
		{
			file.close();
			LOGERROR("���ļ�[%s]����ʧ��!������Ϣʧ��!",sPathFile.data());
			return -2;
		}
		return 0;
	}
	int pice=1;
	int ret;
	BYTE *pBuf = NULL;
	int file_size0 = file_size;
	while(file_size > 0)
	{
		size = file_size;
		if(size > 102400)
			size = 102400;
		if(pBuf == NULL)
			pBuf = new BYTE[size];
		ret = file.readBlock(pBuf,size);
		if(ret != size)
		{
			file.close();
			delete[] pBuf;
			LOGERROR("���ļ�[%s]����ʧ��!�������ļ�����ʱʧ��!",sLocalPathFile.data());
			return -3;
		}
		file_size -= size;
		sReqHeadStr.sprintf("filename=%s;filesize=%d;pice=%d;cont=%d;",sPathFile.data(),file_size0,pice++,file_size<=0?0:1);
		if(!SendAgentMsg(&id,SP_UA_MSG_REQ_PUT_FILE,dwMsgSn,0,pBuf,size,&sReqHeadStr))
		{
			file.close();
			delete[] pBuf;
			LOGERROR("���ļ�[%s]����ʧ��!������Ϣʧ��!",sPathFile.data());
			return -4;
		}
		if(GetSendPackagePtr()->count() > 1000)
			SApi::UsSleep(10000);
	}
	stuSTcpPackage *pPackage=NULL;
	int last_soc = (int)SDateTime::getNowSoc();
	int soc = last_soc;
	WORD wMsgType;
	stuSpUnitAgentMsgHead *pMsgHead;
	BYTE *pAsdu;
	int iAsduLen;
	while(abs(soc-last_soc) <= iTimeout_s)
	{
		pPackage = SearchRecvPackage_Processed(dwMsgSn,SP_UA_MSG_RES_PUT_FILE);
		if(pPackage == NULL)
		{
			soc = (int)SDateTime::getNowSoc();
			SApi::UsSleep(10000);
			continue;
		}
		if(!PackageExplain(pPackage,wMsgType,pMsgHead,pAsdu,iAsduLen))
		{
			delete pPackage;
			return -11;
		}
		DWORD res = pMsgHead->m_dwParamReault;
		delete pPackage;
		if(res == 0)
			return file_size0;
		else
			return -12;
	}
	return -13;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ָ����Ԫɾ��ָ�����ļ�
// ��    ��:  �ۿ���
// ����ʱ��:  2016-2-20 13:33
// ����˵��:  @iUnitIdΪ��Ԫ��ʶ
//         :  @sPathFileΪ��ɾ�����ļ�ȫ·��������{@APP_PATH@}��ʾӦ�ó����Ŀ¼
//         :  @��ʾ��ʱʱ�䣬��λΪ��
// �� �� ֵ:  >=0��ʾɾ���ɹ���<0��ʾɾ��ʧ��
//////////////////////////////////////////////////////////////////////////
int CSpAgentClient::DelUnitFile(int iUnitId,SString sPathFile,int iTimeout_s/*=5*/)
{
	stuSpUnitAgentProcessId id;
	id.m_iUnitId = iUnitId;
	id.SetApplicationId(SP_UA_APPNO_AGENT);
	SString sReqHeadStr;
	sReqHeadStr.sprintf("filename=%s;",sPathFile.data());	
	stuSTcpPackage *pPackage=NULL;
	if(!SendAgentMsgAndRecv(&id,SP_UA_MSG_REQ_DEL_FILE,NewMsgSn(),0,NULL,0,&sReqHeadStr,SP_UA_MSG_RES_DEL_FILE,pPackage,iTimeout_s))
	{
		LOGERROR("ɾ���ļ�[%s]ʧ��!",sPathFile.data());
		return -1;
	}
	WORD wMsgType;
	stuSpUnitAgentMsgHead *pMsgHead;
	BYTE *pAsdu;
	int iAsduLen;
	if(!PackageExplain(pPackage,wMsgType,pMsgHead,pAsdu,iAsduLen))
	{
		if(pPackage != NULL)
			delete pPackage;
		LOGERROR("ɾ���ļ�[%s]ʧ��!",sPathFile.data());
		return -2;
	}
	if(pMsgHead->m_dwParamReault != 0)
	{
		if(pPackage != NULL)
			delete pPackage;
		LOGWARN("ɾ���ļ�ʧ��!ret=%d",pMsgHead->m_dwParamReault);
		return -3;
	}
	return 1;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ����ָ�������Ľ��ձ���,���Ѵ����δ����ı���
// ��    ��:  �ۿ���
// ����ʱ��:  2016-2-20 13:39
// ����˵��:  
// �� �� ֵ:  
//////////////////////////////////////////////////////////////////////////
stuSTcpPackage* CSpAgentClient::SearchRecvPackage_Processed(DWORD sn,WORD wMsgType,WORD wMsgType2/*=0*/,bool bOnlyProcessed)
{
	unsigned long pos=0;
	stuSTcpPackage *pPackage;
	if(!bOnlyProcessed)
	{
		GetRecvPackagePtr()->lock();
		pPackage = GetRecvPackagePtr()->FetchFirst(pos);
		while (pPackage != NULL && IsConnected())
		{
			if(pPackage->m_wFrameType == wMsgType || (wMsgType2 != 0 && wMsgType2 == pPackage->m_wFrameType))
			{
				stuSpUnitAgentMsgHead *pMsg = (stuSpUnitAgentMsgHead*)pPackage->m_pAsduBuffer;
				if(pMsg != NULL && pPackage->m_iAsduLen >= (int)sizeof(stuSpUnitAgentMsgHead))
				{
					DWORD psn = pMsg->m_dwMsgSn;
					SKT_SWAP_DWORD(psn);
					if(psn == sn)
					{
						GetRecvPackagePtr()->remove(pPackage,false);
						GetRecvPackagePtr()->unlock();
						return pPackage;
					}
				}			
			}
			pPackage = GetRecvPackagePtr()->FetchNext(pos);
		}
		GetRecvPackagePtr()->unlock();
	}


	m_RecvPackage_Processed.lock();
	pPackage = m_RecvPackage_Processed.FetchFirst(pos);
	while(pPackage)
	{
		if(pPackage->m_wFrameType == wMsgType || (wMsgType2 != 0 && pPackage->m_wFrameType == wMsgType2))
		{
			stuSpUnitAgentMsgHead *pMsg = (stuSpUnitAgentMsgHead*)pPackage->m_pAsduBuffer;
			DWORD sn0 = pMsg->m_dwMsgSn;
			SKT_SWAP_DWORD(sn0);
			if(sn0 == sn)
			{
				m_RecvPackage_Processed.remove(pPackage,false);
				m_RecvPackage_Processed.unlock();
				return pPackage;
			}
		}
		pPackage = m_RecvPackage_Processed.FetchNext(pos);
	}
	m_RecvPackage_Processed.unlock();
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �������ʹ�����Ϣ���Ѽ���������Ӧ����OnRecvFrame������
// ��    ��:  �ۿ���
// ����ʱ��:  2016-2-3 16:05
// ����˵��:  @pToProcIdΪĿ����̱�ʶ
//         :  @wMsgTypeΪ��Ϣ���,101����ΪӦ�ò���Ϣ
//         :  @dwMsgSnΪ��ϢΨһ��ţ���ͨ����ȷ��Ӧ����
//         :  @dwParamReaultΪ�����򷵻�ֵ
//         :  @pAsduΪӦ�ò�ASDU���ݣ�NULL��ʾ��ASDU
//         :  @iAsduLenΪASDU����
//         :  @psHeadStrΪ��Ϣͷ�ַ���ָ�룬NULL��ʾ���ַ���
// �� �� ֵ:  true��ʾ��Ϣ�ɹ��ʹ�,false��ʾ������Ϣʧ�ܣ������������жϲ�Ŀ�겻�ɴ
//////////////////////////////////////////////////////////////////////////
bool CSpAgentClient::SendAgentMsg(stuSpUnitAgentProcessId *pToProcId,WORD wMsgType,DWORD dwMsgSn,DWORD dwParamReault/*=0*/,BYTE *pAsdu/*=NULL*/,int iAsduLen/*=0*/,SString *psHeadStr/*=NULL*/)
{
	stuSpUnitAgentMsgHead MsgHead/*,*pMsgHead*/;
	memcpy(&MsgHead.m_SrcId,&m_SelfProcId.process_id,(int)sizeof(stuSpUnitAgentProcessId));
	memcpy(&MsgHead.m_DstId,pToProcId,(int)sizeof(stuSpUnitAgentProcessId));
	MsgHead.m_dwMsgSn = dwMsgSn;
	MsgHead.m_dwParamReault = dwParamReault;
	stuSTcpPackage *pPackage = PackageBuild(wMsgType,&MsgHead,pAsdu,iAsduLen);
	if(psHeadStr != NULL)
		pPackage->m_sHead = *psHeadStr;
	//bool bOk = false;
	//WORD wRecvMsgTypeTmp;
	//GetRecvPackagePtr()->lock();
	AddToSend(pPackage);
// 	pPackage = NULL;
// 	int soc0 = (int)SDateTime::getNowSoc();
// 	int soc;
// 	while(1)
// 	{
// 		pPackage = SearchRecvPackage_Processed(dwMsgSn,SP_UA_MSG_SEND_OK,SP_UA_MSG_SEND_ERR,true);
// 		if(pPackage != NULL)
// 		{
// 			if(!PackageExplain(pPackage,wRecvMsgTypeTmp,pMsgHead,pAsdu,iAsduLen))
// 			{
// 				delete pPackage;
// 				continue;
// 			}
// 			if(pMsgHead->m_dwParamReault == (DWORD)wMsgType && pMsgHead->m_dwMsgSn == dwMsgSn)
// 			{
// 				bOk = (wRecvMsgTypeTmp == SP_UA_MSG_SEND_OK);
// 				if(!bOk)
// 				{
// 					LOGWARN("������Ϣ��ȷδ�ʹ�!");
// 				}
// 				break;
// 			}
// 			GetRecvPackagePtr()->append(pPackage);
// 			continue; 
// 		}
// 		soc = (int)SDateTime::getNowSoc();
// 		if(abs(soc-soc0) > 1)
// 			break;
// 		SApi::UsSleep(10000);
// 	}
	//GetRecvPackagePtr()->unlock();
// 	if(pPackage != NULL)
// 		delete pPackage;
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �������ʹ�����Ϣ����ָ��ʱ���ڽ��ջظ�
// ��    ��:  �ۿ���
// ����ʱ��:  2016-2-3 16:52
// ����˵��:  @pToProcIdΪĿ����̱�ʶ
//         :  @wMsgTypeΪ��Ϣ���,101����ΪӦ�ò���Ϣ
//         :  @dwMsgSnΪ��ϢΨһ��ţ���ͨ����ȷ��Ӧ����
//         :  @dwParamReaultΪ�����򷵻�ֵ
//         :  @pAsduΪӦ�ò�ASDU���ݣ�NULL��ʾ��ASDU
//         :  @iAsduLenΪASDU����
//         :  @psHeadStrΪ��Ϣͷ�ַ���ָ�룬NULL��ʾ���ַ���
//         :  @wRecvMsgTypeΪϣ�����յ�����Ӧ��������
//         :  @pRecvPackageΪ���յ�����Ӧ���ģ�������Ӧ�����ͷ�
//         :  @iTimeOut_sΪ���ճ�ʱʱ�䣬��λΪ��
// �� �� ֵ:  true��ʾ���Ͳ����ջظ��ɹ���false��ʾ���͡�����ջظ�ʧ��
//////////////////////////////////////////////////////////////////////////
bool CSpAgentClient::SendAgentMsgAndRecv(stuSpUnitAgentProcessId *pToProcId,WORD wMsgType,DWORD dwMsgSn,DWORD dwParamReault,BYTE *pAsdu,int iAsduLen,SString *psHeadStr,WORD wRecvMsgType,stuSTcpPackage* &pRecvPackage,int iTimeOut_s/*=10*/)
{
	stuSpUnitAgentMsgHead MsgHead,*pMsgHead;
	memcpy(&MsgHead.m_SrcId,&m_SelfProcId.process_id,(int)sizeof(stuSpUnitAgentProcessId));
	memcpy(&MsgHead.m_DstId,pToProcId,(int)sizeof(stuSpUnitAgentProcessId));
	MsgHead.m_dwMsgSn = dwMsgSn;
	MsgHead.m_dwParamReault = dwParamReault;
	stuSTcpPackage *pPackage = PackageBuild(wMsgType,&MsgHead,pAsdu,iAsduLen);
	if(psHeadStr != NULL)
		pPackage->m_sHead = *psHeadStr;
	bool bOk = false;
	//GetRecvPackagePtr()->lock();
	AddToSend(pPackage);
	pPackage = NULL;
	WORD wRecvMsgTypeTmp;
	int soc0 = (int)SDateTime::getNowSoc();
	int soc;
// 	while(1)
// 	{
// 		pPackage = SearchRecvPackage_Processed(dwMsgSn,SP_UA_MSG_SEND_OK,SP_UA_MSG_SEND_ERR,true);
// 		if(pPackage != NULL)
// 		{
// 			if(!PackageExplain(pPackage,wRecvMsgTypeTmp,pMsgHead,pAsdu,iAsduLen))
// 			{
// 				delete pPackage;
// 				continue;
// 			}
// 			if(pMsgHead->m_dwParamReault == (DWORD)wMsgType && pMsgHead->m_dwMsgSn == dwMsgSn)
// 			{
// 				bOk = (wRecvMsgTypeTmp == SP_UA_MSG_SEND_OK);
// 				if(!bOk)
// 				{
// 					LOGWARN("������Ϣ��ȷδ�ʹ�!");
// 				}
// 				break;
// 			}
// 			GetRecvPackagePtr()->append(pPackage);
// 			continue; 
// 		}
// 		soc = (int)SDateTime::getNowSoc();
// 		if(abs(soc-soc0) > 1)
// 			break;
// 		SApi::UsSleep(10000);
// 	}
// 	if(!bOk)
// 	{
// 		//GetRecvPackagePtr()->unlock();
// 		return false;
// 	}

	bOk = false;
	while(1)
	{
		pRecvPackage = SearchRecvPackage_Processed(dwMsgSn,wRecvMsgType);
		if(pRecvPackage != NULL)
		{
			if(!PackageExplain(pRecvPackage,wRecvMsgTypeTmp,pMsgHead,pAsdu,iAsduLen))
				continue;
			if(pMsgHead->m_dwMsgSn == dwMsgSn)
			{
				bOk = true;
				break;
			}
			continue;
		}
		soc = (int)SDateTime::getNowSoc();
		if(abs(soc-soc0) > iTimeOut_s)
			break;
		SApi::UsSleep(10000);
	}
	//GetRecvPackagePtr()->unlock();
	if(pPackage != NULL)
		delete pPackage;
	return bOk;
}

#if 0

CSAgentClient::CSAgentClient()
{
	m_sAgentIp = "127.0.0.1";
	m_iAgentPort = C_SP_UNITAGENT_PORT;
	m_SendList.setShared(true);
	m_SendList.setAutoDelete(true);
	m_pApp = NULL;
	m_iThreads = 0;
	m_pAgentSocket = NULL;
}

CSAgentClient::~CSAgentClient()
{
	if(m_pApp != NULL)
	{
		m_pApp->m_bHalt = true;
		while(m_iThreads > 0)
			SApi::UsSleep(1000);
	}
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �����������
// ��    ��:  �ۿ���
// ����ʱ��:  2012-5-31 10:13
// ����˵��:  @bBlockΪtrue��ʾ�������У�ֱ�������˳���false��ʾ�����������߳����д������
// �� �� ֵ:  true/false
//////////////////////////////////////////////////////////////////////////
bool CSAgentClient::StartAgent(bool bBlock)
{
	if(bBlock)
	{
		ThreadAgent(this);
	}
	else
	{
		SKT_CREATE_THREAD(ThreadAgent,this);
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �������ӵ�ָ���Ĵ��������
// ��    ��:  �ۿ���
// ����ʱ��:  2014-7-28 14:24
// ����˵��:  @ip�´����������IP��ַ
//         :  @portΪ�´����������TCP�˿ںţ�һ��Ϊ6666
// �� �� ֵ:  true/false
//////////////////////////////////////////////////////////////////////////
bool CSAgentClient::ReConnectAgentSvr(SString ip,int port)
{
	m_sAgentIp = ip;
	m_iAgentPort = port;
	if(m_pAgentSocket != NULL)
	{
		m_pAgentSocket->Close();
	}
	m_bLogined = false;
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �����̣߳�ִ�д��������߳�����ʱ��ռ��Run������Ҳ��ֱ������
// ��    ��:  �ۿ���
// ����ʱ��:  2012-5-30 11:00
// ����˵��:  @lp��this
// �� �� ֵ:  NULL
//////////////////////////////////////////////////////////////////////////
void* CSAgentClient::ThreadAgent(void* lp)
{
	CSAgentClient *pThis = (CSAgentClient*)lp;
	if(pThis->m_pApp == NULL)
	{
		LOGERROR("m_pAppδ����!");
		return NULL;
	}
	pThis->m_iThreads ++;
	//����ͨ��
	SSocket skt;
	SString sMsg,sRet;
	BYTE *pBuffer=NULL;
	int ret,iLength,i,cnt,from_handle;
	pThis->m_pAgentSocket = &skt;
	while(!pThis->m_pApp->m_bHalt)
	{
		pThis->m_bLogined = false;
		pThis->m_iCpuPercent = SApi::GetCpuUsedPercent();
		if(pThis->m_pApp != NULL && pThis->m_pApp->IsClosedExist())
		{
			LOGWARN("�յ��˳��ź�! ׼���رյ�ǰ����!");
			pThis->m_pApp->Stop();
			//׼���رյ�ǰ����
			pThis->m_pApp->RemoveClose();
			goto halt;
		}
		if(!skt.Create() || !skt.Connect(pThis->m_sAgentIp.data(),pThis->m_iAgentPort))
		{
			for(i=0;i<200 && !pThis->m_pApp->m_bHalt;i++)
				SApi::UsSleep(10000);
			continue;
		}
		//���͵�¼��Ϣ
		sMsg.sprintf("act=login;pid=%d;modname=%s;modver=%s;moddesc=%s;modcrc=%s;modtime=%s;",
			SApi::GetPid(),pThis->m_pApp->m_sModule.data(),pThis->m_pApp->m_sModuleVer.data(),pThis->m_pApp->m_sModuleDesc.data(),
			pThis->m_pApp->m_sModuleCrc.data(),pThis->m_pApp->m_sModuleTime.data());
		sRet = "";
		skt.SetTimeout(2000,2000);
		if( !pThis->SendMsg(&skt,sMsg) || 
			!pThis->ReceiveMsg(&skt,sRet,pBuffer,iLength) || 
			SString::GetAttributeValueI(sRet,"result") != 1)
		{
			skt.Close();
			if(pBuffer != NULL)
			{
				delete[] pBuffer;
				pBuffer = NULL;
			}
			for(i=0;i<100 && !pThis->m_pApp->m_bHalt;i++)
				SApi::UsSleep(10000);
			continue;
		}
		skt.SetTimeout(2,2000);
		if(pBuffer != NULL)
		{
			delete[] pBuffer;
			pBuffer = NULL;
		}
		time_t lastsoc = ::time(NULL);
		pThis->m_bLogined = true;
		while(!pThis->m_pApp->m_bHalt)
		{
			if(abs((long)(::time(NULL)-lastsoc)) > 2)
			{
				lastsoc = ::time(NULL);
				pThis->m_iCpuPercent = SApi::GetCpuUsedPercent();
			}
			//ѭ����������Ϣ����
			ret = pThis->ReceiveMsg(&skt,sMsg,pBuffer,iLength);
			if(ret < 0)
				break;
			if(ret > 0)
			{
				LOGDEBUG("�յ�������Ϣ:%s",sMsg.data());
				from_handle = SString::GetAttributeValueI(sMsg,"from_handle");
				if(!pThis->ProcessAgentMsgBase(sMsg,pBuffer,iLength))
					pThis->ProcessAgentMsg(from_handle,sMsg,pBuffer,iLength);
				if(pBuffer != NULL)
				{
					delete[] pBuffer;
					pBuffer = NULL;
				}
			}
			//�Ƿ�����Ҫ���͵ı���
			cnt = pThis->m_SendList.count();
			if(cnt == 0)
			{
				for(i=0;i<100;i++)
				{
					SApi::UsSleep(2000);
					if(skt.CheckForRecv() > 0 || pThis->m_SendList.count() > 0 || pThis->m_pApp->m_bHalt)
						break;
				}
			}
			else
			{
				for(i=0;i<cnt&&!pThis->m_pApp->m_bHalt;i++)
				{
					CAgentPackage *p = pThis->m_SendList[0];
					if(p == NULL)
						continue;
					if(pThis->SendMsg(&skt,p->sMsg,p->pBuffer,p->iLength) < 0)
						break;
					pThis->m_SendList.remove(0);
				}
			}
		}
	}
halt:
	pThis->m_pAgentSocket = NULL;
	pThis->m_iThreads --;
	return NULL;
}


////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ȡ��ǰ�Ѿ���¼������Ľ���ģ���б���Ϣ
// ��    ��:  �ۿ���
// ����ʱ��:  2011-3-29 9:55
// ����˵��:  ���÷��ص��б�
// �� �� ֵ:  >=0��ʾ��������,<0��ʾʧ��
//////////////////////////////////////////////////////////////////////////
int CSAgentClient::GetModuleList(SPtrList<stuModuleInfo> &module_list)
{
	SString sMsg = "act=ps;";
	m_sWaitMsgAct = "ps";
	m_sWaitMsg = "";
	this->AddMsgToAgent(sMsg);
	//int times = 0;
	int i;
	for(i=0;i<200;i++)
	{
		if(m_sWaitMsg.length() > 0)
			break;
		SApi::UsSleep(10000);
	}
	if(m_sWaitMsg.length() > 0)
	{
		SString key,sLine;
		//�յ��ظ�
		int size = SString::GetAttributeValueI(m_sWaitMsg,"size");
		for(i=0;i<size;i++)
		{
			key.sprintf("item%d",i+1);
			sLine = SString::GetAttributeValue(m_sWaitMsg,key);
			if(sLine.length() == 0)
				continue;
			stuModuleInfo *pNew = new stuModuleInfo();
			pNew->handle = SString::GetIdAttributeI(1,sLine,",");
			pNew->pid = SString::GetIdAttributeI(2,sLine,",");
			pNew->name = SString::GetIdAttribute(3,sLine,",");
			pNew->version = SString::GetIdAttribute(4,sLine,",");
			pNew->crc = SString::GetIdAttribute(5,sLine,",");
			pNew->compile_time = SString::GetIdAttribute(6,sLine,",");
			pNew->desc = SString::GetIdAttribute(7,sLine,",");
			pNew->login_ip = SString::GetIdAttribute(8,sLine,",");
			pNew->login_port = SString::GetIdAttributeI(9,sLine,",");
			key = SString::GetIdAttribute(10,sLine,",");
			pNew->login_time = SDateTime::makeDateTime("yyyy-MM-dd hh:mm:ss.zzz",key);
			module_list.append(pNew);
		}
	}
	else
	{
		return -1;
	}
	m_sWaitMsgAct = "";
	return module_list.count();
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ�������˵�������Ϣ
// ��    ��:  �ۿ���
// ����ʱ��:  2012-9-26 11:44
// ����˵��:  @sDisk ��ʾ��Ҫ��ȡ��Ӳ����Ϣ��disks=3;disk1=/dev/sda1;disk2=/dev/sda2;disk3=/dev/sda3;
//            @sRunInfo���÷��ص����ַ���������������Ϣ,act=runinfo;cpu_used=%d;mem_all=%d;mem_used=%d;mem_free=%d;disk1_all=%d;disk1_used=%d;disk1_free=%d;disk1_per=%d;
// �� �� ֵ:  1��ʾ�ɹ���0��ʾʧ��
//////////////////////////////////////////////////////////////////////////
bool CSAgentClient::GetRunInfo(SString sDisk,SString &sRunInfo)
{
	SString sMsg = "act=runinfo;"+sDisk;
	m_sWaitMsgAct = "runinfo";
	m_sWaitMsg = "";
	this->AddMsgToAgent(sMsg);
	int i;
	for(i=0;i<200;i++)
	{
		if(m_sWaitMsg.length() > 0)
			break;
		SApi::UsSleep(10000);
	}
	if(m_sWaitMsg.length() > 0)
	{
		sRunInfo = m_sWaitMsg;
	}
	else
	{
		return false;
	}
	m_sWaitMsgAct = "";
	return true;
}


////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡָ��ģ����ڴ�������ϵľ��
// ��    ��:  �ۿ���
// ����ʱ��:  2011-3-29 14:42
// ����˵��:  ģ�����ƣ����������ƣ�WIN32ƽ̨�ϲ���Ҫ��.exe
// �� �� ֵ:  =0��ʾģ��δ�ҵ���>0��ʾ���̾��
//////////////////////////////////////////////////////////////////////////
int CSAgentClient::GetModuleHandle(SString sModName)
{
	SPtrList<stuModuleInfo> module_list;
	module_list.setAutoDelete(true);
	if(GetModuleList(module_list) < 0)
		return 0;
	SString name2 = sModName+".exe";
	unsigned long pos;
	stuModuleInfo *pInfo = module_list.FetchFirst(pos);
	while(pInfo)
	{
		if(pInfo->name == sModName || pInfo->name == name2)
		{
			return pInfo->handle;
		}
		pInfo = module_list.FetchNext(pos);
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ָ����ģ�鷢��һ����Ϣ
// ��    ��:  �ۿ���
// ����ʱ��:  2011-3-29 11:06
// ����˵��:  to_handleΪĿ��ģ��ľ��
// �� �� ֵ:  ���ͳɹ�����ture��ʧ�ܷ���false
//////////////////////////////////////////////////////////////////////////
bool CSAgentClient::AddMsgToModule(int to_handle,SString &sMsg,BYTE* pBuffer/*=NULL*/,int iLength/*=0*/)
{
	sMsg.replace((char*)"act=",(char*)"act2=");
	if(sMsg.right(1) != ";")
		sMsg += ";";
	sMsg += "act=prox;to_handle="+SString::toString(to_handle)+";";
	AddMsgToAgent(sMsg,pBuffer,iLength);
	return true;
}

bool CSAgentClient::ProcessAgentMsgBase(SString &sMsg,BYTE* pBuffer/*=NULL*/,int iLength/*=0*/)
{
	if(m_pApp == NULL)
		return false;
	SString sRet = "";
	//LOGDEBUG("�յ�������Ϣ:%s",sMsg.data());
	SString act = SString::GetAttributeValue(sMsg,"act");
	if(m_sWaitMsgAct == act)
	{
		m_sWaitMsg = sMsg;
		return true;
	}
	if(act == "start")
	{
		LOGWARN("������ϢҪ����������!");
		if(m_pApp->IsStarted() == true)
		{
			LOGERROR("Ӧ���Ѿ������������������");
		}
		else
		{
			m_pApp->m_bHalt = false;
			m_pApp->SetStarted(m_pApp->Start());
		}
	}
	else if(act == "stop")
	{
		LOGWARN("������ϢҪ��رճ���!");
		if(m_pApp->IsStarted() == false)
		{
			LOGERROR("Ӧ����δ���������Թر����");
		}
		else
		{
			m_pApp->m_bHalt = true;
			m_pApp->Stop();
			m_pApp->SetStarted(false);
		}
	}
	else if(act == "restart")
	{
		LOGWARN("�յ�������ϢҪ������Ӧ��!");
		m_pApp->Stop();
		if(!m_pApp->Start())
		{
			m_pApp->SetStarted(false);
			LOGFAULT("����Ӧ��ʧ��!");
			return false;
		}
		else
		{
			m_pApp->SetStarted(true);
		}
	}
	else if(act == "log")
	{
		SString cmd = SString::GetAttributeValue(sMsg,"cmd");
		m_pApp->ParseCommandLine(cmd);
	}
	else if(act == "cpupercent")
	{
		sRet.sprintf("cpu=%d;%s",m_iCpuPercent,sMsg.data());
		AddMsgToAgent(sRet);
	}
	else if(act == "info")
	{
		//�ȸ���ģ����Ϣ
		ProcessAgentMsg(0,sMsg,pBuffer,iLength);

		sRet = "info="+m_pApp->m_sModuleInfo+";"+sMsg;
		AddMsgToAgent(sRet);
	}
	else if(act == "putfile")
	{
		//��ָ��Ŀ¼д���ļ�
		SString pathfile = SString::GetAttributeValue(sMsg,"pathfile");
		SFile file(pathfile);
		if(!file.open(IO_Truncate))
		{
			sRet += ";result=0;";
		}
		else
		{
			if(pBuffer != NULL && iLength > 0)
				file.writeBlock(pBuffer,iLength);
			file.close();
			sRet += ";result=1;";
		}
		AddMsgToAgent(sRet);
	}
	else if(act == "getfile")
	{
		//��ȡָ�����ļ�
		SString pathfile = SString::GetAttributeValue(sMsg,"pathfile");
		SFile file(pathfile);
		if(!file.open(IO_ReadOnly))
		{
			sRet += ";result=0;";
			AddMsgToAgent(sRet);
		}
		else
		{
			int size = file.size();
			if(size == 0)
			{
				sRet += ";result=1;";
				AddMsgToAgent(sRet);
			}
			else
			{
				BYTE *pFileBuf = new BYTE[size];
				if(file.readBlock(pFileBuf,size) != size)
				{
					sRet += ";result=0;";
					AddMsgToAgent(sRet);
					delete[] pFileBuf;
				}
				else
				{
					sRet += ";result=1;";
					AddMsgToAgent(sRet,pFileBuf,size);

				}
				file.close();
			}
		}
	}
	else
		return false;
	return true;
}

int CSAgentClient::ReceiveMsg(SSocket *pSocket,SString &sMsg,BYTE* &pBuffer,int &iLength)
{
	BYTE buf[16];
	int ret,len1,len2;
	ret = pSocket->Receive(buf,10);
	if(ret == 0)
		return 0;
	if(ret != 10)
	{
		return -1;
	}
	if(buf[0] != 0xEC || buf[1] != 0x57)
	{
		return -2;
	}
	memcpy(&len1,buf+2,4);
	memcpy(&len2,buf+6,4);

	//����ֽ���
	SKT_SWAP_DWORD(len1);
	SKT_SWAP_DWORD(len2);
	//SApi::Ysntoh(SKT_LITTLE_ENDIAN,(DWORD)len1);
	//SApi::Ysntoh(SKT_LITTLE_ENDIAN,(DWORD)len2);

	sMsg.SetBuffer(len1+1);
	char *pStr = sMsg.data();
	ret = pSocket->Receive(pStr,len1);
	sMsg.PrepareStrLen();
	if(ret != len1)
	{
		int times = 0;
		while(ret == 0 && times < 1000)
		{
			times++;
			SApi::UsSleep(10000);
			ret = pSocket->Receive(pStr,len1);
		}
		if(ret != len1)
			return -3;
	}
	pStr[len1] = '\0';
	if(len2 > 0)
	{
		if(len2 > iLength)
		{
			if(pBuffer != NULL)
				delete[] pBuffer;
			pBuffer = new BYTE[len2];
		}
		iLength = len2;
		ret = pSocket->Receive(pBuffer,len2);
		iLength = len2;
	}
	else
	{
		pBuffer = NULL;
		iLength = 0;
	}
	//LOGDEBUG("RECV:%s",sMsg.data());
	return len1;
}

int CSAgentClient::SendMsg(SSocket *pSocket,SString &sMsg,BYTE* pBuffer/*=NULL*/,int iLength/*=0*/)
{
	BYTE buf[16];
	int ret,len1,len2;
	buf[0] = 0xEC;
	buf[1] = 0x57;
	len1 = sMsg.length();
	len2 = iLength;

	//����ֽ���
	SKT_SWAP_DWORD(len1);
	SKT_SWAP_DWORD(len2);
	//SApi::Yshton(SKT_LITTLE_ENDIAN,(DWORD)len1);
	//SApi::Yshton(SKT_LITTLE_ENDIAN,(DWORD)len2);

	memcpy(buf+2,&len1,4);
	memcpy(buf+6,&len2,4);

	//����ֽ���
	SKT_SWAP_DWORD(len1);
	SKT_SWAP_DWORD(len2);
	//SApi::Yshton(SKT_LITTLE_ENDIAN,(DWORD)len1);
	//SApi::Yshton(SKT_LITTLE_ENDIAN,(DWORD)len2);

	ret = pSocket->Send(buf,10);
	if(ret != 10)
		return -1;

	ret = pSocket->Send(sMsg.data(),len1);
	if(ret != len1)
		return -2;
	if(len2 != 0 && pBuffer != NULL)
	{
		ret = pSocket->Send(pBuffer,len2);
		if(ret != len2)
			return -3;
	}
	//LOGDEBUG("SEND:%s",sMsg.data());
	return len1;
}
#endif// if 0

#endif//ifdef _WITHOUT_AGENT_CHANNEL_


//////////////////////////////////////////////////////////////////////////
// ��    ��:  SApplication
// ��    ��:  �ۿ���
// ����ʱ��:  2008-10-10 14:16
// ��    ��:  Ӧ�ó��������
//////////////////////////////////////////////////////////////////////////
SApplication* g_pApplicationPtr=NULL;
int g_iAppCount = 0;
SApplication::SApplication()
{
	g_iAppCount++;
	if(g_pApplicationPtr == NULL)
		g_pApplicationPtr = this;
	m_sAppbaseVer = SKTBASE_VERSION;
	m_iArgc = 0;
	m_ppArgv = NULL;
	m_bHalt = false;
	m_bDebugMode = false;
	m_bLogToConsole = true;
	m_bFork = true;
	m_bStarted = false;
	m_pUserPtr = NULL;
#ifndef _WITHOUT_AGENT_CHANNEL_
	m_pApp = this;
	m_sAgentIp = "127.0.0.1";
	m_iAgentPort = C_SP_UNITAGENT_PORT;
	m_bUseAgent = true;
#endif
	m_iApplicationId = 0;
	m_iAppInstId = 0;
	m_iUnitId = 0;
}

SApplication::~SApplication()
{
	if (g_iAppCount <= 1)
	{
		SLog::WaitForLogEmpty();
		SLog::quitLog();
	}
	g_iAppCount--;
	if(g_pApplicationPtr == this)
		g_pApplicationPtr = NULL;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡSSApplication��ʵ��ָ��
// ��    ��:  �ۿ���
// ����ʱ��:  2015-12-16 13:27
// ����˵��:  void
// �� �� ֵ:  SApplication*
//////////////////////////////////////////////////////////////////////////
SApplication* SApplication::GetPtr()
{
	return g_pApplicationPtr;
}

bool SApplication::ParseCommandLine(SString str)
{
	if(str == "log level none")
	{
		SLog::setLogLevel(SLog::LOG_NONE);
	}
	else if(str == "log level fault")
	{
		SLog::setLogLevel(SLog::LOG_FAULT);
	}
	else if(str == "log level error")
	{
		SLog::setLogLevel(SLog::LOG_ERROR);
	}
	else if(str == "log level warn")
	{
		SLog::setLogLevel(SLog::LOG_WARN);
	}
	else if(str == "log level debug")
	{
		SLog::setLogLevel(SLog::LOG_DEBUG);
	}
	else if(str == "log level basedebug")
	{
		SLog::setLogLevel(SLog::LOG_BASEDEBUG);
	}
	else if(str == "log to console on")
	{
		SLog::startLogToConsole();
	}
	else if(str == "log to console off")
	{
		SLog::stopLogToConsole();
	}
	else if(str.left(12) == "log to file ")
	{
		SLog::startLogToFile(str.mid(12).data());
	}
	else if(str.left(14) == "log file size ")
	{
		SLog::setLogFileMaxSize(str.mid(14).toInt());
	}
	else if(str.left(19) == "log file back size ")
	{
		SLog::setLogFileMaxBackFiles(str.mid(19).toInt());
	}
	else if(str == "log to file off")
	{
		SLog::stopLogToFile();
	}
	else if(str == "log to tcp off" || str == "log to TCP off")
	{
		SLog::stopLogToTCPSvr();
	}
	else if(str.left(11) == "log to tcp " || str.left(11) == "log to TCP ")
	{
		SString ip = str.mid(11);
		int port=ip.find(":");
		if(port > 0)
		{
			port = ip.mid(ip.find(":")+1).toInt();
			ip = ip.left(ip.find(":"));
		}
		SLog::startLogToTCPSvr(ip.data(),port);
	}
	else if(str == "log to udp off" || str == "log to UDP off")
	{
		SLog::stopLogToUDPSvr();
	}
	else if(str.left(11) == "log to udp " || str.left(11) == "log to UDP ")
	{
		SString ip = str.mid(11);
		int port=ip.find(":");
		if(port > 0)
		{
			port = ip.mid(ip.find(":")+1).toInt();
			ip = ip.left(ip.find(":"));
		}
		SLog::startLogToUDPSvr(ip.data(),port);
	}
	
	return true;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ����ģ��汾��
// ��    ��:  �ۿ���
// ����ʱ��:  2009-5-19 10:56
// ����˵��:  sVer(����:<��Ʒ�ڲ��ͺ�>.<�׶ΰ汾��>.<ģ��汾��>)
// �� �� ֵ:  NULL
//////////////////////////////////////////////////////////////////////////
void SApplication::SetVersion(SString sVer)
{
	m_sModuleVer = sVer;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ����ģ������
// ��    ��:  �ۿ���
// ����ʱ��:  2009-5-19 12:29
// ����˵��:  sDesc����
// �� �� ֵ:  NULL
//////////////////////////////////////////////////////////////////////////
void SApplication::SetModuleDesc(SString sDesc)
{
	m_sModuleDesc = sDesc;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ����ģ����Ϣ
// ��    ��:  �ۿ���
// ����ʱ��:  2009-5-19 12:29
// ����˵��:   sInfo = name=;ver=;crc=;date=
// �� �� ֵ:  NULL
//////////////////////////////////////////////////////////////////////////
void SApplication::SetModuleInfo(SString sInfo)
{
	m_sModuleInfo	= sInfo;
	m_sModule		= SString::GetAttributeValue(sInfo,"name");
	m_sModuleVer	= SString::GetAttributeValue(sInfo,"ver");
	m_sModuleCrc	= SString::GetAttributeValue(sInfo,"crc");
	m_sModuleTime	=SString::GetAttributeValue(sInfo,"date");
	if (m_sModuleVer.left(1).toLower() == "v")
		m_sModuleVer= m_sModuleVer.mid(1);
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ����Ӧ�ó����ʶ
// ��    ��:  �ۿ���
// ����ʱ��:  2016-2-3 8:49
// ����˵��:  @iAppId��ʾӦ�ú�
//         :  @iInstId��ʾʵ��ID
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SApplication::SetApplicationId(DWORD iAppId,DWORD iInstId)
{
	m_iApplicationId = iAppId;
	m_iAppInstId = iInstId;
}


////////////////////////////////////////////////////////////////////////
// ��    ��:  ����Ӧ�ó���
// ��    ��:  �ۿ���
// ����ʱ��:  2008-10-10 14:17
// ����˵��:  @argc/argvΪmain�������
//            @szDate/szTimeΪ��������ʱ��
//            @bBlockΪ�Ƿ��������У�trueΪ������falseΪ������
// �� �� ֵ:  true/false
//////////////////////////////////////////////////////////////////////////
bool SApplication::Run(int argc, char* argv[],const char* szDate,const char* szTime,bool bBlock/*=true*/)
{
	m_iArgc = argc;
	m_ppArgv = (char**)argv;
	m_sModule = argv[0];
	#ifndef _WITHOUT_AGENT_CHANNEL_
	m_sAgentIp = "127.0.0.1";
	#endif
	SString err;
	SString crc = SApi::GenerateCRCByFile(m_sModule,err);
	m_sModuleCrc = crc;
	m_sModuleTime.sprintf("%s %s",szDate,szTime);
	
	if(m_sModule.findRev('/') >= 0)
		m_sModule = m_sModule.mid(m_sModule.findRev('/')+1);
	if(m_sModule.findRev('\\') >= 0)
		m_sModule = m_sModule.mid(m_sModule.findRev('\\')+1);

	printf("#########################################################\n");
	printf("## SKTBASE Application frame\n");
	printf("## Module  Name: %s \n",m_sModule.data());
	printf("## Description : %s \n",m_sModuleDesc.data());
	printf("## Compile Time: %s %s\n",szDate,szTime);
	printf("## Appbase Ver : V%s \n",m_sAppbaseVer.data());
	printf("## Module  Ver : V%s \n",m_sModuleVer.data());
	printf("## CRC    Code : %s \n",crc.data());
	printf("#########################################################\n");

	SString sTemp="";
	if(IsParam("version",sTemp))
	{
		bool bWrite = false;
		SString sLine;
		SFile file;
		if(sTemp.length() > 0)
		{
			file.setFileName(sTemp);
			if(file.open(IO_Truncate))
				bWrite = true;
		}
		if(bWrite)
		{
			sLine.sprintf("#########################################################\n");
			file.writeString(sLine);
			sLine.sprintf("## SKTBASE Application frame\n");
			file.writeString(sLine);
			sLine.sprintf("## Module  Name: %s \n",m_sModule.data());
			file.writeString(sLine);
			sLine.sprintf("## Description : %s \n",m_sModuleDesc.data());
			file.writeString(sLine);
			sLine.sprintf("## Compile Time: %s %s\n",szDate,szTime);
			file.writeString(sLine);
			sLine.sprintf("## Appbase Ver : V%s \n",m_sAppbaseVer.data());
			file.writeString(sLine);
			sLine.sprintf("## Module  Ver : V%s \n",m_sModuleVer.data());
			file.writeString(sLine);
			sLine.sprintf("## CRC    Code : %s \n",crc.data());
			file.writeString(sLine);
			sLine.sprintf("#########################################################\n");
			file.writeString(sLine);
			file.close();
		}
		exit(0);
		abort();
	}
	if(IsParam("soft_type"))
	{
		m_sSoftwareType = GetParam("soft_type");
		printf("����ͺ�����Ϊ��%s",m_sSoftwareType.data());
	}

	if(m_sModuleDesc.length() == 0)
	{
		printf("ERROR!!! ȱ��ģ��������Ϣ!����û���CSRecApplication::SetModuleDesc����!\n\n\n");
	}
	if(m_sModuleVer.length() == 0)
	{
		printf("ERROR!!! ȱ��ģ��汾��Ϣ!����û���CSRecApplication::SetVersion����!\n\n\n");
	}
#ifndef WIN32
	signal(SIGPIPE,SIG_IGN);//���Թܵ��Ͽ��ź�
	signal(SIGCHLD,SIG_IGN);
	signal(SIGCLD, SIG_IGN);
#endif

#ifndef WIN32
	if(m_bFork)
	{
		//LINUX�½���������Ϊ�ػ�����
		//printf("@@@@@@@@@@@@begin fork()\n");
		if(fork())
		{
			//printf("@@@@@@@@@@@@fork()\n");
			exit(0);
		}
		SLog::init();
	}
	SApi::UsSleep(100000);
	signal(SIGCHLD,SIG_IGN);
#endif

	SLog::setLogLevel(SLog::LOG_DEBUG);
	SLog::setModuleName(m_sModule.data());

	//Ԥ��������
	SXmlConfig xml;
	SString sFileName;
	m_sExePath = SDir::currentDirPath();
	if(m_sExePath.right(1) != "/" && m_sExePath.right(1) != "\\")
	{
#ifdef WIN32
		m_sExePath += "\\";
#else
		m_sExePath += "/";
#endif
	}
	m_sHomePath = m_sExePath;
	while(m_sHomePath.right(1) == "\\" || m_sHomePath.right(1) == "/")
		m_sHomePath = m_sHomePath.left(m_sHomePath.length()-1);

	if(m_sHomePath.right(4) == "\\bin" || m_sHomePath.right(4) == "/bin")
		m_sHomePath = m_sHomePath.left(m_sHomePath.length()-4);
	else if (m_sHomePath.right(6) == "\\Debug" || m_sHomePath.right(6) == "/Debug")
		m_sHomePath = m_sHomePath.left(m_sHomePath.length()-6);
	else if (m_sHomePath.right(8) == "\\Release" || m_sHomePath.right(8) == "/Release")
		m_sHomePath = m_sHomePath.left(m_sHomePath.length()-8);

	while(m_sHomePath.right(1) == "\\" || m_sHomePath.right(1) == "/")
		m_sHomePath = m_sHomePath.left(m_sHomePath.length()-1);
#ifdef WIN32
	m_sHomePath += "\\";
	m_sConfPath = m_sHomePath + "conf\\";
#else
	m_sHomePath += "/";
	m_sConfPath = m_sHomePath + "conf/";
#endif

#if 1 //��ʽ�����ļ�
	//MEMO: �����½ṹ�����ļ��������Ӵ����� [2015-8-1 12:59 �ۿ���]
	sFileName=GetConfPath() + "sys_log.xml";
	if(!xml.ReadConfig(sFileName))
	{
		LOGWARN("��<sys_log.xml>�ļ�ʧ��!���޷�ʹ����־��¼����! file=%s",sFileName.data());
	}
	else
	{
		sFileName = m_sModule;
		SBaseConfig *pPrecommand = xml.SearchChild("log<module='"+sFileName+"'>");
		if(pPrecommand == NULL)
		{
			//ȥ��.exe����һ��
			if(sFileName.right(4).toLower() == ".exe")
				sFileName = sFileName.left(sFileName.length()-4);
			pPrecommand = xml.SearchChild("log<module='"+sFileName+"'>");
		}
		if(pPrecommand == NULL)//û��ר�õģ���ȱʡ��
			pPrecommand = xml.SearchChild("log<module=''>");
		if(pPrecommand == NULL)//û��ר�õģ���ȱʡ��
			pPrecommand = xml.SearchChild("log<module='*'>");
		if(pPrecommand != NULL)
		{
			//Ԥ��������
			/*
			<log level="debug">
				<concole open="true"/>
				<tcp open="true" ip="192.168.0.208" port="7001" />
				<file open="true" size="1024000" files="5" file="d:/sys/log/log_[@modulename].txt"/>
			*/
			SString level = pPrecommand->GetAttribute("level").toLower();
			if(level == "basedebug")
				SLog::setLogLevel(SLog::LOG_BASEDEBUG);
			else if(level == "debug")
				SLog::setLogLevel(SLog::LOG_DEBUG);
			else if(level == "warn")
				SLog::setLogLevel(SLog::LOG_WARN);
			else if(level == "error")
				SLog::setLogLevel(SLog::LOG_ERROR);
			else if(level == "fault")
				SLog::setLogLevel(SLog::LOG_FAULT);
			else if(level == "none" || level == "" || level == "null" || level == "no")
				SLog::setLogLevel(SLog::LOG_NONE);
			else
				SLog::setLogLevel(SLog::LOG_DEBUG);
			if(pPrecommand->SearchNodeAttribute("console","open").toLower() == "true")
				SLog::startLogToConsole();
			else
				SLog::stopLogToConsole();
			if(pPrecommand->SearchNodeAttribute("tcp","open").toLower() == "true")
			{
				SString ip = pPrecommand->SearchNodeAttribute("tcp","ip");
				int port = pPrecommand->SearchNodeAttributeI("tcp","port");
				SLog::startLogToTCPSvr(ip.data(),port);
			}
			if(pPrecommand->SearchNodeAttribute("file","open").toLower() == "true")
			{
				SString file = pPrecommand->SearchNodeAttribute("file","file");
				file = file.replace("[@modulename]",m_sModule);
				int size = pPrecommand->SearchNodeAttributeI("file","size");
				int files = pPrecommand->SearchNodeAttributeI("file","files");
				if(files < 1)
				{
					printf("��Ч����־�ļ�������:%d����ǿ����Ϊ1��\n",files);
					files = 1;
				}
				if(size < 1024)
				{
					printf("��С����־�ļ���С����:%d,��ǿ����Ϊ1024!\n",size);
					size = 1024;
				}
				if(size > 104857600)//100M
				{
					printf("�������־�ļ���С����:%d,��ǿ����Ϊ104857600!",size);
					size = 104857600;
				}
				SLog::startLogToFile(file.data());
				SLog::setLogFileMaxSize(size);
				SLog::setLogFileMaxBackFiles(files);
			}
		}
	}
#else //��ʽ�����ļ�
	sFileName=GetConfPath() + "log_config.xml";
	if(!xml.ReadConfig(sFileName))
	{
		LOGWARN("��<log_config.xml>�ļ�ʧ��!���޷�ʹ����־��¼����! file=%s",sFileName.data());
	}
	else
	{
		sFileName = m_sModule;
		SBaseConfig *pPrecommand = xml.SearchChild("log<module='"+sFileName+"'>");
		if(pPrecommand == NULL)
		{
			if(sFileName.right(4).toLower() == ".exe")
				sFileName = sFileName.left(sFileName.length()-4);
			pPrecommand = xml.SearchChild("log<module='"+sFileName+"'>");
		}

		if(pPrecommand == NULL)
		{
			//û��ר�õģ���ȱʡ��
			pPrecommand = xml.SearchChild("log<module=''>");
		}
		if(pPrecommand != NULL)
		{
			//Ԥ��������
			int i,cnt = pPrecommand==NULL?0:pPrecommand->GetChildCount("command");
			for(i=0;i<cnt;i++)
			{
				SBaseConfig *pCommand = pPrecommand->GetChildNode(i,"command");
				if(pCommand == NULL )
					continue;
				SString cmd = pCommand->GetNodeValue();
				if(cmd.find("[@modulename]") >= 0)
				{
					SString sModule = SLog::getModuleName();
					cmd = cmd.replace("[@modulename]",sModule);
				}
				ParseCommandLine(cmd);
			}
		}
	}
#endif

	//�Ƿ��ڵ���ģʽ
	if(IsParam("debug"))
		m_bDebugMode = true;

	if(IsParam("console",sTemp))
	{
		sTemp = sTemp.toLower();
		if(sTemp == "off")
		{
			//ǿ�ƹرտ���̨����־���
			m_bLogToConsole = false;
			ParseCommandLine("log to console off");
		}
	}
#ifndef _WITHOUT_AGENT_CHANNEL_
	if(IsParam("agentport",sTemp))
	{
		m_iAgentPort = sTemp.toInt();
		if(m_iAgentPort == 0)
			m_iAgentPort = C_SP_UNITAGENT_PORT;
	}
#endif
	if(IsParam("stop"))
	{
		if(!_Stop())
		{
			LOGFAULT("ֹͣӦ��ʧ��!");
			SLog::WaitForLogEmpty();
			return false;
		}
		return true;
	}
	else if(IsParam("restart"))
	{
		_Stop();
		if(!_Start())
		{
			LOGFAULT("����Ӧ��ʧ��!");
			SLog::WaitForLogEmpty();
			return false;
		}
	}
	else
	{
		if(!_Start())
		{
			LOGFAULT("����Ӧ��ʧ��!");
			SLog::WaitForLogEmpty();
			return false;
		}
	}
	LOGDEBUG("Ӧ�÷����������!");

	if(bBlock)
	{
		//������ʽ����
#ifndef _WITHOUT_AGENT_CHANNEL_
		if(m_bUseAgent)
			StartAgent(bBlock);
#else
		if(0)
		{

		}
#endif
		else
		{
			while(!m_bHalt)
			{
				//�ȴ����˷����˳��ź�
				if(IsClosedExist())
				{
					LOGWARN("�յ��˳��ź�! ׼���رյ�ǰ����!");
					Stop();
					//׼���رյ�ǰ����
					this->RemoveClose();
					break;
				}
				SApi::UsSleep(2000000);
			}
		}
		Quit();
		LOGDEBUG("Ӧ�÷������˳�!");
	}
	else
	{
		//��������ʽ����
#ifndef _WITHOUT_AGENT_CHANNEL_
		StartAgent(bBlock);
#endif
	}
	return true;
}


bool SApplication::IsParam(SString sParamName)
{
	SString sLine;
	for(int i=1;i<m_iArgc;i++)
	{
		sLine = m_ppArgv[i];
		if(sLine.left(1) == "-")
			sLine = sLine.mid(1);
		if(SString::GetIdAttribute(1,sLine,"=") == sParamName)
			return true;
	}
	return false;
}
bool SApplication::IsParam(SString sParamName,SString &sParamValue)
{
	SString sLine;
	for(int i=1;i<m_iArgc;i++)
	{
		sLine = m_ppArgv[i];
		if(sLine.left(1) == "-")
			sLine = sLine.mid(1);
		if(SString::GetIdAttribute(1,sLine,"=") == sParamName)
		{
			sParamValue = SString::GetIdAttribute(2,sLine,"=");
			return true;
		}
	}
	return false;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ�����в�������
// ��    ��:  �ۿ���
// ����ʱ��:  2014-5-19 10:13
// ����˵��:  @sParamNameΪ������
// �� �� ֵ:  ����ֵ
//////////////////////////////////////////////////////////////////////////
SString SApplication::GetParam(SString sParamName)
{
	SString sLine;
	for(int i=1;i<m_iArgc;i++)
	{
		sLine = m_ppArgv[i];
		if(sLine.left(1) == "-")
			sLine = sLine.mid(1);
		if(SString::GetIdAttribute(1,sLine,"=") == sParamName)
		{
			return SString::GetIdAttribute(2,sLine,"=");
		}
	}
	return "";
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ���������Ͳ�������
// ��    ��:  �ۿ���
// ����ʱ��:  2014-5-19 10:13
// ����˵��:  @sParamNameΪ������
// �� �� ֵ:  ����ֵ
//////////////////////////////////////////////////////////////////////////
int SApplication::GetParamI(SString sParamName)
{
	return GetParam(sParamName).toInt();
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ�����и����Ͳ�������
// ��    ��:  �ۿ���
// ����ʱ��:  2014-5-19 10:13
// ����˵��:  @sParamNameΪ������
// �� �� ֵ:  ����ֵ
//////////////////////////////////////////////////////////////////////////
float SApplication::GetParamF(SString sParamName)
{
	return GetParam(sParamName).toFloat();
}

bool SApplication::TouchClose()
{
#ifdef WIN32
	SFile file(m_sExePath+"_skt_close_"+m_sModule);
#else
	SFile file("/proc/.skt_close_"+m_sModule);
#endif
	if(!file.open(IO_Truncate))
		return false;
	file.close();
	return true;
}

bool SApplication::RemoveClose()
{
#ifdef WIN32
	return SFile::remove(m_sExePath+"_skt_close_"+m_sModule);
#else
	return SFile::remove("/proc/.skt_close_"+m_sModule);
#endif
}

bool SApplication::IsClosedExist()
{
#ifdef WIN32
	SFile file(m_sExePath+"_skt_close_"+m_sModule);
#else
	SFile file("/proc/.skt_close_"+m_sModule);
#endif
	if(!file.open(IO_ReadOnly))
		return false;
	file.close();
	return true;
}

bool SApplication::SearchOldProcess(int &pid,int &ppid,SString &name)
{
	//�����Ƿ����
	SStringList plist;//pid,ppid,name;....
	SString sLine;
	int p,i,cnt = SApi::GetSystemProcess(plist);
	for(i=0;i<cnt;i++)
	{
		sLine = *plist[i];
		//LOGDEBUG("MOUDLE:%s",sLine.data());
		pid = SString::GetIdAttributeI(1,sLine,",");
		ppid= SString::GetIdAttributeI(2,sLine,",");
		name= SString::GetIdAttribute( 3,sLine,",");
#ifdef WIN32
		name += ".exe";
#endif

		if(pid == SApi::GetPid())
			continue;//��������Ľ���
		p = name.find(m_sModule);
		if(p < 0)
			continue;
		if(name.length() == p+m_sModule.length() ||
			 name.at(p+m_sModule.length()) == ' ')
		{
			LOGDEBUG("�õ���ǰ���̶�Ӧ�Ľ���!");
			return true;
		}
	}
	return false;
}

bool SApplication::_Start()
{
	if(m_bStarted)
		return false;

	//�����Ƿ����
	int pid,ppid;
	SString name;
	if(GetParamI("unique") == 1 && SearchOldProcess(pid,ppid,name)/* && m_sModule.Find("watch") < 0*/)
	{
		LOGWARN("��ǰ���̵���һ��ʵ��������(thispid:%d,pid:%d,ppid%d,name:%s),��ǰ���̽���ֹ����!!!",SApi::GetPid(),pid,ppid,name.data());
		return false;
	}
	if(Start())
	{
		m_bStarted = true;
		return true;
	}
	else
	{
		return false;
	}
}

bool SApplication::_Stop()
{
	if(!m_bStarted)
		return false;

	//�����Ƿ����
	int pid,ppid;
	SString name;
	if(GetParamI("unique") == 1 && SearchOldProcess(pid,ppid,name)/* && m_sModule.Find("watch") < 0*/)
		return false;

	//֪ͨpid���̹ر��������
	TouchClose();
	//�ȴ����
	time_t begin = ::time(NULL);
	while(1)
	{
		SApi::UsSleep(100000);
		if(!IsClosedExist())
			break;
		if(::time(NULL) - begin > 10)
		{
			LOGWARN("�ȴ����̹رճ�ʱ����ֱ��ɱ������(pid=%d,name=%s)",pid,name.data());
			SApi::SystemKillProcess(pid);
			RemoveClose();
			break;
		}
	}
	if(!IsClosedExist())
	{
		m_bStarted = false;
		return true;
	}
	else
	{
		RemoveClose();
		return false;
	}
}
