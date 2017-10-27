/**
 *
 * �� �� �� : SDecodAcsi.h
 * �������� : 2015-6-11 14:11
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : ACSI������
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-6-11	�ۿ�������ļ�
 *
 **/

#ifndef __SKT_DECODE_ACSI_DEF_H__
#define __SKT_DECODE_ACSI_DEF_H__

#include "SInclude.h"
#include "SDecodeBase.h"

enum eAcsiModel
{
	ACSI_MODEL_UNKNOWN=0,		//δ֪
	ACSI_MODEL_SERVER=1,		//������ģ��
	ACSI_MODEL_ASSOCIATION,		//����ģ��
	ACSI_MODEL_LD,				//�߼��豸ģ��
	ACSI_MODEL_LN,				//�߼��ڵ�ģ��
	ACSI_MODEL_DATA,			//����ģ��
	ACSI_MODEL_DATASET,			//���ݼ�ģ��
	ACSI_MODEL_SUBSTITUTION,	//ȡ��ģ��
	ACSI_MODEL_SGCB,			//��ֵ����ƿ�ģ��
	ACSI_MODEL_BRCB,			//���汨����ƿ�ģ��
	ACSI_MODEL_URCB,			//�ǻ��汨����ƿ�ģ��
	ACSI_MODEL_LCB,				//��־���ƿ�ģ��
	ACSI_MODEL_GSE,				//����ͨ�ö���ı��վ�¼�
	ACSI_MODEL_GSSE,			//ͨ�ñ��վ�¼�ģ��
	ACSI_MODEL_MSV,				//�ಥ����ֵ���ƿ�ģ��
	ACSI_MODEL_USV,				//��������ֵ���ƿ�ģ��
	ACSI_MODEL_CTRL,			//����ģ��
	ACSI_MODEL_TIME,			//ʱ��ͬ��ģ��
	ACSI_MODEL_FILE,			//�ļ�����ģ��

	ACSI_MODEL_COUNT,			//ģ��������
};

enum eAcsiServiceMsgType
{
	ACSI_REQUEST=1,		//����
	ACSI_RESPONSE_OK,	//�϶���Ӧ
	ACSI_RESPONSE_ER,	//����Ӧ
};

enum eMmsServiceMsgType
{
	MMS_NONE=0,
	MMS_REQUEST,
	MMS_RESPONSE,
	MMS_ERROR,
};

enum eMmsService
{
	MMS_Unknown = 0,

	//unconfirmed
	MMS_InformationReport,
	MMS_UnsolicitedStatus,
	MMS_EventNotification,

	MMS_Initiate,
	MMS_Conclude,
	MMS_Abort,
	MMS_Reject,
	MMS_Cancel,

