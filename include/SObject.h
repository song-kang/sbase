/**
 *
 * 文 件 名 : SObject.h
 * 创建日期 : 2007-10-26 19:09
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: 2011/08/31 08:43:34 $
 * 当前版本 : $Revision: 1.1 $
 * 功能描述 : 定义SObject基类，所有类均必须从其派生，用于检查对象地址是否4字节对齐
 * 修改记录 : 
 *            $Log: SObject.h,v $
 *            Revision 1.1  2011/08/31 08:43:34  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2011/02/25 03:12:07  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2010/08/09 03:04:43  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2009/09/15 02:14:08  shaokt
 *            通迅记录分析装置项目首次提交
 *
 *            Revision 1.2  2008/09/01 11:48:22  taol
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/01 11:38:28  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/01 08:58:02  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2007/10/30 10:48:38  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2007/10/29 07:30:23  shaokt
 *            添加对象地址是否为4的倍数的检测函数，为检测ARM下由于pack(1)造成的内存问题
 *
 *
 **/

#ifndef __SKT_OBJECT_H__
#define __SKT_OBJECT_H__

#define S_UNUSED(a) (void)a

#ifdef WIN32
  #ifndef _CRT_SECURE_NO_WARNINGS
    #define _CRT_SECURE_NO_WARNINGS
  #endif
  #pragma warning(disable: 4996)//disable CRT SECURE WARNINGS in VC
  #pragma warning(disable: 4251)//warning C4251 template class“AAA”需要有 dll 接口由 class“XX”的客户端使用
#endif

//////////////////////////////////////////////////////////////////////////
#ifdef WIN32
#ifdef SBASE_LIB_STATIC  //静态库，不需要导出、导出
  #define SBASE_EXPORT
#else //动态库，导出动态库中为导出申明，其他为导入申明

  #ifdef SBASE_EXPORT_DLL
    #define SBASE_EXPORT __declspec(dllexport)
  #else//SBASE_EXPORT_DLL
    #define SBASE_EXPORT __declspec(dllimport)
    #ifdef WIN32
      #ifdef _DEBUG
      #pragma comment(lib,"sbased.lib")
      #else
      #pragma comment(lib,"sbase.lib")
    #endif
    #endif//WIN32
  #endif//SBASE_EXPORT_DLL
#endif//SBASE_LIB_STATIC

#ifdef SBASE_DECODE_LIB_STATIC  //静态库，不需要导出、导出
  #define SBASE_DECODE_EXPORT
#else //动态库，导出动态库中为导出申明，其他为导入申明
  #ifdef WIN32
    #ifdef  SBASE_DECODE_EXPORT_DLL
      #define SBASE_DECODE_EXPORT __declspec(dllexport)
    #else //SBASE_DECODE_EXPORT_DLL
      #define SBASE_DECODE_EXPORT __declspec(dllimport)
    #endif//SBASE_DECODE_EXPORT_DLL
  #endif
#endif//SBASE_DECODE_LIB_STATIC

#ifdef SBASE_ESCPRINT_LIB_STATIC  //静态库，不需要导出、导出
#define SBASE_ESCPRINT_EXPORT
#else //动态库，导出动态库中为导出申明，其他为导入申明
#ifdef  SBASE_ESCPRINT_EXPORT_DLL
#define SBASE_ESCPRINT_EXPORT __declspec(dllexport)
#else //SBASE_ESCPRINT_EXPORT_DLL
#define SBASE_ESCPRINT_EXPORT __declspec(dllimport)
#endif//SBASE_ESCPRINT_EXPORT_DLL
#endif//SBASE_ESCPRINT_LIB_STATIC

#ifdef SBASE_MDB_LIB_STATIC  //静态库，不需要导出、导出
#define SBASE_MDB_EXPORT
#else //动态库，导出动态库中为导出申明，其他为导入申明
#ifdef  SBASE_MDB_EXPORT_DLL
#define SBASE_MDB_EXPORT __declspec(dllexport)
#else //SBASE_MDB_EXPORT_DLL
#define SBASE_MDB_EXPORT __declspec(dllimport)
#endif//SBASE_MDB_EXPORT_DLL
#endif//SBASE_MDB_LIB_STATIC

#ifdef SBASE_MYSQL_LIB_STATIC  //静态库，不需要导出、导出
#define SBASE_MYSQL_EXPORT
#else //动态库，导出动态库中为导出申明，其他为导入申明
#ifdef  SBASE_MYSQL_EXPORT_DLL
#define SBASE_MYSQL_EXPORT __declspec(dllexport)
#else //SBASE_MYSQL_EXPORT_DLL
#define SBASE_MYSQL_EXPORT __declspec(dllimport)
#endif//SBASE_MYSQL_EXPORT_DLL
#endif//SBASE_MYSQL_LIB_STATIC

#ifdef SBASE_ODBC_LIB_STATIC  //静态库，不需要导出、导出
#define SBASE_ODBC_EXPORT
#else //动态库，导出动态库中为导出申明，其他为导入申明
#ifdef  SBASE_ODBC_EXPORT_DLL
#define SBASE_ODBC_EXPORT __declspec(dllexport)
#else //SBASE_ODBC_EXPORT_DLL
#define SBASE_ODBC_EXPORT __declspec(dllimport)
#endif//SBASE_ODBC_EXPORT_DLL
#endif//SBASE_ODBC_LIB_STATIC

#ifdef SBASE_ORACLE_LIB_STATIC  //静态库，不需要导出、导出
#define SBASE_ORACLE_EXPORT
#else //动态库，导出动态库中为导出申明，其他为导入申明
#ifdef  SBASE_ORACLE_EXPORT_DLL
#define SBASE_ORACLE_EXPORT __declspec(dllexport)
#else //SBASE_ORACLE_EXPORT_DLL
#define SBASE_ORACLE_EXPORT __declspec(dllimport)
#endif//SBASE_ORACLE_EXPORT_DLL
#endif//SBASE_ORACLE_LIB_STATIC

