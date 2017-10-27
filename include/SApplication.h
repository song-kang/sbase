/**
 *
 * �� �� �� : SApplication.h
 * �������� : 2009-8-20 13:36
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: 2011/08/31 08:43:32 $
 * ��ǰ�汾 : $Revision: 1.1 $
 * �������� : Ӧ�ó�������࣬������Ӧ�ù�������ͻ���
 * �޸ļ�¼ : 
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
 *            ͨѸ��¼����װ����Ŀ�״��ύ
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
// ��    ��:  eSpUnitAgentMsg
// ��    ��:  �ۿ���
// ����ʱ��:  2016-2-3 17:20
// ��    ��:  ������Ϣ���Ͷ���
//////////////////////////////////////////////////////////////////////////
enum SBASE_EXPORT eSpUnitAgentMsg
{
	SP_UA_MSG_REGISTER=1,		//ע�������Ϣ
	SP_UA_MSG_REG_AGENT,		//ע�������Ϣ�����ڴ���以����
	SP_UA_MSG_SEND_OK,			//��Ϣ�ɹ�����
	SP_UA_MSG_SEND_ERR,			//��Ϣ����ʧ��
	SP_UA_MSG_PS,				//ȡ��Ԫ�ϵĽ����б�(����Ԫ)
	SP_UA_MSG_REBOOT,			//��Ԫ����(����Ԫ)
	SP_UA_MSG_HALT,				//��Ԫ�ػ�(����Ԫ)
	SP_UA_MSG_REQ_GET_UNIT,		//ȡ���еĴ���Ԫ�б�(����Ԫ)
	SP_UA_MSG_RES_GET_UNIT,		//ȡ���еĴ���Ԫ�б�(����Ԫ)�ظ�
	SP_UA_MSG_REQ_GET_PROCINFO,	//ȡָ����Ԫ����Ӧ�õ���Ϣ�б�
	SP_UA_MSG_RES_GET_PROCINFO,	//ȡָ����Ԫ����Ӧ�õ���Ϣ�б�ظ�
	SP_UA_MSG_REQ_GET_PROCID,	//ȡָ��Ӧ�úż�ʵ���ŵ�Ӧ��ID(����Ԫ)
	SP_UA_MSG_RES_GET_PROCID,	//ȡָ��Ӧ�úż�ʵ���ŵ�Ӧ��ID(����Ԫ)�ظ�
	SP_UA_MSG_REQ_LS,			//ȡ���������ϵ�Ŀ¼��Ϣ(����Ԫ)
	SP_UA_MSG_RES_LS,			//ȡ���������ϵ�Ŀ¼��Ϣ(����Ԫ)�ظ�
	SP_UA_MSG_REQ_GET_FILE,		//��ȡ�ļ�(����Ԫ)
	SP_UA_MSG_RES_GET_FILE,		//��ȡ�ļ�(����Ԫ)�ظ�
	SP_UA_MSG_REQ_PUT_FILE,		//�ϴ��ļ�(����Ԫ)
	SP_UA_MSG_RES_PUT_FILE,		//�ϴ��ļ�(����Ԫ)�ظ�
	SP_UA_MSG_REQ_DEL_FILE,		//ɾ���ļ�(����Ԫ)
	SP_UA_MSG_RES_DEL_FILE,		//ɾ���ļ�(����Ԫ)�ظ�
	SP_UA_MSG_RUN_CMD,			//��������(����Ԫ)
	SP_UA_MSG_PROC_TXT_CMD_REQ,	//Ӧ�ý��̵��ı���������
	SP_UA_MSG_PROC_TXT_CMD_ACK,	//Ӧ�ý��̵��ı�����Ӧ��
	SP_UA_MSG_APP_STOP,			//Ӧ�ý����˳�
	//SP_UA_CMD_PROX,		//����

	SP_UA_MSG_USER=101,		//Ӧ�ò��Զ������ʼ�����
};

enum SBASE_EXPORT eSpUnitAgentMsgResult
{
	SP_UA_MSG_RESULT_FAIL = 0,
	SP_UA_MSG_RESULT_SUCCESS
};

//////////////////////////////////////////////////////////////////////////
// ��    ��:  eSpUnitAgentApplicationNo
// ��    ��:  �ۿ���
// ����ʱ��:  2016-2-3 17:23
// ��    ��:  Ӧ�úŶ���
//////////////////////////////////////////////////////////////////////////
enum SBASE_EXPORT eSpUnitAgentApplicationNo
{
	SP_UA_APPNO_AGENT=1,	//�������
	SP_UA_APPNO_WATCHDOG,	//���Ź�����
	SP_UA_APPNO_SHIELD_IN,	//�������������ͬ������
	SP_UA_APPNO_SHIELD_OUT,	//�������������ͬ������

	SP_UA_APPNO_USER=101,	//Ӧ�ò��Զ������ʼӦ�ú�
};

#pragma pack(push)  
#pragma pack(1)

//////////////////////////////////////////////////////////////////////////
// ��    ��:  stuSpUnitAgentUnitInfo
// ��    ��:  �ۿ���
// ����ʱ��:  2016-2-3 17:27
// ��    ��:  ��Ԫ��Ϣ�ṹ����
//////////////////////////////////////////////////////////////////////////
struct SBASE_EXPORT stuSpUnitAgentUnitInfo
{
	DWORD m_iUnitId;//��Ԫ��
	char m_sSoftType[32];
	char m_sUnitName[32];
	char m_sUnitDesc[64];
	DWORD m_iUnitIp[8];
};

//////////////////////////////////////////////////////////////////////////
// ��    ��:  stuSpUnitAgentProcessId
// ��    ��:  �ۿ���
// ����ʱ��:  2016-2-2 13:22
// ��    ��:  ��Ԫ�ڽ��̱�ʶ�ṹ
//////////////////////////////////////////////////////////////////////////
struct SBASE_EXPORT stuSpUnitAgentProcessId
{
	DWORD m_iUnitId;			//2.1.1.1��Ԫ��ʶ
	//��Ԫ����ͨ�����͵ĵ�Ԫ����ΪΨһ��ʶ��Ҳ����ͨ����Ԫ��IP��ַΪΨһ��ʶ��
	//��Ԫ��ʶΪ4�ֽ�����������ͨѶ���������ֽ�˳�����У�0��ʾ�㲥��ַ��<=0xffff��ʾ��Ԫ��ţ�1~0xffff����>0xffff��ʾ��Ԫ��IP��ַ��
	//��ʹ��IPΪ��Ԫ��ʶʱ��ȱʡʹ�õ�һ��IP��ַΪ��ʶ������IP��һ����ΪͨѶIPʹ��
	DWORD m_iApplicationId;		//�����ֽ�ΪӦ�úţ���Ӧ�ò㶨�壬0:��ʾ������̣�1~100�����ƽ̨������101~��������Ӧ�ò㶨��
								//�����ֽ�ΪӦ��ʵ���ţ����ͬһӦ�ú��²�ͬʵ���µ�ʵ���ţ����ֶ�����ض�Ӧ����Ч����Чʱ��0

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
// ��    ��:  stuSpUnitAgentMsgHead
// ��    ��:  �ۿ���
// ����ʱ��:  2016-2-2 16:34
// ��    ��:  ��ԪͨѶͷ�ṹ����
//////////////////////////////////////////////////////////////////////////
struct SBASE_EXPORT stuSpUnitAgentMsgHead
{
	stuSpUnitAgentProcessId m_SrcId;	//Դ����ID
	stuSpUnitAgentProcessId m_DstId;	//Ŀ�����ID
	DWORD m_dwMsgSn;						//�������
	DWORD m_dwParamReault;				//��ʾ�����򷵻�ֵ�����������Ϊ�����������Ӧ����Ϊ����ֵ������ֵ��0:ʧ�ܣ�1:�ɹ�������Ӧ�ò��Զ���

	inline void Swap()
	{
		m_SrcId.Swap();
		m_DstId.Swap();
		SKT_SWAP_DWORD(m_dwMsgSn);
		SKT_SWAP_DWORD(m_dwParamReault);
	}
};

//////////////////////////////////////////////////////////////////////////
// ��    ��:  stuModuleInfo
// ��    ��:  �ۿ���
// ����ʱ��:  2011-3-29 9:53
// ��    ��:  ������Ϣ�ṹ
//////////////////////////////////////////////////////////////////////////
struct SBASE_EXPORT stuSpUnitAgentProcInfo
{
	stuSpUnitAgentProcessId process_id;	//����ID
	DWORD pid;							//���̺�
	char name[32];						//�������ƣ�WINDOWSƽ̨����Ҫ��.exe
	char version[32];					//�汾��,����Ҫ��V��
	char desc[64];						//��������
	char crc[16];						//CRCУ����
	char build_time[32];				//��������ʱ��
	DWORD begin_time;					//���̿�ʼ����ʱ��
	char login_ip[24];					//��¼IP��ַ
	DWORD login_port;					//��¼�˿ں�
	DWORD login_time;					//��¼ʱ��
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
	char path[128];//Ŀ¼ȫ·����{@APP_PATH@}��ʾӦ�ó����Ŀ¼������ֱ�ӷ��ʸ�Ŀ¼
	char filter[32];//�����������ձ�ʾ������
};

struct SBASE_EXPORT stuSpUnitAgentLsRes//Result�д���ļ�����
{
	bool is_dir;//�Ƿ�ΪĿ¼
	char name[128];//�ļ�����Ŀ¼��
	int file_size;//�ļ���С���ֽڣ�Ŀ¼ʱΪ0
};


struct SBASE_EXPORT stuSpUnitAgentGetFileReq
{
	char path_file[256];//Դ�ļ���ȫ·����{@APP_PATH@}��ʾӦ�ó����Ŀ¼
};

//struct stuSpUnitAgentGetFileRes
//�ظ�ʱֱ�ӻظ��ļ����ݵ�ASDU�У�����Result�д����Ƭ��ţ���Ƭ��Ŵ�1��ʼ��0��ʾ���һƬ��������֡��




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
	// ��    ��:  �Է�����ʽ�����ͻ��ˣ�������Connect����ͬʱʹ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-2-3 09:52
	// ����˵��:  @ip��ʾ�����IP��ַ��@port��ʾ�˿�
	// �� �� ֵ:  true��ʾ���������ɹ�,false��ʾ��������ʧ��
	//////////////////////////////////////////////////////////////////////////
	virtual bool Start(SString ip,int port);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ������Ϣ����ӿڣ���������ʵ�֣����������뾡����С�����ٷ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-2-3 15:20
	// ����˵��:  @wMsgType��ʾ��Ϣ����
	//         :  @pMsgHeadΪ��Ϣͷ
	//         :  @sHeadStr��Ϣͷ�ַ���
	//         :  @pBufferΪ��ϢЯ���Ķ������������ݣ�NULL��ʾ�޶���������
	//         :  @iLengthΪ���������ݳ���
	// �� �� ֵ:  true��ʾ����ɹ���false��ʾ����ʧ�ܻ�δ����
	//////////////////////////////////////////////////////////////////////////
	virtual bool ProcessAgentMsg(WORD wMsgType,stuSpUnitAgentMsgHead *pMsgHead,SString &sHeadStr,BYTE* pBuffer=NULL,int iLength=0)=0;

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����ı�����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-2-13 15:12
	// ����˵��:  @sCmdΪ�������ݣ����ַ�����ʾȡ�������б�
	//         :  @sResultΪ���ؽ�������ı�
	// �� �� ֵ:  true��ʾ�ɹ���false��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	virtual bool ProcessTxtCmd(SString &sCmd,SString &sResult){S_UNUSED(sCmd);S_UNUSED(sResult);return false;};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ͨѶ���ݰ�����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-2-4 17:29
	// ����˵��:  @pPackageΪԴ���ݰ�
	//         :  @pMsgHead
	// �� �� ֵ:  true��ʾ���Ϸ���false��ʾ���Ϸ�
	//////////////////////////////////////////////////////////////////////////
	bool PackageExplain(stuSTcpPackage *pPackage,WORD &wMsgType, stuSpUnitAgentMsgHead* &pMsgHead,BYTE* &pBuffer,int &iLength);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����ͨѶ���ݰ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-2-4 18:18
	// ����˵��:  @pMsgHeadΪ��Ϣͷ
	//         :  @pBufferΪ��ϢЯ���Ķ������������ݣ�NULL��ʾ�޶���������
	//         :  @iLengthΪ���������ݳ���
	// �� �� ֵ:  stuSTcpPackage*
	//////////////////////////////////////////////////////////////////////////
	stuSTcpPackage* PackageBuild(WORD wMsgType,stuSpUnitAgentMsgHead *pMsgHead,BYTE* pBuffer=NULL,int iLength=0);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��¼�Ϸ���˺�Ļص�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-2-2 22:25
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void OnLogin();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �Ự�߳̽��յ����ĺ�Ļص��麯����������ͨ���˺�������ʱ��Ϣ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-2-3 09:35
	// ����˵��:  @pSessionΪ�Ựʵ��ָ�룬����ͨ��ָ����Զ˷�������
	//            @pPackageΪ�ս��յ������ݰ�
	// �� �� ֵ:  true��ʾ�Ѿ�������ϣ��˱��Ŀ����ͷ��ˣ�false��ʾδ�����˱���Ӧ������ն���
	//////////////////////////////////////////////////////////////////////////
	virtual bool OnRecvFrame(stuSTcpPackage *pPackage);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡΨһ����Ϣ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-2-3 15:35
	// ����˵��:  void
	// �� �� ֵ:  WORD
	//////////////////////////////////////////////////////////////////////////
	static DWORD NewMsgSn();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ���еĴ���Ԫ�б�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-2-3 17:31
	// ����˵��:  @unitsΪ���÷��صĵ�Ԫ��Ϣ
	// �� �� ֵ:  ���ص�Ԫ����,<0��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	int GetUnitInfo(SPtrList<stuSpUnitAgentUnitInfo> &units);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡָ����Ԫ�Ŷ�Ӧ�Ľ�����Ϣ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-2-12 8:53
	// ����˵��:  @iUnitId��ʾ���̱�ʶ��0��ʾ��ǰ��Ԫ
	//         :  @ProcInfosΪ���÷��صĽ�����Ϣ�б�
	// �� �� ֵ:  >=0��ʾ��������,<0��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	int GetProcessInfo(int iUnitId,SPtrList<stuSpUnitAgentProcInfo> &ProcInfos);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��ǰ��Ԫ��ָ��Ӧ�úż�ʵ���Ŷ�Ӧ�Ľ��̱�ʶ��ֻ���ص�һ��ƥ����̱�ʶ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-2-3 17:11
	// ����˵��:  @wAppId��ʾӦ�ú�
	//         :  @wAppInstId��ʾӦ��ʵ���ţ����Ƕ�ʵ��Ӧ��ʱ��0
	//         :  @pProcIdsΪ��ʶָ�룬��ȡ���ı�ʶ��д���ָ���Ӧ���ڴ���
	// �� �� ֵ:  ���ؽ��̱�ʶ����,<0��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	int GetProcessId(WORD wAppId,WORD wAppInstId,SPtrList<stuSpUnitAgentProcessId> &ProcIds);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡָ����Ԫ���ļ��б�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-2-20 13:28
	// ����˵��:  @iUnitIdΪ��Ԫ��ʶ
	//         :  @sPathΪ����ȡ��Ŀ¼��������{@APP_PATH@}��ʾӦ�ó����Ŀ¼
	//         :  @sFilterΪ����������������Ϊ�ջ�*
	//         :  @itemsΪ���ص��ļ���Ŀ¼�����
	//         :  @��ʾ��ʱʱ�䣬��λΪ��
	// �� �� ֵ:  >=0��ʾ��Ŀ������<0��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	int GetUnitDir(int iUnitId,SString sPath,SString sFilter,SPtrList<stuSpUnitAgentLsRes> &items,int iTimeout_s=5);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡָ����Ԫ���ļ����ݣ������浽�����ļ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-2-20 13:30
	// ����˵��:  @iUnitIdΪ��Ԫ��ʶ
	//         :  @sPathFile��ʾ�����ļ���ȫ·��������{@APP_PATH@}��ʾӦ�ó����Ŀ¼
	//         :  @sLocalPathFile��ʾ���ر����ȫ·��
	//         :  @��ʾ��ʱʱ�䣬��λΪ��
	// �� �� ֵ:  >=0��ʾ�ļ��Ĵ�С,<0��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	int GetUnitFile(int iUnitId,SString sPathFile, SString sLocalPathFile,int iTimeout_s=5);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �ӱ����ļ���ȡ�ļ����ݲ��ϴ���ָ����Ԫ��ָ��Ŀ¼�ļ�λ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-2-20 13:31
	// ����˵��:  @iUnitId��ʾ��Ԫ��ʶ 
	//         :  @sPathFileΪĿ���ļ���ȫ·��������{@APP_PATH@}��ʾӦ�ó����Ŀ¼
	//         :  @sLocalPathFile��ʾ����Դ�ļ���ȫ·��
	//         :  @��ʾ��ʱʱ�䣬��λΪ��
	// �� �� ֵ:  >=0��ʾ�ϴ��ɹ���<0��ʾ�ϴ�ʧ��
	//////////////////////////////////////////////////////////////////////////
	int PutUnitFile(int iUnitId,SString sPathFile,SString sLocalPathFile,int iTimeout_s=5);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ָ����Ԫɾ��ָ�����ļ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-2-20 13:33
	// ����˵��:  @iUnitIdΪ��Ԫ��ʶ
	//         :  @sPathFileΪ��ɾ�����ļ�ȫ·��������{@APP_PATH@}��ʾӦ�ó����Ŀ¼
	//         :  @��ʾ��ʱʱ�䣬��λΪ��
	// �� �� ֵ:  >=0��ʾɾ���ɹ���<0��ʾɾ��ʧ��
	//////////////////////////////////////////////////////////////////////////
	int DelUnitFile(int iUnitId,SString sPathFile,int iTimeout_s=5);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �������ʹ�����Ϣ���Ѽ���������Ӧ����OnRecvFrame������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-2-3 16:05
	// ����˵��:  @pToProcIdΪĿ����̱�ʶ
	//         :  @wMsgTypeΪ��Ϣ���,101����ΪӦ�ò���Ϣ
	//         :  @dwMsgSnΪ��ϢΨһ��ţ���ͨ����ȷ��Ӧ����
	//         :  @dwParamReaultΪ�����򷵻�ֵ
	//         :  @pAsduΪӦ�ò�ASDU���ݣ�NULL��ʾ��ASDU
	//         :  @iAsduLenΪASDU����
	//         :  @psHeadStrΪ��Ϣͷ�ַ���ָ�룬NULL��ʾ���ַ���
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool SendAgentMsg(stuSpUnitAgentProcessId *pToProcId,WORD wMsgType,DWORD dwMsgSn,DWORD dwParamReault=0,BYTE *pAsdu=NULL,int iAsduLen=0,SString *psHeadStr=NULL);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �������ʹ�����Ϣ����ָ��ʱ���ڽ��ջظ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-2-3 16:52
	// ����˵��:  @pToProcIdΪĿ����̱�ʶ
	//         :  @wMsgTypeΪ��Ϣ���,101����ΪӦ�ò���Ϣ
	//         :  @dwMsgSnΪ��ϢΨһ��ţ���ͨ����ȷ��Ӧ����
	//         :  @dwParamReaultΪ�����򷵻�ֵ
	//         :  @pAsduΪӦ�ò�ASDU���ݣ�NULL��ʾ��ASDU
	//         :  @iAsduLenΪASDU����
	//         :  @psHeadStrΪ��Ϣͷ�ַ���ָ�룬NULL��ʾ���ַ���
	//         :  @wRecvMsgTypeΪϣ�����յ�����Ӧ��������
	//         :  @pRecvPackageΪ���յ�����Ӧ���ģ�������Ӧ�����ͷ�
	//         :  @iTimeOut_sΪ���ճ�ʱʱ�䣬��λΪ��
	// �� �� ֵ:  true��ʾ���Ͳ����ջظ��ɹ���false��ʾ���͡�����ջظ�ʧ��
	//////////////////////////////////////////////////////////////////////////
	bool SendAgentMsgAndRecv(stuSpUnitAgentProcessId *pToProcId,WORD wMsgType,DWORD dwMsgSn,DWORD dwParamReault,BYTE *pAsdu,int iAsduLen,SString *psHeadStr,WORD wRecvMsgType,stuSTcpPackage* &pRecvPackage,int iTimeOut_s=10);

	stuSpUnitAgentProcInfo m_SelfProcId;//��ǰ���̵�Ψһ��ʶ
	inline void SetAppPtr(SApplication *pApp){m_pApp = pApp;};

private:
	static void* ThreadAgentMsgProc(void *lp);
	SPtrList<stuSTcpPackage> m_RecvPackage_Processed;//�Ѿ������������������ɾ���ı��Ķ��У������߳̿�����Ҫ��������Ӧ����

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����ָ�������Ľ��ձ���,���Ѵ����δ����ı���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-2-20 13:39
	// ����˵��:  @bOnlyProcessedΪtrueʱ��ʾ�����Ѵ���Ķ�������ȡ����
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	stuSTcpPackage* SearchRecvPackage_Processed(DWORD sn,WORD wMsgType,WORD wMsgType2=0,bool bOnlyProcessed=false);
public:
	SApplication *m_pApp;
	DWORD m_iBeginSOC;//ģ�鿪ʼ����ʱ��
	SString m_sSoftwareType;//����ͺţ����ڴ���ͻ�����������ֵ���֤����ͬϵͳ���Ʋ���������ϵ
	SString m_sAgentIp;//����IP��ַ��ȱʡΪ127.0.0.1
	int m_iAgentPort;//����˿�
	bool m_bAgentUplink;//�Ƿ�Ϊ��Ԫ��ļ����Ự
};

#if 0
//////////////////////////////////////////////////////////////////////////
// ��    ��:  CAgentPackage
// ��    ��:  �ۿ���
// ����ʱ��:  2011-3-29 9:54
// ��    ��:  ����ͨ��
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
// ��    ��:  stuModuleInfo
// ��    ��:  �ۿ���
// ����ʱ��:  2011-3-29 9:53
// ��    ��:  ������Ϣ�ṹ
//////////////////////////////////////////////////////////////////////////
struct SBASE_EXPORT stuModuleInfo
{
	int handle;//���
	int pid;//���̺�
	SString name;//ģ����
	SString version;//�汾��
	SString desc;//ģ������
	SString crc;//ģ��CRC
	SString compile_time;//����ʱ��
	SString login_ip;//��¼����IP
	int login_port;//��¼���ж˿�
	SDateTime login_time;//��¼ʱ��
};



class SBASE_EXPORT CSAgentClient
{
public:
	CSAgentClient();
	~CSAgentClient();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���մ�����Ϣ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2010-7-1 16:59
	// ����˵��:  
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	static int ReceiveMsg(SSocket *pSocket,SString &sMsg,BYTE* &pBuffer,int &iLength);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���ʹ�����Ϣ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2010-7-1 16:59
	// ����˵��:  
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	static int SendMsg(SSocket *pSocket,SString &sMsg,BYTE* pBuffer=NULL,int iLength=0);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �������̷�����Ϣ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2010-7-1 16:56
	// ����˵��:  
	// �� �� ֵ:  
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
	// ��    ��:  ��ȡ��ǰ�Ѿ���¼������Ľ���ģ���б���Ϣ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-3-29 9:55
	// ����˵��:  ���÷��ص��б�
	// �� �� ֵ:  >=0��ʾ��������,<0��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	int GetModuleList(SPtrList<stuModuleInfo> &module_list);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ�������˵�������Ϣ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2012-9-26 11:44
	// ����˵��:  @sDisk ��ʾ��Ҫ��ȡ��Ӳ����Ϣ��disks=3;disk1=/dev/sda1;disk2=/dev/sda2;disk3=/dev/sda3;
	//            @sRunInfo���÷��ص����ַ���������������Ϣ,act=runinfo;cpu_used=%d;mem_all=%d;mem_used=%d;mem_free=%d;disk1_all=%d;disk1_used=%d;disk1_free=%d;disk1_per=%d;
	// �� �� ֵ:  1��ʾ�ɹ���0��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	bool GetRunInfo(SString sDisk,SString &sRunInfo);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡָ��ģ����ڴ�������ϵľ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-3-29 14:42
	// ����˵��:  ģ�����ƣ����������ƣ�WIN32ƽ̨�ϲ���Ҫ��.exe
	// �� �� ֵ:  =0��ʾģ��δ�ҵ���>0��ʾ���̾��
	//////////////////////////////////////////////////////////////////////////
	int GetModuleHandle(SString sModName);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ָ����ģ�鷢��һ����Ϣ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-3-29 11:06
	// ����˵��:  handleΪĿ��ģ��ľ��
	// �� �� ֵ:  ���ͳɹ�����ture��ʧ�ܷ���false
	//////////////////////////////////////////////////////////////////////////
	bool AddMsgToModule(int handle,SString &sMsg,BYTE* pBuffer=NULL,int iLength=0);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���ദ�������Ϣ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2010-7-1 16:58
	// ����˵��:  
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	bool ProcessAgentMsgBase(SString &sMsg,BYTE* pBuffer=NULL,int iLength=0);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����ദ�������Ϣ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2010-7-1 16:58
	// ����˵��:  @from_handleΪ��Ϣ��Դģ��ľ��(0��ʾ��Ϣ���Դ������)
	//         :  @sMsgΪ��Ϣ�ַ���
	//         :  @pBufferΪ��ϢЯ���Ķ������������ݣ�NULL��ʾ�޶���������
	//         :  @iLengthΪ���������ݳ���
	// �� �� ֵ:  true��ʾ����ɹ���false��ʾ����ʧ��
	//////////////////////////////////////////////////////////////////////////
	virtual bool ProcessAgentMsg(int from_handle,SString &sMsg,BYTE* pBuffer=NULL,int iLength=0)=0;

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����̣߳�ִ�д��������߳�����ʱ��ռ��Run������Ҳ��ֱ������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2012-5-30 11:00
	// ����˵��:  @lp��this
	// �� �� ֵ:  NULL
	//////////////////////////////////////////////////////////////////////////
	static void* ThreadAgent(void* lp);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����������(�����˻�������������ô�����Ӧ��Start������������execǰ���ñ�����������������Ϊfalse,��������)
	// ��    ��:  �ۿ���
	// ����ʱ��:  2012-5-31 10:13
	// ����˵��:  @bBlockΪtrue��ʾ�������У�ֱ�������˳���false��ʾ�����������߳����д������
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool StartAgent(bool bBlock);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �������ӵ�ָ���Ĵ��������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-7-28 14:24
	// ����˵��:  @ip�´����������IP��ַ
	//         :  @portΪ�´����������TCP�˿ںţ�һ��Ϊ6666
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool ReConnectAgentSvr(SString ip,int port=C_SP_UNITAGENT_PORT);

public:
	bool m_bLogined;//�Ƿ��Ѿ���¼
	SString m_sAgentIp;//����IP��ַ��ȱʡΪ127.0.0.1
	int m_iAgentPort;//����˿�
	SPtrList<CAgentPackage> m_SendList;
	SString m_sWaitMsgAct;//�ȴ���Ϣ��act��־
	SString m_sWaitMsg;//�ȴ���Ϣ�ķ����ַ���
	int m_iCpuPercent;
	SApplication *m_pApp;
	int m_iThreads;//�߳�����
	SSocket *m_pAgentSocket;//��ǰͨѶ��SOCKETָ��
};
#endif//if 0

#endif//ifdef _WITHOUT_AGENT_CHANNEL_

//////////////////////////////////////////////////////////////////////////
// ��    ��:  SApplication
// ��    ��:  �ۿ���
// ����ʱ��:  2008-10-10 14:16
// ��    ��:  Ӧ�ó��������
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
	// ��    ��:  ȡSSApplication��ʵ��ָ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-12-16 13:27
	// ����˵��:  void
	// �� �� ֵ:  SApplication*,NULL��ʾ��δʵ��
	//////////////////////////////////////////////////////////////////////////
	static SApplication* GetPtr();
	
	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����Ӧ�ó���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2008-10-10 14:17
	// ����˵��:  @argc/argvΪmain�������
	//            @szDate/szTimeΪ��������ʱ��
	//            @bBlockΪ�Ƿ��������У�trueΪ������falseΪ������
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool Run(int argc, char* argv[],const char* szDate,const char* szTime,bool bBlock=true);
	
	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �������񣬿�������Ӧ�õķ��񣬸ú���������������ʵ�֣�������������ɺ���뷵��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2010-7-1 9:15
	// ����˵��:  
	// �� �� ֵ:  null
	//////////////////////////////////////////////////////////////////////////
	virtual bool Start()=0;

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ֹͣ���񣬹ر�����Ӧ�õķ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2010-7-1 9:18
	// ����˵��:  
	// �� �� ֵ:  null
	//////////////////////////////////////////////////////////////////////////
	virtual bool Stop()=0;

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����ģ��汾��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2009-5-19 10:56
	// ����˵��:  sVer(����:<��Ʒ�ڲ��ͺ�>.<�׶ΰ汾��>.<ģ��汾��>)
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void SetVersion(SString sVer);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����ģ������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2009-5-19 12:29
	// ����˵��:  sDesc����
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void SetModuleDesc(SString sDesc);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����ģ����Ϣ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2009-5-19 12:29
	// ����˵��:  sInfo = name=;ver=;crc=;date=
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void SetModuleInfo(SString sInfo);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����Ӧ�ó����ʶ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-2-3 8:49
	// ����˵��:  @iAppId��ʾӦ�ú�
	//         :  @iInstId��ʾʵ��ID,0��ʾ��ʵ��Ӧ��
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void SetApplicationId(DWORD iAppId,DWORD iInstId=0);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �˳�ģ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-3-29 8:44
	// ����˵��:  
	// �� �� ֵ:  
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
	// ��    ��:  ȡ�����в�������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-5-19 10:13
	// ����˵��:  @sParamNameΪ������
	// �� �� ֵ:  ����ֵ
	//////////////////////////////////////////////////////////////////////////
	SString GetParam(SString sParamName);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ���������Ͳ�������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-5-19 10:13
	// ����˵��:  @sParamNameΪ������
	// �� �� ֵ:  ����ֵ
	//////////////////////////////////////////////////////////////////////////
	int GetParamI(SString sParamName);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ�����и����Ͳ�������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-5-19 10:13
	// ����˵��:  @sParamNameΪ������
	// �� �� ֵ:  ����ֵ
	//////////////////////////////////////////////////////////////////////////
	float GetParamF(SString sParamName);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡϵͳ��Ŀ¼,�Ѱ�����β�ķָ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-7-31 15:44
	// ����˵��:  void
	// �� �� ֵ:  SString
	//////////////////////////////////////////////////////////////////////////
	virtual SString GetHomePath(){return m_sHomePath;};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ����·��,�Ѱ�����β�ķָ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-7-31 15:29
	// ����˵��:  void
	// �� �� ֵ:  SString
	//////////////////////////////////////////////////////////////////////////
	virtual SString GetConfPath(){return m_sConfPath;};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ�����ļ�·��,�Ѱ�����β�ķָ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-7-31 15:29
	// ����˵��:  void
	// �� �� ֵ:  SString
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
	
	bool TouchClose();//����һ�������̵�close�ļ�����֪ͨԭ���̹ر�
	bool RemoveClose();//�Ƴ�close�ļ�
	bool IsClosedExist();//close�ļ��Ƿ���ڣ�
	bool SearchOldProcess(int &pid,int &ppid,SString &name);

	bool _Start();
	bool _Stop();

	SString m_sExePath;//ִ�г�������·��
	SString m_sConfPath;//�����ļ�·��
	SString m_sHomePath;//ϵͳ��Ŀ¼����·��
	SString m_sModule;//ģ����
	SString m_sAppbaseVer;//�����汾
	SString m_sModuleVer;//ģ��汾
	SString m_sModuleCrc;//ģ��CRC
	SString m_sModuleTime;//ģ������ʱ��
	SString m_sModuleDesc;//ģ������
	SString m_sModuleInfo;//ģ����Ϣ
	bool m_bHalt;//�Ƿ�׼���˳�?

	bool m_bDebugMode;//�Ƿ��ڵ���ģʽ
	bool m_bLogToConsole;//�Ƿ�������־��Ϣ���������̨
	bool m_bFork;//LINUX���Ƿ�ִ��fork
	bool m_bStarted;//�Ƿ��Ѿ�����Ӧ��
	bool m_bUseAgent;//�Ƿ�����Ԫ�������

public:
	
	DWORD m_iApplicationId;
	DWORD m_iAppInstId;
	DWORD m_iUnitId;//
	SString m_sUnitName;
	SString m_sUnitDesc;
	int m_iArgc;
	char** m_ppArgv;
	void* m_pUserPtr;//�û�ָ�룬��Ӧ�ò�ʹ��
};



#endif//__YS_REC_APPLICATION_H__
