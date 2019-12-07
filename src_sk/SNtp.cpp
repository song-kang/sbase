/**
 *
 * �� �� �� : SNtp.cpp
 * �������� : 2015-9-6 22:13
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : NTP��װ��
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-9-6	�ۿ�������ļ�
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
// ��    ��:  ȡ��ǰʱ�̷������ϵ�ʱ��
// ��    ��:  �ۿ���
// ����ʱ��:  2015-9-6 22:53
// ����˵��:  @nowΪ���÷��صĵ�ǰSNTP������ʱ��
//         :  @bSetToSystemΪtrue��ʾʱ���ȡ�ɹ�����Ҫд��ϵͳ��false��ʾ����Ҫд��ϵͳ
// �� �� ֵ:  true��ʾ��ȡ�ɹ���false��ʾ��ȡʧ��
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
// ��    ��:  ����sntpУʱ����
// ��    ��:  �ұ���
// ����ʱ��:  2015-9-24 14:21
// ����˵��:  @
// �� �� ֵ:  false ��ʾ����ʧ��
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
// ��    ��:  ֹͣsntpУʱ���񣬿��ܻ�ȴ�һ��ʱ��
// ��    ��:  �ұ���
// ����ʱ��:  2015-9-24 14:21
// ����˵��:  @
// �� �� ֵ:  true ��ʾֹͣ�ɹ�
//////////////////////////////////////////////////////////////////////////
bool SSntpServer::StopServer()
{
	m_bRun=false;
	while(m_bStoped == false)
		SApi::UsSleep(1000);
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  sntpУʱ������߳�ִ�к�����ѭ�����ܿͻ�������
//            ���udp���ӣ�ʧЧ�����´���
// ��    ��:  �ұ���
// ����ʱ��:  2015-9-25 17:38
// ����˵��:  @arg Ϊ�̴߳��ݲ���
//         :  @
//		   :  @
// �� �� ֵ:  ��
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
		sntp.cLiVnMode = 0x1c;//MEMO: ��Ե�ظ����÷�����ģʽ [2016-3-21 9:18 �ۿ���]
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
// ��    ��:  ȡ��ǰ��������ʱ��
// ��    ��:  �ұ���
// ����ʱ��:  2015-9-24 14:23
// ����˵��:  @nowΪ���÷��صĵ�ǰNTP������ʱ��
//         :  @bSetToSystemΪtrue��ʾʱ���ȡ�ɹ�����Ҫд��ϵͳ��false��ʾ����Ҫд��ϵͳ
// �� �� ֵ:  true��ʾ��ȡ�ɹ���false��ʾ��ȡʧ��
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
// ��    ��:  ʱ��ƫ�����
// ��    ��:  �ۿ���
// ����ʱ��:  2015-12-15 23:02
// ����˵��:  @local_soc
//         :  @local_usec
//         :  @remote_soc
//         :  @remote_usec
//         :  @ts_sec
//         :  @ts_usec
// �� �� ֵ:  true/false
//////////////////////////////////////////////////////////////////////////
bool SNtpClient::TestTimeByTssm(int &local_soc,int &local_usec,int &remote_soc,int &remote_usec,int &ts_sec,int &ts_usec)
{
	SSocket udp;
	stuSNTPPackage sntp;
	sockaddr_in sa;
	struct timeval t0,t1,t2,t3,delta_t;
	//��ʱ�����������Я��ʱ�꣨�����뿪�����ʱ��T0�����ķ��������ʱ��
	//T3 ��������˱��أ���ֱ�ӻ�ȡ�������轫����Ӧ��ָʾ���ݸ�������豸��Ӧ
	//Я��ĳ�ֿ�ʶ���Ӧ�õı�־��������Э���ӳ�����3 �¡�
	//��ʱ�����Ӧ���ر���Ӧ����Я�������ĵ���Ӷ���ʱ��T1 ����Ӧ����
	//�뿪�Ӷ���ʱ��T2
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

	//��t =[(T3 - T2) + (T0 - T1)]/2
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
// ��    ��:  ����ntpУʱ����
// ��    ��:  �ұ���
// ����ʱ��:  2015-9-24 14:21
// ����˵��:  @bTssm �Ƿ�����TSSM��֤
// �� �� ֵ:  false ��ʾ����ʧ��
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
// ��    ��:  ֹͣsntpУʱ���񣬿��ܻ�ȴ�һ��ʱ��
// ��    ��:  �ұ���
// ����ʱ��:  2015-9-24 14:21
// ����˵��:  @
// �� �� ֵ:  true ��ʾֹͣ�ɹ�
//////////////////////////////////////////////////////////////////////////
bool SNtpServer::StopServer()
{
	m_bRun=false;
	while(m_bStoped == false)
		SApi::UsSleep(1000);
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ntpУʱ������߳�ִ�к�����ѭ�����ܿͻ�������
//            ���udp���ӣ�ʧЧ�����´���
// ��    ��:  �ұ���
// ����ʱ��:  2015-9-25 17:38
// ����˵��:  @arg Ϊ�̴߳��ݲ���
//         :  @
//		   :  @
// �� �� ֵ:  ��
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
		sntp.cLiVnMode = 0x1c;//MEMO: ��Ե�ظ����÷�����ģʽ [2016-3-21 9:18 �ۿ���]
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