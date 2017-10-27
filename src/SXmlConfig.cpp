/**
 *
 * 文 件 名 : SXmlConfig.cpp
 * 创建日期 : 2008-03-28
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: 2011/08/31 08:43:35 $
 * 当前版本 : $Revision: 1.1 $
 * 功能描述 : 实现配置文件基类接口的XML配置类
 * 修改记录 : 
 *            $Log: SXmlConfig.cpp,v $
 *            Revision 1.1  2011/08/31 08:43:35  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2011/05/24 08:44:54  shaokt
 *            修改UTF-8格式的XML，前三个字节有0xEF BB BF的情况
 *
 *            Revision 1.2  2011/04/11 12:08:36  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2011/02/25 03:12:07  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2010/08/09 03:04:44  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2010/01/26 06:57:30  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2010/01/22 01:24:26  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2009/09/15 02:14:09  shaokt
 *            通迅记录分析装置项目首次提交
 *
 *            Revision 1.6  2009/05/19 02:50:38  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.5  2009/01/31 06:37:34  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.4  2008/12/22 08:54:13  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2008/11/03 05:54:49  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2008/10/06 08:48:29  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/01 11:38:29  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/01 08:58:04  shaokt
 *            *** empty log message ***
 *
 *
 **/

#include "SXmlConfig.h"

