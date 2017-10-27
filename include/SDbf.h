/**
 *
 * 文 件 名 : SDbf.h
 * 创建日期 : 2008-9-1 10:39
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: 2011/08/31 08:43:33 $
 * 当前版本 : $Revision: 1.1 $
 * 功能描述 : 结构式文件数据库模板
 * 修改记录 : 
 *            $Log: SDbf.h,v $
 *            Revision 1.1  2011/08/31 08:43:33  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.6  2011/05/16 07:21:07  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.5  2011/05/12 12:12:58  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.4  2011/04/21 12:32:17  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2011/04/11 12:08:36  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2011/03/26 09:15:05  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2011/03/03 12:07:40  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.7  2008/12/03 05:53:05  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.6  2008/11/29 04:18:05  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.5  2008/11/03 05:54:49  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.4  2008/09/26 02:16:22  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2008/09/17 08:17:19  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2008/09/16 05:32:17  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/01 11:38:29  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/01 08:58:04  shaokt
 *            *** empty log message ***
 *
 *
 **/

#ifndef __SKTDBF_H__
#define __SKTDBF_H__

#include "SString.h"
#include "SList.h"
#include "SApi.h"

//////////////////////////////////////////////////////////////////////////
// 名    称:  SDbf<class type>
// 作    者:  邵凯田
// 创建时间:  2008-9-1 11:11
// 描    述:  DBF文件操作类
//////////////////////////////////////////////////////////////////////////

template <class hdrtype>
class SVarDbf
{
public:
	//////////////////////////////////////////////////////////////////////////
	// 名    称:  stuFileHead
	// 作    者:  邵凯田
	// 创建时间:  2008-9-1 11:14
	// 描    述:  文件头
	//////////////////////////////////////////////////////////////////////////
	struct stuFileHead
	{
		int firstid;//第一条记录所在的物理记录号（从0开始），从结束位置开始折回
		int records;//当前的总记录数
		hdrtype head;//用户定义的文件头
	};

	SVarDbf(int rowsize)
	{
		m_bShared = true;
		m_iMaxRecords = 0;
		m_iRowSize = rowsize;
		m_bFileSaved = true;
	};
	virtual ~SVarDbf()
	{
		m_File.close();
	};

