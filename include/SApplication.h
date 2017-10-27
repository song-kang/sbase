/**
 *
 * 文 件 名 : SApplication.h
 * 创建日期 : 2009-8-20 13:36
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: 2011/08/31 08:43:32 $
 * 当前版本 : $Revision: 1.1 $
 * 功能描述 : 应用程序基础类，含各种应用管理及代理客户端
 * 修改记录 : 
 *            $Log: SApplication.h,v $
 *            Revision 1.1  2011/08/31 08:43:32  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.8  2011/04/27 02:58:48  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.7  2011/04/23 10:53:52  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.6  2011/04/22 06:32:33  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.5  2011/04/21 02:14:16  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.4  2011/04/11 12:08:34  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2011/03/29 07:07:00  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2011/03/29 01:03:26  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2011/02/25 03:12:05  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2010/11/17 02:19:15  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2010/08/09 03:04:39  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2009/09/15 02:14:09  shaokt
 *            通迅记录分析装置项目首次提交
 *
 *
 **/

#ifndef __SKT_APPLICATION_H__
#define __SKT_APPLICATION_H__

#include "SApi.h"
#include "SFile.h"
#include "SLog.h"
#include "SString.h"
#include "STcpClientBase.h"

class SApplication;

#define C_SP_UNITAGENT_PORT 6666

#define SBASE_SAPP SApplication::GetPtr()

#ifndef _WITHOUT_AGENT_CHANNEL_

//////////////////////////////////////////////////////////////////////////
// 名    称:  eSpUnitAgentMsg
// 作    者:  邵凯田
// 创建时间:  2016-2-3 17:20
// 描    述:  代理消息类型定义
//////////////////////////////////////////////////////////////////////////
enum SBASE_EXPORT eSpUnitAgentMsg
{
	SP_UA_MSG_REGISTER=1,		//注册代理信息
	SP_UA_MSG_REG_AGENT,		//注册代理信息（用于代理间互联）
	SP_UA_MSG_SEND_OK,			//消息成功发送
	SP_UA_MSG_SEND_ERR,			//消息发送失败
	SP_UA_MSG_PS,				//取单元上的进程列表(代理单元)
	SP_UA_MSG_REBOOT,			//单元重启(代理单元)
	SP_UA_MSG_HALT,				//单元关机(代理单元)
	SP_UA_MSG_REQ_GET_UNIT,		//取所有的代理单元列表(代理单元)
	SP_UA_MSG_RES_GET_UNIT,		//取所有的代理单元列表(代理单元)回复
	SP_UA_MSG_REQ_GET_PROCINFO,	//取指定单元所有应用的信息列表
	SP_UA_MSG_RES_GET_PROCINFO,	//取指定单元所有应用的信息列表回复
	SP_UA_MSG_REQ_GET_PROCID,	//取指定应用号及实例号的应用ID(代理单元)
	SP_UA_MSG_RES_GET_PROCID,	//取指定应用号及实例号的应用ID(代理单元)回复
	SP_UA_MSG_REQ_LS,			//取代理主机上的目录信息(代理单元)
	SP_UA_MSG_RES_LS,			//取代理主机上的目录信息(代理单元)回复
	SP_UA_MSG_REQ_GET_FILE,		//读取文件(代理单元)
	SP_UA_MSG_RES_GET_FILE,		//读取文件(代理单元)回复
	SP_UA_MSG_REQ_PUT_FILE,		//上传文件(代理单元)
	SP_UA_MSG_RES_PUT_FILE,		//上传文件(代理单元)回复
	SP_UA_MSG_REQ_DEL_FILE,		//删除文件(代理单元)
	SP_UA_MSG_RES_DEL_FILE,		//删除文件(代理单元)回复
	SP_UA_MSG_RUN_CMD,			//运行命令(代理单元)
	SP_UA_MSG_PROC_TXT_CMD_REQ,	//应用进程的文本命令请求
	SP_UA_MSG_PROC_TXT_CMD_ACK,	//应用进程的文本命令应答
	SP_UA_MSG_APP_STOP,			//应用进程退出
	//SP_UA_CMD_PROX,		//代理

	SP_UA_MSG_USER=101,		//应用层自定义的起始命令号
};

enum SBASE_EXPORT eSpUnitAgentMsgResult
{
	SP_UA_MSG_RESULT_FAIL = 0,
	SP_UA_MSG_RESULT_SUCCESS
};

