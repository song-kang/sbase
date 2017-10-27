/**
 *
 * 文 件 名 : SWait.cpp
 * 创建日期 : 2015-01-09 10:14
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date:  $
 * 当前版本 : $Revision:  $
 * 功能描述 : 异步等待封装类
 * 修改记录 : 
 *            $Log: $

 **/

#include "SWait.h"
#ifdef WIN32
#include <winbase.h>
#else
#endif

//#define _SWAIT_DEBUG

//////////////////////////////////////////////////////////////////////////
// 名    称:  SWait
// 作    者:  邵凯田
// 创建时间:  2015-01-09 10:14
// 描    述:  异步等待封装类
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
// 描    述:  触发信号
// 作    者:  邵凯田
// 创建时间:  2015-01-09 11:02
// 参数说明:  @timeout_ms表示超时时间（单位毫秒），0表示不超时（死等）
// 返 回 值:  true表示是信号被触发，false表示等待超时或错误
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
// 描    述:  等待信号被触发
// 作    者:  邵凯田
// 创建时间:  2015-01-09 11:04
// 参数说明:  @timeout_ms表示超时时间（单位毫秒），0表示不超时（死等）
// 返 回 值:  true表示是信号被触发，false表示等待超时或错误
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

