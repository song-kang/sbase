
#include "SDecodeMms.h"


#if 0
int SDecodeMms::DecodeMms(BYTE *pInDatBuffer,int iLen)
{
	int ret = 0;
	BYTE * p;
	int off = 0;
	unsigned int info_type;
	ST_UCHAR t;
	ST_UCHAR constr_elmnt;
	ST_UINT decode_id;
	ST_UINT id_bytes;
	ST_INT decode_len;
	ST_UINT len_bytes;
	
	int invokeId;
	SDecodeTree *pMMS,*pNode2,pNode3;
	p = pInDatBuffer;
	decode_len=iLen;
#ifdef SDECODE_OFFLINE
	if(m_pDecodeTreeRoot != NULL)
	{
		pMMS = m_pDecodeTreeRoot->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,
			iLen,"MMS");
	}
#endif


	ASN1_IN;
	switch(decode_id)
		//MMSpdu ::= CHOICE
	{
	case 0://	confirmed-RequestPDU	[0] 	IMPLICIT Confirmed-RequestPDU,
#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			pMMS = pMMS->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,
				decode_len,"confirmed-RequestPDU");
		}
#endif
		ASN1_IN;
		if(GetBerTagClass(t) == BER_CLASS_UNI && decode_id == BER_UNI_TAG_INTEGER)
		{
#ifdef SDECODE_OFFLINE
			if(m_pDecodeTreeRoot != NULL)
			{
				pMMS->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,
					decode_len,"invokeID",SString::toFormat("%d",GetIntWithBufLen(p,decode_len)).data());
			}
#endif
		}
		if(GetBerTagClass(t) != BER_CLASS_UNI && decode_id == 1)
		{
#ifdef SDECODE_OFFLINE
			if(m_pDecodeTreeRoot != NULL)
			{
				pMMS = pMMS->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,
					decode_len,"confirmedServiceRequest",SString::toFormat("%d",decode_id).data());
			}
#endif
			ASN1_IN;
			if(decode_id == 0)
			{
				pNode2 = pMMS->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,
					decode_len,"extendedObjectClass",SString::toFormat("%d",decode_id).data());
				ASN1_IN;
				if(decode_id == 0)
				{
					pNode2->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,
						decode_len,"objectClass",SString::toFormat("%d",*p).data());
				}
				ASN1_OUT;
			}
			else if(decode_id == 1)
			{
				pNode2 = pMMS->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,
					decode_len,"objectScope",SString::toFormat("%d",decode_id).data());
				ASN1_IN;
				if(decode_id == 0)
				{
					pNode2->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,
						decode_len,len_bytes == 0||*p==0?"vmdSpecific":(*p==1?"domainSpecific":"aaSpecific"));
				}
				ASN1_OUT;
			}
			ASN1_OUT;
		}
		ASN1_OUT;
		break;
	case 1://	confirmed-ResponsePDU	[1] 	IMPLICIT Confirmed-ResponsePDU,
#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			pMMS = pMMS->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,
				decode_len,"confirmed-ResponsePDU");
		}
#endif
		break;
	case 2://	confirmed-ErrorPDU		[2] 	IMPLICIT Confirmed-ErrorPDU,
#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			pMMS = pMMS->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,
				decode_len,"confirmed-ErrorPDU");
		}
#endif
		break;
	case 3://	unconfirmed-PDU			[3] 	IMPLICIT Unconfirmed-PDU,
#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			pMMS = pMMS->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,
				decode_len,"unconfirmed-PDU");
		}
#endif
		break;
	case 4://	rejectPDU				[4] 	IMPLICIT RejectPDU,
#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			pMMS = pMMS->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,
				decode_len,"rejectPDU");
		}
#endif
		break;
	case 5://	cancel-RequestPDU		[5] 	IMPLICIT Cancel-RequestPDU,
#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			pMMS = pMMS->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,
				decode_len,"cancel-RequestPDU");
		}
#endif
		break;
	case 6://	cancel-ResponsePDU		[6] 	IMPLICIT Cancel-ResponsePDU,
#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			pMMS = pMMS->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,
				decode_len,"cancel-ResponsePDU");
		}
#endif
		break;
	case 7://	cancel-ErrorPDU			[7] 	IMPLICIT Cancel-ErrorPDU,
#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			pMMS = pMMS->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,
				decode_len,"cancel-ErrorPDU");
		}
#endif
		break;
	case 8://	initiate-RequestPDU		[8] 	IMPLICIT Initiate-RequestPDU,
#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			pMMS = pMMS->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,
				decode_len,"initiate-RequestPDU");
		}
#endif
		break;
	case 9://	initiate-ResponsePDU	[9] 	IMPLICIT Initiate-ResponsePDU,
#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			pMMS = pMMS->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,
				decode_len,"initiate-ResponsePDU");
		}
#endif
		break;
	case 10://	initiate-ErrorPDU		[10] 	IMPLICIT Initiate-ErrorPDU,
#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			pMMS = pMMS->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,
				decode_len,"initiate-ErrorPDU");
		}
#endif
		break;
	case 11://	conclude-RequestPDU		[11] 	IMPLICIT Conclude-RequestPDU,
#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			pMMS = pMMS->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,
				decode_len,"conclude-RequestPDU");
		}
#endif
		break;
	case 12://	conclude-ResponsePDU	[12] 	IMPLICIT Conclude-ResponsePDU,
#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			pMMS = pMMS->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,
				decode_len,"conclude-ResponsePDU");
		}
#endif
		break;
	case 13://	conclude-ErrorPDU		[13] 	IMPLICIT Conclude-ErrorPDU
#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			pMMS = pMMS->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,
				decode_len,"conclude-ErrorPDU");
		}
#endif
		break;
	}
	ASN1_OUT;


#if 0


	t = *p;
	decode_id = (ST_UINT)asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF)
	{
		NEW_MALFORMED_ID;
		return SDECODE_FMT_MMS;	
	}
	decode_len = (ST_INT)asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1)
	{
		NEW_MALFORMED_LEN;
		return SDECODE_FMT_MMS;	
	}
#ifdef SDECODE_OFFLINE
	SDecodeTree *pMMS;
	if(m_pDecodeTreeRoot != NULL)
	{
		pMMS = m_pDecodeTreeRoot->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,
			decode_len,"ISO 9506 MMS");
	}
#endif
	// 	m_Osi_9506.m_osi9506_type = c;
	// 	m_9506Type = c;
// 	InitDataBasepCap(&m_Osi_9506.m_9506type,p,DATATYPE_UCHAR,off,off + id_bytes + len_bytes,id_bytes + len_bytes);
// 	InitDataBasepCap(&m_Osi_9506.m_ilen,p,DATATYPE_UCHAR,off,off + id_bytes + len_bytes + decode_len,id_bytes + len_bytes + decode_len);
	p += id_bytes + len_bytes;
	off += id_bytes + len_bytes;
	endoff = off + id_bytes + len_bytes + decode_len;
	
	switch(decode_id)
	//MMSpdu ::= CHOICE
	{
	case 0://	confirmed-RequestPDU	[0] 	IMPLICIT Confirmed-RequestPDU,
#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			pMMS = pMMS->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,
				decode_len,"confirmed-RequestPDU");
		}
#endif
		t = *p;
		constr_elmnt = (ST_UCHAR) (t & CONSTR);/* isolate the constr bit	*/
		decode_len = (ST_INT)asn1r_head_decode_len(p + id_bytes,&len_bytes);
		if (decode_len == -1)
		{
			NEW_MALFORMED_LEN;
			return SDECODE_FMT_MMS;
		}
		if (constr_elmnt)
		{
			p += id_bytes + len_bytes;
			off += id_bytes + len_bytes;
		}
		else
		{
			p += id_bytes + len_bytes + decode_len;
			off += id_bytes + len_bytes + decode_len;
		}
		if(off > iLen)
		{
			NEW_MALFORMED_LEN;
			return SDECODE_FMT_MMS;
		}

		break;
	case 1://	confirmed-ResponsePDU	[1] 	IMPLICIT Confirmed-ResponsePDU,
#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			pMMS = pMMS->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,
				decode_len,"confirmed-ResponsePDU");
		}
#endif
		break;
	case 2://	confirmed-ErrorPDU		[2] 	IMPLICIT Confirmed-ErrorPDU,
#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			pMMS = pMMS->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,
				decode_len,"confirmed-ErrorPDU");
		}
#endif
		break;
	case 3://	unconfirmed-PDU			[3] 	IMPLICIT Unconfirmed-PDU,
#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			pMMS = pMMS->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,
				decode_len,"unconfirmed-PDU");
		}
#endif
		break;
	case 4://	rejectPDU				[4] 	IMPLICIT RejectPDU,
#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			pMMS = pMMS->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,
				decode_len,"rejectPDU");
		}
#endif
		break;
	case 5://	cancel-RequestPDU		[5] 	IMPLICIT Cancel-RequestPDU,
#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			pMMS = pMMS->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,
				decode_len,"cancel-RequestPDU");
		}
#endif
		break;
	case 6://	cancel-ResponsePDU		[6] 	IMPLICIT Cancel-ResponsePDU,
#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			pMMS = pMMS->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,
				decode_len,"cancel-ResponsePDU");
		}
#endif
		break;
	case 7://	cancel-ErrorPDU			[7] 	IMPLICIT Cancel-ErrorPDU,
#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			pMMS = pMMS->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,
				decode_len,"cancel-ErrorPDU");
		}
#endif
		break;
	case 8://	initiate-RequestPDU		[8] 	IMPLICIT Initiate-RequestPDU,
#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			pMMS = pMMS->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,
				decode_len,"initiate-RequestPDU");
		}
#endif
		break;
	case 9://	initiate-ResponsePDU	[9] 	IMPLICIT Initiate-ResponsePDU,
#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			pMMS = pMMS->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,
				decode_len,"initiate-ResponsePDU");
		}
#endif
		break;
	case 10://	initiate-ErrorPDU		[10] 	IMPLICIT Initiate-ErrorPDU,
#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			pMMS = pMMS->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,
				decode_len,"initiate-ErrorPDU");
		}
#endif
		break;
	case 11://	conclude-RequestPDU		[11] 	IMPLICIT Conclude-RequestPDU,
#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			pMMS = pMMS->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,
				decode_len,"conclude-RequestPDU");
		}
#endif
		break;
	case 12://	conclude-ResponsePDU	[12] 	IMPLICIT Conclude-ResponsePDU,
#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			pMMS = pMMS->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,
				decode_len,"conclude-ResponsePDU");
		}
#endif
		break;
	case 13://	conclude-ErrorPDU		[13] 	IMPLICIT Conclude-ErrorPDU
#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			pMMS = pMMS->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,
				decode_len,"conclude-ErrorPDU");
		}
#endif
		break;
	}
	




	switch (c)
	{
	case 0xa0:
	case 0xa1:
		{
#ifdef SDECODE_OFFLINE
			if(m_pDecodeTreeRoot != NULL)
			{
				pMMS = pMMS->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,
					decode_len,c==0xa0?"Confirmed-RequestPDU":"Confirmed-ResponsePDU");
			}
#endif
			decode_id = (ST_UINT)asn1r_head_decode_id(p,&id_bytes);
			if (decode_id == 0xFFFF)
			{
				NEW_MALFORMED_ID;
				return SDECODE_FMT_MMS;	
			}
			c = *p;
			constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
			decode_len = (ST_INT)asn1r_head_decode_len(p + id_bytes,&len_bytes);
			if (decode_len == -1)
			{
				NEW_MALFORMED_LEN;
				return SDECODE_FMT_MMS;	
			}
			p += id_bytes + len_bytes;
			off += id_bytes + len_bytes;
			if (!constr_elmnt)
			{
				invokeId = GetIntWithBufLen(p,decode_len);
#ifdef SDECODE_OFFLINE
				if(m_pDecodeTreeRoot != NULL)
				{
					pMMS->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,
						decode_len,"invokeID",SString::toFormat("%d",invokeId).data());
				}
#endif
				//InitDataBasepCap(&m_Osi_9506.m_InvokeID,p,DATATYPE_UINT32,off - id_bytes - len_bytes,off + decode_len,decode_len);
				//m_iInvokeID = GetData_Val(&m_Osi_9506.m_InvokeID);
				p += decode_len;
				off += decode_len;
			}

			decode_id = (ST_UINT)asn1r_head_decode_id(p,&id_bytes);
			if (decode_id == 0xFFFF)
			{
				NEW_MALFORMED_ID;
				return SDECODE_FMT_MMS;	
			}
			decode_len = (ST_INT)asn1r_head_decode_len(p + id_bytes,&len_bytes);
			if (decode_len == -1)
			{
				NEW_MALFORMED_LEN;
				return SDECODE_FMT_MMS;	
			}
			if (id_bytes == 1)
				info_type = *p;
			else
				info_type = *(p+1);

//			InitDataBasepCap(&m_Osi_9506.m_MmsInfoType,p,DATATYPE_UCHAR,off,off + id_bytes + len_bytes,id_bytes);
// 			if (id_bytes == 1)
// 			{
// 				m_Osi_9506.m_mmsinfo_type = *p;
// 				m_MmsType = *p;
// 			}
// 			else
// 			{
// 				m_Osi_9506.m_mmsinfo_type = *(p + 1);
// 				m_MmsType = *(p + 1);
// 			}
			InitDataBasepCap(&m_Osi_9506.m_InfoData,p,DATATYPE_UCHAR,off,off + id_bytes + len_bytes + decode_len,id_bytes + len_bytes + decode_len);
			//            InitDataBasepCap(&m_Osi_9506.m_InfoData,p,DATATYPE_UCHAR,off,m_BufLength,m_BufLength - off);
			if (m_Osi_9506.m_osi9506_type == 0xa0 &&
				(m_Osi_9506.m_mmsinfo_type == 0xa4 || m_Osi_9506.m_mmsinfo_type == 0xa5))
			{
				GetObjectItemID_Type(p);
			}
			p += id_bytes + len_bytes;
			off += id_bytes + len_bytes;
			p += decode_len;
			off += decode_len;
			//            p += 1;
			//            off += 1;

		}
		break;
	case 0xA8:
	case oxA9:
#ifdef SDECODE_OFFLINE
		if(m_pDecodeTreeRoot != NULL)
		{
			pMMS = pMMS->New(m_pPreprocessInfo->m_pViewBuffer,p-m_pPreprocessInfo->m_pViewBuffer,
				decode_len,c==0xa8?"localDetailCalling":"localDetailCalled");
		}
#endif
		break;
	case 0xA3:
//		InitDataBasepCap(&m_Osi_9506.m_InfoData,p,DATATYPE_UCHAR,off,m_BufLength,m_BufLength - off);
		break;
	default:
		break;
	}
#endif

// 	m_Osi_9506.m_bOsi9506 = TRUE;
// 	ret = endoff;
	
	return 0;
}

#if 0

int SDecodeMms::ShowTree_ACSI_Osi9506(QTreeWidgetItem *pTreeItem, int *expandno, int expandmax, bool bmms2)
{
	int iexpanded_no, iexpanded_max;
	QTreeWidgetItem * item, * item1, * item2;
	QString strItem, strItemInfo, strItemLen, strtmp;
	unsigned char c;
	SWF_MmsAcsi * pMmsAcsi;
	int ioff = m_iMMSEthLen;
	pMmsAcsi = &m_database_Acsi;
	if (bmms2)
	{
		ioff = m_iMMSEthLen + m_database_Acsi.m_BufLength - m_database_Acsi.m_iLeavelen;
		pMmsAcsi = &m_database_Acsi_2;
	}

	iexpanded_no = *expandno;
	iexpanded_max = expandmax;
	item = pTreeItem;

	if (!pMmsAcsi->m_Osi_9506.m_bOsi9506)
	{
		return -1;
	}

	strItem = tr("ISO/IEC 9506 MMS");
	strItemLen.sprintf("(%d-%d)",
		ioff + pMmsAcsi->m_Osi_9506.m_ilen.m_Data_off,
		ioff + pMmsAcsi->m_Osi_9506.m_ilen.m_Data_End - 1);
	item1 = new QTreeWidgetItem(item);
	item1->setText(0,strItem);
	item1->setText(2,strItemLen);
	item->addChild(item1);
	m_items.append(item1);
	if (iexpanded_no < iexpanded_max)
	{
		if (m_ExpandedItemList.at(iexpanded_no) == strItem)
		{
			iexpanded_no ++;
			item1->setExpanded(true);
		}
	}
	item1->setExpanded(true);

	strItem = tr("Type");
	if (pMmsAcsi->m_Osi_9506.m_osi9506_type != 0x00)
	{
		c = pMmsAcsi->m_Osi_9506.m_osi9506_type & 0x0F;
		strtmp.sprintf(" (%d)", c);
		strItemInfo = OSI9506_TYPE_STR.at(c) + strtmp;
	}
	else
	{
		strItem = tr("Not sure of the type");
	}
	strItemLen.sprintf("(%d-%d)",
		ioff + pMmsAcsi->m_Osi_9506.m_9506type.m_Data_off,
		ioff + pMmsAcsi->m_Osi_9506.m_9506type.m_Data_End - 1);
	item2 = new QTreeWidgetItem(item1);
	item2->setText(0,strItem);
	item2->setText(1,strItemInfo);
	item2->setText(2,strItemLen);
	item1->addChild(item2);
	m_items.append(item2);

	if (pMmsAcsi->m_Osi_9506.m_osi9506_type != 0x00)
	{
		switch(pMmsAcsi->m_Osi_9506.m_osi9506_type)
		{
		case 0xa0:
		case 0xa1:
			strItem = tr("mms info type");
			strItemInfo = m_pWnd_FileAnaWindow->m_pWnd_FrameList_Wave_Form->GetMmsTypeStr(
				pMmsAcsi->m_Osi_9506.m_osi9506_type, pMmsAcsi->m_Osi_9506.m_mmsinfo_type);
			if (!strItemInfo.isEmpty())
			{
				if (pMmsAcsi->m_Osi_9506.m_mmsinfo_type & 0x40)
				{
					strtmp.sprintf(" (%d)",pMmsAcsi->m_Osi_9506.m_mmsinfo_type);
				}
				else
				{
					strtmp.sprintf(" (%d)",pMmsAcsi->m_Osi_9506.m_mmsinfo_type & 0x0F);
				}
			}
			else
			{
				strtmp.sprintf(" (%d)",pMmsAcsi->m_Osi_9506.m_mmsinfo_type & 0x0F);
			}
			strItemInfo += strtmp;
			strItemLen.sprintf("(%d-%d)",
				ioff + pMmsAcsi->m_Osi_9506.m_MmsInfoType.m_Data_off,
				ioff + pMmsAcsi->m_Osi_9506.m_MmsInfoType.m_Data_End - 1);
			item2 = new QTreeWidgetItem(item1);
			item2->setText(0,strItem);
			item2->setText(1,strItemInfo);
			item2->setText(2,strItemLen);
			item1->addChild(item2);
			m_items.append(item2);

			strItem = tr("InvokeID");
			strItemInfo.sprintf("%d",DataBase_Info::GetData_Val(&pMmsAcsi->m_Osi_9506.m_InvokeID));
			strItemLen.sprintf("(%d-%d)",
				ioff + pMmsAcsi->m_Osi_9506.m_InvokeID.m_Data_off,
				ioff + pMmsAcsi->m_Osi_9506.m_InvokeID.m_Data_End - 1);
			item2 = new QTreeWidgetItem(item1);
			item2->setText(0,strItem);
			item2->setText(1,strItemInfo);
			item2->setText(2,strItemLen);
			item1->addChild(item2);
			m_items.append(item2);

			strItem = tr("mms info Data");
			strItemInfo = m_pWnd_FileAnaWindow->m_pWnd_FrameList_Wave_Form->GetMmsTypeStr(
				pMmsAcsi->m_Osi_9506.m_osi9506_type, pMmsAcsi->m_Osi_9506.m_mmsinfo_type);
			strItemLen.sprintf("(%d-%d)",
				ioff + pMmsAcsi->m_Osi_9506.m_InfoData.m_Data_off,
				ioff + pMmsAcsi->m_Osi_9506.m_InfoData.m_Data_End - 1);
			item2 = new QTreeWidgetItem(item1);
			item2->setText(0,strItem);
			item2->setText(1,strItemInfo);
			item2->setText(2,strItemLen);
			item1->addChild(item2);
			m_items.append(item2);
			if (iexpanded_no < iexpanded_max)
			{
				if (m_ExpandedItemList.at(iexpanded_no) == strItem)
				{
					iexpanded_no ++;
					item2->setExpanded(true);
				}
			}
			item2->setExpanded(true);

			Ana_CFRQ_CFRS_Info_Show_Tree(item2, &iexpanded_no, iexpanded_max, bmms2);
			break;
		case 0xA8:
		case 0xA9:
			Ana_INIRQ_INIRS_Info_Show_Tree(item1, &iexpanded_no, iexpanded_max, bmms2);
			break;
		case 0xA3:
			Ana_Unconfirmed_Info_Show_Tree(item1, &iexpanded_no, iexpanded_max, bmms2);
			break;
		default:
			break;
		}
	}
	return 0;
}

