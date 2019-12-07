/**
 *
 * �� �� �� : STcpServerBase.cpp
 * �������� : 2011-11-8 17:54
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : 
 * �޸ļ�¼ : 
 *            $Log: $
 *
 **/


#include "STcpServerBase.h"


//////////////////////////////////////////////////////////////////////////
// ��    ��:  STcpServerSession
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-11 8:46
// ��    ��:  ����Ự��
//////////////////////////////////////////////////////////////////////////

STcpServerSession::STcpServerSession()
{
	m_SendPackage.setAutoDelete(true);
	m_RecvPackage.setAutoDelete(false);
	m_SendPackage.setShared(true);
	m_RecvPackage.setShared(true);
	m_pSocket = NULL;
	m_pUserPtr = m_pUserPtr2 = NULL;
	m_ValidWord = 0xeb903469;
}

STcpServerSession::~STcpServerSession()
{
	m_ValidWord = 0;
	if(m_pSocket != NULL && m_pSocket->IsConnected())
		m_pSocket->Close();

	lock();
	if(m_pSocket != NULL)
		delete m_pSocket;
	m_SendPackage.setAutoDelete(true);
	m_RecvPackage.setAutoDelete(true);
	m_SendPackage.clear();
	m_RecvPackage.clear();
	unlock();
}