#ifdef SBASE_PGSQL_LIB_STATIC  //静态库，不需要导出、导出
#define SBASE_PGSQL_EXPORT
#else //动态库，导出动态库中为导出申明，其他为导入申明
#ifdef  SBASE_PGSQL_EXPORT_DLL
#define SBASE_PGSQL_EXPORT __declspec(dllexport)
#else //SBASE_PGSQL_EXPORT_DLL
#define SBASE_PGSQL_EXPORT __declspec(dllimport)
#endif//SBASE_PGSQL_EXPORT_DLL
#endif//SBASE_PGSQL_LIB_STATIC

#ifdef SBASE_QTGUI_LIB_STATIC  //静态库，不需要导出、导出
#define SBASE_QTGUI_EXPORT
#else //动态库，导出动态库中为导出申明，其他为导入申明
#ifdef  SBASE_QTGUI_EXPORT_DLL
#define SBASE_QTGUI_EXPORT __declspec(dllexport)
#else //SBASE_QTGUI_EXPORT_DLL
#define SBASE_QTGUI_EXPORT __declspec(dllimport)
#endif//SBASE_QTGUI_EXPORT_DLL
#endif//SBASE_QTGUI_LIB_STATIC

#ifdef SBASE_RAWSOCK_LIB_STATIC  //静态库，不需要导出、导出
#define SBASE_RAWSOCK_EXPORT
#else //动态库，导出动态库中为导出申明，其他为导入申明
#ifdef  SBASE_RAWSOCK_EXPORT_DLL
#define SBASE_RAWSOCK_EXPORT __declspec(dllexport)
#else //SBASE_RAWSOCK_EXPORT_DLL
#define SBASE_RAWSOCK_EXPORT __declspec(dllimport)
#endif//SBASE_RAWSOCK_EXPORT_DLL
#endif//SBASE_RAWSOCK_LIB_STATIC

#ifdef SBASE_ZIP_LIB_STATIC  //静态库，不需要导出、导出
#define SBASE_ZIP_EXPORT
#else //动态库，导出动态库中为导出申明，其他为导入申明
#ifdef  SBASE_ZIP_EXPORT_DLL
#define SBASE_ZIP_EXPORT __declspec(dllexport)
#else //SBASE_ZIP_EXPORT_DLL
#define SBASE_ZIP_EXPORT __declspec(dllimport)
#endif//SBASE_ZIP_EXPORT_DLL
#endif//SBASE_ZIP_LIB_STATIC

#else//WIN32
#define SBASE_EXPORT
#define SBASE_DECODE_EXPORT
#define SBASE_ESCPRINT_EXPORT
#define SBASE_MDB_EXPORT
#define SBASE_MYSQL_EXPORT
#define SBASE_ODBC_EXPORT
#define SBASE_ORACLE_EXPORT
#define SBASE_PGSQL_EXPORT
#define SBASE_QTGUI_EXPORT
#define SBASE_RAWSOCK_EXPORT
#define SBASE_ZIP_EXPORT
//////////////////////////////////////////////////////////////////////////

#endif//WIN32
//////////////////////////////////////////////////////////////////////////

//#include "SLog.h"
#include <string.h>

#ifndef WIN32

#ifndef WORD
#define WORD unsigned short
#endif

#ifndef DWORD
#define DWORD unsigned int
#endif

#ifndef BYTE
#define BYTE unsigned char
#endif

#ifndef INT64
#ifdef WIN32
#define INT64 __int64
#define UINT64 unsigned __int64
#else
#define INT64 long long
#define UINT64 unsigned long long
#endif
#endif

#ifndef U_LONG
//无符号长整型，32位程序中为4字节，64位程序中为8字节
#define U_LONG unsigned long
#endif

#ifndef NULL
#define NULL 0
#endif

#endif

void SObjectWarn();

#ifndef _CHECK_OBJECT_
#define _SKTOBJECT_
#define SKTOBJECT(cls) 
#else
#define _SKTOBJECT_ \
					if(((int)this) % 4 != 0) \
					{ \
						LOGWARN("对象地址[%p]未按4字节对齐!这将造成ARM平台上的运行不稳定!!!",this); \
						SObjectWarn(); \
					} \
						
#define SKTOBJECT(cls) \
					if(((int)this) % 4 != 0) \
					{ \
						LOGWARN("%s对象地址[%p]未按4字节对齐!这将造成ARM平台上的运行不稳定!!!",cls,this); \
						SObjectWarn(); \
					} \

#endif


class SBASE_EXPORT SConstBuffer
{
public:
	struct SBASE_EXPORT stuConstBuffer
	{
		stuConstBuffer(int size=102400)
		{
			pNext = NULL;
			iBufferSize = size;
			pBuffer = pNextBuffer = new unsigned char[size];
			memset(pBuffer,0,size);
		}
		~stuConstBuffer()
		{
			delete[] pBuffer;
		}
		unsigned char *pBuffer;
		int iBufferSize;
		unsigned char *pNextBuffer;

		stuConstBuffer *pNext;
	};
	SConstBuffer();
	~SConstBuffer();
	void Clear();

	unsigned char* GetConstBuffer(int iLen);
	unsigned char* CopyToConstBuffer(unsigned char *pSrcBuf,int iLen);
	const char* CopyToConstString(char *pStr,int iLen=0);
	void SetBufferSize(int size){m_iBufferSize = size;};

	stuConstBuffer *m_pFirstBuffer;
	stuConstBuffer *m_pBuffer;
	int m_iBufferSize;
};

#endif//__SKT_OBJECT_H__


