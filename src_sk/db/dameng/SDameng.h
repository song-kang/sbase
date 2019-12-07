/**
 *
 * �� �� �� : SDameng.h
 * �������� : 2007-11-24
 * ��    �� : �ۿ���
 * �޸����� : $Date: 2007/11/24 13:29:08 $
 * ��ǰ�汾 : $Revision: 1.3 $
 * �������� : �������ݿ��SktDatabase�ӿ����ʵ��
 * �޸ļ�¼ : 
 *            Revision 1.1  2007/08/15 03:09:22  shaokt
 *            *** empty log message ***
 *
 **/

#ifndef __SKT_DAMENG_H__
#define __SKT_DAMENG_H__

#include "../Database.h"
#include "../../SLog.h"

#ifdef WIN32
#pragma warning(disable:4786)
#pragma comment(lib,"dmdpi.lib")
#endif

#include "DPI.h" 
#include "DPIext.h" 
#include "DPItypes.h" 
#include "stdio.h" 
#include "stdlib.h" 
#include "string.h" 

class SDameng  : public SDatabase
{
public:

private:
	dhenv m_henv; /* ������� */ 
	dhcon m_hcon; /* ���Ӿ�� */ 
	//hstmt m_hstmt;/*�����*/
	int m_iTestTimes;
public:
	SDameng();
	virtual ~SDameng();

	virtual bool Connect();
	virtual bool DisConnect();
	virtual bool TestConnect();
	virtual int Retrieve(SString sql,SRecordset &rs);
	virtual bool Execute(SString sql);

	virtual bool UpdateLobFromFile(SString sTable,SString sLobField,SString sWhere,SString sFile);
	virtual bool ReadLobToFile(SString sTable,SString sLobField,SString sWhere,SString sFile);

	virtual bool ReadLongRawToFile(SString sTable,SString sLobField,SString sWhere,SString ysFile);
	virtual bool UpdateLongRawFromFile(SString sTable,SString sLobField,SString sWhere,SString sFile);

	bool readLob(SString tableName,SString fieldName,SString whereField,SString recordIndex,SString &value);
	bool updateLob(SString tableName,SString fieldName,SString whereField,SString recordIndex,SString &value);

	SString getColumnType(SString tableName,SString fieldName);

};

#endif// __SKT_DAMENG_H__





