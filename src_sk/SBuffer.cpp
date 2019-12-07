/**
 *
 * �� �� �� : SBuffer.cpp
 * �������� : 2009-8-4 10:56
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: 2011/08/31 08:43:33 $
 * ��ǰ�汾 : $Revision: 1.1 $
 * �������� : �ɶ��û���ȡ�Ĳ������ڴ滺������
 * �޸ļ�¼ : 
 *            $Log: SBuffer.cpp,v $
 *            Revision 1.1  2011/08/31 08:43:33  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2011/02/25 03:12:05  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.5  2011/01/17 01:24:28  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.4  2010/12/23 01:38:17  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2010/12/20 08:38:28  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2010/10/29 08:48:41  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2010/08/09 03:04:43  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2009/09/15 02:14:06  shaokt
 *            ͨѸ��¼����װ����Ŀ�״��ύ
 *
 *
 **/


#include "SBuffer.h"


SBuffer::SBuffer()
{
	m_pBuffer = NULL;
	m_ReaderFlag = 0;
	memset(m_iaReaderPos,0,sizeof(m_iaReaderPos));
	m_bShared = true;
}

SBuffer::SBuffer(int iBufferSize)
{
	m_pBuffer = NULL;
	m_ReaderFlag = 0;
	memset(m_iaReaderPos,0,sizeof(m_iaReaderPos));
	init(iBufferSize);
}

