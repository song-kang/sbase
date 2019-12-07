/**
 *
 * 文 件 名 : SNtp.h
 * 创建日期 : 2015-9-6 22:12
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : NTP封装类
 * 修改记录 : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-9-6	邵凯田　创建文件
 *
 **/

#ifndef __SBASE_NTP_H__
#define __SBASE_NTP_H__

#include "SApi.h"
#include "SSocket.h"

#define SNTPSERVER_PORT 123
#define SNTPUSEC(x) (((x)>>12)-759*((((x)>>10)+32768)>>16))  //x/4294.967296
#define SNTPNTPFRAC(x) (4294*(x) + ((1981*(x))>>11))     //x*4294.967296
#define SNTPJAN_1970 0x83aa7e80
#pragma pack(1)

struct ntp_timeval32 {
	int    tv_sec;         /* seconds */
	int    tv_usec;        /* and microseconds */
};
struct stuSNTPPackage
	{//SNTP包结构
	unsigned char cLiVnMode;
	unsigned char cTratum;
	unsigned char cPoll;
	unsigned char cPrecision;
	int nRootDelay;
	int nRootDispersion;
	int nReferenceIdentifier;

	struct ntp_timeval32 tReferenceTimestamp;
	struct ntp_timeval32 tOriginateTimestamp;
	struct ntp_timeval32 tReceiveTimestamp;
	struct ntp_timeval32 tTransmitTimestamp;
};
#pragma pack(4)

class SBASE_EXPORT SNtpBase
{
public:
	SNtpBase()
	{

	}
	virtual ~SNtpBase()
	{

	}
	virtual void GetLocalTime(int &soc,int &usec)
	{
		SDateTime::getSystemTime(soc,usec);
	}
};

class SBASE_EXPORT SSntpClient : public SNtpBase
{
public:
	SSntpClient();
	virtual ~SSntpClient();

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  设置服务器参数
	// 作    者:  邵凯田
	// 创建时间:  2015-9-6 22:52
	// 参数说明:  @ip为SNTP服务器IP地址
	//         :  @port为UDP端口号，缺省为123
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void SetServer(SString ip,int port=SNTPSERVER_PORT)
	{
		m_sServerIp = ip;
		m_iPort = port;
	}

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  设置时间补偿值
	// 作    者:  邵凯田
	// 创建时间:  2015-9-6 22:53
	// 参数说明:  @sec为秒补偿
	//         :  @msec为毫秒补偿
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void SetTimeCompensate(int sec,int msec)
	{
		m_iSecondsAdd = sec;
		m_iMSecondsAdd = msec;
	}

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取当前时刻服务器上的时间
	// 作    者:  邵凯田
	// 创建时间:  2015-9-6 22:53
	// 参数说明:  @now为引用返回的当前SNTP服务器时间
	//         :  @bSetToSystem为true表示时间获取成功后需要写入系统，false表示不需要写入系统
	// 返 回 值:  true表示获取成功，false表示获取失败
	//////////////////////////////////////////////////////////////////////////
	bool GetTime(SDateTime &now,bool bSetToSystem=true);
private:
	SString m_sServerIp;
	int m_iPort;//123
	int m_iSecondsAdd;//秒补偿
	int m_iMSecondsAdd;//毫秒补偿
};

class SBASE_EXPORT SSntpServer : public SNtpBase
{
public:
	SSntpServer();
	virtual ~SSntpServer();
	//////////////////////////////////////////////////////////////////////////
	// 描    述:  设置服务器ip和端口（可以不调用，默认ip为空）
	// 作    者:  褚冰冰
	// 创建时间:  2015-9-24 14:20
	// 参数说明:  @ip为SNTP服务器绑定IP地址
	//         :  @port为UDP端口号，缺省为123
	//		   :  @
	// 返 回 值:  无
	//////////////////////////////////////////////////////////////////////////
	void SetServer(SString ip,int port=SNTPSERVER_PORT)
	{
		m_sServerIp = ip;
		m_iPort = port;
	}
	//////////////////////////////////////////////////////////////////////////
	// 描    述:  启动sntp校时服务
	// 作    者:  褚冰冰
	// 创建时间:  2015-9-24 14:21
	// 参数说明:  @
	// 返 回 值:  false 表示启动失败
	//////////////////////////////////////////////////////////////////////////
	bool StartServer();

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  停止sntp校时服务，可能会等待一段时间
	// 作    者:  褚冰冰
	// 创建时间:  2015-9-24 14:21
	// 参数说明:  @
	// 返 回 值:  true 表示停止成功
	//////////////////////////////////////////////////////////////////////////
	bool StopServer();

private:
	//////////////////////////////////////////////////////////////////////////
	// 描    述:  sntp校时服务端线程执行函数，循环接受客户端请求
	//            检测udp连接，失效会重新创建
	// 作    者:  褚冰冰
	// 创建时间:  2015-9-25 17:38
	// 参数说明:  @arg 为线程传递参数
	//         :  @
	//		   :  @
	// 返 回 值:  无
	//////////////////////////////////////////////////////////////////////////
	static void* funproc(void* arg);
	SString m_sServerIp;
	int m_iPort;//123
	bool m_bRun;
	bool m_bStoped;
	SSocket m_udp;
};

