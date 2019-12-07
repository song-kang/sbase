/**
 *
 * �� �� �� : STcpCommBase.cpp
 * �������� : 2011-11-8 17:54
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : 
 * �޸ļ�¼ : 
 *            $Log: $
 *
 **/


#include "STcpCommBase.h"

//////////////////////////////////////////////////////////////////////////
// ��    ��:  STcpCommBase
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-8 19:41
// ��    ��:  TCPͨѶ����
//////////////////////////////////////////////////////////////////////////

STcpCommBase::STcpCommBase()
{
	m_pSendBuffer = NULL;//���ͻ����������ڷ��ͱ��ģ����ͷţ��������Ļ�����
	m_iSendBufferSize = 0;//���ͻ�������С
	m_wStartWord = 0x51EB;
	SKT_SWAP_WORD(m_wStartWord);
	m_iSendLenLimit = 0;//һ�η��͵����ƴ�С��������ִη���0��ʾ������
	m_iSendSleep = 0;//һ�η��ͺ������ʱ�䣬��λ΢�룬0��ʾ������

}

STcpCommBase::~STcpCommBase()
{
	if(m_pSendBuffer != NULL)
		delete[] m_pSendBuffer;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ����Э���������
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-8 17:55
// ����˵��:  @wStartWord��ʾ�����֣������ֽڣ�
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void STcpCommBase::SetStartWord(WORD wStartWord)
{
	m_wStartWord = wStartWord;
	SKT_SWAP_WORD(m_wStartWord);
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ����һ֡����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-10 12:31
// ����˵��:  @pS��ʾSOCKETʵ��
//            @pPackage��ʾ�����͵����ݰ�
// �� �� ֵ:  true��ʾ���ͳɹ�,false��ʾ����ʧ��
//////////////////////////////////////////////////////////////////////////
bool STcpCommBase::SendFrame(SSocket *pS,stuSTcpPackage *pPackage)
{
	return SendFrame(pS,pPackage->m_sHead,pPackage->m_wFrameType,pPackage->m_pAsduBuffer,pPackage->m_iAsduLen);
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ����һ���µı���
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-11 13:59
// ����˵��:  @sHead��ʾ����ͷ�ַ���
//            @wFrameType��ʾ��������
//            @pAsduBuffer��ʾASDU���ݻ�����
//            @iAsduLen��ʾASDU���ݳ���
//            @bCloneBuffer��ʾ�Ƿ��¡
// �� �� ֵ:  stuSTcpPackage*
//////////////////////////////////////////////////////////////////////////
stuSTcpPackage* STcpCommBase::NewPackage(SString &sHead,WORD wFrameType/*=0*/,BYTE *pAsduBuffer/*=NULL*/,
										   int iAsduLen/*=0*/,bool bCloneBuffer/*=true*/)
{
	stuSTcpPackage *p = new stuSTcpPackage();
	p->m_sHead = sHead;
	p->m_wFrameType = wFrameType;
	if(pAsduBuffer != NULL && iAsduLen != 0)
	{
		if(bCloneBuffer)
		{
			p->m_pAsduBuffer = new BYTE[iAsduLen];
			memcpy(p->m_pAsduBuffer,pAsduBuffer,iAsduLen);
		}
		else
		{
			p->m_pAsduBuffer = pAsduBuffer;
		}
		p->m_iAsduLen = iAsduLen;
	}
	return p;
}
stuSTcpPackage* STcpCommBase::NewPackage(char* sHead,WORD wFrameType/*=0*/,BYTE *pAsduBuffer/*=NULL*/,
										   int iAsduLen/*=0*/,bool bCloneBuffer/*=true*/)
{
	stuSTcpPackage *p = new stuSTcpPackage();
	if(sHead != NULL)
		p->m_sHead = sHead;
	p->m_wFrameType = wFrameType;
	if(pAsduBuffer != NULL && iAsduLen != 0)
	{
		if(bCloneBuffer)
		{
			p->m_pAsduBuffer = new BYTE[iAsduLen];
			memcpy(p->m_pAsduBuffer,pAsduBuffer,iAsduLen);
		}
		else
		{
			p->m_pAsduBuffer = pAsduBuffer;
		}
		p->m_iAsduLen = iAsduLen;
	}
	return p;
}
stuSTcpPackage* STcpCommBase::NewPackage(WORD wFrameType/*=0*/,BYTE *pAsduBuffer/*=NULL*/,
										   int iAsduLen/*=0*/,bool bCloneBuffer/*=true*/)
{
	stuSTcpPackage *p = new stuSTcpPackage();
	p->m_wFrameType = wFrameType;
	if(pAsduBuffer != NULL && iAsduLen != 0)
	{
		if(bCloneBuffer)
		{
			p->m_pAsduBuffer = new BYTE[iAsduLen];
			memcpy(p->m_pAsduBuffer,pAsduBuffer,iAsduLen);
		}
		else
		{
			p->m_pAsduBuffer = pAsduBuffer;
		}
		p->m_iAsduLen = iAsduLen;
	}
	return p;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ԭ���ĸ���һ���±���
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-11 19:50
// ����˵��:  @pPackageԭ����
// �� �� ֵ:  stuSTcpPackage*
//////////////////////////////////////////////////////////////////////////
stuSTcpPackage* STcpCommBase::NewPackage(stuSTcpPackage *pPackage)
{
	stuSTcpPackage *p = new stuSTcpPackage();
	p->m_sHead = pPackage->m_sHead;
	p->m_wFrameType = pPackage->m_wFrameType;
	if(pPackage->m_iAsduLen != 0)
	{
		p->m_pAsduBuffer = new BYTE[pPackage->m_iAsduLen];
		memcpy(p->m_pAsduBuffer,pPackage->m_pAsduBuffer,pPackage->m_iAsduLen);
		p->m_iAsduLen = pPackage->m_iAsduLen;
	}
	return p;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ����һ֡����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-8 19:17
// ����˵��:  @pS��ʾSOCKETʵ��
//            @sHead��ʾ����ͷ�ַ���
//            @wFrameType��ʾ��������
//            @pAsduBuffer��ʾASDU���ݻ���������pAsduBufferΪNULL��iAsduLen��Ϊ0ʱ����ʾʹ��Ԥ��ķ��ͻ�����
//            @iAsduLen��ʾASDU���ݳ���
// �� �� ֵ:  true��ʾ���ͳɹ�,false��ʾ����ʧ��
//////////////////////////////////////////////////////////////////////////
bool STcpCommBase::SendFrame(SSocket *pS,SString &sHead,WORD wFrameType/*=0*/,BYTE *pAsduBuffer/*=NULL*/,int iAsduLen/*=0*/)
{
	if(iAsduLen < 0 )
		return false;
	int slen = sHead.length();
	int len = iAsduLen + slen + 10;//�ܳ���
	if(len > m_iSendBufferSize)
	{
		if(m_iSendBufferSize > 0)
			delete[] m_pSendBuffer;
		m_pSendBuffer = new BYTE[len];
		m_iSendBufferSize = len;
	}
	WORD w;
	DWORD dw;
	w = sHead.length();
	SKT_SWAP_WORD(w);
	memcpy(m_pSendBuffer,&m_wStartWord,2);
	memcpy(m_pSendBuffer+2,&w,2);
	dw = iAsduLen;
	SKT_SWAP_DWORD(dw);
	memcpy(m_pSendBuffer+4,&dw,4);
	SKT_SWAP_WORD(wFrameType);
	memcpy(m_pSendBuffer+8,&wFrameType,2);
	if(slen > 0)
		memcpy(m_pSendBuffer+10,sHead.data(),slen);
	if(pAsduBuffer != NULL && iAsduLen > 0)
		memcpy(m_pSendBuffer+10+slen,pAsduBuffer,iAsduLen);

	int ret;
	if(m_iSendLenLimit == 0)
	{
		ret = pS->Send(m_pSendBuffer,len);
		if(ret != len)
		{
			LOGWARN("STcpCommBase::SendFrame error! len=%d,ret=%d",len,ret);
			return false;
		}
	}
	else
	{
		int pos = 0;
		while(pos < len)
		{
			slen = len - pos;
			if(slen > m_iSendLenLimit)
				slen = m_iSendLenLimit;
			ret = pS->Send(m_pSendBuffer+pos,slen);
			if(ret != slen)
			{
				LOGWARN("STcpCommBase::SendFrame error! len=%d,ret=%d",len,ret);
				return false;
			}
			pos += slen;
			if(m_iSendSleep != 0)
				SApi::UsSleep(m_iSendSleep);

		}
	}
	if(m_iSendSleep != 0)
		SApi::UsSleep(m_iSendSleep);
#ifdef _DEBUG
 	LOGDEBUG("SEND (len=%d,type=%d): sHead=%s, HEX=%s",10+slen+iAsduLen,wFrameType,sHead.left(1000).data(),
 		pAsduBuffer == NULL ? "" : (SString::HexToStr(pAsduBuffer, iAsduLen>100 ? 100 : iAsduLen).data()));
#endif
	return true;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ����һ֡����
// ��    ��:  �ۿ���
// ����ʱ��:  2012-01-17 14:01
// ����˵��:  @pS��ʾSOCKETʵ��
//            @wFrameType��ʾ��������
//            @pAsduBuffer��ʾASDU���ݻ���������pAsduBufferΪNULL��iAsduLen��Ϊ0ʱ����ʾʹ��Ԥ��ķ��ͻ�����
//            @iAsduLen��ʾASDU���ݳ���
// �� �� ֵ:  true��ʾ���ͳɹ�,false��ʾ����ʧ��
//////////////////////////////////////////////////////////////////////////
bool STcpCommBase::SendFrame(SSocket *pS,WORD wFrameType/*=0*/,BYTE *pAsduBuffer/*=NULL*/,int iAsduLen/*=0*/)
{
	if(iAsduLen < 0)
		return false;
	int len = iAsduLen + 10;//�ܳ���
	if(len > m_iSendBufferSize)
	{
		if(m_iSendBufferSize > 0)
			delete[] m_pSendBuffer;
		m_pSendBuffer = new BYTE[len];
		m_iSendBufferSize = len;
	}

	DWORD dw;
	memcpy(m_pSendBuffer,&m_wStartWord,2);
	memset(m_pSendBuffer+2,0,2);
	dw = iAsduLen;
	SKT_SWAP_DWORD(dw);
	memcpy(m_pSendBuffer+4,&dw,4);
	SKT_SWAP_WORD(wFrameType);
	memcpy(m_pSendBuffer+8,&wFrameType,2);
	if(pAsduBuffer != NULL && iAsduLen > 0)
		memcpy(m_pSendBuffer+10,pAsduBuffer,iAsduLen);

	int ret;
	if(m_iSendLenLimit == 0)
	{
		ret = pS->Send(m_pSendBuffer,len);
		if(ret != len)
		{
			LOGWARN("STcpCommBase::SendFrame error! len=%d,ret=%d",len,ret);
			return false;
		}
	}
	else
	{
		int slen,pos = 0;
		while(pos < len)
		{
			slen = len - pos;
			if(slen > m_iSendLenLimit)
				slen = m_iSendLenLimit;
			ret = pS->Send(m_pSendBuffer+pos,slen);
			if(ret != slen)
			{
				LOGWARN("STcpCommBase::SendFrame error! len=%d,ret=%d",len,ret);
				return false;
			}
			pos += slen;
			if(m_iSendSleep != 0)
				SApi::UsSleep(m_iSendSleep);

		}
	}
	if(m_iSendSleep != 0)
		SApi::UsSleep(m_iSendSleep);
	/*
	int ret = pS->Send(m_pSendBuffer,len);
	if(ret != len)
	{
		LOGWARN("STcpCommBase::SendFrame error! len=%d,ret=%d",len,ret);
		return false;
	}
	*/
#ifdef _DEBUG
 	LOGDEBUG("SEND (len=%d,type=%d): sHead=%s, HEX=%s",10+iAsduLen,wFrameType,"",
 		pAsduBuffer == NULL ? "" : (SString::HexToStr(pAsduBuffer, iAsduLen>100 ? 100 : iAsduLen).data()));
#endif
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �������֡��֡ͷ�������ݣ�����֡Ϊ�����ֿ�ʼ��ȫ��TCP���ϵ��û�������
//            ��ͬ���ͱ��ĵ�һ�η���ʱ���ô˺�����仺�屨��ͷ����������ֻ�е����ȸı�ʱ
//            �޸ı��������ص�int*���ݼ���
// ��    ��:  �ۿ���
// ����ʱ��:  2012-4-20 9:00
// ����˵��:  @pBufferΪ����֡�Ļ�����ָ��
//            @wFrameTypeΪ��������
//            @iAsduLenΪASDU�ĳ���
// �� �� ֵ:  int*��ʾ��ǰ�������д��ASDU���ȵ�ָ�룬�޸ĺ���Ҫ����SKT_SWAP_DWORD�����ֽ���
//////////////////////////////////////////////////////////////////////////
int* STcpCommBase::FillFullFrameHead(BYTE *pBuffer,WORD wFrameType,int iAsduLen)
{
	memcpy(pBuffer,&m_wStartWord,2);
	
	memset(pBuffer+2,0,2);
	SKT_SWAP_DWORD(iAsduLen);
	memcpy(pBuffer+4,&iAsduLen,4);
	SKT_SWAP_WORD(wFrameType);
	memcpy(pBuffer+8,&wFrameType,2);

	return (int*)(pBuffer+4);
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  Ԥ�軺������С������ǰ���ͻ�������С���ڸ�����Сʱ����
// ��    ��:  �ۿ���
// ����ʱ��:  2014:4:26 10:51
// ����˵��:  @iStrSizeΪͷ�ַ�������
//         :  @iAsduSizeΪASDU����
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void STcpCommBase::SetSendBufferSize(int iStrSize,int iAsduSize)
{
	int size = GetFramePrefixLen()+iStrSize+iAsduSize;
	if(m_iSendBufferSize < size)
	{
		BYTE *p = m_pSendBuffer;
		m_pSendBuffer = new BYTE[size];
		if(p != NULL)
			delete[] p;
		m_iSendBufferSize = size;
	}
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡASDU�����巢�ͻ������е�ƫ��λ��
// ��    ��:  �ۿ���
// ����ʱ��:  2014:4:26 10:53
// ����˵��:  @iStrSizeΪ���ַ�������
// �� �� ֵ:  BYTE*
//////////////////////////////////////////////////////////////////////////
BYTE* STcpCommBase::GetAsduOffsetInBuffer(int iStrSize)
{
	if(m_pSendBuffer == NULL)
		return NULL;
	return m_pSendBuffer + GetFramePrefixLen() + iStrSize;
}


////////////////////////////////////////////////////////////////////////
// ��    ��:  ����һ�����ݰ���stuSTcpPackageָ���в����÷�����ʵ��
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-10 12:33
// ����˵��:  @pS��ʾSOCKETʵ��
//            @pPackage��ʾ���ݰ�����ָ�룬�ڲ�����stuSTcpPackage�����ⲿ�����ͷ�
// �� �� ֵ:  <0��ʾ����ʧ�ܣ�=0��ʾ���ճ�ʱ��>0��ʾ���ճɹ��ҵ��ڱ����ܳ���
//////////////////////////////////////////////////////////////////////////
int STcpCommBase::RecvFrame(SSocket *pS,stuSTcpPackage* &pPackage)
{
	if(pPackage == NULL)
		pPackage = new stuSTcpPackage();
	else
	{		
		if(pPackage->m_pAsduBuffer == NULL)
			pPackage->m_iAsduBufferLen = 0;
	}
	int ret;
	BYTE head[10];
	ret = pS->Receive(head,10);
	if(ret != 10)
	{
		if(ret == 0)
			return 0;
		LOGERROR("����ʧ��!��������10,ʵ�ʷ���=%d,sockerr=%d,HEX=%s",ret,pS->GetLastError(),ret<=0?"":SString::HexToStr((BYTE*)&head,ret).data());
		if(ret < 0)
			return -1;
		pS->Close();
#ifdef _DEBUG
		LOGDEBUG("RECV(len:%d):%s\n",ret,SString::HexToStr((BYTE*)&head,ret).data());
#endif
		return -1;
	}
	WORD w;
	DWORD dw;
	memcpy(&w,head,2);
	if(w != m_wStartWord)
	{
		LOGWARN("�յ���Ч����!����������ַ�%02X%02X",head[0],head[1]);
// #ifdef _DEBUG
// 		LOGDEBUG("RECV(len:%d):%s\n",ret,SString::HexToStr((BYTE*)&head,ret).data());
// #endif
// 		BYTE *pBuffer=new BYTE[1024];
// 		while(1)
// 		{
// 			ret = pS->Receive(pBuffer,1024);
// 			if(ret <= 0)
// 				break;
// #ifdef _DEBUG
// 			LOGDEBUG("RECV(len:%d):%s\n",ret,SString::HexToStr(pBuffer,ret).data());
// #endif
// 		}
// 		delete[] pBuffer;
		return -2;
	}
	memcpy(&w,&head[2],2);
	SKT_SWAP_WORD(w);
	int slen = w;
	memcpy(&dw,&head[4],4);
	SKT_SWAP_DWORD(dw);
	pPackage->m_iAsduLen = dw;
	if(slen < 0 )
	{
		LOGWARN("�յ���Ч����!����ĳ���:%d:%s",slen,pPackage->m_iAsduLen);
		return -3;
	}
	memcpy(&pPackage->m_wFrameType,&head[8],2);
	SKT_SWAP_WORD(pPackage->m_wFrameType);
	if(slen != 0)
	{
		pPackage->m_sHead.SetBuffer(slen+1);
		ret = pS->Receive(pPackage->m_sHead.data(),slen);
		if(ret > 0)
			(pPackage->m_sHead.data())[ret] = '\0';
		pPackage->m_sHead.PrepareStrLen();
		if(ret != slen)
		{
			LOGWARN("���ձ���ͷ�ַ���ʱʧ��!slen=%d,ret=%d",slen,ret);
			return -4;
		}
	}
	else
		pPackage->m_sHead = "";
	if(pPackage->m_iAsduLen != 0)
	{
		if(pPackage->m_iAsduLen > pPackage->m_iAsduBufferLen)
		{
			if((!pPackage->m_bFixBuffer || pPackage->m_bOverflow) && pPackage->m_pAsduBuffer != NULL)
				delete[] pPackage->m_pAsduBuffer;
			if(pPackage->m_bFixBuffer)
				pPackage->m_bOverflow = true;//�̶����������
			pPackage->m_iAsduBufferLen = pPackage->m_iAsduLen;
			pPackage->m_pAsduBuffer = new BYTE[pPackage->m_iAsduLen];
		}
		int len = 0;
		int timeout_times=0;
		while(len < pPackage->m_iAsduLen)
		{
			ret = pS->Receive(pPackage->m_pAsduBuffer+len,pPackage->m_iAsduLen-len);
			if(ret < 0)
			{
				pS->Close();
				LOGWARN("���ձ�����ʱʧ��!len=%d,ret=%d",pPackage->m_iAsduLen,ret);
				return -5;
			}
			else if(ret == 0)
			{
				timeout_times ++;
				if(timeout_times > 100)
				{
					pS->Close();
					LOGWARN("���ձ�����ʱʧ��!len=%d,ret=%d",pPackage->m_iAsduLen,ret);
					return -6;
				}
				SApi::UsSleep(100000);
			}
			len += ret;
		}
	}
#ifdef _DEBUG
 	LOGDEBUG("RECV (len:%d,type=%d,ret=%d):%s HEX:%s", pPackage->m_iAsduLen, pPackage->m_wFrameType,ret, pPackage->m_sHead.left(1000).data(),
 		pPackage->m_pAsduBuffer == NULL ? "" : (SString::HexToStr(pPackage->m_pAsduBuffer, pPackage->m_iAsduLen>100 ? 100 : pPackage->m_iAsduLen).data()));
#endif
	return 10+slen+pPackage->m_iAsduLen;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ����һ�����ݰ����»�����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-8 19:18
// ����˵��:  @pS��ʾSOCKETʵ��
//            @sHead��ʾ����ͷ�ַ���
//            @wFrameType��ʾ�������ͣ����÷���
//            @pAsduBuffer��ʾASDU���ݻ��������ڲ��������ݲ����÷���
//            @iAsduLen��ʾASDU���ݳ��ȣ����÷���
//            @pFixRefΪ�̶���������Ϣ��NULL��ʾ�ǹ̶�����
// �� �� ֵ:  <0��ʾ����ʧ�ܣ�=0��ʾ���ճ�ʱ��>0��ʾ���ճɹ��ҵ��ڱ����ܳ���
//////////////////////////////////////////////////////////////////////////
#if 0
int STcpCommBase::RecvFrame(SSocket *pS,SString &sHead,WORD &wFrameType,BYTE *&pAsduBuffer,int &iAsduLen,stuYsTcpPackageFixRef *pFixRef/*=NULL*/)
{
	pAsduBuffer = NULL;
	int ret;
	BYTE head[10];
	ret = pS->Receive(head,10);
	if(ret != 10)
	{
		LOGERROR("����ʧ��!��������10,ʵ�ʷ���=%d,sockerr=%d,HEX=%s",ret,pS->GetLastError(),ret<=0?"":SString::HexToStr((BYTE*)&head,ret).data());
		if(ret == 0)
			return 0;
		return -1;
	}
	WORD w;
	DWORD dw;
	memcpy(&w,head,2);
	if(w != m_wStartWord)
	{
		LOGWARN("�յ���Ч����!����������ַ�%02X%02X",head[0],head[1]);
		return -2;
	}
	memcpy(&w,&head[2],2);
	SKT_SWAP_WORD(w);
	int slen = w;
	memcpy(&dw,&head[4],4);
	SKT_SWAP_DWORD(dw);
	iAsduLen = dw;
	if(slen < 0 )
	{
		LOGWARN("�յ���Ч����!����ĳ���:%d:%s",slen,iAsduLen);
		return -3;
	}
	memcpy(&wFrameType,&head[8],2);
	SKT_SWAP_WORD(wFrameType);
	if(slen != 0)
	{
		sHead.SetBuffer(slen+1);
		ret = pS->Receive(sHead.data(),slen);
		pPackage->m_sHead.PrepareStrLen();
		if(ret != slen)
		{
			LOGWARN("���ձ���ͷ�ַ���ʱʧ��!slen=%d,ret=%d",slen,ret);
			return -4;
		}
	}
	else
		pPackage->m_sHead = "";
	if(iAsduLen != 0)
	{
		if(iAsduLen > m_iSendBufferSize)
		pAsduBuffer = new BYTE[iAsduLen];
		int len = 0;
		int timeout_times=0;
		while(len < iAsduLen)
		{
			ret = pS->Receive(pAsduBuffer+len,iAsduLen-len);
			if(ret < 0)
			{
				delete[] pAsduBuffer;
				pAsduBuffer = NULL;
				pS->Close();
				LOGWARN("���ձ�����ʱʧ��!len=%d,ret=%d",iAsduLen,ret);
				return -5;
			}
			else if(ret == 0)
			{
				timeout_times ++;
				if(timeout_times > 100)
				{
					delete[] pAsduBuffer;
					pAsduBuffer = NULL;
					pS->Close();
					LOGWARN("���ձ�����ʱʧ��!len=%d,ret=%d",iAsduLen,ret);
					return -6;
				}
				SApi::UsSleep(100000);
			}
			len += ret;
		}
	}
	return 10+slen+iAsduLen;
}
#endif

////////////////////////////////////////////////////////////////////////
// ��    ��:  
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-17 13:37
// ����˵��:  @pS��ʾSOCKETʵ��
//            @pPackage��ʾ�����͵����ݰ�
//            @pRecvPackage��ʾ���յ����°�
//            @iTimeout_s��ʾ��ʱʱ�䣨��λΪ�룩��0��ʾ����ʱδ�յ�����ʱһֱ�ȴ�
// �� �� ֵ:  true��ʾ���ͳɹ�,false��ʾ����ʧ��
//////////////////////////////////////////////////////////////////////////
bool STcpCommBase::SendAndRecv(SSocket *pS,stuSTcpPackage *pPackage,stuSTcpPackage* &pRecvPackage,int iTimeout_s/*=0*/)
{
	if(!SendFrame(pS,pPackage))
		return false;
	time_t now = ::time(NULL);
	while(pS->IsConnected())
	{
		SApi::UsSleep(100000);
		if(RecvFrame(pS,pRecvPackage)>0)
		{
			return true;
		}
		if(iTimeout_s > 0 && abs((int)(::time(NULL)-now)) > iTimeout_s)
			break;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-17 14:04
// ����˵��:  @pS��ʾSOCKETʵ��
//            @pRecvPackage��ʾ���յ����°�
//            @sHead��ʾ����ͷ�ַ���
//            @wFrameType��ʾ�������ͣ����÷���
//            @pAsduBuffer��ʾASDU���ݻ��������ڲ��������ݲ����÷���
//            @iAsduLen��ʾASDU���ݳ��ȣ����÷���
//            @iTimeout_s��ʾ��ʱʱ�䣨��λΪ�룩��0��ʾ����ʱδ�յ�����ʱһֱ�ȴ�
// �� �� ֵ:  true��ʾ���ճɹ���false��ʾʧ��
//////////////////////////////////////////////////////////////////////////
bool STcpCommBase::SendAndRecv(SSocket *pS,stuSTcpPackage* &pRecvPackage,
				 SString &sHead,WORD wFrameType,BYTE *pAsduBuffer,int iAsduLen,int iTimeout_s/*=0*/)
{
	if(!SendFrame(pS,sHead,wFrameType,pAsduBuffer,iAsduLen))
		return false;
	time_t now = ::time(NULL);
	while(pS->IsConnected())
	{
		SApi::UsSleep(100000);
		if(RecvFrame(pS,pRecvPackage)>0)
		{
			return true;
		}
		if(iTimeout_s > 0 && abs((int)(::time(NULL)-now)) > iTimeout_s)
			break;
	}
	return false;
}

