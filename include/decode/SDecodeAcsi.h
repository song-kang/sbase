/**
 *
 * 文 件 名 : SDecodAcsi.h
 * 创建日期 : 2015-6-11 14:11
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : ACSI服务定义
 * 修改记录 : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-6-11	邵凯田　创建文件
 *
 **/

#ifndef __SKT_DECODE_ACSI_DEF_H__
#define __SKT_DECODE_ACSI_DEF_H__

#include "SInclude.h"
#include "SDecodeBase.h"

enum eAcsiModel
{
	ACSI_MODEL_UNKNOWN=0,		//未知
	ACSI_MODEL_SERVER=1,		//服务器模型
	ACSI_MODEL_ASSOCIATION,		//关联模型
	ACSI_MODEL_LD,				//逻辑设备模型
	ACSI_MODEL_LN,				//逻辑节点模型
	ACSI_MODEL_DATA,			//数据模型
	ACSI_MODEL_DATASET,			//数据集模型
	ACSI_MODEL_SUBSTITUTION,	//取代模型
	ACSI_MODEL_SGCB,			//定值组控制块模型
	ACSI_MODEL_BRCB,			//缓存报告控制块模型
	ACSI_MODEL_URCB,			//非缓存报告控制块模型
	ACSI_MODEL_LCB,				//日志控制块模型
	ACSI_MODEL_GSE,				//面向通用对象的变电站事件
	ACSI_MODEL_GSSE,			//通用变电站事件模型
	ACSI_MODEL_MSV,				//多播采样值控制块模型
	ACSI_MODEL_USV,				//单播采样值控制块模型
	ACSI_MODEL_CTRL,			//控制模型
	ACSI_MODEL_TIME,			//时间同步模型
	ACSI_MODEL_FILE,			//文件传输模型

	ACSI_MODEL_COUNT,			//模型总数量
};

enum eAcsiServiceMsgType
{
	ACSI_REQUEST=1,		//请求
	ACSI_RESPONSE_OK,	//肯定响应
	ACSI_RESPONSE_ER,	//否定响应
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
	// 描    述:  根据当前模型类型，返回对应的模型名称
	// 作    者:  邵凯田
	// 创建时间:  2015-6-11 14:31
	// 参数说明:  @type为类型
	// 返 回 值:  SString
	//////////////////////////////////////////////////////////////////////////
	static SString GetModelName(int type);

	inline void SetService(int iService,int iMsg)
	{
		m_iServiceType = iService;
		m_iMsgType = (eAcsiServiceMsgType)iMsg;
	}

	virtual SString GetServiceName()=0;
	virtual void FillAcsiDecodeTree(SDecodeTree *pAcsiRoot){};
	int m_iServiceType;//服务类型
	eAcsiServiceMsgType m_iMsgType;//消息类型
	eAcsiModel m_iAcsiModelType;//ACSI模型类型
	bool m_bRequest;//是否收到Request请求，收到RESPONSE完成一次请求响应后，置为false等待下一次
	int m_iInvokeID;
};

class SDecodeAcsiInfo_Server : public SDecodeAcsiBaseInfo
{
public:
	enum eServiceType
	{
		GetServerDirectory=0,//读服务器目录
	};

	SDecodeAcsiInfo_Server(){m_iAcsiModelType=ACSI_MODEL_SERVER;};
	virtual ~SDecodeAcsiInfo_Server(){};
	virtual SString GetServiceName()
	{
		if(m_iServiceType == GetServerDirectory)
			return "读服务器目录";
		return SString::toFormat("未知服务:%d",m_iServiceType);
	};
};


class SDecodeAcsiInfo_Association : public SDecodeAcsiBaseInfo
{
public:
	enum eServiceType
	{
		Associate=0,//关联
		Abort,		//异常中止
		Release,	//释放
	};

	SDecodeAcsiInfo_Association(){m_iAcsiModelType=ACSI_MODEL_ASSOCIATION;};
	virtual ~SDecodeAcsiInfo_Association(){};
	virtual SString GetServiceName()
	{
		if(m_iServiceType == Associate)
			return "关联";
		else if(m_iServiceType == Abort)
			return "异常中止";
		else if(m_iServiceType == Release)
			return "释放";
		return SString::toFormat("未知服务:%d",m_iServiceType);
	};
};


class SDecodeAcsiInfo_LD : public SDecodeAcsiBaseInfo
{
public:
	enum eServiceType
	{
		GetLogicalDeviceDirectory=0,//读取逻辑设备目录
	};

	SDecodeAcsiInfo_LD(){m_iAcsiModelType=ACSI_MODEL_LD;};
	virtual ~SDecodeAcsiInfo_LD(){};
	virtual SString GetServiceName()
	{
		if(m_iServiceType == GetLogicalDeviceDirectory)
			return "读取逻辑设备目录";
		return SString::toFormat("未知服务:%d",m_iServiceType);
	};
};


