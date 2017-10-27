/**
 *
 * �� �� �� : MdbClient.h
 * �������� : 2014-4-22 13:10
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : �ڴ����ݿ�ķ��ʿͻ���
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2014-4-22	�ۿ�������ļ�
 *
 **/

#ifndef SKT_MDB_CLIENT
#define SKT_MDB_CLIENT

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../SObject.h"
#include "../../SSocket.h"
#include "../../SString.h"
#include "../../SApi.h"
#include "../../comm/STcpClientBase.h"
#include "../../db/SDatabase.h"
#include "../../SList.h"

//��ȡ���ݼ�֧�ֵ�����¼��
#define C_MDB_MAX_RETRIEVE_ROWS 100000

//////////////////////////////////////////////////////////////////////////
// ��    ��:  eMdbReturnCode
// ��    ��:  �ۿ���
// ����ʱ��:  2014-04-23 12:11
// ��    ��:  MDBMS���ڴ�ʽ���ݿ����ϵͳ���ķ����붨��
//////////////////////////////////////////////////////////////////////////
enum SBASE_MDB_EXPORT eMdbReturnCode
{
	MDB_RET_SUCCESS = 0,
	MDB_RET_TABLE_NOTEXIST = -1099,	//������
	MDB_RET_SYNTAX_ERROR,			//�﷨����
	MDB_RET_SQL_NOTSUPPORT,			//SQL��֧��
	MDB_RET_OBJ_EXIST,				//�����Ѿ�����
	MDB_RET_SYSTAB_ERR,				//ϵͳ�����
	MDB_RET_DISCONNECT,				//���ӶϿ�
	MDB_RET_INDEX_EXIST,			//�����Ѿ�����
	MDB_RET_FIELD_NOTEXIST,			//�ֶβ�����
	MDB_RET_RETRIEVE_OVERFLOW,		//��ȡ���ݼ����
	MDB_RET_COMM_ERROR,				//ͨѶ����
	MDB_RET_TYPE_ERROR,				//���ʹ���
	MDB_RET_DISK_ERROR,				//���̴���
	MDB_RET_CONTEXT_NOTEXIST,		//���ݲ�����
	MDB_RET_UNIQUE_VIOLATION,		//Υ��Ψһ��Լ��
	MDB_RET_DATA_OVERFLOW,			//�������
	MDB_RET_UNKNOWN,				//δ֪����
};

enum SBASE_MDB_EXPORT eMdbFrameType
{
	MDB_FT_INSERT=1,				//����
	MDB_FT_SELECT,					//��ѯ
	MDB_FT_EXECUTE,					//ִ��
	MDB_FT_TRIGGER,					//��������ͻ��˵Ĵ���֪ͨ
	MDB_FT_CONNTRG,					//�ͻ����������ע�ᴥ��֪ͨ
	MDB_FT_DISCONNTRG,				//�ͻ�����������Ͽ�����֪ͨ
	MDB_FT_FIELDS,					//ȡ�ֶ���Ϣ
	MDB_FT_UPDATE_INSERT,			//���»�����¼(������ΪΨһ��ʶ���������ı�����ʹ��)
	MDB_FT_UPDATE_BLOB,				//���´��ֶ�
	MDB_FT_READ_BLOB,				//��ȡ���ֶ�
};

enum SBASE_MDB_EXPORT eMdbTriggerType
{
	MDB_TRG_INSERT=1,				//���봥��
	MDB_TRG_UPDATE,					//���´���
	MDB_TRG_DELETE,					//ɾ������
	MDB_TRG_TRUNCATE,				//��մ���
	MDB_TRG_UPDATE_WITHPK,			//ͨ���������еĸ��£�ֻ��Ϊ��ʷ�������ͣ������ڿͻ��˴���������ͬMDB_TRG_UPDATE��
};

enum SBASE_MDB_EXPORT eMdbTriggerFlag
{
	MDB_TRGFLG_INSERT=1,				//���봥��
	MDB_TRGFLG_UPDATE=2,				//���´���
	MDB_TRGFLG_DELETE=4,				//ɾ������
	MDB_TRGFLG_TRUNCATE=8,				//��մ���
	MDB_TRGFLG_ALL=0x0F,				//���д���
};

