/**
 *
 * 文 件 名	: SDatabase.h
 * 创建日期	: 2007-07-23
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期	: $Date: 2009/01/16 06:16:02 $
 * 当前版本	: $Revision: 1.2 $
 * 功能描述 : 通用数据库接口类及底层类
 * 修改记录 : 
 *            $Log: SDatabase.h,v $
 *            Revision 1.2  2009/01/16 06:16:02  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/02 01:50:08  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.6  2008/04/03 02:44:34  shaokt
 *            LINUX下将ysbase分离为.so，单独编译，设备程序在编译时连接该so，不将目标码生成到执行文件中；WIN32下为调试方便还直接编译基类源码
 *
 *            Revision 1.5  2007/12/10 06:40:28  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.4  2007/10/29 07:30:23  shaokt
 *            添加对象地址是否为4的倍数的检测函数，为检测ARM下由于pack(1)造成的内存问题
 *
 *            Revision 1.3  2007/09/11 08:17:52  shaokt
 *            基类更新
 *
 *            Revision 1.2  2007/08/27 09:51:07  shaokt
 *            基类更新
 *
 *            Revision 1.1  2007/08/15 03:09:22  shaokt
 *            *** empty log message ***
 *
 **/

#if !defined(AFX_SKTDATABASE_H__D6BE0D97_13A8_11D4_A5D2_002078B03530__INCLUDED_)
#define AFX_SKTDATABASE_H__D6BE0D97_13A8_11D4_A5D2_002078B03530__INCLUDED_

#include "../SString.h"
#include "../SList.h"
#include "../SLock.h"
#include "SApi.h"


enum e_SDatabase_Type
{
	DB_UNKNOWN=0,
	DB_MYSQL,
	DB_PGSQL,
	DB_ORACLE,
	DB_DAMENG,
	DB_MDB,
};

//记录类
class SBASE_EXPORT SRecord
{
public:
	SRecord(int iCols);
	SRecord();
	~SRecord();

	//设置列数
	void SetColumns(int iCols);

	//取第几个字段的值，iCol从0开始
	inline SString GetValue(int iCol)
	{
		if(m_pFields == NULL || iCol>=m_iCols || iCol < 0)
			return "";
		return m_pFields[iCol];
	}
	inline const char* GetValueStr(int iCol)
	{
		return m_pFields[iCol].data();
	}
	inline int GetValueInt(int iCol)
	{
		return m_pFields[iCol].toInt();
	}
	inline long GetValueLong(int iCol)
	{
		return m_pFields[iCol].toLong();
	}
	inline INT64 GetValueInt64(int iCol)
	{
		return SString::toInt64(m_pFields[iCol].data());
	}
	inline float GetValueFloat(int iCol)
	{
		return m_pFields[iCol].toFloat();
	}
	inline SString* GetValuePtr(int iCol)
	{
		if(m_pFields == NULL || iCol>=m_iCols || iCol < 0)
			return NULL;
		return &m_pFields[iCol];
	}
	

	//设置第几个字段的值，iCol从0开始
	bool SetValue(int iCol,SString &val);
	bool SetValue(int iCol,const char* val);

	void clear();

	//取列数
	inline int GetCount()				{return m_iCols;};

	inline void SetOid(unsigned int oid){m_iRowOid = oid;};
	inline unsigned int GetOid()		{return m_iRowOid;};
private:
	SString *m_pFields;
	int m_iCols;//列数
	unsigned int m_iRowOid;//行唯一键值，针对内存数据库时使用
};

//记录集类
class SBASE_EXPORT SRecordset
{
public:
	SRecordset();
	SRecordset(int iCols);
	~SRecordset();

	//设置列名
	inline void SetColumnName(int iCol,SString val){m_ColNames.SetValue(iCol,val);};

	//取列名
	inline SString GetColumnName(int iCol){return m_ColNames.GetValue(iCol);};
	void clear();

	//插入记录
	void SetSize(int iRows,int iCols);
	void SetSizeWithoutRecord(int iRows,int iCols);
	inline void SetRecord(int row,SRecord *pRecord)
	{
		m_RecordArray.setAt(row,pRecord);
	}

