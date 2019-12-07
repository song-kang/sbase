// HttpClient.cpp: implementation of the CHttpClient class.
//
//////////////////////////////////////////////////////////////////////

#include "HttpClient.h"
#include "Base64.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHttpClient::CHttpClient()
{
	m_pRecvBuffer = new BYTE[C_MAC_RECV_BUFFERSIZE];
}

CHttpClient::~CHttpClient()
{
	if(m_pRecvBuffer != NULL)
		delete[] m_pRecvBuffer;
}

bool CHttpClient::Connect(YsString ip,int port)
{
	m_ip = ip;
	m_iPort = port;
	m_skt.CreateTcp();
	m_skt.SetTimeout(3000,500);
	if(!m_skt.Connect(ip,port))
	{
		return false;
	}
	return true;
}

void CHttpClient::Close()
{
	m_skt.Close();
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  HTTP Get方法，并返回结果字符串
// 作    者:  邵凯田
// 创建时间:  2013-3-2 13:02
// 参数说明:  
// 返 回 值:  true/false
//////////////////////////////////////////////////////////////////////////
bool CHttpClient::HttpGet(YsString url,int &iResCode,YsString &sResHead,YsString &sResult)
{
	YSDEBUG("HttpGet:%s:%d%s",m_ip.data(),m_iPort,url.data());
	if(m_skt.IsConnected() == false)
	{
		if(!Connect(m_ip,m_iPort))
		{
			YSERROR("Connect HTTP(%s:%d) Error!",m_ip.data(),m_iPort);
			return false;
		}
	}
	url.replace(" ","%20");
	//Authorization: Basic YWRtaW46YWRtaW4=
	/*
	string str="admin:admin";
	ZBase64 z;
	string resstr = z.Encode((unsigned char*)str.c_str(),str.length());
	YsString base64pwd;
	base64pwd.sprintf("Authorization: Basic %s\r\n",resstr.c_str());
	*/

	//char * szHeader= "Accept: */*\r\n"
	YsString sMsg=url;
	YsString sHeader= "Accept: application/xml\r\n"
				      "Accept-Language: zh-cn\r\n"
					  "User-Agent: SktHttpClient\r\n"
					  "Host: "+m_ip+":"+YsString::toString(m_iPort)+"\r\n"
					  //+base64pwd+
					  "Connection: Keep-Alive\r\n\r\n";//Closed
	sMsg +=  " HTTP/1.1\r\n"+sHeader;
			//"Accept: application/xml\r\n"
			//"Accept-Language: zh-cn\r\n"
			//"\r\n";
	sMsg = "GET "+sMsg;

//	YSDEBUG("SEND:%s",sMsg.data());
	if(m_skt.Send(sMsg.data(),sMsg.length()) != sMsg.length())
	{
		YSERROR("Send Error!");
		return false;
	}
	int ret;
	m_pRecvBuffer[0] = '\0';
	ret = m_skt.ReceiveOnce((char *)m_pRecvBuffer, C_MAC_RECV_BUFFERSIZE-1);

	if(ret >= 0)
		m_pRecvBuffer[ret] = '\0';
	char *pBody = strstr((char *)m_pRecvBuffer,"\r\n\r\n");
	YsString sRawXml="";
	if(pBody)
	{
		pBody += 4;
		sResHead = YsString::toString((char *)m_pRecvBuffer,pBody-(char *)m_pRecvBuffer);
		sRawXml = pBody;
	}
	else
		sResHead = (char *)m_pRecvBuffer;
	iResCode = YsString::GetIdAttributeI(2,sResHead," ");

	YSDEBUG("RECV:%s",sResHead.data());
	if(iResCode == 401)
	{
		YsString sTemp;
		MD5 md5;
		//WWW-Authenticate: Digest realm="iMC RESTful Web Services", qop="auth", nonce="MTM2MjQ3OTM4MDY4NDozMzVlZTA1MTg2NDRiYTVhOTFhOWQxOTdmZGY5NzM2MA=="
		YsString www_auth=GetHttpHeadLine(sResHead,"WWW-Authenticate: ");
		if(www_auth.length() > 0)
			www_auth = www_auth.mid(www_auth.Find("realm="));
		www_auth += ", ";
		YsString realm=YsString::GetAttributeValue(www_auth,"realm","=\"","\", ");
		YsString qop=YsString::GetAttributeValue(www_auth,"qop","=\"","\", ");
		YsString nonce=YsString::GetAttributeValue(www_auth,"nonce","=\"","\", ");
		if(qop.Left(1)=="\"")
			qop = qop.mid(1);
		if(nonce.Left(1)=="\"")
			nonce = nonce.mid(1);
		YSDEBUG("realm=%s, qop=%s, nonce=%s",realm.data(),qop.data(),nonce.data());
		static int sn=1;
		sTemp=YsString::toFormat("skt%d%sskt001",sn++,YsDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz").data());
		md5.update(sTemp.data(),sTemp.length());
		YsString cnonce=md5.toString().data();
		md5.reset();
		static int inc=1;
		YsString nc;
		nc.sprintf("%08d",inc++);

		sTemp = m_user+":"+realm+":"+m_password;
		md5.update(sTemp.data(),sTemp.length());
		string HA1 = md5.toString();
//		YSDEBUG("HA1=%s=%s",sTemp.data(),HA1.data());
		md5.reset();

		sTemp.sprintf("GET:%s",url.data());
		md5.update(sTemp.data(),sTemp.length());
		string HA2 = md5.toString();
//		YSDEBUG("HA2=%s=%s",sTemp.data(),HA2.data());
		md5.reset();

		//HA1:nouce:nc:cnonce:qop:HA2
		sTemp.sprintf("%s:%s:%s:%s:%s:%s",HA1.data(),nonce.data(),nc.data(),cnonce.data(),qop.data(),HA2.data());
		md5.update(sTemp.data(),sTemp.length());
		string resstr = md5.toString();
//		YSDEBUG("HA3=%s=%s",sTemp.data(),resstr.data());
		//Authorization: Digest username="admin",realm="iMC RESTful Web Services",nonce="MTM2MjQ3OTM4MDY4NDozMzVlZTA1MTg2NDRiYTVhOTFhOWQxOTdmZGY5NzM2MA==",uri="/imcrs/plat/res/device",cnonce="e123868e88eb5609de4a372468683f2f",nc=00000001,response="3d78ad351496183baf46320189e8a7bc",qop="auth"
		sTemp.sprintf("Authorization: Digest username=\"%s\",realm=\"%s\",nonce=\"%s\",uri=\"%s\",cnonce=\"%s\",nc=%s,response=\"%s\",qop=\"%s\"\r\n",
					m_user.data(),realm.data(),nonce.data(),url.data(),cnonce.data(),nc.data(),resstr.data(),qop.data());
		sMsg = url;
		YsString sHeader= "Accept: application/xml\r\n"
						  "Accept-Language: zh-cn\r\n"
						  "User-Agent: SktHttpClient\r\n"
						  "Host: "+m_ip+":"+YsString::toString(m_iPort)+"\r\n"
						  +sTemp+
						  "Cookie: JSESSIONID=BC9FE462B00D1DD438E913242847D7A7\r\n"//Closed
						  "Connection: Keep-Alive\r\n"
						  "\r\n";//Closed
		sMsg +=  " HTTP/1.1\r\n"+sHeader;
		sMsg = "GET "+sMsg;

		YSDEBUG("SEND:%s",sMsg.data());
		if(m_skt.Send(sMsg.data(),sMsg.length()) != sMsg.length())
		{
			YSERROR("SEND error!");
			return false;
		}
		int ret;
		m_pRecvBuffer[0] = '\0';
		ret = m_skt.ReceiveOnce((char *)m_pRecvBuffer, C_MAC_RECV_BUFFERSIZE-1);

		if(ret >= 0)
		{
			m_pRecvBuffer[ret] = '\0';
			YSDEBUG("RECV(len=%d):%s\n",ret,(char *)m_pRecvBuffer);
		}
		char *pBody = strstr((char *)m_pRecvBuffer,"\r\n\r\n");
		if(pBody)
		{
//			YSDEBUG("FIRST:%s\n",pBody);
			//pBody += 4;
			sResHead = YsString::toString((char *)m_pRecvBuffer,pBody-(char *)m_pRecvBuffer);
			//sResult = pBody;
			sRawXml = pBody;
		}
		else
			sResHead = (char *)m_pRecvBuffer;
		iResCode = YsString::GetIdAttributeI(2,sResHead," ");
		YSDEBUG("RECV:%s",sResHead.data());
	}
	while(1)
	{
		m_pRecvBuffer[0] = '\0';
		ret = m_skt.ReceiveOnce((char *)m_pRecvBuffer, C_MAC_RECV_BUFFERSIZE-1);
		if(ret >= 0)
			m_pRecvBuffer[ret] = '\0';
		YSDEBUG("continue recv ret=%d,str=%s\n",ret,(char *)m_pRecvBuffer);
		if(ret <= 0)
		{
			break;
		}
		sRawXml += (char *)m_pRecvBuffer;
	}
	YsString sLen;
	int p1=0,p2,len;
	YSDEBUG("RAW RECV(%d):%s\n\n",sRawXml.length(),sRawXml.data());
	while(1)
	{
		p1 = sRawXml.find("\r\n");
		if(p1 < 0)
		{
			YSDEBUG("p1=%d\n",p1);
			break;
		}
		p2 = sRawXml.find("\r\n",p1+1);
		if(p1+2==p2)
		{
			p1 = p2;
			p2 = sRawXml.find("\r\n",p1+1);
		}
		if(p2 < 0)
		{
			YSDEBUG("p1=%d,p2=%d\n",p1,p2);
			break;
		}
		if(p2-p2 > 10)
		{
			YSDEBUG("p1=%d,p2=%d\n",p1,p2);
			break;
		}
		sLen = "0X"+sRawXml.mid(p1+2,p2-p1-2);
		len = sLen.toInt();
//		YSDEBUG("sLen=%s,len=%d\n",sLen.data(),len);
		if(len == 0)
		{
			YSDEBUG("err len=0\n");
			break;
		}
		sResult += sRawXml.mid(p2+2,len);
		sRawXml = sRawXml.mid(p2+2+len);
	}
	if(sResult.length() == 0)
		sResult = sRawXml;
	YSDEBUG("XML RECV(%d):%s\n\n",sResult.length(),sResult.data());
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  查找HTTP头字符中的指定字符开关的行
// 作    者:  邵凯田
// 创建时间:  2013-3-6 9:03
// 参数说明:  
// 返 回 值:  
//////////////////////////////////////////////////////////////////////////
YsString CHttpClient::GetHttpHeadLine(YsString &sHead,YsString sLineBegin)
{
	YsString str;
	int pos = sHead.Find(sLineBegin);
	if(pos>=0)
	{
		str = sHead.mid(pos,sHead.Find("\r\n",pos)-pos);
	}
	return str;
}
