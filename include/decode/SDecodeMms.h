/**
 *
 * �� �� �� : SDecodeMms.h
 * �������� : 2015-5-9 23:57
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : ����Э�����
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-5-9	�ۿ�������ļ�
 *
 **/
#ifndef __SKT_DECODER_MMS_H__
#define __SKT_DECODER_MMS_H__

#include "SDecodeBase.h"
#include "Asn1Ber.h"
#include "SDecodeAcsi.h"

enum eSDecodeMmsEvent
{
	//�¼�����					//��Ӧ���¼���ϸ�ṹ���壨<=128�ֽڣ�
	SDECODE_MMS_CONNECT=1,		//stuSDecodeEvent_MmsSession
	SDECODE_MMS_DISCONNECT=2,	//stuSDecodeEvent_MmsSession

	SDECODE_MMS_COUNT			//����¼�����
};

//MMS�¼����ƶ��壬�±���eSDecodeMmsEvent��Ӧ(���±����ʱӦע���±겻Ӧ����SDECODE_MMS_COUNT)
const char g_sDecodeMmsEventName[][32] =
{
	"",
	"����TCP����",				//SDECODE_MMS_CONNECT
	"�Ͽ�TCP����",				//SDECODE_MMS_DISCONNECT
};

struct stuSDecodeEvent_MmsSession
{
	unsigned int m_iClientIp,m_iServerIp;//�ͻ��ˡ������IP�������ֽ�˳��
	unsigned short m_iClientPort,m_iServerPort;//�ͻ��ˡ�����˶˿ںţ������ֽ�˳��
};

struct stuSDecodeMmsBuffer
{

};

//////////////////////////////////////////////////////////////////////////
// ��    ��:  SMmsSessionStatus
// ��    ��:  �ۿ���
// ����ʱ��:  2015-5-25 16:25
// ��    ��:  MMS�Ự״̬�ṹ
//////////////////////////////////////////////////////////////////////////
struct stuMmsSessionStatus
{
	int m_iTpktLeftByte;//TPKTʣ����ֽ�����Ӧ����һ֡ƴ�ӣ����ڴ���TPKT��Ƭ��IP��Ƭ
	int m_iCospType;
	int m_i8823ContextId;
	int m_iMmsInvokeID;
	int m_iMmsConfirmedServiceRequestId;
	int m_iMmsConfirmedServiceReponseId;
	unsigned char m_iCotpClass;//0~4
	unsigned char m_iIsExtended;//class 2~4
	SDecodeAcsiInfo_All *m_pAcsiInfo;//�ͻ��˼�����˵���Ϣָ��
};
struct stuDecodeIdName 
{
	const int  id;
	const char *name;
};

// #define SD_TRUE		1
// #define SD_FALSE	0
// #define SD_SUCCESS 	0
// #define SD_FAILURE 	1

//#define CONSTR	   	0x20 	/* constructor				*/


// #define ST_CHAR			char
// #define ST_INT			signed int
// #define ST_UCHAR		unsigned char
// #define ST_UINT			unsigned int
// #define ST_INT8			signed char
// #define ST_INT16		signed short
// #define ST_INT32		signed long
// #define ST_UINT8		unsigned char
// #define ST_UINT16		unsigned short
// #define ST_UINT32		unsigned long
// #define ST_BOOLEAN		unsigned char
// #define ST_DOUBLE		double

#define COSP_VER1	(ST_UCHAR) 1
#define COSP_VER2	(ST_UCHAR) 2

#define	COSP_SI_DATA   		(ST_UCHAR)  1
#define	COSP_SI_GIVE_TOKEN	(ST_UCHAR)  1  /* this is correct DT=GIVE-TOKEN */
#define	COSP_SI_FINISH 		(ST_UCHAR)  9
#define	COSP_SI_DISCON		(ST_UCHAR) 10
#define	COSP_SI_REFUSE 		(ST_UCHAR) 12
#define	COSP_SI_CONNECT		(ST_UCHAR) 13
#define	COSP_SI_ACCEPT		(ST_UCHAR) 14
#define	COSP_SI_ABORT  		(ST_UCHAR) 25
#define	COSP_SI_AB_ACCEPT  	(ST_UCHAR) 26

#define	COSP_ERR_DEC_INV_LEN	   0x3211  /* Invalid SPDU len (dec vs fun len)	*/
#define	COSP_ERR_DEC_INV_PI_CODE   0x3212  /* Invalid/out-of-place PGI/PI code	*/
#define	COSP_ERR_INV_UDATA_LEN     0x3206  /* Invalid User Data length		*/