	//confirmed
	MMS_Status,
	MMS_GetNameList,
	MMS_Identify,
	MMS_Rename,
	MMS_Read,
	MMS_Write,
	MMS_GetVariableAccessAttributes,//"getVariableAccessAttributes" },
	MMS_DefineNamedVariable,//"defineNamedVariable" },
	MMS_DefineScatteredAccess,//"defineScatteredAccess" },
	MMS_GetScatteredAccessAttributes,//"getScatteredAccessAttributes" },
	MMS_DeleteVariableAccess,//"deleteVariableAccess" },
	MMS_DefineNamedVariableList,//"defineNamedVariableList" },
	MMS_GetNamedVariableListAttributes,//"getNamedVariableListAttributes" },
	MMS_DeleteNamedVariableList,//"deleteNamedVariableList" },
	MMS_DefineNamedType,//"defineNamedType" },
	MMS_GetNamedTypeAttributes,//"getNamedTypeAttributes" },
	MMS_DeleteNamedType,//"deleteNamedType" },
	MMS_Input,//"input" },
	MMS_Output,//"output" },
	MMS_TakeControl,//"takeControl" },
	MMS_RelinquishControl,//"relinquishControl" },
	MMS_DefineSemaphore,//"defineSemaphore" },
	MMS_DeleteSemaphore,//"deleteSemaphore" },
	MMS_ReportSemaphoreStatus,//"reportSemaphoreStatus" },
	MMS_ReportPoolSemaphoreStatus,//"reportPoolSemaphoreStatus" },
	MMS_ReportSemaphoreEntryStatus,//"reportSemaphoreEntryStatus" },
	MMS_InitiateDownloadSequence,//"initiateDownloadSequence" },
	MMS_DownloadSegment,//"downloadSegment" },
	MMS_TerminateDownloadSequence,//"terminateDownloadSequence" },
	MMS_InitiateUploadSequence,//"initiateUploadSequence" },
	MMS_UploadSegment,//"uploadSegment" },
	MMS_TerminateUploadSequence,//"terminateUploadSequence" },
	MMS_RequestDomainDownload,//"requestDomainDownload" },
	MMS_RequestDomainUpload,//"requestDomainUpload" },
	MMS_LoadDomainContent,//"loadDomainContent" },
	MMS_StoreDomainContent,//"storeDomainContent" },
	MMS_DeleteDomain,//"deleteDomain" },
	MMS_GetDomainAttributes,//"getDomainAttributes" },
	MMS_CreateProgramInvocation,//"createProgramInvocation" },
	MMS_DeleteProgramInvocation,//"deleteProgramInvocation" },
	MMS_Start,//"start" },
	MMS_Stop,//"stop" },
	MMS_Resume,//"resume" },
	MMS_Reset,//"reset" },
	MMS_Kill,//"kill" },
	MMS_GetProgramInvocationAttributes,//"getProgramInvocationAttributes" },
	MMS_ObtainFile,//"obtainFile" },
	MMS_DefineEventCondition,//"defineEventCondition" },
	MMS_DeleteEventCondition,//"deleteEventCondition" },
	MMS_GetEventConditionAttributes,//"getEventConditionAttributes" },
	MMS_ReportEventConditionStatus,//"reportEventConditionStatus" },
	MMS_AlterEventConditionMonitoring,//"alterEventConditionMonitoring" },
	MMS_TriggerEvent,//"triggerEvent" },
	MMS_DefineEventAction,//"defineEventAction" },
	MMS_DeleteEventAction,//"deleteEventAction" },
	MMS_GetEventActionAttributes,//"getEventActionAttributes" },
	MMS_ReportEventActionStatus,//"reportEventActionStatus" },
	MMS_DefineEventEnrollment,//"defineEventEnrollment" },
	MMS_DeleteEventEnrollment,//"deleteEventEnrollment" },
	MMS_AlterEventEnrollment,//"alterEventEnrollment" },
	MMS_ReportEventEnrollmentStatus,//"reportEventEnrollmentStatus" },
	MMS_GetEventEnrollmentAttributes,//"getEventEnrollmentAttributes" },
	MMS_AcknowledgeEventNotification,//"acknowledgeEventNotification" },
	MMS_GetAlarmSummary,//"getAlarmSummary" },
	MMS_GetAlarmEnrollmentSummary,//"getAlarmEnrollmentSummary" },
	MMS_ReadJournal,//"readJournal" },
	MMS_WriteJournal,//"writeJournal" },
	MMS_InitializeJournal,//"initializeJournal" },
	MMS_ReportJournalStatus,//"reportJournalStatus" },
	MMS_CreateJournal,//"createJournal" },
	MMS_DeleteJournal,//"deleteJournal" },
	MMS_GetCapabilityList,//"getCapabilityList" },
	MMS_FileOpen,//"fileOpen" },
	MMS_FileRead,//"fileRead" },
	MMS_FileClose,//"fileClose" },
	MMS_FileRename,//"fileRename" },
	MMS_FileDelete,//"fileDelete" },
	MMS_FileDirectory,//"fileDirectory" },
};
class SDecodeAcsiBaseInfo
{
public:
	SDecodeAcsiBaseInfo()
	{
		m_iServiceType = 0;
		m_bRequest = false;
	};
	virtual ~SDecodeAcsiBaseInfo(){};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���ݵ�ǰģ�����ͣ����ض�Ӧ��ģ������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-6-11 14:31
	// ����˵��:  @typeΪ����
	// �� �� ֵ:  SString
	//////////////////////////////////////////////////////////////////////////
	static SString GetModelName(int type);

