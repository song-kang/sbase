/**
 *
 * 文 件 名 : SDecodeAcsi.cpp
 * 创建日期 : 2015-6-11 14:32
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

#include "SDecodeAcsi.h"

//////////////////////////////////////////////////////////////////////////
// 描    述:  根据当前模型类型，返回对应的模型名称
// 作    者:  邵凯田
// 创建时间:  2015-6-11 14:31
// 参数说明:  @type为类型
// 返 回 值:  SString
//////////////////////////////////////////////////////////////////////////
SString SDecodeAcsiBaseInfo::GetModelName(int type)
{
	switch(type)
	{
	case ACSI_MODEL_SERVER:			return "服务器模型";
	case ACSI_MODEL_ASSOCIATION:	return "关联模型";
	case ACSI_MODEL_LD:				return "逻辑设备模型";
	case ACSI_MODEL_LN:				return "逻辑节点模型";
	case ACSI_MODEL_DATA:			return "数据模型";
	case ACSI_MODEL_DATASET:		return "数据集模型";
	case ACSI_MODEL_SUBSTITUTION:	return "取代模型";
	case ACSI_MODEL_SGCB:			return "定值组控制块模型";
	case ACSI_MODEL_BRCB:			return "缓存报告控制块模型";
	case ACSI_MODEL_URCB:			return "非缓存报告控制块模型";
	case ACSI_MODEL_LCB:			return "日志控制块模型";
	case ACSI_MODEL_GSE:			return "面向通用对象的变电站事件";
	case ACSI_MODEL_GSSE:			return "通用变电站事件模型";
	case ACSI_MODEL_MSV:			return "多播采样值控制块模型";
	case ACSI_MODEL_USV:			return "单播采样值控制块模型";
	case ACSI_MODEL_CTRL:			return "控制模型";
	case ACSI_MODEL_TIME:			return "时间同步模型";
	case ACSI_MODEL_FILE:			return "文件传输模型";
	default:						return SString::toFormat("未知类型:%d",type);
	}
}

SString SDecodeAcsiInfo_All::GetACSITitle()
{
	SDecodeAcsiBaseInfo *pAcsi = GetAcsiInfo();
	if(pAcsi == NULL)
		return "";
	SString acsi = "ACSI ";
	acsi += SDecodeAcsiBaseInfo::GetModelName(m_iAcsiModelType);
	acsi += " - ";
	acsi += pAcsi->GetServiceName();
	switch(pAcsi->m_iMsgType)
	{
	case ACSI_REQUEST:
		acsi += "(请求)";
		break;
	case ACSI_RESPONSE_OK:
		acsi += "(肯定响应)";
		break;
	case ACSI_RESPONSE_ER:
		acsi += "(否定响应)";
		break;
	}
	return acsi;
}

SDecodeAcsiBaseInfo* SDecodeAcsiInfo_All::GetAcsiInfo()
{
	if(m_iAcsiModelType <= ACSI_MODEL_UNKNOWN || m_iAcsiModelType >= ACSI_MODEL_COUNT)
		return NULL;
	switch(m_iAcsiModelType)
	{
	case ACSI_MODEL_SERVER:			return &m_Model_Server;
	case ACSI_MODEL_ASSOCIATION:	return &m_Model_Association;		
	case ACSI_MODEL_LD:				return &m_Model_LD;
	case ACSI_MODEL_LN:				return &m_Model_LN;
	case ACSI_MODEL_DATA:			return &m_Model_Data;
	case ACSI_MODEL_DATASET:		return &m_Model_Dataset;
	case ACSI_MODEL_SUBSTITUTION:	return &m_Model_Substitution;
	case ACSI_MODEL_SGCB:			return &m_Model_SGCB;
	case ACSI_MODEL_BRCB:			return &m_Model_BRCB;	
	case ACSI_MODEL_URCB:			return &m_Model_URCB;
	case ACSI_MODEL_LCB:			return &m_Model_LCB;
	case ACSI_MODEL_GSE:			return &m_Model_GSE;
	case ACSI_MODEL_GSSE:			return &m_Model_GSSE;
	case ACSI_MODEL_MSV:			return &m_Model_MSV;
	case ACSI_MODEL_USV:			return &m_Model_USV;
	case ACSI_MODEL_CTRL:			return &m_Model_Ctrl;
	case ACSI_MODEL_TIME:			return &m_Model_Time;
	case ACSI_MODEL_FILE:			return &m_Model_File;
	}
	return NULL;
}

SDecodeAcsiBaseInfo* SDecodeAcsiInfo_All::SearchByInvokeID()
{
	if(m_Model_Server.m_iInvokeID == m_iInvokeID)return &m_Model_Server;
	else if(m_Model_Association.m_iInvokeID == m_iInvokeID)return &m_Model_Association;
	else if(m_Model_LD.m_iInvokeID == m_iInvokeID)return &m_Model_LD;
	else if(m_Model_LN.m_iInvokeID == m_iInvokeID)return &m_Model_LN;
	else if(m_Model_Data.m_iInvokeID == m_iInvokeID)return &m_Model_Data;
	else if(m_Model_Dataset.m_iInvokeID == m_iInvokeID)return &m_Model_Dataset;
	else if(m_Model_Substitution.m_iInvokeID == m_iInvokeID)return &m_Model_Substitution;
	else if(m_Model_SGCB.m_iInvokeID == m_iInvokeID)return &m_Model_SGCB;
	else if(m_Model_BRCB.m_iInvokeID == m_iInvokeID)return &m_Model_BRCB;	
	else if(m_Model_URCB.m_iInvokeID == m_iInvokeID)return &m_Model_URCB;
	else if(m_Model_LCB.m_iInvokeID == m_iInvokeID)return &m_Model_LCB;
	else if(m_Model_GSE.m_iInvokeID == m_iInvokeID)return &m_Model_GSE;
	else if(m_Model_GSSE.m_iInvokeID == m_iInvokeID)return &m_Model_GSSE;
	else if(m_Model_MSV.m_iInvokeID == m_iInvokeID)return &m_Model_MSV;
	else if(m_Model_USV.m_iInvokeID == m_iInvokeID)return &m_Model_USV;
	else if(m_Model_Ctrl.m_iInvokeID == m_iInvokeID)return &m_Model_Ctrl;
	else if(m_Model_Time.m_iInvokeID == m_iInvokeID)return &m_Model_Time;
	else if(m_Model_File.m_iInvokeID == m_iInvokeID)return &m_Model_File;
	else return NULL;
}

void SDecodeAcsiInfo_All::SetModelMsgType(eAcsiModel m,int svr)
{
	m_iAcsiModelType = m;
	m_pLastAcsiInfo = GetAcsiInfo();
	eAcsiServiceMsgType t;
	switch(m_iMmsServiceMsgType)
	{
	case MMS_RESPONSE:
		t = ACSI_RESPONSE_OK;
		break;
	case MMS_ERROR:
		t = ACSI_RESPONSE_ER;
		break;
	default:
		t = ACSI_REQUEST;
		break;
	}
	m_pLastAcsiInfo->SetService(svr,t);
	m_pLastAcsiInfo->m_iInvokeID = m_iInvokeID;
	m_pLastAcsiInfo->m_bRequest = (m_iMmsServiceMsgType == ACSI_REQUEST);
}

#include "SDecodeMms.h"
void SDecodeAcsiInfo_All::AnalyzeAcsi(void *p)
{
	if(m_iMmsService == MMS_Unknown)
		return;
	SDecodeMms *pMms = (SDecodeMms*)p;
	if(pMms->m_TempInvokeId.type == stuMmsTempVal::VAL_INT)
	{
		m_iInvokeID = pMms->m_TempInvokeId.value.v_int;
		if(m_iMmsServiceMsgType != MMS_REQUEST)
		{
			//不是请求报文，根据invokeID找对应的类型直接命中
			m_pLastAcsiInfo = SearchByInvokeID();
			if(m_pLastAcsiInfo != NULL)
			{
				m_iAcsiModelType = m_pLastAcsiInfo->m_iAcsiModelType;
				//eAcsiServiceMsgType t;
				switch(m_iMmsServiceMsgType)
				{
				case MMS_RESPONSE:
					m_pLastAcsiInfo->m_iMsgType = ACSI_RESPONSE_OK;
					break;
				case MMS_ERROR:
					m_pLastAcsiInfo->m_iMsgType = ACSI_RESPONSE_ER;
					break;
				default:
					m_pLastAcsiInfo->m_iMsgType = ACSI_REQUEST;
					break;
				}
				m_pLastAcsiInfo->m_bRequest = false;
			}
		}	
	}

	if(m_iAcsiModelType == ACSI_MODEL_UNKNOWN)
	{
		switch(m_iMmsService)
		{
			//unconfirmed
		case MMS_InformationReport:
		break;case MMS_UnsolicitedStatus:
		break;case MMS_EventNotification:

		break;case MMS_Initiate:
		SetModelMsgType(ACSI_MODEL_ASSOCIATION,SDecodeAcsiInfo_Association::Associate);

		break;case MMS_Conclude:
		SetModelMsgType(ACSI_MODEL_ASSOCIATION,SDecodeAcsiInfo_Association::Release);

		break;case MMS_Abort:
		SetModelMsgType(ACSI_MODEL_ASSOCIATION,SDecodeAcsiInfo_Association::Abort);

		break;case MMS_Reject:
		break;case MMS_Cancel:

			//confirmed
		break;case MMS_Status://
		break;case MMS_GetNameList://
		if(pMms->m_TempObjectClass.type != stuMmsTempVal::VAL_INT)
			break;
		if(pMms->m_TempObjectClass.value.v_int == 9)
			SetModelMsgType(ACSI_MODEL_SERVER,SDecodeAcsiInfo_Server::GetServerDirectory);
		else if(pMms->m_TempObjectClass.value.v_int == 0)
		{
			//objectClass: nammedVariable (0)
			//根据domainSpecific: E1Q1SB1C1确定是否为服务器模型（不含$)
			if(pMms->m_TempDomainSpecific.type == stuMmsTempVal::VAL_STR)
			{
				stuMmsTempVal::stuValStr *pDomainSpecific = &pMms->m_TempDomainSpecific.value.v_str;
				bool bHasDollar=false;
				char *pStr = (char*)pDomainSpecific->val;
				for(int i=0;i<pDomainSpecific->len;i++)
				{
					if(*pStr == '$')
					{
						bHasDollar = true;
						break;
					}
					pStr++;
				}
				if(bHasDollar == false)
				{
					//
					SetModelMsgType(ACSI_MODEL_LD,SDecodeAcsiInfo_LD::GetLogicalDeviceDirectory);
				}
			}
			else if(pMms->m_TempDomainId.type == stuMmsTempVal::VAL_STR && pMms->m_TempItemId.type == stuMmsTempVal::VAL_STR)
			{
				SetModelMsgType(ACSI_MODEL_DATA,SDecodeAcsiInfo_Data::GetDataValues);
			}
		}
		
		break;case MMS_Identify://
		break;case MMS_Rename://
		break;case MMS_Read://
		break;case MMS_Write://
		break;case MMS_GetVariableAccessAttributes:////"getVariableAccessAttributes" }://
		if(pMms->m_TempDomainId.type == stuMmsTempVal::VAL_STR && pMms->m_TempItemId.type == stuMmsTempVal::VAL_STR)
		{
			SetModelMsgType(ACSI_MODEL_DATA,SDecodeAcsiInfo_Data::GetDataDirectory);
		}
		break;case MMS_DefineNamedVariable:////"defineNamedVariable" }://
		break;case MMS_DefineScatteredAccess:////"defineScatteredAccess" }://
		break;case MMS_GetScatteredAccessAttributes:////"getScatteredAccessAttributes" }://
		break;case MMS_DeleteVariableAccess:////"deleteVariableAccess" }://
		break;case MMS_DefineNamedVariableList:////"defineNamedVariableList" }://
		break;case MMS_GetNamedVariableListAttributes:////"getNamedVariableListAttributes" }://
		break;case MMS_DeleteNamedVariableList:////"deleteNamedVariableList" }://
		break;case MMS_DefineNamedType:////"defineNamedType" }://
		break;case MMS_GetNamedTypeAttributes:////"getNamedTypeAttributes" }://
		break;case MMS_DeleteNamedType:////"deleteNamedType" }://
		break;case MMS_Input:////"input" }://
		break;case MMS_Output:////"output" }://
		break;case MMS_TakeControl:////"takeControl" }://
		break;case MMS_RelinquishControl:////"relinquishControl" }://
		break;case MMS_DefineSemaphore:////"defineSemaphore" }://
		break;case MMS_DeleteSemaphore:////"deleteSemaphore" }://
		break;case MMS_ReportSemaphoreStatus:////"reportSemaphoreStatus" }://
		break;case MMS_ReportPoolSemaphoreStatus:////"reportPoolSemaphoreStatus" }://
		break;case MMS_ReportSemaphoreEntryStatus:////"reportSemaphoreEntryStatus" }://
		break;case MMS_InitiateDownloadSequence:////"initiateDownloadSequence" }://
		break;case MMS_DownloadSegment:////"downloadSegment" }://
		break;case MMS_TerminateDownloadSequence:////"terminateDownloadSequence" }://
		break;case MMS_InitiateUploadSequence:////"initiateUploadSequence" }://
		break;case MMS_UploadSegment:////"uploadSegment" }://
		break;case MMS_TerminateUploadSequence:////"terminateUploadSequence" }://
		break;case MMS_RequestDomainDownload:////"requestDomainDownload" }://
		break;case MMS_RequestDomainUpload:////"requestDomainUpload" }://
		break;case MMS_LoadDomainContent:////"loadDomainContent" }://
		break;case MMS_StoreDomainContent:////"storeDomainContent" }://
		break;case MMS_DeleteDomain:////"deleteDomain" }://
		break;case MMS_GetDomainAttributes:////"getDomainAttributes" }://
		break;case MMS_CreateProgramInvocation:////"createProgramInvocation" }://
		break;case MMS_DeleteProgramInvocation:////"deleteProgramInvocation" }://
		break;case MMS_Start:////"start" }://
		break;case MMS_Stop:////"stop" }://
		break;case MMS_Resume:////"resume" }://
		break;case MMS_Reset:////"reset" }://
		break;case MMS_Kill:////"kill" }://
		break;case MMS_GetProgramInvocationAttributes:////"getProgramInvocationAttributes" }://
		break;case MMS_ObtainFile:////"obtainFile" }://
		break;case MMS_DefineEventCondition:////"defineEventCondition" }://
		break;case MMS_DeleteEventCondition:////"deleteEventCondition" }://
		break;case MMS_GetEventConditionAttributes:////"getEventConditionAttributes" }://
		break;case MMS_ReportEventConditionStatus:////"reportEventConditionStatus" }://
		break;case MMS_AlterEventConditionMonitoring:////"alterEventConditionMonitoring" }://
		break;case MMS_TriggerEvent:////"triggerEvent" }://
		break;case MMS_DefineEventAction:////"defineEventAction" }://
		break;case MMS_DeleteEventAction:////"deleteEventAction" }://
		break;case MMS_GetEventActionAttributes:////"getEventActionAttributes" }://
		break;case MMS_ReportEventActionStatus:////"reportEventActionStatus" }://
		break;case MMS_DefineEventEnrollment:////"defineEventEnrollment" }://
		break;case MMS_DeleteEventEnrollment:////"deleteEventEnrollment" }://
		break;case MMS_AlterEventEnrollment:////"alterEventEnrollment" }://
		break;case MMS_ReportEventEnrollmentStatus:////"reportEventEnrollmentStatus" }://
		break;case MMS_GetEventEnrollmentAttributes:////"getEventEnrollmentAttributes" }://
		break;case MMS_AcknowledgeEventNotification:////"acknowledgeEventNotification" }://
		break;case MMS_GetAlarmSummary:////"getAlarmSummary" }://
		break;case MMS_GetAlarmEnrollmentSummary:////"getAlarmEnrollmentSummary" }://
		break;case MMS_ReadJournal:////"readJournal" }://
		break;case MMS_WriteJournal:////"writeJournal" }://
		break;case MMS_InitializeJournal:////"initializeJournal" }://
		break;case MMS_ReportJournalStatus:////"reportJournalStatus" }://
		break;case MMS_CreateJournal:////"createJournal" }://
		break;case MMS_DeleteJournal:////"deleteJournal" }://
		break;case MMS_GetCapabilityList:////"getCapabilityList" }://
		break;case MMS_FileOpen:////"fileOpen" }://
		SetModelMsgType(ACSI_MODEL_FILE,SDecodeAcsiInfo_File::GetFile);
		break;case MMS_FileRead:////"fileRead" }://
		SetModelMsgType(ACSI_MODEL_FILE,SDecodeAcsiInfo_File::GetFile);
		break;case MMS_FileClose:////"fileClose" }://
		SetModelMsgType(ACSI_MODEL_FILE,SDecodeAcsiInfo_File::GetFile);
		break;case MMS_FileRename:////"fileRename" }://
		SetModelMsgType(ACSI_MODEL_FILE,SDecodeAcsiInfo_File::SetFile);
		break;case MMS_FileDelete:////"fileDelete" }://
		SetModelMsgType(ACSI_MODEL_FILE,SDecodeAcsiInfo_File::DeleteFile);
		break;case MMS_FileDirectory:////"fileDirectory" }://
		SetModelMsgType(ACSI_MODEL_FILE,SDecodeAcsiInfo_File::GetFileAttributeValues);
		break;
		}
	}

}

void SDecodeAcsiInfo_All::FillAcsiDecodeTree(SDecodeTree *pAcsiRoot)
{
	SDecodeAcsiBaseInfo *pAcsi = GetAcsiInfo();
	if(pAcsi == NULL)
		return;
	pAcsi->FillAcsiDecodeTree(pAcsiRoot);
}



/*
Tab.1 MMS mapping of ACSI services 



模型

ACSI

服务

MMS

服务



服务器



GetServerDirectory GetNameList 

LD GetLogicalDeviceDirectory 

GetNameList 

LN GetLogicalNodeDirectory 

GetAllDataValues 

GetNameList 

Read 

Data GetDataValues 

SetDataValues 

GetDataDirectory 

GetDataDefinition 

Read 

Write 

GetVariableAccessAttributes

GetVariableAccessAttributes

DataSet GetDataSetValues 

SetDataSetValues 

CreateDataSet 

DeleteDataSet 

GetDataSetDirectory 

Read 

Write 

DefinedNamedVariableList 

DefinedNamedVariableList 

GetNamedVariableListAttrib

utes 

BRCB Report 

GetBRCBValues 

SetBRCBValues 

Peport 

Read 

Write 

LCB GetLCBValues 

SetLCBValues 

QueryLogByTie 

QueryLogByEntry 

GetLogStatusValues 

Read 

Write 

ReadJournal 

ReadJournal 

Read
*/