/**
 *
 * �� �� �� : SDecoder.h
 * �������� : 2015-5-7 16:35
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : ��̫�����Ľ�����
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-5-7	�ۿ�������ļ�
 *
 **/

#ifndef __SKT_DECODER_BASE_H__
#define __SKT_DECODER_BASE_H__

#include "SDecodPub.h"
#include "SInclude.h"
#include "SHash.h"


//////////////////////////////////////////////////////////////////////////
// ��    ��:  eSDecodeProtType
// ��    ��:  �ۿ���
// ����ʱ��:  2015-5-10 11:50
// ��    ��:  ����Э������
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

	SDECODE_PROT_COUNT,		//Э��������
};

//TCP�Ự���ݴ滺����������
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

//MMS�¼����ƶ��壬�±���eSDecodeProtType��Ӧ(���±����ʱӦע���±겻Ӧ����SDECODE_PROT_COUNT)
const char g_sDecodeProtocolName[][32] =
{
	"δ֪",
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


	"��Ч",
};
const char* SDecode_GetProtocolName(eSDecodeProtType idx);

//////////////////////////////////////////////////////////////////////////
// ��    ��:  eSDecodeFmtError
// ��    ��:  �ۿ���
// ����ʱ��:  2015-5-10 11:50
// ��    ��:  ���ĸ�ʽ��������
//////////////////////////////////////////////////////////////////////////
enum eSDecodeFmtError
{
	SDECODE_FMT_NOERROR=0,	//��ʽ�޴���
	SDECODE_FMT_ETH2,		//EthernetII��������
	SDECODE_FMT_IP,			//IP��������
	SDECODE_FMT_TCP,		//TCP��������
	SDECODE_FMT_UDP,		//UDP��������
	SDECODE_FMT_TPKT,		//TIKT��������
	SDECODE_FMT_8073,		//ISO8073(COTP)�������
	SDECODE_FMT_8327,		//ISO8327�������
	SDECODE_FMT_8823,		//ISO8823�������
	SDECODE_FMT_8650,		//ISO8650�������
	SDECODE_FMT_MMS,		//MMS(ISO9506)�������
};

//////////////////////////////////////////////////////////////////////////
// ��    ��:  eSDecodeTcpStatus
// ��    ��:  �ۿ���
// ����ʱ��:  2015-5-10 12:20
// ��    ��:  TCP�Ự������״̬
//////////////////////////////////////////////////////////////////////////
enum eSDecodeTcpStatus
{
	SDECODE_TCP_UNKNOWN=0,		//δ֪
	SDECODE_TCP_CONNECTING,		//���ڴ�������
	SDECODE_TCP_CONNECTED,		//�����Ѵ���������ͨѶ��
	SDECODE_TCP_DISCONNECTING,	//���ڶϿ�����
	SDECODE_TCP_DISCONNECTED,	//�����ѶϿ�
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
// ��    ��:  stuSDecodeEvent
// ��    ��:  �ۿ���
// ����ʱ��:  2015-5-10 13:37
// ��    ��:  �����¼�����
//////////////////////////////////////////////////////////////////////////
struct stuSDecodeEvent
{
	int m_iProtType;//Э������
	short m_iEventCode;//�¼���ţ���Э���¼���ž���1��ʼ
	short m_iEventDataBytes;//�¼���ϸ���ݽṹ�ĳ���
	unsigned char m_pEventData[128];//�¼���ϸ���ݽṹ����ͬЭ�������µĲ�ͬ�¼����ֱ����Լ����¼���ϸ���ݽṹ
};

//ͻ�������¼��ص���������
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

	int m_iMinIdx,m_iMaxIdx;//������ŵ������Сֵ
	SValueList<int> m_FrameIdx;//ƴ��������Ŷ���
};


typedef void (*OnFreeSessionStatus)(BYTE *pStatus);