	inline void SetService(int iService,int iMsg)
	{
		m_iServiceType = iService;
		m_iMsgType = (eAcsiServiceMsgType)iMsg;
	}

	virtual SString GetServiceName()=0;
	virtual void FillAcsiDecodeTree(SDecodeTree *pAcsiRoot){};
	int m_iServiceType;//��������
	eAcsiServiceMsgType m_iMsgType;//��Ϣ����
	eAcsiModel m_iAcsiModelType;//ACSIģ������
	bool m_bRequest;//�Ƿ��յ�Request�����յ�RESPONSE���һ��������Ӧ����Ϊfalse�ȴ���һ��
	int m_iInvokeID;
};

class SDecodeAcsiInfo_Server : public SDecodeAcsiBaseInfo
{
public:
	enum eServiceType
	{
		GetServerDirectory=0,//��������Ŀ¼
	};

	SDecodeAcsiInfo_Server(){m_iAcsiModelType=ACSI_MODEL_SERVER;};
	virtual ~SDecodeAcsiInfo_Server(){};
	virtual SString GetServiceName()
	{
		if(m_iServiceType == GetServerDirectory)
			return "��������Ŀ¼";
		return SString::toFormat("δ֪����:%d",m_iServiceType);
	};
};


class SDecodeAcsiInfo_Association : public SDecodeAcsiBaseInfo
{
public:
	enum eServiceType
	{
		Associate=0,//����
		Abort,		//�쳣��ֹ
		Release,	//�ͷ�
	};

	SDecodeAcsiInfo_Association(){m_iAcsiModelType=ACSI_MODEL_ASSOCIATION;};
	virtual ~SDecodeAcsiInfo_Association(){};
	virtual SString GetServiceName()
	{
		if(m_iServiceType == Associate)
			return "����";
		else if(m_iServiceType == Abort)
			return "�쳣��ֹ";
		else if(m_iServiceType == Release)
			return "�ͷ�";
		return SString::toFormat("δ֪����:%d",m_iServiceType);
	};
};


class SDecodeAcsiInfo_LD : public SDecodeAcsiBaseInfo
{
public:
	enum eServiceType
	{
		GetLogicalDeviceDirectory=0,//��ȡ�߼��豸Ŀ¼
	};

	SDecodeAcsiInfo_LD(){m_iAcsiModelType=ACSI_MODEL_LD;};
	virtual ~SDecodeAcsiInfo_LD(){};
	virtual SString GetServiceName()
	{
		if(m_iServiceType == GetLogicalDeviceDirectory)
			return "��ȡ�߼��豸Ŀ¼";
		return SString::toFormat("δ֪����:%d",m_iServiceType);
	};
};


class SDecodeAcsiInfo_LN : public SDecodeAcsiBaseInfo
{
public:
	enum eServiceType
	{
		GetLogicalNodeDirectory=0,//���߼��ڵ�Ŀ¼
		GetAllDataValues,			//����������ֵ
	};

	SDecodeAcsiInfo_LN(){m_iAcsiModelType=ACSI_MODEL_LN;};
	virtual ~SDecodeAcsiInfo_LN(){};
	virtual SString GetServiceName()
	{
		if(m_iServiceType == GetLogicalNodeDirectory)
			return "���߼��ڵ�Ŀ¼";
		else if(m_iServiceType == GetAllDataValues)
			return "����������ֵ";
		return SString::toFormat("δ֪����:%d",m_iServiceType);
	};
};


class SDecodeAcsiInfo_Data : public SDecodeAcsiBaseInfo
{
public:
	enum eServiceType
	{
		GetDataValues=0,//������ֵ
		SetDataValues,	//��������ֵ
		GetDataDirectory,//������Ŀ¼
		GetDataDefinition,//�����ݶ���
	};

	SDecodeAcsiInfo_Data(){m_iAcsiModelType=ACSI_MODEL_DATA;};
	virtual ~SDecodeAcsiInfo_Data(){};
	virtual SString GetServiceName()
	{
		if(m_iServiceType == GetDataValues)
			return "������ֵ";
		else if(m_iServiceType == SetDataValues)
			return "��������ֵ";
		else if(m_iServiceType == GetDataDirectory)
			return "������Ŀ¼";
		else if(m_iServiceType == GetDataDefinition)
			return "�����ݶ���";
		return SString::toFormat("δ֪����:%d",m_iServiceType);
	};
};


