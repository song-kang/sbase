/**
 *
 * 文 件 名 : STcpServerBase.h
 * 创建日期 : 2011-11-8 17:54
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : 
 * 修改记录 : 
 *            $Log: $
 *
 **/

#ifndef __SKT_TCPSERVER_BASE_H__
#define __SKT_TCPSERVER_BASE_H__

#include "STcpCommBase.h"

class STcpServerBase;

//////////////////////////////////////////////////////////////////////////
// 名    称:  STcpServerSession
// 作    者:  邵凯田
// 创建时间:  2011-11-11 8:46
// 描    述:  服务会话类
//////////////////////////////////////////////////////////////////////////
class SBASE_EXPORT STcpServerSession : public STcpCommBase
{
public:
	friend class STcpServerBase;
	STcpServerSession();
	virtual ~STcpServerSession();

	//对象是否有效
	bool IsValid(){return m_ValidWord==0xeb903469;};
	
	////////////////////////////////////////////////////////////////////////
	// 描    述:  添加数据到发送队列，在发送完成后释放pPackage对象
	// 作    者:  邵凯田
	// 创建时间:  2011-11-10 12:40
	// 参数说明:  @pPackage待发送报文
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void AddToSend(stuSTcpPackage *pPackage);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  添加数据包的接收队列
	// 作    者:  邵凯田
	// 创建时间:  2016-2-13 16:32
	// 参数说明:  @pPackage待发送报文
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void AddToRecv(stuSTcpPackage *pPackage);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  向当前会话对端立即发送一帧报文（不通过发送缓冲区），只能在OnRecvFrame函数中使用，否则线程不安全
	// 作    者:  邵凯田
	// 创建时间:  2012-01-17 13:46
	// 参数说明:  @pPackage表示待发送的数据包
	//            @bAutoDelPackage表示是否在使用完毕后自动删除pPackage
	// 返 回 值:  true表示发送成功,false表示发送失败
	//////////////////////////////////////////////////////////////////////////
	bool SendFrame(stuSTcpPackage *pPackage,bool bAutoDelPackage=false);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  向当前会话对端立即发送一帧报文（不通过发送缓冲区），只能在OnRecvFrame函数中使用，否则线程不安全
	// 作    者:  邵凯田
	// 创建时间:  2012-01-17 13:57
	// 参数说明:  @sHead表示报文头字符串
	//            @wFrameType表示报文类型
	//            @pAsduBuffer表示ASDU内容缓冲区，当pAsduBuffer为NULL且iAsduLen不为0时，表示使用预设的发送缓冲区
	//            @iAsduLen表示ASDU数据长度
	// 返 回 值:  true表示发送成功,false表示发送失败
	//////////////////////////////////////////////////////////////////////////
	bool SendFrame(SString &sHead,WORD wFrameType=0,BYTE *pAsduBuffer=NULL,int iAsduLen=0);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  向当前会话对端立即发送一帧报文（不通过发送缓冲区），只能在OnRecvFrame函数中使用，否则线程不安全
	// 作    者:  邵凯田
	// 创建时间:  2012-01-17 13:57
	// 参数说明:  @wFrameType表示报文类型
	//            @pAsduBuffer表示ASDU内容缓冲区，当pAsduBuffer为NULL且iAsduLen不为0时，表示使用预设的发送缓冲区
	//            @iAsduLen表示ASDU数据长度
	// 返 回 值:  true表示发送成功,false表示发送失败
	//////////////////////////////////////////////////////////////////////////
	bool SendFrame(WORD wFrameType,BYTE *pAsduBuffer=NULL,int iAsduLen=0);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  检索接收队列中的报文
	// 作    者:  邵凯田
	// 创建时间:  2011-11-10 21:32
	// 参数说明:  
	// 返 回 值:  stuSTcpPackage*
	//////////////////////////////////////////////////////////////////////////
	stuSTcpPackage* SearchRecvPackage();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  通过报文头字符串中的act属性检索接收队列中的报文
	// 作    者:  邵凯田
	// 创建时间:  2011-11-10 21:32
	// 参数说明:  @act表示待查的报文
	// 返 回 值:  stuSTcpPackage*
	//////////////////////////////////////////////////////////////////////////
	stuSTcpPackage* SearchRecvPackage(SString act);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  通过报文类型检索接收队列中的报文
	// 作    者:  邵凯田
	// 创建时间:  2011-11-10 21:32
	// 参数说明:  @wFrameType表示
	// 返 回 值:  stuSTcpPackage*
	//////////////////////////////////////////////////////////////////////////
	stuSTcpPackage* SearchRecvPackage(WORD wFrameType);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  会话加锁
	// 作    者:  邵凯田
	// 创建时间:  2011-11-11 13:53
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	inline void lock(){m_Lock.lock();};

