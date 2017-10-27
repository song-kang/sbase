/**
 *
 * �� �� �� : SKeyList.h
 * �������� : 2014-6-11 9:26
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : ��QT��QHash������ģ����
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2014-6-11	�ۿ�������ļ�
 *
 **/

#ifndef __SKEY_LIST_H__
#define __SKEY_LIST_H__


#ifndef NULL
#define NULL 0
#endif

#ifndef ASSERT
#define ASSERT
#endif

#define SKEYLIST_THREAD_SAFE

#ifdef SKEYLIST_THREAD_SAFE
#include "SLock.h"
#endif

//////////////////////////////////////////////////////////////////////////
// ��    ��:  SKeyList
// ��    ��:  �ۿ���
// ����ʱ��:  2014-6-11 9:45
// ��    ��:  ���ڼ�ֵ�����Ķ�̬����ģ����
//////////////////////////////////////////////////////////////////////////
template <class key_type,class obj_type>
class SKeyList
{
public:
	struct NODE
	{
		key_type key;
		obj_type pObj;
		NODE* pNext;
	};
	class iterator;
	iterator& begin()
	{
		m_Iter.m_pNode = m_pNode;
		return m_Iter;
	}
	iterator& end()
	{
		m_Iter.m_pNode = NULL;
		return m_Iter;
	}
	//������
	class iterator
	{
	public:
		iterator()
		{
			m_ppNode = NULL;
			m_pNode = NULL;
		}
		obj_type value()
		{
			if(m_pNode == NULL)
				return NULL;
			return m_pNode->pObj;
		}
		key_type key()
		{
			if(m_pNode == NULL)
				return NULL;
			return m_pNode->key;
		}
		void operator=(iterator &obj)
		{
			this->m_ppNode = obj.m_ppNode;
			this->m_pNode = obj.m_pNode;
		}
		iterator* operator++()
		{
			if(m_pNode == NULL)
				return NULL;
			m_pNode = m_pNode->pNext;
			return this;
		};
		iterator* operator++(int)
		{
			if(m_pNode == NULL)
				return NULL;
			m_pNode = m_pNode->pNext;
			return this;
		};
		bool operator!=(iterator &obj)
		{
			return this->m_pNode != obj.m_pNode;
		}
		bool operator==(iterator &obj)
		{
			return this->m_pNode == obj.m_pNode;
		}
		NODE* m_pNode;
		NODE** m_ppNode;
	};
	
