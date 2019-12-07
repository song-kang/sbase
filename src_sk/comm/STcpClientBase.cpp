/**
 *
 * �� �� �� : STcpClientBase.cpp
 * �������� : 2011-11-8 17:54
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : 
 * �޸ļ�¼ : 
 *            $Log: $
 *
 **/


#include "STcpClientBase.h"


//////////////////////////////////////////////////////////////////////////
// ��    ��:  STcpClientBase
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-8 19:41
// ��    ��:  TCPͨѶ�ͻ��˻���
//////////////////////////////////////////////////////////////////////////
STcpClientBase::STcpClientBase()
{
	m_bConnected = false;
	m_SendPackage.setAutoDelete(true);
	m_RecvPackage.setAutoDelete(false);
	m_SendPackage.setShared(true);
	m_RecvPackage.setShared(true);
	m_bConnectAtStart = false;
	m_pLock = NULL;
	m_pWait = NULL;
	m_pSendWait = NULL;
	//TODO:��ʱֻ��WINDOWSƽ̨������Waitģʽ
#ifdef WIN32
	SetRecvWaitMode(true);
	SetSendWaitMode(true);
#endif
}

STcpClientBase::~STcpClientBase()
{
	Close();
	StopAndWait();
	if (m_pLock != NULL)
	{
		delete m_pLock;
		m_pLock = NULL;
	}
	if(m_pWait != NULL)
	{
		delete m_pWait;
		m_pWait = NULL;
	}
	if(m_pSendWait != NULL)
	{
		delete m_pSendWait;
		m_pSendWait = NULL;
	}
	m_RecvPackage.setAutoDelete(true);
	m_RecvPackage.clear();
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��������
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-8 19:41
// ����˵��:  @ip��ʾ�����IP��ַ��@port��ʾ�˿�
// �� �� ֵ:  true��ʾ���������ɹ�,false��ʾ��������ʧ��
//////////////////////////////////////////////////////////////////////////
bool STcpClientBase::Start(SString ip,int port)
{
	m_sServerIp = ip;
	m_iServerPort = port;
	m_bConnected = false;
	m_bServiceMode = true;//����ģʽ
	if (m_pLock == NULL)
		m_pLock = new SLock();
	if(!SService::Start())
	{
		return false;
	}
	if(m_bConnectAtStart)
	{
		if(!Login())
		{
			Stop();
			return false;
		}
	}
	SKT_CREATE_THREAD(ThreadClient, this);
	SKT_CREATE_THREAD(ThreadRecv, this);
	return true;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ֹͣ����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-8 19:41
// ����˵��:  void
// �� �� ֵ:  true��ʾ����ֹͣ�ɹ�,false��ʾ����ֹͣʧ��
//////////////////////////////////////////////////////////////////////////
bool STcpClientBase::Stop()
{
	m_Socket.Close();
	if(!SService::Stop())
	{
		return false;
	}
	return true;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��¼������
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-17 10:41
// ����˵��:  void
// �� �� ֵ:  true��¼�ɹ�,false��¼ʧ��
//////////////////////////////////////////////////////////////////////////
bool STcpClientBase::Login()
{
	//���ӷ���˲���¼
	SString sHead;
	SString sLoginHead;
	sLoginHead.sprintf("act=login;%s",m_sLoginInfo.data());
	int ret;

	m_bConnected = false;
	SSocket *pS = &m_Socket;
	pS->Close();
	pS->CreateTcp();
	LOGDEBUG("׼����¼�����:%s:%d",m_sServerIp.data(),m_iServerPort);
	if(!pS->Connect(m_sServerIp.data(),m_iServerPort))
	{
		OnClose();
		LOGERROR("���ӷ����ʧ��(%s:%d)!",m_sServerIp.data(),m_iServerPort);
		return false;
	}
	pS->SetTimeout(5000,5000);
	//��¼
	sHead = "";
	if(!SendFrame(sLoginHead))
	{
		m_Socket.Close();
		OnClose();
		LOGERROR("���͵�¼����ʧ��!");
		return false;
	}
	SApi::UsSleep(100000);
	stuSTcpPackage *pPackage = NULL;
	ret = RecvFrame(pPackage);
	if(ret <= 0 || pPackage == NULL)
	{
		m_Socket.Close();
		OnClose();
		if(pPackage != NULL)
			delete pPackage;
		LOGERROR("��¼�����ʧ��(%s:%d)!",m_sServerIp.data(),m_iServerPort);
		return false;
	}
	if(SString::GetAttributeValue(pPackage->m_sHead,"act") != "login" ||
		SString::GetAttributeValueI(pPackage->m_sHead,"result") != 1)
	{
		m_Socket.Close();
		OnClose();
		if(pPackage != NULL)
			delete pPackage;
		LOGERROR("��¼�����ʧ��(%s:%d)!",m_sServerIp.data(),m_iServerPort);
		return false;
	}
	delete pPackage;
	m_bConnected = true;
	OnLogin();
	pS->SetTimeout(1, 5000);
	LOGDEBUG("��¼�����:%s:%d�ɹ�!", m_sServerIp.data(), m_iServerPort);
	return true;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  �ԷǷ�����ʽ���ӿͻ��ˣ�������Start����ͬʱʹ��
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-17 10:08
// ����˵��:  @ip��ʾ�����IP��ַ��@port��ʾ�˿�
// �� �� ֵ:  true��ʾ���ӳɹ�,false��ʾ����ʧ��
//////////////////////////////////////////////////////////////////////////
bool STcpClientBase::Connect(SString ip,int port)
{
	if (m_pLock != NULL)
	{
		delete m_pLock;
		m_pLock = NULL;
	}
	m_bServiceMode = false;//�Ƿ���ģʽ
	this->m_sServerIp = ip;
	this->m_iServerPort = port;
	return Login();
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  �ر�����,��ӦConnect����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-17 10:11
// ����˵��:  void
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void STcpClientBase::Close()
{
	m_Socket.Close();
	m_bConnected = false;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ������ݵ����Ͷ��У��ڷ�����ɺ��ͷ�pPackage����ֻ�����ڷ���ģʽ
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-10 12:40
// ����˵��:  @pPackage�����ͱ���
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void STcpClientBase::AddToSend(stuSTcpPackage *pPackage)
{
	m_SendPackage.append(pPackage);
	while(m_SendPackage.count() > 10000)
		m_SendPackage.remove(0);
	if(m_pSendWait)
		m_pSendWait->post();
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ������ݰ������ն����У�ֻ�����ڷ���ģʽ
// ��    ��:  �ۿ���
// ����ʱ��:  2012-2-3 14:01
// ����˵��:  @pPackageΪ�½��յ������ݰ�
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void STcpClientBase::AddToRecv(stuSTcpPackage *pPackage)
{
	if(!pPackage->m_bAutoDelete)
	{
		//�������Զ�ɾ���Ķ�����Ҫ����һ��
		stuSTcpPackage *pNew = new stuSTcpPackage();
		pNew->m_sHead = pPackage->m_sHead;
		pNew->m_iAsduLen = pNew->m_iAsduBufferLen = pPackage->m_iAsduLen;
		if(pPackage->m_pAsduBuffer != NULL && pPackage->m_iAsduLen > 0)
		{
			pNew->m_pAsduBuffer = new BYTE[pNew->m_iAsduLen];
			memcpy(pNew->m_pAsduBuffer,pPackage->m_pAsduBuffer,pNew->m_iAsduLen);
		}
		pPackage = pNew;
	}
	if( pPackage->m_bFixBuffer && !pPackage->m_bOverflow)
	{
		//�̶��ڴ�ʱ��û����������ܽ�������������У�Ӧ˵�½�����
		if(pPackage->m_pAsduBuffer != NULL && pPackage->m_iAsduLen > 0)
		{
			BYTE *pBuf = new BYTE[pPackage->m_iAsduLen];
			memcpy(pBuf,pPackage->m_pAsduBuffer,pPackage->m_iAsduLen);
			pPackage->m_bFixBuffer = pPackage->m_bOverflow = false;
			pPackage->m_pAsduBuffer = pBuf;
		}

	}
	if(m_RecvPackage.count() < 10000)
		m_RecvPackage.append(pPackage);
	else
		delete pPackage;
// 	while(m_RecvPackage.count() > 10000)
// 	{
// 		m_RecvPackage.remove(0);
// 	}
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ͨ������ͷ�ַ����е�act���Լ������ն����еı��ģ�ֻ�����ڷ���ģʽ
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-10 21:32
// ����˵��:  @act��ʾ����ı���
// �� �� ֵ:  stuSTcpPackage*
//////////////////////////////////////////////////////////////////////////
stuSTcpPackage* STcpClientBase::SearchRecvPackage(SString act,bool bWithLock/*=true*/)
{
	unsigned long pos;
	if(bWithLock)
		m_RecvPackage.lock();
	stuSTcpPackage *pPackage = m_RecvPackage.FetchFirst(pos);
	while (pPackage != NULL && IsConnected())
	{
		if(SString::GetAttributeValue(pPackage->m_sHead,"act") == act)
		{
			m_RecvPackage.remove(pPackage,false);
			if(bWithLock)
				m_RecvPackage.unlock();
			return pPackage;
		}
		pPackage = m_RecvPackage.FetchNext(pos);
	}
	if(bWithLock)
		m_RecvPackage.unlock();
	return NULL;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ͨ���������ͼ������ն����еı��ģ�ֻ�����ڷ���ģʽ
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-10 21:32
// ����˵��:  @wFrameType��ʾ
//         :  @wFrameType2��ʾ�ڶ����ͣ�0��ʾû�еڶ�����
// �� �� ֵ:  stuSTcpPackage*
//////////////////////////////////////////////////////////////////////////
stuSTcpPackage* STcpClientBase::SearchRecvPackage(WORD wFrameType,WORD wFrameType2/*=0*/,bool bWithLock/*=true*/)
{
	unsigned long pos=0;
	if(bWithLock)
		m_RecvPackage.lock();
	stuSTcpPackage *pPackage = m_RecvPackage.FetchFirst(pos);
	while (pPackage != NULL && IsConnected())
	{
		if(pPackage->m_wFrameType == wFrameType || (wFrameType2 != 0 && wFrameType2 == pPackage->m_wFrameType))
		{
			m_RecvPackage.remove(pPackage,false);
			if(bWithLock)
				m_RecvPackage.unlock();
			return pPackage;
		}
		pPackage = m_RecvPackage.FetchNext(pos);
	}
	if(bWithLock)
		m_RecvPackage.unlock();
	return NULL;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  �ͻ���ͨѶ�߳�
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-8 19:41
// ����˵��:  this
// �� �� ֵ:  NULL
//////////////////////////////////////////////////////////////////////////
void* STcpClientBase::ThreadClient(void* lp)
{
	STcpClientBase *pThis = (STcpClientBase*)lp;
	pThis->BeginThread();
	int cnt;
	//SSocket *pS = &pThis->m_Socket;
	SString sLoginHead;
	sLoginHead.sprintf("act=login;%s",pThis->m_sLoginInfo.data());
	SString sHead;
	stuSTcpPackage *pPackage;
	int last_send_time = (int)::time(NULL);//��������ݵ�ʱ��
	int no_send=0;//û�з������ݵĴ���
	bool first = true;

	while(!pThis->IsQuit())
	{
		if(first)
		{
			first = false;
			if(!pThis->m_bConnected)
			{
				if(!pThis->Login())
				{
					LOGERROR("��¼�����ʧ��!");
					for(int i=0;!pThis->IsQuit() && i<100;i++)
						SApi::UsSleep(50000);
					continue;
				}
				pThis->m_bConnected = true;
			}
		}
		else
		{
			pThis->OnClose();
			//���ӷ���˲���¼
			if(!pThis->Login())
			{
				//LOGERROR("��¼�����ʧ��!");
				for(int i=0;!pThis->IsQuit() && i<100;i++)
					SApi::UsSleep(50000);
				continue;
			}
			pThis->m_bConnected = true;
		}
		
		while(!pThis->IsQuit())
		{
			//�Ƿ�����Ҫ���͵�����
			//more = false;
			cnt = 0;
			pPackage = pThis->m_SendPackage[0];
			if(pPackage == NULL)
			{
				if(++no_send > 10)
				{
					no_send = 0;
					if(abs((int)::time(NULL)-last_send_time) >= 10)
					{
						//10��û�����ݷ��ͣ���һ������֡
						sHead = "act=echo;";
						if(!pThis->SendFrame(sHead))
						{
							pThis->m_bConnected = false;
							pThis->m_Socket.Close();
							pThis->OnClose();
							LOGERROR("������������ʧ��!");
							break;
						}
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
				if(!pThis->SendFrame(pPackage))
				{
					LOGERROR("���ͱ���ʧ��!");
					pThis->m_Socket.Close();
					pThis->OnClose();
					pThis->m_bConnected = false;
					break;
				}
				pThis->m_SendPackage.remove(pPackage);
				if(cnt ++ > 100)
				{
					//if(pPackage != NULL)
					//	more = true;
					last_send_time = (int)::time(NULL);//���·���ʱ��
					break;
				}
				pPackage = pThis->m_SendPackage[0];
				if(pPackage == NULL)
					last_send_time = (int)::time(NULL);//���·���ʱ��
			}
			if(!pThis->m_bConnected)
			{
				LOGWARN("���ӶϿ�!");
				pThis->m_Socket.Close();
				pThis->OnClose();
				break;
			}
			if(pThis->m_pSendWait)
				pThis->m_pSendWait->wait(1);
			else
				SApi::UsSleep(1000);
		}
		SApi::UsSleep(1000000);
	}
	pThis->OnClose();

	pThis->EndThread();
	return NULL;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  �ͻ���ͨѶ�̣߳������Ľ���
// ��    ��:  �ۿ���
// ����ʱ��:  2014-9-28 15:41
// ����˵��:  this
// �� �� ֵ:  NULL
//////////////////////////////////////////////////////////////////////////
void* STcpClientBase::ThreadRecv(void* lp)
{
	STcpClientBase *pThis = (STcpClientBase*)lp;
	pThis->BeginThread();
	int ret;
	SSocket *pS = &pThis->m_Socket;
	SString sHead;
	stuSTcpPackage *pPackage = NULL;
	int last_recv_time = (int)::time(NULL);//�����յ����ݵ�ʱ��
	int no_recv = 0;//û���յ����ݵĴ���
	pS->SetTimeout(10, 5000);
	while (!pThis->IsQuit())
	{
		while (1)
		{
			if(pThis->IsQuit())
			{
				goto end;
			}
			if(pThis->m_bConnected)
			{
				last_recv_time = (int)::time(NULL);//�����յ����ݵ�ʱ��
				break;
			}
			SApi::UsSleep(100000);
			continue;
		}
		//���ձ���
		//pThis->lock();
		ret = pS->CheckForRecv();
		//pThis->unlock();
		if (ret < 0)
		{
			LOGWARN("���ձ���ʱʧ��!");
			pThis->m_Socket.Close();			
			continue;
		}
		else if (ret == 0)
		{
			//û�����ݵ���
			if (++no_recv > 10)
			{
				no_recv = 0;
				if (abs((int)::time(NULL) - last_recv_time) >= 30)
				{
					last_recv_time = (int)::time(NULL);
					pThis->m_bConnected = false;
					pThis->m_Socket.Close();
					LOGWARN("30����û�����ݵ���ʱ����Ϊ�����쳣���Ͽ�����!");
					continue;//30����û�����ݵ���ʱ����Ϊ�����쳣���Ͽ�����
				}
			}
			//SApi::UsSleep(10000);
		}
		else if (ret == 1)
		{
			//���µ����ݵ���
			no_recv = 0;
			//pPackage = NULL;

			ret = pThis->RecvFrame(pPackage);
			if (ret == 0)
			{
				//û������
			}
			else if (ret < 0)
			{
				//�쳣
				LOGWARN("���ձ���ʱʧ��!");
				pThis->m_bConnected = false;
				pThis->m_Socket.Close();
				continue;
			}
			else
			{
				if (pPackage != NULL)
				{
					if (pPackage->m_wFrameType == 0 && SString::GetAttributeValue(pPackage->m_sHead, "act") == "echo")
					{
// 						pPackage=pPackage;
// 						delete pPackage;//0������Ϊ����������
// 						pPackage = NULL;
					}
					else if (!pThis->OnRecvFrame(pPackage))
					{
						if(pThis->m_RecvPackage.count() < 1000)
						{
							pThis->m_RecvPackage.append(pPackage);
							pPackage = NULL;
						}
						//pThis->m_pWait->post();�ĵ�Recv�ɹ�ʱpost�ź�
// 						if (pThis->m_RecvPackage.count()>10000)
// 							pThis->m_RecvPackage.remove(0);
// 						pPackage = NULL;
					}
					else
					{
// 						if (pPackage->m_bAutoDelete)
// 						{
// 							delete pPackage;
// 							pPackage = NULL;
// 						}
					}
					last_recv_time = (int)::time(NULL);//���½���ʱ��
//					pPackage = NULL;
				}
			}
// 			if (pPackage != NULL)
// 			{
// 				delete pPackage;
// 				pPackage = NULL;
// 			}
		}
	}
end:
	if(pPackage != NULL)
		delete pPackage;
	pThis->EndThread();
	return NULL;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ����һ֡����,����ģʽ�¼������У�ֱ��ģʽ������
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-10 12:31
// ����˵��:  @pPackage��ʾ�����͵����ݰ�
// �� �� ֵ:  true��ʾ���ͳɹ�,false��ʾ����ʧ��
//////////////////////////////////////////////////////////////////////////
bool STcpClientBase::SendFrame(stuSTcpPackage *pPackage)
{
	bool ret;
	lock();
	ret = STcpCommBase::SendFrame(&m_Socket,pPackage);
	unlock();
	return ret;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ����һ֡����,����ģʽ�¼������У�ֱ��ģʽ������
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-8 19:17
// ����˵��:  @sHead��ʾ����ͷ�ַ���
//            @wFrameType��������
//            @pAsduBuffer��ʾASDU���ݻ���������pAsduBufferΪNULL��iAsduLen��Ϊ0ʱ����ʾʹ��Ԥ��ķ��ͻ�����
//            @iAsduLen��ʾASDU���ݳ���
// �� �� ֵ:  true��ʾ���ͳɹ�,false��ʾ����ʧ��
//////////////////////////////////////////////////////////////////////////
bool STcpClientBase::SendFrame(SString &sHead,WORD wFrameType,BYTE *pAsduBuffer,int iAsduLen)
{
	bool ret;
	lock();
	ret = STcpCommBase::SendFrame(&m_Socket,sHead,wFrameType,pAsduBuffer,iAsduLen);
	unlock();
#ifdef _DEBUG
	LOGDEBUG("SEND %s:%d(len:%d,type=%d,ret=%d):%s HEX:%s", m_sServerIp.data(), m_iServerPort, iAsduLen, wFrameType, ret, sHead.left(1000).data(), 
		pAsduBuffer==NULL?"":(SString::HexToStr(pAsduBuffer,iAsduLen>100?100:iAsduLen).data()));
#endif
	return ret;
}


////////////////////////////////////////////////////////////////////////
// ��    ��:  ����һ�����ݰ���stuSTcpPackageָ���в����÷�����ʵ��,,����ģʽ�¼������У�ֱ��ģʽ������
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-10 12:33
// ����˵��:  @pPackage��ʾ���ݰ�����ָ�룬�ڲ�����stuSTcpPackage�����ⲿ�����ͷ�
// �� �� ֵ:  <0��ʾ����ʧ�ܣ�=0��ʾ���ճ�ʱ��>0��ʾ���ճɹ��ҵ��ڱ����ܳ���
//////////////////////////////////////////////////////////////////////////
int STcpClientBase::RecvFrame(stuSTcpPackage* &pPackage)
{
	int ret;
	lock();
	ret = STcpCommBase::RecvFrame(&m_Socket,pPackage);
	unlock();
	if(ret > 0)
	{
		if(m_pWait != NULL)
			m_pWait->post();
	}
#ifdef _DEBUG
	LOGDEBUG("RECV %s:%d(len:%d,type=%d,ret=%d):%s HEX:%s", m_sServerIp.data(), m_iServerPort, pPackage->m_iAsduLen, pPackage->m_wFrameType,ret, pPackage->m_sHead.left(1000).data(),
		pPackage->m_pAsduBuffer == NULL ? "" : (SString::HexToStr(pPackage->m_pAsduBuffer, pPackage->m_iAsduLen>100 ? 100 : pPackage->m_iAsduLen).data()));
#endif
	return ret;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ����һ�����ݰ���stuSTcpPackageָ���в����÷�����ʵ��,,����ģʽ�¼������У�ֱ��ģʽ������
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-8 19:18
// ����˵��:  @sHead��ʾ����ͷ�ַ���
//            @wFrameType��ʾ�������ͣ����÷���
//            @pAsduBuffer��ʾASDU���ݻ��������ڲ��������ݲ����÷���
//            @iAsduLen��ʾASDU���ݳ��ȣ����÷���
// �� �� ֵ:  <0��ʾ����ʧ�ܣ�=0��ʾ���ճ�ʱ��>0��ʾ���ճɹ��ҵ��ڱ����ܳ���
//////////////////////////////////////////////////////////////////////////
#if 0
int STcpClientBase::RecvFrame(SString &sHead,WORD &wFrameType,BYTE *&pAsduBuffer,int &iAsduLen)
{
	return STcpCommBase::RecvFrame(&m_Socket,sHead,wFrameType,pAsduBuffer,iAsduLen);
}
#endif

////////////////////////////////////////////////////////////////////////
// ��    ��:  ���Ͳ�����һ֡���ģ�����ʹ���ڷ���ģʽ�£�ֱ��ģʽ�н��ն��п��ܻ���ɱ������߳��յ������ж�ֱ�ӽ��յò���
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-17 13:37
// ����˵��:  @pPackage��ʾ�����͵����ݰ�
//            @pRecvPackage��ʾ���յ����°�
//            @iTimeout_s��ʾ��ʱʱ�䣨��λΪ�룩��0��ʾ����ʱδ�յ�����ʱһֱ�ȴ�
//         :  @wRecvFrameType��ʾ���յ����ͺţ�0��ʾ������������
// �� �� ֵ:  true��ʾ���ͳɹ�,false��ʾ����ʧ��
//////////////////////////////////////////////////////////////////////////
bool STcpClientBase::SendAndRecv(stuSTcpPackage *pPackage,stuSTcpPackage* &pRecvPackage,int iTimeout_s,WORD wRecvFrameType)
{
	bool ret;
	lock();
	int soc = (int)::time(NULL);
	ret = STcpCommBase::SendAndRecv(&m_Socket,pPackage,pRecvPackage,iTimeout_s);
	if(ret && wRecvFrameType != 0 && pRecvPackage->m_wFrameType != wRecvFrameType)
	{
		//���Բ���Ԥ�ڵı���
		if(OnRecvFrame(pRecvPackage))
		{
			if(pRecvPackage->m_bAutoDelete)
				delete pRecvPackage;
		}
		else
			AddToRecv(pRecvPackage);
		pRecvPackage = NULL;
		int iRet;
		ret = false;
		while(iTimeout_s == 0 || abs((int)::time(NULL)-soc) < iTimeout_s)
		{
			iRet = STcpCommBase::RecvFrame(&m_Socket,pRecvPackage);
			if(iRet < 0 || pRecvPackage == NULL)
				break;
			if(iRet == 0)
				continue;
			if(m_pWait != NULL)
				m_pWait->post();
			if(pRecvPackage->m_wFrameType == wRecvFrameType)
			{
				ret = true;
				break;
			}
			if(OnRecvFrame(pRecvPackage))
			{
				if(pRecvPackage->m_bAutoDelete)
					delete pRecvPackage;
			}
			else
				AddToRecv(pRecvPackage);
			pRecvPackage = NULL;
		}
		if(!ret && pRecvPackage != NULL)
		{
			delete pRecvPackage;
			pRecvPackage = NULL;
		}
	}
	unlock();
	return ret;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ���Ͳ�����һ֡���ģ�����ʹ���ڷ���ģʽ�£�ֱ��ģʽ�н��ն��п��ܻ���ɱ������߳��յ������ж�ֱ�ӽ��յò���
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-17 14:04
// ����˵��:  @pRecvPackage��ʾ���յ����°�
//            @sHead��ʾ����ͷ�ַ���
//            @wFrameType��ʾ�������ͣ����÷���
//            @pAsduBuffer��ʾASDU���ݻ��������ڲ��������ݲ����÷���
//            @iAsduLen��ʾASDU���ݳ��ȣ����÷���
//            @iTimeout_s��ʾ��ʱʱ�䣨��λΪ�룩��0��ʾ����ʱδ�յ�����ʱһֱ�ȴ�
//         :  @wRecvFrameType��ʾ���յ����ͺţ�0��ʾ������������
// �� �� ֵ:  true��ʾ���ճɹ���false��ʾʧ��
//////////////////////////////////////////////////////////////////////////
bool STcpClientBase::SendAndRecv(stuSTcpPackage* &pRecvPackage,
				 SString &sHead,WORD wFrameType,BYTE *pAsduBuffer,int iAsduLen,int iTimeout_s,WORD wRecvFrameType)
{
	bool ret;
	lock();
	int soc = (int)::time(NULL);
	ret = STcpCommBase::SendAndRecv(&m_Socket,pRecvPackage,sHead,wFrameType,pAsduBuffer,iAsduLen,iTimeout_s);
	if(ret && wRecvFrameType != 0 && pRecvPackage->m_wFrameType != wRecvFrameType)
	{
		//���Բ���Ԥ�ڵı���
		//�ַ�����
		if(OnRecvFrame(pRecvPackage))
		{
			if(pRecvPackage->m_bAutoDelete)
				delete pRecvPackage;
		}
		else
			AddToRecv(pRecvPackage);
		pRecvPackage = NULL;

		int iRet=0;
		ret = false;
		while(iTimeout_s == 0 || abs((int)::time(NULL)-soc) < iTimeout_s)
		{
			iRet = STcpCommBase::RecvFrame(&m_Socket,pRecvPackage);
			if(iRet < 0 || pRecvPackage == NULL)
				break;
			if(m_pWait != NULL)
				m_pWait->post();
			if(pRecvPackage->m_wFrameType == wRecvFrameType)
			{
				ret = true;
				break;
			}
			if(iRet == 0)
				continue;
			//�ַ�����
			if(OnRecvFrame(pRecvPackage))
			{
				if(pRecvPackage->m_bAutoDelete)
					delete pRecvPackage;
			}
			else
				AddToRecv(pRecvPackage);
			pRecvPackage = NULL;
		}
		if(!ret && pRecvPackage != NULL)
		{
			delete pRecvPackage;
			pRecvPackage = NULL;
		}
	}
	unlock();
	return ret;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ����һ֡���ģ����ӽ��ն��еȴ�ָ���Ļظ�����
// ��    ��:  �ۿ���
// ����ʱ��:  2014-10-11 13:37
// ����˵��:  @pPackage��ʾ�����͵����ݰ�
//            @pRecvPackage��ʾ���յ����°�
//            @iTimeout_s��ʾ��ʱʱ�䣨��λΪ�룩��0��ʾ����ʱδ�յ�����ʱһֱ�ȴ�
// �� �� ֵ:  true��ʾ���ͳɹ�,false��ʾ����ʧ��
//////////////////////////////////////////////////////////////////////////
bool STcpClientBase::SendAndSearchPkg(stuSTcpPackage *pPackage, stuSTcpPackage* &pRecvPackage, int iTimeout_s)
{
	if (!SendFrame(pPackage))
		return false;
	//lock();
	pRecvPackage = NULL;
#if 0
	time_t soc = 0;
	if (iTimeout_s > 0)
		soc = ::time(NULL);
	while (pRecvPackage == NULL && IsConnected())
	{
		pRecvPackage = SearchRecvPackage(pPackage->m_wFrameType);
		if (pRecvPackage != NULL)
			break;
		if (iTimeout_s > 0 && abs(::time(NULL) - soc) > iTimeout_s)
			break;
		SApi::UsSleep(10000);
	}
#else
	time_t soc = 0;
	if (iTimeout_s > 0)
		soc = ::time(NULL);
	while (pRecvPackage == NULL && IsConnected())
	{
		if(m_pWait != NULL)
			m_pWait->wait(1);//iTimeout_s * 1000);
		else
			SApi::UsSleep(1000);
		pRecvPackage = SearchRecvPackage(pPackage->m_wFrameType);
		if (pRecvPackage != NULL)
			break;
		if (iTimeout_s > 0 && abs((long)(::time(NULL) - soc)) > iTimeout_s)
			break;
	}
	//m_pWait->wait(iTimeout_s * 1000);
	//pRecvPackage = SearchRecvPackage(pPackage->m_wFrameType);
#endif
	//unlock();
	if (pRecvPackage == NULL)
		return false;
	return true;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ����һ֡���ģ����ӽ��ն��еȴ�ָ���Ļظ�����
// ��    ��:  �ۿ���
// ����ʱ��:  2014-10-11 14:04
// ����˵��:  @pRecvPackage��ʾ���յ����°�
//            @sHead��ʾ����ͷ�ַ���
//            @wFrameType��ʾ�������ͣ����÷���
//            @pAsduBuffer��ʾASDU���ݻ��������ڲ��������ݲ����÷���
//            @iAsduLen��ʾASDU���ݳ��ȣ����÷���
//            @iTimeout_s��ʾ��ʱʱ�䣨��λΪ�룩��0��ʾ����ʱδ�յ�����ʱһֱ�ȴ�
// �� �� ֵ:  true��ʾ���ճɹ���false��ʾʧ��
//////////////////////////////////////////////////////////////////////////
bool STcpClientBase::SendAndSearchPkg(stuSTcpPackage* &pRecvPackage,
	SString &sHead, WORD wFrameType, BYTE *pAsduBuffer, int iAsduLen, int iTimeout_s)
{
	if (!SendFrame(sHead,wFrameType,pAsduBuffer,iAsduLen))
		return false;
	//lock();
	pRecvPackage = NULL;
#if 0
	time_t soc = 0;
	soc = 0;
	if (iTimeout_s > 0)
		soc = ::time(NULL);
	
	while (pRecvPackage == NULL && IsConnected())
	{
		pRecvPackage = SearchRecvPackage(wFrameType);
		if (pRecvPackage != NULL)
			break;
		if (iTimeout_s > 0 && abs(::time(NULL) - soc) > iTimeout_s)
			break;
		SApi::UsSleep(10000);
	}
#else
	time_t soc = 0;
	soc = 0;
	if (iTimeout_s > 0)
		soc = ::time(NULL);
	while (pRecvPackage == NULL && IsConnected())
	{
		if(m_pWait != NULL)
			m_pWait->wait(1);//iTimeout_s * 1000);
		else
			SApi::UsSleep(1000);
		pRecvPackage = SearchRecvPackage(wFrameType);
		if (pRecvPackage != NULL)
			break;
		if (iTimeout_s > 0 && abs((long)(::time(NULL) - soc)) > iTimeout_s)
			break;
	}
#endif
	//unlock();
	//LOGDEBUG("SendAndSearchPkg=%p", pRecvPackage);
	if (pRecvPackage == NULL)
		return false;
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ����ָ�����͵ı���,����ģʽ�¼������У�ֱ��ģʽ������,һ��ֻ��ֱ��ģʽ��ʹ��
// ��    ��:  �ۿ���
// ����ʱ��:  2012-2-15 11:09
// ����˵��:  @sFrameType��ʾ�����յı�������
//            @pRecvPackage��ʾ���ձ���ָ�룬���÷���
//            @iTimeout_s��ʾ��ʱʱ��
// �� �� ֵ:  >0��ʾ�յ�Ŀ�걨�ģ�=0��ʾ���ճ�ʱ��<0��ʾʧ��
//////////////////////////////////////////////////////////////////////////
int STcpClientBase::RecvPackage(WORD sFrameType,stuSTcpPackage* &pRecvPackage,int iTimeout_s/*=5*/)
{
	time_t now = ::time(NULL);
	int ret;
	while(this->IsConnected())
	{
		ret = RecvFrame(pRecvPackage);
		if(ret == 0)
		{
			if(abs((int)(::time(NULL)-now)) > iTimeout_s)
			{
				LOGWARN("RECV type=%d timeout(%d seconds)!",sFrameType,iTimeout_s);
				break;
			}
			SApi::UsSleep(100000);
			continue;
		}
		if(ret < 0)
		{
			return ret;
		}
		if(pRecvPackage == NULL)
		{
			SApi::UsSleep(100000);
			continue;
		}
		if(pRecvPackage->m_wFrameType != sFrameType)
		{
			if(OnRecvFrame(pRecvPackage))
			{
				if(pRecvPackage->m_bAutoDelete)
				{
					delete pRecvPackage;
					pRecvPackage = NULL;
				}
			}
			else
				AddToRecv(pRecvPackage);
			continue;
		}
		return 1;
	}
	return 0;

}

