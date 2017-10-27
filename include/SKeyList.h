/**
 *
 * 文 件 名 : SKeyList.h
 * 创建日期 : 2014-6-11 9:26
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : 类QT中QHash的链表模板类
 * 修改记录 : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2014-6-11	邵凯田　创建文件
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
// 名    称:  SKeyList
// 作    者:  邵凯田
// 创建时间:  2014-6-11 9:45
// 描    述:  基于键值与对象的动态链表模板类
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
	//迭代器
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
	// 描    述:  设置自动释放模式（是否在删除节点时释放节点对应的对象）
	// 作    者:  邵凯田
	// 创建时间:  2014-6-11 9:47
	// 参数说明:  @b是否释放对象
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void setAutoDelete(bool b)
	{
		m_bAutoDel = b;
	}

#ifdef SKEYLIST_THREAD_SAFE
	////////////////////////////////////////////////////////////////////////
	// 描    述:  是否为线程共享模式
	// 作    者:  邵凯田
	// 创建时间:  2014-6-11 9:47
	// 参数说明:  void
	// 返 回 值:  true表示是线程共享模式，false表示不是共享模式
	//////////////////////////////////////////////////////////////////////////
	bool isShared() {return m_pLock != NULL;};

	////////////////////////////////////////////////////////////////////////
	// 描    述:  设置线程共享模式，当多线程可能并发读写时需要设为true
	// 作    者:  邵凯田
	// 创建时间:  2014-6-11 9:48
	// 参数说明:  @b是否线程共享模式
	// 返 回 值:  void
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
	// 描    述:  队列加锁
	// 作    者:  邵凯田
	// 创建时间:  2014-6-11 9:49
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	inline void lock()
	{
#ifdef SKEYLIST_THREAD_SAFE
		if(m_pLock != NULL)
			m_pLock->lock();
#endif
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  队列解锁
	// 作    者:  邵凯田
	// 创建时间:  2014-6-11 9:49
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	inline void unlock()
	{
#ifdef SKEYLIST_THREAD_SAFE
		if(m_pLock != NULL)
			m_pLock->unlock();
#endif
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  删除全部节点
	// 作    者:  邵凯田
	// 创建时间:  2014-6-11 9:51
	// 参数说明:  void
	// 返 回 值:  void
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
	// 描    述:  删除指定指针的对象
	// 作    者:  邵凯田
	// 创建时间:  2014-6-11 9:52
	// 参数说明:  @pObj为待删除的对象指针
	// 返 回 值:  true表示删除成功，false失败
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
	// 描    述:  删除指定键值的对象
	// 作    者:  邵凯田
	// 创建时间:  2014-6-11 9:53
	// 参数说明:  @pObj为待删除的对象指针
	// 返 回 值:  true表示删除成功，false失败
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
	// 描    述:  指定的键值是否存在
	// 作    者:  邵凯田
	// 创建时间:  2014-6-11 9:53
	// 参数说明:  @pObj为对象的指针
	// 返 回 值:  true表示此对象存在，false表示不存在
	//////////////////////////////////////////////////////////////////////////
	bool exist(key_type pKey)
	{
		return position(pKey) >= 0;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取指定对象在队列中的位置,从0开始编号
	// 作    者:  邵凯田
	// 创建时间:  2014-6-11 9:54
	// 参数说明:  @pObj为对象的指针
	// 返 回 值:  >0为对象位置，-1表示对象不存在
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
	// 描    述:  取队列的总节点数量
	// 作    者:  邵凯田
	// 创建时间:  2014-6-11 9:55
	// 参数说明:  void
	// 返 回 值:  节点数量
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
	// 描    述:  队列是否为空
	// 作    者:  邵凯田
	// 创建时间:  2014-6-11 9:55
	// 参数说明:  void
	// 返 回 值:  true表示队列为空，false表示队列非空
	//////////////////////////////////////////////////////////////////////////
	inline bool isEmpty()
	{
		return m_iSize == 0;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  向队列末尾添加一个新的节点
	// 作    者:  邵凯田
	// 创建时间:  2014-6-11 9:55
	// 参数说明:  @pObj为新节点对应的对象
	// 返 回 值:  true
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
			//快速插入到尾节点
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
	// 描    述:  取指定位置的节点对象
	// 作    者:  邵凯田
	// 创建时间:  2014-6-11 9:55
	// 参数说明:  @i表示待取对象在队列中的位置，i从0开始编号
	// 返 回 值:  对象指针，NULL表示未找到
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
	// 描    述:  取指定位置的节点对象
	// 作    者:  邵凯田
	// 创建时间:  2014-6-11 9:55
	// 参数说明:  @i表示待取对象在队列中的位置，i从0开始编号
	// 返 回 值:  对象指针，NULL表示未找到
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
	// 描    述:  添加节点
	// 作    者:  邵凯田
	// 创建时间:  2014-6-11 9:55
	// 参数说明:  @pNew新节点
	// 返 回 值:  true
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
