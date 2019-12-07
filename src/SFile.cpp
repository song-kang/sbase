/**
 *
 * �� �� �� : SFile.cpp
 * �������� : 2007-01-01
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: 2011/08/31 08:43:33 $
 * ��ǰ�汾 : $Revision: 1.1 $
 * �������� : ��QT���ļ���
 * �޸ļ�¼ : 
 *            $Log: SFile.cpp,v $
 *            Revision 1.1  2011/08/31 08:43:33  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2011/04/19 09:10:58  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2011/02/25 03:12:07  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2011/01/17 01:24:28  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2010/08/09 03:04:43  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2010/01/22 01:24:25  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2009/09/15 02:14:07  shaokt
 *            ͨѸ��¼����װ����Ŀ�״��ύ
 *
 *            Revision 1.2  2008/09/11 05:17:25  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/01 11:38:28  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/01 08:58:01  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.7  2008/02/18 01:07:55  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.6  2007/10/29 07:30:23  shaokt
 *            ��Ӷ����ַ�Ƿ�Ϊ4�ı����ļ�⺯����Ϊ���ARM������pack(1)��ɵ��ڴ�����
 *
 *            Revision 1.5  2007/10/19 11:57:26  shaokt
 *            size�������ӿ�ѡ���ļ�������
 *
 *            Revision 1.4  2007/09/11 08:17:52  shaokt
 *            �������
 *
 *            Revision 1.3  2007/09/04 01:43:38  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2007/08/16 08:10:25  shaokt
 *            ���������ļ�ʱ����'b'��־�������0x0D����β
 *
 *            Revision 1.1  2007/08/15 03:09:22  shaokt
 *            *** empty log message ***
 *
 **/

#include "SFile.h"
#include "SLog.h"

static SString g_currDir;

//////////////////////////////////////////////////////////////////////////
// ��    ��:  SFile
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 10:08
// ��    ��:  �ļ���
//////////////////////////////////////////////////////////////////////////

SFile::SFile()
{
	SKTOBJECT("SFile");
	m_fp = 0;
}

SFile::SFile(SString fileName)  
{
	SKTOBJECT("SFile");
	m_fileName = fileName;m_fp=0;
}

SFile::~SFile()
{
	close();
}

