/**
 *
 * �� �� ��	: SDatabaseOper.h
 * ��������	: 2008-06-16
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸�����	: $Date: $
 * ��ǰ�汾	: $Revision: $
 * �������� : ���ݿ���������� ,֧���������ݿ�ͬ�������������ݿ�ͬʱ����ʱ��ͬ�����£�����һ������ʱ���򽫸���SQLд�����߿��ͬ�����У�����һ���ָ�ʱ����ͬ��
 *           �ڲ������������ݿ�ͬ�������̣߳����и���SQL����ͨ������������ܱ�֤ͬ�����������ñ������ݿ⣬��ͬ�������Զ��ر�
 *			 ͬ����ṹ�磺create table  t_dbsync_history ( oper_sn numeric(12,0) not null, sql_text varchar(1024) not null,constraint pk_dbsync_history primary key(oper_sn));
 * �޸ļ�¼ : 
 *            $Log: $
 *
 **/

#if !defined(AFX_SKTDATABASE_OPER_H__D6BE0D97_13A8_11D4_A5D2_002078B03530__INCLUDED_)
#define AFX_SKTDATABASE_OPER_H__D6BE0D97_13A8_11D4_A5D2_002078B03530__INCLUDED_

#include "SDatabase.h"
#include "SDateTime.h"
#include "SApi.h"
#include "SWait.h"
#include "../comm/STcpClientBase.h"
#include "../comm/STcpServerBase.h"

//////////////////////////////////////////////////////////////////////////
// ��    ��:  SDmlPublishServer
// ��    ��:  �ۿ���
// ����ʱ��:  2016-8-11 18:48
// ��    ��:  ���ݿ�DML��䶩�ķ�������
//////////////////////////////////////////////////////////////////////////
class SBASE_EXPORT SDmlPublishServer : public STcpServerBase
{
public:
	SDmlPublishServer()
	{
		SetStartWord(0xeb25);
	}
	virtual ~SDmlPublishServer()
	{

	}

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �»Ự�ص��麯��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-8-30 13:35
	// ����˵��:  @pSessionΪ�´����ĻỰʵ��
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void OnNewSession(STcpServerSession *pSession)
	{
		if(pSession->GetSessionSock() != NULL)
		{
			pSession->GetSessionSock()->SetRecvBufSize(1024*1024);
			pSession->GetSessionSock()->SetRecvBufSize(1024*1024*5);
			pSession->GetSessionSock()->SetRecvBufSize(1024*1024*10);
		}
	};

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��¼�ص��麯��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-8-30 10:35
	// ����˵��:  @ip��¼�ͻ��˵�IP
	//            @port��¼�ͻ��˵Ķ˿�
	//            @sLoginHead��¼�ַ���
	// �� �� ֵ:  true��ʾ�����¼,false��ʾ�ܾ���¼
	//////////////////////////////////////////////////////////////////////////
// 	virtual bool OnLogin(SString ip,int port,SString &sLoginHead)
// 	{
// 		if(SString::GetAttributeValue(sLoginHead,"type") == "dml_publish")
// 			return true;
// 		return false;
// 	};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  SQL�����麯�������ڻ�ȡDML���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-8-11 18:51
	// ����˵��:  @sqls����ʾSQL��DML����䣬���SQLͨ��{$SQL_SEP$}�ָ�
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void OnProcessSqls(SString &sqls)=0;

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �Ự�߳̽��յ����ĺ�Ļص��麯����������ͨ���˺�������ʱ��Ϣ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-8-11 18:48
	// ����˵��:  @pSessionΪ�Ựʵ��ָ�룬����ͨ��ָ����Զ˷�������
	//            @pPackageΪ�ս��յ������ݰ�
	// �� �� ֵ:  true��ʾ�Ѿ�������ϣ��˱��Ŀ����ͷ��ˣ�false��ʾδ�����˱���Ӧ������ն���
	//////////////////////////////////////////////////////////////////////////
	virtual bool OnRecvFrame(STcpServerSession *pSession,stuSTcpPackage *pPackage)
	{
		S_UNUSED(pSession);
		if(pPackage->m_wFrameType == 1 && pPackage->m_iAsduLen > 0)
		{
			SString str = SString::toString((char*)pPackage->m_pAsduBuffer,pPackage->m_iAsduLen);
			OnProcessSqls(str);
		}
		return true;
	}
};

class SBASE_EXPORT SDatabaseOper
{
public:

	class SBASE_EXPORT SDmlPubClient : public STcpClientBase
	{
	public:
		SDmlPubClient()
		{
			SetStartWord(0xeb25);
		}
		virtual ~SDmlPubClient()
		{

		}
		////////////////////////////////////////////////////////////////////////
		// ��    ��:  �Ự�߳̽��յ����ĺ�Ļص��麯����������ͨ���˺�������ʱ��Ϣ
		// ��    ��:  �ۿ���
		// ����ʱ��:  2016-8-11 18:43
		// ����˵��:  @pSessionΪ�Ựʵ��ָ�룬����ͨ��ָ����Զ˷�������
		//            @pPackageΪ�ս��յ������ݰ�
		// �� �� ֵ:  true��ʾ�Ѿ�������ϣ��˱��Ŀ����ͷ��ˣ�false��ʾδ�����˱���Ӧ������ն���
		//////////////////////////////////////////////////////////////////////////
		virtual bool OnRecvFrame(stuSTcpPackage *pPackage)
		{
			S_UNUSED(pPackage);
			return true;
		};
	};	

	SDatabaseOper();
	~SDatabaseOper();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �������ݿ����ӳ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-7-30 9:16
	// ����˵��:  @pPoolΪ�����ݿ����ӳ�
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void SetDatabasePool(SDatabasePool<SDatabase> *pPool);

	SDatabasePool<SDatabase>* GetDatabasePool(){return m_pDatabasePool;};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �豸�������ݿ����ӳ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-7-30 9:20
	// ����˵��:  @pPoolΪ�������ݿ����ӳ�
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void SetSlaveDatabasePool(SDatabasePool<SDatabase> *pPool);

	SDatabasePool<SDatabase>* GetSlaveDatabasePool(){return m_pSlaveDatabasePool;};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ�����ݿ�����ݿ�����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-8-26 14:40
	// ����˵��:  void
	// �� �� ֵ:  eSsp_Database_Type
	//////////////////////////////////////////////////////////////////////////
	inline e_SDatabase_Type GetDbType(){return m_pDatabasePool==NULL?DB_UNKNOWN:m_pDatabasePool->GetDatabaseByIdx(0)->GetDbType();};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ�����ݿ�����ݿ�����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-8-26 14:40
	// ����˵��:  void
	// �� �� ֵ:  eSsp_Database_Type
	//////////////////////////////////////////////////////////////////////////
	inline e_SDatabase_Type GetSlaveDbType(){return m_pSlaveDatabasePool==NULL?DB_UNKNOWN:m_pSlaveDatabasePool->GetDatabaseByIdx(0)->GetDbType();};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡָ��SQL�ļ�¼����ʧ�ܻط�<0
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-7-30 9:25
	// ����˵��:  @sqlΪSELECT���
	//         :  @rsΪ���÷��صļ�¼��
	// �� �� ֵ:  int,��ʾ���������=0��ʾ�޼�¼��<0��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	int RetrieveRecordset(SString sql,SRecordset &rs);
	inline int Retrieve(SString sql,SRecordset &rs){return RetrieveRecordset(sql,rs);};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����ָ��SELECT���������0�е�col�е�ֵ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-7-30 9:27
	// ����˵��:  @sqlΪSELECT���
	//         :  @colΪ�кţ���0��ʼ
	// �� �� ֵ:  SString/int/float
	//////////////////////////////////////////////////////////////////////////
	SString SelectInto(SString sql,int col=0);
	int SelectIntoI(SString sql,int col=0);
	float SelectIntoF(SString sql,int col=0);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ִ��SQL
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-7-30 9:29
	// ����˵��:  @sqlΪ����SQL��䣬��update/delete/truncate/drop/...
	// �� �� ֵ:  �ɹ�����true,ʧ�ܷ���false
	//////////////////////////////////////////////////////////////////////////
	bool ExecuteSQL(SString sql);
	inline bool Execute(SString sql){return ExecuteSQL(sql);};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �������ݿ�ͬ������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-7-30 9:32
	// ����˵��:  @bGlobalSync��ʾ�Ƿ���ȫ�ֿ�ͬ����true��ʾ�򿪣�false��ʾ�ر�
	//         :  һ������£�һ��ϵͳ�н���Ҫһ�����̿�����ͬ�����ý���Ӧȷ�����ڴ�������״̬
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool Start(bool bGlobalSync=false);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �˳����ݿ�ͬ������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-7-30 9:32
	// ����˵��:  void
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool Quit();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ͨ���ļ����´��ֶ�����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-7-30 9:54
	// ����˵��:  @sTableΪ�����µı���
	//         :  @sLobFieldsΪ���ֶε��ֶ���
	//         :  @sWhere��ʾΨһȷ�������е�������������������˳�Ψһ��һ�м�¼
	//         :  @sFileΪ�����ļ���ȫ·���������ϴ��Ĵ��ֶ�����
	// �� �� ֵ:  true��ʾ�ɹ���false��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	bool UpdateLobFromFile(SString sTable,SString sLobField,SString sWhere,SString sFile);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ָ���Ĵ��ֶζ�ȡ���ݲ�������ļ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-7-30 9:58
	// ����˵��:  @sTableΪ�����µı���
	//         :  @sLobFieldsΪ���ֶε��ֶ���
	//         :  @sWhere��ʾΨһȷ�������е�������������������˳�Ψһ��һ�м�¼
	//         :  @sFileΪ�����ļ���ȫ·������ȡ�������ݽ�д����ļ�
	// �� �� ֵ:  true��ʾ�ɹ���false��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	bool ReadLobToFile(SString sTable,SString sLobField,SString sWhere,SString sFile);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ָ���Ĵ��ֶζ�ȡ���ݲ�������ļ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-4 14:31
	// ����˵��:  @sTableΪ�����µı���
	//         :  @sLobFieldsΪ���ֶε��ֶ���
	//         :  @sWhere��ʾΨһȷ�������е�������������������˳�Ψһ��һ�м�¼
	//         :  @pBufΪ���÷��صĻ�������ַ��������ʹ�ú�Ӧͨ��delete[]�ͷ�
	//         :  @iBufLenΪ���÷��صĻ���������
	// �� �� ֵ:  true��ʾ�ɹ���false��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	bool ReadLobToMem(SString sTable,SString sLobField,SString sWhere,unsigned char* &pBuf,int &iBufLen);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����ͷ�ReadLobToMem���������Ļ�����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-12-1 11:01
	// ����˵��:  @pBufΪ������ָ��
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void FreeLobMem(unsigned char* pBuf);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���ڴ���ֶ�д�����ݿ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-29 17:43
	// ����˵��:  
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool UpdateLobFromMem(SString sTable,SString sLobField,SString sWhere,unsigned char* pBuf,int iBufLen);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����DML����ʵʱ����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-8-11 17:16
	// ����˵��:  
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	bool StartDmlPublish(SString sIp,int iTcpPort);

private:
	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���ݿ�ͬ���߳�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-7-30 9:31
	// ����˵��:  @lpΪthis
	// �� �� ֵ:  NULL
	//////////////////////////////////////////////////////////////////////////
	static void* ThreadDbSync(void* lp);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  DML���ķ�������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-8-11 17:12
	// ����˵��:  @lpΪthis
	// �� �� ֵ:  NULL
	//////////////////////////////////////////////////////////////////////////
	static void* ThreadDmlPublish(void* lp);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����ʷ����SQL��¼��ָ�����ӳض�Ӧ����һ�����ӳأ�����һ����Ϊ��ʱ���Դ˲���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-7-30 10:50
	// ����˵��:  @pThisPool��ʾ��ǰ��
	//         :  @sqlΪ����SQL
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void AddHistoryOperSql(SDatabasePool<SDatabase> *pThisPool,SString &sql);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ�����ָ�����ӵĵڶ������ӣ�����������������뱸�����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-7-30 10:58
	// ����˵��:  @pThisPool��ʾ����ָ��
	// �� �� ֵ:  SDatabasePool<SDatabase> *
	//////////////////////////////////////////////////////////////////////////
	inline SDatabasePool<SDatabase> *GetSecondPool(SDatabasePool<SDatabase> *pThisPool)
	{
		return (pThisPool==m_pDatabasePool?m_pSlaveDatabasePool:m_pDatabasePool);
	}

