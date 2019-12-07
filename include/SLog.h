/**
 *
 * 文 件 名 : SLog.h
 * 创建日期 : 2007-01-01
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: 2011/08/31 08:43:33 $
 * 当前版本 : $Revision: 1.1 $
 * 功能描述 : 日志记录的信息元素有：
 *						1 模块名
 *						2 日志类型：1:debug/2:warn/3:error/4:fault
 *						3 发生时间
 *						4 源文件名
 *						5 源文件行号
 *						6 日志内容描述
 * 修改记录 : 
 *            $Log: SLog.h,v $
 *            Revision 1.1  2011/08/31 08:43:33  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2011/04/29 08:53:08  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2011/04/20 14:00:54  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2011/02/25 03:12:07  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2010/08/09 03:04:43  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2009/09/15 02:14:08  shaokt
 *            通迅记录分析装置项目首次提交
 *
 *            Revision 1.2  2009/06/19 10:35:05  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/02 01:50:08  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.5  2008/02/18 01:07:55  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.4  2007/10/19 06:18:33  shaokt
 *            限制日志文件大小
 *
 *            Revision 1.3  2007/09/11 08:17:53  shaokt
 *            基类更新
 *
 *            Revision 1.2  2007/09/04 01:43:38  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2007/08/15 03:09:22  shaokt
 *            *** empty log message ***
 *
 **/

#ifndef SKTLOGGER_H_
#define SKTLOGGER_H_



#ifdef WIN32
	#ifndef _CRT_SECURE_NO_WARNINGS
	#define _CRT_SECURE_NO_WARNINGS
	#endif
	#define WIN32_LEAN_AND_MEAN 
	#include <stdio.h> 
	#include <stdlib.h> 
	#include <winsock2.h> 
	#pragma comment(lib,"ws2_32.lib")
#else
	#include <sys/socket.h>
	#include <arpa/inet.h>
	#include <sys/select.h>
	#include <unistd.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include "SLock.h"

//#define LOG_HAS_FUNCTION_NAME

//最大缓存日志数量
#define LOG_MAX_BUFFER_ITEMS 10000

#ifdef _POWERPC
	#ifndef DWORD
		#define DWORD unsigned int
	#endif
	#define LOG_SWAP_DWORD(dw) dw = (((DWORD)dw) << 24 | ((DWORD)dw) >> 24) | ((((DWORD)dw)&0x0000ff00) << 8 | (((DWORD)dw)&0x00ff0000) >> 8);
#else
	#define LOG_SWAP_DWORD(dw)
#endif

using namespace std;

#ifdef WIN32
	#ifndef __FUNCTION__
		#define __FUNCTION__ ""
	#endif
#endif

//非VC编译时，默认为VC7以下版本
#ifndef _MSC_VER
	#define _SKT_MSVC_DEFINED
	#define _MSC_VER 1400
#endif

#ifdef SLOG_CLOSED
	#define _SKT_WITHOUT_BASEDEBUG
	#define _SKT_WITHOUT_DEBUG
	#define _SKT_WITHOUT_WARN
	#define _SKT_WITHOUT_ERROR
	#define _SKT_WITHOUT_FAULT
#endif

#ifdef _SKT_WITHOUT_BASEDEBUG
	#if _MSC_VER<=1200 //<=VC6.0
		#define LOGBASEDEBUG 
	#else
		#define LOGBASEDEBUG(...)
	#endif
#else
	#if _MSC_VER<=1200 //<=VC6.0
		#define LOGBASEDEBUG SLog::getLog()->logHead(0,__FILE__,__LINE__,__FUNCTION__);SLog::getLog()->logBody
	#else
		#define LOGBASEDEBUG(...) {SLog *pSLog=SLog::getLog();if(pSLog->isLogLevelOn(0)){ pSLog->logHead(0,__FILE__,__LINE__,__FUNCTION__);pSLog->logBody(__VA_ARGS__);}}
	#endif
#endif

#ifdef _SKT_WITHOUT_DEBUG
	#if _MSC_VER<=1200 //<=VC6.0
		#define LOGDEBUG 
	#else
		#define LOGDEBUG(...)
	#endif
#else
	#if _MSC_VER<=1200 //<=VC6.0
		#define LOGDEBUG SLog::getLog()->logHead(1,__FILE__,__LINE__,__FUNCTION__);SLog::getLog()->logBody
	#else
		#define LOGDEBUG(...) {SLog *pSLog=SLog::getLog();if(pSLog->isLogLevelOn(1)){ pSLog->logHead(1,__FILE__,__LINE__,__FUNCTION__);pSLog->logBody(__VA_ARGS__);}}
	#endif
#endif
#ifdef _SKT_WITHOUT_WARN
	#if _MSC_VER<=1200 //<=VC6.0
		#define LOGWARN 
	#else
		#define LOGWARN(...)
	#endif
