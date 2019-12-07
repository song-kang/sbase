/**
 *
 * �� �� �� : SRawSocketRecv.h
 * �������� : 2011-12-1 10:08
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : ԭʼ�׽��ֻ�����
 * �޸ļ�¼ : 
 *            $Log: $
 *
 **/

#ifndef __SKT_RAW_SOCKET_H__
#define __SKT_RAW_SOCKET_H__

#ifdef WIN32
	#include <stdio.h> 
	#define HAVE_REMOTE
	#define WPCAP
	#include <pcap.h>
	#pragma comment(lib,"wpcap.lib")
#else
	#include <stdio.h> 
	#include <stdlib.h> 
	#include <sys/socket.h> 
	#include <netinet/in.h> 
	#include <arpa/inet.h> 
	#include <netinet/ip.h> 
	#include <string.h> 
	#include <netdb.h> 
	#include <netinet/tcp.h> 
	#include <netinet/udp.h> 
	#include <stdlib.h> 
	#include <unistd.h> 
	#include <signal.h> 
	#include <net/if.h> 
	#include <sys/ioctl.h> 
	#include <sys/stat.h> 
	#include <fcntl.h> 
	#include <linux/if_ether.h> 
	#include <net/ethernet.h> 

	/* ssl_ifindex��ʾ�������*/
	struct sockaddr_ll 
	{ 
		u_short sll_family; 
		u_short sll_protocol; 
		int sll_ifindex; //�����ֽ��򣬣�3Ϊeth1,5Ϊeth3��
		u_short sll_hatype; 
		u_char sll_pkttype; 
		u_char sll_halen; 
		u_char sll_addr[8]; 
	};

#endif

#ifndef _WIN64
#ifndef SOCKET
#define SOCKET long
#endif
#endif

#include "../SApi.h"
#include "../SService.h"

//���յ�ԭʼ���Ļص���������
typedef void (*OnReceivePcapData)(void* cbparam,int soc,int usec,BYTE *pBuffer,int iLength);
typedef BYTE* (*OnGetWriteBuffer)(void* cbparam);

//////////////////////////////////////////////////////////////////////////
// ��    ��:  SRawSocketRecv
// ��    ��:  �ۿ���
// ����ʱ��:  2011-12-1 10:09
// ��    ��:  ԭʼ�׽��ֽ�����
//////////////////////////////////////////////////////////////////////////
class SBASE_RAWSOCK_EXPORT SRawSocketRecv : public SService
{
public:
	SRawSocketRecv();
	~SRawSocketRecv();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-12-1 10:37
	// ����˵��:  void
	// �� �� ֵ:  true��ʾ���������ɹ�,false��ʾ��������ʧ��
	//////////////////////////////////////////////////////////////////////////
	virtual bool Start();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ֹͣ����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-12-1 10:37
	// ����˵��:  void
	// �� �� ֵ:  true��ʾ����ֹͣ�ɹ�,false��ʾ����ֹͣʧ��
	//////////////////////////////////////////////////////////////////////////
	virtual bool Stop();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���ô������������豸������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-12-1 10:37
	// ����˵��:  @sDevName��WINDOWSƽ̨�µ�WINPCAP���豸����LINUX��Ϊ������
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	inline void SetDevName(SString sDevName){m_sDevName = sDevName;};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���յ�ԭʼ���ĵĴ����麯��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-12-24 20:53
	// ����˵��:  
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	virtual void OnReceive(int soc,int usec,BYTE *pBuffer,int iLength){S_UNUSED(soc),S_UNUSED(usec);S_UNUSED(pBuffer);S_UNUSED(iLength);};


	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���ûص��������û�ָ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-12-1 10:47
	// ����˵��:  @pΪ��ָ��
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	inline void SetCallback(OnReceivePcapData pFun,void *p)
	{
		m_pCallbackOnReceive = pFun;
		m_pCallbackUserPtr = p;
	};