	inline int GetRows(){return m_RecordArray.count();};
	inline int GetColumns(){return m_ColNames.GetCount();};

	//取记录
	inline SRecord* GetRecord(int iRow){return m_RecordArray.getAt(iRow);};

	inline SString GetValue(int iRow,int iCol)
	{
		SRecord *p = GetRecord(iRow);
		if(p == NULL)
			return "";
		return m_RecordArray.getAt(iRow)->GetValue(iCol);
	}
	inline const char* GetValueStr(int iRow,int iCol)
	{
		return m_RecordArray.getAt(iRow)->GetValueStr(iCol);
	}
	inline int GetValueInt(int iRow,int iCol)
	{
		return m_RecordArray.getAt(iRow)->GetValueInt(iCol);
	}
	inline int GetValueLong(int iRow,int iCol)
	{
		return m_RecordArray.getAt(iRow)->GetValueLong(iCol);
	}
	inline INT64 GetValueInt64(int iRow,int iCol)
	{
		return m_RecordArray.getAt(iRow)->GetValueInt64(iCol);
	}
	inline float GetValueFloat(int iRow,int iCol)
	{
		return m_RecordArray.getAt(iRow)->GetValueFloat(iCol);
	}
	bool SetValue(int iRow,int iCol,SString &val);
	bool SetValue(int iRow,int iCol,const char* val);

	inline int GetOidBytes(){return m_iOidBytes;};
	inline void SetOidBytes(int bytes){m_iOidBytes = bytes;};

private:
	SRecord m_ColNames;
	int m_iOidBytes;//仅内存数据库使用，表示OID的长度，字节数，必须小于等于4字节
	SPtrArray<SRecord> m_RecordArray;
};

//数据库操作类，含连接池接口
class SBASE_EXPORT SDatabase
{
public:
	enum SBASE_EXPORT STATUS
	{
		UNCONNECT,
		IDLE,
		BUSY,
		DBERROR
	};
	SDatabase();
	virtual ~SDatabase();

	inline void SetParams(SString sParams)	{m_sParams = sParams;};
	inline SString GetParams()				{return m_sParams;};

	virtual bool Connect()=0;
	virtual bool DisConnect(){return false;};
	virtual bool TestConnect()=0;
	virtual int Retrieve(SString sql,SRecordset &rs)=0;
	virtual bool Execute(SString sql)=0;
	virtual bool UpdateLobFromFile(SString sTable,SString sLobField,SString sWhere,SString sFile)=0;
	virtual bool ReadLobToFile(SString sTable,SString sLobField,SString sWhere,SString sFile)=0;

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  开始一个新的事务
	// 作    者:  邵凯田
	// 创建时间:  2017-8-24 15:55
	// 参数说明:  void
	// 返 回 值:  true/false
	//////////////////////////////////////////////////////////////////////////
	virtual bool BeginTrans(){return false;};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  提交当前事务，并退出事务模式
	// 作    者:  邵凯田
	// 创建时间:  2017-8-24 15:55
	// 参数说明:  void
	// 返 回 值:  true/false
	//////////////////////////////////////////////////////////////////////////
	virtual bool Commit(){return false;};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  回滚当前事务，并退出事务模式
	// 作    者:  邵凯田
	// 创建时间:  2017-8-24 15:56
	// 参数说明:  void
	// 返 回 值:  true/false
	//////////////////////////////////////////////////////////////////////////
	virtual bool Rollback(){return false;};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  从数据库读取指定的大字段内容到缓冲区，缓冲区内容由调用都负责释放
	// 作    者:  邵凯田
	// 创建时间:  2015-8-4 14:15
	// 参数说明:  
	// 返 回 值:  true/false
	//////////////////////////////////////////////////////////////////////////
	virtual bool ReadLobToMem(SString sTable,SString sLobField,SString sWhere,unsigned char* &pBuf,int &iBufLen)=0;

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  将内存大字段写入数据库
	// 作    者:  邵凯田
	// 创建时间:  2015-8-29 17:43
	// 参数说明:  
	// 返 回 值:  true/false
	//////////////////////////////////////////////////////////////////////////
	virtual bool UpdateLobFromMem(SString sTable,SString sLobField,SString sWhere,unsigned char* pBuf,int iBufLen)=0;

