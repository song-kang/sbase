/**
 *
 * �� �� �� : SDbf.h
 * �������� : 2008-9-1 10:39
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: 2011/08/31 08:43:33 $
 * ��ǰ�汾 : $Revision: 1.1 $
 * �������� : �ṹʽ�ļ����ݿ�ģ��
 * �޸ļ�¼ : 
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
// ��    ��:  SDbf<class type>
// ��    ��:  �ۿ���
// ����ʱ��:  2008-9-1 11:11
// ��    ��:  DBF�ļ�������
//////////////////////////////////////////////////////////////////////////

template <class hdrtype>
class SVarDbf
{
public:
	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  stuFileHead
	// ��    ��:  �ۿ���
	// ����ʱ��:  2008-9-1 11:14
	// ��    ��:  �ļ�ͷ
	//////////////////////////////////////////////////////////////////////////
	struct stuFileHead
	{
		int firstid;//��һ����¼���ڵ������¼�ţ���0��ʼ�����ӽ���λ�ÿ�ʼ�ۻ�
		int records;//��ǰ���ܼ�¼��
		hdrtype head;//�û�������ļ�ͷ
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
	// ��    ��:  ������м�¼
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 16:02
	// ����˵��:  @maxΪ����¼����0��ʾ������
	// �� �� ֵ:  void
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
			LOGFAULT("�����ļ�(%s)ʧ��!", m_File.filename().data());
			return false;
		}
		//д���ļ�ͷ
		m_Head.firstid = 0;
		m_Head.records = 0;
		m_File.write((BYTE*)&m_Head, sizeof(m_Head));
		m_File.close();

		if (!m_File.openByMode((char*)"r+b"))
		{
			unlock();
			LOGFAULT("���ļ�(%s)ʧ��", m_File.filename().data());
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
	// ��    ��:  ��������¼��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 16:02
	// ����˵��:  @maxΪ����¼����0��ʾ������
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void SetMaxRecords(int max)	{m_iMaxRecords = max;};

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���ļ�������ļ����������ȴ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2008-9-1 10:55
	// ����˵��:  ����·�����ļ���
	// �� �� ֵ:  1��ʾ�򿪳ɹ���2��ʾ�����ɹ���<=0��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	int open(const char* sDbFile)
	{
		stuFileHead head;
		int ret = 1;
		memset(&head, 0, sizeof(head));
		m_File.setFileName(sDbFile);
		if (!m_File.exists())
		{
			//�ļ������ڣ���Ҫ�Ƚ������ļ�
			if (!m_File.open(IO_Truncate))
			{
				LOGFAULT("�����ļ�(%s)ʧ��!", sDbFile);
				return 0;
			}
			//д���ļ�ͷ
			head.firstid = 0;
			m_File.write((BYTE*)&head, sizeof(head));
			m_File.close();
			ret = 2;
		}
		if (!m_File.openByMode("r+b"))
		{
			LOGFAULT("���ļ�(%s)ʧ��", sDbFile);
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
	// ��    ��:  ����ļ��Ƿ�Ϸ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2013-7-1 14:37
	// ����˵��:  void
	// �� �� ֵ:  true�Ϸ���false���Ϸ�
	//////////////////////////////////////////////////////////////////////////
	bool check()
	{
		if(m_File.isOpen() == false)
			return false;
		if(m_iMaxRecords > 0 && m_Head.records > m_iMaxRecords)
		{
			LOGWARN("��¼����Ч!recs=%d",m_Head.records);
			return false;//
		}
		int size = m_File.size();
		if(size != sizeof(stuFileHead) + m_Head.records * m_iRowSize)
		{
			LOGWARN("�ļ���С��Ч!size=%d,���۴�С:%d", size, sizeof(stuFileHead)+m_Head.records * m_iRowSize);
			return false;
		}
		return true;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �ر��ļ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 16:04
	// ����˵��:  void
	// �� �� ֵ:  true��ʾ�رճɹ���false��ʾ�ر�ʧ��
	//////////////////////////////////////////////////////////////////////////
	bool close()
	{
		//�����ļ�ͷ
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
	// ��    ��:  ͬ���ļ�����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-12-17 18:56
	// ����˵��:  void
	// �� �� ֵ:  true��ʾͬ���ɹ���false��ʾͬ��ʧ��
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
	// ��    ��:  �����ļ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 16:04
	// ����˵��:  void
	// �� �� ֵ:  true��ʾ����ɹ���false��ʾ����ʧ��
	//////////////////////////////////////////////////////////////////////////
	bool save()
	{
		lock();

		//�����ļ�ͷ
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
			LOGFAULT("���ļ�(%s)ʧ��",m_File.filename().data());
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
	// ��    ��:  �Ƿ��̹߳���ģʽ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 16:04
	// ����˵��:  void
	// �� �� ֵ:  true��ʾ����ģʽ��false��ʾ������ģʽ
	//////////////////////////////////////////////////////////////////////////
	bool isShared() {return m_bShared;};

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����̹߳���ģʽ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 16:08
	// ����˵��:  @b
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void setShared(bool b) {m_bShared=b;};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��¼������ǰ�Ĵ����麯��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2013-3-17 14:15
	// ����˵��:  @pObj
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void BeforeOverwrite(BYTE *pObj)
	{
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  дǰ�������Ԥ�����麯���������ڴ����ֽ�˳�������,��ʵ���ദ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 16:08
	// ����˵��:  @pObj�������
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void PrepareObj(BYTE *pObj)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  дǰ �������Ԥ�����麯���������ļ�ͷ�ṹ�������ڴ����ֽ�˳�������,��ʵ���ദ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2013-3-17 13:27
	// ����˵��:  @pHdr������Ķ���
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void PrepareHdr(hdrtype *pHdr)
	{
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���һ����¼
	// ��    ��:  �ۿ���
	// ����ʱ��:  2008-9-1 11:07
	// ����˵��:  type*
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool append(BYTE *pObj)
	{
		int ret;
		lock();
		if(m_iMaxRecords == 0 || m_Head.records < m_iMaxRecords)
		{
			//����������
			//��ֹ�ļ�ĩβ����Ч���ֽڣ�������seedEnd�ķ�������ʹ�ü����ƫ�Ƴ���
			//m_File.seekEnd();
			int size = sizeof(m_Head)+m_iRowSize*m_Head.records;
			m_File.seekTo(size);
			int pos = m_File.position();
			if(pos != size)
			{
				if(pos <= (int)sizeof(m_Head))
				{
					//���ֶ�
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
			//��ʱ��Ҫ�������������,���ݼ�¼�Ŵ�0~m_iMaxRecords-1
			int icurrrow = m_Head.firstid;
			m_Head.firstid ++;
			if(m_Head.firstid >= m_iMaxRecords)
				m_Head.firstid = 0;
			//��icurrrowλ��д�뵱ǰ��¼
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
	// ��    ��:  ���һ����¼
	// ��    ��:  �ۿ���
	// ����ʱ��:  2008-9-1 11:07
	// ����˵��:  type**,size
	// �� �� ֵ:  true/false
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
			//����������
			//��ֹ�ļ�ĩβ����Ч���ֽڣ�������seedEnd�ķ�������ʹ�ü����ƫ�Ƴ���
			//m_File.seekEnd();
			int size = sizeof(m_Head)+m_iRowSize*m_Head.records;
			m_File.seekTo(size);
			int pos = m_File.position();
			if (pos != size)
			{
				if (pos <= (int)sizeof(m_Head))
				{
					//���ֶ�
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
			//��ʱ��Ҫ�������������,���ݼ�¼�Ŵ�0~m_iMaxRecords-1
			int icurrrow = m_Head.firstid;
			m_Head.firstid += iRows;
			if (m_Head.firstid >= m_iMaxRecords)
				m_Head.firstid = 0;
			//��icurrrowλ��д�뵱ǰ��¼
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
	// ��    ��:  ɾ��ָ���ļ�¼
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-11-21 09:45
	// ����˵��:  @iRowΪ��ɾ����¼����ţ���0��ʼ���
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool remove(int iRow)
	{
		if (iRow<0 || iRow >= m_Head.records)
			return false;
		int r;
		lock();
		if (iRow != m_Head.records - 1)
		{
			//��ɾ���ļ�¼�������һ������ȡ���һ�������ݲ�д��ɾ��λ�ã�Ȼ�󽫼�¼βָ��ǰ��һ��
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
			//д�뱻ɾ��λ��
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
	// ��    ��:  ȡ�ܼ�¼��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2008-9-16 18:02
	// ����˵��:  
	// �� �� ֵ:  ��ǰ�ܼ�¼��
	//////////////////////////////////////////////////////////////////////////
	int getRowCount()
	{
		return m_Head.records;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ȡָ����¼������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2008-9-17 14:22
	// ����˵��:  ��¼���Ǵ����ϵ����ݿ�ʼ��0������
	// �� �� ֵ:  true/false
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
	// ��    ��:  ��ȡָ����¼������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2008-9-17 14:22
	// ����˵��:  @iStartRow��¼���Ǵ����ϵ����ݿ�ʼ��0������
	//         :  @pObjsΪ��д��Ļ�����
	//         :  @iRowsΪ�ƻ���ȡ������
	// �� �� ֵ:  >=0��ʾʵ�ʶ�ȡ��������<0��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	int readRecords(int iStartRow,BYTE* pObjs,int iRows)
	{
		if(iStartRow<0 || iStartRow>=m_Head.records)
			return 0;
		int r,r2,cnt;
		lock();
		if(iRows > m_Head.records-iStartRow)
			iRows = m_Head.records-iStartRow;
		//��ȡ�ۻ�ǰ���ۻغ�������
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
			LOGERROR("��ȡ��¼ʱʧ��!r=%d->%d",r,r2);
			return -1;
		}
		if(cnt != iRows)
		{
			//��Ҫ�ۻض�ȡ
			pObjs += m_iRowSize*cnt;
			r = 0;
			cnt = r2 = iRows-cnt;
			m_File.seekTo(sizeof(m_Head)+m_iRowSize*r);
			int ret = m_File.readBlock((BYTE*)pObjs, m_iRowSize*cnt);
			if (ret != m_iRowSize*cnt)
			{
				unlock();
				LOGERROR("��ȡ��¼ʱʧ��!r=%d->%d",r,r2);
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
	// ��    ��:  ��дָ����¼������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2008-11-28 19:54
	// ����˵��:  ��¼���Ǵ����ϵ����ݿ�ʼ��0������
	// �� �� ֵ:  true/false
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
	// ��    ��:  �Ƿ��ļ��Ѿ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-3-26 12:51
	// ����˵��:  void
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	inline bool isOpen(){return m_File.isOpen();};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ�Զ�����ļ�ͷ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2013-3-17 13:25
	// ����˵��:  
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	inline hdrtype* getHdr(){return &m_Head.head;};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ�ļ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-11-20 13:19
	// ����˵��:  void
	// �� �� ֵ:  SString
	//////////////////////////////////////////////////////////////////////////
	inline SString getFileName(){ return m_File.filename(); };

protected:

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 16:08
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	inline void lock()
	{
		if(m_bShared)
			m_Lock.lock();
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 16:08
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	inline void unlock()
	{
		if(m_bShared)
			m_Lock.unlock();
	}

private:
	SFile m_File;
	bool m_bShared;//�Ƿ��̹߳����繲����Ҫ�ź�������
	SLock m_Lock;
	stuFileHead m_Head;//�ļ�ͷ
	int m_iMaxRecords;//�������ļ�¼�����������ۻظ�������ļ�¼,����Ϊ0ʱ��ʾ������
	int m_iRowSize;//һ�м�¼���ܳ���
	bool m_bFileSaved;//�Ƿ��ļ���δ�����ͬ��
};


template <class hdrtype, class type>
class SDbf
{
public:
	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  stuFileHead
	// ��    ��:  �ۿ���
	// ����ʱ��:  2008-9-1 11:14
	// ��    ��:  �ļ�ͷ
	//////////////////////////////////////////////////////////////////////////
	struct stuFileHead
	{
		int firstid;//��һ����¼���ڵ������¼�ţ���0��ʼ�����ӽ���λ�ÿ�ʼ�ۻ�
		int records;//��ǰ���ܼ�¼��
		hdrtype head;//�û�������ļ�ͷ
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
	// ��    ��:  ��������¼��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 16:02
	// ����˵��:  @maxΪ����¼����0��ʾ������
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void SetMaxRecords(int max)	{ m_iMaxRecords = max; };

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���ļ�������ļ����������ȴ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2008-9-1 10:55
	// ����˵��:  ����·�����ļ���
	// �� �� ֵ:  1��ʾ�򿪳ɹ���2��ʾ�����ɹ���<=0��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	int open(const char* sDbFile)
	{
		stuFileHead head;
		int ret = 1;
		memset(&head, 0, sizeof(head));
		m_File.setFileName(sDbFile);
		if (!m_File.exists())
		{
			//�ļ������ڣ���Ҫ�Ƚ������ļ�
			if (!m_File.open(IO_Truncate))
			{
				LOGFAULT("�����ļ�(%s)ʧ��!", sDbFile);
				return 0;
			}
			//д���ļ�ͷ
			head.firstid = 0;
			m_File.write((BYTE*)&head, sizeof(head));
			m_File.close();
			ret = 2;
		}
		if (!m_File.openByMode("r+b"))
		{
			LOGFAULT("���ļ�(%s)ʧ��", sDbFile);
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
	// ��    ��:  ����ļ��Ƿ�Ϸ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2013-7-1 14:37
	// ����˵��:  void
	// �� �� ֵ:  true�Ϸ���false���Ϸ�
	//////////////////////////////////////////////////////////////////////////
	bool check()
	{
		if (m_File.isOpen() == false)
			return false;
		if (m_iMaxRecords > 0 && m_Head.records > m_iMaxRecords)
		{
			LOGWARN("��¼����Ч!recs=%d", m_Head.records);
			return false;//
		}
		int size = m_File.size();
		if (size != sizeof(stuFileHead)+m_Head.records * sizeof(type))
		{
			LOGWARN("�ļ���С��Ч!size=%d,���۴�С:%d", size, sizeof(stuFileHead)+m_Head.records * sizeof(type));
			return false;
		}
		return true;
	}


	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ������м�¼
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 16:02
	// ����˵��:  @maxΪ����¼����0��ʾ������
	// �� �� ֵ:  void
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
			LOGFAULT("�����ļ�(%s)ʧ��!", m_File.filename().data());
			return false;
		}
		//д���ļ�ͷ
		m_Head.firstid = 0;
		m_Head.records = 0;
		m_File.write((BYTE*)&m_Head, sizeof(m_Head));
		m_File.close();

		if (!m_File.openByMode("r+b"))
		{
			unlock();
			LOGFAULT("���ļ�(%s)ʧ��", m_File.filename().data());
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
	// ��    ��:  �ر��ļ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 16:04
	// ����˵��:  void
	// �� �� ֵ:  true��ʾ�رճɹ���false��ʾ�ر�ʧ��
	//////////////////////////////////////////////////////////////////////////
	bool close()
	{
		//�����ļ�ͷ
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
	// ��    ��:  ͬ���ļ�����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-12-17 18:56
	// ����˵��:  void
	// �� �� ֵ:  true��ʾͬ���ɹ���false��ʾͬ��ʧ��
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
	// ��    ��:  �����ļ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 16:04
	// ����˵��:  void
	// �� �� ֵ:  true��ʾ����ɹ���false��ʾ����ʧ��
	//////////////////////////////////////////////////////////////////////////
	bool save()
	{
		lock();

		//�����ļ�ͷ
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
			LOGFAULT("���ļ�(%s)ʧ��", m_File.filename().data());
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
	// ��    ��:  �Ƿ��̹߳���ģʽ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 16:04
	// ����˵��:  void
	// �� �� ֵ:  true��ʾ����ģʽ��false��ʾ������ģʽ
	//////////////////////////////////////////////////////////////////////////
	bool isShared() { return m_bShared; };

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����̹߳���ģʽ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 16:08
	// ����˵��:  @b
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void setShared(bool b) { m_bShared = b; };

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��¼������ǰ�Ĵ����麯��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2013-3-17 14:15
	// ����˵��:  @pObj
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void BeforeOverwrite(type *pObj)
	{
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  дǰ�������Ԥ�����麯���������ڴ����ֽ�˳�������,��ʵ���ദ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 16:08
	// ����˵��:  @pObj�������
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void PrepareObj(type *pObj)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  дǰ �������Ԥ�����麯���������ļ�ͷ�ṹ�������ڴ����ֽ�˳�������,��ʵ���ദ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2013-3-17 13:27
	// ����˵��:  @pHdr������Ķ���
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void PrepareHdr(hdrtype *pHdr)
	{
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���һ����¼
	// ��    ��:  �ۿ���
	// ����ʱ��:  2008-9-1 11:07
	// ����˵��:  type*
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool append(type *pObj)
	{
		int ret;
		lock();
		if (m_iMaxRecords == 0 || m_Head.records < m_iMaxRecords)
		{
			//����������
			//��ֹ�ļ�ĩβ����Ч���ֽڣ�������seedEnd�ķ�������ʹ�ü����ƫ�Ƴ���
			//m_File.seekEnd();
			int size = sizeof(m_Head)+sizeof(type)*m_Head.records;
			m_File.seekTo(size);
			int pos = m_File.position();
			if (pos != size)
			{
				if (pos <= (int)sizeof(m_Head))
				{
					//���ֶ�
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
			//��ʱ��Ҫ�������������,���ݼ�¼�Ŵ�0~m_iMaxRecords-1
			int icurrrow = m_Head.firstid;
			m_Head.firstid++;
			if (m_Head.firstid >= m_iMaxRecords)
				m_Head.firstid = 0;
			//��icurrrowλ��д�뵱ǰ��¼
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
	// ��    ��:  ���һ����¼
	// ��    ��:  �ۿ���
	// ����ʱ��:  2008-9-1 11:07
	// ����˵��:  type*,size
	// �� �� ֵ:  true/false
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
			//����������
			//��ֹ�ļ�ĩβ����Ч���ֽڣ�������seedEnd�ķ�������ʹ�ü����ƫ�Ƴ���
			//m_File.seekEnd();
			int size = sizeof(m_Head)+sizeof(type)*m_Head.records;
			m_File.seekTo(size);
			int pos = m_File.position();
			if (pos != size)
			{
				if (pos <= (int)sizeof(m_Head))
				{
					//���ֶ�
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
			//��ʱ��Ҫ�������������,���ݼ�¼�Ŵ�0~m_iMaxRecords-1
			int icurrrow = m_Head.firstid;
			m_Head.firstid += iRows;
			if (m_Head.firstid >= m_iMaxRecords)
				m_Head.firstid = 0;
			//��icurrrowλ��д�뵱ǰ��¼
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
	// ��    ��:  ɾ��ָ���ļ�¼
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-11-21 09:45
	// ����˵��:  @iRowΪ��ɾ����¼����ţ���0��ʼ���
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool remove(int iRow)
	{
		if (iRow<0 || iRow >= m_Head.records)
			return false;
		int r;
		lock();
		if (iRow != m_Head.records - 1)
		{
			//��ɾ���ļ�¼�������һ������ȡ���һ�������ݲ�д��ɾ��λ�ã�Ȼ�󽫼�¼βָ��ǰ��һ��
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
			//д�뱻ɾ��λ��
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
	// ��    ��:  ȡ�ܼ�¼��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2008-9-16 18:02
	// ����˵��:  
	// �� �� ֵ:  ��ǰ�ܼ�¼��
	//////////////////////////////////////////////////////////////////////////
	int getRowCount()
	{
		return m_Head.records;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ȡָ����¼������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2008-9-17 14:22
	// ����˵��:  ��¼���Ǵ����ϵ����ݿ�ʼ��0������
	// �� �� ֵ:  true/false
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
	// ��    ��:  ��ȡָ����¼������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2008-9-17 14:22
	// ����˵��:  @iStartRow��¼���Ǵ����ϵ����ݿ�ʼ��0������
	//         :  @pObjsΪ��д��Ļ�����
	//         :  @iRowsΪ�ƻ���ȡ������
	// �� �� ֵ:  >=0��ʾʵ�ʶ�ȡ��������<0��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	int readRecords(int iStartRow, type* pObjs, int iRows)
	{
		if (iStartRow<0 || iStartRow >= m_Head.records)
			return 0;
		int r, r2, cnt;
		lock();
		if (iRows > m_Head.records - iStartRow)
			iRows = m_Head.records - iStartRow;
		//��ȡ�ۻ�ǰ���ۻغ�������
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
			LOGERROR("��ȡ��¼ʱʧ��!r=%d->%d", r, r2);
			return -1;
		}
		if (cnt != iRows)
		{
			//��Ҫ�ۻض�ȡ
			pObjs += cnt;
			r = 0;
			cnt = r2 = iRows - cnt;
			m_File.seekTo(sizeof(m_Head)+sizeof(type)*r);
			int ret = m_File.readBlock((BYTE*)pObjs, sizeof(type)*cnt);
			if (ret != sizeof(type)*cnt)
			{
				unlock();
				LOGERROR("��ȡ��¼ʱʧ��!r=%d->%d", r, r2);
				return -2;
			}
		}
		unlock();
		for (r = 0; r<iRows; r++)
			PrepareObj(&pObjs[r]);
		return iRows;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��дָ����¼������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2008-11-28 19:54
	// ����˵��:  ��¼���Ǵ����ϵ����ݿ�ʼ��0������
	// �� �� ֵ:  true/false
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
	// ��    ��:  �Ƿ��ļ��Ѿ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-3-26 12:51
	// ����˵��:  void
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	inline bool isOpen(){ return m_File.isOpen(); };

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ�Զ�����ļ�ͷ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2013-3-17 13:25
	// ����˵��:  
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	inline hdrtype* getHdr(){ return &m_Head.head; };
protected:

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 16:08
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	inline void lock()
	{
		if (m_bShared)
			m_Lock.lock();
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 16:08
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	inline void unlock()
	{
		if (m_bShared)
			m_Lock.unlock();
	}

private:
	SFile m_File;
	bool m_bShared;//�Ƿ��̹߳����繲����Ҫ�ź�������
	SLock m_Lock;
	stuFileHead m_Head;//�ļ�ͷ
	int m_iMaxRecords;//�������ļ�¼�����������ۻظ�������ļ�¼,����Ϊ0ʱ��ʾ������
	bool m_bFileSaved;//�Ƿ��ļ���δ�����ͬ��
};


#endif//__SKTDBF_H__
