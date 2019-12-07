/**
 *
 * 文 件 名 : EscPrintCom.cpp
 * 创建日期 : 2014-02-21 13:12
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : ESC(EPSON Standard Code for Printer)指令集基于串口的打印类
 * 修改记录 : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2014-02-21  邵凯田  串口打印支持
 *
 **/

#include "SEscPrintCom.h"


SEscPrintCom::SEscPrintCom()
{

}

SEscPrintCom::~SEscPrintCom()
{
	m_Com.Close();
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  打印初始化
// 作    者:  邵凯田
// 创建时间:  2014:2:21 13:46
// 参数说明:  @sAttr为扩展属性，不同实现类提供不同的参数
// 返 回 值:  true表示初始化成功,false表示初始化失败
//////////////////////////////////////////////////////////////////////////
bool SEscPrintCom::Init(SString sAttr)
{
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  打开打印机
// 作    者:  邵凯田
// 创建时间:  2014:2:21 13:23
// 参数说明:  @sAttr为打印机参数
// 返 回 值:  true打开成功，false打开失败
//////////////////////////////////////////////////////////////////////////
bool SEscPrintCom::Open(SString sAttr)
{
	SString dev = SString::GetAttributeValue(sAttr,"dev");
	int baud = SString::GetAttributeValueI(sAttr,"baud");
	if(baud == 0)
		baud = 19200;
	SString chk = SString::GetAttributeValue(sAttr,"chk");
	if(chk.length() == 0)
		chk = "N";
	if(!m_Com.Open(dev.data(),baud,chk.at(0)))
	{
		LOGERROR("打开串口时失败!attr=%s",sAttr.data());
		return false;
	}

	if(SEscPrint::Open(sAttr))
	{
		LOGDEBUG("串口打印机打开成功!");
		return true;
	}
	else
	{
		LOGDEBUG("串口打印机打开失败!");
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  关闭打印机
// 作    者:  邵凯田
// 创建时间:  2014:2:21 13:26
// 参数说明:  void
// 返 回 值:  true关闭成功，false关闭失败
//////////////////////////////////////////////////////////////////////////
bool SEscPrintCom::Close()
{
	m_Com.Close();
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  向打印机写数据
// 作    者:  邵凯田
// 创建时间:  2014:2:21 13:31
// 参数说明:  @pBuffer待写入内容缓冲区指针
//            @iLen为缓冲区字节长度
// 返 回 值:  >0表示实际发送字节数，<=0表示失败
//////////////////////////////////////////////////////////////////////////
int SEscPrintCom::Write(BYTE *pBuffer,int iLen)
{
	if(m_Com.GetHandle() == 0)
		return -1;
	int send_len = m_Com.Send(pBuffer,iLen);
	LOGDEBUG("SEND(len=%d): %s",iLen,SString::HexToStr(pBuffer,iLen).data());
	LOGDEBUG("SEND(len=%d): %s",iLen,SString::toString((char*)pBuffer,iLen).data());
	if (send_len < 0)
	{
		LOGERROR("向串口打印写数据失败:%d",send_len);
		return -2;
	}
#ifdef WIN32
	//	Sleep(10);
#else
	if(iLen == 1)
		usleep(100000);
	else if(iLen > 2000)
		usleep(300000);
	else 
		usleep(200000);
#endif


	return send_len;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  从打印机读取数据
// 作    者:  邵凯田
// 创建时间:  2014:2:21 13:35
// 参数说明:  @pBuffer为待读入缓冲区指针
//            @iLen为期望读取字节长度
// 返 回 值:  >0表示实际读取字节数，<0表示读取失败，=0表示读取超时
//////////////////////////////////////////////////////////////////////////
int SEscPrintCom::Read(BYTE *pBuffer,int iLen)
{
	int recv_len;
	recv_len = m_Com.Receive(pBuffer, iLen);
	if (recv_len < 0)
	{
		LOGERROR("从串口打印读数据失败:%d",recv_len);
		return -2;
	}

	return recv_len;
}





