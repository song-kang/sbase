/**
 *
 * 文 件 名 : STcpClientBase.h
 * 创建日期 : 2011-11-8 17:54
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : TCP通讯客户端服务基类,有内置线程的服务模式和非服务模式两种，
              服务模块的Socket操作完全由服务线程接管，上层不能直接操作SOCKET，只能通过收发队列
			  由非服务模式只负责建立连接和登录服务端，然后由上层程序自行进行收发的SOCKET操作
 * 修改记录 : 
 *            $Log: $
 *
 **/




#ifndef __SKT_TCPCLIENT_BASE_H__
#define __SKT_TCPCLIENT_BASE_H__

#include "STcpCommBase.h"
#include "../SWait.h"

//////////////////////////////////////////////////////////////////////////
// 名    称:  CTcpClientBase
// 作    者:  邵凯田
// 创建时间:  2011-11-8 19:41
// 描    述:  TCP通讯客户端服务基类
//////////////////////////////////////////////////////////////////////////
class SBASE_EXPORT STcpClientBase : public STcpCommBase, public SService
{
public:
	STcpClientBase();
	virtual	~STcpClientBase();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  以服务形式启动客户端，不能与Connect函数同时使用
	// 作    者:  邵凯田
	// 创建时间:  2011-11-8 19:41
	// 参数说明:  @ip表示服务端IP地址，@port表示端口
	// 返 回 值:  true表示服务启动成功,false表示服务启动失败
	//////////////////////////////////////////////////////////////////////////
	virtual bool Start(SString ip,int port);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  停止服务
	// 作    者:  邵凯田
	// 创建时间:  2011-11-8 19:41
	// 参数说明:  void
	// 返 回 值:  true表示服务停止成功,false表示服务停止失败
	//////////////////////////////////////////////////////////////////////////
	virtual bool Stop();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  以非服务形式连接客户端，不能与Start函数同时使用
	// 作    者:  邵凯田
	// 创建时间:  2011-11-17 10:08
	// 参数说明:  @ip表示服务端IP地址，@port表示端口
	// 返 回 值:  true表示连接成功,false表示连接失败
	//////////////////////////////////////////////////////////////////////////
	bool Connect(SString ip,int port);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  登录上服务端后的回调
	// 作    者:  邵凯田
	// 创建时间:  2012-2-7 16:16
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void OnLogin(){};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  断开服务端后的回调
	// 作    者:  邵凯田
	// 创建时间:  2012-4-11 19:42
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void OnClose(){};

	////////////////////////////////////////////////////////////////////////
	// 描    述:  关闭连接,对应Connect函数
	// 作    者:  邵凯田
	// 创建时间:  2011-11-17 10:11
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void Close();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  查询当前通讯状态是否已经连接
	// 作    者:  邵凯田
	// 创建时间:  2011-11-10 21:38
	// 参数说明:  void
	// 返 回 值:  true表示已经连接,false表示未连接
	//////////////////////////////////////////////////////////////////////////
	inline bool IsConnected()	{return m_bConnected;};

	////////////////////////////////////////////////////////////////////////
	// 描    述:  添加数据到发送队列，在发送完成后释放pPackage对象，只能用于服务模式
	// 作    者:  邵凯田
	// 创建时间:  2011-11-10 12:40
	// 参数说明:  @pPackage待发送报文
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void AddToSend(stuSTcpPackage *pPackage);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  添加数据包到接收队列中，只能用于服务模式
	// 作    者:  邵凯田
	// 创建时间:  2012-2-3 14:01
	// 参数说明:  @pPackage为新接收到的数据包
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void AddToRecv(stuSTcpPackage *pPackage);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  通过报文头字符串中的act属性检索接收队列中的报文，只能用于服务模式
	// 作    者:  邵凯田
	// 创建时间:  2011-11-10 21:32
	// 参数说明:  @act表示待查的报文
	// 返 回 值:  stuSTcpPackage*
	//////////////////////////////////////////////////////////////////////////
	stuSTcpPackage* SearchRecvPackage(SString act,bool bWithLock=true);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  通过报文类型检索接收队列中的报文，只能用于服务模式
	// 作    者:  邵凯田
	// 创建时间:  2011-11-10 21:32
	// 参数说明:  @wFrameType表示
	//         :  @wFrameType2表示第二类型，0表示没有第二类型
	// 返 回 值:  stuSTcpPackage*
	//////////////////////////////////////////////////////////////////////////
	stuSTcpPackage* SearchRecvPackage(WORD wFrameType,WORD wFrameType2=0,bool bWithLock=true);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  设置客户端类型
	// 作    者:  邵凯田
	// 创建时间:  2011-11-11 10:13
	// 参数说明:  @str表示客户端登录信息
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void SetLoginInfo(SString str){m_sLoginInfo = str;};