////////////////////////////////////////////////////////////////////////
// ��    ��:  ������ݵ����Ͷ��У��ڷ�����ɺ��ͷ�pPackage����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-10 12:40
// ����˵��:  @pPackage�����ͱ���
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void STcpServerSession::AddToSend(stuSTcpPackage *pPackage)
{
	if(m_SendPackage.count() > m_pServer->m_iSendBufferPkgs)
	{
		static int last_soc = 0;
		if((int)::time(NULL) != last_soc)
		{
			last_soc = (int)::time(NULL);
			LOGWARN("�����ж���(>%d)������ʧ��������!",m_pServer->m_iSendBufferPkgs);
		}
		delete pPackage;
	}
	else
	{
		m_SendPackage.append(pPackage);
	}
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ������ݰ��Ľ��ն���
// ��    ��:  �ۿ���
// ����ʱ��:  2016-2-13 16:32
// ����˵��:  @pPackage�����ͱ���
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void STcpServerSession::AddToRecv(stuSTcpPackage *pPackage)
{
	if(m_RecvPackage.count() > m_pServer->m_iRecvBufferPkgs)
	{
		static int last_soc = 0;
		if((int)::time(NULL) != last_soc)
		{
			last_soc = (int)::time(NULL);
			LOGWARN("�����ж���(>%d)������ʧ��������!",m_pServer->m_iRecvBufferPkgs);
		}
		delete pPackage;
	}
	else
	{
		m_RecvPackage.append(pPackage);
	}
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ǰ�Ự�Զ˷���һ֡���ģ�ֻ����OnRecvFrame������ʹ�ã������̲߳���ȫ
// ��    ��:  �ۿ���
// ����ʱ��:  2012-01-17 13:46
// ����˵��:  @pPackage��ʾ�����͵����ݰ�
//            @bAutoDelPackage��ʾ�Ƿ���ʹ����Ϻ��Զ�ɾ��pPackage
// �� �� ֵ:  true��ʾ���ͳɹ�,false��ʾ����ʧ��
//////////////////////////////////////////////////////////////////////////
bool STcpServerSession::SendFrame(stuSTcpPackage *pPackage,bool bAutoDelPackage/*=false*/)
{
	bool ret = STcpCommBase::SendFrame(m_pSocket,pPackage);
#ifdef _DEBUG
// 	LOGDEBUG("SEND %s->%d(len:%d,type=%d,ret=%d):%s", m_pSocket->GetPeerIp().data(),m_pServer->m_iServerPort,
// 		pPackage->m_iAsduLen, pPackage->m_wFrameType, ret, pPackage->m_sHead.left(1000).data());
#endif
	if(bAutoDelPackage)
		delete pPackage;
	return ret;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ǰ�Ự�Զ���������һ֡���ģ���ͨ�����ͻ���������ֻ����OnRecvFrame������ʹ�ã������̲߳���ȫ
// ��    ��:  �ۿ���
// ����ʱ��:  2012-01-17 13:57
// ����˵��:  @sHead��ʾ����ͷ�ַ���
//            @wFrameType��ʾ��������
//            @pAsduBuffer��ʾASDU���ݻ���������pAsduBufferΪNULL��iAsduLen��Ϊ0ʱ����ʾʹ��Ԥ��ķ��ͻ�����
//            @iAsduLen��ʾASDU���ݳ���
// �� �� ֵ:  true��ʾ���ͳɹ�,false��ʾ����ʧ��
//////////////////////////////////////////////////////////////////////////
bool STcpServerSession::SendFrame(SString &sHead,WORD wFrameType/*=0*/,BYTE *pAsduBuffer/*=NULL*/,int iAsduLen/*=0*/)
{
	bool ret = STcpCommBase::SendFrame(m_pSocket,sHead,wFrameType,pAsduBuffer,iAsduLen);
#ifdef _DEBUG
// 	LOGDEBUG("SEND %s->%d(len:%d,type=%d,ret=%d):%s", m_pSocket->GetPeerIp().data(), m_pServer->m_iServerPort,
// 		iAsduLen, wFrameType, ret, sHead.left(1000).data());
#endif
	return ret;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ǰ�Ự�Զ���������һ֡���ģ���ͨ�����ͻ���������ֻ����OnRecvFrame������ʹ�ã������̲߳���ȫ
// ��    ��:  �ۿ���
// ����ʱ��:  2012-01-17 13:57
// ����˵��:  @wFrameType��ʾ��������
//            @pAsduBuffer��ʾASDU���ݻ���������pAsduBufferΪNULL��iAsduLen��Ϊ0ʱ����ʾʹ��Ԥ��ķ��ͻ�����
//            @iAsduLen��ʾASDU���ݳ���
// �� �� ֵ:  true��ʾ���ͳɹ�,false��ʾ����ʧ��
//////////////////////////////////////////////////////////////////////////
bool STcpServerSession::SendFrame(WORD wFrameType,BYTE *pAsduBuffer/*=NULL*/,int iAsduLen/*=0*/)
{
	bool ret = STcpCommBase::SendFrame(m_pSocket,wFrameType,pAsduBuffer,iAsduLen);
#ifdef _DEBUG
// 	LOGDEBUG("SEND %s->%d(len:%d,type=%d,ret=%d):%s", m_pSocket->GetPeerIp().data(), m_pServer->m_iServerPort,
// 		iAsduLen, wFrameType, ret,"");
#endif
	return ret;
}


////////////////////////////////////////////////////////////////////////
// ��    ��:  �������ն����еı���
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-10 21:32
// ����˵��:  
// �� �� ֵ:  stuSTcpPackage*
//////////////////////////////////////////////////////////////////////////
stuSTcpPackage* STcpServerSession::SearchRecvPackage()
{
	stuSTcpPackage *pPackage = m_RecvPackage[0];
	if(pPackage != NULL)
	{
		m_RecvPackage.remove(pPackage);
	}
	return pPackage;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ͨ������ͷ�ַ����е�act���Լ������ն����еı���
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-10 21:32
// ����˵��:  @act��ʾ����ı���
// �� �� ֵ:  stuSTcpPackage*
//////////////////////////////////////////////////////////////////////////
stuSTcpPackage* STcpServerSession::SearchRecvPackage(SString act)
{
	unsigned long pos;
	m_RecvPackage.lock();
	stuSTcpPackage *pPackage = m_RecvPackage.FetchFirst(pos);
	while(pPackage != NULL)
	{
		if(SString::GetAttributeValue(pPackage->m_sHead,"act") == act)
		{
			m_RecvPackage.remove(pPackage,false);
			m_RecvPackage.unlock();
			return pPackage;
		}
		pPackage = m_RecvPackage.FetchNext(pos);
	}
	m_RecvPackage.unlock();
	return NULL;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ͨ���������ͼ������ն����еı���
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-10 21:32
// ����˵��:  @wFrameType��ʾ
// �� �� ֵ:  stuSTcpPackage*
//////////////////////////////////////////////////////////////////////////
stuSTcpPackage* STcpServerSession::SearchRecvPackage(WORD wFrameType)
{
	unsigned long pos=0;
	m_RecvPackage.lock();
	stuSTcpPackage *pPackage = m_RecvPackage.FetchFirst(pos);
	while(pPackage != NULL)
	{
		if(pPackage->m_wFrameType == wFrameType)
		{
			m_RecvPackage.remove(pPackage,false);
			m_RecvPackage.unlock();
			return pPackage;
		}
		pPackage = m_RecvPackage.FetchNext(pos);
	}
	m_RecvPackage.unlock();
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  STcpServerBase
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-8 19:41
// ��    ��:  TCPͨѶ�ͻ��˻���
//////////////////////////////////////////////////////////////////////////
STcpServerBase::STcpServerBase()
{
	m_Sessions.setShared(true);
	m_Sessions.setAutoDelete(true);
	m_iMaxSessionsAllowed = 100;
	m_bSessionOnThread = true;
	m_iSendBufferPkgs = 10000;
	m_iRecvBufferPkgs = 10000;
	m_bListened = false;
}

STcpServerBase::~STcpServerBase()
{
	
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��������
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-8 19:41
// ����˵��:  @port��ʾ�˿�,@ip��ʾ�����IP��ַ
// �� �� ֵ:  true��ʾ���������ɹ�,false��ʾ��������ʧ��
//////////////////////////////////////////////////////////////////////////
bool STcpServerBase::Start(int port,SString ip/*=""*/)
{
	m_sServerIp = ip;
	m_iServerPort = port;
	if(!SService::Start())
	{
		return false;
	}
	SKT_CREATE_THREAD(ThreadListen,this);
	if(m_bSessionOnThread == false)
	{
		SKT_CREATE_THREAD(ThreadServiceAll,this);
	}
	return true;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ֹͣ����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-8 19:41
// ����˵��:  void
// �� �� ֵ:  true��ʾ����ֹͣ�ɹ�,false��ʾ����ֹͣʧ��
//////////////////////////////////////////////////////////////////////////
bool STcpServerBase::Stop()
{
	m_Socket.Close();
	if(!SService::Stop())
	{
		return false;
	}
	return true;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ������ݵ����Ͷ��У��ڷ�����ɺ��ͷ�pPackage����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-10 12:40
// ����˵��:  @pPackage�����ͱ���
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void STcpServerBase::AddToSend(stuSTcpPackage *pPackage)
{
	unsigned long pos;
	m_Sessions.lock();
	STcpServerSession *p2,*pSession = m_Sessions.FetchFirst(pos);
	while(pSession)
	{
		p2 = m_Sessions.FetchNext(pos);
		if(p2 == NULL)
		{
			pSession->AddToSend(pPackage);
			pPackage = NULL;
		}
		else
		{
			pSession->AddToSend(NewPackage(pPackage));
		}
		pSession = p2;
	}
	m_Sessions.unlock();
	if(pPackage != NULL)
		delete pPackage;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  �������ն����еı���
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-10 21:32
// ����˵��:  @pSessionΪ���õĻỰָ��
// �� �� ֵ:  stuSTcpPackage*
//////////////////////////////////////////////////////////////////////////
stuSTcpPackage* STcpServerBase::SearchRecvPackage(STcpServerSession* &pSession)
{
	unsigned long pos;
	stuSTcpPackage *pPackage;
	m_Sessions.lock();
	STcpServerSession *pSession0 = m_Sessions.FetchFirst(pos);
	while(pSession0)
	{
		pPackage = pSession0->SearchRecvPackage();
		if(pPackage != NULL)
		{
			pSession = pSession0;
			pSession->m_RecvPackage.remove(pPackage);
			m_Sessions.unlock();
			return pPackage;
		}
		pSession0 = m_Sessions.FetchNext(pos);
	}
	m_Sessions.unlock();
	return NULL;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ͨ������ͷ�ַ����е�act���Լ������ն����еı���
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-10 21:32
// ����˵��:  @act��ʾ����ı���
//            @pSessionΪ���õĻỰָ��
// �� �� ֵ:  stuSTcpPackage*
//////////////////////////////////////////////////////////////////////////
stuSTcpPackage* STcpServerBase::SearchRecvPackage(SString act,STcpServerSession* &pSession)
{
	unsigned long pos;
	stuSTcpPackage *pPackage;
	m_Sessions.lock();
	STcpServerSession *pSession0 = m_Sessions.FetchFirst(pos);
	while(pSession0)
	{
		pPackage = pSession0->SearchRecvPackage(act);
		if(pPackage != NULL)
		{
			pSession = pSession0;
			pSession->m_RecvPackage.remove(pPackage);
			m_Sessions.unlock();
			return pPackage;
		}
		pSession0 = m_Sessions.FetchNext(pos);
	}
	m_Sessions.unlock();
	return NULL;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ͨ���������ͼ������ն����еı���
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-10 21:32
// ����˵��:  @wFrameType��ʾ
//            @pSessionΪ���õĻỰָ��
// �� �� ֵ:  stuSTcpPackage*
//////////////////////////////////////////////////////////////////////////
stuSTcpPackage* STcpServerBase::SearchRecvPackage(WORD wFrameType,STcpServerSession* &pSession)
{
	unsigned long pos;
	stuSTcpPackage *pPackage;
	m_Sessions.lock();
	STcpServerSession *pSession0 = m_Sessions.FetchFirst(pos);
	while(pSession0)
	{
		pPackage = pSession0->SearchRecvPackage(wFrameType);
		if(pPackage != NULL)
		{
			pSession = pSession0;
			pSession->m_RecvPackage.remove(pPackage);
			m_Sessions.unlock();
			return pPackage;
		}
		pSession0 = m_Sessions.FetchNext(pos);
	}
	m_Sessions.unlock();
	return NULL;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  �����֡���߳�
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-8 19:41
// ����˵��:  this
// �� �� ֵ:  NULL
//////////////////////////////////////////////////////////////////////////
void* STcpServerBase::ThreadListen(void* lp)
{
	STcpServerBase *pThis = (STcpServerBase*)lp;
	pThis->BeginThread();
	SSocket *pS = &pThis->m_Socket;
	SSocket *pNewS = NULL;
	stuSTcpPackage *pPackage=NULL;
	int ret;
	SString sHead;
	SString sPeerIp;
	int iPeerPort;

	while(!pThis->IsQuit())
	{
		pThis->m_bListened = false;
		pS->CreateTcp();
		if( !pS->Bind(pThis->m_iServerPort,pThis->m_sServerIp.length()==0?NULL:pThis->m_sServerIp.data()) || 
			!pS->Listen())
		{
			LOGERROR("�󶨷���˿�(%s:%d)ʱʧ��!",pThis->m_sServerIp.data(),pThis->m_iServerPort);
			for(int i=0;!pThis->IsQuit() && i<100;i++)
				SApi::UsSleep(50000);
			continue;
		}
		pThis->m_bListened = true;
		LOGDEBUG("�󶨷���˿�(%s:%d)�ɹ�!",pThis->m_sServerIp.data(),pThis->m_iServerPort);
		while(!pThis->IsQuit())
		{
			SApi::UsSleep(100000);
			if(pNewS != NULL)
			{
				pNewS->Close();
				delete pNewS;
			}
			pNewS = new SSocket();
			if(!pS->Accept(*pNewS))
			{
				if (pThis->IsQuit())
					break;
				LOGERROR("�ڶ˿�(%d)��������ʱʧ��!",pThis->m_iServerPort);
				break;
			}
			sPeerIp = pNewS->GetPeerIp();
			iPeerPort = pNewS->GetPeerPort();
			LOGWARN("�õ��µĻỰ����!(%s:%d --> %s:%d)",
					sPeerIp.data(),iPeerPort,
					pThis->m_sServerIp.data(),pThis->m_iServerPort);
			//��֤��¼
			if(pPackage != NULL)
			{
				delete pPackage;
				pPackage = NULL;
			}
			pNewS->SetTimeout(2000,5000);
			ret = pThis->RecvFrame(pNewS,pPackage);
			if (ret <= 0 || pPackage == NULL)
			{
				if(ret == 0)
				{
					if(pThis->OnNewTimeoutSocket(pNewS))
					{
						LOGDEBUG("����(%s:%d --> %s:%d)��Ӧ�ò��й�!",
							sPeerIp.data(),iPeerPort,
							pThis->m_sServerIp.data(), pThis->m_iServerPort);
						SApi::UsSleep(500000);
						continue;//SOCKET���й�
					}
				}
				LOGDEBUG("��֤��¼ʧ�ܣ����ܾ�����!(%s:%d --> %s:%d)",
					sPeerIp.data(),iPeerPort,
					pThis->m_sServerIp.data(), pThis->m_iServerPort);
				SApi::UsSleep(500000);
				continue;
			}
			if (SString::GetAttributeValue(pPackage->m_sHead, "act") != "login")
			{
				LOGDEBUG("��֤��¼ʧ�ܣ����ܾ�����!(%s:%d --> %s:%d)",
					sPeerIp.data(),iPeerPort,
					pThis->m_sServerIp.data(), pThis->m_iServerPort);
				SApi::UsSleep(500000);
				continue;
			}
			sHead = "act=login;result=1;";
			if(!pThis->SendFrame(pNewS,sHead))
			{
				LOGDEBUG("���Ӹյ�¼���Ͽ�!");
				SApi::UsSleep(500000);
				continue;
			}
			if(!pThis->OnLogin(sPeerIp.data(),iPeerPort,pPackage->m_sHead))
			{
				LOGDEBUG("��¼�����ϲ�ܾ�!");
				SApi::UsSleep(500000);
				continue;
			}
			if (pThis->m_Sessions.count() > pThis->m_iMaxSessionsAllowed)
			{
				LOGDEBUG("��¼�ﵽ�����������(%d)!", pThis->m_iMaxSessionsAllowed);
				SApi::UsSleep(500000);
				continue;
			}
			STcpServerSession *pNewSession = new STcpServerSession();
			WORD startword = pThis->GetStartWord();
			SKT_SWAP_WORD(startword);
			pNewSession->SetStartWord(startword);
			pNewSession->m_sPeerIp = sPeerIp;
			pNewSession->m_iPeerPort = iPeerPort;
			pNewSession->m_pServer = pThis;
			pNewSession->m_pSocket = pNewS;
			pNewSession->m_sLoginAttr = pPackage->m_sHead;
			pThis->m_Sessions.append(pNewSession);
			//�»Ự�ص�
			pThis->OnNewSession(pNewSession);
			if(pThis->m_bSessionOnThread)
			{
				SKT_CREATE_THREAD(ThreadService,pNewSession);
			}
			else
			{
				pNewS->SetTimeout(1000,5000);
				pNewSession->m_last_recv_time = (int)::time(NULL);
				pNewSession->m_last_send_time = (int)::time(NULL);
				pNewSession->m_no_recv = 0;
				pNewSession->m_no_send = 0;
			}
			pNewS = NULL;
		}
		if(pNewS != NULL)
		{
			delete pNewS;
			pNewS = NULL;
		}
	}
	if(pPackage != NULL)
		delete pPackage;
	pThis->EndThread();
	LOGDEBUG("ThreadListen exit!");
	return NULL;
}


////////////////////////////////////////////////////////////////////////
// ��    ��:  ����˻Ự�߳�
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-10 22:20
// ����˵��:  this
// �� �� ֵ:  NULL
//////////////////////////////////////////////////////////////////////////
void* STcpServerBase::ThreadService(void* lp)
{
	STcpServerSession *pSession = (STcpServerSession*)lp;
	STcpServerBase *pThis = pSession->m_pServer;

	stuSTcpPackage *pPackage=NULL;
	stuSTcpPackage *pRecvPackage=NULL;
	int cnt,ret;
	bool bError = false;
	SString sHead;
	SSocket *pS = pSession->m_pSocket;
	pThis->BeginThread();
	pS->SetTimeout(100,5000);
	int last_recv_time = (int)::time(NULL);//�����յ����ݵ�ʱ��
	int last_send_time = (int)::time(NULL);//��������ݵ�ʱ��
	int no_recv=0;//û���յ����ݵĴ���
	int no_send=0;//û�з������ݵĴ���
	while(!pThis->IsQuit())
	{
		//more = false;
		cnt = 0;
		//�Ƿ�����Ҫ���͵�����
		pPackage = pSession->m_SendPackage[0];
		if(pPackage == NULL)
		{
			if(++no_send > 100)
			{
				no_send = 0;
				if(abs((int)::time(NULL)-last_send_time) >= 10)
				{
					//10��û�����ݷ��ͣ���һ������֡
					sHead = "act=echo;";
					//LOGDEBUG("lock");
					pSession->lock();					
					if(!((STcpCommBase*)pSession)->SendFrame(pS,sHead))
					{
						pSession->unlock();
						LOGERROR("������������ʧ��!");
						bError = true;
						break;
					}
#ifdef _DEBUG
//  					LOGDEBUG("SEND %s->%d(len:%d,type=%d,ret=%d):%s", pS->GetPeerIp().data(), pThis->m_iServerPort,
//  						0, 0, 1, sHead.data());
#endif
					//LOGDEBUG("unlock");
					pSession->unlock();
					last_send_time = (int)::time(NULL);
				}
			}
		}
		else
		{
			no_send = 0;
		}
		while(pPackage)
		{
			//LOGDEBUG("lock");
			pSession->lock();
			if(!((STcpCommBase*)pSession)->SendFrame(pS,pPackage))
			{
				//LOGDEBUG("unlock");
				pSession->unlock();
				LOGERROR("���ͱ���ʧ��!");
				bError = true;
				break;
			}
			pSession->m_SendPackage.remove(pPackage);
			pPackage = pSession->m_SendPackage[0];
			//LOGDEBUG("unlock");
			pSession->unlock();
			if(cnt ++ > 100)
			{
				//if(pPackage != NULL)
				//	more = true;
				last_send_time = (int)::time(NULL);//���·���ʱ��
				break;
			}
			if(pPackage == NULL)
				last_send_time = (int)::time(NULL);//���·���ʱ��
		}
		if(bError)
			break;

		//���ձ���
		pS->SetTimeout(100,5000);
		//LOGDEBUG("lock");
		pSession->lock();
		//int soc0 = SDateTime::getNowSoc();
		ret = pS->CheckForRecv();
		//LOGDEBUG("unlock");
		pSession->unlock();
		//int soc1 = SDateTime::getNowSoc();
		//if(soc1-soc0 > 1)
		//{
		//	LOGDEBUG("CheckForRecv too long time(%ds)!!!!",soc1-soc0);
		//}
		pS->SetTimeout(100,5000);
		if(ret < 0)
		{
			//�쳣
			break;
		}
		else if(ret == 0)
		{
			//û�����ݵ���
			if(++no_recv > 100)
			{
				no_recv = 0;
				if(abs((int)::time(NULL)-last_recv_time) > 30)
				{
					LOGWARN("30����û�����ݵ���ʱ����Ϊ�����쳣���Ͽ�����!");
					break;//30����û�����ݵ���ʱ����Ϊ�����쳣���Ͽ�����
				}
			}
			SApi::UsSleep(1000);
		}
		else if(ret == 1)
		{
			//���µ����ݵ���
			no_recv = 0;
			//pPackage = NULL;
			//LOGDEBUG("lock");
			pSession->lock();
			ret = pThis->RecvFrame(pS,pRecvPackage);
#ifdef _DEBUG
//  			LOGDEBUG("RECV %s->%d(len:%d,type=%d,ret=%d):%s", pS->GetPeerIp().data(), pThis->m_iServerPort, 
//  				pRecvPackage->m_iAsduLen, pRecvPackage->m_wFrameType, ret, pRecvPackage->m_sHead.left(1000).data());
#endif
			if (ret == 0)
			{
				//û������
			}
			else if(ret < 0)
			{
				//�쳣
				//LOGDEBUG("unlock");
				pSession->unlock();
				LOGERROR("���ձ���ʱʧ��! ret=%d",ret);
// 				if(pPackage != NULL)
// 				{
// 					delete pPackage;
// 					pPackage = NULL;
// 				}
				break;
			}
			else
			{
				if(pRecvPackage != NULL)
				{
					//pSession->lock();
					if(pRecvPackage->m_wFrameType == 0 && SString::GetAttributeValue(pRecvPackage->m_sHead,"act")=="echo")
					{
						//delete pPackage;//0������Ϊ����������
						//pPackage = NULL;
					}
					else
					{
						if(!pThis->OnRecvFrame(pSession,pRecvPackage))
						{							
							if(pSession->m_RecvPackage.count()>pThis->m_iRecvBufferPkgs)
							{
								//pSession->m_RecvPackage.remove(0);
								static int last_soc = 0;
								if((int)::time(NULL) != last_soc)
								{
									last_soc = (int)::time(NULL);
									LOGWARN("�����ж���(>%d)������ʧ��������!",pThis->m_iRecvBufferPkgs);
								}
								delete pRecvPackage;
							}
							else
							{
								pSession->m_RecvPackage.append(pRecvPackage);
							}
							pRecvPackage = NULL;
						}
						else
						{
// 							if(pPackage->m_bAutoDelete)
// 							{
// 								delete pPackage;
// 								pPackage = NULL;
// 							}
						}
					}
					//pSession->unlock();
					last_recv_time = (int)::time(NULL);//���½���ʱ��
				}
			}
			//LOGDEBUG("unlock");
			pSession->unlock();
// 			if(pPackage != NULL)
// 			{
// 				delete pPackage;
// 				pPackage = NULL;
// 			}
			//more = true;
		}
		//if (!more && pSession->m_SendPackage.count() == 0)
		//	SApi::UsSleep(10000);
	}
	if(pRecvPackage != NULL)
		delete pRecvPackage;
	pS->Close();
	pThis->OnDeleteSession(pSession);
	SApi::UsSleep(1000000);//��ʱһ�����ɾ��
	pThis->EndThread();
	pThis->m_Sessions.remove(pSession);
	return NULL;
}

void* STcpServerBase::ThreadServiceAll(void* lp)
{
	STcpServerBase *pThis = (STcpServerBase*)lp;
	STcpServerSession *pSession;

	stuSTcpPackage *pPackage;
	unsigned long pos;
	int soc;
	int cnt,ret;
	bool bError = false;
	SString sHead;
	SSocket *pS;// = pSession->m_pSocket;
	pThis->BeginThread();
	while(!pThis->IsQuit())
	{
		//more = false;
		cnt = 0;
		//�Ƿ�����Ҫ���͵�����
		soc = (int)SDateTime::getNowSoc();
		pSession = pThis->m_Sessions.FetchFirst(pos);
		while(pSession)
		{
			pS = pSession->m_pSocket;
			pPackage = pSession->m_SendPackage[0];
			if(pPackage == NULL)
			{
				if(++pSession->m_no_send > 100)
				{
					pSession->m_no_send = 0;
					if(abs(soc-pSession->m_last_send_time) >= 10)
					{
						//10��û�����ݷ��ͣ���һ������֡
						sHead = "act=echo;";
						//LOGDEBUG("lock");
						pSession->lock();
						if(!((STcpCommBase*)pSession)->SendFrame(pS,sHead))
						{
							//LOGDEBUG("unlock");
							pSession->unlock();
							LOGERROR("������������ʧ��!");
							pThis->OnDeleteSession(pSession);
							pThis->m_Sessions.remove(pSession);
							break;
						}
#ifdef _DEBUG
// 						LOGDEBUG("SEND %s->%d(len:%d,type=%d,ret=%d):%s", pS->GetPeerIp().data(), pThis->m_iServerPort,
// 							0, 0, 1, sHead.data());
#endif
						//LOGDEBUG("unlock");
						pSession->unlock();
						pSession->m_last_send_time = soc;
					}
				}
			}
			else
			{
				pSession->m_no_send = 0;
			}
			bError = false;
			while(pPackage)
			{
				//LOGDEBUG("lock");
				pSession->lock();
				if(!((STcpCommBase*)pSession)->SendFrame(pS,pPackage))
				{
					//LOGDEBUG("unlock");
					pSession->unlock();
					LOGERROR("���ͱ���ʧ��!");
					bError = true;
					break;
				}
				pSession->m_SendPackage.remove(pPackage);
				pPackage = pSession->m_SendPackage[0];
				//LOGDEBUG("unlock");
				pSession->unlock();
				if(pPackage == NULL || cnt ++ > 100)
				{
					pSession->m_last_send_time = soc;//���·���ʱ��
					break;
				}
			}
 			if(bError)
 			{
				pThis->OnDeleteSession(pSession);
				pThis->m_Sessions.remove(pSession);
				break;
			}

			//���ձ���
			if(pThis->m_Sessions.count() > 10)
				pS->SetTimeout(1,5000);
			else
				pS->SetTimeout(10,5000);
			pSession->lock();
			ret = pS->CheckForRecv();
			pSession->unlock();
			pS->SetTimeout(100,5000);
			if(ret < 0)
			{
				//�쳣
				pThis->OnDeleteSession(pSession);
				pThis->m_Sessions.remove(pSession);
				break;
			}
			else if(ret == 0)
			{
				//û�����ݵ���
				if(++pSession->m_no_recv > 100)
				{
					pSession->m_no_recv = 0;
					if(abs(soc-pSession->m_last_recv_time) > 30)
					{
						LOGWARN("30����û�����ݵ���ʱ����Ϊ�����쳣���Ͽ�����!");
						pThis->OnDeleteSession(pSession);
						pThis->m_Sessions.remove(pSession);
						break;//30����û�����ݵ���ʱ����Ϊ�����쳣���Ͽ�����
					}
				}
			}
			else if(ret == 1)
			{
				//���µ����ݵ���
				pSession->m_no_recv = 0;
				pPackage = NULL;
				//LOGDEBUG("lock");
				pSession->lock();
				ret = pThis->RecvFrame(pS,pPackage);
#ifdef _DEBUG
// 				LOGDEBUG("RECV %s->%d(len:%d,type=%d,ret=%d):%s", pS->GetPeerIp().data(), pThis->m_iServerPort, 
// 					pPackage->m_iAsduLen, pPackage->m_wFrameType, ret, pPackage->m_sHead.left(1000).data());
#endif
				if (ret == 0)
				{
					//û������
				}
				else if(ret < 0)
				{
					//�쳣
					//LOGDEBUG("unlock");
					pSession->unlock();
					LOGERROR("���ձ���ʱʧ��! ret=%d",ret);
					pThis->OnDeleteSession(pSession);
					pThis->m_Sessions.remove(pSession);
					break;
				}
				else
				{
					if(pPackage != NULL)
					{
						//pSession->lock();
						if(pPackage->m_wFrameType == 0 && SString::GetAttributeValue(pPackage->m_sHead,"act")=="echo")
						{
							delete pPackage;//0������Ϊ����������
							pPackage = NULL;
						}
						else
						{
							if(!pThis->OnRecvFrame(pSession,pPackage))
							{
								if(pSession->m_RecvPackage.count()>pThis->m_iRecvBufferPkgs)
								{
									//pSession->m_RecvPackage.remove(0);
									static int last_soc = 0;
									if((int)::time(NULL) != last_soc)
									{
										last_soc = (int)::time(NULL);
										LOGWARN("�����ж���(>%d)����ɾ����������!",pThis->m_iRecvBufferPkgs);
									}
									delete pPackage;
								}
								else
								{
									pSession->m_RecvPackage.append(pPackage);
								}
								pPackage = NULL;
							}
							else
							{
								if(pPackage->m_bAutoDelete)
								{
									delete pPackage;
									pPackage = NULL;
								}
							}
						}
						//pSession->unlock();
						pSession->m_last_recv_time = soc;//���½���ʱ��
					}
				}
				//LOGDEBUG("unlock");
				pSession->unlock();
				if(pPackage != NULL)
				{
					delete pPackage;
					pPackage = NULL;
				}
			}
			pSession = pThis->m_Sessions.FetchNext(pos);
		}
		

		//if (!more && pSession->m_SendPackage.count() == 0)
		//	SApi::UsSleep(10000);
	}
	pThis->m_Sessions.clear();
	pThis->EndThread();
	return NULL;
}
