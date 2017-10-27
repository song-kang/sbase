/**
 *
 * �� �� ��	: SDatabase.h
 * ��������	: 2007-07-23
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸�����	: $Date: 2009/01/16 06:16:02 $
 * ��ǰ�汾	: $Revision: 1.2 $
 * �������� : ͨ�����ݿ�ӿ��༰�ײ���
 * �޸ļ�¼ : 
 *            $Log: SDatabase.h,v $
 *            Revision 1.2  2009/01/16 06:16:02  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/02 01:50:08  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.6  2008/04/03 02:44:34  shaokt
 *            LINUX�½�ysbase����Ϊ.so���������룬�豸�����ڱ���ʱ���Ӹ�so������Ŀ�������ɵ�ִ���ļ��У�WIN32��Ϊ���Է��㻹ֱ�ӱ������Դ��
 *
 *            Revision 1.5  2007/12/10 06:40:28  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.4  2007/10/29 07:30:23  shaokt
 *            ��Ӷ����ַ�Ƿ�Ϊ4�ı����ļ�⺯����Ϊ���ARM������pack(1)��ɵ��ڴ�����
 *
 *            Revision 1.3  2007/09/11 08:17:52  shaokt
 *            �������
 *
 *            Revision 1.2  2007/08/27 09:51:07  shaokt
 *            �������
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

//��¼��
class SBASE_EXPORT SRecord
{
public:
	SRecord(int iCols);
	SRecord();
	~SRecord();

	//��������
	void SetColumns(int iCols);

	//ȡ�ڼ����ֶε�ֵ��iCol��0��ʼ
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
	

	//���õڼ����ֶε�ֵ��iCol��0��ʼ
	bool SetValue(int iCol,SString &val);
	bool SetValue(int iCol,const char* val);

	void clear();

	//ȡ����
	inline int GetCount()				{return m_iCols;};

	inline void SetOid(unsigned int oid){m_iRowOid = oid;};
	inline unsigned int GetOid()		{return m_iRowOid;};
private:
	SString *m_pFields;
	int m_iCols;//����
	unsigned int m_iRowOid;//��Ψһ��ֵ������ڴ����ݿ�ʱʹ��
};

//��¼����
class SBASE_EXPORT SRecordset
{
public:
	SRecordset();
	SRecordset(int iCols);
	~SRecordset();

	//��������
	inline void SetColumnName(int iCol,SString val){m_ColNames.SetValue(iCol,val);};

	//ȡ����
	inline SString GetColumnName(int iCol){return m_ColNames.GetValue(iCol);};
	void clear();

	//�����¼
	void SetSize(int iRows,int iCols);
	void SetSizeWithoutRecord(int iRows,int iCols);
	inline void SetRecord(int row,SRecord *pRecord)
	{
		m_RecordArray.setAt(row,pRecord);
	}

	inline int GetRows(){return m_RecordArray.count();};
	inline int GetColumns(){return m_ColNames.GetCount();};

	//ȡ��¼
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
	int m_iOidBytes;//���ڴ����ݿ�ʹ�ã���ʾOID�ĳ��ȣ��ֽ���������С�ڵ���4�ֽ�
	SPtrArray<SRecord> m_RecordArray;
};

//���ݿ�����࣬�����ӳؽӿ�
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
	// ��    ��:  ��ʼһ���µ�����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-8-24 15:55
	// ����˵��:  void
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	virtual bool BeginTrans(){return false;};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �ύ��ǰ���񣬲��˳�����ģʽ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-8-24 15:55
	// ����˵��:  void
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	virtual bool Commit(){return false;};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �ع���ǰ���񣬲��˳�����ģʽ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-8-24 15:56
	// ����˵��:  void
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	virtual bool Rollback(){return false;};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����ݿ��ȡָ���Ĵ��ֶ����ݵ��������������������ɵ��ö������ͷ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-4 14:15
	// ����˵��:  
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	virtual bool ReadLobToMem(SString sTable,SString sLobField,SString sWhere,unsigned char* &pBuf,int &iBufLen)=0;

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���ڴ���ֶ�д�����ݿ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-29 17:43
	// ����˵��:  
	// �� �� ֵ:  true/false
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
	SLock m_Lock;//ʵ����
	SString m_sParams;//���Ӳ���������:dbname=znl4;user=postgres;password=123456;
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

	//����ָ����С�����ӳ�
	bool CreateDatabasePool(int iPools)
	{
		m_Lock.lock();
		m_DatabasePool.clear();
		m_DatabasePool.setSize(iPools);
		for(int i=0;i<iPools;i++)
		{
			m_DatabasePool.setAt(i,new type());//ȱ�����´�������ʵ��
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

	//ȡ��������ʵ��
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
				//�п�������ֱ��ʹ��
				if(pDb->GetStatus() == SDatabase::DBERROR)//���ݿ����
				{
					pDb->Reconnect();
				}
				m_Lock.unlock();

				if(bTestConnect && !pDb->TestConnect())
				{
					pDb->GetLock()->unlock();
					return NULL;//������Ч
				}
				return pDb;//����
			}
		}
		//û�п��У��ȴ�
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
	SLock m_Lock;//����
	SPtrArray<type> m_DatabasePool;//���ӳ�ʵ����
	SString m_sParams;//���Ӳ�������:dbname=znl4;user=postgres;password=123456;
	
};

#endif // !defined(AFX_SKTDATABASE_H__D6BE0D97_13A8_11D4_A5D2_002078B03530__INCLUDED_)