SBuffer::~SBuffer()
{
	if(m_pBuffer != NULL)
		delete[] m_pBuffer;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ʼ��������
// ��    ��:  �ۿ���
// ����ʱ��:  2009-8-4 11:00
// ����˵��:  
// �� �� ֵ:  
//////////////////////////////////////////////////////////////////////////
bool SBuffer::init(int iBufferSize)
{
	if(m_pBuffer != NULL)
		delete[] m_pBuffer;
	m_pBuffer = new BYTE[iBufferSize];
	if(m_pBuffer == NULL)
	{
		printf("@@@ ERROR @@@ Memory out! SBuffer::Init(%d)",iBufferSize);
		return false;
	}
	m_iBufferSize = iBufferSize;
	m_iWritePoint = 0;
	//������Ϊ�գ����пռ���Ϊ������
	stuBufferHeader *pHead = (stuBufferHeader*)m_pBuffer;
#ifdef YS_BUFFER_DEBUG
	pHead->flag = 0xeb905372;
#endif
	pHead->iLength = 0;
	pHead->iFreeLength = m_iBufferSize-sizeof(stuBufferHeader);
	return true;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ���е�ֻ���û���û�п��з���BUFFER_READER_NONE
// ��    ��:  �ۿ���
// ����ʱ��:  2008-9-2 17:26
// ����˵��:  
// �� �� ֵ:  
//////////////////////////////////////////////////////////////////////////
BUFFER_READER_LIST SBuffer::getIdleReader()
{
	DWORD d;
	static bool bDoing = false;//���ڼ򵥻���
	while(bDoing);
	bDoing = true;
	for(int i=0;i<C_BUFFER_READER_COUNT;i++)
	{
		d = m_ReaderFlag;
		d = d >> i;
		if((d&1) == 0)
		{
			//д��ʶ
			DWORD d0=(1 << i);
			m_ReaderFlag |= d0;
			m_iaReaderPos[i] = m_iWritePoint;//��ָ���û�д�뵱ǰ���ݵ�λ��
			bDoing = false;
			return (BUFFER_READER_LIST)i;
		}
	}
	bDoing = false;
	return BUFFER_READER_NONE;
}

void SBuffer::releaseIdleReader(BUFFER_READER_LIST reader)
{
	DWORD d;
	d = ~(1 << reader);
	m_ReaderFlag &= d;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ����һ���µ����ݿ�
// ��    ��:  �ۿ���
// ����ʱ��:  2009-8-4 11:12
// ����˵��:  
// �� �� ֵ:  
//////////////////////////////////////////////////////////////////////////
bool SBuffer::append(BYTE *pBuf,int iLen,BYTE *pBuf2,int iLen2)
{
	if(m_bShared)
		m_Lock.lock();
	stuBufferHeader *pNext,*pHead = (stuBufferHeader*)(&m_pBuffer[m_iWritePoint]);
	int iLenFull = iLen + iLen2 + sizeof(stuBufferHeader);
	int lastWritePoint = m_iWritePoint;//���������дλ��
	int iNextPoint;
	bool bWriteAll = false;
//printf("+(%d)",m_iWritePoint);
//printf("\n%d->",m_iWritePoint);
	while(1)
	{
		//��ǰλ�õĿ��������Ƿ��㹻��ŵ�ǰ���ݿ�
		if(/*pHead->iLength == 0 && */pHead->iFreeLength >= iLenFull)
		{
			memcpy((((BYTE*)pHead)+sizeof(stuBufferHeader)), pBuf,iLen);
			if(iLen2 > 0)
				memcpy((((BYTE*)pHead)+sizeof(stuBufferHeader)+iLen), pBuf2,iLen2);
			pHead->iLength = iLen+iLen2;
			pNext = (stuBufferHeader*)(((BYTE*)pHead) + pHead->iLength+sizeof(stuBufferHeader));
			#ifdef YS_BUFFER_DEBUG
			pNext->flag = 0xeb905372;
			#endif
			pNext->iLength = 0;//��һ��дλ��
			pNext->iFreeLength = pHead->iFreeLength - iLenFull;
			pHead->iFreeLength = 0;
			m_iWritePoint += pHead->iLength+sizeof(stuBufferHeader);
			if(m_iWritePoint >= m_iBufferSize)
				m_iWritePoint = 0;
		}
		else
		{
			//�ռ䲻�㣬��Ҫ�ϲ���һ����¼
			while(pHead->iFreeLength < iLenFull)
			{
				iNextPoint = m_iWritePoint + pHead->iLength + pHead->iFreeLength + sizeof(stuBufferHeader);
				if(iNextPoint == lastWritePoint)
				{
					//������ȫ���Ļ����������ж�λ�þ��Ѿ�ʧЧ
					bWriteAll = true;
				}
				if(iNextPoint >= m_iBufferSize)
					break;//����ĩβ���Ѿ�û�к������ݿ���
				pNext = (stuBufferHeader*)(&m_pBuffer[iNextPoint]);
				pHead->iFreeLength += pNext->iLength + pNext->iFreeLength + sizeof(stuBufferHeader);
			}
			if(pHead->iFreeLength < iLenFull)
			{
				//β���ռ䲻�㣬��Ҫ��ͷ��ʼ���,�ȳԵ�ͷ�����������ռ�
				printf("#");
				m_iWritePoint = 0;
				pHead = (stuBufferHeader*)(&m_pBuffer[m_iWritePoint]);
				pHead->iFreeLength += pHead->iLength;
				pHead->iLength = 0;
			}
			

			continue;
		}
		break;
	}



	//��ǰ�ӵ����һ��λ��
	iNextPoint = m_iWritePoint + pNext->iLength + pNext->iFreeLength + sizeof(stuBufferHeader);
	if(iNextPoint >= m_iBufferSize)
		iNextPoint = 0;
	if(bWriteAll)
	{
		//�Ƿ��ж�λ�ô��ڱ���������
		for(int i=0;i<C_BUFFER_READER_COUNT;i++)
		{
			//׼����λ����Ҫ����
			m_iaReaderPos[i] = iNextPoint;
		}
	}
	else if(iNextPoint > lastWritePoint)
	{
		//�Ƿ��ж�λ�ô��ڱ���������
		for(int i=0;i<C_BUFFER_READER_COUNT;i++)
		{
			//�����ĳ����λ��λ�ڵ�ǰ׼��д��λ�õģ����뽫�������ƶ�
			if(m_iaReaderPos[i] > lastWritePoint && m_iaReaderPos[i] < iNextPoint)
			{
				//׼����λ����Ҫ����
				m_iaReaderPos[i] = iNextPoint;
			}
		}
	}
	else
	{
		//�Ƿ��ж�λ�ô��ڱ���������
		for(int i=0;i<C_BUFFER_READER_COUNT;i++)
		{
			//�����ĳ����λ��λ�ڵ�ǰ׼��д��λ�õģ����뽫�������ƶ�
			if(m_iaReaderPos[i] > lastWritePoint || m_iaReaderPos[i] < iNextPoint)
			{
				//׼����λ����Ҫ����
				m_iaReaderPos[i] = iNextPoint;
			}
		}
	}

	if(m_bShared)
		m_Lock.unlock();

	return true;
}

bool SBuffer::getNextData(BUFFER_READER_LIST reader,BYTE* pBuffer,int &iLen)
{
	//######����쳣�ж�(�����)
	if (reader==BUFFER_READER_NONE)
	{
		return false;
	}
	if(m_bShared)
		m_Lock.lock();
	int pos = m_iaReaderPos[reader];
	stuBufferHeader *p = (stuBufferHeader*)(&m_pBuffer[pos]);
	if(pos == m_iWritePoint)
	{
		//ȱ������
		if(m_bShared)
			m_Lock.unlock();
		return false;
	}
	while(p->iLength == 0)
	{
		if(pos == m_iWritePoint)
		{
			//ȱ������
			if(m_bShared)
				m_Lock.unlock();
			return false;
		}
		//ֱ�����������ݣ��Ҳ�����дλ�õ����ݿ�
		pos += (p->iFreeLength+p->iLength+sizeof(stuBufferHeader));
		if(pos >= m_iBufferSize)
		{
			p = (stuBufferHeader*)m_pBuffer;
			pos = 0;
		}
		else
		{
			p = (stuBufferHeader*)(&m_pBuffer[pos]);
		}
	}
	if(p->iLength > iLen)
	{
		//��ǰ�������Ų���
		iLen = p->iLength;
		pos += p->iLength + p->iFreeLength + sizeof(stuBufferHeader);
		if(pos >= m_iBufferSize)
			pos = 0;
		m_iaReaderPos[reader] = pos;
		if(m_bShared)
			m_Lock.unlock();
		return false;
	}
	iLen = p->iLength;
	memcpy(pBuffer,&m_pBuffer[pos+sizeof(stuBufferHeader)],iLen);
	pos += p->iLength + p->iFreeLength + sizeof(stuBufferHeader);
	if(pos >= m_iBufferSize)
		pos = 0;
	m_iaReaderPos[reader] = pos;
	if(m_bShared)
		m_Lock.unlock();
	return true;

}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ������е�����
// ��    ��:  �ۿ���
// ����ʱ��:  2010-10-29 16:04
// ����˵��:  
// �� �� ֵ:  
//////////////////////////////////////////////////////////////////////////
bool SBuffer::clear()
{
	if(m_bShared)
		m_Lock.lock();
	if(m_pBuffer == NULL)
	{
		if(m_bShared)
			m_Lock.unlock();
		return false;
	}
	//���ע���û�

	m_ReaderFlag = 0;
	memset(m_iaReaderPos,0,sizeof(m_iaReaderPos));
	m_iWritePoint = 0;
	//������Ϊ�գ����пռ���Ϊ������
	stuBufferHeader *pHead = (stuBufferHeader*)m_pBuffer;
	#ifdef YS_BUFFER_DEBUG
		pHead->flag = 0xeb905372;
	#endif
	pHead->iLength = 0;
	pHead->iFreeLength = m_iBufferSize-sizeof(stuBufferHeader);

	if(m_bShared)
		m_Lock.unlock();
	return true;
}
