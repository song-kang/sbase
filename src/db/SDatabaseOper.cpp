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

SDatabaseOper::SDatabaseOper()
{
	m_pDatabasePool = m_pSlaveDatabasePool = m_pActionDatabasePool = NULL;
	m_bQuit = false;
	m_bDmlPublishThreadRuning = false;
	m_sDmlSqlBuffered.setAutoDelete(true);
	m_sDmlSqlBuffered.setShared(true);
}

SDatabaseOper::~SDatabaseOper()
{
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
		if(m_sDmlSqlBuffered.count() < 500000)
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
	if(m_bQuit == false || m_pSlaveDatabasePool == NULL)
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
			sql.sprintf("table=%s{$SEP$}field=%s{$SEP$}where=%s{$SEP$}",sTable.data(),sLobField.data(),sWhere.data());
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
	SDatabasePool<SDatabase> *pThisPool;
	SDatabase *pDb;
	SString sql;
	SRecordset rs;
	int last_soc=0;
	int soc;
	bool bret;
	int cnt;
	while(!pThis->m_bQuit)
	{
		//�������ݿ�
		pThisPool = pThis->m_pActionDatabasePool;
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

		//ȫ��ͬ��
		if(pThis->m_bGlobalSync)
		{
			soc = (int)SDateTime::getNowSoc();
			if(abs(soc - last_soc) >= 5)//5��ִ��һ��ȫ��ͬ��
			{
				for(int i=0;i<2&&!pThis->m_bQuit;i++)
				{
					if(i==0)
						pThisPool = pThis->m_pDatabasePool;
					else
						pThisPool = pThis->m_pSlaveDatabasePool;
					rs.clear();
					pDb = pThisPool->GetDatabase();
					if(pDb == NULL)
						continue;
					cnt = pDb->Retrieve("select oper_sn,sql_text from t_dbsync_history order by oper_sn",rs);
					if(cnt > 0)
					{
						for(int j=0;j<cnt&&!pThis->m_bQuit;j++)
						{
							sql = rs.GetValue(j,1);
							if(!pDb->Execute(sql))
							{
								LOGERROR("ͬ��%s���ݿ�ʱʧ��!sql=%s",pThisPool==pThis->m_pDatabasePool?"��":"��",sql.left(1000).data());
								break;
							}
							sql.sprintf("delete from t_dbsync_history where oper_sn=%s",rs.GetValue(j,0).data());
							pDb->Execute(sql);
						}
					}
					pThisPool->Release(pDb);
				}
				last_soc = soc;
			}
		}

		//ִ����ʷͬ��
		if(pThis->m_DbSyncSqls.count())
		{
			pDb = pThis->m_pDatabasePool->GetDatabase();
			if(pDb != NULL)
			{
				while(pThis->m_DbSyncSqls.count()>0 && !pThis->m_bQuit)
				{
					bret = pDb->Execute(*pThis->m_DbSyncSqls[0]);
					if(!bret)
						break;
					pThis->m_DbSyncSqls.remove(0);
				}
			}
			pThis->m_pDatabasePool->Release(pDb);
		}
		if(pThis->m_SlaveDbSyncSqls.count())
		{
			pDb = pThis->m_pSlaveDatabasePool->GetDatabase();
			if(pDb != NULL)
			{
				while(pThis->m_SlaveDbSyncSqls.count()>0 && !pThis->m_bQuit)
				{
					bret = pDb->Execute(*pThis->m_SlaveDbSyncSqls[0]);
					if(!bret)
						break;
					pThis->m_SlaveDbSyncSqls.remove(0);
				}
			}
			pThis->m_pSlaveDatabasePool->Release(pDb);
		}

		pThis->m_Wait.wait(500);		
	}

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
	m_sDmlPublishServerIp = sIp;
	m_iDmlPublishServerTcpPort = iTcpPort;
	SKT_CREATE_THREAD(ThreadDmlPublish,this);
	return true;
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
	SDatabaseOper *pThis = (SDatabaseOper*)lp;
	S_INTO_THREAD;
	SDmlPubClient tcp;
	SString *pSql;
	SString sqls = "";
	SString sHead = "";
	int last_send_soc=(int)SDateTime::getNowSoc();
// 	SString sLogin ="type=dml_publish;";
// 	tcp.SetLoginInfo(sLogin);
	
	while(!pThis->m_bQuit)
	{
		if(!tcp.Connect(pThis->m_sDmlPublishServerIp,pThis->m_iDmlPublishServerTcpPort))
		{
			SApi::UsSleep(100000);
			continue;
		}
		while(!pThis->m_bQuit)
		{
			if(pThis->m_sDmlSqlBuffered.count() == 0)
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
			if(sqls.length() >= 65536)
			{
				if(!tcp.SendFrame(sHead,1,(BYTE*)sqls.data(),sqls.length()))
					break;
				last_send_soc=(int)SDateTime::getNowSoc();
				sqls = "";
			}
			pSql = pThis->m_sDmlSqlBuffered[0];
			if(sqls.length() > 0)
				sqls += "{$SQL_SEP$}";
			sqls += *pSql;
			pThis->m_sDmlSqlBuffered.remove(0);
		}
	}
	if(sqls.length() > 0)
		tcp.SendFrame(sHead,1,(BYTE*)sqls.data(),sqls.length());
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