	////////////////////////////////////////////////////////////////////////
	// 描    述:  发送一帧报文,服务模式下加锁运行，直连模式不加锁
	// 作    者:  邵凯田
	// 创建时间:  2011-11-10 12:31
	// 参数说明:  @pPackage表示待发送的数据包
	// 返 回 值:  true表示发送成功,false表示发送失败
	//////////////////////////////////////////////////////////////////////////
	bool SendFrame(stuSTcpPackage *pPackage);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  发送一帧报文,服务模式下加锁运行，直连模式不加锁
	// 作    者:  邵凯田
	// 创建时间:  2011-11-8 19:17
	// 参数说明:  @sHead表示报文头字符串
	//            @wFrameType表示报文类型
	//            @pAsduBuffer表示ASDU内容缓冲区，当pAsduBuffer为NULL且iAsduLen不为0时，表示使用预设的发送缓冲区
	//            @iAsduLen表示ASDU数据长度
	// 返 回 值:  true表示发送成功,false表示发送失败
	//////////////////////////////////////////////////////////////////////////
	bool SendFrame(SString &sHead,WORD wFrameType=0,BYTE *pAsduBuffer=NULL,int iAsduLen=0);


	////////////////////////////////////////////////////////////////////////
	// 描    述:  接收一个数据包到stuSTcpPackage指针中并引用返回新实例,,服务模式下加锁运行，直连模式不加锁
	// 作    者:  邵凯田
	// 创建时间:  2011-11-10 12:33
	// 参数说明:  @pPackage表示数据包引用指针，内部申请stuSTcpPackage对象，外部负责释放
	// 返 回 值:  <0表示接收失败，=0表示接收超时，>0表示接收成功且等于报文总长度
	//////////////////////////////////////////////////////////////////////////
	int RecvFrame(stuSTcpPackage* &pPackage);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  接收一个数据包到新缓冲区,服务模式下加锁运行，直连模式不加锁
	// 作    者:  邵凯田
	// 创建时间:  2011-11-8 19:18
	// 参数说明:  @sHead表示报文头字符串
	//            @wFrameType表示报文类型，引用返回
	//            @pAsduBuffer表示ASDU内容缓冲区，内部申请内容并引用返回
	//            @iAsduLen表示ASDU数据长度，引用返回
	// 返 回 值:  <0表示接收失败，=0表示接收超时，>0表示接收成功且等于报文总长度
	//////////////////////////////////////////////////////////////////////////
	//int RecvFrame(SString &sHead,WORD &wFrameType,BYTE *&pAsduBuffer,int &iAsduLen);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  发送并接收一帧报文，仅限使用于服务模式下，直连模式有接收队列可能会造成被接收线程收到队列中而直接接收得不到
	// 作    者:  邵凯田
	// 创建时间:  2011-11-17 13:37
	// 参数说明:  @pPackage表示待发送的数据包
	//            @pRecvPackage表示接收到的新包
	//            @iTimeout_s表示超时时间（单位为秒），0表示不超时未收到数据时一直等待
	//         :  @wRecvFrameType表示接收的类型号，0表示接收所有类型
	// 返 回 值:  true表示发送成功,false表示发送失败
	//////////////////////////////////////////////////////////////////////////
	bool SendAndRecv(stuSTcpPackage *pPackage,stuSTcpPackage* &pRecvPackage,int iTimeout_s=0,WORD wRecvFrameType=0);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  发送并接收一帧报文，仅限使用于服务模式下，直连模式有接收队列可能会造成被接收线程收到队列中而直接接收得不到
	// 作    者:  邵凯田
	// 创建时间:  2011-11-17 14:04
	// 参数说明:  @pRecvPackage表示接收到的新包
	//            @sHead表示报文头字符串
	//            @wFrameType表示报文类型，引用返回
	//            @pAsduBuffer表示ASDU内容缓冲区，内部申请内容并引用返回
	//            @iAsduLen表示ASDU数据长度，引用返回
	//            @iTimeout_s表示超时时间（单位为秒），0表示不超时未收到数据时一直等待
	//         :  @wRecvFrameType表示接收的类型号，0表示接收所有类型
	// 返 回 值:  true表示发收成功，false表示失败
	//////////////////////////////////////////////////////////////////////////
	bool SendAndRecv(stuSTcpPackage* &pRecvPackage,
					 SString &sHead,WORD wFrameType=0,BYTE *pAsduBuffer=NULL,int iAsduLen=0,int iTimeout_s=0,WORD wRecvFrameType=0);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  发送一帧报文，并从接收队列等待指定的回复报文
	// 作    者:  邵凯田
	// 创建时间:  2014-10-11 13:37
	// 参数说明:  @pPackage表示待发送的数据包
	//            @pRecvPackage表示接收到的新包
	//            @iTimeout_s表示超时时间（单位为秒），0表示不超时未收到数据时一直等待
	// 返 回 值:  true表示发送成功,false表示发送失败
	//////////////////////////////////////////////////////////////////////////
	bool SendAndSearchPkg(stuSTcpPackage *pPackage, stuSTcpPackage* &pRecvPackage, int iTimeout_s = 10);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  发送一帧报文，并从接收队列等待指定的回复报文
	// 作    者:  邵凯田
	// 创建时间:  2014-10-11 14:04
	// 参数说明:  @pRecvPackage表示接收到的新包
	//            @sHead表示报文头字符串
	//            @wFrameType表示报文类型，引用返回
	//            @pAsduBuffer表示ASDU内容缓冲区，内部申请内容并引用返回
	//            @iAsduLen表示ASDU数据长度，引用返回
	//            @iTimeout_s表示超时时间（单位为秒），0表示不超时未收到数据时一直等待
	// 返 回 值:  true表示发收成功，false表示失败
	//////////////////////////////////////////////////////////////////////////
	bool SendAndSearchPkg(stuSTcpPackage* &pRecvPackage,
		SString &sHead, WORD wFrameType = 0, BYTE *pAsduBuffer = NULL, int iAsduLen = 0, int iTimeout_s = 10);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  接收指定类型的报文,服务模式下加锁运行，直连模式不加锁,一般只在直连模式下使用
	// 作    者:  邵凯田
	// 创建时间:  2012-2-15 11:09
	// 参数说明:  @sFrameType表示待接收的报文类型
	//            @pRecvPackage表示接收报文指针，引用返回
	//            @iTimeout_s表示超时时间
	// 返 回 值:  >0表示收到目标报文，=0表示接收超时，<0表示失败
	//////////////////////////////////////////////////////////////////////////
	int RecvPackage(WORD sFrameType,stuSTcpPackage* &pRecvPackage,int iTimeout_s=5);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  加锁，不允许在OnRecvFrame中使用
	// 作    者:  邵凯田
	// 创建时间:  2012-2-2 14:31
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	inline void lock()
	{
		if (m_pLock != NULL)
			m_pLock->lock(); 
	};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  解锁
	// 作    者:  邵凯田
	// 创建时间:  2012-2-2 14:32
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	inline void unlock()
	{
		if (m_pLock != NULL)
			m_pLock->unlock();
	};