	////////////////////////////////////////////////////////////////////////
	// 描    述:  会话解锁
	// 作    者:  邵凯田
	// 创建时间:  2011-11-11 13:53
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	inline void unlock(){m_Lock.unlock();};

	////////////////////////////////////////////////////////////////////////
	// 描    述:  是否通讯处于连接状态
	// 作    者:  邵凯田
	// 创建时间:  2011-11-12 22:13
	// 参数说明:   void
	// 返 回 值:  true表示连接中,false表示未连接
	//////////////////////////////////////////////////////////////////////////
	inline bool IsConnected(){return m_pSocket==NULL?false:m_pSocket->IsConnected();};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取Socket指针
	// 作    者:  邵凯田
	// 创建时间:  2012-4-26 13:55
	// 参数说明:  void
	// 返 回 值:  SSocket*
	//////////////////////////////////////////////////////////////////////////
	inline SSocket* GetSessionSock(){return m_pSocket;}

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取发送缓冲区
	// 作    者:  邵凯田
	// 创建时间:  2012-11-17 12:09
	// 参数说明:  void
	// 返 回 值:  SPtrList<stuSTcpPackage>*
	//////////////////////////////////////////////////////////////////////////
	inline SPtrList<stuSTcpPackage>* GetSendPackages(){return &m_SendPackage;};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  扩展属性存取支持
	// 作    者:  邵凯田
	// 创建时间:  2015-11-19 10:18
	// 参数说明:  
	// 返 回 值:  
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
	DWORD m_ValidWord;//是否有效的标记字
	SLock m_Lock;
	STcpServerBase *m_pServer;//指向帧听服务类的指针
	SSocket *m_pSocket;//会话SOCKET
	SPtrList<stuSTcpPackage> m_RecvPackage;//接收到的数据包
	SPtrList<stuSTcpPackage> m_SendPackage;//待发送的数据包
	SString m_sLoginAttr;//登录属性,如: act=login;guesttype=htys.ca8110.steady;
	//临时中间变量
	int m_last_recv_time;//最后接收到数据的时间
	int m_last_send_time;//最后发送数据的时间
	int m_no_recv;//没有收到数据的次数
	int m_no_send;//没有发送数据的次数
	SBaseConfig m_Attribute;
	void* m_pUserPtr;//用户指针
	void* m_pUserPtr2;//用户指针2
	SString m_sPeerIp;
	int m_iPeerPort;
};

//////////////////////////////////////////////////////////////////////////
// 名    称:  CTcpClientBase
// 作    者:  邵凯田
// 创建时间:  2011-11-8 19:41
// 描    述:  TCP通讯客户端服务基类
//////////////////////////////////////////////////////////////////////////
class SBASE_EXPORT STcpServerBase : public STcpCommBase, public SService
{
public:
	friend class STcpServerSession;

	STcpServerBase();
	virtual	~STcpServerBase();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  启动服务
	// 作    者:  邵凯田
	// 创建时间:  2011-11-8 19:41
	// 参数说明:  @port表示端口,@ip表示服务端IP地址
	// 返 回 值:  true表示服务启动成功,false表示服务启动失败
	//////////////////////////////////////////////////////////////////////////
	virtual bool Start(int port,SString ip="");