//////////////////////////////////////////////////////////////////////////
// 名    称:  eSpUnitAgentApplicationNo
// 作    者:  邵凯田
// 创建时间:  2016-2-3 17:23
// 描    述:  应用号定义
//////////////////////////////////////////////////////////////////////////
enum SBASE_EXPORT eSpUnitAgentApplicationNo
{
	SP_UA_APPNO_AGENT=1,	//代理进程
	SP_UA_APPNO_WATCHDOG,	//看门狗进程
	SP_UA_APPNO_SHIELD_IN,	//内网侧正向隔离同步进程
	SP_UA_APPNO_SHIELD_OUT,	//外网侧正向隔离同步进程

	SP_UA_APPNO_USER=101,	//应用层自定义的起始应用号
};

#pragma pack(push)  
#pragma pack(1)

//////////////////////////////////////////////////////////////////////////
// 名    称:  stuSpUnitAgentUnitInfo
// 作    者:  邵凯田
// 创建时间:  2016-2-3 17:27
// 描    述:  单元信息结构定义
//////////////////////////////////////////////////////////////////////////
struct SBASE_EXPORT stuSpUnitAgentUnitInfo
{
	DWORD m_iUnitId;//单元号
	char m_sSoftType[32];
	char m_sUnitName[32];
	char m_sUnitDesc[64];
	DWORD m_iUnitIp[8];
};

//////////////////////////////////////////////////////////////////////////
// 名    称:  stuSpUnitAgentProcessId
// 作    者:  邵凯田
// 创建时间:  2016-2-2 13:22
// 描    述:  单元内进程标识结构
//////////////////////////////////////////////////////////////////////////
struct SBASE_EXPORT stuSpUnitAgentProcessId
{
	DWORD m_iUnitId;			//2.1.1.1单元标识
	//单元可以通过整型的单元号作为唯一标识，也可以通过单元的IP地址为唯一标识；
	//单元标识为4字节整数，网络通讯采用网络字节顺序，其中：0表示广播地址，<=0xffff表示单元编号（1~0xffff），>0xffff表示单元的IP地址；
	//当使用IP为单元标识时，缺省使用第一个IP地址为标识，但该IP不一定作为通讯IP使用
	DWORD m_iApplicationId;		//高两字节为应用号，由应用层定义，0:表示代理进程，1~100由软件平台保留，101~由其它由应用层定义
								//低两字节为应用实例号，针对同一应用号下不同实例下的实例号，该字段针对特定应用有效，无效时置0

	inline void SetApplicationId(WORD appid,WORD instid=0){m_iApplicationId = ((DWORD)appid<<16) | instid;};
	inline WORD GetApplicationId(){return (WORD)(m_iApplicationId>>16);};
	inline WORD GetAppInstId(){return (WORD)(m_iApplicationId&0xffff);};
	inline void Swap()
	{
		SKT_SWAP_DWORD(m_iUnitId);
		SKT_SWAP_DWORD(m_iApplicationId);
	}
};

//////////////////////////////////////////////////////////////////////////
// 名    称:  stuSpUnitAgentMsgHead
// 作    者:  邵凯田
// 创建时间:  2016-2-2 16:34
// 描    述:  单元通讯头结构定义
//////////////////////////////////////////////////////////////////////////
struct SBASE_EXPORT stuSpUnitAgentMsgHead
{
	stuSpUnitAgentProcessId m_SrcId;	//源进程ID
	stuSpUnitAgentProcessId m_DstId;	//目标进程ID
	DWORD m_dwMsgSn;						//命令序号
	DWORD m_dwParamReault;				//表示参数或返回值，针对请求报文为参数，针对响应报文为返回值，返回值：0:失败，1:成功，其它应用层自定义

	inline void Swap()
	{
		m_SrcId.Swap();
		m_DstId.Swap();
		SKT_SWAP_DWORD(m_dwMsgSn);
		SKT_SWAP_DWORD(m_dwParamReault);
	}
};

//////////////////////////////////////////////////////////////////////////
// 名    称:  stuModuleInfo
// 作    者:  邵凯田
// 创建时间:  2011-3-29 9:53
// 描    述:  描述信息结构
//////////////////////////////////////////////////////////////////////////
struct SBASE_EXPORT stuSpUnitAgentProcInfo
{
	stuSpUnitAgentProcessId process_id;	//进程ID
	DWORD pid;							//进程号
	char name[32];						//进程名称，WINDOWS平台不需要带.exe
	char version[32];					//版本号,不需要‘V’
	char desc[64];						//进程描述
	char crc[16];						//CRC校验码
	char build_time[32];				//程序生成时间
	DWORD begin_time;					//进程开始运行时间
	char login_ip[24];					//登录IP地址
	DWORD login_port;					//登录端口号
	DWORD login_time;					//登录时间
	inline void Swap()
	{
		process_id.Swap();
		SKT_SWAP_DWORD(begin_time);
		SKT_SWAP_DWORD(login_port);
		SKT_SWAP_DWORD(login_time);
	}
};