	SKeyList()
	{
		m_pNode = NULL;
		m_iSize  = 0;
		m_bAutoDel = false;
		m_pNode = NULL;
		m_pTail = NULL;
#ifdef SKEYLIST_THREAD_SAFE
		m_pLock = NULL;
#endif
		m_Iter.m_ppNode = &m_pNode;
		m_Iter.m_pNode = m_pNode;
	};
	virtual ~SKeyList()
	{
		clear();
#ifdef SKEYLIST_THREAD_SAFE
		if(m_pLock != NULL)
		{
			delete m_pLock;
			m_pLock = NULL;
		}
#endif
	};


	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����Զ��ͷ�ģʽ���Ƿ���ɾ���ڵ�ʱ�ͷŽڵ��Ӧ�Ķ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-6-11 9:47
	// ����˵��:  @b�Ƿ��ͷŶ���
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void setAutoDelete(bool b)
	{
		m_bAutoDel = b;
	}

#ifdef SKEYLIST_THREAD_SAFE
	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �Ƿ�Ϊ�̹߳���ģʽ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-6-11 9:47
	// ����˵��:  void
	// �� �� ֵ:  true��ʾ���̹߳���ģʽ��false��ʾ���ǹ���ģʽ
	//////////////////////////////////////////////////////////////////////////
	bool isShared() {return m_pLock != NULL;};

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����̹߳���ģʽ�������߳̿��ܲ�����дʱ��Ҫ��Ϊtrue
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-6-11 9:48
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
#endif

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���м���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-6-11 9:49
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	inline void lock()
	{
#ifdef SKEYLIST_THREAD_SAFE
		if(m_pLock != NULL)
			m_pLock->lock();
#endif
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���н���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-6-11 9:49
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	inline void unlock()
	{
#ifdef SKEYLIST_THREAD_SAFE
		if(m_pLock != NULL)
			m_pLock->unlock();
#endif
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ɾ��ȫ���ڵ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-6-11 9:51
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void clear()
	{
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
		unlock();
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ɾ��ָ��ָ��Ķ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-6-11 9:52
	// ����˵��:  @pObjΪ��ɾ���Ķ���ָ��
	// �� �� ֵ:  true��ʾɾ���ɹ���falseʧ��
	//////////////////////////////////////////////////////////////////////////
	bool remove(obj_type pObj)
	{
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
				unlock();
				return true;
			}
			prev = p;
			p = p->pNext;
		}
		unlock();
		return false;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ɾ��ָ����ֵ�Ķ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-6-11 9:53
	// ����˵��:  @pObjΪ��ɾ���Ķ���ָ��
	// �� �� ֵ:  true��ʾɾ���ɹ���falseʧ��
	//////////////////////////////////////////////////////////////////////////
	bool remove(key_type pKey)
	{
		lock();
		NODE *p=m_pNode,*prev=NULL;
		while(p)
		{
			if(p->key == pKey)
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
				unlock();
				return true;
			}
			prev = p;
			p = p->pNext;
		}
		unlock();
		return false;
	}


	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ָ���ļ�ֵ�Ƿ����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-6-11 9:53
	// ����˵��:  @pObjΪ�����ָ��
	// �� �� ֵ:  true��ʾ�˶�����ڣ�false��ʾ������
	//////////////////////////////////////////////////////////////////////////
	bool exist(key_type pKey)
	{
		return position(pKey) >= 0;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡָ�������ڶ����е�λ��,��0��ʼ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-6-11 9:54
	// ����˵��:  @pObjΪ�����ָ��
	// �� �� ֵ:  >0Ϊ����λ�ã�-1��ʾ���󲻴���
	//////////////////////////////////////////////////////////////////////////
	int position(key_type pKey)
	{
		lock();
		NODE *p=m_pNode,*prev=NULL;
		int i=-1;
		while(p)
		{
			i++;
			if(p->key == pKey)
			{
				unlock();
				return i;
			}
			prev = p;
			p = p->pNext;
		}
		unlock();
		return -1;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ���е��ܽڵ�����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-6-11 9:55
	// ����˵��:  void
	// �� �� ֵ:  �ڵ�����
	//////////////////////////////////////////////////////////////////////////
	inline int count()
	{
		return m_iSize;
	}
	inline int size()
	{
		return m_iSize;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����Ƿ�Ϊ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-6-11 9:55
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
	// ����ʱ��:  2014-6-11 9:55
	// ����˵��:  @pObjΪ�½ڵ��Ӧ�Ķ���
	// �� �� ֵ:  true
	//////////////////////////////////////////////////////////////////////////
	bool insert(key_type key,obj_type pObj)
	{
		remove(key);
		lock();
		NODE *p=m_pNode;
		NODE *pNew = new NODE;
		pNew->key = key;
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
		unlock();
		return true;
	}


	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡָ��λ�õĽڵ����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-6-11 9:55
	// ����˵��:  @i��ʾ��ȡ�����ڶ����е�λ�ã�i��0��ʼ���
	// �� �� ֵ:  ����ָ�룬NULL��ʾδ�ҵ�
	//////////////////////////////////////////////////////////////////////////
	iterator find(key_type key)
	{
		lock();
		NODE *p=m_pNode;
		while(p)
		{
			if(p->key == key)
			{
				unlock();
				m_Iter.m_pNode = p;
				return m_Iter;
			}
			p = p->pNext;
		}
		unlock();
		m_Iter.m_pNode = NULL;
		return m_Iter;
	}

	obj_type value(key_type key)
	{
		lock();
		NODE *p=m_pNode;
		while(p)
		{
			if(p->key == key)
			{
				unlock();
				return p->pObj;
			}
			p = p->pNext;
		}
		unlock();
		return NULL;
	}


	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡָ��λ�õĽڵ����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-6-11 9:55
	// ����˵��:  @i��ʾ��ȡ�����ڶ����е�λ�ã�i��0��ʼ���
	// �� �� ֵ:  ����ָ�룬NULL��ʾδ�ҵ�
	//////////////////////////////////////////////////////////////////////////
	obj_type operator[](key_type key)
	{
		iterator p = find(key);
		if(p == NULL)
			return NULL;
		return p.value();
	}


	NODE *m_pNode,*m_pTail;
private:

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ӽڵ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-6-11 9:55
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
	iterator m_Iter;
#ifdef SKEYLIST_THREAD_SAFE
	SLock *m_pLock;
#endif

};


#endif //__SKEY_LIST_H__
