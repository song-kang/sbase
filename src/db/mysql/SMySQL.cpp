/**
*
* 文 件 名 : SMySQL.cpp
* 创建日期 : 2007-10-9 14:10
* 作    者 : 邵凯田(skt001@163.com)
* 修改日期 : $Date: 2008/09/02 01:50:09 $
* 当前版本 : $Revision: 1.1 $
* 功能描述 : MySQL数据库对SDatabase接口类的实现
* 修改记录 : 
*            $Log: SMySQL.cpp,v $
*            Revision 1.1  2008/09/02 01:50:09  shaokt
*            *** empty log message ***
*
*            Revision 1.3  2007/10/29 07:30:23  shaokt
*            添加对象地址是否为4的倍数的检测函数，为检测ARM下由于pack(1)造成的内存问题
*
*            Revision 1.2  2007/10/10 13:28:58  shaokt
*            *** empty log message ***
*
*            Revision 1.1  2007/10/09 11:59:45  shaokt
*            MYSQL数据库操作接口
*
*
**/


#include "SMySQL.h"

/////////////////////////////////////////////////////////////////////
// Construction/Destruction
/////////////////////////////////////////////////////////////////////
SMySQL::SMySQL()
{
	SKTOBJECT("SMySQL");
	m_pConn = NULL;
	m_iTestTimes = 0;
	m_tLastSQLTime = 0;
	m_dbType = DB_MYSQL;
}

SMySQL::~SMySQL()
{
}

bool SMySQL::Connect()
{
	if(m_pConn != NULL)
		DisConnect();

	m_pConn = mysql_init(NULL);
	if(m_pConn == NULL)
	{
		LOGERROR("Couldn't initialize mysql struct!");
		return false;
	}

	/*
	MYSQL *		STDCALL mysql_real_connect(MYSQL *mysql, const char *host,
	const char *user,
	const char *passwd,
	const char *db,
	unsigned int port,
	const char *unix_socket,
	unsigned long clientflag);
	*/
	SString sParam = GetParams();//hostaddr=127.0.0.1;port=5432;dbname=znl4;user=postgres;password=123456;
	if(mysql_real_connect(m_pConn,
		SString::GetAttributeValue(sParam,"hostaddr").data(),
		SString::GetAttributeValue(sParam,"user").data(),
		SString::GetAttributeValue(sParam,"password").data(),
		SString::GetAttributeValue(sParam,"dbname").data(),
		SString::GetAttributeValue(sParam,"port").toInt(),
		NULL,0) == NULL)
	{
		LOGERROR("Couldn't connect to engine!error:%s",mysql_error(m_pConn));
		DisConnect();
		return false;
	}
	m_pConn->reconnect = 1;

	LOGDEBUG("Connect to MySQL OK hostaddr=%s",SString::GetAttributeValue(sParam,"hostaddr").data());
	SetEncoding();
	SetStatus(IDLE);
	return true;
}

void SMySQL::SetEncoding()
{
	//自动设置为GB编码
	int now = (int)::time(NULL);
	if(abs(now - m_tLastSQLTime) > 30)
	{
		m_tLastSQLTime = now;
		Execute("set names gb2312");
	}
	else
		m_tLastSQLTime = now;
}

bool SMySQL::DisConnect()
{
	if(m_pConn != NULL)
	{
		mysql_close(m_pConn);
		m_pConn = NULL;
	}
	m_tLastSQLTime = 0;
	SetStatus(UNCONNECT);
	return true;
}

bool SMySQL::TestConnect()
{
	if(m_iTestTimes>0)
	{
		SetStatus(DBERROR);
		return false;
	}
	m_iTestTimes ++;
	if(SelectInto("select 'skt'") != "skt")
	{
		SetStatus(DBERROR);
		m_iTestTimes  = 0;
		return false;
	}
	SetStatus(IDLE);
	m_iTestTimes  = 0;
	return true;
}