class SDecodeAcsiInfo_LN : public SDecodeAcsiBaseInfo
{
public:
	enum eServiceType
	{
		GetLogicalNodeDirectory=0,//读逻辑节点目录
		GetAllDataValues,			//读所有数据值
	};

	SDecodeAcsiInfo_LN(){m_iAcsiModelType=ACSI_MODEL_LN;};
	virtual ~SDecodeAcsiInfo_LN(){};
	virtual SString GetServiceName()
	{
		if(m_iServiceType == GetLogicalNodeDirectory)
			return "读逻辑节点目录";
		else if(m_iServiceType == GetAllDataValues)
			return "读所有数据值";
		return SString::toFormat("未知服务:%d",m_iServiceType);
	};
};


class SDecodeAcsiInfo_Data : public SDecodeAcsiBaseInfo
{
public:
	enum eServiceType
	{
		GetDataValues=0,//读数据值
		SetDataValues,	//设置数据值
		GetDataDirectory,//读数据目录
		GetDataDefinition,//读数据定义
	};

	SDecodeAcsiInfo_Data(){m_iAcsiModelType=ACSI_MODEL_DATA;};
	virtual ~SDecodeAcsiInfo_Data(){};
	virtual SString GetServiceName()
	{
		if(m_iServiceType == GetDataValues)
			return "读数据值";
		else if(m_iServiceType == SetDataValues)
			return "设置数据值";
		else if(m_iServiceType == GetDataDirectory)
			return "读数据目录";
		else if(m_iServiceType == GetDataDefinition)
			return "读数据定义";
		return SString::toFormat("未知服务:%d",m_iServiceType);
	};
};


class SDecodeAcsiInfo_Dataset : public SDecodeAcsiBaseInfo
{
public:
	enum eServiceType
	{
		GetDataSetValues=0,//读数据集值
		SetDataSetValues,	//设置数据集值
		CreateDataSet,//建立数据集
		DeleteDataSet,//删除数据集
		GetDataSetDirectory,//读数据集目录
	};

	SDecodeAcsiInfo_Dataset(){m_iAcsiModelType=ACSI_MODEL_DATASET;};
	virtual ~SDecodeAcsiInfo_Dataset(){};
	virtual SString GetServiceName()
	{
		if(m_iServiceType == GetDataSetValues)
			return "读数据集值";
		else if(m_iServiceType == SetDataSetValues)
			return "设置数据集值";
		else if(m_iServiceType == CreateDataSet)
			return "建立数据集";
		else if(m_iServiceType == DeleteDataSet)
			return "删除数据集";
		else if(m_iServiceType == GetDataSetDirectory)
			return "读数据集目录";
		return SString::toFormat("未知服务:%d",m_iServiceType);
	};
};


class SDecodeAcsiInfo_Substitution : public SDecodeAcsiBaseInfo
{
public:
	enum eServiceType
	{
		SetDataValues=0,//设置数据值
		GetDataValues,//读数据值
	};

	SDecodeAcsiInfo_Substitution(){m_iAcsiModelType=ACSI_MODEL_SUBSTITUTION;};
	virtual ~SDecodeAcsiInfo_Substitution(){};
	virtual SString GetServiceName()
	{
		if(m_iServiceType == SetDataValues)
			return "设置数据值";
		else if(m_iServiceType == GetDataValues)
			return "读数据值";
		return SString::toFormat("未知服务:%d",m_iServiceType);
	};
};


class SDecodeAcsiInfo_SGCB : public SDecodeAcsiBaseInfo
{
public:
	enum eServiceType
	{
		SelectActiveSG=0,//选择激活定值组
		SelectEditSG,//选择编辑定值组
		SetSGValues,//设置定值组值
		ConfirmEditSGValues,//确认编辑定值组值
		GetSGValues,//读定值组值
		GetSGCBValues,//读定值组控制块值
	};

	SDecodeAcsiInfo_SGCB(){m_iAcsiModelType=ACSI_MODEL_SGCB;};
	virtual ~SDecodeAcsiInfo_SGCB(){};
	virtual SString GetServiceName()
	{
		if(m_iServiceType == SelectActiveSG)
			return "选择激活定值组";
		else if(m_iServiceType == SelectEditSG)
			return "选择编辑定值组";
		else if(m_iServiceType == SetSGValues)
			return "设置定值组值";
		else if(m_iServiceType == ConfirmEditSGValues)
			return "确认编辑定值组值";
		else if(m_iServiceType == GetSGValues)
			return "读定值组值";
		else if(m_iServiceType == GetSGCBValues)
			return "读定值组控制块值";
		return SString::toFormat("未知服务:%d",m_iServiceType);
	};
};