int SDecodeMms::Ana_CFRQ_CFRS_Info_Show_Tree(QTreeWidgetItem * pTreeItem, int * expandno, int expandmax, bool bmms2)
{
	int iexpanded_no, iexpanded_max;
	QTreeWidgetItem * item;
	SWF_MmsAcsi * pMmsAcsi;
	int ioff = m_iMMSEthLen;
	pMmsAcsi = &m_database_Acsi;
	if (bmms2)
	{
		ioff = m_iMMSEthLen + m_database_Acsi.m_BufLength - m_database_Acsi.m_iLeavelen;
		pMmsAcsi = &m_database_Acsi_2;
	}

	iexpanded_no = *expandno;
	iexpanded_max = expandmax;
	item = pTreeItem;

	if (!pMmsAcsi->m_Osi_9506.m_bOsi9506)
		return 0;
	unsigned char * pInfodata = pMmsAcsi->m_Osi_9506.m_InfoData.m_Data.pBYTE;
	if (pInfodata == NULL)
		return 0;

	int off = (int)(pMmsAcsi->m_Osi_9506.m_InfoData.m_Data.pBYTE - pMmsAcsi->m_pBuf);
	unsigned char * p = pInfodata;
	int ilen = 0;
	ST_UINT decode_id;
	ST_UINT id_bytes;
	ST_INT decode_len;
	ST_UINT len_bytes;

	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return 0;
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return 0;
	ilen = id_bytes + len_bytes;
	p += ilen;
	off += ilen;

	if (pMmsAcsi->m_Osi_9506.m_osi9506_type == 0xa0 && pMmsAcsi->m_Osi_9506.m_mmsinfo_type == 0xa1)
	{
		Ana_CFRQ_GetNameList_Show_Tree(item,p,off, &iexpanded_no, iexpanded_max, bmms2);
	}
	else if(pMmsAcsi->m_Osi_9506.m_osi9506_type == 0xa1 && pMmsAcsi->m_Osi_9506.m_mmsinfo_type == 0xa1)
	{
		Ana_CFRS_GetNameList_Show_Tree(item,p,off, &iexpanded_no, iexpanded_max, bmms2);
	}
	else if(pMmsAcsi->m_Osi_9506.m_osi9506_type == 0xa0 && pMmsAcsi->m_Osi_9506.m_mmsinfo_type == 0xa4)
	{
		Ana_CFRQ_Read_Show_Tree(item,p,off, &iexpanded_no, iexpanded_max, bmms2);
	}
	else if(pMmsAcsi->m_Osi_9506.m_osi9506_type == 0xa1 && pMmsAcsi->m_Osi_9506.m_mmsinfo_type == 0xa4)
	{
		Ana_CFRS_Read_Show_Tree(item,p,off, &iexpanded_no, iexpanded_max, bmms2);
	}
	else if(pMmsAcsi->m_Osi_9506.m_osi9506_type == 0xa0 && pMmsAcsi->m_Osi_9506.m_mmsinfo_type == 0xa5)
	{
		Ana_CFRQ_Write_Show_Tree(item,p,off, &iexpanded_no, iexpanded_max, bmms2);
	}
	else if(pMmsAcsi->m_Osi_9506.m_osi9506_type == 0xa1 && pMmsAcsi->m_Osi_9506.m_mmsinfo_type == 0xa5)
	{
		Ana_CFRS_Write_Show_Tree(item,p,off, &iexpanded_no, iexpanded_max, bmms2);
	}
	else if(pMmsAcsi->m_Osi_9506.m_osi9506_type == 0xa0 && pMmsAcsi->m_Osi_9506.m_mmsinfo_type == 0xa6)
	{
		Ana_CFRQ_GetVariableAA_Show_Tree(item,p,off, &iexpanded_no, iexpanded_max, bmms2);
	}
	else if(pMmsAcsi->m_Osi_9506.m_osi9506_type == 0xa1 && pMmsAcsi->m_Osi_9506.m_mmsinfo_type == 0xa6)
	{
		Ana_CFRS_GetVariableAA_Show_Tree(item,p,off, &iexpanded_no, iexpanded_max, bmms2);
	}
	else if(pMmsAcsi->m_Osi_9506.m_osi9506_type == 0xa0 && pMmsAcsi->m_Osi_9506.m_mmsinfo_type == 0xac)
	{
		Ana_CFRQ_GetNamedVarVLA_Show_Tree(item,p,off, &iexpanded_no, iexpanded_max, bmms2);
	}
	else if(pMmsAcsi->m_Osi_9506.m_osi9506_type == 0xa1 && pMmsAcsi->m_Osi_9506.m_mmsinfo_type == 0xac)
	{
		Ana_CFRS_GetNamedVarVLA_Show_Tree(item,p,off, &iexpanded_no, iexpanded_max, bmms2);
	}
	else if(pMmsAcsi->m_Osi_9506.m_osi9506_type == 0xa0 && pMmsAcsi->m_Osi_9506.m_mmsinfo_type == 0x4D)
	{
		Ana_CFRQ_FileDir_Show_Tree(item,p,off, &iexpanded_no, iexpanded_max, bmms2);
		// 		GetDataToTree(InfoRoot,pMmsA,p,off,decode_len);
	}
	else if(pMmsAcsi->m_Osi_9506.m_osi9506_type == 0xa1 && pMmsAcsi->m_Osi_9506.m_mmsinfo_type == 0x4D)
	{
		Ana_CFRS_FileDir_Show_Tree(item,p,off, &iexpanded_no, iexpanded_max, bmms2);
	}
	else if(pMmsAcsi->m_Osi_9506.m_osi9506_type == 0xa0 && pMmsAcsi->m_Osi_9506.m_mmsinfo_type == 0x48)
	{
		Ana_CFRQ_FileOpen_Show_Tree(item,p,off, &iexpanded_no, iexpanded_max, bmms2);
		// 		GetDataToTree(InfoRoot,pMmsA,p,off,decode_len);
	}
	else if(pMmsAcsi->m_Osi_9506.m_osi9506_type == 0xa1 && pMmsAcsi->m_Osi_9506.m_mmsinfo_type == 0x48)
	{
		Ana_CFRS_FileOpen_Show_Tree(item,p,off, &iexpanded_no, iexpanded_max, bmms2);
	}
	else if(pMmsAcsi->m_Osi_9506.m_osi9506_type == 0xa0 && pMmsAcsi->m_Osi_9506.m_mmsinfo_type == 0x49)
	{
		Ana_CFRQ_FileRead_Show_Tree(item,p - ilen,off - ilen, &iexpanded_no, iexpanded_max, bmms2);
	}
	else if(pMmsAcsi->m_Osi_9506.m_osi9506_type == 0xa1 && pMmsAcsi->m_Osi_9506.m_mmsinfo_type == 0x49)
	{
		Ana_CFRS_FileRead_Show_Tree(item,p,off, &iexpanded_no, iexpanded_max, bmms2);
	}
	else if(pMmsAcsi->m_Osi_9506.m_osi9506_type == 0xa0 && pMmsAcsi->m_Osi_9506.m_mmsinfo_type == 0x4A)
	{
		Ana_CFRQ_FileClose_Show_Tree(item,p - ilen,off - ilen, &iexpanded_no, iexpanded_max, bmms2);
	}
	else if(pMmsAcsi->m_Osi_9506.m_osi9506_type == 0xa0 && pMmsAcsi->m_Osi_9506.m_mmsinfo_type == 0x4B)
	{
		Ana_CFRQ_FileRename_Show_Tree(item,p,off, &iexpanded_no, iexpanded_max, bmms2);
	}
	else if(pMmsAcsi->m_Osi_9506.m_osi9506_type == 0xa0 && pMmsAcsi->m_Osi_9506.m_mmsinfo_type == 0x4C)
	{
		Ana_CFRQ_FileDelete_Show_Tree(item,p,off, &iexpanded_no, iexpanded_max, bmms2);
	}
	else if(pMmsAcsi->m_Osi_9506.m_osi9506_type == 0xa1 && pMmsAcsi->m_Osi_9506.m_mmsinfo_type == 0xa2)
	{
		Ana_CFRS_Identify_Show_Tree(item,p,off, &iexpanded_no, iexpanded_max, bmms2);
	}
	else if(pMmsAcsi->m_Osi_9506.m_osi9506_type == 0xa0 && pMmsAcsi->m_Osi_9506.m_mmsinfo_type == 0x80)
	{
		Ana_CFRQ_Status_Show_Tree(item,p,off, &iexpanded_no, iexpanded_max, bmms2);
	}
	else if(pMmsAcsi->m_Osi_9506.m_osi9506_type == 0xa1 && pMmsAcsi->m_Osi_9506.m_mmsinfo_type == 0xa0)
	{
		Ana_CFRS_Status_Show_Tree(item,p,off, &iexpanded_no, iexpanded_max, bmms2);
	}

	return 0;
}

int SDecodeMms::Ana_INIRQ_INIRS_Info_Show_Tree(QTreeWidgetItem * pTreeItem, int * expandno, int expandmax, bool bmms2)
{
	int iexpanded_no, iexpanded_max;
	QTreeWidgetItem * item, * item1, * item2;
	QString strItem, strItemInfo, strItemLen;
	SWF_MmsAcsi * pMmsAcsi;
	int ioff = m_iMMSEthLen;
	pMmsAcsi = &m_database_Acsi;
	if (bmms2)
	{
		ioff = m_iMMSEthLen + m_database_Acsi.m_BufLength - m_database_Acsi.m_iLeavelen;
		pMmsAcsi = &m_database_Acsi_2;
	}

	iexpanded_no = *expandno;
	iexpanded_max = expandmax;
	item = pTreeItem;


	unsigned char * pInfodata = pMmsAcsi->m_Osi_9506.m_9506type.m_Data.pBYTE + pMmsAcsi->m_Osi_9506.m_9506type.m_Data_len;
	if (pInfodata == NULL)
		return 0;

	int off = (int)(pMmsAcsi->m_Osi_9506.m_9506type.m_Data.pBYTE - pMmsAcsi->m_pBuf) + (int)pMmsAcsi->m_Osi_9506.m_9506type.m_Data_len;
	unsigned char * p = pInfodata;
	int ilen = 0;
	ST_UINT decode_id;
	ST_UINT id_bytes;
	ST_INT decode_len;
	ST_UINT len_bytes;
	ST_UCHAR c;
	ST_UCHAR constr_elmnt;
	ST_INT datavar = 0;

	//localDetailCalling localDetailCalled
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return -1;
	c = *p;
	constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return -1;
	ilen = id_bytes + len_bytes;

	if(pMmsAcsi->m_Osi_9506.m_osi9506_type == 0xA8)
		strItem = tr("localDetailCalling");
	else
		strItem = tr("localDetailCalled");
	if (DataBase_Info::DataPCharToVal(p + ilen,&datavar,decode_len))
	{
		strItemInfo.sprintf("%d", datavar);
	}
	else
	{
		strItemInfo = tr("");
	}
	strItemLen.sprintf("(%d-%d)",
		ioff + off,
		ioff + off + ilen + decode_len - 1);
	item1 = new QTreeWidgetItem(item);
	item1->setText(0,strItem);
	item1->setText(1,strItemInfo);
	item1->setText(2,strItemLen);
	item->addChild(item1);
	m_items.append(item1);
	p += ilen;
	off += ilen;
	if (!constr_elmnt)
	{
		p += decode_len;
		off += decode_len;
	}

	//proposedMaxServOutstandingCalling negociatedMaxServOutstandingCalling
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return -1;
	c = *p;
	constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return -1;
	ilen = id_bytes + len_bytes;

	if(pMmsAcsi->m_Osi_9506.m_osi9506_type == 0xA8)
		strItem = tr("proposedMaxServOutstandingCalling");
	else
		strItem = tr("negociatedMaxServOutstandingCalling");
	if (DataBase_Info::DataPCharToVal(p + ilen,&datavar,decode_len))
	{
		strItemInfo.sprintf("%d", datavar);
	}
	else
	{
		strItemInfo = tr("");
	}
	strItemLen.sprintf("(%d-%d)",
		ioff + off,
		ioff + off + ilen + decode_len - 1);
	item1 = new QTreeWidgetItem(item);
	item1->setText(0,strItem);
	item1->setText(1,strItemInfo);
	item1->setText(2,strItemLen);
	item->addChild(item1);
	m_items.append(item1);
	p += ilen;
	off += ilen;
	if (!constr_elmnt)
	{
		p += decode_len;
		off += decode_len;
	}

	//proposedMaxServOutstandingCalled negociatedMaxServOutstandingCalled
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return -1;
	c = *p;
	constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return -1;
	ilen = id_bytes + len_bytes;

	if(pMmsAcsi->m_Osi_9506.m_osi9506_type == 0xA8)
		strItem = tr("proposedMaxServOutstandingCalled");
	else
		strItem = tr("negociatedMaxServOutstandingCalled");
	if (DataBase_Info::DataPCharToVal(p + ilen,&datavar,decode_len))
	{
		strItemInfo.sprintf("%d", datavar);
	}
	else
	{
		strItemInfo = tr("");
	}
	strItemLen.sprintf("(%d-%d)",
		ioff + off,
		ioff + off + ilen + decode_len - 1);
	item1 = new QTreeWidgetItem(item);
	item1->setText(0,strItem);
	item1->setText(1,strItemInfo);
	item1->setText(2,strItemLen);
	item->addChild(item1);
	m_items.append(item1);
	p += ilen;
	off += ilen;
	if (!constr_elmnt)
	{
		p += decode_len;
		off += decode_len;
	}

	//proposedDataStructureNestingLevel negociatedDataStructureNestingLevel
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return -1;
	c = *p;
	constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return -1;
	ilen = id_bytes + len_bytes;

	if(pMmsAcsi->m_Osi_9506.m_osi9506_type == 0xA8)
		strItem = tr("proposedDataStructureNestingLevel");
	else
		strItem = tr("negociatedDataStructureNestingLevel");
	if (DataBase_Info::DataPCharToVal(p + ilen,&datavar,decode_len))
	{
		strItemInfo.sprintf("%d", datavar);
	}
	else
	{
		strItemInfo = tr("");
	}
	strItemLen.sprintf("(%d-%d)",
		ioff + off,
		ioff + off + ilen + decode_len - 1);
	item1 = new QTreeWidgetItem(item);
	item1->setText(0,strItem);
	item1->setText(1,strItemInfo);
	item1->setText(2,strItemLen);
	item->addChild(item1);
	m_items.append(item1);
	p += ilen;
	off += ilen;
	if (!constr_elmnt)
	{
		p += decode_len;
		off += decode_len;
	}

	//InitRequestDetail InitRequestDetail
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return -1;
	c = *p;
	constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return -1;
	ilen = id_bytes + len_bytes;

	if(pMmsAcsi->m_Osi_9506.m_osi9506_type == 0xA8)
		strItem = tr("InitRequestDetail");
	else
		strItem = tr("InitResponseDetail");
	if (DataBase_Info::DataPCharToVal(p + ilen,&datavar,decode_len))
	{
		strItemInfo.sprintf("%d", datavar);
	}
	else
	{
		strItemInfo = tr("");
	}
	strItemLen.sprintf("(%d-%d)",
		ioff + off,
		ioff + off + ilen + decode_len - 1);
	item1 = new QTreeWidgetItem(item);
	item1->setText(0,strItem);
	item1->setText(1,strItemInfo);
	item1->setText(2,strItemLen);
	item->addChild(item1);
	m_items.append(item1);
	p += ilen;
	off += ilen;

	//proposedVersionNumber negociatedVersionNumber
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return -1;
	c = *p;
	constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return -1;
	ilen = id_bytes + len_bytes;

	if(pMmsAcsi->m_Osi_9506.m_osi9506_type == 0xA8)
		strItem = tr("proposedVersionNumber");
	else
		strItem = tr("negociatedVersionNumber");
	if (DataBase_Info::DataPCharToVal(p + ilen,&datavar,decode_len))
	{
		strItemInfo.sprintf("%d", datavar);
	}
	else
	{
		strItemInfo = tr("");
	}
	strItemLen.sprintf("(%d-%d)",
		ioff + off,
		ioff + off + ilen + decode_len - 1);
	item2 = new QTreeWidgetItem(item1);
	item2->setText(0,strItem);
	item2->setText(1,strItemInfo);
	item2->setText(2,strItemLen);
	item1->addChild(item2);
	m_items.append(item2);
	p += ilen;
	off += ilen;
	if (!constr_elmnt)
	{
		p += decode_len;
		off += decode_len;
	}

	//proposedParameterCBB negociatedParameterCBB
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return -1;
	c = *p;
	constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return -1;
	ilen = id_bytes + len_bytes;

	if(pMmsAcsi->m_Osi_9506.m_osi9506_type == 0xA8)
		strItem = tr("proposedParameterCBB");
	else
		strItem = tr("negociatedParameterCBB");
	strItemLen.sprintf("(%d-%d)",
		ioff + off,
		ioff + off + ilen + decode_len - 1);
	item2 = new QTreeWidgetItem(item1);
	item2->setText(0,strItem);
	item2->setText(1,strItemInfo);
	item2->setText(2,strItemLen);
	item1->addChild(item2);
	m_items.append(item2);
	if (iexpanded_no < iexpanded_max)
	{
		if (m_ExpandedItemList.at(iexpanded_no) == strItem)
		{
			iexpanded_no ++;
			item2->setExpanded(true);
		}
	}
	p += ilen;
	off += ilen;
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	GetParameterSO_ToTree(item2,p,decode_len,ioff + off - ilen,ioff + off + decode_len - 1);
	if (!constr_elmnt)
	{
		p += decode_len;
		off += decode_len;
	}

	//servicesSupportedCalling servicesSupportedCalled
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return -1;
	c = *p;
	constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return -1;
	ilen = id_bytes + len_bytes;

	if(pMmsAcsi->m_Osi_9506.m_osi9506_type == 0xA8)
		strItem = tr("servicesSupportedCalling");
	else
		strItem = tr("servicesSupportedCalled");
	strItemLen.sprintf("(%d-%d)",
		ioff + off,
		ioff + off + ilen + decode_len - 1);
	item2 = new QTreeWidgetItem(item1);
	item2->setText(0,strItem);
	item2->setText(1,strItemInfo);
	item2->setText(2,strItemLen);
	item1->addChild(item2);
	m_items.append(item2);
	if (iexpanded_no < iexpanded_max)
	{
		if (m_ExpandedItemList.at(iexpanded_no) == strItem)
		{
			iexpanded_no ++;
			item1->setExpanded(true);
		}
	}
	p += ilen;
	off += ilen;
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	GetServiceSO_ToTree(item2,p,decode_len,ioff + off - ilen,ioff + off + decode_len - 1);
	if (!constr_elmnt)
	{
		p += decode_len;
		off += decode_len;
	}
	return 0;
}

