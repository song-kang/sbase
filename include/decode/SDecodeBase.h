/**
 *
 * 文 件 名 : SDecoder.h
 * 创建日期 : 2015-5-7 16:35
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : 以太网报文解码器
 * 修改记录 : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-5-7	邵凯田　创建文件
 *
 **/

#ifndef __SKT_DECODER_BASE_H__
#define __SKT_DECODER_BASE_H__

#include "SDecodPub.h"
#include "SInclude.h"
#include "SHash.h"


//////////////////////////////////////////////////////////////////////////
// 名    称:  eSDecodeProtType
// 作    者:  邵凯田
// 创建时间:  2015-5-10 11:50
// 描    述:  解码协议类型
//////////////////////////////////////////////////////////////////////////
enum eSDecodeProtType
{
	SDECODE_PROT_UNKNOWN=0,
	SDECODE_PROT_SV92,
	SDECODE_PROT_SV91,
	SDECODE_PROT_GOOSE,
	SDECODE_PROT_MMS,		//TCP:102
	SDECODE_PROT_IEC104,	//TCP:2404
	SDECODE_PROT_PTP,
	SDECODE_PROT_SNTP,		//UDP:123
	SDECODE_PROT_GMRP,		
	SDECODE_PROT_TCP,
	SDECODE_PROT_UDP,
	SDECODE_PROT_ICMP,
	SDECODE_PROT_IP,
	SDECODE_PROT_ARP,
	SDECODE_PROT_ETH2,

	SDECODE_PROT_COUNT,		//协议总数量
};

//TCP会话中暂存缓冲区的类型
enum eSDecodeBufType
{
	SDECODE_BUF_DAT1=0,
	SDECODE_BUF_DAT2,
	SDECODE_BUF_DAT3,
	SDECODE_BUF_DAT4,
	SDECODE_BUF_DAT5,
	SDECODE_BUF_DAT6,
	SDECODE_BUF_DAT7,
	SDECODE_BUF_DAT8,

	SDECODE_BUF_COUNT
};

//MMS事件名称定义，下标与eSDecodeProtType对应(用下标访问时应注意下标不应大于SDECODE_PROT_COUNT)
const char g_sDecodeProtocolName[][32] =
{
	"未知",
	"SMV9-2",	//SDECODE_PROT_SV92,
	"SMV9-1",	//SDECODE_PROT_SV91,
	"GOOSE",	//SDECODE_PROT_GOOSE,
	"MMS",		//SDECODE_PROT_MMS,		//TCP:102
	"IEC104",	//SDECODE_PROT_IEC104,	//TCP:2404
	"PTP",		//SDECODE_PROT_PTP,
	"SNTP",		//SDECODE_PROT_SNTP,	//UDP:123
	"GMRP",		//SDECODE_PROT_GMRP,		
	"TCP",		//SDECODE_PROT_TCP,
	"UDP",		//SDECODE_PROT_UDP,
	"ICMP",		//SDECODE_PROT_ICMP,
	"IP",		//SDECODE_PROT_IP,
	"ARP",		//SDECODE_PROT_ARP,
	"ETH2",		//SDECODE_PROT_ETH2,


	"无效",
};
const char* SDecode_GetProtocolName(eSDecodeProtType idx);

//////////////////////////////////////////////////////////////////////////
// 名    称:  eSDecodeFmtError
// 作    者:  邵凯田
// 创建时间:  2015-5-10 11:50
// 描    述:  报文格式错误类型
//////////////////////////////////////////////////////////////////////////
enum eSDecodeFmtError
{
	SDECODE_FMT_NOERROR=0,	//格式无错误
	SDECODE_FMT_ETH2,		//EthernetII层编码错误
	SDECODE_FMT_IP,			//IP层编码错误
	SDECODE_FMT_TCP,		//TCP层编码错误
	SDECODE_FMT_UDP,		//UDP层编码错误
	SDECODE_FMT_TPKT,		//TIKT层编码错误
	SDECODE_FMT_8073,		//ISO8073(COTP)编码错误
	SDECODE_FMT_8327,		//ISO8327编码错误
	SDECODE_FMT_8823,		//ISO8823编码错误
	SDECODE_FMT_8650,		//ISO8650编码错误
	SDECODE_FMT_MMS,		//MMS(ISO9506)编码错误
};

