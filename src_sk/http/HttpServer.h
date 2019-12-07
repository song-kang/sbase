/**
 *
 * �� �� �� : HttpServer.h
 * �������� : 2013-5-23 13:14
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : HTTP����ͨѶ������
 * �޸ļ�¼ : 
 *            $Log: $
 *
 **/



#if !defined(AFX_HttpServer_H__F3729FA5_6E26_44EE_BE52_C42C3855FEC4__INCLUDED_)
#define AFX_HttpServer_H__F3729FA5_6E26_44EE_BE52_C42C3855FEC4__INCLUDED_

#include "ysbase/YsService.h"


//�ͻ���ȡ�ļ�ʱһ�η��͵�����С
#define C_MAX_GETFILESIZE 65536
#define C_MAX_HTTPSVR_RECVBUF_SIZE 102400

class CUrlEncoding
{
public:
	CUrlEncoding();
	~CUrlEncoding();
	//��str����Ϊ��ҳ�е� GB2312 url encode ,Ӣ�Ĳ��䣬����˫�ֽ� ��%3D%AE%88
	YsString toUrlGB2312(char * str);
	YsString toUrlUTF8(char * str);
	char CharToInt(char ch);
	char StrToBin(char *str);
	YsString UrlGB2312Decode(YsString str);
	YsString UrlUTF8Decode(YsString str);
};

//////////////////////////////////////////////////////////////////////////
// ��    ��:  CHttpServerSession
// ��    ��:  �ۿ���
// ����ʱ��:  2013-5-23 13:26
// ��    ��:  
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
	YsString m_user;//��¼�û�ID
	YsString m_sLastURL;//�����ʵ�URL
	time_t m_iLastOperTime;//������ʱ�䣬����10���ӿ����˳���¼���رջỰ
	time_t m_iFirstTime;//���β���ʱ��
	time_t m_iLoginTime;//��¼ʱ��
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
	// ��    ��:  ȡHTTP���ַ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2013-5-24 14:00
	// ����˵��:  
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	void GetHttpHead(YsString &sHead,YsString sContentType="text/html");

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����HTMLͷ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2013-5-24 13:39
	// ����˵��:  @sTitleΪҳ�����
	// �� �� ֵ:  int
	//////////////////////////////////////////////////////////////////////////
	int SendHead(YsString sTitle);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����HTMLβ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2013-5-24 13:39
	// ����˵��:  @sTitleΪҳ�����
	// �� �� ֵ:  int
	//////////////////////////////////////////////////////////////////////////
	int SendTail();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����HTML���ݣ�ͬʱ����ַ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2013-5-24 13:40
	// ����˵��:  @sHtmlΪHTML�ַ���
	// �� �� ֵ:  int
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
// ��    ��:  CHttpServer
// ��    ��:  �ۿ���
// ����ʱ��:  2013-5-23 13:14
// ��    ��:  ͨѶ����
//////////////////////////////////////////////////////////////////////////
class CHttpServer : public YsService  
{
public:
	CHttpServer();
	virtual ~CHttpServer();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2013-5-23 16:40
	// ����˵��:  @port��ʾ�˿�,@ip��ʾ�����IP��ַ
	// �� �� ֵ:  true��ʾ���������ɹ�,false��ʾ��������ʧ��
	//////////////////////////////////////////////////////////////////////////
	virtual bool Start(int port,YsString ip="");

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ֹͣ����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2013-5-23 16:40
	// ����˵��:  void
	// �� �� ֵ:  true��ʾ����ֹͣ�ɹ�,false��ʾ����ֹͣʧ��
	//////////////////////////////////////////////////////////////////////////
	virtual bool Stop();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��¼�ص��麯��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2013-5-23 16:40
	// ����˵��:  @ip��¼�ͻ��˵�IP
	//            @port��¼�ͻ��˵Ķ˿�
	//            @sLoginHead��¼�ַ���
	// �� �� ֵ:  true��ʾ�����¼,false��ʾ�ܾ���¼
	//////////////////////////////////////////////////////////////////////////
	virtual bool OnLogin(YsString ip,int port){return true;};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �»Ự�ص��麯��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2013-5-23 11:56
	// ����˵��:  @pSessionΪ�´����ĻỰʵ��
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void OnNewSession(CHttpServerSession *pSession){};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����HTTP����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2013-5-23 13:42
	// ����˵��:  
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	void ProcessHttp(CHttpServerSession *pSession,YsString &sHttpRecvStr);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����ָ��URL���󣬷���HTML�ַ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2013-5-23 15:07
	// ����˵��:  
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	virtual bool ProcessUrl(CHttpServerSession *pSession,YsString sUrl,YsString &sHtml){return false;};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ɾ���Ự�Ļص��麯��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2013-5-23 10:07
	// ����˵��:  @pSessionΪ������ɾ���ĻỰʵ��
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void OnDeleteSession(CHttpServerSession *pSession){};

protected:	
	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����֡���߳�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2013-5-23 19:41
	// ����˵��:  this
	// �� �� ֵ:  NULL
	//////////////////////////////////////////////////////////////////////////
	static void* ThreadListen(void* lp);
	static void* ThreadLoop(void* lp);

	CHttpIpSession* GetIpSession(YsString ip);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����˻Ự�߳�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2013-5-23 22:20
	// ����˵��:  this
	// �� �� ֵ:  NULL
	//////////////////////////////////////////////////////////////////////////
	static void* ThreadService(void* lp);

private:
	YsString m_sServerIp;//������֡��IP���ձ�ʾȫ��IP��ַ֡��
	int m_iServerPort;//�������˿ں�
	YsSocket m_Socket;//�����֡����SOCKET
	YsPtrList<CHttpServerSession> m_Sessions;//�Ự
	YsPtrList<CHttpIpSession> m_IpSessions;//IP�Ự
};

#endif // !defined(AFX_HttpServer_H__F3729FA5_6E26_44EE_BE52_C42C3855FEC4__INCLUDED_)
