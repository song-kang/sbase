/**
 *
 * 文 件 名 : SDecodeOth.cpp
 * 创建日期 : 2015-5-9 23:51
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : 其它协议解码
 * 修改记录 : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-5-9	邵凯田　创建文件
 *
 **/

#include "SDecodeOth.h"

//////////////////////////////////////////////////////////////////////////
// 名    称:  SDecodeArp
// 作    者:  邵凯田
// 创建时间:  2015-5-9 23:51
// 描    述:  ARP
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