void SFile::setName(SString name)
{
	m_fileName = name;
#ifdef WIN32
	if(name.left(2).right(1) != ":")
#else
	if(name.left(1) != "/")
#endif
	{
		if(g_currDir.right(1) != "/")
			g_currDir += "/";
		m_fileName = g_currDir+m_fileName;
	}
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  �ر��ļ�
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 10:19
// ����˵��:  void
// �� �� ֵ:  true��ʾ�رճɹ�,flase��ʾ�ر�ʧ��
//////////////////////////////////////////////////////////////////////////
bool SFile::close()
{
	if(m_fp)
	{
		fclose(m_fp);
		m_fp = 0;
	}
	return true;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ���ļ�
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 10:20
// ����˵��:  @mode��ʾ�򿪷�ʽ
// �� �� ֵ:  true��ʾ�򿪳ɹ���false��ʾ��ʧ��
//////////////////////////////////////////////////////////////////////////
bool SFile::open(OpenMode mode)
{
	if(m_fp)
	{
		return false;
	}
	
	SString m="";
	if(mode == IO_Append)
		m="ab+";//rb+ //"wb+";
	else if(mode == IO_ReadOnly)
		m="rb";
	else if(mode == IO_ReadWrite)
		m= "r+b";//"arb+";
	else if(mode == IO_Translate)
		m= "rtb";
	else if(mode == IO_Truncate)
		m="wb";
	else if(mode == IO_WriteOnly)
		m="wb";
	m_fp = fopen(m_fileName.data(), m.data());
	if(m_fp == 0)
		return false;
	return true;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ָ����ģʽ�ַ������ļ�
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 10:20
// ����˵��:  @modeΪģʽ����"wb"
// �� �� ֵ:  true��ʾ�򿪳ɹ���false��ʾ��ʧ��
//////////////////////////////////////////////////////////////////////////
bool SFile::openByMode(const char* mode)
{
	if(m_fp)
	{
		return false;
	}
	m_fp = fopen(m_fileName.data(), mode);
	if(m_fp == 0)
		return false;
	return true;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ɾ����ǰ�ļ�
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 10:21
// ����˵��:  void
// �� �� ֵ:  trueɾ���ɹ���falseɾ��ʧ��
//////////////////////////////////////////////////////////////////////////
bool SFile::remove()
{
	return remove(m_fileName.data()) == 0;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��������ǰ���ļ�
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 10:21
// ����˵��:  @newname��ʾ�µ��ļ���
// �� �� ֵ:  true��ʾ�����ɹ���false��ʾ����ʧ��
//////////////////////////////////////////////////////////////////////////
bool SFile::rename(SString newName)
{
	return rename(m_fileName, newName) == 0;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ǰ�ļ��Ƿ����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 10:22
// ����˵��:  void
// �� �� ֵ:  true��ʾ�ļ�����,false��ʾ�ļ�������
//////////////////////////////////////////////////////////////////////////
bool SFile::exists()
{
	return exists(m_fileName.data());
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ���Ƶ�ǰ�ļ�������һ���ļ�
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 10:22
// ����˵��:  @newFile���ļ����ļ���
// �� �� ֵ:  true��ʾ���Ƴɹ���false��ʾ����ʧ��
//////////////////////////////////////////////////////////////////////////
bool SFile::copy(SString newFile)
{
	newFile=newFile;return true;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ȡһ�����ݿ�
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 10:24
// ����˵��:  @pBufferΪ��д��Ļ�����
//            @maxSizeΪ���������ܴ�С,Ҳ�ǵ�ǰ������ȡ������ֽ���
// �� �� ֵ:  ʵ�ʶ�ȡ�����ֽ�����,<0��ʾʧ��
//////////////////////////////////////////////////////////////////////////
long SFile::readBlock(BYTE *pBuff, int maxSize)
{
	if(m_fp == 0)
		return -1;
	return (long)fread(pBuff,1,maxSize, m_fp);
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  д��ָ��������0�ֽ�
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 10:27
// ����˵��:  @bytes��ʾҪ������ֽ�����
// �� �� ֵ:  ����1��ʾ�ɹ���<0��ʾ�쳣
//////////////////////////////////////////////////////////////////////////
long SFile::writeZero(int bytes)//д��ָ��������0�ֽ�
{
	if(m_fp == 0)
		return -1;
	int size = 0;
	BYTE *zero = new BYTE[4096];
	memset(zero,0,4096);
	while(bytes > 0)
	{
		size = bytes;
		if(size > 4096)
			size = 4096;
		fwrite(zero,1,size,m_fp);
		bytes -= size;
	}
	delete[] zero;
	return 1;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  д�����ݿ�
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 10:27
// ����˵��:  @pBuff��ʾ��������ַ
//            @size��ʾ��д�볤��
// �� �� ֵ:  ����ʵ��д�볤�ȣ�<0��ʾʧ��
//////////////////////////////////////////////////////////////////////////
long SFile::writeBlock(BYTE *pBuff, int size)
{
	if(m_fp == 0)
		return -1;
	return (long)fwrite(pBuff,1,size, m_fp);
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ȡһ�е�ָ���Ļ�����������\n���ļ������򻺳�����ʱ�˳�
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 10:30
// ����˵��:  @pBuff��ʾ��������ַ
//            @maxSize��ʾ��������С���ֽ�����
// �� �� ֵ:  ����ʵ�ʶ�ȡ���ֽ���
//////////////////////////////////////////////////////////////////////////
long SFile::readLine(char *pBuff, int maxSize)
{
	if(m_fp == 0)
		return -1;
	for(int i=0; i<maxSize-1; i++)
	{
		fread(pBuff+i,1,1,m_fp);
		if(pBuff[i] == '\n' || feof(m_fp))
		{
			pBuff[i] = '\0';
			if(pBuff[i-1] == '\r')
			{
				pBuff[i-1] = '\0';
				return i-1;
			}
			return i;
		}
	}return -1;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ȡһ�е������ַ�����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 10:32
// ����˵��:  @maxSize��ʾ�ַ�������󳤶�
// �� �� ֵ:  �����ַ���
//////////////////////////////////////////////////////////////////////////
SString SFile::readLine(int maxSize/*=10240*/)
{
	char *buf = new char[maxSize+1];
	memset(buf,0,maxSize+1);
	readLine(buf,maxSize);
	SString str=buf;
	delete[] buf;
	return str;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡָ���ļ��ļ�
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 10:41
// ����˵��:  @sFileNameΪָ�����ļ�����δָ��ʱΪ��ǰ�ļ�
// �� �� ֵ:  �����ļ���С�ֽ���
//////////////////////////////////////////////////////////////////////////
long SFile::size(SString sFileName)
{
	SFile file(sFileName==""?m_fileName:sFileName);
	if(!file.open(IO_ReadOnly))
		return -1;
	fseek(file.getHandle(),0,SEEK_END);
	long len=ftell(file.getHandle());
	file.close();
	return len;
	
}


////////////////////////////////////////////////////////////////////////
// ��    ��:  ���ļ��д�ӡ��ʽ���ַ���,����ȷ����ʽ������ַ�������С��10240�ֽڣ���������쳣
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 10:46
// ����˵��:  ͬprintf
// �� �� ֵ:  ����ʵ��д�볤�ȣ�<0��ʾʧ��
//////////////////////////////////////////////////////////////////////////
long SFile::fprintf(const char * cformat, ...)
{
	char buf[10240];
	memset(buf,0,10240);
	va_list ap;
	va_start(ap, cformat);
	vsprintf(buf,cformat,ap);
	va_end(ap);
	return writeBlock((BYTE*)buf,(int)strlen(buf));
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��λ�ļ�ָ�뵽ָ���ľ���λ��
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 10:48
// ����˵��:  @posΪ�ļ�ƫ�Ƶľ���λ��
// �� �� ֵ:  0��ʾ�ɹ���������Ϊʧ��
//////////////////////////////////////////////////////////////////////////
int SFile::at(int pos)
{
	if(m_fp == 0)
		return -1;
	return fseek(m_fp, pos, SEEK_SET);
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��λ�ļ�ָ�뵽ָ���ľ���λ��
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 10:48
// ����˵��:  @posΪ�ļ�ƫ�Ƶľ���λ��
// �� �� ֵ:  0��ʾ�ɹ���������Ϊʧ��
//////////////////////////////////////////////////////////////////////////
int SFile::seekTo(int pos)
{
	if(m_fp == 0)
		return -1;
	return fseek(m_fp, pos, SEEK_SET);
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��λ�ļ�ָ�뵽ָ�������λ��
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 10:48
// ����˵��:  @posΪ�ļ�ƫ�Ƶ����λ��
// �� �� ֵ:  0��ʾ�ɹ���������Ϊʧ��
//////////////////////////////////////////////////////////////////////////
int SFile::seekCur(int pos)
{
	if(m_fp == 0)
		return -1;
	return fseek(m_fp, pos, SEEK_CUR);
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��λ�ļ�ָ�뵽�ļ���ʼλ��
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 10:48
// ����˵��:  void
// �� �� ֵ:  0��ʾ�ɹ���������Ϊʧ��
//////////////////////////////////////////////////////////////////////////
int SFile::seekBegin()
{
	if(m_fp == 0)
		return -1;
	return fseek(m_fp, 0, SEEK_SET);
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��λ�ļ�ָ�뵽�ļ�����λ��
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 10:48
// ����˵��:  void
// �� �� ֵ:  0��ʾ�ɹ���������Ϊʧ��
//////////////////////////////////////////////////////////////////////////
int SFile::seekEnd()
{
	if(m_fp == 0)
		return -1;
	return fseek(m_fp, 0, SEEK_END);
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ����ļ���������һ�����ڽ�д���ļ�������Ч
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 11:00
// ����˵��:  void
// �� �� ֵ:  0��ʾ�ɹ���������Ϊʧ��
//////////////////////////////////////////////////////////////////////////
int SFile::flush()
{
	if(m_fp == 0)
		return -1;
	int ret = fflush(m_fp);
#ifndef WIN32
	sync();
#endif
	return ret;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ��ǰ�ļ�ָ���λ��
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 11:01
// ����˵��:  void
// �� �� ֵ:  �����ļ�ָ��ĵ�ǰλ��
//////////////////////////////////////////////////////////////////////////
int SFile::position()
{
	if(m_fp == 0)
		return -1;
	return ftell(m_fp);
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  �ж��ļ�ָ���Ƿ����ļ�ĩβ
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 11:05
// ����˵��:  void
// �� �� ֵ:  true��ʾ���ļ�ĩβ��false��ʾ����ĩβ
//////////////////////////////////////////////////////////////////////////
bool SFile::atEnd()
{
	if(feof(m_fp) == 0)
		return false;
	else
		return true;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ��ǰ�ļ���ȫ·���е��ļ�������
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 11:05
// ����˵��:  void
// �� �� ֵ:  �ļ���
//////////////////////////////////////////////////////////////////////////
SString SFile::name()
{
	int pos = m_fileName.findRev("/");
	if(m_fileName.findRev("\\") > pos)
		pos = m_fileName.findRev("\\");
	if(pos < 0)
		return m_fileName;
	pos ++;
	return m_fileName.right(m_fileName.length() - pos);
}

///static

////////////////////////////////////////////////////////////////////////
// ��    ��:  ����һ���յ����ļ���������ָ���ļ���С
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 10:25
// ����˵��:  @sPathName��ʾ�ļ�ȫ·����
//            @iFileSize��ʾ�ļ���С
// �� �� ֵ:  true��ʾ�ɹ���false��ʾʧ��
//////////////////////////////////////////////////////////////////////////
bool SFile::touchFile(SString sPathName,int iFileSize/*=0*/)//����ָ����С�Ŀ��ļ�
{
	SFile fl;
	fl.setFileName(sPathName);
	//time_t t1 = ::time(NULL);
	if(!fl.open(IO_WriteOnly))
	{
		LOGERROR("SFile::touchFile :%s failed........",sPathName.data());
		return false;
	}
#ifdef WIN32
	BYTE b=0;
	fl.seekTo(iFileSize-1);
	fl.writeBlock(&b,1);
#else
	fl.seekBegin();
	int s,slen = 65536;
	if(iFileSize > 10485760)
		slen = 1048576;
	BYTE *buf = new BYTE[slen];
	memset(buf,0,slen);
	while(iFileSize>0)
	{
		s = iFileSize;
		if(s > slen)
			s = slen;
		fl.write(buf,s);
		iFileSize -= s;
#ifdef _POWERPC //PPCƽ̨û��DMA������ʱ�ᵼ��CPU����Ϊ0
		usleep(10000);//ÿд1M��ʱ10���룬1G�ļ��໨10��
#endif
	}
	delete[] buf;
#endif
	fl.close();
	return true;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  �����ļ�
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 11:09
// ����˵��:  @fileName��ʾԴ�ļ���
//            @newFile��ʾĿ���ļ�
// �� �� ֵ:  true��ʾ���Ƴɹ���false��ʾ����ʧ��
//////////////////////////////////////////////////////////////////////////
bool SFile::copy(SString fileName, SString newFile)
{
	SFile f1(fileName);
	SFile f2(newFile);
	if(!f1.open(IO_ReadOnly) || !f2.open(IO_WriteOnly))
	{
		f1.close();
		f2.close();
		return false;
	}
	BYTE buf[1025];
	while(1)
	{
		int ret = f1.readBlock(buf,1025);
		if(ret <= 0)
			break;
		f2.writeBlock(buf,ret);
	}
	f1.close();
	f2.close();
	return true;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  �ϲ��ļ�,��Դ�ļ������ݸ��Ƶ�Ŀ���ļ��ĺ���
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 11:11
// ����˵��:  @fileFrom��ʾԴ�ļ���
//            @fileTo��ʾĿ���ļ���
// �� �� ֵ:  true��ʾ�ϲ��ɹ���false��ʾ�ϲ�ʧ��
//////////////////////////////////////////////////////////////////////////
bool SFile::merge(SString fileFrom,SString fileTo)
{
	SFile f1(fileFrom);
	SFile f2(fileTo);
	if(!f1.open(IO_ReadOnly) || !f2.open(IO_Append))
	{
		f1.close();
		f2.close();
		return false;
	}
	f2.seekEnd();
	BYTE buf[1025];
	while(1)
	{
		int ret = f1.readBlock(buf,1025);
		if(ret <= 0)
			break;
		f2.writeBlock(buf,ret);
	}
	f1.close();
	f2.close();
	return true;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ָ���ļ��Ƿ����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 11:12
// ����˵��:  @fileName��ʾ�ļ���
// �� �� ֵ:  true��ʾ�ļ����ڣ�false��ʾ�ļ�������
//////////////////////////////////////////////////////////////////////////
bool SFile::exists(SString fileName)
{
	SFile f(fileName);
	bool b=f.open(IO_ReadOnly);
	f.close();
	return b;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ɾ��ָ�����ļ�
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 11:15
// ����˵��:  @fileName��ɾ�����ļ�
// �� �� ֵ:  true��ʾɾ���ɹ���false��ʾɾ��ʧ��
//////////////////////////////////////////////////////////////////////////
bool SFile::remove(SString fileName)
{
	LOGDEBUG("ɾ���ļ�:%s",fileName.data());
	return ::remove(fileName.data()) == 0;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ������ָ�����ļ�
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 11:15
// ����˵��:  @fileNameΪԭ�ļ���
//            @newFileΪ���ļ�
// �� �� ֵ:  true��ʾ�������ɹ���false��ʾʧ��
//////////////////////////////////////////////////////////////////////////
bool SFile::rename(SString fileName,SString newFile)
{
	return ::rename(fileName.data(),newFile.data()) == 0;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡָ���ļ������ļ��޸�ʱ��
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 11:16
// ����˵��:  @fileNameΪ�ļ���
// �� �� ֵ:  SDateTime�޸�ʱ��
//////////////////////////////////////////////////////////////////////////
SDateTime SFile::filetime(SString fileName)
{
	struct stat statbuf;
	stat(fileName.data(),&statbuf);
	return SDateTime::makeDateTime(statbuf.st_mtime);
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ʾ���ļ��Ƿ��ִ��
// ��    ��:  �ۿ���
// ����ʱ��:  2009-2-11 17:12
// ����˵��:  fileName��ʾ�ļ���
// �� �� ֵ:  true:��ʾ������Ȩ��,false:��ʾû������Ȩ��
//////////////////////////////////////////////////////////////////////////
bool SFile::is_exec_able(SString fileName)
{
#ifdef WIN32
	if(fileName.right(4) == ".exe")
		return true;
	else
		return false;
#else
	struct stat statbuf;
	stat(fileName.data(),&statbuf);
	unsigned int mod = statbuf.st_mode;
//8����       9876543210
//644	1000000110100100	��X
//744	1000000111100100	1X	 bit6  ӵ����
//754	1000000111101100	2X	 bit3  ��
//755	1000000111101101	3X	 bit0  ����
//bit:6  3  0		
	if((mod & (1<<6)) != 0)
	{
		//˵����ӵ����Ȩ��
		return true;
	}
	else
		return false;
#endif	
}



////////////////////////////////////////////////////////////////////////////////////////////////////
//STextStream

STextStream::STextStream(SFile *pFile)
{
	SKTOBJECT("STextStream");
	m_pFile = pFile;
}
STextStream::~STextStream()
{
}

//operator
STextStream& STextStream::operator<<(const SString &str)
{
	m_pFile->write(str.data(),str.length());
	return *this;
}

STextStream& STextStream::operator<<(const char* pStr)
{
	SString str=pStr;
	m_pFile->write(str.data(),str.length());
	return *this;
}

SString STextStream::readLine()
{
	return m_pFile->readLine();
}

bool STextStream::atEnd()
{
	return m_pFile->atEnd();
}


//////////////////////////////////////////////////////////////////////////
// ��    ��:  SDir
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 11:20
// ��    ��:  Ŀ¼��
//////////////////////////////////////////////////////////////////////////

SDir::SDir()
{
	SKTOBJECT("SDir");
	m_fileList.setAutoDelete(true);
	m_fileAttrList.setAutoDelete(true);
}
SDir::SDir(SString dirName, SString nameFilter/*="*"*/)
{
	SKTOBJECT("SDir");
	m_sDirName = dirName;
	m_sFilter = nameFilter;
	m_fileList.setAutoDelete(true);
	m_fileAttrList.setAutoDelete(true);
}
SDir::~SDir()
{
	m_fileList.clear();
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ����ǰĿ¼�µ�һ���ļ�������
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 11:30
// ����˵��:  @oldName��ʾԭ�ļ���
//            @newName��ʾ�µ��ļ���
// �� �� ֵ:  true��ʾ�����ɹ���false��ʾ����ʧ��
//////////////////////////////////////////////////////////////////////////
bool SDir::rename(SString oldName, SString newName)
{
	if(m_sDirName.length() > 0 && oldName.left(1) != "/" && oldName.left(2).right(1) != ":")
	{
		oldName = m_sDirName+"/"+oldName;
		newName = m_sDirName+"/"+newName;
	}
	return ::rename(oldName.data(),newName.data()) == 0;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ɾ����ǰĿ¼��ָ�����ļ���
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 11:31
// ����˵��:  @name����·�����ļ���
// �� �� ֵ:  trueɾ���ɹ���falseɾ��ʧ��
//////////////////////////////////////////////////////////////////////////
bool SDir::remove(SString name)
{
	return SFile::remove(m_sDirName+"/"+name);
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ǰĿ¼��ָ�����ļ����Ƿ����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 11:31
// ����˵��:  @name����·�����ļ���
// �� �� ֵ:  true�ļ����ڣ�false��
//////////////////////////////////////////////////////////////////////////
bool SDir::exists(SString name)
{
	return SFile::exists(m_sDirName+"/"+name);
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ͳ�Ƶ�ǰĿ¼�µ��ļ�����Ŀ¼����������������Ҫָ������m_sFilter
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 11:32
// ����˵��:  void
// �� �� ֵ:  �ļ�����Ŀ¼������
//////////////////////////////////////////////////////////////////////////
int SDir::count()
{
	m_fileList.clear();
	m_fileAttrList.clear();
	
#ifdef WIN32
	if(m_sFilter == "")
		m_sFilter = "*";
	SString str = m_sDirName+"/"+m_sFilter;
	void *m_pNextInfo = new WIN32_FIND_DATA;
#ifdef UNICODE
	WCHAR *pStr = (WCHAR*)str.data();
#else
	char *pStr = str.data();
#endif

	HANDLE h = ::FindFirstFile(pStr, (WIN32_FIND_DATA*) m_pNextInfo);
	while(h != INVALID_HANDLE_VALUE)
	{
		SString *pStr=new SString((char*)((LPWIN32_FIND_DATA) m_pNextInfo)->cFileName);			
		m_fileList.append(pStr);
		SString *pAttr=new SString();
		DWORD attr = ((LPWIN32_FIND_DATA) m_pNextInfo)->dwFileAttributes;
		if((attr & FILE_ATTRIBUTE_DIRECTORY) != 0)
			*pAttr = "isdir=1;";//��Ŀ¼
		else
			*pAttr = "isdir=0;";//����Ŀ¼
		m_fileAttrList.append(pAttr);
		BOOL bFile = ::FindNextFile(h, (LPWIN32_FIND_DATA) m_pNextInfo);
		if(bFile == FALSE)
			break;
	}
	if(h != INVALID_HANDLE_VALUE)
		FindClose(h);
	delete m_pNextInfo;
#else
	DIR *dir;
	struct dirent *ptr;
	SString sTemp;
	dir=opendir(m_sDirName.data());
	if(dir == NULL)
		return 0;//����Ŀ¼�����ڣ�����readdir
	while((ptr = readdir(dir)) != 0)
	{
		SString str=m_sFilter.toLower();
		str.replace((char*)"*",(char*)"");
		SString *pStr=new SString(ptr->d_name);			
		if(str.length() > 0 && pStr->toLower().find(str) < 0)
		{
			delete pStr;
			continue;
		}

		SString *pAttr=new SString();
		struct   stat   st;
		sTemp.sprintf("%s/%s",m_sDirName.data(),pStr->data());
		stat(sTemp.data(),&st);
		if(S_ISDIR(st.st_mode))
			*pAttr = "isdir=1;";//��Ŀ¼
		else
			*pAttr = "isdir=0;";//����Ŀ¼
		m_fileAttrList.append(pAttr);

		m_fileList.append(pStr);
	}
	closedir(dir);
#endif
	return m_fileList.count();
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡָ���ĵ�index���ļ�/Ŀ¼������
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 11:34
// ����˵��:  @indexΪ��ţ���0��ʼ���
// �� �� ֵ:  �ļ�������Ŀ¼��
//////////////////////////////////////////////////////////////////////////
SString SDir::operator[](int index)
{
	if(index >= (int)m_fileList.count())
		return "";
	return *(m_fileList[index]);
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡָ���ĵ�index���ļ�/Ŀ¼�������ַ���
//                    (isdir=1;��ʾ����Ŀ¼  isdir=0;��ʾ���ļ�)
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 11:36
// ����˵��:  @indexΪ��ţ���0��ʼ���
// �� �� ֵ:  �����ַ���
//////////////////////////////////////////////////////////////////////////
SString SDir::attribute(int index)
{
	if(index >= (int)m_fileAttrList.count())
		return "";
	return *(m_fileAttrList[index]);
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��index���ļ�/Ŀ¼�Ƿ���Ŀ¼
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 11:38
// ����˵��:  @indexΪ��ţ���0��ʼ���
// �� �� ֵ:  true��ʾ��Ŀ¼��false��ʾ����Ŀ¼�����ļ�
//////////////////////////////////////////////////////////////////////////
bool SDir::IsDir(int index)
{
	if(index >= (int)m_fileAttrList.count())
		return false;
	return SString::GetAttributeValueI(*(m_fileAttrList[index]),"isdir")==1;
}

//static member��̬����
////////////////////////////////////////////////////////////////////////
// ��    ��:  ���õ�ǰ·����һ��ȫ�ֱ�����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 11:39
// ����˵��:  @dirΪ�����õ���·��
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SDir::setCurrent(SString dir)
{
	g_currDir = dir;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ��ǰ����ִ�г�������·��������Ŀ¼�ָ�����β
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 11:44
// ����˵��:  void
// �� �� ֵ:  ��ǰ·������·���ָ���
//////////////////////////////////////////////////////////////////////////
SString SDir::currentPath()
{
	SString str;
	char currentName[SKT_PATH_MAX+1];
	memset(currentName,0,SKT_PATH_MAX+1);
#ifdef WIN32
#ifdef UNICODE
	WCHAR *pStr = (WCHAR*)currentName;
#else
	char *pStr = (char*)currentName;
#endif
	GetModuleFileName(NULL,pStr,SKT_PATH_MAX);
	str = currentName;
	int pos=str.findRev("\\");
	if(pos>0)
		str = str.left(pos+1);
#else
	//ssize_t readlink(const char *path, char *buf, size_t bufsiz);
	int len = readlink(SString::toFormat("/proc/%d/exe",getpid()).data(),currentName,SKT_PATH_MAX);
	if(len <= 0)
	{
		if(getcwd(currentName,SKT_PATH_MAX))
			str = currentName;
		else
			str = "";
	}
	else
		str = currentName;
	int pos=str.findRev("/");
	if(pos>0)
		str = str.left(pos+1);
#endif
	return str;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ����һ���µ�Ŀ¼
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 11:40
// ����˵��:  @dirΪ��������Ŀ¼ȫ·��
// �� �� ֵ:  true��ʾ�����ĳɹ���false��ʾʧ��
//////////////////////////////////////////////////////////////////////////
bool SDir::createDir(SString dir)
{
#ifdef WIN32
	return _mkdir(dir.data()) == 0;
#else
	return mkdir(dir.data(),10705) == 0;
#endif
}


////////////////////////////////////////////////////////////////////////
// ��    ��:  ���ָ��Ŀ¼�µ�ȫ�����ݣ�ͬʱ�ݹ����ȫ����Ŀ¼�������ָ����Ŀ¼����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 11:40
// ����˵��:  @dirΪָ����Ŀ¼
// �� �� ֵ:  true����ɹ���false��ʾ���ʧ��
//////////////////////////////////////////////////////////////////////////
bool SDir::clearDir(SString sPath)//���ָ��Ŀ¼�µ�ȫ������
{
	if(sPath.Right(1) != "\\" && sPath.Right(1) != "/")
	{
#ifdef WIN32
		sPath += "\\";
#else
		sPath += "/";
#endif
	}
	SDir dir(sPath,"*");
	bool err = false;
	int i,cnt = dir.count();
	for(i=0;i<cnt;i++)
	{
		SString sFile = dir[i];
		if(sFile == "." || sFile == "..")
			continue;
		SString attr = dir.attribute(i);
		if(SString::GetAttributeValueI(attr,"isdir") == 1)
		{
			if(!clearDir(sPath+sFile))
				err = true;
			if(!SDir::removeDir(sPath+sFile))
				err = true;
		}
		else
		{
			if(!SFile::remove(sPath+sFile))
				err = true;
		}
	}
	if(err)
		return false;
	return true;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ����ָ��Ŀ¼��ָ���������ļ�������䵽���ص��ļ�ȫ·���б�
// ��    ��:  �ۿ���
// ����ʱ��:  2014:10:9 9:04
// ����˵��:  @sDirΪĿ���ļ���
//         :  @sFilterΪͨ�����*��ʾ�����ļ���*.txt��ʾ����TXT�ļ��������ִ�Сд
//         :  @slResultΪ���÷��ص��ļ��б������ļ�ȫ·����
//         :  @iLimitΪ��෵�ص��ļ��б�������0��ʾ����������
// �� �� ֵ:  �����ļ��б����ݣ�<0��ʾʧ�ܣ�0��ʾδ�ҵ��ļ�
//////////////////////////////////////////////////////////////////////////
int SDir::searchDirFiles(SString sDir,SString sFilter,SString sFileFilter,SStringList &slResult,int iLimit/*=0*/)
{
	if(sDir.Right(1) != "\\" && sDir.Right(1) != "/")
	{
#ifdef WIN32
		sDir += "\\";
#else
		sDir += "/";
#endif
	}
	sFileFilter = sFileFilter.replace((char*)"*",(char*)"").toLower();
	SDir dir(sDir,sFilter);
	SString str;
	int ret = 0;
	int i,cnt = dir.count();
	for(i=0;i<cnt;i++)
	{
		SString sFile = dir[i];
		if(sFile == "." || sFile == "..")
			continue;
		SString attr = dir.attribute(i);
		if(SString::GetAttributeValueI(attr,"isdir") == 1)
		{
			if((ret = searchDirFiles(sDir+sFile,sFilter,sFileFilter,slResult,iLimit)) < 0)
				return ret;
			if(iLimit != 0 && (int)slResult.count() >= iLimit)
				return slResult.count();
		}
		else
		{
			if(sFileFilter.length() > 0 && sFile.toLower().find(sFileFilter) < 0)
				continue;
			str = sDir+sFile;
			slResult.append(str);
			if(iLimit != 0 && (int)slResult.count() >= iLimit)
				return slResult.count();
		}
	}
	return slResult.count();
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ɾ��һ��ָ����Ŀ¼��ֻ�е�Ŀ¼Ϊ�ղ���ɾ���ɹ�
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 11:42
// ����˵��:  @dir��ɾ����Ŀ¼
// �� �� ֵ:  trueɾ��Ŀ¼�ɹ���falseʧ��
//////////////////////////////////////////////////////////////////////////
bool SDir::removeDir(SString dir)
{
#ifdef WIN32
	return _rmdir(dir.data()) == 0;
#else
	return rmdir(dir.data()) == 0;
#endif
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  �ж�ָ�����ļ�Ŀ¼�Ƿ����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-18 11:43
// ����˵��:  @sDirΪĿ¼ȫ·��
// �� �� ֵ:  true��ʾĿ¼���ڣ�false��ʾ������
//////////////////////////////////////////////////////////////////////////
bool SDir::dirExists(SString sDir)
{
#ifdef WIN32
	sDir.replace("/","\\");
	while(sDir.right(1) == "\\")
		sDir = sDir.left(sDir.length()-1);
	//void *m_pNextInfo = new WIN32_FIND_DATA;
	WIN32_FIND_DATA data;
#ifdef UNICODE
	WCHAR *pStr = (WCHAR*)sDir.data();
#else
	char *pStr = sDir.data();
#endif
	HANDLE h = ::FindFirstFile(pStr, (WIN32_FIND_DATA*) &data/*m_pNextInfo*/);
	if(h == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	FindClose(h);
#else
	DIR *dir;
	//struct dirent *ptr;
	dir=opendir(sDir.data());
	if(dir == NULL)
		return false;//����Ŀ¼�����ڣ�����readdir
	closedir(dir);
#endif
	return true;
}
