// HttpClient.h: interface for the CHttpClient class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HTTPCLIENT_H__59BC3CAC_28CA_4EB2_BEA9_C28BB62AAF87__INCLUDED_)
#define AFX_HTTPCLIENT_H__59BC3CAC_28CA_4EB2_BEA9_C28BB62AAF87__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ysbase/YsSocket.h"
#include "ysbase/YsApi.h"
#include "md5.h"

#define C_MAC_RECV_BUFFERSIZE 1024000

class CHttpClient  
{
public:
	CHttpClient();
	virtual ~CHttpClient();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2013-3-6 20:32
	// ����˵��:  @ip�����IP��ַ
	//            @port����˶˿ں�
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool Connect(YsString ip,int port);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����û�������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2013-3-6 20:32
	// ����˵��:  
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	void SetUserPwd(YsString user,YsString pwd)
	{
		m_user = user;
		m_password = pwd;
	}

	void Close();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  HTTP Get�����������ؽ���ַ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2013-3-2 13:02
	// ����˵��:  
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool HttpGet(YsString url,int &iResCode,YsString &sResHead,YsString &sResult);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����HTTPͷ�ַ��е�ָ���ַ����ص���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2013-3-6 9:03
	// ����˵��:  
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	YsString GetHttpHeadLine(YsString &sHead,YsString sLineBegin);

protected:
	YsSocket m_skt;
	YsString m_ip;
	int m_iPort;
	YsString m_user;
	YsString m_password;
	BYTE *m_pRecvBuffer;//���ջ�����
};

#endif // !defined(AFX_HTTPCLIENT_H__59BC3CAC_28CA_4EB2_BEA9_C28BB62AAF87__INCLUDED_)
