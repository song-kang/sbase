/**
 *
 * �� �� �� : SObject.cpp
 * �������� : 2007-10-26 19:09
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: 2011/08/31 08:43:33 $
 * ��ǰ�汾 : $Revision: 1.1 $
 * �������� : ����SObject���࣬�����������������������ڼ������ַ�Ƿ�4�ֽڶ���
 * �޸ļ�¼ : 
 *            $Log: SObject.cpp,v $
 *            Revision 1.1  2011/08/31 08:43:33  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2011/02/25 03:12:07  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2010/08/09 03:04:43  shaokt
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
 *            Revision 1.1  2007/10/29 07:30:23  shaokt
 *            ��Ӷ����ַ�Ƿ�Ϊ4�ı����ļ�⺯����Ϊ���ARM������pack(1)��ɵ��ڴ�����
 *
 *
 **/


#include "SObject.h"

void SObjectWarn()
{
	//��������öϵ���Ҷ������
}


SConstBuffer::SConstBuffer()
{
	m_iBufferSize = 102400;
	m_pBuffer = m_pFirstBuffer = NULL;
}

SConstBuffer::~SConstBuffer()
{

}

void SConstBuffer::Clear()
{
	m_pBuffer = NULL;
	stuConstBuffer *p = m_pFirstBuffer;
	while(p)
	{
		m_pFirstBuffer = p->pNext;
		delete p;
		p = m_pFirstBuffer;
	}
	m_pFirstBuffer = NULL;
	m_pBuffer = NULL;
}

unsigned char* SConstBuffer::GetConstBuffer(int iLen)
{
	if(m_pBuffer == NULL)
	{
		if(iLen > m_iBufferSize)
			m_iBufferSize = (iLen << 2);
		m_pFirstBuffer = m_pBuffer = new stuConstBuffer(m_iBufferSize);
	}
	int iLeft = m_pBuffer->iBufferSize - (int)(m_pBuffer->pNextBuffer - m_pBuffer->pBuffer);
	if(iLeft <= iLen)
	{
		stuConstBuffer *pNew;
		if(iLen >= m_iBufferSize)
			pNew = new stuConstBuffer(iLen<<2);
		else
			pNew = new stuConstBuffer(m_iBufferSize);
		m_pBuffer->pNext = pNew;
		m_pBuffer = pNew;
	}
	unsigned char *p = m_pBuffer->pNextBuffer;
// 	memcpy(m_pBuffer->pNextBuffer,pStr,iStrLen);
// 	m_pBuffer->pNextBuffer[iLen] = '\0';
	m_pBuffer->pNextBuffer += iLen/*+1*/;
	return p;
}

unsigned char* SConstBuffer::CopyToConstBuffer(unsigned char *pSrcBuf,int iLen)
{	
	unsigned char *p = GetConstBuffer(iLen);
	memcpy(p,pSrcBuf,iLen);
	return p;
}

const char* SConstBuffer::CopyToConstString(char *pStr,int iLen)
{
	if(iLen == 0)
		iLen = (int)strlen(pStr)+1;
	else
		iLen++;
	const char* p = (const char*)GetConstBuffer(iLen);
	iLen --;
	memcpy((char*)p,pStr,iLen);
	((char*)p)[iLen] = '\0';
	return p;
}
