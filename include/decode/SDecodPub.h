/**
 *
 * 文 件 名 : SDecodPub.h
 * 创建日期 : 2015-5-8 14:18
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : 公共结构定义
 * 修改记录 : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-5-8	邵凯田　创建文件
 *
 **/

#include "SApi.h"

#define SDECODE_OFFLINE //离线分析预编译开关

#pragma pack(1)

struct SDECODE_MAC_HEADER
{
	unsigned char dst_mac_addr[6];
	unsigned char src_mac_addr[6];
	unsigned short type;
};


struct SDECODE_IPV4
{
	unsigned int ip;
	inline unsigned char GetIpSector1()
	{
#ifdef _SKT_NUMBER_SWAP
		return ip >> 24;
#else
		return ip & 0xff;
#endif
	}
	inline unsigned char GetIpSector2()
	{
#ifdef _SKT_NUMBER_SWAP
		return (ip >> 16) & 0xff;
#else
		return (ip >> 8) & 0xff;
#endif
	}
	inline unsigned char GetIpSector3()
	{
#ifdef _SKT_NUMBER_SWAP
		return (ip >> 8) & 0xff;
#else
		return (ip >> 16) & 0xff;
#endif
	}
	inline unsigned char GetIpSector4()
	{
#ifdef _SKT_NUMBER_SWAP
		return ip & 0xff;
#else
		return (ip >> 24);
#endif
	}
};

//    IP报文组成
//    ----------------------------------------------
//    | ip header | tcp header(or x header) | data | 
//    ----------------------------------------------
//

//    IP header structure:
//  0                   1                   2                   3   
// 	0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 
// 	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// 	|Version|  IHL  |Type of Service|          Total Length         |
// 	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// 	|         Identification        |Flags|      Fragment Offset    |
// 	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// 	|  Time to Live |    Protocol   |         Header Checksum       |
// 	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// 	|                       Source Address                          |
// 	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// 	|                    Destination Address                        |
// 	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// 	|                    Options                    |    Padding    |
// 	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

struct SDECODE_IP_HEADER // IP HEADER: 20 bytes ( = 160 bits )
{
	unsigned char   ver_len;        // 版本 + 首部长度

	unsigned char   tos;            // 服务类型
	unsigned short  len;            // 总长度
	unsigned short  id;             // 标识
	unsigned short  id_off;         // 3为偏移和13位片偏移
	unsigned char   ttl;            // 生存时间
	unsigned char   ptl;            // 协议
	unsigned short  sum;            // 首部校验和
	unsigned int    src_ip;         // 源IP
	unsigned int    dst_ip;         // 目标IP

	inline unsigned char GetIpVer()
	{
		return ver_len>>4;
	}
	inline unsigned char GetIpHeadLen()
	{
		return (ver_len&0x0f)<<2;
	}
	inline SString GetSrcIpStr()
	{
		BYTE *p = (BYTE*)&src_ip;
#ifdef _SKT_NUMBER_SWAP
		return SString::toFormat("%d.%d.%d.%d",p[3],p[2],p[1],p[0]);
#else
		return SString::toFormat("%d.%d.%d.%d",p[0],p[1],p[2],p[3]);
#endif
	}
	inline SString GetDstIpStr()
	{
		BYTE *p = (BYTE*)&dst_ip;
#ifdef _SKT_NUMBER_SWAP
		return SString::toFormat("%d.%d.%d.%d",p[3],p[2],p[1],p[0]);
#else
		return SString::toFormat("%d.%d.%d.%d",p[0],p[1],p[2],p[3]);
#endif
	}
};

//    TCP header structure:
//  0                1                2                3 
// 	0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 
// 	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// 	|       Source Port       |    Destination Port        |
// 	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// 	|                      Sequence Number                      |
// 	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// 	|                    Acknowledgment Number                   |
// 	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// 	|   Data |           |U|A|P|R|S|F|                            |
// 	| Offset| Reserved   |R|C|S|S|Y|I|          Window          |
// 	|    |           |G|K|H|T|N|N|                            |
// 	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// 	|           Checksum          |       Urgent Pointer        |
// 	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// 	|                    Options                    | Padding |
// 	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// 	|                             data                            |
// 	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

