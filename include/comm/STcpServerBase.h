/**
 *
 * �� �� �� : STcpServerBase.h
 * �������� : 2011-11-8 17:54
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : 
 * �޸ļ�¼ : 
 *            $Log: $
 *
 **/

#ifndef __SKT_TCPSERVER_BASE_H__
#define __SKT_TCPSERVER_BASE_H__

#include "STcpCommBase.h"

class STcpServerBase;

//////////////////////////////////////////////////////////////////////////
// ��    ��:  STcpServerSession
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-11 8:46
// ��    ��:  ����Ự��
//////////////////////////////////////////////////////////////////////////
class SBASE_EXPORT STcpServerSession : public STcpCommBase
{
public:
	friend class STcpServerBase;
	STcpServerSession();
	virtual ~STcpServerSession();

	//�����Ƿ���Ч
	bool IsValid(){return m_ValidWord==0xeb903469;};
	
	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �������ݵ����Ͷ��У��ڷ�����ɺ��ͷ�pPackage����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-10 12:40
	// ����˵��:  @pPackage�����ͱ���
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void AddToSend(stuSTcpPackage *pPackage);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �������ݰ��Ľ��ն���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-2-13 16:32
	// ����˵��:  @pPackage�����ͱ���
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void AddToRecv(stuSTcpPackage *pPackage);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ǰ�Ự�Զ���������һ֡���ģ���ͨ�����ͻ���������ֻ����OnRecvFrame������ʹ�ã������̲߳���ȫ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2012-01-17 13:46
	// ����˵��:  @pPackage��ʾ�����͵����ݰ�
	//            @bAutoDelPackage��ʾ�Ƿ���ʹ����Ϻ��Զ�ɾ��pPackage
	// �� �� ֵ:  true��ʾ���ͳɹ�,false��ʾ����ʧ��
	//////////////////////////////////////////////////////////////////////////
	bool SendFrame(stuSTcpPackage *pPackage,bool bAutoDelPackage=false);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ǰ�Ự�Զ���������һ֡���ģ���ͨ�����ͻ���������ֻ����OnRecvFrame������ʹ�ã������̲߳���ȫ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2012-01-17 13:57
	// ����˵��:  @sHead��ʾ����ͷ�ַ���
	//            @wFrameType��ʾ��������
	//            @pAsduBuffer��ʾASDU���ݻ���������pAsduBufferΪNULL��iAsduLen��Ϊ0ʱ����ʾʹ��Ԥ��ķ��ͻ�����
	//            @iAsduLen��ʾASDU���ݳ���
	// �� �� ֵ:  true��ʾ���ͳɹ�,false��ʾ����ʧ��
	//////////////////////////////////////////////////////////////////////////
	bool SendFrame(SString &sHead,WORD wFrameType=0,BYTE *pAsduBuffer=NULL,int iAsduLen=0);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ǰ�Ự�Զ���������һ֡���ģ���ͨ�����ͻ���������ֻ����OnRecvFrame������ʹ�ã������̲߳���ȫ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2012-01-17 13:57
	// ����˵��:  @wFrameType��ʾ��������
	//            @pAsduBuffer��ʾASDU���ݻ���������pAsduBufferΪNULL��iAsduLen��Ϊ0ʱ����ʾʹ��Ԥ��ķ��ͻ�����
	//            @iAsduLen��ʾASDU���ݳ���
	// �� �� ֵ:  true��ʾ���ͳɹ�,false��ʾ����ʧ��
	//////////////////////////////////////////////////////////////////////////
	bool SendFrame(WORD wFrameType,BYTE *pAsduBuffer=NULL,int iAsduLen=0);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �������ն����еı���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-10 21:32
	// ����˵��:  
	// �� �� ֵ:  stuSTcpPackage*
	//////////////////////////////////////////////////////////////////////////
	stuSTcpPackage* SearchRecvPackage();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ͨ������ͷ�ַ����е�act���Լ������ն����еı���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-10 21:32
	// ����˵��:  @act��ʾ����ı���
	// �� �� ֵ:  stuSTcpPackage*
	//////////////////////////////////////////////////////////////////////////
	stuSTcpPackage* SearchRecvPackage(SString act);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ͨ���������ͼ������ն����еı���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-10 21:32
	// ����˵��:  @wFrameType��ʾ
	// �� �� ֵ:  stuSTcpPackage*
	//////////////////////////////////////////////////////////////////////////
	stuSTcpPackage* SearchRecvPackage(WORD wFrameType);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �Ự����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-11 13:53
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	inline void lock(){m_Lock.lock();};

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �Ự����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-11 13:53
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	inline void unlock(){m_Lock.unlock();};

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �Ƿ�ͨѶ��������״̬
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-12 22:13
	// ����˵��:   void
	// �� �� ֵ:  true��ʾ������,false��ʾδ����
	//////////////////////////////////////////////////////////////////////////
	inline bool IsConnected(){return m_pSocket==NULL?false:m_pSocket->IsConnected();};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡSocketָ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2012-4-26 13:55
	// ����˵��:  void
	// �� �� ֵ:  SSocket*
	//////////////////////////////////////////////////////////////////////////
	inline SSocket* GetSessionSock(){return m_pSocket;}

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ���ͻ�����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2012-11-17 12:09
	// ����˵��:  void
	// �� �� ֵ:  SPtrList<stuSTcpPackage>*
	//////////////////////////////////////////////////////////////////////////
	inline SPtrList<stuSTcpPackage>* GetSendPackages(){return &m_SendPackage;};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��չ���Դ�ȡ֧��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-11-19 10:18
	// ����˵��:  
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	inline void SetAttributeValue(SString name, SString value){m_Attribute.SetAttribute(name,value);};
	inline void SetAttributeValue(SString name, int value){m_Attribute.SetAttribute(name,SString::toString(value));};
	inline void SetAttributeValue(SString name, float value){m_Attribute.SetAttribute(name,SString::toString(value));};
	inline SString GetAttributeValue(SString name){return m_Attribute.GetAttribute(name);};
	inline int GetAttributeValueI(SString name){return GetAttributeValue(name).toInt();};
	inline float GetAttributeValueF(SString name){return GetAttributeValue(name).toFloat();};