struct SBASE_EXPORT stuSpUnitAgentLsReq
{
	char path[128];//目录全路径，{@APP_PATH@}表示应用程序根目录，可以直接访问根目录
	char filter[32];//过滤条件，空表示不过滤
};

struct SBASE_EXPORT stuSpUnitAgentLsRes//Result中存放文件数量
{
	bool is_dir;//是否为目录
	char name[128];//文件名或目录名
	int file_size;//文件大小，字节，目录时为0
};


struct SBASE_EXPORT stuSpUnitAgentGetFileReq
{
	char path_file[256];//源文件名全路径，{@APP_PATH@}表示应用程序根目录
};

//struct stuSpUnitAgentGetFileRes
//回复时直接回复文件内容到ASDU中，并在Result中存入分片序号（分片序号从1开始，0表示最后一片，即结束帧）




#pragma pack(4)
#pragma pack(pop)  

class SApplication;
class SBASE_EXPORT CSpAgentClient : public STcpClientBase
{
public:
	CSpAgentClient();
	virtual ~CSpAgentClient();

	bool StartAgent(bool bBlock);
	bool StopAgent();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  以服务形式启动客户端，不能与Connect函数同时使用
	// 作    者:  邵凯田
	// 创建时间:  2016-2-3 09:52
	// 参数说明:  @ip表示服务端IP地址，@port表示端口
	// 返 回 值:  true表示服务启动成功,false表示服务启动失败
	//////////////////////////////////////////////////////////////////////////
	virtual bool Start(SString ip,int port);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  代理消息处理接口，由派生类实现，处理函数必须尽量短小，快速返回
	// 作    者:  邵凯田
	// 创建时间:  2016-2-3 15:20
	// 参数说明:  @wMsgType表示消息类型
	//         :  @pMsgHead为消息头
	//         :  @sHeadStr消息头字符串
	//         :  @pBuffer为消息携带的二进制数据内容，NULL表示无二进制数据
	//         :  @iLength为二进制数据长度
	// 返 回 值:  true表示处理成功，false表示处理失败或未处理
	//////////////////////////////////////////////////////////////////////////
	virtual bool ProcessAgentMsg(WORD wMsgType,stuSpUnitAgentMsgHead *pMsgHead,SString &sHeadStr,BYTE* pBuffer=NULL,int iLength=0)=0;

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  处理文本命令
	// 作    者:  邵凯田
	// 创建时间:  2016-2-13 15:12
	// 参数说明:  @sCmd为命令内容，空字符串表示取表命令列表
	//         :  @sResult为返回结果，纯文本
	// 返 回 值:  true表示成功，false表示失败
	//////////////////////////////////////////////////////////////////////////
	virtual bool ProcessTxtCmd(SString &sCmd,SString &sResult){S_UNUSED(sCmd);S_UNUSED(sResult);return false;};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  通讯数据包解释
	// 作    者:  邵凯田
	// 创建时间:  2016-2-4 17:29
	// 参数说明:  @pPackage为源数据包
	//         :  @pMsgHead
	// 返 回 值:  true表示包合法，false表示不合法
	//////////////////////////////////////////////////////////////////////////
	bool PackageExplain(stuSTcpPackage *pPackage,WORD &wMsgType, stuSpUnitAgentMsgHead* &pMsgHead,BYTE* &pBuffer,int &iLength);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  构建通讯数据包
	// 作    者:  邵凯田
	// 创建时间:  2016-2-4 18:18
	// 参数说明:  @pMsgHead为消息头
	//         :  @pBuffer为消息携带的二进制数据内容，NULL表示无二进制数据
	//         :  @iLength为二进制数据长度
	// 返 回 值:  stuSTcpPackage*
	//////////////////////////////////////////////////////////////////////////
	stuSTcpPackage* PackageBuild(WORD wMsgType,stuSpUnitAgentMsgHead *pMsgHead,BYTE* pBuffer=NULL,int iLength=0);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  登录上服务端后的回调
	// 作    者:  邵凯田
	// 创建时间:  2016-2-2 22:25
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void OnLogin();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  会话线程接收到报文后的回调虚函数，派生类通过此函数处理即时消息
	// 作    者:  邵凯田
	// 创建时间:  2016-2-3 09:35
	// 参数说明:  @pSession为会话实例指针，可以通过指针向对端发送数据
	//            @pPackage为刚接收到的数据包
	// 返 回 值:  true表示已经处理完毕，此报文可以释放了，false表示未处理，此报文应放入接收队列
	//////////////////////////////////////////////////////////////////////////
	virtual bool OnRecvFrame(stuSTcpPackage *pPackage);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取唯一的消息编号
	// 作    者:  邵凯田
	// 创建时间:  2016-2-3 15:35
	// 参数说明:  void
	// 返 回 值:  WORD
	//////////////////////////////////////////////////////////////////////////
	static DWORD NewMsgSn();

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取所有的代理单元列表
	// 作    者:  邵凯田
	// 创建时间:  2016-2-3 17:31
	// 参数说明:  @units为引用返回的单元信息
	// 返 回 值:  返回单元数量,<0表示失败
	//////////////////////////////////////////////////////////////////////////
	int GetUnitInfo(SPtrList<stuSpUnitAgentUnitInfo> &units);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取指定单元号对应的进程信息
	// 作    者:  邵凯田
	// 创建时间:  2016-2-12 8:53
	// 参数说明:  @iUnitId表示进程标识，0表示当前单元
	//         :  @ProcInfos为引用返回的进程信息列表
	// 返 回 值:  >=0表示进程数量,<0表示失败
	//////////////////////////////////////////////////////////////////////////
	int GetProcessInfo(int iUnitId,SPtrList<stuSpUnitAgentProcInfo> &ProcInfos);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取当前单元中指定应用号及实例号对应的进程标识，只返回第一个匹配进程标识
	// 作    者:  邵凯田
	// 创建时间:  2016-2-3 17:11
	// 参数说明:  @wAppId表示应用号
	//         :  @wAppInstId表示应用实例号，不是多实例应用时设0
	//         :  @pProcIds为标识指针，读取到的标识将写入该指针对应的内存区
	// 返 回 值:  返回进程标识数量,<0表示失败
	//////////////////////////////////////////////////////////////////////////
	int GetProcessId(WORD wAppId,WORD wAppInstId,SPtrList<stuSpUnitAgentProcessId> &ProcIds);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取指定单元的文件列表
	// 作    者:  邵凯田
	// 创建时间:  2016-2-20 13:28
	// 参数说明:  @iUnitId为单元标识
	//         :  @sPath为待读取的目录名，其中{@APP_PATH@}表示应用程序根目录
	//         :  @sFilter为过滤条件，不过滤为空或*
	//         :  @items为返回的文件或目录项队列
	//         :  @表示超时时间，单位为秒
	// 返 回 值:  >=0表示项目数量，<0表示失败
	//////////////////////////////////////////////////////////////////////////
	int GetUnitDir(int iUnitId,SString sPath,SString sFilter,SPtrList<stuSpUnitAgentLsRes> &items,int iTimeout_s=5);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取指定单元的文件内容，并保存到本地文件
	// 作    者:  邵凯田
	// 创建时间:  2016-2-20 13:30
	// 参数说明:  @iUnitId为单元标识
	//         :  @sPathFile表示请求文件的全路径，其中{@APP_PATH@}表示应用程序根目录
	//         :  @sLocalPathFile表示本地保存的全路径
	//         :  @表示超时时间，单位为秒
	// 返 回 值:  >=0表示文件的大小,<0表示失败
	//////////////////////////////////////////////////////////////////////////
	int GetUnitFile(int iUnitId,SString sPathFile, SString sLocalPathFile,int iTimeout_s=5);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  从本地文件读取文件内容并上传到指定单元的指定目录文件位置
	// 作    者:  邵凯田
	// 创建时间:  2016-2-20 13:31
	// 参数说明:  @iUnitId表示单元标识 
	//         :  @sPathFile为目标文件的全路径，其中{@APP_PATH@}表示应用程序根目录
	//         :  @sLocalPathFile表示本地源文件的全路径
	//         :  @表示超时时间，单位为秒
	// 返 回 值:  >=0表示上传成功，<0表示上传失败
	//////////////////////////////////////////////////////////////////////////
	int PutUnitFile(int iUnitId,SString sPathFile,SString sLocalPathFile,int iTimeout_s=5);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  从指定单元删除指定的文件
	// 作    者:  邵凯田
	// 创建时间:  2016-2-20 13:33
	// 参数说明:  @iUnitId为单元标识
	//         :  @sPathFile为待删除的文件全路径，其中{@APP_PATH@}表示应用程序根目录
	//         :  @表示超时时间，单位为秒
	// 返 回 值:  >=0表示删除成功，<0表示删除失败
	//////////////////////////////////////////////////////////////////////////
	int DelUnitFile(int iUnitId,SString sPathFile,int iTimeout_s=5);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  立即发送代理消息，已加锁，不能应用于OnRecvFrame函数内
	// 作    者:  邵凯田
	// 创建时间:  2016-2-3 16:05
	// 参数说明:  @pToProcId为目标进程标识
	//         :  @wMsgType为消息编号,101以上为应用层消息
	//         :  @dwMsgSn为消息唯一编号，可通过其确定应答报文
	//         :  @dwParamReault为参数或返回值
	//         :  @pAsdu为应用层ASDU内容，NULL表示无ASDU
	//         :  @iAsduLen为ASDU长度
	//         :  @psHeadStr为消息头字符串指针，NULL表示无字符串
	// 返 回 值:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool SendAgentMsg(stuSpUnitAgentProcessId *pToProcId,WORD wMsgType,DWORD dwMsgSn,DWORD dwParamReault=0,BYTE *pAsdu=NULL,int iAsduLen=0,SString *psHeadStr=NULL);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  立即发送代理消息并在指定时间内接收回复
	// 作    者:  邵凯田
	// 创建时间:  2016-2-3 16:52
	// 参数说明:  @pToProcId为目标进程标识
	//         :  @wMsgType为消息编号,101以上为应用层消息
	//         :  @dwMsgSn为消息唯一编号，可通过其确定应答报文
	//         :  @dwParamReault为参数或返回值
	//         :  @pAsdu为应用层ASDU内容，NULL表示无ASDU
	//         :  @iAsduLen为ASDU长度
	//         :  @psHeadStr为消息头字符串指针，NULL表示无字符串
	//         :  @wRecvMsgType为希望接收到的响应报文类型
	//         :  @pRecvPackage为接收到的响应报文，调用者应负责释放
	//         :  @iTimeOut_s为接收超时时间，单位为秒
	// 返 回 值:  true表示发送并接收回复成功，false表示发送、或接收回复失败
	//////////////////////////////////////////////////////////////////////////
	bool SendAgentMsgAndRecv(stuSpUnitAgentProcessId *pToProcId,WORD wMsgType,DWORD dwMsgSn,DWORD dwParamReault,BYTE *pAsdu,int iAsduLen,SString *psHeadStr,WORD wRecvMsgType,stuSTcpPackage* &pRecvPackage,int iTimeOut_s=10);