//////////////////////////////////////////////////////////////////////////
// 名    称:  eSDecodeTcpStatus
// 作    者:  邵凯田
// 创建时间:  2015-5-10 12:20
// 描    述:  TCP会话的连接状态
//////////////////////////////////////////////////////////////////////////
enum eSDecodeTcpStatus
{
	SDECODE_TCP_UNKNOWN=0,		//未知
	SDECODE_TCP_CONNECTING,		//正在创建连接
	SDECODE_TCP_CONNECTED,		//连接已创建，正常通讯中
	SDECODE_TCP_DISCONNECTING,	//正在断开连接
	SDECODE_TCP_DISCONNECTED,	//连接已断开
};


#define DATATYPE_UCHAR		1
#define DATATYPE_UINT32		2
#define DATATYPE_UINT16		3
#define DATATYPE_INT32		4
#define DATATYPE_INT16		5
#define DATATYPE_FLOAT		6
#define DATATYPE_BOOL		7
#define DATATYPE_BYTE		8
#define DATATYPE_T			9
#define DATATYPE_SVDATA		10
#define DATATYPE_ASNLEN		11


const unsigned char DatSetType_STRUCT		= 0xa2;
const unsigned char DatSetType_INTEGER		= 0x85;
const unsigned char DatSetType_OSTRING		= 0x89;
const unsigned char DatSetType_UNSIGNED		= 0x86;
const unsigned char DatSetType_BITSTRING	= 0x84;
const unsigned char DatSetType_UTCTIME		= 0x91;
const unsigned char DatSetType_BOOLEAN		= 0x83;
const unsigned char DatSetType_FLOAT		= 0x87;

const unsigned char DataType_Array			= 0xA1;
const unsigned char DataType_Structure		= 0xA2;
const unsigned char DataType_Boolean		= 0x83;
const unsigned char DataType_Bit_string		= 0x84;
const unsigned char DataType_Integer		= 0x85;
const unsigned char DataType_Unsigned		= 0x86;
const unsigned char DataType_Floating_point	= 0x87;
const unsigned char DataType_Real			= 0x88;
const unsigned char DataType_Octet_string	= 0x89;
const unsigned char DataType_Visible_string	= 0x8A;
const unsigned char DataType_Binary_time	= 0x8C;
const unsigned char DataType_Bcd			= 0x8D;
const unsigned char DataType_BooleanArray	= 0x8E;
const unsigned char DataType_UTCTIME		= 0x91;
const unsigned char DataType_UNICODE		= 0x90;
const unsigned char DataType_FloatingPoint	= 0xA7;

class SDecodeFieldViewInfo
{
public:
	unsigned char			m_DatSetType;
	union
	{
		unsigned char *		pUCHAR;
		unsigned int *		pUINT32;
		unsigned short *	pUINT16;
		int *				pINT32;
		short *				pINT16;
		float *				pFLOAT;
		unsigned char *		pBOOL;
		unsigned char *		pBYTE;
	}m_Data;
	unsigned char	m_DataType;
	unsigned short	m_Data_off;
	unsigned short	m_Data_End;
	unsigned short	m_Data_len;
};

//////////////////////////////////////////////////////////////////////////
// 名    称:  stuSDecodeEvent
// 作    者:  邵凯田
// 创建时间:  2015-5-10 13:37
// 描    述:  解码事件定义
//////////////////////////////////////////////////////////////////////////
struct stuSDecodeEvent
{
	int m_iProtType;//协议类型
	short m_iEventCode;//事件编号，各协议事件编号均从1开始
	short m_iEventDataBytes;//事件详细数据结构的长度
	unsigned char m_pEventData[128];//事件详细数据结构，不同协议类型下的不同事件，分别定义自己的事件详细数据结构
};

//突发解码事件回调函数定义
typedef void (*OnDecodeEvent)(void* cbParam,stuSDecodeEvent *pEvent);


