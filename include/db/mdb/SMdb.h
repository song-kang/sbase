/**
 *
 * 文 件 名 : SMdb.h
 * 创建日期 : 2007-07-24
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: 2008/09/02 01:50:09 $
 * 当前版本 : $Revision: 1.1 $
 * 功能描述 : 内存数据库MDB对SDatabase接口类的实现
 * 修改记录 : 
 *            $Log: SMdb.h,v $
 *
 **/

#if !defined(AFX_SMDB_H__D6BE0D97_13A8_11D4_A5D2_002078B03530__INCLUDED_)
#define AFX_SMDB_H__D6BE0D97_13A8_11D4_A5D2_002078B03530__INCLUDED_

#include "../SDatabase.h"
#include "../../SLog.h"

#include "MdbClient.h"

//////////////////////////////////////////////////////////////////////////
// 名    称:  SMdb
// 作    者:  邵凯田
// 创建时间:  2015-8-4 14:15
// 描    述:  连接参数为：hostaddr=127.0.0.1;port=1400;user=skt_mdb;password=skt001@163.com;
//////////////////////////////////////////////////////////////////////////
class SBASE_MDB_EXPORT SMdb : public SDatabase
{
public:
	SMdb();
	virtual ~SMdb();
	
	virtual bool Connect();
	virtual bool DisConnect();
	virtual bool TestConnect();
	virtual int Retrieve(SString sql,SRecordset &rs);
	virtual bool Execute(SString sql);
	virtual bool UpdateLobFromFile(SString sTable,SString sLobField,SString sWhere,SString sFile);
	virtual bool ReadLobToFile(SString sTable,SString sLobField,SString sWhere,SString sFile);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  从数据库读取指定的大字段内容到缓冲区，缓冲区由调用都负责使用FreeLobMem函数进行释放
	// 作    者:  邵凯田
	// 创建时间:  2015-8-4 14:15
	// 参数说明:  
	// 返 回 值:  true/false
	//////////////////////////////////////////////////////////////////////////
	virtual bool ReadLobToMem(SString sTable,SString sLobField,SString sWhere,unsigned char* &pBuf,int &iBufLen);


	//////////////////////////////////////////////////////////////////////////
	// 描    述:  用于释放ReadLobToMem函数产生的缓冲区
	// 作    者:  邵凯田
	// 创建时间:  2017-12-1 11:01
	// 参数说明:  @pBuf为缓冲区指针
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void FreeLobMem(unsigned char* pBuf);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  将内存大字段写入数据库
	// 作    者:  邵凯田
	// 创建时间:  2015-8-29 17:43
	// 参数说明:  
	// 返 回 值:  true/false
	//////////////////////////////////////////////////////////////////////////
	virtual bool UpdateLobFromMem(SString sTable,SString sLobField,SString sWhere,unsigned char* pBuf,int iBufLen);

	virtual void OnLogin();
	virtual void OnClose();
	SString GetLastError();
	inline CMdbClient* GetMdbClient(){ return &m_MdbClient; };
private:
	CMdbClient m_MdbClient;
};


#endif // !defined(AFX_SMDB_H__D6BE0D97_13A8_11D4_A5D2_002078B03530__INCLUDED_)
