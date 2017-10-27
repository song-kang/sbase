/**
 *
 * 文 件 名 : SDecodIp.cpp
 * 创建日期 : 2015-5-8 14:09
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : IP协议及其上层的标准协议解码
 * 修改记录 : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-5-8	邵凯田　创建文件
 *
 **/

#include "SDecodeIp.h"

//////////////////////////////////////////////////////////////////////////
// 名    称:  SDecodeIp
// 作    者:  邵凯田
// 创建时间:  2015-5-8 14:09
// 描    述:  IP协议的解码
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
// 名    称:  SDecodeTcp
// 作    者:  邵凯田
// 创建时间:  2015-5-8 14:09
// 描    述:  IP协议的解码
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
// 名    称:  SDecodeUdp
// 作    者:  邵凯田
// 创建时间:  2015-5-8 14:09
// 描    述:  IP协议的解码
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
// 名    称:  SDecodeIcmp
// 作    者:  邵凯田
// 创建时间:  2015-5-8 14:09
// 描    述:  IP协议的解码
//////////////////////////////////////////////////////////////////////////
SDecodeIcmp::SDecodeIcmp()
{
	m_ProtType = SDECODE_PROT_ICMP;
	m_LastEvent.m_iProtType = (int)m_ProtType;
}

SDecodeIcmp::~SDecodeIcmp()
{

}

