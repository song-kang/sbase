/**
 *
 * 文 件 名 : PcapFile.h
 * 创建日期 : 2015-5-8 13:27
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : PCAP文件读取类
 * 修改记录 : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-5-8	邵凯田　创建文件
 *
 **/
#ifndef __SKT_DECODE_PCAP_FILE_H__
#define __SKT_DECODE_PCAP_FILE_H__
#include "SInclude.h"
#include <string>
#include <vector>
using namespace std;

typedef unsigned int bpf_u_int32;
typedef unsigned short u_short;
typedef int	bpf_int32;

struct ys_pcap_file_header {
	bpf_u_int32 magic;
	u_short version_major;
	u_short version_minor;
	bpf_int32 thiszone;	/* gmt to local correction */
	bpf_u_int32 sigfigs;	/* accuracy of timestamps */
	bpf_u_int32 snaplen;	/* max length saved portion of each pkt */
	bpf_u_int32 linktype;	/* data link type (LINKTYPE_*) */
};


struct ys_timeval {
	long    tv_sec;         /* seconds */
	long    tv_usec;        /* and microseconds */
};

struct ys_pcap_pkthdr {
	struct ys_timeval ts;	/* time stamp */
	bpf_u_int32 caplen;	/* length of portion present */
	bpf_u_int32 len;	/* length this packet (off wire) */
};


struct pcap_dthdr 
{
	ys_pcap_pkthdr pp;
	BYTE dstaddr[6];
	BYTE srcaddr[6];
	WORD vlan;
	WORD priflag;
	WORD enettype;
	int  nDataPos;
};

struct pcap_dtinfo 
{
	pcap_dtinfo()
	{
		pData = NULL;
		buflen = 0;
	}
	~pcap_dtinfo()
	{
		if(pData != NULL)
			delete[] pData;
	}
	int	   sec;
	int    usec;
	int	   len;		//包含地址字段
	BYTE*  pData;	
	int    buflen;//缓冲区大小
};

struct Ysp_Data_Info 
{
	union lf
	{
		BYTE byte[4];
		DWORD dw;
	}f;
	DWORD soc;
	DWORD nSec;
};

struct Ysp_File_Header 
{
	BYTE unt;
	BYTE crd;
	BYTE chn;
	BYTE unused;
	DWORD soc_from;
	DWORD nsec_from;
	DWORD soc_to;
	DWORD nsec_to;
	DWORD unused2;
};

struct Ysp_Index 
{
	DWORD soc;
	DWORD nsec;
	DWORD offset;
};

class CPcapFile  
{
public:
	CPcapFile();
	explicit CPcapFile(string sFileName);
	explicit CPcapFile(const char* sFileName);
	~CPcapFile();
	bool OpenPcapFile(string sFileName);
	bool OpenPcapFile(const char* sFileName);
	int	 GetFileLength() const {return m_nFileLen;}
	ys_pcap_file_header* GetFileHeader() const {return m_pHeader;};
	bool GetNext(pcap_dtinfo* pd);
	bool GetFirst(pcap_dtinfo* pd);
	bool GetCurrent(pcap_dtinfo* pd);
	bool GetLast(pcap_dtinfo* pd);
	void ClosePcapFile();
	bool TransFromYspToPcap(string sSrcFile,string sDstFile);
	bool TransToYsp(string sDstFile="");
	operator FILE*()const {return m_fpFile;}

	//add by skt 不预加载文件方式的快速读取方法，不可与原方法交叉使用
	bool OpenEx(char* sFileName);
	void GotoFirstEx();//跳转到第一条记录的位置
	bool ReadFrameEx(unsigned char* &pBuffer,int &iLen,int &iBufLen);
	bool CloseEx();
	ys_pcap_pkthdr m_pp;//当前当前记录的头信息

	//add by skt 直接读取一个文件到内存缓冲区，然后可从此缓冲区读取记录
	bool OpenToBuffer(char* sFileName,unsigned char* &pAllBuffer,int &iAllLen);
	bool ReadFrameFromBuffer(unsigned char* pAllBuffer,int iAllLen,int &pos,int &soc,int &usec,unsigned char* &pBuffer,int &iLen);

private:
	int  GetFileLen();
	//未被打开的文件
	int  GetFileLen(string sFile);
	//已打开的文件
	int  GetFileLen(FILE* fp);
	void Reset();
	void MakeHeader(ys_pcap_file_header& pfh);
private:
	ys_pcap_file_header*	m_pHeader;
	vector<pcap_dthdr>  m_vDthdt;
	string m_sFile;
	FILE*  m_fpFile;
	int	   m_nFileLen;
	int	   m_nCurr;
};

#endif //__SKT_DECODE_PCAP_FILE_H__
