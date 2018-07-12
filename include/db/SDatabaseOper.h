/**
 *
 * 文 件 名	: SDatabaseOper.h
 * 创建日期	: 2008-06-16
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期	: $Date: $
 * 当前版本	: $Revision: $
 * 功能描述 : 数据库操作助手类 ,支持主备数据库同步，当主备数据库同时在线时则同步更新，当有一个离线时，则将更新SQL写入在线库的同步表中，待另一个恢复时进行同步
 *           内部负责启动数据库同步服务线程，所有更新SQL必须通过本类操作方能保证同步，若不配置备用数据库，则同步机制自动关闭
 *			 同步表结构如：create table  t_dbsync_history ( oper_sn numeric(12,0) not null, sql_text varchar(1024) not null,constraint pk_dbsync_history primary key(oper_sn));
 * 修改记录 : 
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
// 名    称:  SDmlPublishServer
// 作    者:  邵凯田
// 创建时间:  2016-8-11 18:48
// 描    述:  数据库DML语句订阅服务器类
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
	// 描    述:  新会话回调虚函数
	// 作    者:  邵凯田
	// 创建时间:  2016-8-30 13:35
	// 参数说明:  @pSession为新创建的会话实例
	// 返 回 值:  void
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
	// 描    述:  登录回调虚函数
	// 作    者:  邵凯田
	// 创建时间:  2016-8-30 10:35
	// 参数说明:  @ip登录客户端的IP
	//            @port登录客户端的端口
	//            @sLoginHead登录字符串
	// 返 回 值:  true表示允许登录,false表示拒绝登录
	//////////////////////////////////////////////////////////////////////////
// 	virtual bool OnLogin(SString ip,int port,SString &sLoginHead)
// 	{
// 		if(SString::GetAttributeValue(sLoginHead,"type") == "dml_publish")
// 			return true;
// 		return false;
// 	};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  SQL处理虚函数，用于获取DML语句
	// 作    者:  邵凯田
	// 创建时间:  2016-8-11 18:51
	// 参数说明:  @sqls，表示SQL（DML）语句，多个SQL通过{$SQL_SEP$}分隔
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void OnProcessSqls(SString &sqls)=0;

	////////////////////////////////////////////////////////////////////////
	// 描    述:  会话线程接收到报文后的回调虚函数，派生类通过此函数处理即时消息
	// 作    者:  邵凯田
	// 创建时间:  2016-8-11 18:48
	// 参数说明:  @pSession为会话实例指针，可以通过指针向对端发送数据
	//            @pPackage为刚接收到的数据包
	// 返 回 值:  true表示已经处理完毕，此报文可以释放了，false表示未处理，此报文应放入接收队列
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
		// 描    述:  会话线程接收到报文后的回调虚函数，派生类通过此函数处理即时消息
		// 作    者:  邵凯田
		// 创建时间:  2016-8-11 18:43
		// 参数说明:  @pSession为会话实例指针，可以通过指针向对端发送数据
		//            @pPackage为刚接收到的数据包
		// 返 回 值:  true表示已经处理完毕，此报文可以释放了，false表示未处理，此报文应放入接收队列
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
	// 描    述:  设置数据库连接池
	// 作    者:  邵凯田
	// 创建时间:  2015-7-30 9:16
	// 参数说明:  @pPool为主数据库连接池
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void SetDatabasePool(SDatabasePool<SDatabase> *pPool);

	SDatabasePool<SDatabase>* GetDatabasePool(){return m_pDatabasePool;};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  设备备用数据库连接池
	// 作    者:  邵凯田
	// 创建时间:  2015-7-30 9:20
	// 参数说明:  @pPool为备用数据库连接池
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void SetSlaveDatabasePool(SDatabasePool<SDatabase> *pPool);

	SDatabasePool<SDatabase>* GetSlaveDatabasePool(){return m_pSlaveDatabasePool;};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取主数据库的数据库类型
	// 作    者:  邵凯田
	// 创建时间:  2017-8-26 14:40
	// 参数说明:  void
	// 返 回 值:  eSsp_Database_Type
	//////////////////////////////////////////////////////////////////////////
	inline e_SDatabase_Type GetDbType(){return m_pDatabasePool==NULL?DB_UNKNOWN:m_pDatabasePool->GetDatabaseByIdx(0)->GetDbType();};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取从数据库的数据库类型
	// 作    者:  邵凯田
	// 创建时间:  2017-8-26 14:40
	// 参数说明:  void
	// 返 回 值:  eSsp_Database_Type
	//////////////////////////////////////////////////////////////////////////
	inline e_SDatabase_Type GetSlaveDbType(){return m_pSlaveDatabasePool==NULL?DB_UNKNOWN:m_pSlaveDatabasePool->GetDatabaseByIdx(0)->GetDbType();};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取指定SQL的记录集，失败回返<0
	// 作    者:  邵凯田
	// 创建时间:  2015-7-30 9:25
	// 参数说明:  @sql为SELECT语句
	//         :  @rs为引用返回的记录集
	// 返 回 值:  int,表示结果数量，=0表示无记录，<0表示失败
	//////////////////////////////////////////////////////////////////////////
	int RetrieveRecordset(SString sql,SRecordset &rs);
	inline int Retrieve(SString sql,SRecordset &rs){return RetrieveRecordset(sql,rs);};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  返回指定SELECT语句结果集第0行第col列的值
	// 作    者:  邵凯田
	// 创建时间:  2015-7-30 9:27
	// 参数说明:  @sql为SELECT语句
	//         :  @col为列号，从0开始
	// 返 回 值:  SString/int/float
	//////////////////////////////////////////////////////////////////////////
	SString SelectInto(SString sql,int col=0);
	int SelectIntoI(SString sql,int col=0);
	float SelectIntoF(SString sql,int col=0);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  执行SQL
	// 作    者:  邵凯田
	// 创建时间:  2015-7-30 9:29
	// 参数说明:  @sql为更新SQL语句，如update/delete/truncate/drop/...
	// 返 回 值:  成功返回true,失败返回false
	//////////////////////////////////////////////////////////////////////////
	bool ExecuteSQL(SString sql);
	inline bool Execute(SString sql){return ExecuteSQL(sql);};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  启动数据库同步服务
	// 作    者:  邵凯田
	// 创建时间:  2015-7-30 9:32
	// 参数说明:  @bGlobalSync表示是否开启全局库同步，true表示打开，false表示关闭
	//         :  一般情况下，一个系统中仅需要一个进程开启该同步，该进程应确保长期处于运行状态
	// 返 回 值:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool Start(bool bGlobalSync=false);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  退出数据库同步服务
	// 作    者:  邵凯田
	// 创建时间:  2015-7-30 9:32
	// 参数说明:  void
	// 返 回 值:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool Quit();

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  通过文件更新大字段内容
	// 作    者:  邵凯田
	// 创建时间:  2015-7-30 9:54
	// 参数说明:  @sTable为待更新的表名
	//         :  @sLobFields为大字段的字段名
	//         :  @sWhere表示唯一确定更新行的条件，该条件必须过滤出唯一的一行记录
	//         :  @sFile为本地文件名全路径，即待上传的大字段内容
	// 返 回 值:  true表示成功，false表示失败
	//////////////////////////////////////////////////////////////////////////
	bool UpdateLobFromFile(SString sTable,SString sLobField,SString sWhere,SString sFile);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  从指定的大字段读取内容并输出到文件
	// 作    者:  邵凯田
	// 创建时间:  2015-7-30 9:58
	// 参数说明:  @sTable为待更新的表名
	//         :  @sLobFields为大字段的字段名
	//         :  @sWhere表示唯一确定更新行的条件，该条件必须过滤出唯一的一行记录
	//         :  @sFile为本地文件名全路径，读取到的内容将写入该文件
	// 返 回 值:  true表示成功，false表示失败
	//////////////////////////////////////////////////////////////////////////
	bool ReadLobToFile(SString sTable,SString sLobField,SString sWhere,SString sFile);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  从指定的大字段读取内容并输出到文件
	// 作    者:  邵凯田
	// 创建时间:  2015-8-4 14:31
	// 参数说明:  @sTable为待更新的表名
	//         :  @sLobFields为大字段的字段名
	//         :  @sWhere表示唯一确定更新行的条件，该条件必须过滤出唯一的一行记录
	//         :  @pBuf为引用返回的缓冲区地址，调用者使用后应通过delete[]释放
	//         :  @iBufLen为引用返回的缓冲区长度
	// 返 回 值:  true表示成功，false表示失败
	//////////////////////////////////////////////////////////////////////////
	bool ReadLobToMem(SString sTable,SString sLobField,SString sWhere,unsigned char* &pBuf,int &iBufLen);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  用于释放ReadLobToMem函数产生的缓冲区
	// 作    者:  邵凯田
	// 创建时间:  2017-12-1 11:01
	// 参数说明:  @pBuf为缓冲区指针
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void FreeLobMem(unsigned char* pBuf);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  将内存大字段写入数据库
	// 作    者:  邵凯田
	// 创建时间:  2015-8-29 17:43
	// 参数说明:  
	// 返 回 值:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool UpdateLobFromMem(SString sTable,SString sLobField,SString sWhere,unsigned char* pBuf,int iBufLen);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  开启DML语句的实时发布
	// 作    者:  邵凯田
	// 创建时间:  2016-8-11 17:16
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	bool StartDmlPublish(SString sIp,int iTcpPort);

private:
	//////////////////////////////////////////////////////////////////////////
	// 描    述:  数据库同步线程
	// 作    者:  邵凯田
	// 创建时间:  2015-7-30 9:31
	// 参数说明:  @lp为this
	// 返 回 值:  NULL
	//////////////////////////////////////////////////////////////////////////
	static void* ThreadDbSync(void* lp);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  DML语句的发布进程
	// 作    者:  邵凯田
	// 创建时间:  2016-8-11 17:12
	// 参数说明:  @lp为this
	// 返 回 值:  NULL
	//////////////////////////////////////////////////////////////////////////
	static void* ThreadDmlPublish(void* lp);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  添加历史操作SQL记录到指定连接池对应的另一个连接池，当另一个池为空时忽略此操作
	// 作    者:  邵凯田
	// 创建时间:  2015-7-30 10:50
	// 参数说明:  @pThisPool表示当前池
	//         :  @sql为更新SQL
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void AddHistoryOperSql(SDatabasePool<SDatabase> *pThisPool,SString &sql);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取相对于指定连接的第二个连接，输入主输出备，输入备输出主
	// 作    者:  邵凯田
	// 创建时间:  2015-7-30 10:58
	// 参数说明:  @pThisPool表示连接指针
	// 返 回 值:  SDatabasePool<SDatabase> *
	//////////////////////////////////////////////////////////////////////////
	inline SDatabasePool<SDatabase> *GetSecondPool(SDatabasePool<SDatabase> *pThisPool)
	{
		return (pThisPool==m_pDatabasePool?m_pSlaveDatabasePool:m_pDatabasePool);
	}

	SDatabasePool<SDatabase> *m_pDatabasePool;		//主数据库连接池
	SDatabasePool<SDatabase> *m_pSlaveDatabasePool;	//备用数据库连接池
	SDatabasePool<SDatabase> *m_pActionDatabasePool;//活动的数据库连接池，为主、备连接池之一，优先主连接池，主连接池异常时切换为备连接池，主恢复时则恢复为主
	bool m_bQuit;									//是否退出服务
	SStringList m_DbSyncSqls;						//主数据库同步更新SQL，UPDATE_BLOB=开头的表示大字段同步，后面跟随大字段属性串：table=XX{$SEP$}field=XX{$SEP$}where=XX{$SEP$}，从备库取文件设置到主库
	SStringList m_SlaveDbSyncSqls;					//备用数据库同步更新SQL，同上
	SWait m_Wait;									//同步等待信号
	bool m_bGlobalSync;								//表示是否开启全局库同步，true表示打开，false表示关闭
													//一般情况下，一个系统中仅需要一个进程开启该同步，该进程应确保长期处于运行状态
	SString m_sDmlPublishServerIp;					//发布服务器IP地址
	int m_iDmlPublishServerTcpPort;					//发布服务器TCP端口号
	SPtrList<SString> m_sDmlSqlBuffered;			//当前缓存的SQL
	bool m_bDmlPublishThreadRuning;					//DML发布线程是否正在运行
};


#endif // !defined(AFX_SKTDATABASE_OPER_H__D6BE0D97_13A8_11D4_A5D2_002078B03530__INCLUDED_)
