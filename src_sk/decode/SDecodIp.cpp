/**
 *
 * �� �� �� : SDecodIp.cpp
 * �������� : 2015-5-8 14:09
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

#include "SDecodeIp.h"

//////////////////////////////////////////////////////////////////////////
// ��    ��:  SDecodeIp
// ��    ��:  �ۿ���
// ����ʱ��:  2015-5-8 14:09
// ��    ��:  IPЭ��Ľ���
//////////////////////////////////////////////////////////////////////////
SDecodeIp::SDecodeIp()
{
	m_ProtType = SDECODE_PROT_IP;
	m_LastEvent.m_iProtType = (int)m_ProtType;
}

SDecodeIp::~SDecodeIp()
{

}


//////////////////////////////////////////////////////////////////////////
// ��    ��:  SDecodeTcp
// ��    ��:  �ۿ���
// ����ʱ��:  2015-5-8 14:09
// ��    ��:  IPЭ��Ľ���
//////////////////////////////////////////////////////////////////////////
SDecodeTcp::SDecodeTcp()
{
	m_ProtType = SDECODE_PROT_TCP;
	m_LastEvent.m_iProtType = (int)m_ProtType;
}

SDecodeTcp::~SDecodeTcp()
{

}


//////////////////////////////////////////////////////////////////////////
// ��    ��:  SDecodeUdp
// ��    ��:  �ۿ���
// ����ʱ��:  2015-5-8 14:09
// ��    ��:  IPЭ��Ľ���
//////////////////////////////////////////////////////////////////////////
SDecodeUdp::SDecodeUdp()
{
	m_ProtType = SDECODE_PROT_UDP;
	m_LastEvent.m_iProtType = (int)m_ProtType;
}

SDecodeUdp::~SDecodeUdp()
{

}


//////////////////////////////////////////////////////////////////////////
// ��    ��:  SDecodeIcmp
// ��    ��:  �ۿ���
// ����ʱ��:  2015-5-8 14:09
// ��    ��:  IPЭ��Ľ���
//////////////////////////////////////////////////////////////////////////
SDecodeIcmp::SDecodeIcmp()
{
	m_ProtType = SDECODE_PROT_ICMP;
	m_LastEvent.m_iProtType = (int)m_ProtType;
}

SDecodeIcmp::~SDecodeIcmp()
{

}

