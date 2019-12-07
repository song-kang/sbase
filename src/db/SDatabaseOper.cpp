/**
 *
 * 文 件 名	: SDatabaseOper.cpp
 * 创建日期	: 2008-06-16
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期	: $Date: $
 * 当前版本	: $Revision: $
 * 功能描述 : 
 * 修改记录 : 
 *            $Log: $
 *
 **/

#include "SDatabaseOper.h"
#include "SApi.h"

//最大的DMLSQL缓冲数量
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
// 描    述:  设置数据库连接池
// 作    者:  邵凯田
// 创建时间:  2015-7-30 9:16
// 参数说明:  @pPool为主数据库连接池
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SDatabaseOper::SetDatabasePool(SDatabasePool<SDatabase> *pPool)
{
	m_pActionDatabasePool = m_pDatabasePool = pPool;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  设备备用数据库连接池
// 作    者:  邵凯田
// 创建时间:  2015-7-30 9:20
// 参数说明:  @pPool为备用数据库连接池
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SDatabaseOper::SetSlaveDatabasePool(SDatabasePool<SDatabase> *pPool)
{
	m_pSlaveDatabasePool = pPool;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  取指定SQL的记录集
// 作    者:  邵凯田
// 创建时间:  2015-7-30 9:25
// 参数说明:  @sql为SELECT语句
//         :  @rs为引用返回的记录集
// 返 回 值:  int,表示结果数量，=0表示无记录，<0表示失败
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
			LOGWARN("无法获取可用的数据库连接!sql=%s",sql.left(1000).data());
			goto err;
		}
		ret = pDB->Retrieve(sql,rs);
		pThisPool->Release(pDB);
		if(pDB->GetStatus() != SDatabase::DBERROR)
		{
			if(i == 1)
			{
				m_pActionDatabasePool = pThisPool;
				LOGWARN("数据库已切换到%s数据库运行!",pThisPool==m_pDatabasePool?"主":"备用");
			}
			return ret;
		}

err://连接并连接池错误，尝试切换数据库
		pThisPool = GetSecondPool(pThisPool);
		if(pThisPool != NULL)
		{
			LOGWARN("尝试切换到%s数据库重新执行查询指令！",pThisPool==m_pDatabasePool?"主":"备用");
		}
	}
	return -1001;
}


//////////////////////////////////////////////////////////////////////////
// 描    述:  返回指定SELECT语句结果集第0行第col列的值
// 作    者:  邵凯田
// 创建时间:  2015-7-30 9:27
// 参数说明:  @sql为SELECT语句
//         :  @col为列号，从0开始
// 返 回 值:  SString/int/float
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
			LOGWARN("无法获取可用的数据库连接!sql=%s",sql.left(1000).data());
			goto err;
		}
		ret = pDB->SelectInto(sql,col);
		pThisPool->Release(pDB);
		if(pDB->GetStatus() != SDatabase::DBERROR)
		{
			if(i == 1)
			{
				m_pActionDatabasePool = pThisPool;
				LOGWARN("数据库已切换到%s数据库运行!",pThisPool==m_pDatabasePool?"主":"备用");
			}
			return ret;
		}
		
