/**
 *
 * 文 件 名 : SFile.h
 * 创建日期 : 2007-01-01
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: 2011/09/28 07:12:55 $
 * 当前版本 : $Revision: 1.2 $
 * 功能描述 : 类QT的文件类
 * 修改记录 : 
 *            $Log: SFile.h,v $
 *            Revision 1.2  2011/09/28 07:12:55  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2011/08/31 08:43:33  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.4  2011/04/11 12:08:36  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2011/03/23 01:09:29  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2011/03/23 01:07:49  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2011/02/25 03:12:07  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2010/10/29 08:48:41  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2010/08/09 03:04:43  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2010/01/22 01:24:25  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2009/09/15 02:14:07  shaokt
 *            通迅记录分析装置项目首次提交
 *
 *            Revision 1.9  2009/02/21 15:55:39  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.8  2008/12/22 08:54:13  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.7  2008/11/03 05:54:49  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.6  2008/09/16 05:32:17  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.5  2008/09/12 05:40:25  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.4  2008/09/12 05:18:45  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2008/09/11 09:47:28  shaokt
 *            *** empty log message ***
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
 *            Revision 1.6  2007/10/29 07:30:23  shaokt
 *            添加对象地址是否为4的倍数的检测函数，为检测ARM下由于pack(1)造成的内存问题
 *
 *            Revision 1.5  2007/10/19 11:57:26  shaokt
 *            size函数增加可选的文件名参数
 *
 *            Revision 1.4  2007/10/19 06:17:40  shaokt
 *            添加文件时间
 *
 *            Revision 1.3  2007/09/11 08:17:52  shaokt
 *            基类更新
 *
 *            Revision 1.2  2007/09/04 01:43:38  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2007/08/15 03:09:22  shaokt
 *            *** empty log message ***
 *
 **/

#ifndef __SKTFILE_H_
#define __SKTFILE_H_

#ifndef WIN32
#include <dirent.h>
#include <unistd.h>
#else
#include <direct.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include "SObject.h"

#ifdef WIN32
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#ifndef _POSIX_
#define _POSIX_
#endif
#endif

#define SKT_PATH_MAX 1024

#include <stdio.h>
#include <limits.h>
#include "SString.h"
#include "SList.h"
#include "SDateTime.h"

class SFile;
//typedef SFile QFile;
#ifndef WIN32
  #ifndef  BYTE
    #define BYTE unsigned char
  #endif
  #define SDIR_SEPARATOR "/"
#else
  #define SDIR_SEPARATOR "\\"
#endif

typedef enum  _OPENMODE
{
	IO_Append,
	IO_ReadOnly,
	IO_ReadWrite,
	IO_Translate,
	IO_Truncate,
	IO_WriteOnly
} OpenMode;

//////////////////////////////////////////////////////////////////////////
// 名    称:  SFile
// 作    者:  邵凯田
// 创建时间:  2011-11-18 10:08
// 描    述:  文件类
//////////////////////////////////////////////////////////////////////////
class SBASE_EXPORT SFile
{
public:
	SFile();
	SFile(SString fileName);
	virtual ~SFile();
	
	////////////////////////////////////////////////////////////////////////
	// 描    述:  取文件名
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 10:11
	// 参数说明:  void
	// 返 回 值:  文件名
	//////////////////////////////////////////////////////////////////////////
	SString filename() {return m_fileName;};

	////////////////////////////////////////////////////////////////////////
	// 描    述:  设置文件名
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 10:11
	// 参数说明:  @name为文件名
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void setFileName(SString name) {m_fileName=name;};

	////////////////////////////////////////////////////////////////////////
	// 描    述:  设置文件名称，如果没有带绝对路径，则加上绝对路径
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 10:12
	// 参数说明:  @name文件名
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void setName(SString name);
	
