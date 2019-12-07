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
    /*����
    DataByte
        [in]��������ݳ���,���ֽ�Ϊ��λ
    */
    string Encode(const unsigned char* Data,int DataByte);
    /*����
    DataByte
        [in]��������ݳ���,���ֽ�Ϊ��λ
    OutByte
        [out]��������ݳ���,���ֽ�Ϊ��λ,�벻Ҫͨ������ֵ����
        ������ݵĳ���
    */
    string Decode(const char* Data,int DataByte,int& OutByte);
};

#endif // !defined(AFX_BASE64_H__9B671C00_30CA_4CA4_BCA3_C99226911694__INCLUDED_)
