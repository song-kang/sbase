/**
 *
 * 文 件 名 : Asn1Ber.cpp
 * 创建日期 : 2015-5-20 18:43
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

#include "Asn1Ber.h"

CAsn1BerDecoder::CAsn1BerDecoder()
{
	m_iFmtErrorNo = 100;
}


CAsn1BerDecoder::~CAsn1BerDecoder()
{

}

int CAsn1BerDecoder::GetIntWithBufLen(BYTE *p,int l)
{
	register int v=0;
	if(l>4)
		return -1;
	memcpy(((BYTE*)&v)+4-l,p,l);
	return ntohl(v);
}

int CAsn1BerDecoder::DecodeAsn1Ber(BYTE *pInBuf,int iLen,CallbackAsn1Decode pCallback,void* cbParam)
{
	ST_UCHAR c;
	//ST_UINT decode_id,id_bytes,len_bytes;
	//ST_INT decode_len;
	stuAsn1Pdu pdu;
	BYTE *p = pInBuf;
	int ret;

	while(p < pInBuf + iLen)
	{
		//id
		c = *(p ++);
		pdu.tag.cls = c>>6;
		pdu.tag.fmt = (c>>5)&1;
		if ((c &= 0x1F) == 0x1F)
		{
			if ((c = *(p++)) & 0x80)
			{
				if (*p & 0x80)
				{
					//NEW_MALFORMED_ID;
					return m_iFmtErrorNo;
				}
				ST_UINT16 ui16 = (ST_UINT16) (c & 0x7F) << 7;
				ui16 |= (*(p++) & 0x7F);
				pdu.tag.id =(WORD) ui16;
				//id_bytes = 3;
			}
			else
			{
				pdu.tag.id = (WORD) c;
				//id_bytes = 2;
			}
		}
		else
		{
			pdu.tag.id = (WORD) c;
			//id_bytes = 1;
		}

		//length
		if ((c = *(p ++)) & 0x80)
		{
			if (c &= 0x7F)
			{
				//len_bytes = c+1;

				if(c>4)
				{
					//NEW_MALFORMED_LEN;
					return m_iFmtErrorNo;
				}
				register int v=0;
				memcpy(((BYTE*)&v)+4-c,p,c);
				p += c;
				pdu.len = ntohl(v);
			}
			else
			{
				//NEW_MALFORMED_LEN;
				return m_iFmtErrorNo;
			}
		}
		else
		{
			pdu.len = (ST_INT)c;
			//len_bytes = 1;
		}
		pdu.val = p;
		
		if((ret = pCallback(cbParam,&pdu)) != 0)
		{
			//NEW_MALFORMED;
			return ret;
		}
		p += pdu.len;
		if(p > pInBuf + iLen)
		{
			//NEW_MALFORMED;
			return m_iFmtErrorNo;
		}		
	}
	return 0;
}

#if 0
int
dissect_ber_choice(asn1_ctx_t *actx, proto_tree *parent_tree, tvbuff_t *tvb, int offset, const ber_choice_t *choice, int hf_id, int ett_id, int *branch_taken)
{
	char class;
	bool pc, ind, imp_tag = FALSE;
	int tag;
	unsigned int len;
	const ber_choice_t *ch;
	proto_tree *tree=parent_tree;
	proto_item *item=NULL;
	int end_offset, start_offset, count;
	int hoffset = offset;
	header_field_info	*hfinfo;
	int length, length_remaining;
	tvbuff_t *next_tvb;
	bool first_pass;

#ifdef DEBUG_BER_CHOICE
{
const char *name;
header_field_info *hfinfo;
if(hf_id>=0){
hfinfo = proto_registrar_get_nth(hf_id);
name=hfinfo->name;
} else {
name="unnamed";
}
if(tvb_length_remaining(tvb,offset)>3){
printf("CHOICE dissect_ber_choice(%s) entered offset:%d len:%d %02x:%02x:%02x\n",name,offset,tvb_length_remaining(tvb,offset),tvb_get_guint8(tvb,offset),tvb_get_guint8(tvb,offset+1),tvb_get_guint8(tvb,offset+2));
}else{
printf("CHOICE dissect_ber_choice(%s) entered len:%d\n",name,tvb_length_remaining(tvb,offset));
}
}
#endif
	start_offset=offset;

        if(tvb_length_remaining(tvb,offset) == 0) {
                item = proto_tree_add_text(parent_tree, tvb, offset, 0, "BER Error: Empty choice was found");
                proto_item_set_expert_flags(item, PI_MALFORMED, PI_WARN);
                expert_add_info_format(actx->pinfo, item, PI_MALFORMED, PI_WARN, "BER Error: Empty choice was found");
                return offset; 
        }       

	/* read header and len for choice field */
	offset=get_ber_identifier(tvb, offset, &class, &pc, &tag);
	offset=get_ber_length(tvb, offset, &len, &ind);
	  end_offset = offset + len ;

	/* Some sanity checks.
	 * The hf field passed to us MUST be an integer type
	 */
	if(hf_id >= 0){
		hfinfo=proto_registrar_get_nth(hf_id);
		switch(hfinfo->type) {
			case FT_UINT8:
			case FT_UINT16:
			case FT_UINT24:
			case FT_UINT32:
				break;
		default:
			proto_tree_add_text(tree, tvb, offset, len,"dissect_ber_choice(): Was passed a HF field that was not integer type : %s",hfinfo->abbrev);
			fprintf(stderr,"dissect_ber_choice(): frame:%u offset:%d Was passed a HF field that was not integer type : %s\n",actx->pinfo->fd->num,offset,hfinfo->abbrev);
			return end_offset;
		}
	}



	/* loop over all entries until we find the right choice or
	   run out of entries */
	ch = choice;
	if(branch_taken){
		*branch_taken=-1;
	}
	first_pass = TRUE;
	while(ch->func || first_pass){
		if(branch_taken){
			(*branch_taken)++;
		}
	  /* we reset for a second pass when we will look for choices */
	  if(!ch->func) {
	    first_pass = FALSE;
	    ch = choice; /* reset to the beginning */
		if(branch_taken){
			*branch_taken=-1;
		}
	  }

choice_try_again:
#ifdef DEBUG_BER_CHOICE
printf("CHOICE testing potential subdissector class[%p]:%d:(expected)%d  tag:%d:(expected)%d flags:%d\n",ch,class,ch->class,tag,ch->tag,ch->flags);
#endif
		if( (first_pass && (((ch->class==class)&&(ch->tag==tag))
		     ||  ((ch->class==class)&&(ch->tag==-1)&&(ch->flags&BER_FLAGS_NOOWNTAG)))) ||
		    (!first_pass && (((ch->class == BER_CLASS_ANY) && (ch->tag == -1)))) /* we failed on the first pass so now try any choices */
		){
			if(!(ch->flags & BER_FLAGS_NOOWNTAG)){
				/* dissect header and len for field */
				hoffset = dissect_ber_identifier(actx->pinfo, tree, tvb, start_offset, NULL, NULL, NULL);
				hoffset = dissect_ber_length(actx->pinfo, tree, tvb, hoffset, NULL, NULL);
				start_offset=hoffset;
				if (ind)
					{
					length = len-2;
					}
				else
					{
					length = len;
					}
			}
			else
				length = end_offset- hoffset;
			/* create subtree */
			if(hf_id >= 0){
				if(parent_tree){
					item = proto_tree_add_uint(parent_tree, hf_id, tvb, hoffset, end_offset - hoffset, ch->value);
					tree = proto_item_add_subtree(item, ett_id);
				}
			}

			length_remaining=tvb_length_remaining(tvb, hoffset);
			if(length_remaining>length)
				length_remaining=length;

#ifdef REMOVED
			/* This is bogus and makes the OID_1.0.9506.1.1.cap file
			 * in Steven J Schaeffer's email of 2005-09-12 fail to dissect
			 * properly.  Maybe we should get rid of 'first_pass'
			 * completely.
			 * It was added as a qad workaround for some problem CMIP
			 * traces anyway.
			 * God, this file is a mess and it is my fault. /ronnie
			 */
			if(first_pass)
			next_tvb=tvb_new_subset(tvb, hoffset, length_remaining, length);
			else
			  next_tvb = tvb; /* we didn't make selection on this class/tag so pass it on */
#endif
			next_tvb=tvb_new_subset(tvb, hoffset, length_remaining, length);


#ifdef DEBUG_BER_CHOICE
{
const char *name;
header_field_info *hfinfo;
if(hf_id>=0){
hfinfo = proto_registrar_get_nth(hf_id);
name=hfinfo->name;
} else {
name="unnamed";
}
if(tvb_length_remaining(next_tvb,0)>3){
printf("CHOICE dissect_ber_choice(%s) calling subdissector start_offset:%d offset:%d len:%d %02x:%02x:%02x\n",name,start_offset,offset,tvb_length_remaining(next_tvb,0),tvb_get_guint8(next_tvb,0),tvb_get_guint8(next_tvb,1),tvb_get_guint8(next_tvb,2));
}else{
printf("CHOICE dissect_ber_choice(%s) calling subdissector len:%d\n",name,tvb_length(next_tvb));
}
}
#endif
			if (next_tvb == NULL) {
				/* Assume that we have a malformed packet. */
				THROW(ReportedBoundsError);
			}
			imp_tag = FALSE;
			if ((ch->flags & BER_FLAGS_IMPLTAG))
				imp_tag = TRUE;
			count=ch->func(imp_tag, next_tvb, 0, actx, tree, *ch->p_id);
#ifdef DEBUG_BER_CHOICE
{
const char *name;
header_field_info *hfinfo;
if(hf_id>=0){
hfinfo = proto_registrar_get_nth(hf_id);
name=hfinfo->name;
} else {
name="unnamed";
}
printf("CHOICE dissect_ber_choice(%s) subdissector ate %d bytes\n",name,count);
}
#endif
			if((count==0)&&(((ch->class==class)&&(ch->tag==-1)&&(ch->flags&BER_FLAGS_NOOWNTAG)) || !first_pass)){
				/* wrong one, break and try again */
				ch++;
#ifdef DEBUG_BER_CHOICE
{
const char *name;
header_field_info *hfinfo;
if(hf_id>=0){
hfinfo = proto_registrar_get_nth(hf_id);
name=hfinfo->name;
} else {
name="unnamed";
}
printf("CHOICE dissect_ber_choice(%s) trying again\n",name);
}
#endif
				goto choice_try_again;
			}
			if(!(ch->flags & BER_FLAGS_NOOWNTAG)){
			 if(ind)
			 	{
			 	/* we are traversing a indfinite length choice where we did not pass the tag length */
			 	/* we need to eat the EOC */
			 	if(show_internal_ber_fields){
					proto_tree_add_text(tree, tvb, start_offset, count+2, "CHOICE EOC");
				}
			 }
			}
			return end_offset;
		}
		ch++;
	}
	if(branch_taken){
		/* none of the branches were taken so set the param
		   back to -1 */
		*branch_taken=-1;
	}

