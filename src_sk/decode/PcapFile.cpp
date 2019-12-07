/**
 *
 * 文 件 名 : PcapFile.cpp
 * 创建日期 : 2015-5-8 13:30
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
#include "PcapFile.h"
#include <ctime>
#include <sstream>
using namespace std;

CPcapFile::CPcapFile()
{
	m_pHeader = NULL;
	m_fpFile  = NULL;
	m_nCurr   = 0;
}

CPcapFile::CPcapFile( string sFileName )
{
	m_pHeader = NULL;
	m_fpFile  = NULL;
	m_nCurr	  = 0;
	OpenPcapFile(sFileName);
}

CPcapFile::CPcapFile( const char* sFileName )
{
	m_pHeader = NULL;
	m_fpFile  = NULL;
	m_nCurr	  = 0;
	OpenPcapFile(sFileName);
}

CPcapFile::~CPcapFile()
{
	Reset();
	ClosePcapFile();
}

bool CPcapFile::OpenEx(char* sFileName)
{
	m_fpFile = fopen(sFileName,"rb");
	if (m_fpFile == NULL) 
		return false;
	m_nFileLen = GetFileLen();
	int nRead = 0;
	if (m_pHeader == NULL)
		m_pHeader = new ys_pcap_file_header;
	int ret = fread(m_pHeader,1,sizeof(ys_pcap_file_header),m_fpFile);
	if (ret != sizeof(ys_pcap_file_header)) 
		return false;
	return true;
}
void CPcapFile::GotoFirstEx()//跳转到第一条记录的位置
{
	fseek(m_fpFile,sizeof(ys_pcap_file_header),SEEK_SET);
}
bool CPcapFile::ReadFrameEx(unsigned char* &pBuffer,int &iLen,int &iBufLen)
{
	//读取pcap文件头
	int ret = fread(&m_pp,1,sizeof(ys_pcap_pkthdr),m_fpFile);
	if (ret != sizeof(ys_pcap_pkthdr))
		return false;
	if(iBufLen < (int)m_pp.caplen)
	{
		if(pBuffer != NULL)
			delete[] pBuffer;
		pBuffer = new unsigned char[m_pp.caplen];
		iBufLen = iLen = m_pp.caplen;
	}
	else
		iLen = m_pp.caplen;
	ret = fread(pBuffer,1,iLen,m_fpFile);
	if(ret != iLen)
		return false;
	return true;
}
bool CPcapFile::CloseEx()
{
	if(m_fpFile)
	{
		fclose(m_fpFile);
		m_fpFile = NULL;
	}
	return true;
}

//add by skt 直接读取一个文件到内存缓冲区，然后可从此缓冲区读取记录
bool CPcapFile::OpenToBuffer(char* sFileName,unsigned char* &pAllBuffer,int &iAllLen)
{
	m_fpFile = fopen(sFileName,"rb");
	if (m_fpFile == NULL) 
		return false;
	m_nFileLen = GetFileLen();
	if(m_nFileLen <= sizeof(ys_pcap_file_header))
	{
		CloseEx();
		return false;
	}
	pAllBuffer = new unsigned char[m_nFileLen];
	if(pAllBuffer == NULL)
	{
		CloseEx();
		return false;
	}
	iAllLen = m_nFileLen;
	int left = iAllLen;
	int pos=0;
	while(left > 0)
	{
		int len = left;
		if(len > 1024*1024)
			len = 1024*1024;
		int ret = fread(pAllBuffer+pos,1,len,m_fpFile);
		if(ret != len)
		{
			delete[] pAllBuffer;
			pAllBuffer = NULL;
			CloseEx();
			return false;
		}
		pos += len;
		left -= len;
	}

	CloseEx();

	if(((ys_pcap_file_header*)pAllBuffer)->version_major == 0x200)
	{
		int pos = sizeof(ys_pcap_file_header);
		while(pos < iAllLen)
		{
			ys_pcap_pkthdr *pHead = (ys_pcap_pkthdr*)&pAllBuffer[pos];
			pHead->ts.tv_sec = ntohl(pHead->ts.tv_sec);
			pHead->ts.tv_usec = ntohl(pHead->ts.tv_usec);
			pHead->caplen = ntohl(pHead->caplen);

			pos += sizeof(ys_pcap_pkthdr)+pHead->caplen;
		}
	}


	return true;

}

bool CPcapFile::ReadFrameFromBuffer(unsigned char* pAllBuffer,int iAllLen,int &pos,int &soc,int &usec,unsigned char* &pBuffer,int &iLen)
{
	if(pos == 0)
	{
		//跳过ys_pcap_file_header
		pos = sizeof(ys_pcap_file_header);		
	}
	if(pos >= iAllLen)
		return false;
	ys_pcap_pkthdr *pHead = (ys_pcap_pkthdr*)&pAllBuffer[pos];
	soc = pHead->ts.tv_sec;
	usec = pHead->ts.tv_usec;
	iLen = pHead->caplen;

	pos += sizeof(ys_pcap_pkthdr);
	pBuffer = &pAllBuffer[pos];
	pos += iLen;

	return true;

}

bool CPcapFile::OpenPcapFile( string sFileName )
{
	//重置，打开其他文件
	Reset();
	m_sFile = sFileName;	
	m_fpFile = fopen(sFileName.c_str(),"rb");
	if (!m_fpFile) return false;
	m_nFileLen = GetFileLen();
	int nRead = 0;
	if (!m_pHeader)
		m_pHeader = new ys_pcap_file_header;
	int ret = fread(m_pHeader,1,sizeof(ys_pcap_file_header),m_fpFile);
	if (ret != sizeof(ys_pcap_file_header)) return false;
	nRead += ret;
	char buf[20];
	while (nRead < m_nFileLen)
	{
		pcap_dthdr  pkdt;
		int n=0;
		memset(buf,0,sizeof(buf));

		//读取pcap文件头
		ret = fread(&pkdt.pp,1,sizeof(ys_pcap_pkthdr),m_fpFile);
		if (ret != sizeof(ys_pcap_pkthdr))
			return false;
		nRead += ret;

		//目的地址
		ret = fread(pkdt.dstaddr,sizeof(char),6,m_fpFile);
		//源地址
		ret = fread(pkdt.srcaddr,sizeof(char),6,m_fpFile);
		ret = fread(buf,sizeof(char),2,m_fpFile);
		n += 14;
		WORD w = *((WORD*)buf);
		if (w == 0x81)
		{
			//有vlan信息的多4个字节的vlan信息
			pkdt.vlan = w;
			ret = fread(buf,1,2,m_fpFile);
			pkdt.priflag = *((WORD*)buf);
			ret = fread(buf,1,2,m_fpFile);
			pkdt.enettype= *((WORD*)buf);
			n += 4;
		}
		else
		{
			//没有vlan信息，这两位就是以太网类型
			pkdt.vlan = 0;
			pkdt.priflag = 0;
			pkdt.enettype= w;
		}
		pkdt.nDataPos = nRead;
		m_vDthdt.push_back(pkdt);
		nRead += pkdt.pp.caplen;
		fseek(m_fpFile,pkdt.pp.caplen-n,SEEK_CUR);
	}
	return true;
}

bool CPcapFile::OpenPcapFile( const char* sFileName )
{
	return OpenPcapFile(string(sFileName));
}

int CPcapFile::GetFileLen()
{
	int npos = ftell(m_fpFile);
	fseek(m_fpFile,0,SEEK_END);
	int len  = ftell(m_fpFile);
	fseek(m_fpFile,npos,SEEK_SET);
	return len;
}

int CPcapFile::GetFileLen( string sFile)
{

	FILE* fp = fopen(sFile.c_str(),"rb");
	if(!fp) return -1;
	fseek(fp,0,SEEK_END);
	int len  = ftell(fp);
	fclose(fp);
	return len;
}

int CPcapFile::GetFileLen(FILE* fp)
{
	if(fp == NULL)
		return -1;
	int pos = ftell(fp);
	fseek(fp,0,SEEK_END);
	int len  = ftell(fp);
	fseek(fp,pos,SEEK_SET);
	return len;
}

bool CPcapFile::GetNext(pcap_dtinfo* pd)
{
	++m_nCurr;
	return GetCurrent(pd);
}

void CPcapFile::ClosePcapFile()
{
	if(m_fpFile)
	{
		fclose(m_fpFile);
		m_fpFile = NULL;
	}
}

void CPcapFile::Reset()
{
	m_sFile = "";
	if (m_fpFile!=NULL)
	{
		ClosePcapFile();
		m_fpFile = NULL;
	}
	if (m_pHeader)
	{
		delete m_pHeader;
		m_pHeader = NULL;
	}
	m_vDthdt.clear();
	m_nCurr = 0;
}

bool CPcapFile::GetFirst(pcap_dtinfo* pd)
{
	m_nCurr = 0;
	return GetCurrent(pd);
}

bool CPcapFile::GetCurrent( pcap_dtinfo* pd )
{
	if (pd == NULL)
		return false;
	if (m_nCurr>=(int)m_vDthdt.size())
		return false;

	ys_pcap_pkthdr phdr = m_vDthdt[m_nCurr].pp;
	pd->sec = phdr.ts.tv_sec;
	//modify by fjq 2010-1-6
	pd->usec= phdr.ts.tv_usec;
	///////////////////////
	pd->len   = m_vDthdt[m_nCurr].pp.caplen;
	fseek(m_fpFile,m_vDthdt[m_nCurr].nDataPos,SEEK_SET);
	if(pd->len > pd->buflen)
	{
		if(pd->pData != NULL)
			delete[] pd->pData;
		pd->pData = new BYTE[pd->len];
		pd->buflen = pd->len;
	}
	fread(pd->pData,sizeof(BYTE),pd->len,m_fpFile);
	return true;
}

void CPcapFile::MakeHeader( ys_pcap_file_header& pfh )
{
	pfh.magic			= 0xa1b2c3d4;
	pfh.version_major	= 0x02;
	pfh.version_minor	= 0x04;
	pfh.thiszone		= 0;
	pfh.sigfigs			= 0;
	pfh.snaplen			= 0xffff;
	pfh.linktype		= 1;
}

bool CPcapFile::TransFromYspToPcap( string sSrcFile,string sDstFile )
{
	FILE* fpcap = fopen(sDstFile.c_str(),"wb");	
	if (!fpcap) return false;
	//写入文件头
	ys_pcap_file_header pfh;
	MakeHeader(pfh);
	fwrite(&pfh,1,sizeof(pfh),fpcap);

	FILE* fp = fopen(sSrcFile.c_str(),"rb");
	if (!fp) return false;

	fseek(fp,1224,SEEK_SET);
	int len = GetFileLen(fp);
	int npos = 1224;
	char *pBuffer;
	while (npos < len)
	{
		Ysp_Data_Info ydi;
		fread(&ydi,1,sizeof(ydi),fp);
		npos += sizeof(ydi);
		int nDataLen = (ydi.f.dw &0x0fff);
		pBuffer = new char[nDataLen];
		memset(pBuffer,0,nDataLen);
		fread(pBuffer,1,nDataLen,fp);
		npos += nDataLen;
		ys_pcap_pkthdr pp;
		pp.ts.tv_sec = ydi.soc;
		//modify by fjq 2010-1-6
		pp.ts.tv_usec= ydi.nSec/1000;
		pp.len = nDataLen;
		pp.caplen= nDataLen;
		fwrite(&pp,1,sizeof(pp),fpcap);
		fwrite(pBuffer,1,nDataLen,fpcap);
		delete []pBuffer;
		pBuffer = NULL;
	}

	fclose(fp);
	fclose(fpcap);
	return true;
}

bool CPcapFile::GetLast( pcap_dtinfo* pd )
{
	m_nCurr = m_vDthdt.size()-1;
	return GetCurrent(pd);
}

bool CPcapFile::TransToYsp( string sDstFile )
{
	if (m_sFile=="" || m_fpFile==NULL)
	{
		return false;
	}
	if (sDstFile=="")
	{
		sDstFile = m_sFile.substr(0,m_sFile.size()-5)+".ysp";
	}

	FILE* fYsp = fopen(sDstFile.c_str(),"wb");
	if (!fYsp) return false;
	//写入文件头
	Ysp_File_Header yfh;
	pcap_dtinfo pd;
	yfh.unt = 1;
	yfh.crd = 1;
	yfh.chn = 1;
	GetFirst(&pd);
	yfh.soc_from = pd.sec;
	yfh.nsec_from= pd.usec*1000;
	GetLast(&pd);
	yfh.soc_to  = pd.sec;
	yfh.nsec_to = pd.usec;
	fwrite(&yfh,1,sizeof(yfh),fYsp);

	Ysp_Index* pyi = new Ysp_Index[100];
	int nOffset = 0;
	int i;
	//填写索引
	for (i=0,GetFirst(&pd);i<100;++i,GetNext(&pd))
	{
		pyi[i].soc = pd.sec;
		pyi[i].nsec= pd.usec*1000;
		pyi[i].offset = 1224+nOffset;
		nOffset += (12+pd.len);
	}
	fwrite(pyi,sizeof(Ysp_Index),100,fYsp);

	//写入数据段
	GetFirst(&pd);
	do 
	{
		Ysp_Data_Info ydi;
		ydi.f.dw = pd.len;
		ydi.soc = pd.sec;
		ydi.nSec= pd.usec*1000;
		fwrite(&ydi,1,sizeof(ydi),fYsp);
		fwrite(pd.pData,1,pd.len,fYsp);
	} while (GetNext(&pd));	

	fclose(fYsp);	
	return true;
}

