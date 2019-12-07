/**
 *
 * 文 件 名 : HttpServer.cpp
 * 创建日期 : 2013-5-23 13:14
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : HTTP简易通讯服务类
 * 修改记录 : 
 *            $Log: $
 *
 **/


#include "HttpServer.h"

CUrlEncoding::CUrlEncoding()
{

}

CUrlEncoding::~CUrlEncoding()
{

}

//把str编码为网页中的 GB2312 url encode ,英文不变，汉字双字节 如%3D%AE%88
YsString CUrlEncoding::toUrlGB2312(char * str)
{
    YsString dd;
    size_t len = strlen(str);
    for (size_t i=0;i<len;i++)
    {
        if(isalnum((BYTE)str[i]))
        {
            char tempbuff[2];
            sprintf(tempbuff,"%c",str[i]);
            dd += tempbuff;
        }
        else if (isspace((BYTE)str[i]))
        {
            dd += "+";
        }
        else
        {
            char tempbuff[4];
            sprintf(tempbuff,"%%%X%X",((BYTE*)str)[i] >>4,((BYTE*)str)[i] %16);
            dd += tempbuff;
        }

    }
    return dd;
}

//把str编码为网页中的 UTF-8 url encode ,英文不变，汉字三字节 如%3D%AE%88

YsString CUrlEncoding::toUrlUTF8(char * str)
{
    YsString tt;
    YsString dd;
    tt = str;
	tt = tt.toUtf8();

    size_t len=tt.length();
    for (size_t i=0;i<len;i++)
    {
        if(isalnum((BYTE)tt.at(i)))
        {
            char tempbuff[2]={0};
            sprintf(tempbuff,"%c",(BYTE)tt.at(i));
            dd += tempbuff;
        }
        else if (isspace((BYTE)tt.at(i)))
        {
            dd += "+";
        }
        else
        {
            char tempbuff[4];
            sprintf(tempbuff,"%%%X%X",((BYTE)tt.at(i)) >>4,((BYTE)tt.at(i)) %16);
            dd += tempbuff;
        }

    }
    return dd;
}

//解Url时使用
char CUrlEncoding::CharToInt(char ch)
{
        if(ch>='0' && ch<='9')return (char)(ch-'0');
        if(ch>='a' && ch<='f')return (char)(ch-'a'+10);
        if(ch>='A' && ch<='F')return (char)(ch-'A'+10);
        return -1;
}
char CUrlEncoding::StrToBin(char *str)
{
        char tempWord[2];
        char chn;

        tempWord[0] = CharToInt(str[0]);                         //make the B to 11 -- 00001011
        tempWord[1] = CharToInt(str[1]);                         //make the 0 to 0 -- 00000000

        chn = (tempWord[0] << 4) | tempWord[1];                //to change the BO to 10110000

        return chn;
}
//把url GB2312解码
YsString CUrlEncoding::UrlGB2312Decode(YsString str)
{
	YsString output="";
    char tmp[2];
    int i=0,idx=0,len=str.length();
   
    while(i<len)
	{
        if(str[i]=='%')
		{
            tmp[0]=str[i+1];
            tmp[1]=str[i+2];
            output += StrToBin(tmp);
            i=i+3;
        }
        else if(str[i]=='+')
		{
            output+=' ';
            i++;
        }
        else
		{
            output+=str[i];
            i++;
        }
    }
   
    return output;
}
//把url utf8解码
YsString CUrlEncoding::UrlUTF8Decode(YsString str)
{
    YsString output="";
    YsString temp =UrlGB2312Decode(str);
	temp = temp.toGb2312();
    return output;
}


