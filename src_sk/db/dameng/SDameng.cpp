/**
 *
 * 文 件 名 : SDameng.cpp
 * 创建日期 : 2007-11-4
 * 作    者 : 邵凯田
 * 修改日期 : $Date: 2007/11/24 07:30:23 $
 * 当前版本 : $Revision: 1.8 $
 * 功能描述 : 达梦数据库对SDatabase接口类的实现
 * 修改记录 : 
 *            Revision 1.1  2007/08/15 03:09:22  shaokt
 *            *** empty log message ***
 *
 **/

#include "SDameng.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SDameng::SDameng()
{
	m_henv = NULL;
	m_hcon = NULL;
	m_iTestTimes = 0;
}

SDameng::~SDameng()
{
	DisConnect();
}

/****************************************************** 
Notes: 
错误信息获取打印 
Param: 
hndl_type: 句柄类型 
hndl: 句柄 
Return: 
无 
*******************************************************/ 
void 
dpi_err_msg_print(sdint2 hndl_type, dhandle hndl) 
{ 
	sdint4 err_code; 
	sdint2 msg_len; 
	sdbyte err_msg[SDBYTE_MAX]; 
	/* 获取错误信息字段 */ 
	/* dpi_get_diag_field(hndl_type, hndl, 1, DSQL_DIAG_MESSAGE_TEXT, err_msg, sizeof(err_msg), 
	NULL); 
	printf("err_msg = %s\n", err_msg);*/ 
	/* 获取错误信息集合 */ 
	memset(err_msg,0,sizeof(err_msg));
	dpi_get_diag_rec(hndl_type, hndl, 1, &err_code, err_msg, sizeof(err_msg), &msg_len); 
	printf("err_msg = %s, err_code = %d\n", err_msg, err_code); 
} 

bool SDameng::Connect()
{
	SString sParam = GetParams();//server=127.0.0.1;user=postgres;password=123456;

	SString user = SString::GetAttributeValue(sParam,"user");
	SString password = SString::GetAttributeValue(sParam,"password");
	SString server = SString::GetAttributeValue(sParam,"server");
 	DPIRETURN rt;
	LOGDEBUG("Begin to Connect Dameng:%s",sParam.data());

	//dm_api_init();
	/* 申请环境句柄 */ 
	rt = dpi_alloc_env(&m_henv); 
	if(!DSQL_SUCCEEDED(rt))
	{
		dpi_err_msg_print(DSQL_HANDLE_ENV, m_henv);
		return false;
	}

	/* 申请连接句柄 */ 
	rt = dpi_alloc_con(m_henv, &m_hcon); 
	if(!DSQL_SUCCEEDED(rt))
	{
		dpi_err_msg_print(DSQL_HANDLE_DBC, m_hcon);
		return false;
	}
	/* 连接数据库服务器 */ 
	rt = dpi_login(m_hcon,(sdbyte*) server.data(),(sdbyte*) user.data(),(sdbyte*) password.data());
	if(!DSQL_SUCCEEDED(rt))
	{
		dpi_err_msg_print(DSQL_HANDLE_DBC, m_hcon);
		return false;
	}
	/* 设置连接句柄属性，关闭自动提交功能 */ 
//	SQLSetConnectAttr(m_hcon, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_ON, 

	LOGDEBUG("Connect to Dameng OK params:%s",sParam.data());	
	return true;
}

bool SDameng::DisConnect()
{
	DPIRETURN rt;
	if(m_hcon != NULL)
	{
		rt = dpi_logout(m_hcon); 
		if(!DSQL_SUCCEEDED(rt))
		{
			dpi_err_msg_print(DSQL_HANDLE_DBC, m_hcon);
			return false;
		}
		/* 释放连接句柄和环境句柄 */ 
		rt = dpi_free_con(m_hcon); 
		if(!DSQL_SUCCEEDED(rt))
		{
			dpi_err_msg_print(DSQL_HANDLE_DBC, m_hcon);
			return false;
		}
	}
	if(m_henv != NULL)
	{
		rt = dpi_free_env(m_henv); 
		if(!DSQL_SUCCEEDED(rt))
		{
			dpi_err_msg_print(DSQL_HANDLE_ENV, m_henv);
			return false;
		}
	}
	return true;
}

bool SDameng::TestConnect() 
{
	//bConnect:true时，如果断了，自动重连
	/*
	OdbRecordSet rs; 
	long hStmt=0;
	string sSql="select 1 from SYS.user$";
    sword status = m_pConn->Select(sSql,rs,hStmt);
	if(status != 0) 
	{
		return Connect();
	}
	m_pConn->ClearRecordSets(hStmt,rs);
	*/
 	return true;
}