	inline void SetCallbackGetWriteBuffer(OnGetWriteBuffer pFun,int iMaxLen)
	{
		m_pGetWriteBuffer = pFun;
		m_iMaxWriteBufferLen = iMaxLen;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���ý��ջ������Ĵ�С
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-12-1 10:57
	// ����˵��:  @bytes�´�С����λ�ֽ�
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	inline void SetRecvBufferSize(int bytes){m_iRecvBufferSize = bytes;};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���ù����ֽ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2012-2-1 8:47
	// ����˵��:  @pFilterBytes�����ֽ��׵�ַ
	//            @len�����ֽڳ���
	//            @iFilterOffsetPos����λ�ã�����ԭʼ�����е�ƫ���ֽ���
	//            @bFilterExcept:true��ʾ���˵�����ָ�����ݵı��ģ�false��ʾ���˵�����ָ�����ݵı���
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void SetFilterBytes(BYTE *pFilterBytes,int len,int iFilterOffsetPos,bool bFilterExcept);

private:
	////////////////////////////////////////////////////////////////////////
	// ��    ��:  PCAPץ���߳�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-12-1 9:35
	// ����˵��:  this
	// �� �� ֵ:  NULL
	//////////////////////////////////////////////////////////////////////////
	static void* ThreadPcap(void *lp);

	SString m_sDevName;//PCAP������ӿ���
	OnReceivePcapData m_pCallbackOnReceive;//���ջص������ĵ�ַ
	OnGetWriteBuffer m_pGetWriteBuffer;//ȡ���ջ�������ַ�Ļص�����
	int m_iMaxWriteBufferLen;//��֡���ĵ���󳤶�
	void *m_pCallbackUserPtr;//�ص�ʱ���û�ָ��
	int m_iRecvBufferSize;//���ջ������Ĵ�С����λ�ֽڣ�ȱʡ20M

	BYTE *m_pFilterBytes;//�����ֽڴ�,NULL��ʾ������
	int m_iFilterOffsetPos;//�����ֽڴ���ԭʼ�����е�ƫ��λ��
	int m_iFilterByteLen;//�����ֽڴ�����
	bool m_bFilterExcept;//true��ʾ������ˣ�ָ���˵�����ָ�����ݵı��ģ�false��ʾ���˵�����ָ�����ݵı���

#ifdef WIN32
	pcap_t *m_pCap;
	char m_ErrBuf[PCAP_ERRBUF_SIZE];
	static void ys_packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data);
public:
#else
	int *m_pRawsock;//ԭʼ����SOCKET
	static void raw_die(char *why, int n);
	static bool raw_do_promisc(char *nif, int sock );
	static void raw_set_recvbuffer(int sock,int iBufferSize);
	public:
	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �������������豸����ǰ׺��ȱʡΪeth,UNIXϵͳ�¿����������豸���ƣ���ʱ��Ҫ���ã�������Ӱ��FindAllPcapDevices���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-1-5 11:10
	// ����˵��:  @eth
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	static void SetEthNamePrex(SString eth);
#endif
	struct stuPcapDevice 
	{
		SString name;
		SString desc;
		SString ip;
		SString subnetip;
		SString netmask;
		SString mac;//':'�ָ�
	};
	static int FindAllPcapDevices(SPtrList<stuPcapDevice> &list);
};

//////////////////////////////////////////////////////////////////////////
// ��    ��:  SRawSocketSend
// ��    ��:  �ۿ���
// ����ʱ��:  2011-12-1 14:15
// ��    ��:  ԭʼ�׽��ֵķ�����
//////////////////////////////////////////////////////////////////////////
class SBASE_RAWSOCK_EXPORT SRawSocketSend
{
public:
	SRawSocketSend();
	~SRawSocketSend();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����һ�����ڷ��͵�ԭʼ�׽���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-12-1 14:16
	// ����˵��:  @sDevNameΪѡ�����ڷ��͵�Ŀ��������
	// �� �� ֵ:  true��ʾ�����ɹ���false��ʾʧ�� 
	//////////////////////////////////////////////////////////////////////////
	bool Create(SString sDevName="eth0");

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �ر�ԭʼ�׽���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-12-1 15:37
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void Close();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����ָ����С��ԭʼ����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-12-1 14:18
	// ����˵��:  @pBufferΪԭʼ����ȫ�����ݣ���Ŀ�ꡢԴMAC��ַ
	//            @iLenΪԭʼ�����ܳ��ȣ���λ�ֽ�
	// �� �� ֵ:  >0��ʾ���ͳɹ����ֽ�����<0��ʾʧ�ܣ�=0��ʾ���ͳ�ʱ
	//////////////////////////////////////////////////////////////////////////
	int SendRaw(BYTE *pBuffer,int iLen);

private:

#ifdef WIN32
	pcap_t *m_pCap;
#ifdef _WPCAP_USE_SEND_QUEUE
	pcap_send_queue *m_pSendQueue;
	struct pcap_pkthdr m_PktHdr;
#endif
	char m_ErrBuf[PCAP_ERRBUF_SIZE];
#else
	int m_sock;//Raw socket HANDLE
    struct sockaddr_ll m_sll; 
	
#endif
};

#endif//__SKT_RAW_SOCKET_H__
