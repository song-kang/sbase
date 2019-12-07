/**
 *
 * 文 件 名 : SLog.cpp
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
 *            $Log: SLog.cpp,v $
 *            Revision 1.1  2011/08/31 08:43:33  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.5  2011/04/21 02:14:17  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.4  2011/04/20 14:00:53  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2011/04/20 05:37:57  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2011/02/28 01:28:33  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2010/08/09 03:04:43  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2009/09/15 02:14:08  shaokt
 *            通迅记录分析装置项目首次提交
 *
 *            Revision 1.2  2009/06/19 10:35:04  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.4  2009/01/23 03:36:35  shaokt
 *            修改日志文件写满时的处理方式，原来方式为将文件转存为一个减少10%的swap文件，这样还是会经常写满，改成写满后将文件转存为back文件，并删除当前的日志,这样文件不会经常写满，提高程序执行效率
 *
 *            Revision 1.3  2009/01/19 08:07:26  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2009/01/16 06:16:02  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/02 01:50:08  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.7  2008/04/30 07:38:43  shaokt
 *            增加分级别写日志文件
 *
 *            Revision 1.6  2007/10/29 07:30:23  shaokt
 *            添加对象地址是否为4的倍数的检测函数，为检测ARM下由于pack(1)造成的内存问题
 *
 *            Revision 1.5  2007/10/19 06:18:33  shaokt
 *            限制日志文件大小
 *
 *            Revision 1.4  2007/10/17 10:02:11  shaokt
 *            控制台输入增加模块名
 *
 *            Revision 1.3  2007/09/21 07:53:41  shaokt
 *            基类更新
 *
 *            Revision 1.2  2007/09/11 08:17:53  shaokt
 *            基类更新
 *
 *            Revision 1.1  2007/08/15 03:09:22  shaokt
 *            *** empty log message ***
 *
 **/

#include "SLog.h"
#include "SList.h"

// #ifdef _DEBUG
// #undef THIS_FILE
// static char THIS_FILE[]=__FILE__;
// #define new DEBUG_NEW
// #endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SLock g_skt_LogLock;//日志锁
SPtrList<SLog::stuLog> g_slBufferedLog;//缓冲日志
/*static*/ SLog g_skt_log;
/*static*/ bool g_b_quit_log = false;
/*static*/ bool g_b_log_quited = false;
short g_skt_CurrLogLevel;//当前日志的输出级别
std::string SLog::bufToHexString(BYTE *pBuffer, int iLen)
{
	std::string str="";
	char buf[4];
	memset(buf,0,4);
	bool b=false;
	if(iLen>1000)
	{
		b = true;
		iLen = 1000;
	}
	for(int i=0;i<iLen;i++)
	{
		sprintf(buf,"%02X ",pBuffer[i]);
		str += buf;
	}
	if(b)
		str += " ...... ";
	return str;
}

SLog::SLog()
{
	m_sModuleName = new std::string();
	m_sLogFile = new std::string();
	//printf("into SLog::SLog() this=%p\n",this);
	m_hUdpSocket = /*NULL*/0;
	m_hTcpSocket = /*NULL*/0;
	memset(&m_log,0,sizeof(stuLog));
	m_iLogLevel = LOG_ERROR;
	m_iMaxFileSize = 1024 * 1024;//最大保存1M的日志文件
	m_iMaxBackFiles = 5;
	g_slBufferedLog.setAutoDelete(true);
	g_slBufferedLog.setShared(true);
	init();
}

SLog::~SLog()
{
	delete m_sModuleName;
	delete m_sLogFile;
// 	int* p=0;
// 	*p = 0;
	//printf("SLog::~SLog() this=%p\n",this);
}

