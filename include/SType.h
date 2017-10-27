/**
 *
 * �� �� �� : SType.h
 * �������� : 2014-7-23 12:50
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : �����������Ͷ���
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2014-7-23	�ۿ�������ļ�
 *
 **/
#ifndef __S_TYPE_DEFINE_H__
#define __S_TYPE_DEFINE_H__

//8λ����
#define S_INT8 char
#define S_UINT8 unsigned char

//�ַ�
#define S_CHAR char
#define S_UCHAR unsigned char

//16λ����
#define S_INT16 short
#define S_UINT16 unsigned short


//32λ����
#define S_INT32 int
#define S_UINT32 unsigned int

//64λ����
#ifdef WIN32
#define S_INT64 __int64
#define S_UINT64 unsigned __int64
#else
#define S_INT64 long long
#define S_UINT64 unsigned long long
#endif

#define S_PTR unsigned long

#define S_FLOAT float
#define S_DOUBLE double

#define S_BOOL bool

#define S_VOID void

#endif//__S_TYPE_DEFINE_H__