	inline void SetUserPtr(void* p){m_pUserPtr = p;};
	inline void* GetUserPtr(){return m_pUserPtr;};
	inline void SetUserPtr2(void* p){m_pUserPtr2 = p;};
	inline void* GetUserPtr2(){return m_pUserPtr2;};
	inline SString GetPeerIp(){return m_sPeerIp;};
	inline int GetPeerPort(){return m_iPeerPort;};
protected:
	DWORD m_ValidWord;//�Ƿ���Ч�ı����
	SLock m_Lock;
	STcpServerBase *m_pServer;//ָ��֡���������ָ��
	SSocket *m_pSocket;//�ỰSOCKET
	SPtrList<stuSTcpPackage> m_RecvPackage;//���յ������ݰ�
	SPtrList<stuSTcpPackage> m_SendPackage;//�����͵����ݰ�
	SString m_sLoginAttr;//��¼����,��: act=login;guesttype=htys.ca8110.steady;
	//��ʱ�м����
	int m_last_recv_time;//�����յ����ݵ�ʱ��
	int m_last_send_time;//��������ݵ�ʱ��
	int m_no_recv;//û���յ����ݵĴ���
	int m_no_send;//û�з������ݵĴ���
	SBaseConfig m_Attribute;
	void* m_pUserPtr;//�û�ָ��
	void* m_pUserPtr2;//�û�ָ��2
	SString m_sPeerIp;
	int m_iPeerPort;
};

//////////////////////////////////////////////////////////////////////////
// ��    ��:  CTcpClientBase
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-8 19:41
// ��    ��:  TCPͨѶ�ͻ��˷������
//////////////////////////////////////////////////////////////////////////
class SBASE_EXPORT STcpServerBase : public STcpCommBase, public SService
{
public:
	friend class STcpServerSession;