	////////////////////////////////////////////////////////////////////////
	// 描    述:  停止服务
	// 作    者:  邵凯田
	// 创建时间:  2011-11-8 19:41
	// 参数说明:  void
	// 返 回 值:  true表示服务停止成功,false表示服务停止失败
	//////////////////////////////////////////////////////////////////////////
	virtual bool Stop();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  添加数据到发送队列，在发送完成后释放pPackage对象
	// 作    者:  邵凯田
	// 创建时间:  2011-11-10 12:40
	// 参数说明:  @pPackage待发送报文
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void AddToSend(stuSTcpPackage *pPackage);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  检索接收队列中的报文
	// 作    者:  邵凯田
	// 创建时间:  2011-11-10 21:32
	// 参数说明:  @pSession为引用的会话指针
	// 返 回 值:  stuSTcpPackage*
	//////////////////////////////////////////////////////////////////////////
	stuSTcpPackage* SearchRecvPackage(STcpServerSession* &pSession);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  通过报文头字符串中的act属性检索接收队列中的报文
	// 作    者:  邵凯田
	// 创建时间:  2011-11-10 21:32
	// 参数说明:  @act表示待查的报文
	//            @pSession为引用的会话指针
	// 返 回 值:  stuSTcpPackage*
	//////////////////////////////////////////////////////////////////////////
	stuSTcpPackage* SearchRecvPackage(SString act,STcpServerSession* &pSession);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  通过报文类型检索接收队列中的报文
	// 作    者:  邵凯田
	// 创建时间:  2011-11-10 21:32
	// 参数说明:  @wFrameType表示
	//            @pSession为引用的会话指针
	// 返 回 值:  stuSTcpPackage*
	//////////////////////////////////////////////////////////////////////////
	stuSTcpPackage* SearchRecvPackage(WORD wFrameType,STcpServerSession* &pSession);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  登录回调虚函数
	// 作    者:  邵凯田
	// 创建时间:  2011-11-11 18:56
	// 参数说明:  @ip登录客户端的IP
	//            @port登录客户端的端口
	//            @sLoginHead登录字符串
	// 返 回 值:  true表示允许登录,false表示拒绝登录
	//////////////////////////////////////////////////////////////////////////
	virtual bool OnLogin(SString ip,int port,SString &sLoginHead){S_UNUSED(ip);S_UNUSED(port);S_UNUSED(sLoginHead);return true;};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  新的连接超时请求，可将SOCKET用于其它用途
	// 作    者:  邵凯田
	// 创建时间:  2016-2-5 17:17
	// 参数说明:  @pSocket为SOCKET指针
	// 返 回 值:  true表示SOCKET被成功托管（应从服务中分离，不再需要服务类维护该SOCKET），false表示不托管，应立即释放该连接
	//////////////////////////////////////////////////////////////////////////
	virtual bool OnNewTimeoutSocket(SSocket* &pSocket){S_UNUSED(pSocket);return false;};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  新会话回调虚函数
	// 作    者:  邵凯田
	// 创建时间:  2012-6-1 11:56
	// 参数说明:  @pSession为新创建的会话实例
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void OnNewSession(STcpServerSession *pSession){S_UNUSED(pSession);};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  删除会话的回调虚函数
	// 作    者:  邵凯田
	// 创建时间:  2013-3-21 10:07
	// 参数说明:  @pSession为即将将删除的会话实例
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void OnDeleteSession(STcpServerSession *pSession){S_UNUSED(pSession);};

