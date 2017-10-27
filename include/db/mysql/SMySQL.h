/**
 *
 * �� �� �� : SMySQL.h
 * �������� : 2007-10-9 14:10
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: 2008/09/02 01:50:09 $
 * ��ǰ�汾 : $Revision: 1.1 $
 * �������� : MySQL���ݿ��YsDatabase�ӿ����ʵ��
 * �޸ļ�¼ : 
 *            $Log: SMySQL.h,v $
 *            Revision 1.1  2008/09/02 01:50:09  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2007/10/09 11:59:45  shaokt
 *            MYSQL���ݿ�����ӿ�
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
// ��    ��:  
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-9 13:37
// ��    ��:  ����Ϊ��hostaddr=127.0.0.1;port=3306;dbname=ukdb;user=root;password=123456;
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
	// ��    ��:  ��ʼһ���µ�����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-8-24 15:55
	// ����˵��:  void
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	virtual bool BeginTrans();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �ύ��ǰ���񣬲��˳�����ģʽ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-8-24 15:55
	// ����˵��:  void
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	virtual bool Commit();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �ع���ǰ���񣬲��˳�����ģʽ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-8-24 15:56
	// ����˵��:  void
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	virtual bool Rollback();


	virtual bool UpdateLobFromFile(SString sTable,SString sLobField,SString sWhere,SString sFile);
	virtual bool ReadLobToFile(SString sTable,SString sLobField,SString sWhere,SString sFile);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����ݿ��ȡָ���Ĵ��ֶ����ݵ��������������������ɵ����߸����ͷ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-4 14:15
	// ����˵��:  
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	virtual bool ReadLobToMem(SString sTable,SString sLobField,SString sWhere,unsigned char* &pBuf,int &iBufLen);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���ڴ���ֶ�д�����ݿ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-29 17:43
	// ����˵��:  
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	virtual bool UpdateLobFromMem(SString sTable,SString sLobField,SString sWhere,unsigned char* pBuf,int iBufLen);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ���Ӿ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-3-24 17:28
	// ����˵��:  void
	// �� �� ֵ:  MYSQL*,NULL��ʾ��δ����
	//////////////////////////////////////////////////////////////////////////
	inline MYSQL* GetConnectHandle(){return m_pConn;};
private:
	MYSQL *m_pConn;//mysql struct
	int m_iTestTimes;
	int m_tLastSQLTime;//���һ��ִ��SQL��ִ�л�ѡ��ʱ��ʱ�䣬��ʱִ��ǰ��Ҫѡ������룬�������Ḵλ��������
};


#endif // !defined(AFX_SKTMYSQL_H__D6BE0D97_13A8_11D4_A5D2_002078B03530__INCLUDED_)