	STcpServerBase();
	virtual	~STcpServerBase();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-8 19:41
	// ����˵��:  @port��ʾ�˿�,@ip��ʾ�����IP��ַ
	// �� �� ֵ:  true��ʾ���������ɹ�,false��ʾ��������ʧ��
	//////////////////////////////////////////////////////////////////////////
	virtual bool Start(int port,SString ip="");

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ֹͣ����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-8 19:41
	// ����˵��:  void
	// �� �� ֵ:  true��ʾ����ֹͣ�ɹ�,false��ʾ����ֹͣʧ��
	//////////////////////////////////////////////////////////////////////////
	virtual bool Stop();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �������ݵ����Ͷ��У��ڷ�����ɺ��ͷ�pPackage����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-10 12:40
	// ����˵��:  @pPackage�����ͱ���
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void AddToSend(stuSTcpPackage *pPackage);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �������ն����еı���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-10 21:32
	// ����˵��:  @pSessionΪ���õĻỰָ��
	// �� �� ֵ:  stuSTcpPackage*
	//////////////////////////////////////////////////////////////////////////
	stuSTcpPackage* SearchRecvPackage(STcpServerSession* &pSession);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ͨ������ͷ�ַ����е�act���Լ������ն����еı���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-10 21:32
	// ����˵��:  @act��ʾ����ı���
	//            @pSessionΪ���õĻỰָ��
	// �� �� ֵ:  stuSTcpPackage*
	//////////////////////////////////////////////////////////////////////////
	stuSTcpPackage* SearchRecvPackage(SString act,STcpServerSession* &pSession);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ͨ���������ͼ������ն����еı���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-10 21:32
	// ����˵��:  @wFrameType��ʾ
	//            @pSessionΪ���õĻỰָ��
	// �� �� ֵ:  stuSTcpPackage*
	//////////////////////////////////////////////////////////////////////////
	stuSTcpPackage* SearchRecvPackage(WORD wFrameType,STcpServerSession* &pSession);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��¼�ص��麯��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-11 18:56
	// ����˵��:  @ip��¼�ͻ��˵�IP
	//            @port��¼�ͻ��˵Ķ˿�
	//            @sLoginHead��¼�ַ���
	// �� �� ֵ:  true��ʾ������¼,false��ʾ�ܾ���¼
	//////////////////////////////////////////////////////////////////////////
	virtual bool OnLogin(SString ip,int port,SString &sLoginHead){S_UNUSED(ip);S_UNUSED(port);S_UNUSED(sLoginHead);return true;};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �µ����ӳ�ʱ���󣬿ɽ�SOCKET����������;
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-2-5 17:17
	// ����˵��:  @pSocketΪSOCKETָ��
	// �� �� ֵ:  true��ʾSOCKET���ɹ��йܣ�Ӧ�ӷ����з��룬������Ҫ������ά����SOCKET����false��ʾ���йܣ�Ӧ�����ͷŸ�����
	//////////////////////////////////////////////////////////////////////////
	virtual bool OnNewTimeoutSocket(SSocket* &pSocket){S_UNUSED(pSocket);return false;};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �»Ự�ص��麯��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2012-6-1 11:56
	// ����˵��:  @pSessionΪ�´����ĻỰʵ��
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void OnNewSession(STcpServerSession *pSession){S_UNUSED(pSession);};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ɾ���Ự�Ļص��麯��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2013-3-21 10:07
	// ����˵��:  @pSessionΪ������ɾ���ĻỰʵ��
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void OnDeleteSession(STcpServerSession *pSession){S_UNUSED(pSession);};

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �Ự�߳̽��յ����ĺ�Ļص��麯����������ͨ���˺���������ʱ��Ϣ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-11 13:47
	// ����˵��:  @pSessionΪ�Ựʵ��ָ�룬����ͨ��ָ����Զ˷�������
	//            @pPackageΪ�ս��յ������ݰ�
	// �� �� ֵ:  true��ʾ�Ѿ�������ϣ��˱��Ŀ����ͷ��ˣ�false��ʾδ�������˱���Ӧ������ն���
	//////////////////////////////////////////////////////////////////////////
	virtual bool OnRecvFrame(STcpServerSession *pSession,stuSTcpPackage *pPackage)=0;

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��ǰ�����ж�Ӧ�ĻỰ����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2012-2-15 14:58
	// ����˵��:  void
	// �� �� ֵ:  int
	//////////////////////////////////////////////////////////////////////////
	inline int GetSessionCount(){return m_Sessions.count();};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��idx���Ựʵ��ָ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2012-4-5 15:07
	// ����˵��:  @idxΪ�Ự��ţ���0��ʼ
	// �� �� ֵ:  STcpServerSession*
	//////////////////////////////////////////////////////////////////////////
	inline STcpServerSession* GetSession(int idx){return m_Sessions[idx];};

