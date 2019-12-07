/**
 *
 * �� �� �� : SNtp.h
 * �������� : 2015-9-6 22:12
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : NTP��װ��
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-9-6	�ۿ�������ļ�
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
	{//SNTP���ṹ
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
	// ��    ��:  ���÷���������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-9-6 22:52
	// ����˵��:  @ipΪSNTP������IP��ַ
	//         :  @portΪUDP�˿ںţ�ȱʡΪ123
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void SetServer(SString ip,int port=SNTPSERVER_PORT)
	{
		m_sServerIp = ip;
		m_iPort = port;
	}

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����ʱ�䲹��ֵ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-9-6 22:53
	// ����˵��:  @secΪ�벹��
	//         :  @msecΪ���벹��
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void SetTimeCompensate(int sec,int msec)
	{
		m_iSecondsAdd = sec;
		m_iMSecondsAdd = msec;
	}

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��ǰʱ�̷������ϵ�ʱ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-9-6 22:53
	// ����˵��:  @nowΪ���÷��صĵ�ǰSNTP������ʱ��
	//         :  @bSetToSystemΪtrue��ʾʱ���ȡ�ɹ�����Ҫд��ϵͳ��false��ʾ����Ҫд��ϵͳ
	// �� �� ֵ:  true��ʾ��ȡ�ɹ���false��ʾ��ȡʧ��
	//////////////////////////////////////////////////////////////////////////
	bool GetTime(SDateTime &now,bool bSetToSystem=true);
private:
	SString m_sServerIp;
	int m_iPort;//123
	int m_iSecondsAdd;//�벹��
	int m_iMSecondsAdd;//���벹��
};

class SBASE_EXPORT SSntpServer : public SNtpBase
{
public:
	SSntpServer();
	virtual ~SSntpServer();
	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���÷�����ip�Ͷ˿ڣ����Բ����ã�Ĭ��ipΪ�գ�
	// ��    ��:  �ұ���
	// ����ʱ��:  2015-9-24 14:20
	// ����˵��:  @ipΪSNTP��������IP��ַ
	//         :  @portΪUDP�˿ںţ�ȱʡΪ123
	//		   :  @
	// �� �� ֵ:  ��
	//////////////////////////////////////////////////////////////////////////
	void SetServer(SString ip,int port=SNTPSERVER_PORT)
	{
		m_sServerIp = ip;
		m_iPort = port;
	}
	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����sntpУʱ����
	// ��    ��:  �ұ���
	// ����ʱ��:  2015-9-24 14:21
	// ����˵��:  @
	// �� �� ֵ:  false ��ʾ����ʧ��
	//////////////////////////////////////////////////////////////////////////
	bool StartServer();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ֹͣsntpУʱ���񣬿��ܻ�ȴ�һ��ʱ��
	// ��    ��:  �ұ���
	// ����ʱ��:  2015-9-24 14:21
	// ����˵��:  @
	// �� �� ֵ:  true ��ʾֹͣ�ɹ�
	//////////////////////////////////////////////////////////////////////////
	bool StopServer();

private:
	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  sntpУʱ������߳�ִ�к�����ѭ�����ܿͻ�������
	//            ���udp���ӣ�ʧЧ�����´���
	// ��    ��:  �ұ���
	// ����ʱ��:  2015-9-25 17:38
	// ����˵��:  @arg Ϊ�̴߳��ݲ���
	//         :  @
	//		   :  @
	// �� �� ֵ:  ��
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
	// ��    ��:  ���������������ip�Ͷ˿�
	// ��    ��:  �ұ���
	// ����ʱ��:  2015-9-24 14:20
	// ����˵��:  @ipΪNTP������IP��ַ
	//         :  @portΪUDP�˿ںţ�ȱʡΪ123
	//		   :  @
	// �� �� ֵ:  ��
	//////////////////////////////////////////////////////////////////////////
	void SetServer(SString ip,int port=SNTPSERVER_PORT)
	{
		m_sServerIp = ip;
		m_iPort = port;
	}
	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ʩ����ֵ
	// ��    ��:  �ұ���
	// ����ʱ��:  2015-9-24 14:23
	// ����˵��:  @secΪ�벹��
	//         :  @mescΪ���벹��
	//		   :  @
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	void SetTimeCompensate(int sec,int msec)
	{
		m_iSecondsAdd = sec;
		m_iMSecondsAdd = msec;
	}

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��ǰ��������ʱ��
	// ��    ��:  �ұ���
	// ����ʱ��:  2015-9-24 14:23
	// ����˵��:  @nowΪ���÷��صĵ�ǰNTP������ʱ��
	//         :  @bSetToSystemΪtrue��ʾʱ���ȡ�ɹ�����Ҫд��ϵͳ��false��ʾ����Ҫд��ϵͳ
	// �� �� ֵ:  true��ʾ��ȡ�ɹ���false��ʾ��ȡʧ��
	//////////////////////////////////////////////////////////////////////////
	bool GetTime(SDateTime &now,bool bSetToSystem=true);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ʱ��ƫ�����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-12-15 23:02
	// ����˵��:  @local_soc
	//         :  @local_usec
	//         :  @remote_soc
	//         :  @remote_usec
	//         :  @ts_sec
	//         :  @ts_usec
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool TestTimeByTssm(int &local_soc,int &local_usec,int &remote_soc,int &remote_usec,int &ts_sec,int &ts_usec);

private:
	SString m_sServerIp;
	int m_iPort;//123
	int m_iSecondsAdd;//�벹��
	int m_iMSecondsAdd;//���벹��
};

class SBASE_EXPORT SNtpServer : public SNtpBase
{
public:
	SNtpServer();
	virtual ~SNtpServer();
	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���������������ip�Ͷ˿ڣ����Բ����ã�Ĭ��ipΪ�գ�
	// ��    ��:  �ұ���
	// ����ʱ��:  2015-9-24 14:20
	// ����˵��:  @ipΪSNTP��������IP��ַ
	//         :  @portΪUDP�˿ںţ�ȱʡΪ123
	//		   :  @
	// �� �� ֵ:  ��
	//////////////////////////////////////////////////////////////////////////
	void SetServer(SString ip,int port=SNTPSERVER_PORT)
	{
		m_sServerIp = ip;
		m_iPort = port;
	}

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����ntpУʱ����
	// ��    ��:  �ұ���
	// ����ʱ��:  2015-9-24 14:21
	// ����˵��:  @bTssm �Ƿ�����TSSM��֤
	// �� �� ֵ:  false ��ʾ����ʧ��
	//////////////////////////////////////////////////////////////////////////
	bool StartServer(bool bTssm=true);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ֹͣsntpУʱ���񣬿��ܻ�ȴ�һ��ʱ��
	// ��    ��:  �ұ���
	// ����ʱ��:  2015-9-24 14:21
	// ����˵��:  @
	// �� �� ֵ:  true ��ʾֹͣ�ɹ�
	//////////////////////////////////////////////////////////////////////////
	bool StopServer();
private:
	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ntpУʱ������߳�ִ�к�����ѭ�����ܿͻ�������
	//            ���udp���ӣ�ʧЧ�����´���
	// ��    ��:  �ұ���
	// ����ʱ��:  2015-9-25 17:38
	// ����˵��:  @arg Ϊ�̴߳��ݲ���
	//         :  @
	//		   :  @
	// �� �� ֵ:  ��
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
