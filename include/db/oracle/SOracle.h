/**
 *
 * �� �� �� : SOracle.h
 * �������� : 2007-11-24
 * ��    �� : �ۿ���
 * �޸����� : $Date: 2007/11/24 13:29:08 $
 * ��ǰ�汾 : $Revision: 1.3 $
 * �������� : Oracle���ݿ��SDatabase�ӿ����ʵ��
 * �޸ļ�¼ : 
 *            Revision 1.1  2007/08/15 03:09:22  shaokt
 *            *** empty log message ***
 *
 **/

#ifndef _SKT_ORACLE_
#define _SKT_ORACLE_

#include "../SDatabase.h"
#include "../../SLog.h"

#ifdef WIN32
#pragma warning(disable:4786)
#pragma warning(disable:4996)
#endif

/*
  1.ִ��һ��Sql���ʱ,�����Update,Insert,Delete�Ȳ���Ҫ�󶨲��������,���Բ������в���:
     string Sql="update table1 set table1_a='123' where table1_b=1985 "
     SqlExecute(Sql���);
     Commit;
  2.ִ��һ��Sql��Select���,������Ҫ�󶨲�����,���Բ������в���:
     string sql="select a,b,c from table2 where recordindex='111111' ";
     OdbRecordSet rs; long hStmt=0;
     sword status=Select(Sql,rs,hStmt);
     while(status==0)
     {
         status=FetchNext(hStmt,rs);	 a_value=rs.GetFieldValue(0);	 b_value=rs.GetFieldValue(1);	 c_Value=rs/GetFieldValue("c");
     }
     ClearRecordSets(hStmt,rs);
   3.ִ��һ��Sql��Select���,��Ҫ�󶨲�����,���Բ������в���:  
     �������->׼��Sql(SqlPrepare)->�󶨲���->ִ��Sql->�������->ȡ��¼
     ����:
     string sql="select a,b,c from table2 where recordindex=:1"; //����:1������ִ��ʱ��ֵ
     OdbRecordSet rs; long hStmt=0;
     //�������
     OdbParamArrary oa;
     OdbParam* pParam=new OdbParam;
     pParam->wType=SQLT_STR; pParam->Set("111111");  oa.paParams.push_back(pParam);
     //
     db.SqlPrepare(sele,hStmt);     db.BindParam(hStmt,oa);     db.SqlExecuteWithParam(hStmt);     status=db.DefineColumn(hStmt,rs);
     while(status==0)
     {
         status=FetchNext(hStmt,rs);	 a_value=rs.GetFieldValue(0);	 b_value=rs.GetFieldValue(1);	 c_Value=rs/GetFieldValue("c");
     }
     ClearRecordSets(hStmt,rs);  db.FreeParams(hStmt,oa);    
   4.ִ��һ������CLOB/BLOB��ѯ��������
     ������ķ���һ��,����ȡ��¼ʱ��ͬ,����:
    string sele="select note from unionnote_rpt";
    long Stmt;  OdbRecordSet rs; db.Select(sele,rs,Stmt);
    while(db.FetchNext(Stmt,rs)==0)
    {
	for(int i=0;i<rs.paFields.size();i++)
	{
	    OdbField* pField=rs.paFields[i];
	    if(pField->wType==SQLT_CLOB||pField->wType==SQLT_BLOB)
	       db.ReadLobToFile(pField,"/home/temp.txt");   //�����ݱ��浽�ļ�.�˴�Ҳ����:StringArray oa;db.ReadCLob(pField,oa);�����ַ�������
        }
    }
    db.ClearRecordSets(Stmt,rs);
    return 1;
   5.ִ��һ������CLOB/BLOB��Update���,ֱ��д������
     ������ķ���һ��,����ȡ��¼ʱ��ͬ,����:
    string sele="Update unionnote_rpt set Note=:1";
    long gg;   OdbParamArray oa;
    OdbParam* pParam=new OdbParam;    pParam->wType=SQLT_CLOB;  pParam->BindPos=1; oa.paParams.push_back(pParam);
    db.SqlPrepare(sele,gg);   db.BindParam(gg,oa);
    StringArrary sac;   sac.push_back("we are mark1");  sac.push_back("we are mark2"); sac.push_back("we are mark3"); sac.push_back("we are mark4");
    db.WriteCLob(pParam,sac);
    db.SqlExecuteWithParam(gg); db.Commit(); db.FreeParams(gg,oa);
   6.ִ��һ������CLOB/BLOB��Update���,���ļ����ݱ��浽CLOB/BLOB�ֶ���
    string sele="Update unionnote_rpt set Note=:1";string filename='/home/temp.txt';
    long gg;   OdbParamArray oa;
    OdbParam* pParam=new OdbParam;    pParam->wType=SQLT_CLOB;  pParam->BindPos=1; oa.paParams.push_back(pParam);
    db.SqlPrepare(sele,gg);   db.BindParam(gg,oa);
    WriteLobFromFile(pParam,filename);
    db.SqlExecuteWithParam(gg); db.Commit(); db.FreeParams(gg,oa);
   7.ִ��һ���洢���̵��÷�,�洢����testp(a1 in varchar2,a2 in/out varchar2)
    string sele="begin testp(:1,:2); end;";
    long gg;   OdbParamArray oa;
    OdbParam* pParam=new OdbParam;    pParam->wType=SQLT_STR;  pParam->BindPos=1; oa.paParams.push_back(pParam);
              pParam=new OdbParam;    pParam->wType=SQLT_STR;  pParam->BindPos=2; oa.paParams.push_back(pParam);
    db.SqlPrepare(sele,gg);   db.BindParam(gg,oa);
    db.SqlExecuteWithParam(gg); db.FreeParams(gg,oa);
    //a2���������
*/


