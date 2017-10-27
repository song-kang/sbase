/**
 *
 * 文 件 名 : SWait.h
 * 创建日期 : 2015-01-09 10:14
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date:  $
 * 当前版本 : $Revision:  $
 * 功能描述 : 异步等待封装类
 * 修改记录 : 
 *            $Log: $

 **/

#ifndef __SKT_WAIT_H__
#define __SKT_WAIT_H__

#include "SApi.h"
#ifdef WIN32
//#include <wtypes.h>
#else
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <errno.h>
#include <pthread.h>
#endif

//////////////////////////////////////////////////////////////////////////
// 名    称:  SWait
// 作    者:  邵凯田
// 创建时间:  2015-01-09 10:14
// 描    述:  异步等待封装类
//////////////////////////////////////////////////////////////////////////
class SBASE_EXPORT SWait
{
public:
	SWait();
	~SWait();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  触发信号
	// 作    者:  邵凯田
	// 创建时间:  2015-01-09 11:02
	// 参数说明:  @timeout_ms表示超时时间（单位毫秒），0表示不超时（死等）
	// 返 回 值:  true表示是信号被触发，false表示等待超时或错误
	//////////////////////////////////////////////////////////////////////////
	void post();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  等待信号被触发
	// 作    者:  邵凯田
	// 创建时间:  2015-01-09 11:04
	// 参数说明:  @timeout_ms表示超时时间（单位毫秒），0表示不超时（死等）
	// 返 回 值:  true表示是信号被触发，false表示等待超时或错误
	//////////////////////////////////////////////////////////////////////////
	bool wait(int timeout_ms);

private:

#ifdef WIN32
	HANDLE m_event;
#else
	pthread_mutex_t m_mutex;
	pthread_cond_t m_cond;
#endif
};

#endif//__SKT_WAIT_H__
