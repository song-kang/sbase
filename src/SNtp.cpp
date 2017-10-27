/**
 *
 * 文 件 名 : SNtp.cpp
 * 创建日期 : 2015-9-6 22:13
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : NTP封装类
 * 修改记录 : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-9-6	邵凯田　创建文件
 *
 **/
#include "SNtp.h"

SSntpClient::SSntpClient()
{
	m_iSecondsAdd = 0;
	m_iMSecondsAdd = 0;
}

SSntpClient::~SSntpClient()
{

}

//////////////////////////////////////////////////////////////////////////
// 描    述:  取当前时刻服务器上的时间
// 作    者:  邵凯田
// 创建时间:  2015-9-6 22:53
// 参数说明:  @now为引用返回的当前SNTP服务器时间
//         :  @bSetToSystem为true表示时间获取成功后需要写入系统，false表示不需要写入系统
// 返 回 值:  true表示获取成功，false表示获取失败
//////////////////////////////////////////////////////////////////////////
bool SSntpClient::GetTime(SDateTime &now,bool bSetToSystem/*=true*/)
{
	stuSNTPPackage sntp;
	SSocket udp;
	sockaddr_in sa;
	struct timeval tTime;
	udp.SetTimeout(1000,1000);
	//long cha=0xc9b34d23-0x4608cea3;
	//long ms_cha=0xABB90C58-0x27FB7940;//0x83BD9318  2210239256
	
	memset((char *)&sntp,0,sizeof(stuSNTPPackage));
	sntp.cLiVnMode=0x1b;

	if(!udp.CreateUdp())
	{
		return false;
	}
	if(udp.SendTo(m_sServerIp.data(),m_iPort,(char *)&sntp,sizeof(stuSNTPPackage)) <= 0)
	{
		return false;
	}
	int nLen=sizeof(stuSNTPPackage);
	if(udp.RecvFrom((char *)&sntp,nLen,sa) != sizeof(stuSNTPPackage))
	{
		return false;
	}
	udp.Close();
	tTime=sntp.tTransmitTimestamp;
	tTime.tv_sec=ntohl(tTime.tv_sec)+m_iSecondsAdd-SNTPJAN_1970;
	tTime.tv_usec=ntohl(tTime.tv_usec);
	tTime.tv_usec = (int)(((double)((unsigned int)tTime.tv_usec))/4294.967296)+m_iMSecondsAdd*1000;

	now = SDateTime::makeDateTime((time_t)(tTime.tv_sec),(tTime.tv_usec/1000)%1000,tTime.tv_usec%1000);
	if(bSetToSystem)
		SDateTime::setSystemTime(now);
	LOGDEBUG("now:%s\n",now.toString("yyyy-MM-dd hh:mm:ss.zzz").data());

	return true;
}


SSntpServer::SSntpServer()
{
	m_bRun=true;
	m_bStoped = false;
	m_sServerIp="";
	m_iPort=SNTPSERVER_PORT;
}

SSntpServer::~SSntpServer()
{

}