	stuSpUnitAgentProcInfo m_SelfProcId;//当前进程的唯一标识
	inline void SetAppPtr(SApplication *pApp){m_pApp = pApp;};

private:
	static void* ThreadAgentMsgProc(void *lp);
	SPtrList<stuSTcpPackage> m_RecvPackage_Processed;//已经处理过，但不能马上删除的报文队列，其它线程可能需要从中找响应报文

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  查找指定条件的接收报文,含已处理和未处理的报文
	// 作    者:  邵凯田
	// 创建时间:  2016-2-20 13:39
	// 参数说明:  @bOnlyProcessed为true时表示仅从已处理的队列中提取命令
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	stuSTcpPackage* SearchRecvPackage_Processed(DWORD sn,WORD wMsgType,WORD wMsgType2=0,bool bOnlyProcessed=false);
public:
	SApplication *m_pApp;
	DWORD m_iBeginSOC;//模块开始运行时间
	SString m_sSoftwareType;//软件型号，用于代理客户及代理间拉手的认证，不同系统名称不允许建立联系
	SString m_sAgentIp;//代理IP地址，缺省为127.0.0.1
	int m_iAgentPort;//代理端口
	bool m_bAgentUplink;//是否为单元间的级联会话
};

#if 0
//////////////////////////////////////////////////////////////////////////
// 名    称:  CAgentPackage
// 作    者:  邵凯田
// 创建时间:  2011-3-29 9:54
// 描    述:  代理通道
//////////////////////////////////////////////////////////////////////////
struct SBASE_EXPORT CAgentPackage
{
public:
	CAgentPackage()
	{
		pBuffer = NULL;
		iLength = 0;
	}
	virtual ~CAgentPackage()
	{
		if(pBuffer != NULL)
		{
			delete[] pBuffer;
			pBuffer = NULL;
			iLength = 0;
		}
	}
	SString sMsg;
	BYTE *pBuffer;
	int iLength;
};