#else
	#if _MSC_VER<=1200 //<=VC6.0
		#define LOGWARN SLog::getLog()->logHead(2,__FILE__,__LINE__,__FUNCTION__);SLog::getLog()->logBody
	#else
		#define LOGWARN(...) {SLog *pSLog=SLog::getLog();if(pSLog->isLogLevelOn(2)){ pSLog->logHead(2,__FILE__,__LINE__,__FUNCTION__);pSLog->logBody(__VA_ARGS__);}}
	#endif
#endif
#ifdef _SKT_WITHOUT_ERROR
	#if _MSC_VER<=1200 //<=VC6.0
		#define LOGERROR 
	#else
		#define LOGERROR(...)
	#endif
#else
	#if _MSC_VER<=1200 //<=VC6.0
		#define LOGERROR SLog::getLog()->logHead(3,__FILE__,__LINE__,__FUNCTION__);SLog::getLog()->logBody
	#else
		#define LOGERROR(...) {SLog *pSLog=SLog::getLog();if(pSLog->isLogLevelOn(3)){ pSLog->logHead(3,__FILE__,__LINE__,__FUNCTION__);pSLog->logBody(__VA_ARGS__);}}
	#endif
#endif
#ifdef _SKT_WITHOUT_FAULT
	#if _MSC_VER<=1200 //<=VC6.0
		#define LOGFAULT 
	#else
		#define LOGFAULT(...)
	#endif
#else
	#if _MSC_VER<=1200 //<=VC6.0
		#define LOGFAULT SLog::getLog()->logHead(4,__FILE__,__LINE__,__FUNCTION__);SLog::getLog()->logBody
	#else
		#define LOGFAULT(...) {SLog *pSLog=SLog::getLog();if(pSLog->isLogLevelOn(4)){ pSLog->logHead(4,__FILE__,__LINE__,__FUNCTION__);pSLog->logBody(__VA_ARGS__);}}
	#endif
#endif

#ifdef _SKT_MSVC_DEFINED
	#undef _MSC_VER
#endif


#ifndef _WIN64
#ifndef SOCKET 
#define SOCKET unsigned long
#endif
#endif

#ifndef NULL 
#define NULL 0
#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET -1
#endif

#ifndef SOCKET_ERROR
#define SOCKET_ERROR -1
#endif

#ifndef WIN32
#ifndef BYTE
#define BYTE unsigned char
#endif
#endif

#define MAX_PACKAGE_SIZE 1020


#pragma pack(push)
#pragma pack(1)


class SBASE_EXPORT SLog  
{
public:
	enum SBASE_EXPORT LOG_LEVEL
	{
		LOG_BASEDEBUG=0,
		LOG_DEBUG=1,
		LOG_WARN=2,
		LOG_ERROR=3,
		LOG_FAULT=4,
		LOG_NONE=5
	};
	struct SBASE_EXPORT stuDateTime
	{
		char year;
		char month;
		char day;
		char hour;
		char minute;
		char second;
	};
	struct SBASE_EXPORT stuLog
	{
		int	size;
		int	threadid;
		char	logtype;
		stuDateTime		datetime;
		char	modulename[20];
		char	filename[40];
		int		line;
#ifdef LOG_HAS_FUNCTION_NAME
		char	funname[40];
#endif
		int     sub_no;//子类型号
		char	msg[10240];//attr:m=xxx;f=xxx;t=xxx;
	};

	SLog();
	virtual ~SLog();


