/**
 *
 * �� �� �� : SWait.cpp
 * �������� : 2015-01-09 10:14
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date:  $
 * ��ǰ�汾 : $Revision:  $
 * �������� : �첽�ȴ���װ��
 * �޸ļ�¼ : 
 *            $Log: $

 **/

#include "SWait.h"
#ifdef WIN32
#include <winbase.h>
#else
#endif

//#define _SWAIT_DEBUG

//////////////////////////////////////////////////////////////////////////
// ��    ��:  SWait
// ��    ��:  �ۿ���
// ����ʱ��:  2015-01-09 10:14
// ��    ��:  �첽�ȴ���װ��
//////////////////////////////////////////////////////////////////////////

SWait::SWait()
{
#ifdef WIN32
	m_event = CreateEvent(NULL, FALSE, FALSE, NULL);
#else
	pthread_mutex_init(&m_mutex, NULL);
	pthread_cond_init(&m_cond, NULL);
#ifdef _SWAIT_DEBUG
	printf("@@@@@@@@@@@@@@@@@@@ new SWait=%p\n",this);
#endif
#endif
};

SWait::~SWait()
{
#ifdef WIN32
	CloseHandle(m_event);
#else
	pthread_cond_destroy(&m_cond);
	pthread_mutex_destroy(&m_mutex);
#ifdef _SWAIT_DEBUG
	printf("@@@@@@@@@@@@@@@@@@@ free SWait=%p\n",this);
#endif
#endif
};

////////////////////////////////////////////////////////////////////////
// ��    ��:  �����ź�
// ��    ��:  �ۿ���
// ����ʱ��:  2015-01-09 11:02
// ����˵��:  @timeout_ms��ʾ��ʱʱ�䣨��λ���룩��0��ʾ����ʱ�����ȣ�
// �� �� ֵ:  true��ʾ���źű�������false��ʾ�ȴ���ʱ�����
//////////////////////////////////////////////////////////////////////////
void SWait::post()
{
#ifdef WIN32
	SetEvent(m_event);
#else
	//pthread_mutex_lock(&m_mutex); 
	pthread_cond_signal(&m_cond);
	//pthread_mutex_unlock(&m_mutex); 
#ifdef _SWAIT_DEBUG
	printf("@@@@@@@@@@@@@@@@@@@ post SWait=%p\n",this);
#endif
#endif
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  �ȴ��źű�����
// ��    ��:  �ۿ���
// ����ʱ��:  2015-01-09 11:04
// ����˵��:  @timeout_ms��ʾ��ʱʱ�䣨��λ���룩��0��ʾ����ʱ�����ȣ�
// �� �� ֵ:  true��ʾ���źű�������false��ʾ�ȴ���ʱ�����
//////////////////////////////////////////////////////////////////////////
bool SWait::wait(int timeout_ms)
{
#ifdef WIN32
	if (timeout_ms == 0)
		return (WaitForSingleObject(m_event, INFINITE) == WAIT_OBJECT_0);
	return (WaitForSingleObject(m_event, timeout_ms) == WAIT_OBJECT_0);
#else
	bool ret=false;
	//pthread_mutex_lock(&m_mutex); 
	if (timeout_ms == 0)
	{
		ret = (pthread_cond_wait(&m_cond, &m_mutex) == 0);
	}
	else
	{
		struct timeval tv;
		gettimeofday(&tv, 0);
		timespec ti;
		ti.tv_nsec = (tv.tv_usec + (timeout_ms % 1000) * 1000) * 1000;
		ti.tv_sec = tv.tv_sec + (timeout_ms / 1000) + (ti.tv_nsec / 1000000000);
		ti.tv_nsec %= 1000000000;
#ifdef _SWAIT_DEBUG
		printf("@@@@@@@@@@@@@@@@@@@ begin wait SWait(%d)=%p sec=%d,nsec=%d\n",timeout_ms,this,(int)ti.tv_sec,(int)ti.tv_nsec);
#endif
		ret = (pthread_cond_timedwait(&m_cond, &m_mutex, &ti) == 0);
#ifdef _SWAIT_DEBUG
		printf("@@@@@@@@@@@@@@@@@@@ end wait SWait(%d)=%p, ret=%d\n",timeout_ms,this,ret);
#endif
	}
	//pthread_mutex_unlock(&m_mutex); 
	return ret;
#endif
}