int SDecodeMms::Ana_Unconfirmed_Info_Show_Tree(QTreeWidgetItem * pTreeItem, int * expandno, int expandmax, bool bmms2)
{
	int iexpanded_no, iexpanded_max;
	QTreeWidgetItem * item, * item1, * item2, * item3;
	QString strItem, strItemInfo, strItemLen;
	SWF_MmsAcsi * pMmsAcsi;
	int ioff = m_iMMSEthLen;
	pMmsAcsi = &m_database_Acsi;
	if (bmms2)
	{
		ioff = m_iMMSEthLen + m_database_Acsi.m_BufLength - m_database_Acsi.m_iLeavelen;
		pMmsAcsi = &m_database_Acsi_2;
	}

	iexpanded_no = *expandno;
	iexpanded_max = expandmax;
	item = pTreeItem;


	unsigned char * pInfodata = pMmsAcsi->m_Osi_9506.m_InfoData.m_Data.pBYTE;
	if (pInfodata == NULL)
		return 0;

	int off = (int)(pMmsAcsi->m_Osi_9506.m_InfoData.m_Data.pBYTE - pMmsAcsi->m_pBuf);
	unsigned char * p = pInfodata;
	int ilen = 0;
	ST_UINT decode_id;
	ST_UINT id_bytes;
	ST_INT decode_len;
	ST_UINT len_bytes;
	ST_UCHAR c;
	ST_UCHAR constr_elmnt;
	char * str = NULL;

	//InformationReport
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return -1;
	c = *p;
	constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return -1;
	ilen = id_bytes + len_bytes;

	strItem = tr("InformationReport");
	strItemLen.sprintf("(%d-%d)",
		ioff + off,
		ioff + off + ilen + decode_len - 1);
	item1 = new QTreeWidgetItem(item);
	item1->setText(0,strItem);
	item1->setText(2,strItemLen);
	item->addChild(item1);
	m_items.append(item1);
	if (iexpanded_no < iexpanded_max)
	{
		if (m_ExpandedItemList.at(iexpanded_no) == strItem)
		{
			iexpanded_no ++;
			item1->setExpanded(true);
		}
	}
	item1->setExpanded(true);
	p += ilen;
	off += ilen;
	if (!constr_elmnt)
	{
		p += decode_len;
		off += decode_len;
	}

	//VariableList
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return -1;
	c = *p;
	constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return -1;
	ilen = id_bytes + len_bytes;

	strItem = tr("VariableList");
	strItemLen.sprintf("(%d-%d)",
		ioff + off,
		ioff + off + ilen + decode_len - 1);
	item2 = new QTreeWidgetItem(item1);
	item2->setText(0,strItem);
	item2->setText(2,strItemLen);
	item1->addChild(item2);
	m_items.append(item2);
	if (iexpanded_no < iexpanded_max)
	{
		if (m_ExpandedItemList.at(iexpanded_no) == strItem)
		{
			iexpanded_no ++;
			item2->setExpanded(true);
		}
	}
	item2->setExpanded(true);
	p += ilen;
	off += ilen;
	if (!constr_elmnt)
	{
		p += decode_len;
		off += decode_len;
	}

	//Variable Name
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return -1;
	c = *p;
	constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return -1;
	ilen = id_bytes + len_bytes;

	strItem = tr("Variable Name");
	str = new char[decode_len + 1];
	memset(str,0,decode_len + 1);
	strncpy(str,(char *)(p + ilen),decode_len);
	strItemInfo = QString::fromStdString(str);
	delete[] str;
	strItemLen.sprintf("(%d-%d)",
		ioff + off,
		ioff + off + ilen + decode_len - 1);
	item3 = new QTreeWidgetItem(item2);
	item3->setText(0,strItem);
	item3->setText(1,strItemInfo);
	item3->setText(2,strItemLen);
	item2->addChild(item3);
	m_items.append(item3);
	p += ilen;
	off += ilen;
	if (!constr_elmnt)
	{
		p += decode_len;
		off += decode_len;
	}

	//AccessResult
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return -1;
	c = *p;
	constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return -1;
	ilen = id_bytes + len_bytes;

	strItem = tr("AccessResult");
	strItemLen.sprintf("(%d-%d)",
		ioff + off,
		ioff + off + ilen + decode_len - 1);
	item2 = new QTreeWidgetItem(item1);
	item2->setText(0,strItem);
	item2->setText(2,strItemLen);
	item1->addChild(item2);
	m_items.append(item2);
	if (iexpanded_no < iexpanded_max)
	{
		if (m_ExpandedItemList.at(iexpanded_no) == strItem)
		{
			iexpanded_no ++;
			item2->setExpanded(true);
		}
	}
	item2->setExpanded(true);
	p += ilen;
	off += ilen;
	if (!constr_elmnt)
	{
		p += decode_len;
		off += decode_len;
	}

	ilen = GetRptDataToTree(item2,p,off,decode_len, &iexpanded_no, iexpanded_max, bmms2);
	p += ilen;
	off += ilen;

	return 0;
}

void SDecodeMms::Ana_CFRQ_GetNameList_Show_Tree(QTreeWidgetItem * pTreeItem, unsigned char *pData, int ioff, int * expandno, int expandmax, bool bmms2)
{
	int iexpanded_no, iexpanded_max;
	QTreeWidgetItem * item, * item1, * item2;
	QString strItem, strItemInfo, strItemLen;
	SWF_MmsAcsi * pMmsAcsi;
	int ihoff = m_iMMSEthLen;
	pMmsAcsi = &m_database_Acsi;
	if (bmms2)
	{
		ihoff = m_iMMSEthLen + m_database_Acsi.m_BufLength - m_database_Acsi.m_iLeavelen;
		pMmsAcsi = &m_database_Acsi_2;
	}

	iexpanded_no = *expandno;
	iexpanded_max = expandmax;
	item = pTreeItem;


	int off = ioff;
	unsigned char * p = pData;
	int ilen = 0;
	ST_UINT decode_id;
	ST_UINT id_bytes;
	ST_INT decode_len;
	ST_UINT len_bytes;

	//extendedObjectClass
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return;
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return;
	ilen = id_bytes + len_bytes;

	strItem = tr("extendedObjectClass");
	strItemLen.sprintf("(%d-%d)",
		ihoff + off,
		ihoff + off + ilen + decode_len - 1);
	item1 = new QTreeWidgetItem(item);
	item1->setText(0,strItem);
	item1->setText(2,strItemLen);
	item->addChild(item1);
	m_items.append(item1);
	if (iexpanded_no < iexpanded_max)
	{
		if (m_ExpandedItemList.at(iexpanded_no) == strItem)
		{
			iexpanded_no ++;
			item1->setExpanded(true);
		}
	}
	item1->setExpanded(true);
	p += ilen;
	off += ilen;

	//OBJECT Class
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return;
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return;
	ilen = id_bytes + len_bytes;

	strItem = tr("OBJECT Class");
	if (decode_len == 1)
		strItemInfo = GetNameList_Request__ObjectClass.at(*(p + ilen));
	else
		strItemInfo = tr("");
	strItemLen.sprintf("(%d-%d)",
		ihoff + off,
		ihoff + off + ilen + decode_len - 1);
	item2 = new QTreeWidgetItem(item1);
	item2->setText(0,strItem);
	item2->setText(1,strItemInfo);
	item2->setText(2,strItemLen);
	item1->addChild(item2);
	m_items.append(item2);
	p += ilen + decode_len;
	off += ilen + decode_len;

	//objectScope
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return;
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return;
	ilen = id_bytes + len_bytes;

	strItem = tr("objectScope");
	strItemLen.sprintf("(%d-%d)",
		ihoff + off,
		ihoff + off + ilen + decode_len - 1);
	item1 = new QTreeWidgetItem(item);
	item1->setText(0,strItem);
	item1->setText(2,strItemLen);
	item->addChild(item1);
	m_items.append(item1);
	if (iexpanded_no < iexpanded_max)
	{
		if (m_ExpandedItemList.at(iexpanded_no) == strItem)
		{
			iexpanded_no ++;
			item1->setExpanded(true);
		}
	}
	item1->setExpanded(true);
	p += ilen;
	off += ilen;

	//objectScope list
	while(off < pMmsAcsi->m_BufLength)
	{
		decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
		if (decode_id == 0xFFFF) return;
		decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
		if (decode_len == -1) return;
		ilen = id_bytes + len_bytes;

		switch (decode_id)
		{
		case 0x00:
			strItem = tr("VmdSpecific");
			strItemInfo = tr("0");
			break;
		case 0x01:
		default:
			{
				strItem = tr("Item");
				char * str = new char[decode_len + 1];
				memset(str,0,decode_len + 1);
				strncpy(str,(char *)(p + ilen),decode_len);
				strItemInfo = QString::fromStdString(str);
				delete[] str;
				break;
			}
		}
		strItemLen.sprintf("(%d-%d)",
			ihoff + off,
			ihoff + off + ilen + decode_len - 1);
		item2 = new QTreeWidgetItem(item1);
		item2->setText(0,strItem);
		item2->setText(1,strItemInfo);
		item2->setText(2,strItemLen);
		item1->addChild(item2);
		m_items.append(item2);
		p += ilen + decode_len;
		off += ilen + decode_len;
	}
}

void SDecodeMms::Ana_CFRS_GetNameList_Show_Tree(QTreeWidgetItem * pTreeItem, unsigned char *pData, int ioff, int * expandno, int expandmax, bool bmms2)
{
	int iexpanded_no, iexpanded_max;
	QTreeWidgetItem * item, * item1, * item2;
	QString strItem, strItemInfo, strItemLen;
	SWF_MmsAcsi * pMmsAcsi;
	int ihoff = m_iMMSEthLen;
	pMmsAcsi = &m_database_Acsi;
	if (bmms2)
	{
		ihoff = m_iMMSEthLen + m_database_Acsi.m_BufLength - m_database_Acsi.m_iLeavelen;
		pMmsAcsi = &m_database_Acsi_2;
	}

	iexpanded_no = *expandno;
	iexpanded_max = expandmax;
	item = pTreeItem;


	int off = ioff;
	unsigned char * p = pData;
	int ilen = 0;
	ST_UINT decode_id;
	ST_UINT id_bytes;
	ST_INT decode_len;
	ST_UINT len_bytes;
	ST_INT lentmp;

	//List Of Identifier
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return;
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return;
	ilen = id_bytes + len_bytes;

	strItem = tr("List Of Identifier");
	strItemLen.sprintf("(%d-%d)",
		ihoff + off,
		ihoff + off + ilen + decode_len - 1);
	item1 = new QTreeWidgetItem(item);
	item1->setText(0,strItem);
	item1->setText(2,strItemLen);
	item->addChild(item1);
	m_items.append(item1);
	if (iexpanded_no < iexpanded_max)
	{
		if (m_ExpandedItemList.at(iexpanded_no) == strItem)
		{
			iexpanded_no ++;
			item1->setExpanded(true);
		}
	}
	item1->setExpanded(true);
	p += ilen;
	off += ilen;

	lentmp = off + decode_len;
	//List Of Identifier list
	while(off < lentmp)
	{
		decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
		if (decode_id == 0xFFFF) return;
		decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
		if (decode_len == -1) return;
		ilen = id_bytes + len_bytes;

		switch (decode_id)
		{
		case 0x00:
			strItem = tr("Item");
			strItemInfo = tr("0");
			break;
		case 0x1a:
		default:
			{
				strItem = tr("Item");
				char * str = new char[decode_len + 1];
				memset(str,0,decode_len + 1);
				strncpy(str,(char *)(p + ilen),decode_len);
				strItemInfo = QString::fromStdString(str);
				delete[] str;
				break;
			}
		}
		strItemLen.sprintf("(%d-%d)",
			ihoff + off,
			ihoff + off + ilen + decode_len - 1);
		item2 = new QTreeWidgetItem(item1);
		item2->setText(0,strItem);
		item2->setText(1,strItemInfo);
		item2->setText(2,strItemLen);
		item1->addChild(item2);
		m_items.append(item2);
		p += ilen + decode_len;
		off += ilen + decode_len;
	}

	//MoreFollows
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return;
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return;
	ilen = id_bytes + len_bytes;

	strItem = tr("MoreFollows");
	if (decode_len == 1)
	{
		if (*(p + ilen) == 1)
		{
			strItemInfo = tr("TRUE");
		}
		else
		{
			strItemInfo = tr("FALSE");
		}
	}
	else
	{
		strItemInfo = tr("FALSE");
	}
	strItemLen.sprintf("(%d-%d)",
		ihoff + off,
		ihoff + off + ilen + decode_len - 1);
	item1 = new QTreeWidgetItem(item);
	item1->setText(0,strItem);
	item1->setText(1,strItemInfo);
	item1->setText(2,strItemLen);
	item->addChild(item1);
	m_items.append(item1);
	p += ilen;
	off += ilen;
}

void SDecodeMms::Ana_CFRQ_Read_Show_Tree(QTreeWidgetItem * pTreeItem, unsigned char *pData, int ioff, int * expandno, int expandmax, bool bmms2)
{
	int iexpanded_no, iexpanded_max;
	QTreeWidgetItem * item, * item1, * item2, * item3;
	QString strItem, strItemInfo, strItemLen;
	SWF_MmsAcsi * pMmsAcsi;
	int ihoff = m_iMMSEthLen;
	pMmsAcsi = &m_database_Acsi;
	if (bmms2)
	{
		ihoff = m_iMMSEthLen + m_database_Acsi.m_BufLength - m_database_Acsi.m_iLeavelen;
		pMmsAcsi = &m_database_Acsi_2;
	}

	iexpanded_no = *expandno;
	iexpanded_max = expandmax;
	item = pTreeItem;


	int off = ioff;
	unsigned char * p = pData;
	int ilen = 0;
	ST_UINT decode_id;
	ST_UINT id_bytes;
	ST_INT decode_len;
	ST_UINT len_bytes;

	//VariableAccessSpecification
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return;
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return;
	ilen = id_bytes + len_bytes;

	strItem = tr("VariableAccessSpecification");
	strItemLen.sprintf("(%d-%d)",
		ihoff + off,
		ihoff + off + ilen + decode_len - 1);
	item1 = new QTreeWidgetItem(item);
	item1->setText(0,strItem);
	item1->setText(2,strItemLen);
	item->addChild(item1);
	m_items.append(item1);
	if (iexpanded_no < iexpanded_max)
	{
		if (m_ExpandedItemList.at(iexpanded_no) == strItem)
		{
			iexpanded_no ++;
			item1->setExpanded(true);
		}
	}
	item1->setExpanded(true);
	p += ilen;
	off += ilen;

	//listOfVariable
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return;
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return;
	ilen = id_bytes + len_bytes;

	strItem = tr("list Of Variable");
	strItemLen.sprintf("(%d-%d)",
		ihoff + off,
		ihoff + off + ilen + decode_len - 1);
	item2 = new QTreeWidgetItem(item1);
	item2->setText(0,strItem);
	item2->setText(2,strItemLen);
	item1->addChild(item2);
	m_items.append(item2);
	if (iexpanded_no < iexpanded_max)
	{
		if (m_ExpandedItemList.at(iexpanded_no) == strItem)
		{
			iexpanded_no ++;
			item2->setExpanded(true);
		}
	}
	item2->setExpanded(true);
	p += ilen;
	off += ilen;

	//VariableSpecification
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return;
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return;
	ilen = id_bytes + len_bytes;

	strItem = tr("Variable Specification");
	strItemLen.sprintf("(%d-%d)",
		ihoff + off,
		ihoff + off + ilen + decode_len - 1);
	item3 = new QTreeWidgetItem(item2);
	item3->setText(0,strItem);
	item3->setText(2,strItemLen);
	item2->addChild(item3);
	m_items.append(item3);
	if (iexpanded_no < iexpanded_max)
	{
		if (m_ExpandedItemList.at(iexpanded_no) == strItem)
		{
			iexpanded_no ++;
			item3->setExpanded(true);
		}
	}
	item3->setExpanded(true);
	p += ilen;
	off += ilen;

	ilen = GetObjectNameToTree(item3,p,off, &iexpanded_no, iexpanded_max, bmms2);
	p += ilen;
	off += ilen;
}