//////////////////////////////////////////////////////////////////////////
// ��    ��:  SDecodeTcpSession
// ��    ��:  �ۿ���
// ����ʱ��:  2015-5-7 17:14
// ��    ��:  TCPЭ�飨������TCP���ϲ�Э�飩�ĻỰ��
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

	//��̬����������ӿ�

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡTCP�Ự��̬���ݻ�������ַ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-5-20 10:55
	// ����˵��:  @buf_type��ʾ����������
	// �� �� ֵ:  BYTE*
	//////////////////////////////////////////////////////////////////////////
	inline BYTE* GetDatBuf(eSDecodeBufType buf_type)				{return m_pBufferData[buf_type];};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ�ݴ滺��������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-5-20 10:56
	// ����˵��:  @buf_type��ʾ����������
	// �� �� ֵ:  int
	//////////////////////////////////////////////////////////////////////////
	inline int GetDatBufLen(eSDecodeBufType buf_type)				{return m_iBufferLen[buf_type];};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���C2S�������ݵ����ݻ�����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-5-10 22:59
	// ����˵��:  @buf_type��ʾ����������
	//         :  @p��������
	//         :  @l���ݳ���
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void AddToDatBuf(eSDecodeBufType buf_type,BYTE* p,int l);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ɾ��C2S������ǰ��ָ�����ֽ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-5-10 23:27
	// ����˵��:  @buf_type��ʾ����������
	//         :  @sizeΪ��ɾ���ĳ���
	// �� �� ֵ:  true�ɹ���falseʧ��
	//////////////////////////////////////////////////////////////////////////
	bool RemoveDatBufFront(eSDecodeBufType buf_type,int size);

public:
#ifdef SDECODE_OFFLINE
	//���ϱ��ļ�¼�����ߵ�֡����ʱ����¼���еĸ��ϱ���
	//SValueList<int> m_ComplexS2C,m_ComplexC2S;//����ͨѶ����ĸ��ϱ������
	int m_iNoComplexCnt;//û�и��ϱ��ĵ�֡����0��ʾ�и��ϱ��ģ���Ҫƴ��
	SPtrList<SDecodeComplexFrameSet> m_ComplexSet;
	SDecodeComplexFrameSet *m_pLastComplexSet;//���ĸ��ϱ��ļ�ָ�룬��û�и��ϱ�������NULL
	SDecodeComplexFrameSet* SearchComplexSet(int idx);//�������ϱ��ļ�
	SDecodeComplexFrameSet* NewComplexSet(int idx);//�½����ϱ��ļ�
	inline void SetComplexSetFlag()
	{
		m_iNoComplexCnt = 0;
	}
#endif

public:	
	int m_iHashKey;//ɢ�м�ֵ����Ψһ��
	//�ỰΨһ��ʶ
	//unsigned char m_bServerMac[6],m_bClientMac[6];//����ˡ��ͻ��˵�MAC��ַ
	int m_iServerIp,m_iClientIp;//����ˡ��ͻ��˵�IP��ַ�����������ֽ�˳��
	unsigned short m_iServerPort,m_iClientPort;//����ˡ��ͻ��˵�TCP�˿ںţ����������ֽ�˳��Ϊ����ƥ��Ự��

	int m_iConnectSOC;//�������ӵ�ʱ�䣨�����룩
	int m_iDisconnectSOC;//�������ӵ�ʱ�䣨�����룩
	BYTE *m_pStatusData;//״̬���ݣ�����ʱ�ͷ�
	int m_iStatusLen;//״̬���ݳ���
	BYTE *m_pBufferData[SDECODE_BUF_COUNT];//�������ݣ�����ʱ�ͷ�
	int m_iBufferLen[SDECODE_BUF_COUNT];//�������ݳ��ȣ���Ч���ݾ�����)
	int m_iBufferMaxSize[SDECODE_BUF_COUNT];//�������ܴ�С

	bool m_bUnknownSC;//�Ƿ�ȷ��˭�Ƿ���ˣ�˭�ǿͻ��ˣ����δ�õ��������ֱ���ʱΪtrue����ʾ����˺Ϳͻ��˿��ܲ���ȷ�����ݶ˿ںŴ�С�µģ�
	bool m_bLastFrameS2C;//���һ�α��ĵķ����Ƿ�ΪS->C?
	int m_iLastSOC;//���һ֡���ĵ�ʱ��

	eSDecodeTcpStatus m_iTcpStatus;//TCP�Ự����״̬
	int m_iTotalFrames;//��ǰ�Ự���ܱ�������
	int m_iTotalFrames_G;//��ǰ�Ự���ܱ�������(G)
	int m_iTotalBytes;//��ǰ�Ự���ۼƱ������ֽ�����
	int m_iTotalBytes_G;//��ǰ�Ự���ۼƱ������ֽ�����(G)
	unsigned int m_iLastServerSeqNo;//���ķ����SEQ
	unsigned int m_iLastClientSeqNo;//���Ŀͻ���SEQ
	OnFreeSessionStatus m_pFreeSessionstatusFun;
};