class SBASE_EXPORT SNtpClient : public SNtpBase
{
public:
	SNtpClient();
	virtual ~SNtpClient(); 
	//////////////////////////////////////////////////////////////////////////
	// 描    述:  设置请求服务器的ip和端口
	// 作    者:  褚冰冰
	// 创建时间:  2015-9-24 14:20
	// 参数说明:  @ip为NTP服务器IP地址
	//         :  @port为UDP端口号，缺省为123
	//		   :  @
	// 返 回 值:  无
	//////////////////////////////////////////////////////////////////////////
	void SetServer(SString ip,int port=SNTPSERVER_PORT)
	{
		m_sServerIp = ip;
		m_iPort = port;
	}
	//////////////////////////////////////////////////////////////////////////
	// 描    述:  设施补偿值
	// 作    者:  褚冰冰
	// 创建时间:  2015-9-24 14:23
	// 参数说明:  @sec为秒补偿
	//         :  @mesc为毫秒补偿
	//		   :  @
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	void SetTimeCompensate(int sec,int msec)
	{
		m_iSecondsAdd = sec;
		m_iMSecondsAdd = msec;
	}

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取当前服务器上时间
	// 作    者:  褚冰冰
	// 创建时间:  2015-9-24 14:23
	// 参数说明:  @now为引用返回的当前NTP服务器时间
	//         :  @bSetToSystem为true表示时间获取成功后需要写入系统，false表示不需要写入系统
	// 返 回 值:  true表示获取成功，false表示获取失败
	//////////////////////////////////////////////////////////////////////////
	bool GetTime(SDateTime &now,bool bSetToSystem=true);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  时间偏差计算
	// 作    者:  邵凯田
	// 创建时间:  2015-12-15 23:02
	// 参数说明:  @local_soc
	//         :  @local_usec
	//         :  @remote_soc
	//         :  @remote_usec
	//         :  @ts_sec
	//         :  @ts_usec
	// 返 回 值:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool TestTimeByTssm(int &local_soc,int &local_usec,int &remote_soc,int &remote_usec,int &ts_sec,int &ts_usec);

private:
	SString m_sServerIp;
	int m_iPort;//123
	int m_iSecondsAdd;//秒补偿
	int m_iMSecondsAdd;//毫秒补偿
};

class SBASE_EXPORT SNtpServer : public SNtpBase
{
public:
	SNtpServer();
	virtual ~SNtpServer();
	//////////////////////////////////////////////////////////////////////////
	// 描    述:  设置请求服务器的ip和端口（可以不调用，默认ip为空）
	// 作    者:  褚冰冰
	// 创建时间:  2015-9-24 14:20
	// 参数说明:  @ip为SNTP服务器绑定IP地址
	//         :  @port为UDP端口号，缺省为123
	//		   :  @
	// 返 回 值:  无
	//////////////////////////////////////////////////////////////////////////
	void SetServer(SString ip,int port=SNTPSERVER_PORT)
	{
		m_sServerIp = ip;
		m_iPort = port;
	}

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  启动ntp校时服务
	// 作    者:  褚冰冰
	// 创建时间:  2015-9-24 14:21
	// 参数说明:  @bTssm 是否启动TSSM验证
	// 返 回 值:  false 表示启动失败
	//////////////////////////////////////////////////////////////////////////
	bool StartServer(bool bTssm=true);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  停止sntp校时服务，可能会等待一段时间
	// 作    者:  褚冰冰
	// 创建时间:  2015-9-24 14:21
	// 参数说明:  @
	// 返 回 值:  true 表示停止成功
	//////////////////////////////////////////////////////////////////////////
	bool StopServer();
private:
	//////////////////////////////////////////////////////////////////////////
	// 描    述:  ntp校时服务端线程执行函数，循环接受客户端请求
	//            检测udp连接，失效会重新创建
	// 作    者:  褚冰冰
	// 创建时间:  2015-9-25 17:38
	// 参数说明:  @arg 为线程传递参数
	//         :  @
	//		   :  @
	// 返 回 值:  无
	//////////////////////////////////////////////////////////////////////////
	static void *funproc(void *arg);
	bool m_bTssm;
	SString m_sServerIp;
	int m_iPort;//123
	bool m_bRun;
	bool m_bStoped;
	SSocket m_udp;
};
#endif//__SBASE_NTP_H__
