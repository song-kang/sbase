/**
 *
 * 文 件 名	: SLock.cpp
 * 创建日期	: 2006-09-10
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期	: $Date: 2011/08/31 08:43:33 $
 * 当前版本	: $Revision: 1.1 $
 * 功能描述 : 系统锁、适用于Linux/Windows
 * 修改记录 : 
 *            $Log: SLock.h,v $
 *            Revision 1.1  2011/08/31 08:43:33  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.4  2011/04/29 08:53:08  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2011/04/23 10:53:52  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2011/04/01 02:49:12  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2011/02/25 03:12:07  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2010/08/09 03:04:43  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2010/01/22 01:24:25  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2009/09/15 02:14:08  shaokt
 *            通迅记录分析装置项目首次提交
 *
 *            Revision 1.1  2008/09/01 11:38:28  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/01 08:58:02  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.4  2008/02/18 01:07:55  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2007/09/21 07:53:41  shaokt
 *            基类更新
 *
 *            Revision 1.2  2007/09/11 08:17:53  shaokt
 *            基类更新
 *
 *            Revision 1.1  2007/08/15 03:09:22  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2007/05/28 07:33:59  shaokt
 *            *** empty log message ***
 *
 **/

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SLock.h"

SLock::SLock()
{
	m_bTIDError = false;
	m_CurrTID = 0;
	#ifdef WIN32
	InitializeCriticalSection(&m_cs);
	#endif
}

SLock::~SLock()
{
	#ifdef WIN32
		DeleteCriticalSection(&m_cs);
	#endif
}

////////////////////////////////////////////////////////////////////////
// 描    述:  加锁
// 作    者:  邵凯田
// 创建时间:  2011-11-18 17:12
// 参数说明:  void
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SLock::lock()
{
	m_bTIDError = false;
	#ifdef WIN32
		if(m_CurrTID == (int)GetCurrentThreadId())
		{
			m_bTIDError = true;
			printf("ERROR: 线程(%X)出现重复锁!!!\n",m_CurrTID);
			return;
		}
		m_CurrTID = (int)GetCurrentThreadId();
	#else
		if(m_CurrTID == (int)pthread_self())
		{
			m_bTIDError = true;
			printf("ERROR: 线程(%X)出现重复锁!!!中止程序\n",m_CurrTID);
			char* p = NULL;
			printf("%C",p[1]);
			return;
		}
		m_CurrTID = (int)pthread_self();
	#endif
	
	#ifdef WIN32
	try
	{
		//InitializeCriticalSectionAndSpinCount(&m_cs,1);
		EnterCriticalSection(&m_cs);
	}
	catch(...)
	{
	}
	#else
		m_Mutex.lock();
	#endif
}

////////////////////////////////////////////////////////////////////////
// 描    述:  尝试加锁，
// 作    者:  邵凯田
// 创建时间:  2011-11-18 17:12
// 参数说明:  void
// 返 回 值:  如果已经被其它线程加过锁了，返回false，否则加上锁返回true
//////////////////////////////////////////////////////////////////////////
bool SLock::trylock()
{
//		bool b = false;
	#ifdef WIN32
		if(m_cs.RecursionCount == 0)//TryEnterCriticalSection(&m_cs))
		{
			lock();
			return true;
		}
		else
		{
			return false;
		}
	#else
		return m_Mutex.trylock();
	#endif
}

////////////////////////////////////////////////////////////////////////
// 描    述:  解锁
// 作    者:  邵凯田
// 创建时间:  2011-11-18 17:13
// 参数说明:  void
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SLock::unlock()
{
	//if(m_CurrTID == 0)
	//	return;
	#ifdef WIN32
		if(m_cs.RecursionCount>0)
			LeaveCriticalSection(&m_cs);
	#else
		m_Mutex.unlock();
	#endif
	m_CurrTID = 0;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  取当前锁状态
// 作    者:  邵凯田
// 创建时间:  2011-11-18 17:13
// 参数说明:  void
// 返 回 值:  int，0表示未被锁定，>0表示已被锁定
//////////////////////////////////////////////////////////////////////////
int SLock::get_value()
{
#ifdef WIN32
	return m_cs.RecursionCount;
#else
	return m_Mutex.get_value();
#endif
}

////////////////////////////////////////////////////////////////////////
// 描    述:  取重复锁错误
// 作    者:  邵凯田
// 创建时间:  2011-11-18 17:14
// 参数说明:  void
// 返 回 值:  true表示有错误，false表示没有错误
//////////////////////////////////////////////////////////////////////////
bool SLock::IsTidError()
{
	return m_bTIDError;
}