	////////////////////////////////////////////////////////////////////////
	// 描    述:  关闭文件
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 10:19
	// 参数说明:  void
	// 返 回 值:  true表示关闭成功,flase表示关闭失败
	//////////////////////////////////////////////////////////////////////////
	bool close();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  打开文件
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 10:20
	// 参数说明:  @mode表示打开方式
	// 返 回 值:  true表示打开成功，false表示打开失败
	//////////////////////////////////////////////////////////////////////////
	bool open(OpenMode mode);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  以指定的模式字符串打开文件
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 10:20
	// 参数说明:  @mode为模式，如"wb"
	// 返 回 值:  true表示打开成功，false表示打开失败
	//////////////////////////////////////////////////////////////////////////
	bool openByMode(const char* mode);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  删除当前文件
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 10:21
	// 参数说明:  void
	// 返 回 值:  true删除成功，false删除失败
	//////////////////////////////////////////////////////////////////////////
	bool remove();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  重命名当前的文件
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 10:21
	// 参数说明:  @newname表示新的文件名
	// 返 回 值:  true表示命名成功，false表示命名失败
	//////////////////////////////////////////////////////////////////////////
	bool rename(SString newName);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  当前文件是否存在
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 10:22
	// 参数说明:  void
	// 返 回 值:  true表示文件存在,false表示文件不存在
	//////////////////////////////////////////////////////////////////////////
	bool exists();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  复制当前文件到另外一个文件
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 10:22
	// 参数说明:  @newFile新文件的文件名
	// 返 回 值:  true表示复制成功，false表示复制失败
	//////////////////////////////////////////////////////////////////////////
	bool copy(SString newFile);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  读取一个数据块
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 10:24
	// 参数说明:  @pBuffer为待写入的缓冲区
	//            @maxSize为缓冲区的总大小,也是当前期望读取的最大字节数
	// 返 回 值:  实际读取到的字节数量,<0表示失败
	//////////////////////////////////////////////////////////////////////////
	long readBlock(BYTE *pBuff, int maxSize);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  写入指定数量的0字节
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 10:27
	// 参数说明:  @bytes表示要定稿的字节数量
	// 返 回 值:  返回1表示成功，<0表示异常
	//////////////////////////////////////////////////////////////////////////
	long writeZero(int bytes);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  写入数据块
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 10:27
	// 参数说明:  @pBuff表示缓冲区地址
	//            @size表示待写入长度
	// 返 回 值:  返回实际写入长度，<0表示失败
	//////////////////////////////////////////////////////////////////////////
	long writeBlock(BYTE *pBuff, int size);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  写入数据块
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 10:27
	// 参数说明:  @pBuff表示缓冲区地址
	//            @size表示待写入长度
	// 返 回 值:  返回实际写入长度，<0表示失败
	//////////////////////////////////////////////////////////////////////////
	inline long writeBlock(BYTE *pBuff, unsigned char size)
	{
		return writeBlock(pBuff,(int)size);
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  写入数据块
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 10:27
	// 参数说明:  @pBuff表示缓冲区地址
	//            @size表示待写入长度
	// 返 回 值:  返回实际写入长度，<0表示失败
	//////////////////////////////////////////////////////////////////////////
	inline long writeBlock(BYTE *pBuff, unsigned int size)
	{
		return writeBlock(pBuff,(int)size);
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  写入数据块
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 10:27
	// 参数说明:  @pBuff表示缓冲区地址
	//            @size表示待写入长度
	// 返 回 值:  返回实际写入长度，<0表示失败
	//////////////////////////////////////////////////////////////////////////
	inline long writeBlock(BYTE *pBuff, char size)
	{
		return writeBlock(pBuff,(int)size);
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  读取一行到指定的缓冲区，碰到\n、文件结束或缓冲区满时退出
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 10:30
	// 参数说明:  @pBuff表示缓冲区地址
	//            @maxSize表示缓冲区大小（字节数）
	// 返 回 值:  返回实际读取的字节数
	//////////////////////////////////////////////////////////////////////////
	long readLine(char *pBuff, int maxSize);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  读取一行到返回字符串中
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 10:32
	// 参数说明:  @maxSize表示字符串的最大长度
	// 返 回 值:  返回字符串
	//////////////////////////////////////////////////////////////////////////
	SString readLine(int maxSize=10240);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取指定文件大小
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 10:41
	// 参数说明:  @sFileName为指定的文件名，未指定时为当前文件
	// 返 回 值:  返回文件大小字节数
	//////////////////////////////////////////////////////////////////////////
	long size(SString sFileName="");

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取指定文件大小的静态函数  
	// 作    者:  邵凯田
	// 创建时间:  2013:9:23 12:44
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	static long fileSize(SString sFileName)
	{
		SFile f;
		return f.size(sFileName);
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  文件写入函数
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 10:42
	// 参数说明:  @pBuff表示待写入的缓冲区
	//            @size表示待写入长度
	// 返 回 值:  返回实际写入长度，<0表示失败
	//////////////////////////////////////////////////////////////////////////
	long write(BYTE *pBuff, int size)	{return writeBlock(pBuff,size);};

	////////////////////////////////////////////////////////////////////////
	// 描    述:  文件写入函数
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 10:42
	// 参数说明:  @pBuff表示待写入的缓冲区
	//            @size表示待写入长度
	// 返 回 值:  返回实际写入长度，<0表示失败
	//////////////////////////////////////////////////////////////////////////
	long write(char* pBuff, int size) {return writeBlock((BYTE*)pBuff, size);};

	////////////////////////////////////////////////////////////////////////
	// 描    述:  写入一个字符串到文件中
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 10:44
	// 参数说明:  @str为待写入的字符串内容
	// 返 回 值:  返回实际写入长度，<0表示失败
	//////////////////////////////////////////////////////////////////////////
	long writeString(SString str) {return write(str.data(),str.length());};

	////////////////////////////////////////////////////////////////////////
	// 描    述:  向文件中打印格式化字符串,必须确保格式化后的字符串长度小于10240字节，否则将造成异常
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 10:46
	// 参数说明:  同printf
	// 返 回 值:  返回实际写入长度，<0表示失败
	//////////////////////////////////////////////////////////////////////////
	long fprintf(const char * cformat, ...);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  判断文件句柄是否已经打开
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 10:47
	// 参数说明:  void
	// 返 回 值:  true表示文件已经打开，false表示文件尚未打开
	//////////////////////////////////////////////////////////////////////////
	bool isOpen(){return m_fp!=NULL;};

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取文件句柄
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 10:47
	// 参数说明:  void
	// 返 回 值:  FILE*为文件句柄
	//////////////////////////////////////////////////////////////////////////
	FILE * getHandle()	{return m_fp;};

	////////////////////////////////////////////////////////////////////////
	// 描    述:  定位文件指针到指定的绝对位置
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 10:48
	// 参数说明:  @pos为文件偏移的绝对位置
	// 返 回 值:  0表示成功，其它均为失败
	//////////////////////////////////////////////////////////////////////////
	int at(int pos);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  定位文件指针到指定的绝对位置
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 10:48
	// 参数说明:  @pos为文件偏移的绝对位置
	// 返 回 值:  0表示成功，其它均为失败
	//////////////////////////////////////////////////////////////////////////
	int seekTo(int pos);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  定位文件指针到指定的相对位置
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 10:48
	// 参数说明:  @pos为文件偏移的相对位置
	// 返 回 值:  0表示成功，其它均为失败
	//////////////////////////////////////////////////////////////////////////
	int seekCur(int pos);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  定位文件指针到文件开始位置
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 10:48
	// 参数说明:  void
	// 返 回 值:  0表示成功，其它均为失败
	//////////////////////////////////////////////////////////////////////////
	int seekBegin();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  定位文件指针到文件结束位置
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 10:48
	// 参数说明:  void
	// 返 回 值:  0表示成功，其它均为失败
	//////////////////////////////////////////////////////////////////////////
	int seekEnd();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  清除文件缓冲区，一般用于将写入文件立即生效
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 11:00
	// 参数说明:  void
	// 返 回 值:  0表示成功，其它均为失败
	//////////////////////////////////////////////////////////////////////////
	int flush();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取当前文件指针的位置
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 11:01
	// 参数说明:  void
	// 返 回 值:  返回文件指针的当前位置
	//////////////////////////////////////////////////////////////////////////
	int position();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  判断文件指针是否在文件末尾
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 11:05
	// 参数说明:  void
	// 返 回 值:  true表示在文件末尾，false表示不在末尾
	//////////////////////////////////////////////////////////////////////////
	bool atEnd();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取当前文件名全路径中的文件名部分
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 11:05
	// 参数说明:  void
	// 返 回 值:  文件名
	//////////////////////////////////////////////////////////////////////////
	SString name();
	
	//static member静态函数
	////////////////////////////////////////////////////////////////////////
	// 描    述:  创建一个空的新文件，并允许指定文件大小
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 10:25
	// 参数说明:  @sPathName表示文件全路径名
	//            @iFileSize表示文件大小
	// 返 回 值:  true表示成功，false表示失败
	//////////////////////////////////////////////////////////////////////////
	static bool touchFile(SString sPathName,int iFileSize=0);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  复制文件
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 11:09
	// 参数说明:  @fileName表示源文件名
	//            @newFile表示目标文件
	// 返 回 值:  true表示复制成功，false表示复制失败
	//////////////////////////////////////////////////////////////////////////
	static bool copy(SString fileName, SString newFile);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  合并文件,将源文件的内容复制到目标文件的后面
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 11:11
	// 参数说明:  @fileFrom表示源文件名
	//            @fileTo表示目标文件名
	// 返 回 值:  true表示合并成功，false表示合并失败
	//////////////////////////////////////////////////////////////////////////
	static bool merge(SString fileFrom,SString fileTo);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  指定文件是否存在
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 11:12
	// 参数说明:  @fileName表示文件名
	// 返 回 值:  true表示文件存在，false表示文件不存在
	//////////////////////////////////////////////////////////////////////////
	static bool exists(SString fileName);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  删除指定的文件
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 11:15
	// 参数说明:  @fileName待删除的文件
	// 返 回 值:  true表示删除成功，false表示删除失败
	//////////////////////////////////////////////////////////////////////////
	static bool remove(SString fileName);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  重命名指定的文件
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 11:15
	// 参数说明:  @fileName为原文件名
	//            @newFile为新文件
	// 返 回 值:  true表示重命名成功，false表示失败
	//////////////////////////////////////////////////////////////////////////
	static bool rename(SString fileName,SString newFile);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取指定文件名的文件修改时间
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 11:16
	// 参数说明:  @fileName为文件名
	// 返 回 值:  SDateTime修改时间
	//////////////////////////////////////////////////////////////////////////
	static SDateTime filetime(SString fileName);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  表示此文件是否可执行
	// 作    者:  邵凯田
	// 创建时间:  2009-2-11 17:12
	// 参数说明:  fileName表示文件名
	// 返 回 值:  true:表示有运行权限,false:表示没有运行权限
	//////////////////////////////////////////////////////////////////////////
	static bool is_exec_able(SString fileName);

private:
	SString m_fileName;
	FILE *m_fp;	
};


class SBASE_EXPORT STextStream
{
public:
	STextStream(SFile *pFile);
	~STextStream();
	
	//operator
	STextStream& operator<<(const SString &str);
	STextStream& operator<<(const char* pStr);
	
	SString readLine();
	bool atEnd();
private:
	SFile *m_pFile;
};

class SDir;
//typedef SDir QDir;

//////////////////////////////////////////////////////////////////////////
// 名    称:  SDir
// 作    者:  邵凯田
// 创建时间:  2011-11-18 11:20
// 描    述:  目录类
//////////////////////////////////////////////////////////////////////////
class SBASE_EXPORT SDir
{
public:
	SDir();
	SDir(SString dirName, SString nameFilter="*");
	~SDir();
	
	////////////////////////////////////////////////////////////////////////
	// 描    述:  将当前目录下的一个文件重命名
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 11:30
	// 参数说明:  @oldName表示原文件名
	//            @newName表示新的文件名
	// 返 回 值:  true表示改名成功，false表示改名失败
	//////////////////////////////////////////////////////////////////////////
	bool rename(SString oldName, SString newName);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  删除当前目录下指定的文件名
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 11:31
	// 参数说明:  @name不含路径的文件名
	// 返 回 值:  true删除成功，false删除失败
	//////////////////////////////////////////////////////////////////////////
	bool remove(SString name);
	
	////////////////////////////////////////////////////////////////////////
	// 描    述:  当前目录下指定的文件名是否存在
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 11:31
	// 参数说明:  @name不含路径的文件名
	// 返 回 值:  true文件存在，false不存在
	//////////////////////////////////////////////////////////////////////////
	bool exists(SString name);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  统计当前目录下的文件和子目录，并返回数量，需要指定条件m_sFilter
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 11:32
	// 参数说明:  void
	// 返 回 值:  文件和子目录的数量
	//////////////////////////////////////////////////////////////////////////
	int count();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取指定的第index个文件/目录的名称
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 11:34
	// 参数说明:  @index为序号，从0开始编号
	// 返 回 值:  文件名或子目录名
	//////////////////////////////////////////////////////////////////////////
	SString operator[](int index);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取指定的第index个文件/目录的属性字符串
	//                    (isdir=1;表示是子目录  isdir=0;表示是文件)
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 11:36
	// 参数说明:  @index为序号，从0开始编号
	// 返 回 值:  属性字符串
	//////////////////////////////////////////////////////////////////////////
	SString attribute(int index);
	
	////////////////////////////////////////////////////////////////////////
	// 描    述:  第index个文件/目录是否是目录
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 11:38
	// 参数说明:  @index为序号，从0开始编号
	// 返 回 值:  true表示是目录，false表示不是目录而是文件
	//////////////////////////////////////////////////////////////////////////
	bool IsDir(int index);

	//static member静态函数
	////////////////////////////////////////////////////////////////////////
	// 描    述:  设置当前路径到一个全局变量中
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 11:39
	// 参数说明:  @dir为待设置的新路径
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	static void setCurrent(SString dir);
	
	////////////////////////////////////////////////////////////////////////
	// 描    述:  取当前进程执行程序所在路径，并带目录分隔符结尾
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 11:44
	// 参数说明:  void
	// 返 回 值:  当前路径，含路径分隔符
	//////////////////////////////////////////////////////////////////////////
	static SString currentPath();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取当前进程执行程序所在路径，并带目录分隔符结尾
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 11:44
	// 参数说明:  void
	// 返 回 值:  当前路径，含路径分隔符
	//////////////////////////////////////////////////////////////////////////
	static SString currentDirPath() {return currentPath();};

	////////////////////////////////////////////////////////////////////////
	// 描    述:  创建一个新的目录
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 11:40
	// 参数说明:  @dir为待创建的目录全路径
	// 返 回 值:  true表示创建的成功，false表示失败
	//////////////////////////////////////////////////////////////////////////
	static bool createDir(SString dir);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  清除指定目录下的全部内容，同时递归清除全部子目录，不清除指定的目录自身
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 11:40
	// 参数说明:  @dir为指定的目录
	// 返 回 值:  true清除成功，false表示清除失败
	//////////////////////////////////////////////////////////////////////////
	static bool clearDir(SString dir);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  检索指定目录的指针条件的文件，并填充到返回的文件全路径列表
	// 作    者:  邵凯田
	// 创建时间:  2014:10:9 9:04
	// 参数说明:  @sDir为目标文件夹
	//         :  @sFilter为通配符，*表示所有文件，*.txt表示所有TXT文件，不区分大小写
	//         :  @slResult为引用返回的文件列表（采用文件全路径）
	//         :  @iLimit为最多返回的文件列表数量，0表示不限制数量
	// 返 回 值:  返回文件列表数据，<0表示失败，0表示未找到文件
	//////////////////////////////////////////////////////////////////////////
	static int searchDirFiles(SString sDir,SString sFilter,SString sFileFilter,SStringList &slResult,int iLimit=0);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  删除一个指定的目录，只有当目录为空才能删除成功
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 11:42
	// 参数说明:  @dir待删除的目录
	// 返 回 值:  true删除目录成功，false失败
	//////////////////////////////////////////////////////////////////////////
	static bool removeDir(SString dir);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  判断指定的目录是否存在
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 11:43
	// 参数说明:  @sDir为目录全路径
	// 返 回 值:  true表示目录存在，false表示不存在
	//////////////////////////////////////////////////////////////////////////
	static bool dirExists(SString sDir);

private:
	SString m_sDirName;
	SString m_sFilter;
	SPtrList<SString> m_fileList;
	SPtrList<SString> m_fileAttrList;
};

#endif /*__SKTFILE_H_*/
