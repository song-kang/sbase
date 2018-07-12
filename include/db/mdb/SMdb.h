/**
 *
 * �� �� �� : SMdb.h
 * �������� : 2007-07-24
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: 2008/09/02 01:50:09 $
 * ��ǰ�汾 : $Revision: 1.1 $
 * �������� : �ڴ����ݿ�MDB��SDatabase�ӿ����ʵ��
 * �޸ļ�¼ : 
 *            $Log: SMdb.h,v $
 *
 **/

#if !defined(AFX_SMDB_H__D6BE0D97_13A8_11D4_A5D2_002078B03530__INCLUDED_)
#define AFX_SMDB_H__D6BE0D97_13A8_11D4_A5D2_002078B03530__INCLUDED_

#include "../SDatabase.h"
#include "../../SLog.h"

#include "MdbClient.h"

//////////////////////////////////////////////////////////////////////////
// ��    ��:  SMdb
// ��    ��:  �ۿ���
// ����ʱ��:  2015-8-4 14:15
// ��    ��:  ���Ӳ���Ϊ��hostaddr=127.0.0.1;port=1400;user=skt_mdb;password=skt001@163.com;
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
	// ��    ��:  �����ݿ��ȡָ���Ĵ��ֶ����ݵ����������������ɵ��ö�����ʹ��FreeLobMem���������ͷ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-4 14:15
	// ����˵��:  
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	virtual bool ReadLobToMem(SString sTable,SString sLobField,SString sWhere,unsigned char* &pBuf,int &iBufLen);


	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����ͷ�ReadLobToMem���������Ļ�����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-12-1 11:01
	// ����˵��:  @pBufΪ������ָ��
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void FreeLobMem(unsigned char* pBuf);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���ڴ���ֶ�д�����ݿ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-29 17:43
	// ����˵��:  
	// �� �� ֵ:  true/false
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