//////////////////////////////////////////////////////////////////////////
// 名    称:  stuModuleInfo
// 作    者:  邵凯田
// 创建时间:  2011-3-29 9:53
// 描    述:  描述信息结构
//////////////////////////////////////////////////////////////////////////
struct SBASE_EXPORT stuModuleInfo
{
	int handle;//句柄
	int pid;//进程号
	SString name;//模块名
	SString version;//版本号
	SString desc;//模块描述
	SString crc;//模块CRC
	SString compile_time;//编译时间
	SString login_ip;//登录运行IP
	int login_port;//登录运行端口
	SDateTime login_time;//登录时间
};



class SBASE_EXPORT CSAgentClient
{
public:
	CSAgentClient();
	~CSAgentClient();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  接收代理消息
	// 作    者:  邵凯田
	// 创建时间:  2010-7-1 16:59
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	static int ReceiveMsg(SSocket *pSocket,SString &sMsg,BYTE* &pBuffer,int &iLength);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  发送代理消息
	// 作    者:  邵凯田
	// 创建时间:  2010-7-1 16:59
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	static int SendMsg(SSocket *pSocket,SString &sMsg,BYTE* pBuffer=NULL,int iLength=0);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  向代理进程发送消息
	// 作    者:  邵凯田
	// 创建时间:  2010-7-1 16:56
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	void AddMsgToAgent(SString &sMsg,BYTE* pBuffer=NULL,int iLength=0)
	{
		CAgentPackage *p = new CAgentPackage();
		p->sMsg = sMsg;
		p->pBuffer = pBuffer;
		p->iLength = iLength;
		m_SendList.append(p);
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  提取当前已经登录到代理的进程模块列表信息
	// 作    者:  邵凯田
	// 创建时间:  2011-3-29 9:55
	// 参数说明:  引用返回的列表
	// 返 回 值:  >=0表示进程数量,<0表示失败
	//////////////////////////////////////////////////////////////////////////
	int GetModuleList(SPtrList<stuModuleInfo> &module_list);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取代理服务端的运行信息
	// 作    者:  邵凯田
	// 创建时间:  2012-9-26 11:44
	// 参数说明:  @sDisk 表示需要读取的硬盘信息，disks=3;disk1=/dev/sda1;disk2=/dev/sda2;disk3=/dev/sda3;
	//            @sRunInfo引用返回的以字符串描述的运行信息,act=runinfo;cpu_used=%d;mem_all=%d;mem_used=%d;mem_free=%d;disk1_all=%d;disk1_used=%d;disk1_free=%d;disk1_per=%d;
	// 返 回 值:  1表示成功，0表示失败
	//////////////////////////////////////////////////////////////////////////
	bool GetRunInfo(SString sDisk,SString &sRunInfo);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取指定模块的在代理进程上的句柄
	// 作    者:  邵凯田
	// 创建时间:  2011-3-29 14:42
	// 参数说明:  模块名称，即进程名称，WIN32平台上不需要带.exe
	// 返 回 值:  =0表示模块未找到，>0表示进程句柄
	//////////////////////////////////////////////////////////////////////////
	int GetModuleHandle(SString sModName);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  向指定的模块发送一个消息
	// 作    者:  邵凯田
	// 创建时间:  2011-3-29 11:06
	// 参数说明:  handle为目标模块的句柄
	// 返 回 值:  发送成功返回ture，失败返回false
	//////////////////////////////////////////////////////////////////////////
	bool AddMsgToModule(int handle,SString &sMsg,BYTE* pBuffer=NULL,int iLength=0);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  基类处理代理消息
	// 作    者:  邵凯田
	// 创建时间:  2010-7-1 16:58
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	bool ProcessAgentMsgBase(SString &sMsg,BYTE* pBuffer=NULL,int iLength=0);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  派生类处理代理消息
	// 作    者:  邵凯田
	// 创建时间:  2010-7-1 16:58
	// 参数说明:  @from_handle为消息来源模块的句柄(0表示消息来自代理进程)
	//         :  @sMsg为消息字符串
	//         :  @pBuffer为消息携带的二进制数据内容，NULL表示无二进制数据
	//         :  @iLength为二进制数据长度
	// 返 回 值:  true表示处理成功，false表示处理失败
	//////////////////////////////////////////////////////////////////////////
	virtual bool ProcessAgentMsg(int from_handle,SString &sMsg,BYTE* pBuffer=NULL,int iLength=0)=0;

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  代理线程，执行代理任务，线程运行时不占用Run函数，也可直接运行
	// 作    者:  邵凯田
	// 创建时间:  2012-5-30 11:00
	// 参数说明:  @lp：this
	// 返 回 值:  NULL
	//////////////////////////////////////////////////////////////////////////
	static void* ThreadAgent(void* lp);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  启动代理服务(对于人机界面程序，如启用代理，则应在Start函数中主窗口exec前调用本函数，并将参数设为false,即非阻塞)
	// 作    者:  邵凯田
	// 创建时间:  2012-5-31 10:13
	// 参数说明:  @bBlock为true表示阻塞运行，直到程序退出，false表示不阻塞启动线程运行代理服务
	// 返 回 值:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool StartAgent(bool bBlock);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  重新连接到指定的代理服务器
	// 作    者:  邵凯田
	// 创建时间:  2014-7-28 14:24
	// 参数说明:  @ip新代理服务器的IP地址
	//         :  @port为新代理服务器的TCP端口号，一般为6666
	// 返 回 值:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool ReConnectAgentSvr(SString ip,int port=C_SP_UNITAGENT_PORT);

public:
	bool m_bLogined;//是否已经登录
	SString m_sAgentIp;//代理IP地址，缺省为127.0.0.1
	int m_iAgentPort;//代理端口
	SPtrList<CAgentPackage> m_SendList;
	SString m_sWaitMsgAct;//等待消息的act标志
	SString m_sWaitMsg;//等待消息的返回字符串
	int m_iCpuPercent;
	SApplication *m_pApp;
	int m_iThreads;//线程数量
	SSocket *m_pAgentSocket;//当前通讯的SOCKET指针
};
#endif//if 0

#endif//ifdef _WITHOUT_AGENT_CHANNEL_

//////////////////////////////////////////////////////////////////////////
// 名    称:  SApplication
// 作    者:  邵凯田
// 创建时间:  2008-10-10 14:16
// 描    述:  应用程序管理类
//////////////////////////////////////////////////////////////////////////
#ifndef _WITHOUT_AGENT_CHANNEL_
class SBASE_EXPORT SApplication : public CSpAgentClient
{
public:
	friend class CSpAgentClient;
#else
class SBASE_EXPORT SApplication
{
public:
#endif
	SApplication();
	virtual ~SApplication();

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取SSApplication的实例指针
	// 作    者:  邵凯田
	// 创建时间:  2015-12-16 13:27
	// 参数说明:  void
	// 返 回 值:  SApplication*,NULL表示尚未实例
	//////////////////////////////////////////////////////////////////////////
	static SApplication* GetPtr();
	
