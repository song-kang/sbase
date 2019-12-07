/**
 *
 * �� �� �� : EscPrintCom.cpp
 * �������� : 2014-02-21 13:12
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : ESC(EPSON Standard Code for Printer)ָ����ڴ��ڵĴ�ӡ��
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2014-02-21  �ۿ���  ���ڴ�ӡ֧��
 *
 **/

#include "SEscPrintCom.h"


SEscPrintCom::SEscPrintCom()
{

}

SEscPrintCom::~SEscPrintCom()
{
	m_Com.Close();
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ӡ��ʼ��
// ��    ��:  �ۿ���
// ����ʱ��:  2014:2:21 13:46
// ����˵��:  @sAttrΪ��չ���ԣ���ͬʵ�����ṩ��ͬ�Ĳ���
// �� �� ֵ:  true��ʾ��ʼ���ɹ�,false��ʾ��ʼ��ʧ��
//////////////////////////////////////////////////////////////////////////
bool SEscPrintCom::Init(SString sAttr)
{
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �򿪴�ӡ��
// ��    ��:  �ۿ���
// ����ʱ��:  2014:2:21 13:23
// ����˵��:  @sAttrΪ��ӡ������
// �� �� ֵ:  true�򿪳ɹ���false��ʧ��
//////////////////////////////////////////////////////////////////////////
bool SEscPrintCom::Open(SString sAttr)
{
	SString dev = SString::GetAttributeValue(sAttr,"dev");
	int baud = SString::GetAttributeValueI(sAttr,"baud");
	if(baud == 0)
		baud = 19200;
	SString chk = SString::GetAttributeValue(sAttr,"chk");
	if(chk.length() == 0)
		chk = "N";
	if(!m_Com.Open(dev.data(),baud,chk.at(0)))
	{
		LOGERROR("�򿪴���ʱʧ��!attr=%s",sAttr.data());
		return false;
	}

	if(SEscPrint::Open(sAttr))
	{
		LOGDEBUG("���ڴ�ӡ���򿪳ɹ�!");
		return true;
	}
	else
	{
		LOGDEBUG("���ڴ�ӡ����ʧ��!");
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �رմ�ӡ��
// ��    ��:  �ۿ���
// ����ʱ��:  2014:2:21 13:26
// ����˵��:  void
// �� �� ֵ:  true�رճɹ���false�ر�ʧ��
//////////////////////////////////////////////////////////////////////////
bool SEscPrintCom::Close()
{
	m_Com.Close();
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ���ӡ��д����
// ��    ��:  �ۿ���
// ����ʱ��:  2014:2:21 13:31
// ����˵��:  @pBuffer��д�����ݻ�����ָ��
//            @iLenΪ�������ֽڳ���
// �� �� ֵ:  >0��ʾʵ�ʷ����ֽ�����<=0��ʾʧ��
//////////////////////////////////////////////////////////////////////////
int SEscPrintCom::Write(BYTE *pBuffer,int iLen)
{
	if(m_Com.GetHandle() == 0)
		return -1;
	int send_len = m_Com.Send(pBuffer,iLen);
	LOGDEBUG("SEND(len=%d): %s",iLen,SString::HexToStr(pBuffer,iLen).data());
	LOGDEBUG("SEND(len=%d): %s",iLen,SString::toString((char*)pBuffer,iLen).data());
	if (send_len < 0)
	{
		LOGERROR("�򴮿ڴ�ӡд����ʧ��:%d",send_len);
		return -2;
	}
#ifdef WIN32
	//	Sleep(10);
#else
	if(iLen == 1)
		usleep(100000);
	else if(iLen > 2000)
		usleep(300000);
	else 
		usleep(200000);
#endif


	return send_len;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �Ӵ�ӡ����ȡ����
// ��    ��:  �ۿ���
// ����ʱ��:  2014:2:21 13:35
// ����˵��:  @pBufferΪ�����뻺����ָ��
//            @iLenΪ������ȡ�ֽڳ���
// �� �� ֵ:  >0��ʾʵ�ʶ�ȡ�ֽ�����<0��ʾ��ȡʧ�ܣ�=0��ʾ��ȡ��ʱ
//////////////////////////////////////////////////////////////////////////
int SEscPrintCom::Read(BYTE *pBuffer,int iLen)
{
	int recv_len;
	recv_len = m_Com.Receive(pBuffer, iLen);
	if (recv_len < 0)
	{
		LOGERROR("�Ӵ��ڴ�ӡ������ʧ��:%d",recv_len);
		return -2;
	}

	return recv_len;
}





