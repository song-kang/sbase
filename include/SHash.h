/**
 *
 * 文 件 名 : SHash.h
 * 创建日期 : 2014-4-26 14:22
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : 基于散列的对象管理模板
 * 修改记录 : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2014-4-26	邵凯田　创建文件
 *
 **/

#ifndef __SKT_HASH_H__
#define __SKT_HASH_H__

#include "SLock.h"
#include "SList.h"
#include "SObject.h"

#ifndef NULL
#define NULL 0
#endif

typedef int (*SHash_Key)(void *cbParam,void *pItem);
typedef bool (*SHash_Compare)(void *cbParam,void *pItem1,void *pItem2);

//////////////////////////////////////////////////////////////////////////
// 名    称:  SIntHash
// 作    者:  邵凯田
// 创建时间:  2014:4:26 17:21
// 描    述:  基于整型数键值的散列模板类，需要指定对指定对象的散列唯一键值的计算函数
//////////////////////////////////////////////////////////////////////////
template <class type,SHash_Key pFunKey>
class SIntHash
{
public:
	struct NODE
	{
		type *pObj;
		NODE* pNext;
	};

	////////////////////////////////////////////////////////////////////////
	// 描    述:  构造函数
	// 作    者:  邵凯田
	// 创建时间:  2014:4:26 17:23
	// 参数说明:  @iHashTabSize为散列表的大小
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	SIntHash(int iHashTabSize)
	{
		SKTOBJECT("SIntHash");
		m_iHashTabSize  = iHashTabSize;
		m_pHashTable = new NODE*[m_iHashTabSize];
		memset(m_pHashTable,0,sizeof(NODE*)*m_iHashTabSize);
		m_bAutoDel = false;
		m_pLock = NULL;
		m_iSize = 0;
	};

	SIntHash()
	{
		SKTOBJECT("SIntHash");
		m_iHashTabSize  = 0;
		m_pHashTable = NULL;
		m_bAutoDel = false;
		m_pLock = NULL;
		m_iSize = 0;
	};