class SDecodeAcsiInfo_Dataset : public SDecodeAcsiBaseInfo
{
public:
	enum eServiceType
	{
		GetDataSetValues=0,//�����ݼ�ֵ
		SetDataSetValues,	//�������ݼ�ֵ
		CreateDataSet,//�������ݼ�
		DeleteDataSet,//ɾ�����ݼ�
		GetDataSetDirectory,//�����ݼ�Ŀ¼
	};

	SDecodeAcsiInfo_Dataset(){m_iAcsiModelType=ACSI_MODEL_DATASET;};
	virtual ~SDecodeAcsiInfo_Dataset(){};
	virtual SString GetServiceName()
	{
		if(m_iServiceType == GetDataSetValues)
			return "�����ݼ�ֵ";
		else if(m_iServiceType == SetDataSetValues)
			return "�������ݼ�ֵ";
		else if(m_iServiceType == CreateDataSet)
			return "�������ݼ�";
		else if(m_iServiceType == DeleteDataSet)
			return "ɾ�����ݼ�";
		else if(m_iServiceType == GetDataSetDirectory)
			return "�����ݼ�Ŀ¼";
		return SString::toFormat("δ֪����:%d",m_iServiceType);
	};
};


class SDecodeAcsiInfo_Substitution : public SDecodeAcsiBaseInfo
{
public:
	enum eServiceType
	{
		SetDataValues=0,//��������ֵ
		GetDataValues,//������ֵ
	};

	SDecodeAcsiInfo_Substitution(){m_iAcsiModelType=ACSI_MODEL_SUBSTITUTION;};
	virtual ~SDecodeAcsiInfo_Substitution(){};
	virtual SString GetServiceName()
	{
		if(m_iServiceType == SetDataValues)
			return "��������ֵ";
		else if(m_iServiceType == GetDataValues)
			return "������ֵ";
		return SString::toFormat("δ֪����:%d",m_iServiceType);
	};
};


class SDecodeAcsiInfo_SGCB : public SDecodeAcsiBaseInfo
{
public:
	enum eServiceType
	{
		SelectActiveSG=0,//ѡ�񼤻ֵ��
		SelectEditSG,//ѡ��༭��ֵ��
		SetSGValues,//���ö�ֵ��ֵ
		ConfirmEditSGValues,//ȷ�ϱ༭��ֵ��ֵ
		GetSGValues,//����ֵ��ֵ
		GetSGCBValues,//����ֵ����ƿ�ֵ
	};

	SDecodeAcsiInfo_SGCB(){m_iAcsiModelType=ACSI_MODEL_SGCB;};
	virtual ~SDecodeAcsiInfo_SGCB(){};
	virtual SString GetServiceName()
	{
		if(m_iServiceType == SelectActiveSG)
			return "ѡ�񼤻ֵ��";
		else if(m_iServiceType == SelectEditSG)
			return "ѡ��༭��ֵ��";
		else if(m_iServiceType == SetSGValues)
			return "���ö�ֵ��ֵ";
		else if(m_iServiceType == ConfirmEditSGValues)
			return "ȷ�ϱ༭��ֵ��ֵ";
		else if(m_iServiceType == GetSGValues)
			return "����ֵ��ֵ";
		else if(m_iServiceType == GetSGCBValues)
			return "����ֵ����ƿ�ֵ";
		return SString::toFormat("δ֪����:%d",m_iServiceType);
	};
};


class SDecodeAcsiInfo_BRCB : public SDecodeAcsiBaseInfo
{
public:
	enum eServiceType
	{
		Report=0,//����
		GetBRCBValues,//�����汨����ƿ�ֵ
		SetBRCBValues,//���û��汨����ƿ�ֵ
	};