	inline void SessionsLock(){m_Sessions.lock();};

	inline void SessionsUnLock(){m_Sessions.unlock();};

	inline SPtrList<STcpServerSession>* GetSessionPtr(){return &m_Sessions;};

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��������������ӵĻỰ������������ʱĬ��Ϊ100
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-12-10 13:42
	// ����˵��:  @sessionsΪ����������������
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void SetMaxSessionsAllowed(int sessions){ m_iMaxSessionsAllowed = sessions; };

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �Ƿ�Ϊÿ���Ự����һ��������ͨѶ�̣߳���Start֮ǰ����,Start֮�󲻿�����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-7-21 17:27
	// ����˵��:  @bMultiThread,true��ʾÿ���Ựһ���̣߳�false��ʾ���лỰ����һ���߳�
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void SetSessionOnThread(bool bMultiThread){m_bSessionOnThread = bMultiThread;};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���÷��ͻ������������������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-18 12:34
	// ����˵��:  
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	void SetSendBufferMaxPkgs(int pkgs){m_iSendBufferPkgs = pkgs;};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���ý��ջ������������������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-18 12:35
	// ����˵��:  
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	void SetRecvBufferMaxPkgs(int pkgs){m_iRecvBufferPkgs = pkgs;};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �жϵ�ǰ����˿��Ƿ��֡��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-11-18 10:51
	// ����˵��:  void
	// �� �� ֵ:  true��ʾ��֡����false��ʾδ��
	//////////////////////////////////////////////////////////////////////////
	inline bool IsListened(){return m_bListened;};
protected:
	
	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����֡���߳�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-8 19:41
	// ����˵��:  this
	// �� �� ֵ:  NULL
	//////////////////////////////////////////////////////////////////////////
	static void* ThreadListen(void* lp);


	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����˻Ự�߳�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-10 22:20
	// ����˵��:  this
	// �� �� ֵ:  NULL
	//////////////////////////////////////////////////////////////////////////
	static void* ThreadService(void* lp);

	static void* ThreadServiceAll(void* lp);

private:
	bool m_bListened;//�Ƿ��Ѿ������˿�֡��
	SString m_sServerIp;//������֡��IP���ձ�ʾȫ��IP��ַ֡��
	int m_iServerPort;//�������˿ں�
	int m_iSendBufferPkgs;//���ͻ���������������
	int m_iRecvBufferPkgs;//���ջ���������������
	SSocket m_Socket;//�����֡����SOCKET
	SPtrList<STcpServerSession> m_Sessions;//�Ự
	int m_iMaxSessionsAllowed;//���������ͻ�����������
	bool m_bSessionOnThread;//�Ƿ�Ϊÿ���Ự����һ��������ͨѶ�̣߳�true��ʾÿ���Ựһ���̣߳�false��ʾ���лỰ����һ���߳�
};

#endif//__SKT_TCPSERVER_BASE_H__