//////////////////////////////////////////////////////////////////////////
// 描    述:  启动sntp校时服务
// 作    者:  褚冰冰
// 创建时间:  2015-9-24 14:21
// 参数说明:  @
// 返 回 值:  false 表示启动失败
//////////////////////////////////////////////////////////////////////////
bool SSntpServer::StartServer()
{
	m_udp.SetTimeout(1000,1000);
	if(!m_udp.CreateUdp())
	{
		return false;
	}
	if (!m_udp.Bind(m_iPort,m_sServerIp.data()))
	{
		return false;
	}
	m_bStoped = false;
	S_CREATE_THREAD(funproc,this);
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  停止sntp校时服务，可能会等待一段时间
// 作    者:  褚冰冰
// 创建时间:  2015-9-24 14:21
// 参数说明:  @
// 返 回 值:  true 表示停止成功
//////////////////////////////////////////////////////////////////////////
bool SSntpServer::StopServer()
{
	m_bRun=false;
	while(m_bStoped == false)
		SApi::UsSleep(1000);
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  sntp校时服务端线程执行函数，循环接受客户端请求
//            检测udp连接，失效会重新创建
// 作    者:  褚冰冰
// 创建时间:  2015-9-25 17:38
// 参数说明:  @arg 为线程传递参数
//         :  @
//		   :  @
// 返 回 值:  无
//////////////////////////////////////////////////////////////////////////
void* SSntpServer::funproc(void* arg)
{
	SSntpServer* pThis = (SSntpServer*)arg;
	SSocket*pudp = &pThis->m_udp;
	stuSNTPPackage sntp;
	sockaddr_in sa;
	int sec =0,uec=0;
	int nLen=sizeof(stuSNTPPackage);
	while(pThis->m_bRun)
	{
		memset(&sntp,0,sizeof(stuSNTPPackage));
		if(pudp->RecvFrom((char *)&sntp,nLen,sa) !=nLen)
		{
			while ((!pudp->IsConnected())&&pThis->m_bRun)
			{
				SApi::UsSleep(1000);
				pudp->Close();
				if(!pudp->CreateUdp())
				{
					continue;
				}
				if (!pudp->Bind(pThis->m_iPort,pThis->m_sServerIp.data()))
				{
					continue;;
				}
			}
			SApi::UsSleep(1000);
			continue;
		}
		pThis->GetLocalTime(sec,uec);
		sntp.cLiVnMode = 0x1c;//MEMO: 点对点回复采用服务器模式 [2016-3-21 9:18 邵凯田]
		sntp.tTransmitTimestamp.tv_sec=htonl(sec+SNTPJAN_1970);
		sntp.tTransmitTimestamp.tv_usec=htonl(SNTPNTPFRAC(uec));
		pudp->SendTo(sa,&sntp,nLen);
	}
	pudp->Close();
	pThis->m_bStoped = true;
	return 0;
}

SNtpClient::SNtpClient()
{
	m_iSecondsAdd = 0;
	m_iMSecondsAdd = 0;
}

SNtpClient::~SNtpClient()
{

}

//////////////////////////////////////////////////////////////////////////
// 描    述:  取当前服务器上时间
// 作    者:  褚冰冰
// 创建时间:  2015-9-24 14:23
// 参数说明:  @now为引用返回的当前NTP服务器时间
//         :  @bSetToSystem为true表示时间获取成功后需要写入系统，false表示不需要写入系统
// 返 回 值:  true表示获取成功，false表示获取失败
//////////////////////////////////////////////////////////////////////////
bool SNtpClient::GetTime(SDateTime &now,bool bSetToSystem)
{
	SSocket udp;
	stuSNTPPackage sntp;
	sockaddr_in sa;
	struct timeval tNowTime,tRoadTime;
	struct timeval tTime1,tTime2,tTime3,tTime4;
	int sec=0,uec=0;
	udp.SetTimeout(1000,1000);
	memset((char *)&sntp,0,sizeof(stuSNTPPackage));
	sntp.cLiVnMode=0x1b;
	sntp.cTratum=0x0;
	sntp.cPoll=0x4;
	sntp.cPrecision=0x6;
	int nLen=sizeof(stuSNTPPackage);
	//memcpy(&sntp.nReferenceIdentifier,"TSSM",4);
	//sntp.nReferenceIdentifier=htonl(sntp.nReferenceIdentifier);
	if(!udp.CreateUdp())
	{
		return false;
	}
	GetLocalTime(sec,uec);
	sntp.tOriginateTimestamp.tv_sec=htonl(sec);
	sntp.tOriginateTimestamp.tv_usec=htonl(SNTPNTPFRAC(uec));
	if(udp.SendTo(m_sServerIp.data(),m_iPort,(char *)&sntp,nLen) <= 0)
	{
		return false;
	}
	tTime1.tv_sec=sec;
	tTime1.tv_usec=uec;
	if(udp.RecvFrom((char *)&sntp,nLen,sa) != nLen)
	{
		return false;
	}
	GetLocalTime(sec,uec);
	tTime4.tv_sec=sec;
	tTime4.tv_usec=uec;
	tTime2.tv_sec=ntohl(sntp.tReceiveTimestamp.tv_sec)-SNTPJAN_1970;
	tTime2.tv_usec=SNTPUSEC(ntohl(sntp.tReceiveTimestamp.tv_usec));
	tTime3.tv_sec=ntohl(sntp.tTransmitTimestamp.tv_sec)-SNTPJAN_1970;
	tTime3.tv_usec=SNTPUSEC(ntohl(sntp.tTransmitTimestamp.tv_usec));
	tRoadTime.tv_sec=((tTime4.tv_sec-tTime1.tv_sec)-(tTime3.tv_sec-tTime2.tv_sec));
	tRoadTime.tv_usec=((tTime4.tv_usec-tTime1.tv_usec)-(tTime3.tv_usec-tTime2.tv_usec));
	if ((tRoadTime.tv_sec&0x1)==1)
	{
		tRoadTime.tv_sec-=1;
		tRoadTime.tv_usec+=1000000;
	}
	tRoadTime.tv_sec=tRoadTime.tv_sec>>1;
	tRoadTime.tv_usec=tRoadTime.tv_usec>>1;
	tNowTime.tv_sec=tTime3.tv_sec+tRoadTime.tv_sec;
	tNowTime.tv_usec=tTime3.tv_usec+tRoadTime.tv_usec;
	if (tNowTime.tv_usec<0)
	{
		tNowTime.tv_usec=1000000+tNowTime.tv_usec;
		tNowTime.tv_sec-=1;
	}else if (tNowTime.tv_usec>=1000000)
	{
		tNowTime.tv_usec-=1000000;
		tNowTime.tv_sec+=1;
	}
	tNowTime.tv_usec+=m_iSecondsAdd;
	tNowTime.tv_usec+=m_iMSecondsAdd*1000;
	now = SDateTime::makeDateTime((time_t)(tNowTime.tv_sec),(tNowTime.tv_usec/1000)%1000,tNowTime.tv_usec%1000);
	if(bSetToSystem)
		SDateTime::setSystemTime(now);
	udp.Close();
	LOGDEBUG("now:%s\n",now.toString("yyyy-MM-dd hh:mm:ss.zzz").data());
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  时间偏差计算
// 作    者:  邵凯田
// 创建时间:  2015-12-15 23:02
// 参数说明:  @local_soc
//         :  @local_usec
//         :  @remote_soc
//         :  @remote_usec
//         :  @ts_sec
//         :  @ts_usec
// 返 回 值:  true/false
//////////////////////////////////////////////////////////////////////////
bool SNtpClient::TestTimeByTssm(int &local_soc,int &local_usec,int &remote_soc,int &remote_usec,int &ts_sec,int &ts_usec)
{
	SSocket udp;
	stuSNTPPackage sntp;
	sockaddr_in sa;
	struct timeval t0,t1,t2,t3,delta_t;
	//对时监测请求无需携带时标（报文离开请求端时标T0，报文返回请求端时标
	//T3 都在请求端本地，可直接获取），仅需将启动应用指示传递给被监测设备（应
	//携带某种可识别该应用的标志），具体协议的映射见第3 章。
	//对时监测响应返回报文应至少携带请求报文到达从动端时标T1 和响应报文
	//离开从动端时标T2
	int sec=0,uec=0;
	udp.SetTimeout(1000,1000);
	memset((char *)&sntp,0,sizeof(stuSNTPPackage));
	sntp.cLiVnMode=0x1b;
	sntp.cTratum=0x0;
	sntp.cPoll=0x4;
	sntp.cPrecision=0x6;
	int nLen=sizeof(stuSNTPPackage);
	memcpy(&sntp.nReferenceIdentifier,"TSSM",4);
	//sntp.nReferenceIdentifier=htonl(sntp.nReferenceIdentifier);
	if(!udp.CreateUdp())
	{
		return false;
	}
	GetLocalTime(sec,uec);
#if 0
	sntp.tOriginateTimestamp.tv_sec=htonl(sec);
	sntp.tOriginateTimestamp.tv_usec=htonl(SNTPNTPFRAC(uec));
#endif
	if(udp.SendTo(m_sServerIp.data(),m_iPort,(char *)&sntp,nLen) <= 0)
	{
		return false;
	}
	t0.tv_sec = sec;
	t0.tv_usec= uec;

	if(udp.RecvFrom((char *)&sntp,nLen,sa) != nLen)
	{
		return false;
	}
	GetLocalTime(sec,uec);
	t3.tv_sec=sec;
	t3.tv_usec=uec;

	t1.tv_sec	= ntohl(sntp.tReceiveTimestamp.tv_sec)-SNTPJAN_1970;
	t1.tv_usec	= SNTPUSEC(ntohl(sntp.tReceiveTimestamp.tv_usec));
	t2.tv_sec	= ntohl(sntp.tTransmitTimestamp.tv_sec)-SNTPJAN_1970;
	t2.tv_usec	= SNTPUSEC(ntohl(sntp.tTransmitTimestamp.tv_usec));

	//△t =[(T3 - T2) + (T0 - T1)]/2
	delta_t.tv_sec = ((t3.tv_sec-t2.tv_sec)+(t0.tv_sec-t1.tv_sec))>>1;
	delta_t.tv_usec = ((t3.tv_usec-t2.tv_usec)+(t0.tv_usec-t1.tv_usec))>>1;
	INT64 ll_usec = (delta_t.tv_sec*1000000);
	ll_usec += delta_t.tv_usec;
	delta_t.tv_sec = (int)(ll_usec/1000000);
	delta_t.tv_usec = (int)(ll_usec%1000000);
// 	if(delta_t.tv_usec < 0)
// 	{
// 		delta_t.tv_usec += 1000000;
// 		delta_t.tv_sec --;
// 	}
// 	else if(delta_t.tv_usec >=1000000)
// 	{
// 		delta_t.tv_usec -= 1000000;
// 		delta_t.tv_sec ++;
// 	}
	udp.Close();
	local_soc	= t0.tv_sec;
	local_usec	= t0.tv_usec;
	ts_sec		= delta_t.tv_sec;
	ts_usec		= delta_t.tv_usec;
	remote_soc	= local_soc+ts_sec;
	remote_usec	= local_usec+ts_usec;
	if(remote_usec < 0)
	{
		remote_usec += 1000000;
		remote_soc --;
	}
	else if(remote_usec >=1000000)
	{
		remote_usec -= 1000000;
		remote_soc ++;
	}
	return true;
}

SNtpServer::SNtpServer()
{
	m_bRun=true;
	m_bStoped = false;
	m_sServerIp="";
	m_iPort=SNTPSERVER_PORT;
}

SNtpServer::~SNtpServer()
{

}

//////////////////////////////////////////////////////////////////////////
// 描    述:  启动ntp校时服务
// 作    者:  褚冰冰
// 创建时间:  2015-9-24 14:21
// 参数说明:  @bTssm 是否启动TSSM验证
// 返 回 值:  false 表示启动失败
//////////////////////////////////////////////////////////////////////////
bool SNtpServer::StartServer(bool bTssm)
{
	m_bTssm=bTssm;
	m_udp.SetTimeout(1000,1000);
	if(!m_udp.CreateUdp())
	{
		return false;
	}
	if (!m_udp.Bind(m_iPort,m_sServerIp.length()==0?NULL:m_sServerIp.data()))
	{
		return false;
	}
	m_bStoped = false;
	S_CREATE_THREAD(funproc,this);
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  停止sntp校时服务，可能会等待一段时间
// 作    者:  褚冰冰
// 创建时间:  2015-9-24 14:21
// 参数说明:  @
// 返 回 值:  true 表示停止成功
//////////////////////////////////////////////////////////////////////////
bool SNtpServer::StopServer()
{
	m_bRun=false;
	while(m_bStoped == false)
		SApi::UsSleep(1000);
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  ntp校时服务端线程执行函数，循环接受客户端请求
//            检测udp连接，失效会重新创建
// 作    者:  褚冰冰
// 创建时间:  2015-9-25 17:38
// 参数说明:  @arg 为线程传递参数
//         :  @
//		   :  @
// 返 回 值:  无
//////////////////////////////////////////////////////////////////////////
void *SNtpServer::funproc(void *arg)
{
	SNtpServer* pThis = (SNtpServer*)arg;
	SSocket*pudp =&pThis->m_udp;
	stuSNTPPackage sntp;
	sockaddr_in sa;
	int sec =0,uec=0;
	int nLen=sizeof(stuSNTPPackage);
	
	while(pThis->m_bRun)
	{
		memset((char *)&sntp,0,nLen);
		if(pudp->RecvFrom((char *)&sntp,nLen,sa)!= nLen)
		{
			while ((!pudp->IsConnected())&&pThis->m_bRun)
			{
				SApi::UsSleep(1000);
				pudp->Close();
				if(!pudp->CreateUdp())
				{
					continue;
				}
				if (!pudp->Bind(pThis->m_iPort,pThis->m_sServerIp.data()))
				{
					continue;
				}
			}
			SApi::UsSleep(1000);
			continue;
		}
		pThis->GetLocalTime(sec,uec);
		sntp.cLiVnMode = 0x1c;//MEMO: 点对点回复采用服务器模式 [2016-3-21 9:18 邵凯田]
		sntp.tReceiveTimestamp.tv_sec=htonl(sec+SNTPJAN_1970);
		sntp.tReceiveTimestamp.tv_usec=htonl(SNTPNTPFRAC(uec));
		if (pThis->m_bTssm)
		{
			sntp.nReferenceIdentifier=ntohl(sntp.nReferenceIdentifier);
			if (strncmp((char *)(&sntp.nReferenceIdentifier),"TSSM",4)!=0)
			{
				continue;
			}
		}
		pThis->GetLocalTime(sec,uec);
		sntp.tTransmitTimestamp.tv_sec=htonl(sec+SNTPJAN_1970);
		sntp.tTransmitTimestamp.tv_usec=htonl(SNTPNTPFRAC(uec));
		pudp->SendTo(sa,&sntp,nLen);
	}
	pudp->Close();
	pThis->m_bStoped=true;
	return 0;
}