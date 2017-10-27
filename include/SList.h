/**
 *
 * �� �� ��	: SList.h
 * ��������	: 2006-09
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸�����	: $Date: 2011/08/31 08:43:33 $
 * ��ǰ�汾	: $Revision: 1.1 $
 * �������� : ��QT������ģ���� SPtrList SValueList SClassList
 * �޸ļ�¼ :
 *            $Log: SList.h,v $
 *            Revision 1.1  2011/08/31 08:43:33  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.5  2011/04/26 01:16:55  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.4  2011/04/26 00:52:37  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2011/04/04 06:27:29  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2011/03/21 05:14:56  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2011/02/25 03:12:07  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2010/09/28 08:14:23  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2010/08/31 07:19:28  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2010/08/09 03:04:43  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2010/01/22 01:24:25  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2009/09/18 07:46:31  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2009/09/15 02:14:08  shaokt
 *            ͨѸ��¼����װ����Ŀ�״��ύ
 *
 *            Revision 1.3  2009/02/21 15:55:39  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2008/10/06 08:48:28  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/01 11:38:28  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/01 08:58:01  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.5  2008/02/18 01:07:55  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.4  2007/10/29 07:30:23  shaokt
 *            ��Ӷ����ַ�Ƿ�Ϊ4�ı����ļ�⺯����Ϊ���ARM������pack(1)��ɵ��ڴ�����
 *
 *            Revision 1.3  2007/09/21 07:53:41  shaokt
 *            �������
 *
 *            Revision 1.2  2007/09/11 08:17:52  shaokt
 *            �������
 *
 *            Revision 1.1  2007/08/15 03:09:22  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2007/05/28 07:33:59  shaokt
 *            *** empty log message ***
 *
 **/

#if !defined(AFX_SKTPTRLIST_H__12695A0C_F4BA_4DF7_9F35_17CD7703DDE1__INCLUDED_)
#define AFX_SKTPTRLIST_H__12695A0C_F4BA_4DF7_9F35_17CD7703DDE1__INCLUDED_
#include <list>

#include "SLock.h"
#include "SObject.h"

#ifndef NULL
#define NULL 0
#endif

#ifndef ASSERT
#define ASSERT
#endif

class SConstString;