	SString SelectInto(SString sql,int col=0);
	bool Reconnect();

	int SelectIntoI(SString sql,int col=0);

	float SelectIntoF(SString sql,int col=0);

	STATUS GetStatus()						{return m_Status;};
	inline void SetStatus(STATUS stu)		{m_Status = stu;};
	SLock* GetLock()						{return &m_Lock;};
	inline e_SDatabase_Type GetDbType()	{return m_dbType;};

private:
	STATUS m_Status;
	SLock m_Lock;//实例锁
	SString m_sParams;//连接参数集，如:dbname=znl4;user=postgres;password=123456;
protected:
	e_SDatabase_Type m_dbType;
};

template <class type>
class SDatabasePool
{
public:
	SDatabasePool()
	{
		SKTOBJECT("SDatabasePool");
		m_DatabasePool.setAutoDelete(true);
	}

	~SDatabasePool()
	{
		m_DatabasePool.clear();
	}

	void SetParams(SString sParams)
	{
		m_sParams = sParams;
	}

	SString GetParams()
	{
		return m_sParams;
	}

	int GetPoolSize()
	{
		return m_DatabasePool.count();
	}

	//创建指定大小的连接池
	bool CreateDatabasePool(int iPools)
	{
		m_Lock.lock();
		m_DatabasePool.clear();
		m_DatabasePool.setSize(iPools);
		for(int i=0;i<iPools;i++)
		{
			m_DatabasePool.setAt(i,new type());//缺少则新创建连接实例
			m_DatabasePool.getAt(i)->SetParams(m_sParams);
			m_DatabasePool.getAt(i)->DisConnect();
			if(m_DatabasePool.getAt(i)->Connect())
			{
				m_DatabasePool.getAt(i)->SetStatus(SDatabase::IDLE);
			}
			else
			{
				m_DatabasePool.getAt(i)->SetStatus(SDatabase::DBERROR);
			}
		}
		m_Lock.unlock();
		return true;
	}
	
	inline SDatabase* GetDatabaseByIdx(int idx)
	{
		return m_DatabasePool.at(idx);
	}

	//取可用连接实例
	SDatabase* GetDatabase(bool bTestConnect=false)
	{
		//bTestConnect = true;//add by skt
		SDatabase *pDb = NULL;
		int iPools;
		int times=0;
		m_Lock.lock();
	begin:
		iPools = m_DatabasePool.count();
		if(iPools <= 0)
		{
			m_Lock.unlock();
			return NULL;
		}
		for(int i=0;i<iPools;i++)
		{
			pDb = (SDatabase*)m_DatabasePool.getAt(i);
			if(pDb->GetLock()->trylock())
			{
				//有空闲连接直接使用
				if(pDb->GetStatus() == SDatabase::DBERROR)//数据库错误
				{
					pDb->Reconnect();
				}
				m_Lock.unlock();

				if(bTestConnect && !pDb->TestConnect())
				{
					pDb->GetLock()->unlock();
					return NULL;//连接无效
				}
				return pDb;//连接
			}
		}
		//没有空闲，等待
		#ifdef WIN32
			Sleep(1);//100
		#else
			usleep(1000);//100
		#endif
		times++;
		if(times < 10000)//600
			goto begin;
		m_Lock.unlock();
		return NULL;
	}

	void Release(SDatabase* pDb)
	{
		if(pDb)
			pDb->GetLock()->unlock();
	}

public:
	SLock m_Lock;//池锁
	SPtrArray<type> m_DatabasePool;//连接池实例集
	SString m_sParams;//连接参数，如:dbname=znl4;user=postgres;password=123456;
	
};

#endif // !defined(AFX_SKTDATABASE_H__D6BE0D97_13A8_11D4_A5D2_002078B03530__INCLUDED_)
