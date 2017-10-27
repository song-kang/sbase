/**
 *
 * 文 件 名 : Asn1Ber.h
 * 创建日期 : 2015-5-20 18:42
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : ASN.1的BER解码类封装
 * 修改记录 : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-5-20	邵凯田　创建文件
 *
 **/

#ifndef __SDECODE_ASN1BER_H__
#define __SDECODE_ASN1BER_H__

#include "SInclude.h"

struct stuAsn1Tag
{
	char cls;//cls定义 – 标签类编码
			//标签类 	位8 位7
			//通用 		0 	0
			//应用 		0 	1
			//上下文特定 1 	0
			//专用		1	1
	bool fmt;//0：基本数据类型  1：构造数据类型
	int id;
//	bool ok;
};

// #ifndef _SKT_NUMBER_SWAP
// struct stuAsn1Tag
// {
// 	BYTE cls:2;
// 	BYTE fmt:1;//0：基本数据类型  1：构造数据类型
// 	BYTE id:5;	
// 	BYTE cls;
// 	BYTE fmt;//0：基本数据类型  1：构造数据类型
// 	DWORD id;
// 	//cls定义 – 标签类编码
// 	//标签类 	位8 位7
// 	//通用 		0 	0
// 	//应用 		0 	1
// 	//上下文特定 1 	0
// 	//专用		1	1
// 
// };
// #else
// struct stuAsn1Tag
// {
// 	BYTE id:5;
// 	BYTE fmt:1;
// 	BYTE cls:2;
// };
// #endif

struct stuAsn1Pdu
{
	stuAsn1Pdu()
	{
		memset(this,0,sizeof(stuAsn1Pdu));
	}

	stuAsn1Tag tag;
	unsigned int len;
	unsigned char *val;
	bool implicit_tag;

	//输出内容的开始位置和长度
	unsigned char *out_val;
	int out_len;
};


//解码回调函数
typedef int (*CallbackAsn1Decode)(void* cbParam, stuAsn1Pdu *pPdu);

#define CB_CHILD(pPdu,pInBuf) ((CallbackAsn1Decode*)pFun)(cbParam,pPdu);

#define SD_TRUE		1
#define SD_FALSE	0
#define SD_SUCCESS 	0
#define SD_FAILURE 	1

//#define CONSTR	   	0x20 	/* constructor				*/


#define ST_CHAR			char
#define ST_INT			signed int
#define ST_UCHAR		unsigned char
#define ST_UINT			unsigned int
#define ST_INT8			signed char
#define ST_INT16		signed short
#define ST_INT32		signed long
#define ST_UINT8		unsigned char
#define ST_UINT16		unsigned short
#define ST_UINT32		unsigned long
#define ST_BOOLEAN		unsigned char
#define ST_DOUBLE		double



#define BER_FLAGS_OPTIONAL	0x00000001
#define BER_FLAGS_IMPLTAG	0x00000002
#define BER_FLAGS_NOOWNTAG	0x00000004
#define BER_FLAGS_NOTCHKTAG	0x00000008


#define BER_CLASS_UNI	0
#define BER_CLASS_APP	1
#define BER_CLASS_CON	2
#define BER_CLASS_PRI	3
#define BER_CLASS_ANY   99			/* dont check class nor tag */

#define BER_UNI_TAG_EOC					0	/* 'end-of-content' */
#define BER_UNI_TAG_BOOLEAN				1
#define BER_UNI_TAG_INTEGER				2
#define BER_UNI_TAG_BITSTRING		    3
#define BER_UNI_TAG_OCTETSTRING		    4
#define BER_UNI_TAG_NULL				5
#define BER_UNI_TAG_OID					6	/* OBJECT IDENTIFIER */
#define BER_UNI_TAG_ObjectDescriptor	7
#define BER_UNI_TAG_EXTERNAL			8
#define BER_UNI_TAG_REAL				9
#define BER_UNI_TAG_ENUMERATED		    10
#define BER_UNI_TAG_EMBEDDED_PDV	    11
#define BER_UNI_TAG_UTF8String		    12
#define BER_UNI_TAG_RELATIVE_OID	    13
/* UNIVERSAL 14-15	
 * Reserved for future editions of this
 * Recommendation | International Standard
 */
#define BER_UNI_TAG_SEQUENCE		    16	/* SEQUENCE, SEQUENCE OF */
#define BER_UNI_TAG_SET					17	/* SET, SET OF */
/* UNIVERSAL 18-22 Character string types */
#define BER_UNI_TAG_NumericString	    18
#define BER_UNI_TAG_PrintableString	    19
#define BER_UNI_TAG_TeletexString	    20  /* TeletextString, T61String */
#define BER_UNI_TAG_VideotexString	    21
#define BER_UNI_TAG_IA5String		    22
/* UNIVERSAL 23-24 Time types */
#define BER_UNI_TAG_UTCTime				23
#define BER_UNI_TAG_GeneralizedTime	    24
/* UNIVERSAL 25-30 Character string types */
#define BER_UNI_TAG_GraphicString	    25
#define BER_UNI_TAG_VisibleString	    26  /* VisibleString, ISO64String */
#define BER_UNI_TAG_GeneralString	    27
#define BER_UNI_TAG_UniversalString	    28
#define BER_UNI_TAG_CHARACTERSTRING	    29
#define BER_UNI_TAG_BMPString		    30
/* UNIVERSAL 31- ...
 * Reserved for addenda to this Recommendation | International Standard
 */
struct ber_choice_t
{
	unsigned int value;
	//const int *p_id;
	char ber_class;
	int	tag;
	unsigned int flags;
	CallbackAsn1Decode func;
	const char   *strptr;
};

struct ber_sequence_t
{
	//const int *p_id;
	char ber_class;
	int	tag;
	unsigned int flags;
	CallbackAsn1Decode	func;
	const char   *strptr;
};
struct value_string
{
	unsigned int  value;
	const char   *strptr;
};

struct asn_namedbit
{
	unsigned int bit;
	//int *p_id;
	int gb0;  /* the 1st bit of "bit group", -1 = the 1st bit of current byte */
	int gb1;  /* last bit of "bit group", -1 = last bit of current byte */
	const char *tstr;  /* true string */
	const char *fstr;  /* false string */
};

class CAsn1BerDecoder
{
public:
	CAsn1BerDecoder();
	virtual ~CAsn1BerDecoder();

	int GetIntWithBufLen(BYTE *p,int l);
	int DecodeAsn1Ber(BYTE *pInBuf,int iLen,CallbackAsn1Decode pCallback,void* cbParam);

	int m_iFmtErrorNo;//当前报文解码对应的错误号
};

#endif