	SDatabasePool<SDatabase> *m_pDatabasePool;		//�����ݿ����ӳ�
	SDatabasePool<SDatabase> *m_pSlaveDatabasePool;	//�������ݿ����ӳ�
	SDatabasePool<SDatabase> *m_pActionDatabasePool;//������ݿ����ӳأ�Ϊ���������ӳ�֮һ�����������ӳأ������ӳ��쳣ʱ�л�Ϊ�����ӳأ����ָ�ʱ��ָ�Ϊ��
	bool m_bQuit;									//�Ƿ��˳�����
	SStringList m_DbSyncSqls;						//�����ݿ�ͬ������SQL��UPDATE_BLOB=��ͷ�ı�ʾ���ֶ�ͬ�������������ֶ����Դ���table=XX{$SEP$}field=XX{$SEP$}where=XX{$SEP$}���ӱ���ȡ�ļ����õ�����
	SStringList m_SlaveDbSyncSqls;					//�������ݿ�ͬ������SQL��ͬ��
	SWait m_Wait;									//ͬ���ȴ��ź�
	bool m_bGlobalSync;								//��ʾ�Ƿ���ȫ�ֿ�ͬ����true��ʾ�򿪣�false��ʾ�ر�
													//һ������£�һ��ϵͳ�н���Ҫһ�����̿�����ͬ�����ý���Ӧȷ�����ڴ�������״̬
	SString m_sDmlPublishServerIp;					//����������IP��ַ
	int m_iDmlPublishServerTcpPort;					//����������TCP�˿ں�
	SPtrList<SString> m_sDmlSqlBuffered;			//��ǰ�����SQL
	bool m_bDmlPublishThreadRuning;					//DML�����߳��Ƿ���������
};


#endif // !defined(AFX_SKTDATABASE_OPER_H__D6BE0D97_13A8_11D4_A5D2_002078B03530__INCLUDED_)