void SDecodeMms::Ana_CFRS_Read_Show_Tree(QTreeWidgetItem * pTreeItem, unsigned char *pData, int ioff, int * expandno, int expandmax, bool bmms2)
{
	int iexpanded_no, iexpanded_max;
	QTreeWidgetItem * item, * item1;
	QString strItem, strItemInfo, strItemLen;
	SWF_MmsAcsi * pMmsAcsi;
	int ihoff = m_iMMSEthLen;
	pMmsAcsi = &m_database_Acsi;
	if (bmms2)
	{
		ihoff = m_iMMSEthLen + m_database_Acsi.m_BufLength - m_database_Acsi.m_iLeavelen;
		pMmsAcsi = &m_database_Acsi_2;
	}

	iexpanded_no = *expandno;
	iexpanded_max = expandmax;
	item = pTreeItem;


	int off = ioff;
	unsigned char * p = pData;
	int ilen = 0;
	ST_UINT decode_id;
	ST_UINT id_bytes;
	ST_INT decode_len;
	ST_UINT len_bytes;

	//Access Result
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return;
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return;
	ilen = id_bytes + len_bytes;

	strItem = tr("Access Result");
	strItemLen.sprintf("(%d-%d)",
		ihoff + off,
		ihoff + off + ilen + decode_len - 1);
	item1 = new QTreeWidgetItem(item);
	item1->setText(0,strItem);
	item1->setText(2,strItemLen);
	item->addChild(item1);
	m_items.append(item1);
	if (iexpanded_no < iexpanded_max)
	{
		if (m_ExpandedItemList.at(iexpanded_no) == strItem)
		{
			iexpanded_no ++;
			item1->setExpanded(true);
		}
	}
	item1->setExpanded(true);
	p += ilen;
	off += ilen;

	ilen = GetDataToTree(item1,p,off,decode_len, &iexpanded_no, iexpanded_max, bmms2);
	if (ilen == -1)
		return;
	p += ilen;
	off += ilen;
}

void SDecodeMms::Ana_CFRQ_Write_Show_Tree(QTreeWidgetItem * pTreeItem, unsigned char *pData, int ioff, int * expandno, int expandmax, bool bmms2)
{
	int iexpanded_no, iexpanded_max;
	QTreeWidgetItem * item, * item1, * item2, * item3;
	QString strItem, strItemInfo, strItemLen;
	SWF_MmsAcsi * pMmsAcsi;
	int ihoff = m_iMMSEthLen;
	pMmsAcsi = &m_database_Acsi;
	if (bmms2)
	{
		ihoff = m_iMMSEthLen + m_database_Acsi.m_BufLength - m_database_Acsi.m_iLeavelen;
		pMmsAcsi = &m_database_Acsi_2;
	}

	iexpanded_no = *expandno;
	iexpanded_max = expandmax;
	item = pTreeItem;


	int off = ioff;
	unsigned char * p = pData;
	int ilen = 0;
	ST_UINT decode_id;
	ST_UINT id_bytes;
	ST_INT decode_len;
	ST_UINT len_bytes;

	//VariableAccessSpecification
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return;
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return;
	ilen = id_bytes + len_bytes;

	strItem = tr("VariableAccessSpecification");
	strItemLen.sprintf("(%d-%d)",
		ihoff + off,
		ihoff + off + ilen + decode_len - 1);
	item1 = new QTreeWidgetItem(item);
	item1->setText(0,strItem);
	item1->setText(2,strItemLen);
	item->addChild(item1);
	m_items.append(item1);
	if (iexpanded_no < iexpanded_max)
	{
		if (m_ExpandedItemList.at(iexpanded_no) == strItem)
		{
			iexpanded_no ++;
			item1->setExpanded(true);
		}
	}
	item1->setExpanded(true);
	//    p += ilen;
	//    off += ilen;

	//listOfVariable
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return;
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return;
	ilen = id_bytes + len_bytes;

	strItem = tr("list Of Variable");
	strItemLen.sprintf("(%d-%d)",
		ihoff + off,
		ihoff + off + ilen + decode_len - 1);
	item2 = new QTreeWidgetItem(item1);
	item2->setText(0,strItem);
	item2->setText(2,strItemLen);
	item1->addChild(item2);
	m_items.append(item2);
	if (iexpanded_no < iexpanded_max)
	{
		if (m_ExpandedItemList.at(iexpanded_no) == strItem)
		{
			iexpanded_no ++;
			item2->setExpanded(true);
		}
	}
	item2->setExpanded(true);
	p += ilen;
	off += ilen;

	//VariableSpecification
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return;
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return;
	ilen = id_bytes + len_bytes;

	strItem = tr("Variable Specification");
	strItemLen.sprintf("(%d-%d)",
		ihoff + off,
		ihoff + off + ilen + decode_len - 1);
	item3 = new QTreeWidgetItem(item2);
	item3->setText(0,strItem);
	item3->setText(2,strItemLen);
	item2->addChild(item3);
	m_items.append(item3);
	if (iexpanded_no < iexpanded_max)
	{
		if (m_ExpandedItemList.at(iexpanded_no) == strItem)
		{
			iexpanded_no ++;
			item3->setExpanded(true);
		}
	}
	item3->setExpanded(true);
	p += ilen;
	off += ilen;

	ilen = GetObjectNameToTree(item3,p,off, &iexpanded_no, iexpanded_max, bmms2);
	p += ilen;
	off += ilen;

	//listOfData
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return;
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return;
	ilen = id_bytes + len_bytes;

	strItem = tr("list Of Data");
	strItemLen.sprintf("(%d-%d)",
		ihoff + off,
		ihoff + off + ilen + decode_len - 1);
	item1 = new QTreeWidgetItem(item);
	item1->setText(0,strItem);
	item1->setText(2,strItemLen);
	item->addChild(item1);
	m_items.append(item1);
	if (iexpanded_no < iexpanded_max)
	{
		if (m_ExpandedItemList.at(iexpanded_no) == strItem)
		{
			iexpanded_no ++;
			item1->setExpanded(true);
		}
	}
	item1->setExpanded(true);
	p += ilen;
	off += ilen;

	ilen = GetDataToTree(item1,p,off,decode_len, &iexpanded_no, iexpanded_max, bmms2);
	if (ilen == -1)
		return;
	p += ilen;
	off += ilen;
}

void SDecodeMms::Ana_CFRS_Write_Show_Tree(QTreeWidgetItem * pTreeItem, unsigned char *pData, int ioff, int * expandno, int expandmax, bool bmms2)
{
	int iexpanded_no, iexpanded_max;
	QTreeWidgetItem * item, * item1;
	QString strItem, strItemInfo, strItemLen;
	SWF_MmsAcsi * pMmsAcsi;
	int ihoff = m_iMMSEthLen;
	pMmsAcsi = &m_database_Acsi;
	if (bmms2)
	{
		ihoff = m_iMMSEthLen + m_database_Acsi.m_BufLength - m_database_Acsi.m_iLeavelen;
		pMmsAcsi = &m_database_Acsi_2;
	}

	iexpanded_no = *expandno;
	iexpanded_max = expandmax;
	item = pTreeItem;


	int off = ioff;
	unsigned char * p = pData;
	int ilen = 0;
	ST_UINT decode_id;
	ST_UINT id_bytes;
	ST_INT decode_len;
	ST_UINT len_bytes;

	//Data Write
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return;
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return;
	ilen = id_bytes + len_bytes;

	strItem = tr("Data Write");
	if (decode_len == 0)
	{
		strItemInfo = tr("Success");
	}
	else
	{
		ST_INT datavar = 0;
		DataBase_Info::DataPCharToVal((ST_UCHAR *)(p + ilen),&datavar,decode_len);
		if(datavar == 0)
			strItemInfo = tr("Success");
		else
			strItemInfo = tr("Failure");
	}
	strItemLen.sprintf("(%d-%d)",
		ihoff + off,
		ihoff + off + ilen + decode_len - 1);
	item1 = new QTreeWidgetItem(item);
	item1->setText(0,strItem);
	item1->setText(1,strItemInfo);
	item1->setText(2,strItemLen);
	item->addChild(item1);
	m_items.append(item1);
	p += ilen;
	off += ilen;
}

void SDecodeMms::Ana_CFRQ_GetVariableAA_Show_Tree(QTreeWidgetItem * pTreeItem, unsigned char *pData, int ioff, int * expandno, int expandmax, bool bmms2)
{
	int iexpanded_no, iexpanded_max;
	QTreeWidgetItem * item;
	SWF_MmsAcsi * pMmsAcsi;
	int ihoff = m_iMMSEthLen;
	pMmsAcsi = &m_database_Acsi;
	if (bmms2)
	{
		ihoff = m_iMMSEthLen + m_database_Acsi.m_BufLength - m_database_Acsi.m_iLeavelen;
		pMmsAcsi = &m_database_Acsi_2;
	}

	iexpanded_no = *expandno;
	iexpanded_max = expandmax;
	item = pTreeItem;


	int off = ioff;
	unsigned char * p = pData;
	int ilen = 0;

	ilen = GetObjectNameToTree(item,p,off, &iexpanded_no, iexpanded_max, bmms2);
	p += ilen;
	off += ilen;
}

void SDecodeMms::Ana_CFRS_GetVariableAA_Show_Tree(QTreeWidgetItem * pTreeItem, unsigned char *pData, int ioff, int * expandno, int expandmax, bool bmms2)
{
	int iexpanded_no, iexpanded_max;
	QTreeWidgetItem * item, * item1, * item2;
	QString strItem, strItemInfo, strItemLen;
	char strtmp[255];
	SWF_MmsAcsi * pMmsAcsi;
	int ihoff = m_iMMSEthLen;
	pMmsAcsi = &m_database_Acsi;
	if (bmms2)
	{
		ihoff = m_iMMSEthLen + m_database_Acsi.m_BufLength - m_database_Acsi.m_iLeavelen;
		pMmsAcsi = &m_database_Acsi_2;
	}

	iexpanded_no = *expandno;
	iexpanded_max = expandmax;
	item = pTreeItem;


	int off = ioff;
	unsigned char * p = pData;
	int ilen = 0;
	ST_UINT decode_id;
	ST_UINT id_bytes;
	ST_INT decode_len;
	ST_UINT len_bytes;
	ST_UCHAR c;
	ST_UCHAR constr_elmnt;
	ST_INT datavar = 0;

	//mmsDeletable
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return;
	c = *p;
	constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return;
	ilen = id_bytes + len_bytes;

	strItem = tr("mmsDeletable");
	if(DataBase_Info::DataPCharToVal((ST_UCHAR *)(p + ilen),&datavar,decode_len))
	{
		if (datavar == 0)
			strItemInfo = tr("False");
		else
			strItemInfo = tr("True");
	}
	else
		strItemInfo = tr("False");
	strItemLen.sprintf("(%d-%d)",
		ihoff + off,
		ihoff + off + ilen + decode_len - 1);
	item1 = new QTreeWidgetItem(item);
	item1->setText(0,strItem);
	item1->setText(1,strItemInfo);
	item1->setText(2,strItemLen);
	item->addChild(item1);
	m_items.append(item1);
	p += ilen;
	off += ilen;
	if (!constr_elmnt)
	{
		p += decode_len;
		off += decode_len;
	}

	//TypeSpecification
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return;
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return;
	ilen = id_bytes + len_bytes;

	strItem = tr("list Of Variable");
	strItemLen.sprintf("(%d-%d)",
		ihoff + off,
		ihoff + off + ilen + decode_len - 1);
	item1 = new QTreeWidgetItem(item);
	item1->setText(0,strItem);
	item1->setText(2,strItemLen);
	item->addChild(item1);
	m_items.append(item1);
	if (iexpanded_no < iexpanded_max)
	{
		if (m_ExpandedItemList.at(iexpanded_no) == strItem)
		{
			iexpanded_no ++;
			item1->setExpanded(true);
		}
	}
	item1->setExpanded(true);
	p += ilen;
	off += ilen;

	//TypeSpecification_u
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return;
	c = *p;
	constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return;
	ilen = id_bytes + len_bytes;

	switch (c)
	{
	case 0xA0:
		{
			ilen = GetObjectNameToTree(item1,p,off, &iexpanded_no, iexpanded_max, bmms2);
			p += ilen;
			off += ilen;
			break;
		}
	case DataType_Array:
	case DataType_Structure:
		{
			ilen = GetDataToTree(item1,p,off,decode_len, &iexpanded_no, iexpanded_max, bmms2);
			p += ilen;
			off += ilen;
			break;
		}
	case DataType_Boolean:
		{
			strItem = tr("Boolean");
			memset(strtmp,0,255);
			DataBase_Info::GetPByte_DataStr(DataType_Boolean,p + ilen,decode_len,strtmp);
			strItemInfo = QString::fromStdString(strtmp);
			break;
		}
	case DataType_Binary_time:
		{
			strItem = tr("Binary_time");
			memset(strtmp,0,255);
			DataBase_Info::GetPByte_DataStr(DataType_Binary_time,p + ilen,decode_len,strtmp);
			strItemInfo = QString::fromStdString(strtmp);
			break;
		}
	case DataType_Integer:
		{
			strItem = tr("Integer");
			memset(strtmp,0,255);
			DataBase_Info::GetPByte_DataStr(DataType_Integer,p + ilen,decode_len,strtmp);
			strItemInfo = QString::fromStdString(strtmp);
			break;
		}
	case 0x90:
		{
			strItem = tr("UNICODE");
			char * str = new char[decode_len + 1];
			memset(str,0,decode_len + 1);
			strncpy(str,(char *)(p + ilen),decode_len);
			strItemInfo = QString::fromUtf8(str);
			delete[] str;
			//            int dataUns;
			//            if(DataBase_Info::DataPCharToVal((ST_UCHAR *)(p + ilen),(ST_INT *)&dataUns,decode_len))
			//            {
			//                strItemInfo.sprintf("%d",dataUns);
			//            }
			//            else
			//                strItemInfo = tr("");
			break;
		}
	case DataType_Unsigned:
		{
			strItem = tr("Unsigned");
			int dataUns;
			if(DataBase_Info::DataPCharToVal((ST_UCHAR *)(p + ilen),(ST_INT *)&dataUns,decode_len))
			{
				strItemInfo.sprintf("%d",dataUns);
			}
			else
				strItemInfo = tr("");
			break;
		}
	case DataType_UTCTIME:
		{
			strItem = tr("UTC");
			memset(strtmp,0,255);
			DataBase_Info::GetPByte_DataStr(DataType_UTCTIME,p + ilen,decode_len,strtmp);
			strItemInfo = QString::fromStdString(strtmp);
			break;
		}
	default:
		{
			strItem.sprintf("%d",c);
			memset(strtmp,0,255);
			DataBase_Info::GetPByte_DataStr(DataType_Integer,p + ilen,decode_len,strtmp);
			strItemInfo = QString::fromStdString(strtmp);
			break;
		}
	}
	if (!(c == 0xA0 || c == DataType_Structure || c == DataType_Array))
	{
		strItemLen.sprintf("(%d-%d)",
			ihoff + off,
			ihoff + off + ilen + decode_len - 1);
		item2 = new QTreeWidgetItem(item1);
		item2->setText(0,strItem);
		item2->setText(1,strItemInfo);
		item2->setText(2,strItemLen);
		item1->addChild(item2);
		m_items.append(item2);
		p += ilen;
		off += ilen;
		if (!constr_elmnt)
		{
			p += decode_len;
			off += decode_len;
		}
	}
}

void SDecodeMms::Ana_CFRQ_GetNamedVarVLA_Show_Tree(QTreeWidgetItem * pTreeItem, unsigned char *pData, int ioff, int * expandno, int expandmax, bool bmms2)
{
	int iexpanded_no, iexpanded_max;
	QTreeWidgetItem * item, * item1, * item2, * item3;
	QString strItem, strItemInfo, strItemLen;
	SWF_MmsAcsi * pMmsAcsi;
	int ihoff = m_iMMSEthLen;
	pMmsAcsi = &m_database_Acsi;
	if (bmms2)
	{
		ihoff = m_iMMSEthLen + m_database_Acsi.m_BufLength - m_database_Acsi.m_iLeavelen;
		pMmsAcsi = &m_database_Acsi_2;
	}

	iexpanded_no = *expandno;
	iexpanded_max = expandmax;
	item = pTreeItem;


	int off = ioff;
	unsigned char * p = pData;
	int ilen = 0;
	ST_UINT decode_id;
	ST_UINT id_bytes;
	ST_INT decode_len;
	ST_UINT len_bytes;
	ST_UCHAR c;
	ST_UCHAR constr_elmnt;
	char * str = NULL;

	//ObjectName
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return;
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return;
	ilen = id_bytes + len_bytes;

	strItem = tr("ObjectName");
	strItemLen.sprintf("(%d-%d)",
		ihoff + off,
		ihoff + off + ilen + decode_len - 1);
	item1 = new QTreeWidgetItem(item);
	item1->setText(0,strItem);
	item1->setText(2,strItemLen);
	item->addChild(item1);
	m_items.append(item1);
	if (iexpanded_no < iexpanded_max)
	{
		if (m_ExpandedItemList.at(iexpanded_no) == strItem)
		{
			iexpanded_no ++;
			item1->setExpanded(true);
		}
	}
	item1->setExpanded(true);

	//domain_specific
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return;
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return;
	ilen = id_bytes + len_bytes;

	strItem = tr("domain_specific");
	strItemLen.sprintf("(%d-%d)",
		ihoff + off,
		ihoff + off + ilen + decode_len - 1);
	item2 = new QTreeWidgetItem(item1);
	item2->setText(0,strItem);
	item2->setText(2,strItemLen);
	item1->addChild(item2);
	m_items.append(item2);
	if (iexpanded_no < iexpanded_max)
	{
		if (m_ExpandedItemList.at(iexpanded_no) == strItem)
		{
			iexpanded_no ++;
			item2->setExpanded(true);
		}
	}
	item2->setExpanded(true);
	p += ilen;
	off += ilen;

	//domainId
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return;
	c = *p;
	constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return;
	ilen = id_bytes + len_bytes;

	strItem = tr("domainId");
	str = new char[decode_len + 1];
	memset(str,0,decode_len + 1);
	strncpy(str,(char *)(p + ilen),decode_len);
	strItemInfo = QString::fromStdString(str);
	delete[] str;
	strItemLen.sprintf("(%d-%d)",
		ihoff + off,
		ihoff + off + ilen + decode_len - 1);
	item3 = new QTreeWidgetItem(item2);
	item3->setText(0,strItem);
	item3->setText(1,strItemInfo);
	item3->setText(2,strItemLen);
	item2->addChild(item3);
	m_items.append(item3);
	p += ilen;
	off += ilen;
	if (!constr_elmnt)
	{
		p += decode_len;
		off += decode_len;
	}

	//itemId
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return;
	c = *p;
	constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return;
	ilen = id_bytes + len_bytes;

	strItem = tr("itemId");
	str = new char[decode_len + 1];
	memset(str,0,decode_len + 1);
	strncpy(str,(char *)(p + ilen),decode_len);
	strItemInfo = QString::fromStdString(str);
	delete[] str;
	strItemLen.sprintf("(%d-%d)",
		ihoff + off,
		ihoff + off + ilen + decode_len - 1);
	item3 = new QTreeWidgetItem(item2);
	item3->setText(0,strItem);
	item3->setText(1,strItemInfo);
	item3->setText(2,strItemLen);
	item2->addChild(item3);
	m_items.append(item3);
	p += ilen;
	off += ilen;
	if (!constr_elmnt)
	{
		p += decode_len;
		off += decode_len;
	}
}

