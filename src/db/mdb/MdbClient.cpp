/**
 *
 * 文 件 名 : MdbClient.cpp
 * 创建日期 : 2014-4-22 13:11
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

#include "MdbClient.h"
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////////
// 名    称:  CMdbClient
// 作    者:  邵凯田
// 创建时间:  2014:4:24 22:01
// 描    述:  自定义内存环形数据库的操作类
//////////////////////////////////////////////////////////////////////////
CMdbClient::CMdbClient()
{
	SetStartWord(0x518C);
	memset(m_sLastError,0,sizeof(m_sLastError));
	//m_pRecvPackage = NULL;
	m_TrgReg = new SPtrList<stuMdbClientTriggerReg>();
	m_TrgReg->setShared(true);
	m_TrgReg->setAutoDelete(false);
}

CMdbClient::~CMdbClient()
{
	//if(m_pRecvPackage)
	//	delete m_pRecvPackage;
	m_TrgReg->setAutoDelete(true);
	m_TrgReg->clear();
	delete m_TrgReg;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  连接数据库服务器
// 作    者:  邵凯田
// 创建时间:  2014:4:24 22:41
// 参数说明:  @ip为数据库IP地址
//         :  @port为数据库TCP服务端口
//         :  @dbuser为用户名
//         :  @dbpwd为密码
// 返 回 值:  true表示登录成功，false表示登录失败
//////////////////////////////////////////////////////////////////////////
bool CMdbClient::Connect(SString ip,int port,SString dbuser,SString dbpwd)
{
	m_sIp = ip;
	m_iPort = port;
	SetLoginInfo(SString::toFormat("dbuser=%s;dbpwd=%s;",dbuser.data(),dbpwd.data()));
	STcpClientBase::SetConnectMode(true);
	//将连接方式改为线程连接
	if(!Start(ip, port))
	//if(!STcpClientBase::Connect(ip,port))
	{
		sprintf(m_sLastError,"连接到MDB(%s:%d)失败!",ip.data(),port);
		LOGWARN("%s",m_sLastError);
		return false;
	}
	OnLogin();
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  登录上内存数据库服务端时的回调函数，主要用于断开重连后执行重新注册触发回调
// 作    者:  邵凯田
// 创建时间:  2015-8-7 16:35
// 参数说明:  void
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void CMdbClient::OnLogin()
{
	//如果有注册需求，重新进行注册
	unsigned long pos;
	SString sHead;
	m_TrgReg->lock();
	stuMdbClientTriggerReg *pReg = m_TrgReg->FetchFirst(pos);
	while(pReg)
	{
		sHead.sprintf("table=%s;action=%d;", pReg->sTableName.data(),pReg->iTriggerFlag);
		if (!SendFrame(sHead, MDB_FT_CONNTRG))
		{
			LOGERROR("发送连接触发器命令时失败!");
		}
		pReg = m_TrgReg->FetchNext(pos);
	}
	m_TrgReg->unlock();
}

////////////////////////////////////////////////////////////////////////
// 描    述:  断开数据库连接
// 作    者:  邵凯田
// 创建时间:  2014:4:24 22:42
// 参数说明:  void
// 返 回 值:  true断开成功，false失败
//////////////////////////////////////////////////////////////////////////
bool CMdbClient::DisConnect()
{
	if (GetSocket() != NULL && GetSocket()->GetSocketHandle() != 0)
		GetSocket()->Close();
	STcpClientBase::StopAndWait();
	return true;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  插入新的一批记录
// 作    者:  邵凯田
// 创建时间:  2014:4:23 12:52
// 参数说明:  @sTableName为表名
//         :  @pRowData表示记录数据，含与建表时定义一致的所有记录
//         :  @iRowSize表示一条记录的长度
//         :  @iRows表示当前一共有多少条记录待插入，多条记录连续存储在pRowData中
// 返 回 值:  >0表示成功插入的记录数，<0表示失败返回码
//////////////////////////////////////////////////////////////////////////
int CMdbClient::Insert(const char* sTableName,void *pRowData,int iRowSize,int iRows)
{
	SString sHead;
	sHead.sprintf("table=%s;rowsize=%d;rows=%d;",sTableName,iRowSize,iRows);
	if (!IsConnected())
		return MDB_RET_DISCONNECT;
	stuSTcpPackage *pRecvPackage = NULL;
	if (!SendAndSearchPkg(pRecvPackage, sHead, MDB_FT_INSERT, (BYTE*)pRowData, iRowSize*iRows))
	{
		sprintf(m_sLastError,"插入失败!");
		LOGWARN("%s",m_sLastError);
		if (pRecvPackage != NULL)
			delete pRecvPackage;
		return MDB_RET_DISCONNECT;
	}
	if (pRecvPackage->m_wFrameType != MDB_FT_INSERT || pRecvPackage->m_iAsduLen != 4)
	{
		delete pRecvPackage;
		return MDB_RET_UNKNOWN;
	}
	SKT_SWAP_DWORD(*((int*)m_pRecvPackage->m_pAsduBuffer));
	int ret = *((int*)pRecvPackage->m_pAsduBuffer);
	if(ret < MDB_RET_SUCCESS)
	{
		sprintf(m_sLastError, "%s", pRecvPackage->m_sHead.data());
	}
	delete pRecvPackage;
	return ret;
}


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
int CMdbClient::UpdateWithInsert(const char* sTableName, void *pRowData, int iRowSize, int iRows)
{
	SString sHead;
	sHead.sprintf("table=%s;rowsize=%d;rows=%d;", sTableName, iRowSize, iRows);
	if (!IsConnected())
		return MDB_RET_DISCONNECT;
	stuSTcpPackage *pRecvPackage = NULL;
	if (!SendAndSearchPkg(pRecvPackage, sHead, MDB_FT_UPDATE_INSERT, (BYTE*)pRowData, iRowSize*iRows))
	{
		sprintf(m_sLastError, "更新/插入失败!");
		LOGWARN("%s", m_sLastError);
		if (pRecvPackage != NULL)
			delete pRecvPackage;
		if (!IsConnected())
			return MDB_RET_DISCONNECT;
		return MDB_RET_UNKNOWN;
	}
	if (pRecvPackage->m_wFrameType != MDB_FT_UPDATE_INSERT || pRecvPackage->m_iAsduLen != 4)
	{
		delete pRecvPackage;
		return MDB_RET_UNKNOWN;
	}
	SKT_SWAP_DWORD(*((int*)pRecvPackage->m_pAsduBuffer));
	int ret = *((int*)pRecvPackage->m_pAsduBuffer);
	if (ret < MDB_RET_SUCCESS)
	{
		sprintf(m_sLastError, "%s", pRecvPackage->m_sHead.data());
	}
	delete pRecvPackage;
	return ret;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  执行SQL语句，可创建表、删除表、创建索引、插入记录、更新记录、删除记录等
// 作    者:  邵凯田
// 创建时间:  2014:4:23 12:05
// 参数说明:  @sql为可执行的SQL语句，支持简化语法
// 返 回 值:  >=0表示成功以及影响的行数，<0表示失败返回码
//////////////////////////////////////////////////////////////////////////
int CMdbClient::Execute(SString &sql)
{
	if (!IsConnected())
		return MDB_RET_DISCONNECT;
	stuSTcpPackage* pRecvPackage = NULL;
	SString sHead;
	//if (!SendAndSearchPkg(pRecvPackage, sql, MDB_FT_EXECUTE))
	if (!SendAndSearchPkg(pRecvPackage, sHead, MDB_FT_EXECUTE,(BYTE*)sql.data(),sql.length()+1,60))
	{
		sprintf(m_sLastError,"执行SQL失败!");
		LOGWARN("%s",m_sLastError);
		if (pRecvPackage != NULL)
			delete pRecvPackage;
		return MDB_RET_DISCONNECT;
	}
	if (pRecvPackage->m_wFrameType != MDB_FT_EXECUTE || pRecvPackage->m_iAsduLen != 4)
	{
		delete pRecvPackage;
		return MDB_RET_UNKNOWN;
	}
	SKT_SWAP_DWORD(*((int*)pRecvPackage->m_pAsduBuffer));
	int ret = *((int*)pRecvPackage->m_pAsduBuffer);
	if(ret < MDB_RET_SUCCESS)
	{
		sprintf(m_sLastError, "%s", pRecvPackage->m_sHead.data());
	}
	delete pRecvPackage;
	return ret;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  查询指定SQL语句的数据集
// 作    者:  邵凯田
// 创建时间:  2014:4:23 12:07
// 参数说明:  @sql为可执行的SQL语句，支持简化语法
//         :  @iRowSize表示一行记录的总长度(引用返回）
//         :  @pRowData为记录集数组，总长度为记录数*iRowSize，当该指针不为NULL时，使用者应负责释放内容
// 返 回 值:  >=0表示成功返回的记录数，<0表示失败返回码
//			  当select count(*),min(f1)... 时返回1，且pRowData为NULL，SQL中为按\t分隔的结果字符串
//////////////////////////////////////////////////////////////////////////
int CMdbClient::Retrieve(SString &sql,int &iRowSize,void* &pRowData)
{
	if (!IsConnected())
		return MDB_RET_DISCONNECT;
	stuSTcpPackage *pRecvPackage = NULL;
	if (!SendAndSearchPkg(pRecvPackage, sql, MDB_FT_SELECT,NULL,0,30))
	{
		sprintf(m_sLastError,"执行SQL失败!");
		LOGWARN("%s",m_sLastError);
		if (pRecvPackage != NULL)
			delete pRecvPackage;
		return MDB_RET_DISCONNECT;
	}
	if(pRecvPackage == NULL)
	{
		return MDB_RET_UNKNOWN;
	}
	if (pRecvPackage->m_wFrameType != MDB_FT_SELECT || pRecvPackage->m_iAsduLen < 8)
	{
		delete pRecvPackage;
		return MDB_RET_UNKNOWN;
	}
	//ret rows
	int ret = *((int*)pRecvPackage->m_pAsduBuffer);
	SKT_SWAP_DWORD(ret);
	//row size
	iRowSize = *((int*)(pRecvPackage->m_pAsduBuffer + 4));
	SKT_SWAP_DWORD(iRowSize);
	if(ret < 0)
	{
		sprintf(m_sLastError, "%s", pRecvPackage->m_sHead.data());
	}
	else if(ret == 0)
	{
		delete pRecvPackage;
		return 0;
	}
	else if (ret == 1 && pRecvPackage->m_iAsduLen == 8)
	{
		//返回统计结果
		sql = pRecvPackage->m_sHead;
		pRowData = NULL;
		delete pRecvPackage;
		return 1;
	}
	if (iRowSize < 0)
	{
		delete pRecvPackage;
		return MDB_RET_UNKNOWN;
	}
	int bufsize = iRowSize*ret;
	if (pRecvPackage->m_iAsduLen - 8 != bufsize)
	{
		if (pRecvPackage->m_iAsduLen - 8 == 0)
		{
			delete pRecvPackage;
			return ret;
		}
		delete pRecvPackage;
		return MDB_RET_UNKNOWN;
	}
	if (iRowSize > C_MDB_MAX_RETRIEVE_ROWS)
	{
		delete pRecvPackage;
		return MDB_RET_RETRIEVE_OVERFLOW;
	}
	pRowData = new BYTE[bufsize];
	memcpy(pRowData, pRecvPackage->m_pAsduBuffer + 8, bufsize);
	delete pRecvPackage;
	return ret;
}

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
int CMdbClient::UpdateBlobFromFile(SString sTable, SString sLobField, SString sWhere, SString sFile)
{
	if (!IsConnected())
		return MDB_RET_DISCONNECT;
	SString sHead;
	sHead.sprintf("table=%s;field=%s;where=%s;", sTable.data(), sLobField.data(), sWhere.data());
	SFile f(sFile);
	BYTE *pFileBuf = NULL;
	int size = f.size();
	if (size > 0 && f.open(IO_ReadOnly))
	{
		pFileBuf = new BYTE[size];
		if (f.readBlock(pFileBuf, size) != size)
		{
			LOGERROR("读取文件(%s)时失败!",sFile.data());
			delete[] pFileBuf;
			return MDB_RET_UNKNOWN;
		}
	}
	else
		size = 0;
	stuSTcpPackage *pRecvPackage = NULL;
	if (!SendAndSearchPkg(pRecvPackage, sHead, MDB_FT_UPDATE_BLOB, pFileBuf, size))
	{
		f.close();
		if (pFileBuf != NULL)
			delete[] pFileBuf;
		sprintf(m_sLastError, "更新大字段时失败!");
		LOGWARN("%s", m_sLastError);
		if (pRecvPackage != NULL)
			delete pRecvPackage;
		return MDB_RET_DISCONNECT;
	}
	f.close();
	if (pFileBuf != NULL)
		delete[] pFileBuf;
	if (pRecvPackage->m_wFrameType != MDB_FT_UPDATE_BLOB || pRecvPackage->m_iAsduLen != 4)
	{
		delete pRecvPackage;
		return MDB_RET_UNKNOWN;
	}
	//ret rows
	int ret = *((int*)pRecvPackage->m_pAsduBuffer);
	SKT_SWAP_DWORD(ret);
	if (ret < 0)
	{
		sprintf(m_sLastError, "%s", pRecvPackage->m_sHead.data());
	}
	delete pRecvPackage;
	return ret;
}

int CMdbClient::UpdateBlobFromMem(SString sTable, SString sLobField, SString sWhere, BYTE *pBuf,int iBufLen)
{
	if (!IsConnected())
		return MDB_RET_DISCONNECT;
	SString sHead;
	sHead.sprintf("table=%s;field=%s;where=%s;", sTable.data(), sLobField.data(), sWhere.data());
	BYTE *pFileBuf = pBuf;
	int size = iBufLen;
	stuSTcpPackage *pRecvPackage = NULL;
	if (!SendAndSearchPkg(pRecvPackage, sHead, MDB_FT_UPDATE_BLOB, pFileBuf, size))
	{
		sprintf(m_sLastError, "更新大字段时失败!");
		LOGWARN("%s", m_sLastError);
		if (pRecvPackage != NULL)
			delete pRecvPackage;
		return MDB_RET_DISCONNECT;
	}
	if (pRecvPackage->m_wFrameType != MDB_FT_UPDATE_BLOB || pRecvPackage->m_iAsduLen != 4)
	{
		delete pRecvPackage;
		return MDB_RET_UNKNOWN;
	}
	//ret rows
	int ret = *((int*)pRecvPackage->m_pAsduBuffer);
	SKT_SWAP_DWORD(ret);
	if (ret < 0)
	{
		sprintf(m_sLastError, "%s", pRecvPackage->m_sHead.data());
	}
	delete pRecvPackage;
	return ret;
}

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
int CMdbClient::ReadBlobToFile(SString sTable, SString sLobField, SString sWhere, SString sFile)
{
	if (!IsConnected())
		return MDB_RET_DISCONNECT;
	SString sHead;
	stuSTcpPackage *pRecvPackage = NULL;
	sHead.sprintf("table=%s;field=%s;where=%s;", sTable.data(), sLobField.data(), sWhere.data());
	if (!SendAndSearchPkg(pRecvPackage, sHead, MDB_FT_READ_BLOB))
	{

		sprintf(m_sLastError, "读取大字段时失败!");
		LOGWARN("%s", m_sLastError);
		if (pRecvPackage != NULL)
			delete pRecvPackage;
		return MDB_RET_DISCONNECT;
	}
	
	if (pRecvPackage->m_wFrameType != MDB_FT_READ_BLOB)
	{
		delete pRecvPackage;
		return MDB_RET_UNKNOWN;
	}
	SFile f(sFile);
	int ret = SString::GetAttributeValueI(pRecvPackage->m_sHead, "ret");
	int size = SString::GetAttributeValueI(pRecvPackage->m_sHead, "size");
	if (size != pRecvPackage->m_iAsduLen || ret < 0)
	{
		delete pRecvPackage;
		return MDB_RET_UNKNOWN;
	}
	if (!f.open(IO_Truncate))
	{
		LOGERROR("创建文件(%s)时失败!", sFile.data());
		delete pRecvPackage;
		return MDB_RET_UNKNOWN;
	}
	if (size > 0)
	{
		if (f.writeBlock(pRecvPackage->m_pAsduBuffer, size) != size)
		{
			LOGERROR("写文件(%s)时失败!", sFile.data());
			delete pRecvPackage;
			return MDB_RET_UNKNOWN;
		}
	}
	f.close();
	delete pRecvPackage;
	return ret;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  从数据库中读取指定记录的指定大字段到缓冲区，由调用者负责释放
// 作    者:  邵凯田
// 创建时间:  2014-12-22 15:26
// 参数说明:  @sTable表名
//			  @sLobField为字段名
//			  @sWhere条件（唯一确定一条记录)
//			  @pBlobBuf表示大字段内容缓冲区
//		      @iBlobLen表示大字段字节数
// 返 回 值:  >0表示成功，=0表示未找到对应记录，<0表示失败
//////////////////////////////////////////////////////////////////////////
int CMdbClient::ReadBlobToMem(SString sTable, SString sLobField, SString sWhere, BYTE* &pBlobBuf, int &iBlobLen)
{
	if (!IsConnected())
		return MDB_RET_DISCONNECT;
	SString sHead;
	stuSTcpPackage *pRecvPackage = NULL;
	sHead.sprintf("table=%s;field=%s;where=%s;", sTable.data(), sLobField.data(), sWhere.data());
	if (!SendAndSearchPkg(pRecvPackage, sHead, MDB_FT_READ_BLOB))
	{

		sprintf(m_sLastError, "读取大字段时失败!");
		LOGWARN("%s", m_sLastError);
		if (pRecvPackage != NULL)
			delete pRecvPackage;
		return MDB_RET_DISCONNECT;
	}

	if (pRecvPackage->m_wFrameType != MDB_FT_READ_BLOB)
	{
		delete pRecvPackage;
		return MDB_RET_UNKNOWN;
	}
	int ret = SString::GetAttributeValueI(pRecvPackage->m_sHead, "ret");
	int size = SString::GetAttributeValueI(pRecvPackage->m_sHead, "size");
	if (size != pRecvPackage->m_iAsduLen || ret < 0)
	{
		delete pRecvPackage;
		return MDB_RET_UNKNOWN;
	}
	iBlobLen = size;
	if (size == 0)
	{
		pBlobBuf = NULL;
	}
	else
	{
		pBlobBuf = new BYTE[size+1];
		memcpy(pBlobBuf, pRecvPackage->m_pAsduBuffer, size);
		pBlobBuf[size] = 0;
	}
	delete pRecvPackage;
	return ret;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  会话线程接收到报文后的回调虚函数，派生类通过此函数处理即时消息
// 作    者:  邵凯田
// 创建时间:  2014:4:27 21:45
// 参数说明:  @pSession为会话实例指针，可以通过指针向对端发送数据
//            @pPackage为刚接收到的数据包
// 返 回 值:  true表示已经处理完毕，此报文可以释放了，false表示未处理，此报文应放入接收队列
//////////////////////////////////////////////////////////////////////////
bool CMdbClient::OnRecvFrame(stuSTcpPackage *pPackage)
{
	if (pPackage->m_wFrameType == MDB_FT_TRIGGER)
	{
		if (m_TrgReg->count() > 0)
		{
			SString sTable;
			eMdbTriggerType eType;
			int iTrgRows, iRowSize;
			unsigned long pos;
			//sHead.sprintf("table=%s;rowsize=%d;trgrows=%d;act=1;", pTable->GetTableName().data(), pTable->GetRowSize(), pTrg->m_iTrgRows);
			sTable = SString::GetAttributeValue(pPackage->m_sHead, "table");
			eType = (eMdbTriggerType)SString::GetAttributeValueI(pPackage->m_sHead, "act");
			iTrgRows = SString::GetAttributeValueI(pPackage->m_sHead, "trgrows");
			iRowSize = SString::GetAttributeValueI(pPackage->m_sHead, "rowsize");
			if (eType != MDB_TRG_TRUNCATE && iTrgRows * iRowSize != pPackage->m_iAsduLen)
			{
				return true;
			}
			m_TrgReg->lock();
			stuMdbClientTriggerReg *p = m_TrgReg->FetchFirst(pos);
			while (p)
			{
				//run callback for each
				if(p->sTableName == sTable && (p->iTriggerFlag & 1<<(eType-1)) != 0)
					p->pFun(p->cbParam, sTable, eType, iTrgRows, iRowSize, pPackage->m_pAsduBuffer);
				p = m_TrgReg->FetchNext(pos);
			}
			m_TrgReg->unlock();
		}
		return true;
	}
	else
	{
		//LOGERROR("无效的突发报文:%d",pPackage->m_wFrameType);
	}
	return false;//未处理,将放入接收队列，等待应用层检索
}

////////////////////////////////////////////////////////////////////////
// 描    述:  注册触发回调函数，可注册多个回调函数
// 作    者:  邵凯田
// 创建时间:  2014-10-18 15:46
// 参数说明:  @pFun为回调函数地址
//			  @cbParam为回调函数
//            @sTableName为触发的表名
//            @iTriggerFlag为触发标记//bit0:insert; 1:update; 2:delete; 3:truncate,  eMdbTriggerType-1
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void CMdbClient::RegisterTriggerCallback(OnMdbClientTrgCallback pFun, void *cbParam, SString sTableName, BYTE iTriggerFlag)
{
	bool bExist = false;
	unsigned long pos;
	m_TrgReg->lock();
	stuMdbClientTriggerReg *p = m_TrgReg->FetchFirst(pos);
	while (p)
	{
		if (p->pFun == pFun && p->cbParam == cbParam && p->sTableName == sTableName)
		{
			if ((p->iTriggerFlag | iTriggerFlag) == iTriggerFlag)
			{
				bExist = true;
				break;
			}
		}
		p = m_TrgReg->FetchNext(pos);
	}
	m_TrgReg->unlock();
	if(bExist)
	{
		LOGWARN("表[%s]的触发[0x%X]已注册，重复的注册指令被取消!",sTableName.data(),iTriggerFlag);
		return ;
	}

	SString sHead;
	sHead.sprintf("table=%s;action=%d;", sTableName.data(), iTriggerFlag);
	if (!SendFrame(sHead, MDB_FT_CONNTRG))
	{
		LOGERROR("发送连接触发器命令时失败!");
		return;
	}
	//注册成功
	p = new stuMdbClientTriggerReg();
	p->pFun = pFun;
	p->cbParam = cbParam;
	p->sTableName = sTableName;
	p->iTriggerFlag = iTriggerFlag;
	m_TrgReg->append(p);
}


//////////////////////////////////////////////////////////////////////////
// 描    述:  将当前实例下的触发器移动到另一个实例中，并在其中实例，取消本地触发
// 作    者:  邵凯田
// 创建时间:  2019-5-3 9:30
// 参数说明:  @pAnotherMdb
// 返 回 值:  表示移动的触发器数量
//////////////////////////////////////////////////////////////////////////
int CMdbClient::MoveTriggerToAnother(CMdbClient *pAnotherMdb)
{
	unsigned long pos;
	int cnt = 0;
	m_TrgReg->lock();
	stuMdbClientTriggerReg *p = m_TrgReg->FetchFirst(pos);
	while (p)
	{
		cnt++;
		pAnotherMdb->RegisterTriggerCallback(p->pFun,p->cbParam,p->sTableName,p->iTriggerFlag);
		p = m_TrgReg->FetchNext(pos);
	}
	m_TrgReg->clear();
	m_TrgReg->unlock();
	return cnt;
}

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
void CMdbClient::RemoveTriggerCallback(OnMdbClientTrgCallback pFun, void *cbParam, SString sTableName, BYTE iTriggerFlag)
{
	unsigned long pos=0;
	SPtrList<stuMdbClientTriggerReg> trg;
	trg.setAutoDelete(true);
	m_TrgReg->lock();
	stuMdbClientTriggerReg *p = m_TrgReg->FetchFirst(pos);
	while (p)
	{
		if (p->pFun == pFun && p->cbParam == cbParam && p->sTableName == sTableName)
		{
			if (p->iTriggerFlag == iTriggerFlag)
			{
				m_TrgReg->remove(p,false);
				trg.append(p);
			}
			else
			{
				p->iTriggerFlag ^= iTriggerFlag;
			}
			break;
		}
		p = m_TrgReg->FetchNext(pos);
	}
	m_TrgReg->unlock();
	SString sHead;
	sHead.sprintf("table=%s;action=%d;", sTableName.data(), iTriggerFlag);
	if (!SendFrame(sHead, MDB_FT_DISCONNTRG))
	{
		LOGERROR("发送连接触发器命令时失败!");
		return;
	}
	if(trg.count() > 0)
	{
		SApi::UsSleep(10000);
		trg.clear();
	}
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  删除所有触发回调
// 作    者:  邵凯田
// 创建时间:  2016-5-11 16:46
// 参数说明:  void
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void CMdbClient::RemoveAllTrigger()
{
	m_TrgReg->setAutoDelete(true);
	m_TrgReg->clear();
	m_TrgReg->setAutoDelete(false);
}

////////////////////////////////////////////////////////////////////////
// 描    述:  根据输入的MDB返回码取对应的返回描述
// 作    者:  邵凯田
// 创建时间:  2014-5-19 13:42
// 参数说明:  @ret为返回码类型
// 返 回 值:  对应描述字符串 
//////////////////////////////////////////////////////////////////////////
SString CMdbClient::GetRetCodeText(int ret)
{
	switch(ret)
	{
	case MDB_RET_SUCCESS:
		return "成功";
	case MDB_RET_TABLE_NOTEXIST:
		return "表不存在";
	case MDB_RET_SYNTAX_ERROR:
		return "语法错误";
	case MDB_RET_SQL_NOTSUPPORT:
		return "SQL不支持";
	case MDB_RET_OBJ_EXIST:
		return "对象已经存在";
	case MDB_RET_SYSTAB_ERR:
		return "系统表错误";
	case MDB_RET_DISCONNECT:
		return "连接断开";
	case MDB_RET_INDEX_EXIST:
		return "索引已经存在";
	case MDB_RET_FIELD_NOTEXIST:
		return "字段不存在";
	case MDB_RET_RETRIEVE_OVERFLOW:
		return "读取数据集溢出";
	case MDB_RET_COMM_ERROR:
		return "通讯错误";
	case MDB_RET_TYPE_ERROR:
		return "类型错误";
	case MDB_RET_DISK_ERROR:
		return "磁盘错误";
	case MDB_RET_CONTEXT_NOTEXIST:
		return "内容不存在";
	case MDB_RET_UNIQUE_VIOLATION:
		return "违反唯一性约束";
	case MDB_RET_DATA_OVERFLOW:
		return "数据溢出";
	case MDB_RET_UNKNOWN:
		return "未知错误";
	}
	return "Unknown";
}


////////////////////////////////////////////////////////////////////////
// 描    述:  取当前库中所有数据表
// 作    者:  邵凯田
// 创建时间:  2014-11-13 13:11
// 参数说明:  @SysTables为引用返回的系统表列表
// 返 回 值:  >=0表示数据表数量，<0表示失败
//////////////////////////////////////////////////////////////////////////
int CMdbClient::GetSysTables(SPtrList<stuSysTable> &SysTables)
{
	void *pTables = NULL;
	stuSysTable* pNew;
	SString sql = "select * from systable";
	SysTables.clear();
	SysTables.setAutoDelete(true);
	int rowsize = sizeof(stuSysTable);
	int ret = Retrieve(sql, rowsize, pTables);
	if (ret <= 0)
	{
		if (pTables != NULL)
			delete[] (stuSysTable*)pTables;
	}
	for (int i = 0; i < ret; i++)
	{
		pNew = new stuSysTable;
		memcpy(pNew, &((stuSysTable*)pTables)[i], sizeof(stuSysTable));
		SysTables.append(pNew);
	}
	return ret;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  取指定数据表中的字段信息
// 作    者:  邵凯田
// 创建时间:  2014-11-13 13:49
// 参数说明:  @sTable表名
//			  @TableFields为引用返回的字段信息列表
// 返 回 值:  >=0表示字段数量，<0表示失败
//////////////////////////////////////////////////////////////////////////
int CMdbClient::GetTableFields(const char* sTable, SPtrList<stuTableField> &TableFields)
{
	if (!IsConnected())
		return MDB_RET_DISCONNECT;
	TableFields.clear();
	TableFields.setAutoDelete(true);
	SString head = sTable;
	stuSTcpPackage* pRecvPackage = NULL;
	if (!SendAndSearchPkg(pRecvPackage, head, MDB_FT_FIELDS))
	{
		sprintf(m_sLastError, "执行GetTableFields失败!");
		LOGWARN("%s", m_sLastError);
		if (pRecvPackage != NULL)
			delete pRecvPackage;
		return MDB_RET_COMM_ERROR;
	}
	if (pRecvPackage->m_wFrameType != MDB_FT_FIELDS || pRecvPackage->m_iAsduLen != 4)
	{
		delete pRecvPackage;
		return MDB_RET_UNKNOWN;
	}
	int ret = *((int*)pRecvPackage->m_pAsduBuffer);
	SKT_SWAP_DWORD(ret);
	if (ret > 0)
	{
		stuTableField *pNew;
		SString str;
		int i, cnt = SString::GetAttributeCount(pRecvPackage->m_sHead, ";");
		for (i = 1; i <= cnt; i++)
		{
			str = SString::GetIdAttribute(i, pRecvPackage->m_sHead, ";");
			pNew = new stuTableField;
			memset(pNew, 0, sizeof(stuTableField));
			strcpy(pNew->name, SString::GetIdAttribute(1, str, ",").left(sizeof(pNew->name) - 1));
			pNew->type		= SString::GetIdAttributeI(2, str, ",");
			pNew->start_pos = SString::GetIdAttributeI(3, str, ",");
			pNew->bytes		= SString::GetIdAttributeI(4, str, ",");
			TableFields.append(pNew);
		}
	}
	delete pRecvPackage;
	return ret;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  取记录中指定序号的字段字符串值内容
// 作    者:  邵凯田
// 创建时间:  2014-11-14 10:46
// 参数说明:  @pRow为记录内容
//			  @pFld为字段信息
// 返 回 值:  字符串型值内容
//////////////////////////////////////////////////////////////////////////
SString CMdbClient::GetValueStr(BYTE *pRow, stuTableField *pFld)
{
	SString str;
	switch (pFld->type)
	{
		case MDB_INT:	//4 bytes
		case MDB_BLOB:
		{
	#ifndef _ARM
			str = SString::toString(*((int*)(pRow + pFld->start_pos)));

	#else
			int val = 0;
			memset(&val,pRow + pFld->start_pos,sizeof(val));
			str = SString::toString(val);
	#endif
		}
		break;
		case MDB_SHORT:	//2 bytes
		{
	#ifndef _ARM
			str = SString::toString(*((short*)(pRow + pFld->start_pos)));

	#else
			short val = 0;
			memset(&val,pRow + pFld->start_pos,sizeof(val));
			str = SString::toString(val);
	#endif
		}
			break;
		case MDB_BYTE:	//1 bytes
			str = SString::toString(pRow[pFld->start_pos]);
			break;
		case MDB_INT64:	//8 bytes
		{
	#ifndef _ARM
			str = SString::toFormat("%ld",*((INT64*)(pRow + pFld->start_pos)));

	#else
			INT64 val = 0;
			memset(&val,pRow + pFld->start_pos,sizeof(val));
			str = SString::toFormat("%ld",val);
	#endif
		}
			break;
		case MDB_FLOAT:	//4 bytes
		{
	#ifndef _ARM
			str = SString::toFormat("%f", *((float*)(pRow + pFld->start_pos)));

	#else
			float val = 0;
			memset(&val,pRow + pFld->start_pos,sizeof(val));
			str = SString::toFormat("%f",val);
	#endif
		}
			break;
		case MDB_STR:	//N bytes（由建表语法决定）
		{
			str = SString::toString((char*)(pRow + pFld->start_pos), pFld->bytes);					
		}
			break;
		default:
		{
			LOGDEBUG("字段%s有无效的数据类型:%d", pFld->name, pFld->type);
		}
			break;
	}
	return str;
}

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
int CMdbClient::SetValueFromRs(BYTE *pRows,int iRows,int iRowSize,SRecordset *pRs,SPtrList<stuTableField> &TableFields)
{
#if 1
	unsigned long pos;
	int i;
	int rows = pRs->GetRows();
	if(rows > iRows)
	{
		LOGERROR("缓冲区行数(%d)不足，数据集共有(%d)行!",iRows,rows);
		return -1;
	}
	int col=0;
	stuTableField *pFld = TableFields.FetchFirst(pos);
	
	while(pFld)
	{
		switch (pFld->type)
		{
		case MDB_INT:	//4 bytes
		case MDB_BLOB:
			{
				for(i=0;i<rows;i++)
				{
#ifndef _ARM
				*((int*)(pRows + i*iRowSize + pFld->start_pos)) = pRs->GetValue(i,col).toInt();

#else
				int val = pRs->GetValue(i,col).toInt();
				memset(pRows + i*iRowSize + pFld->start_pos,&val,sizeof(val));
#endif
				}
			}
			break;
		case MDB_SHORT:	//2 bytes
			{
				for(i=0;i<rows;i++)
				{
#ifndef _ARM
				*((short*)(pRows + i*iRowSize + pFld->start_pos)) = pRs->GetValue(i,col).toShort();
#else
				short val = pRs->GetValue(i,col).toShort();
				memset(pRows + i*iRowSize + pFld->start_pos,&val,sizeof(val));
#endif
				}
			}
			break;
		case MDB_BYTE:	//1 bytes
			for(i=0;i<rows;i++)
			{
				pRows[i*iRowSize + pFld->start_pos] = (BYTE)pRs->GetValue(i,col).toInt();
			}
			break;
		case MDB_INT64:	//8 bytes
			{
				for(i=0;i<rows;i++)
				{
#ifndef _ARM
				*((INT64*)(pRows + i*iRowSize + pFld->start_pos)) = SString::toInt64(pRs->GetValue(i,col).data());

#else
				INT64 val = SString::toInt64(pRs->GetValue(i,col).data());
				memset(pRows + i*iRowSize + pFld->start_pos,&val,sizeof(val));
#endif
				}
			}
			break;
		case MDB_FLOAT:	//4 bytes
			{
				for(i=0;i<rows;i++)
				{
#ifndef _ARM
				*((float*)(pRows + i*iRowSize + pFld->start_pos)) = pRs->GetValue(i,col).toFloat();

#else
				float val = pRs->GetValue(i,col).toFloat();
				memset(pRows + i*iRowSize + pFld->start_pos,&val,sizeof(val));
#endif
				}
			}
			break;
		case MDB_STR:	//N bytes（由建表语法决定）
			for(i=0;i<rows;i++)
			{
				SString::strncpy((char*)(pRows + i*iRowSize + pFld->start_pos), pFld->bytes,pRs->GetValue(i,col).data());
			}
			break;
		default:
			{
				LOGDEBUG("字段%s有无效的数据类型:%d", pFld->name, pFld->type);
			}
			break;
		}
		col++;
		pFld = TableFields.FetchNext(pos);
	}
#endif
	return rows;
}