enum SBASE_MDB_EXPORT eMdbFieldType
{
	MDB_INT = 1,//4 bytes
	MDB_INT64,	//8 bytes
	MDB_FLOAT,	//4 bytes
	MDB_SHORT,	//2 bytes
	MDB_BYTE,	//1 bytes
	MDB_STR,	//N bytes���ɽ����﷨������
	MDB_BLOB,	//4�ֽڣ��ļ���OID��
};


#pragma pack(push)  
#pragma pack(1)

//////////////////////////////////////////////////////////////////////////
// ��    ��:  stuSysTable
// ��    ��:  �ۿ���
// ����ʱ��:  2014-04-24 11:20
// ��    ��:  ϵͳ��ṹ
//////////////////////////////////////////////////////////////////////////
struct SBASE_MDB_EXPORT stuSysTable
{
	char name[32];	//������
	int  rows;				//�ܼ�¼��
	int  rowsize;			//һ����¼���ܳ���
};

//////////////////////////////////////////////////////////////////////////
// ��    ��:  stuSysTable
// ��    ��:  �ۿ���
// ����ʱ��:  2014-04-24 11:20
// ��    ��:  ϵͳ��ṹ
//////////////////////////////////////////////////////////////////////////
struct SBASE_MDB_EXPORT stuTableField
{
	char name[32];
	int type;//1:int 2:int64 3:float 4:short 5:char 6:char*
	short start_pos;
	short bytes;
	BYTE is_pk;
	BYTE is_null;
	WORD reverse;
	char defval[16];
};

#pragma pack()
#pragma pack(pop)  

//�������ݶ���
typedef BYTE* (*OnMdbClientTrgCallback)(void* cbParam, SString &sTable, eMdbTriggerType eType,int iTrgRows,int iRowSize,BYTE *pTrgData);

