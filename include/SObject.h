/**
 *
 * �� �� �� : SObject.h
 * �������� : 2007-10-26 19:09
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: 2011/08/31 08:43:34 $
 * ��ǰ�汾 : $Revision: 1.1 $
 * �������� : ����SObject���࣬�����������������������ڼ������ַ�Ƿ�4�ֽڶ���
 * �޸ļ�¼ : 
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
 *            ͨѸ��¼����װ����Ŀ�״��ύ
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
 *            ��Ӷ����ַ�Ƿ�Ϊ4�ı����ļ�⺯����Ϊ���ARM������pack(1)��ɵ��ڴ�����
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
  #pragma warning(disable: 4251)//warning C4251 template class��AAA����Ҫ�� dll �ӿ��� class��XX���Ŀͻ���ʹ��
#endif

//////////////////////////////////////////////////////////////////////////
#ifdef WIN32
#ifdef SBASE_LIB_STATIC  //��̬�⣬����Ҫ����������
  #define SBASE_EXPORT
#else //��̬�⣬������̬����Ϊ��������������Ϊ��������

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

#ifdef SBASE_DECODE_LIB_STATIC  //��̬�⣬����Ҫ����������
  #define SBASE_DECODE_EXPORT
#else //��̬�⣬������̬����Ϊ��������������Ϊ��������
  #ifdef WIN32
    #ifdef  SBASE_DECODE_EXPORT_DLL
      #define SBASE_DECODE_EXPORT __declspec(dllexport)
    #else //SBASE_DECODE_EXPORT_DLL
      #define SBASE_DECODE_EXPORT __declspec(dllimport)
    #endif//SBASE_DECODE_EXPORT_DLL
  #endif
#endif//SBASE_DECODE_LIB_STATIC

#ifdef SBASE_ESCPRINT_LIB_STATIC  //��̬�⣬����Ҫ����������
#define SBASE_ESCPRINT_EXPORT
#else //��̬�⣬������̬����Ϊ��������������Ϊ��������
#ifdef  SBASE_ESCPRINT_EXPORT_DLL
#define SBASE_ESCPRINT_EXPORT __declspec(dllexport)
#else //SBASE_ESCPRINT_EXPORT_DLL
#define SBASE_ESCPRINT_EXPORT __declspec(dllimport)
#endif//SBASE_ESCPRINT_EXPORT_DLL
#endif//SBASE_ESCPRINT_LIB_STATIC

#ifdef SBASE_MDB_LIB_STATIC  //��̬�⣬����Ҫ����������
#define SBASE_MDB_EXPORT
#else //��̬�⣬������̬����Ϊ��������������Ϊ��������
#ifdef  SBASE_MDB_EXPORT_DLL
#define SBASE_MDB_EXPORT __declspec(dllexport)
#else //SBASE_MDB_EXPORT_DLL
#define SBASE_MDB_EXPORT __declspec(dllimport)
#endif//SBASE_MDB_EXPORT_DLL
#endif//SBASE_MDB_LIB_STATIC

#ifdef SBASE_MYSQL_LIB_STATIC  //��̬�⣬����Ҫ����������
#define SBASE_MYSQL_EXPORT
#else //��̬�⣬������̬����Ϊ��������������Ϊ��������
#ifdef  SBASE_MYSQL_EXPORT_DLL
#define SBASE_MYSQL_EXPORT __declspec(dllexport)
#else //SBASE_MYSQL_EXPORT_DLL
#define SBASE_MYSQL_EXPORT __declspec(dllimport)
#endif//SBASE_MYSQL_EXPORT_DLL
#endif//SBASE_MYSQL_LIB_STATIC

#ifdef SBASE_ODBC_LIB_STATIC  //��̬�⣬����Ҫ����������
#define SBASE_ODBC_EXPORT
#else //��̬�⣬������̬����Ϊ��������������Ϊ��������
#ifdef  SBASE_ODBC_EXPORT_DLL
#define SBASE_ODBC_EXPORT __declspec(dllexport)
#else //SBASE_ODBC_EXPORT_DLL
#define SBASE_ODBC_EXPORT __declspec(dllimport)
#endif//SBASE_ODBC_EXPORT_DLL
#endif//SBASE_ODBC_LIB_STATIC

#ifdef SBASE_ORACLE_LIB_STATIC  //��̬�⣬����Ҫ����������
#define SBASE_ORACLE_EXPORT
#else //��̬�⣬������̬����Ϊ��������������Ϊ��������
#ifdef  SBASE_ORACLE_EXPORT_DLL
#define SBASE_ORACLE_EXPORT __declspec(dllexport)
#else //SBASE_ORACLE_EXPORT_DLL
#define SBASE_ORACLE_EXPORT __declspec(dllimport)
#endif//SBASE_ORACLE_EXPORT_DLL
#endif//SBASE_ORACLE_LIB_STATIC

#ifdef SBASE_PGSQL_LIB_STATIC  //��̬�⣬����Ҫ����������
#define SBASE_PGSQL_EXPORT
#else //��̬�⣬������̬����Ϊ��������������Ϊ��������
#ifdef  SBASE_PGSQL_EXPORT_DLL
#define SBASE_PGSQL_EXPORT __declspec(dllexport)
#else //SBASE_PGSQL_EXPORT_DLL
#define SBASE_PGSQL_EXPORT __declspec(dllimport)
#endif//SBASE_PGSQL_EXPORT_DLL
#endif//SBASE_PGSQL_LIB_STATIC

#ifdef SBASE_QTGUI_LIB_STATIC  //��̬�⣬����Ҫ����������
#define SBASE_QTGUI_EXPORT
#else //��̬�⣬������̬����Ϊ��������������Ϊ��������
#ifdef  SBASE_QTGUI_EXPORT_DLL
#define SBASE_QTGUI_EXPORT __declspec(dllexport)
#else //SBASE_QTGUI_EXPORT_DLL
#define SBASE_QTGUI_EXPORT __declspec(dllimport)
#endif//SBASE_QTGUI_EXPORT_DLL
#endif//SBASE_QTGUI_LIB_STATIC

#ifdef SBASE_RAWSOCK_LIB_STATIC  //��̬�⣬����Ҫ����������
#define SBASE_RAWSOCK_EXPORT
#else //��̬�⣬������̬����Ϊ��������������Ϊ��������
#ifdef  SBASE_RAWSOCK_EXPORT_DLL
#define SBASE_RAWSOCK_EXPORT __declspec(dllexport)
#else //SBASE_RAWSOCK_EXPORT_DLL
#define SBASE_RAWSOCK_EXPORT __declspec(dllimport)
#endif//SBASE_RAWSOCK_EXPORT_DLL
#endif//SBASE_RAWSOCK_LIB_STATIC

#ifdef SBASE_ZIP_LIB_STATIC  //��̬�⣬����Ҫ����������
#define SBASE_ZIP_EXPORT
#else //��̬�⣬������̬����Ϊ��������������Ϊ��������
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
//�޷��ų����ͣ�32λ������Ϊ4�ֽڣ�64λ������Ϊ8�ֽ�
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
						LOGWARN("�����ַ[%p]δ��4�ֽڶ���!�⽫���ARMƽ̨�ϵ����в��ȶ�!!!",this); \
						SObjectWarn(); \
					} \
						
#define SKTOBJECT(cls) \
					if(((int)this) % 4 != 0) \
					{ \
						LOGWARN("%s�����ַ[%p]δ��4�ֽڶ���!�⽫���ARMƽ̨�ϵ����в��ȶ�!!!",cls,this); \
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