int SDameng::Retrieve(SString sql,SRecordset &rs)
{
	if(m_hcon == NULL)
		return -1;
	string sLastError;
	dhstmt hstmt=NULL; /* 语句句柄 */ 
	try
	{
		LOGBASEDEBUG("Retrieve:%s",sql.data());
		DPIRETURN rt;

		rs.clear();
		rt = dpi_alloc_stmt(m_hcon, &hstmt); 
		if(!DSQL_SUCCEEDED(rt))
		{
			dpi_err_msg_print(DSQL_HANDLE_DBC, m_hcon);
			return false;
		}
		ulength val = DSQL_CURSOR_DYNAMIC; 
		rt = dpi_set_stmt_attr(hstmt, DSQL_ATTR_CURSOR_TYPE, (dpointer)val, 0);
		if(!DSQL_SUCCEEDED(rt))
		{
			dpi_err_msg_print(DSQL_HANDLE_STMT, hstmt);
			return false;
		}

		/* 执行sql */ 
		rt = dpi_exec_direct(hstmt,(sdbyte*)sql.data());
		if(rt != DSQL_SUCCESS && rt != DSQL_SUCCESS_WITH_INFO)
		{
			dpi_free_stmt(hstmt); 
			dpi_err_msg_print(DSQL_HANDLE_STMT, hstmt);
			return false;
		}
		int i,row,rows=0;
		int len;
		sdint2 cols=0;
		int buf[16];
		ulength row_num;
		slength col_idx;
		rt = dpi_number_columns(hstmt,&cols);

		char* pbuffer = NULL;
		int block_size = 256;
		long cbphone = 0;

		rt = dpi_fetch_scroll(hstmt, DSQL_FETCH_FIRST, 0, &row_num);
		if(rt == DSQL_NO_DATA)
		{
			//无数据
			dpi_free_stmt(hstmt); 
			return 0;
		}
		if(!DSQL_SUCCEEDED(rt))
		{
			dpi_err_msg_print(DSQL_HANDLE_STMT, hstmt);
			return false;
		}
		while(rt != DSQL_NO_DATA) 
		{ 
			rows++;
			rt = dpi_fetch_scroll(hstmt, DSQL_FETCH_NEXT, 0, &row_num);
		} 
		if (cols > 0 && rows > 0)
		{
			pbuffer = new char[cols * block_size];
			memset(pbuffer, 0, cols * block_size);
			for (i = 0; i < cols; ++i)
			{
				rt = dpi_bind_col(hstmt, i+1, DSQL_C_NCHAR, pbuffer+(i*block_size), block_size, &col_idx);
			}
			rs.SetSize(rows,cols);

			row = 0;
			rt = dpi_fetch_scroll(hstmt, DSQL_FETCH_FIRST, 0, &row_num);
			while(rt != DSQL_NO_DATA) 
			{ 
				SRecord* oneRecord = rs.GetRecord(row);
				for(i=0;i<cols;i++)
				{
					oneRecord->SetValue(i, (char*)(pbuffer+(i*block_size)));
				}
				row++;
				rt = dpi_fetch_scroll(hstmt, DSQL_FETCH_NEXT, 0, &row_num);
			} 
			delete[] pbuffer;
		}

		rt = dpi_free_stmt(hstmt); 
		if(!DSQL_SUCCEEDED(rt))
		{
			dpi_err_msg_print(DSQL_HANDLE_STMT, hstmt);
			return false;
		}
	}
	catch(...)
	{
		if(hstmt != NULL)
			dpi_free_stmt(hstmt); 
		return false;
	}

	return rs.GetRows();
}

bool SDameng::Execute(SString sql)
{
	if(m_hcon == NULL)
		return false;
	string sLastError;
	try
	{
		LOGBASEDEBUG("Execute:%s",sql.data());
		DPIRETURN rt;
		dhstmt hstmt; /* 语句句柄 */ 

		rt = dpi_alloc_stmt(m_hcon, &hstmt); 
		if(!DSQL_SUCCEEDED(rt))
		{
			dpi_err_msg_print(DSQL_HANDLE_STMT, hstmt);
			return false;
		}
		/* 执行sql */ 
		rt = dpi_exec_direct(hstmt,(sdbyte*)sql.data());
		if(rt != DSQL_SUCCESS && rt != DSQL_SUCCESS_WITH_INFO)
		{
			dpi_err_msg_print(DSQL_HANDLE_STMT, hstmt);
// 			sLastError=m_pConn->GetErrorText();
// 			LOGDEBUG("SQL语句错误:%s,err:%s\n",sql.data(),sLastError.data());
			return false;
		}
		rt = dpi_free_stmt(hstmt); 
		if(!DSQL_SUCCEEDED(rt))
		{
			dpi_err_msg_print(DSQL_HANDLE_STMT, hstmt);
			return false;
		}
	}
	catch(...)
	{
// 		sLastError=m_pConn->GetErrorText();
// 		LOGDEBUG("SQL语句错误:%s,err:%s\n", sql.data(),sLastError.data());

		return false;
	}

	return true;
}

bool SDameng::UpdateLobFromFile(SString sTable,SString sLobField,SString sWhere,SString sFile) 
{

	return true;
}

bool SDameng::ReadLobToFile(SString sTable,SString sLobField,SString sWhere,SString ysFile)
{ 

    return true;
}

bool SDameng::UpdateLongRawFromFile(SString sTable,SString sLobField,SString sWhere,SString ysFile) 
{

	return false;
}

bool SDameng::ReadLongRawToFile(SString sTable,SString sLobField,SString sWhere,SString ysFile)
{ 
	return false;
}

bool SDameng::readLob(SString tableName,SString fieldName,SString whereField,SString recordIndex,SString &value)
{
	return false;

}

bool SDameng::updateLob(SString tableName,SString fieldName,SString whereField,SString recordIndex,SString &value)
{
	return false;

}

SString SDameng::getColumnType(SString tableName,SString fieldName)
{

	return "";


}

