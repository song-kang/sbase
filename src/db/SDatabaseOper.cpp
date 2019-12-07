/**
 *
 * �� �� ��	: SDatabaseOper.cpp
 * ��������	: 2008-06-16
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸�����	: $Date: $
 * ��ǰ�汾	: $Revision: $
 * �������� : 
 * �޸ļ�¼ : 
 *            $Log: $
 *
 **/

#include "SDatabaseOper.h"
#include "SApi.h"

//����DMLSQL��������
static int g_iMaxDmlBufferSize = 500000;
static int g_iMaxDmlPackageBytes = 65536;
#define C_SQL_SIGNAL_QUOTES "~@~"
SDatabaseOper::SDatabaseOper()
{
	m_pDatabasePool = m_pSlaveDatabasePool = m_pActionDatabasePool = NULL;
	m_bQuit = false;
	m_bDmlPublishThreadRuning = false;
	m_sDmlSqlBuffered.setAutoDelete(true);
	m_sDmlSqlBuffered.setShared(true);
	m_DbSyncSqls.setShared(true);
	m_SlaveDbSyncSqls.setShared(true);
	m_iThreads = 0;
}

SDatabaseOper::~SDatabaseOper()
{
	m_pDatabasePool = m_pSlaveDatabasePool = m_pActionDatabasePool = NULL;
	WaitQuit();
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �������ݿ����ӳ�
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-30 9:16
// ����˵��:  @pPoolΪ�����ݿ����ӳ�
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SDatabaseOper::SetDatabasePool(SDatabasePool<SDatabase> *pPool)
{
	m_pActionDatabasePool = m_pDatabasePool = pPool;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �豸�������ݿ����ӳ�
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-30 9:20
// ����˵��:  @pPoolΪ�������ݿ����ӳ�
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SDatabaseOper::SetSlaveDatabasePool(SDatabasePool<SDatabase> *pPool)
{
	m_pSlaveDatabasePool = pPool;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡָ��SQL�ļ�¼��
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-30 9:25
// ����˵��:  @sqlΪSELECT���
//         :  @rsΪ���÷��صļ�¼��
// �� �� ֵ:  int,��ʾ���������=0��ʾ�޼�¼��<0��ʾʧ��
//////////////////////////////////////////////////////////////////////////
int SDatabaseOper::RetrieveRecordset(SString sql,SRecordset &rs)
{
	SDatabasePool<SDatabase> *pThisPool = m_pActionDatabasePool;
	SDatabase *pDB;
	int ret;
	for(int i=0;i<2;i++)
	{
		if(pThisPool == NULL)
			break;
		pDB = pThisPool->GetDatabase();
		if(pDB == NULL)
		{
			LOGWARN("�޷���ȡ���õ����ݿ�����!sql=%s",sql.left(1000).data());
			goto err;
		}
		ret = pDB->Retrieve(sql,rs);
		pThisPool->Release(pDB);
		if(pDB->GetStatus() != SDatabase::DBERROR)
		{
			if(i == 1)
			{
				m_pActionDatabasePool = pThisPool;
				LOGWARN("���ݿ����л���%s���ݿ�����!",pThisPool==m_pDatabasePool?"��":"����");
			}
			return ret;
		}

err://���Ӳ����ӳش��󣬳����л����ݿ�
		pThisPool = GetSecondPool(pThisPool);
		if(pThisPool != NULL)
		{
			LOGWARN("�����л���%s���ݿ�����ִ�в�ѯָ�",pThisPool==m_pDatabasePool?"��":"����");
		}
	}
	return -1001;
}


//////////////////////////////////////////////////////////////////////////
// ��    ��:  ����ָ��SELECT���������0�е�col�е�ֵ
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-30 9:27
// ����˵��:  @sqlΪSELECT���
//         :  @colΪ�кţ���0��ʼ
// �� �� ֵ:  SString/int/float
//////////////////////////////////////////////////////////////////////////
SString SDatabaseOper::SelectInto(SString sql,int col/*=0*/)
{
	SDatabasePool<SDatabase> *pThisPool = m_pActionDatabasePool;
	SDatabase *pDB;
	SString ret;
	for(int i=0;i<2;i++)
	{
		if(pThisPool == NULL)
			break;
		pDB = pThisPool->GetDatabase();
		if(pDB == NULL)
		{
			LOGWARN("�޷���ȡ���õ����ݿ�����!sql=%s",sql.left(1000).data());
			goto err;
		}
		ret = pDB->SelectInto(sql,col);
		pThisPool->Release(pDB);
		if(pDB->GetStatus() != SDatabase::DBERROR)
		{
			if(i == 1)
			{
				m_pActionDatabasePool = pThisPool;
				LOGWARN("���ݿ����л���%s���ݿ�����!",pThisPool==m_pDatabasePool?"��":"����");
			}
			return ret;
		}
		
err://���Ӳ����ӳش��󣬳����л����ݿ�
		pThisPool = GetSecondPool(pThisPool);
		if(pThisPool != NULL)
		{
			LOGWARN("�����л���%s���ݿ�����ִ�в�ѯָ�",pThisPool==m_pDatabasePool?"��":"����");
		}
	}
	return "";
}

int SDatabaseOper::SelectIntoI(SString sql,int col/*=0*/)
{
	return SelectInto(sql,col).toInt();
}
float SDatabaseOper::SelectIntoF(SString sql,int col/*=0*/)
{
	return SelectInto(sql,col).toFloat();
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ִ��SQL
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-30 9:29
// ����˵��:  @sqlΪ����SQL��䣬��update/delete/truncate/drop/...
// �� �� ֵ:  �ɹ�����true,ʧ�ܷ���false
//////////////////////////////////////////////////////////////////////////
bool SDatabaseOper::ExecuteSQL(SString sql)
{
	if(m_bDmlPublishThreadRuning && sql.left(18) != "/*ssp_no_publish*/")
	{
		if(m_sDmlSqlBuffered.count() < g_iMaxDmlBufferSize)
			m_sDmlSqlBuffered.append(new SString(sql));
		else
		{
			LOGERROR("DML������SQL������������������!");
		}
	}
	SDatabasePool<SDatabase> *pThisPool = m_pActionDatabasePool;
	SDatabase *pDB;
	bool ret;
	for(int i=0;i<2;i++)
	{
		if(pThisPool == NULL)
			break;
		pDB = pThisPool->GetDatabase();
		if(pDB == NULL)
		{
			LOGWARN("�޷���ȡ���õ����ݿ�����!sql=%s",sql.left(1000).data());
			goto err;
		}
		ret = pDB->Execute(sql);
		pThisPool->Release(pDB);
		if(ret)
		{
			if(i == 1)
			{
				m_pActionDatabasePool = pThisPool;
				LOGWARN("���ݿ����л���%s���ݿ�����!",pThisPool==m_pDatabasePool?"��":"����");
			}
			AddHistoryOperSql(pThisPool,sql);
			m_Wait.post();
			return true;
		}
		else if(pDB->GetStatus() != SDatabase::DBERROR)
		{
			//���ݿ�δ�쳣��Ӧ����SQL�������𣬶����������쳣
			return false;
		}
		
		err://���Ӳ����ӳش��󣬳����л����ݿ�
		pThisPool = GetSecondPool(pThisPool);
		if(pThisPool != NULL)
		{
			LOGWARN("�����л���%s���ݿ�����ִ��ָ�",pThisPool==m_pDatabasePool?"��":"����");
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �������ݿ�ͬ������
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-30 9:32
// ����˵��:  @bGlobalSync��ʾ�Ƿ���ȫ�ֿ�ͬ����true��ʾ�򿪣�false��ʾ�ر�
//         :  һ������£�һ��ϵͳ�н���Ҫһ�����̿�����ͬ�����ý���Ӧȷ�����ڴ�������״̬
// �� �� ֵ:  true/false
//////////////////////////////////////////////////////////////////////////
bool SDatabaseOper::Start(bool bGlobalSync)
{
	if(/*m_bQuit == false || */m_pSlaveDatabasePool == NULL)
	{
		if(m_pSlaveDatabasePool == NULL)
		{
			LOGWARN("�������ݿ����ӳ�δ���ã��������ݿ�ͬ������ʧ��!");
		}
		return false;
	}
	m_bQuit = false;
	m_bGlobalSync = bGlobalSync;
	S_CREATE_THREAD(ThreadDbSync,this);
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �˳����ݿ�ͬ������
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-30 9:32
// ����˵��:  void
// �� �� ֵ:  true/false
//////////////////////////////////////////////////////////////////////////
bool SDatabaseOper::Quit()
{
	m_bQuit = true;

	return true;
}

bool SDatabaseOper::WaitQuit()
{
	while(m_iThreads > 0)
		SApi::UsSleep(10000);
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ͨ���ļ����´��ֶ�����
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-30 9:54
// ����˵��:  @sTableΪ�����µı���
//         :  @sLobFieldsΪ���ֶε��ֶ���
//         :  @sWhere��ʾΨһȷ�������е�������������������˳�Ψһ��һ�м�¼
//         :  @sFileΪ�����ļ���ȫ·���������ϴ��Ĵ��ֶ�����
// �� �� ֵ:  true��ʾ�ɹ���false��ʾʧ��
//////////////////////////////////////////////////////////////////////////
bool SDatabaseOper::UpdateLobFromFile(SString sTable,SString sLobField,SString sWhere,SString sFile)
{
	SDatabasePool<SDatabase> *pThisPool = m_pActionDatabasePool;
	SDatabase *pDB;
	bool ret;
	for(int i=0;i<2;i++)
	{
		if(pThisPool == NULL)
			break;
		pDB = pThisPool->GetDatabase();
		if(pDB == NULL)
		{
			LOGWARN("�޷���ȡ���õ����ݿ�����!");
			goto err;
		}
		ret = pDB->UpdateLobFromFile(sTable,sLobField,sWhere,sFile);
		pThisPool->Release(pDB);
		if(ret)
		{
			if(i == 1)
			{
				m_pActionDatabasePool = pThisPool;
				LOGWARN("���ݿ����л���%s���ݿ�����!",pThisPool==m_pDatabasePool?"��":"����");
			}
			SString sql;
			sql.sprintf("act=update_lob{$SEP$}table=%s{$SEP$}field=%s{$SEP$}where=%s{$SEP$}",sTable.data(),sLobField.data(),sWhere.data());
			AddHistoryOperSql(pThisPool,sql);
			m_Wait.post();
			return true;
		}
		else if(pDB->GetStatus() != SDatabase::DBERROR)
		{
			//���ݿ�δ�쳣��Ӧ����SQL�������𣬶����������쳣
			return false;
		}

err://���Ӳ����ӳش��󣬳����л����ݿ�
		pThisPool = GetSecondPool(pThisPool);
		if(pThisPool != NULL)
		{
			LOGWARN("�����л���%s���ݿ�����ִ��ָ�",pThisPool==m_pDatabasePool?"��":"����");
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ָ���Ĵ��ֶζ�ȡ���ݲ�������ļ�
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-30 9:58
// ����˵��:  @sTableΪ�����µı���
//         :  @sLobFieldsΪ���ֶε��ֶ���
//         :  @sWhere��ʾΨһȷ�������е�������������������˳�Ψһ��һ�м�¼
//         :  @sFileΪ�����ļ���ȫ·������ȡ�������ݽ�д����ļ�
// �� �� ֵ:  true��ʾ�ɹ���false��ʾʧ��
//////////////////////////////////////////////////////////////////////////
bool SDatabaseOper::ReadLobToFile(SString sTable,SString sLobField,SString sWhere,SString sFile)
{
	SDatabasePool<SDatabase> *pThisPool = m_pActionDatabasePool;
	SDatabase *pDB;
	bool ret;
	for(int i=0;i<2;i++)
	{
		if(pThisPool == NULL)
			break;
		pDB = pThisPool->GetDatabase();
		if(pDB == NULL)
		{
			LOGWARN("�޷���ȡ���õ����ݿ�����!");
			goto err;
		}
		ret = pDB->ReadLobToFile(sTable,sLobField,sWhere,sFile);
		pThisPool->Release(pDB);
		if(ret)
		{
			if(i == 1)
			{
				m_pActionDatabasePool = pThisPool;
				LOGWARN("���ݿ����л���%s���ݿ�����!",pThisPool==m_pDatabasePool?"��":"����");
			}
			return true;
		}
		else if(pDB->GetStatus() != SDatabase::DBERROR)
		{
			//���ݿ�δ�쳣��Ӧ����SQL�������𣬶����������쳣
			return false;
		}

err://���Ӳ����ӳش��󣬳����л����ݿ�
		pThisPool = GetSecondPool(pThisPool);
		if(pThisPool != NULL)
		{
			LOGWARN("�����л���%s���ݿ�����ִ��ָ�",pThisPool==m_pDatabasePool?"��":"����");
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ָ���Ĵ��ֶζ�ȡ���ݲ�������ļ�
// ��    ��:  �ۿ���
// ����ʱ��:  2015-8-4 14:31
// ����˵��:  @sTableΪ�����µı���
//         :  @sLobFieldsΪ���ֶε��ֶ���
//         :  @sWhere��ʾΨһȷ�������е�������������������˳�Ψһ��һ�м�¼
//         :  @pBufΪ
// �� �� ֵ:  true��ʾ�ɹ���false��ʾʧ��
//////////////////////////////////////////////////////////////////////////
bool SDatabaseOper::ReadLobToMem(SString sTable,SString sLobField,SString sWhere,unsigned char* &pBuf,int &iBufLen)
{
	SDatabasePool<SDatabase> *pThisPool = m_pActionDatabasePool;
	SDatabase *pDB;
	bool ret;
	for(int i=0;i<2;i++)
	{
		if(pThisPool == NULL)
			break;
		pDB = pThisPool->GetDatabase();
		if(pDB == NULL)
		{
			LOGWARN("�޷���ȡ���õ����ݿ�����!");
			goto err;
		}
		ret = pDB->ReadLobToMem(sTable,sLobField,sWhere,pBuf,iBufLen);
		pThisPool->Release(pDB);
		if(ret)
		{
			if(i == 1)
			{
				m_pActionDatabasePool = pThisPool;
				LOGWARN("���ݿ����л���%s���ݿ�����!",pThisPool==m_pDatabasePool?"��":"����");
			}			
			return true;
		}
		else if(pDB->GetStatus() != SDatabase::DBERROR)
		{
			//���ݿ�δ�쳣��Ӧ����SQL�������𣬶����������쳣
			return false;
		}

err://���Ӳ����ӳش��󣬳����л����ݿ�
		pThisPool = GetSecondPool(pThisPool);
		if(pThisPool != NULL)
		{
			LOGWARN("�����л���%s���ݿ�����ִ��ָ�",pThisPool==m_pDatabasePool?"��":"����");
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �����ͷ�ReadLobToMem���������Ļ�����
// ��    ��:  �ۿ���
// ����ʱ��:  2017-12-1 11:01
// ����˵��:  @pBufΪ������ָ��
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SDatabaseOper::FreeLobMem(unsigned char* pBuf)
{
	SDatabase *p = m_pActionDatabasePool->GetDatabaseByIdx(0);
	if(p)
		p->FreeLobMem(pBuf);
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ���ڴ���ֶ�д�����ݿ�
// ��    ��:  �ۿ���
// ����ʱ��:  2015-8-29 17:43
// ����˵��:  
// �� �� ֵ:  true/false
//////////////////////////////////////////////////////////////////////////
bool SDatabaseOper::UpdateLobFromMem(SString sTable,SString sLobField,SString sWhere,unsigned char* pBuf,int iBufLen)
{
	SDatabasePool<SDatabase> *pThisPool = m_pActionDatabasePool;
	SDatabase *pDB;
	bool ret;
	for(int i=0;i<2;i++)
	{
		if(pThisPool == NULL)
			break;
		pDB = pThisPool->GetDatabase();
		if(pDB == NULL)
		{
			LOGWARN("�޷���ȡ���õ����ݿ�����!");
			goto err;
		}
		ret = pDB->UpdateLobFromMem(sTable,sLobField,sWhere,pBuf,iBufLen);
		pThisPool->Release(pDB);
		if(ret)
		{
			if(i == 1)
			{
				m_pActionDatabasePool = pThisPool;
				LOGWARN("���ݿ����л���%s���ݿ�����!",pThisPool==m_pDatabasePool?"��":"����");
			}
			SString sql;
			sql.sprintf("act=update_lob{$SEP$}table=%s{$SEP$}field=%s{$SEP$}where=%s{$SEP$}",sTable.data(),sLobField.data(),sWhere.data());
			AddHistoryOperSql(pThisPool,sql);
			m_Wait.post();
			return true;
		}
		else if(pDB->GetStatus() != SDatabase::DBERROR)
		{
			//���ݿ�δ�쳣��Ӧ����SQL�������𣬶����������쳣
			return false;
		}

err://���Ӳ����ӳش��󣬳����л����ݿ�
		pThisPool = GetSecondPool(pThisPool);
		if(pThisPool != NULL)
		{
			LOGWARN("�����л���%s���ݿ�����ִ��ָ�",pThisPool==m_pDatabasePool?"��":"����");
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ���ݿ�ͬ���߳�
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-30 9:31
// ����˵��:  @lpΪthis
// �� �� ֵ:  NULL
//////////////////////////////////////////////////////////////////////////
void* SDatabaseOper::ThreadDbSync(void* lp)
{
	SDatabaseOper *pThis = (SDatabaseOper*)lp;
	S_INTO_THREAD;
	SDatabasePool<SDatabase> *pThisPool,*pThatPool;
	SDatabase *pDb,*pDb2;
	SString *pSql;
	SString sql;
	SRecordset rs;
	int last_soc=0;
	int last_soc0=0;
	int soc;
	bool bret;
	int cnt;
	int times=0;
	pThis->m_iThreads++;
	while(!pThis->m_bQuit)
	{
		//�������ݿ�
		pThisPool = pThis->m_pActionDatabasePool;
		soc = (int)SDateTime::getNowSoc();
		if(abs(soc - last_soc0) > 1)
		{
			last_soc0 = soc;
			if(pThisPool != pThis->m_pDatabasePool)
			{
				//�ж������ݿ��Ƿ�ָ���������?
				pThisPool = pThis->m_pDatabasePool;
				pDb = pThisPool->GetDatabase(true);
				if(pDb != NULL)
				{
					pThisPool->Release(pDb);
					LOGWARN("�����ݿ����ӻָ����л��������ݿ�����!");
					pThis->m_pActionDatabasePool = pThis->m_pDatabasePool;
				}
			}
			if(pThisPool != pThis->m_pSlaveDatabasePool && pThis->m_pSlaveDatabasePool != NULL)
			{
				//�ж������ݿ��Ƿ�ָ���������?
				pThisPool = pThis->m_pSlaveDatabasePool;
				pDb = pThisPool->GetDatabase(true);
				if(pDb != NULL)
				{
					pThisPool->Release(pDb);
				}
			}
		}
		//ȫ��ͬ��
		if(pThis->m_bGlobalSync)
		{
			soc = (int)SDateTime::getNowSoc();
			if(times<5 || abs(soc - last_soc) >= 5)//5��ִ��һ��ȫ��ͬ��,�������ݿ⻺����ͬ��
			{
				times ++;
				int total=0;
				for(int i=0;i<2&&!pThis->m_bQuit;i++)
				{
					if(i==0)
					{
						pThisPool = pThis->m_pDatabasePool;
						pThatPool = pThis->m_pSlaveDatabasePool;
					}
					else
					{
						pThisPool = pThis->m_pSlaveDatabasePool;
						pThatPool = pThis->m_pDatabasePool;
					}
					rs.clear();
					pDb = pThisPool->GetDatabase();
					if(pDb == NULL)
						continue;
					if(pDb->GetDbType() == DB_ORACLE)
						cnt = pDb->Retrieve("select sql_sn,sql_text from t_ssp_db_sql_buffer where rownum<1000 order by sql_sn",rs);
					else if(pDb->GetDbType() == DB_MYSQL)
						cnt = pDb->Retrieve("select sql_sn,sql_text from t_ssp_db_sql_buffer order by sql_sn limit 1000",rs);
					else
						cnt = pDb->Retrieve("select sql_sn,sql_text from t_ssp_db_sql_buffer order by sql_sn",rs);
					if(cnt > 0)
					{
						pDb2 = pThatPool->GetDatabase(true);
						total += cnt;
						for(int j=0;pDb2!=NULL&&j<cnt&&!pThis->m_bQuit;j++)
						{
							sql = rs.GetValue(j,1);
							if(sql.left(23) == "act=update_lob{$SEP$}table=")
							{
								SString sTable,sLobField,sWhere,sFile;
								sTable = SString::GetAttributeValue(sql,"table","=","{$SEP$}");
								sLobField = SString::GetAttributeValue(sql,"field","=","{$SEP$}");
								sWhere = SString::GetAttributeValue(sql,"where","=","{$SEP$}");
								BYTE *pBuf = NULL;
								int iBufLen = 0;
								if(pDb->ReadLobToMem(sTable,sLobField,sWhere,pBuf,iBufLen))
								{
									if(pDb2->UpdateLobFromMem(sTable,sLobField,sWhere,pBuf,iBufLen))
									{
										sql.sprintf("delete from t_ssp_db_sql_buffer where sql_sn=%s",rs.GetValue(j,0).data());
										pDb->Execute(sql);
									}
								}
								if(pBuf != NULL)
									delete[] pBuf;
								continue;
							}
							sql.replace(C_SQL_SIGNAL_QUOTES,"'");
							if(!pDb2->Execute(sql))
							{
								LOGERROR("ͬ��%s���ݿ�ʱʧ��!sql=%s",pThatPool==pThis->m_pDatabasePool?"��":"��",sql.left(1000).data());
								break;
							}
							sql.sprintf("delete from t_ssp_db_sql_buffer where sql_sn=%s",rs.GetValue(j,0).data());
							pDb->Execute(sql);
						}
						if(pDb2 != NULL)
							pThatPool->Release(pDb2);
					}
					pThisPool->Release(pDb);
				}
				if(total < 1000)
					last_soc = soc;
			}
		}

		//ִ����ʷͬ��
		if(pThis->m_DbSyncSqls.count())
		{
			pDb = pThis->m_pDatabasePool->GetDatabase();
			if(pDb != NULL)
			{
				pDb2 = NULL;
				while(pThis->m_DbSyncSqls.count()>0 && !pThis->m_bQuit)
				{
					pSql = pThis->m_DbSyncSqls[0];
					if(pSql->left(27) == "act=update_lob{$SEP$}table=")
					{
						//update blob
						if(pDb2 == NULL)
							pDb2 = pThis->m_pSlaveDatabasePool->GetDatabase(true);
						if(pDb2 != NULL)
						{
							SString sTable,sLobField,sWhere,sFile;
							sTable = SString::GetAttributeValue(*pSql,"table","=","{$SEP$}");
							sLobField = SString::GetAttributeValue(*pSql,"field","=","{$SEP$}");
							sWhere = SString::GetAttributeValue(*pSql,"where","=","{$SEP$}");
							BYTE *pBuf = NULL;
							int iBufLen = 0;
							bret = false;
							if(pDb2->ReadLobToMem(sTable,sLobField,sWhere,pBuf,iBufLen))
								bret = pDb->UpdateLobFromMem(sTable,sLobField,sWhere,pBuf,iBufLen);
							if(pBuf != NULL)
								delete[] pBuf;
						}
					}
					else
					{
						bret = pDb->Execute(*pSql);
					}
					if(!bret)
					{
						if(pDb->GetStatus() == SDatabase::DBERROR)
						{
							//���ݿ����Ӵ���д����һ���ӳص�SQL����
							SDatabase *pDb2 = pThis->m_pSlaveDatabasePool->GetDatabase();
							if(pDb2 != NULL)
							{
								SString rawSql = *pSql;
								rawSql.replace("'",C_SQL_SIGNAL_QUOTES);
								sql = "insert into t_ssp_db_sql_buffer(sql_sn,sql_text)  (select (select ifnull(max(sql_sn),0)+1 from t_ssp_db_sql_buffer),'";
								sql += rawSql;
								sql += "')";
								pDb2->Execute(sql);
							}
						}
						else
						{
							static int cnt = 0;
							if(cnt ++ < 1000)
							{
								LOGERROR("�����޷�ͬ��д��ӿ�SQL��%s",pSql->data());
							}
						}
						//break;
					}
					pThis->m_DbSyncSqls.remove(0);
				}
				if(pDb2 != NULL)
					pThis->m_pSlaveDatabasePool->Release(pDb2);
			}
			pThis->m_pDatabasePool->Release(pDb);
		}
		if(pThis->m_SlaveDbSyncSqls.count())
		{
			pDb = pThis->m_pSlaveDatabasePool->GetDatabase();
			if(pDb != NULL)
			{
				pDb2 = NULL;
				while(pThis->m_SlaveDbSyncSqls.count()>0 && !pThis->m_bQuit)
				{
					pSql = pThis->m_SlaveDbSyncSqls[0];
					if(pSql->left(27) == "act=update_lob{$SEP$}table=")
					{
						//update blob
						if(pDb2 == NULL)
							pDb2 = pThis->m_pDatabasePool->GetDatabase(true);
						if(pDb2 != NULL)
						{
							SString sTable,sLobField,sWhere,sFile;
							sTable = SString::GetAttributeValue(*pSql,"table","=","{$SEP$}");
							sLobField = SString::GetAttributeValue(*pSql,"field","=","{$SEP$}");
							sWhere = SString::GetAttributeValue(*pSql,"where","=","{$SEP$}");
							sFile = SString::GetAttributeValue(*pSql,"file","=","{$SEP$}");
							BYTE *pBuf = NULL;
							int iBufLen = 0;
							bret = false;
							if(pDb2->ReadLobToMem(sTable,sLobField,sWhere,pBuf,iBufLen))
								bret = pDb->UpdateLobFromMem(sTable,sLobField,sWhere,pBuf,iBufLen);
							if(pBuf != NULL)
								delete[] pBuf;
						}
					}
					else
					{
						bret = pDb->Execute(*pSql);
					}
					if(!bret)
					{
						if(pDb->GetStatus() == SDatabase::DBERROR)
						{
							//���ݿ����Ӵ���д����һ���ӳص�SQL����
							SDatabase *pDb2 = pThis->m_pDatabasePool->GetDatabase();
							if(pDb2 != NULL)
							{
								SString rawSql = *pSql;
								rawSql.replace("'",C_SQL_SIGNAL_QUOTES);
								sql = "insert into t_ssp_db_sql_buffer(sql_sn,sql_text)  (select (select ifnull(max(sql_sn),0)+1 from t_ssp_db_sql_buffer),'";
								sql += rawSql;
								sql += "')";
								pDb2->Execute(sql);
							}
						}
						else
						{
							static int cnt = 0;
							if(cnt ++ < 1000)
							{
								LOGERROR("�ӿ��޷�ͬ��д������SQL��%s",pSql->data());
							}
						}
						//break;
					}
					pThis->m_SlaveDbSyncSqls.remove(0);
				}
				if(pDb2 != NULL)
					pThis->m_pDatabasePool->Release(pDb2);
			}
			pThis->m_pSlaveDatabasePool->Release(pDb);
		}

		pThis->m_Wait.wait(500);		
	}
	pThis->m_iThreads--;
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ����DML����ʵʱ����
// ��    ��:  �ۿ���
// ����ʱ��:  2016-8-11 17:16
// ����˵��:  @sIp��ʾ����Ŀ���������IP
//         :  @iTcpPort��ʾ����Ŀ������TCP�˿ں�
// �� �� ֵ:  true��ʾ�ɹ���false��ʾʧ��
//////////////////////////////////////////////////////////////////////////
bool SDatabaseOper::StartDmlPublish(SString sIp,int iTcpPort)
{
	if(m_bDmlPublishThreadRuning)
		return false;
	m_bDmlPublishThreadRuning = true;

	stuDmlPublishThreadParam *pParam = new stuDmlPublishThreadParam();
	pParam->pThis = this;
	pParam->pub_dml_sqls = &m_sDmlSqlBuffered;
	pParam->pub_ip = sIp;
	pParam->pub_port = iTcpPort;
	pParam->is_master = true;
	SKT_CREATE_THREAD(ThreadDmlPublish,pParam);
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �豸DML��������󻺳�����,ȱʡ500000
// ��    ��:  �ۿ���
// ����ʱ��:  2019-4-23 19:23
// ����˵��:  @maxΪ��󻺳�SQL������,0��ʾ����������
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SDatabaseOper::SetDmlPublishMaxBufferSize(int max)
{
	g_iMaxDmlBufferSize = max;
}


//////////////////////////////////////////////////////////////////////////
// ��    ��:  �豸DML�������ݰ��Ĵ�С��ȱʡ65536
// ��    ��:  �ۿ���
// ����ʱ��:  2019-4-23 19:30
// ����˵��:  @bytesΪ��������͵����ݰ��ֽڴ�С
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SDatabaseOper::SetDmlPublishPackageSize(int bytes)
{
	g_iMaxDmlPackageBytes = bytes;
}


//////////////////////////////////////////////////////////////////////////
// ��    ��:  �������е���ʷDML�����¼�����ݿ�
// ��    ��:  �ۿ���
// ����ʱ��:  2019-4-24 0:04
// ����˵��:  void
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SDatabaseOper::SaveHisDmlSqls(SPtrList<SString> *pub_dml_sqls,SDatabase *pDb)
{
	if(pub_dml_sqls->count() <= 0)
		return;
	///*ssp_no_publish*/insert into t_ssp_db_sql_publish(sql_sn,sql_text) (select (select ifnull(max(sql_sn),0)+1 from t_ssp_db_sql_publish),'aaa' )
	SString sql;
	SString *pSql;
	unsigned long pos;
	SString rawSql;
	pSql = pub_dml_sqls->FetchFirst(pos);
	while(pSql)
	{
		rawSql = *pSql;
		rawSql.replace("'",C_SQL_SIGNAL_QUOTES);
		sql = "/*ssp_no_publish*/insert into t_ssp_db_sql_publish(sql_sn,sql_text)  (select (select ifnull(max(sql_sn),0)+1 from t_ssp_db_sql_publish),'";
		sql += rawSql;
		sql += "')";
		pDb->Execute(sql.left(sql.length()-1));
		pSql = pub_dml_sqls->FetchNext(pos);
	}
}


//////////////////////////////////////////////////////////////////////////
// ��    ��:  DML���ķ�������
// ��    ��:  �ۿ���
// ����ʱ��:  2016-8-11 17:12
// ����˵��:  @lpΪthis
// �� �� ֵ:  NULL
//////////////////////////////////////////////////////////////////////////
void* SDatabaseOper::ThreadDmlPublish(void* lp)
{
	stuDmlPublishThreadParam *pParam = (stuDmlPublishThreadParam*)lp;
	SDatabaseOper *pThis = pParam->pThis;
	S_INTO_THREAD;
	SDmlPubClient tcp;
	SString *pSql;
	SString sqls = "";
	SString sHead = "";
	int last_send_soc=(int)SDateTime::getNowSoc();
	int sql_cnt = 0;
	int i,ret,sn;
	unsigned long pos;
	SRecordset rs;
	SString sql;
	stuSTcpPackage* pRecvPackage = NULL;
	SDatabasePool<SDatabase> *pPool = pParam->is_master?pThis->m_pDatabasePool:pThis->m_pSlaveDatabasePool;
	SDatabase *pDb;
	pThis->m_iThreads++;
// 	SString sLogin ="type=dml_publish;";
// 	tcp.SetLoginInfo(sLogin);
	
	while(!pThis->m_bQuit)
	{
		if(!tcp.Connect(pParam->pub_ip,pParam->pub_port))
		{
			//��ȫ������SQL��¼���浽���ݿ���г־û�
			pDb = pPool->GetDatabase();
			if(pDb != NULL)
			{
				pThis->SaveHisDmlSqls(pParam->pub_dml_sqls,pDb);
				pPool->Release(pDb);
			}
			SApi::UsSleep(100000);
			continue;
		}
		//�����ݿ����ȫ����ʷ����SQL
		sql = "select sql_sn,sql_text from t_ssp_db_sql_publish order by sql_sn";
		pDb = pPool->GetDatabase();
		rs.clear();
		if(pDb != NULL)
		{
			pDb->Retrieve(sql,rs);
			pPool->Release(pDb);
		}
		for(i=0;i<rs.GetRows();i++)
		{
			sn = rs.GetValueInt(i,0);
			sql = rs.GetValueStr(i,1);
			pParam->pub_dml_sqls->append(new SString(sql));
		}
		while(!pThis->m_bQuit)
		{
			if(pParam->pub_dml_sqls->count() == 0)
			{
				if(sqls.length() > 0)
				{
					if(!tcp.SendFrame(sHead,1,(BYTE*)sqls.data(),sqls.length()))
						break;
					last_send_soc=(int)SDateTime::getNowSoc();
					sqls = "";
				}
				else if(last_send_soc+10 < (int)SDateTime::getNowSoc())
				{
					sHead = "act=echo;";
					if(!tcp.SendFrame(sHead))
					{
						sHead = "";
						break;
					}
					last_send_soc=(int)SDateTime::getNowSoc();
					sHead = "";
				}
				SApi::UsSleep(10000);
				continue;
			}

			if(sqls.length() < g_iMaxDmlPackageBytes)
			{
				pSql = pParam->pub_dml_sqls->FetchFirst(pos);
				while(pSql)
				{
					sql_cnt++;
					if(sqls.length() > 0)
						sqls += "{$SQL_SEP$}";
					sqls += *pSql;
					if(sqls.length() >= g_iMaxDmlPackageBytes)
						break;
					pSql = pParam->pub_dml_sqls->FetchNext(pos);
				}
			}

			if(sqls.length() >= g_iMaxDmlPackageBytes)
			{
				last_send_soc=(int)SDateTime::getNowSoc();
				ret = tcp.SendAndRecv(pRecvPackage,sHead,1,(BYTE*)sqls.data(),sqls.length(),60,2);
				if(!ret)
					break;
				if(pRecvPackage == NULL || pRecvPackage->m_wFrameType != 2)
					continue;
				//���Ķ˷��ص�finish_cnt��ʾ�ɹ�ִ�е�����
				int cnt = SString::GetAttributeValueI(pRecvPackage->m_sHead,"finish_cnt");
				//ɾ��cnt��Ӧǰcnt��SQL��¼
				for(i=0;i<cnt;i++)
					pParam->pub_dml_sqls->remove(0);
				sqls = "";
			}
		}//end while
	}//end while
	pDb = pPool->GetDatabase();
	if(pDb != NULL)
	{
		pThis->SaveHisDmlSqls(pParam->pub_dml_sqls,pDb);
		pPool->Release(pDb);
	}
	pThis->m_iThreads--;
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �����ʷ����SQL��¼��ָ�����ӳض�Ӧ����һ�����ӳأ�����һ����Ϊ��ʱ���Դ˲���
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-30 10:50
// ����˵��:  @pThisPool��ʾ��ǰ��
//         :  @sqlΪ����SQL
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SDatabaseOper::AddHistoryOperSql(SDatabasePool<SDatabase> *pThisPool,SString &sql)
{
	SDatabasePool<SDatabase> *pSecPool = GetSecondPool(pThisPool);
	if(pSecPool == NULL)
		return;
	(pSecPool==m_pDatabasePool?m_DbSyncSqls:m_SlaveDbSyncSqls).append(sql);
}
