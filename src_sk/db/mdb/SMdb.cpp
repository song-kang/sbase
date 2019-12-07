/**
 *
 * 文 件 名 : SMdb.cpp
 * 创建日期 : 2007-07-24
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: 2009/01/16 06:16:02 $
 * 当前版本 : $Revision: 1.2 $
 * 功能描述 : 内存数据库MDB对SDatabase接口类的实现
 * 修改记录 : 
 *            $Log: SMdb.cpp,v $
 *
 **/

#include "SMdb.h"

/////////////////////////////////////////////////////////////////////
// Construction/Destruction
/////////////////////////////////////////////////////////////////////
SMdb::SMdb()
{
	SKTOBJECT("SMdb");
	m_dbType = DB_MDB;
}

SMdb::~SMdb()
{
	DisConnect();
}

bool SMdb::Connect()
{
	if (m_MdbClient.IsConnected())
		return true;

	SString sParam = GetParams();//hostaddr=127.0.0.1;port=1400;user=skt_mdb;password=skt001@163.com;
	SString sTemp,sTempNoPwd;
	if(SString::GetAttributeValue(sParam,"hostaddr").length() == 0)
		return false;
	sTemp.sprintf("hostaddr=%s port=%s user=%s password=%s",
		SString::GetAttributeValue(sParam,"hostaddr").data(),
		SString::GetAttributeValue(sParam,"port").data(),
		SString::GetAttributeValue(sParam,"user").data(),
		SString::GetAttributeValue(sParam,"password").data());
	sTempNoPwd.sprintf("hostaddr=%s port=%s user=%s password=%s",
		SString::GetAttributeValue(sParam,"hostaddr").data(),
		SString::GetAttributeValue(sParam,"port").data(),
		SString::GetAttributeValue(sParam,"user").data(),
		"******");

	LOGDEBUG("Begin to Connect MDB:%s",sTempNoPwd.data());
	bool ret;
	ret = m_MdbClient.Connect(SString::GetAttributeValue(sParam, "hostaddr"),
		SString::GetAttributeValueI(sParam, "port"),
		SString::GetAttributeValue(sParam, "user"),
		SString::GetAttributeValue(sParam, "password"));
	
	if (!ret)
	{
		SetStatus(DBERROR);
		SString err;

		err.sprintf("Connect to MDB ERROR:%s params:%s", m_MdbClient.GetLastError().data(), sTempNoPwd.data());
		LOGERROR("%s",err.data());
		return false;
	}
	LOGDEBUG("Connect to MDB OK params:%s",sTempNoPwd.data());
	SetStatus(IDLE);
	return true;
}

bool SMdb::DisConnect()
{
	m_MdbClient.DisConnect();
	SetStatus(UNCONNECT);
	return true;
}

void SMdb::OnLogin()
{
	SetStatus(IDLE);
}

void SMdb::OnClose()
{
	SetStatus(DBERROR);
}

bool SMdb::TestConnect()
{
	if (m_MdbClient.IsConnected())
		return true;
	else
		return false;
	SetStatus(IDLE);
	return true;
}

