/**
 *
 * 文 件 名 : HttpServer.h
 * 创建日期 : 2013-5-23 13:14
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : HTTP简易通讯服务类
 * 修改记录 : 
 *            $Log: $
 *
 **/



#if !defined(AFX_HttpServer_H__F3729FA5_6E26_44EE_BE52_C42C3855FEC4__INCLUDED_)
#define AFX_HttpServer_H__F3729FA5_6E26_44EE_BE52_C42C3855FEC4__INCLUDED_

#include "ysbase/YsService.h"


//客户端取文件时一次发送的最大大小
#define C_MAX_GETFILESIZE 65536
#define C_MAX_HTTPSVR_RECVBUF_SIZE 102400

class CUrlEncoding
{
public:
	CUrlEncoding();
	~CUrlEncoding();
	//把str编码为网页中的 GB2312 url encode ,英文不变，汉字双字节 如%3D%AE%88
	YsString toUrlGB2312(char * str);
	YsString toUrlUTF8(char * str);
	char CharToInt(char ch);
	char StrToBin(char *str);
	YsString UrlGB2312Decode(YsString str);
	YsString UrlUTF8Decode(YsString str);
};

//////////////////////////////////////////////////////////////////////////
// 名    称:  CHttpServerSession
// 作    者:  邵凯田
// 创建时间:  2013-5-23 13:26
// 描    述:  
//////////////////////////////////////////////////////////////////////////
class CHttpServer;

class CHttpIpSession
{
public:
	CHttpIpSession()
	{

	}
	~CHttpIpSession()
	{

	}
	YsString m_ip;
	YsString m_user;//登录用户ID
	YsString m_sLastURL;//最后访问的URL
	time_t m_iLastOperTime;//最后操作时间，超过10分钟可以退出登录，关闭会话
	time_t m_iFirstTime;//初次操作时间
	time_t m_iLoginTime;//登录时间
};
class CHttpServerSession
{
public:
	CHttpServerSession()
	{
		m_pServer = NULL;
		m_pSocket = NULL;
	}
	~CHttpServerSession()
	{
		if(m_pSocket != NULL && m_pSocket->IsConnected())
			m_pSocket->Close();

		lock();
		if(m_pSocket != NULL)
			delete m_pSocket;
		unlock();
	}

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取HTTP字字符
	// 作    者:  邵凯田
	// 创建时间:  2013-5-24 14:00
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	void GetHttpHead(YsString &sHead,YsString sContentType="text/html");

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  发送HTML头
	// 作    者:  邵凯田
	// 创建时间:  2013-5-24 13:39
	// 参数说明:  @sTitle为页面标题
	// 返 回 值:  int
	//////////////////////////////////////////////////////////////////////////
	int SendHead(YsString sTitle);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  发送HTML尾部
	// 作    者:  邵凯田
	// 创建时间:  2013-5-24 13:39
	// 参数说明:  @sTitle为页面标题
	// 返 回 值:  int
	//////////////////////////////////////////////////////////////////////////
	int SendTail();

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  发送HTML内容，同时清除字符串
	// 作    者:  邵凯田
	// 创建时间:  2013-5-24 13:40
	// 参数说明:  @sHtml为HTML字符串
	// 返 回 值:  int
	//////////////////////////////////////////////////////////////////////////
	int SendHtml(YsString &sHtml);

	YsString GetParam(YsString name)
	{
		return YsString::GetAttributeValue(m_sParam,name,"=","&");
	}
	int GetParamI(YsString name)
	{
		return YsString::GetAttributeValueI(m_sParam,name,"=","&");
	}

	inline void lock(){m_Lock.lock();};
	inline void unlock(){m_Lock.unlock();};

	YsLock m_Lock;
	CHttpServer *m_pServer;
	YsSocket *m_pSocket;
	YsString m_sUrl,m_sParam;
	CHttpIpSession *m_pIpSession;
};

//////////////////////////////////////////////////////////////////////////
// 名    称:  CHttpServer
// 作    者:  邵凯田
// 创建时间:  2013-5-23 13:14
// 描    述:  通讯服务
//////////////////////////////////////////////////////////////////////////
class CHttpServer : public YsService  
{
public:
	CHttpServer();
	virtual ~CHttpServer();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  启动服务
	// 作    者:  邵凯田
	// 创建时间:  2013-5-23 16:40
	// 参数说明:  @port表示端口,@ip表示服务端IP地址
	// 返 回 值:  true表示服务启动成功,false表示服务启动失败
	//////////////////////////////////////////////////////////////////////////
	virtual bool Start(int port,YsString ip="");

	////////////////////////////////////////////////////////////////////////
	// 描    述:  停止服务
	// 作    者:  邵凯田
	// 创建时间:  2013-5-23 16:40
	// 参数说明:  void
	// 返 回 值:  true表示服务停止成功,false表示服务停止失败
	//////////////////////////////////////////////////////////////////////////
	virtual bool Stop();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  登录回调虚函数
	// 作    者:  邵凯田
	// 创建时间:  2013-5-23 16:40
	// 参数说明:  @ip登录客户端的IP
	//            @port登录客户端的端口
	//            @sLoginHead登录字符串
	// 返 回 值:  true表示允许登录,false表示拒绝登录
	//////////////////////////////////////////////////////////////////////////
	virtual bool OnLogin(YsString ip,int port){return true;};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  新会话回调虚函数
	// 作    者:  邵凯田
	// 创建时间:  2013-5-23 11:56
	// 参数说明:  @pSession为新创建的会话实例
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void OnNewSession(CHttpServerSession *pSession){};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  处理HTTP请求
	// 作    者:  邵凯田
	// 创建时间:  2013-5-23 13:42
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	void ProcessHttp(CHttpServerSession *pSession,YsString &sHttpRecvStr);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  处理指定URL请求，返回HTML字符
	// 作    者:  邵凯田
	// 创建时间:  2013-5-23 15:07
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	virtual bool ProcessUrl(CHttpServerSession *pSession,YsString sUrl,YsString &sHtml){return false;};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  删除会话的回调虚函数
	// 作    者:  邵凯田
	// 创建时间:  2013-5-23 10:07
	// 参数说明:  @pSession为即将将删除的会话实例
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void OnDeleteSession(CHttpServerSession *pSession){};

protected:	
	////////////////////////////////////////////////////////////////////////
	// 描    述:  服务端帧听线程
	// 作    者:  邵凯田
	// 创建时间:  2013-5-23 19:41
	// 参数说明:  this
	// 返 回 值:  NULL
	//////////////////////////////////////////////////////////////////////////
	static void* ThreadListen(void* lp);
	static void* ThreadLoop(void* lp);

	CHttpIpSession* GetIpSession(YsString ip);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  服务端会话线程
	// 作    者:  邵凯田
	// 创建时间:  2013-5-23 22:20
	// 参数说明:  this
	// 返 回 值:  NULL
	//////////////////////////////////////////////////////////////////////////
	static void* ThreadService(void* lp);

private:
	YsString m_sServerIp;//服务器帧听IP，空表示全部IP地址帧听
	int m_iServerPort;//服务器端口号
	YsSocket m_Socket;//服务端帧听用SOCKET
	YsPtrList<CHttpServerSession> m_Sessions;//会话
	YsPtrList<CHttpIpSession> m_IpSessions;//IP会话
};

#endif // !defined(AFX_HttpServer_H__F3729FA5_6E26_44EE_BE52_C42C3855FEC4__INCLUDED_)