	////////////////////////////////////////////////////////////////////////
	// 描    述:  会话线程接收到报文后的回调虚函数，派生类通过此函数处理即时消息
	// 作    者:  邵凯田
	// 创建时间:  2011-11-11 13:47
	// 参数说明:  @pSession为会话实例指针，可以通过指针向对端发送数据
	//            @pPackage为刚接收到的数据包
	// 返 回 值:  true表示已经处理完毕，此报文可以释放了，false表示未处理，此报文应放入接收队列
	//////////////////////////////////////////////////////////////////////////
	virtual bool OnRecvFrame(STcpServerSession *pSession,stuSTcpPackage *pPackage)=0;

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取当前服务中对应的会话数量
	// 作    者:  邵凯田
	// 创建时间:  2012-2-15 14:58
	// 参数说明:  void
	// 返 回 值:  int
	//////////////////////////////////////////////////////////////////////////
	inline int GetSessionCount(){return m_Sessions.count();};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取第idx个会话实例指针
	// 作    者:  邵凯田
	// 创建时间:  2012-4-5 15:07
	// 参数说明:  @idx为会话序号，从0开始
	// 返 回 值:  STcpServerSession*
	//////////////////////////////////////////////////////////////////////////
	inline STcpServerSession* GetSession(int idx){return m_Sessions[idx];};

	inline void SessionsLock(){m_Sessions.lock();};

	inline void SessionsUnLock(){m_Sessions.unlock();};

	inline SPtrList<STcpServerSession>* GetSessionPtr(){return &m_Sessions;};

	////////////////////////////////////////////////////////////////////////
	// 描    述:  设置最大允许连接的会话数量，不设置时默认为100
	// 作    者:  邵凯田
	// 创建时间:  2014-12-10 13:42
	// 参数说明:  @sessions为允许并发连接数量
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void SetMaxSessionsAllowed(int sessions){ m_iMaxSessionsAllowed = sessions; };

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  是否为每个会话创建一个独立的通讯线程，在Start之前设置,Start之后不可设置
	// 作    者:  邵凯田
	// 创建时间:  2015-7-21 17:27
	// 参数说明:  @bMultiThread,true表示每个会话一个线程，false表示所有会话共用一个线程
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void SetSessionOnThread(bool bMultiThread){m_bSessionOnThread = bMultiThread;};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  设置发送缓冲区最大允许包数量
	// 作    者:  邵凯田
	// 创建时间:  2015-8-18 12:34
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	void SetSendBufferMaxPkgs(int pkgs){m_iSendBufferPkgs = pkgs;};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  设置接收缓冲区最大允许包数量
	// 作    者:  邵凯田
	// 创建时间:  2015-8-18 12:35
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	void SetRecvBufferMaxPkgs(int pkgs){m_iRecvBufferPkgs = pkgs;};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  判断当前服务端口是否打开帧听
	// 作    者:  邵凯田
	// 创建时间:  2015-11-18 10:51
	// 参数说明:  void
	// 返 回 值:  true表示打开帧听，false表示未打开
	//////////////////////////////////////////////////////////////////////////
	inline bool IsListened(){return m_bListened;};
protected:
	
	////////////////////////////////////////////////////////////////////////
	// 描    述:  服务端帧听线程
	// 作    者:  邵凯田
	// 创建时间:  2011-11-8 19:41
	// 参数说明:  this
	// 返 回 值:  NULL
	//////////////////////////////////////////////////////////////////////////
	static void* ThreadListen(void* lp);


	////////////////////////////////////////////////////////////////////////
	// 描    述:  服务端会话线程
	// 作    者:  邵凯田
	// 创建时间:  2011-11-10 22:20
	// 参数说明:  this
	// 返 回 值:  NULL
	//////////////////////////////////////////////////////////////////////////
	static void* ThreadService(void* lp);

	static void* ThreadServiceAll(void* lp);

private:
	bool m_bListened;//是否已经启动端口帧听
	SString m_sServerIp;//服务器帧听IP，空表示全部IP地址帧听
	int m_iServerPort;//服务器端口号
	int m_iSendBufferPkgs;//发送缓冲区的最大包数量
	int m_iRecvBufferPkgs;//接收缓冲区的最大包数量
	SSocket m_Socket;//服务端帧听用SOCKET
	SPtrList<STcpServerSession> m_Sessions;//会话
	int m_iMaxSessionsAllowed;//允许的最大客户端连接数量
	bool m_bSessionOnThread;//是否为每个会话创建一个独立的通讯线程，true表示每个会话一个线程，false表示所有会话共用一个线程
};

#endif//__SKT_TCPSERVER_BASE_H__
