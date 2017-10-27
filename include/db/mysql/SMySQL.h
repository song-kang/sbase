/**
 *
 * 文 件 名 : SMySQL.h
 * 创建日期 : 2007-10-9 14:10
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: 2008/09/02 01:50:09 $
 * 当前版本 : $Revision: 1.1 $
 * 功能描述 : MySQL数据库对YsDatabase接口类的实现
 * 修改记录 : 
 *            $Log: SMySQL.h,v $
 *            Revision 1.1  2008/09/02 01:50:09  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2007/10/09 11:59:45  shaokt
 *            MYSQL数据库操作接口
 *
 *
 **/

#if !defined(AFX_SKTMYSQL_H__D6BE0D97_13A8_11D4_A5D2_002078B03530__INCLUDED_)
#define AFX_SKTMYSQL_H__D6BE0D97_13A8_11D4_A5D2_002078B03530__INCLUDED_

#include "../SDatabase.h"
#include "../../SLog.h"
#include "../../SFile.h"

#include <mysql.h>

#ifdef WIN32
	#pragma comment(lib,"libmysql.lib")
#endif

#ifndef NULL
#define NULL 0
#endif

//////////////////////////////////////////////////////////////////////////
// 名    称:  
// 作    者:  邵凯田
// 创建时间:  2015-7-9 13:37
// 描    述:  参数为：hostaddr=127.0.0.1;port=3306;dbname=ukdb;user=root;password=123456;
//////////////////////////////////////////////////////////////////////////
class SBASE_MYSQL_EXPORT SMySQL : public SDatabase
{
public:
	SMySQL();
	virtual ~SMySQL();

	virtual bool Connect();
	void SetEncoding();
	virtual bool DisConnect();
	virtual bool TestConnect();
	virtual int Retrieve(SString sql,SRecordset &rs);
	virtual bool Execute(SString sql);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  开始一个新的事务
	// 作    者:  邵凯田
	// 创建时间:  2017-8-24 15:55
	// 参数说明:  void
	// 返 回 值:  true/false
	//////////////////////////////////////////////////////////////////////////
	virtual bool BeginTrans();

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  提交当前事务，并退出事务模式
	// 作    者:  邵凯田
	// 创建时间:  2017-8-24 15:55
	// 参数说明:  void
	// 返 回 值:  true/false
	//////////////////////////////////////////////////////////////////////////
	virtual bool Commit();

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  回滚当前事务，并退出事务模式
	// 作    者:  邵凯田
	// 创建时间:  2017-8-24 15:56
	// 参数说明:  void
	// 返 回 值:  true/false
	//////////////////////////////////////////////////////////////////////////
	virtual bool Rollback();


	virtual bool UpdateLobFromFile(SString sTable,SString sLobField,SString sWhere,SString sFile);
	virtual bool ReadLobToFile(SString sTable,SString sLobField,SString sWhere,SString sFile);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  从数据库读取指定的大字段内容到缓冲区，缓冲区内容由调用者负责释放
	// 作    者:  邵凯田
	// 创建时间:  2015-8-4 14:15
	// 参数说明:  
	// 返 回 值:  true/false
	//////////////////////////////////////////////////////////////////////////
	virtual bool ReadLobToMem(SString sTable,SString sLobField,SString sWhere,unsigned char* &pBuf,int &iBufLen);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  将内存大字段写入数据库
	// 作    者:  邵凯田
	// 创建时间:  2015-8-29 17:43
	// 参数说明:  
	// 返 回 值:  true/false
	//////////////////////////////////////////////////////////////////////////
	virtual bool UpdateLobFromMem(SString sTable,SString sLobField,SString sWhere,unsigned char* pBuf,int iBufLen);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取连接句柄
	// 作    者:  邵凯田
	// 创建时间:  2016-3-24 17:28
	// 参数说明:  void
	// 返 回 值:  MYSQL*,NULL表示尚未连接
	//////////////////////////////////////////////////////////////////////////
	inline MYSQL* GetConnectHandle(){return m_pConn;};
private:
	MYSQL *m_pConn;//mysql struct
	int m_iTestTimes;
	int m_tLastSQLTime;//最后一次执行SQL（执行或选择）时的时间，超时执行前需要选重设编码，否则编码会复位导致乱码
};


#endif // !defined(AFX_SKTMYSQL_H__D6BE0D97_13A8_11D4_A5D2_002078B03530__INCLUDED_)