#ifdef REMOVED
	/*XXX here we should have another flag to the CHOICE to distinguish
	 * between the case when we know it is a mandatory   or if the CHOICE is optional == no arm matched */

	/* oops no more entries and we still havent found
	 * our guy :-(
	 */
	item = proto_tree_add_text(tree, tvb, offset, len, "BER Error: This choice field was not found.");
	proto_item_set_expert_flags(item, PI_MALFORMED, PI_WARN);
	expert_add_info_format(actx->pinfo, item, PI_MALFORMED, PI_WARN, "BER Error: This choice field was not found");
	return end_offset;
#endif

	return start_offset;
}
#endif

#if 0


/* Computes the absolute offset and length based on a possibly-negative offset
 * and a length that is possible -1 (which means "to the end of the data").
 * Returns TRUE/FALSE indicating whether the offset is in bounds or
 * not. The integer ptrs are modified with the new offset and length.
 * No exception is thrown.
 *
 * XXX - we return TRUE, not FALSE, if the offset is positive and right
 * after the end of the tvbuff (i.e., equal to the length).  We do this
 * so that a dissector constructing a subset tvbuff for the next protocol
 * will get a zero-length tvbuff, not an exception, if there's no data
 * left for the next protocol - we want the next protocol to be the one
 * that gets an exception, so the error is reported as an error in that
 * protocol rather than the containing protocol.  */