	virtual ~SIntHash()
	{
		clear();
		if(m_pLock != NULL)
		{
			delete m_pLock;
			m_pLock = NULL;
		}
		if(m_pHashTable != NULL)
			delete[] m_pHashTable;
	};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  设置散列表大小，如已存在数据则先清楚全部数据
	// 作    者:  邵凯田
	// 创建时间:  2015-7-21 14:51
	// 参数说明:  @iHashTabSize为散列表大小
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void SetHashSize(int iHashTabSize)
	{
		if(m_pHashTable != NULL)
			delete[] m_pHashTable;
		m_iHashTabSize  = iHashTabSize;
		m_pHashTable = new NODE*[m_iHashTabSize];
		memset(m_pHashTable,0,sizeof(NODE*)*m_iHashTabSize);
	}

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取散列表大小
	// 作    者:  邵凯田
	// 创建时间:  2015-7-21 14:51
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	int GetHashSize()
	{
		return m_iHashTabSize;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  设置自动释放模式（是否在删除节点时释放节点对应的对象）
	// 作    者:  邵凯田
	// 创建时间:  2014:4:26 17:24
	// 参数说明:  @b是否释放对象
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void setAutoDelete(bool b)
	{
		m_bAutoDel = b;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  是否为线程共享模式
	// 作    者:  邵凯田
	// 创建时间:  2014:4:26 17:24
	// 参数说明:  void
	// 返 回 值:  true表示是线程共享模式，false表示不是共享模式
	//////////////////////////////////////////////////////////////////////////
	bool isShared() {return m_pLock != NULL;};

	////////////////////////////////////////////////////////////////////////
	// 描    述:  设置线程共享模式，当多线程可能并发读写时需要设为true
	// 作    者:  邵凯田
	// 创建时间:  2014:4:26 17:24
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

	////////////////////////////////////////////////////////////////////////
	// 描    述:  队列加锁
	// 作    者:  邵凯田
	// 创建时间:  2014:4:26 17:24
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	inline void lock()
	{
		if(m_pLock != NULL)
			m_pLock->lock();
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  队列解锁
	// 作    者:  邵凯田
	// 创建时间:  2014:4:26 17:24
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	inline void unlock()
	{
		if(m_pLock != NULL)
			m_pLock->unlock();
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  清空对象集合，删除所有节点，设为自动释放时还同时释放全部对象
	// 作    者:  邵凯田
	// 创建时间:  2014:4:26 17:24
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void clear()
	{
		m_iSize = 0;
		lock();
		int i;
		NODE *p,*p0;
		for(i=0;i<m_iHashTabSize;i++)
		{
			p = m_pHashTable[i];
			while(p)
			{
				p0 = p->pNext;
				if(m_bAutoDel && p->pObj != NULL)
					delete p->pObj;
				delete p;
				p = p0;
			}
		}
		memset(m_pHashTable,0,sizeof(NODE*)*m_iHashTabSize);
		unlock();
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  删除指点的对象指针
	// 作    者:  邵凯田
	// 创建时间:  2014:4:26 17:25
	// 参数说明:  @pObj为对象指针
	// 返 回 值:  true表示删除成功，false表示删除失败
	//////////////////////////////////////////////////////////////////////////
	bool remove(type *pObj)
	{
		int hash_key = pFunKey(m_cbParam, pObj);
		lock();
		int hash_idx = hash_key%m_iHashTabSize;
		NODE *p1=NULL,*p = m_pHashTable[hash_idx];
		while(p != NULL && p->pObj != pObj)
		{
			p1 = p;
			p = p->pNext;
		}
		if(p == NULL)
		{
			unlock();
			return false;
		}
		if(p1 == NULL)
			m_pHashTable[hash_idx] = p->pNext;
		else
			p1->pNext = p->pNext;
		if(m_bAutoDel)
			delete p->pObj;
		delete p;
		m_iSize --;
		unlock();
		return true;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  删除指定唯一键值的对象节点
	// 作    者:  邵凯田
	// 创建时间:  2014:4:26 17:25
	// 参数说明:  @hash_key为对应待删除对象唯一的键值
	// 返 回 值:  true表示删除成功，false表示删除失败
	//////////////////////////////////////////////////////////////////////////
	bool remove(int hash_key)
	{
		lock();
		int hash_idx = hash_key%m_iHashTabSize;
		NODE *p1=NULL,*p = m_pHashTable[hash_idx];
		while(p != NULL && pFunKey(m_cbParam,p->pObj) != hash_key)
		{
			p1 = p;
			p = p->pNext;
		}
		if(p == NULL)
		{
			unlock();
			return false;
		}
		if(p1 == NULL)
			m_pHashTable[hash_idx] = p->pNext;
		else
			p1->pNext = p->pNext;
		if(m_bAutoDel)
			delete p->pObj;
		delete p;
		m_iSize --;
		unlock();
		return true;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  添加一个新的节点到对象集合中
	// 作    者:  邵凯田
	// 创建时间:  2014:4:26 17:26
	// 参数说明:  @pObj为待添加的对象指针
	// 返 回 值:  true表示添加成功，false表示添加失败
	//////////////////////////////////////////////////////////////////////////
	bool append(type *pObj)
	{
		lock();
		int hash_idx = pFunKey(m_cbParam,pObj)%m_iHashTabSize;
		NODE *p = new NODE();
		p->pObj = pObj;
		p->pNext = m_pHashTable[hash_idx];
		m_pHashTable[hash_idx] = p;
		m_iSize++;
		unlock();
		return true;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  测试打指定对象是否存在于集合中，比较对象指针而不是值内容
	// 作    者:  邵凯田
	// 创建时间:  2014:4:26 17:27
	// 参数说明:  @pObj为对象指针
	// 返 回 值:  true表示对象存在，false表示对象不存在
	//////////////////////////////////////////////////////////////////////////
	bool exist(type *pObj)
	{
		lock();
		int hash_idx = pFunKey(m_cbParam,pObj)%m_iHashTabSize;
		NODE *p = m_pHashTable[hash_idx];
		while(p != NULL && p->pObj != pObj)
		{
			p = p->pNext;
		}
		if(p == NULL)
		{
			unlock();
			return false;
		}
		unlock();
		return true;

		return false;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  当前集合中的节点数量
	// 作    者:  邵凯田
	// 创建时间:  2014:4:26 17:28
	// 参数说明:  void
	// 返 回 值:  int
	//////////////////////////////////////////////////////////////////////////
	inline int count()
	{
		return m_iSize;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  当前集合是否为空
	// 作    者:  邵凯田
	// 创建时间:  2014:4:26 17:28
	// 参数说明:  void
	// 返 回 值:  true表示为空，false表示不为空
	//////////////////////////////////////////////////////////////////////////
	inline bool isEmpty()
	{
		return m_iSize == 0;
	}	
	
	////////////////////////////////////////////////////////////////////////
	// 描    述:  取指定唯一散列键值的对象指针，如有多个匹配时仅返回第一个对象
	// 作    者:  邵凯田
	// 创建时间:  2014:4:26 17:29
	// 参数说明:  @hash_key为待查询的对象唯一散列键值
	// 返 回 值:  type*,NULL表示未找到
	//////////////////////////////////////////////////////////////////////////
	type * at(int hash_key)
	{
		lock();
		NODE *p = m_pHashTable[hash_key%m_iHashTabSize];
		while(p != NULL && pFunKey(m_cbParam,p->pObj) != hash_key)
		{
			p = p->pNext;
		}
		if(p == NULL)
		{
			unlock();
			return NULL;
		}
		unlock();
		return p->pObj;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  搜索指定唯一散列键值的对象指针集合（有多个匹配返回多个）
	// 作    者:  邵凯田
	// 创建时间:  2015-01-15  16:29
	// 参数说明:  @hash_key为待查询的对象唯一散列键值
	//            @list表示列表数量
	// 返 回 值:  匹配的对象数量
	//////////////////////////////////////////////////////////////////////////
	int search(int hash_key, SPtrList<type> &list)
	{
		list.clear();
		lock();
		NODE *p = m_pHashTable[hash_key%m_iHashTabSize];
		while (p != NULL)
		{
			if (pFunKey(m_cbParam, p->pObj) == hash_key)
				list.append(p->pObj);
			p = p->pNext;
		}
		unlock();
		return list.count();
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取指定位置的节点对象
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 15:13
	// 参数说明:  @i表示待取对象在队列中的位置，i从0开始编号
	// 返 回 值:  对象指针，NULL表示未找到
	//////////////////////////////////////////////////////////////////////////
	type * operator[](int hash_key)
	{
		return at(hash_key);
	}

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  设置回调函数参数
	// 作    者:  邵凯田
	// 创建时间:  2014-6-11 16:49
	// 参数说明:  @p为参数指针
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	inline void SetCallbackParam(void* p)
	{
		m_cbParam = p;
	}

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  遍历指定对象对应键值的第一个对象地址
	// 作    者:  邵凯田
	// 创建时间:  2014-6-12 9:01
	// 参数说明:  @hash_key为索引键值
	//         :  @pos为引用返回的下一位置，供FetchNext使用
	// 返 回 值:  type*
	//////////////////////////////////////////////////////////////////////////
	type* FetchFirst(int hash_key,unsigned long &pos)
	{
		int hash_idx = hash_key%m_iHashTabSize; 
		NODE *p = m_pHashTable[hash_idx];
		if(p == NULL)
		{
			pos = 0;
			return NULL;
		}
		else
		{
			while(p != NULL && hash_key != pFunKey(m_cbParam,p->pObj))
			{
				p = p->pNext;
			}
			if(p != NULL)
			{
				pos = (unsigned long)p;
				return p->pObj;
			}
			else
			{
				pos = 0;
				return NULL;			
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  遍历指定对象对应键值的下一个对象地址
	// 作    者:  邵凯田
	// 创建时间:  2014-6-12 13:01
	// 参数说明:  @hash_key为索引键值
	//         :  @pos为引用返回的下一位置，供下一次FetchNext使用
	// 返 回 值:  type*
	//////////////////////////////////////////////////////////////////////////
	type* FetchNext(int hash_key,unsigned long &pos)
	{
		NODE *p = (NODE *)pos;
		while(p != NULL && hash_key != pFunKey(m_cbParam,p->pObj))
		{
			p = p->pNext;
		}
		if(p != NULL)
		{
			pos = (unsigned long)p;
			return p->pObj;
		}
		else
		{
			pos = 0;
			return NULL;
		}
	}

private:
	NODE **m_pHashTable;//散列表
	int m_iHashTabSize;	//散列表大小
	bool m_bAutoDel;	//是否自动删除
	int m_iSize;		//当前节点数量
	SLock *m_pLock;		//线程锁
	void* m_cbParam;	//回调函数参数
};



//////////////////////////////////////////////////////////////////////////
// 名    称:  SGeneralHash
// 作    者:  邵凯田
// 创建时间:  2014:4:26 17:32
// 描    述:  基于未知键值的散列模板类，需要指定对指定对象的散列键值的计算函数(键值不必唯一)，另需指定对象比较函数(精确比较)
//////////////////////////////////////////////////////////////////////////
template <class type,SHash_Key pFunKey,SHash_Compare pFunCompare>
class SGeneralHash
{
public:
	struct NODE
	{
		type *pObj;
		NODE* pNext;
	};

	SGeneralHash()
	{
		SKTOBJECT("SClassHash");
		m_pHashTable = NULL;
		m_iHashTabSize  = 0;
		m_bAutoDel = false;
		m_pLock = NULL;
		m_iSize = 0;
	};

	////////////////////////////////////////////////////////////////////////
	// 描    述:  构造函数
	// 作    者:  邵凯田
	// 创建时间:  2014:4:26 17:33
	// 参数说明:  @iHashTabSize为散列表的大小
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	SGeneralHash(int iHashTabSize)
	{
		SKTOBJECT("SClassHash");
		m_iHashTabSize  = 0;
		m_pHashTable = NULL;
		m_bAutoDel = false;
		m_pLock = NULL;
		m_iSize = 0;
		SetHashSize(iHashTabSize);
	};

	virtual ~SGeneralHash()
	{
		clear();
		if(m_pLock != NULL)
		{
			delete m_pLock;
			m_pLock = NULL;
		}
		if(m_pHashTable != NULL)
			delete[] m_pHashTable;
	};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  设置散列表大小，设置后将丢失全部的散列内容
	// 作    者:  邵凯田
	// 创建时间:  2014-6-11 17:00
	// 参数说明:  @iHashTabSize为散列大小
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void SetHashSize(int iHashTabSize)
	{
		if(m_pHashTable != NULL)
			delete[] m_pHashTable;
		m_iHashTabSize  = iHashTabSize;
		m_pHashTable = new NODE*[m_iHashTabSize];
		memset(m_pHashTable,0,sizeof(NODE*)*m_iHashTabSize);
	}

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取散列表大小
	// 作    者:  邵凯田
	// 创建时间:  2014-6-11 17:00
	// 参数说明:  void
	// 返 回 值:  int
	//////////////////////////////////////////////////////////////////////////
	int GetHashSize()
	{
		return m_iHashTabSize;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  设置自动释放模式（是否在删除节点时释放节点对应的对象）
	// 作    者:  邵凯田
	// 创建时间:  2014:4:26 17:34
	// 参数说明:  @b是否释放对象
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void setAutoDelete(bool b)
	{
		m_bAutoDel = b;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  是否为线程共享模式
	// 作    者:  邵凯田
	// 创建时间:  2014:4:26 17:34
	// 参数说明:  void
	// 返 回 值:  true表示是线程共享模式，false表示不是共享模式
	//////////////////////////////////////////////////////////////////////////
	bool isShared() {return m_pLock != NULL;};

	////////////////////////////////////////////////////////////////////////
	// 描    述:  设置线程共享模式，当多线程可能并发读写时需要设为true
	// 作    者:  邵凯田
	// 创建时间:  2014:4:26 17:34
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

	////////////////////////////////////////////////////////////////////////
	// 描    述:  队列加锁
	// 作    者:  邵凯田
	// 创建时间:  2014:4:26 17:34
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	inline void lock()
	{
		if(m_pLock != NULL)
			m_pLock->lock();
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  队列解锁
	// 作    者:  邵凯田
	// 创建时间:  2014:4:26 17:34
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	inline void unlock()
	{
		if(m_pLock != NULL)
			m_pLock->unlock();
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  清空对象集合，删除所有节点，设为自动释放时还同时释放全部对象
	// 作    者:  邵凯田
	// 创建时间:  2014:4:26 17:34
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void clear()
	{
		m_iSize = 0;
		lock();
		int i;
		NODE *p,*p0;
		for(i=0;i<m_iHashTabSize;i++)
		{
			p = m_pHashTable[i];
			while(p)
			{
				p0 = p->pNext;
				if(m_bAutoDel && p->pObj != NULL)
					delete p->pObj;
				delete p;
				p = p0;
			}
		}
		memset(m_pHashTable,0,sizeof(NODE*)*m_iHashTabSize);
		unlock();
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  删除指定的对象指针
	// 作    者:  邵凯田
	// 创建时间:  2014:4:26 17:35
	// 参数说明:  @pObj为对象指针
	// 返 回 值:  true表示删除成功，false表示删除失败
	//////////////////////////////////////////////////////////////////////////
	bool remove(type *pObj)
	{
		int hash_key = pFunKey(m_cbParam,pObj);
		lock();
		int hash_idx = hash_key%m_iHashTabSize;
		NODE *p1=NULL,*p = m_pHashTable[hash_idx];
		while(p != NULL && p->pObj != pObj)
		{
			p1 = p;
			p = p->pNext;
		}
		if(p == NULL)
		{
			unlock();
			return false;
		}
		if(p1 == NULL)
			m_pHashTable[hash_idx] = p->pNext;
		else
			p1->pNext = p->pNext;
		if(m_bAutoDel)
			delete p->pObj;
		delete p;
		m_iSize --;
		unlock();
		return true;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  删除与给定对象相等的节点
	// 作    者:  邵凯田
	// 创建时间:  2014:4:26 17:35
	// 参数说明:  @Obj为待删除参考对象，该对象不必与集合中的对象是同一个，但必须保证SHash_Compare函数比较结果相等
	// 返 回 值:  true表示删除成功，false表示删除失败
	//////////////////////////////////////////////////////////////////////////
	bool remove(type &Obj)
	{
		lock();
		int hash_idx = pFunKey(m_cbParam,&Obj)%m_iHashTabSize;
		NODE *p1=NULL,*p = m_pHashTable[hash_idx];
		while(p != NULL && !pFunCompare(m_cbParam,p->pObj,&Obj))
		{
			p1 = p;
			p = p->pNext;
		}
		if(p == NULL)
		{
			unlock();
			return false;
		}
		if(p1 == NULL)
			m_pHashTable[hash_idx] = p->pNext;
		else
			p1->pNext = p->pNext;
		if(m_bAutoDel)
			delete p->pObj;
		delete p;
		m_iSize --;
		unlock();
		return true;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  添加一个新的节点到对象集合中
	// 作    者:  邵凯田
	// 创建时间:  2014:4:26 17:36
	// 参数说明:  @pObj为待添加的对象指针
	// 返 回 值:  true表示添加成功，false表示添加失败
	//////////////////////////////////////////////////////////////////////////
	bool append(type *pObj)
	{
		lock();
		int hash_idx = pFunKey(m_cbParam,pObj)%m_iHashTabSize;
		NODE *p = new NODE();
		p->pObj = pObj;
		p->pNext = m_pHashTable[hash_idx];
		m_pHashTable[hash_idx] = p;
		m_iSize++;
		unlock();
		return true;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  测试打指定对象是否存在于集合中，比较对象指针而不是内容
	// 作    者:  邵凯田
	// 创建时间:  2014:4:26 17:37
	// 参数说明:  @pObj为对象指针
	// 返 回 值:  true表示对象存在，false表示对象不存在
	//////////////////////////////////////////////////////////////////////////
	bool exist(type *pObj)
	{
		lock();
		int hash_idx = pFunKey(m_cbParam,pObj)%m_iHashTabSize;
		NODE *p = m_pHashTable[hash_idx];
		while(p != NULL && p->pObj != pObj)
		{
			p = p->pNext;
		}
		if(p == NULL)
		{
			unlock();
			return false;
		}
		unlock();
		return true;

		return false;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  当前集合中的节点数量
	// 作    者:  邵凯田
	// 创建时间:  2014:4:26 17:38
	// 参数说明:  void
	// 返 回 值:  int
	//////////////////////////////////////////////////////////////////////////
	inline int count()
	{
		return m_iSize;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  当前集合是否为空
	// 作    者:  邵凯田
	// 创建时间:  2014:4:26 17:38
	// 参数说明:  void
	// 返 回 值:  true表示为空，false表示不为空
	//////////////////////////////////////////////////////////////////////////
	inline bool isEmpty()
	{
		return m_iSize == 0;
	}	

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取与给定对象相等的对象指针，如多个对象匹配时仅返回第一个对象
	// 作    者:  邵凯田
	// 创建时间:  2014:4:26 17:39
	// 参数说明:  @Obj为比较参考对象
	// 返 回 值:  type*,NULL表示未找到
	//////////////////////////////////////////////////////////////////////////
	type * at(type &Obj)
	{
		lock();
		int hash_idx = pFunKey(m_cbParam,&Obj)%m_iHashTabSize;
		NODE *p = m_pHashTable[hash_idx];
		while(p != NULL && !pFunCompare(m_cbParam,p->pObj,&Obj))
		{
			p = p->pNext;
		}
		if(p == NULL)
		{
			unlock();
			return NULL;
		}
		unlock();
		return p->pObj;
	}


	////////////////////////////////////////////////////////////////////////
	// 描    述:  搜索指定唯一散列键值的对象指针集合（有多个匹配返回多个）
	// 作    者:  邵凯田
	// 创建时间:  2015-01-15  16:29
	// 参数说明:  @Obj为参考对象，待寻找的对象通过pFunCompare函数比较应相等
	//            @list表示列表数量
	// 返 回 值:  匹配的对象数量
	//////////////////////////////////////////////////////////////////////////
	int search(type &Obj, SPtrList<type> &list)
	{
		list.clear();
		lock();
		int hash_idx = pFunKey(m_cbParam, &Obj) % m_iHashTabSize;
		NODE *p = m_pHashTable[hash_idx];
		while (p != NULL)
		{
			if (pFunCompare(m_cbParam, p->pObj, &Obj))
				list.append(p->pObj);
			p = p->pNext;
		}
		unlock();
		return list.count();
	}


	////////////////////////////////////////////////////////////////////////
	// 描    述:  取与给定对象相等的对象指针
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 15:33
	// 参数说明:  @Obj为比较参考对象
	// 返 回 值:  对象指针，NULL表示未找到
	//////////////////////////////////////////////////////////////////////////
	type * operator[](type &Obj)
	{
		return at(Obj);
	}

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  设置回调函数参数
	// 作    者:  邵凯田
	// 创建时间:  2014-6-11 16:49
	// 参数说明:  @p为参数指针
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	inline void SetCallbackParam(void* p)
	{
		m_cbParam = p;
	}

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  遍历指定对象对应键值的第一个对象地址
	// 作    者:  邵凯田
	// 创建时间:  2014-6-12 9:01
	// 参数说明:  @hash_key为索引键值
	//         :  @pos为引用返回的下一位置，供FetchNext使用
	// 返 回 值:  type*
	//////////////////////////////////////////////////////////////////////////
	type* FetchFirst(int hash_key,unsigned long &pos)
	{
		int hash_idx = hash_key%m_iHashTabSize; 
		NODE *p = m_pHashTable[hash_idx];
		if(p == NULL)
		{
			pos = 0;
			return NULL;
		}
		else
		{
			while(p != NULL && hash_key != pFunKey(m_cbParam,p->pObj))
			{
				p = p->pNext;
			}
			if(p != NULL)
			{
				pos = (unsigned long)p;
				return p->pObj;
			}
			else
			{
				pos = 0;
				return NULL;			
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  遍历指定对象对应键值的下一个对象地址
	// 作    者:  邵凯田
	// 创建时间:  2014-6-12 13:01
	// 参数说明:  @hash_key为索引键值
	//         :  @pos为引用返回的下一位置，供下一次FetchNext使用
	// 返 回 值:  type*
	//////////////////////////////////////////////////////////////////////////
	type* FetchNext(int hash_key,unsigned long &pos)
	{
		NODE *p = (NODE *)pos;
		if(p == NULL)
			return NULL;
		p = p->pNext;
		while(p != NULL && hash_key != pFunKey(m_cbParam,p->pObj))
		{
			p = p->pNext;
		}
		if(p != NULL)
		{
			pos = (unsigned long)p;
			return p->pObj;
		}
		else
		{
			pos = 0;
			return NULL;
		}
	}

private:
	NODE **m_pHashTable;//散列表
	int m_iHashTabSize;	//散列表大小
	bool m_bAutoDel;	//是否自动删除
	int m_iSize;		//当前节点数量
	SLock *m_pLock;		//线程锁
	void* m_cbParam;	//回调函数参数
};




#endif // !defined(AFX_SKTPTRLIST_H__12695A0C_F4BA_4DF7_9F35_17CD7703DDE1__INCLUDED_)