#include <stdio.h>
#include <stdlib.h>
#include <oci.h>
#include <string>
#include <vector>

#include <sys/timeb.h>
#include <time.h>

#ifndef lint 
typedef         ub4      duword;                   
typedef         sb4      dsword;                 
typedef         dsword   dword;

#else 
#define duword ub4 
#define dsword sb4 
#define dword  dsword
#endif  

#define READ_FILE_LENGTH 1024
typedef std::vector<std::string> StringArrary;
struct SBASE_ORACLE_EXPORT OdbField
{
    std::string 	Field_Name;					//�ֶ�����
    dvoid* 	pBuffer;						//����
    ub2   	wType;						//�ֶ�����
    ub2		wSize;						//�ֶγ���
	ub2		wLen;						//���ݳ���
	sb2		nInd;						//˳��
	ub2		*pwLen;						//���ݳ���
	sb2		*pnInd;						//˳��
	OCILobLocator* oLocator;					//LOB������
	OCILobLocator** ppoLocator;					//LOB������
	int iRows;
    std::string   FileName;
    bool	bQuotedOnUpdate;				//��ʹ��
    dword   status;
    OdbField();							//����
    ~OdbField();							//����
    void SetSize(ub2 w,int once_rows=1);						//����pBuffer���ڴ�,��С w
    void Set(std::string cs);					//��cs��ֵ����pBuffer
    std::string Get(int row=0) const;						//�õ�pBuffer��ֵ,����string
    void Free(int free);						//�ͷ�pBuffer��oLocator
};

struct SBASE_ORACLE_EXPORT OdbRecordSet
{
    std::vector<OdbField*> paFields;				//��ѡ��¼�Ľ���ֶμ���
    int             m_nRows;					//����
    std::string           m_csStatement;				//
    
    OdbRecordSet();
    ~OdbRecordSet();
	int GetFieldCount();
    int Find(const std::string &csName);				//����һ���ֶ��ڼ����е�λ��
    std::string GetFieldValue(const std::string csName);//�õ�һ���ֶε�ֵ
    std::string GetFieldValue(int iField,int row=0);	//�õ��ڼ����ֶε�ֵ
	inline void GetFieldValueToStr(SString *pStr,int iField,int row=0)
	{
		//if(iField<0||iField>=(int)paFields.size())
		//	return "";
		OdbField *pField = paFields[iField];
		ub2 len = pField->pwLen != NULL?pField->pwLen[row]:pField->wLen;
		if(len>0)
		{
			pStr->SetBuffer(len+1);
			char* buff = (char*)pStr->data();
			sprintf(buff,"%.*s",len,(char*)pField->pBuffer+row*pField->wSize);
			pStr->PrepareStrLen();
		}
		else
			*pStr = "";
	}

    void RemoveAll();						//����ֶεĽ����
};

struct SBASE_ORACLE_EXPORT OdbParam
{
    dvoid* 	pVar;						//�󶨱���
    ub2   	wType;						//��������
    ub2		wSize;						//��������
    ub2		wLen;						//���ݳ���
    ub2		BindPos;						//��λ��
    std::string   BindName;					//�󶨵�����
    ub2       By;                                       //ͨ��ʲô��ʽ��:0=λ��;1=����
    ub2       InOut;						//�������0=����,1=���;2=�������
    OCIBind*  bindhp;						//�󶨾��
    OCILobLocator *oLocator;					//CLOB.BLOBָʾ��
    std::string   FileName;
    OdbParam();
    ~OdbParam();
    void SetSize(ub2 w);
    void Set(std::string cs);
    void Set(dvoid* mem,int len);
    std::string Get() const;
    void Free(int free);
};

struct SBASE_ORACLE_EXPORT OdbParamArray
{
    std::vector<OdbParam*> paParams;				//�󶨱����ļ���
    OdbParamArray();						
    ~OdbParamArray();
    void SetFieldValue(int iParam,const std::string cs);		//�趨�ڼ���������ֵ
    std::string GetFieldValue(int iParam);				//�õ��ڼ���������ֵ
    void RemoveAll();
};

