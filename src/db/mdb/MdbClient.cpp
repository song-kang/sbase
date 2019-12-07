/**
 *
 * �� �� �� : MdbClient.cpp
 * �������� : 2014-4-22 13:11
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : �ڴ����ݿ�ķ��ʿͻ���
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2014-4-22	�ۿ�������ļ�
 *
 **/

#include "MdbClient.h"
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////////
// ��    ��:  CMdbClient
// ��    ��:  �ۿ���
// ����ʱ��:  2014:4:24 22:01
// ��    ��:  �Զ����ڴ滷�����ݿ�Ĳ�����
//////////////////////////////////////////////////////////////////////////
CMdbClient::CMdbClient()
{
	SetStartWord(0x518C);
	memset(m_sLastError,0,sizeof(m_sLastError));
	//m_pRecvPackage = NULL;
	m_TrgReg = new SPtrList<stuMdbClientTriggerReg>();
	m_TrgReg->setShared(true);
	m_TrgReg->setAutoDelete(false);
}

CMdbClient::~CMdbClient()
{
	//if(m_pRecvPackage)
	//	delete m_pRecvPackage;
	m_TrgReg->setAutoDelete(true);
	m_TrgReg->clear();
	delete m_TrgReg;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  �������ݿ������
// ��    ��:  �ۿ���
// ����ʱ��:  2014:4:24 22:41
// ����˵��:  @ipΪ���ݿ�IP��ַ
//         :  @portΪ���ݿ�TCP����˿�
//         :  @dbuserΪ�û���
//         :  @dbpwdΪ����
// �� �� ֵ:  true��ʾ��¼�ɹ���false��ʾ��¼ʧ��
//////////////////////////////////////////////////////////////////////////
bool CMdbClient::Connect(SString ip,int port,SString dbuser,SString dbpwd)
{
	m_sIp = ip;
	m_iPort = port;
	SetLoginInfo(SString::toFormat("dbuser=%s;dbpwd=%s;",dbuser.data(),dbpwd.data()));
	STcpClientBase::SetConnectMode(true);
	//�����ӷ�ʽ��Ϊ�߳�����
	if(!Start(ip, port))
	//if(!STcpClientBase::Connect(ip,port))
	{
		sprintf(m_sLastError,"���ӵ�MDB(%s:%d)ʧ��!",ip.data(),port);
		LOGWARN("%s",m_sLastError);
		return false;
	}
	OnLogin();
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ��¼���ڴ����ݿ�����ʱ�Ļص���������Ҫ���ڶϿ�������ִ������ע�ᴥ���ص�
// ��    ��:  �ۿ���
// ����ʱ��:  2015-8-7 16:35
// ����˵��:  void
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void CMdbClient::OnLogin()
{
	//�����ע���������½���ע��
	unsigned long pos;
	SString sHead;
	m_TrgReg->lock();
	stuMdbClientTriggerReg *pReg = m_TrgReg->FetchFirst(pos);
	while(pReg)
	{
		sHead.sprintf("table=%s;action=%d;", pReg->sTableName.data(),pReg->iTriggerFlag);
		if (!SendFrame(sHead, MDB_FT_CONNTRG))
		{
			LOGERROR("�������Ӵ���������ʱʧ��!");
		}
		pReg = m_TrgReg->FetchNext(pos);
	}
	m_TrgReg->unlock();
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  �Ͽ����ݿ�����
// ��    ��:  �ۿ���
// ����ʱ��:  2014:4:24 22:42
// ����˵��:  void
// �� �� ֵ:  true�Ͽ��ɹ���falseʧ��
//////////////////////////////////////////////////////////////////////////
bool CMdbClient::DisConnect()
{
	if (GetSocket() != NULL && GetSocket()->GetSocketHandle() != 0)
		GetSocket()->Close();
	STcpClientBase::StopAndWait();
	return true;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  �����µ�һ����¼
// ��    ��:  �ۿ���
// ����ʱ��:  2014:4:23 12:52
// ����˵��:  @sTableNameΪ����
//         :  @pRowData��ʾ��¼���ݣ����뽨��ʱ����һ�µ����м�¼
//         :  @iRowSize��ʾһ����¼�ĳ���
//         :  @iRows��ʾ��ǰһ���ж�������¼�����룬������¼�����洢��pRowData��
// �� �� ֵ:  >0��ʾ�ɹ�����ļ�¼����<0��ʾʧ�ܷ�����
//////////////////////////////////////////////////////////////////////////
int CMdbClient::Insert(const char* sTableName,void *pRowData,int iRowSize,int iRows)
{
	SString sHead;
	sHead.sprintf("table=%s;rowsize=%d;rows=%d;",sTableName,iRowSize,iRows);
	if (!IsConnected())
		return MDB_RET_DISCONNECT;
	stuSTcpPackage *pRecvPackage = NULL;
	if (!SendAndSearchPkg(pRecvPackage, sHead, MDB_FT_INSERT, (BYTE*)pRowData, iRowSize*iRows))
	{
		sprintf(m_sLastError,"����ʧ��!");
		LOGWARN("%s",m_sLastError);
		if (pRecvPackage != NULL)
			delete pRecvPackage;
		return MDB_RET_DISCONNECT;
	}
	if (pRecvPackage->m_wFrameType != MDB_FT_INSERT || pRecvPackage->m_iAsduLen != 4)
	{
		delete pRecvPackage;
		return MDB_RET_UNKNOWN;
	}
	SKT_SWAP_DWORD(*((int*)m_pRecvPackage->m_pAsduBuffer));
	int ret = *((int*)pRecvPackage->m_pAsduBuffer);
	if(ret < MDB_RET_SUCCESS)
	{
		sprintf(m_sLastError, "%s", pRecvPackage->m_sHead.data());
	}
	delete pRecvPackage;
	return ret;
}


////////////////////////////////////////////////////////////////////////
// ��    ��:  ���»����һ����¼��������ΪΨһ��ʶ�������¼���ڸ��£������������
// ��    ��:  �ۿ���
// ����ʱ��:  2014-12-17 12:52
// ����˵��:  @sTableNameΪ����
//         :  @pRowData��ʾ��¼���ݣ����뽨��ʱ����һ�µ����м�¼
//         :  @iRowSize��ʾһ����¼�ĳ���
//         :  @iRows��ʾ��ǰһ���ж�������¼�����룬������¼�����洢��pRowData��
// �� �� ֵ:  >0��ʾ�ɹ�����ļ�¼����<0��ʾʧ�ܷ�����
//////////////////////////////////////////////////////////////////////////
int CMdbClient::UpdateWithInsert(const char* sTableName, void *pRowData, int iRowSize, int iRows)
{
	SString sHead;
	sHead.sprintf("table=%s;rowsize=%d;rows=%d;", sTableName, iRowSize, iRows);
	if (!IsConnected())
		return MDB_RET_DISCONNECT;
	stuSTcpPackage *pRecvPackage = NULL;
	if (!SendAndSearchPkg(pRecvPackage, sHead, MDB_FT_UPDATE_INSERT, (BYTE*)pRowData, iRowSize*iRows))
	{
		sprintf(m_sLastError, "����/����ʧ��!");
		LOGWARN("%s", m_sLastError);
		if (pRecvPackage != NULL)
			delete pRecvPackage;
		if (!IsConnected())
			return MDB_RET_DISCONNECT;
		return MDB_RET_UNKNOWN;
	}
	if (pRecvPackage->m_wFrameType != MDB_FT_UPDATE_INSERT || pRecvPackage->m_iAsduLen != 4)
	{
		delete pRecvPackage;
		return MDB_RET_UNKNOWN;
	}
	SKT_SWAP_DWORD(*((int*)pRecvPackage->m_pAsduBuffer));
	int ret = *((int*)pRecvPackage->m_pAsduBuffer);
	if (ret < MDB_RET_SUCCESS)
	{
		sprintf(m_sLastError, "%s", pRecvPackage->m_sHead.data());
	}
	delete pRecvPackage;
	return ret;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ִ��SQL��䣬�ɴ�����ɾ�������������������¼�����¼�¼��ɾ����¼��
// ��    ��:  �ۿ���
// ����ʱ��:  2014:4:23 12:05
// ����˵��:  @sqlΪ��ִ�е�SQL��䣬֧�ּ��﷨
// �� �� ֵ:  >=0��ʾ�ɹ��Լ�Ӱ���������<0��ʾʧ�ܷ�����
//////////////////////////////////////////////////////////////////////////
int CMdbClient::Execute(SString &sql)
{
	if (!IsConnected())
		return MDB_RET_DISCONNECT;
	stuSTcpPackage* pRecvPackage = NULL;
	SString sHead;
	//if (!SendAndSearchPkg(pRecvPackage, sql, MDB_FT_EXECUTE))
	if (!SendAndSearchPkg(pRecvPackage, sHead, MDB_FT_EXECUTE,(BYTE*)sql.data(),sql.length()+1,60))
	{
		sprintf(m_sLastError,"ִ��SQLʧ��!");
		LOGWARN("%s",m_sLastError);
		if (pRecvPackage != NULL)
			delete pRecvPackage;
		return MDB_RET_DISCONNECT;
	}
	if (pRecvPackage->m_wFrameType != MDB_FT_EXECUTE || pRecvPackage->m_iAsduLen != 4)
	{
		delete pRecvPackage;
		return MDB_RET_UNKNOWN;
	}
	SKT_SWAP_DWORD(*((int*)pRecvPackage->m_pAsduBuffer));
	int ret = *((int*)pRecvPackage->m_pAsduBuffer);
	if(ret < MDB_RET_SUCCESS)
	{
		sprintf(m_sLastError, "%s", pRecvPackage->m_sHead.data());
	}
	delete pRecvPackage;
	return ret;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ѯָ��SQL�������ݼ�
// ��    ��:  �ۿ���
// ����ʱ��:  2014:4:23 12:07
// ����˵��:  @sqlΪ��ִ�е�SQL��䣬֧�ּ��﷨
//         :  @iRowSize��ʾһ�м�¼���ܳ���(���÷��أ�
//         :  @pRowDataΪ��¼�����飬�ܳ���Ϊ��¼��*iRowSize������ָ�벻ΪNULLʱ��ʹ����Ӧ�����ͷ�����
// �� �� ֵ:  >=0��ʾ�ɹ����صļ�¼����<0��ʾʧ�ܷ�����
//			  ��select count(*),min(f1)... ʱ����1����pRowDataΪNULL��SQL��Ϊ��\t�ָ��Ľ���ַ���
//////////////////////////////////////////////////////////////////////////
int CMdbClient::Retrieve(SString &sql,int &iRowSize,void* &pRowData)
{
	if (!IsConnected())
		return MDB_RET_DISCONNECT;
	stuSTcpPackage *pRecvPackage = NULL;
	if (!SendAndSearchPkg(pRecvPackage, sql, MDB_FT_SELECT,NULL,0,30))
	{
		sprintf(m_sLastError,"ִ��SQLʧ��!");
		LOGWARN("%s",m_sLastError);
		if (pRecvPackage != NULL)
			delete pRecvPackage;
		return MDB_RET_DISCONNECT;
	}
	if(pRecvPackage == NULL)
	{
		return MDB_RET_UNKNOWN;
	}
	if (pRecvPackage->m_wFrameType != MDB_FT_SELECT || pRecvPackage->m_iAsduLen < 8)
	{
		delete pRecvPackage;
		return MDB_RET_UNKNOWN;
	}
	//ret rows
	int ret = *((int*)pRecvPackage->m_pAsduBuffer);
	SKT_SWAP_DWORD(ret);
	//row size
	iRowSize = *((int*)(pRecvPackage->m_pAsduBuffer + 4));
	SKT_SWAP_DWORD(iRowSize);
	if(ret < 0)
	{
		sprintf(m_sLastError, "%s", pRecvPackage->m_sHead.data());
	}
	else if(ret == 0)
	{
		delete pRecvPackage;
		return 0;
	}
	else if (ret == 1 && pRecvPackage->m_iAsduLen == 8)
	{
		//����ͳ�ƽ��
		sql = pRecvPackage->m_sHead;
		pRowData = NULL;
		delete pRecvPackage;
		return 1;
	}
	if (iRowSize < 0)
	{
		delete pRecvPackage;
		return MDB_RET_UNKNOWN;
	}
	int bufsize = iRowSize*ret;
	if (pRecvPackage->m_iAsduLen - 8 != bufsize)
	{
		if (pRecvPackage->m_iAsduLen - 8 == 0)
		{
			delete pRecvPackage;
			return ret;
		}
		delete pRecvPackage;
		return MDB_RET_UNKNOWN;
	}
	if (iRowSize > C_MDB_MAX_RETRIEVE_ROWS)
	{
		delete pRecvPackage;
		return MDB_RET_RETRIEVE_OVERFLOW;
	}
	pRowData = new BYTE[bufsize];
	memcpy(pRowData, pRecvPackage->m_pAsduBuffer + 8, bufsize);
	delete pRecvPackage;
	return ret;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ���ļ��������ݿ���ָ����¼��ָ�����ֶ�
// ��    ��:  �ۿ���
// ����ʱ��:  2014-12-22 15:16
// ����˵��:  @sTable����
//			  @sLobFieldΪ�ֶ���
//			  @sWhere������Ψһȷ��һ����¼)
//			  @sFileΪ�����ļ���
// �� �� ֵ:  >0��ʾ�ɹ���=0��ʾδ�ҵ���Ӧ��¼��<0��ʾʧ��
//////////////////////////////////////////////////////////////////////////
int CMdbClient::UpdateBlobFromFile(SString sTable, SString sLobField, SString sWhere, SString sFile)
{
	if (!IsConnected())
		return MDB_RET_DISCONNECT;
	SString sHead;
	sHead.sprintf("table=%s;field=%s;where=%s;", sTable.data(), sLobField.data(), sWhere.data());
	SFile f(sFile);
	BYTE *pFileBuf = NULL;
	int size = f.size();
	if (size > 0 && f.open(IO_ReadOnly))
	{
		pFileBuf = new BYTE[size];
		if (f.readBlock(pFileBuf, size) != size)
		{
			LOGERROR("��ȡ�ļ�(%s)ʱʧ��!",sFile.data());
			delete[] pFileBuf;
			return MDB_RET_UNKNOWN;
		}
	}
	else
		size = 0;
	stuSTcpPackage *pRecvPackage = NULL;
	if (!SendAndSearchPkg(pRecvPackage, sHead, MDB_FT_UPDATE_BLOB, pFileBuf, size))
	{
		f.close();
		if (pFileBuf != NULL)
			delete[] pFileBuf;
		sprintf(m_sLastError, "���´��ֶ�ʱʧ��!");
		LOGWARN("%s", m_sLastError);
		if (pRecvPackage != NULL)
			delete pRecvPackage;
		return MDB_RET_DISCONNECT;
	}
	f.close();
	if (pFileBuf != NULL)
		delete[] pFileBuf;
	if (pRecvPackage->m_wFrameType != MDB_FT_UPDATE_BLOB || pRecvPackage->m_iAsduLen != 4)
	{
		delete pRecvPackage;
		return MDB_RET_UNKNOWN;
	}
	//ret rows
	int ret = *((int*)pRecvPackage->m_pAsduBuffer);
	SKT_SWAP_DWORD(ret);
	if (ret < 0)
	{
		sprintf(m_sLastError, "%s", pRecvPackage->m_sHead.data());
	}
	delete pRecvPackage;
	return ret;
}

int CMdbClient::UpdateBlobFromMem(SString sTable, SString sLobField, SString sWhere, BYTE *pBuf,int iBufLen)
{
	if (!IsConnected())
		return MDB_RET_DISCONNECT;
	SString sHead;
	sHead.sprintf("table=%s;field=%s;where=%s;", sTable.data(), sLobField.data(), sWhere.data());
	BYTE *pFileBuf = pBuf;
	int size = iBufLen;
	stuSTcpPackage *pRecvPackage = NULL;
	if (!SendAndSearchPkg(pRecvPackage, sHead, MDB_FT_UPDATE_BLOB, pFileBuf, size))
	{
		sprintf(m_sLastError, "���´��ֶ�ʱʧ��!");
		LOGWARN("%s", m_sLastError);
		if (pRecvPackage != NULL)
			delete pRecvPackage;
		return MDB_RET_DISCONNECT;
	}
	if (pRecvPackage->m_wFrameType != MDB_FT_UPDATE_BLOB || pRecvPackage->m_iAsduLen != 4)
	{
		delete pRecvPackage;
		return MDB_RET_UNKNOWN;
	}
	//ret rows
	int ret = *((int*)pRecvPackage->m_pAsduBuffer);
	SKT_SWAP_DWORD(ret);
	if (ret < 0)
	{
		sprintf(m_sLastError, "%s", pRecvPackage->m_sHead.data());
	}
	delete pRecvPackage;
	return ret;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  �����ݿ��ж�ȡָ����¼��ָ�����ֶε������ļ�
// ��    ��:  �ۿ���
// ����ʱ��:  2014-12-22 15:26
// ����˵��:  @sTable����
//			  @sLobFieldΪ�ֶ���
//			  @sWhere������Ψһȷ��һ����¼)
//			  @sFileΪ�����ļ���
// �� �� ֵ:  >0��ʾ�ɹ���=0��ʾδ�ҵ���Ӧ��¼��<0��ʾʧ��
//////////////////////////////////////////////////////////////////////////
int CMdbClient::ReadBlobToFile(SString sTable, SString sLobField, SString sWhere, SString sFile)
{
	if (!IsConnected())
		return MDB_RET_DISCONNECT;
	SString sHead;
	stuSTcpPackage *pRecvPackage = NULL;
	sHead.sprintf("table=%s;field=%s;where=%s;", sTable.data(), sLobField.data(), sWhere.data());
	if (!SendAndSearchPkg(pRecvPackage, sHead, MDB_FT_READ_BLOB))
	{

		sprintf(m_sLastError, "��ȡ���ֶ�ʱʧ��!");
		LOGWARN("%s", m_sLastError);
		if (pRecvPackage != NULL)
			delete pRecvPackage;
		return MDB_RET_DISCONNECT;
	}
	
	if (pRecvPackage->m_wFrameType != MDB_FT_READ_BLOB)
	{
		delete pRecvPackage;
		return MDB_RET_UNKNOWN;
	}
	SFile f(sFile);
	int ret = SString::GetAttributeValueI(pRecvPackage->m_sHead, "ret");
	int size = SString::GetAttributeValueI(pRecvPackage->m_sHead, "size");
	if (size != pRecvPackage->m_iAsduLen || ret < 0)
	{
		delete pRecvPackage;
		return MDB_RET_UNKNOWN;
	}
	if (!f.open(IO_Truncate))
	{
		LOGERROR("�����ļ�(%s)ʱʧ��!", sFile.data());
		delete pRecvPackage;
		return MDB_RET_UNKNOWN;
	}
	if (size > 0)
	{
		if (f.writeBlock(pRecvPackage->m_pAsduBuffer, size) != size)
		{
			LOGERROR("д�ļ�(%s)ʱʧ��!", sFile.data());
			delete pRecvPackage;
			return MDB_RET_UNKNOWN;
		}
	}
	f.close();
	delete pRecvPackage;
	return ret;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  �����ݿ��ж�ȡָ����¼��ָ�����ֶε����������ɵ����߸����ͷ�
// ��    ��:  �ۿ���
// ����ʱ��:  2014-12-22 15:26
// ����˵��:  @sTable����
//			  @sLobFieldΪ�ֶ���
//			  @sWhere������Ψһȷ��һ����¼)
//			  @pBlobBuf��ʾ���ֶ����ݻ�����
//		      @iBlobLen��ʾ���ֶ��ֽ���
// �� �� ֵ:  >0��ʾ�ɹ���=0��ʾδ�ҵ���Ӧ��¼��<0��ʾʧ��
//////////////////////////////////////////////////////////////////////////
int CMdbClient::ReadBlobToMem(SString sTable, SString sLobField, SString sWhere, BYTE* &pBlobBuf, int &iBlobLen)
{
	if (!IsConnected())
		return MDB_RET_DISCONNECT;
	SString sHead;
	stuSTcpPackage *pRecvPackage = NULL;
	sHead.sprintf("table=%s;field=%s;where=%s;", sTable.data(), sLobField.data(), sWhere.data());
	if (!SendAndSearchPkg(pRecvPackage, sHead, MDB_FT_READ_BLOB))
	{

		sprintf(m_sLastError, "��ȡ���ֶ�ʱʧ��!");
		LOGWARN("%s", m_sLastError);
		if (pRecvPackage != NULL)
			delete pRecvPackage;
		return MDB_RET_DISCONNECT;
	}

	if (pRecvPackage->m_wFrameType != MDB_FT_READ_BLOB)
	{
		delete pRecvPackage;
		return MDB_RET_UNKNOWN;
	}
	int ret = SString::GetAttributeValueI(pRecvPackage->m_sHead, "ret");
	int size = SString::GetAttributeValueI(pRecvPackage->m_sHead, "size");
	if (size != pRecvPackage->m_iAsduLen || ret < 0)
	{
		delete pRecvPackage;
		return MDB_RET_UNKNOWN;
	}
	iBlobLen = size;
	if (size == 0)
	{
		pBlobBuf = NULL;
	}
	else
	{
		pBlobBuf = new BYTE[size+1];
		memcpy(pBlobBuf, pRecvPackage->m_pAsduBuffer, size);
		pBlobBuf[size] = 0;
	}
	delete pRecvPackage;
	return ret;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  �Ự�߳̽��յ����ĺ�Ļص��麯����������ͨ���˺�������ʱ��Ϣ
// ��    ��:  �ۿ���
// ����ʱ��:  2014:4:27 21:45
// ����˵��:  @pSessionΪ�Ựʵ��ָ�룬����ͨ��ָ����Զ˷�������
//            @pPackageΪ�ս��յ������ݰ�
// �� �� ֵ:  true��ʾ�Ѿ�������ϣ��˱��Ŀ����ͷ��ˣ�false��ʾδ�����˱���Ӧ������ն���
//////////////////////////////////////////////////////////////////////////
bool CMdbClient::OnRecvFrame(stuSTcpPackage *pPackage)
{
	if (pPackage->m_wFrameType == MDB_FT_TRIGGER)
	{
		if (m_TrgReg->count() > 0)
		{
			SString sTable;
			eMdbTriggerType eType;
			int iTrgRows, iRowSize;
			unsigned long pos;
			//sHead.sprintf("table=%s;rowsize=%d;trgrows=%d;act=1;", pTable->GetTableName().data(), pTable->GetRowSize(), pTrg->m_iTrgRows);
			sTable = SString::GetAttributeValue(pPackage->m_sHead, "table");
			eType = (eMdbTriggerType)SString::GetAttributeValueI(pPackage->m_sHead, "act");
			iTrgRows = SString::GetAttributeValueI(pPackage->m_sHead, "trgrows");
			iRowSize = SString::GetAttributeValueI(pPackage->m_sHead, "rowsize");
			if (eType != MDB_TRG_TRUNCATE && iTrgRows * iRowSize != pPackage->m_iAsduLen)
			{
				return true;
			}
			m_TrgReg->lock();
			stuMdbClientTriggerReg *p = m_TrgReg->FetchFirst(pos);
			while (p)
			{
				//run callback for each
				if(p->sTableName == sTable && (p->iTriggerFlag & 1<<(eType-1)) != 0)
					p->pFun(p->cbParam, sTable, eType, iTrgRows, iRowSize, pPackage->m_pAsduBuffer);
				p = m_TrgReg->FetchNext(pos);
			}
			m_TrgReg->unlock();
		}
		return true;
	}
	else
	{
		//LOGERROR("��Ч��ͻ������:%d",pPackage->m_wFrameType);
	}
	return false;//δ����,��������ն��У��ȴ�Ӧ�ò����
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ע�ᴥ���ص���������ע�����ص�����
// ��    ��:  �ۿ���
// ����ʱ��:  2014-10-18 15:46
// ����˵��:  @pFunΪ�ص�������ַ
//			  @cbParamΪ�ص�����
//            @sTableNameΪ�����ı���
//            @iTriggerFlagΪ�������//bit0:insert; 1:update; 2:delete; 3:truncate,  eMdbTriggerType-1
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void CMdbClient::RegisterTriggerCallback(OnMdbClientTrgCallback pFun, void *cbParam, SString sTableName, BYTE iTriggerFlag)
{
	bool bExist = false;
	unsigned long pos;
	m_TrgReg->lock();
	stuMdbClientTriggerReg *p = m_TrgReg->FetchFirst(pos);
	while (p)
	{
		if (p->pFun == pFun && p->cbParam == cbParam && p->sTableName == sTableName)
		{
			if ((p->iTriggerFlag | iTriggerFlag) == iTriggerFlag)
			{
				bExist = true;
				break;
			}
		}
		p = m_TrgReg->FetchNext(pos);
	}
	m_TrgReg->unlock();
	if(bExist)
	{
		LOGWARN("��[%s]�Ĵ���[0x%X]��ע�ᣬ�ظ���ע��ָ�ȡ��!",sTableName.data(),iTriggerFlag);
		return ;
	}

	SString sHead;
	sHead.sprintf("table=%s;action=%d;", sTableName.data(), iTriggerFlag);
	if (!SendFrame(sHead, MDB_FT_CONNTRG))
	{
		LOGERROR("�������Ӵ���������ʱʧ��!");
		return;
	}
	//ע��ɹ�
	p = new stuMdbClientTriggerReg();
	p->pFun = pFun;
	p->cbParam = cbParam;
	p->sTableName = sTableName;
	p->iTriggerFlag = iTriggerFlag;
	m_TrgReg->append(p);
}


//////////////////////////////////////////////////////////////////////////
// ��    ��:  ����ǰʵ���µĴ������ƶ�����һ��ʵ���У���������ʵ����ȡ�����ش���
// ��    ��:  �ۿ���
// ����ʱ��:  2019-5-3 9:30
// ����˵��:  @pAnotherMdb
// �� �� ֵ:  ��ʾ�ƶ��Ĵ���������
//////////////////////////////////////////////////////////////////////////
int CMdbClient::MoveTriggerToAnother(CMdbClient *pAnotherMdb)
{
	unsigned long pos;
	int cnt = 0;
	m_TrgReg->lock();
	stuMdbClientTriggerReg *p = m_TrgReg->FetchFirst(pos);
	while (p)
	{
		cnt++;
		pAnotherMdb->RegisterTriggerCallback(p->pFun,p->cbParam,p->sTableName,p->iTriggerFlag);
		p = m_TrgReg->FetchNext(pos);
	}
	m_TrgReg->clear();
	m_TrgReg->unlock();
	return cnt;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ɾ�������ص�����
// ��    ��:  �ۿ���
// ����ʱ��:  2014-10-18 16:03
// ����˵��:  @pFunΪ�ص�������ַ
//			  @cbParamΪ�ص�����
//            @sTableNameΪ�����ı���
//            @iTriggerFlagΪ�������//bit0:insert; 1:update; 2:delete; 3:truncate,  eMdbTriggerType-1
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void CMdbClient::RemoveTriggerCallback(OnMdbClientTrgCallback pFun, void *cbParam, SString sTableName, BYTE iTriggerFlag)
{
	unsigned long pos=0;
	SPtrList<stuMdbClientTriggerReg> trg;
	trg.setAutoDelete(true);
	m_TrgReg->lock();
	stuMdbClientTriggerReg *p = m_TrgReg->FetchFirst(pos);
	while (p)
	{
		if (p->pFun == pFun && p->cbParam == cbParam && p->sTableName == sTableName)
		{
			if (p->iTriggerFlag == iTriggerFlag)
			{
				m_TrgReg->remove(p,false);
				trg.append(p);
			}
			else
			{
				p->iTriggerFlag ^= iTriggerFlag;
			}
			break;
		}
		p = m_TrgReg->FetchNext(pos);
	}
	m_TrgReg->unlock();
	SString sHead;
	sHead.sprintf("table=%s;action=%d;", sTableName.data(), iTriggerFlag);
	if (!SendFrame(sHead, MDB_FT_DISCONNTRG))
	{
		LOGERROR("�������Ӵ���������ʱʧ��!");
		return;
	}
	if(trg.count() > 0)
	{
		SApi::UsSleep(10000);
		trg.clear();
	}
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ɾ�����д����ص�
// ��    ��:  �ۿ���
// ����ʱ��:  2016-5-11 16:46
// ����˵��:  void
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void CMdbClient::RemoveAllTrigger()
{
	m_TrgReg->setAutoDelete(true);
	m_TrgReg->clear();
	m_TrgReg->setAutoDelete(false);
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ���������MDB������ȡ��Ӧ�ķ�������
// ��    ��:  �ۿ���
// ����ʱ��:  2014-5-19 13:42
// ����˵��:  @retΪ����������
// �� �� ֵ:  ��Ӧ�����ַ��� 
//////////////////////////////////////////////////////////////////////////
SString CMdbClient::GetRetCodeText(int ret)
{
	switch(ret)
	{
	case MDB_RET_SUCCESS:
		return "�ɹ�";
	case MDB_RET_TABLE_NOTEXIST:
		return "������";
	case MDB_RET_SYNTAX_ERROR:
		return "�﷨����";
	case MDB_RET_SQL_NOTSUPPORT:
		return "SQL��֧��";
	case MDB_RET_OBJ_EXIST:
		return "�����Ѿ�����";
	case MDB_RET_SYSTAB_ERR:
		return "ϵͳ�����";
	case MDB_RET_DISCONNECT:
		return "���ӶϿ�";
	case MDB_RET_INDEX_EXIST:
		return "�����Ѿ�����";
	case MDB_RET_FIELD_NOTEXIST:
		return "�ֶβ�����";
	case MDB_RET_RETRIEVE_OVERFLOW:
		return "��ȡ���ݼ����";
	case MDB_RET_COMM_ERROR:
		return "ͨѶ����";
	case MDB_RET_TYPE_ERROR:
		return "���ʹ���";
	case MDB_RET_DISK_ERROR:
		return "���̴���";
	case MDB_RET_CONTEXT_NOTEXIST:
		return "���ݲ�����";
	case MDB_RET_UNIQUE_VIOLATION:
		return "Υ��Ψһ��Լ��";
	case MDB_RET_DATA_OVERFLOW:
		return "�������";
	case MDB_RET_UNKNOWN:
		return "δ֪����";
	}
	return "Unknown";
}


////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ��ǰ�����������ݱ�
// ��    ��:  �ۿ���
// ����ʱ��:  2014-11-13 13:11
// ����˵��:  @SysTablesΪ���÷��ص�ϵͳ���б�
// �� �� ֵ:  >=0��ʾ���ݱ�������<0��ʾʧ��
//////////////////////////////////////////////////////////////////////////
int CMdbClient::GetSysTables(SPtrList<stuSysTable> &SysTables)
{
	void *pTables = NULL;
	stuSysTable* pNew;
	SString sql = "select * from systable";
	SysTables.clear();
	SysTables.setAutoDelete(true);
	int rowsize = sizeof(stuSysTable);
	int ret = Retrieve(sql, rowsize, pTables);
	if (ret <= 0)
	{
		if (pTables != NULL)
			delete[] (stuSysTable*)pTables;
	}
	for (int i = 0; i < ret; i++)
	{
		pNew = new stuSysTable;
		memcpy(pNew, &((stuSysTable*)pTables)[i], sizeof(stuSysTable));
		SysTables.append(pNew);
	}
	return ret;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡָ�����ݱ��е��ֶ���Ϣ
// ��    ��:  �ۿ���
// ����ʱ��:  2014-11-13 13:49
// ����˵��:  @sTable����
//			  @TableFieldsΪ���÷��ص��ֶ���Ϣ�б�
// �� �� ֵ:  >=0��ʾ�ֶ�������<0��ʾʧ��
//////////////////////////////////////////////////////////////////////////
int CMdbClient::GetTableFields(const char* sTable, SPtrList<stuTableField> &TableFields)
{
	if (!IsConnected())
		return MDB_RET_DISCONNECT;
	TableFields.clear();
	TableFields.setAutoDelete(true);
	SString head = sTable;
	stuSTcpPackage* pRecvPackage = NULL;
	if (!SendAndSearchPkg(pRecvPackage, head, MDB_FT_FIELDS))
	{
		sprintf(m_sLastError, "ִ��GetTableFieldsʧ��!");
		LOGWARN("%s", m_sLastError);
		if (pRecvPackage != NULL)
			delete pRecvPackage;
		return MDB_RET_COMM_ERROR;
	}
	if (pRecvPackage->m_wFrameType != MDB_FT_FIELDS || pRecvPackage->m_iAsduLen != 4)
	{
		delete pRecvPackage;
		return MDB_RET_UNKNOWN;
	}
	int ret = *((int*)pRecvPackage->m_pAsduBuffer);
	SKT_SWAP_DWORD(ret);
	if (ret > 0)
	{
		stuTableField *pNew;
		SString str;
		int i, cnt = SString::GetAttributeCount(pRecvPackage->m_sHead, ";");
		for (i = 1; i <= cnt; i++)
		{
			str = SString::GetIdAttribute(i, pRecvPackage->m_sHead, ";");
			pNew = new stuTableField;
			memset(pNew, 0, sizeof(stuTableField));
			strcpy(pNew->name, SString::GetIdAttribute(1, str, ",").left(sizeof(pNew->name) - 1));
			pNew->type		= SString::GetIdAttributeI(2, str, ",");
			pNew->start_pos = SString::GetIdAttributeI(3, str, ",");
			pNew->bytes		= SString::GetIdAttributeI(4, str, ",");
			TableFields.append(pNew);
		}
	}
	delete pRecvPackage;
	return ret;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ��¼��ָ����ŵ��ֶ��ַ���ֵ����
// ��    ��:  �ۿ���
// ����ʱ��:  2014-11-14 10:46
// ����˵��:  @pRowΪ��¼����
//			  @pFldΪ�ֶ���Ϣ
// �� �� ֵ:  �ַ�����ֵ����
//////////////////////////////////////////////////////////////////////////
SString CMdbClient::GetValueStr(BYTE *pRow, stuTableField *pFld)
{
	SString str;
	switch (pFld->type)
	{
		case MDB_INT:	//4 bytes
		case MDB_BLOB:
		{
	#ifndef _ARM
			str = SString::toString(*((int*)(pRow + pFld->start_pos)));

	#else
			int val = 0;
			memset(&val,pRow + pFld->start_pos,sizeof(val));
			str = SString::toString(val);
	#endif
		}
		break;
		case MDB_SHORT:	//2 bytes
		{
	#ifndef _ARM
			str = SString::toString(*((short*)(pRow + pFld->start_pos)));

	#else
			short val = 0;
			memset(&val,pRow + pFld->start_pos,sizeof(val));
			str = SString::toString(val);
	#endif
		}
			break;
		case MDB_BYTE:	//1 bytes
			str = SString::toString(pRow[pFld->start_pos]);
			break;
		case MDB_INT64:	//8 bytes
		{
	#ifndef _ARM
			str = SString::toFormat("%ld",*((INT64*)(pRow + pFld->start_pos)));

	#else
			INT64 val = 0;
			memset(&val,pRow + pFld->start_pos,sizeof(val));
			str = SString::toFormat("%ld",val);
	#endif
		}
			break;
		case MDB_FLOAT:	//4 bytes
		{
	#ifndef _ARM
			str = SString::toFormat("%f", *((float*)(pRow + pFld->start_pos)));

	#else
			float val = 0;
			memset(&val,pRow + pFld->start_pos,sizeof(val));
			str = SString::toFormat("%f",val);
	#endif
		}
			break;
		case MDB_STR:	//N bytes���ɽ����﷨������
		{
			str = SString::toString((char*)(pRow + pFld->start_pos), pFld->bytes);					
		}
			break;
		default:
		{
			LOGDEBUG("�ֶ�%s����Ч����������:%d", pFld->name, pFld->type);
		}
			break;
	}
	return str;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ͨ�����ݼ������������ڴ�ṹ��ֵ
// ��    ��:  �ۿ���
// ����ʱ��:  2016-3-24 10:08
// ����˵��:  @pRows��ʾ�����еĻ�����
//         :  @iRows��ʾ������������
//         :  @iRowSize��ʾ�г���
//         :  @pRs��ʾ���ݼ�ָ��
//         :  @TableFields��ʾ���ֶ�����,�ֶ�������˳�������pRs���ݼ��е�һ��
// �� �� ֵ:  ���سɹ����Ƶ���Ч����
//////////////////////////////////////////////////////////////////////////
int CMdbClient::SetValueFromRs(BYTE *pRows,int iRows,int iRowSize,SRecordset *pRs,SPtrList<stuTableField> &TableFields)
{
#if 1
	unsigned long pos;
	int i;
	int rows = pRs->GetRows();
	if(rows > iRows)
	{
		LOGERROR("����������(%d)���㣬���ݼ�����(%d)��!",iRows,rows);
		return -1;
	}
	int col=0;
	stuTableField *pFld = TableFields.FetchFirst(pos);
	
	while(pFld)
	{
		switch (pFld->type)
		{
		case MDB_INT:	//4 bytes
		case MDB_BLOB:
			{
				for(i=0;i<rows;i++)
				{
#ifndef _ARM
				*((int*)(pRows + i*iRowSize + pFld->start_pos)) = pRs->GetValue(i,col).toInt();

#else
				int val = pRs->GetValue(i,col).toInt();
				memset(pRows + i*iRowSize + pFld->start_pos,&val,sizeof(val));
#endif
				}
			}
			break;
		case MDB_SHORT:	//2 bytes
			{
				for(i=0;i<rows;i++)
				{
#ifndef _ARM
				*((short*)(pRows + i*iRowSize + pFld->start_pos)) = pRs->GetValue(i,col).toShort();
#else
				short val = pRs->GetValue(i,col).toShort();
				memset(pRows + i*iRowSize + pFld->start_pos,&val,sizeof(val));
#endif
				}
			}
			break;
		case MDB_BYTE:	//1 bytes
			for(i=0;i<rows;i++)
			{
				pRows[i*iRowSize + pFld->start_pos] = (BYTE)pRs->GetValue(i,col).toInt();
			}
			break;
		case MDB_INT64:	//8 bytes
			{
				for(i=0;i<rows;i++)
				{
#ifndef _ARM
				*((INT64*)(pRows + i*iRowSize + pFld->start_pos)) = SString::toInt64(pRs->GetValue(i,col).data());

#else
				INT64 val = SString::toInt64(pRs->GetValue(i,col).data());
				memset(pRows + i*iRowSize + pFld->start_pos,&val,sizeof(val));
#endif
				}
			}
			break;
		case MDB_FLOAT:	//4 bytes
			{
				for(i=0;i<rows;i++)
				{
#ifndef _ARM
				*((float*)(pRows + i*iRowSize + pFld->start_pos)) = pRs->GetValue(i,col).toFloat();

#else
				float val = pRs->GetValue(i,col).toFloat();
				memset(pRows + i*iRowSize + pFld->start_pos,&val,sizeof(val));
#endif
				}
			}
			break;
		case MDB_STR:	//N bytes���ɽ����﷨������
			for(i=0;i<rows;i++)
			{
				SString::strncpy((char*)(pRows + i*iRowSize + pFld->start_pos), pFld->bytes,pRs->GetValue(i,col).data());
			}
			break;
		default:
			{
				LOGDEBUG("�ֶ�%s����Ч����������:%d", pFld->name, pFld->type);
			}
			break;
		}
		col++;
		pFld = TableFields.FetchNext(pos);
	}
#endif
	return rows;
}