int SMySQL::Retrieve(SString sql,SRecordset &rs)
{
	SetEncoding();
	LOGDEBUG("into SMySQL::Retrieve(%s)",sql.data());
	if(m_pConn == NULL)
	{
		LOGWARN("m_pConn is NULL is SMySQL::Retrieve, Connect it at first!");
		Connect();
		if(m_pConn == NULL)
		{
			LOGWARN("m_pConn is NULL is SMySQL::Retrieve, Connect error!");
			return -1;
		}
	}
	//Zero if the query was successful. Non-zero if an error occurred. 
	int ret = mysql_query(m_pConn,sql.data());
	if(ret != 0)
	{
		LOGWARN("Retrieve mysql_query error:%s",mysql_error(m_pConn));
		if(TestConnect() == true)//连接可用
			return -1;
		//连接不可用自动重连一次数据库
		if(!Reconnect())
			return -2;//连接失败
		ret = mysql_query(m_pConn,sql.data());
		if(ret != 0)
		{
			LOGERROR("Retrieve mysql_query error:%s",mysql_error(m_pConn));
			return -1;
		}
	}

	MYSQL_RES *pRes;
	if (!(pRes=mysql_store_result(m_pConn)))
	{
		LOGERROR("Couldn't get result from %s",mysql_error(m_pConn));
		return -3;
	}

	int cols = (int)mysql_num_fields(pRes);
	int rows = (int)mysql_num_rows(pRes);
	int i,j;
	SRecord *pRecord;
	//写入记录集
	rs.clear();
	rs.SetSize(rows,cols);
	for(i=0;i<cols;i++)
	{
		MYSQL_FIELD	*field = mysql_fetch_field_direct(pRes,i);
		if(field != NULL)
			rs.SetColumnName(i,field->name);
	}
	MYSQL_ROW row;
	row= mysql_fetch_row(pRes);
	for(i=0;row != NULL && i<rows;i++)
	{
		pRecord = rs.GetRecord(i);
		for(j=0;j<cols;j++)
		{
			pRecord->SetValue(j,row[j]);
		}
		row= mysql_fetch_row(pRes);
	}
	mysql_free_result(pRes);

	return rows;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  开始一个新的事务
// 作    者:  邵凯田
// 创建时间:  2017-8-24 15:55
// 参数说明:  void
// 返 回 值:  true/false
//////////////////////////////////////////////////////////////////////////
bool SMySQL::BeginTrans()
{
	if(m_pConn == NULL)
		return false;
	return mysql_autocommit(m_pConn,0) != 0;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  提交当前事务，并退出事务模式
// 作    者:  邵凯田
// 创建时间:  2017-8-24 15:55
// 参数说明:  void
// 返 回 值:  true/false
//////////////////////////////////////////////////////////////////////////
bool SMySQL::Commit()
{
	if(m_pConn == NULL)
		return false;
	my_bool ret = mysql_commit(m_pConn);
	mysql_autocommit(m_pConn,1);
	return ret != 0;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  回滚当前事务，并退出事务模式
// 作    者:  邵凯田
// 创建时间:  2017-8-24 15:56
// 参数说明:  void
// 返 回 值:  true/false
//////////////////////////////////////////////////////////////////////////
bool SMySQL::Rollback()
{
	if(m_pConn == NULL)
		return false;
	my_bool ret = mysql_rollback(m_pConn);
	mysql_autocommit(m_pConn,1);
	return ret != 0;
}

bool SMySQL::Execute(SString sql)
{
	SetEncoding();
	LOGDEBUG("into SMySQL::Execute(%s)",sql.left(1000).data());
	if(m_pConn == NULL)
	{
		LOGWARN("m_pConn is NULL is SMySQL::Execute, Connect it at first!");
		Connect();
		if(m_pConn == NULL)
		{
			LOGWARN("m_pConn is NULL is SMySQL::Execute, Connect error!");
			return false;
		}
	}
	sql.replace("\\","\\\\");
	int ret = mysql_query(m_pConn,sql.data());
	if(ret != 0)
	{
		LOGWARN("Execute mysql_query error:%d,%s, sql:%s",mysql_errno(m_pConn),mysql_error(m_pConn),sql.left(1000).data());
		if(TestConnect() == true)//连接可用
			return false;
		//连接不可用自动重连一次数据库
		if(!Reconnect())
			return false;//连接失败
		ret = mysql_query(m_pConn,sql.data());
		if(ret != 0)
		{
			LOGERROR("Execute mysql_query error:%s",mysql_error(m_pConn));
			return false;
		}
	}
	//MEMO: 如果影响行数为0，认为是失败，但不生成错误日志 [2015-11-5 20:14 邵凯田]
	//if(mysql_affected_rows(m_pConn) == 0)
	//	return false;
	return true;
}

//update wave_detail_rpt set efile=%d where 
bool SMySQL::UpdateLobFromFile(SString sTable,SString sLobField,SString sWhere,SString sFile)
{
	SetEncoding();
	LOGDEBUG("Into SMySQL::UpdateLobFromFile(%s,%s,%s,%s)",
		sTable.data(),sLobField.data(),sWhere.data(),sFile.data());
	SString sql;
	SString param;
	if(m_pConn == NULL)
	{
		LOGWARN("m_pConn is NULL is SMySQL::Retrieve, Connect it at first!");
		Connect();
		if(m_pConn == NULL)
		{
			LOGWARN("m_pConn is NULL is SMySQL::Retrieve, Connect error!");
			return false;
		}
	}

	sql.sprintf("update %s set %s=? where %s",sTable.data(),sLobField.data(),sWhere.data());
	//sql.sprintf("select count(1) from (select f.id from t1 f where f.id=?) as x");
	MYSQL_BIND bind[1];
	MYSQL_STMT *stmt= mysql_stmt_init(m_pConn);
	int ret = mysql_stmt_prepare(stmt, sql.data(), sql.length());
	if (stmt == NULL || ret != 0)
	{
		mysql_stmt_close(stmt);
		LOGERROR("UpdateLobFromFile mysql_stmt_prepare error:%s",mysql_error(m_pConn));
		return false;
	}
	memset((char*) bind,0, sizeof(bind));
	SFile file(sFile);
	if(!file.open(IO_ReadOnly))
	{
		mysql_stmt_close(stmt);
		LOGERROR("ERROR in SMySQL::UpdateLobFromFile(%s,%s,%s,%s) , file(%s) not exist!",
			sTable.data(),sLobField.data(),sWhere.data(),sFile.data(),sFile.data());
		return false;
	}

	int iFileSize = file.size();
	BYTE *pBuffer = new BYTE[iFileSize];
	file.readBlock(pBuffer,iFileSize);
	file.close();
	int rc;
	/*BLOB*/
	bind[0].buffer_type= MYSQL_TYPE_BLOB;
	bind[0].buffer= (void *)pBuffer;
	bind[0].buffer_length = iFileSize;
	//long param_count= mysql_stmt_param_count(stmt);
	rc = mysql_stmt_bind_param(stmt, bind);
	if(rc != 0)
	{
		LOGERROR("UpdateLobFromFile mysql_stmt_bind_param error:%s",mysql_error(m_pConn));
		delete[] pBuffer;
		return false;
	}
	rc = mysql_stmt_execute(stmt);
	if(rc != 0)
	{
		LOGERROR("UpdateLobFromFile mysql_stmt_execute error:%s",mysql_error(m_pConn));
		delete[] pBuffer;
		return false;
	}
	mysql_stmt_close(stmt);
	delete[] pBuffer;
	return true;
}

bool SMySQL::ReadLobToFile(SString sTable,SString sLobField,SString sWhere,SString sFile)
{
	SetEncoding();
	LOGDEBUG("Into SMySQL::ReadLobToFile(%s,%s,%s,%s)",
		sTable.data(),sLobField.data(),sWhere.data(),sFile.data());
	SString sql;
	sql.sprintf("select %s from %s where %s",sLobField.data(),sTable.data(),sWhere.data());
	//Zero if the query was successful. Non-zero if an error occurred. 
	int ret = mysql_query(m_pConn,sql.data());
	if(ret != 0)
	{
		LOGWARN("ReadLobToFile mysql_query error:%s",mysql_error(m_pConn));
		if(TestConnect() == true)//连接可用
			return false;
		//连接不可用自动重连一次数据库
		if(!Reconnect())
			return false;//连接失败
		ret = mysql_query(m_pConn,sql.data());
		if(ret != 0)
		{
			LOGERROR("ReadLobToFile mysql_query error:%s",mysql_error(m_pConn));
			return false;
		}
	}

	MYSQL_RES *pRes;
	if (!(pRes=mysql_store_result(m_pConn)))
	{
		LOGERROR("Couldn't get result from %s",mysql_error(m_pConn));
		return false;
	}

	int cols = (int)mysql_num_fields(pRes);
	int rows = (int)mysql_num_rows(pRes);

	if(rows <= 0 || cols <= 0)
	{
		mysql_free_result(pRes);
		LOGERROR("Couldn't get result from %s",mysql_error(m_pConn));
		return false;
	}
	MYSQL_FIELD	*field = mysql_fetch_field_direct(pRes,0);
	if(field == NULL)
	{
		mysql_free_result(pRes);
		LOGERROR("Couldn't get result from %s",mysql_error(m_pConn));
		return false;
	}
	SFile file(sFile);
	if(!file.open(IO_Truncate))
	{
		mysql_free_result(pRes);
		LOGERROR("Into SMySQL::ReadLobToFile(%s,%s,%s,%s) Create file error!",
			sTable.data(),sLobField.data(),sWhere.data(),sFile.data());
		return false;
	}
	MYSQL_ROW row;
	row= mysql_fetch_row(pRes);
	if(row == NULL)
	{
		file.close();
		mysql_free_result(pRes);
		LOGERROR("Couldn't get result from %s",mysql_error(m_pConn));
		return false;
	}
	unsigned long len,*plengths = mysql_fetch_lengths(pRes);
	if(plengths == NULL)
		len = 0;
	else
		len = plengths[0];

	file.writeBlock((BYTE*)row[0],(unsigned int)len);
	file.close();

	mysql_free_result(pRes);

	return true;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  从数据库读取指定的大字段内容到缓冲区，缓冲区内容由调用者负责释放
// 作    者:  邵凯田
// 创建时间:  2015-8-4 14:15
// 参数说明:  
// 返 回 值:  true/false
//////////////////////////////////////////////////////////////////////////
bool SMySQL::ReadLobToMem(SString sTable,SString sLobField,SString sWhere,unsigned char* &pBuf,int &iBufLen)
{
	SetEncoding();
	LOGDEBUG("Into SMySQL::ReadLobToFile(%s,%s,%s)",
		sTable.data(),sLobField.data(),sWhere.data());
	SString sql;
	pBuf = NULL;
	iBufLen = 0;
	sql.sprintf("select %s from %s where %s",sLobField.data(),sTable.data(),sWhere.data());
	//Zero if the query was successful. Non-zero if an error occurred. 
	int ret = mysql_query(m_pConn,sql.data());
	if(ret != 0)
	{
		LOGWARN("ReadLobToFile mysql_query error:%s",mysql_error(m_pConn));
		if(TestConnect() == true)//连接可用
			return false;
		//连接不可用自动重连一次数据库
		if(!Reconnect())
			return false;//连接失败
		ret = mysql_query(m_pConn,sql.data());
		if(ret != 0)
		{
			LOGERROR("ReadLobToFile mysql_query error:%s",mysql_error(m_pConn));
			return false;
		}
	}

	MYSQL_RES *pRes;
	if (!(pRes=mysql_store_result(m_pConn)))
	{
		LOGERROR("Couldn't get result from %s",mysql_error(m_pConn));
		return false;
	}

	int cols = (int)mysql_num_fields(pRes);
	int rows = (int)mysql_num_rows(pRes);

	if(rows <= 0 || cols <= 0)
	{
		mysql_free_result(pRes);
		LOGERROR("Couldn't get result from %s",mysql_error(m_pConn));
		return false;
	}
	MYSQL_FIELD	*field = mysql_fetch_field_direct(pRes,0);
	if(field == NULL)
	{
		mysql_free_result(pRes);
		LOGERROR("Couldn't get result from %s",mysql_error(m_pConn));
		return false;
	}
	MYSQL_ROW row;
	row= mysql_fetch_row(pRes);
	if(row == NULL)
	{
		mysql_free_result(pRes);
		LOGERROR("Couldn't get result from %s",mysql_error(m_pConn));
		return false;
	}
	unsigned long /*len,*/*plengths = mysql_fetch_lengths(pRes);
	if(plengths == NULL || plengths[0] <= 0)
		return true;
	iBufLen = plengths[0];
	pBuf = new unsigned char[iBufLen+1];
	memcpy(pBuf,(BYTE*)row[0],(unsigned int)iBufLen);
	pBuf[iBufLen] = '\0';
	
	mysql_free_result(pRes);
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  用于释放ReadLobToMem函数产生的缓冲区
// 作    者:  邵凯田
// 创建时间:  2017-12-1 11:01
// 参数说明:  @pBuf为缓冲区指针
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SMySQL::FreeLobMem(unsigned char* pBuf)
{
	if(pBuf)
		delete[] pBuf;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  将内存大字段写入数据库
// 作    者:  邵凯田
// 创建时间:  2015-8-29 17:43
// 参数说明:  
// 返 回 值:  true/false
//////////////////////////////////////////////////////////////////////////
bool SMySQL::UpdateLobFromMem(SString sTable,SString sLobField,SString sWhere,unsigned char* pBuf,int iBufLen)
{
	SetEncoding();
	LOGDEBUG("Into SMySQL::UpdateLobFromMem(%s,%s,%s)",
		sTable.data(),sLobField.data(),sWhere.data());
	SString sql;
	SString param;
	if(m_pConn == NULL)
	{
		LOGWARN("m_pConn is NULL is SMySQL::Retrieve, Connect it at first!");
		Connect();
		if(m_pConn == NULL)
		{
			LOGWARN("m_pConn is NULL is SMySQL::Retrieve, Connect error!");
			return false;
		}
	}

	sql.sprintf("update %s set %s=? where %s",sTable.data(),sLobField.data(),sWhere.data());
	//sql.sprintf("select count(1) from (select f.id from t1 f where f.id=?) as x");
	MYSQL_BIND bind[1];
	MYSQL_STMT *stmt= mysql_stmt_init(m_pConn);
	int ret = mysql_stmt_prepare(stmt, sql.data(), sql.length());
	if (stmt == NULL || ret != 0)
	{
		mysql_stmt_close(stmt);
		LOGERROR("UpdateLobFromFile mysql_stmt_prepare error:%s",mysql_error(m_pConn));
		return false;
	}
	memset((char*) bind,0, sizeof(bind));
	int iFileSize = iBufLen;
	BYTE *pBuffer = pBuf;
	int rc;
	/*BLOB*/
	bind[0].buffer_type= MYSQL_TYPE_BLOB;
	bind[0].buffer= (void *)pBuffer;
	bind[0].buffer_length = iFileSize;
	//long param_count= mysql_stmt_param_count(stmt);
	rc = mysql_stmt_bind_param(stmt, bind);
	if(rc != 0)
	{
		LOGERROR("UpdateLobFromFile mysql_stmt_bind_param error:%s",mysql_error(m_pConn));
		return false;
	}
	rc = mysql_stmt_execute(stmt);
	if(rc != 0)
	{
		LOGERROR("UpdateLobFromFile mysql_stmt_execute error:%s",mysql_error(m_pConn));
		return false;
	}
	mysql_stmt_close(stmt);

	return true;
}
