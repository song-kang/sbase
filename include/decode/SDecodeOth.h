/**
 *
 * �� �� �� : SDecodeOth.h
 * �������� : 2015-5-9 23:51
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
#ifndef __SKT_DECODER_OTH_H__
#define __SKT_DECODER_OTH_H__

#include "SDecodeBase.h"

//////////////////////////////////////////////////////////////////////////
// ��    ��:  SDecodeArp
// ��    ��:  �ۿ���
// ����ʱ��:  2015-5-9 23:51
// ��    ��:  ARP
//////////////////////////////////////////////////////////////////////////
class SDecodeArp : public SDecodeBase
{
public:
	SDecodeArp();
	virtual ~SDecodeArp();

};

class SDecodeEth2 : public SDecodeBase
{
public:
	SDecodeEth2();
	virtual ~SDecodeEth2();

};



#endif//__SKT_DECODER_OTH_H__