static bool compute_offset_length(unsigned int tvb_length, unsigned int tvb_reported_length, int offset, int length,
		unsigned int *offset_ptr, unsigned int *length_ptr, int *exception)
{
	/* Compute the offset */
	if (offset >= 0)
	{
		/* Positive offset - relative to the beginning of the packet. */
		if ((unsigned int) offset > tvb_reported_length)
			return false;
		else if ((unsigned int) offset > tvb_length)
			return false;
		else
			*offset_ptr = offset;
	}
	else {
		/* Negative offset - relative to the end of the packet. */
		if ((unsigned int) -offset > tvb_reported_length)
			return false;
		else if ((unsigned int) -offset > tvb_length)
			return false;
		else
			*offset_ptr = tvb_length + offset;
	}

	/* Compute the length */
	if (length < -1)
		return false;
	else if (length == -1) 
		*length_ptr = tvb_length - *offset_ptr;
	else
		*length_ptr = length;

	return true;
}

int tvb_length_remaining(tvbuff_t *tvb, int offset)
{
	unsigned int abs_offset, abs_length;

	if (compute_offset_length(tvb->length, tvb->reported_length, offset, -1, &abs_offset, &abs_length))
		return abs_length;
	else 
		return -1;
}

/*  8.1.2 Identifier octets */
int get_ber_identifier0(tvbuff_t *tvb, int offset, char *mClass, bool *pc, int *tag)
{
	unsigned char id, t;
	char tmp_class;
	bool tmp_pc;
	int tmp_tag;

	id = tvb_get_guint8(tvb, offset);
	offset += 1;
	/* 8.1.2.2 */
	tmp_class = (id>>6) & 0x03;
	tmp_pc = (id>>5) & 0x01;
	tmp_tag = id&0x1F;
	/* 8.1.2.4 */
	if (tmp_tag == 0x1F)
	{
		tmp_tag = 0;
		while (tvb_length_remaining(tvb, offset) > 0)
		{
			t = tvb_get_guint8(tvb, offset);
			offset += 1;
			tmp_tag <<= 7;
			tmp_tag |= t & 0x7F;
			if (!(t & 0x80)) break;
		}
	}
	if (mClass)
		*mClass = tmp_class;
	if (pc)
		*pc = tmp_pc;
	if (tag)
		*tag = tmp_tag;

	return offset;
}
 
