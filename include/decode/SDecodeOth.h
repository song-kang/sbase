/**
 *
 * 文 件 名 : SDecodeOth.h
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
#ifndef __SKT_DECODER_OTH_H__
#define __SKT_DECODER_OTH_H__

#include "SDecodeBase.h"

//////////////////////////////////////////////////////////////////////////
// 名    称:  SDecodeArp
// 作    者:  邵凯田
// 创建时间:  2015-5-9 23:51
// 描    述:  ARP
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