class SDecodeAcsiInfo_BRCB : public SDecodeAcsiBaseInfo
{
public:
	enum eServiceType
	{
		Report=0,//报告
		GetBRCBValues,//读缓存报告控制块值
		SetBRCBValues,//设置缓存报告控制块值
	};

	SDecodeAcsiInfo_BRCB(){m_iAcsiModelType=ACSI_MODEL_BRCB;};
	virtual ~SDecodeAcsiInfo_BRCB(){};
	virtual SString GetServiceName()
	{
		if(m_iServiceType == Report)
			return "报告";
		else if(m_iServiceType == GetBRCBValues)
			return "读缓存报告控制块值";
		else if(m_iServiceType == SetBRCBValues)
			return "设置缓存报告控制块值";
		return SString::toFormat("未知服务:%d",m_iServiceType);
	};
};


class SDecodeAcsiInfo_URCB : public SDecodeAcsiBaseInfo
{
public:
	enum eServiceType
	{
		Report=0,//报告
		GetBRCBValues,//读缓存报告控制块值
		SetBRCBValues,//设置缓存报告控制块值
	};

	SDecodeAcsiInfo_URCB(){m_iAcsiModelType=ACSI_MODEL_URCB;};
	virtual ~SDecodeAcsiInfo_URCB(){};
	virtual SString GetServiceName()
	{
		if(m_iServiceType == Report)
			return "报告";
		else if(m_iServiceType == GetBRCBValues)
			return "读非缓存报告控制块值";
		else if(m_iServiceType == SetBRCBValues)
			return "设置非缓存报告控制块值";
		return SString::toFormat("未知服务:%d",m_iServiceType);
	};
};


class SDecodeAcsiInfo_LCB : public SDecodeAcsiBaseInfo
{
public:
	enum eServiceType
	{
		GetLCBValues=0,//读日志控制块值
		SetLCBValues,//设置日志控制块值
		QueryLogByTime,//按时间查询日志
		QueryLogAfter,//查询某条目以后的日志
		GetLogStatusValues,//读日志状态值
	};

	SDecodeAcsiInfo_LCB(){m_iAcsiModelType=ACSI_MODEL_LCB;};
	virtual ~SDecodeAcsiInfo_LCB(){};
	virtual SString GetServiceName()
	{
		if(m_iServiceType == GetLCBValues)
			return "读日志控制块值";
		else if(m_iServiceType == SetLCBValues)
			return "设置日志控制块值";
		else if(m_iServiceType == QueryLogByTime)
			return "按时间查询日志";
		else if(m_iServiceType == QueryLogAfter)
			return "查询某条目以后的日志";
		else if(m_iServiceType == GetLogStatusValues)
			return "读日志状态值";
		return SString::toFormat("未知服务:%d",m_iServiceType);
	};
};


class SDecodeAcsiInfo_GSE : public SDecodeAcsiBaseInfo
{
public:
	enum eServiceType
	{
		SendGOOSEMessage=0,//发送GOOSE报文
		GetGoReference,//读Go引用
		GetGOOSEElementNumber,//读GOOSE元素数目
		GetGoCBValues,//读GOOSE控制块值
		SetGoCBValues,//设置GOOSE控制块值
	};

	SDecodeAcsiInfo_GSE(){m_iAcsiModelType=ACSI_MODEL_GSE;};
	virtual ~SDecodeAcsiInfo_GSE(){};
	virtual SString GetServiceName()
	{
		if(m_iServiceType == SendGOOSEMessage)
			return "发送GOOSE报文";
		else if(m_iServiceType == GetGoReference)
			return "读Go引用";
		else if(m_iServiceType == GetGOOSEElementNumber)
			return "读GOOSE元素数目";
		else if(m_iServiceType == GetGoCBValues)
			return "读GOOSE控制块值";
		else if(m_iServiceType == SetGoCBValues)
			return "设置GOOSE控制块值";
		return SString::toFormat("未知服务:%d",m_iServiceType);
	};
};


class SDecodeAcsiInfo_GSSE : public SDecodeAcsiBaseInfo
{
public:
	enum eServiceType
	{
		SendGSSEMessage=0,//发送GSSE报文
		GetGsReference,//读Gs引用
		GetGSSEElementNumber,//读GSSE元素数目
		GetGsCBValues,//读GSSE控制块值
		SetGsCBValues,//设置GSSE控制块值
	};

	SDecodeAcsiInfo_GSSE(){m_iAcsiModelType=ACSI_MODEL_GSSE;};
	virtual ~SDecodeAcsiInfo_GSSE(){};
	virtual SString GetServiceName()
	{
		if(m_iServiceType == SendGSSEMessage)
			return "发送GSSE报文";
		else if(m_iServiceType == GetGsReference)
			return "读Gs引用";
		else if(m_iServiceType == GetGSSEElementNumber)
			return "读GSSE元素数目";
		else if(m_iServiceType == GetGsCBValues)
			return "读GSSE控制块值";
		else if(m_iServiceType == SetGsCBValues)
			return "设置GSSE控制块值";
		return SString::toFormat("未知服务:%d",m_iServiceType);
	};
};


