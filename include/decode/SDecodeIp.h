/**
 *
 * �� �� �� : SDecodeIp.h
 * �������� : 2015-5-8 14:05
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : IPЭ�鼰���ϲ�ı�׼Э�����
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-5-8	�ۿ�������ļ�
 *
 **/
#ifndef __SKT_DECODER_IP_H__
#define __SKT_DECODER_IP_H__

#include "SDecodeBase.h"

//////////////////////////////////////////////////////////////////////////
// ��    ��:  SDecodeIp
// ��    ��:  �ۿ���
// ����ʱ��:  2015-5-8 14:09
// ��    ��:  IPЭ��Ľ���
//////////////////////////////////////////////////////////////////////////
class SDecodeIp : public SDecodeBase
{
public:
	SDecodeIp();
	virtual ~SDecodeIp();

};

class SDecodeTcp : public SDecodeBase
{
public:
	SDecodeTcp();
	virtual ~SDecodeTcp();

};

class SDecodeUdp : public SDecodeBase
{
public:
	SDecodeUdp();
	virtual ~SDecodeUdp();

};

class SDecodeIcmp : public SDecodeBase
{
public:
	SDecodeIcmp();
	virtual ~SDecodeIcmp();

};


#endif//__SKT_DECODER_IP_H__

