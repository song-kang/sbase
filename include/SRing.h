/**
 *
 * �� �� �� : SRing.h
 * �������� : 2008-05-23
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: 2011/08/31 08:43:34 $
 * ��ǰ�汾 : $Revision: 1.1 $
 * �������� : �ڴ滷ģ��
 * �޸ļ�¼ :
 *            $Log: SRing.h,v $
 *            Revision 1.1  2011/08/31 08:43:34  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2011/02/25 03:12:07  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2011/01/17 01:24:28  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2010/08/09 03:04:43  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2009/09/15 02:14:08  shaokt
 *            ͨѸ��¼����װ����Ŀ�״��ύ
 *
 *            Revision 1.7  2009/03/10 07:44:22  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.6  2009/01/31 06:37:34  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.5  2009/01/06 09:58:18  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.4  2008/09/11 05:17:25  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2008/09/05 06:32:31  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2008/09/02 10:59:10  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/01 11:38:28  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/01 08:58:03  shaokt
 *            *** empty log message ***
 *
 *
 **/

#if !defined(AFX_SKTRING_H__12695A0C_F4BA_4DF7_9F35_17CD7703DDE1__INCLUDED_)
#define AFX_SKTRING_H__12695A0C_F4BA_4DF7_9F35_17CD7703DDE1__INCLUDED_

#include "SApi.h"

//�����ٸ�ֻ���û�
#define C_READER_COUNT 10

//���ڴ滷ִ�ж��������û�
enum RING_READER_LIST
{
	RING_READER1 = 0,
	RING_READER2 = 1,
	RING_READER3 = 2,
	RING_READER4 = 3,
	RING_READER5 = 4,
	RING_READER6 = 5,
	RING_READER7 = 6,
	RING_READER8 = 7,
	RING_READER9 = 8,
	RING_READER10 = 9,

	RING_READER_NONE = 199
};

template <class type>
class SRing
{
public:
	SRing(int iSize=0)
	{
		m_iWritePos = -1;
		m_pObjects = NULL;
		m_ReaderFlag = 0;
		init(iSize);
		m_Flag = 0;
	};
    
