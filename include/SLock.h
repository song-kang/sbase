/**
 *
 * �� �� ��	: SLock.h
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

#if !defined(AFX_SKTLOCK_H__3082C864_723C_4BE6_AB17_2CFE75460894__INCLUDED_)
#define AFX_SKTLOCK_H__3082C864_723C_4BE6_AB17_2CFE75460894__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SObject.h"
#ifdef WIN32
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <string.h>
#include <time.h>		/* timespec{} for pselect() */
#include <stdio.h>
#include <winsock2.h> 
#else
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <errno.h>
#include <pthread.h>
#endif

#ifdef WIN32
#else
//�������Posix�źŵƻ���,�ڴ��źŵ�
class SBASE_EXPORT CLocalMutex 
{
public:
    CLocalMutex(int isLocal=0,unsigned int value=1)
    {
		Open_Semaphore(isLocal,value);
    }
    
    CLocalMutex(bool isManul)
    {
		if(!isManul)
		{
			Open_Semaphore();
		}
    }
    
    ~CLocalMutex()
    {
		Destroy_Semaphore();
    }
    
public:
    sem_t	m_sem;

    bool Open_Semaphore(int isLocal=0,unsigned int value=0)
    {
		int rc=sem_init(&m_sem,isLocal,value);
	//	printf("rc=%d\n",rc);
		if(rc<0)
		{
			printf("sem_init error!ret=%d\n",rc);
			return false;
		}
		return true;
    }
    
    bool Destroy_Semaphore()
    {
		int rc=sem_destroy(&m_sem);
		return (!(rc<0));
    }
    
    bool lock()
    {
		int rc = sem_wait(&m_sem);
        while(rc != 0 && errno == EINTR)//Interrupted system call
        {
            printf(" sem_wait ret=%d, errno=%d/EINTR Interrupted system call !!!!!!!!!!!!!!\n",rc,errno);
            rc = sem_wait(&m_sem);
        }
		if(rc != 0)
		{
			printf(" sem_wait ret=%d, errno=%d !!!!!!!!!!!!!!\n",rc,errno);
			abort();
		}
		return true;
    }
    
    bool trylock()
    {
		int rc=sem_trywait(&m_sem);
		//by skt return !(rc==EAGAIN);
		return rc==0;
    }
    
    bool unlock()
    {
		sem_post(&m_sem);
		return true;
    }
	int get_value()
	{
		int i=0;
		
		int rc = sem_getvalue(&m_sem,&i);
		if(rc != 0)
		{
			printf("sem_getvalue error,rc=%d\n",rc);
			return -1;
		}
		return i;
	}
};

class SBASE_EXPORT CLMutexLocker 
{
public:
    CLMutexLocker(CLocalMutex *LocalMutex,bool bWait=false)
    {
		m_LocalMutex=LocalMutex;
		if(!bWait)
			m_LocalMutex->trylock();
		else
			m_LocalMutex->lock();
    }
    
    ~CLMutexLocker()
    {
		m_LocalMutex->unlock();
    }
    
    public:
    CLocalMutex* GetMutex()
    {
		return m_LocalMutex;
    }
    
private:
    CLocalMutex* m_LocalMutex;
};

#endif

#define MUTEX_TIMEOUT 60000	//�ź����ĳ�ʱ������

class SBASE_EXPORT SLock  
{
public:
//	int cnt;
#ifdef WIN32
	CRITICAL_SECTION m_cs;
#endif
	SLock();

	virtual ~SLock();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 17:12
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void lock();
	
	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���Լ�����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 17:12
	// ����˵��:  void
	// �� �� ֵ:  ����Ѿ��������̼߳ӹ����ˣ�����false���������������true
	//////////////////////////////////////////////////////////////////////////
	bool trylock();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 17:13
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void unlock();
	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��ǰ��״̬
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 17:13
	// ����˵��:  void
	// �� �� ֵ:  int��0��ʾδ��������>0��ʾ�ѱ�����
	//////////////////////////////////////////////////////////////////////////
	int get_value();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ�ظ�������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 17:14
	// ����˵��:  void
	// �� �� ֵ:  true��ʾ�д���false��ʾû�д���
	//////////////////////////////////////////////////////////////////////////
	bool IsTidError();

private:
	int m_CurrTID;
	bool m_bTIDError;//�߳��ظ����Ĵ���
#ifdef WIN32
#else
	CLocalMutex m_Mutex;
#endif
};

#endif // !defined(AFX_SKTLOCK_H__3082C864_723C_4BE6_AB17_2CFE75460894__INCLUDED_)