//////////////////////////////////////////////////////////////////////////
// 描    述:  取HTTP字字符
// 作    者:  邵凯田
// 创建时间:  2013-5-24 14:00
// 参数说明:  
// 返 回 值:  
//////////////////////////////////////////////////////////////////////////
void CHttpServerSession::GetHttpHead(YsString &sHead,YsString sContentType)
{
	sHead = "HTTP/1.1 200 OK\r\n"
		"Date: "+YsDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")+
		"Cache-Control: max-age=3600, must-revalidate, public\r\n"
		"Connection: close\r\n"//Keep-Alive
		"Server: SktHttpServer(V1.0.0) skt001@163.com\r\n"
		"Content-Type: "+sContentType+"\r\n"
		"Last-Modified: Sun, 30 Dec 2012 14:49:22 GMT\r\n"
		"\r\n";
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  发送HTML头
// 作    者:  邵凯田
// 创建时间:  2013-5-24 13:39
// 参数说明:  @sTitle为页面标题
// 返 回 值:  int
//////////////////////////////////////////////////////////////////////////
int CHttpServerSession::SendHead(YsString sTitle)
{
	YsString sHtml;
	GetHttpHead(sHtml);
	sHtml +="<HTML>\r\n"
			"<head>\r\n"
			"<meta http-equiv='Content-Type' content='text/html; charset=gb2312'>\r\n"
			"<title>"+sTitle+"</title>\r\n";
	sHtml +="<style>\r\n"
			"body		"
			"{    \r\n"
			"	font-family: '宋体','Lucida Sans Unicode',Arial,Verdana,sans-serif; \r\n"
			"	font-size: 12px;    \r\n"
			"	BACKGROUND-COLOR: #EFF2F7; \r\n"
			"	MARGIN: 0px 0px 0px 0px;\r\n"
			"}    \r\n"
			"     \r\n"
			"table {"
			"	cellPadding: 0px;"
			"	border-spacing: 0px;"
			"	empty-cells: show;\r\n"
			"}    \r\n"
			"TD\r\n"
			"{\r\n"
			"    FONT: 9pt 宋体;\r\n"
			"    TEXT-TRANSFORM: none\r\n"
			"}\r\n"
			".list_th   \r\n"
			"{    \r\n"
			"HEIGHT: 24px;\r\n"
			"BACKGROUND-COLOR: #9EBBDB;\r\n"
			"TEXT-ALIGN2: center\r\n"
			"font-family: '宋体','Lucida Sans Unicode',Arial,Verdana,sans-serif;  \r\n"
			"font-size: 12px;     \r\n"
			"font-weight: bold;   \r\n"
			"white-space: nowrap; \r\n"
			"}    \r\n"
			"     \r\n"
			".list_td   \r\n"
			"{    \r\n"
			"font-family: '宋体','Lucida Sans Unicode',Arial,Verdana,sans-serif;  \r\n"
			"font-size: 12px;     \r\n"
			"HEIGHT: 20px;\r\n"
			"BORDER-BOTTOM: #9CBE52 1px solid;\r\n"
			"BACKGROUND-COLOR: #EFF2F7\r\n"
			"}    \r\n"
			".list_td2   \r\n"
			"{    \r\n"
			"font-family: '宋体','Lucida Sans Unicode',Arial,Verdana,sans-serif;  \r\n"
			"font-size: 12px;     \r\n"
			"	HEIGHT: 24px;\r\n"
			"	BACKGROUND-COLOR: #EEEEFF;\r\n"
			"	TEXT-ALIGN2: center\r\n"
			"}    \r\n"
			"     \r\n"
			"span.leftmenuitem    \r\n"
			"{    \r\n"
			"font-family: '宋体','Lucida Sans Unicode',Arial,Verdana,sans-serif;  \r\n"
			"font-size: 12px;     \r\n"
			"font-weight: bold;   \r\n"
			"}    \r\n"
			"     \r\n"
			"canvas "
			"{    \r\n"
			"font-family: '宋体','Lucida Sans Unicode',Arial,Verdana,sans-serif;  \r\n"
			"font-size: 9px;      \r\n"
			"}    \r\n"
			"     \r\n"
			"     \r\n"
			".wrap {"
			"white-space: normal; \r\n"
			"}    \r\n"
			"     \r\n"
			"address"
			"{font-family: '宋体','Lucida Sans Unicode',Arial,Verdana,sans-serif; \r\n"
			"font-size: 8pt}      \r\n"
			"     \r\n"
			"h5   \r\n"
			"{font-family: '宋体','Lucida Sans Unicode',Arial,Verdana,sans-serif; \r\n"
			"font-size: 8pt}      \r\n"
			"     \r\n"
			"form \r\n"
			"{font-family: '宋体','Lucida Sans Unicode',Arial,Verdana,sans-serif; \r\n"
			"font-size: 12px}     \r\n"
			"     \r\n"
			"h1   \r\n"
			"{font-family: Helvetica, Verdana, Arial, sans-serif; \r\n"
			"text-decoration:underline;           \r\n"
			"text-align: center;  \r\n"
			"font-size: 16pt;     \r\n"
			"font-weight: bold}   \r\n"
			"     \r\n"
			"h2   \r\n"
			"{font-family: Verdana, Arial, Helvetica, sans-serif; \r\n"
			"font-size: 14pt;     \r\n"
			"font-weight: bold}   \r\n"
			"     \r\n"
			"A:link "
			"{color: blue;        \r\n"
			"font-family: '宋体','Lucida Sans Unicode',Arial,Verdana,sans-serif;  \r\n"
			"font-size: 12px;     \r\n"
			"text-decoration: none} "
			"     \r\n"
			"A:visited            \r\n"
			"{color: blue;        \r\n"
			"font-family: '宋体','Lucida Sans Unicode',Arial,Verdana,sans-serif;  \r\n"
			"font-size: 12px;     \r\n"
			"text-decoration: none} "
			"     \r\n"
			"A:hover"
			"{color: red;         \r\n"
			"font-family: '宋体','Lucida Sans Unicode',Arial,Verdana,sans-serif;  \r\n"
			"font-size: 12px;     \r\n"
			"text-decoration: underline}          \r\n"
			"     \r\n"
			"A:link.nav           \r\n"
			"{color: #000000;     \r\n"
			"font-family: '宋体','Lucida Sans Unicode',Arial,Verdana,sans-serif;} \r\n"
			"     \r\n"
			"A:visited.nav        \r\n"
			"{color: #000000;     \r\n"
			"font-family: '宋体','Lucida Sans Unicode',Arial,Verdana,sans-serif;} \r\n"
			"     \r\n"
			"A:hover.nav          \r\n"
			"{color: red;         \r\n"
			"font-family: '宋体','Lucida Sans Unicode',Arial,Verdana,sans-serif;} \r\n"
			"     \r\n"
			".nav \r\n"
			"{color: #000000;     \r\n"
			"font-family: '宋体','Lucida Sans Unicode',Arial,Verdana,sans-serif;} \r\n"
			"     \r\n"
			"A.age0min { color:#FF0000 }          \r\n"
			"A.age5min { color:#FF00FF }          \r\n"
			"A.age15min { color:#FF7F00 }         \r\n"
			"A.age30min { color:#007FFF }         \r\n"
			"A.age60min { color:#0000FF }         \r\n"
			"     \r\n"
			"A.external {background: url(/external.png) center right no-repeat; padding-right: 13px; }            \r\n"
			"A.lock     {background: url(/lock.png) center right no-repeat; padding-right: 16px; }\r\n"
			"A.mailto   {background: url(/mail.gif) center right no-repeat; padding-right: 18px; }\r\n"
			"     \r\n"
			"A:visited.age0min { color:#FF0000 }  \r\n"
			"A:visited.age5min { color:#FF00FF }  \r\n"
			"A:visited.age15min { color:#FF7F00 } \r\n"
			"A:visited.age30min { color:#007FFF } \r\n"
			"A:visited.age60min { color:#0000FF } \r\n"
			"     \r\n"
			".PutdSet\r\n"
			"{\r\n"
			"    HEIGHT: 25px;\r\n"
			"    TEXT-ALIGN: right\r\n"
			"}\r\n"
			".Input\r\n"
			"{\r\n"
			"    BACKGROUND-COLOR: transparent;\r\n"
			"    HEIGHT: 22px;\r\n"
			"    COLOR: black;\r\n"
			"    FONT-FAMILY: 宋体;\r\n"
			"    FONT-SIZE: 9pt\r\n"
			"    BORDER-BOTTOM: #f1f1f1 1px solid;\r\n"
			"    BORDER-LEFT: #f1f1f1 1px solid;\r\n"
			"    BORDER-RIGHT: #f1f1f1 1px solid;\r\n"
			"    BORDER-TOP: #f1f1f1 1px solid;\r\n"
			"    COLOR: #1e65a7;\r\n"
			"    CURSOR: hand;\r\n"
			"}\r\n"
			".Button\r\n"
			"{\r\n"
			"	BACKGROUND-COLOR: #a6aad6;\r\n"
			"	FONT-SIZE: 9pt;\r\n"
			"	PADDING-TOP: 1px;\r\n"
			"	PADDING-RIGHT: 2px;\r\n"
			"	PADDING-LEFT: 2px;\r\n"
			"	BORDER-RIGHT: #046296 1px solid;\r\n"
			"	BORDER-TOP: #1182C0 1px solid;\r\n"
			"	BORDER-LEFT: #046296 1px solid;\r\n"
			"	BORDER-BOTTOM: #046296 1px solid;\r\n"
			"	FONT-FAMILY: '宋体','Arial', 'Helvetica', 'sans-serif';\r\n"
			"   COLOR: #1e25a7;\r\n"
			"	HEIGHT: 22px;\r\n"
			"}\r\n"
			".Window_Menubar\r\n"
			"{\r\n"
			"	font-weight: normal;\r\n"
			"	font-size: 9pt;\r\n"
			"	background: #b6d6f5;\r\n"
			"	width: 100%;\r\n"
			"	color: #ffffff;\r\n"
			"	font-family: 宋体;\r\n"
			"	height: 24px;\r\n"
			"	text-decoration: none;\r\n"
			"}\r\n"

			".Window_Body\r\n"
			"{\r\n"
			"	font-weight: normal;\r\n"
			"	font-size: 9pt;\r\n"
			"	background: #ffffff;\r\n"
			"	color: #000000;\r\n"
			"	font-family: 宋体;\r\n"
			"	text-decoration: none;\r\n"
			"}\r\n"
			".Window_Footer\r\n"
			"{\r\n"
			"	font-size: 9pt;\r\n"
			"	background: #a5be52;\r\n"
			"	color: #ffffff;\r\n"
			"	font-family: 宋体;\r\n"
			"	height: 16px;\r\n"
			"	text-decoration: none;\r\n"
			"}\r\n"
			".Window_Label\r\n"
			"{\r\n"
			"	background: #cccccc;\r\n"
			"	margin: 0px;\r\n"
			"	width: 100%;\r\n"
			"	height: 24px;\r\n"
			"	padding-top: 0px;\r\n"
			"	padding-right: 0px;\r\n"
			"	padding-left: 0px;\r\n"
			"	padding-bottom: 2px;\r\n"
			"	font-weight: normal;\r\n"
			"	font-family: 宋体;\r\n"
			"	font-size: 9pt;\r\n"
			"	color: black;\r\n"
			"	border-top-style: none;\r\n"
			"	border-right-style: none;\r\n"
			"	border-left-style: none;\r\n"
			"	border-bottom-style: none;\r\n"
			"	text-decoration: none;\r\n"
			"}\r\n"
			".Window_TableBg\r\n"
			"{\r\n"
			"	bordercolor: #FFFFFF;\r\n"
			"}\r\n"
			".Window_Table\r\n"
			"{\r\n"
			"	border: black 10px solid\r\n"
			"}\r\n"
			".Table_noBoard\r\n"
			"{\r\n"
			"	BORDER-BOTTOM: black 1px solid;\r\n"
			"	BORDER-LEFT: black 1px solid;\r\n"
			"	BORDER-RIGHT: black 1px solid;\r\n"
			"	BORDER-TOP: black 1px solid;\r\n"
			"	font-weight: normal;\r\n"
			"	font-size: 9pt;\r\n"
			"	background: #ffffff;\r\n"
			"	color: #000000;\r\n"
			"	font-family: 宋体;\r\n"
			"	text-decoration: none;\r\n"
			"}\r\n"
			".Window_TbT\r\n"
			"{\r\n"
			"	HEIGHT: 24px;\r\n"
			"	BACKGROUND-COLOR: #9EBBDB;\r\n"
			"	TEXT-ALIGN2: center\r\n"
			"}\r\n"
			".Window_TbB2\r\n"
			"{\r\n"
			"	HEIGHT: 24px;\r\n"
			"	BACKGROUND-COLOR: #EEEEFF;\r\n"
			"	TEXT-ALIGN2: center\r\n"
			"}\r\n"
			".Window_TbB\r\n"
			"{\r\n"
			"	HEIGHT: 20px;\r\n"
			"  BORDER-BOTTOM: #9CBE52 1px solid;\r\n"
			"	BACKGROUND-COLOR: #EFF2F7\r\n"
			"}\r\n"
			".Window_TbE\r\n"
			"{\r\n"
			"	HEIGHT: 20px;\r\n"
			"	BACKGROUND-COLOR: #b6d6f5\r\n"
			"}\r\n"
			"Input\r\n"
			"{\r\n"
			"    BACKGROUND-COLOR: transparent;\r\n"
			"    HEIGHT2: 22px;\r\n"
			"    COLOR: black;\r\n"
			"    FONT-FAMILY: 宋体;\r\n"
			"    FONT-SIZE: 9pt\r\n"
			"    BORDER-BOTTOM: #f1f1f1 1px solid;\r\n"
			"    BORDER-LEFT: #f1f1f1 1px solid;\r\n"
			"    BORDER-RIGHT: #f1f1f1 1px solid;\r\n"
			"    BORDER-TOP: #f1f1f1 1px solid;\r\n"
			"    COLOR: #1e65a7;\r\n"
			"    CURSOR: hand;\r\n"
			"}\r\n"
			".InputHelp\r\n"
			"{\r\n"
			"    BACKGROUND-COLOR: #ffffff;\r\n"
			"    HEIGHT: 22px;\r\n"
			"    COLOR: black;\r\n"
			"    FONT-FAMILY: 宋体;\r\n"
			"    FONT-SIZE: 9pt\r\n"
			"    BORDER-BOTTOM: #f1f1f1 1px solid;\r\n"
			"    BORDER-LEFT: #f1f1f1 1px solid;\r\n"
			"    BORDER-RIGHT: #f1f1f1 1px solid;\r\n"
			"    BORDER-TOP: #f1f1f1 1px solid;\r\n"
			"    COLOR: #1e65a7;\r\n"
			"    CURSOR: hand;\r\n"
			"}\r\n"
			".LoginInput\r\n"
			"{\r\n"
			"    HEIGHT: 20px;\r\n"
			"    BORDER-BOTTOM: #000000 1px solid;\r\n"
			"    BORDER-LEFT: #000000 1px solid;\r\n"
			"    BORDER-RIGHT: #000000 1px solid;\r\n"
			"    BORDER-TOP: #000000 1px solid;\r\n"
			"}\r\n"
			".LineInput\r\n"
			"{\r\n"
			"    BACKGROUND-COLOR: transparent;\r\n"
			"    BORDER-BOTTOM: #000000 1px solid;\r\n"
			"    BORDER-LEFT: 0px solid;\r\n"
			"    BORDER-RIGHT: 0px solid;\r\n"
			"    BORDER-TOP: 0px solid;\r\n"
			"    HEIGHT: 18px;\r\n"
			"    COLOR: black;\r\n"
			"    FONT-FAMILY: 宋体;\r\n"
			"    FONT-SIZE: 9pt;\r\n"
			"}\r\n"
			".PageStyle\r\n"
			"{\r\n"
			"    COLOR: #000000;\r\n"
			"    BACKGROUND-COLOR: transparent;\r\n"
			"}\r\n"
			"/************************字体、线条样式****************************************/\r\n"
			".WriteSign\r\n"
			"{\r\n"
			"    COLOR: #d42d00;\r\n"
			"    FONT-FAMILY: 宋体;\r\n"
			"    FONT-SIZE: 9pt;\r\n"
			"    FONT-WEIGHT: normal\r\n"
			"}\r\n"
			".AlertSign\r\n"
			"{\r\n"
			"    COLOR: #0E650D;\r\n"
			"    FONT-FAMILY: 宋体;\r\n"
			"    FONT-SIZE: 11pt;\r\n"
			"    FONT-WEIGHT: normal\r\n"
			"}\r\n"
			".AlarmSign\r\n"
			"{\r\n"
			"    COLOR: #000000;\r\n"
			"    FONT-FAMILY: 楷体_GB2312;\r\n"
			"    FONT-SIZE: 11pt;\r\n"
			"    FONT-WEIGHT: normal\r\n"
			"}\r\n"
			".LineSign\r\n"
			"{\r\n"
			"    COLOR: GREEN\r\n"
			"}\r\n"
			".HeadSign\r\n"
			"{\r\n"
			"    COLOR: #FFFFFF;\r\n"
			"    FONT-FAMILY: 宋体;\r\n"
			"    FONT-SIZE: 9pt;\r\n"
			"    FONT-WEIGHT: normal\r\n"
			"}\r\n"
			"</style>\r\n";
			"</head><body>\r\n";
	return m_pSocket->Send(sHtml.data(),sHtml.length());
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  发送HTML尾部
// 作    者:  邵凯田
// 创建时间:  2013-5-24 13:39
// 参数说明:  @sTitle为页面标题
// 返 回 值:  int
//////////////////////////////////////////////////////////////////////////
int CHttpServerSession::SendTail()
{
	YsString sHtml = "</body></html>";
	return m_pSocket->Send(sHtml.data(),sHtml.length());
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  发送HTML内容，同时清除字符串
// 作    者:  邵凯田
// 创建时间:  2013-5-24 13:40
// 参数说明:  @sHtml为HTML字符串
// 返 回 值:  int
//////////////////////////////////////////////////////////////////////////
int CHttpServerSession::SendHtml(YsString &sHtml)
{
	if(sHtml.length() < 10000)
		return 0;
	int ret = m_pSocket->Send(sHtml.data(),sHtml.length());
	sHtml = "";
	return ret;
}

//////////////////////////////////////////////////////////////////////////
// 名    称:  CHttpServer
// 作    者:  邵凯田
// 创建时间:  2012-5-31 17:14
// 描    述:  通讯服务
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHttpServer::CHttpServer()
{
	m_IpSessions.setShared(true);
	m_IpSessions.setAutoDelete(true);
}

CHttpServer::~CHttpServer()
{

}

////////////////////////////////////////////////////////////////////////
// 描    述:  启动服务
// 作    者:  邵凯田
// 创建时间:  2012-5-31 16:40
// 参数说明:  @port表示端口,@ip表示服务端IP地址
// 返 回 值:  true表示服务启动成功,false表示服务启动失败
//////////////////////////////////////////////////////////////////////////
bool CHttpServer::Start(int port,YsString ip)
{
	m_sServerIp = ip;
	m_iServerPort = port;
	if(!YsService::Start())
	{
		return false;
	}
	YS_CREATE_THREAD(ThreadListen,this);
	YS_CREATE_THREAD(ThreadLoop,this);
	return true;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  停止服务
// 作    者:  邵凯田
// 创建时间:  2012-5-31 16:40
// 参数说明:  void
// 返 回 值:  true表示服务停止成功,false表示服务停止失败
//////////////////////////////////////////////////////////////////////////
bool CHttpServer::Stop()
{
	return YsService::Stop();
}


////////////////////////////////////////////////////////////////////////
// 描    述:  服务端帧听线程
// 作    者:  邵凯田
// 创建时间:  2011-11-8 19:41
// 参数说明:  this
// 返 回 值:  NULL
//////////////////////////////////////////////////////////////////////////
void* CHttpServer::ThreadListen(void* lp)
{
	CHttpServer *pThis = (CHttpServer*)lp;
	pThis->BeginThread();
	YsSocket *pSkt = &pThis->m_Socket;
	YsSocket *pNewSkt = NULL;

	while(!pThis->IsQuit())
	{
		pSkt->CreateTcp();
		if( !pSkt->Bind(pThis->m_iServerPort,pThis->m_sServerIp.length()==0?NULL:pThis->m_sServerIp.data()) || 
			!pSkt->Listen())
		{
			YSERROR("绑定服务端口(%s:%d)时失败!",pThis->m_sServerIp.data(),pThis->m_iServerPort);
			for(int i=0;!pThis->IsQuit() && i<100;i++)
				YsApi::YsSleep(50000);
			continue;
		}
		YSDEBUG("在端口[%d]启动HTTP服务成功!",pThis->m_iServerPort);
		while(!pThis->IsQuit())
		{
			YsApi::YsSleep(100000);
			if(pNewSkt != NULL)
			{
				pNewSkt->Close();
				delete pNewSkt;
			}
			pNewSkt = new YsSocket();
			if(!pSkt->Accept(*pNewSkt))
			{
				YSERROR("在端口(%d)接受连接时失败!",pThis->m_iServerPort);
				break;
			}
			YSWARN("得到新的会话请求!(%s:%d --> %s:%d)",
					pNewSkt->GetPeerIp().data(),pNewSkt->GetPeerPort(),
					pThis->m_sServerIp.data(),pThis->m_iServerPort);
			if(!pThis->OnLogin(pThis->m_sServerIp.data(),pThis->m_iServerPort))
			{
				YSDEBUG("登录请求被上层拒绝!");
				YsApi::YsSleep(500000);
				continue;
			}
			CHttpServerSession *pNewSession = new CHttpServerSession();
			pNewSession->m_pServer = pThis;
			pNewSession->m_pSocket = pNewSkt;
			pNewSession->m_pIpSession = pThis->GetIpSession(pNewSkt->GetPeerIp());
			pThis->m_Sessions.append(pNewSession);
			//新会话回调
			pThis->OnNewSession(pNewSession);
			YS_CREATE_THREAD(ThreadService,pNewSession);
			pNewSkt = NULL;
		}
	}
	pThis->EndThread();
	return NULL;
}

void* CHttpServer::ThreadLoop(void* lp)
{
	CHttpServer *pThis = (CHttpServer*)lp;
	pThis->BeginThread();
	int i,soc;

	while(!pThis->IsQuit())
	{
		soc = ::time(NULL);
		for(i=0;i<pThis->m_IpSessions.count();i++)
		{
			CHttpIpSession *p = pThis->m_IpSessions[i];
			if(p == NULL)
				continue;
			if(abs(p->m_iLastOperTime-soc) > 600)//10分钟以上退出登录
			{
				pThis->m_IpSessions.remove(p);
				i--;
				continue;
			}
		}
		YsApi::YsSleep(2000000);
	}
	pThis->EndThread();
	return NULL;
}

CHttpIpSession* CHttpServer::GetIpSession(YsString ip)
{
	unsigned long pos;
	CHttpIpSession *p = m_IpSessions.FetchFirst(pos);
	while(p)
	{
		if(p->m_ip == ip)
			break;
		p = m_IpSessions.FetchNext(pos);
	}
	if(p == NULL)
	{
		p = new CHttpIpSession();
		p->m_ip = ip;
		p->m_user = "";
		p->m_iFirstTime = ::time(NULL);
		m_IpSessions.append(p);
	}
	p->m_iLastOperTime = ::time(NULL);

	return p;
}


////////////////////////////////////////////////////////////////////////
// 描    述:  服务端会话线程
// 作    者:  邵凯田
// 创建时间:  2011-11-10 22:20
// 参数说明:  this
// 返 回 值:  NULL
//////////////////////////////////////////////////////////////////////////
void* CHttpServer::ThreadService(void* lp)
{
	CHttpServerSession *pSession = (CHttpServerSession*)lp;
	CHttpServer *pThis = pSession->m_pServer;
	BYTE *pRecvBuf = new BYTE[C_MAX_HTTPSVR_RECVBUF_SIZE];
	int ret;
	YsSocket *pSkt = pSession->m_pSocket;
	pThis->BeginThread();
	pSkt->SetTimeout(1000,1000);
	int last_recv_time = ::time(NULL);//最后接收到数据的时间
	int last_send_time = ::time(NULL);//最后发送数据的时间
	int no_recv=0;//没有收到数据的次数
	int no_send=0;//没有发送数据的次数
	YsString sHttpRecv;
	while(!pThis->IsQuit() && pSkt->IsConnected())
	{
		//接收报文
		pSession->lock();
		ret = pSkt->CheckForRecv();
		pSession->unlock();
		if(ret < 0)
		{
			//异常
			break;
		}
		else if(ret == 0)
		{
			//没有数据到达
			YsApi::YsSleep(100000);
		}
		else// if(ret == 1)
		{
			//有新的数据到达
			sHttpRecv = "";
			while(1)
			{
				ret = pSkt->ReceiveOnce((char *)pRecvBuf, C_MAX_HTTPSVR_RECVBUF_SIZE-1);
				if(ret >= 0)
					pRecvBuf[ret] = '\0';
				//YSDEBUG("HTTP RECV ret=%d,str=%s\n",ret,(char *)pRecvBuf);
				if(ret <= 0)
				{
					break;
				}
				sHttpRecv += (char *)pRecvBuf;
				break;
			}
			if(sHttpRecv.length() > 0)
				pThis->ProcessHttp(pSession,sHttpRecv);
			pSession->m_pSocket->Close();
			pSession->unlock();
		}
	}
	pSkt->Close();
	pThis->EndThread();
	pThis->m_Sessions.remove(pSession);
	delete[] pRecvBuf;
	return NULL;
}


//////////////////////////////////////////////////////////////////////////
// 描    述:  处理HTTP请求
// 作    者:  邵凯田
// 创建时间:  2013-5-23 13:42
// 参数说明:  
// 返 回 值:  
//////////////////////////////////////////////////////////////////////////
void CHttpServer::ProcessHttp(CHttpServerSession *pSession,YsString &sHttpRecvStr)
{
	//GET / HTTP/1.1

	// POST /nf_devtype_add.html HTTP/1.1
	// Host: 127.0.0.1
	// User-Agent: Mozilla/5.0 (Windows NT 5.1; rv:20.0) Gecko/20100101 Firefox/20.0
	// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
	// Accept-Language: zh-cn,zh;q=0.8,en-us;q=0.5,en;q=0.3
	// Accept-Encoding: gzip, deflate
	// Referer: http://127.0.0.1/nf_devtype_add.html
	// Connection: keep-alive
	// Content-Type: application/x-www-form-urlencoded
	// Content-Length: 37
	// 
	// devtype_id=111&devtype_name=123&Demo=


	YsString sLine = sHttpRecvStr.left(sHttpRecvStr.find("\r\n"));
	YsString sFun = YsString::GetIdAttribute(1,sLine," ");
	YsString sUrl = YsString::GetIdAttribute(2,sLine," ");
	sUrl.replace("%20"," ");
	pSession->m_sUrl = sUrl;
	pSession->m_sParam = "";
	if(sUrl.find("?")>0)
	{
		pSession->m_sParam = sUrl.mid(sUrl.find("?")+1);
	}
	if(sFun == "POST")
	{
		//提取POST内容
		sLine = sHttpRecvStr.mid(sHttpRecvStr.find("\r\n\r\n")+4);
		if(sLine.length() > 0)
		{
			if(pSession->m_sParam.length() == 0)
				pSession->m_sParam = sLine;
			else
				pSession->m_sParam += "&"+sLine;
		}
	}
	CUrlEncoding url;
	pSession->m_sParam = url.UrlGB2312Decode(pSession->m_sParam);
	if(sUrl != "/" && sUrl.find(".html") < 0  && sUrl.find(".csv") < 0 )
	{
		YsString sHead=	"HTTP/1.1 404 Not Found\r\n\r\n";
		pSession->m_pSocket->Send(sHead.data(),sHead.length());
		//YSDEBUG("Http Send! len=%d,str=%s",sHead.length(),sHead.data());
		return;
	}
// 	YsString sHead=	"HTTP/1.1 200 OK\r\n"
// 		"Date: "+YsDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")+
// 		"Cache-Control: max-age=3600, must-revalidate, public\r\n"
// 		"Connection: close\r\n"//Keep-Alive
// 		"Server: SktHttpServer(V1.0.0) skt001@163.com\r\n"
// 		"Content-Type: text/html\r\n"
// 		"Last-Modified: Sun, 30 Dec 2012 14:49:22 GMT\r\n"
// 		"\r\n";
// 	pSession->m_pSocket->Send(sHead.data(),sHead.length());
	YsString sHtml;
	ProcessUrl(pSession,sUrl,sHtml);
	if(sHtml.length() > 0)
		pSession->m_pSocket->Send(sHtml.data(),sHtml.length());
	//YSDEBUG("Http Send! len=%d,str=%s",sHead.length(),sHead.data());
}