	inline void SetRecvWaitMode(bool bWaitMode)
	{
		if(bWaitMode)
		{
			if(m_pWait == NULL)
				m_pWait = new SWait();
		}
		else
		{
			if(m_pWait != NULL)
			{
				delete m_pWait;
				m_pWait = NULL;
			}
		}
	}
	inline void SetSendWaitMode(bool bWaitMode)
	{
		if(bWaitMode)
		{
			if(m_pSendWait == NULL)
				m_pSendWait = new SWait();
		}
		else
		{
			if(m_pSendWait != NULL)
			{
				delete m_pSendWait;
				m_pSendWait = NULL;
			}
		}
	}
	

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取锁状态
	// 作    者:  邵凯田
	// 创建时间:  2012-2-2 14:32
	// 参数说明:  void
	// 返 回 值:  0表示未锁定，>0表示锁定
	//////////////////////////////////////////////////////////////////////////
	inline int lock_status()
	{
		if (m_pLock != NULL)
			return m_pLock->get_value();
		else
			return 0;
	};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取SOCKET实例指针
	// 作    者:  邵凯田
	// 创建时间:  2012-2-7 16:18
	// 参数说明:  void
	// 返 回 值:  SSocket*
	//////////////////////////////////////////////////////////////////////////
	inline SSocket* GetSocket()	{return &m_Socket;};

