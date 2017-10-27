/**
 *
 * 文 件 名 : SType.h
 * 创建日期 : 2014-7-23 12:50
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : 基本数据类型定义
 * 修改记录 : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2014-7-23	邵凯田　创建文件
 *
 **/
#ifndef __S_TYPE_DEFINE_H__
#define __S_TYPE_DEFINE_H__

//8位整数
#define S_INT8 char
#define S_UINT8 unsigned char

//字符
#define S_CHAR char
#define S_UCHAR unsigned char

//16位整数
#define S_INT16 short
#define S_UINT16 unsigned short


//32位整数
#define S_INT32 int
#define S_UINT32 unsigned int

//64位整数
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