/**
 *
 * �� �� �� : SLog.h
 * �������� : 2007-01-01
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: 2011/08/31 08:43:33 $
 * ��ǰ�汾 : $Revision: 1.1 $
 * �������� : ��־��¼����ϢԪ���У�
 *						1 ģ����
 *						2 ��־���ͣ�1:debug/2:warn/3:error/4:fault
 *						3 ����ʱ��
 *						4 Դ�ļ���
 *						5 Դ�ļ��к�
 *						6 ��־��������
 * �޸ļ�¼ : 
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
 *            ͨѸ��¼����װ����Ŀ�״��ύ
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
 *            ������־�ļ���С
 *
 *            Revision 1.3  2007/09/11 08:17:53  shaokt
 *            �������
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

//��󻺴���־����
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

//��VC����ʱ��Ĭ��ΪVC7���°汾
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
		int     sub_no;//�����ͺ�
		char	msg[10240];//attr:m=xxx;f=xxx;t=xxx;
	};

	SLog();
	virtual ~SLog();


	static std::string bufToHexString(BYTE *pBuffer, int iLen);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��̬��־ʵ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 22:31
	// ����˵��:  void
	// �� �� ֵ:  SLog*
	//////////////////////////////////////////////////////////////////////////
	static SLog* getLog();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����ָ������־�����Ƿ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-6-10 8:50
	// ����˵��:  @logtypeΪ��־����
	// �� �� ֵ:  true��ʾ���������false��ʾ�����
	//////////////////////////////////////////////////////////////////////////
	inline bool isLogLevelOn(short logtype)
	{
		if(logtype < m_iLogLevel)
			return false;
		return true;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����־ͷ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 22:32
	// ����˵��:  
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	void logHead(short logtype,const char* file,int line,const char* func);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����־��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 22:32
	// ����˵��:  
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	void logBody(const char * cformat, ...);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��¼���ļ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 22:32
	// ����˵��:  @buff��ʾ��־����
	//            @logtype��ʾ��־����
	//            @sub_no��ʾ��־�ӷ���ţ�-1��ʾ���ӷ���
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	inline void logFile(char *buff,int logtype,int sub_no);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  д�ļ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 22:33
	// ����˵��:  
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	static void writeFileLine(char * filename,char *buff);
	
	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ������־����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 22:17
	// ����˵��:  @lΪ�µ���־����
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	static void setLogLevel(LOG_LEVEL l);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �˳���־�̣߳�ֻ�ڽ����˳�ʱʹ�ã��˳����޷��ٿ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-6-11 16:23
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	static void quitLog();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ������־�ļ�����С
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 22:17
	// ����˵��:  @size�ļ���С����λ�ֽ�
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	static void setLogFileMaxSize(int size);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���ñ����ļ����������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 22:18
	// ����˵��:  @files��ʾ�����ļ��������
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	static void setLogFileMaxBackFiles(int files);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���õ�ǰģ������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 22:19
	// ����˵��:  @name�µ�ģ������
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	static void setModuleName(const char* name);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��ǰģ������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 22:19
	// ����˵��:  void
	// �� �� ֵ:  ģ������
	//////////////////////////////////////////////////////////////////////////
	static char* getModuleName();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ʼ��־�����UDP������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 22:20
	// ����˵��:  @ipΪ������IP��ַ
	//            @port��ʾ�˿�
	// �� �� ֵ:  true��ʾ�ɹ���false��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	static bool startLogToUDPSvr(char* ip,int port=7001);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ʼ��־�����TCP������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 22:24
	// ����˵��:  @ipΪ������IP��ַ
	//            @port��ʾ�˿�
	// �� �� ֵ:  true��ʾ�ɹ���false��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	static bool startLogToTCPSvr(char* ip,int port=7001);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ʼ��־�����ָ�����ļ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 22:25
	// ����˵��:  @file��ʾ��־�ļ�����
	// �� �� ֵ:  true��ʾ�ɹ���false��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	static bool startLogToFile(char* file);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ʼ��־���������̨
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 22:25
	// ����˵��:  void
	// �� �� ֵ:  true��ʾ�ɹ���false��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	static bool startLogToConsole();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ֹͣ��־�����UDP������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 22:25
	// ����˵��:  void
	// �� �� ֵ:  true��ʾ�ɹ���false��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	static bool stopLogToUDPSvr();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ֹͣ��־�����TCP������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 22:26
	// ����˵��:  void
	// �� �� ֵ:  true��ʾ�ɹ���false��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	static bool stopLogToTCPSvr();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ֹͣ��־������ļ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 22:27
	// ����˵��:  void
	// �� �� ֵ:  true��ʾ�ɹ���false��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	static bool stopLogToFile();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ֹͣ��־���������̨
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 22:27
	// ����˵��:  void
	// �� �� ֵ:  true��ʾ�ɹ���false��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	static bool stopLogToConsole();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��ǰ��־�л���δ�����¼����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-6-10 19:17
	// ����˵��:  void
	// �� �� ֵ:  int
	//////////////////////////////////////////////////////////////////////////
	static int getLogBufferItems();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �ڹ涨��ʱ���ڵȴ���־ϵͳ������ȫ����־����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-6-10 19:18
	// ����˵��:  @secsΪ�ȴ���ʱ����
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	static void WaitForLogEmpty(int secs=2);

//	SLock lock;

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ʼ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-18 22:28
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	static void init();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ������־�л�����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-6-12 9:29
	// ����˵��:  @pStrΪ�����ַ���
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	static void RunLogCommand(char* pStr);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��־ϵͳ����̨����ѭ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-6-12 10:56
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	static void LoopForLogCommand();

private:
	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��־ϵͳ���̣߳����ڴӻ����з�����־������̨���ļ���TCP��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-6-10 8:40
	// ����˵��:  this
	// �� �� ֵ:  NULL
	//////////////////////////////////////////////////////////////////////////
	static void* ThreadLog(void* lp);

	SOCKET m_hTcpSocket;
	SOCKET m_hUdpSocket;
	sockaddr_in m_dest_sin;//LOG�������ĵ�ַ

	std::string *m_sModuleName;//ģ����
	std::string *m_sLogFile;//LOG�ļ���

	bool m_bToConsole;//�Ƿ����������̨
	bool m_bToFile;//�Ƿ�������ļ�
	bool m_bToLogUDPSvr;//�Ƿ������UDP������
	bool m_bToLogTCPSvr;//�Ƿ������TCP������

	sockaddr_in m_udp_dest_sin;

	int m_iLogLevel;//��־����
	long m_iMaxFileSize;//������־�ļ���С
	int m_iMaxBackFiles;//���ı�����־�ļ�����ȱʡ5��
	stuLog m_log;
};
#pragma pack()
#pragma pack(pop)


#endif 
