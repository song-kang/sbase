// Base64.h: interface for the Base64 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASE64_H__9B671C00_30CA_4CA4_BCA3_C99226911694__INCLUDED_)
#define AFX_BASE64_H__9B671C00_30CA_4CA4_BCA3_C99226911694__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
using namespace std;

class ZBase64
{
public:
    /*编码
    DataByte
        [in]输入的数据长度,以字节为单位
    */
    string Encode(const unsigned char* Data,int DataByte);
    /*解码
    DataByte
        [in]输入的数据长度,以字节为单位
    OutByte
        [out]输出的数据长度,以字节为单位,请不要通过返回值计算
        输出数据的长度
    */
    string Decode(const char* Data,int DataByte,int& OutByte);
};

#endif // !defined(AFX_BASE64_H__9B671C00_30CA_4CA4_BCA3_C99226911694__INCLUDED_)