#define MAX_SSEL_LEN	4	/* GOSIP Ver2 recommends len of 2	*/

#define COSP_MAX_UDATA_CON	(ST_UINT) 10240
#define COSP_MAX_ENVELOPE	(ST_UINT) 56
#define COSP_MAX_UDATA	       ((ST_UINT) 65535 - COSP_MAX_ENVELOPE)


//ASN.1
// ���ߡ���λָ��Tag�����ͣ�UniversalTag������00��ʾ;����λָ����������primitive(0)��ʽ���뻹��constructed(1)��ʽ���롣
#define BER_CLASS_UNI	0	//UNIVERSAL
#define BER_CLASS_APP	1	//Application
#define BER_CLASS_CON	2	//Context specific
#define BER_CLASS_PRI	3	//Private
#define BER_CLASS_ANY   99	/* dont check class nor tag */

#define BER_ENCODE_TYPE_PRIMITIVE 0
#define BER_ENCODE_TYPE_CONSTRUCTED 1

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

#define DECODE_CHILD(pFun) ret = pThis->DecodeAsn1Ber(pPdu->val,pPdu->len,pFun,pThis);

#ifdef SDECODE_OFFLINE
#define DECODE_CALLBACK_INTO \
	SDecodeMms *pThis = (SDecodeMms*)cbParam;\
	SDecodeTree *pLastTreeNode0=pThis->m_pLastTreeNode;\
	int ret = 0;
#define DECODE_TREE_SON(nodename,nodevalue,noderemark) \
	if(pThis->m_pLastTreeNode != NULL)\
	{\
	pThis->m_pLastTreeNode = pThis->m_pLastTreeNode->New(pThis->m_pPreprocessInfo->m_pViewBuffer,\
	pPdu->val-pThis->m_pPreprocessInfo->m_pViewBuffer,\
	pPdu->len,nodename,nodevalue,noderemark);\
}
#define DECODE_TREE_POS(pos,len,nodename,nodevalue,noderemark)\
	if(pLastTreeNode0 != NULL)\
	{\
	pThis->m_pLastTreeNode = pLastTreeNode0->New(pThis->m_pPreprocessInfo->m_pViewBuffer,\
	pPdu->val+pos-pThis->m_pPreprocessInfo->m_pViewBuffer,\
	len,nodename,nodevalue,noderemark);\
	}
#define DECODE_TREE(nodename,nodevalue,noderemark) \
	if(pLastTreeNode0 != NULL)\
	{\
	pThis->m_pLastTreeNode = pLastTreeNode0->New(pThis->m_pPreprocessInfo->m_pViewBuffer,\
	pPdu->val-pThis->m_pPreprocessInfo->m_pViewBuffer,\
	pPdu->len,nodename,nodevalue,noderemark);\
}
#define DECODE_CALLBACK_RET \
	pThis->m_pLastTreeNode = pLastTreeNode0;\
	return ret;
#define DECODE_ADDNAME(str) \
	if(pLastTreeNode0 != NULL) \
	pLastTreeNode0->AddNameText(str);
#define DECODE_ADDVALUE(str) \
	if(pLastTreeNode0 != NULL) \
	pLastTreeNode0->AddValueText(str);
#define SET_MMS_TYPE(a,b) pThis->m_pAcsiInfo->SetMmsType(a,b);
#else

#define DECODE_CALLBACK_INTO \
	SDecodeMms *pThis = (SDecodeMms*)cbParam;\
	int ret = 0;
#define DECODE_TREE_SON 
#define DECODE_TREE
#define DECODE_TREE_POS
#define DECODE_CALLBACK_RET \
	return ret;
#define DECODE_ADDNAME
#define DECODE_ADDVALUE
#define SET_MMS_TYPE(a,b) pThis->m_pAcsiInfo->SetMmsType(a,b);
#endif


