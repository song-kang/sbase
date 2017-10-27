/**
 *
 * 文 件 名 : MdbClient.h
 * 创建日期 : 2014-4-22 13:10
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : 内存数据库的访问客户端
 * 修改记录 : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2014-4-22	邵凯田　创建文件
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

//读取数据集支持的最大记录数
#define C_MDB_MAX_RETRIEVE_ROWS 100000

//////////////////////////////////////////////////////////////////////////
// 名    称:  eMdbReturnCode
// 作    者:  邵凯田
// 创建时间:  2014-04-23 12:11
// 描    述:  MDBMS（内存式数据库管理系统）的返回码定义
//////////////////////////////////////////////////////////////////////////
enum SBASE_MDB_EXPORT eMdbReturnCode
{
	MDB_RET_SUCCESS = 0,
	MDB_RET_TABLE_NOTEXIST = -1099,	//表不存在
	MDB_RET_SYNTAX_ERROR,			//语法错误
	MDB_RET_SQL_NOTSUPPORT,			//SQL不支持
	MDB_RET_OBJ_EXIST,				//对象已经存在
	MDB_RET_SYSTAB_ERR,				//系统表错误
	MDB_RET_DISCONNECT,				//连接断开
	MDB_RET_INDEX_EXIST,			//索引已经存在
	MDB_RET_FIELD_NOTEXIST,			//字段不存在
	MDB_RET_RETRIEVE_OVERFLOW,		//读取数据集溢出
	MDB_RET_COMM_ERROR,				//通讯错误
	MDB_RET_TYPE_ERROR,				//类型错误
	MDB_RET_DISK_ERROR,				//磁盘错误
	MDB_RET_CONTEXT_NOTEXIST,		//内容不存在
	MDB_RET_UNIQUE_VIOLATION,		//违反唯一性约束
	MDB_RET_DATA_OVERFLOW,			//数据溢出
	MDB_RET_UNKNOWN,				//未知错误
};

enum SBASE_MDB_EXPORT eMdbFrameType
{
	MDB_FT_INSERT=1,				//插入
	MDB_FT_SELECT,					//查询
	MDB_FT_EXECUTE,					//执行
	MDB_FT_TRIGGER,					//服务器向客户端的触发通知
	MDB_FT_CONNTRG,					//客户端向服务器注册触发通知
	MDB_FT_DISCONNTRG,				//客户端向服务器断开触发通知
	MDB_FT_FIELDS,					//取字段信息
	MDB_FT_UPDATE_INSERT,			//更新或插入记录(以主键为唯一标识，无主键的表不允许使用)
	MDB_FT_UPDATE_BLOB,				//更新大字段
	MDB_FT_READ_BLOB,				//读取大字段
};

enum SBASE_MDB_EXPORT eMdbTriggerType
{
	MDB_TRG_INSERT=1,				//插入触发
	MDB_TRG_UPDATE,					//更新触发
	MDB_TRG_DELETE,					//删除触发
	MDB_TRG_TRUNCATE,				//清空触发
	MDB_TRG_UPDATE_WITHPK,			//通过主键进行的更新，只作为历史操作类型，不用于客户端触发（触发同MDB_TRG_UPDATE）
};

enum SBASE_MDB_EXPORT eMdbTriggerFlag
{
	MDB_TRGFLG_INSERT=1,				//插入触发
	MDB_TRGFLG_UPDATE=2,				//更新触发
	MDB_TRGFLG_DELETE=4,				//删除触发
	MDB_TRGFLG_TRUNCATE=8,				//清空触发
	MDB_TRGFLG_ALL=0x0F,				//所有触发
};

enum SBASE_MDB_EXPORT eMdbFieldType
{
	MDB_INT = 1,//4 bytes
	MDB_INT64,	//8 bytes
	MDB_FLOAT,	//4 bytes
	MDB_SHORT,	//2 bytes
	MDB_BYTE,	//1 bytes
	MDB_STR,	//N bytes（由建表语法决定）
	MDB_BLOB,	//4字节（文件名OID）
};


#pragma pack(push)  
#pragma pack(1)

//////////////////////////////////////////////////////////////////////////
// 名    称:  stuSysTable
// 作    者:  邵凯田
// 创建时间:  2014-04-24 11:20
// 描    述:  系统表结构
//////////////////////////////////////////////////////////////////////////
struct SBASE_MDB_EXPORT stuSysTable
{
	char name[32];	//表名称
	int  rows;				//总记录数
	int  rowsize;			//一条记录的总长度
};

//////////////////////////////////////////////////////////////////////////
// 名    称:  stuSysTable
// 作    者:  邵凯田
// 创建时间:  2014-04-24 11:20
// 描    述:  系统表结构
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

//触发数据定义
typedef BYTE* (*OnMdbClientTrgCallback)(void* cbParam, SString &sTable, eMdbTriggerType eType,int iTrgRows,int iRowSize,BYTE *pTrgData);

//////////////////////////////////////////////////////////////////////////
// 名    称:  CMdbClient
// 作    者:  邵凯田
// 创建时间:  2014-04-24 22:01
// 描    述:  自定义内存环形数据库的操作类
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
	// 描    述:  连接数据库服务器
	// 作    者:  邵凯田
	// 创建时间:  2014-04-24 22:41
	// 参数说明:  @ip为数据库IP地址
	//         :  @port为数据库TCP服务端口
	//         :  @dbuser为用户名
	//         :  @dbpwd为密码
	// 返 回 值:  true表示登录成功，false表示登录失败
	//////////////////////////////////////////////////////////////////////////
	bool Connect(SString ip,int port,SString dbuser,SString dbpwd);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  断开数据库连接
	// 作    者:  邵凯田
	// 创建时间:  2014-04-24 22:42
	// 参数说明:  void
	// 返 回 值:  true断开成功，false失败
	//////////////////////////////////////////////////////////////////////////
	bool DisConnect();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  插入新的一批记录
	// 作    者:  邵凯田
	// 创建时间:  2014-04-23 12:52
	// 参数说明:  @sTableName为表名
	//         :  @pRowData表示记录数据，含与建表时定义一致的所有记录
	//         :  @iRowSize表示一条记录的长度
	//         :  @iRows表示当前一共有多少条记录待插入，多条记录连续存储在pRowData中
	// 返 回 值:  >0表示成功插入的记录数，<0表示失败返回码
	//////////////////////////////////////////////////////////////////////////
	int Insert(const char* sTableName, void *pRowData, int iRowSize, int iRows);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  更新或插入一批记录，以主键为唯一标识，如果记录存在更新，不存在则插入
	// 作    者:  邵凯田
	// 创建时间:  2014-12-17 12:52
	// 参数说明:  @sTableName为表名
	//         :  @pRowData表示记录数据，含与建表时定义一致的所有记录
	//         :  @iRowSize表示一条记录的长度
	//         :  @iRows表示当前一共有多少条记录待插入，多条记录连续存储在pRowData中
	// 返 回 值:  >0表示成功插入的记录数，<0表示失败返回码
	//////////////////////////////////////////////////////////////////////////
	int UpdateWithInsert(const char* sTableName, void *pRowData, int iRowSize, int iRows);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  执行SQL语句，可创建表、删除表、创建索引、插入记录、更新记录、删除记录等
	// 作    者:  邵凯田
	// 创建时间:  2014-04-23 12:05
	// 参数说明:  @sql为可执行的SQL语句，支持简化语法
	// 返 回 值:  >=0表示成功以及影响的行数，<0表示失败返回码
	//////////////////////////////////////////////////////////////////////////
	int Execute(SString &sql);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  查询指定SQL语句的数据集
	// 作    者:  邵凯田
	// 创建时间:  2014-04-23 12:07
	// 参数说明:  @sql为可执行的SQL语句，支持简化语法
	//         :  @iRowSize表示一行记录的总长度(引用返回）
	//         :  @pRowData为记录集数组，总长度为记录数*iRowSize，当该指针不为NULL时，使用者应负责释放内容
	// 返 回 值:  >=0表示成功返回的记录数，<0表示失败返回码
	//			  当select count(*),min(f1)... 时返回1，且pRowData为NULL，SQL中为按\t分隔的结果字符串
	//////////////////////////////////////////////////////////////////////////
	int Retrieve(SString &sql,int &iRowSize,void* &pRowData);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取最后的错误描述
	// 作    者:  邵凯田
	// 创建时间:  2014-04-24 22:05
	// 参数说明:  void
	// 返 回 值:  SString
	//////////////////////////////////////////////////////////////////////////
	inline SString GetLastError(){return m_sLastError;};
#ifndef WIN32
	inline void SetLastError(const char* sErr){ strcpy(m_sLastError, sErr); };
#else
	inline void SetLastError(const char* sErr){ strcpy_s(m_sLastError, sizeof(m_sLastError), sErr); };
#endif


	////////////////////////////////////////////////////////////////////////
	// 描    述:  会话线程接收到报文后的回调虚函数，派生类通过此函数处理即时消息
	// 作    者:  邵凯田
	// 创建时间:  2014:4:27 21:45
	// 参数说明:  @pSession为会话实例指针，可以通过指针向对端发送数据
	//            @pPackage为刚接收到的数据包
	// 返 回 值:  true表示已经处理完毕，此报文可以释放了，false表示未处理，此报文应放入接收队列
	//////////////////////////////////////////////////////////////////////////
	virtual bool OnRecvFrame(stuSTcpPackage *pPackage);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  根据输入的MDB返回码取对应的返回描述
	// 作    者:  邵凯田
	// 创建时间:  2014-5-19 13:42
	// 参数说明:  @ret为返回码类型
	// 返 回 值:  对应描述字符串 
	//////////////////////////////////////////////////////////////////////////
	static SString GetRetCodeText(int ret);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  注册触发回调函数，可注册多个回调函数
	// 作    者:  邵凯田
	// 创建时间:  2014-10-18 15:46
	// 参数说明:  @pFun为回调函数地址
	//			  @cbParam为回调函数
	//            @sTableName为触发的表名
	//            @iTriggerFlag为触发标记//bit0:insert; 1:update; 2:delete; 3:truncate,  eMdbTriggerFlag
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void RegisterTriggerCallback(OnMdbClientTrgCallback pFun, void *cbParam, SString sTableName, BYTE iTriggerFlag);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  删除触发回调函数
	// 作    者:  邵凯田
	// 创建时间:  2014-10-18 16:03
	// 参数说明:  @pFun为回调函数地址
	//			  @cbParam为回调函数
	//            @sTableName为触发的表名
	//            @iTriggerFlag为触发标记//bit0:insert; 1:update; 2:delete; 3:truncate,  eMdbTriggerType-1
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void RemoveTriggerCallback(OnMdbClientTrgCallback pFun, void *cbParam, SString sTableName, BYTE iTriggerFlag);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  删除所有触发回调
	// 作    者:  邵凯田
	// 创建时间:  2016-5-11 16:46
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void RemoveAllTrigger();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取当前库中所有数据表
	// 作    者:  邵凯田
	// 创建时间:  2014-11-13 13:11
	// 参数说明:  @SysTables为引用返回的系统表列表
	// 返 回 值:  >=0表示数据表数量，<0表示失败
	//////////////////////////////////////////////////////////////////////////
	int GetSysTables(SPtrList<stuSysTable> &SysTables);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取指定数据表中的字段信息
	// 作    者:  邵凯田
	// 创建时间:  2014-11-13 13:49
	// 参数说明:  @sTable表名
	//			  @TableFields为引用返回的字段信息列表
	// 返 回 值:  >=0表示字段数量，<0表示失败
	//////////////////////////////////////////////////////////////////////////
	int GetTableFields(const char* sTable, SPtrList<stuTableField> &TableFields);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取记录中指定序号的字段字符串值内容
	// 作    者:  邵凯田
	// 创建时间:  2014-11-14 10:46
	// 参数说明:  @pRow为记录内容
	//			  @pFld为字段信息
	// 返 回 值:  字符串型值内容
	//////////////////////////////////////////////////////////////////////////
	static SString GetValueStr(BYTE *pRow, stuTableField *pFld);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  通过数据集的内容设置内存结构的值
	// 作    者:  邵凯田
	// 创建时间:  2016-3-24 10:08
	// 参数说明:  @pRows表示所有行的缓冲区
	//         :  @iRows表示缓冲区总行数
	//         :  @iRowSize表示行长度
	//         :  @pRs表示数据集指针
	//         :  @TableFields表示表字段引用,字段数量及顺序必须与pRs数据集中的一致
	// 返 回 值:  返回成功复制的有效行数
	//////////////////////////////////////////////////////////////////////////
	static int SetValueFromRs(BYTE *pRows,int iRows,int iRowSize,SRecordset *pRs,SPtrList<stuTableField> &TableFields);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  用文件更新数据库中指定记录的指定大字段
	// 作    者:  邵凯田
	// 创建时间:  2014-12-22 15:16
	// 参数说明:  @sTable表名
	//			  @sLobField为字段名
	//			  @sWhere条件（唯一确定一条记录)
	//			  @sFile为本地文件名
	// 返 回 值:  >0表示成功，=0表示未找到对应记录，<0表示失败
	//////////////////////////////////////////////////////////////////////////
	int UpdateBlobFromFile(SString sTable, SString sLobField, SString sWhere, SString sFile);
	int UpdateBlobFromMem(SString sTable, SString sLobField, SString sWhere, BYTE *pBuf,int iBufLen);
	
	////////////////////////////////////////////////////////////////////////
	// 描    述:  从数据库中读取指定记录的指定大字段到本地文件
	// 作    者:  邵凯田
	// 创建时间:  2014-12-22 15:26
	// 参数说明:  @sTable表名
	//			  @sLobField为字段名
	//			  @sWhere条件（唯一确定一条记录)
	//			  @sFile为本地文件名
	// 返 回 值:  >0表示成功，=0表示未找到对应记录，<0表示失败
	//////////////////////////////////////////////////////////////////////////
	int ReadBlobToFile(SString sTable, SString sLobField, SString sWhere, SString sFile);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  从数据库中读取指定记录的指定缓冲区，由调用者负责释放
	// 作    者:  邵凯田
	// 创建时间:  2014-12-22 15:26
	// 参数说明:  @sTable表名
	//			  @sLobField为字段名
	//			  @sWhere条件（唯一确定一条记录)
	//			  @pBlobBuf表示大字段内容缓冲区
	//		      @iBlobLen表示大字段字节数
	// 返 回 值:  >0表示成功，=0表示未找到对应记录，<0表示失败
	//////////////////////////////////////////////////////////////////////////
	int ReadBlobToMem(SString sTable, SString sLobField, SString sWhere, BYTE* &pBlobBuf, int &iBlobLen);

protected:
	//////////////////////////////////////////////////////////////////////////
	// 描    述:  登录上内存数据库服务端时的回调函数，主要用于断开重连后执行重新注册触发回调
	// 作    者:  邵凯田
	// 创建时间:  2015-8-7 16:35
	// 参数说明:  void
	// 返 回 值:  void
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
