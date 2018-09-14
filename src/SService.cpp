/**
 *
 * �� �� �� : SService.cpp
 * �������� : 2011-11-4 13:48
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : 
 * �޸ļ�¼ : 
 *            $Log: $
 *
 **/

#include "SService.h"

SService::SService()
{
	m_iRunThreads = 0;
	m_bQuit = false;
	m_bRuning = false;
}

SService::~SService()
{
	if(m_bRuning || m_iRunThreads > 0)
	{
		StopAndWait();
	}
}


////////////////////////////////////////////////////////////////////////
// ��    ��:  ��������,�麯�������������б����ȵ��ô˺���
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-4 13:51
// ����˵��:  void
// �� �� ֵ:  true��ʾ���������ɹ�,false��ʾ��������ʧ��
//////////////////////////////////////////////////////////////////////////
bool SService::Start()
{
	IsStoped();
	if(m_bRuning)
	{
		//LOGERROR("�����Ѿ���������״̬�������ٴ�����!");
		return false;
	}
	m_bQuit = false;
	m_iRunThreads = 0;
	m_bRuning = true;
	return true;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ֹͣ�������������б����ȵ��ô˺���
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-4 13:52
// ����˵��:  void
// �� �� ֵ:  true��ʾ����ֹͣ�ɹ�,false��ʾ����ֹͣʧ��
//////////////////////////////////////////////////////////////////////////
bool SService::Stop()
{
	if(!m_bRuning)
	{
		m_bQuit = true;
		//LOGWARN("������δ���У�����ֹͣ!");
		return false;
	}
	m_bQuit = true;
	return true;
}


////////////////////////////////////////////////////////////////////////
// ��    ��:  ֹͣ����ͬʱ�ڸ����ĳ�ʱʱ���ڵȴ�������ȫ����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-4 14:02
// ����˵��:  @iTimeout��ʾ��ʱ������0��ʾ����ʱ(һֱ������ȥ)
// �� �� ֵ:  true��ʾֹͣ�ɹ�,false��ʾֹͣʧ��
//////////////////////////////////////////////////////////////////////////
bool SService::StopAndWait(int iTimeout/*=0*/)
{
	Stop();
	SApi::UsSleep(20000);//��ֹ�մ���(�������߳�)������ʱ����Ұ�߳�
	iTimeout *= 20;
	int times = 0;
	while(!IsStoped())
	{
		times++;
		if(iTimeout > 0 && times > iTimeout)
		{
			m_bRuning = false;
			LOGERROR("ֹͣ����ʱ!");
			return false;
		}
		SApi::UsSleep(10000);
	}
	m_bRuning = false;
	return true;
}


////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ʼһ�������߳�
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-7 16:15
// ����˵��:  void
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SService::BeginThread()
{
	#ifndef WIN32
		pthread_detach(pthread_self());
		signal(SIGPIPE,SIG_IGN);
		sigset_t signal_mask;
		sigemptyset (&signal_mask);
		sigaddset (&signal_mask, SIGPIPE);
	#endif
	m_iRunThreads++;
};

////////////////////////////////////////////////////////////////////////
// ��    ��:  ����һ�������߳�
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-7 16:15
// ����˵��:  void
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SService::EndThread()
{
	m_iRunThreads--;
	if(m_iRunThreads < 0)
		m_iRunThreads = 0;
};