SLog* SLog::getLog()
{
	return &g_skt_log;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  设置日志级别
// 作    者:  邵凯田
// 创建时间:  2011-11-18 22:17
// 参数说明:  @l为新的日志级别
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SLog::setLogLevel(SLog::LOG_LEVEL l)
{
	getLog()->m_iLogLevel = l;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  退出日志线程，只在进程退出时使用，退出后无法再开启
// 作    者:  邵凯田
// 创建时间:  2015-6-11 16:23
// 参数说明:  void
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SLog::quitLog()
{
	printf("quitLog() now!\n");
	g_b_quit_log = true;
#ifdef WIN32
	Sleep(10);
#else
	usleep(10000);
#endif
	int i=0;
	while(g_slBufferedLog.count() > 0)
	{
#ifdef WIN32
		Sleep(1);
#else
		usleep(1000);
#endif
		if(i++>100)
			break;
	}
	i=0;
	while(!g_b_log_quited)
	{
#ifdef WIN32
		Sleep(1);
#else
		usleep(1000);
#endif
		if(i++>100)
			break;
	}
	g_slBufferedLog.clear();
}

////////////////////////////////////////////////////////////////////////
// 描    述:  设置日志文件最大大小
// 作    者:  邵凯田
// 创建时间:  2011-11-18 22:17
// 参数说明:  @size文件大小，单位字节
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SLog::setLogFileMaxSize(int size)
{
	getLog()->m_iMaxFileSize = size;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  设置备份文件的最多数量
// 作    者:  邵凯田
// 创建时间:  2011-11-18 22:18
// 参数说明:  @files表示备份文件最多数量
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SLog::setLogFileMaxBackFiles(int files)
{
	getLog()->m_iMaxBackFiles = files;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  设置当前模块名称
// 作    者:  邵凯田
// 创建时间:  2011-11-18 22:19
// 参数说明:  @name新的模块名称
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SLog::setModuleName(const char* name)
{
	if(getLog()->m_sModuleName->length() > 0)
		return;//如果已经被设置过一次，忽略以后的再次设置
	*(getLog()->m_sModuleName) = name;
	int p=(int)getLog()->m_sModuleName->find_last_of('\\',getLog()->m_sModuleName->length());
	if(p>=0)
		*(getLog()->m_sModuleName) = getLog()->m_sModuleName->substr(p+1,getLog()->m_sModuleName->length()-p-1);
	p=(int)getLog()->m_sModuleName->find_last_of('/',getLog()->m_sModuleName->length());
	if(p>=0)
		*(getLog()->m_sModuleName) = getLog()->m_sModuleName->substr(p+1,getLog()->m_sModuleName->length()-p-1);
#ifdef WIN32
	if(getLog()->m_sModuleName->length()>4 && getLog()->m_sModuleName->substr(getLog()->m_sModuleName->length()-4,4) == ".exe")
		*(getLog()->m_sModuleName) = getLog()->m_sModuleName->substr(0,getLog()->m_sModuleName->length()-4);
#endif
}

////////////////////////////////////////////////////////////////////////
// 描    述:  取当前模块名称
// 作    者:  邵凯田
// 创建时间:  2011-11-18 22:19
// 参数说明:  void
// 返 回 值:  模块名称
//////////////////////////////////////////////////////////////////////////
char* SLog::getModuleName()
{
	return (char*)getLog()->m_sModuleName->c_str();
}

////////////////////////////////////////////////////////////////////////
// 描    述:  开始日志输出到UDP服务器
// 作    者:  邵凯田
// 创建时间:  2011-11-18 22:20
// 参数说明:  @ip为服务器IP地址
//            @port表示端口
// 返 回 值:  true表示成功，false表示失败
//////////////////////////////////////////////////////////////////////////
bool SLog::startLogToUDPSvr(char* ip,int port)
{
	SLog *p = getLog();
	if(p == /*NULL*/0)
		return false;

	#ifdef _WIN32
		WSADATA WSAData;
		WSAStartup( MAKEWORD( 1, 1 ), &WSAData );
	#endif
	p->m_hUdpSocket = (SOCKET)::socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if(p->m_hUdpSocket == INVALID_SOCKET)
	{
	  p->m_hUdpSocket = /*NULL*/0;
		return false;
	}
	memset(&p->m_udp_dest_sin, 0 ,sizeof(p->m_udp_dest_sin));
	p->m_udp_dest_sin.sin_family 			= AF_INET;
	p->m_udp_dest_sin.sin_addr.s_addr = inet_addr(ip);
	p->m_udp_dest_sin.sin_port 				= htons(port); 
	p->m_bToLogUDPSvr=true;
	return true;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  开始日志输出到TCP服务器
// 作    者:  邵凯田
// 创建时间:  2011-11-18 22:24
// 参数说明:  @ip为服务器IP地址
//            @port表示端口
// 返 回 值:  true表示成功，false表示失败
//////////////////////////////////////////////////////////////////////////
bool SLog::startLogToTCPSvr(char* ip,int port)
{
	SLog *p = getLog();
	printf("SLog::startLogToTCPSvr(%s,%d) p=%p\n",ip,port,p);
	if(p == /*NULL*/0)
		return false;
	#ifdef _WIN32
		WSADATA WSAData;
		WSAStartup( MAKEWORD( 1, 1 ), &WSAData );
	#endif
	p->m_hTcpSocket = (SOCKET)::socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	printf("log tcp af=%d,type=%d,protocol=%d,handle=%X\n",AF_INET,SOCK_STREAM,IPPROTO_TCP,p->m_hTcpSocket);
	if(p->m_hTcpSocket == INVALID_SOCKET)
	{
		p->m_hTcpSocket = /*NULL*/0;
	  return false;
	}
#ifdef WIN32
  int bNodelay = 1;  
	int err = setsockopt(p->m_hTcpSocket,IPPROTO_TCP,TCP_NODELAY,(char *)&bNodelay,sizeof(bNodelay));//不采用延时算法 
	if ( err == SOCKET_ERROR ) 
	{
	  stopLogToTCPSvr();
		return false;       
	} 		
#endif
	int ret;
	sockaddr_in dest_sin;
	memset(&dest_sin, 0 ,sizeof(dest_sin));
	dest_sin.sin_family 		= AF_INET;
	dest_sin.sin_addr.s_addr = inet_addr(ip);
	dest_sin.sin_port 			= htons(port); 
	ret = ::connect(p->m_hTcpSocket, (sockaddr *)&dest_sin, sizeof(dest_sin));
	if( ret < 0)
	{
	  stopLogToTCPSvr();
		return false;
	}

	p->m_bToLogTCPSvr=true;
	return true;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  开始日志输出到指定的文件
// 作    者:  邵凯田
// 创建时间:  2011-11-18 22:25
// 参数说明:  @file表示日志文件名称
// 返 回 值:  true表示成功，false表示失败
//////////////////////////////////////////////////////////////////////////
bool SLog::startLogToFile(char* file)	
{
	if(getLog()->m_sLogFile->length() > 0)
		return false;
	printf("SLog::startLogToFile(%s)\n",file);
	*(getLog()->m_sLogFile) = file;
	getLog()->m_bToFile=true;
	return true;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  开始日志输出到控制台
// 作    者:  邵凯田
// 创建时间:  2011-11-18 22:25
// 参数说明:  void
// 返 回 值:  true表示成功，false表示失败
//////////////////////////////////////////////////////////////////////////
bool SLog::startLogToConsole()	
{
	printf("Slog::startLogToConsole()\n");
	getLog()->m_bToConsole = true;
	return true;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  停止日志输出到UDP服务器
// 作    者:  邵凯田
// 创建时间:  2011-11-18 22:25
// 参数说明:  void
// 返 回 值:  true表示成功，false表示失败
//////////////////////////////////////////////////////////////////////////
bool SLog::stopLogToUDPSvr()
{
	SLog *p = getLog();
	if(p->m_hUdpSocket != /*NULL*/0)
	{
		shutdown(p->m_hUdpSocket, 2);
		#ifdef WIN32
			closesocket(p->m_hUdpSocket);
		#else
			close(p->m_hUdpSocket);
		#endif
	}
	p->m_bToLogUDPSvr=false;
	return true;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  停止日志输出到TCP服务器
// 作    者:  邵凯田
// 创建时间:  2011-11-18 22:26
// 参数说明:  void
// 返 回 值:  true表示成功，false表示失败
//////////////////////////////////////////////////////////////////////////
bool SLog::stopLogToTCPSvr()
{
	SLog *p = getLog();
	if(p->m_hTcpSocket != /*NULL*/0)
	{
		shutdown(p->m_hTcpSocket, 2);
		#ifdef WIN32
			closesocket(p->m_hTcpSocket);
		#else
			close(p->m_hTcpSocket);
		#endif
	}
	p->m_bToLogTCPSvr=false;
	return true;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  停止日志输出到文件
// 作    者:  邵凯田
// 创建时间:  2011-11-18 22:27
// 参数说明:  void
// 返 回 值:  true表示成功，false表示失败
//////////////////////////////////////////////////////////////////////////
bool SLog::stopLogToFile()
{
	getLog()->m_bToFile=false;
	return true;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  停止日志输出到控制台
// 作    者:  邵凯田
// 创建时间:  2011-11-18 22:27
// 参数说明:  void
// 返 回 值:  true表示成功，false表示失败
//////////////////////////////////////////////////////////////////////////
bool SLog::stopLogToConsole()
{
	getLog()->m_bToConsole=false;
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  取当前日志中缓存未处理记录数量
// 作    者:  邵凯田
// 创建时间:  2014-6-10 19:17
// 参数说明:  void
// 返 回 值:  int
//////////////////////////////////////////////////////////////////////////
int SLog::getLogBufferItems()
{
	return g_slBufferedLog.count();
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  在规定的时间内等待日志系统处理完全部日志内容
// 作    者:  邵凯田
// 创建时间:  2014-6-10 19:18
// 参数说明:  @secs为等待超时秒数
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SLog::WaitForLogEmpty(int secs)
{
	time_t last=::time(NULL);
	if(secs <= 0)
		secs = 1;
	while(g_slBufferedLog.count() > 0 && abs((int)(::time(NULL)-last)) < secs)
	{
#ifdef WIN32
		Sleep(500);
#else
		usleep(500000);
#endif
	}
	g_slBufferedLog.setShared(false);
}


////////////////////////////////////////////////////////////////////////
// 描    述:  初始化
// 作    者:  邵凯田
// 创建时间:  2011-11-18 22:28
// 参数说明:  void
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SLog::init()
{
	SLog* pThis = getLog();
	printf("Into SLog::init() this=%p\n",pThis);
	*(pThis->m_sModuleName) = "";
	pThis->m_bToConsole = true;
	pThis->m_bToFile	 = false;
	pThis->m_bToLogUDPSvr  = false;
	pThis->m_bToLogTCPSvr  = false;
#ifdef WIN32
	char buf[1205];
	memset(buf,0,1025);
#ifdef UNICODE
	WCHAR *pStr = (WCHAR*)buf;
#else
	char *pStr = buf;
#endif
	GetModuleFileName(/*NULL*/0,pStr,1024);
	pThis->setModuleName(buf);
#endif
	//static bool bInited=false;
	//if(!bInited)
	{
		//printf("not init yet! start thread now!\n");
		//bInited = true;
#ifdef WIN32
		HANDLE h; 
		h = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)(void*)ThreadLog, (LPVOID)pThis, 0, 0); 
		CloseHandle(h); 
#else
		pthread_t t=0; 
		int stacksize = 204800;/*20480为20K*/; 
		pthread_attr_t attr,*pattr=NULL; 
		if(pthread_attr_init(&attr) == 0 && pthread_attr_setstacksize(&attr, stacksize) == 0) 
			pattr = &attr; 
		else 
			printf("@@@@ pthread_attr_setstacksize to %d error!\r\n",stacksize); 
		pthread_create(&t, pattr, ThreadLog,(void*)pThis); 
		if(t == 0) 
			printf("@@@ pthread_create ThreadLog error!"); 
#endif
	}
}

static string g_log_type_desc[]={"basedebug","debug","warn","error","fault"};
//处理日志头
void SLog::logHead(short logtype,const char* file,int line,const char* func)
{
	if(g_b_quit_log)
		return;
	g_skt_LogLock.lock();
	if(logtype<m_iLogLevel)
	{
		m_log.logtype = 100;
		return;
	}
	string str;
#ifdef WIN32
	m_log.threadid = (int)GetCurrentThreadId();
#else
	m_log.threadid = (int)pthread_self();
#endif
	m_log.logtype = (char)logtype;
	str = file;
	int p;
	p=(int)str.find_last_of('\\',str.length());
	if(p>=0)
	{
		p++;
		str = str.substr(p,str.length()-p);
	}
	p=(int)str.find_last_of('/',str.length());
	if(p>=0)
	{
		p++;
		str = str.substr(p,str.length()-p);
	}
	if(str.length()>=sizeof(m_log.filename))
		str = str.substr(0,sizeof(m_log.filename)-1);
	strcpy(m_log.filename,str.c_str());
	m_log.line = line;
#ifdef LOG_HAS_FUNCTION_NAME
	strcpy(m_log.funname,func);
#endif
	str = *m_sModuleName;
	if(str.length()>=sizeof(m_log.modulename))
		str = str.substr(0,sizeof(m_log.modulename)-1);
	strcpy(m_log.modulename,str.c_str());
	struct  tm *t;
	time_t now;
	time(&now);
	t = localtime(&now);
	m_log.datetime.year = t->tm_year;
	m_log.datetime.month = t->tm_mon;
	m_log.datetime.day = t->tm_mday;
	m_log.datetime.hour = t->tm_hour;
	m_log.datetime.minute = t->tm_min;
	m_log.datetime.second = t->tm_sec;
}

void SLog::logBody(const char * cformat, ...)
{
	if(g_b_quit_log)
	{
		return;
	}
	if(m_log.logtype == 100)
	{
		g_skt_LogLock.unlock();
		return;
	}
	
//	memset(m_log.msg,0x00,sizeof(m_log.msg));
    m_log.msg[sizeof(m_log.msg)-1] = '\0';
	va_list ap;
	va_start(ap, cformat);
	vsprintf(m_log.msg,cformat,ap);
	va_end(ap);

    if(m_log.msg[sizeof(m_log.msg)-1] != '\0')
    {
        printf("SLog error!!! Log body text overflow!!!\n");
        m_log.msg[sizeof(m_log.msg)-1] = '\0';
        printf("Log text: %s\n",m_log.msg);
        printf("SLog error!!! Log body text overflow!!!\n");
        exit(0);
    }
	int slen = (int)strlen(m_log.msg);
	if(slen > 0)
	{
		char *pCh = &m_log.msg[slen-1];
		while(pCh > m_log.msg && (*pCh == '\n' || *pCh == '\r'))
		{
			*pCh = '\0';
			pCh--;
			slen--;
		}
	}
	int len = (sizeof(stuLog)-sizeof(m_log.msg))+slen+1;
	stuLog *p = (stuLog*)new BYTE[len];
	memcpy(p,&m_log,len);
	p->sub_no = -1;

	//print to console
	if (m_bToConsole)
	{
#ifdef LOG_HAS_FUNCTION_NAME
		printf("{%s} %04d-%02d-%02d %02d:%02d:%02d[%p] %s at %s(%s line:%d):\t%s\r\n", 
			m_sModuleName->data(),p->datetime.year + 1900, p->datetime.month + 1, p->datetime.day,
			p->datetime.hour, p->datetime.minute, p->datetime.second, (void*)p->threadid, g_log_type_desc[(int)p->logtype].c_str(),
			p->filename, p->funname, p->line, p->msg);
#else
#if 0
		int msec;
		SYSTEMTIME st;
		GetLocalTime(&st);
		msec = st.wMilliseconds;
		printf("{%s} %04d-%02d-%02d %02d:%02d:%02d.%03d[%d] %s at %s(%s line:%d):\t%s\r\n", 
			m_sModuleName->data(),p->datetime.year + 1900, p->datetime.month + 1, p->datetime.day,
			p->datetime.hour, p->datetime.minute, p->datetime.second, msec, p->threadid, g_log_type_desc[(int)p->logtype].c_str(),
			p->filename, "", p->line, p->msg);
#else
		printf("{%s} %04d-%02d-%02d %02d:%02d:%02d[%d] %s at %s(%s line:%d):\t%s\r\n", 
			m_sModuleName->data(),(int)p->datetime.year + 1900, (int)p->datetime.month + 1, (int)p->datetime.day,
			(int)p->datetime.hour, (int)p->datetime.minute, (int)p->datetime.second, p->threadid, (char*)(g_log_type_desc[(int)p->logtype].c_str()),
			p->filename, "", p->line, p->msg);
#endif
#endif
	}

	if(m_bToFile || m_bToLogTCPSvr || m_bToLogUDPSvr)
	{
		//printf("add to g_slBufferedLog, count()=%d\n",g_slBufferedLog.count());
		g_slBufferedLog.append(p);
		if(g_slBufferedLog.count() > LOG_MAX_BUFFER_ITEMS)
			g_slBufferedLog.remove(10);//为避免并发错误，删除第10个最老的记录
	}
	else
	{
		delete p;
	}

	g_skt_LogLock.unlock();
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  日志系统主线程，用于从缓存中发布日志到控制台、文件、TCP等
// 作    者:  邵凯田
// 创建时间:  2014-6-10 8:40
// 参数说明:  this
// 返 回 值:  NULL
//////////////////////////////////////////////////////////////////////////
#ifndef WIN32
#include <signal.h>   
#endif
void* SLog::ThreadLog(void* lp)
{
#ifndef WIN32
	pthread_detach(pthread_self());
	signal(SIGPIPE,SIG_IGN);
	sigset_t signal_mask;
	sigemptyset (&signal_mask);
	sigaddset (&signal_mask, SIGPIPE);
#endif
	SLog *pThis = (SLog*)lp;
	char *buf = new char[20480];
	stuLog *p;
	g_slBufferedLog.setShared(true);
	g_slBufferedLog.setAutoDelete(true);
	printf("into SLog::ThreadLog()... g_b_quit_log=%d this=%p\n",g_b_quit_log,pThis);
// 	while(1)
// 	{
// #ifdef WIN32
// 		Sleep(50);
// #else
// 		usleep(5000);
// #endif
// 		printf("thread\n");
// 	}
	while(!g_b_quit_log)
	{
		if(g_slBufferedLog.count() == 0)
		{
			#ifdef WIN32
				Sleep(50);
			#else
				usleep(50000);
			#endif
//			printf(".\n");
			continue;
		}
//		printf("*\n");
		p = g_slBufferedLog[0];
		if(p == NULL)
		{
//			printf("#\n");
			continue;
		}

		//memset(buf,0,20480);
#ifdef LOG_HAS_FUNCTION_NAME
		sprintf(buf,"%04d-%02d-%02d %02d:%02d:%02d[%d] %s at %s(%s line:%d):\t%s\r\n",p->datetime.year+1900,p->datetime.month+1,p->datetime.day,
			p->datetime.hour,p->datetime.minute,p->datetime.second,p->threadid, (char*)(g_log_type_desc[(int)p->logtype].c_str()),
			p->filename,p->funname,p->line,p->msg);
#else
		sprintf(buf,"%04d-%02d-%02d %02d:%02d:%02d[%d] %s at %s(%s line:%d):\t%s\r\n",(int)p->datetime.year+1900,(int)p->datetime.month+1,(int)p->datetime.day,
			(int)p->datetime.hour,(int)p->datetime.minute,(int)p->datetime.second,p->threadid, (char*)(g_log_type_desc[(int)p->logtype].c_str()),
			p->filename,"",p->line,p->msg);
#endif
//		printf("THREAD(%d,%d) {%s}%s",pThis->m_bToFile,pThis->m_bToLogTCPSvr,p->modulename,buf);
		if(pThis->m_bToFile)
			pThis->logFile(buf, p->logtype, p->sub_no);

		if(pThis->m_bToLogUDPSvr)
		{
			p->size = (int)((sizeof(stuLog) - sizeof(p->msg))+strlen(p->msg));
			int size = p->size;
			p->size = htonl(p->size);
			//LOG_SWAP_DWORD(p->size);
			LOG_SWAP_DWORD(p->line);
			LOG_SWAP_DWORD(p->threadid);
			int ret = sendto(pThis->m_hUdpSocket, (char *)p, size, 0, (sockaddr*)&pThis->m_udp_dest_sin, sizeof(pThis->m_udp_dest_sin));
			//LOG_SWAP_DWORD(p->size);
			LOG_SWAP_DWORD(p->line);
			LOG_SWAP_DWORD(p->threadid);
			if(ret != sizeof(stuLog))
			{
				pThis->stopLogToUDPSvr();//发送失败不再发送到UDP
			}
		}
		if(pThis->m_bToLogTCPSvr)
		{
			p->size = (int)((sizeof(stuLog) - sizeof(p->msg))+strlen(p->msg));
			int size = p->size;
			p->size = htonl(p->size);
			//LOG_SWAP_DWORD(p->size);
			LOG_SWAP_DWORD(p->line);
			LOG_SWAP_DWORD(p->threadid);
			int ret=::send(pThis->m_hTcpSocket, (char *)p, size, 0);
			//LOG_SWAP_DWORD(p->size);
			LOG_SWAP_DWORD(p->line);
			LOG_SWAP_DWORD(p->threadid);
			if(ret != size)
			{
				pThis->stopLogToTCPSvr();//发送失败不再发送到TCP
			}
		}
		g_slBufferedLog.remove(p);

	}
	printf("Exit SLog::ThreadLog\n");
	g_slBufferedLog.clear();
	g_b_log_quited = true;
	delete[] buf;
	return NULL;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  记录到文件
// 作    者:  邵凯田
// 创建时间:  2011-11-18 22:32
// 参数说明:  @buff表示日志内容
//            @logtype表示日志级别
//            @sub_no表示日志子分类号，-1表示无子分类
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SLog::logFile(char *buff, int logtype, int sub_no)
{
	string pathfile = *m_sLogFile;
	if (sub_no >= 0)
	{
		static char str[16] = { 0 };
		sprintf(str, "_%d", sub_no);
		pathfile += str;
	}
	for (int i = 0; i <= 1; i++)
	{
		if(i==1)
		{
			pathfile += "."+g_log_type_desc[(int)logtype];
		}
		FILE *fp=fopen(pathfile.c_str(),"ab+");
		if(fp == /*NULL*/0)
			return;
		fseek(fp,0,SEEK_END);
		fwrite(buff,sizeof(char),strlen(buff),fp);
		long len = ftell(fp);
		fclose(fp);
		//文件是否超过最大长度?
		if(len > m_iMaxFileSize)
		{
			char tmpfile0[512];
			char tmpfile1[512];

			for(int t=2;t<=m_iMaxBackFiles;t++)
			{
				memset(tmpfile0,0,sizeof(tmpfile0));
				memset(tmpfile1,0,sizeof(tmpfile1));
				sprintf(tmpfile0,"%s.back%d.zip",pathfile.c_str(),t-1);
				sprintf(tmpfile1,"%s.back%d.zip",pathfile.c_str(),t);
				::remove(tmpfile0);
				::rename(tmpfile1,tmpfile0);
			}
			char cmd[1024];
			memset(cmd,0,sizeof(cmd));
#ifndef WIN32
			//tar zcvf X.zip X.back
			sprintf(cmd,"tar zcvf \"%s\" \"%s\" ",tmpfile1,pathfile.c_str());
			::system(cmd);
#else
			//是否winrar存在
			FILE *fp2 = fopen("C:\\Program Files\\WinRAR\\winrar.exe","rb");
			if(fp2 == 0)
			{
				printf("@ERROR, [C:\\Program Files\\WinRAR\\winrar.exe]不存在!将无法进行日志压缩!");
				::rename(pathfile.c_str(),tmpfile1);
			}
			else
			{
				fclose(fp2);
				//"C:\Program Files\WinRAR\winrar.exe" a c:\ysr_dgs_local.rar c:\ysr_dgs_local.log
				sprintf(cmd,"\"C:\\Program Files\\WinRAR\\winrar.exe\" a \"%s\" \"%s\" ",tmpfile1,pathfile.c_str());
				::system(cmd);
			}
#endif
			::remove(pathfile.c_str());
			/*
			//将文件复制到另一处
			string tmpfile = pathfile+".back";
			FILE *fptmp=fopen(tmpfile.c_str(),"wb");
			if(fptmp == NULL)
			{
				printf("@@@@@@@@@@ 创建交换文件失败，清除日志文件!");
				::remove(pathfile.c_str());
				return;
			}
			fp=fopen(pathfile.c_str(),"rb");
			if(fp == NULL)
			{
				printf("@@@@@@@@@@ 再次打开日志文件失败，清除日志文件!");
				::remove(pathfile.c_str());
				return;
			}
			BYTE buf[1024];
			int ret;
			//不减小文件 fseek(fp,(m_iMaxFileSize/10),SEEK_SET);
			while(1)
			{
				ret = fread(buf,1,1024,fp);
				if(ret <= 0)
					break;
				fwrite(buf,1,ret,fptmp);
			}
			fclose(fp);
			fclose(fptmp);
			::remove(pathfile.c_str());
			//文件复制完后，不改名，而是直接删除原文件，这样由下次创建文件，就不会需要经常复制文件了
			//::rename(tmpfile.c_str(),pathfile.c_str());		
			*/
		}
	}
}

void SLog::writeFileLine(char * filename,char *buff)
{
	FILE *fp=fopen(filename,"ab+");
	if(fp == /*NULL*/0)
		return;
	fseek(fp,0,SEEK_END);
	fwrite(buff,sizeof(char),strlen(buff),fp);
	fclose(fp);
}


//////////////////////////////////////////////////////////////////////////
// 描    述:  运行日志切换命令
// 作    者:  邵凯田
// 创建时间:  2014-6-12 9:29
// 参数说明:  @pStr为命令字符串
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SLog::RunLogCommand(char* pStr)
{
	string str = pStr;
	if(str == "log level none")
	{
		SLog::setLogLevel(SLog::LOG_NONE);
	}
	else if(str == "log level fault")
	{
		SLog::setLogLevel(SLog::LOG_FAULT);
	}
	else if(str == "log level error")
	{
		SLog::setLogLevel(SLog::LOG_ERROR);
	}
	else if(str == "log level warn")
	{
		SLog::setLogLevel(SLog::LOG_WARN);
	}
	else if(str == "log level debug")
	{
		SLog::setLogLevel(SLog::LOG_DEBUG);
	}
	else if(str == "log level basedebug")
	{
		SLog::setLogLevel(SLog::LOG_BASEDEBUG);
	}
	else if(str == "log to console on")
	{
		SLog::startLogToConsole();
	}
	else if(str == "log to console off")
	{
		SLog::stopLogToConsole();
	}
	else if(str.substr(0,12) == "log to file ")
	{
		SLog::startLogToFile((char*)str.substr(12).c_str());
	}
	else if(str.substr(0,14) == "log file size ")
	{
		SLog::setLogFileMaxSize(atoi(str.substr(14).c_str()));
	}
	else if(str.substr(0,19) == "log file back size ")
	{
		SLog::setLogFileMaxBackFiles(atoi(str.substr(19).c_str()));
	}
	else if(str == "log to file off")
	{
		SLog::stopLogToFile();
	}
	else if(str == "log to tcp off" || str == "log to TCP off")
	{
		SLog::stopLogToTCPSvr();
	}
	else if(str.substr(0,11) == "log to tcp " || str.substr(0,11) == "log to TCP ")
	{
		string ip = str.substr(11);
		int port=(int)ip.find(":");
		if(port > 0)
		{
			port = atoi(ip.substr(ip.find(":")+1).c_str());
			ip = ip.substr(0,ip.find(":"));
		}
		SLog::startLogToTCPSvr((char*)ip.c_str(),port);
	}
	else if(str == "log to udp off" || str == "log to UDP off")
	{
		SLog::stopLogToUDPSvr();
	}
	else if(str.substr(0,11) == "log to udp " || str.substr(0,11) == "log to UDP ")
	{
		string ip = str.substr(11);
		int port=(int)ip.find(":");
		if(port > 0)
		{
			port = atoi(ip.substr(ip.find(":")+1).c_str());
			ip = ip.substr(0,ip.find(":"));
		}
		SLog::startLogToUDPSvr((char*)ip.c_str(),port);
	}
}

void SLog::LoopForLogCommand()
{
	string sCmd;
	char ch;
	while(1)
	{
#ifdef WIN32
		Sleep(10);
#else
		usleep(10000);
#endif
		ch = getchar();
		if(ch == '\n' )
		{
			if(sCmd.length()>0)
			{
				if(sCmd == "quit")
					return;
				else if(sCmd == "help")
				{
					string sRet = "";
					sRet += "--------------------------------------------------------------\r\n";
					sRet += "日志管理程序控制台帮助信息\r\n";
					sRet += "--------------------------------------------------------------\r\n";
					sRet += "log level none       关闭日志功能                             \r\n";
					sRet += "log level fault      设置日志级别为严重故障级                 \r\n";
					sRet += "log level error      设置日志级别为一般错误级                 \r\n";
					sRet += "log level warn       设置日志级别为警告级                     \r\n";
					sRet += "log level debug      设置日志级别为调试级                     \r\n";
					sRet += "log level basedebug  设置日志级别为基类调试级                 \r\n";
					sRet += "--------------------------------------------------------------\r\n";
					sRet += "log to console on    输出日志到控制台                         \r\n";
					sRet += "log to console off   停止输出日志到控制台                     \r\n";
					sRet += "log to file <path>   输出日志到指定文件                       \r\n";
					sRet += "log to file off      停止日志到文件                           \r\n";
					sRet += "log file size <n>    指定日志文件最大长度                     \r\n";
					sRet += "log to TCP <ip:n>    输出日志到指定IP和端口的TCP服务器        \r\n";
					sRet += "log to TCP off       停止日志到TCP服务器                      \r\n";
					sRet += "log to UDP <ip:n>    输出日志到指定IP和端口的UDP服务器        \r\n";
					sRet += "log to UDP off       停止日志到UDP服务器                      \r\n";
					sRet += "--------------------------------------------------------------\r\n";
					sRet += "quit                 退出日志管理程序控制台\r\n";
					sRet += "\r\n";
					printf("%s",sRet.c_str());
				}
				RunLogCommand((char*)sCmd.c_str());				
				sCmd = "";
			}
			continue;
		}
		sCmd += ch;
		if(sCmd.length() > 200)
			sCmd = "";
	}
}