//�������߷������������쳣ʱ����������֡����
#ifdef SDECODE_OFFLINE
#define NEW_MALFORMED {if(pThis->m_pDecodeTreeRoot != NULL) pThis->m_pDecodeTreeRoot->NewMalformed(NULL,0,0);}
#define NEW_MALFORMED_PD(buf_root,offset,len,desc) {if(pThis->m_pDecodeTreeRoot != NULL) pThis->m_pDecodeTreeRoot->NewMalformed(buf_root,offset,len,desc);}
#define NEW_MALFORMED_P(buf_root,offset,len) {if(pThis->m_pDecodeTreeRoot != NULL) pThis->m_pDecodeTreeRoot->NewMalformed(buf_root,offset,len);}
#define NEW_MALFORMED_D(desc) {if(pThis->m_pDecodeTreeRoot != NULL) pThis->m_pDecodeTreeRoot->NewMalformed(NULL,0,0,desc);}
#define NEW_MALFORMED_LEN {if(pThis->m_pDecodeTreeRoot != NULL) pThis->m_pDecodeTreeRoot->NewMalformed(NULL,0,0,"length invalid");}
#define NEW_MALFORMED_ID {if(pThis->m_pDecodeTreeRoot != NULL) pThis->m_pDecodeTreeRoot->NewMalformed(NULL,0,0,"identifier invalid");}
#else
#define NEW_MALFORMED
#define NEW_MALFORMED_PD(buf_root,offset,len,desc)
#define NEW_MALFORMED_P(buf_root,offset,len)
#define NEW_MALFORMED_D(desc)
#define NEW_MALFORMED_LEN
#define NEW_MALFORMED_ID
#endif 

struct stuMmsTempVal
{
	enum eValType
	{
		VAL_UNKNOWN=0,
		VAL_BOOL,
		VAL_INT,
		VAL_FLOAT,
		VAL_DOUBLE,
		VAL_STR,
		VAL_BITSTR,
		VAL_OCTSTR,
		VAL_TIME,
	};
	struct stuValStr
	{
		unsigned char* val;
		int len;//bitstrʱ��ʾBIT�����ַ���ʱ��ʾ�ֽ���
	};
	struct stuValTime
	{
		int soc;
		int nsec;
		int q;//-1��ʾ��Q
	};
	eValType type;
	union
	{
		bool v_bool;
		int v_int;
		float v_float;
		double v_double;
		stuValStr v_str;//��bitstr
		stuValTime v_time;
	} value;
};

//////////////////////////////////////////////////////////////////////////
// ��    ��:  SDecodeMms
// ��    ��:  �ۿ���
// ����ʱ��:  2015-5-9 23:57
// ��    ��:  ARP
//         :  SDECODE_BUF_C2S/SDECODE_BUF_S2C		��ӦTPKT����IP�㣩��Ƭ
//         :  SDECODE_BUF_EXT1/SDECODE_BUF_EXT2		��ӦCOTP��Ƭ
//////////////////////////////////////////////////////////////////////////
class SDecodeMms : public SDecodeBase,public CAsn1BerDecoder
{
public:
	SDecodeMms();
	virtual ~SDecodeMms();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����һ֡����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-5-10 11:15
	// ����˵��:  @soc��ʾ����ʱ�꣨�룩
	//         :  @nsec��ʾ����ʱ�꣨���룩
	// �� �� ֵ:  int,0��ʾ����ɹ�����ӦeSDecodeFmtError
	//////////////////////////////////////////////////////////////////////////
	virtual int DecodeFrame(int soc,int nsec);

	static void CB_FreeSessionStatus(BYTE *pStatus)
	{
		stuMmsSessionStatus *p = (stuMmsSessionStatus*)pStatus;
		if(p == NULL)
			return ;
		if(p->m_pAcsiInfo != NULL)
			delete p->m_pAcsiInfo;
	}
private:
	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  TPKT���뺯��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-5-25 19:40
	// ����˵��:  @p��ʾ��������ַ
	//         :  @len��ʾ����
	// �� �� ֵ:  0��ʾ�ɹ��������ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	int DecodeTpkt(BYTE *p,int len);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  8073(COTP)���뺯��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-5-25 19:41
	// ����˵��:  @p��ʾ��������ַ
	//         :  @len��ʾ����
	// �� �� ֵ:  0��ʾ�ɹ��������ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	int Decode8073(BYTE *p,int len);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����8327����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-5-11 8:57
	// ����˵��:  @pDatBuffer��ʾ8327����Ч���ݵĻ�������ַ
	//         :  @iLen��ʾ���ݳ���
	// �� �� ֵ:  int,0��ʾ����ɹ�����ӦeSDecodeFmtError
	//////////////////////////////////////////////////////////////////////////
	int Decode8327(BYTE *p8327DatBuffer,int iLen);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  8327Э��ȡ���ȼ�����ռ���ֽ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-5-25 19:42
	// ����˵��:  @bufΪ���Ȼ����׵�ַ
	//         :  @bytes��ʾ����ռ���ֽ�����ָ�룬�������
	// �� �� ֵ:  WORD�ͳ���
	//////////////////////////////////////////////////////////////////////////
	WORD Decode8327_GetLen(BYTE *buf, ST_UINT *bytes);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  8327���ӡ����ܱ��Ľ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-5-25 19:43
	// ����˵��:  
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	int Decode8327_Con_Acp(char *spdu_buf, ST_UINT spdu_len, ST_UCHAR spdu_type, BYTE* &pUserData,int &iUserDataLen);

