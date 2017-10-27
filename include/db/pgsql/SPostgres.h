/**
 *
 * �� �� �� : SPostgres.h
 * �������� : 2007-07-24
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: 2008/09/02 01:50:09 $
 * ��ǰ�汾 : $Revision: 1.1 $
 * �������� : Postgres���ݿ��SDatabase�ӿ����ʵ��
 * �޸ļ�¼ : 
 *            $Log: SPostgres.h,v $
 *            Revision 1.1  2008/09/02 01:50:09  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2007/10/10 13:29:08  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2007/09/11 08:17:53  shaokt
 *            �������
 *
 *            Revision 1.1  2007/08/15 03:09:22  shaokt
 *            *** empty log message ***
 *
 **/

#if !defined(AFX_YSPOSTGRES_H__D6BE0D97_13A8_11D4_A5D2_002078B03530__INCLUDED_)
#define AFX_YSPOSTGRES_H__D6BE0D97_13A8_11D4_A5D2_002078B03530__INCLUDED_

#include "../SDatabase.h"
#include "../../SLog.h"


#include <libpq-fe.h>
#include <libpq/libpq-fs.h>

#ifdef WIN32
	#pragma comment(lib,"libpq.lib")
	#pragma comment(lib,"libecpg.lib")
#endif

#ifndef NULL
#define NULL 0
#endif

class SBASE_PGSQL_EXPORT SPostgres : public SDatabase
{
public:
	SPostgres();
	virtual ~SPostgres();

	virtual bool Connect();
	virtual bool DisConnect();
	virtual bool TestConnect();
	virtual int Retrieve(SString sql,SRecordset &rs);
	virtual bool Execute(SString sql);
	virtual bool UpdateLobFromFile(SString sTable,SString sLobField,SString sWhere,SString sFile);
	virtual bool ReadLobToFile(SString sTable,SString sLobField,SString sWhere,SString sFile);

private:
	PGconn* m_pConn;
	int m_iTestTimes;
};


#endif // !defined(AFX_YSPOSTGRES_H__D6BE0D97_13A8_11D4_A5D2_002078B03530__INCLUDED_)