//////////////////////////////////////////////////////////////////////////
// ��    ��:  SPtrList
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 15:57
// ��    ��:  ��ָ̬�����ģ����
//////////////////////////////////////////////////////////////////////////
template <class type>
class SPtrList
{
public:
	struct NODE
	{
		type *pObj;
		NODE* pNext;
	};
	struct FREENODE
	{
		NODE* p;
		int iDelayUS;//��ʾ��ʱ΢����
		bool bAutoDel;
	};
	SPtrList()
	{
		SKTOBJECT("SPtrList");
		m_pNode = NULL;
		m_iSize  = 0;
		m_bAutoDel = false;
		m_pNode = NULL;
		m_pTail = NULL;
		m_pLock = NULL;
		m_pFetchPrev = NULL;
	};
	virtual ~SPtrList()
	{
		clear();
		if(m_pLock != NULL)
		{
			delete m_pLock;
			m_pLock = NULL;
		}
	};

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����Զ��ͷ�ģʽ���Ƿ���ɾ���ڵ�ʱ�ͷŽڵ��Ӧ�Ķ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 14:27
	// ����˵��:  @b�Ƿ��ͷŶ���
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void setAutoDelete(bool b)
	{
		m_bAutoDel = b;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �Ƿ�Ϊ�̹߳���ģʽ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 14:29
	// ����˵��:  void
	// �� �� ֵ:  true��ʾ���̹߳���ģʽ��false��ʾ���ǹ���ģʽ
	//////////////////////////////////////////////////////////////////////////
	bool isShared() {return m_pLock != NULL;};

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����̹߳���ģʽ�������߳̿��ܲ�����дʱ��Ҫ��Ϊtrue
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 14:29
	// ����˵��:  @b�Ƿ��̹߳���ģʽ
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void setShared(bool b) 
	{
		if(b)
		{
			if(m_pLock == NULL)
				m_pLock = new SLock();
		}
		else
		{
			if(m_pLock != NULL)
			{
				delete m_pLock;
				m_pLock = NULL;
			}
		}
	};

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���м���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 14:30
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	inline void lock()
	{
		if(m_pLock != NULL)
			m_pLock->lock();
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���н���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 14:30
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	inline void unlock()
	{
		if(m_pLock != NULL)
			m_pLock->unlock();
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����ǰ�����е����ݸ��Ƶ�list��ʾ����һ��������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 14:31
	// ����˵��:  @listΪĿ�����
	// ����˵��:  @bNeedLock��ʾ�Ƿ���Ҫ����(��������ģʽʱ��Ч)��true��ʾ��Ҫ������false��ʾ����Ҫ�����ⲿ�Ѿ���������
	// ����˵��:  @bNeedLockList��ʾ�Ƿ���Ҫ��list����(��������ģʽʱ��Ч)��true��ʾ��Ҫ��list������false��ʾ����Ҫ�����ⲿ�Ѿ���������
	// �� �� ֵ:  ���ظ��ƹ�ȥ������
	//////////////////////////////////////////////////////////////////////////
	int copyto(SPtrList<type> &list,bool bNeedLock=true,bool bNeedLockList=true)
	{
		if(bNeedLock)
			lock();
		unsigned long  pos=0;
		type *p = FetchFirst(pos);
		while(p)
		{
			list.append(p,bNeedLockList);
			p = FetchNext(pos);
		}
		if(bNeedLock)
			unlock();
		return count();
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���ú�̨�߳�ɾ��ȫ���ڵ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 14:33
	// ����˵��:  @iDelayMs��ʾ��ʱ����΢���ͷ����ݶ���0��ʾ����ʱ
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void clearbk(int iDelayUS = 0)
	{
		lock();
		NODE *p=m_pNode;
		m_pNode = NULL;
		m_iSize  = 0;
		unlock();
		FREENODE *pNode = new FREENODE();
		pNode->p = p;
		pNode->iDelayUS = iDelayUS;
		pNode->bAutoDel = m_bAutoDel;

#ifdef WIN32
		HANDLE h;
		h = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFree, (LPVOID)pNode, 0, 0);
		CloseHandle(h);
	#else
		pthread_t t;
		pthread_create(&t, NULL, ThreadFree,(void*) pNode);
	#endif	
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ɾ��ȫ���ڵ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 14:34
	// ����˵��:  @bNeedLock��ʾ�Ƿ���Ҫ����(��������ģʽʱ��Ч)��true��ʾ��Ҫ������false��ʾ����Ҫ�����ⲿ�Ѿ���������
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void clear(bool bNeedLock=true)
	{
		if(bNeedLock)
			lock();
		NODE *next,*p=m_pNode;
		m_pNode = NULL;
		m_iSize  = 0;
		while(p)
		{
			next = p->pNext;
			if(m_bAutoDel)
				delete p->pObj;
			delete p;
			p = next;
		}
		if(bNeedLock)
			unlock();
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ɾ��ָ��ָ��Ķ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 14:34
	// ����˵��:  @pObjΪ��ɾ���Ķ���ָ��
	//         :  @bNeedLock��ʾ�Ƿ���Ҫ����(��������ģʽʱ��Ч)��true��ʾ��Ҫ������false��ʾ����Ҫ�����ⲿ�Ѿ���������
	// �� �� ֵ:  true��ʾɾ���ɹ���falseʧ��
	//////////////////////////////////////////////////////////////////////////
	bool remove(type *pObj,bool bNeedLock=true)
	{
		if(bNeedLock)
			lock();
		NODE *p=m_pNode,*prev=NULL;
		while(p)
		{
			if(p->pObj == pObj)
			{
				if(prev == NULL)//first?
				{
					m_pNode = p->pNext;
					if(p == m_pTail)
						m_pTail = m_pNode;
				}
				else
				{
					prev->pNext = p->pNext;
					if(p == m_pTail)
						m_pTail = prev;
				}
				m_iSize --;
				if(m_bAutoDel)
					delete p->pObj;
				delete p;
				if(bNeedLock)
					unlock();
				return true;
			}
			prev = p;
			p = p->pNext;
		}
		if(bNeedLock)
			unlock();
		return false;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ָ���Ķ����Ƿ����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 14:37
	// ����˵��:  @pObjΪ�����ָ��
	// �� �� ֵ:  true��ʾ�˶�����ڣ�false��ʾ������
	//////////////////////////////////////////////////////////////////////////
	bool exist(type *pObj)
	{
		return position(pObj) >= 0;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡָ�������ڶ����е�λ��,��0��ʼ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 14:44
	// ����˵��:  @pObjΪ�����ָ��
	//         :  @bNeedLock��ʾ�Ƿ���Ҫ����(��������ģʽʱ��Ч)��true��ʾ��Ҫ������false��ʾ����Ҫ�����ⲿ�Ѿ���������
	// �� �� ֵ:  >0Ϊ����λ�ã�-1��ʾ���󲻴���
	//////////////////////////////////////////////////////////////////////////
	int position(type *pObj,bool bNeedLock=true)
	{
		if(bNeedLock)
			lock();
		NODE *p=m_pNode;
		int i=-1;
		while(p)
		{
			i++;
			if(p->pObj == pObj)
			{
				if(bNeedLock)
					unlock();
				return i;
			}
			p = p->pNext;
		}
		if(bNeedLock)
			unlock();
		return -1;
	}
	
	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �滻ָ��λ�õĶ���Ϊһ���µĶ���ָ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 14:48
	// ����˵��:  @i��ʾ�����е�λ�ã���0��ʼ���
	//            @pObj��ʾ�µĶ���ָ��
	//            @bRemoveOld��ʾ�Ƿ��ͷŽ����滻�Ķ��󣬽����Զ��ͷ�ģʽʱ��Ч
	// �� �� ֵ:  true��ʾ�滻�ɹ���false��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	bool replace(int i, type*pObj,bool bRemoveOld=false)
	{
		lock();
		NODE *p=m_pNode,*prev=NULL;
		int cnt=0;
		while(p)
		{
			if(i == cnt)
			{
				if(m_bAutoDel && bRemoveOld && p->pObj != NULL)
				{
					delete p->pObj;
				}
				p->pObj = pObj;
				unlock();
				return true;
			}
			prev = p;
			p = p->pNext;
			cnt ++;
		}
		unlock();
		return false;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ɾ��ָ��λ��i�Ľڵ㣬i��0��ʼ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 14:50
	// ����˵��:  @i��ɾ���Ľڵ�λ�ã���0��ʼ
	//         :  @bNeedLock��ʾ�Ƿ���Ҫ����(��������ģʽʱ��Ч)��true��ʾ��Ҫ������false��ʾ����Ҫ�����ⲿ�Ѿ���������
	// �� �� ֵ:  true��ʾɾ���ɹ���false��ʾɾ��ʧ��
	//////////////////////////////////////////////////////////////////////////
	bool remove(int i,bool bNeedLock=true)
	{
		if(bNeedLock)
			lock();
		NODE *p=m_pNode,*prev=NULL;
		int cnt=0;
		while(p)
		{
			if(i == cnt)
			{
				if(prev == NULL)//first?
				{
					m_pNode = p->pNext;
					if(p == m_pTail)
						m_pTail = m_pNode;
				}
				else
				{
					prev->pNext = p->pNext;
					if(p == m_pTail)
						m_pTail = prev;
				}
				m_iSize --;
				if(m_bAutoDel)
					delete p->pObj;
				delete p;
				if(bNeedLock)
					unlock();
				return true;
			}
			prev = p;
			p = p->pNext;
			cnt ++;
		}
		if(bNeedLock)
			unlock();
		return false;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ɾ��ȫ���ڵ㣬���Ƽ�ʹ�ã�����ʹ��clear����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 15:04
	// ����˵��:  void
	// �� �� ֵ:  true
	//////////////////////////////////////////////////////////////////////////
	bool removeAll()
	{
		while(count()>0)
			remove((int)0);
		return true;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ���е��ܽڵ�����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 15:05
	// ����˵��:  void
	// �� �� ֵ:  �ڵ�����
	//////////////////////////////////////////////////////////////////////////
	inline int count()
	{
		return m_iSize;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����Ƿ�Ϊ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 15:06
	// ����˵��:  void
	// �� �� ֵ:  true��ʾ����Ϊ�գ�false��ʾ���зǿ�
	//////////////////////////////////////////////////////////////////////////
	inline bool isEmpty()
	{
		return m_iSize == 0;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����ĩβ���һ���µĽڵ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 15:06
	// ����˵��:  @pObjΪ�½ڵ��Ӧ�Ķ���
	//         :  @bNeedLock��ʾ�Ƿ���Ҫ����(��������ģʽʱ��Ч)��true��ʾ��Ҫ������false��ʾ����Ҫ�����ⲿ�Ѿ���������
	// �� �� ֵ:  true
	//////////////////////////////////////////////////////////////////////////
	bool append(type *pObj,bool bNeedLock=true)
	{
		if(bNeedLock)
			lock();
		NODE *p=m_pNode;
		NODE *pNew = new NODE;
		pNew->pObj = pObj;
		pNew->pNext = NULL;
		if(p == NULL)
		{
			m_pTail = m_pNode = pNew;
		}
		else if(m_pTail != NULL)
		{
			//���ٲ��뵽β�ڵ�
			pNew->pNext = m_pTail->pNext;
			m_pTail->pNext = pNew;
			m_pTail = pNew;
		}
		else
		{
			while(p->pNext)
				p = p->pNext;
			p->pNext = pNew;
			m_pTail = pNew;
		}
		m_iSize ++;
		if(bNeedLock)
			unlock();
		return true;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ָ���ڵ�ǰ�����½ڵ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2009-2-16 9:16
	// ����˵��:  pNodeΪ�ο��ڵ�NULL��ʾ�ӵ����pObjΪ�½ڵ�
	//         :  @bNeedLock��ʾ�Ƿ���Ҫ����(��������ģʽʱ��Ч)��true��ʾ��Ҫ������false��ʾ����Ҫ�����ⲿ�Ѿ���������
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool insertBefore(type* pNode,type *pObj,bool bNeedLock=true)
	{
		if(bNeedLock)
			lock();
		NODE *p=m_pNode,*prev=NULL;
		NODE *pNew = new NODE;
		pNew->pObj = pObj;
		if (p == NULL)
		{
			pNew->pNext = NULL;
			m_iSize++;
			m_pTail = m_pNode = pNew;
		}
		else
		{
			if(pNode == NULL)
			{
				pNew->pNext = NULL;
				m_pTail->pNext = pNew;
				m_pTail = pNew;
				m_iSize++;
				if(bNeedLock)
					unlock();
				return true;
			}
			while(p != NULL)
			{
				if(p->pObj == pNode)
				{
					pNew->pNext = p;
					if(prev == NULL)//first?
					{
						if(m_pNode == NULL)
							m_pTail = pNew;
						m_pNode = pNew;
					}
					else
					{
						prev->pNext = pNew;
					}
					m_iSize ++;
					if(bNeedLock)
						unlock();
					return true;
				}
				prev = p;
				p = p->pNext;
			}
		}
		if(bNeedLock)
			unlock();
		return false;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ָ���ڵ������½ڵ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2009-2-16 9:16
	// ����˵��:  pNodeΪ�ο��ڵ�NULL��ʾ�ӵ����pObjΪ�½ڵ�
	//         :  @bNeedLock��ʾ�Ƿ���Ҫ����(��������ģʽʱ��Ч)��true��ʾ��Ҫ������false��ʾ����Ҫ�����ⲿ�Ѿ���������
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool insertAfter(type* pNode,type *pObj,bool bNeedLock=true)
	{
		if(bNeedLock)
			lock();
		NODE *p=m_pNode;
		NODE *pNew = new NODE;
		pNew->pObj = pObj;
		if (p == NULL)
		{
			pNew->pNext = NULL;
			m_iSize++;
			m_pTail = m_pNode = pNew;
		}
		else
		{
			if(pNode == NULL)
			{
				pNew->pNext = NULL;
				m_pTail->pNext = pNew;
				m_pTail = pNew;
				m_iSize++;
				if(bNeedLock)
					unlock();
				return true;
			}
			while (p != NULL/* && p->pNext != NULL*/)
			{
				if (p->pObj == pNode)
				{
					pNew->pNext = p->pNext;
					p->pNext = pNew;
					if (p == m_pTail)
						m_pTail = pNew;
					m_iSize++;
					if(bNeedLock)
						unlock();
					return true;
				}
				p = p->pNext;
			}
		}
		if(bNeedLock)
			unlock();
		return false;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡָ��λ�õĽڵ����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 15:13
	// ����˵��:  @i��ʾ��ȡ�����ڶ����е�λ�ã�i��0��ʼ���
	//         :  @bNeedLock��ʾ�Ƿ���Ҫ����(��������ģʽʱ��Ч)��true��ʾ��Ҫ������false��ʾ����Ҫ�����ⲿ�Ѿ���������
	// �� �� ֵ:  ����ָ�룬NULL��ʾδ�ҵ�
	//////////////////////////////////////////////////////////////////////////
	type * at(int i,bool bNeedLock=true)
	{
		if(bNeedLock)
			lock();
		NODE *p=m_pNode;
		int cnt=0;
		while(p)
		{
			if(i == cnt)
			{
				if(bNeedLock)
					unlock();
				return p->pObj;
			}
			p = p->pNext;
			cnt ++;
		}
		if(bNeedLock)
			unlock();
		return NULL;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡָ��λ�õĽڵ����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 15:13
	// ����˵��:  @i��ʾ��ȡ�����ڶ����е�λ�ã�i��0��ʼ���
	// �� �� ֵ:  ����ָ�룬NULL��ʾδ�ҵ�
	//////////////////////////////////////////////////////////////////////////
	type * operator[](int i)
	{
		return at(i);
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �ƶ����ڵ�Ŀ�ʼλ�ã��˺�����FetchNextһ��ʹ�ã�
	//            ������̲߳���ȫ�ģ���Ҫ�ڱ���ǰ��ӽ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 15:14
	// ����˵��:  @pos��ʾ���÷��ص�λ����Ϣ��Ϊ�´�Fetchʱʹ��
	// �� �� ֵ:  ����ָ�룬NULL��ʾδ�ҵ�
	//////////////////////////////////////////////////////////////////////////
	type* FetchFirst(unsigned long &pos)
	{
		m_pFetchPrev = NULL;
		if(m_pNode == NULL)
			return NULL;
		else
		{
			pos = (unsigned long)m_pNode;
			return m_pNode->pObj;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ɾ��Fetch Pos��Ӧ�Ľڵ�(������)������FetchNext,ɾ����ǰ�ڵ㣬����ת����һ���ڵ�
	//            ������̲߳���ȫ�ģ���Ҫ�ڱ���ǰ��ӽ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-10-20 15:09
	// ����˵��:  @pos��ʾ��ɾ���Ľڵ�pos��������ɺ����÷��ص���һλ����Ϣ��
	// �� �� ֵ:  true��ʾɾ���ɹ���false��ʾɾ��ʧ��
	//////////////////////////////////////////////////////////////////////////
	type* FetchNextAndRemoveThis(unsigned long &pos)
	{
		NODE* p = (NODE*)pos;
		NODE* p0 = p;
		if(p == NULL)
			return NULL;
		if(m_pFetchPrev == NULL)
		{
			//û��ǰ���ڵ㣬��Ϊ�ǵ�һ���ڵ�
			if(m_pNode != p)
				return NULL;//���ǲ��ǵ�һ���ڵ㣬δ֪����
			m_pNode = p->pNext;
			p = m_pNode;
		}
		else
		{
			m_pFetchPrev->pNext = p->pNext;
			p = p->pNext;
		}
		if(p0 == m_pTail)
			m_pTail = m_pFetchPrev;
		if(m_bAutoDel)
			delete p0->pObj;
		delete p0;
		if(p == NULL)
			return NULL;
		pos = (unsigned long)p;
		return p->pObj;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �ƶ����ڵ��ָ��λ�ã�id��0��ʼ���˺�����FetchNextһ��ʹ�ã�
	//            ������̲߳���ȫ�ģ���Ҫ�ڱ���ǰ��ӽ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 15:14
	// ����˵��:  @idΪ��Ҫ�ƶ�����λ�ã�id��0��ʼ
	//            @pos��ʾ���÷��ص�λ����Ϣ��Ϊ�´�Fetchʱʹ��
	// �� �� ֵ:  ����ָ�룬NULL��ʾδ�ҵ�
	//////////////////////////////////////////////////////////////////////////
	type* FetchToIndex(int id,unsigned long &pos)
	{
		int cnt = 0;
		type* p = FetchFirst(pos);
		while(p != NULL && cnt < id)
		{
			p = FetchNext(pos);
			cnt++;
		}
		return p;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �ƶ��´�һ���ڵ�λ�ã��˺�����FetchFirst��һ��ʹ�ã�
	//            ������̲߳���ȫ�ģ���Ҫ�ڱ���ǰ��ӽ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 15:14
	// ����˵��:  @idΪ��Ҫ�ƶ�����λ�ã�id��0��ʼ
	//            @pos��ʾ���÷��ص�λ����Ϣ��Ϊ�´�Fetchʱʹ��
	// �� �� ֵ:  ����ָ�룬NULL��ʾ����ײ�����ʱposֵ�����0���´����¼�¼�����
	//            �ٴ�ʹ��FetchNext(����pos����)����Ȼ�ܵõ���һ����¼
	//////////////////////////////////////////////////////////////////////////
	type* FetchNext(unsigned long &pos)
	{
		NODE* p = (NODE*)pos;
		m_pFetchPrev = p;
		if(p == NULL)
			return NULL;
		p = p->pNext;
		if(p == NULL)
			return NULL;
		else
		{
			pos = (unsigned long)p;
			return p->pObj;
		}
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ���һ���ڵ�Ķ���ָ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 15:45
	// ����˵��:  void
	// �� �� ֵ:  ����ָ��
	//////////////////////////////////////////////////////////////////////////
	type* getTail()
	{
		if(m_pTail == NULL)
			return NULL;
		return m_pTail->pObj;
	}

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����ǰ�б�ת��Ϊ��������,������typeΪ�򵥽ṹ���������ڴ�ָ����麯������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-5-19 19:26
	// ����˵��:  @obj_array��ʾ����������ָ�룬�ɵ����߿��ٿռ䣬��ȷ�����Դ��iMaxRows����¼
	//         :  @iSize���ζ�ȡ�����������÷���
	//         :  @iMaxRows��ʾ��ǰ������������¼��
	//         :  @pos��ʾ���÷��ص�λ����Ϣ��Ϊ�´�toNextArrayʱʹ�ã�����ʹ��ʱΪ0
	// �� �� ֵ:  =0��ʾû��δȡ��¼��>0��ʾ����δȡ��¼
	//////////////////////////////////////////////////////////////////////////
	int toNextArray(type* obj_array,int &iSize,int iMaxRows,unsigned long &pos)
	{
		NODE* p = (NODE*)pos;
		iSize = 0;
		if(p == NULL)
			p = m_pNode;
		if(p == NULL)
			return 0;
		while(p != NULL && iSize < iMaxRows)
		{
			memcpy((unsigned char*)&obj_array[iSize++],(unsigned char*)p->pObj,sizeof(type));
			pos = (unsigned long)p->pNext;
			p = p->pNext;
		}
		if(p == NULL)
			return 0;
		return 1;
	}

	NODE *m_pNode,*m_pTail,*m_pFetchPrev;
private:

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��̨�ͷŽڵ���̣߳����ڵ�����̫��ʱ����ʹ���߳��ͷ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 14:32
	// ����˵��:  this
	// �� �� ֵ:  NULL
	//////////////////////////////////////////////////////////////////////////
	static void* ThreadFree(void *lp)
	{
	#ifndef WIN32
		pthread_detach(pthread_self());
	#endif
		FREENODE *pNode = (FREENODE*)lp;
		if (pNode->iDelayUS > 0)
		{
#ifdef WIN32
			int ms = pNode->iDelayUS / 1000;
			if (ms <= 0)
				ms = 1;
			Sleep(ms);
#else
			usleep(pNode->iDelayUS);
#endif
		}
		NODE *next, *p = pNode->p;
#ifdef WIN32
		try
		{
#endif
			while(p)
			{
				next = p->pNext;
				if(pNode->bAutoDel)
					delete p->pObj;
				delete p;
				p = next;
			}
#ifdef WIN32
		}
		catch(...)
		{
			ASSERT(0);
		}
		try
		{
#endif
			delete pNode;
#ifdef WIN32
		}
		catch(...)
		{
			ASSERT(0);
		}
#endif
		return NULL;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ӽڵ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 15:46
	// ����˵��:  @pNew�½ڵ�
	// �� �� ֵ:  true
	//////////////////////////////////////////////////////////////////////////
	bool addNode(NODE* pNew)
	{
		pNew->pNext = NULL;
		if(!m_pNode)
			m_pNode = pNew;
		else
		{
			NODE *p=m_pNode;
			while(p->pNext)
				p = p->pNext;
			p->pNext = pNew;
		}
		return true;
	}


	int m_iSize;
	bool m_bAutoDel;
	//bool m_bShared;//�Ƿ��̹߳����繲����Ҫ�ź�������
	SLock *m_pLock;

};


//////////////////////////////////////////////////////////////////////////
// ��    ��:  SConstPtrList
// ��    ��:  �ۿ���
// ����ʱ��:  2015-12-21 21:24
// ��    ��:  ���ڳ����ڴ�����ָ�����ģ����
//////////////////////////////////////////////////////////////////////////
template <class type>
class SConstPtrList
{
public:
	struct NODE
	{
		type *pObj;
		NODE* pNext;
	};
	struct FREENODE
	{
		NODE* p;
		int iDelayUS;//��ʾ��ʱ΢����
		bool bAutoDel;
	};
	SConstPtrList()
	{
		SKTOBJECT("SConstPtrList");
		m_pNode = NULL;
		m_iSize  = 0;
		m_bAutoDel = false;
		m_pNode = NULL;
		m_pTail = NULL;
		m_pLock = NULL;
	};
	virtual ~SConstPtrList()
	{
		clear();
		if(m_pLock != NULL)
		{
			delete m_pLock;
			m_pLock = NULL;
		}
	};

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����Զ��ͷ�ģʽ���Ƿ���ɾ���ڵ�ʱ�ͷŽڵ��Ӧ�Ķ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 14:27
	// ����˵��:  @b�Ƿ��ͷŶ���
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void setAutoDelete(bool b)
	{
		m_bAutoDel = b;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �Ƿ�Ϊ�̹߳���ģʽ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 14:29
	// ����˵��:  void
	// �� �� ֵ:  true��ʾ���̹߳���ģʽ��false��ʾ���ǹ���ģʽ
	//////////////////////////////////////////////////////////////////////////
	bool isShared() {return m_pLock != NULL;};

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����̹߳���ģʽ�������߳̿��ܲ�����дʱ��Ҫ��Ϊtrue
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 14:29
	// ����˵��:  @b�Ƿ��̹߳���ģʽ
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void setShared(bool b) 
	{
		if(b)
		{
			if(m_pLock == NULL)
				m_pLock = new SLock();
		}
		else
		{
			if(m_pLock != NULL)
			{
				delete m_pLock;
				m_pLock = NULL;
			}
		}
	};

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���м���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 14:30
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	inline void lock()
	{
		if(m_pLock != NULL)
			m_pLock->lock();
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���н���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 14:30
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	inline void unlock()
	{
		if(m_pLock != NULL)
			m_pLock->unlock();
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ɾ��ȫ���ڵ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 14:34
	// ����˵��:  @bNeedLock��ʾ�Ƿ���Ҫ����(��������ģʽʱ��Ч)��true��ʾ��Ҫ������false��ʾ����Ҫ�����ⲿ�Ѿ���������
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void clear(bool bNeedLock=true)
	{
		if(bNeedLock)
			lock();
		NODE *next,*p=m_pNode;
		m_pNode = NULL;
		m_iSize  = 0;
		if(m_bAutoDel)
		{
			while(p)
			{
				next = p->pNext;
				//if(m_bAutoDel)
					delete p->pObj;
				//delete p;
				p = next;
			}
		}
		if(bNeedLock)
			unlock();
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ɾ��ָ��ָ��Ķ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 14:34
	// ����˵��:  @pObjΪ��ɾ���Ķ���ָ��
	//         :  @bNeedLock��ʾ�Ƿ���Ҫ����(��������ģʽʱ��Ч)��true��ʾ��Ҫ������false��ʾ����Ҫ�����ⲿ�Ѿ���������
	// �� �� ֵ:  true��ʾɾ���ɹ���falseʧ��
	//////////////////////////////////////////////////////////////////////////
	bool remove(type *pObj,bool bNeedLock=true)
	{
		if(bNeedLock)
			lock();
		NODE *p=m_pNode,*prev=NULL;
		while(p)
		{
			if(p->pObj == pObj)
			{
				if(prev == NULL)//first?
				{
					m_pNode = p->pNext;
					if(p == m_pTail)
						m_pTail = m_pNode;
				}
				else
				{
					prev->pNext = p->pNext;
					if(p == m_pTail)
						m_pTail = prev;
				}
				m_iSize --;
				if(m_bAutoDel)
					delete p->pObj;
				//delete p;
				if(bNeedLock)
					unlock();
				return true;
			}
			prev = p;
			p = p->pNext;
		}
		if(bNeedLock)
			unlock();
		return false;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ָ���Ķ����Ƿ����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 14:37
	// ����˵��:  @pObjΪ�����ָ��
	// �� �� ֵ:  true��ʾ�˶�����ڣ�false��ʾ������
	//////////////////////////////////////////////////////////////////////////
	bool exist(type *pObj)
	{
		return position(pObj) >= 0;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡָ�������ڶ����е�λ��,��0��ʼ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 14:44
	// ����˵��:  @pObjΪ�����ָ��
	//         :  @bNeedLock��ʾ�Ƿ���Ҫ����(��������ģʽʱ��Ч)��true��ʾ��Ҫ������false��ʾ����Ҫ�����ⲿ�Ѿ���������
	// �� �� ֵ:  >0Ϊ����λ�ã�-1��ʾ���󲻴���
	//////////////////////////////////////////////////////////////////////////
	int position(type *pObj,bool bNeedLock=true)
	{
		if(bNeedLock)
			lock();
		NODE *p=m_pNode/*,*prev=NULL*/;
		int i=-1;
		while(p)
		{
			i++;
			if(p->pObj == pObj)
			{
				if(bNeedLock)
					unlock();
				return i;
			}
			//prev = p;
			p = p->pNext;
		}
		if(bNeedLock)
			unlock();
		return -1;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �滻ָ��λ�õĶ���Ϊһ���µĶ���ָ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 14:48
	// ����˵��:  @i��ʾ�����е�λ�ã���0��ʼ���
	//            @pObj��ʾ�µĶ���ָ��
	//            @bRemoveOld��ʾ�Ƿ��ͷŽ����滻�Ķ��󣬽����Զ��ͷ�ģʽʱ��Ч
	// �� �� ֵ:  true��ʾ�滻�ɹ���false��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	bool replace(int i, type*pObj,bool bRemoveOld=false)
	{
		lock();
		NODE *p=m_pNode,*prev=NULL;
		int cnt=0;
		while(p)
		{
			if(i == cnt)
			{
				if(m_bAutoDel && bRemoveOld && p->pObj != NULL)
				{
					delete p->pObj;
				}
				p->pObj = pObj;
				unlock();
				return true;
			}
			prev = p;
			p = p->pNext;
			cnt ++;
		}
		unlock();
		return false;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ɾ��ָ��λ��i�Ľڵ㣬i��0��ʼ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 14:50
	// ����˵��:  @i��ɾ���Ľڵ�λ�ã���0��ʼ
	//         :  @bNeedLock��ʾ�Ƿ���Ҫ����(��������ģʽʱ��Ч)��true��ʾ��Ҫ������false��ʾ����Ҫ�����ⲿ�Ѿ���������
	// �� �� ֵ:  true��ʾɾ���ɹ���false��ʾɾ��ʧ��
	//////////////////////////////////////////////////////////////////////////
	bool remove(int i,bool bNeedLock=true)
	{
		if(bNeedLock)
			lock();
		NODE *p=m_pNode,*prev=NULL;
		int cnt=0;
		while(p)
		{
			if(i == cnt)
			{
				if(prev == NULL)//first?
				{
					m_pNode = p->pNext;
					if(p == m_pTail)
						m_pTail = m_pNode;
				}
				else
				{
					prev->pNext = p->pNext;
					if(p == m_pTail)
						m_pTail = prev;
				}
				m_iSize --;
				if(m_bAutoDel)
					delete p->pObj;
				//delete p;
				if(bNeedLock)
					unlock();
				return true;
			}
			prev = p;
			p = p->pNext;
			cnt ++;
		}
		if(bNeedLock)
			unlock();
		return false;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ɾ��ȫ���ڵ㣬���Ƽ�ʹ�ã�����ʹ��clear����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 15:04
	// ����˵��:  void
	// �� �� ֵ:  true
	//////////////////////////////////////////////////////////////////////////
	bool removeAll()
	{
		while(count()>0)
			remove((int)0);
		return true;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ���е��ܽڵ�����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 15:05
	// ����˵��:  void
	// �� �� ֵ:  �ڵ�����
	//////////////////////////////////////////////////////////////////////////
	inline int count()
	{
		return m_iSize;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����Ƿ�Ϊ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 15:06
	// ����˵��:  void
	// �� �� ֵ:  true��ʾ����Ϊ�գ�false��ʾ���зǿ�
	//////////////////////////////////////////////////////////////////////////
	inline bool isEmpty()
	{
		return m_iSize == 0;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����ĩβ���һ���µĽڵ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 15:06
	// ����˵��:  @pObjΪ�½ڵ��Ӧ�Ķ���
	//         :  @bNeedLock��ʾ�Ƿ���Ҫ����(��������ģʽʱ��Ч)��true��ʾ��Ҫ������false��ʾ����Ҫ�����ⲿ�Ѿ���������
	// �� �� ֵ:  true
	//////////////////////////////////////////////////////////////////////////
	bool append(SConstBuffer &cs,type *pObj,bool bNeedLock=true)
	{
		if(bNeedLock)
			lock();
		NODE *p=m_pNode;
		static NODE node;
		NODE *pNew = (NODE*)cs.GetConstBuffer(sizeof(node));//new NODE;
		pNew->pObj = pObj;
		pNew->pNext = NULL;
		if(p == NULL)
		{
			m_pTail = m_pNode = pNew;
		}
		else if(m_pTail != NULL)
		{
			//���ٲ��뵽β�ڵ�
			pNew->pNext = m_pTail->pNext;
			m_pTail->pNext = pNew;
			m_pTail = pNew;
		}
		else
		{
			while(p->pNext)
				p = p->pNext;
			p->pNext = pNew;
			m_pTail = pNew;
		}
		m_iSize ++;
		if(bNeedLock)
			unlock();
		return true;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ָ���ڵ�ǰ�����½ڵ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2009-2-16 9:16
	// ����˵��:  pNodeΪ�ο��ڵ�NULL��ʾ�ӵ����pObjΪ�½ڵ�
	//         :  @bNeedLock��ʾ�Ƿ���Ҫ����(��������ģʽʱ��Ч)��true��ʾ��Ҫ������false��ʾ����Ҫ�����ⲿ�Ѿ���������
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool insertBefore(SConstBuffer &cs,type* pNode,type *pObj,bool bNeedLock=true)
	{
		if(bNeedLock)
			lock();
		NODE *p=m_pNode,*prev=NULL;
		static NODE node;
		NODE *pNew = (NODE*)cs.GetConstBuffer(sizeof(node));//new NODE;
		pNew->pObj = pObj;
		if (p == NULL)
		{
			pNew->pNext = NULL;
			m_iSize++;
			m_pTail = m_pNode = pNew;
		}
		else
		{
			if(pNode == NULL)
			{
				pNew->pNext = NULL;
				m_pTail->pNext = pNew;
				m_pTail = pNew;
				m_iSize++;
				if(bNeedLock)
					unlock();
				return true;
			}
			while(p != NULL)
			{
				if(p->pObj == pNode)
				{
					pNew->pNext = p;
					if(prev == NULL)//first?
					{
						if(m_pNode == NULL)
							m_pTail = pNew;
						m_pNode = pNew;
					}
					else
					{
						prev->pNext = pNew;
					}
					m_iSize ++;
					if(bNeedLock)
						unlock();
					return true;
				}
				prev = p;
				p = p->pNext;
			}
		}
		if(bNeedLock)
			unlock();
		return false;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ָ���ڵ������½ڵ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2009-2-16 9:16
	// ����˵��:  pNodeΪ�ο��ڵ�NULL��ʾ�ӵ����pObjΪ�½ڵ�
	//         :  @bNeedLock��ʾ�Ƿ���Ҫ����(��������ģʽʱ��Ч)��true��ʾ��Ҫ������false��ʾ����Ҫ�����ⲿ�Ѿ���������
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool insertAfter(SConstBuffer &cs,type* pNode,type *pObj,bool bNeedLock=true)
	{
		if(bNeedLock)
			lock();
		NODE *p=m_pNode;
		static NODE node;
		NODE *pNew = (NODE*)cs.GetConstBuffer(sizeof(node));//new NODE;
		pNew->pObj = pObj;
		if (p == NULL)
		{
			pNew->pNext = NULL;
			m_iSize++;
			m_pTail = m_pNode = pNew;
		}
		else
		{
			if(pNode == NULL)
			{
				pNew->pNext = NULL;
				m_pTail->pNext = pNew;
				m_pTail = pNew;
				m_iSize++;
				if(bNeedLock)
					unlock();
				return true;
			}
			while (p != NULL/* && p->pNext != NULL*/)
			{
				if (p->pObj == pNode)
				{
					pNew->pNext = p->pNext;
					p->pNext = pNew;
					if (p == m_pTail)
						m_pTail = pNew;
					m_iSize++;
					if(bNeedLock)
						unlock();
					return true;
				}
				p = p->pNext;
			}
		}
		if(bNeedLock)
			unlock();
		return false;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡָ��λ�õĽڵ����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 15:13
	// ����˵��:  @i��ʾ��ȡ�����ڶ����е�λ�ã�i��0��ʼ���
	//         :  @bNeedLock��ʾ�Ƿ���Ҫ����(��������ģʽʱ��Ч)��true��ʾ��Ҫ������false��ʾ����Ҫ�����ⲿ�Ѿ���������
	// �� �� ֵ:  ����ָ�룬NULL��ʾδ�ҵ�
	//////////////////////////////////////////////////////////////////////////
	type * at(int i,bool bNeedLock=true)
	{
		if(bNeedLock)
			lock();
		NODE *p=m_pNode/*,*prev=NULL*/;
		int cnt=0;
		while(p)
		{
			if(i == cnt)
			{
				if(bNeedLock)
					unlock();
				return p->pObj;
			}
			//prev = p;
			p = p->pNext;
			cnt ++;
		}
		if(bNeedLock)
			unlock();
		return NULL;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡָ��λ�õĽڵ����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 15:13
	// ����˵��:  @i��ʾ��ȡ�����ڶ����е�λ�ã�i��0��ʼ���
	// �� �� ֵ:  ����ָ�룬NULL��ʾδ�ҵ�
	//////////////////////////////////////////////////////////////////////////
	type * operator[](int i)
	{
		return at(i);
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �ƶ����ڵ�Ŀ�ʼλ�ã��˺�����FetchNextһ��ʹ�ã�
	//            ������̲߳���ȫ�ģ���Ҫ�ڱ���ǰ��ӽ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 15:14
	// ����˵��:  @pos��ʾ���÷��ص�λ����Ϣ��Ϊ�´�Fetchʱʹ��
	// �� �� ֵ:  ����ָ�룬NULL��ʾδ�ҵ�
	//////////////////////////////////////////////////////////////////////////
	type* FetchFirst(unsigned long &pos)
	{
		if(m_pNode == NULL)
			return NULL;
		else
		{
			pos = (unsigned long)m_pNode;
			return m_pNode->pObj;
		}
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �ƶ����ڵ��ָ��λ�ã�id��0��ʼ���˺�����FetchNextһ��ʹ�ã�
	//            ������̲߳���ȫ�ģ���Ҫ�ڱ���ǰ��ӽ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 15:14
	// ����˵��:  @idΪ��Ҫ�ƶ�����λ�ã�id��0��ʼ
	//            @pos��ʾ���÷��ص�λ����Ϣ��Ϊ�´�Fetchʱʹ��
	// �� �� ֵ:  ����ָ�룬NULL��ʾδ�ҵ�
	//////////////////////////////////////////////////////////////////////////
	type* FetchToIndex(int id,unsigned long &pos)
	{
		int cnt = 0;
		type* p = FetchFirst(pos);
		while(p != NULL && cnt < id)
		{
			p = FetchNext(pos);
			cnt++;
		}
		return p;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �ƶ��´�һ���ڵ�λ�ã��˺�����FetchFirst��һ��ʹ�ã�
	//            ������̲߳���ȫ�ģ���Ҫ�ڱ���ǰ��ӽ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 15:14
	// ����˵��:  @idΪ��Ҫ�ƶ�����λ�ã�id��0��ʼ
	//            @pos��ʾ���÷��ص�λ����Ϣ��Ϊ�´�Fetchʱʹ��
	// �� �� ֵ:  ����ָ�룬NULL��ʾ����ײ�����ʱposֵ�����0���´����¼�¼�����
	//            �ٴ�ʹ��FetchNext(����pos����)����Ȼ�ܵõ���һ����¼
	//////////////////////////////////////////////////////////////////////////
	type* FetchNext(unsigned long &pos)
	{
		NODE* p = (NODE*)pos;
		if(p == NULL)
			return NULL;
		p = p->pNext;
		if(p == NULL)
			return NULL;
		else
		{
			pos = (unsigned long)p;
			return p->pObj;
		}
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ���һ���ڵ�Ķ���ָ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 15:45
	// ����˵��:  void
	// �� �� ֵ:  ����ָ��
	//////////////////////////////////////////////////////////////////////////
	type* getTail()
	{
		if(m_pTail == NULL)
			return NULL;
		return m_pTail->pObj;
	}

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����ǰ�б�ת��Ϊ��������,������typeΪ�򵥽ṹ���������ڴ�ָ����麯������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-5-19 19:26
	// ����˵��:  @obj_array��ʾ����������ָ�룬�ɵ����߿��ٿռ䣬��ȷ�����Դ��iMaxRows����¼
	//         :  @iSize���ζ�ȡ�����������÷���
	//         :  @iMaxRows��ʾ��ǰ������������¼��
	//         :  @pos��ʾ���÷��ص�λ����Ϣ��Ϊ�´�toNextArrayʱʹ�ã�����ʹ��ʱΪ0
	// �� �� ֵ:  =0��ʾû��δȡ��¼��>0��ʾ����δȡ��¼
	//////////////////////////////////////////////////////////////////////////
	int toNextArray(type* obj_array,int &iSize,int iMaxRows,unsigned long &pos)
	{
		NODE* p = (NODE*)pos;
		iSize = 0;
		if(p == NULL)
			p = m_pNode;
		if(p == NULL)
			return 0;
		while(p != NULL && iSize < iMaxRows)
		{
			memcpy((unsigned char*)&obj_array[iSize++],(unsigned char*)p->pObj,sizeof(type));
			pos = (unsigned long)p->pNext;
			p = p->pNext;
		}
		if(p == NULL)
			return 0;
		return 1;
	}

	NODE *m_pNode,*m_pTail;
private:


	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ӽڵ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 15:46
	// ����˵��:  @pNew�½ڵ�
	// �� �� ֵ:  true
	//////////////////////////////////////////////////////////////////////////
	bool addNode(NODE* pNew)
	{
		pNew->pNext = NULL;
		if(!m_pNode)
			m_pNode = pNew;
		else
		{
			NODE *p=m_pNode;
			while(p->pNext)
				p = p->pNext;
			p->pNext = pNew;
		}
		return true;
	}


	int m_iSize;
	bool m_bAutoDel;
	//bool m_bShared;//�Ƿ��̹߳����繲����Ҫ�ź�������
	SLock *m_pLock;

};

//////////////////////////////////////////////////////////////////////////
// ��    ��:  SValueList
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 15:57
// ��    ��:  ��ֵ̬ģ���а���
//////////////////////////////////////////////////////////////////////////
template <typename T>
class SValueList
{
public:
	typedef T* iterator;
	struct iterator2
	{
		void *pObj;
	};
	struct NODE
	{
		T obj;
		NODE* pNext;
	};
	SValueList()
	{
		SKTOBJECT("SValueList");
		m_pNode = NULL;
		m_iSize  = 0;
	};
	virtual ~SValueList()
	{
		clear();
	};

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 16:01
	// ����˵��:  void
	// �� �� ֵ:  true
	//////////////////////////////////////////////////////////////////////////
	bool clear()
	{
		NODE *next,*p=m_pNode;
		m_pNode = NULL;
		m_iSize  = 0;
		while(p)
		{
			next = p->pNext;
			delete p;
			p = next;
		}
		return true;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ɾ��ָ���Ľڵ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 16:02
	// ����˵��:  @pObj��ʾ��ɾ���Ķ���ָ��
	// �� �� ֵ:  true��ʾɾ���ɹ���false��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	bool remove(T *pObj)
	{
		NODE *p=m_pNode,*prev=NULL;
		while(p != NULL && p->pNext != NULL)
		{
			if(&p->obj == pObj)
			{
				if(prev == NULL)//first?
					m_pNode = p->pNext;
				else
					prev->pNext = p->pNext;
				m_iSize --;
				delete p;
				return true;
			}
			prev = p;
			p = p->pNext;
		}
		return false;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ɾ����i���ڵ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 16:03
	// ����˵��:  @iΪָ���λ�ã�i��0��ʼ
	// �� �� ֵ:  true��ʾɾ���ɹ���false��ʾɾ��ʧ��
	//////////////////////////////////////////////////////////////////////////
	bool remove(int i)
	{
		NODE *p=m_pNode,*prev=NULL;
		int cnt=0;
		while(p)
		{
			if(i == cnt)
			{
				if(prev == NULL)//first?
					m_pNode = p->pNext;
				else
					prev->pNext = p->pNext;
				m_iSize --;
				delete p;
				return true;
			}
			prev = p;
			p = p->pNext;
			cnt ++;
		}
		return false;
	}
	
	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ɾ��ȫ���ڵ㣬���Ƽ�ʹ�ã�����ʹ��clear�������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 16:04
	// ����˵��:  void
	// �� �� ֵ:  true
	//////////////////////////////////////////////////////////////////////////
	bool removeAll()
	{
		while(count()>0)
			remove((int)0);
		return true;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ���е�����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 16:05
	// ����˵��:  void
	// �� �� ֵ:  ������
	//////////////////////////////////////////////////////////////////////////
	inline unsigned long count()
	{
		return m_iSize;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����Ƿ�Ϊ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 16:05
	// ����˵��:  void
	// �� �� ֵ:  true��ʾΪ�գ�false��ʾ�ǿ�
	//////////////////////////////////////////////////////////////////////////
	inline bool isEmpty()
	{
		return m_iSize == 0;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���һ���µĽڵ㵽ĩβ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 16:07
	// ����˵��:  @obj��ʾ�µĽڵ�ֵ
	// �� �� ֵ:  true
	//////////////////////////////////////////////////////////////////////////
	bool append(T &obj)
	{
		NODE *p=m_pNode;
		NODE *pNew = new NODE;
		memcpy(&pNew->obj, &obj,sizeof(T));
		pNew->pNext = NULL;
		if(p == NULL)
			m_pNode = pNew;
		else
		{
			while(p->pNext)
				p = p->pNext;
			p->pNext = pNew;
		}
		m_iSize ++;
		return true;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ָ���ڵ�ǰ�����½ڵ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2009-2-16 9:16
	// ����˵��:  pNodeΪ�ο��ڵ㣬objΪ�½ڵ�
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool insertBefore(T* pNode,T &obj)
	{
		NODE *p=m_pNode,*prev=NULL;
		NODE *pNew = new NODE;
		memcpy(&pNew->obj, &obj,sizeof(T));
		while(p != NULL && p->pNext != NULL)
		{
			if(&p->obj == pNode)
			{
				if(prev == NULL)//first?
				{
					m_pNode = pNew;
					pNew->pNext = p;
				}
				else
				{
					prev->pNext = pNew;
					pNew->pNext = p;
				}
				m_iSize ++;
				return true;
			}
			prev = p;
			p = p->pNext;
		}
		return false;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ָ���ڵ������½ڵ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2009-2-16 9:16
	// ����˵��:  pNodeΪ�ο��ڵ㣬objΪ�½ڵ�
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool insertAfter(T* pNode,T &obj)
	{
		NODE *p=m_pNode;
		NODE *pNew = new NODE;
		memcpy(&pNew->obj, &obj,sizeof(T));
		while(p != NULL/* && p->pNext != NULL*/)
		{
			if(&p->obj == pNode)
			{
				pNew->pNext = p->pNext;
				p->pNext = pNew;
				m_iSize ++;
				return true;
			}
			p = p->pNext;
		}
		return false;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡָ��λ�õĽڵ�ֵָ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 16:07
	// ����˵��:  @i��ʾָ����λ�ã�i��0��ʼ���
	// �� �� ֵ:  type*ֵָ��
	//////////////////////////////////////////////////////////////////////////
	iterator at(int i)
	{
		NODE *p=m_pNode;
		int cnt=0;
		while(p)
		{
			if(i == cnt)
			{
				return &p->obj;
			}
			p = p->pNext;
			cnt ++;
		}
		return NULL;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡָ��λ�õĽڵ�ֵָ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 16:07
	// ����˵��:  @i��ʾָ����λ�ã�i��0��ʼ���
	// �� �� ֵ:  type*ֵָ��
	//////////////////////////////////////////////////////////////////////////
	iterator operator[](int i)
	{
		return at(i);
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �ƶ����ڵ�Ŀ�ʼλ�ã��˺�����FetchNextһ��ʹ�ã�
	//            ������̲߳���ȫ�ģ���Ҫ�ڱ���ǰ��ӽ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 15:14
	// ����˵��:  @pos��ʾ���÷��ص�λ����Ϣ��Ϊ�´�Fetchʱʹ��
	// �� �� ֵ:  ����ָ�룬NULL��ʾδ�ҵ�
	//////////////////////////////////////////////////////////////////////////
	T* FetchFirst(unsigned long &pos)
	{
		if(m_pNode == NULL)
			return NULL;
		else
		{
			pos = (unsigned long)m_pNode;
			return &m_pNode->obj;
		}
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �ƶ����ڵ��ָ��λ�ã�id��0��ʼ���˺�����FetchNextһ��ʹ�ã�
	//            ������̲߳���ȫ�ģ���Ҫ�ڱ���ǰ��ӽ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 15:14
	// ����˵��:  @idΪ��Ҫ�ƶ�����λ�ã�id��0��ʼ
	//            @pos��ʾ���÷��ص�λ����Ϣ��Ϊ�´�Fetchʱʹ��
	// �� �� ֵ:  ����ָ�룬NULL��ʾδ�ҵ�
	//////////////////////////////////////////////////////////////////////////
	T* FetchToIndex(int id,unsigned long &pos)
	{
		int cnt = 0;
		T* p = FetchFirst(pos);
		while(p != NULL && cnt < id)
		{
			p = FetchNext(pos);
			cnt++;
		}
		return p;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �ƶ��´�һ���ڵ�λ�ã��˺�����FetchFirst��һ��ʹ�ã�
	//            ������̲߳���ȫ�ģ���Ҫ�ڱ���ǰ��ӽ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 15:14
	// ����˵��:  @idΪ��Ҫ�ƶ�����λ�ã�id��0��ʼ
	//            @pos��ʾ���÷��ص�λ����Ϣ��Ϊ�´�Fetchʱʹ��
	// �� �� ֵ:  ����ָ�룬NULL��ʾ����ײ�����ʱposֵ�����0���´����¼�¼�����
	//            �ٴ�ʹ��FetchNext(����pos����)����Ȼ�ܵõ���һ����¼
	//////////////////////////////////////////////////////////////////////////
	T* FetchNext(unsigned long &pos)
	{
		NODE* p = (NODE*)pos;
		if(p == NULL)
			return NULL;
		p = p->pNext;
		if(p == NULL)
			return NULL;
		else
		{
			pos = (unsigned long)p;
			return &p->obj;
		}
	}
private:
	NODE *m_pNode;
	int m_iSize;

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ӽڵ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 16:08
	// ����˵��:  @pNew�½ڵ�
	// �� �� ֵ:  true
	//////////////////////////////////////////////////////////////////////////
	bool addNode(NODE* pNew)
	{
		pNew->pNext = NULL;
		if(!m_pNode)
			m_pNode = pNew;
		else
		{
			NODE *p=m_pNode;
			while(p->pNext)
				p = p->pNext;
			p->pNext = pNew;
		}
		return true;
	}
};

//////////////////////////////////////////////////////////////////////////
// ��    ��:  SClassList
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 15:57
// ��    ��:  ��̬�������ģ����
//////////////////////////////////////////////////////////////////////////
template <class T>
class SClassList
{
public:
	typedef T* iterator;
	struct iterator2
	{
		void *pObj;
	};
	struct NODE
	{
		T obj;
		NODE* pNext;
	};
	SClassList()
	{
		SKTOBJECT("SClassList");
		m_pNode = NULL;
		m_iSize  = 0;
		m_bShared = false;
	};
	virtual ~SClassList()
	{
		clear();
	};

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����̹߳���ģʽ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 16:09
	// ����˵��:  @b��ʾ�Ƿ���Ϊ�̹߳���ģʽ
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void setShared(bool b)
	{
		m_bShared = b;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 16:10
	// ����˵��:  void
	// �� �� ֵ:  true
	//////////////////////////////////////////////////////////////////////////
	bool clear()
	{
		if(m_bShared)
			m_Lock.lock();
		NODE *next,*p=m_pNode;
		m_pNode = NULL;
		m_iSize  = 0;
		while(p)
		{
			next = p->pNext;
			delete p;
			p = next;
		}
		if(m_bShared)
			m_Lock.unlock();
		return true;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ɾ��������ָ����ָ��Ľڵ�ֵ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 16:10
	// ����˵��:  @pObjΪָ��
	// �� �� ֵ:  true��ʾɾ���ɹ���false��ʾɾ��ʧ��
	//////////////////////////////////////////////////////////////////////////
	bool remove(T *pObj)
	{
		if(m_bShared)
			m_Lock.lock();
		NODE *p=m_pNode,*prev=NULL;
		if(p == NULL)
		{
			if(m_bShared)
				m_Lock.unlock();
			return false;
		}
		if(p->pNext == NULL)
		{
			m_pNode = NULL;
			m_iSize --;
			delete p;
			if(m_bShared)
				m_Lock.unlock();
			return true;
		}
		while(p->pNext)
		{
			if(&(p->obj) == pObj)
			{
				if(prev == NULL)//first?
					m_pNode = p->pNext;
				else
					prev->pNext = p->pNext;
				m_iSize --;
				delete p;
				if(m_bShared)
					m_Lock.unlock();
				return true;
			}
			prev = p;
			p = p->pNext;
		}
		if(m_bShared)
			m_Lock.unlock();
		return false;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ɾ��ָ��λ�õĽڵ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 16:11
	// ����˵��:  @iΪ��ɾ����λ��,i��0��ʼ���
	// �� �� ֵ:  true��ʾɾ���ɹ���falseʧ��
	//////////////////////////////////////////////////////////////////////////
	bool remove(int i)
	{
		if(m_bShared)
			m_Lock.lock();
		NODE *p=m_pNode,*prev=NULL;
		int cnt=0;
		while(p)
		{
			if(i == cnt)
			{
				if(prev == NULL)//first?
					m_pNode = p->pNext;
				else
					prev->pNext = p->pNext;
				m_iSize --;
				delete p;
				if(m_bShared)
					m_Lock.unlock();
				return true;
			}
			prev = p;
			p = p->pNext;
			cnt ++;
		}
		if(m_bShared)
			m_Lock.unlock();
		return false;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 16:12
	// ����˵��:  void
	// �� �� ֵ:  ����
	//////////////////////////////////////////////////////////////////////////
	inline unsigned long count()
	{
		return m_iSize;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����Ƿ�Ϊ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 16:12
	// ����˵��:  void
	// �� �� ֵ:  true����Ϊ�գ�falseΪ�ǿ�
	//////////////////////////////////////////////////////////////////////////
	bool isEmpty()
	{
		return m_iSize == 0;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �Զ���ĩβ���һ���µĽڵ�ֵ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 16:13
	// ����˵��:  @objΪ�µĽڵ�ֵ
	// �� �� ֵ:  true
	//////////////////////////////////////////////////////////////////////////
	bool append(const T &obj)
	{
		if(m_bShared)
			m_Lock.lock();
		NODE *p=m_pNode;
		NODE *pNew = new NODE;
		pNew->obj = obj;
		//memcpy(&pNew->obj, &obj,sizeof(T));
		pNew->pNext = NULL;
		if(p == NULL)
			m_pNode = pNew;
		else
		{
			while(p->pNext)
				p = p->pNext;
			p->pNext = pNew;
		}
		m_iSize ++;
		if(m_bShared)
			m_Lock.unlock();
		return true;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��i���ڵ��ֵָ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 16:13
	// ����˵��:  @i��ʾָ����λ�ã�i��0��ʼ���
	// �� �� ֵ:  type*ֵָ��,NULL��ʾδ�ҵ�
	//////////////////////////////////////////////////////////////////////////
	iterator at(int i)
	{
		if(m_bShared)
			m_Lock.lock();
		NODE *p=m_pNode;
		int cnt=0;
		while(p)
		{
			if(i == cnt)
			{
				if(m_bShared)
					m_Lock.unlock();
				return &p->obj;
			}
			p = p->pNext;
			cnt ++;
		}
		if(m_bShared)
			m_Lock.unlock();
		return NULL;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��i���ڵ��ֵָ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 16:13
	// ����˵��:  @i��ʾָ����λ�ã�i��0��ʼ���
	// �� �� ֵ:  type*ֵָ��,NULL��ʾδ�ҵ�
	//////////////////////////////////////////////////////////////////////////
	iterator operator[](int i)
	{
		return at(i);
	}

private:
	NODE *m_pNode;
	int m_iSize;
	bool m_bShared;//�Ƿ��̹߳����繲����Ҫ�ź�������
	SLock m_Lock;
};
//#define QPtrList SPtrList
//#define QValueList SValueList
//typedef SPtrList QPtrList;
//typedef SValueList QValueList;


//////////////////////////////////////////////////////////////////////////
// ��    ��:  SArray
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 15:58
// ��    ��:  ����ģ����
//////////////////////////////////////////////////////////////////////////
template <class type>
class SArray
{
public:
	SArray()
	{
		SKTOBJECT("SArray");
		m_pObjs = NULL;
		m_iSize  = 0;
		m_bShared = false;
	};
	virtual ~SArray()
	{
		clear();
	};

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �Ƿ�Ϊ�̹߳���ģʽ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 16:15
	// ����˵��:  void
	// �� �� ֵ:  trueΪ�̹߳���ģʽ��false����
	//////////////////////////////////////////////////////////////////////////
	bool isShared() {return m_bShared;};

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����̹߳���ģʽ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 16:16
	// ����˵��:  @bΪ�̹߳���ģʽ
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void setShared(bool b) {m_bShared=b;};

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 16:16
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	inline void lock()
	{
		if(m_bShared)
			m_Lock.lock();
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 16:16
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	inline void unlock()
	{
		if(m_bShared)
			m_Lock.unlock();
	}
	
	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �������ȫ���ڵ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 16:17
	// ����˵��:  void
	// �� �� ֵ:  true
	//////////////////////////////////////////////////////////////////////////
	bool clear()
	{
		lock();
		if(m_pObjs != NULL)
		{
			delete[] m_pObjs;
			m_pObjs = NULL;
			m_iSize = 0;
		}
		unlock();
		return true;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���������С
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 16:17
	// ����˵��:  @iSizeΪ�µ������С
	//            @bKeepOldData��ʾ�Ƿ���������
	// �� �� ֵ:  �ɹ�����true��ʧ�ܷ���false
	//////////////////////////////////////////////////////////////////////////
	bool setSize(int iSize, bool bKeepOldData=true)
	{
		if(iSize<=0)
		{
			clear();
			return true;
		}
		if (iSize == m_iSize)
			return true;
		type *pOld = m_pObjs;
		m_pObjs = new type[iSize];
		memset(m_pObjs,0,sizeof(type)*iSize);
		if(pOld != NULL)
		{
			if (bKeepOldData)
				memcpy(m_pObjs,pOld,(m_iSize>iSize?iSize:m_iSize)*sizeof(type));
			delete[] pOld;
		}
		m_iSize = iSize;
		return true;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ʹ�ô��ڵ�����Ϊ���ݣ����ԭ�е����ݣ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-12-29 18:56
	// ����˵��:  @pObjsΪ�����׵�ַ������ɹ�����SArray�������ͷ�
	//            @iSizeΪ��������
	// �� �� ֵ:  ��������
	//////////////////////////////////////////////////////////////////////////
	bool setArray(type* pObjs, int iSize)
	{
		clear();
		if (pObjs == NULL || iSize <= 0)
		{
			m_pObjs = NULL;
			m_iSize = 0;
			return false;
		}
		m_pObjs = pObjs;
		m_iSize = iSize;
		return true;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ʾ��������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 16:18
	// ����˵��:  void
	// �� �� ֵ:  ��������
	//////////////////////////////////////////////////////////////////////////
	inline int count()
	{
		return m_iSize;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����Ƿ�Ϊ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 16:18
	// ����˵��:  void
	// �� �� ֵ:  true��ʾ����Ϊ�գ�false��ʾ�ǿ�
	//////////////////////////////////////////////////////////////////////////
	bool isEmpty()
	{
		return m_iSize == 0;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���һ���µ�Ԫ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 16:26
	// ����˵��:  @obj�µ�����Ԫ��
	// �� �� ֵ:  ��������������
	//////////////////////////////////////////////////////////////////////////
	int append(type &obj)
	{
		int id = count()+1;
		setSize(id);
		id--;
		setAt(id,obj);
		return id;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���õ�i������Ԫ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 16:27
	// ����˵��:  @iΪ����λ�ã���0��ʼ���
	//            @obj��ʾ��Ԫ��
	// �� �� ֵ:  true��ʾ���óɹ���false��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	bool setAt(int i,type &obj)
	{
		lock();
		if(i>=m_iSize || i<0)
		{
			unlock();
			return false;
		}
		memcpy(&(m_pObjs[i]), &obj,sizeof(type));
		unlock();
		return true;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��i������Ԫ��ָ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 16:28
	// ����˵��:  @iΪ����λ��
	// �� �� ֵ:  ָ��Ԫ�ص�ָ�룬NULL��ʾδ�ҵ�
	//////////////////////////////////////////////////////////////////////////
	type* getAt(int i)
	{
		lock();
		if(i>=m_iSize || i<0)
			return NULL;
		type *p=&(m_pObjs[i]);
		unlock();
		return p;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ������Ԫ�ص�ָ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 16:29
	// ����˵��:  void
	// �� �� ֵ:  ��Ԫ��ָ��
	//////////////////////////////////////////////////////////////////////////
	type * getData()
	{
		return m_pObjs;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��i������Ԫ��ָ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 16:28
	// ����˵��:  @iΪ����λ��
	// �� �� ֵ:  ָ��Ԫ�ص�ָ�룬NULL��ʾδ�ҵ�
	//////////////////////////////////////////////////////////////////////////
	type * at(int i)
	{
		return getAt(i);
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��i������Ԫ��ָ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 16:28
	// ����˵��:  @iΪ����λ��
	// �� �� ֵ:  ָ��Ԫ�ص�ָ�룬NULL��ʾδ�ҵ�
	//////////////////////////////////////////////////////////////////////////
	type * operator[](int i)
	{
		return at(i);
	}

private:
	type *m_pObjs;
	int m_iSize;
	bool m_bShared;//�Ƿ��̹߳����繲����Ҫ�ź�������
	SLock m_Lock;
};

//////////////////////////////////////////////////////////////////////////
// ��    ��:  SPtrArray
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 15:58
// ��    ��:  ָ������ģ����
//////////////////////////////////////////////////////////////////////////
template <class type>
class SPtrArray
{
public:
	SPtrArray()
	{
		SKTOBJECT("SPtrArray");
		m_pObjs = NULL;
		m_iSize  = 0;
		m_bAutoDel = false;
		m_bShared = false;
	};
	virtual ~SPtrArray()
	{
		clear();
	};

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����Ƿ��Զ��ͷ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 16:33
	// ����˵��:  @b�Ƿ��Զ��ͷţ�trueΪ�Զ��ͷţ�false���Զ��ͷ�
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void setAutoDelete(bool b)
	{
		m_bAutoDel = b;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �Ƿ��̹߳���ģʽ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 16:35
	// ����˵��:  void
	// �� �� ֵ:  true��ʾ���̹߳���ģʽ��falseΪ�ǹ���ģʽ
	//////////////////////////////////////////////////////////////////////////
	bool isShared() {return m_bShared;};

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���ù���ģʽ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 16:35
	// ����˵��:  @b��ʾ�����õĹ���ģʽ��true����false������
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void setShared(bool b) {m_bShared=b;};

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 16:36
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	inline void lock()
	{
		if(m_bShared)
			m_Lock.lock();
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 16:36
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	inline void unlock()
	{
		if(m_bShared)
			m_Lock.unlock();
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �������ȫ������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 16:37
	// ����˵��:  void
	// �� �� ֵ:  true
	//////////////////////////////////////////////////////////////////////////
	bool clear()
	{
		lock();
		if(m_pObjs != NULL)
		{
			if(m_bAutoDel)
			{
				for(int i=0;i<m_iSize;i++)
				{
					type * pObj = m_pObjs[i];
					if(pObj != NULL)
						delete pObj;
				}
			}
			delete[] m_pObjs;
			m_iSize = 0;
			m_pObjs = NULL;
		}
		unlock();
		return true;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���������С
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 16:37
	// ����˵��:  @iSize��ʾ�����С
	// �� �� ֵ:  true��ʾ����ɹ���false��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	bool setSize(int iSize)
	{
		if(iSize<=0)
		{
			clear();
			return true;
		}
		lock();
		type **pOld = m_pObjs;
		char *pBuf = new char[sizeof(type*)*iSize];
		m_pObjs = (type**)pBuf;
		memset(m_pObjs,0,sizeof(type*)*iSize);
		if(pOld != NULL)
		{
			memcpy(m_pObjs,pOld,m_iSize>iSize?iSize:m_iSize);
			delete[] pOld;
		}
		m_iSize = iSize;
		unlock();
		return true;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ����Ԫ������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 16:37
	// ����˵��:  void
	// �� �� ֵ:  ����
	//////////////////////////////////////////////////////////////////////////
	inline int count()
	{
		return m_iSize;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ǰ�����Ƿ�Ϊ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 16:38
	// ����˵��:  void
	// �� �� ֵ:  true��ʾ����Ϊ�գ�false��ʾ����ǿ�
	//////////////////////////////////////////////////////////////////////////
	inline bool isEmpty()
	{
		return m_iSize == 0;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����ָ��λ�õ�ָ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 16:38
	// ����˵��:  @iΪָ����λ��
	//            @pObjΪ�µ�ָ��
	// �� �� ֵ:  true��ʾ���óɹ���false��ʾ����ʧ��
	//////////////////////////////////////////////////////////////////////////
	bool setAt(int i,type *pObj)
	{
		lock();
		if(i>=m_iSize || i<0)
		{
			unlock();
			return false;
		}
		m_pObjs[i] = pObj;
		unlock();
		return true;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��i��λ�õ�����Ԫ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 16:39
	// ����˵��:  @i��ȡԪ�ص�λ�ã�i��0��ʼ
	// �� �� ֵ:  ��Ӧλ�õ�Ԫ��ָ�룬NULL��ʾδ�ҵ�
	//////////////////////////////////////////////////////////////////////////
	type* getAt(int i)
	{
		lock();
		if(i>=m_iSize || i<0)
		{
			unlock();
			return NULL;
		}
		type *p=m_pObjs[i];
		unlock();
		return p;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ɾ����i��Ԫ��,i��0��ʼ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 16:40
	// ����˵��:  @i��ɾ��Ԫ�ص�λ�ã�i��0��ʼ
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void remove(int i)
	{
		lock();
		if(i>=m_iSize || i<0)
		{
			unlock();
			return;
		}
		type *p=m_pObjs[i];
		unlock();
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��i��λ�õ�����Ԫ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 16:39
	// ����˵��:  @i��ȡԪ�ص�λ�ã�i��0��ʼ
	// �� �� ֵ:  ��Ӧλ�õ�Ԫ��ָ�룬NULL��ʾδ�ҵ�
	//////////////////////////////////////////////////////////////////////////
	type * at(int i)
	{
		return getAt(i);
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��i��λ�õ�����Ԫ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 16:39
	// ����˵��:  @i��ȡԪ�ص�λ�ã�i��0��ʼ
	// �� �� ֵ:  ��Ӧλ�õ�Ԫ��ָ�룬NULL��ʾδ�ҵ�
	//////////////////////////////////////////////////////////////////////////
	type * operator[](int i)
	{
		return at(i);
	}

private:
	type **m_pObjs;
	int m_iSize;
	bool m_bAutoDel;
	bool m_bShared;//�Ƿ��̹߳����繲����Ҫ�ź�������
	SLock m_Lock;

};

#endif // !defined(AFX_SKTPTRLIST_H__12695A0C_F4BA_4DF7_9F35_17CD7703DDE1__INCLUDED_)