int dissect_ber_identifier0(packet_info *pinfo _U_, proto_tree *tree, tvbuff_t *tvb, int offset, char *mClass, bool *pc, int *tag)
{
	int old_offset = offset;
	char tmp_class;
	bool tmp_pc;
	int tmp_tag;

	offset = get_ber_identifier(tvb, offset, &tmp_class, &tmp_pc, &tmp_tag);

	/*
	if(show_internal_ber_fields)
	{
		proto_tree_add_uint(tree, hf_ber_id_class, tvb, old_offset, 1, tmp_class<<6);
		proto_tree_add_boolean(tree, hf_ber_id_pc, tvb, old_offset, 1, (tmp_pc)?0x20:0x00);
		if(tmp_tag>0x1F)
		{
			if(tmp_class==BER_CLASS_UNI)
			{
				proto_tree_add_uint(tree, hf_ber_id_uni_tag_ext, tvb, old_offset + 1, offset - (old_offset + 1), tmp_tag);
			} 
			else
			{
				proto_tree_add_uint(tree, hf_ber_id_tag_ext, tvb, old_offset + 1, offset - (old_offset + 1), tmp_tag);
			}
		} 
		else 
		{
			if(tmp_class==BER_CLASS_UNI)
			{
				proto_tree_add_uint(tree, hf_ber_id_uni_tag, tvb, old_offset, 1, tmp_tag);
			} 
			else
			{
				proto_tree_add_uint(tree, hf_ber_id_tag, tvb, old_offset, 1, tmp_tag);
			}
		}
	}
	*/
	if(mClass)
		*mClass = tmp_class;
	if(pc)
		*pc = tmp_pc;
	if(tag)
		*tag = tmp_tag;

	return offset;
}

/* this function gets the length octets of the BER TLV.
 * We only handle (TAGs and) LENGTHs that fit inside 32 bit integers.
 */
/* 8.1.3 Length octets */
int get_ber_length0(tvbuff_t *tvb, int offset, unsigned int *length, bool *ind)
{
	unsigned char oct, len;
	unsigned int tmp_len;
	unsigned int tmp_length;
	bool tmp_ind;
	int tmp_offset,s_offset;
	char tclass;
	bool tpc;
	int ttag;
	tmp_length = 0;
	tmp_ind = FALSE;

	oct = tvb_get_guint8(tvb, offset);
	offset += 1;

	if(!(oct&0x80))
	{
		/* 8.1.3.4 */
		tmp_length = oct;
	}
	else
	{
		len = oct & 0x7F;
		if(len > tvb->length - offset)
			return -1;
		if(len)
		{
			/* 8.1.3.5 */
			while (len--)
			{
				oct = tvb_get_guint8(tvb, offset);
				offset++;
				tmp_length = (tmp_length<<8) + oct;
			}
		} 
		else
		{
			/* 8.1.3.6 */
			tmp_offset = offset;
			/* ok in here we can traverse the BER to find the length, this will fix most indefinite length issues */
			/* Assumption here is that indefinite length is always used on constructed types*/
			/* check for EOC */
			while (tvb_get_guint8(tvb, offset) || tvb_get_guint8(tvb, offset+1))
			{
				/* not an EOC at offset */
				s_offset=offset;
				offset= get_ber_identifier(tvb, offset, &tclass, &tpc, &ttag);
				offset= get_ber_length(tvb,offset, &tmp_len, NULL);
				tmp_length += tmp_len+(offset-s_offset); /* length + tag and length */
				offset += tmp_len;
                                /* Make sure we've moved forward in the packet */
				if (offset <= s_offset)
					THROW(ReportedBoundsError);
			}
			tmp_length += 2;
			tmp_ind = TRUE;
			offset = tmp_offset;
		}
	}

	if (length)
		*length = tmp_length;
	if (ind)
		*ind = tmp_ind;

#ifdef DEBUG_BER
printf("get BER length %d, offset %d (remaining %d)\n", tmp_length, offset, tvb_length_remaining(tvb, offset));
#endif

	return offset;
}

#endif