	////////////////////////////////////////////////////////////////////////
	// 描    述:  会话线程接收到报文后的回调虚函数，派生类通过此函数处理即时消息
	// 作    者:  邵凯田
	// 创建时间:  2011-11-11 13:47
	// 参数说明:  @pSession为会话实例指针，可以通过指针向对端发送数据
	//            @pPackage为刚接收到的数据包
	// 返 回 值:  true表示已经处理完毕，此报文可以释放了，false表示未处理，此报文应放入接收队列
	//////////////////////////////////////////////////////////////////////////
	virtual bool OnRecvFrame(stuSTcpPackage *pPackage)=0;

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  设置服务启动时的连接服务端的模式
	// 作    者:  邵凯田
	// 创建时间:  2012-4-18 14:41
	// 参数说明:  @bConnectAtStart为true表示在启动服务时立即连接服务端，如果连接失败则返回失败，false表示由线程连接服务端
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void SetConnectMode(bool bConnectAtStart)
	{
		m_bConnectAtStart = bConnectAtStart;
	}

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取接收缓冲区队列指针
	// 作    者:  邵凯田
	// 创建时间:  2016-2-13 12:23
	// 参数说明:  void
	// 返 回 值:  SPtrList<stuSTcpPackage>*
	//////////////////////////////////////////////////////////////////////////
	inline SPtrList<stuSTcpPackage>* GetRecvPackagePtr(){return &m_RecvPackage;};
	inline SPtrList<stuSTcpPackage>* GetSendPackagePtr(){return &m_SendPackage;};
protected:
	
	////////////////////////////////////////////////////////////////////////
	// 描    述:  登录服务器
	// 作    者:  邵凯田
	// 创建时间:  2011-11-17 10:41
	// 参数说明:  void
	// 返 回 值:  true登录成功,false登录失败
	//////////////////////////////////////////////////////////////////////////
	bool Login();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  客户端通讯线程，负责链接维持与报文发送
	// 作    者:  邵凯田
	// 创建时间:  2011-11-8 19:41
	// 参数说明:  this
	// 返 回 值:  NULL
	//////////////////////////////////////////////////////////////////////////
	static void* ThreadClient(void* lp);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  客户端通讯线程，负责报文接收
	// 作    者:  邵凯田
	// 创建时间:  2014-9-28 15:41
	// 参数说明:  this
	// 返 回 值:  NULL
	//////////////////////////////////////////////////////////////////////////
	static void* ThreadRecv(void* lp);
private:
	SPtrList<stuSTcpPackage> m_RecvPackage;//接收到的数据包
	SPtrList<stuSTcpPackage> m_SendPackage;//待发送的数据包
	bool m_bConnected;//是否已经连接
	bool m_bServiceMode;//是否有服务模式，（服务模式为独立的收发线程，所有发送函数需要加锁）
	SString m_sServerIp;//服务端IP地址
	int m_iServerPort;//服务器端口号
	SString m_sLoginInfo;//客户端登录信息
	SSocket m_Socket;//客户端通讯SOCKET
	SLock *m_pLock;//锁
	SWait *m_pWait;//异步等待信号，用于接收缓冲区的异步触发
	SWait *m_pSendWait;//发送异常等待信号，用于发送缓冲区的异步触发
	bool m_bConnectAtStart;//true表示在启动服务时立即连接服务端，如果连接失败则返回失败，false表示由线程连接服务端
};

#endif//__SKT_TCPCLIENT_BASE_H__
