/**
 *
 * 文 件 名 : SBuffer.h
 * 创建日期 : 2009-8-4 10:25
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: 2011/08/31 08:43:33 $
 * 当前版本 : $Revision: 1.1 $
 * 功能描述 : 可多用户读取的不定长内存缓冲区类
 * 修改记录 : 
 *            $Log: SBuffer.h,v $
 *            Revision 1.1  2011/08/31 08:43:33  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2011/02/25 03:12:05  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2010/12/20 08:38:28  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2010/10/29 08:48:41  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2010/08/09 03:04:43  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2009/09/15 02:14:06  shaokt
 *            通迅记录分析装置项目首次提交
 *
 *
 **/


#if !defined(AFX_SKTBUFFER_H__12695A0C_F4BA_4DF7_9F35_17CD7703DDE1__INCLUDED_)
#define AFX_SKTBUFFER_H__12695A0C_F4BA_4DF7_9F35_17CD7703DDE1__INCLUDED_

#include "SApi.h"
#include "SLock.h"

//最多多少个只读用户
#define C_BUFFER_READER_COUNT 10

//#define YS_BUFFER_DEBUG

//对缓冲区执行读操作的用户
enum SBASE_EXPORT BUFFER_READER_LIST
{
	BUFFER_READER1 = 0,
	BUFFER_READER2 = 1,
	BUFFER_READER3 = 2,
	BUFFER_READER4 = 3,
	BUFFER_READER5 = 4,
	BUFFER_READER6 = 5,
	BUFFER_READER7 = 6,
	BUFFER_READER8 = 7,
	BUFFER_READER9 = 8,
	BUFFER_READER10 = 9,

	BUFFER_READER_NONE = 199
};

//////////////////////////////////////////////////////////////////////////
// 名    称:  SBuffer
// 作    者:  邵凯田
// 创建时间:  2010-12-17 15:22
// 描    述:  写位置只能放在数据区大小为0的节点上
//			  读位置停止于写位置处，即读位置等于写位置时，无数据可读,不等于写位置时预到无数据的空闲区直接跳过
//////////////////////////////////////////////////////////////////////////
class SBASE_EXPORT SBuffer
{
public:
	struct SBASE_EXPORT stuBufferHeader
	{
#ifdef YS_BUFFER_DEBUG
		int flag;//0xeb905372
#endif
		int iLength;//数据区大小(字节数)
		int iFreeLength;//空闲区大小(字节数)
	};
	
	SBuffer();
	SBuffer(int iBufferSize);
	~SBuffer();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  初始化缓冲区
	// 作    者:  邵凯田
	// 创建时间:  2009-8-4 11:00
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	bool init(int iBufferSize);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取空闲的只读用户，没有空闲返回BUFFER_READER_NONE
	// 作    者:  邵凯田
	// 创建时间:  2009-8-4 10:59
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	BUFFER_READER_LIST getIdleReader();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  释放一个只读用户
	// 作    者:  邵凯田
	// 创建时间:  2009-8-4 10:59
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	void releaseIdleReader(BUFFER_READER_LIST reader);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  插入一块新的数据块
	// 作    者:  邵凯田
	// 创建时间:  2009-8-4 11:12
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	bool append(BYTE *pBuf,int iLen,BYTE *pBuf2=NULL,int iLen2=0);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取当前读用户的下一个数据块
	// 作    者:  邵凯田
	// 创建时间:  2009-8-4 11:14
	// 参数说明:  @pBuffer为待写入的缓冲区指针
	//            @iLen输入表示缓冲区最大长度，输出为实际读取到的长度(仅当返回true时)
	// 返 回 值:  true表示读取成功，false表示读取失败
	//////////////////////////////////////////////////////////////////////////
	bool getNextData(BUFFER_READER_LIST reader,BYTE* pBuffer,int &iLen);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  清除所有的数据
	// 作    者:  邵凯田
	// 创建时间:  2010-10-29 16:04
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	bool clear();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  设置线程共享模式，当有多线程同时读写时，需要设置为true
	// 作    者:  邵凯田
	// 创建时间:  2011-11-14 14:58
	// 参数说明:  @b表示是否共享,true表示设为共享，false表示设为不共享
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	inline void setShared(bool b)	{m_bShared = b;};

	////////////////////////////////////////////////////////////////////////
	// 描    述:  是否没有读用户
	// 作    者:  邵凯田
	// 创建时间:  2011-11-14 14:59
	// 参数说明:  void
	// 返 回 值:  true表示没有读用户，false表示已经有读用户
	//////////////////////////////////////////////////////////////////////////
	inline bool isNoReader()		{return m_ReaderFlag == 0;};

private:
	int m_iWritePoint;//当前准备写数据块的位置
	BYTE *m_pBuffer;
	int m_iBufferSize;
	int m_iaReaderPos[C_BUFFER_READER_COUNT];//10个只读用户的状态
	DWORD m_ReaderFlag;//只读用户的注册状态，按位表示，0表示未使用，1表示已使用
	bool m_bShared;
	SLock m_Lock;
};

#endif // !defined(AFX_SKTBUFFER_H__12695A0C_F4BA_4DF7_9F35_17CD7703DDE1__INCLUDED_)