class SDecodeComplexFrameSet
{
public:
	SDecodeComplexFrameSet()
	{
	}
	~SDecodeComplexFrameSet()
	{
	}
	bool SearchIdx(int idx)
	{
		if(idx < m_iMinIdx || idx > m_iMaxIdx)
			return false;
		unsigned long pos;
		int *pId = m_FrameIdx.FetchFirst(pos);
		while(pId)
		{
			if(*pId = idx)
				return true;
			pId = m_FrameIdx.FetchNext(pos);
		}
		return false;
	}
	void AddIdx(int idx)
	{
		if(idx<m_iMinIdx)
			m_iMinIdx = idx;
		if(idx > m_iMaxIdx)
			m_iMaxIdx = idx;
		m_FrameIdx.append(idx);
	}

	int m_iMinIdx,m_iMaxIdx;//报文序号的最大、最小值
	SValueList<int> m_FrameIdx;//拼包报文序号队列
};


typedef void (*OnFreeSessionStatus)(BYTE *pStatus);

//////////////////////////////////////////////////////////////////////////
// 名    称:  SDecodeTcpSession
// 作    者:  邵凯田
// 创建时间:  2015-5-7 17:14
// 描    述:  TCP协议（含基于TCP的上层协议）的会话类
//////////////////////////////////////////////////////////////////////////
class SDecodeTcpSession
{
public:
	friend class SDecodeBase;
	SDecodeTcpSession();
	~SDecodeTcpSession();

	inline bool IsUnknownSC()				{return m_bUnknownSC;};
	inline unsigned int GetServerIp()		{return m_iServerIp;};
	inline unsigned short GetServerPort()	{return m_iServerPort;};
	inline unsigned int GetClientIp()		{return m_iClientIp;};
	inline unsigned short GetClientPort()	{return m_iClientPort;};
	inline time_t GetConnectSOC()			{return m_iConnectSOC;};
	inline time_t GetDisconnectSOC()		{return m_iDisconnectSOC;};
	inline BYTE* GetStatusBuf()				{return m_pStatusData;};
	inline int GetStatusBufLen()			{return m_iStatusLen;};
	inline int GetTotalFrames()				{return m_iTotalFrames;};
	inline int GetTotalBytes()				{return m_iTotalBytes;};
	inline void SetStatusBuf(BYTE *p,int l)	{m_pStatusData=p;m_iStatusLen=l;};
	inline void SetStatusBufFreeFun(OnFreeSessionStatus p){m_pFreeSessionstatusFun = p;};
	inline bool IsLastFrameS2C()			{return m_bLastFrameS2C;};

	//暂态缓冲区管理接口

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取TCP会话暂态数据缓冲区地址
	// 作    者:  邵凯田
	// 创建时间:  2015-5-20 10:55
	// 参数说明:  @buf_type表示缓冲区类型
	// 返 回 值:  BYTE*
	//////////////////////////////////////////////////////////////////////////
	inline BYTE* GetDatBuf(eSDecodeBufType buf_type)				{return m_pBufferData[buf_type];};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取暂存缓冲区长度
	// 作    者:  邵凯田
	// 创建时间:  2015-5-20 10:56
	// 参数说明:  @buf_type表示缓冲区类型
	// 返 回 值:  int
	//////////////////////////////////////////////////////////////////////////
	inline int GetDatBufLen(eSDecodeBufType buf_type)				{return m_iBufferLen[buf_type];};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  添加C2S缓存数据到数据缓冲区
	// 作    者:  邵凯田
	// 创建时间:  2015-5-10 22:59
	// 参数说明:  @buf_type表示缓冲区类型
	//         :  @p数据内容
	//         :  @l数据长度
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void AddToDatBuf(eSDecodeBufType buf_type,BYTE* p,int l);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  删除C2S缓冲区前部指定的字节
	// 作    者:  邵凯田
	// 创建时间:  2015-5-10 23:27
	// 参数说明:  @buf_type表示缓冲区类型
	//         :  @size为待删除的长度
	// 返 回 值:  true成功，false失败
	//////////////////////////////////////////////////////////////////////////
	bool RemoveDatBufFront(eSDecodeBufType buf_type,int size);

public:
#ifdef SDECODE_OFFLINE
	//复合报文记录，离线单帧解码时，记录所有的复合报文
	//SValueList<int> m_ComplexS2C,m_ComplexC2S;//两个通讯方向的复合报文序号
	int m_iNoComplexCnt;//没有复合报文的帧数，0表示有复合报文，需要拼包
	SPtrList<SDecodeComplexFrameSet> m_ComplexSet;
	SDecodeComplexFrameSet *m_pLastComplexSet;//最后的复合报文集指针，当没有复合报文是置NULL
	SDecodeComplexFrameSet* SearchComplexSet(int idx);//搜索复合报文集
	SDecodeComplexFrameSet* NewComplexSet(int idx);//新建复合报文集
	inline void SetComplexSetFlag()
	{
		m_iNoComplexCnt = 0;
	}
#endif

public:	
	int m_iHashKey;//散列键值（不唯一）
	//会话唯一标识
	//unsigned char m_bServerMac[6],m_bClientMac[6];//服务端、客户端的MAC地址
	int m_iServerIp,m_iClientIp;//服务端、客户端的IP地址，采用网络字节顺序
	unsigned short m_iServerPort,m_iClientPort;//服务端、客户端的TCP端口号，采用网络字节顺序（为快速匹配会话）