class SDecodeAcsiInfo_MSV : public SDecodeAcsiBaseInfo
{
public:
	enum eServiceType
	{
		SendMSVMessage=0,//发送MSV报文
		GetMSVCBValues,//读MSV控制块值
		SetMSVCBValues,//设置MSV控制块值
	};

	SDecodeAcsiInfo_MSV(){m_iAcsiModelType=ACSI_MODEL_MSV;};
	virtual ~SDecodeAcsiInfo_MSV(){};
	virtual SString GetServiceName()
	{
		if(m_iServiceType == SendMSVMessage)
			return "发送MSV报文";
		else if(m_iServiceType == GetMSVCBValues)
			return "读MSV控制块值";
		else if(m_iServiceType == SetMSVCBValues)
			return "设置MSV控制块值";
		return SString::toFormat("未知服务:%d",m_iServiceType);
	};
};


class SDecodeAcsiInfo_USV : public SDecodeAcsiBaseInfo
{
public:
	enum eServiceType
	{
		SendUSVMessage=0,//发送USV报文
		GetUSVCBValues,//读USV控制块值
		SetUSVCBValues,//设置USV控制块值
	};

	SDecodeAcsiInfo_USV(){m_iAcsiModelType=ACSI_MODEL_USV;};
	virtual ~SDecodeAcsiInfo_USV(){};
	virtual SString GetServiceName()
	{
		if(m_iServiceType == SendUSVMessage)
			return "发送USV报文";
		else if(m_iServiceType == GetUSVCBValues)
			return "读USV控制块值";
		else if(m_iServiceType == SetUSVCBValues)
			return "设置USV控制块值";
		return SString::toFormat("未知服务:%d",m_iServiceType);
	};
};



class SDecodeAcsiInfo_Ctrl : public SDecodeAcsiBaseInfo
{
public:
	enum eServiceType
	{
		Select=0,//选择
		SelectWithValue,//带值选择
		Cancel,//取消
		Operate,//执行
		CommandTermination,//终止
		TimeActivatedOperate,//时间激活操作
	};

	SDecodeAcsiInfo_Ctrl(){m_iAcsiModelType=ACSI_MODEL_CTRL;};
	virtual ~SDecodeAcsiInfo_Ctrl(){};
	virtual SString GetServiceName()
	{
		if(m_iServiceType == Select)
			return "选择";
		else if(m_iServiceType == SelectWithValue)
			return "带值选择";
		else if(m_iServiceType == Cancel)
			return "取消";
		else if(m_iServiceType == Operate)
			return "执行";
		else if(m_iServiceType == CommandTermination)
			return "终止";
		else if(m_iServiceType == TimeActivatedOperate)
			return "时间激活操作";
		return SString::toFormat("未知服务:%d",m_iServiceType);
	};
};


class SDecodeAcsiInfo_Time : public SDecodeAcsiBaseInfo
{
public:
	enum eServiceType
	{
		TimeSynchronisation=0,//时间同步
	};

	SDecodeAcsiInfo_Time(){m_iAcsiModelType=ACSI_MODEL_TIME;};
	virtual ~SDecodeAcsiInfo_Time(){};
	virtual SString GetServiceName()
	{
		if(m_iServiceType == TimeSynchronisation)
			return "时间同步";
		return SString::toFormat("未知服务:%d",m_iServiceType);
	};
};


class SDecodeAcsiInfo_File : public SDecodeAcsiBaseInfo
{
public:
	enum eServiceType
	{
		GetFile=0,//读文件
		SetFile,//设置文件
		DeleteFile,//删除文件
		GetFileAttributeValues,//读文件属性值
	};

	SDecodeAcsiInfo_File(){m_iAcsiModelType=ACSI_MODEL_FILE;};
	virtual ~SDecodeAcsiInfo_File(){};
	virtual SString GetServiceName()
	{
		if(m_iServiceType == GetFile)
			return "读文件";
		else if(m_iServiceType == SetFile)
			return "设置文件";
		else if(m_iServiceType == DeleteFile)
			return "删除文件";
		else if(m_iServiceType == GetFileAttributeValues)
			return "读文件属性值";
		return SString::toFormat("未知服务:%d",m_iServiceType);
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

	eAcsiModel m_iAcsiModelType;//ACSI模型类型
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

	//MMS一次解码的中间值
// 	int m_iObjectClass;
 	int m_iInvokeID;
// 	char m_sIdentifierStr[512];
};
#endif//__SKT_DECODE_ACSI_DEF_H__