err://连接并连接池错误，尝试切换数据库
		pThisPool = GetSecondPool(pThisPool);
		if(pThisPool != NULL)
		{
			LOGWARN("尝试切换到%s数据库重新执行查询指令！",pThisPool==m_pDatabasePool?"主":"备用");
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
// 描    述:  执行SQL
// 作    者:  邵凯田
// 创建时间:  2015-7-30 9:29
// 参数说明:  @sql为更新SQL语句，如update/delete/truncate/drop/...
// 返 回 值:  成功返回true,失败返回false
//////////////////////////////////////////////////////////////////////////
bool SDatabaseOper::ExecuteSQL(SString sql)
{
	if(m_bDmlPublishThreadRuning && sql.left(18) != "/*ssp_no_publish*/")
	{
		if(m_sDmlSqlBuffered.count() < g_iMaxDmlBufferSize)
			m_sDmlSqlBuffered.append(new SString(sql));
		else
		{
			LOGERROR("DML发布的SQL语句由于数量过多溢出!");
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
			LOGWARN("无法获取可用的数据库连接!sql=%s",sql.left(1000).data());
			goto err;
		}
		ret = pDB->Execute(sql);
		pThisPool->Release(pDB);
		if(ret)
		{
			if(i == 1)
			{
				m_pActionDatabasePool = pThisPool;
				LOGWARN("数据库已切换到%s数据库运行!",pThisPool==m_pDatabasePool?"主":"备用");
			}
			AddHistoryOperSql(pThisPool,sql);
			m_Wait.post();
			return true;
		}
		else if(pDB->GetStatus() != SDatabase::DBERROR)
		{
			//数据库未异常，应该是SQL错误引起，而不是连接异常
			return false;
		}
		
		err://连接并连接池错误，尝试切换数据库
		pThisPool = GetSecondPool(pThisPool);
		if(pThisPool != NULL)
		{
			LOGWARN("尝试切换到%s数据库重新执行指令！",pThisPool==m_pDatabasePool?"主":"备用");
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  启动数据库同步服务
// 作    者:  邵凯田
// 创建时间:  2015-7-30 9:32
// 参数说明:  @bGlobalSync表示是否开启全局库同步，true表示打开，false表示关闭
//         :  一般情况下，一个系统中仅需要一个进程开启该同步，该进程应确保长期处于运行状态
// 返 回 值:  true/false
//////////////////////////////////////////////////////////////////////////
bool SDatabaseOper::Start(bool bGlobalSync)
{
	if(/*m_bQuit == false || */m_pSlaveDatabasePool == NULL)
	{
		if(m_pSlaveDatabasePool == NULL)
		{
			LOGWARN("备用数据库连接池未设置，启动数据库同步服务失败!");
		}
		return false;
	}
	m_bQuit = false;
	m_bGlobalSync = bGlobalSync;
	S_CREATE_THREAD(ThreadDbSync,this);
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  退出数据库同步服务
// 作    者:  邵凯田
// 创建时间:  2015-7-30 9:32
// 参数说明:  void
// 返 回 值:  true/false
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
// 描    述:  通过文件更新大字段内容
// 作    者:  邵凯田
// 创建时间:  2015-7-30 9:54
// 参数说明:  @sTable为待更新的表名
//         :  @sLobFields为大字段的字段名
//         :  @sWhere表示唯一确定更新行的条件，该条件必须过滤出唯一的一行记录
//         :  @sFile为本地文件名全路径，即待上传的大字段内容
// 返 回 值:  true表示成功，false表示失败
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
			LOGWARN("无法获取可用的数据库连接!");
			goto err;
		}
		ret = pDB->UpdateLobFromFile(sTable,sLobField,sWhere,sFile);
		pThisPool->Release(pDB);
		if(ret)
		{
			if(i == 1)
			{
				m_pActionDatabasePool = pThisPool;
				LOGWARN("数据库已切换到%s数据库运行!",pThisPool==m_pDatabasePool?"主":"备用");
			}
			SString sql;
			sql.sprintf("act=update_lob{$SEP$}table=%s{$SEP$}field=%s{$SEP$}where=%s{$SEP$}",sTable.data(),sLobField.data(),sWhere.data());
			AddHistoryOperSql(pThisPool,sql);
			m_Wait.post();
			return true;
		}
		else if(pDB->GetStatus() != SDatabase::DBERROR)
		{
			//数据库未异常，应该是SQL错误引起，而不是连接异常
			return false;
		}

err://连接并连接池错误，尝试切换数据库
		pThisPool = GetSecondPool(pThisPool);
		if(pThisPool != NULL)
		{
			LOGWARN("尝试切换到%s数据库重新执行指令！",pThisPool==m_pDatabasePool?"主":"备用");
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  从指定的大字段读取内容并输出到文件
// 作    者:  邵凯田
// 创建时间:  2015-7-30 9:58
// 参数说明:  @sTable为待更新的表名
//         :  @sLobFields为大字段的字段名
//         :  @sWhere表示唯一确定更新行的条件，该条件必须过滤出唯一的一行记录
//         :  @sFile为本地文件名全路径，读取到的内容将写入该文件
// 返 回 值:  true表示成功，false表示失败
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
			LOGWARN("无法获取可用的数据库连接!");
			goto err;
		}
		ret = pDB->ReadLobToFile(sTable,sLobField,sWhere,sFile);
		pThisPool->Release(pDB);
		if(ret)
		{
			if(i == 1)
			{
				m_pActionDatabasePool = pThisPool;
				LOGWARN("数据库已切换到%s数据库运行!",pThisPool==m_pDatabasePool?"主":"备用");
			}
			return true;
		}
		else if(pDB->GetStatus() != SDatabase::DBERROR)
		{
			//数据库未异常，应该是SQL错误引起，而不是连接异常
			return false;
		}

err://连接并连接池错误，尝试切换数据库
		pThisPool = GetSecondPool(pThisPool);
		if(pThisPool != NULL)
		{
			LOGWARN("尝试切换到%s数据库重新执行指令！",pThisPool==m_pDatabasePool?"主":"备用");
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  从指定的大字段读取内容并输出到文件
// 作    者:  邵凯田
// 创建时间:  2015-8-4 14:31
// 参数说明:  @sTable为待更新的表名
//         :  @sLobFields为大字段的字段名
//         :  @sWhere表示唯一确定更新行的条件，该条件必须过滤出唯一的一行记录
//         :  @pBuf为
// 返 回 值:  true表示成功，false表示失败
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
			LOGWARN("无法获取可用的数据库连接!");
			goto err;
		}
		ret = pDB->ReadLobToMem(sTable,sLobField,sWhere,pBuf,iBufLen);
		pThisPool->Release(pDB);
		if(ret)
		{
			if(i == 1)
			{
				m_pActionDatabasePool = pThisPool;
				LOGWARN("数据库已切换到%s数据库运行!",pThisPool==m_pDatabasePool?"主":"备用");
			}			
			return true;
		}
		else if(pDB->GetStatus() != SDatabase::DBERROR)
		{
			//数据库未异常，应该是SQL错误引起，而不是连接异常
			return false;
		}

err://连接并连接池错误，尝试切换数据库
		pThisPool = GetSecondPool(pThisPool);
		if(pThisPool != NULL)
		{
			LOGWARN("尝试切换到%s数据库重新执行指令！",pThisPool==m_pDatabasePool?"主":"备用");
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  用于释放ReadLobToMem函数产生的缓冲区
// 作    者:  邵凯田
// 创建时间:  2017-12-1 11:01
// 参数说明:  @pBuf为缓冲区指针
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SDatabaseOper::FreeLobMem(unsigned char* pBuf)
{
	SDatabase *p = m_pActionDatabasePool->GetDatabaseByIdx(0);
	if(p)
		p->FreeLobMem(pBuf);
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  将内存大字段写入数据库
// 作    者:  邵凯田
// 创建时间:  2015-8-29 17:43
// 参数说明:  
// 返 回 值:  true/false
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
			LOGWARN("无法获取可用的数据库连接!");
			goto err;
		}
		ret = pDB->UpdateLobFromMem(sTable,sLobField,sWhere,pBuf,iBufLen);
		pThisPool->Release(pDB);
		if(ret)
		{
			if(i == 1)
			{
				m_pActionDatabasePool = pThisPool;
				LOGWARN("数据库已切换到%s数据库运行!",pThisPool==m_pDatabasePool?"主":"备用");
			}
			SString sql;
			sql.sprintf("act=update_lob{$SEP$}table=%s{$SEP$}field=%s{$SEP$}where=%s{$SEP$}",sTable.data(),sLobField.data(),sWhere.data());
			AddHistoryOperSql(pThisPool,sql);
			m_Wait.post();
			return true;
		}
		else if(pDB->GetStatus() != SDatabase::DBERROR)
		{
			//数据库未异常，应该是SQL错误引起，而不是连接异常
			return false;
		}

err://连接并连接池错误，尝试切换数据库
		pThisPool = GetSecondPool(pThisPool);
		if(pThisPool != NULL)
		{
			LOGWARN("尝试切换到%s数据库重新执行指令！",pThisPool==m_pDatabasePool?"主":"备用");
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  数据库同步线程
// 作    者:  邵凯田
// 创建时间:  2015-7-30 9:31
// 参数说明:  @lp为this
// 返 回 值:  NULL
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
		//重连数据库
		pThisPool = pThis->m_pActionDatabasePool;
		soc = (int)SDateTime::getNowSoc();
		if(abs(soc - last_soc0) > 1)
		{
			last_soc0 = soc;
			if(pThisPool != pThis->m_pDatabasePool)
			{
				//判断主数据库是否恢复连接正常?
				pThisPool = pThis->m_pDatabasePool;
				pDb = pThisPool->GetDatabase(true);
				if(pDb != NULL)
				{
					pThisPool->Release(pDb);
					LOGWARN("主数据库连接恢复，切换回主数据库运行!");
					pThis->m_pActionDatabasePool = pThis->m_pDatabasePool;
				}
			}
			if(pThisPool != pThis->m_pSlaveDatabasePool && pThis->m_pSlaveDatabasePool != NULL)
			{
				//判断主数据库是否恢复连接正常?
				pThisPool = pThis->m_pSlaveDatabasePool;
				pDb = pThisPool->GetDatabase(true);
				if(pDb != NULL)
				{
					pThisPool->Release(pDb);
				}
			}
		}
		//全局同步
		if(pThis->m_bGlobalSync)
		{
			soc = (int)SDateTime::getNowSoc();
			if(times<5 || abs(soc - last_soc) >= 5)//5秒执行一次全局同步,即从数据库缓存中同步
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
								LOGERROR("同步%s数据库时失败!sql=%s",pThatPool==pThis->m_pDatabasePool?"主":"备",sql.left(1000).data());
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

		//执行历史同步
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
							//数据库连接错误，写入另一连接池的SQL缓存
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
								LOGERROR("主库无法同步写入从库SQL：%s",pSql->data());
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
							//数据库连接错误，写入另一连接池的SQL缓存
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
								LOGERROR("从库无法同步写入主库SQL：%s",pSql->data());
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
// 描    述:  开启DML语句的实时发布
// 作    者:  邵凯田
// 创建时间:  2016-8-11 17:16
// 参数说明:  @sIp表示发布目标服务器的IP
//         :  @iTcpPort表示发布目标服务的TCP端口号
// 返 回 值:  true表示成功，false表示失败
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
// 描    述:  设备DML发布的最大缓冲数量,缺省500000
// 作    者:  邵凯田
// 创建时间:  2019-4-23 19:23
// 参数说明:  @max为最大缓冲SQL的数量,0表示不限制数量
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SDatabaseOper::SetDmlPublishMaxBufferSize(int max)
{
	g_iMaxDmlBufferSize = max;
}


//////////////////////////////////////////////////////////////////////////
// 描    述:  设备DML发布数据包的大小，缺省65536
// 作    者:  邵凯田
// 创建时间:  2019-4-23 19:30
// 参数说明:  @bytes为单次最大发送的数据包字节大小
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SDatabaseOper::SetDmlPublishPackageSize(int bytes)
{
	g_iMaxDmlPackageBytes = bytes;
}


//////////////////////////////////////////////////////////////////////////
// 描    述:  保存所有的历史DML缓冲记录到数据库
// 作    者:  邵凯田
// 创建时间:  2019-4-24 0:04
// 参数说明:  void
// 返 回 值:  void
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
// 描    述:  DML语句的发布进程
// 作    者:  邵凯田
// 创建时间:  2016-8-11 17:12
// 参数说明:  @lp为this
// 返 回 值:  NULL
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
			//将全部缓存SQL记录保存到数据库进行持久化
			pDb = pPool->GetDatabase();
			if(pDb != NULL)
			{
				pThis->SaveHisDmlSqls(pParam->pub_dml_sqls,pDb);
				pPool->Release(pDb);
			}
			SApi::UsSleep(100000);
			continue;
		}
		//从数据库加载全部历史缓冲SQL
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
				//订阅端返回的finish_cnt表示成功执行的数量
				int cnt = SString::GetAttributeValueI(pRecvPackage->m_sHead,"finish_cnt");
				//删除cnt对应前cnt条SQL记录
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
// 描    述:  添加历史操作SQL记录到指定连接池对应的另一个连接池，当另一个池为空时忽略此操作
// 作    者:  邵凯田
// 创建时间:  2015-7-30 10:50
// 参数说明:  @pThisPool表示当前池
//         :  @sql为更新SQL
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SDatabaseOper::AddHistoryOperSql(SDatabasePool<SDatabase> *pThisPool,SString &sql)
{
	SDatabasePool<SDatabase> *pSecPool = GetSecondPool(pThisPool);
	if(pSecPool == NULL)
		return;
	(pSecPool==m_pDatabasePool?m_DbSyncSqls:m_SlaveDbSyncSqls).append(sql);
}