	virtual ~SRing()
	{
		if(m_pObjects != NULL)
			delete[] m_pObjects;
	};

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ʼ���ڴ滷��С
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 22:44
	// ����˵��:  
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	void init(int iSize)
	{
		if(m_pObjects != NULL && m_iSize != iSize)
		{
			delete[] m_pObjects;
			m_pObjects = NULL;
		}
		m_iSize		= iSize;
		if(iSize == 0)
		{
			if(m_pObjects != NULL)
				delete[] m_pObjects;
			m_pObjects = NULL;
		}
		else if(m_pObjects == NULL)
		{
			m_pObjects	= new type[iSize];
			memset(m_pObjects,0,sizeof(type)*iSize);
		}
		m_iWritePos = -1;//��ǰ���һ������Ԫ�ص�λ��
		for(int i=0;i<C_READER_COUNT;i++)
		{
			m_iaReaderPos[i] = -1;
		}
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ���е�ֻ���û���û�п��з���RING_READER_NONE
	// ��    ��:  �ۿ���
	// ����ʱ��:  2008-9-2 17:26
	// ����˵��:  void
	// �� �� ֵ:  RING_READER_LIST
	//////////////////////////////////////////////////////////////////////////
	RING_READER_LIST getIdleReader()
	{
		DWORD d;
		static bool bDoing = false;//���ڼ򵥻���
		while(bDoing);
		bDoing = true;
		for(int i=0;i<C_READER_COUNT;i++)
		{
			d = m_ReaderFlag;
			d = d >> i;
			if((d&1) == 0)
			{
				//д��ʶ
				DWORD d0=(1 << i);
				m_ReaderFlag |= d0;
				bDoing = false;
				return (RING_READER_LIST)i;
			}
		}
		bDoing = false;
		return RING_READER_NONE;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �ͷ�ָ���Ķ��û�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 22:44
	// ����˵��:  @reader��ʾ���û�
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void releaseIdleReader(RING_READER_LIST reader)
	{
		DWORD d;
		d = ~(1 << reader);
		m_ReaderFlag &= d;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �ƶ�������λ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 22:44
	// ����˵��:  @reader���û�
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void gotoLastPos(RING_READER_LIST reader)
	{
		m_iaReaderPos[reader] = m_iWritePos;
	}

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ȡ��������Ϣ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2012-2-24 10:45
	// ����˵��:  @pBufferΪ��������ַ
	//            @iItemBytesΪһ��Ԫ����ռ���ֽ���
	//            @iAllItemsΪԪ���ܸ���
	//            @piLastItemPosΪ���һ��Ԫ�ص�λ��
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void getBufferInfo(const BYTE* &pBuffer,int &iItemBytes,int &iAllItems,const int* &piLastItemPos)
	{
		pBuffer = (BYTE*)m_pObjects;
		iItemBytes = sizeof(type);
		iAllItems = m_iSize;
		piLastItemPos = &m_iWritePos;
	}

	void P()
	{return;
		while(m_Flag != 0)
		{
#ifdef WIN32
			Sleep(1);
#else
			usleep(1);
#endif
		}
		m_Flag ++;
	}
	void V()
	{return;
		m_Flag--;
	}
	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��һ��׼��д��λ�õĶ���ָ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2008-5-23
	// ����˵��:  void
	// �� �� ֵ:  type*
	//////////////////////////////////////////////////////////////////////////
	type* getNextObjForWrite()
	{
		P();
		int pos;
		if(m_iWritePos == m_iSize-1)
			pos = 0;//�ۻ�
		else
			pos = m_iWritePos + 1;
		V();
		return &(m_pObjects[pos]);
	}

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ���µ�һ������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2008-7-30 9:37
	// ����˵��:  void
	// �� �� ֵ:  type*
	//////////////////////////////////////////////////////////////////////////
	type* getLastObj()
	{
		if(m_iWritePos < 0)
			return NULL;
		else
			return &(m_pObjects[m_iWritePos]);
	}
	
	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡǰ���������ǰһ������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2008-7-30 12:22
	// ����˵��:  @pCurrObj�ο�����
	//            @iTimes��ʾ����
	// �� �� ֵ:  type*
	//////////////////////////////////////////////////////////////////////////
	type* getPrevObj(type* pCurrObj,int iTimes=1)
	{
		pCurrObj  -= iTimes;
		
		if(pCurrObj<m_pObjects)
			pCurrObj = (&(m_pObjects[m_iSize-1]))-(m_pObjects-pCurrObj);
		return pCurrObj;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ�����Ķ���ĺ�һ������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2009-3-4 21:27
	// ����˵��:  @pCurrObj�ο�����
	//            @iTimes��ʾ����
	// �� �� ֵ:  type*
	//////////////////////////////////////////////////////////////////////////
	type* getNextObj(type* pCurrObj,int iTimes=1)
	{
		pCurrObj += iTimes;
		if(pCurrObj < m_pObjects)
			pCurrObj = m_pObjects;
		else if(pCurrObj > &(m_pObjects[m_iSize-1]))
			pCurrObj = m_pObjects+(pCurrObj - &(m_pObjects[m_iSize-1]));
		return pCurrObj;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ�����Ķ���ĺ�һ������,ֱ����ǰдλ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2009-3-4 21:27
	// ����˵��:  @iCurrPosָ��λ��,��0��ʼ
	// �� �� ֵ:  ������λ��
	//////////////////////////////////////////////////////////////////////////
	int getNextPosUntilWrite(int iCurrPos)
	{
		iCurrPos ++;
		if(iCurrPos >= m_iSize)
			iCurrPos = 0;
		if((m_iWritePos == 0 && iCurrPos == m_iSize) || iCurrPos == m_iWritePos+1)
			return -1;//û���µļ�¼
		return iCurrPos;
	}

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��дָ���ƶ�����һ������λ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2008-5-23
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void moveNext()
	{
		P();
		if(m_iWritePos == m_iSize-1)
			m_iWritePos = 0;//�ۻ�
		else
			m_iWritePos ++;
		V();
	}
	
	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��дָ���ƶ�����һ������λ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2008-5-23
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void operator++()
	{
		moveNext();
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡָ���û��Ķ�λ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 22:47
	// ����˵��:  @reader���û�
	// �� �� ֵ:  λ��
	//////////////////////////////////////////////////////////////////////////
	int GetReaderPos(RING_READER_LIST reader)
	{
		return m_iaReaderPos[reader];
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ���µĶ�Ӧ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 22:48
	// ����˵��:  void
	// �� �� ֵ:  type*
	//////////////////////////////////////////////////////////////////////////
	type* getNewestObj()
	{
		if(m_iWritePos < 0)
			return NULL;
		if(m_iWritePos == 0)
			return &m_pObjects[m_iSize-1];
		else
			return &m_pObjects[m_iWritePos-1];
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ���µĶ�Ӧ��λ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 22:48
	// ����˵��:  void
	// �� �� ֵ:  λ��
	//////////////////////////////////////////////////////////////////////////
	int getNewestObjPos()
	{
		if(m_iWritePos < 0)
			return -1;
		if(m_iWritePos == 0)
			return m_iSize-1;
		else
			return m_iWritePos-1;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��һ���ɶ���Ԫ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 22:48
	// ����˵��:  @reader���û�
	// �� �� ֵ:  type*
	//////////////////////////////////////////////////////////////////////////
	type* getNextObjForRead(RING_READER_LIST reader)
	{
		P();
		type *p = NULL;
		
		if(m_iaReaderPos[reader] != m_iWritePos)
		{
			m_iaReaderPos[reader] ++;
			if(m_iaReaderPos[reader] == m_iSize)
				m_iaReaderPos[reader] = 0;
			p = &m_pObjects[m_iaReaderPos[reader]];
		}
		V();
		return p;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��ǰ���û���λ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 22:49
	// ����˵��:  @reader���û�
	// �� �� ֵ:  λ��
	//////////////////////////////////////////////////////////////////////////
	int getReaderPos(RING_READER_LIST reader)
	{
		return m_iaReaderPos[reader];
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ȡ��ǰ���û���λ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2009-1-14 18:07
	// ����˵��:  @reader���û�
	// �� �� ֵ:  λ��
	//////////////////////////////////////////////////////////////////////////
	int getCurrentReaderPos(RING_READER_LIST reader)
	{
		return m_iaReaderPos[reader];
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���õ�ǰ���û���λ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2009-1-14 18:07
	// ����˵��:  @reader���û�
	//            @pos��λ��
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void setCurrentReaderPos(RING_READER_LIST reader, int pos)
	{
		m_iaReaderPos[reader] = pos;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡָ�����û�����һ��Ԫ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 22:50
	// ����˵��:  @reader���û�
	// �� �� ֵ:  type*
	//////////////////////////////////////////////////////////////////////////
	type * operator[](RING_READER_LIST reader)
	{
		return getNextObjForRead(reader);
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 22:50
	// ����˵��:  void
	// �� �� ֵ:  ����
	//////////////////////////////////////////////////////////////////////////
	int GetSize()
	{
		return m_iSize;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��id��Ԫ�ص�ָ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 22:51
	// ����˵��:  idΪ��ţ���0��ʼ���
	// �� �� ֵ:  type*
	//////////////////////////////////////////////////////////////////////////
	type* GetAt(int id)
	{
		if(id < 0 || id >= m_iSize)
			return NULL;
		return &(m_pObjects[id]);
	}

private:
	type *m_pObjects;
	int m_iSize;//Ԫ����Ŀ
	int m_iWritePos;//��һ��дԪ�ص�λ��
	int m_iaReaderPos[C_READER_COUNT];//10��ֻ���û���״̬
	DWORD m_ReaderFlag;//ֻ���û���ע��״̬����λ��ʾ��0��ʾδʹ�ã�1��ʾ��ʹ��
	int m_Flag;//0��ʾ����
};


#endif // !defined(AFX_SKTRING_H__12695A0C_F4BA_4DF7_9F35_17CD7703DDE1__INCLUDED_)