	////////////////////////////////////////////////////////////////////////
	// 描    述:  清空所有记录
	// 作    者:  邵凯田
	// 创建时间:  2011-11-14 16:02
	// 参数说明:  @max为最大记录数，0表示不受限
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	bool clear()
	{
		//if (m_File.isOpen() == false)
		//	return false;
		lock();
		m_File.close();
		m_File.remove();
		//if (!m_File.remove())
		//{
			//unlock();
			//return false;
		//}
		if (!m_File.open(IO_Truncate))
		{
			unlock();
			LOGFAULT("创建文件(%s)失败!", m_File.filename().data());
			return false;
		}
		//写入文件头
		m_Head.firstid = 0;
		m_Head.records = 0;
		m_File.write((BYTE*)&m_Head, sizeof(m_Head));
		m_File.close();

		if (!m_File.openByMode((char*)"r+b"))
		{
			unlock();
			LOGFAULT("打开文件(%s)失败", m_File.filename().data());
			return false;
		}
		m_File.readBlock((BYTE*)&m_Head, sizeof(m_Head));
		SKT_SWAP_DWORD(m_Head.firstid);
		SKT_SWAP_DWORD(m_Head.records);
		PrepareHdr(&m_Head.head);
		m_bFileSaved = true;
		unlock();
		return true;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  设置最大记录数
	// 作    者:  邵凯田
	// 创建时间:  2011-11-14 16:02
	// 参数说明:  @max为最大记录数，0表示不受限
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void SetMaxRecords(int max)	{m_iMaxRecords = max;};

	////////////////////////////////////////////////////////////////////////
	// 描    述:  打开文件，如果文件不存在则先创建
	// 作    者:  邵凯田
	// 创建时间:  2008-9-1 10:55
	// 参数说明:  绝对路径的文件名
	// 返 回 值:  1表示打开成功，2表示创建成功，<=0表示失败
	//////////////////////////////////////////////////////////////////////////
	int open(const char* sDbFile)
	{
		stuFileHead head;
		int ret = 1;
		memset(&head, 0, sizeof(head));
		m_File.setFileName(sDbFile);
		if (!m_File.exists())
		{
			//文件不存在，需要先建立新文件
			if (!m_File.open(IO_Truncate))
			{
				LOGFAULT("创建文件(%s)失败!", sDbFile);
				return 0;
			}
			//写入文件头
			head.firstid = 0;
			m_File.write((BYTE*)&head, sizeof(head));
			m_File.close();
			ret = 2;
		}
		if (!m_File.openByMode("r+b"))
		{
			LOGFAULT("打开文件(%s)失败", sDbFile);
			return 0;
		}
		m_File.readBlock((BYTE*)&m_Head, sizeof(m_Head));
		SKT_SWAP_DWORD(m_Head.firstid);
		SKT_SWAP_DWORD(m_Head.records);
		PrepareHdr(&m_Head.head);
		m_bFileSaved = true;
		return ret;
	}

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  检查文件是否合法
	// 作    者:  邵凯田
	// 创建时间:  2013-7-1 14:37
	// 参数说明:  void
	// 返 回 值:  true合法，false不合法
	//////////////////////////////////////////////////////////////////////////
	bool check()
	{
		if(m_File.isOpen() == false)
			return false;
		if(m_iMaxRecords > 0 && m_Head.records > m_iMaxRecords)
		{
			LOGWARN("记录数无效!recs=%d",m_Head.records);
			return false;//
		}
		int size = m_File.size();
		if(size != sizeof(stuFileHead) + m_Head.records * m_iRowSize)
		{
			LOGWARN("文件大小无效!size=%d,理论大小:%d", size, sizeof(stuFileHead)+m_Head.records * m_iRowSize);
			return false;
		}
		return true;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  关闭文件
	// 作    者:  邵凯田
	// 创建时间:  2011-11-14 16:04
	// 参数说明:  void
	// 返 回 值:  true表示关闭成功，false表示关闭失败
	//////////////////////////////////////////////////////////////////////////
	bool close()
	{
		//保存文件头
		lock();
		m_File.seekBegin();
		SKT_SWAP_DWORD(m_Head.firstid);
		SKT_SWAP_DWORD(m_Head.records);
		PrepareHdr(&m_Head.head);
		m_File.write((char*)&m_Head,sizeof(m_Head));
		SKT_SWAP_DWORD(m_Head.firstid);
		SKT_SWAP_DWORD(m_Head.records);
		PrepareHdr(&m_Head.head);
		unlock();
		m_bFileSaved = true;
		return m_File.close();
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  同步文件内容
	// 作    者:  邵凯田
	// 创建时间:  2014-12-17 18:56
	// 参数说明:  void
	// 返 回 值:  true表示同步成功，false表示同步失败
	//////////////////////////////////////////////////////////////////////////
	bool flush()
	{
		if (m_File.getHandle() == NULL)
			return false;
		if (!m_bFileSaved)
		{
			lock();
			m_File.flush();
			m_bFileSaved = true;
			unlock();
		}
		return true;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  保存文件
	// 作    者:  邵凯田
	// 创建时间:  2011-11-14 16:04
	// 参数说明:  void
	// 返 回 值:  true表示保存成功，false表示保存失败
	//////////////////////////////////////////////////////////////////////////
	bool save()
	{
		lock();

		//保存文件头
		m_File.seekBegin();
		SKT_SWAP_DWORD(m_Head.firstid);
		SKT_SWAP_DWORD(m_Head.records);
		PrepareHdr(&m_Head.head);
		m_File.write((char*)&m_Head,sizeof(m_Head));
		SKT_SWAP_DWORD(m_Head.firstid);
		SKT_SWAP_DWORD(m_Head.records);
		PrepareHdr(&m_Head.head);

		m_File.close();
		if(!m_File.openByMode("r+b"))
		{
			unlock();
			LOGFAULT("打开文件(%s)失败",m_File.filename().data());
			return false;
		}
		m_File.readBlock((BYTE*)&m_Head,sizeof(m_Head));
		SKT_SWAP_DWORD(m_Head.firstid);
		SKT_SWAP_DWORD(m_Head.records);
		PrepareHdr(&m_Head.head);
		m_bFileSaved = true;
		unlock();
		return true;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  是否线程共享模式
	// 作    者:  邵凯田
	// 创建时间:  2011-11-14 16:04
	// 参数说明:  void
	// 返 回 值:  true表示共享模式，false表示不共享模式
	//////////////////////////////////////////////////////////////////////////
	bool isShared() {return m_bShared;};

	////////////////////////////////////////////////////////////////////////
	// 描    述:  设置线程共享模式
	// 作    者:  邵凯田
	// 创建时间:  2011-11-14 16:08
	// 参数说明:  @b
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void setShared(bool b) {m_bShared=b;};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  记录被覆盖前的处理虚函数
	// 作    者:  邵凯田
	// 创建时间:  2013-3-17 14:15
	// 参数说明:  @pObj
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void BeforeOverwrite(BYTE *pObj)
	{
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  写前及读后的预处理虚函数，可用于处理字节顺序等问题,由实现类处理
	// 作    者:  邵凯田
	// 创建时间:  2011-11-14 16:08
	// 参数说明:  @pObj处理对象
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void PrepareObj(BYTE *pObj)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  写前 及读后的预处理虚函数，处理文件头结构，可用于处理字节顺序等问题,由实现类处理
	// 作    者:  邵凯田
	// 创建时间:  2013-3-17 13:27
	// 参数说明:  @pHdr待处理的对象
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void PrepareHdr(hdrtype *pHdr)
	{
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  添加一条记录
	// 作    者:  邵凯田
	// 创建时间:  2008-9-1 11:07
	// 参数说明:  type*
	// 返 回 值:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool append(BYTE *pObj)
	{
		int ret;
		lock();
		if(m_iMaxRecords == 0 || m_Head.records < m_iMaxRecords)
		{
			//数据无限制
			//防止文件末尾有无效的字节，不采用seedEnd的方法，而使用计算的偏移长度
			//m_File.seekEnd();
			int size = sizeof(m_Head)+m_iRowSize*m_Head.records;
			m_File.seekTo(size);
			int pos = m_File.position();
			if(pos != size)
			{
				if(pos <= (int)sizeof(m_Head))
				{
					//无字段
					m_Head.firstid = 0;
					m_Head.records = 0;
				}
				else
				{
					m_Head.firstid = 0;
					m_Head.records = (size - sizeof(m_Head)) / m_iRowSize;
				}
				size = sizeof(m_Head)+m_iRowSize*m_Head.records;
				m_File.seekTo(size);
			}
			
			PrepareObj(pObj);
			ret = m_File.write((char*)pObj, m_iRowSize);
			PrepareObj(pObj);
			if (ret != m_iRowSize)
			{
				unlock();
				return false;
			}
			m_Head.records ++;
			
		}
		else
		{
			//此时需要覆盖最早的数据,数据记录号从0~m_iMaxRecords-1
			int icurrrow = m_Head.firstid;
			m_Head.firstid ++;
			if(m_Head.firstid >= m_iMaxRecords)
				m_Head.firstid = 0;
			//在icurrrow位置写入当前记录
			m_File.seekTo(sizeof(m_Head)+m_iRowSize*icurrrow);
			PrepareObj(pObj);
			ret = m_File.write((char*)pObj, m_iRowSize);
			PrepareObj(pObj);
			if (ret != m_iRowSize)
			{
				unlock();
				return false;
			}			
		}
		m_File.seekBegin();
		SKT_SWAP_DWORD(m_Head.firstid);
		SKT_SWAP_DWORD(m_Head.records);
		PrepareHdr(&m_Head.head);
		m_File.write((char*)&m_Head,sizeof(m_Head));
		SKT_SWAP_DWORD(m_Head.firstid);
		SKT_SWAP_DWORD(m_Head.records);
		PrepareHdr(&m_Head.head);
		m_bFileSaved = false;
		//m_File.flush();
		unlock();
		return true;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  添加一批记录
	// 作    者:  邵凯田
	// 创建时间:  2008-9-1 11:07
	// 参数说明:  type**,size
	// 返 回 值:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool append(BYTE *pObj,int iRows)
	{
		int ret,i;
		lock();
		int iRows0 = 0;
		if (m_iMaxRecords != 0 && m_Head.records + iRows > m_iMaxRecords)
		{
			iRows0 = iRows - (m_Head.records - m_iMaxRecords);
			iRows = iRows - iRows0;
		}
again:
		if (m_iMaxRecords == 0 || m_Head.records + iRows <= m_iMaxRecords)
		{
			//数据无限制
			//防止文件末尾有无效的字节，不采用seedEnd的方法，而使用计算的偏移长度
			//m_File.seekEnd();
			int size = sizeof(m_Head)+m_iRowSize*m_Head.records;
			m_File.seekTo(size);
			int pos = m_File.position();
			if (pos != size)
			{
				if (pos <= (int)sizeof(m_Head))
				{
					//无字段
					m_Head.firstid = 0;
					m_Head.records = 0;
				}
				else
				{
					m_Head.firstid = 0;
					m_Head.records = (size - sizeof(m_Head)) / m_iRowSize;
				}
				size = sizeof(m_Head)+m_iRowSize*m_Head.records;
				m_File.seekTo(size);
			}
			for (i = 0; i < iRows; i++)
			{
				PrepareObj(pObj);
			}
			ret = m_File.write((char*)pObj, m_iRowSize*iRows);
			for (i = 0; i < iRows; i++)
			{
				PrepareObj(pObj);
			}
			if (ret != m_iRowSize*iRows)
			{
				unlock();
				return false;
			}
			m_Head.records += iRows;
			if (iRows0 > 0)
			{
				iRows = iRows0;
				iRows0 = 0;
				goto again;
			}
		}
		else
		{
			//此时需要覆盖最早的数据,数据记录号从0~m_iMaxRecords-1
			int icurrrow = m_Head.firstid;
			m_Head.firstid += iRows;
			if (m_Head.firstid >= m_iMaxRecords)
				m_Head.firstid = 0;
			//在icurrrow位置写入当前记录
			m_File.seekTo(sizeof(m_Head)+m_iRowSize*icurrrow);
			for (i = 0; i < iRows; i++)
			{
				PrepareObj(pObj);
			}
			ret = m_File.write((char*)pObj, m_iRowSize*iRows);
			for (i = 0; i < iRows; i++)
			{
				PrepareObj(pObj);
			}
			if (ret != m_iRowSize*iRows)
			{
				unlock();
				return false;
			}
		}
		m_File.seekBegin();
		SKT_SWAP_DWORD(m_Head.firstid);
		SKT_SWAP_DWORD(m_Head.records);
		PrepareHdr(&m_Head.head);
		m_File.write((char*)&m_Head, sizeof(m_Head));
		SKT_SWAP_DWORD(m_Head.firstid);
		SKT_SWAP_DWORD(m_Head.records);
		PrepareHdr(&m_Head.head);
		m_bFileSaved = false;
		//m_File.flush();
		unlock();
		return true;

		//for(int i=0;i<size;i++)
		//{
		//	if (!append(pObj + i*m_iRowSize))
		//		return false;
		//}
		//return true;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  删除指定的记录
	// 作    者:  邵凯田
	// 创建时间:  2014-11-21 09:45
	// 参数说明:  @iRow为待删除记录的序号，从0开始编号
	// 返 回 值:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool remove(int iRow)
	{
		if (iRow<0 || iRow >= m_Head.records)
			return false;
		int r;
		lock();
		if (iRow != m_Head.records - 1)
		{
			//待删除的记录不是最后一条，读取最后一条的内容并写入删除位置，然后将记录尾指针前移一行
			BYTE *v = new BYTE[m_iRowSize];
			if (m_iMaxRecords != 0)
			{
				r = ((m_Head.records - 1) + m_Head.firstid) % m_iMaxRecords;
			}
			else
				r = (m_Head.records - 1);
			m_File.seekTo(sizeof(m_Head)+m_iRowSize*r);
			int ret = m_File.readBlock((BYTE*)v, m_iRowSize);
			if (ret != m_iRowSize)
			{
				unlock();
				delete[] v;
				return false;
			}
			//写入被删除位置
			if (m_iMaxRecords != 0)
			{
				r = (iRow + m_Head.firstid) % m_iMaxRecords;
			}
			else
				r = iRow;
			m_File.seekTo(sizeof(m_Head)+m_iRowSize*r);
			ret = m_File.writeBlock((BYTE*)v, m_iRowSize);
			delete[] v;
			if (ret != m_iRowSize)
			{
				unlock();
				return false;
			}
		}
		m_Head.records--;
		m_bFileSaved = false;
		//m_File.flush();
		unlock();
		return save();
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取总记录数
	// 作    者:  邵凯田
	// 创建时间:  2008-9-16 18:02
	// 参数说明:  
	// 返 回 值:  当前总记录数
	//////////////////////////////////////////////////////////////////////////
	int getRowCount()
	{
		return m_Head.records;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  读取指定记录的数据
	// 作    者:  邵凯田
	// 创建时间:  2008-9-17 14:22
	// 参数说明:  记录号是从最老的数据开始从0计数的
	// 返 回 值:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool readRecord(int iRow,BYTE *pObj)
	{
		if(iRow<0 || iRow>=m_Head.records)
			return false;
		int r;
		lock();
		if(m_iMaxRecords != 0)
		{
			r = (iRow + m_Head.firstid)%m_iMaxRecords;
		}
		else
			r = iRow;
		m_File.seekTo(sizeof(m_Head)+m_iRowSize*r);
		int ret = m_File.readBlock(pObj, m_iRowSize);
		unlock();
		if (ret != m_iRowSize)
			return false;
		PrepareObj(pObj);
		return true;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  读取指定记录的数据
	// 作    者:  邵凯田
	// 创建时间:  2008-9-17 14:22
	// 参数说明:  @iStartRow记录号是从最老的数据开始从0计数的
	//         :  @pObjs为待写入的缓存区
	//         :  @iRows为计划读取的行数
	// 返 回 值:  >=0表示实际读取的行数，<0表示失败
	//////////////////////////////////////////////////////////////////////////
	int readRecords(int iStartRow,BYTE* pObjs,int iRows)
	{
		if(iStartRow<0 || iStartRow>=m_Head.records)
			return 0;
		int r,r2,cnt;
		lock();
		if(iRows > m_Head.records-iStartRow)
			iRows = m_Head.records-iStartRow;
		//读取折回前和折回后两部分
		r = (iStartRow + m_Head.firstid)%m_Head.records;
		r2 = r + iRows;
		if(r2 > m_Head.records)
			r2 = m_Head.records;
		cnt = r2 - r;
		m_File.seekTo(sizeof(m_Head)+m_iRowSize*r);
		int ret = m_File.readBlock((BYTE*)pObjs, m_iRowSize*cnt);
		if (ret != m_iRowSize*cnt)
		{
			unlock();
			LOGERROR("读取记录时失败!r=%d->%d",r,r2);
			return -1;
		}
		if(cnt != iRows)
		{
			//需要折回读取
			pObjs += m_iRowSize*cnt;
			r = 0;
			cnt = r2 = iRows-cnt;
			m_File.seekTo(sizeof(m_Head)+m_iRowSize*r);
			int ret = m_File.readBlock((BYTE*)pObjs, m_iRowSize*cnt);
			if (ret != m_iRowSize*cnt)
			{
				unlock();
				LOGERROR("读取记录时失败!r=%d->%d",r,r2);
				return -2;
			}
		}
		unlock();
		for (r = 0; r < iRows; r++)
		{
			PrepareObj(pObjs+r*m_iRowSize);
		}
		return iRows;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  重写指定记录的数据
	// 作    者:  邵凯田
	// 创建时间:  2008-11-28 19:54
	// 参数说明:  记录号是从最老的数据开始从0计数的
	// 返 回 值:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool writeRecord(int iRow,BYTE* pObj)
	{
		if(iRow<0 || iRow>=m_Head.records)
			return false;
		int r;
		lock();
		if(m_iMaxRecords != 0)
		{
			r = (iRow + m_Head.firstid)%m_iMaxRecords;
		}
		else
			r = iRow;
		m_File.seekTo(sizeof(m_Head)+m_iRowSize*r);
		PrepareObj(pObj);
		int ret = m_File.writeBlock((BYTE*)pObj, m_iRowSize);
		PrepareObj(pObj);
		m_bFileSaved = false;
		//m_File.flush();
		unlock();
		if (ret != m_iRowSize)
			return false;
		return true;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  是否文件已经打开
	// 作    者:  邵凯田
	// 创建时间:  2011-3-26 12:51
	// 参数说明:  void
	// 返 回 值:  true/false
	//////////////////////////////////////////////////////////////////////////
	inline bool isOpen(){return m_File.isOpen();};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取自定义的文件头
	// 作    者:  邵凯田
	// 创建时间:  2013-3-17 13:25
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	inline hdrtype* getHdr(){return &m_Head.head;};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取文件名
	// 作    者:  邵凯田
	// 创建时间:  2014-11-20 13:19
	// 参数说明:  void
	// 返 回 值:  SString
	//////////////////////////////////////////////////////////////////////////
	inline SString getFileName(){ return m_File.filename(); };

protected:

	////////////////////////////////////////////////////////////////////////
	// 描    述:  加锁
	// 作    者:  邵凯田
	// 创建时间:  2011-11-14 16:08
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	inline void lock()
	{
		if(m_bShared)
			m_Lock.lock();
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  解锁
	// 作    者:  邵凯田
	// 创建时间:  2011-11-14 16:08
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	inline void unlock()
	{
		if(m_bShared)
			m_Lock.unlock();
	}

private:
	SFile m_File;
	bool m_bShared;//是否线程共享，如共享需要信号量互拆
	SLock m_Lock;
	stuFileHead m_Head;//文件头
	int m_iMaxRecords;//最大允许的记录数，超过则折回覆盖最早的记录,当设为0时表示不限制
	int m_iRowSize;//一行记录的总长度
	bool m_bFileSaved;//是否文件尚未保存或同步
};


template <class hdrtype, class type>
class SDbf
{
public:
	//////////////////////////////////////////////////////////////////////////
	// 名    称:  stuFileHead
	// 作    者:  邵凯田
	// 创建时间:  2008-9-1 11:14
	// 描    述:  文件头
	//////////////////////////////////////////////////////////////////////////
	struct stuFileHead
	{
		int firstid;//第一条记录所在的物理记录号（从0开始），从结束位置开始折回
		int records;//当前的总记录数
		hdrtype head;//用户定义的文件头
	};

	SDbf()
	{
		m_bShared = true;
		m_iMaxRecords = 0;
		m_bFileSaved = true;
	};
	virtual ~SDbf()
	{
		m_File.close();
	};

	////////////////////////////////////////////////////////////////////////
	// 描    述:  设置最大记录数
	// 作    者:  邵凯田
	// 创建时间:  2011-11-14 16:02
	// 参数说明:  @max为最大记录数，0表示不受限
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void SetMaxRecords(int max)	{ m_iMaxRecords = max; };

	////////////////////////////////////////////////////////////////////////
	// 描    述:  打开文件，如果文件不存在则先创建
	// 作    者:  邵凯田
	// 创建时间:  2008-9-1 10:55
	// 参数说明:  绝对路径的文件名
	// 返 回 值:  1表示打开成功，2表示创建成功，<=0表示失败
	//////////////////////////////////////////////////////////////////////////
	int open(const char* sDbFile)
	{
		stuFileHead head;
		int ret = 1;
		memset(&head, 0, sizeof(head));
		m_File.setFileName(sDbFile);
		if (!m_File.exists())
		{
			//文件不存在，需要先建立新文件
			if (!m_File.open(IO_Truncate))
			{
				LOGFAULT("创建文件(%s)失败!", sDbFile);
				return 0;
			}
			//写入文件头
			head.firstid = 0;
			m_File.write((BYTE*)&head, sizeof(head));
			m_File.close();
			ret = 2;
		}
		if (!m_File.openByMode("r+b"))
		{
			LOGFAULT("打开文件(%s)失败", sDbFile);
			return 0;
		}
		m_File.readBlock((BYTE*)&m_Head, sizeof(m_Head));
		SKT_SWAP_DWORD(m_Head.firstid);
		SKT_SWAP_DWORD(m_Head.records);
		PrepareHdr(&m_Head.head);
		m_bFileSaved = true;
		return ret;
	}

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  检查文件是否合法
	// 作    者:  邵凯田
	// 创建时间:  2013-7-1 14:37
	// 参数说明:  void
	// 返 回 值:  true合法，false不合法
	//////////////////////////////////////////////////////////////////////////
	bool check()
	{
		if (m_File.isOpen() == false)
			return false;
		if (m_iMaxRecords > 0 && m_Head.records > m_iMaxRecords)
		{
			LOGWARN("记录数无效!recs=%d", m_Head.records);
			return false;//
		}
		int size = m_File.size();
		if (size != sizeof(stuFileHead)+m_Head.records * sizeof(type))
		{
			LOGWARN("文件大小无效!size=%d,理论大小:%d", size, sizeof(stuFileHead)+m_Head.records * sizeof(type));
			return false;
		}
		return true;
	}


	////////////////////////////////////////////////////////////////////////
	// 描    述:  清空所有记录
	// 作    者:  邵凯田
	// 创建时间:  2011-11-14 16:02
	// 参数说明:  @max为最大记录数，0表示不受限
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	bool clear()
	{
		if (m_File.isOpen() == false)
			return false;
		lock();
		m_File.close();
		if (!m_File.remove())
		{
			unlock();
			return false;
		}
		if (!m_File.open(IO_Truncate))
		{
			unlock();
			LOGFAULT("创建文件(%s)失败!", m_File.filename().data());
			return false;
		}
		//写入文件头
		m_Head.firstid = 0;
		m_Head.records = 0;
		m_File.write((BYTE*)&m_Head, sizeof(m_Head));
		m_File.close();

		if (!m_File.openByMode("r+b"))
		{
			unlock();
			LOGFAULT("打开文件(%s)失败", m_File.filename().data());
			return false;
		}
		m_File.readBlock((BYTE*)&m_Head, sizeof(m_Head));
		SKT_SWAP_DWORD(m_Head.firstid);
		SKT_SWAP_DWORD(m_Head.records);
		PrepareHdr(&m_Head.head);
		m_bFileSaved = true;
		unlock();
		return true;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  关闭文件
	// 作    者:  邵凯田
	// 创建时间:  2011-11-14 16:04
	// 参数说明:  void
	// 返 回 值:  true表示关闭成功，false表示关闭失败
	//////////////////////////////////////////////////////////////////////////
	bool close()
	{
		//保存文件头
		lock();
		m_File.seekBegin();
		SKT_SWAP_DWORD(m_Head.firstid);
		SKT_SWAP_DWORD(m_Head.records);
		PrepareHdr(&m_Head.head);
		m_File.write((char*)&m_Head, sizeof(m_Head));
		SKT_SWAP_DWORD(m_Head.firstid);
		SKT_SWAP_DWORD(m_Head.records);
		PrepareHdr(&m_Head.head);
		m_bFileSaved = true;
		unlock();

		return m_File.close();
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  同步文件内容
	// 作    者:  邵凯田
	// 创建时间:  2014-12-17 18:56
	// 参数说明:  void
	// 返 回 值:  true表示同步成功，false表示同步失败
	//////////////////////////////////////////////////////////////////////////
	bool flush()
	{
		if (m_File.getHandle() == NULL)
			return false;
		if (!m_bFileSaved)
		{
			lock();
			m_File.flush();
			m_bFileSaved = true;
			unlock();
		}
		return true;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  保存文件
	// 作    者:  邵凯田
	// 创建时间:  2011-11-14 16:04
	// 参数说明:  void
	// 返 回 值:  true表示保存成功，false表示保存失败
	//////////////////////////////////////////////////////////////////////////
	bool save()
	{
		lock();

		//保存文件头
		m_File.seekBegin();
		SKT_SWAP_DWORD(m_Head.firstid);
		SKT_SWAP_DWORD(m_Head.records);
		PrepareHdr(&m_Head.head);
		m_File.write((char*)&m_Head, sizeof(m_Head));
		SKT_SWAP_DWORD(m_Head.firstid);
		SKT_SWAP_DWORD(m_Head.records);
		PrepareHdr(&m_Head.head);

		m_File.close();
		if (!m_File.openByMode("r+b"))
		{
			unlock();
			LOGFAULT("打开文件(%s)失败", m_File.filename().data());
			return false;
		}
		m_File.readBlock((BYTE*)&m_Head, sizeof(m_Head));
		SKT_SWAP_DWORD(m_Head.firstid);
		SKT_SWAP_DWORD(m_Head.records);
		PrepareHdr(&m_Head.head);
		m_bFileSaved = true;
		unlock();
		return true;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  是否线程共享模式
	// 作    者:  邵凯田
	// 创建时间:  2011-11-14 16:04
	// 参数说明:  void
	// 返 回 值:  true表示共享模式，false表示不共享模式
	//////////////////////////////////////////////////////////////////////////
	bool isShared() { return m_bShared; };

	////////////////////////////////////////////////////////////////////////
	// 描    述:  设置线程共享模式
	// 作    者:  邵凯田
	// 创建时间:  2011-11-14 16:08
	// 参数说明:  @b
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void setShared(bool b) { m_bShared = b; };

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  记录被覆盖前的处理虚函数
	// 作    者:  邵凯田
	// 创建时间:  2013-3-17 14:15
	// 参数说明:  @pObj
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void BeforeOverwrite(type *pObj)
	{
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  写前及读后的预处理虚函数，可用于处理字节顺序等问题,由实现类处理
	// 作    者:  邵凯田
	// 创建时间:  2011-11-14 16:08
	// 参数说明:  @pObj处理对象
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void PrepareObj(type *pObj)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  写前 及读后的预处理虚函数，处理文件头结构，可用于处理字节顺序等问题,由实现类处理
	// 作    者:  邵凯田
	// 创建时间:  2013-3-17 13:27
	// 参数说明:  @pHdr待处理的对象
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void PrepareHdr(hdrtype *pHdr)
	{
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  添加一条记录
	// 作    者:  邵凯田
	// 创建时间:  2008-9-1 11:07
	// 参数说明:  type*
	// 返 回 值:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool append(type *pObj)
	{
		int ret;
		lock();
		if (m_iMaxRecords == 0 || m_Head.records < m_iMaxRecords)
		{
			//数据无限制
			//防止文件末尾有无效的字节，不采用seedEnd的方法，而使用计算的偏移长度
			//m_File.seekEnd();
			int size = sizeof(m_Head)+sizeof(type)*m_Head.records;
			m_File.seekTo(size);
			int pos = m_File.position();
			if (pos != size)
			{
				if (pos <= (int)sizeof(m_Head))
				{
					//无字段
					m_Head.firstid = 0;
					m_Head.records = 0;
				}
				else
				{
					m_Head.firstid = 0;
					m_Head.records = (size - sizeof(m_Head)) / sizeof(type);
				}
				size = sizeof(m_Head)+sizeof(type)*m_Head.records;
				m_File.seekTo(size);
			}

			PrepareObj(pObj);
			ret = m_File.write((char*)pObj, sizeof(type));
			PrepareObj(pObj);
			if (ret != sizeof(type))
			{
				unlock();
				return false;
			}
			m_Head.records++;

		}
		else
		{
			//此时需要覆盖最早的数据,数据记录号从0~m_iMaxRecords-1
			int icurrrow = m_Head.firstid;
			m_Head.firstid++;
			if (m_Head.firstid >= m_iMaxRecords)
				m_Head.firstid = 0;
			//在icurrrow位置写入当前记录
			m_File.seekTo(sizeof(m_Head)+sizeof(type)*icurrrow);
			PrepareObj(pObj);
			ret = m_File.write((char*)pObj, sizeof(type));
			PrepareObj(pObj);
			if (ret != sizeof(type))
			{
				unlock();
				return false;
			}
		}
		m_File.seekBegin();
		SKT_SWAP_DWORD(m_Head.firstid);
		SKT_SWAP_DWORD(m_Head.records);
		PrepareHdr(&m_Head.head);
		m_File.write((char*)&m_Head, sizeof(m_Head));
		SKT_SWAP_DWORD(m_Head.firstid);
		SKT_SWAP_DWORD(m_Head.records);
		PrepareHdr(&m_Head.head);
		m_bFileSaved = false;
		//m_File.flush();
		unlock();
		return true;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  添加一批记录
	// 作    者:  邵凯田
	// 创建时间:  2008-9-1 11:07
	// 参数说明:  type*,size
	// 返 回 值:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool append(type *pObj, int iRows)
	{
		int ret, i;
		lock();
		int iRows0 = 0;
		if (m_iMaxRecords != 0 && m_Head.records + iRows > m_iMaxRecords)
		{
			iRows0 = iRows - (m_Head.records - m_iMaxRecords);
			iRows = iRows - iRows0;
		}
	again:
		if (m_iMaxRecords == 0 || m_Head.records + iRows <= m_iMaxRecords)
		{
			//数据无限制
			//防止文件末尾有无效的字节，不采用seedEnd的方法，而使用计算的偏移长度
			//m_File.seekEnd();
			int size = sizeof(m_Head)+sizeof(type)*m_Head.records;
			m_File.seekTo(size);
			int pos = m_File.position();
			if (pos != size)
			{
				if (pos <= (int)sizeof(m_Head))
				{
					//无字段
					m_Head.firstid = 0;
					m_Head.records = 0;
				}
				else
				{
					m_Head.firstid = 0;
					m_Head.records = (size - sizeof(m_Head)) / sizeof(type);
				}
				size = sizeof(m_Head)+sizeof(type)*m_Head.records;
				m_File.seekTo(size);
			}
			for (i = 0; i < iRows; i++)
			{
				PrepareObj(pObj);
			}
			ret = m_File.write((char*)pObj, sizeof(type)*iRows);
			for (i = 0; i < iRows; i++)
			{
				PrepareObj(pObj);
			}
			if (ret != sizeof(type)*iRows)
			{
				unlock();
				return false;
			}
			m_Head.records += iRows;
			if (iRows0 > 0)
			{
				iRows = iRows0;
				iRows0 = 0;
				goto again;
			}
		}
		else
		{
			//此时需要覆盖最早的数据,数据记录号从0~m_iMaxRecords-1
			int icurrrow = m_Head.firstid;
			m_Head.firstid += iRows;
			if (m_Head.firstid >= m_iMaxRecords)
				m_Head.firstid = 0;
			//在icurrrow位置写入当前记录
			m_File.seekTo(sizeof(m_Head)+sizeof(type)*icurrrow);
			for (i = 0; i < iRows; i++)
			{
				PrepareObj(pObj);
			}
			ret = m_File.write((char*)pObj, sizeof(type)*iRows);
			for (i = 0; i < iRows; i++)
			{
				PrepareObj(pObj);
			}
			if (ret != sizeof(type)*iRows)
			{
				unlock();
				return false;
			}
		}
		m_File.seekBegin();
		SKT_SWAP_DWORD(m_Head.firstid);
		SKT_SWAP_DWORD(m_Head.records);
		PrepareHdr(&m_Head.head);
		m_File.write((char*)&m_Head, sizeof(m_Head));
		SKT_SWAP_DWORD(m_Head.firstid);
		SKT_SWAP_DWORD(m_Head.records);
		PrepareHdr(&m_Head.head);
		m_bFileSaved = false;
		//m_File.flush();
		unlock();
		return true;

		//for (int i = 0; i<size; i++)
		//{
		//	if (!append(ppObj[i]))
		//		return false;
		//}
		//return true;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  删除指定的记录
	// 作    者:  邵凯田
	// 创建时间:  2014-11-21 09:45
	// 参数说明:  @iRow为待删除记录的序号，从0开始编号
	// 返 回 值:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool remove(int iRow)
	{
		if (iRow<0 || iRow >= m_Head.records)
			return false;
		int r;
		lock();
		if (iRow != m_Head.records - 1)
		{
			//待删除的记录不是最后一条，读取最后一条的内容并写入删除位置，然后将记录尾指针前移一行
			type v;
			if (m_iMaxRecords != 0)
			{
				r = ((m_Head.records - 1) + m_Head.firstid) % m_iMaxRecords;
			}
			else
				r = (m_Head.records - 1);
			m_File.seekTo(sizeof(m_Head)+sizeof(type)*r);
			int ret = m_File.readBlock((BYTE*)&v, sizeof(type));
			if (ret != sizeof(type))
			{
				unlock();
				return false;
			}
			//写入被删除位置
			if (m_iMaxRecords != 0)
			{
				r = (iRow + m_Head.firstid) % m_iMaxRecords;
			}
			else
				r = iRow;
			m_File.seekTo(sizeof(m_Head)+sizeof(type)*r);
			ret = m_File.writeBlock((BYTE*)&v, (unsigned int)sizeof(type));
			if (ret != sizeof(type))
			{
				unlock();
				return false;
			}
		}
		m_Head.records--;
		m_bFileSaved = false;
		//m_File.flush();
		unlock();
		return save();
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取总记录数
	// 作    者:  邵凯田
	// 创建时间:  2008-9-16 18:02
	// 参数说明:  
	// 返 回 值:  当前总记录数
	//////////////////////////////////////////////////////////////////////////
	int getRowCount()
	{
		return m_Head.records;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  读取指定记录的数据
	// 作    者:  邵凯田
	// 创建时间:  2008-9-17 14:22
	// 参数说明:  记录号是从最老的数据开始从0计数的
	// 返 回 值:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool readRecord(int iRow, type &obj)
	{
		if (iRow<0 || iRow >= m_Head.records)
			return false;
		int r;
		lock();
		if (m_iMaxRecords != 0)
		{
			r = (iRow + m_Head.firstid) % m_iMaxRecords;
		}
		else
			r = iRow;
		m_File.seekTo(sizeof(m_Head)+sizeof(type)*r);
		int ret = m_File.readBlock((BYTE*)&obj, sizeof(type));
		unlock();
		if (ret != sizeof(type))
			return false;
		PrepareObj(&obj);
		return true;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  读取指定记录的数据
	// 作    者:  邵凯田
	// 创建时间:  2008-9-17 14:22
	// 参数说明:  @iStartRow记录号是从最老的数据开始从0计数的
	//         :  @pObjs为待写入的缓存区
	//         :  @iRows为计划读取的行数
	// 返 回 值:  >=0表示实际读取的行数，<0表示失败
	//////////////////////////////////////////////////////////////////////////
	int readRecords(int iStartRow, type* pObjs, int iRows)
	{
		if (iStartRow<0 || iStartRow >= m_Head.records)
			return 0;
		int r, r2, cnt;
		lock();
		if (iRows > m_Head.records - iStartRow)
			iRows = m_Head.records - iStartRow;
		//读取折回前和折回后两部分
		r = (iStartRow + m_Head.firstid) % m_Head.records;
		r2 = r + iRows;
		if (r2 > m_Head.records)
			r2 = m_Head.records;
		cnt = r2 - r;
		m_File.seekTo(sizeof(m_Head)+sizeof(type)*r);
		int ret = m_File.readBlock((BYTE*)pObjs, sizeof(type)*cnt);
		if (ret != sizeof(type)*cnt)
		{
			unlock();
			LOGERROR("读取记录时失败!r=%d->%d", r, r2);
			return -1;
		}
		if (cnt != iRows)
		{
			//需要折回读取
			pObjs += cnt;
			r = 0;
			cnt = r2 = iRows - cnt;
			m_File.seekTo(sizeof(m_Head)+sizeof(type)*r);
			int ret = m_File.readBlock((BYTE*)pObjs, sizeof(type)*cnt);
			if (ret != sizeof(type)*cnt)
			{
				unlock();
				LOGERROR("读取记录时失败!r=%d->%d", r, r2);
				return -2;
			}
		}
		unlock();
		for (r = 0; r<iRows; r++)
			PrepareObj(&pObjs[r]);
		return iRows;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  重写指定记录的数据
	// 作    者:  邵凯田
	// 创建时间:  2008-11-28 19:54
	// 参数说明:  记录号是从最老的数据开始从0计数的
	// 返 回 值:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool writeRecord(int iRow, type &obj)
	{
		if (iRow<0 || iRow >= m_Head.records)
			return false;
		int r;
		lock();
		if (m_iMaxRecords != 0)
		{
			r = (iRow + m_Head.firstid) % m_iMaxRecords;
		}
		else
			r = iRow;
		m_File.seekTo(sizeof(m_Head)+sizeof(type)*r);
		PrepareObj(&obj);
		int ret = m_File.writeBlock((BYTE*)&obj, (unsigned int)sizeof(type));
		PrepareObj(&obj);
		//m_File.flush();
		m_bFileSaved = false;
		unlock();
		if (ret != sizeof(type))
			return false;
		return true;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  是否文件已经打开
	// 作    者:  邵凯田
	// 创建时间:  2011-3-26 12:51
	// 参数说明:  void
	// 返 回 值:  true/false
	//////////////////////////////////////////////////////////////////////////
	inline bool isOpen(){ return m_File.isOpen(); };

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取自定义的文件头
	// 作    者:  邵凯田
	// 创建时间:  2013-3-17 13:25
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	inline hdrtype* getHdr(){ return &m_Head.head; };
protected:

	////////////////////////////////////////////////////////////////////////
	// 描    述:  加锁
	// 作    者:  邵凯田
	// 创建时间:  2011-11-14 16:08
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	inline void lock()
	{
		if (m_bShared)
			m_Lock.lock();
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  解锁
	// 作    者:  邵凯田
	// 创建时间:  2011-11-14 16:08
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	inline void unlock()
	{
		if (m_bShared)
			m_Lock.unlock();
	}

private:
	SFile m_File;
	bool m_bShared;//是否线程共享，如共享需要信号量互拆
	SLock m_Lock;
	stuFileHead m_Head;//文件头
	int m_iMaxRecords;//最大允许的记录数，超过则折回覆盖最早的记录,当设为0时表示不限制
	bool m_bFileSaved;//是否文件尚未保存或同步
};


#endif//__SKTDBF_H__