SXmlConfig::SXmlConfig()
{
	SKTOBJECT("SXmlConfig");
	m_pLock = NULL;
}
SXmlConfig::~SXmlConfig()
{
	if(m_pLock != NULL)
		delete m_pLock;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  设置对象的共享模式
// 作    者:  邵凯田
// 创建时间:  2015-10-26 10:53
// 参数说明:  @bShared为true表示线程间共享（需要加锁），false表示线程间不共享（不用加锁）
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SXmlConfig::SetShared(bool bShared)
{
	if(bShared)
	{
		if(m_pLock == NULL)
			m_pLock = new SLock();
	}
	else
	{
		if(m_pLock != NULL)
		{
			delete m_pLock;
			m_pLock = NULL;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  加锁
// 作    者:  邵凯田
// 创建时间:  2015-10-26 10:54
// 参数说明:  
// 返 回 值:  
//////////////////////////////////////////////////////////////////////////
void SXmlConfig::lock()
{
	if(m_pLock == NULL)
		return;
	m_pLock->lock();
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  解锁
// 作    者:  邵凯田
// 创建时间:  2015-10-26 10:54
// 参数说明:  
// 返 回 值:  
//////////////////////////////////////////////////////////////////////////
void SXmlConfig::unlock()
{
	if(m_pLock == NULL)
		return;
	m_pLock->unlock();
}

////////////////////////////////////////////////////////////////////////
// 描    述:  从文件读取XML
// 作    者:  邵凯田
// 创建时间:  2010-1-7 14:12
// 参数说明:  @sXmlFile文件名称
// 返 回 值:  true读取文件
//////////////////////////////////////////////////////////////////////////
bool SXmlConfig::ReadConfig(SString sXmlFile)
{
    int nRead;
    char *sXMLData;

	SFile file(sXmlFile);
	if(!file.open(IO_ReadOnly))
	{
		printf("ReadConfig('%s') error, file not exist!\n",sXmlFile.data());
		return false;
	}
	int filelen = file.size();
	sXMLData = new char[filelen+1];
	memset(sXMLData,0,filelen+1);
	for(nRead=0;nRead<filelen;)
	{
		int size = filelen - nRead;
		if(size > 10240)
			size = 10240;
		int ret = file.readBlock((BYTE*)(sXMLData+nRead),size);
		if(ret <= 0)
			break;
		nRead += ret;
	}
	file.close();

    sXMLData[nRead] = 0;

	char *pEncoding = strstr(sXMLData,"encoding=");
	if(pEncoding != NULL)
	{
		pEncoding += 10;
// 		while(*pEncoding != '\'' && *pEncoding != '\"' && *pEncoding != '?' )
// 			pEncoding++;
		if(SString::toString(pEncoding,5).toUpper() == "UTF-8")
		{
			//UTF-8编码
#ifdef WIN32
			//Windows下只需转换一次
			SString::Utf82Gb(sXMLData,filelen+1);
			filelen = strlen(sXMLData);
#else
			//char *pSrc = sXMLData;
			SString sTemp,sNew;
			int i=0,cnt=nRead;
			int s;
			int size = 10240;//10240
			sNew.SetBuffer(filelen);
			for(i=0;i<cnt;i+=s)
			{
				s = nRead-i;
				if(s > size)
					s = size;
				while(((unsigned char)sXMLData[i+s-1]) >= 128 && s > size/2)
				{
					s --;
				}
				sTemp = SString::toString(sXMLData+i,s);
				sNew += sTemp.toGb2312();
			}
			char *pNewStr = sNew.data();
			filelen = strlen(pNewStr);
			while(filelen > 0 && *pNewStr != '\0' && *pNewStr != '<')
			{
				pNewStr++;
				filelen--;
			}
			memcpy(sXMLData,pNewStr,filelen);
			sXMLData[filelen] = '\0';
#endif
		}
	}
	if(!LoadXmlByText(sXMLData))
	{
		printf("ReadConfig('%s') error, xml syntax error!\n",sXmlFile.data());
		delete[] sXMLData;
		return false;
	}
	
	delete[] sXMLData;
	return true;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  从文本字符串加载XML
// 作    者:  邵凯田
// 创建时间:  2011-11-24 15:10
// 参数说明:  @szXmlText为文本内容
// 返 回 值:  true表示读取成功，false表示读取失败
//////////////////////////////////////////////////////////////////////////
bool SXmlConfig::LoadXmlByText(const char * szXmlText)
{
	SString sXmlVer="";
	SBaseConfig *pNode = this;
	SBaseConfig *pParentNode = NULL;
	char prefix;
	char *pStr,*s0;
	char sNodeName[128];
	char sAttrName[128];
	int iAttrValueLen = 1024;
	char *sAttrValue = new char[iAttrValueLen+10];
	int iAttrValueSize;

	if(szXmlText[0] != '<')
		szXmlText = strstr(szXmlText,"<");
	if(szXmlText == NULL)
	{
		delete[] sAttrValue;
		return false;
	}

	SString sNodeValue;
	while(1)
	{
		//去空格
		while((*szXmlText> 0 && isspace(*szXmlText)) && *szXmlText!='\0')
			szXmlText++;
		if(*szXmlText == '\0')
		{
			if(this->GetNodeName().length() > 0)
				break;
			else
			{
				delete[] sAttrValue;
				return false;
			}
		}
		if (/* *szXmlText != '\0' && */ *szXmlText != '<') // 找不到节点起始标志!
		{
			if(pParentNode != NULL)
			{
				//读取节点值到父节点
				sAttrValue[0] = '\0';
				iAttrValueSize = -1;
				while(*szXmlText != '<' && *szXmlText != '\0')
				{
					iAttrValueSize++;
					if(iAttrValueSize >= iAttrValueLen)
					{
						char *pNew = new char[iAttrValueLen*2+10];
						memcpy(pNew,sAttrValue,iAttrValueLen);
						iAttrValueLen = iAttrValueLen*2;
						delete[] sAttrValue;
						sAttrValue = pNew;
					}
					if(*szXmlText == '&')
					{
						if(strstr(szXmlText,"&apos;") == szXmlText)
						{
							//sAttrValue[strlen(sAttrValue)+1] = '\0';
							sAttrValue[iAttrValueSize] = '\'';
							szXmlText += 6;
							continue;
						}
						else if(strstr(szXmlText,"&quot;") == szXmlText)
						{
							//sAttrValue[strlen(sAttrValue)+1] = '\0';
							sAttrValue[iAttrValueSize] = '\"';
							szXmlText += 6;
							continue;
						}
						else if(strstr(szXmlText,"&lt;") == szXmlText)
						{
							//sAttrValue[strlen(sAttrValue)+1] = '\0';
							sAttrValue[iAttrValueSize] = '<';
							szXmlText += 4;
							continue;
						}
						else if(strstr(szXmlText,"&gt;") == szXmlText)
						{
							//sAttrValue[strlen(sAttrValue)+1] = '\0';
							sAttrValue[iAttrValueSize] = '>';
							szXmlText += 4;
							continue;
						}
						else if(strstr(szXmlText,"&amp;") == szXmlText)
						{
							//sAttrValue[strlen(sAttrValue)+1] = '\0';
							sAttrValue[iAttrValueSize] = '&';
							szXmlText += 5;
							continue;
						}
					}
					//sAttrValue[strlen(sAttrValue)+1] = '\0';
					sAttrValue[iAttrValueSize] = *szXmlText;
					szXmlText++;
				}
				if(iAttrValueSize >= 0)
					sAttrValue[iAttrValueSize+1] ='\0';
				pParentNode->SetNodeValueEx(sAttrValue);
				continue;
			}
			delete[] sAttrValue;
			return false;
		}
		szXmlText++;
		switch(*szXmlText)
		{
			case '?': // 跳过版本检查
				sXmlVer += "<";
				while (*szXmlText!='>' && *szXmlText!='<' && *szXmlText!='\0')
				{
					sXmlVer += *szXmlText;
					szXmlText++;
				}
				if (*szXmlText++ !='>')
				{
					delete[] sAttrValue;
					return false;
				}

				sXmlVer += '>';
				break;

			case '!': // 跳过注释
				//<![CDATA[content]]>
				if(strstr(szXmlText,"![CDATA[") == szXmlText)
				{
					szXmlText += 8;
					pStr = strstr((char*)szXmlText,"]]>");
					if(pStr == NULL)
					{
						//未关闭的原始文本
						delete[] sAttrValue;
						return false;
					}
					//认为是原始文本
					sNodeValue = SString::toString(szXmlText,pStr-szXmlText);
					pParentNode->SetNodeValueEx(sNodeValue.data());
					szXmlText = pStr + 3;
					break;
				}
				szXmlText = strstr(szXmlText,"-->");
				if(szXmlText == NULL)
				{
					delete[] sAttrValue;
					return false;
				}
				szXmlText += strlen("-->");
				break;

			case '/': // 节点闭环标志: </XXX>
				//当前当前父节点
				if(pParentNode == NULL)
				{
					//没有要关闭的节点
					delete[] sAttrValue;
					return false;
				}
				szXmlText++;
				sNodeName[0] = '\0';
				iAttrValueSize = 0;
				while(!((*szXmlText>0 && isspace(*szXmlText)) || '='==*szXmlText || '\n'==*szXmlText || '>'==*szXmlText || '/' == *szXmlText || '\0' == *szXmlText))
				{
					sNodeName[iAttrValueSize+1] = '\0';
					sNodeName[iAttrValueSize] = *szXmlText;
					szXmlText ++;
					iAttrValueSize ++;
				}
// 				s0 = strstr(sNodeName,":");
// 				if(s0 != NULL)
// 					s0 += 1;
// 				else
					s0 = sNodeName;
				//if(strlen(s0) == 0)
				if(s0[0] == '\0')
				{
					//缺少节点名称
					delete[] sAttrValue;
					return false;
				}
				if(strcmp(s0, pParentNode->m_sNodeName) != 0)
				{
					//关闭节点名不正确
					delete[] sAttrValue;
					return false;
				}
				//去空格
				while((*szXmlText>0 && isspace(*szXmlText)) && *szXmlText!='\0')
					szXmlText++;
				if(*szXmlText != '>')
				{
					//关闭节点没有封装
					delete[] sAttrValue;
					return false;
				}
				szXmlText++;
				pParentNode = pParentNode->GetParentNode();
				break;

			default:
				//提取节点信息到pNode中
				sNodeName[0] = '\0';
				iAttrValueSize = 0;
				while(!((*szXmlText>0 && isspace(*szXmlText)) || '='==*szXmlText || '\n'==*szXmlText || '>'==*szXmlText || '/' == *szXmlText || '\0' == *szXmlText))
				{
					sNodeName[iAttrValueSize+1] = '\0';
					sNodeName[iAttrValueSize] = *szXmlText;
					szXmlText++;
					iAttrValueSize++;
				}
// 				s0 = strstr(sNodeName,":");
// 				if(s0 != NULL)
// 					s0 += 1;
// 				else
					s0 = sNodeName;
//容错DRL600HDR
// 				if(strlen(s0) == 0)
// 				{
// 					//缺少节点名称
//					delete[] sAttrValue;
// 					return false;
// 				}
				if(pParentNode == NULL)
				{
					pNode = this;
					pNode->SetNodeNameEx(s0);
				}
				else
				{
					pNode = pParentNode->AddChildNodeEx(s0);
				}
				//去空格
				while((*szXmlText>0 && isspace(*szXmlText)) && *szXmlText!='\0')
					szXmlText++;
				//提取属性
				while(*szXmlText != '>' && *szXmlText != '/' && *szXmlText != '\0')
				{
					sAttrName[0] = '\0';
					iAttrValueSize = 0;
					while(!((*szXmlText>0 && isspace(*szXmlText)) || '='==*szXmlText || '\n'==*szXmlText || '>'==*szXmlText || '/' == *szXmlText))
					{
						if(*szXmlText == '\0')
							break;
						sAttrName[iAttrValueSize+1] = '\0';
						sAttrName[iAttrValueSize] = *szXmlText;
						szXmlText++;
						iAttrValueSize ++;
					}
					if(sAttrName[0] == '\0' || *szXmlText == '\0')
					{
						//缺少属性名
						delete[] sAttrValue;
						return false;
					}
					//去空格
					while((*szXmlText>0 && isspace(*szXmlText)) && *szXmlText!='\0')
						szXmlText++;
					if(*szXmlText != '=')
					{
						//缺少属性=
						delete[] sAttrValue;
						return false;
					}
					szXmlText++;
					//去空格
					while((*szXmlText>0 && isspace(*szXmlText)) && *szXmlText!='\0')
						szXmlText++;
					if(*szXmlText != '\'' && *szXmlText != '\"')
					{
						//缺少属性值的引号
						delete[] sAttrValue;
						return false;
					}
					prefix = *szXmlText;
					szXmlText++;
					sAttrValue[0] = '\0';
					iAttrValueSize = 0;
					while(*szXmlText != prefix && *szXmlText != '\0')
					{
						if(iAttrValueSize >= iAttrValueLen)
						{
							char *pNew = new char[iAttrValueLen+520];
							memcpy(pNew,sAttrValue,iAttrValueLen);
							iAttrValueLen = iAttrValueLen+512;
							delete[] sAttrValue;
							sAttrValue = pNew;
						}

						if(*szXmlText == '&')
						{
							if(strstr(szXmlText,"&apos;") == szXmlText)
							{
								sAttrValue[iAttrValueSize+1] = '\0';
								sAttrValue[iAttrValueSize] = '\'';
								szXmlText += 6;
								iAttrValueSize += 6;
								continue;
							}
							else if(strstr(szXmlText,"&quot;") == szXmlText)
							{
								sAttrValue[iAttrValueSize+1] = '\0';
								sAttrValue[iAttrValueSize] = '\"';
								szXmlText += 6;
								iAttrValueSize += 6;
								continue;
							}
							else if(strstr(szXmlText,"&lt;") == szXmlText)
							{
								sAttrValue[iAttrValueSize+1] = '\0';
								sAttrValue[iAttrValueSize] = '<';
								szXmlText += 4;
								iAttrValueSize += 4;
								continue;
							}
							else if(strstr(szXmlText,"&gt;") == szXmlText)
							{
								sAttrValue[iAttrValueSize+1] = '\0';
								sAttrValue[iAttrValueSize] = '>';
								szXmlText += 4;
								iAttrValueSize += 4;
								continue;
							}
							else if(strstr(szXmlText,"&amp;") == szXmlText)
							{
								sAttrValue[iAttrValueSize+1] = '\0';
								sAttrValue[iAttrValueSize] = '&';
								szXmlText += 5;
								iAttrValueSize += 5;
								continue;
							}
						}
						sAttrValue[iAttrValueSize+1] = '\0';
						sAttrValue[iAttrValueSize] = *szXmlText;
						szXmlText++;
						iAttrValueSize++;
					}
					if(*szXmlText != prefix)
					{
						//属性值没有结束
						delete[] sAttrValue;
						return false;
					}
					pNode->SetAttributeEx(sAttrName,sAttrValue);
					szXmlText++;
					//去空格
					while((*szXmlText>0 && isspace(*szXmlText)) && *szXmlText!='\0')
						szXmlText++;
				}//end while
				if(*szXmlText == '/')
				{
					//无下级节点及节点值，立即关闭当前节点
					szXmlText++;
					if(*szXmlText != '>')
					{
						//'/'后没有跟'>'
						delete[] sAttrValue;
						return false;
					}
					szXmlText++;
					pParentNode = pNode->GetParentNode();
					break;
				}
				if(*szXmlText != '>' )
				{
					//节点未关闭
					delete[] sAttrValue;
					return false;
				}
				szXmlText++;
				//提取下级节点
				pParentNode = pNode;				
				break;
		}
		
	}
	delete[] sAttrValue;
	return true;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  从文本字符串加载XML
// 作    者:  邵凯田
// 创建时间:  2011-11-24 15:10
// 参数说明:  @sXmlText为文本内容
// 返 回 值:  true表示读取成功，false表示读取失败
//////////////////////////////////////////////////////////////////////////
bool SXmlConfig::ReadText(const char* sXmlText)
{
	return LoadXmlByText(sXmlText);
}

////////////////////////////////////////////////////////////////////////
// 描    述:  用当前实例保存XML配置文件
// 作    者:  邵凯田
// 创建时间:  2011-11-24 15:11
// 参数说明:  @sXmlFile为目标文件名
//            @sEncoding字符集编码,缺省为GB
// 返 回 值:  true表示保存成功，false为失败
//////////////////////////////////////////////////////////////////////////
bool SXmlConfig::SaveConfig(SString sXmlFile,SString sEncoding/*="GB2312"*/)
{
	if(sEncoding.compareNoCase("UTF-8") == 0)
		m_iEncodeing = STR_ENCODING_UTF8;
	else
		m_iEncodeing = STR_ENCODING_GB2312;
	SFile file(sXmlFile);
	if(!file.open(IO_Truncate))
		return false;

	int level=0;
	if(m_iEncodeing == STR_ENCODING_UTF8)
		file.writeString("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n");
	else
		file.writeString("<?xml version=\"1.0\" encoding=\""+sEncoding+"\"?>\r\n");
	SaveNode(file,this,level);

	return true;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  用当前实例保存XML配置文件
// 作    者:  邵凯田
// 创建时间:  2014-07-30 14:16
// 参数说明:  @sXml为输出的XML文本
//            @sEncoding字符集编码,缺省为GB
// 返 回 值:  true表示保存成功，false为失败
//////////////////////////////////////////////////////////////////////////
bool SXmlConfig::SaveConfigToText(SString &sXml,SString sEncoding)
{
	if(sEncoding.compareNoCase("UTF-8") == 0)
		m_iEncodeing = STR_ENCODING_UTF8;
	else
		m_iEncodeing = STR_ENCODING_GB2312;

	sXml = "";
	int level=0;
	if(sEncoding.length() > 0)
	{
		if(m_iEncodeing == STR_ENCODING_UTF8)
			sXml += "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n";
		else
			sXml += "<?xml version=\"1.0\" encoding=\""+sEncoding+"\"?>\r\n";
	}
	SaveNodeText(sXml,this,level);

	return true;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  保存指定的节点
// 作    者:  邵凯田
// 创建时间:  2011-11-24 15:12
// 参数说明:  @file为文件实例
//            @pNode为节点实例
//            @level为当前级别
// 返 回 值:  
//////////////////////////////////////////////////////////////////////////
bool SXmlConfig::SaveNode(SFile &file,SBaseConfig *pNode,int &level)
{
	SString sLine;
	SString sTab;
	SString sNodeValue;
	int i,cnt = pNode->GetChildCount();
	sTab = "";
	for(i=0;i<level;i++)
		sTab += "\t";
	sLine.sprintf("%s<%s",sTab.data(),pNode->GetNodeName().data());
	if(m_iEncodeing == STR_ENCODING_UTF8)
		sLine = sLine.toUtf8();
	file.writeString(sLine);
	int cols = pNode->GetAttributeCount();
	for(i=0;i<cols;i++)
	{
		if(pNode->GetAttributeName(i).length() > 0)
		{
			sLine.sprintf(" %s=\"%s\"",pNode->GetAttributeName(i).data(),pNode->GetAttribute(pNode->GetAttributeName(i)).data());
			if(m_iEncodeing == STR_ENCODING_UTF8)
				sLine = sLine.toUtf8();
			file.writeString(sLine);
		}
	}
	sNodeValue = pNode->GetNodeValue();
#if 1
	if(cnt == 0 && sNodeValue == "")
	{
		file.writeString("/>\r\n");
		return true;
	}
	else if(sNodeValue != "")
	{
		file.writeString(">");
	}
	else
		file.writeString(">\r\n");
#else
	if(cnt == 0)
	{
		file.writeString(">");
	}
	else
		file.writeString(">\r\n");
#endif
	//node value
	if(sNodeValue != "")
	{
		//MEMO: 增加特殊格式的节点内容加限定符存储 [2015-8-19 17:19 邵凯田]
		//如果节点内容中含有关键字则定义为<![CDATA[ 。。。 ]]>
		if(sNodeValue.find("<") >= 0 && (sNodeValue.find("/>") >= 0 || sNodeValue.find("</") >= 0))
		{
			sLine = "<![CDATA[";
			sLine += sNodeValue;
			sLine += "]]>";
			//sLine.sprintf("<![CDATA[%s]]>",sNodeValue.data());
		}
		else
		{
			//sLine.sprintf("%s",sNodeValue.data());
			sLine = sNodeValue;
		}
		if(m_iEncodeing == STR_ENCODING_UTF8)
			sLine = sLine.toUtf8();
		file.writeString(sLine);

	}
	if(cnt > 0)
	{
		for(i=0;i<cnt;i++)
		{
			level ++;
			SBaseConfig *p = pNode->GetChildNode(i);
			SaveNode(file,p,level);
			level --;
		}
		sLine.sprintf("%s</%s>\r\n",sTab.data(),pNode->GetNodeName().data());
		if(m_iEncodeing == STR_ENCODING_UTF8)
			sLine = sLine.toUtf8();
		file.writeString(sLine);
	}
	else
	{
		sLine.sprintf("</%s>\r\n",pNode->GetNodeName().data());
		if(m_iEncodeing == STR_ENCODING_UTF8)
			sLine = sLine.toUtf8();
		file.writeString(sLine);
	}
	return true;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  保存指定的节点
// 作    者:  邵凯田
// 创建时间:  2014-07-30 14:18
// 参数说明:  @sXml为XML文本
//            @pNode为节点实例
//            @level为当前级别
// 返 回 值:  true/false
//////////////////////////////////////////////////////////////////////////
bool SXmlConfig::SaveNodeText(SString &sXml,SBaseConfig *pNode,int &level)
{
	SString sLine;
	SString sTab;
	int i,cnt = pNode->GetChildCount();
	sTab = "";
	for(i=0;i<level;i++)
		sTab += "\t";
	sLine.sprintf("%s<%s",sTab.data(),pNode->GetNodeName().data());
	if(m_iEncodeing == STR_ENCODING_UTF8)
		sLine = sLine.toUtf8();
	sXml += sLine;
	int cols = pNode->GetAttributeCount();
	for(i=0;i<cols;i++)
	{
		if(pNode->GetAttributeName(i).length() > 0)
		{
			sLine.sprintf(" %s=\"%s\"",pNode->GetAttributeName(i).data(),pNode->GetAttribute(pNode->GetAttributeName(i)).data());
			if(m_iEncodeing == STR_ENCODING_UTF8)
				sLine = sLine.toUtf8();
			sXml += sLine;
		}
	}
#if 1
	if(cnt == 0 && pNode->GetNodeValue().length() == 0)
	{
		sXml += "/>\r\n";
		return true;
	}
	else if(pNode->GetNodeValue() != "")
	{
		sXml += ">";
	}
	else
		sXml += ">\r\n";
#else
	if(cnt == 0)
	{
		sXml += ">";
	}
	else
		sXml += ">\r\n";
#endif
	//node value
	if(pNode->GetNodeValue().length() > 0)
	{
		//sLine.sprintf("%s",pNode->GetNodeValue().data());
		if(m_iEncodeing == STR_ENCODING_UTF8)
			sXml += pNode->GetNodeValue().toUtf8();
		else
			sXml += pNode->GetNodeValue();
	}
	if(cnt > 0)
	{
		for(i=0;i<cnt;i++)
		{
			level ++;
			SBaseConfig *p = pNode->GetChildNode(i);
			SaveNodeText(sXml,p,level);
			level --;
		}
		sLine.sprintf("%s</%s>\r\n",sTab.data(),pNode->GetNodeName().data());
		if(m_iEncodeing == STR_ENCODING_UTF8)
			sLine = sLine.toUtf8();
		sXml += sLine;
	}
	else
	{
		sLine.sprintf("</%s>\r\n",pNode->GetNodeName().data());
		if(m_iEncodeing == STR_ENCODING_UTF8)
			sLine = sLine.toUtf8();
		sXml += sLine;
	}
	return true;
}