class SBASE_ORACLE_EXPORT COracleDataBase
{
    public:
    COracleDataBase();
    COracleDataBase(char* UserName,char* PassWord,char* DbName);
    ~COracleDataBase();
    public:
    bool ConnectDataBase();
    bool ConnectDataBase(const char* UserName,const char* PassWord,const char* DbName);
    bool ConnectDataBase(const char* UserName,const char* PassWord,const char* DbName,long *pHenv,long *pHdbc);   //�������ݿ�
    bool DisConnectDataBase();
    //��ѯ���ݿ�����
    long Select(const std::string Stmt,OdbRecordSet &rs,long &hpSelect/*Stmt handle*/,int once_rows=0);			 //��ѯ
    sword FetchNext(long hpSelect,OdbRecordSet &rs,int once_rows=1);    						 //ȡ���
    void ClearRecordSets(long hpSelect,OdbRecordSet &rs);						 //������
    //ִ��Sql���
    long SqlExecute(const std::string Stmt);								 //ִ��SQL���
    long Commit();                       							 //���Insert,Update,Delete�ύ
    long Rollback();										 //�ع�
    //ִ�а󶨲�����Sql���
    long SqlPrepare(const std::string Stmt,long &hpStmt);						 //׼��Sql���
    long DefineColumn(long hpSelect,OdbRecordSet &rs);						 //�����������
    long BindParam(long hpStmt,OdbParamArray &params);						 //�󶨲���
    long SqlExecuteWithParam(long hpStmt); 							 //ִ��Sql���
    void FreeParams(long hpStmt,OdbParamArray &rs);						 //�ͷž��������
    //�õ�������������ļ�¼��
    long SqlGetRecordCount(const std::string Stmt);
    long SqlGetRecordCount(const std::string Table,const std::string Where);
    //LOB�ֶεĴ�����
    long SqlGetLobLength(OdbField* pField);							//�õ�LOB�ֶ����ݳ���
    long SqlReadLob(OdbField* pParam,long Offset,long iRead,long &ReadCount);			//��LOB�ֶ�
    long SqlReadLob(OdbRecordSet &rs,long Offset,long iRead,long &ReadCount);			//��LOB�ֶ�
    long SqlWriteLob(OdbParam* pParam,long Offset,long iWrite,long &WriteCount);			//дLOB�ֶ�
    long SqlWriteLob(OdbParamArray &rs,long Offset,long iWrite,long &WriteCount);			//дLOB�ֶ�
    //
    long ReadCLob(OdbField* pField,StringArrary &sa);
	long ReadLobToFile(OdbField* pField,std::string FileName);
	long ReadLobToMem(OdbField* pField,unsigned char* &pBuf,int &iBufLen);
    long WriteCLob(OdbParam* pParam,StringArrary &sa);
    long WriteLobFromFile(OdbParam* pParam,std::string FileName);
	long WriteLobFromMem(OdbParam* pParam,unsigned char* pBuf,int iBufLen);
    //
    long GetFileLength(std::string FileName);
    long BindLongRaw(long hpStmt,OdbParamArray &params);
    long ExecLongRawFile(long hpStmt,OdbParam* pParam);
    long ReadLongRawToFile(long hpStmt,std::string Filename);
public:
    std::string GetErrorText();
    std::string MakeRecordIndex(int nSubNo, int nDevNo, int nCpuNo);								//�õ�������Ϣ
    std::string Format(const char* fmt,...);
    public:
    std::string m_UserName;
    std::string m_PassWord;
    std::string m_DbName;
    long  m_pHenv;
    long  m_pHdbc;
    private:
    std::string checkerr(OCIError* pErr, sword status,bool bPrint=true);
    private:
    OCIEnv *envhp;
    OCIError *errhp;
    OCIServer *srvhp;
    OCISvcCtx *svchp;
    OCISession *authp;
    std::string errText;
public:
	int m_iSelectRowsOnce;
};


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef WIN32
#pragma comment(lib,"oci.lib")
//#pragma comment(lib,"ociw32.lib")
#endif

#ifndef NULL
#define NULL 0
#endif

class SBASE_ORACLE_EXPORT SOracle  : public SDatabase
{
private:
	COracleDataBase* m_pConn;
	int m_iTestTimes;
	bool m_bInTrans;//�Ƿ���������

public:
	SOracle();
	virtual ~SOracle();
	COracleDataBase* GetConnectHandle(){return m_pConn;};

	virtual bool Connect();
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

	virtual bool ReadLongRawToFile(SString sTable,SString sLobField,SString sWhere,SString ysFile);
	virtual bool UpdateLongRawFromFile(SString sTable,SString sLobField,SString sWhere,SString sFile);

	//MEMO: ��δʵ�� [2016-4-5 15:36 �ۿ���]
	virtual bool ReadLobToMem(SString sTable,SString sLobField,SString sWhere,unsigned char* &pBuf,int &iBufLen);
	virtual bool UpdateLobFromMem(SString sTable,SString sLobField,SString sWhere,unsigned char* pBuf,int iBufLen);

	bool readLob(SString tableName,SString fieldName,SString whereField,SString recordIndex,SString &value);
	bool updateLob(SString tableName,SString fieldName,SString whereField,SString recordIndex,SString &value);

	SString getColumnType(SString tableName,SString fieldName);

};

#endif// _SKT_ORACLE_