void SDecodeMms::Ana_CFRS_GetNamedVarVLA_Show_Tree(QTreeWidgetItem * pTreeItem, unsigned char *pData, int ioff, int * expandno, int expandmax, bool bmms2)
{
	int iexpanded_no, iexpanded_max;
	QTreeWidgetItem * item, * item1, * item2;
	QString strItem, strItemInfo, strItemLen;
	SWF_MmsAcsi * pMmsAcsi;
	int ihoff = m_iMMSEthLen;
	pMmsAcsi = &m_database_Acsi;
	if (bmms2)
	{
		ihoff = m_iMMSEthLen + m_database_Acsi.m_BufLength - m_database_Acsi.m_iLeavelen;
		pMmsAcsi = &m_database_Acsi_2;
	}

	iexpanded_no = *expandno;
	iexpanded_max = expandmax;
	item = pTreeItem;


	int off = ioff;
	unsigned char * p = pData;
	int ilen = 0;
	ST_UINT decode_id;
	ST_UINT id_bytes;
	ST_INT decode_len;
	ST_UINT len_bytes;
	ST_UCHAR c;
	ST_UCHAR constr_elmnt;
	ST_INT datavar = 0;
	ST_INT lentmp;

	//mmsDeletable
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return;
	c = *p;
	constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return;
	ilen = id_bytes + len_bytes;

	strItem = tr("mmsDeletable");
	if(DataBase_Info::DataPCharToVal((ST_UCHAR *)(p + ilen),&datavar,decode_len))
	{
		if (datavar == 0)
			strItemInfo = tr("False");
		else
			strItemInfo = tr("True");
	}
	else
		strItemInfo = tr("False");
	strItemLen.sprintf("(%d-%d)",
		ihoff + off,
		ihoff + off + ilen + decode_len - 1);
	item1 = new QTreeWidgetItem(item);
	item1->setText(0,strItem);
	item1->setText(1,strItemInfo);
	item1->setText(2,strItemLen);
	item->addChild(item1);
	m_items.append(item1);
	p += ilen;
	off += ilen;
	if (!constr_elmnt)
	{
		p += decode_len;
		off += decode_len;
	}

	//listOfVariable
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return;
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return;
	ilen = id_bytes + len_bytes;

	strItem = tr("list Of Variable");
	strItemLen.sprintf("(%d-%d)",
		ihoff + off,
		ihoff + off + ilen + decode_len - 1);
	item1 = new QTreeWidgetItem(item);
	item1->setText(0,strItem);
	item1->setText(2,strItemLen);
	item->addChild(item1);
	m_items.append(item1);
	if (iexpanded_no < iexpanded_max)
	{
		if (m_ExpandedItemList.at(iexpanded_no) == strItem)
		{
			iexpanded_no ++;
			item1->setExpanded(true);
		}
	}
	item1->setExpanded(true);
	p += ilen;
	off += ilen;

	lentmp = off + decode_len;
	//List Of Identifier list
	while(off < lentmp)
	{
		//VariableSpecification
		decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
		if (decode_id == 0xFFFF) return;
		decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
		if (decode_len == -1) return;
		ilen = id_bytes + len_bytes;

		strItem = tr("Variable Specification");
		strItemLen.sprintf("(%d-%d)",
			ihoff + off,
			ihoff + off + ilen + decode_len - 1);
		item2 = new QTreeWidgetItem(item1);
		item2->setText(0,strItem);
		item2->setText(2,strItemLen);
		item1->addChild(item2);
		m_items.append(item2);
		if (iexpanded_no < iexpanded_max)
		{
			if (m_ExpandedItemList.at(iexpanded_no) == strItem)
			{
				iexpanded_no ++;
				item2->setExpanded(true);
			}
		}
		item2->setExpanded(true);
		p += ilen;
		off += ilen;

		ilen = GetObjectNameToTree(item2,p,off, &iexpanded_no, iexpanded_max, bmms2);
		p += ilen;
		off += ilen;
	}
}

void SDecodeMms::Ana_CFRQ_FileDir_Show_Tree(QTreeWidgetItem * pTreeItem, unsigned char *pData, int ioff, int * expandno, int expandmax, bool bmms2)
{
	int iexpanded_no, iexpanded_max;
	QTreeWidgetItem * item, * item1, * item2;
	QString strItem, strItemInfo, strItemLen;
	SWF_MmsAcsi * pMmsAcsi;
	int ihoff = m_iMMSEthLen;
	pMmsAcsi = &m_database_Acsi;
	if (bmms2)
	{
		ihoff = m_iMMSEthLen + m_database_Acsi.m_BufLength - m_database_Acsi.m_iLeavelen;
		pMmsAcsi = &m_database_Acsi_2;
	}

	iexpanded_no = *expandno;
	iexpanded_max = expandmax;
	item = pTreeItem;


	int off = ioff;
	unsigned char * p = pData;
	int ilen = 0;
	ST_UINT decode_id;
	ST_UINT id_bytes;
	ST_INT decode_len;
	ST_UINT len_bytes;
	ST_UCHAR c;
	ST_UCHAR constr_elmnt;
	char * str = NULL;

	//fileSpecification
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return;
	c = *p;
	constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return;
	ilen = id_bytes + len_bytes;

	strItem = tr("fileSpecification");
	strItemLen.sprintf("(%d-%d)",
		ihoff + off,
		ihoff + off + ilen + decode_len - 1);
	item1 = new QTreeWidgetItem(item);
	item1->setText(0,strItem);
	item1->setText(2,strItemLen);
	item->addChild(item1);
	m_items.append(item1);
	if (iexpanded_no < iexpanded_max)
	{
		if (m_ExpandedItemList.at(iexpanded_no) == strItem)
		{
			iexpanded_no ++;
			item1->setExpanded(true);
		}
	}
	item1->setExpanded(true);
	p += ilen;
	off += ilen;
	if (!constr_elmnt)
	{
		p += decode_len;
		off += decode_len;
	}

	//FileName GraphicString
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return;
	c = *p;
	constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return;
	ilen = id_bytes + len_bytes;

	strItem = tr("FileDir");
	str = new char[decode_len + 1];
	memset(str,0,decode_len + 1);
	strncpy(str,(char *)(p + ilen),decode_len);
	strItemInfo = QString::fromStdString(str);
	delete[] str;
	strItemLen.sprintf("(%d-%d)",
		ihoff + off,
		ihoff + off + ilen + decode_len - 1);
	item2 = new QTreeWidgetItem(item1);
	item2->setText(0,strItem);
	item2->setText(1,strItemInfo);
	item2->setText(2,strItemLen);
	item1->addChild(item2);
	m_items.append(item2);
	if (iexpanded_no < iexpanded_max)
	{
		if (m_ExpandedItemList.at(iexpanded_no) == strItem)
		{
			iexpanded_no ++;
			item2->setExpanded(true);
		}
	}
	item2->setExpanded(true);
	p += ilen;
	off += ilen;
	if (!constr_elmnt)
	{
		p += decode_len;
		off += decode_len;
	}
}

void SDecodeMms::Ana_CFRS_FileDir_Show_Tree(QTreeWidgetItem * pTreeItem, unsigned char *pData, int ioff, int * expandno, int expandmax, bool bmms2)
{
	int iexpanded_no, iexpanded_max;
	QTreeWidgetItem * item, * item1, * item2, * item3, * item4, * item5;
	QString strItem, strItemInfo, strItemLen;
	SWF_MmsAcsi * pMmsAcsi;
	int ihoff = m_iMMSEthLen;
	pMmsAcsi = &m_database_Acsi;
	if (bmms2)
	{
		ihoff = m_iMMSEthLen + m_database_Acsi.m_BufLength - m_database_Acsi.m_iLeavelen;
		pMmsAcsi = &m_database_Acsi_2;
	}

	iexpanded_no = *expandno;
	iexpanded_max = expandmax;
	item = pTreeItem;


	int off = ioff;
	unsigned char * p = pData;
	int ilen = 0;
	ST_UINT decode_id;
	ST_UINT id_bytes;
	ST_INT decode_len;
	ST_UINT len_bytes;
	ST_UCHAR c;
	ST_UCHAR constr_elmnt;
	ST_INT datavar = 0;
	ST_INT lentmp;
	char * str = NULL;

	//listOfDirectoryEntry
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return;
	c = *p;
	constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return;
	ilen = id_bytes + len_bytes;

	strItem = tr("listOfDirectoryEntry");
	strItemLen.sprintf("(%d-%d)",
		ihoff + off,
		ihoff + off + ilen + decode_len - 1);
	item1 = new QTreeWidgetItem(item);
	item1->setText(0,strItem);
	item1->setText(2,strItemLen);
	item->addChild(item1);
	m_items.append(item1);
	if (iexpanded_no < iexpanded_max)
	{
		if (m_ExpandedItemList.at(iexpanded_no) == strItem)
		{
			iexpanded_no ++;
			item1->setExpanded(true);
		}
	}
	item1->setExpanded(true);
	p += ilen;
	off += ilen;
	if (!constr_elmnt)
	{
		p += decode_len;
		off += decode_len;
	}

	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return;
	c = *p;
	constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return;
	ilen = id_bytes + len_bytes;
	p += ilen;
	off += ilen;
	if (!constr_elmnt)
	{
		p += decode_len;
		off += decode_len;
	}

	int filenum = 0;
	lentmp = off + decode_len;
	//List Of Identifier list
	while(off < lentmp)
	{
		//DirectoryEntry
		decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
		if (decode_id == 0xFFFF) return;
		c = *p;
		constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
		decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
		if (decode_len == -1) return;
		ilen = id_bytes + len_bytes;

		strItem = tr("DirectoryEntry");
		filenum ++;
		strItemInfo.sprintf("%d",filenum);
		strItemLen.sprintf("(%d-%d)",
			ihoff + off,
			ihoff + off + ilen + decode_len - 1);
		item2 = new QTreeWidgetItem(item1);
		item2->setText(0,strItem);
		item2->setText(1,strItemInfo);
		item2->setText(2,strItemLen);
		item1->addChild(item2);
		m_items.append(item2);
		if (iexpanded_no < iexpanded_max)
		{
			if (m_ExpandedItemList.at(iexpanded_no) == strItem)
			{
				iexpanded_no ++;
				item2->setExpanded(true);
			}
		}
		item2->setExpanded(true);
		p += ilen;
		off += ilen;
		if (!constr_elmnt)
		{
			p += decode_len;
			off += decode_len;
		}

		//FileName
		decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
		if (decode_id == 0xFFFF) return;
		c = *p;
		constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
		decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
		if (decode_len == -1) return;
		ilen = id_bytes + len_bytes;

		strItem = tr("File");
		strItemLen.sprintf("(%d-%d)",
			ihoff + off,
			ihoff + off + ilen + decode_len - 1);
		item3 = new QTreeWidgetItem(item2);
		item3->setText(0,strItem);
		item3->setText(2,strItemLen);
		item2->addChild(item3);
		m_items.append(item3);
		if (iexpanded_no < iexpanded_max)
		{
			if (m_ExpandedItemList.at(iexpanded_no) == strItem)
			{
				iexpanded_no ++;
				item3->setExpanded(true);
			}
		}
		item3->setExpanded(true);
		p += ilen;
		off += ilen;
		if (!constr_elmnt)
		{
			p += decode_len;
			off += decode_len;
		}

		//FileName GraphicString
		decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
		if (decode_id == 0xFFFF) return;
		c = *p;
		constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
		decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
		if (decode_len == -1) return;
		ilen = id_bytes + len_bytes;

		strItem = tr("FileName");
		str = new char[decode_len + 1];
		memset(str,0,decode_len + 1);
		strncpy(str,(char *)(p + ilen),decode_len);
		strItemInfo = QString::fromStdString(str);
		delete[] str;
		strItemLen.sprintf("(%d-%d)",
			ihoff + off,
			ihoff + off + ilen + decode_len - 1);
		item4 = new QTreeWidgetItem(item3);
		item4->setText(0,strItem);
		item4->setText(1,strItemInfo);
		item4->setText(2,strItemLen);
		item3->addChild(item4);
		m_items.append(item4);
		p += ilen;
		off += ilen;
		if (!constr_elmnt)
		{
			p += decode_len;
			off += decode_len;
		}

		//FileAttributes
		decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
		if (decode_id == 0xFFFF) return;
		c = *p;
		constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
		decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
		if (decode_len == -1) return;
		ilen = id_bytes + len_bytes;

		strItem = tr("FileAttributes");
		strItemLen.sprintf("(%d-%d)",
			ihoff + off,
			ihoff + off + ilen + decode_len - 1);
		item4 = new QTreeWidgetItem(item3);
		item4->setText(0,strItem);
		item4->setText(2,strItemLen);
		item3->addChild(item4);
		m_items.append(item4);
		if (iexpanded_no < iexpanded_max)
		{
			if (m_ExpandedItemList.at(iexpanded_no) == strItem)
			{
				iexpanded_no ++;
				item4->setExpanded(true);
			}
		}
		item4->setExpanded(true);
		p += ilen;
		off += ilen;
		if (!constr_elmnt)
		{
			p += decode_len;
			off += decode_len;
		}

		//sizeOfFile
		decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
		if (decode_id == 0xFFFF) return;
		c = *p;
		constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
		decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
		if (decode_len == -1) return;
		ilen = id_bytes + len_bytes;

		strItem = tr("sizeOfFile");
		DataBase_Info::DataPCharToVal(p + ilen,&datavar,decode_len);
		strItemInfo.sprintf("%d",datavar);
		strItemLen.sprintf("(%d-%d)",
			ihoff + off,
			ihoff + off + ilen + decode_len - 1);
		item5 = new QTreeWidgetItem(item4);
		item5->setText(0,strItem);
		item5->setText(1,strItemInfo);
		item5->setText(2,strItemLen);
		item4->addChild(item5);
		m_items.append(item5);
		p += ilen;
		off += ilen;
		if (!constr_elmnt)
		{
			p += decode_len;
			off += decode_len;
		}

		//lastModified
		decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
		if (decode_id == 0xFFFF) return;
		c = *p;
		constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
		decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
		if (decode_len == -1) return;
		ilen = id_bytes + len_bytes;

		strItem = tr("lastModified");
		str = new char[decode_len + 1];
		memset(str,0,decode_len + 1);
		strncpy(str,(char *)(p + ilen),decode_len);
		struct tm tmVal;
		char tmstr[5];
		memset(tmstr,0,5);
		strncpy(tmstr,str,4);
		tmVal.tm_year = atoi(tmstr);
		memset(tmstr,0,5);
		strncpy(tmstr,str + 4,2);
		tmVal.tm_mon = atoi(tmstr);
		memset(tmstr,0,5);
		strncpy(tmstr,str + 6,2);
		tmVal.tm_mday = atoi(tmstr);
		memset(tmstr,0,5);
		strncpy(tmstr,str + 8,2);
		tmVal.tm_hour = atoi(tmstr);
		memset(tmstr,0,5);
		strncpy(tmstr,str + 10,2);
		tmVal.tm_min = atoi(tmstr);
		memset(tmstr,0,5);
		strncpy(tmstr,str + 12,2);
		tmVal.tm_sec = atoi(tmstr);
		strItemInfo.sprintf("%04d-%02d-%02d %02d:%02d:%02d",tmVal.tm_year,tmVal.tm_mon,tmVal.tm_mday,tmVal.tm_hour,tmVal.tm_min,tmVal.tm_sec);
		delete[] str;
		strItemLen.sprintf("(%d-%d)",
			ihoff + off,
			ihoff + off + ilen + decode_len - 1);
		item5 = new QTreeWidgetItem(item4);
		item5->setText(0,strItem);
		item5->setText(1,strItemInfo);
		item5->setText(2,strItemLen);
		item4->addChild(item5);
		m_items.append(item5);
		p += ilen;
		off += ilen;
		if (!constr_elmnt)
		{
			p += decode_len;
			off += decode_len;
		}
	}

	if (off < m_database_Acsi.m_BufLength)
	{
		//moreFollows
		decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
		if (decode_id == 0xFFFF) return;
		c = *p;
		constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
		decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
		if (decode_len == -1) return;
		ilen = id_bytes + len_bytes;

		strItem = tr("MoreFollows");
		char strtmp[255];
		memset(strtmp,0,255);
		DataBase_Info::GetPByte_DataStr(DataType_Boolean,p + ilen,decode_len,strtmp);
		strItemInfo = QString::fromStdString(strtmp);
		strItemLen.sprintf("(%d-%d)",
			ihoff + off,
			ihoff + off + ilen + decode_len - 1);
		item1 = new QTreeWidgetItem(item);
		item1->setText(0,strItem);
		item1->setText(1,strItemInfo);
		item1->setText(2,strItemLen);
		item->addChild(item1);
		m_items.append(item1);
		p += ilen;
		off += ilen;
		if (!constr_elmnt)
		{
			p += decode_len;
			off += decode_len;
		}
	}
}

