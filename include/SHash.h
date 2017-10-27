/**
 *
 * �� �� �� : SHash.h
 * �������� : 2014-4-26 14:22
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : ����ɢ�еĶ������ģ��
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2014-4-26	�ۿ�������ļ�
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
// ��    ��:  SIntHash
// ��    ��:  �ۿ���
// ����ʱ��:  2014:4:26 17:21
// ��    ��:  ������������ֵ��ɢ��ģ���࣬��Ҫָ����ָ�������ɢ��Ψһ��ֵ�ļ��㺯��
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
	// ��    ��:  ���캯��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014:4:26 17:23
	// ����˵��:  @iHashTabSizeΪɢ�б�Ĵ�С
	// �� �� ֵ:  void
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
	// ��    ��:  ����ɢ�б��С�����Ѵ��������������ȫ������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-7-21 14:51
	// ����˵��:  @iHashTabSizeΪɢ�б��С
	// �� �� ֵ:  void
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
	// ��    ��:  ȡɢ�б��С
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-7-21 14:51
	// ����˵��:  
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	int GetHashSize()
	{
		return m_iHashTabSize;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����Զ��ͷ�ģʽ���Ƿ���ɾ���ڵ�ʱ�ͷŽڵ��Ӧ�Ķ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014:4:26 17:24
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
	// ����ʱ��:  2014:4:26 17:24
	// ����˵��:  void
	// �� �� ֵ:  true��ʾ���̹߳���ģʽ��false��ʾ���ǹ���ģʽ
	//////////////////////////////////////////////////////////////////////////
	bool isShared() {return m_pLock != NULL;};

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����̹߳���ģʽ�������߳̿��ܲ�����дʱ��Ҫ��Ϊtrue
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014:4:26 17:24
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
	// ����ʱ��:  2014:4:26 17:24
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
	// ����ʱ��:  2014:4:26 17:24
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	inline void unlock()
	{
		if(m_pLock != NULL)
			m_pLock->unlock();
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ն��󼯺ϣ�ɾ�����нڵ㣬��Ϊ�Զ��ͷ�ʱ��ͬʱ�ͷ�ȫ������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014:4:26 17:24
	// ����˵��:  void
	// �� �� ֵ:  void
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
	// ��    ��:  ɾ��ָ��Ķ���ָ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014:4:26 17:25
	// ����˵��:  @pObjΪ����ָ��
	// �� �� ֵ:  true��ʾɾ���ɹ���false��ʾɾ��ʧ��
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
	// ��    ��:  ɾ��ָ��Ψһ��ֵ�Ķ���ڵ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014:4:26 17:25
	// ����˵��:  @hash_keyΪ��Ӧ��ɾ������Ψһ�ļ�ֵ
	// �� �� ֵ:  true��ʾɾ���ɹ���false��ʾɾ��ʧ��
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
	// ��    ��:  ���һ���µĽڵ㵽���󼯺���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014:4:26 17:26
	// ����˵��:  @pObjΪ����ӵĶ���ָ��
	// �� �� ֵ:  true��ʾ��ӳɹ���false��ʾ���ʧ��
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
	// ��    ��:  ���Դ�ָ�������Ƿ�����ڼ����У��Ƚ϶���ָ�������ֵ����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014:4:26 17:27
	// ����˵��:  @pObjΪ����ָ��
	// �� �� ֵ:  true��ʾ������ڣ�false��ʾ���󲻴���
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
	// ��    ��:  ��ǰ�����еĽڵ�����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014:4:26 17:28
	// ����˵��:  void
	// �� �� ֵ:  int
	//////////////////////////////////////////////////////////////////////////
	inline int count()
	{
		return m_iSize;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ǰ�����Ƿ�Ϊ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014:4:26 17:28
	// ����˵��:  void
	// �� �� ֵ:  true��ʾΪ�գ�false��ʾ��Ϊ��
	//////////////////////////////////////////////////////////////////////////
	inline bool isEmpty()
	{
		return m_iSize == 0;
	}	
	
	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡָ��Ψһɢ�м�ֵ�Ķ���ָ�룬���ж��ƥ��ʱ�����ص�һ������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014:4:26 17:29
	// ����˵��:  @hash_keyΪ����ѯ�Ķ���Ψһɢ�м�ֵ
	// �� �� ֵ:  type*,NULL��ʾδ�ҵ�
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
	// ��    ��:  ����ָ��Ψһɢ�м�ֵ�Ķ���ָ�뼯�ϣ��ж��ƥ�䷵�ض����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-01-15  16:29
	// ����˵��:  @hash_keyΪ����ѯ�Ķ���Ψһɢ�м�ֵ
	//            @list��ʾ�б�����
	// �� �� ֵ:  ƥ��Ķ�������
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
	// ��    ��:  ȡָ��λ�õĽڵ����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 15:13
	// ����˵��:  @i��ʾ��ȡ�����ڶ����е�λ�ã�i��0��ʼ���
	// �� �� ֵ:  ����ָ�룬NULL��ʾδ�ҵ�
	//////////////////////////////////////////////////////////////////////////
	type * operator[](int hash_key)
	{
		return at(hash_key);
	}

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���ûص���������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-6-11 16:49
	// ����˵��:  @pΪ����ָ��
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	inline void SetCallbackParam(void* p)
	{
		m_cbParam = p;
	}

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����ָ�������Ӧ��ֵ�ĵ�һ�������ַ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-6-12 9:01
	// ����˵��:  @hash_keyΪ������ֵ
	//         :  @posΪ���÷��ص���һλ�ã���FetchNextʹ��
	// �� �� ֵ:  type*
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
	// ��    ��:  ����ָ�������Ӧ��ֵ����һ�������ַ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-6-12 13:01
	// ����˵��:  @hash_keyΪ������ֵ
	//         :  @posΪ���÷��ص���һλ�ã�����һ��FetchNextʹ��
	// �� �� ֵ:  type*
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
	NODE **m_pHashTable;//ɢ�б�
	int m_iHashTabSize;	//ɢ�б��С
	bool m_bAutoDel;	//�Ƿ��Զ�ɾ��
	int m_iSize;		//��ǰ�ڵ�����
	SLock *m_pLock;		//�߳���
	void* m_cbParam;	//�ص���������
};



//////////////////////////////////////////////////////////////////////////
// ��    ��:  SGeneralHash
// ��    ��:  �ۿ���
// ����ʱ��:  2014:4:26 17:32
// ��    ��:  ����δ֪��ֵ��ɢ��ģ���࣬��Ҫָ����ָ�������ɢ�м�ֵ�ļ��㺯��(��ֵ����Ψһ)������ָ������ȽϺ���(��ȷ�Ƚ�)
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
	// ��    ��:  ���캯��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014:4:26 17:33
	// ����˵��:  @iHashTabSizeΪɢ�б�Ĵ�С
	// �� �� ֵ:  void
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
	// ��    ��:  ����ɢ�б��С�����ú󽫶�ʧȫ����ɢ������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-6-11 17:00
	// ����˵��:  @iHashTabSizeΪɢ�д�С
	// �� �� ֵ:  void
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
	// ��    ��:  ȡɢ�б��С
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-6-11 17:00
	// ����˵��:  void
	// �� �� ֵ:  int
	//////////////////////////////////////////////////////////////////////////
	int GetHashSize()
	{
		return m_iHashTabSize;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����Զ��ͷ�ģʽ���Ƿ���ɾ���ڵ�ʱ�ͷŽڵ��Ӧ�Ķ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014:4:26 17:34
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
	// ����ʱ��:  2014:4:26 17:34
	// ����˵��:  void
	// �� �� ֵ:  true��ʾ���̹߳���ģʽ��false��ʾ���ǹ���ģʽ
	//////////////////////////////////////////////////////////////////////////
	bool isShared() {return m_pLock != NULL;};

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����̹߳���ģʽ�������߳̿��ܲ�����дʱ��Ҫ��Ϊtrue
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014:4:26 17:34
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
	// ����ʱ��:  2014:4:26 17:34
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
	// ����ʱ��:  2014:4:26 17:34
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	inline void unlock()
	{
		if(m_pLock != NULL)
			m_pLock->unlock();
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ն��󼯺ϣ�ɾ�����нڵ㣬��Ϊ�Զ��ͷ�ʱ��ͬʱ�ͷ�ȫ������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014:4:26 17:34
	// ����˵��:  void
	// �� �� ֵ:  void
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
	// ��    ��:  ɾ��ָ���Ķ���ָ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014:4:26 17:35
	// ����˵��:  @pObjΪ����ָ��
	// �� �� ֵ:  true��ʾɾ���ɹ���false��ʾɾ��ʧ��
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
	// ��    ��:  ɾ�������������ȵĽڵ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014:4:26 17:35
	// ����˵��:  @ObjΪ��ɾ���ο����󣬸ö��󲻱��뼯���еĶ�����ͬһ���������뱣֤SHash_Compare�����ȽϽ�����
	// �� �� ֵ:  true��ʾɾ���ɹ���false��ʾɾ��ʧ��
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
	// ��    ��:  ���һ���µĽڵ㵽���󼯺���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014:4:26 17:36
	// ����˵��:  @pObjΪ����ӵĶ���ָ��
	// �� �� ֵ:  true��ʾ��ӳɹ���false��ʾ���ʧ��
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
	// ��    ��:  ���Դ�ָ�������Ƿ�����ڼ����У��Ƚ϶���ָ�����������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014:4:26 17:37
	// ����˵��:  @pObjΪ����ָ��
	// �� �� ֵ:  true��ʾ������ڣ�false��ʾ���󲻴���
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
	// ��    ��:  ��ǰ�����еĽڵ�����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014:4:26 17:38
	// ����˵��:  void
	// �� �� ֵ:  int
	//////////////////////////////////////////////////////////////////////////
	inline int count()
	{
		return m_iSize;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ǰ�����Ƿ�Ϊ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014:4:26 17:38
	// ����˵��:  void
	// �� �� ֵ:  true��ʾΪ�գ�false��ʾ��Ϊ��
	//////////////////////////////////////////////////////////////////////////
	inline bool isEmpty()
	{
		return m_iSize == 0;
	}	

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ�����������ȵĶ���ָ�룬��������ƥ��ʱ�����ص�һ������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014:4:26 17:39
	// ����˵��:  @ObjΪ�Ƚϲο�����
	// �� �� ֵ:  type*,NULL��ʾδ�ҵ�
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
	// ��    ��:  ����ָ��Ψһɢ�м�ֵ�Ķ���ָ�뼯�ϣ��ж��ƥ�䷵�ض����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-01-15  16:29
	// ����˵��:  @ObjΪ�ο����󣬴�Ѱ�ҵĶ���ͨ��pFunCompare�����Ƚ�Ӧ���
	//            @list��ʾ�б�����
	// �� �� ֵ:  ƥ��Ķ�������
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
	// ��    ��:  ȡ�����������ȵĶ���ָ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 15:33
	// ����˵��:  @ObjΪ�Ƚϲο�����
	// �� �� ֵ:  ����ָ�룬NULL��ʾδ�ҵ�
	//////////////////////////////////////////////////////////////////////////
	type * operator[](type &Obj)
	{
		return at(Obj);
	}

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���ûص���������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-6-11 16:49
	// ����˵��:  @pΪ����ָ��
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	inline void SetCallbackParam(void* p)
	{
		m_cbParam = p;
	}

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����ָ�������Ӧ��ֵ�ĵ�һ�������ַ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-6-12 9:01
	// ����˵��:  @hash_keyΪ������ֵ
	//         :  @posΪ���÷��ص���һλ�ã���FetchNextʹ��
	// �� �� ֵ:  type*
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
	// ��    ��:  ����ָ�������Ӧ��ֵ����һ�������ַ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-6-12 13:01
	// ����˵��:  @hash_keyΪ������ֵ
	//         :  @posΪ���÷��ص���һλ�ã�����һ��FetchNextʹ��
	// �� �� ֵ:  type*
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
	NODE **m_pHashTable;//ɢ�б�
	int m_iHashTabSize;	//ɢ�б��С
	bool m_bAutoDel;	//�Ƿ��Զ�ɾ��
	int m_iSize;		//��ǰ�ڵ�����
	SLock *m_pLock;		//�߳���
	void* m_cbParam;	//�ص���������
};




#endif // !defined(AFX_SKTPTRLIST_H__12695A0C_F4BA_4DF7_9F35_17CD7703DDE1__INCLUDED_)