	SDecodeAcsiInfo_BRCB(){m_iAcsiModelType=ACSI_MODEL_BRCB;};
	virtual ~SDecodeAcsiInfo_BRCB(){};
	virtual SString GetServiceName()
	{
		if(m_iServiceType == Report)
			return "����";
		else if(m_iServiceType == GetBRCBValues)
			return "�����汨����ƿ�ֵ";
		else if(m_iServiceType == SetBRCBValues)
			return "���û��汨����ƿ�ֵ";
		return SString::toFormat("δ֪����:%d",m_iServiceType);
	};
};


class SDecodeAcsiInfo_URCB : public SDecodeAcsiBaseInfo
{
public:
	enum eServiceType
	{
		Report=0,//����
		GetBRCBValues,//�����汨����ƿ�ֵ
		SetBRCBValues,//���û��汨����ƿ�ֵ
	};

	SDecodeAcsiInfo_URCB(){m_iAcsiModelType=ACSI_MODEL_URCB;};
	virtual ~SDecodeAcsiInfo_URCB(){};
	virtual SString GetServiceName()
	{
		if(m_iServiceType == Report)
			return "����";
		else if(m_iServiceType == GetBRCBValues)
			return "���ǻ��汨����ƿ�ֵ";
		else if(m_iServiceType == SetBRCBValues)
			return "���÷ǻ��汨����ƿ�ֵ";
		return SString::toFormat("δ֪����:%d",m_iServiceType);
	};
};


class SDecodeAcsiInfo_LCB : public SDecodeAcsiBaseInfo
{
public:
	enum eServiceType
	{
		GetLCBValues=0,//����־���ƿ�ֵ
		SetLCBValues,//������־���ƿ�ֵ
		QueryLogByTime,//��ʱ���ѯ��־
		QueryLogAfter,//��ѯĳ��Ŀ�Ժ����־
		GetLogStatusValues,//����־״ֵ̬
	};

	SDecodeAcsiInfo_LCB(){m_iAcsiModelType=ACSI_MODEL_LCB;};
	virtual ~SDecodeAcsiInfo_LCB(){};
	virtual SString GetServiceName()
	{
		if(m_iServiceType == GetLCBValues)
			return "����־���ƿ�ֵ";
		else if(m_iServiceType == SetLCBValues)
			return "������־���ƿ�ֵ";
		else if(m_iServiceType == QueryLogByTime)
			return "��ʱ���ѯ��־";
		else if(m_iServiceType == QueryLogAfter)
			return "��ѯĳ��Ŀ�Ժ����־";
		else if(m_iServiceType == GetLogStatusValues)
			return "����־״ֵ̬";
		return SString::toFormat("δ֪����:%d",m_iServiceType);
	};
};


class SDecodeAcsiInfo_GSE : public SDecodeAcsiBaseInfo
{
public:
	enum eServiceType
	{
		SendGOOSEMessage=0,//����GOOSE����
		GetGoReference,//��Go����
		GetGOOSEElementNumber,//��GOOSEԪ����Ŀ
		GetGoCBValues,//��GOOSE���ƿ�ֵ
		SetGoCBValues,//����GOOSE���ƿ�ֵ
	};

	SDecodeAcsiInfo_GSE(){m_iAcsiModelType=ACSI_MODEL_GSE;};
	virtual ~SDecodeAcsiInfo_GSE(){};
	virtual SString GetServiceName()
	{
		if(m_iServiceType == SendGOOSEMessage)
			return "����GOOSE����";
		else if(m_iServiceType == GetGoReference)
			return "��Go����";
		else if(m_iServiceType == GetGOOSEElementNumber)
			return "��GOOSEԪ����Ŀ";
		else if(m_iServiceType == GetGoCBValues)
			return "��GOOSE���ƿ�ֵ";
		else if(m_iServiceType == SetGoCBValues)
			return "����GOOSE���ƿ�ֵ";
		return SString::toFormat("δ֪����:%d",m_iServiceType);
	};
};


class SDecodeAcsiInfo_GSSE : public SDecodeAcsiBaseInfo
{
public:
	enum eServiceType
	{
		SendGSSEMessage=0,//����GSSE����
		GetGsReference,//��Gs����
		GetGSSEElementNumber,//��GSSEԪ����Ŀ
		GetGsCBValues,//��GSSE���ƿ�ֵ
		SetGsCBValues,//����GSSE���ƿ�ֵ
	};

