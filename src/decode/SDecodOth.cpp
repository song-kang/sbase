/**
 *
 * �� �� �� : SDecodeOth.cpp
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

#include "SDecodeOth.h"

//////////////////////////////////////////////////////////////////////////
// ��    ��:  SDecodeArp
// ��    ��:  �ۿ���
// ����ʱ��:  2015-5-9 23:51
// ��    ��:  ARP
//////////////////////////////////////////////////////////////////////////
SDecodeArp::SDecodeArp()
{
	m_ProtType = SDECODE_PROT_ARP;
	m_LastEvent.m_iProtType = (int)m_ProtType;
}

SDecodeArp::~SDecodeArp()
{

}

SDecodeEth2::SDecodeEth2()
{
	m_ProtType = SDECODE_PROT_ETH2;
	m_LastEvent.m_iProtType = (int)m_ProtType;
}

SDecodeEth2::~SDecodeEth2()
{

}