//////////////////////////////////////////////////////////////////////////
// ��    ��:  CMdbClient
// ��    ��:  �ۿ���
// ����ʱ��:  2014-04-24 22:01
// ��    ��:  �Զ����ڴ滷�����ݿ�Ĳ�����
//////////////////////////////////////////////////////////////////////////
class SBASE_MDB_EXPORT CMdbClient : public STcpClientBase
{
public:
	struct SBASE_MDB_EXPORT stuMdbClientTriggerReg
	{
		OnMdbClientTrgCallback pFun;
		void* cbParam;
		SString sTableName;
		BYTE iTriggerFlag;//bit0:insert; 1:update; 2:delete; 3:truncate,  eMdbTriggerFlag
	};
	CMdbClient();
	virtual ~CMdbClient();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �������ݿ������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-04-24 22:41
	// ����˵��:  @ipΪ���ݿ�IP��ַ
	//         :  @portΪ���ݿ�TCP����˿�
	//         :  @dbuserΪ�û���
	//         :  @dbpwdΪ����
	// �� �� ֵ:  true��ʾ��¼�ɹ���false��ʾ��¼ʧ��
	//////////////////////////////////////////////////////////////////////////
	bool Connect(SString ip,int port,SString dbuser,SString dbpwd);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �Ͽ����ݿ�����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-04-24 22:42
	// ����˵��:  void
	// �� �� ֵ:  true�Ͽ��ɹ���falseʧ��
	//////////////////////////////////////////////////////////////////////////
	bool DisConnect();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����µ�һ����¼
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-04-23 12:52
	// ����˵��:  @sTableNameΪ����
	//         :  @pRowData��ʾ��¼���ݣ����뽨��ʱ����һ�µ����м�¼
	//         :  @iRowSize��ʾһ����¼�ĳ���
	//         :  @iRows��ʾ��ǰһ���ж�������¼�����룬������¼�����洢��pRowData��
	// �� �� ֵ:  >0��ʾ�ɹ�����ļ�¼����<0��ʾʧ�ܷ�����
	//////////////////////////////////////////////////////////////////////////
	int Insert(const char* sTableName, void *pRowData, int iRowSize, int iRows);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���»����һ����¼��������ΪΨһ��ʶ�������¼���ڸ��£������������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-12-17 12:52
	// ����˵��:  @sTableNameΪ����
	//         :  @pRowData��ʾ��¼���ݣ����뽨��ʱ����һ�µ����м�¼
	//         :  @iRowSize��ʾһ����¼�ĳ���
	//         :  @iRows��ʾ��ǰһ���ж�������¼�����룬������¼�����洢��pRowData��
	// �� �� ֵ:  >0��ʾ�ɹ�����ļ�¼����<0��ʾʧ�ܷ�����
	//////////////////////////////////////////////////////////////////////////
	int UpdateWithInsert(const char* sTableName, void *pRowData, int iRowSize, int iRows);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ִ��SQL��䣬�ɴ�����ɾ�������������������¼�����¼�¼��ɾ����¼��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-04-23 12:05
	// ����˵��:  @sqlΪ��ִ�е�SQL��䣬֧�ּ��﷨
	// �� �� ֵ:  >=0��ʾ�ɹ��Լ�Ӱ���������<0��ʾʧ�ܷ�����
	//////////////////////////////////////////////////////////////////////////
	int Execute(SString &sql);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ѯָ��SQL�������ݼ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-04-23 12:07
	// ����˵��:  @sqlΪ��ִ�е�SQL��䣬֧�ּ��﷨
	//         :  @iRowSize��ʾһ�м�¼���ܳ���(���÷��أ�
	//         :  @pRowDataΪ��¼�����飬�ܳ���Ϊ��¼��*iRowSize������ָ�벻ΪNULLʱ��ʹ����Ӧ�����ͷ�����
	// �� �� ֵ:  >=0��ʾ�ɹ����صļ�¼����<0��ʾʧ�ܷ�����
	//			  ��select count(*),min(f1)... ʱ����1����pRowDataΪNULL��SQL��Ϊ��\t�ָ��Ľ���ַ���
	//////////////////////////////////////////////////////////////////////////
	int Retrieve(SString &sql,int &iRowSize,void* &pRowData);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ���Ĵ�������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-04-24 22:05
	// ����˵��:  void
	// �� �� ֵ:  SString
	//////////////////////////////////////////////////////////////////////////
	inline SString GetLastError(){return m_sLastError;};
#ifndef WIN32
	inline void SetLastError(const char* sErr){ strcpy(m_sLastError, sErr); };
#else
	inline void SetLastError(const char* sErr){ strcpy_s(m_sLastError, sizeof(m_sLastError), sErr); };
#endif


	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �Ự�߳̽��յ����ĺ�Ļص��麯����������ͨ���˺�������ʱ��Ϣ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014:4:27 21:45
	// ����˵��:  @pSessionΪ�Ựʵ��ָ�룬����ͨ��ָ����Զ˷�������
	//            @pPackageΪ�ս��յ������ݰ�
	// �� �� ֵ:  true��ʾ�Ѿ�������ϣ��˱��Ŀ����ͷ��ˣ�false��ʾδ�����˱���Ӧ������ն���
	//////////////////////////////////////////////////////////////////////////
	virtual bool OnRecvFrame(stuSTcpPackage *pPackage);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���������MDB������ȡ��Ӧ�ķ�������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-5-19 13:42
	// ����˵��:  @retΪ����������
	// �� �� ֵ:  ��Ӧ�����ַ��� 
	//////////////////////////////////////////////////////////////////////////
	static SString GetRetCodeText(int ret);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ע�ᴥ���ص���������ע�����ص�����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-10-18 15:46
	// ����˵��:  @pFunΪ�ص�������ַ
	//			  @cbParamΪ�ص�����
	//            @sTableNameΪ�����ı���
	//            @iTriggerFlagΪ�������//bit0:insert; 1:update; 2:delete; 3:truncate,  eMdbTriggerFlag
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void RegisterTriggerCallback(OnMdbClientTrgCallback pFun, void *cbParam, SString sTableName, BYTE iTriggerFlag);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ɾ�������ص�����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-10-18 16:03
	// ����˵��:  @pFunΪ�ص�������ַ
	//			  @cbParamΪ�ص�����
	//            @sTableNameΪ�����ı���
	//            @iTriggerFlagΪ�������//bit0:insert; 1:update; 2:delete; 3:truncate,  eMdbTriggerType-1
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void RemoveTriggerCallback(OnMdbClientTrgCallback pFun, void *cbParam, SString sTableName, BYTE iTriggerFlag);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ɾ�����д����ص�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-5-11 16:46
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void RemoveAllTrigger();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��ǰ�����������ݱ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-11-13 13:11
	// ����˵��:  @SysTablesΪ���÷��ص�ϵͳ���б�
	// �� �� ֵ:  >=0��ʾ���ݱ�������<0��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	int GetSysTables(SPtrList<stuSysTable> &SysTables);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡָ�����ݱ��е��ֶ���Ϣ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-11-13 13:49
	// ����˵��:  @sTable����
	//			  @TableFieldsΪ���÷��ص��ֶ���Ϣ�б�
	// �� �� ֵ:  >=0��ʾ�ֶ�������<0��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	int GetTableFields(const char* sTable, SPtrList<stuTableField> &TableFields);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��¼��ָ����ŵ��ֶ��ַ���ֵ����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-11-14 10:46
	// ����˵��:  @pRowΪ��¼����
	//			  @pFldΪ�ֶ���Ϣ
	// �� �� ֵ:  �ַ�����ֵ����
	//////////////////////////////////////////////////////////////////////////
	static SString GetValueStr(BYTE *pRow, stuTableField *pFld);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ͨ�����ݼ������������ڴ�ṹ��ֵ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-3-24 10:08
	// ����˵��:  @pRows��ʾ�����еĻ�����
	//         :  @iRows��ʾ������������
	//         :  @iRowSize��ʾ�г���
	//         :  @pRs��ʾ���ݼ�ָ��
	//         :  @TableFields��ʾ���ֶ�����,�ֶ�������˳�������pRs���ݼ��е�һ��
	// �� �� ֵ:  ���سɹ����Ƶ���Ч����
	//////////////////////////////////////////////////////////////////////////
	static int SetValueFromRs(BYTE *pRows,int iRows,int iRowSize,SRecordset *pRs,SPtrList<stuTableField> &TableFields);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���ļ��������ݿ���ָ����¼��ָ�����ֶ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-12-22 15:16
	// ����˵��:  @sTable����
	//			  @sLobFieldΪ�ֶ���
	//			  @sWhere������Ψһȷ��һ����¼)
	//			  @sFileΪ�����ļ���
	// �� �� ֵ:  >0��ʾ�ɹ���=0��ʾδ�ҵ���Ӧ��¼��<0��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	int UpdateBlobFromFile(SString sTable, SString sLobField, SString sWhere, SString sFile);
	int UpdateBlobFromMem(SString sTable, SString sLobField, SString sWhere, BYTE *pBuf,int iBufLen);
	
	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����ݿ��ж�ȡָ����¼��ָ�����ֶε������ļ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-12-22 15:26
	// ����˵��:  @sTable����
	//			  @sLobFieldΪ�ֶ���
	//			  @sWhere������Ψһȷ��һ����¼)
	//			  @sFileΪ�����ļ���
	// �� �� ֵ:  >0��ʾ�ɹ���=0��ʾδ�ҵ���Ӧ��¼��<0��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	int ReadBlobToFile(SString sTable, SString sLobField, SString sWhere, SString sFile);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����ݿ��ж�ȡָ����¼��ָ�����������ɵ����߸����ͷ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-12-22 15:26
	// ����˵��:  @sTable����
	//			  @sLobFieldΪ�ֶ���
	//			  @sWhere������Ψһȷ��һ����¼)
	//			  @pBlobBuf��ʾ���ֶ����ݻ�����
	//		      @iBlobLen��ʾ���ֶ��ֽ���
	// �� �� ֵ:  >0��ʾ�ɹ���=0��ʾδ�ҵ���Ӧ��¼��<0��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	int ReadBlobToMem(SString sTable, SString sLobField, SString sWhere, BYTE* &pBlobBuf, int &iBlobLen);

protected:
	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��¼���ڴ����ݿ�����ʱ�Ļص���������Ҫ���ڶϿ�������ִ������ע�ᴥ���ص�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-7 16:35
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void OnLogin();

private:
	SString m_sIp;
	int m_iPort;
	char m_sLastError[1024];
	//stuSTcpPackage* m_pRecvPackage;
	SPtrList<stuMdbClientTriggerReg> *m_TrgReg;
};

#endif // !defined(SKT_MDB_CLIENT)
