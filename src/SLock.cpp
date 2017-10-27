/**
 *
 * �� �� ��	: SLock.cpp
 * ��������	: 2006-09-10
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸�����	: $Date: 2011/08/31 08:43:33 $
 * ��ǰ�汾	: $Revision: 1.1 $
 * �������� : ϵͳ����������Linux/Windows
 * �޸ļ�¼ : 
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
 *            ͨѸ��¼����װ����Ŀ�״��ύ
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
 *            �������
 *
 *            Revision 1.2  2007/09/11 08:17:53  shaokt
 *            �������
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
// ��    ��:  ����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 17:12
// ����˵��:  void
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SLock::lock()
{
	m_bTIDError = false;
	#ifdef WIN32
		if(m_CurrTID == (int)GetCurrentThreadId())
		{
			m_bTIDError = true;
			printf("ERROR: �߳�(%X)�����ظ���!!!\n",m_CurrTID);
			return;
		}
		m_CurrTID = (int)GetCurrentThreadId();
	#else
		if(m_CurrTID == (int)pthread_self())
		{
			m_bTIDError = true;
			printf("ERROR: �߳�(%X)�����ظ���!!!��ֹ����\n",m_CurrTID);
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
// ��    ��:  ���Լ�����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 17:12
// ����˵��:  void
// �� �� ֵ:  ����Ѿ��������̼߳ӹ����ˣ�����false���������������true
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
// ��    ��:  ����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 17:13
// ����˵��:  void
// �� �� ֵ:  void
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
// ��    ��:  ȡ��ǰ��״̬
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 17:13
// ����˵��:  void
// �� �� ֵ:  int��0��ʾδ��������>0��ʾ�ѱ�����
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
// ��    ��:  ȡ�ظ�������
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 17:14
// ����˵��:  void
// �� �� ֵ:  true��ʾ�д���false��ʾû�д���
//////////////////////////////////////////////////////////////////////////
bool SLock::IsTidError()
{
	return m_bTIDError;
}
