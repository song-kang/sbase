/**
 *
 * �� �� �� : SDecodPub.h
 * �������� : 2015-5-8 14:18
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : �����ṹ����
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-5-8	�ۿ�������ļ�
 *
 **/

#include "SApi.h"

#define SDECODE_OFFLINE //���߷���Ԥ���뿪��

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

//    IP�������
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
	unsigned char   ver_len;        // �汾 + �ײ�����

	unsigned char   tos;            // ��������
	unsigned short  len;            // �ܳ���
	unsigned short  id;             // ��ʶ
	unsigned short  id_off;         // 3Ϊƫ�ƺ�13λƬƫ��
	unsigned char   ttl;            // ����ʱ��
	unsigned char   ptl;            // Э��
	unsigned short  sum;            // �ײ�У���
	unsigned int    src_ip;         // ԴIP
	unsigned int    dst_ip;         // Ŀ��IP

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
	unsigned short src_port;    // Դ�˿�
	unsigned short dst_port;    // Ŀ��˿�
	unsigned int   seq_num;     // �������
	unsigned int   ack_num;     // ȷ�����

	unsigned short len_flags;   // �ײ����� + ��ʶλ('s)
	unsigned short window;      // �������ڴ�С
	unsigned short chk_sum;     // У���
	unsigned short urg_ptr;     // ����ָ��

	inline unsigned short GetWindowSize(){return ntohs(window)&0x7fff;};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  TCP flagλ��ȡ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-5-9 21:31
	// ����˵��:  
	// �� �� ֵ:  
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
	// ��    ��:  ȡTCPͷ���ĳ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-5-10 10:25
	// ����˵��:  
	// �� �� ֵ:  
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
	unsigned short src_port;    // Դ�˿�
	unsigned short dst_port;    // Ŀ�Ķ˿�
	unsigned short len;         // UDP����
	unsigned short chk_sum;     // У���

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡUDPͷ�����������ܳ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-5-10 10:52
	// ����˵��:  
	// �� �� ֵ:  
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
	// ��    ��:  ȡUDP��������Ч��Ϣ�ܳ���(-8�ֽڵ�ͷ��)
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-5-10 10:54
	// ����˵��:  
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	inline int GetUdpDataLen(){return GetUdpHeadDataLen()-8;};

};

struct SDECODE_VLAN_HEADER
{
	unsigned short prior_num;	//���ȼ���CFI��VLAN��
	unsigned short prot;		//�²�Э������(����˳��)
	unsigned char GetPriority()	{return prior_num >> 13;};
	unsigned char GetCFI()		{return (prior_num >> 12)&1;};
	unsigned short GetVLanNum()	{return prior_num&0x0FFF;};
};

#pragma pack()