	int m_iConnectSOC;//建立连接的时间（世纪秒）
	int m_iDisconnectSOC;//结束连接的时间（世纪秒）
	BYTE *m_pStatusData;//状态数据，析构时释放
	int m_iStatusLen;//状态数据长度
	BYTE *m_pBufferData[SDECODE_BUF_COUNT];//缓存数据，析构时释放
	int m_iBufferLen[SDECODE_BUF_COUNT];//缓存数据长度（有效数据净长度)
	int m_iBufferMaxSize[SDECODE_BUF_COUNT];//缓存区总大小

	bool m_bUnknownSC;//是否不确定谁是服务端，谁是客户端；针对未得到连接握手报文时为true，表示服务端和客户端可能不正确（根据端口号大小猜的）
	bool m_bLastFrameS2C;//最后一次报文的方向是否为S->C?
	int m_iLastSOC;//最后一帧报文的时间

	eSDecodeTcpStatus m_iTcpStatus;//TCP会话连接状态
	int m_iTotalFrames;//当前会话的总报文数量
	int m_iTotalFrames_G;//当前会话的总报文数量(G)
	int m_iTotalBytes;//当前会话的累计报文总字节数量
	int m_iTotalBytes_G;//当前会话的累计报文总字节数量(G)
	unsigned int m_iLastServerSeqNo;//最后的服务端SEQ
	unsigned int m_iLastClientSeqNo;//最后的客户端SEQ
	OnFreeSessionStatus m_pFreeSessionstatusFun;
};

//////////////////////////////////////////////////////////////////////////
// 名    称:  SDecodeTree
// 作    者:  邵凯田
// 创建时间:  2015-5-7 17:38
// 描    述:  解码的结果树
//////////////////////////////////////////////////////////////////////////
class SDecodeTree
{
public:
	friend class SDecodeMgr;
	friend class SDecodeBase;
	SDecodeTree();
	~SDecodeTree();
	SPtrList<SDecodeTree>* GetChildsPtr(){return &m_Childs;};
	inline int GetNodeType(){return m_iNodeType;};
	inline char* GetNodeName(){return m_sNodeName.data();};
	inline char* GetNodeValue(){return m_sNodeValue.data();};
	inline char* GetNodeRemark(){return m_sNodeRemark.data();};
	inline BYTE* GetFrameBuffer(){return m_pFrameBuffer;};
	inline int GetDataOffset(){return m_iDataOffset;};
	inline int GetDataBytes(){return m_iDataBytes;};
	inline void AddNameText(char *str){m_sNodeName += str;};
	inline void AddValueText(char *str)
	{
		if(m_sNodeValue.length() > 0 && m_sNodeValue.left(1) != " ")
			m_sNodeValue += " ";
		m_sNodeValue += str;
	};