struct SDECODE_TCP_HEADER // TCP HEADER
{
	unsigned short src_port;    // 源端口
	unsigned short dst_port;    // 目标端口
	unsigned int   seq_num;     // 报文序号
	unsigned int   ack_num;     // 确认序号

	unsigned short len_flags;   // 首部长度 + 标识位('s)
	unsigned short window;      // 滑动窗口大小
	unsigned short chk_sum;     // 校验和
	unsigned short urg_ptr;     // 紧急指针

	inline unsigned short GetWindowSize(){return ntohs(window)&0x7fff;};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  TCP flag位提取
	// 作    者:  邵凯田
	// 创建时间:  2015-5-9 21:31
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	inline unsigned char GetFlag()
	{
#ifdef _SKT_NUMBER_SWAP
		return len_flags&0xff;
#else
		return (len_flags>>8);
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取TCP头部的长度
	// 作    者:  邵凯田
	// 创建时间:  2015-5-10 10:25
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	inline unsigned char GetTcpHeadLen()
	{
#ifdef _SKT_NUMBER_SWAP
		return (len_flags >> 12) << 2;
#else
		return (len_flags&0xF0)>>2;
#endif
	}
	inline bool IsSetFlagFin()	{return (GetFlag()&0x01) != 0;};
	inline bool IsSetFlagSyn()	{return (GetFlag()&0x02) != 0;};
	inline bool IsSetFlagReset()	{return (GetFlag()&0x04) != 0;};
	inline bool IsSetFlagPush()	{return (GetFlag()&0x08) != 0;};
	inline bool IsSetFlagAck()	{return (GetFlag()&0x10) != 0;};
	inline bool IsSetFlagUrgent()	{return (GetFlag()&0x20) != 0;};
	inline bool IsSetFlagECNEcho(){return (GetFlag()&0x40) != 0;};
	inline bool IsSetFlagCWR()	{return (GetFlag()&0x80) != 0;};
	inline bool IsSetFlagNonce()	
	{
#ifdef _SKT_NUMBER_SWAP
		return ((len_flags>>8)&0x01) != 0;
#else
		return ((len_flags&0xff)*0x01) != 0;
#endif
	};
};

//    UDP header structure:
//	 0      7 8     15 16    23 24    31  
//	+--------+--------+--------+--------+ 
//	|     Source      |   Destination   | 
//	|      Port       |      Port       | 
//	+--------+--------+--------+--------+ 
//	|                 |                 | 
//	|     Length      |    Checksum     | 
//	+--------+--------+--------+--------+ 
//	|                                     
//	|          data octets ...            
//	+---------------- ...                 

struct SDECODE_UDP_HEADER
{
	unsigned short src_port;    // 源端口
	unsigned short dst_port;    // 目的端口
	unsigned short len;         // UDP长度
	unsigned short chk_sum;     // 校验和

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取UDP头部及数据区总长度
	// 作    者:  邵凯田
	// 创建时间:  2015-5-10 10:52
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	inline int GetUdpHeadDataLen()
	{
#ifdef _SKT_NUMBER_SWAP
		return len;
#else
		return ntohs(len);
#endif
	};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取UDP数据区有效信息总长度(-8字节的头部)
	// 作    者:  邵凯田
	// 创建时间:  2015-5-10 10:54
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	inline int GetUdpDataLen(){return GetUdpHeadDataLen()-8;};

};

struct SDECODE_VLAN_HEADER
{
	unsigned short prior_num;	//优先级、CFI、VLAN号
	unsigned short prot;		//下层协议类型(网络顺序)
	unsigned char GetPriority()	{return prior_num >> 13;};
	unsigned char GetCFI()		{return (prior_num >> 12)&1;};
	unsigned short GetVLanNum()	{return prior_num&0x0FFF;};
};

#pragma pack()