	static std::string bufToHexString(BYTE *pBuffer, int iLen);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取静态日志实例
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 22:31
	// 参数说明:  void
	// 返 回 值:  SLog*
	//////////////////////////////////////////////////////////////////////////
	static SLog* getLog();

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  测试指定的日志级别是否开启
	// 作    者:  邵凯田
	// 创建时间:  2014-6-10 8:50
	// 参数说明:  @logtype为日志级别
	// 返 回 值:  true表示可以输出，false表示不输出
	//////////////////////////////////////////////////////////////////////////
	inline bool isLogLevelOn(short logtype)
	{
		if(logtype < m_iLogLevel)
			return false;
		return true;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  输出日志头
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 22:32
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	void logHead(short logtype,const char* file,int line,const char* func);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  输出日志体
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 22:32
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	void logBody(const char * cformat, ...);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  记录到文件
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 22:32
	// 参数说明:  @buff表示日志内容
	//            @logtype表示日志级别
	//            @sub_no表示日志子分类号，-1表示无子分类
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	inline void logFile(char *buff,int logtype,int sub_no);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  写文件行
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 22:33
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	static void writeFileLine(char * filename,char *buff);
	
	////////////////////////////////////////////////////////////////////////
	// 描    述:  设置日志级别
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 22:17
	// 参数说明:  @l为新的日志级别
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	static void setLogLevel(LOG_LEVEL l);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  退出日志线程，只在进程退出时使用，退出后无法再开启
	// 作    者:  邵凯田
	// 创建时间:  2015-6-11 16:23
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	static void quitLog();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  设置日志文件最大大小
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 22:17
	// 参数说明:  @size文件大小，单位字节
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	static void setLogFileMaxSize(int size);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  设置备份文件的最多数量
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 22:18
	// 参数说明:  @files表示备份文件最多数量
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	static void setLogFileMaxBackFiles(int files);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  设置当前模块名称
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 22:19
	// 参数说明:  @name新的模块名称
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	static void setModuleName(const char* name);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取当前模块名称
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 22:19
	// 参数说明:  void
	// 返 回 值:  模块名称
	//////////////////////////////////////////////////////////////////////////
	static char* getModuleName();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  开始日志输出到UDP服务器
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 22:20
	// 参数说明:  @ip为服务器IP地址
	//            @port表示端口
	// 返 回 值:  true表示成功，false表示失败
	//////////////////////////////////////////////////////////////////////////
	static bool startLogToUDPSvr(char* ip,int port=7001);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  开始日志输出到TCP服务器
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 22:24
	// 参数说明:  @ip为服务器IP地址
	//            @port表示端口
	// 返 回 值:  true表示成功，false表示失败
	//////////////////////////////////////////////////////////////////////////
	static bool startLogToTCPSvr(char* ip,int port=7001);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  开始日志输出到指定的文件
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 22:25
	// 参数说明:  @file表示日志文件名称
	// 返 回 值:  true表示成功，false表示失败
	//////////////////////////////////////////////////////////////////////////
	static bool startLogToFile(char* file);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  开始日志输出到控制台
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 22:25
	// 参数说明:  void
	// 返 回 值:  true表示成功，false表示失败
	//////////////////////////////////////////////////////////////////////////
	static bool startLogToConsole();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  停止日志输出到UDP服务器
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 22:25
	// 参数说明:  void
	// 返 回 值:  true表示成功，false表示失败
	//////////////////////////////////////////////////////////////////////////
	static bool stopLogToUDPSvr();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  停止日志输出到TCP服务器
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 22:26
	// 参数说明:  void
	// 返 回 值:  true表示成功，false表示失败
	//////////////////////////////////////////////////////////////////////////
	static bool stopLogToTCPSvr();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  停止日志输出到文件
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 22:27
	// 参数说明:  void
	// 返 回 值:  true表示成功，false表示失败
	//////////////////////////////////////////////////////////////////////////
	static bool stopLogToFile();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  停止日志输出到控制台
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 22:27
	// 参数说明:  void
	// 返 回 值:  true表示成功，false表示失败
	//////////////////////////////////////////////////////////////////////////
	static bool stopLogToConsole();

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取当前日志中缓存未处理记录数量
	// 作    者:  邵凯田
	// 创建时间:  2014-6-10 19:17
	// 参数说明:  void
	// 返 回 值:  int
	//////////////////////////////////////////////////////////////////////////
	static int getLogBufferItems();

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  在规定的时间内等待日志系统处理完全部日志内容
	// 作    者:  邵凯田
	// 创建时间:  2014-6-10 19:18
	// 参数说明:  @secs为等待超时秒数
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	static void WaitForLogEmpty(int secs=2);

//	SLock lock;

	////////////////////////////////////////////////////////////////////////
	// 描    述:  初始化
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 22:28
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	static void init();

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  运行日志切换命令
	// 作    者:  邵凯田
	// 创建时间:  2014-6-12 9:29
	// 参数说明:  @pStr为命令字符串
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	static void RunLogCommand(char* pStr);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  日志系统控制台命令循环
	// 作    者:  邵凯田
	// 创建时间:  2014-6-12 10:56
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	static void LoopForLogCommand();

private:
	//////////////////////////////////////////////////////////////////////////
	// 描    述:  日志系统主线程，用于从缓存中发布日志到控制台、文件、TCP等
	// 作    者:  邵凯田
	// 创建时间:  2014-6-10 8:40
	// 参数说明:  this
	// 返 回 值:  NULL
	//////////////////////////////////////////////////////////////////////////
	static void* ThreadLog(void* lp);

	SOCKET m_hTcpSocket;
	SOCKET m_hUdpSocket;
	sockaddr_in m_dest_sin;//LOG服务器的地址

	std::string *m_sModuleName;//模块名
	std::string *m_sLogFile;//LOG文件名

	bool m_bToConsole;//是否输出到控制台
	bool m_bToFile;//是否输出到文件
	bool m_bToLogUDPSvr;//是否输出到UDP服务器
	bool m_bToLogTCPSvr;//是否输出到TCP服务器

	sockaddr_in m_udp_dest_sin;

	int m_iLogLevel;//日志级别
	long m_iMaxFileSize;//最大的日志文件大小
	int m_iMaxBackFiles;//最多的备份日志文件数，缺省5个
	stuLog m_log;
};
#pragma pack()
#pragma pack(pop)


#endif 