	SDecodeAcsiInfo_GSSE(){m_iAcsiModelType=ACSI_MODEL_GSSE;};
	virtual ~SDecodeAcsiInfo_GSSE(){};
	virtual SString GetServiceName()
	{
		if(m_iServiceType == SendGSSEMessage)
			return "����GSSE����";
		else if(m_iServiceType == GetGsReference)
			return "��Gs����";
		else if(m_iServiceType == GetGSSEElementNumber)
			return "��GSSEԪ����Ŀ";
		else if(m_iServiceType == GetGsCBValues)
			return "��GSSE���ƿ�ֵ";
		else if(m_iServiceType == SetGsCBValues)
			return "����GSSE���ƿ�ֵ";
		return SString::toFormat("δ֪����:%d",m_iServiceType);
	};
};


class SDecodeAcsiInfo_MSV : public SDecodeAcsiBaseInfo
{
public:
	enum eServiceType
	{
		SendMSVMessage=0,//����MSV����
		GetMSVCBValues,//��MSV���ƿ�ֵ
		SetMSVCBValues,//����MSV���ƿ�ֵ
	};

	SDecodeAcsiInfo_MSV(){m_iAcsiModelType=ACSI_MODEL_MSV;};
	virtual ~SDecodeAcsiInfo_MSV(){};
	virtual SString GetServiceName()
	{
		if(m_iServiceType == SendMSVMessage)
			return "����MSV����";
		else if(m_iServiceType == GetMSVCBValues)
			return "��MSV���ƿ�ֵ";
		else if(m_iServiceType == SetMSVCBValues)
			return "����MSV���ƿ�ֵ";
		return SString::toFormat("δ֪����:%d",m_iServiceType);
	};
};


class SDecodeAcsiInfo_USV : public SDecodeAcsiBaseInfo
{
public:
	enum eServiceType
	{
		SendUSVMessage=0,//����USV����
		GetUSVCBValues,//��USV���ƿ�ֵ
		SetUSVCBValues,//����USV���ƿ�ֵ
	};

	SDecodeAcsiInfo_USV(){m_iAcsiModelType=ACSI_MODEL_USV;};
	virtual ~SDecodeAcsiInfo_USV(){};
	virtual SString GetServiceName()
	{
		if(m_iServiceType == SendUSVMessage)
			return "����USV����";
		else if(m_iServiceType == GetUSVCBValues)
			return "��USV���ƿ�ֵ";
		else if(m_iServiceType == SetUSVCBValues)
			return "����USV���ƿ�ֵ";
		return SString::toFormat("δ֪����:%d",m_iServiceType);
	};
};



class SDecodeAcsiInfo_Ctrl : public SDecodeAcsiBaseInfo
{
public:
	enum eServiceType
	{
		Select=0,//ѡ��
		SelectWithValue,//��ֵѡ��
		Cancel,//ȡ��
		Operate,//ִ��
		CommandTermination,//��ֹ
		TimeActivatedOperate,//ʱ�伤�����
	};

	SDecodeAcsiInfo_Ctrl(){m_iAcsiModelType=ACSI_MODEL_CTRL;};
	virtual ~SDecodeAcsiInfo_Ctrl(){};
	virtual SString GetServiceName()
	{
		if(m_iServiceType == Select)
			return "ѡ��";
		else if(m_iServiceType == SelectWithValue)
			return "��ֵѡ��";
		else if(m_iServiceType == Cancel)
			return "ȡ��";
		else if(m_iServiceType == Operate)
			return "ִ��";
		else if(m_iServiceType == CommandTermination)
			return "��ֹ";
		else if(m_iServiceType == TimeActivatedOperate)
			return "ʱ�伤�����";
		return SString::toFormat("δ֪����:%d",m_iServiceType);
	};
};


class SDecodeAcsiInfo_Time : public SDecodeAcsiBaseInfo
{
public:
	enum eServiceType
	{
		TimeSynchronisation=0,//ʱ��ͬ��
	};