	//����ΪASN.1���뷽ʽЭ��Ľ��룬��8823������ASN.1�����

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  8823����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-5-25 19:43
	// ����˵��:  @pDatbuffer��ʾ��������ַ
	//         :  @iLen��ʾ����
	// �� �� ֵ:  0��ʾ�ɹ��������ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	int Decode8823(BYTE *pDatBuffer,int iLen);
	static int Callback_8823(void* cbParam, stuAsn1Pdu *pPdu);
	static int Callback_8823_CP_type(void* cbParam, stuAsn1Pdu *pPdu);
	static int Callback_8823_CPA_PPDU(void* cbParam, stuAsn1Pdu *pPdu);
	static int Callback_8823_mode_selector(void* cbParam, stuAsn1Pdu *pPdu);
	static int Callback_8823_normal_mode_parameters(void* cbParam, stuAsn1Pdu *pPdu);
	static int Callback_8823_CPA_normal_mode_parameters(void* cbParam, stuAsn1Pdu *pPdu);
	static int Callback_8823_user_data(void* cbParam, stuAsn1Pdu *pPdu);
	static int Callback_8823_PDV_list(void* cbParam, stuAsn1Pdu *pPdu);
	int Decode8650(BYTE *pDatBuffer,int iLen);
	static int Callback_8650(void* cbParam, stuAsn1Pdu *pPdu);
	static int Callback_8650_aarq(void* cbParam, stuAsn1Pdu *pPdu);
	static int Callback_8650_aare(void* cbParam, stuAsn1Pdu *pPdu);
	static int Callback_8650_rlrq(void* cbParam, stuAsn1Pdu *pPdu);
	static int Callback_8650_rlre(void* cbParam, stuAsn1Pdu *pPdu);
	static int Callback_8650_Association_data(void* cbParam, stuAsn1Pdu *pPdu);
	static int Callback_8650_EXTERNALt(void* cbParam, stuAsn1Pdu *pPdu);
	int DecodeMms(BYTE *pDatBuffer,int iLen);
	static int Callback_MMS_pdu(void* cbParam, stuAsn1Pdu *pPdu);
	/*
	static int Callback_MMS_confirmed_RequestPDU(void* cbParam, stuAsn1Pdu *pPdu);
	static int Callback_MMS_confirmed_ResponsePDU(void* cbParam, stuAsn1Pdu *pPdu);
	static int Callback_MMS_confirmed_ErrorPDU(void* cbParam, stuAsn1Pdu *pPdu);
	static int Callback_MMS_unconfirmed_PDU(void* cbParam, stuAsn1Pdu *pPdu);
	static int Callback_MMS_rejectPDU(void* cbParam, stuAsn1Pdu *pPdu);
	static int Callback_MMS_cancel_RequestPDU(void* cbParam, stuAsn1Pdu *pPdu);
	static int Callback_MMS_cancel_ResponsePDU(void* cbParam, stuAsn1Pdu *pPdu);
	static int Callback_MMS_cancel_ErrorPDU(void* cbParam, stuAsn1Pdu *pPdu);
	static int Callback_MMS_initiate_RequestPDU(void* cbParam, stuAsn1Pdu *pPdu);
	static int Callback_MMS_initiate_ResponsePDU(void* cbParam, stuAsn1Pdu *pPdu);
	static int Callback_MMS_initRequestDetail(void* cbParam, stuAsn1Pdu *pPdu);
	static int Callback_MMS_ParameterSupportOptions(void* cbParam, stuAsn1Pdu *pPdu);
	static int Callback_MMS_ServiceSupportOptions(void* cbParam, stuAsn1Pdu *pPdu);
	static int Callback_MMS_initiate_ErrorPDU(void* cbParam, stuAsn1Pdu *pPdu);
	static int Callback_MMS_conclude_RequestPDU(void* cbParam, stuAsn1Pdu *pPdu);
	static int Callback_MMS_conclude_ResponsePDU(void* cbParam, stuAsn1Pdu *pPdu);
	static int Callback_MMS_conclude_ErrorPDU(void* cbParam, stuAsn1Pdu *pPdu);
	static int Callback_MMS_confirmedServiceRequest(void* cbParam, stuAsn1Pdu *pPdu);
	static int Callback_MMS_GetNameList_Request(void* cbParam, stuAsn1Pdu *pPdu);
	static int Callback_MMS_ObjectClass(void* cbParam, stuAsn1Pdu *pPdu);
	*/
public:
	//���һ�ν����Ӧ��״ָ̬��
	stuMmsSessionStatus *m_pLastSessionStatus;
	SDecodeMms *pThis;
	SDecodeAcsiInfo_All *m_pAcsiInfo;

