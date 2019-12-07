/**
 *
 * 文 件 名 : STcpServerBase.cpp
 * 创建日期 : 2011-11-8 17:54
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : 
 * 修改记录 : 
 *            $Log: $
 *
 **/


#include "STcpServerBase.h"


//////////////////////////////////////////////////////////////////////////
// 名    称:  STcpServerSession
// 作    者:  邵凯田
// 创建时间:  2011-11-11 8:46
// 描    述:  服务会话类
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
// 描    述:  添加数据到发送队列，在发送完成后释放pPackage对象
// 作    者:  邵凯田
// 创建时间:  2011-11-10 12:40
// 参数说明:  @pPackage待发送报文
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void STcpServerSession::AddToSend(stuSTcpPackage *pPackage)
{
	if(m_SendPackage.count() > m_pServer->m_iSendBufferPkgs)
	{
		static int last_soc = 0;
		if((int)::time(NULL) != last_soc)
		{
			last_soc = (int)::time(NULL);
			LOGWARN("发送列队满(>%d)，将丢失部分数据!",m_pServer->m_iSendBufferPkgs);
		}
		delete pPackage;
	}
	else
	{
		m_SendPackage.append(pPackage);
	}
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  添加数据包的接收队列
// 作    者:  邵凯田
// 创建时间:  2016-2-13 16:32
// 参数说明:  @pPackage待发送报文
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void STcpServerSession::AddToRecv(stuSTcpPackage *pPackage)
{
	if(m_RecvPackage.count() > m_pServer->m_iRecvBufferPkgs)
	{
		static int last_soc = 0;
		if((int)::time(NULL) != last_soc)
		{
			last_soc = (int)::time(NULL);
			LOGWARN("接收列队满(>%d)，将丢失部分数据!",m_pServer->m_iRecvBufferPkgs);
		}
		delete pPackage;
	}
	else
	{
		m_RecvPackage.append(pPackage);
	}
}

////////////////////////////////////////////////////////////////////////
// 描    述:  向当前会话对端发送一帧报文，只能在OnRecvFrame函数中使用，否则线程不安全
// 作    者:  邵凯田
// 创建时间:  2012-01-17 13:46
// 参数说明:  @pPackage表示待发送的数据包
//            @bAutoDelPackage表示是否在使用完毕后自动删除pPackage
// 返 回 值:  true表示发送成功,false表示发送失败
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
// 描    述:  向当前会话对端立即发送一帧报文（不通过发送缓冲区），只能在OnRecvFrame函数中使用，否则线程不安全
// 作    者:  邵凯田
// 创建时间:  2012-01-17 13:57
// 参数说明:  @sHead表示报文头字符串
//            @wFrameType表示报文类型
//            @pAsduBuffer表示ASDU内容缓冲区，当pAsduBuffer为NULL且iAsduLen不为0时，表示使用预设的发送缓冲区
//            @iAsduLen表示ASDU数据长度
// 返 回 值:  true表示发送成功,false表示发送失败
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
// 描    述:  向当前会话对端立即发送一帧报文（不通过发送缓冲区），只能在OnRecvFrame函数中使用，否则线程不安全
// 作    者:  邵凯田
// 创建时间:  2012-01-17 13:57
// 参数说明:  @wFrameType表示报文类型
//            @pAsduBuffer表示ASDU内容缓冲区，当pAsduBuffer为NULL且iAsduLen不为0时，表示使用预设的发送缓冲区
//            @iAsduLen表示ASDU数据长度
// 返 回 值:  true表示发送成功,false表示发送失败
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
// 描    述:  检索接收队列中的报文
// 作    者:  邵凯田
// 创建时间:  2011-11-10 21:32
// 参数说明:  
// 返 回 值:  stuSTcpPackage*
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
// 描    述:  通过报文头字符串中的act属性检索接收队列中的报文
// 作    者:  邵凯田
// 创建时间:  2011-11-10 21:32
// 参数说明:  @act表示待查的报文
// 返 回 值:  stuSTcpPackage*
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
// 描    述:  通过报文类型检索接收队列中的报文
// 作    者:  邵凯田
// 创建时间:  2011-11-10 21:32
// 参数说明:  @wFrameType表示
// 返 回 值:  stuSTcpPackage*
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
// 名    称:  STcpServerBase
// 作    者:  邵凯田
// 创建时间:  2011-11-8 19:41
// 描    述:  TCP通讯客户端基类
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
// 描    述:  启动服务
// 作    者:  邵凯田
// 创建时间:  2011-11-8 19:41
// 参数说明:  @port表示端口,@ip表示服务端IP地址
// 返 回 值:  true表示服务启动成功,false表示服务启动失败
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
// 描    述:  停止服务
// 作    者:  邵凯田
// 创建时间:  2011-11-8 19:41
// 参数说明:  void
// 返 回 值:  true表示服务停止成功,false表示服务停止失败
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
// 描    述:  添加数据到发送队列，在发送完成后释放pPackage对象
// 作    者:  邵凯田
// 创建时间:  2011-11-10 12:40
// 参数说明:  @pPackage待发送报文
// 返 回 值:  void
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
// 描    述:  检索接收队列中的报文
// 作    者:  邵凯田
// 创建时间:  2011-11-10 21:32
// 参数说明:  @pSession为引用的会话指针
// 返 回 值:  stuSTcpPackage*
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
// 描    述:  通过报文头字符串中的act属性检索接收队列中的报文
// 作    者:  邵凯田
// 创建时间:  2011-11-10 21:32
// 参数说明:  @act表示待查的报文
//            @pSession为引用的会话指针
// 返 回 值:  stuSTcpPackage*
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
// 描    述:  通过报文类型检索接收队列中的报文
// 作    者:  邵凯田
// 创建时间:  2011-11-10 21:32
// 参数说明:  @wFrameType表示
//            @pSession为引用的会话指针
// 返 回 值:  stuSTcpPackage*
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
// 描    述:  服务端帧听线程
// 作    者:  邵凯田
// 创建时间:  2011-11-8 19:41
// 参数说明:  this
// 返 回 值:  NULL
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
			LOGERROR("绑定服务端口(%s:%d)时失败!",pThis->m_sServerIp.data(),pThis->m_iServerPort);
			for(int i=0;!pThis->IsQuit() && i<100;i++)
				SApi::UsSleep(50000);
			continue;
		}
		pThis->m_bListened = true;
		LOGDEBUG("绑定服务端口(%s:%d)成功!",pThis->m_sServerIp.data(),pThis->m_iServerPort);
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
				LOGERROR("在端口(%d)接受连接时失败!",pThis->m_iServerPort);
				break;
			}
			sPeerIp = pNewS->GetPeerIp();
			iPeerPort = pNewS->GetPeerPort();
			LOGWARN("得到新的会话请求!(%s:%d --> %s:%d)",
					sPeerIp.data(),iPeerPort,
					pThis->m_sServerIp.data(),pThis->m_iServerPort);
			//验证登录
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
						LOGDEBUG("连接(%s:%d --> %s:%d)被应用层托管!",
							sPeerIp.data(),iPeerPort,
							pThis->m_sServerIp.data(), pThis->m_iServerPort);
						SApi::UsSleep(500000);
						continue;//SOCKET被托管
					}
				}
				LOGDEBUG("验证登录失败，将拒绝连接!(%s:%d --> %s:%d)",
					sPeerIp.data(),iPeerPort,
					pThis->m_sServerIp.data(), pThis->m_iServerPort);
				SApi::UsSleep(500000);
				continue;
			}
			if (SString::GetAttributeValue(pPackage->m_sHead, "act") != "login")
			{
				LOGDEBUG("验证登录失败，将拒绝连接!(%s:%d --> %s:%d)",
					sPeerIp.data(),iPeerPort,
					pThis->m_sServerIp.data(), pThis->m_iServerPort);
				SApi::UsSleep(500000);
				continue;
			}
			sHead = "act=login;result=1;";
			if(!pThis->SendFrame(pNewS,sHead))
			{
				LOGDEBUG("连接刚登录即断开!");
				SApi::UsSleep(500000);
				continue;
			}
			if(!pThis->OnLogin(sPeerIp.data(),iPeerPort,pPackage->m_sHead))
			{
				LOGDEBUG("登录请求被上层拒绝!");
				SApi::UsSleep(500000);
				continue;
			}
			if (pThis->m_Sessions.count() > pThis->m_iMaxSessionsAllowed)
			{
				LOGDEBUG("登录达到最大连接限制(%d)!", pThis->m_iMaxSessionsAllowed);
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
			//新会话回调
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
// 描    述:  服务端会话线程
// 作    者:  邵凯田
// 创建时间:  2011-11-10 22:20
// 参数说明:  this
// 返 回 值:  NULL
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
	int last_recv_time = (int)::time(NULL);//最后接收到数据的时间
	int last_send_time = (int)::time(NULL);//最后发送数据的时间
	int no_recv=0;//没有收到数据的次数
	int no_send=0;//没有发送数据的次数
	while(!pThis->IsQuit())
	{
		//more = false;
		cnt = 0;
		//是否有需要发送的数据
		pPackage = pSession->m_SendPackage[0];
		if(pPackage == NULL)
		{
			if(++no_send > 100)
			{
				no_send = 0;
				if(abs((int)::time(NULL)-last_send_time) >= 10)
				{
					//10秒没有内容发送，发一次心跳帧
					sHead = "act=echo;";
					//LOGDEBUG("lock");
					pSession->lock();					
					if(!((STcpCommBase*)pSession)->SendFrame(pS,sHead))
					{
						pSession->unlock();
						LOGERROR("发送心跳报文失败!");
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
				LOGERROR("发送报文失败!");
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
				last_send_time = (int)::time(NULL);//更新发送时间
				break;
			}
			if(pPackage == NULL)
				last_send_time = (int)::time(NULL);//更新发送时间
		}
		if(bError)
			break;

		//接收报文
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
			//异常
			break;
		}
		else if(ret == 0)
		{
			//没有数据到达
			if(++no_recv > 100)
			{
				no_recv = 0;
				if(abs((int)::time(NULL)-last_recv_time) > 30)
				{
					LOGWARN("30秒钟没有数据到达时，认为连接异常，断开连接!");
					break;//30秒钟没有数据到达时，认为连接异常，断开连接
				}
			}
			SApi::UsSleep(1000);
		}
		else if(ret == 1)
		{
			//有新的数据到达
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
				//没有数据
			}
			else if(ret < 0)
			{
				//异常
				//LOGDEBUG("unlock");
				pSession->unlock();
				LOGERROR("接收报文时失败! ret=%d",ret);
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
						//delete pPackage;//0类型认为是心跳报文
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
									LOGWARN("接收列队满(>%d)，将丢失部分数据!",pThis->m_iRecvBufferPkgs);
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
					last_recv_time = (int)::time(NULL);//更新接收时间
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
	SApi::UsSleep(1000000);//延时一秒后再删除
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
		//是否有需要发送的数据
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
						//10秒没有内容发送，发一次心跳帧
						sHead = "act=echo;";
						//LOGDEBUG("lock");
						pSession->lock();
						if(!((STcpCommBase*)pSession)->SendFrame(pS,sHead))
						{
							//LOGDEBUG("unlock");
							pSession->unlock();
							LOGERROR("发送心跳报文失败!");
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
					LOGERROR("发送报文失败!");
					bError = true;
					break;
				}
				pSession->m_SendPackage.remove(pPackage);
				pPackage = pSession->m_SendPackage[0];
				//LOGDEBUG("unlock");
				pSession->unlock();
				if(pPackage == NULL || cnt ++ > 100)
				{
					pSession->m_last_send_time = soc;//更新发送时间
					break;
				}
			}
 			if(bError)
 			{
				pThis->OnDeleteSession(pSession);
				pThis->m_Sessions.remove(pSession);
				break;
			}

			//接收报文
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
				//异常
				pThis->OnDeleteSession(pSession);
				pThis->m_Sessions.remove(pSession);
				break;
			}
			else if(ret == 0)
			{
				//没有数据到达
				if(++pSession->m_no_recv > 100)
				{
					pSession->m_no_recv = 0;
					if(abs(soc-pSession->m_last_recv_time) > 30)
					{
						LOGWARN("30秒钟没有数据到达时，认为连接异常，断开连接!");
						pThis->OnDeleteSession(pSession);
						pThis->m_Sessions.remove(pSession);
						break;//30秒钟没有数据到达时，认为连接异常，断开连接
					}
				}
			}
			else if(ret == 1)
			{
				//有新的数据到达
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
					//没有数据
				}
				else if(ret < 0)
				{
					//异常
					//LOGDEBUG("unlock");
					pSession->unlock();
					LOGERROR("接收报文时失败! ret=%d",ret);
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
							delete pPackage;//0类型认为是心跳报文
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
										LOGWARN("接收列队满(>%d)，将删除部分数据!",pThis->m_iRecvBufferPkgs);
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
						pSession->m_last_recv_time = soc;//更新接收时间
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
