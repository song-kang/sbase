/**
 *
 * 文 件 名 : SDecodeIp.h
 * 创建日期 : 2015-5-8 14:05
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
#ifndef __SKT_DECODER_IP_H__
#define __SKT_DECODER_IP_H__

#include "SDecodeBase.h"

//////////////////////////////////////////////////////////////////////////
// 名    称:  SDecodeIp
// 作    者:  邵凯田
// 创建时间:  2015-5-8 14:09
// 描    述:  IP协议的解码
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