	//��ʱMMS�����������ʱ����
	stuMmsTempVal *m_pTempVal;//�´ο���д�����ʱ����λ�ã�д������
	stuMmsTempVal m_TempObjectClass;
	stuMmsTempVal m_TempInvokeId;
	stuMmsTempVal m_TempDomainSpecific;
	stuMmsTempVal m_TempDomainId;
	stuMmsTempVal m_TempItemId;
	void ClearTempVal()
	{
		m_TempObjectClass.type		= stuMmsTempVal::VAL_UNKNOWN;
		m_TempInvokeId.type			= stuMmsTempVal::VAL_UNKNOWN;
		m_TempDomainSpecific.type	= stuMmsTempVal::VAL_UNKNOWN;
		m_TempDomainId.type			= stuMmsTempVal::VAL_UNKNOWN;
		m_TempItemId.type			= stuMmsTempVal::VAL_UNKNOWN;
	}
// 	int m_iTempValue;
// 	float m_fTempValue;
// 	unsigned char* m_pPtr_domainId;//domainId
// 	int m_iLen_domainId;//domainId

#ifdef SDECODE_OFFLINE
	SDecodeTree *m_pLastTreeNode;
#endif
};

int get_ber_identifier(unsigned char *buf,int offset,char *cls,bool *pc, int *bytes);
int get_ber_length(unsigned char *buf, int offset, unsigned int *len, bool *idx);

int dissect_ber_sequence(void *cbParam,stuAsn1Pdu *pPdu,const ber_sequence_t *seq);
int dissect_ber_choice(void *cbParam,stuAsn1Pdu *pPdu,const ber_choice_t *choice);
int dissect_ber_bitstring(void *cbParam,stuAsn1Pdu *pPdu,const asn_namedbit *seq);
int dissect_ber_integer(void *cbParam,stuAsn1Pdu *pPdu,const value_string *vs);
int dissect_ber_uinteger(void *cbParam,stuAsn1Pdu *pPdu,const value_string *vs);
int dissect_ber_restricted_string(void *cbParam,stuAsn1Pdu *pPdu,void *seq);
int dissect_ber_boolean(void *cbParam,stuAsn1Pdu *pPdu,void *seq);
int dissect_ber_sequence_of(void *cbParam,stuAsn1Pdu *pPdu,const ber_sequence_t *seq);
int dissect_ber_null(void *cbParam,stuAsn1Pdu *pPdu,void *seq);
int dissect_ber_FloatingAndDouble(void *cbParam,stuAsn1Pdu *pPdu,void *seq);
int dissect_ber_octet_string(void *cbParam,stuAsn1Pdu *pPdu,void *seq);
int dissect_ber_object_identifier(void *cbParam,stuAsn1Pdu *pPdu,void *seq);
int dissect_ber_GeneralizedTime(void *cbParam,stuAsn1Pdu *pPdu,void *seq);
int dissect_ber_tagged_type(void *cbParam,stuAsn1Pdu *pPdu,const ber_sequence_t *seq);
//int dissect_mms_Unsigned8(void *cbParam,stuAsn1Pdu *pPdu,void *seq);
//int dissect_mms_Identifier(void *cbParam,stuAsn1Pdu *pPdu,void *seq);
int dissect_ber_utctime(void *cbParam,stuAsn1Pdu *pPdu,void *seq);
int dissect_ber_timeofday(void *cbParam,stuAsn1Pdu *pPdu,void *seq);

//int dissect_mms_MMSpdu(void* cbParam, stuAsn1Pdu *pPdu);

#endif//__SKT_DECODER_MMS_H__

