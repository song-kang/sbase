/**
 *
 * 文 件 名 : SObject.cpp
 * 创建日期 : 2007-10-26 19:09
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: 2011/08/31 08:43:33 $
 * 当前版本 : $Revision: 1.1 $
 * 功能描述 : 定义SObject基类，所有类均必须从其派生，用于检查对象地址是否4字节对齐
 * 修改记录 : 
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
 *            通迅记录分析装置项目首次提交
 *
 *            Revision 1.1  2008/09/01 11:38:28  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/01 08:58:02  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2007/10/29 07:30:23  shaokt
 *            添加对象地址是否为4的倍数的检测函数，为检测ARM下由于pack(1)造成的内存问题
 *
 *
 **/


#include "SObject.h"

void SObjectWarn()
{
	//在这里放置断点查找对齐错误
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