	////////////////////////////////////////////////////////////////////////
	// 描    述:  启动应用程序
	// 作    者:  邵凯田
	// 创建时间:  2008-10-10 14:17
	// 参数说明:  @argc/argv为main输入参数
	//            @szDate/szTime为编译日期时间
	//            @bBlock为是否阻塞运行，true为阻塞，false为不阻塞
	// 返 回 值:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool Run(int argc, char* argv[],const char* szDate,const char* szTime,bool bBlock=true);
	
	////////////////////////////////////////////////////////////////////////
	// 描    述:  启动服务，开启所有应用的服务，该函数必须由派生类实现，且启动服务完成后必须返回
	// 作    者:  邵凯田
	// 创建时间:  2010-7-1 9:15
	// 参数说明:  
	// 返 回 值:  null
	//////////////////////////////////////////////////////////////////////////
	virtual bool Start()=0;

	////////////////////////////////////////////////////////////////////////
	// 描    述:  停止服务，关闭所有应用的服务，
	// 作    者:  邵凯田
	// 创建时间:  2010-7-1 9:18
	// 参数说明:  
	// 返 回 值:  null
	//////////////////////////////////////////////////////////////////////////
	virtual bool Stop()=0;

	////////////////////////////////////////////////////////////////////////
	// 描    述:  设置模块版本号
	// 作    者:  邵凯田
	// 创建时间:  2009-5-19 10:56
	// 参数说明:  sVer(采用:<产品内部型号>.<阶段版本号>.<模块版本号>)
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void SetVersion(SString sVer);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  设置模块描述
	// 作    者:  邵凯田
	// 创建时间:  2009-5-19 12:29
	// 参数说明:  sDesc描述
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void SetModuleDesc(SString sDesc);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  设置模块信息
	// 作    者:  邵凯田
	// 创建时间:  2009-5-19 12:29
	// 参数说明:  sInfo = name=;ver=;crc=;date=
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void SetModuleInfo(SString sInfo);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  设置应用程序标识
	// 作    者:  邵凯田
	// 创建时间:  2016-2-3 8:49
	// 参数说明:  @iAppId表示应用号
	//         :  @iInstId表示实例ID,0表示单实例应用
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void SetApplicationId(DWORD iAppId,DWORD iInstId=0);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  退出模块
	// 作    者:  邵凯田
	// 创建时间:  2011-3-29 8:44
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	void Quit()
	{
		m_bHalt = true;
// 		SLog::quitLog();
// 		SLog::WaitForLogEmpty();
	}
	inline bool IsDebugMode(){return m_bDebugMode;};
	inline bool IsLogToConsole(){return m_bLogToConsole;};
	bool IsParam(SString sParamName);
	bool IsParam(SString sParamName,SString &sParamValue);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取命令行参数内容
	// 作    者:  邵凯田
	// 创建时间:  2014-5-19 10:13
	// 参数说明:  @sParamName为参数名
	// 返 回 值:  参数值
	//////////////////////////////////////////////////////////////////////////
	SString GetParam(SString sParamName);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取命令行整型参数内容
	// 作    者:  邵凯田
	// 创建时间:  2014-5-19 10:13
	// 参数说明:  @sParamName为参数名
	// 返 回 值:  参数值
	//////////////////////////////////////////////////////////////////////////
	int GetParamI(SString sParamName);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取命令行浮点型参数内容
	// 作    者:  邵凯田
	// 创建时间:  2014-5-19 10:13
	// 参数说明:  @sParamName为参数名
	// 返 回 值:  参数值
	//////////////////////////////////////////////////////////////////////////
	float GetParamF(SString sParamName);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取系统根目录,已包含结尾的分隔符
	// 作    者:  邵凯田
	// 创建时间:  2014-7-31 15:44
	// 参数说明:  void
	// 返 回 值:  SString
	//////////////////////////////////////////////////////////////////////////
	virtual SString GetHomePath(){return m_sHomePath;};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取配置路径,已包含结尾的分隔符
	// 作    者:  邵凯田
	// 创建时间:  2014-7-31 15:29
	// 参数说明:  void
	// 返 回 值:  SString
	//////////////////////////////////////////////////////////////////////////
	virtual SString GetConfPath(){return m_sConfPath;};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取运行文件路径,已包含结尾的分隔符
	// 作    者:  邵凯田
	// 创建时间:  2014-7-31 15:29
	// 参数说明:  void
	// 返 回 值:  SString
	//////////////////////////////////////////////////////////////////////////
	virtual SString GetBinPath(){return m_sExePath;};