//////////////////////////////////////////////////////////////////////////
// ��    ��:  SDecodeTree
// ��    ��:  �ۿ���
// ����ʱ��:  2015-5-7 17:38
// ��    ��:  ����Ľ����
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
		return New(pBuf,iOffset,iBytes,"[Malformed packets]",sDesc==NULL?"":sDesc,"���α��ģ��޷��������½���");
	}
public:
	int m_iNodeType;//�ڵ�����,�ɶ�Ӧ��Э��ȷ��
	SString m_sNodeName;//�ڵ����������
	SString m_sNodeValue;//�ڵ�ֵ
	SString m_sNodeRemark;//�ڵ�ı�ע
	BYTE *m_pFrameBuffer;//�����׵�ַ,NULL��ʾ��ǰ����ԭ�루δƴ��)
	int m_iDataOffset;//���ĵ�ƫ���ֽ���
	int m_iDataBytes;//���ĵ��ֽ���
	SPtrList<SDecodeTree> m_Childs;//�ӽڵ�
	SPtrList<SDecodeTree> *m_pTempTree;
};


//////////////////////////////////////////////////////////////////////////
// ��    ��:  stuSDecodePreprocessInfo
// ��    ��:  �ۿ���
// ����ʱ��:  2015-5-8 17:58
// ��    ��:  ����Ԥ������Ϣָ��
//////////////////////////////////////////////////////////////////////////
struct stuSDecodePreprocessInfo
{
	eSDecodeProtType m_iDecodeType;//Э������
	unsigned char *m_pViewBuffer;//��ǰ���ӱ��Ļ���������ƴ��ʱΪm_pFrameBuffer������Ϊƴ��������
	int m_iViewBytes;//���ӱ��ĳ���
	unsigned char *m_pFrameBuffer;//��ǰ���Ļ�����
	int m_iFrameBytes;//��ǰ�����ܳ���
	int m_iFrameValidBytes;//��ǰ������Ч���ȣ���IP��Э��ʱ���ܴ���IP�㲹�����Ч�����ݣ��������ڴ˳����ڣ�<=��ȡ�����ĵ���ʵ���ȣ�
	SDECODE_VLAN_HEADER *m_pVlan;//VLAN�ṹ,NULL��ʾ��
	SDECODE_IP_HEADER *m_pIp;//IP�ṹ,NULL��ʾ��
	SDECODE_TCP_HEADER *m_pTcp;//TCP�ṹ,NULL��ʾ��
	SDECODE_UDP_HEADER *m_pUdp;//UDP�ṹ,NULL��ʾ��
	SDecodeTcpSession *m_pSession;//��ǰ���Ķ�Ӧ��TCP�Ựָ�룬NULL��ʾ��TCP�Ự
	eSDecodeFmtError m_iFmtErrorCode;//Ԥ����ʱ�ı������ţ�0��ʾ�޴��󣬱�������֡��Ԥ������ٽ��д���
	int m_iStartPos;//Э���Ӧ������ʼλ��
	//int m_iBytes;//Э���Ӧ��Ч���ݳ���
};

//////////////////////////////////////////////////////////////////////////
// ��    ��:  SDecodeBase
// ��    ��:  �ۿ���
// ����ʱ��:  2015-5-7 17:00
// ��    ��:  ����Э�����
//////////////////////////////////////////////////////////////////////////
class SDecodeBase
{
public:
	friend class SDecodeMgr;
	SDecodeBase();
	virtual ~SDecodeBase();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ���һ�δ����Ķ�Ӧ��TCP�Ựʵ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-5-8 8:42
	// ����˵��:  void
	// �� �� ֵ:  SDecodeTcpSession*��NULL��ʾû�ж�Ӧ��TCP�Ự
	//////////////////////////////////////////////////////////////////////////
	inline SDecodeTcpSession* GetLastSession(){return m_pLastSession;};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡTCP�Ựʵ������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-5-8 8:43
	// ����˵��:  void
	// �� �� ֵ:  int,��ʾTCP�Ự��������
	//////////////////////////////////////////////////////////////////////////
	inline int GetSessionCount(){return m_SessionsList.count();};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡָ����ŵ�TCP�Ựʵ��ָ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-5-8 8:43
	// ����˵��:  @i��ʾ��ţ���0��ʼ���
	// �� �� ֵ:  SDecodeTcpSession*��NULL��ʾû�ж�Ӧ��ŵĻỰ�������ѱ�ɾ����
	//////////////////////////////////////////////////////////////////////////
	inline SDecodeTcpSession* GetSession(int i){return m_SessionsList[i];};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��ǰ����Э���Ӧ��Э������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-5-8 13:47
	// ����˵��:  void
	// �� �� ֵ:  eSDecodeProtType
	//////////////////////////////////////////////////////////////////////////
	inline eSDecodeProtType GetProtType()		{return m_ProtType;};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����һ֡����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-5-9 22:27
	// ����˵��:  @soc��ʾ����ʱ�꣨�룩
	//         :  @nsec��ʾ����ʱ�꣨���룩
	// �� �� ֵ:  int,0��ʾ����ɹ�����ӦeSDecodeFmtError
	//////////////////////////////////////////////////////////////////////////
	virtual int DecodeFrame(int soc,int nsec);
	void AfterDecode();

protected:

	void SetDecodeTreeRootPtr(SDecodeTree *p){m_pDecodeTreeRoot = p;};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����¼�����Ļص�����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-5-10 14:20
	// ����˵��:  @OnDecodeEvent�ص�����
	//         :  @cbParam�ص�ʱ�ĵ�һ��������ͨ�����ڻص��������ֲ�ͬ�Ľ���ʵ��)
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void SetEventCallback(OnDecodeEvent pFun,void *cbParam){m_pEventCallbackFun = pFun;m_pEventCallbackParam = cbParam;};


	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����TCP�Ự��Ϣ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-5-8 18:35
	// ����˵��:  @socΪ����ʱ��(������)
	//         :  @bCreateNew��ʾ�粻����ʱ�Ƿ񴴽��µĻỰʵ��,true��ʾ������false��ʾ������
	// �� �� ֵ:  SDecodeTcpSession*Ϊ�Ựָ�룬NULL��ʾ������
	//////////////////////////////////////////////////////////////////////////
	SDecodeTcpSession* SearchTcpSession(int soc,bool bCreateNew);

	void SetPreprocessInfoPtr(stuSDecodePreprocessInfo *p){m_pPreprocessInfo = p;};


	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���������Ч��TCP����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-5-10 12:56
	// ����˵��:  @soc��ʾ��ǰʱ��
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void RemoveInvalidTcpSession(int soc);

protected:
	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡTCP�Ự��ǵ�ɢ��ֵ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-5-8 18:58
	// ����˵��:  
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	static int TcpSession_Key(void *cbParam,void *pItem);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���ȱȽ������Ự�Ƿ���ͬ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-5-8 18:58
	// ����˵��:  
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	static bool TcpSession_Compare(void *cbParam,void *pItem1,void *pItem2);

	void InitFieldViewInfo(SDecodeFieldViewInfo * pViewInfo, unsigned char * pbuf, unsigned char datatype, unsigned short dataoff, unsigned short dataend, unsigned short datalen, unsigned char datsettype, bool inval);
public:
	SGeneralHash<SDecodeTcpSession,TcpSession_Key,TcpSession_Compare> m_TcpSessions;//���е�TCP�Ự�б�
	SPtrList<SDecodeTcpSession> m_SessionsList;//���е�TCP�Ự��������ʽ
	SPtrList<SDecodeTcpSession> m_RemovedSessions;//�����ѶϿ���û���ͷŵ�TCP�Ự������ʱ��û�б��Ľ�����TCP�ỰҲ����ɾ�����У�
	SDecodeTcpSession* m_pLastSession;//������Ķ�Ӧ�ĻỰ��NULL��ʾ�޻Ự��
	eSDecodeProtType m_ProtType;//��ǰ����Э���Ӧ�����ͺ�
	stuSDecodePreprocessInfo *m_pPreprocessInfo;//Ԥ������ָ�룬ʵ����ʱ��ָ�룬��;���ٸı�
	stuSDecodeEvent m_LastEvent;//����͵��¼�
	SDecodeTree *m_pDecodeTreeRoot;//������������ĸ��ڵ㣬NULL��ʾ����ʱ����Ҫ���ɽṹ��
	OnDecodeEvent m_pEventCallbackFun;
	void *m_pEventCallbackParam;

#ifdef SDECODE_OFFLINE
	int *m_piCurrFrameIdx;//��ǰ������ŵ�ָ��
#endif
};

#endif//__SKT_DECODER_BASE_H__