void SDecodeMms::Ana_CFRQ_FileOpen_Show_Tree(QTreeWidgetItem * pTreeItem, unsigned char *pData, int ioff, int * expandno, int expandmax, bool bmms2)
{
	int iexpanded_no, iexpanded_max;
	QTreeWidgetItem * item, * item1, * item2;
	QString strItem, strItemInfo, strItemLen;
	SWF_MmsAcsi * pMmsAcsi;
	int ihoff = m_iMMSEthLen;
	pMmsAcsi = &m_database_Acsi;
	if (bmms2)
	{
		ihoff = m_iMMSEthLen + m_database_Acsi.m_BufLength - m_database_Acsi.m_iLeavelen;
		pMmsAcsi = &m_database_Acsi_2;
	}

	iexpanded_no = *expandno;
	iexpanded_max = expandmax;
	item = pTreeItem;


	int off = ioff;
	unsigned char * p = pData;
	int ilen = 0;
	ST_UINT decode_id;
	ST_UINT id_bytes;
	ST_INT decode_len;
	ST_UINT len_bytes;
	ST_UCHAR c;
	ST_UCHAR constr_elmnt;
	ST_INT datavar = 0;
	char * str = NULL;

	//FileName
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return;
	c = *p;
	constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return;
	ilen = id_bytes + len_bytes;

	strItem = tr("File");
	strItemLen.sprintf("(%d-%d)",
		ihoff + off,
		ihoff + off + ilen + decode_len - 1);
	item1 = new QTreeWidgetItem(item);
	item1->setText(0,strItem);
	item1->setText(2,strItemLen);
	item->addChild(item1);
	m_items.append(item1);
	if (iexpanded_no < iexpanded_max)
	{
		if (m_ExpandedItemList.at(iexpanded_no) == strItem)
		{
			iexpanded_no ++;
			item1->setExpanded(true);
		}
	}
	item1->setExpanded(true);
	p += ilen;
	off += ilen;
	if (!constr_elmnt)
	{
		p += decode_len;
		off += decode_len;
	}

	//FileName GraphicString
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return;
	c = *p;
	constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return;
	ilen = id_bytes + len_bytes;

	strItem = tr("FileName");
	str = new char[decode_len + 1];
	memset(str,0,decode_len + 1);
	strncpy(str,(char *)(p + ilen),decode_len);
	strItemInfo = QString::fromStdString(str);
	delete[] str;
	strItemLen.sprintf("(%d-%d)",
		ihoff + off,
		ihoff + off + ilen + decode_len - 1);
	item2 = new QTreeWidgetItem(item1);
	item2->setText(0,strItem);
	item2->setText(1,strItemInfo);
	item2->setText(2,strItemLen);
	item1->addChild(item2);
	m_items.append(item2);
	p += ilen;
	off += ilen;
	if (!constr_elmnt)
	{
		p += decode_len;
		off += decode_len;
	}

	//initialPosition
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return;
	c = *p;
	constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return;
	ilen = id_bytes + len_bytes;

	strItem = tr("initialPosition");
	DataBase_Info::DataPCharToVal(p +ilen,&datavar,decode_len);
	strItemInfo.sprintf("%d", datavar);
	strItemLen.sprintf("(%d-%d)",
		ihoff + off,
		ihoff + off + ilen + decode_len - 1);
	item2 = new QTreeWidgetItem(item1);
	item2->setText(0,strItem);
	item2->setText(1,strItemInfo);
	item2->setText(2,strItemLen);
	item1->addChild(item2);
	m_items.append(item2);
	p += ilen;
	off += ilen;
	if (!constr_elmnt)
	{
		p += decode_len;
		off += decode_len;
	}
}

void SDecodeMms::Ana_CFRS_FileOpen_Show_Tree(QTreeWidgetItem * pTreeItem, unsigned char *pData, int ioff, int * expandno, int expandmax, bool bmms2)
{
	int iexpanded_no, iexpanded_max;
	QTreeWidgetItem * item, * item1, * item2;
	QString strItem, strItemInfo, strItemLen;
	SWF_MmsAcsi * pMmsAcsi;
	int ihoff = m_iMMSEthLen;
	pMmsAcsi = &m_database_Acsi;
	if (bmms2)
	{
		ihoff = m_iMMSEthLen + m_database_Acsi.m_BufLength - m_database_Acsi.m_iLeavelen;
		pMmsAcsi = &m_database_Acsi_2;
	}

	iexpanded_no = *expandno;
	iexpanded_max = expandmax;
	item = pTreeItem;


	int off = ioff;
	unsigned char * p = pData;
	int ilen = 0;
	ST_UINT decode_id;
	ST_UINT id_bytes;
	ST_INT decode_len;
	ST_UINT len_bytes;
	ST_UCHAR c;
	ST_UCHAR constr_elmnt;
	ST_INT datavar = 0;
	char * str = NULL;

	//frsmID
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return;
	c = *p;
	constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return;
	ilen = id_bytes + len_bytes;

	strItem = tr("frsmID");
	DataBase_Info::DataPCharToVal(p +ilen,&datavar,decode_len);
	strItemInfo.sprintf("%d", datavar);
	strItemLen.sprintf("(%d-%d)",
		ihoff + off,
		ihoff + off + ilen + decode_len - 1);
	item1 = new QTreeWidgetItem(item);
	item1->setText(0,strItem);
	item1->setText(1,strItemInfo);
	item1->setText(2,strItemLen);
	item->addChild(item1);
	m_items.append(item1);
	p += ilen;
	off += ilen;
	if (!constr_elmnt)
	{
		p += decode_len;
		off += decode_len;
	}

	//FileAttributes
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return;
	c = *p;
	constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return;
	ilen = id_bytes + len_bytes;

	strItem = tr("FileAttributes");
	strItemLen.sprintf("(%d-%d)",
		ihoff + off,
		ihoff + off + ilen + decode_len - 1);
	item1 = new QTreeWidgetItem(item);
	item1->setText(0,strItem);
	item1->setText(2,strItemLen);
	item->addChild(item1);
	m_items.append(item1);
	if (iexpanded_no < iexpanded_max)
	{
		if (m_ExpandedItemList.at(iexpanded_no) == strItem)
		{
			iexpanded_no ++;
			item1->setExpanded(true);
		}
	}
	item1->setExpanded(true);
	p += ilen;
	off += ilen;
	if (!constr_elmnt)
	{
		p += decode_len;
		off += decode_len;
	}

	//sizeOfFile
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return;
	c = *p;
	constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return;
	ilen = id_bytes + len_bytes;

	strItem = tr("sizeOfFile");
	DataBase_Info::DataPCharToVal(p + ilen,&datavar,decode_len);
	strItemInfo.sprintf("%d",datavar);
	strItemLen.sprintf("(%d-%d)",
		ihoff + off,
		ihoff + off + ilen + decode_len - 1);
	item2 = new QTreeWidgetItem(item1);
	item2->setText(0,strItem);
	item2->setText(1,strItemInfo);
	item2->setText(2,strItemLen);
	item1->addChild(item2);
	m_items.append(item2);
	p += ilen;
	off += ilen;
	if (!constr_elmnt)
	{
		p += decode_len;
		off += decode_len;
	}

	//lastModified
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return;
	c = *p;
	constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return;
	ilen = id_bytes + len_bytes;

	strItem = tr("lastModified");
	str = new char[decode_len + 1];
	memset(str,0,decode_len + 1);
	strncpy(str,(char *)(p + ilen),decode_len);
	struct tm tmVal;
	char tmstr[5];
	memset(tmstr,0,5);
	strncpy(tmstr,str,4);
	tmVal.tm_year = atoi(tmstr);
	memset(tmstr,0,5);
	strncpy(tmstr,str + 4,2);
	tmVal.tm_mon = atoi(tmstr);
	memset(tmstr,0,5);
	strncpy(tmstr,str + 6,2);
	tmVal.tm_mday = atoi(tmstr);
	memset(tmstr,0,5);
	strncpy(tmstr,str + 8,2);
	tmVal.tm_hour = atoi(tmstr);
	memset(tmstr,0,5);
	strncpy(tmstr,str + 10,2);
	tmVal.tm_min = atoi(tmstr);
	memset(tmstr,0,5);
	strncpy(tmstr,str + 12,2);
	tmVal.tm_sec = atoi(tmstr);
	strItemInfo.sprintf("%04d-%02d-%02d %02d:%02d:%02d",tmVal.tm_year,tmVal.tm_mon,tmVal.tm_mday,tmVal.tm_hour,tmVal.tm_min,tmVal.tm_sec);
	delete[] str;
	strItemLen.sprintf("(%d-%d)",
		ihoff + off,
		ihoff + off + ilen + decode_len - 1);
	item2 = new QTreeWidgetItem(item1);
	item2->setText(0,strItem);
	item2->setText(1,strItemInfo);
	item2->setText(2,strItemLen);
	item1->addChild(item2);
	m_items.append(item2);
	p += ilen;
	off += ilen;
	if (!constr_elmnt)
	{
		p += decode_len;
		off += decode_len;
	}
}

void SDecodeMms::Ana_CFRQ_FileRead_Show_Tree(QTreeWidgetItem * pTreeItem, unsigned char *pData, int ioff, int * expandno, int expandmax, bool bmms2)
{
	int iexpanded_no, iexpanded_max;
	QTreeWidgetItem * item, * item1;
	QString strItem, strItemInfo, strItemLen;
	SWF_MmsAcsi * pMmsAcsi;
	int ihoff = m_iMMSEthLen;
	pMmsAcsi = &m_database_Acsi;
	if (bmms2)
	{
		ihoff = m_iMMSEthLen + m_database_Acsi.m_BufLength - m_database_Acsi.m_iLeavelen;
		pMmsAcsi = &m_database_Acsi_2;
	}

	iexpanded_no = *expandno;
	iexpanded_max = expandmax;
	item = pTreeItem;


	int off = ioff;
	unsigned char * p = pData;
	int ilen = 0;
	ST_UINT decode_id;
	ST_UINT id_bytes;
	ST_INT decode_len;
	ST_UINT len_bytes;
	ST_UCHAR c;
	ST_UCHAR constr_elmnt;
	ST_INT datavar = 0;

	//FileRead
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return;
	c = *p;
	constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return;
	ilen = id_bytes + len_bytes;

	strItem = tr("FileRead");
	DataBase_Info::DataPCharToVal(p +ilen,&datavar,decode_len);
	strItemInfo.sprintf("%d", datavar);
	strItemLen.sprintf("(%d-%d)",
		ihoff + off,
		ihoff + off + ilen + decode_len - 1);
	item1 = new QTreeWidgetItem(item);
	item1->setText(0,strItem);
	item1->setText(1,strItemInfo);
	item1->setText(2,strItemLen);
	item->addChild(item1);
	m_items.append(item1);
	p += ilen;
	off += ilen;
	if (!constr_elmnt)
	{
		p += decode_len;
		off += decode_len;
	}
}

void SDecodeMms::Ana_CFRS_FileRead_Show_Tree(QTreeWidgetItem * pTreeItem, unsigned char *pData, int ioff, int * expandno, int expandmax, bool bmms2)
{
	int iexpanded_no, iexpanded_max;
	QTreeWidgetItem * item, * item1;
	QString strItem, strItemInfo, strItemLen;
	SWF_MmsAcsi * pMmsAcsi;
	int ihoff = m_iMMSEthLen;
	pMmsAcsi = &m_database_Acsi;
	if (bmms2)
	{
		ihoff = m_iMMSEthLen + m_database_Acsi.m_BufLength - m_database_Acsi.m_iLeavelen;
		pMmsAcsi = &m_database_Acsi_2;
	}

	iexpanded_no = *expandno;
	iexpanded_max = expandmax;
	item = pTreeItem;


	int off = ioff;
	unsigned char * p = pData;
	int ilen = 0;
	ST_UINT decode_id;
	ST_UINT id_bytes;
	ST_INT decode_len;
	ST_UINT len_bytes;
	ST_UCHAR c;
	ST_UCHAR constr_elmnt;

	//FileRead Data
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return;
	c = *p;
	constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return;
	ilen = id_bytes + len_bytes;

	strItem = tr("File Data");
	strItemInfo.sprintf("%d", decode_len);
	strItemLen.sprintf("(%d-%d)",
		ihoff + off,
		ihoff + off + ilen + decode_len - 1);
	item1 = new QTreeWidgetItem(item);
	item1->setText(0,strItem);
	item1->setText(1,strItemInfo);
	item1->setText(2,strItemLen);
	item->addChild(item1);
	m_items.append(item1);
	p += ilen;
	off += ilen;
	if (!constr_elmnt)
	{
		p += decode_len;
		off += decode_len;
	}

	if (off < pMmsAcsi->m_BufLength)
	{
		//moreFollows
		decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
		if (decode_id == 0xFFFF) return;
		c = *p;
		constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
		decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
		if (decode_len == -1) return;
		ilen = id_bytes + len_bytes;

		strItem = tr("MoreFollows");
		char strtmp[255];
		memset(strtmp,0,255);
		DataBase_Info::GetPByte_DataStr(DataType_Boolean,p + ilen,decode_len,strtmp);
		strItemInfo = QString::fromStdString(strtmp);
		strItemLen.sprintf("(%d-%d)",
			ihoff + off,
			ihoff + off + ilen + decode_len - 1);
		item1 = new QTreeWidgetItem(item);
		item1->setText(0,strItem);
		item1->setText(1,strItemInfo);
		item1->setText(2,strItemLen);
		item->addChild(item1);
		m_items.append(item1);
		p += ilen;
		off += ilen;
		if (!constr_elmnt)
		{
			p += decode_len;
			off += decode_len;
		}
	}
}

void SDecodeMms::Ana_CFRQ_FileClose_Show_Tree(QTreeWidgetItem * pTreeItem, unsigned char *pData, int ioff, int * expandno, int expandmax, bool bmms2)
{
	int iexpanded_no, iexpanded_max;
	QTreeWidgetItem * item, * item1;
	QString strItem, strItemInfo, strItemLen;
	SWF_MmsAcsi * pMmsAcsi;
	int ihoff = m_iMMSEthLen;
	pMmsAcsi = &m_database_Acsi;
	if (bmms2)
	{
		ihoff = m_iMMSEthLen + m_database_Acsi.m_BufLength - m_database_Acsi.m_iLeavelen;
		pMmsAcsi = &m_database_Acsi_2;
	}

	iexpanded_no = *expandno;
	iexpanded_max = expandmax;
	item = pTreeItem;


	int off = ioff;
	unsigned char * p = pData;
	int ilen = 0;
	ST_UINT decode_id;
	ST_UINT id_bytes;
	ST_INT decode_len;
	ST_UINT len_bytes;
	ST_UCHAR c;
	ST_UCHAR constr_elmnt;
	ST_INT datavar = 0;

	//FileClose
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return;
	c = *p;
	constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return;
	ilen = id_bytes + len_bytes;

	strItem = tr("FileClose");
	DataBase_Info::DataPCharToVal(p +ilen,&datavar,decode_len);
	strItemInfo.sprintf("%d", datavar);
	strItemLen.sprintf("(%d-%d)",
		ihoff + off,
		ihoff + off + ilen + decode_len - 1);
	item1 = new QTreeWidgetItem(item);
	item1->setText(0,strItem);
	item1->setText(1,strItemInfo);
	item1->setText(2,strItemLen);
	item->addChild(item1);
	m_items.append(item1);
	p += ilen;
	off += ilen;
	if (!constr_elmnt)
	{
		p += decode_len;
		off += decode_len;
	}
}

void SDecodeMms::Ana_CFRQ_FileRename_Show_Tree(QTreeWidgetItem * pTreeItem, unsigned char *pData, int ioff, int * expandno, int expandmax, bool bmms2)
{
	int iexpanded_no, iexpanded_max;
	QTreeWidgetItem * item, * item1, * item2;
	QString strItem, strItemInfo, strItemLen;
	SWF_MmsAcsi * pMmsAcsi;
	int ihoff = m_iMMSEthLen;
	pMmsAcsi = &m_database_Acsi;
	if (bmms2)
	{
		ihoff = m_iMMSEthLen + m_database_Acsi.m_BufLength - m_database_Acsi.m_iLeavelen;
		pMmsAcsi = &m_database_Acsi_2;
	}

	iexpanded_no = *expandno;
	iexpanded_max = expandmax;
	item = pTreeItem;


	int off = ioff;
	unsigned char * p = pData;
	int ilen = 0;
	ST_UINT decode_id;
	ST_UINT id_bytes;
	ST_INT decode_len;
	ST_UINT len_bytes;
	ST_UCHAR c;
	ST_UCHAR constr_elmnt;
	char * str = NULL;

	//currentFileName
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return;
	c = *p;
	constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return;
	ilen = id_bytes + len_bytes;

	strItem = tr("currentFileName");
	strItemLen.sprintf("(%d-%d)",
		ihoff + off,
		ihoff + off + ilen + decode_len - 1);
	item1 = new QTreeWidgetItem(item);
	item1->setText(0,strItem);
	item1->setText(2,strItemLen);
	item->addChild(item1);
	m_items.append(item1);
	if (iexpanded_no < iexpanded_max)
	{
		if (m_ExpandedItemList.at(iexpanded_no) == strItem)
		{
			iexpanded_no ++;
			item1->setExpanded(true);
		}
	}
	item1->setExpanded(true);
	p += ilen;
	off += ilen;
	if (!constr_elmnt)
	{
		p += decode_len;
		off += decode_len;
	}

	//currentFileName filename
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return;
	c = *p;
	constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return;
	ilen = id_bytes + len_bytes;

	strItem = tr("FileName");
	str = new char[decode_len + 1];
	memset(str,0,decode_len + 1);
	strncpy(str,(char *)(p + ilen),decode_len);
	strItemInfo = QString::fromStdString(str);
	delete[] str;
	strItemLen.sprintf("(%d-%d)",
		ihoff + off,
		ihoff + off + ilen + decode_len - 1);
	item2 = new QTreeWidgetItem(item1);
	item2->setText(0,strItem);
	item2->setText(1,strItemInfo);
	item2->setText(2,strItemLen);
	item1->addChild(item2);
	m_items.append(item2);
	p += ilen;
	off += ilen;
	if (!constr_elmnt)
	{
		p += decode_len;
		off += decode_len;
	}

	//newFileName
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return;
	c = *p;
	constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return;
	ilen = id_bytes + len_bytes;

	strItem = tr("newFileName");
	strItemLen.sprintf("(%d-%d)",
		ihoff + off,
		ihoff + off + ilen + decode_len - 1);
	item1 = new QTreeWidgetItem(item);
	item1->setText(0,strItem);
	item1->setText(2,strItemLen);
	item->addChild(item1);
	m_items.append(item1);
	if (iexpanded_no < iexpanded_max)
	{
		if (m_ExpandedItemList.at(iexpanded_no) == strItem)
		{
			iexpanded_no ++;
			item1->setExpanded(true);
		}
	}
	item1->setExpanded(true);
	p += ilen;
	off += ilen;
	if (!constr_elmnt)
	{
		p += decode_len;
		off += decode_len;
	}

	//newFileName filename
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return;
	c = *p;
	constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return;
	ilen = id_bytes + len_bytes;

	strItem = tr("FileName");
	str = new char[decode_len + 1];
	memset(str,0,decode_len + 1);
	strncpy(str,(char *)(p + ilen),decode_len);
	strItemInfo = QString::fromStdString(str);
	delete[] str;
	strItemLen.sprintf("(%d-%d)",
		ihoff + off,
		ihoff + off + ilen + decode_len - 1);
	item2 = new QTreeWidgetItem(item1);
	item2->setText(0,strItem);
	item2->setText(1,strItemInfo);
	item2->setText(2,strItemLen);
	item1->addChild(item2);
	m_items.append(item2);
	p += ilen;
	off += ilen;
	if (!constr_elmnt)
	{
		p += decode_len;
		off += decode_len;
	}
}