	inline SString GetModule(){return m_sModule;};
	inline SString GetAppbaseVer(){return m_sAppbaseVer;};
	inline SString GetModuleVer(){return m_sModuleVer;};
	inline SString GetModuleCrc(){return m_sModuleCrc;};
	inline SString GetModuleTime(){return m_sModuleTime;};
	inline SString GetModuleDesc(){return m_sModuleDesc;};
	inline void SetFork(bool b){m_bFork = b;};
	inline SString GetInfo(){return m_sModuleInfo;};
	inline void SetInfo(SString info){m_sModuleInfo = info;};
#ifndef _WITHOUT_AGENT_CHANNEL_
	inline void SetUseAgent(bool buse){m_bUseAgent = buse;};
#endif

	static bool ParseCommandLine(SString str);

	inline bool IsStarted(){return m_bStarted;};
	inline void SetStarted(bool b){m_bStarted=b;};
private:
	
	bool TouchClose();//创建一个本进程的close文件，以通知原进程关闭
	bool RemoveClose();//移除close文件
	bool IsClosedExist();//close文件是否存在？
	bool SearchOldProcess(int &pid,int &ppid,SString &name);

	bool _Start();
	bool _Stop();

	SString m_sExePath;//执行程序所在路径
	SString m_sConfPath;//配置文件路径
	SString m_sHomePath;//系统根目录绝对路径
	SString m_sModule;//模块名
	SString m_sAppbaseVer;//基类库版本
	SString m_sModuleVer;//模块版本
	SString m_sModuleCrc;//模块CRC
	SString m_sModuleTime;//模块生成时间
	SString m_sModuleDesc;//模块描述
	SString m_sModuleInfo;//模块信息
	bool m_bHalt;//是否准备退出?

	bool m_bDebugMode;//是否处于调试模式
	bool m_bLogToConsole;//是否允许日志信息输出到控制台
	bool m_bFork;//LINUX下是否执行fork
	bool m_bStarted;//是否已经启动应用
	bool m_bUseAgent;//是否开启单元代理服务

public:
	
	DWORD m_iApplicationId;
	DWORD m_iAppInstId;
	DWORD m_iUnitId;//
	SString m_sUnitName;
	SString m_sUnitDesc;
	int m_iArgc;
	char** m_ppArgv;
	void* m_pUserPtr;//用户指针，由应用层使用
};



#endif//__YS_REC_APPLICATION_H__