	void ClearChild();
	SDecodeTree* New(BYTE *pBuf,int iOffset,int iBytes,const char* sName,const char* sVal="",const char* sRemark="");
	SDecodeTree* NewMalformed(BYTE *pBuf,int iOffset,int iBytes,char *sDesc=NULL)
	{
		return New(pBuf,iOffset,iBytes,"[Malformed packets]",sDesc==NULL?"":sDesc,"畸形报文，无法继续向下解码");
	}
public:
	int m_iNodeType;//节点类型,由对应的协议确定
	SString m_sNodeName;//节点的描述文字
	SString m_sNodeValue;//节点值
	SString m_sNodeRemark;//节点的备注
	BYTE *m_pFrameBuffer;//报文首地址,NULL表示当前报文原码（未拼包)
	int m_iDataOffset;//报文的偏移字节数
	int m_iDataBytes;//报文的字节数
	SPtrList<SDecodeTree> m_Childs;//子节点
	SPtrList<SDecodeTree> *m_pTempTree;
};


//////////////////////////////////////////////////////////////////////////
// 名    称:  stuSDecodePreprocessInfo
// 作    者:  邵凯田
// 创建时间:  2015-5-8 17:58
// 描    述:  解码预处理信息指针
//////////////////////////////////////////////////////////////////////////
struct stuSDecodePreprocessInfo
{
	eSDecodeProtType m_iDecodeType;//协议类型
	unsigned char *m_pViewBuffer;//当前可视报文缓冲区，无拼包时为m_pFrameBuffer，否则为拼包缓冲区
	int m_iViewBytes;//可视报文长度
	unsigned char *m_pFrameBuffer;//当前报文缓冲区
	int m_iFrameBytes;//当前报文总长度
	int m_iFrameValidBytes;//当前报文有效长度（含IP层协议时可能存在IP层补齐的无效后导数据，不计算在此长度内，<=获取到报文的真实长度）
	SDECODE_VLAN_HEADER *m_pVlan;//VLAN结构,NULL表示无
	SDECODE_IP_HEADER *m_pIp;//IP结构,NULL表示无
	SDECODE_TCP_HEADER *m_pTcp;//TCP结构,NULL表示无
	SDECODE_UDP_HEADER *m_pUdp;//UDP结构,NULL表示无
	SDecodeTcpSession *m_pSession;//当前报文对应的TCP会话指针，NULL表示无TCP会话
	eSDecodeFmtError m_iFmtErrorCode;//预解码时的编码错误号，0表示无错误，编码错误的帧在预解码后不再进行处理
	int m_iStartPos;//协议对应解码起始位置
	//int m_iBytes;//协议对应有效数据长度
};

//////////////////////////////////////////////////////////////////////////
// 名    称:  SDecodeBase
// 作    者:  邵凯田
// 创建时间:  2015-5-7 17:00
// 描    述:  解码协议基类
//////////////////////////////////////////////////////////////////////////
class SDecodeBase
{
public:
	friend class SDecodeMgr;
	SDecodeBase();
	virtual ~SDecodeBase();

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取最后一次处理报文对应的TCP会话实例
	// 作    者:  邵凯田
	// 创建时间:  2015-5-8 8:42
	// 参数说明:  void
	// 返 回 值:  SDecodeTcpSession*，NULL表示没有对应的TCP会话
	//////////////////////////////////////////////////////////////////////////
	inline SDecodeTcpSession* GetLastSession(){return m_pLastSession;};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取TCP会话实例数量
	// 作    者:  邵凯田
	// 创建时间:  2015-5-8 8:43
	// 参数说明:  void
	// 返 回 值:  int,表示TCP会话的总数量
	//////////////////////////////////////////////////////////////////////////
	inline int GetSessionCount(){return m_SessionsList.count();};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取指定序号的TCP会话实例指针
	// 作    者:  邵凯田
	// 创建时间:  2015-5-8 8:43
	// 参数说明:  @i表示序号，从0开始编号
	// 返 回 值:  SDecodeTcpSession*，NULL表示没有对应序号的会话（可能已被删除）
	//////////////////////////////////////////////////////////////////////////
	inline SDecodeTcpSession* GetSession(int i){return m_SessionsList[i];};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取当前解码协议对应的协议类型
	// 作    者:  邵凯田
	// 创建时间:  2015-5-8 13:47
	// 参数说明:  void
	// 返 回 值:  eSDecodeProtType
	//////////////////////////////////////////////////////////////////////////
	inline eSDecodeProtType GetProtType()		{return m_ProtType;};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  解码一帧报文
	// 作    者:  邵凯田
	// 创建时间:  2015-5-9 22:27
	// 参数说明:  @soc表示报文时标（秒）
	//         :  @nsec表示报文时标（纳秒）
	// 返 回 值:  int,0表示解码成功，对应eSDecodeFmtError
	//////////////////////////////////////////////////////////////////////////
	virtual int DecodeFrame(int soc,int nsec);
	void AfterDecode();

protected:

	void SetDecodeTreeRootPtr(SDecodeTree *p){m_pDecodeTreeRoot = p;};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  设置事件处理的回调函数
	// 作    者:  邵凯田
	// 创建时间:  2015-5-10 14:20
	// 参数说明:  @OnDecodeEvent回调函数
	//         :  @cbParam回调时的第一个参数（通常用于回调函数区分不同的解码实例)
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void SetEventCallback(OnDecodeEvent pFun,void *cbParam){m_pEventCallbackFun = pFun;m_pEventCallbackParam = cbParam;};


	//////////////////////////////////////////////////////////////////////////
	// 描    述:  搜索TCP会话信息
	// 作    者:  邵凯田
	// 创建时间:  2015-5-8 18:35
	// 参数说明:  @soc为报文时间(世纪秒)
	//         :  @bCreateNew表示如不存在时是否创建新的会话实例,true表示创建，false表示不创建
	// 返 回 值:  SDecodeTcpSession*为会话指针，NULL表示不存在
	//////////////////////////////////////////////////////////////////////////
	SDecodeTcpSession* SearchTcpSession(int soc,bool bCreateNew);

	void SetPreprocessInfoPtr(stuSDecodePreprocessInfo *p){m_pPreprocessInfo = p;};


	//////////////////////////////////////////////////////////////////////////
	// 描    述:  清除过期无效的TCP连接
	// 作    者:  邵凯田
	// 创建时间:  2015-5-10 12:56
	// 参数说明:  @soc表示当前时间
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void RemoveInvalidTcpSession(int soc);

protected:
	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取TCP会话标记的散列值
	// 作    者:  邵凯田
	// 创建时间:  2015-5-8 18:58
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	static int TcpSession_Key(void *cbParam,void *pItem);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  精度比较两个会话是否相同
	// 作    者:  邵凯田
	// 创建时间:  2015-5-8 18:58
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	static bool TcpSession_Compare(void *cbParam,void *pItem1,void *pItem2);

	void InitFieldViewInfo(SDecodeFieldViewInfo * pViewInfo, unsigned char * pbuf, unsigned char datatype, unsigned short dataoff, unsigned short dataend, unsigned short datalen, unsigned char datsettype, bool inval);
public:
	SGeneralHash<SDecodeTcpSession,TcpSession_Key,TcpSession_Compare> m_TcpSessions;//所有的TCP会话列表
	SPtrList<SDecodeTcpSession> m_SessionsList;//所有的TCP会话，队列形式
	SPtrList<SDecodeTcpSession> m_RemovedSessions;//所有已断开且没有释放的TCP会话，（长时间没有报文交互的TCP会话也进入删除队列）
	SDecodeTcpSession* m_pLastSession;//最后处理报文对应的会话，NULL表示无会话；
	eSDecodeProtType m_ProtType;//当前解码协议对应的类型号
	stuSDecodePreprocessInfo *m_pPreprocessInfo;//预解码结果指针，实例化时即指针，中途不再改变
	stuSDecodeEvent m_LastEvent;//最后发送的事件
	SDecodeTree *m_pDecodeTreeRoot;//解码结果输出树的根节点，NULL表示解码时不需要生成结构树
	OnDecodeEvent m_pEventCallbackFun;
	void *m_pEventCallbackParam;

#ifdef SDECODE_OFFLINE
	int *m_piCurrFrameIdx;//当前报文序号的指针
#endif
};

#endif//__SKT_DECODER_BASE_H__