int SMdb::Retrieve(SString sql,SRecordset &rs)
{
	sql.stripWhiteSpace();
	//LOGBASEDEBUG("into SMdb::Retrieve(%s)",sql.left(1000).data());
	if (m_MdbClient.IsConnected() == false)
	{
		m_MdbClient.SetLastError("连接已断开!");
		return -1;
	}
	SString sql_lower = sql.toLower();
	sql_lower.stripWhiteSpace();
	if (sql_lower.left(6) != "select")
	{
		m_MdbClient.SetLastError("语法错误!");
		return -12;
	}
	int p = sql_lower.find(" from ");
	if (p < 0)
	{
		m_MdbClient.SetLastError("语法错误!");
		return -2;
	}
	SString table = sql.mid(p + 6).trim();
	SString colnames = sql_lower.left(p).mid(7);
	colnames = colnames.replace((char*)" ", (char*)"");
	table = SString::GetIdAttribute(1, table, " ");
	SPtrList<stuTableField> fields;
	int i,rows,j, cols = m_MdbClient.GetTableFields(table.data(), fields);
	if (cols <= 0)
	{
		m_MdbClient.SetLastError("表名无效!");
		return -3;
	}
	int rs_cols = cols;
	if (colnames != "*")
		rs_cols = SString::GetAttributeCount(colnames, ",");
	void *pRowData = NULL;
	int iRowSize = 0;
	SRecord *pR;
	rows = m_MdbClient.Retrieve(sql, iRowSize, pRowData);
	if (rows < 0)
	{
		if (pRowData != NULL)
			delete[] (BYTE*)pRowData;
		return rows;
	}
	if (rows == 1 && pRowData == NULL)
	{
		//统计信息
		rs.SetSize(1, rs_cols);
		for (i = 1; i <= rs_cols; i++)
		{
			rs.SetColumnName(i - 1, SString::GetIdAttribute(i, colnames, ",").stripWhiteSpace());
			rs.SetValue(0, i-1, SString::GetIdAttribute(i, sql, "\t"));
		}
		return 1;
	}
	unsigned int oid;

	if (colnames == "*")
	{
		rs.SetSize(rows, cols);
		i = 0;
		unsigned long pos;
		BYTE *pRow;
		stuTableField *pF = fields.FetchFirst(pos);
		while (pF)
		{
			rs.SetColumnName(i++, pF->name);
			pF = fields.FetchNext(pos);
		}
		pRow = (BYTE*)pRowData;
		pF = fields.FetchFirst(pos);
		if((int)pF->bytes > (int)sizeof(oid))
		{
			LOGWARN("OID长度[%d]过长，可能导致数据无法准确同步!",pF->bytes);
			rs.SetOidBytes(sizeof(oid));
		}
		else
			rs.SetOidBytes(pF->bytes);
		
		for (i = 0; i < rows; i++)
		{
			oid = 0;
			memcpy(&oid,pRow,rs.GetOidBytes());
			pR = rs.GetRecord(i);
			pR->SetOid(oid);
			j = 0;
			pF = fields.FetchFirst(pos);
			while (pF)
			{
				pR->SetValue(j++, m_MdbClient.GetValueStr(pRow, pF));
				pF = fields.FetchNext(pos);
			}
			pRow += iRowSize;
		}
	}
	else
	{
		rs.SetSize(rows, rs_cols);
		i = 0;
		unsigned long pos;
		BYTE *pRow;
		SString* colname = new SString[rs_cols];
		stuTableField **pFlds = new stuTableField*[rs_cols];//数据集的列
		for (i = 0; i < rs_cols; i++)
		{
			colname[i] = SString::GetIdAttribute(i + 1, colnames, ",");
			if (colname[i].left(1) == "'" && colname[i].right(1) == "'")
				colname[i] = colname[i].mid(1,colname[i].length() - 2);
			pFlds[i] = NULL;
			stuTableField *pF = fields.FetchFirst(pos);
			while (pF)
			{
				if (strcmp(pF->name, colname[i].data()) == 0)
					pFlds[i] = pF;
				pF = fields.FetchNext(pos);
			}			
			rs.SetColumnName(i, SString::GetIdAttribute(i+1,colnames, ","));
		}
		pRow = (BYTE*)pRowData;
		stuTableField *pF = fields.FetchFirst(pos);
		if((int)pF->bytes > (int)sizeof(oid))
		{
			LOGWARN("OID长度[%d]过长，可能导致数据无法准确同步!",pF->bytes);
			rs.SetOidBytes(sizeof(oid));
		}
		else
			rs.SetOidBytes(pF->bytes);
		for (i = 0; i < rows; i++)
		{
			j = 0;
			oid = 0;
			memcpy(&oid,pRow,rs.GetOidBytes());
			pR = rs.GetRecord(i);
			pR->SetOid(oid);
			for (j = 0; j < rs_cols; j++)
			{
				if (pFlds[j] == NULL)
				{
					pR->SetValue(j, colname[j]);
				}
				else
				{
					pR->SetValue(j, m_MdbClient.GetValueStr(pRow, pFlds[j]));
				}
			}
			pRow += iRowSize;
		}
		delete[] colname;
		delete[] pFlds;
	}

	if (pRowData != NULL)
		delete[] (BYTE*)pRowData;
	return rows;
}

bool SMdb::Execute(SString sql)
{
	//LOGBASEDEBUG("into SMdb::Execute(%s)",sql.left(1000).data());
	if (m_MdbClient.IsConnected() == false)
	{
		m_MdbClient.SetLastError("连接已断开!");
		return false;
	}
	int ret = m_MdbClient.Execute(sql);
	//MEMO: ==0表示影响行数为0，也认为是失败 [2015-11-6 10:06 邵凯田]
	if (ret < 0)
		return false;
	return true;
}

SString SMdb::GetLastError()
{
	return m_MdbClient.GetLastError();
}

//update wave_detail_rpt set efile=%d where 
bool SMdb::UpdateLobFromFile(SString sTable,SString sLobField,SString sWhere,SString sFile)
{
	LOGBASEDEBUG("Into SMdb::UpdateLobFromFile(%s,%s,%s,%s)",
						sTable.data(),sLobField.data(),sWhere.data(),sFile.data());
	return m_MdbClient.UpdateBlobFromFile(sTable, sLobField, sWhere, sFile) > 0;
}

bool SMdb::ReadLobToFile(SString sTable,SString sLobField,SString sWhere,SString sFile)
{
	LOGBASEDEBUG("Into SMdb::ReadLobToFile(%s,%s,%s,%s)",
						sTable.data(),sLobField.data(),sWhere.data(),sFile.data());
	return m_MdbClient.ReadBlobToFile(sTable, sLobField, sWhere, sFile) > 0;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  从数据库读取指定的大字段内容到缓冲区，缓冲区内容由调用都负责释放
// 作    者:  邵凯田
// 创建时间:  2015-8-4 14:15
// 参数说明:  
// 返 回 值:  true/false
//////////////////////////////////////////////////////////////////////////
bool SMdb::ReadLobToMem(SString sTable,SString sLobField,SString sWhere,unsigned char* &pBuf,int &iBufLen)
{
	LOGBASEDEBUG("Into SMdb::ReadLobToMem(%s,%s,%s)",
		sTable.data(),sLobField.data(),sWhere.data());
	return m_MdbClient.ReadBlobToMem(sTable, sLobField, sWhere, pBuf,iBufLen) > 0;

}

//////////////////////////////////////////////////////////////////////////
// 描    述:  用于释放ReadLobToMem函数产生的缓冲区
// 作    者:  邵凯田
// 创建时间:  2017-12-1 11:01
// 参数说明:  @pBuf为缓冲区指针
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SMdb::FreeLobMem(unsigned char* pBuf)
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
bool SMdb::UpdateLobFromMem(SString sTable,SString sLobField,SString sWhere,unsigned char* pBuf,int iBufLen)
{
	LOGBASEDEBUG("Into SMdb::UpdateLobFromMem(%s,%s,%s)",
		sTable.data(),sLobField.data(),sWhere.data());
	return m_MdbClient.UpdateBlobFromMem(sTable,sLobField,sWhere,pBuf,iBufLen) > 0;
}
