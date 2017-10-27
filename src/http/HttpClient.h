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
	// 描    述:  连接
	// 作    者:  邵凯田
	// 创建时间:  2013-3-6 20:32
	// 参数说明:  @ip服务端IP地址
	//            @port服务端端口号
	// 返 回 值:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool Connect(YsString ip,int port);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  设置用户名密码
	// 作    者:  邵凯田
	// 创建时间:  2013-3-6 20:32
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	void SetUserPwd(YsString user,YsString pwd)
	{
		m_user = user;
		m_password = pwd;
	}

	void Close();

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  HTTP Get方法，并返回结果字符串
	// 作    者:  邵凯田
	// 创建时间:  2013-3-2 13:02
	// 参数说明:  
	// 返 回 值:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool HttpGet(YsString url,int &iResCode,YsString &sResHead,YsString &sResult);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  查找HTTP头字符中的指定字符开关的行
	// 作    者:  邵凯田
	// 创建时间:  2013-3-6 9:03
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	YsString GetHttpHeadLine(YsString &sHead,YsString sLineBegin);

protected:
	YsSocket m_skt;
	YsString m_ip;
	int m_iPort;
	YsString m_user;
	YsString m_password;
	BYTE *m_pRecvBuffer;//接收缓冲区
};

#endif // !defined(AFX_HTTPCLIENT_H__59BC3CAC_28CA_4EB2_BEA9_C28BB62AAF87__INCLUDED_)
