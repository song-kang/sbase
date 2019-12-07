/**
 *
 * �� �� ��	: SDatabase.cpp
 * ��������	: 2007-07-23
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸�����	: $Date: 2008/09/02 01:50:08 $
 * ��ǰ�汾	: $Revision: 1.1 $
 * �������� : ͨ�����ݿ�ӿ��༰�ײ���
 * �޸ļ�¼ : 
 *            $Log: SDatabase.cpp,v $
 *            Revision 1.1  2008/09/02 01:50:08  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2008/04/03 02:44:34  shaokt
 *            LINUX�½�ysbase����Ϊ.so���������룬�豸�����ڱ���ʱ���Ӹ�so������Ŀ�������ɵ�ִ���ļ��У�WIN32��Ϊ���Է��㻹ֱ�ӱ������Դ��
 *
 *            Revision 1.2  2007/10/29 07:30:23  shaokt
 *            ��Ӷ����ַ�Ƿ�Ϊ4�ı����ļ�⺯����Ϊ���ARM������pack(1)��ɵ��ڴ�����
 *
 *            Revision 1.1  2007/08/15 03:09:22  shaokt
 *            *** empty log message ***
 *
 **/

#include "SDatabase.h"


/////////////////////////////////////////////////////////////////////
// Construction/Destruction
/////////////////////////////////////////////////////////////////////
SRecord::SRecord()
{
	SKTOBJECT("SRecord");
	m_pFields = NULL;
	m_iCols = 0;
}

SRecord::SRecord(int iCols)
{
	SKTOBJECT("SRecord");
	m_pFields = NULL;
	m_iCols = 0;
	SetColumns(iCols);
}

SRecord::~SRecord()
{
	clear();
}

//��������
void SRecord::SetColumns(int iCols)
{
	if(m_iCols == iCols)
		return;
	m_iCols = iCols;
	if(iCols>0)
	{
		m_pFields = new SString[iCols];
	}
}

void SRecord::clear()
{
	if(m_pFields != NULL)
	{
		delete[] m_pFields;
		m_pFields = NULL;
		m_iCols = 0;
	}
}


//���õڼ����ֶε�ֵ��iCol��0��ʼ
bool SRecord::SetValue(int iCol,SString &val)
{
	if(m_pFields == NULL || iCol>=m_iCols || iCol < 0)
		return false;
	m_pFields[iCol] = val;
	return true;
}

bool SRecord::SetValue(int iCol,const char* val)
{
	if(m_pFields == NULL || iCol>=m_iCols || iCol < 0)
		return false;
	m_pFields[iCol] = val;
	return true;
}

SRecordset::SRecordset()
{
	SKTOBJECT("SRecordset");
	m_RecordArray.setAutoDelete(true);
	m_iOidBytes = 0;
}

SRecordset::SRecordset(int iCols)
{
	SKTOBJECT("SRecordset");
	m_iOidBytes = 0;
	m_RecordArray.setAutoDelete(true);
	if(iCols>0)
	{
		m_ColNames.SetColumns(iCols);
	}
}

SRecordset::~SRecordset()
{
	clear();
}

void SRecordset::clear()
{
	m_RecordArray.clear();
	m_ColNames.clear();
}

//�����¼
void SRecordset::SetSize(int iRows,int iCols)
{
	clear();
	m_ColNames.SetColumns(iCols);
	m_RecordArray.setSize(iRows);
	for(int i=0;i<iRows;i++)
	{
		m_RecordArray.setAt(i,new SRecord(iCols));
	}
}

void SRecordset::SetSizeWithoutRecord(int iRows,int iCols)
{
	m_ColNames.SetColumns(iCols);
	m_RecordArray.setSize(iRows);
}

//ȡ��¼
// SString SRecordset::GetValue(int iRow,int iCol)
// {
// 	SRecord *p = GetRecord(iRow);
// 	if(p == NULL)
// 		return "";
// 	return p->GetValue(iCol);
// }

bool SRecordset::SetValue(int iRow,int iCol,SString &val)
{
	SRecord *p = GetRecord(iRow);
	if(p == NULL)
		return false;
	return p->SetValue(iCol,val);
}

bool SRecordset::SetValue(int iRow,int iCol,const char* val)
{
	SRecord *p = GetRecord(iRow);
	if(p == NULL)
		return false;
	return p->SetValue(iCol,val);
}

SDatabase::SDatabase()
{
	SKTOBJECT("SDatabase");
	m_Status = UNCONNECT;
	m_dbType = DB_UNKNOWN;
}

SDatabase::~SDatabase()
{
	DisConnect();
}

SString SDatabase::SelectInto(SString sql,int col/*=0*/)	
{
	SRecordset rs;
	if(Retrieve(sql,rs) > 0)
	{
		return rs.GetRecord(0)->GetValue(col);
	}
	return "";
}

bool SDatabase::Reconnect()
{
	DisConnect();
	if(Connect())
	{
		SetStatus(SDatabase::IDLE);
		return true;
	}
	else
	{
		SetStatus(SDatabase::DBERROR);
		return false;
	}		
}

int SDatabase::SelectIntoI(SString sql,int col/*=0*/)
{
	SString s = SelectInto(sql,col);
	return s.toInt();
}

float SDatabase::SelectIntoF(SString sql,int col/*=0*/)
{
	SString s = SelectInto(sql,col);
	return s.toFloat();
}