void SDecodeMms::Ana_CFRQ_FileDelete_Show_Tree(QTreeWidgetItem * pTreeItem, unsigned char *pData, int ioff, int * expandno, int expandmax, bool bmms2)
{
	int iexpanded_no, iexpanded_max;
	QTreeWidgetItem * item, * item1;
	QString strItem, strItemInfo, strItemLen;
	SWF_MmsAcsi * pMmsAcsi;
	int ihoff = m_iMMSEthLen;
	pMmsAcsi = &m_database_Acsi;
	if (bmms2)
	{
		ihoff = m_iMMSEthLen + m_database_Acsi.m_BufLength - m_database_Acsi.m_iLeavelen;
		pMmsAcsi = &m_database_Acsi_2;
	}

	iexpanded_no = *expandno;
	iexpanded_max = expandmax;
	item = pTreeItem;


	int off = ioff;
	unsigned char * p = pData;
	int ilen = 0;
	ST_UINT decode_id;
	ST_UINT id_bytes;
	ST_INT decode_len;
	ST_UINT len_bytes;
	ST_UCHAR c;
	ST_UCHAR constr_elmnt;
	char * str = NULL;

	//FileDelete FileName
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return;
	c = *p;
	constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return;
	ilen = id_bytes + len_bytes;

	strItem = tr("Delete FileName");
	str = new char[decode_len + 1];
	memset(str,0,decode_len + 1);
	strncpy(str,(char *)(p + ilen),decode_len);
	strItemInfo = QString::fromStdString(str);
	delete[] str;
	strItemLen.sprintf("(%d-%d)",
		ihoff + off,
		ihoff + off + ilen + decode_len - 1);
	item1 = new QTreeWidgetItem(item);
	item1->setText(0,strItem);
	item1->setText(1,strItemInfo);
	item1->setText(2,strItemLen);
	item->addChild(item1);
	m_items.append(item1);
	p += ilen;
	off += ilen;
	if (!constr_elmnt)
	{
		p += decode_len;
		off += decode_len;
	}
}

void SDecodeMms::Ana_CFRS_Identify_Show_Tree(QTreeWidgetItem * pTreeItem, unsigned char *pData, int ioff, int * expandno, int expandmax, bool bmms2)
{
	int iexpanded_no, iexpanded_max;
	QTreeWidgetItem * item, * item1;
	QString strItem, strItemInfo, strItemLen;
	SWF_MmsAcsi * pMmsAcsi;
	int ihoff = m_iMMSEthLen;
	pMmsAcsi = &m_database_Acsi;
	if (bmms2)
	{
		ihoff = m_iMMSEthLen + m_database_Acsi.m_BufLength - m_database_Acsi.m_iLeavelen;
		pMmsAcsi = &m_database_Acsi_2;
	}

	iexpanded_no = *expandno;
	iexpanded_max = expandmax;
	item = pTreeItem;


	int off = ioff;
	unsigned char * p = pData;
	int ilen = 0;
	ST_UINT decode_id;
	ST_UINT id_bytes;
	ST_INT decode_len;
	ST_UINT len_bytes;
	ST_UCHAR c;
	ST_UCHAR constr_elmnt;
	char * str = NULL;

	//vendorName
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return;
	c = *p;
	constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return;
	ilen = id_bytes + len_bytes;

	strItem = tr("vendorName");
	str = new char[decode_len + 1];
	memset(str,0,decode_len + 1);
	strncpy(str,(char *)(p + ilen),decode_len);
	strItemInfo = QString::fromStdString(str);
	delete[] str;
	strItemLen.sprintf("(%d-%d)",
		ihoff + off,
		ihoff + off + ilen + decode_len - 1);
	item1 = new QTreeWidgetItem(item);
	item1->setText(0,strItem);
	item1->setText(1,strItemInfo);
	item1->setText(2,strItemLen);
	item->addChild(item1);
	m_items.append(item1);
	p += ilen;
	off += ilen;
	if (!constr_elmnt)
	{
		p += decode_len;
		off += decode_len;
	}

	//modelName
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return;
	c = *p;
	constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return;
	ilen = id_bytes + len_bytes;

	strItem = tr("modelName");
	str = new char[decode_len + 1];
	memset(str,0,decode_len + 1);
	strncpy(str,(char *)(p + ilen),decode_len);
	strItemInfo = QString::fromStdString(str);
	delete[] str;
	strItemLen.sprintf("(%d-%d)",
		ihoff + off,
		ihoff + off + ilen + decode_len - 1);
	item1 = new QTreeWidgetItem(item);
	item1->setText(0,strItem);
	item1->setText(1,strItemInfo);
	item1->setText(2,strItemLen);
	item->addChild(item1);
	m_items.append(item1);
	p += ilen;
	off += ilen;
	if (!constr_elmnt)
	{
		p += decode_len;
		off += decode_len;
	}

	//revision
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return;
	c = *p;
	constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return;
	ilen = id_bytes + len_bytes;

	strItem = tr("revision");
	str = new char[decode_len + 1];
	memset(str,0,decode_len + 1);
	strncpy(str,(char *)(p + ilen),decode_len);
	strItemInfo = QString::fromStdString(str);
	delete[] str;
	strItemLen.sprintf("(%d-%d)",
		ihoff + off,
		ihoff + off + ilen + decode_len - 1);
	item1 = new QTreeWidgetItem(item);
	item1->setText(0,strItem);
	item1->setText(1,strItemInfo);
	item1->setText(2,strItemLen);
	item->addChild(item1);
	m_items.append(item1);
	p += ilen;
	off += ilen;
	if (!constr_elmnt)
	{
		p += decode_len;
		off += decode_len;
	}
}

void SDecodeMms::Ana_CFRQ_Status_Show_Tree(QTreeWidgetItem * pTreeItem, unsigned char *pData, int ioff, int * expandno, int expandmax, bool bmms2)
{
	int iexpanded_no, iexpanded_max;
	QTreeWidgetItem * item, * item1;
	QString strItem, strItemInfo, strItemLen;
	char strtmp[255];
	SWF_MmsAcsi * pMmsAcsi;
	int ihoff = m_iMMSEthLen;
	pMmsAcsi = &m_database_Acsi;
	if (bmms2)
	{
		ihoff = m_iMMSEthLen + m_database_Acsi.m_BufLength - m_database_Acsi.m_iLeavelen;
		pMmsAcsi = &m_database_Acsi_2;
	}

	iexpanded_no = *expandno;
	iexpanded_max = expandmax;
	item = pTreeItem;


	int off = ioff;
	unsigned char * p = pData;

	strItem = tr("Boolean");
	memset(strtmp,0,255);
	DataBase_Info::GetPByte_DataStr(DataType_Boolean,p,1,strtmp);
	strItemInfo = QString::fromStdString(strtmp);
	strItemLen.sprintf("(%d-%d)",
		ihoff + off,
		ihoff + off + 1 - 1);
	item1 = new QTreeWidgetItem(item);
	item1->setText(0,strItem);
	item1->setText(1,strItemInfo);
	item1->setText(2,strItemLen);
	item->addChild(item1);
	m_items.append(item1);
	p += 1;
	off += 1;

}

void SDecodeMms::Ana_CFRS_Status_Show_Tree(QTreeWidgetItem * pTreeItem, unsigned char *pData, int ioff, int * expandno, int expandmax, bool bmms2)
{
	int iexpanded_no, iexpanded_max;
	QTreeWidgetItem * item, * item1;
	QString strItem, strItemInfo, strItemLen;
	SWF_MmsAcsi * pMmsAcsi;
	int ihoff = m_iMMSEthLen;
	pMmsAcsi = &m_database_Acsi;
	if (bmms2)
	{
		ihoff = m_iMMSEthLen + m_database_Acsi.m_BufLength - m_database_Acsi.m_iLeavelen;
		pMmsAcsi = &m_database_Acsi_2;
	}

	iexpanded_no = *expandno;
	iexpanded_max = expandmax;
	item = pTreeItem;


	int off = ioff;
	unsigned char * p = pData;
	int ilen = 0;
	ST_UINT decode_id;
	ST_UINT id_bytes;
	ST_INT decode_len;
	ST_UINT len_bytes;
	ST_UCHAR c;
	ST_UCHAR constr_elmnt;
	ST_INT datavar = 0;

	//vmdLogicalStatus
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return;
	c = *p;
	constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return;
	ilen = id_bytes + len_bytes;

	strItem = tr("vmdLogicalStatus");
	DataBase_Info::DataPCharToVal(p + ilen,&datavar,decode_len);
	strItemInfo.sprintf("%d", datavar);
	strItemLen.sprintf("(%d-%d)",
		ihoff + off,
		ihoff + off + ilen + decode_len - 1);
	item1 = new QTreeWidgetItem(item);
	item1->setText(0,strItem);
	item1->setText(1,strItemInfo);
	item1->setText(2,strItemLen);
	item->addChild(item1);
	m_items.append(item1);
	p += ilen;
	off += ilen;
	if (!constr_elmnt)
	{
		p += decode_len;
		off += decode_len;
	}

	//vmdPhysicalStatus
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return;
	c = *p;
	constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return;
	ilen = id_bytes + len_bytes;

	strItem = tr("vmdPhysicalStatus");
	DataBase_Info::DataPCharToVal(p + ilen,&datavar,decode_len);
	strItemInfo.sprintf("%d", datavar);
	strItemLen.sprintf("(%d-%d)",
		ihoff + off,
		ihoff + off + ilen + decode_len - 1);
	item1 = new QTreeWidgetItem(item);
	item1->setText(0,strItem);
	item1->setText(1,strItemInfo);
	item1->setText(2,strItemLen);
	item->addChild(item1);
	m_items.append(item1);
	p += ilen;
	off += ilen;
	if (!constr_elmnt)
	{
		p += decode_len;
		off += decode_len;
	}
}

int SDecodeMms::GetObjectNameToTree(QTreeWidgetItem * pTreeItem, unsigned char *pData, int ioff, int * expandno, int expandmax, bool bmms2)
{
	int iexpanded_no, iexpanded_max;
	QTreeWidgetItem * item, * item1, * item2, * item3;
	QString strItem, strItemInfo, strItemLen;
	SWF_MmsAcsi * pMmsAcsi;
	int ihoff = m_iMMSEthLen;
	pMmsAcsi = &m_database_Acsi;
	if (bmms2)
	{
		ihoff = m_iMMSEthLen + m_database_Acsi.m_BufLength - m_database_Acsi.m_iLeavelen;
		pMmsAcsi = &m_database_Acsi_2;
	}

	iexpanded_no = *expandno;
	iexpanded_max = expandmax;
	item = pTreeItem;


	int off = ioff;
	unsigned char * p = pData;
	int ilen = 0;
	ST_UINT decode_id;
	ST_UINT id_bytes;
	ST_INT decode_len;
	ST_UINT len_bytes;
	ST_UCHAR c;
	ST_UCHAR constr_elmnt;
	char * str = NULL;

	//ObjectName
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return (off - ioff);
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return (off - ioff);
	ilen = id_bytes + len_bytes;

	strItem = tr("ObjectName");
	strItemLen.sprintf("(%d-%d)",
		ihoff + off,
		ihoff + off + ilen + decode_len - 1);
	item1 = new QTreeWidgetItem(item);
	item1->setText(0,strItem);
	item1->setText(2,strItemLen);
	item->addChild(item1);
	m_items.append(item1);
	if (iexpanded_no < iexpanded_max)
	{
		if (m_ExpandedItemList.at(iexpanded_no) == strItem)
		{
			iexpanded_no ++;
			item1->setExpanded(true);
		}
	}
	item1->setExpanded(true);
	p += ilen;
	off += ilen;

	//domain_specific
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return (off - ioff);
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return (off - ioff);
	ilen = id_bytes + len_bytes;

	strItem = tr("domain_specific");
	strItemLen.sprintf("(%d-%d)",
		ihoff + off,
		ihoff + off + ilen + decode_len - 1);
	item2 = new QTreeWidgetItem(item1);
	item2->setText(0,strItem);
	item2->setText(2,strItemLen);
	item1->addChild(item2);
	m_items.append(item2);
	if (iexpanded_no < iexpanded_max)
	{
		if (m_ExpandedItemList.at(iexpanded_no) == strItem)
		{
			iexpanded_no ++;
			item2->setExpanded(true);
		}
	}
	item2->setExpanded(true);
	p += ilen;
	off += ilen;

	//domainId
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return (off - ioff);
	c = *p;
	constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return (off - ioff);
	ilen = id_bytes + len_bytes;

	strItem = tr("domainId");
	str = new char[decode_len + 1];
	memset(str,0,decode_len + 1);
	strncpy(str,(char *)(p + ilen),decode_len);
	strItemInfo = QString::fromStdString(str);
	delete[] str;
	strItemLen.sprintf("(%d-%d)",
		ihoff + off,
		ihoff + off + ilen + decode_len - 1);
	item3 = new QTreeWidgetItem(item2);
	item3->setText(0,strItem);
	item3->setText(1,strItemInfo);
	item3->setText(2,strItemLen);
	item2->addChild(item3);
	m_items.append(item3);
	p += ilen;
	off += ilen;
	if (!constr_elmnt)
	{
		p += decode_len;
		off += decode_len;
	}

	//itemId
	decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
	if (decode_id == 0xFFFF) return (off - ioff);
	c = *p;
	constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
	decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
	if (decode_len == -1) return (off - ioff);
	ilen = id_bytes + len_bytes;

	strItem = tr("itemId");
	str = new char[decode_len + 1];
	memset(str,0,decode_len + 1);
	strncpy(str,(char *)(p + ilen),decode_len);
	strItemInfo = QString::fromStdString(str);
	delete[] str;
	strItemLen.sprintf("(%d-%d)",
		ihoff + off,
		ihoff + off + ilen + decode_len - 1);
	item3 = new QTreeWidgetItem(item2);
	item3->setText(0,strItem);
	item3->setText(1,strItemInfo);
	item3->setText(2,strItemLen);
	item2->addChild(item3);
	m_items.append(item3);
	p += ilen;
	off += ilen;
	if (!constr_elmnt)
	{
		p += decode_len;
		off += decode_len;
	}

	return (off - ioff);
}