	SDecodeAcsiInfo_Time(){m_iAcsiModelType=ACSI_MODEL_TIME;};
	virtual ~SDecodeAcsiInfo_Time(){};
	virtual SString GetServiceName()
	{
		if(m_iServiceType == TimeSynchronisation)
			return "ʱ��ͬ��";
		return SString::toFormat("δ֪����:%d",m_iServiceType);
	};
};


class SDecodeAcsiInfo_File : public SDecodeAcsiBaseInfo
{
public:
	enum eServiceType
	{
		GetFile=0,//���ļ�
		SetFile,//�����ļ�
		DeleteFile,//ɾ���ļ�
		GetFileAttributeValues,//���ļ�����ֵ
	};

	SDecodeAcsiInfo_File(){m_iAcsiModelType=ACSI_MODEL_FILE;};
	virtual ~SDecodeAcsiInfo_File(){};
	virtual SString GetServiceName()
	{
		if(m_iServiceType == GetFile)
			return "���ļ�";
		else if(m_iServiceType == SetFile)
			return "�����ļ�";
		else if(m_iServiceType == DeleteFile)
			return "ɾ���ļ�";
		else if(m_iServiceType == GetFileAttributeValues)
			return "���ļ�����ֵ";
		return SString::toFormat("δ֪����:%d",m_iServiceType);
	};
};

class SDecodeAcsiInfo_All
{
public:
	SDecodeAcsiInfo_All(){m_iAcsiModelType = ACSI_MODEL_UNKNOWN;};
	~SDecodeAcsiInfo_All(){};
	SString GetACSITitle();
	inline void Clear()
	{
		m_iInvokeID = -1;
		m_pLastAcsiInfo = NULL;
		m_iMmsService = MMS_Unknown;
		m_iMmsServiceMsgType = MMS_NONE;
		m_iAcsiModelType = ACSI_MODEL_UNKNOWN;
	}
	inline void SetModelType(eAcsiModel m)
	{
		m_iAcsiModelType = m;
		m_pLastAcsiInfo = GetAcsiInfo();
	}
	void SetModelMsgType(eAcsiModel m,int svr);
	inline void SetMmsType(eMmsService s,eMmsServiceMsgType m)
	{
		m_iMmsService = s;
		m_iMmsServiceMsgType = m;
	}
	SDecodeAcsiBaseInfo* GetAcsiInfo();
	SDecodeAcsiBaseInfo* SearchByInvokeID();
	void AnalyzeAcsi(void *p);
	void FillAcsiDecodeTree(SDecodeTree *pAcsiRoot);

	eAcsiModel m_iAcsiModelType;//ACSIģ������
	eMmsServiceMsgType m_iMmsServiceMsgType;
	eMmsService        m_iMmsService;
	SDecodeAcsiBaseInfo* m_pLastAcsiInfo;

	SDecodeAcsiInfo_Server			m_Model_Server;
	SDecodeAcsiInfo_Association		m_Model_Association;
	SDecodeAcsiInfo_LD				m_Model_LD;
	SDecodeAcsiInfo_LN				m_Model_LN;
	SDecodeAcsiInfo_Data			m_Model_Data;
	SDecodeAcsiInfo_Dataset			m_Model_Dataset;
	SDecodeAcsiInfo_Substitution	m_Model_Substitution;
	SDecodeAcsiInfo_SGCB			m_Model_SGCB;
	SDecodeAcsiInfo_BRCB			m_Model_BRCB;
	SDecodeAcsiInfo_URCB			m_Model_URCB;
	SDecodeAcsiInfo_LCB				m_Model_LCB;
	SDecodeAcsiInfo_GSE				m_Model_GSE;
	SDecodeAcsiInfo_GSSE			m_Model_GSSE;
	SDecodeAcsiInfo_MSV				m_Model_MSV;
	SDecodeAcsiInfo_USV				m_Model_USV;
	SDecodeAcsiInfo_Ctrl			m_Model_Ctrl;
	SDecodeAcsiInfo_Time			m_Model_Time;
	SDecodeAcsiInfo_File			m_Model_File;

	//MMSһ�ν�����м�ֵ
// 	int m_iObjectClass;
 	int m_iInvokeID;
// 	char m_sIdentifierStr[512];
};
#endif//__SKT_DECODE_ACSI_DEF_H__