int SDecodeMms::GetDataToTree(QTreeWidgetItem * pTreeItem, unsigned char *pData, int ioff, int leftlen, int * expandno, int expandmax, bool bmms2)
{
	int iexpanded_no, iexpanded_max;
	QTreeWidgetItem * item, * item1;
	QString strItem, strItemInfo, strItemLen;
	char strtmp[255];
	SWF_MmsAcsi * pMmsAcsi;
	int ihoff = m_iMMSEthLen;
	pMmsAcsi = &m_database_Acsi;
	if (bmms2)
	{
		ihoff = m_iMMSEthLen + m_database_Acsi.m_BufLength - m_database_Acsi.m_iLeavelen;
		pMmsAcsi = &m_database_Acsi_2;
	}

	iexpanded_no = *expandno;
	iexpanded_max = expandmax;
	item = pTreeItem;


	int off = ioff;
	unsigned char * p = pData;
	int ilen = 0;
	ST_UINT decode_id;
	ST_UINT id_bytes;
	ST_INT decode_len;
	ST_UINT len_bytes;
	ST_INT lentmp;
	ST_UCHAR c;
	ST_UCHAR constr_elmnt;
	char * str = NULL;

	//Data
	lentmp = off + leftlen;
	while(off < lentmp)
	{
		decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
		if (decode_id == 0xFFFF) return -1;
		c = *p;
		constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
		decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
		if (decode_len == -1) return -1;
		ilen = id_bytes + len_bytes;

		str = new char[decode_len + 1];
		memset(str,0,decode_len + 1);
		switch (c)
		{
		case 0x30:
			{
				strItem = tr("Sequence");
				delete[] str;
				strItemLen.sprintf("(%d-%d)",
					ihoff + off,
					ihoff + off + ilen + decode_len - 1);
				item1 = new QTreeWidgetItem(item);
				item1->setText(0,strItem);
				item1->setText(2,strItemLen);
				item->addChild(item1);
				m_items.append(item1);
				if (iexpanded_no < iexpanded_max)
				{
					if (m_ExpandedItemList.at(iexpanded_no) == strItem)
					{
						iexpanded_no ++;
						item1->setExpanded(true);
					}
				}
				item1->setExpanded(true);
				p += ilen;
				off += ilen;

				ilen = GetDataToTree(item1,p,off,decode_len, &iexpanded_no, iexpanded_max, bmms2);
				if (ilen == -1)
				{
					return - 1;
				}
				p += ilen;
				off += ilen;
				continue;
			}
		case DataType_Structure:
			{
				strItem = tr("Structure");
				delete[] str;
				strItemLen.sprintf("(%d-%d)",
					ihoff + off,
					ihoff + off + ilen + decode_len - 1);
				item1 = new QTreeWidgetItem(item);
				item1->setText(0,strItem);
				item1->setText(2,strItemLen);
				item->addChild(item1);
				m_items.append(item1);
				if (iexpanded_no < iexpanded_max)
				{
					if (m_ExpandedItemList.at(iexpanded_no) == strItem)
					{
						iexpanded_no ++;
						item1->setExpanded(true);
					}
				}
				item1->setExpanded(true);
				p += ilen;
				off += ilen;

				ilen = GetDataToTree(item1,p,off,decode_len, &iexpanded_no, iexpanded_max, bmms2);
				if (ilen == -1)
				{
					return - 1;
				}
				p += ilen;
				off += ilen;
				continue;
			}
		case DataType_FloatingPoint:
			{
				strItem = tr("FloatingPoint");
				delete[] str;
				strItemLen.sprintf("(%d-%d)",
					ihoff + off,
					ihoff + off + ilen + decode_len - 1);
				item1 = new QTreeWidgetItem(item);
				item1->setText(0,strItem);
				item1->setText(2,strItemLen);
				item->addChild(item1);
				m_items.append(item1);
				if (iexpanded_no < iexpanded_max)
				{
					if (m_ExpandedItemList.at(iexpanded_no) == strItem)
					{
						iexpanded_no ++;
						item1->setExpanded(true);
					}
				}
				item1->setExpanded(true);
				p += ilen;
				off += ilen;

				ilen = GetDataToTree(item1,p,off,decode_len, &iexpanded_no, iexpanded_max, bmms2);
				if (ilen == -1)
				{
					return - 1;
				}
				p += ilen;
				off += ilen;
				continue;
			}
		case DataType_Array:
			{
				strItem = tr("Array");
				delete[] str;
				strItemLen.sprintf("(%d-%d)",
					ihoff + off,
					ihoff + off + ilen + decode_len - 1);
				item1 = new QTreeWidgetItem(item);
				item1->setText(0,strItem);
				item1->setText(2,strItemLen);
				item->addChild(item1);
				m_items.append(item1);
				if (iexpanded_no < iexpanded_max)
				{
					if (m_ExpandedItemList.at(iexpanded_no) == strItem)
					{
						iexpanded_no ++;
						item1->setExpanded(true);
					}
				}
				item1->setExpanded(true);
				p += ilen;
				off += ilen;

				ilen = GetDataToTree(item1,p,off,decode_len, &iexpanded_no, iexpanded_max, bmms2);
				if (ilen == -1)
				{
					return - 1;
				}
				p += ilen;
				off += ilen;
				continue;
			}
		case DataType_Boolean:
			{
				strItem = tr("Boolean");
				memset(strtmp,0,255);
				DataBase_Info::GetPByte_DataStr(DataType_Boolean,p + ilen,decode_len,strtmp);
				strItemInfo = QString::fromStdString(strtmp);
				break;
			}
		case DataType_Integer:
			{
				strItem = tr("Integer");
				memset(strtmp,0,255);
				DataBase_Info::GetPByte_DataStr(DataType_Integer,p + ilen,decode_len,strtmp);
				strItemInfo = QString::fromStdString(strtmp);
				break;
			}
		case 0x90:
			{
				strItem = tr("UNICODE");
				strncpy(str,(char *)(p + ilen),decode_len);
				strItemInfo = QString::fromUtf8(str);
				//                int dataUns;
				//                if(DataBase_Info::DataPCharToVal((ST_UCHAR *)(p + ilen),(ST_INT *)&dataUns,decode_len))
				//                {
				//                    strItemInfo.sprintf("%d",dataUns);
				//                }
				//                else
				//                {
				//                    strItemInfo = QString::fromUtf8(str);
				//                }
				break;
			}
		case DataType_Unsigned:
			{
				strItem = tr("Unsigned");
				int dataUns;
				if(DataBase_Info::DataPCharToVal((ST_UCHAR *)(p + ilen),(ST_INT *)&dataUns,decode_len))
				{
					strItemInfo.sprintf("%d",dataUns);
				}
				else
					strItemInfo = tr("");
				break;
			}
		case DataType_Bcd:
			{
				strItem = tr("Bcd");
				int dataBcd;
				if(DataBase_Info::DataPCharToVal((ST_UCHAR *)(p + ilen),(ST_INT *)&dataBcd,decode_len))
				{
					strItemInfo.sprintf("%d",dataBcd);
				}
				else
					strItemInfo = tr("");
				break;
			}
		case DataType_Real:
			{
				strItem = tr("Real");
				int dataReal;
				if(DataBase_Info::DataPCharToVal((ST_UCHAR *)(p + ilen),(ST_INT *)&dataReal,decode_len))
				{
					strItemInfo.sprintf("%d",dataReal);
				}
				else
					strItemInfo = tr("");
				break;
			}
		case DataType_Binary_time:
			{
				strItem = tr("Binary_time");
				memset(strtmp,0,255);
				DataBase_Info::GetPByte_DataStr(DataType_Binary_time,p + ilen,decode_len,strtmp);
				strItemInfo = QString::fromStdString(strtmp);
				break;
			}
		case DataType_UTCTIME:
			{
				strItem = tr("UTC");
				memset(strtmp,0,255);
				DataBase_Info::GetPByte_DataStr(DataType_UTCTIME,p + ilen,decode_len,strtmp);
				strItemInfo = QString::fromStdString(strtmp);
				break;
			}
		case DataType_Floating_point:
			{
				strItem = tr("Floating_point");
				memset(strtmp,0,255);
				DataBase_Info::GetPByte_DataStr(DataType_Floating_point,p + ilen,decode_len,strtmp);
				strItemInfo = QString::fromStdString(strtmp);
				break;
			}
		case DataType_Bit_string:
			{
				strItem = tr("Bit_string");
				memset(strtmp,0,255);
				DataBase_Info::GetPByte_DataStr(DataType_Bit_string,p + ilen,decode_len,strtmp);
				strItemInfo = QString::fromStdString(strtmp);
				break;
			}
		case DataType_Octet_string:
			{
				strItem = tr("Octet_string");
				memset(strtmp,0,255);
				DataBase_Info::GetPByte_DataStr(DataType_Octet_string,p + ilen,decode_len,strtmp);
				strItemInfo = QString::fromStdString(strtmp);
				break;
			}
		case 0x80:
		case DataType_Visible_string:
			{
				strItem = tr("visible_string");
				strncpy(str,(char *)(p + ilen),decode_len);
				strItemInfo = QString::fromLocal8Bit(str, decode_len);
				break;
			}
		case DataType_BooleanArray:
		default:
			{
				strItem = tr("The unknown");
				strItemInfo = tr("");
				break;
			}
		}
		strItemLen.sprintf("(%d-%d)",
			ihoff + off,
			ihoff + off + ilen + decode_len - 1);
		delete[] str;
		item1 = new QTreeWidgetItem(item);
		item1->setText(0,strItem);
		item1->setText(1,strItemInfo);
		item1->setText(2,strItemLen);
		item->addChild(item1);
		m_items.append(item1);
		p += ilen;
		off += ilen;
		if (!constr_elmnt)
		{
			p += decode_len;
			off += decode_len;
		}
	}
	return (off - ioff);
}

void SDecodeMms::GetParameterSO_ToTree(QTreeWidgetItem * pTreeItem, unsigned char *pData, int datalen, int ioff, int ioffend)
{
	if (datalen <= 1)
	{
		return;
	}
	QTreeWidgetItem * item, * item1;
	QString strItem, strItemInfo, strItemLen;

	item = pTreeItem;

	int i,j,dataoff;
	unsigned char * p = pData;
	unsigned char datatmp = 0;

	for (i = 1;i < datalen;i ++)
	{
		for (j = 0; j < 8; j ++)
		{
			datatmp = (p[i] >> (7 - j)) & 0x01;
			if (datatmp == 0x01)
			{
				dataoff = (i - 1) * 8 + j;
				strItem = QString::fromStdString(ParameterSO_Str[dataoff]);
				strItemInfo.sprintf("%d", dataoff);
				strItemLen.sprintf("(%d-%d)",
					ioff,
					ioffend);
				item1 = new QTreeWidgetItem(item);
				item1->setText(0,strItem);
				item1->setText(1,strItemInfo);
				item1->setText(2,strItemLen);
				item->addChild(item1);
				m_items.append(item1);
			}
		}
	}
}

void SDecodeMms::GetServiceSO_ToTree(QTreeWidgetItem * pTreeItem, unsigned char *pData, int datalen, int ioff, int ioffend)
{
	if (datalen <= 1)
	{
		return;
	}
	QTreeWidgetItem * item, * item1;
	QString strItem, strItemInfo, strItemLen;

	item = pTreeItem;

	int i,j,dataoff;
	unsigned char * p = pData;
	unsigned char datatmp = 0;

	for (i = 1;i < datalen;i ++)
	{
		for (j = 0; j < 8; j ++)
		{
			datatmp = (p[i] >> (7 - j)) & 0x01;
			if (datatmp == 0x01)
			{
				dataoff = (i - 1) * 8 + j;
				strItem = QString::fromStdString(ServiceSO_Str[dataoff]);
				strItemInfo.sprintf("%d", dataoff);
				strItemLen.sprintf("(%d-%d)",
					ioff,
					ioffend);
				item1 = new QTreeWidgetItem(item);
				item1->setText(0,strItem);
				item1->setText(1,strItemInfo);
				item1->setText(2,strItemLen);
				item->addChild(item1);
				m_items.append(item1);
			}
		}
	}
}

int SDecodeMms::GetRptDataToTree(QTreeWidgetItem * pTreeItem, unsigned char *pData, int ioff, int leftlen, int * expandno, int expandmax, bool bmms2)
{
	int iexpanded_no, iexpanded_max;
	QTreeWidgetItem * item, * item1;
	QString strItem, strItemInfo, strItemLen;
	int itemnum = 0 , data1,data2,data3;
	char strtmp[1000];
	SWF_MmsAcsi * pMmsAcsi;
	int ihoff = m_iMMSEthLen;
	pMmsAcsi = &m_database_Acsi;
	if (bmms2)
	{
		ihoff = m_iMMSEthLen + m_database_Acsi.m_BufLength - m_database_Acsi.m_iLeavelen;
		pMmsAcsi = &m_database_Acsi_2;
	}

	iexpanded_no = *expandno;
	iexpanded_max = expandmax;
	item = pTreeItem;


	int off = ioff;
	unsigned char * p = pData;
	int ilen = 0;
	ST_UINT decode_id;
	ST_UINT id_bytes;
	ST_INT decode_len;
	ST_UINT len_bytes;
	ST_INT lentmp;
	ST_UCHAR c;
	ST_UCHAR constr_elmnt;
	char * str = NULL;

	data1 = data2 = data3 = 0;
	//Data
	lentmp = off + leftlen;
	while(off < lentmp)
	{
		decode_id = (ST_UINT)DataBase_Info::asn1r_head_decode_id(p,&id_bytes);
		if (decode_id == 0xFFFF) return -1;
		c = *p;
		constr_elmnt = (ST_UCHAR) (c & CONSTR);/* isolate the constr bit	*/
		decode_len = (ST_INT)DataBase_Info::asn1r_head_decode_len(p + id_bytes,&len_bytes);
		if (decode_len == -1) return -1;
		ilen = id_bytes + len_bytes;

		itemnum ++;

		str = new char[decode_len + 1];
		memset(str,0,decode_len + 1);
		switch (c)
		{
		case DataType_Structure:
			{
				strItem = tr("Structure");
				delete[] str;
				strItemLen.sprintf("(%d-%d)",
					ihoff + off,
					ihoff + off + ilen + decode_len - 1);
				item1 = new QTreeWidgetItem(item);
				item1->setText(0,strItem);
				item1->setText(2,strItemLen);
				item->addChild(item1);
				m_items.append(item1);
				if (iexpanded_no < iexpanded_max)
				{
					if (m_ExpandedItemList.at(iexpanded_no) == strItem)
					{
						iexpanded_no ++;
						item1->setExpanded(true);
					}
				}
				item1->setExpanded(true);
				p += ilen;
				off += ilen;

				ilen = GetDataToTree(item1,p,off,decode_len, &iexpanded_no, iexpanded_max, bmms2);
				if (ilen == -1)
				{
					return - 1;
				}
				p += ilen;
				off += ilen;
				continue;
			}
		case DataType_FloatingPoint:
			{
				strItem = tr("FloatingPoint");
				delete[] str;
				strItemLen.sprintf("(%d-%d)",
					ihoff + off,
					ihoff + off + ilen + decode_len - 1);
				item1 = new QTreeWidgetItem(item);
				item1->setText(0,strItem);
				item1->setText(2,strItemLen);
				item->addChild(item1);
				m_items.append(item1);
				if (iexpanded_no < iexpanded_max)
				{
					if (m_ExpandedItemList.at(iexpanded_no) == strItem)
					{
						iexpanded_no ++;
						item1->setExpanded(true);
					}
				}
				item1->setExpanded(true);
				p += ilen;
				off += ilen;

				ilen = GetDataToTree(item1,p,off,decode_len, &iexpanded_no, iexpanded_max, bmms2);
				if (ilen == -1)
				{
					return - 1;
				}
				p += ilen;
				off += ilen;
				continue;
			}
		case DataType_Array:
			{
				strItem = tr("Array");
				delete[] str;
				strItemLen.sprintf("(%d-%d)",
					ihoff + off,
					ihoff + off + ilen + decode_len - 1);
				item1 = new QTreeWidgetItem(item);
				item1->setText(0,strItem);
				item1->setText(2,strItemLen);
				item->addChild(item1);
				m_items.append(item1);
				if (iexpanded_no < iexpanded_max)
				{
					if (m_ExpandedItemList.at(iexpanded_no) == strItem)
					{
						iexpanded_no ++;
						item1->setExpanded(true);
					}
				}
				item1->setExpanded(true);
				p += ilen;
				off += ilen;

				ilen = GetDataToTree(item1,p,off,decode_len, &iexpanded_no, iexpanded_max, bmms2);
				if (ilen == -1)
				{
					return - 1;
				}
				p += ilen;
				off += ilen;
				continue;
			}
		case DataType_Boolean:
			{
				if (itemnum == 5)
				{
					strItem = tr("More Segment Follow");
				}
				else
				{
					strItem = tr("Boolean");
				}
				memset(strtmp,0,1000);
				DataBase_Info::GetPByte_DataStr(DataType_Boolean,p + ilen,decode_len,strtmp);
				strItemInfo = QString::fromStdString(strtmp);
				break;
			}
		case DataType_Integer:
			{
				strItem = tr("Integer");
				memset(strtmp,0,1000);
				DataBase_Info::GetPByte_DataStr(DataType_Integer,p + ilen,decode_len,strtmp);
				strItemInfo = QString::fromStdString(strtmp);
				break;
			}
		case 0x90:
			{
				strItem = tr("UNICODE");
				strncpy(str,(char *)(p + ilen),decode_len);
				strItemInfo = QString::fromUtf8(str);
				//                int dataUns;
				//                if(DataBase_Info::DataPCharToVal((ST_UCHAR *)(p + ilen),(ST_INT *)&dataUns,decode_len))
				//                {
				//                    strItemInfo.sprintf("%d",dataUns);
				//                }
				//                else
				//                    strItemInfo = tr("");
				break;
			}
		case DataType_Unsigned:
			{
				if (itemnum == 6)
				{
					strItem = tr("Sub Sequence Number");
				}
				else
				{
					strItem = tr("Unsigned");
				}
				int dataUns;
				if(DataBase_Info::DataPCharToVal((ST_UCHAR *)(p + ilen),(ST_INT *)&dataUns,decode_len))
				{
					strItemInfo.sprintf("%d",dataUns);
				}
				else
					strItemInfo = tr("");
				break;
			}
		case DataType_Bcd:
			{
				strItem = tr("Bcd");
				int dataBcd;
				if(DataBase_Info::DataPCharToVal((ST_UCHAR *)(p + ilen),(ST_INT *)&dataBcd,decode_len))
				{
					strItemInfo.sprintf("%d",dataBcd);
				}
				else
					strItemInfo = tr("");
				break;
			}
		case DataType_Real:
			{
				strItem = tr("Real");
				int dataReal;
				if(DataBase_Info::DataPCharToVal((ST_UCHAR *)(p + ilen),(ST_INT *)&dataReal,decode_len))
				{
					strItemInfo.sprintf("%d",dataReal);
				}
				else
					strItemInfo = tr("");
				break;
			}
		case DataType_Binary_time:
			{
				strItem = tr("Binary_time");
				memset(strtmp,0,1000);
				DataBase_Info::GetPByte_DataStr(DataType_Binary_time,p + ilen,decode_len,strtmp);
				strItemInfo = QString::fromStdString(strtmp);
				break;
			}
		case DataType_UTCTIME:
			{
				strItem = tr("UTC");
				memset(strtmp,0,1000);
				DataBase_Info::GetPByte_DataStr(DataType_UTCTIME,p + ilen,decode_len,strtmp);
				strItemInfo = QString::fromStdString(strtmp);
				break;
			}
		case DataType_Floating_point:
			{
				memset(strtmp,0,1000);
				DataBase_Info::GetPByte_DataStr(DataType_Floating_point,p + ilen,decode_len,strtmp);
				if (itemnum >= 9)
				{
					data2 ++;
					strItem = tr("Data Value[%1]").arg(data2);
				}
				else
				{
					strItem = tr("Floating_point");
				}
				strItemInfo = QString::fromStdString(strtmp);
				break;
			}
		case DataType_Bit_string:
			{
				if (itemnum == 2)
				{
					strItem = tr("Reported OptFlds");
				}
				else if (itemnum == 7)
				{
					strItem = tr("Include BitStr");
				}
				else if (itemnum >= 9)
				{
					data3 ++;
					strItem = tr("Reason Code[%1]").arg(data3);
				}
				else
				{
					strItem = tr("Bit_string");
				}
				memset(strtmp,0,1000);
				DataBase_Info::GetPByte_DataStr(DataType_Bit_string,p + ilen,decode_len,strtmp);
				strItemInfo = QString::fromStdString(strtmp);
				break;
			}
		case DataType_Octet_string:
			{
				strItem = tr("Octet_string");
				memset(strtmp,0,1000);
				DataBase_Info::GetPByte_DataStr(DataType_Octet_string,p + ilen,decode_len,strtmp);
				strItemInfo = QString::fromStdString(strtmp);
				break;
			}
		case DataType_Visible_string:
			{
				strncpy(str,(char *)(p + ilen),decode_len);
				if (itemnum == 1)
				{
					strItem = tr("Report ID");
				}
				else if (itemnum == 5)
				{
					strItem = tr("DataSet");
					pMmsAcsi->m_bDatSet = TRUE;
					sprintf(pMmsAcsi->m_DatSet, "%s", str);
				}
				else if (itemnum >= 9)
				{
					data1 ++;
					strItem = tr("Data Reference[%1]").arg(data1);
				}
				else
				{
					strItem = tr("visible_string");
				}
				strItemInfo = QString::fromStdString(str);
				break;
			}
		case DataType_BooleanArray:
		default:
			{
				strItem = tr("The unknown");
				strItemInfo = tr("");
				break;
			}
		}
		strItemLen.sprintf("(%d-%d)",
			ihoff + off,
			ihoff + off + ilen + decode_len - 1);
		delete[] str;
		item1 = new QTreeWidgetItem(item);
		item1->setText(0,strItem);
		item1->setText(1,strItemInfo);
		item1->setText(2,strItemLen);
		item->addChild(item1);
		m_items.append(item1);
		p += ilen;
		off += ilen;
		if (!constr_elmnt)
		{
			p += decode_len;
			off += decode_len;
		}
	}
	return (off - ioff);
}

#endif
#endif
