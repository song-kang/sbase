/**
 *
 * 文 件 名 : SSaxXml.cpp
 * 创建日期 : 2015-4-7 16:18
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : 基于SAX的XML解析类
 * 修改记录 : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-4-7	邵凯田　创建文件
 *
 **/

#include "SSaxXml.h"
#include "SFile.h"

SSaxXml::SSaxXml()
{
	m_Encoding = SSaxXml_GBK;
}

SSaxXml::~SSaxXml()
{

}


//////////////////////////////////////////////////////////////////////////
// 描    述:  解析XML文件
// 作    者:  邵凯田
// 创建时间:  2015-4-8 9:28
// 参数说明:  @xXmlFileName为XML文件名的全路径
//         :  @encoding表示期望的编码类型，默认为GBK
// 返 回 值:  true表示解析成功，false表示解析失败
//////////////////////////////////////////////////////////////////////////
bool SSaxXml::ParseXmlFile(char* sXmlFileName, SSaxXmlEncoding encoding)
{
	char *sXMLData,*sValidXml;

	//复位解析器
	ResetParser();

	SFile file(sXmlFileName);
	if(!file.open(IO_ReadOnly))
	{
		printf("ParseXmlFile('%s') error, file not exist!\n",sXmlFileName);
		return false;
	}
	int filelen = file.size();
	if(filelen == 0)
	{
		printf("ParseXmlFile('%s') error, file is empty!\n",sXmlFileName);
		return false;
	}
	sXMLData = new char[filelen+1];
	file.seekBegin();
	int offset = 0;
	while(offset < filelen)
	{
		int readlen = filelen-offset;
		if(readlen > 1048576)
			readlen = 1048576;
		int ret = file.readBlock((BYTE*)sXMLData+offset,readlen);
		if(ret != readlen)
		{
			file.close();
			printf("ParseXmlFile('%s') error, read file error!\n",sXmlFileName);
			delete[] sXMLData;
			return false;
		}
		offset += readlen;
	}

// 	int ret = file.readBlock((BYTE*)sXMLData,filelen);
// 	if(ret != filelen)
// 	{
// 		file.close();
// 		printf("ParseXmlFile('%s') error, read file error!\n",sXmlFileName);
// 		delete[] sXMLData;
// 		return false;
// 	}
	sXMLData[filelen] = '\0';
	file.close();

	char *pEncoding = strstr(sXMLData,"encoding=");
	if(pEncoding != NULL)
	{
		pEncoding += 10;
		//while(*pEncoding != '\'' && *pEncoding != '"' && *pEncoding != '?' )
		//	pEncoding++;
		if(SString::toString(pEncoding,5).toUpper() == "UTF-8" && encoding == SSaxXml_GBK)
		{
			//UTF-8编码需要转为GBK，暂时不考虑GB转为UTF8的情况（极少使用）
#ifdef WIN32
			//Windows下只需转换一次
			SString::Utf82Gb(sXMLData,filelen+1);
			filelen = (int)strlen(sXMLData);
#else
			//char *pSrc = sXMLData;
			SString sTemp,sNew;
			int i=0,cnt=filelen;
			int s;
			int size = 10240;//10240
			sNew.SetBuffer(filelen);
			for(i=0;i<cnt;i+=s)
			{
				s = filelen-i;
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

	StartDocument();
	
	if(!ParseText(sXMLData, sValidXml, true))
	{
		EndDocument();
		delete[] sXMLData;
		return false;
	}

	EndDocument();
	delete[] sXMLData;
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  解析一段XML文件，可分多次解析一个文件，文本应采用GBK/GB18030字符编码
// 作    者:  邵凯田
// 创建时间:  2015-4-8 9:34
// 参数说明:  @sXmlText表示待解析的文本内容
//         :  @sValidXmlText表示被有效解析的文本位置，下一次解析时应从将该位置开始
//         :  @bLastText表示是否为最后一批待解析文本
// 返 回 值:  true表示解析成功，false表示解析失败
//////////////////////////////////////////////////////////////////////////
bool SSaxXml::ParseText(char* sXmlText, char* &sValidXmlText,bool bLastText)
{
	SString sXmlVer="";
	char prefix;
	char *pStr,*s0;
	char * p_maohao;
	int iPos;

	sValidXmlText = sXmlText;
	if(sXmlText[0] != '<')
		sXmlText = strstr(sXmlText,"<");
	if(sXmlText == NULL)
		return false;
	int iAttrBufSum = 128;//属性最大数量
	int iThisAttrs;//当前节点的属性数量
	SSaxAttribute *pSaxAttr=new SSaxAttribute[iAttrBufSum];
	SSaxNode *pThisNode = NULL;
	SString sNodeValue;
	while(1)
	{
		//去空格
		while((*sXmlText> 0 && isspace(*sXmlText)) && *sXmlText!='\0')
			sXmlText++;
		if(*sXmlText == '\0')
		{
			if(bLastText && !m_XmlStack.is_empty())
			{
				Error(SSaxErr_SYNTAX_ERR,"XML结尾不合法!");
				if(pSaxAttr != NULL)
					delete[] pSaxAttr;
				return false;
			}
			break;
		}
		if (*sXmlText != '<') // 找不到节点起始标志!
		{
			if(!m_XmlStack.is_empty())
			{
				//读取节点值到当前节点
				m_sAttrValue[0] = '\0';
				int iPos = 0;
				while(*sXmlText != '<' && *sXmlText != '\0')
				{
					if(*sXmlText == '&')
					{
						if(SString::equals(sXmlText,"&apos;"))
						{							
							m_sAttrValue[iPos++] = '\'';
							sXmlText += 6;
							continue;
						}
						else if(SString::equals(sXmlText,"&quot;"))
						{
							m_sAttrValue[iPos++] = '"';
							sXmlText += 6;
							continue;
						}
						else if(SString::equals(sXmlText,"&lt;"))
						{
							m_sAttrValue[iPos++] = '<';
							sXmlText += 4;
							continue;
						}
						else if(SString::equals(sXmlText,"&gt;"))
						{
							m_sAttrValue[iPos++] = '>';
							sXmlText += 4;
							continue;
						}
						else if(SString::equals(sXmlText,"&amp;"))
						{
							m_sAttrValue[iPos++] = '&';
							sXmlText += 5;
							continue;
						}
					}
					m_sAttrValue[iPos++] = *sXmlText;
					sXmlText++;
				}
				m_sAttrValue[iPos] = '\0';

				if(*sXmlText == '<' || (bLastText && *sXmlText != '\0'))
				{
					Characters(m_sAttrValue);
					sValidXmlText = sXmlText;
				}
				continue;
			}
			if(pSaxAttr != NULL)
				delete[] pSaxAttr;
			return false;
		}
		sXmlText++;
		switch(*sXmlText)
		{
		case '?': // 跳过版本检查
			sXmlVer += "<";
			while (*sXmlText!='>' && *sXmlText!='<' && *sXmlText!='\0')
			{
				sXmlVer += *sXmlText;
				sXmlText++;
			}
			if (*sXmlText++ !='>')
			{
				if(pSaxAttr != NULL)
					delete[] pSaxAttr;
				if(!bLastText)
					return true;
				Error(SSaxErr_SYNTAX_ERR,"无效的XML语法申明!");
				return false;
			}

			sXmlVer += '>';
			sValidXmlText = sXmlText;
			break;

		case '!': // 跳过注释
			//<![CDATA[content]]>
			//if(SString::equals(sXmlText,"![CDATA["))
			if(sXmlText[0] == '!' && sXmlText[1] == '[' && sXmlText[2] == 'C' && sXmlText[3] == 'D' && sXmlText[4] == 'A' && sXmlText[5] == 'T' && sXmlText[6] == 'A' && sXmlText[7] == '[')
			{
				sXmlText += 8;
				pStr = strstr(sXmlText,"]]>");
				if(pStr != NULL)
				{
					//认为是原始文本
					sNodeValue = SString::toString(sXmlText,(int)(pStr-sXmlText));
					if(!m_XmlStack.is_empty())
						Characters(sNodeValue.data());
					sXmlText = pStr + 3;
					sValidXmlText = sXmlText;
				}
				else if(bLastText)
				{
					//未关闭的原始文本
					if(pSaxAttr != NULL)
						delete[] pSaxAttr;
					if(!bLastText)
						return true;
					Error(SSaxErr_SYNTAX_ERR,"未关闭的原始文本CDATA!");
					return false;
				}
				break;
			}
			sXmlText = strstr(sXmlText,"-->");
			if(sXmlText == NULL && bLastText)
			{
				if(pSaxAttr != NULL)
					delete[] pSaxAttr;
				if(!bLastText)
					return true;
				Error(SSaxErr_SYNTAX_ERR,"未关闭的注释!");
				return false;
			}
			sXmlText += 3;//strlen("-->");
			sValidXmlText = sXmlText;
			break;

		case '/': // 节点闭环标志: </XXX>
			//当前当前父节点
			if(m_XmlStack.is_empty())
			{
				//没有要关闭的节点
				if(pSaxAttr != NULL)
					delete[] pSaxAttr;
				if(!bLastText)
					return true;
				Error(SSaxErr_SYNTAX_ERR,"没有要关闭的节点!");
				return false;
			}
			sXmlText++;
			m_sNodeName[0] = '\0';
			iPos = 0;
			while(!((*sXmlText> 0 && isspace(*sXmlText)) || '='==*sXmlText || '\n'==*sXmlText || '>'==*sXmlText || '/' == *sXmlText || '\0' == *sXmlText))
			{
				m_sNodeName[iPos++] = *sXmlText;
				sXmlText ++;
			}
			m_sNodeName[iPos] = '\0';
// 			s0 = strstr(m_sNodeName,":");
// 			if(s0 != NULL)
// 				s0 += 1;
// 			else
			s0 = m_sNodeName;
// 			p_maohao = strstr(s0,":");
// 			if(p_maohao != 0)
// 				s0 = p_maohao+1;
			if(s0[0] == '\0')
			{
				//缺少节点名称
				if(pSaxAttr != NULL)
					delete[] pSaxAttr;
				if(!bLastText)
					return true;
				Error(SSaxErr_SYNTAX_ERR,"缺少待关闭的节点名称!");
				return false;
			}
			if(strcmp(s0, m_XmlStack.top()->m_sNodeName.data()) != 0)
			{
				//关闭节点名不正确
				if(pSaxAttr != NULL)
					delete[] pSaxAttr;
				if(!bLastText)
					return true;
				Error(SSaxErr_SYNTAX_ERR,"关闭节点名称不匹配!");
				return false;
			}
			//去空格
			while((*sXmlText> 0 && isspace(*sXmlText)) && *sXmlText!='\0')
				sXmlText++;
			if(*sXmlText != '>')
			{
				//关闭节点没有封装
				if(pSaxAttr != NULL)
					delete[] pSaxAttr;
				if(!bLastText)
					return true;
				Error(SSaxErr_SYNTAX_ERR,"关闭节点没有封装!");
				return false;
			}
			sXmlText++;
			sValidXmlText = sXmlText;

			//弹出栈中的顶节点
			if(pThisNode != NULL)
				delete pThisNode;
			pThisNode = m_XmlStack.pop();
			EndElement(pThisNode->m_sNodeName.data());
			break;

		default:
			//提取节点信息到pNode中
			m_sNodeName[0] = '\0';
			m_iNodeNameLen = 0;
			while(!((*sXmlText> 0 && isspace(*sXmlText)) || '='==*sXmlText || '\n'==*sXmlText || '>'==*sXmlText || '/' == *sXmlText || '\0' == *sXmlText))
			{
				m_sNodeName[m_iNodeNameLen++] = *sXmlText;
// 				m_sNodeName[strlen(m_sNodeName)+1] = '\0';
// 				m_sNodeName[strlen(m_sNodeName)] = *sXmlText;
				sXmlText++;
			}
			m_sNodeName[m_iNodeNameLen] = '\0';
// 			s0 = strstr(m_sNodeName,":");
// 			if(s0 != NULL)
// 				s0 += 1;
// 			else
 				s0 = m_sNodeName;
			if(m_iNodeNameLen == 0)
			{
				//缺少节点名称
				if(pSaxAttr != NULL)
					delete[] pSaxAttr;
				if(!bLastText)
					return true;
				Error(SSaxErr_SYNTAX_ERR,"缺少节点名称!");
				return false;
			}

			//去空格
			while((*sXmlText> 0 && isspace(*sXmlText)) && *sXmlText!='\0')
				sXmlText++;
			//提取属性
			iThisAttrs = 0;
			while(*sXmlText != '>' && *sXmlText != '/' && *sXmlText != '\0')
			{
				m_sAttrName[0] = '\0';
				m_iAttrNameLen = 0;
				//register char ch = *sXmlText;
				while(!((*sXmlText> 0 && isspace(*sXmlText)) || '='==*sXmlText || '\n'==*sXmlText || '>'==*sXmlText || '/' == *sXmlText))
				//while(!((isspace(ch) && (unsigned char)ch<0x80) || '='==ch || '\n'==ch || '>'==ch || '/' == ch))
				{
					if(*sXmlText == '\0')
					{
						if(pSaxAttr != NULL)
							delete[] pSaxAttr;
						if(!bLastText)
							return true;
						Error(SSaxErr_SYNTAX_ERR,"无效的XML结尾!");
						return false;
					}
					m_sAttrName[m_iAttrNameLen++] = *sXmlText;//ch;
// 					m_sAttrName[strlen(m_sAttrName)+1] = '\0';
// 					m_sAttrName[strlen(m_sAttrName)] = *sXmlText;//ch;
					sXmlText++;
					//ch = *sXmlText;
				}
				m_sAttrName[m_iAttrNameLen] = '\0';

				if(m_sAttrName[0] == '\0' || *sXmlText == '\0')
				{
					//缺少属性名
					if(pSaxAttr != NULL)
						delete[] pSaxAttr;
					if(!bLastText)
						return true;
					Error(SSaxErr_SYNTAX_ERR,"缺少属性名称!");
					return false;
				}
				//去空格
				while((*sXmlText> 0 && isspace(*sXmlText)) && *sXmlText!='\0')
					sXmlText++;
				if(*sXmlText != '=')
				{
					//缺少属性=
					if(pSaxAttr != NULL)
						delete[] pSaxAttr;
					if(!bLastText)
						return true;
					Error(SSaxErr_SYNTAX_ERR,"缺少属性‘=’!");
					return false;
				}
				sXmlText++;
				//去空格
				while((*sXmlText> 0 && isspace(*sXmlText)) && *sXmlText!='\0')
					sXmlText++;
				if(*sXmlText != '\'' && *sXmlText != '"')
				{
					//缺少属性值的引号
					if(pSaxAttr != NULL)
						delete[] pSaxAttr;
					if(!bLastText)
						return true;
					Error(SSaxErr_SYNTAX_ERR,"缺少属性值的引号!");
					return false;
				}
				prefix = *sXmlText;
				sXmlText++;
				m_sAttrValue[0] = '\0';
				m_iAttrValueLen = 0;
				while(*sXmlText != prefix && *sXmlText != '\0')
				{
					if(*sXmlText == '&')
					{
						if(SString::equals(sXmlText,"&apos;"))
						{
							m_sAttrValue[m_iAttrValueLen++] = '\'';
// 							m_sAttrValue[strlen(m_sAttrValue)+1] = '\0';
// 							m_sAttrValue[strlen(m_sAttrValue)] = '\'';
							sXmlText += 6;
							continue;
						}
						else if(SString::equals(sXmlText,"&quot;"))
						{
							m_sAttrValue[m_iAttrValueLen++] = '"';
// 							m_sAttrValue[strlen(m_sAttrValue)+1] = '\0';
// 							m_sAttrValue[strlen(m_sAttrValue)] = '"';
							sXmlText += 6;
							continue;
						}
						else if(SString::equals(sXmlText,"&lt;"))
						{
							m_sAttrValue[m_iAttrValueLen++] = '<';
// 							m_sAttrValue[strlen(m_sAttrValue)+1] = '\0';
// 							m_sAttrValue[strlen(m_sAttrValue)] = '<';
							sXmlText += 4;
							continue;
						}
						else if(SString::equals(sXmlText,"&gt;"))
						{
							m_sAttrValue[m_iAttrValueLen++] = '>';
// 							m_sAttrValue[strlen(m_sAttrValue)+1] = '\0';
// 							m_sAttrValue[strlen(m_sAttrValue)] = '>';
							sXmlText += 4;
							continue;
						}
						else if(SString::equals(sXmlText,"&amp;"))
						{
							m_sAttrValue[m_iAttrValueLen++] = '&';
// 							m_sAttrValue[strlen(m_sAttrValue)+1] = '\0';
// 							m_sAttrValue[strlen(m_sAttrValue)] = '&';
// 							sXmlText += 5;
							continue;
						}
					}
					m_sAttrValue[m_iAttrValueLen++] = *sXmlText;
// 					m_sAttrValue[strlen(m_sAttrValue)+1] = '\0';
// 					m_sAttrValue[strlen(m_sAttrValue)] = *sXmlText;
					sXmlText++;
				}
				m_sAttrValue[m_iAttrValueLen] = '\0';

				if(*sXmlText != prefix)
				{
					if(pSaxAttr != NULL)
						delete[] pSaxAttr;
					if(bLastText == false)
						return true;
					//属性值没有结束
					Error(SSaxErr_SYNTAX_ERR,"属性值 没有结束!");
					return false;
				}
				if(iThisAttrs >= iAttrBufSum)
				{
					iAttrBufSum *= 2;
					SSaxAttribute *pNew = new SSaxAttribute[iAttrBufSum];
					for(int i=0;i<iThisAttrs;i++)
					{
						pNew[i].m_sAttrName = pSaxAttr[i].m_sAttrName;
						pNew[i].m_sAttrValue = pSaxAttr[i].m_sAttrValue;
					}
					delete[] pSaxAttr;
					pSaxAttr = pNew;
				}
				pSaxAttr[iThisAttrs].m_sAttrName = m_sAttrName;
				pSaxAttr[iThisAttrs].m_sAttrValue = m_sAttrValue;
				iThisAttrs ++;
				sXmlText++;
				//去空格
				while((*sXmlText> 0 && isspace(*sXmlText)) && *sXmlText!='\0')
					sXmlText++;
			}//end while
			
			if(*sXmlText == '/')
			{
				//无下级节点及节点值，立即关闭当前节点
				sXmlText++;
				if(*sXmlText != '>')
				{
					//'/'后没有跟'>'
					if(pSaxAttr != NULL)
						delete[] pSaxAttr;
					if(!bLastText)
						return true;
					Error(SSaxErr_SYNTAX_ERR,"'/'后没有跟'>' !");
					return false;
				}
				StartElement(s0,pSaxAttr,iThisAttrs);
				EndElement(s0);
				sXmlText++;
				sValidXmlText = sXmlText;
				break;
			}
			if(*sXmlText == '\0')
			{
				if(bLastText)
				{
					//意外的结尾
					//节点未关闭
					if(pSaxAttr != NULL)
						delete[] pSaxAttr;
					if(!bLastText)
						return true;
					Error(SSaxErr_SYNTAX_ERR,"节点未关闭!");
					return false;
				}
				break;
			}
			if(*sXmlText != '>' && bLastText)
			{
				//节点未关闭
				if(pSaxAttr != NULL)
					delete[] pSaxAttr;
				if(!bLastText)
					return true;
				Error(SSaxErr_SYNTAX_ERR,"节点未关闭!");
				return false;
			}
			StartElement(s0,pSaxAttr,iThisAttrs);
			if(pThisNode == NULL)
				pThisNode = new SSaxNode;
			pThisNode->m_sNodeName = s0;
			m_XmlStack.push(pThisNode);
			pThisNode = NULL;

			sXmlText ++;
			sValidXmlText = sXmlText;
			//提取下级节点
			break;
		}

	}
	if(pSaxAttr != NULL)
		delete[] pSaxAttr;
	if(pThisNode != NULL)
	{
		delete pThisNode;
		pThisNode = NULL;
	}
	if(bLastText && !m_XmlStack.is_empty())
	{
		Error(SSaxErr_SYNTAX_ERR,"XML未正常结束!");
		return false;
	}
	
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  复位解析器，为下一个完整的XML解析做准备
// 作    者:  邵凯田
// 创建时间:  2015-4-8 9:54
// 参数说明:  void
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SSaxXml::ResetParser()
{
	SSaxNode *p;
	while(!m_XmlStack.is_empty())
	{
		p = m_XmlStack.pop();
		delete p;
	}
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// 名    称:  SSaxXmlEx
// 作    者:  邵凯田
// 创建时间:  2015-4-8 11:15
// 描    述:  基于SAX的轻量级XML解析类，通过逐级注册回调方法处理XML，
//            上层处理类不需要记忆当前处理节点位置，而不在不同节点注册不同的入口回调函数，
//			  该类使用较SSaxXml繁琐，但效率高于SSaxXml类
//////////////////////////////////////////////////////////////////////////

SSaxXmlEx::SSaxXmlEx()
{
	m_Encoding = SSaxXml_GBK;
	m_pRootNodeCallbackFun = NULL;
	m_psValidText = m_psTextStart = NULL;
	m_ppsTextNow = &m_psTextStart;
}

SSaxXmlEx::~SSaxXmlEx()
{
	m_ConstString.Clear();
}


//////////////////////////////////////////////////////////////////////////
// 描    述:  解析XML文件
// 作    者:  邵凯田
// 创建时间:  2015-4-8 9:28
// 参数说明:  @xXmlFileName为XML文件名的全路径
//         :  @encoding表示期望的编码类型，默认为GBK
// 返 回 值:  true表示解析成功，false表示解析失败
//////////////////////////////////////////////////////////////////////////
bool SSaxXmlEx::ParseXmlFile(char* sXmlFileName, SSaxXmlEncoding encoding)
{
	if(m_pRootNodeCallbackFun == NULL)
		return false;
	//复位解析器
	ResetParser();

	SFile file(sXmlFileName);
	if(!file.open(IO_ReadOnly))
	{
		printf("ParseXmlFile('%s') error, file not exist!\n",sXmlFileName);
		return false;
	}
	int filelen = file.size();
	if(filelen == 0)
	{
		printf("ParseXmlFile('%s') error, file is empty!\n",sXmlFileName);
		return false;
	}
	m_psTextStart = new char[filelen+1];
	m_iXmlTextSize = filelen;
	m_ppsTextNow = &m_psTextStart;
	//当文件超过10M时分批读取
	int ret = 0;
	while(ret != filelen)
	{
		int len = filelen-ret;
		if(len > 10485760)
			len = 1048576;
		int ret0 = file.readBlock((BYTE*)m_psTextStart+ret,len);
		if(ret0 != len)
		{
			m_ppsTextNow = NULL;
			file.close();
			printf("ParseXmlFile('%s') error, read file error!\n",sXmlFileName);
			delete[] m_psTextStart;
			return false;
		}
		ret += ret0;
	}
	//int ret = file.readBlock((BYTE*)m_psTextStart,filelen);
	if(ret != filelen)
	{
		m_ppsTextNow = NULL;
		file.close();
		printf("ParseXmlFile('%s') error, read file error!\n",sXmlFileName);
		delete[] m_psTextStart;
		return false;
	}
	m_psTextStart[ret] = '\0';
	file.close();

	char *pEncoding = strstr(m_psTextStart,"encoding=");
	if(pEncoding != NULL)
	{
		pEncoding += 10;
		//while(*pEncoding != '\'' && *pEncoding != '"' && *pEncoding != '?' )
		//	pEncoding++;
		if(SString::toString(pEncoding,5).toUpper() == "UTF-8" && encoding == SSaxXml_GBK)
		{
			//UTF-8编码需要转为GBK，暂时不考虑GB转为UTF8的情况（极少使用）
#if 0//def WIN32//文件太大时有问题，统一分批转
			//Windows下只需转换一次
			SString::Utf82Gb(m_psTextStart,filelen+1);
			filelen = strlen(m_psTextStart);
#else
			//char *pSrc = sXMLData;
			SString sTemp,sNew;
			int i=0,cnt=filelen;
			int s;
			int size = 10240;//10240
			sNew.SetBuffer(filelen);
			for(i=0;i<cnt;i+=s)
			{
				s = filelen-i;
				if(s > size)
					s = size;
				while(((unsigned char)m_psTextStart[i+s-1]) >= 128 && s > size/2)
				{
					s --;
				}
				sTemp = SString::toString(m_psTextStart+i,s);
				sNew += sTemp.toGb2312();
			}
			char *pNewStr = sNew.data();
			filelen = (int)strlen(pNewStr);
			while(filelen > 0 && *pNewStr != '\0' && *pNewStr != '<')
			{
				pNewStr++;
				filelen--;
			}
			memcpy(m_psTextStart,pNewStr,filelen);
			m_psTextStart[filelen] = '\0';
#endif
		}
	}

	StartDocument();
	filelen = (filelen>>2);
	if(filelen < 102400)
		filelen = 102400;
	m_ConstString.SetBufferSize(filelen);
	if(!ParseText(m_psTextStart, m_psValidText, true))
	{
		EndDocument();
		m_ppsTextNow = NULL;
		delete[] m_psTextStart;
		return false;
	}

	EndDocument();
	m_ppsTextNow = NULL;
	delete[] m_psTextStart;
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  解析一段XML文件，可分多次解析一个文件，文本应采用GBK/GB18030字符编码
// 作    者:  邵凯田
// 创建时间:  2015-4-8 9:34
// 参数说明:  @sXmlText表示待解析的文本内容
//         :  @sValidXmlText表示被有效解析的文本位置，下一次解析时应从将该位置开始
//         :  @bLastText表示是否为最后一批待解析文本
// 返 回 值:  true表示解析成功，false表示解析失败
//////////////////////////////////////////////////////////////////////////
bool SSaxXmlEx::ParseText(char* sXmlText0, char* &sValidXmlText,bool bLastText)
{
	if(m_pRootNodeCallbackFun == NULL)
		return false;
	SString sXmlVer="";
	register char* sXmlText = sXmlText0;
	register char prefix;
	register char *pStr,*s0,*s1,*s2,*s3;
	char * p_maohao;
	int iPos;

	SString sTemp;
	m_ppsTextNow = &sXmlText;
	sValidXmlText = sXmlText;
	if(sXmlText[0] != '<')
		sXmlText = strstr(sXmlText,"<");
	if(sXmlText == NULL)
	{
		m_ppsTextNow = &sValidXmlText;
		return false;
	}
	int iAttrBufSum = 128;//属性最大数量
	int iThisAttrs;//当前节点的属性数量
	SSaxAttribute *pSaxAttr=new SSaxAttribute[iAttrBufSum];
	SSaxNodeEx *pThisNode = NULL;
	SString sNodeValue;
	stuSaxChoice *pChoice = NULL;//子节点入口
	stuSaxChoice *pThisChoice = NULL;//当前选中的子节点
	while(1)
	{
		//去空格
		while((*sXmlText> 0 && isspace(*sXmlText)) && *sXmlText!='\0')
			sXmlText++;
		if(*sXmlText == '\0')
		{
			if(bLastText && !m_XmlStackEx.is_empty())
			{
				Error(SSaxErr_SYNTAX_ERR,"XML结尾不合法!");
				if(pSaxAttr != NULL)
					delete[] pSaxAttr;
				m_ppsTextNow = &sValidXmlText;
				return false;
			}
			break;
		}
		if (*sXmlText != '<') // 找不到节点起始标志!
		{
			if(!m_XmlStackEx.is_empty())
			{
				//读取节点值到当前节点
				m_sAttrValue[0] = '\0';
				int iPos = 0;
				while(*sXmlText != '<' && *sXmlText != '\0')
				{
					if(*sXmlText == '&')
					{
						if(SString::equals(sXmlText,"&apos;"))
						{
							m_sAttrValue[iPos++] = '\'';
							sXmlText += 6;
							continue;
						}
						else if(SString::equals(sXmlText,"&quot;"))
						{
							m_sAttrValue[iPos++] = '"';
							sXmlText += 6;
							continue;
						}
						else if(SString::equals(sXmlText,"&lt;"))
						{
							m_sAttrValue[iPos++] = '<';
							sXmlText += 4;
							continue;
						}
						else if(SString::equals(sXmlText,"&gt;"))
						{
							m_sAttrValue[iPos++] = '>';
							sXmlText += 4;
							continue;
						}
						else if(SString::equals(sXmlText,"&amp;"))
						{
							m_sAttrValue[iPos++] = '&';
							sXmlText += 5;
							continue;
						}
					}					
					m_sAttrValue[iPos++] = *sXmlText;
					sXmlText++;
				}
				m_sAttrValue[iPos] = '\0';
				if(*sXmlText == '<' || (bLastText && *sXmlText != '\0'))
				{
// 					if(pThisChoice != NULL && pThisChoice->pCallbackFun != NULL)
// 					{
// 						pChoice = pThisChoice->pCallbackFun(this,pSaxAttr,iThisAttrs,m_sAttrValue);
// 						pThisChoice = NULL;
// 					}
					//Characters(m_sAttrValue);
					sValidXmlText = sXmlText;
				}
				else
				{
// 					if(pThisChoice != NULL && pThisChoice->pCallbackFun != NULL)
// 					{
// 						pChoice = pThisChoice->pCallbackFun(this,pSaxAttr,iThisAttrs,"");
// 						pThisChoice = NULL;
// 					}
				}
				continue;
			}
			if(pSaxAttr != NULL)
				delete[] pSaxAttr;
			m_ppsTextNow = &sValidXmlText;
			return false;
		}
		else
		{
			//<![CDATA[content]]>
			if(strcmp(sXmlText,"<![CDATA[") == 0)
			{
				sXmlText += 8;
				pStr = strstr(sXmlText,"]]>");
				if(pStr != NULL)
				{
					//认为是原始文本
					//sNodeValue = SString::toString(sXmlText,pStr-sXmlText);
					if(!m_XmlStackEx.is_empty())
					{
// 						if(pThisChoice != NULL && pThisChoice->pCallbackFun != NULL)
// 						{
// 							int len = pStr-sXmlText;
// 							char ch = sXmlText[len];
// 							sXmlText[len] = '\0';
// 							pChoice = pThisChoice->pCallbackFun(this,pSaxAttr,iThisAttrs,sXmlText);
// 							pThisChoice = NULL;
// 							sXmlText[len] = ch;
// 						}
						//Characters(sNodeValue.data());
					}
// 					else if(pThisChoice != NULL && pThisChoice->pCallbackFun != NULL)
// 					{
// 						pChoice = pThisChoice->pCallbackFun(this,pSaxAttr,iThisAttrs,"");
// 						pThisChoice = NULL;
// 					}
					sXmlText = pStr + 3;
					sValidXmlText = sXmlText;
				}
				else if(bLastText)
				{
					//未关闭的原始文本
					if(pSaxAttr != NULL)
						delete[] pSaxAttr;
					if(!bLastText)
					{
						m_ppsTextNow = &sValidXmlText;
						return true;
					}
					Error(SSaxErr_SYNTAX_ERR,"未关闭的原始文本CDATA!");
					m_ppsTextNow = &sValidXmlText;
					return false;
				}
				break;
			}
			else
			{
// 				if(pThisChoice != NULL && pThisChoice->pCallbackFun != NULL)
// 				{
// 					pChoice = pThisChoice->pCallbackFun(this,pSaxAttr,iThisAttrs,"");
// 					pThisChoice = NULL;
// 				}
			}
		}
		sXmlText++;
		switch(*sXmlText)
		{
		case '?': // 跳过版本检查
			sXmlVer += "<";
			while (*sXmlText!='>' && *sXmlText!='<' && *sXmlText!='\0')
			{
				sXmlVer += *sXmlText;
				sXmlText++;
			}
			if (*sXmlText++ !='>')
			{
				if(pSaxAttr != NULL)
					delete[] pSaxAttr;
				if(!bLastText)
				{
					m_ppsTextNow = &sValidXmlText;
					return true;
				}
				Error(SSaxErr_SYNTAX_ERR,"无效的XML语法申明!");
				m_ppsTextNow = &sValidXmlText;
				return false;
			}

			sXmlVer += '>';
			sValidXmlText = sXmlText;
			break;

		case '!': // 跳过注释
			//<![CDATA[content]]>
			if(SString::equals(sXmlText,"![CDATA["))
			{
				sXmlText += 8;
				pStr = strstr(sXmlText,"]]>");
				if(pStr != NULL)
				{
					//认为是原始文本
					sNodeValue = SString::toString(sXmlText,(int)(pStr-sXmlText));
// 					if(!m_XmlStack.is_empty())
// 						Characters(sNodeValue.data());
					sXmlText = pStr + 3;
					sValidXmlText = sXmlText;
				}
				else if(bLastText)
				{
					//未关闭的原始文本
					if(pSaxAttr != NULL)
						delete[] pSaxAttr;
					if(!bLastText)
					{
						m_ppsTextNow = &sValidXmlText;
						return true;
					}
					Error(SSaxErr_SYNTAX_ERR,"未关闭的原始文本CDATA!");
					m_ppsTextNow = &sValidXmlText;
					return false;
				}
				break;
			}
			sXmlText = strstr(sXmlText,"-->");
			if(sXmlText == NULL && bLastText)
			{
				if(pSaxAttr != NULL)
					delete[] pSaxAttr;
				if(!bLastText)
				{
					m_ppsTextNow = &sValidXmlText;
					return true;
				}
				Error(SSaxErr_SYNTAX_ERR,"未关闭的注释!");
				m_ppsTextNow = &sValidXmlText;
				return false;
			}
			sXmlText += 3;//strlen("-->");
			sValidXmlText = sXmlText;
			break;

		case '/': // 节点闭环标志: </XXX>
			//当前当前父节点
			if(m_XmlStackEx.is_empty())
			{
				//没有要关闭的节点
				if(pSaxAttr != NULL)
					delete[] pSaxAttr;
				if(!bLastText)
				{
					m_ppsTextNow = &sValidXmlText;
					return true;
				}
				Error(SSaxErr_SYNTAX_ERR,"没有要关闭的节点!");
				m_ppsTextNow = &sValidXmlText;
				return false;
			}
			sXmlText++;
			m_sNodeName[0] = '\0';
			iPos = 0;
			while(!((*sXmlText> 0 && isspace(*sXmlText)) || '='==*sXmlText || '\n'==*sXmlText || '>'==*sXmlText || '/' == *sXmlText || '\0' == *sXmlText))
			{				
				m_sNodeName[iPos++] = *sXmlText;
				sXmlText ++;
			}
			m_sNodeName[iPos] = '\0';
			// 			s0 = strstr(m_sNodeName,":");
			// 			if(s0 != NULL)
			// 				s0 += 1;
			// 			else
			s0 = m_sNodeName;
// 			p_maohao = strstr(s0,":");
// 			if(p_maohao != 0)
// 				s0 = p_maohao+1;
			if(s0[0] == '\0')
			{
				//缺少节点名称
				if(pSaxAttr != NULL)
					delete[] pSaxAttr;
				if(!bLastText)
				{
					m_ppsTextNow = &sValidXmlText;
					return true;
				}
				Error(SSaxErr_SYNTAX_ERR,"缺少待关闭的节点名称!");
				m_ppsTextNow = &sValidXmlText;
				return false;
			}
			if(strcmp(s0, m_XmlStackEx.top()->psNodeName) != 0)
			{
				//关闭节点名不正确
				if(pSaxAttr != NULL)
					delete[] pSaxAttr;
				if(!bLastText)
				{
					m_ppsTextNow = &sValidXmlText;
					return true;
				}
				Error(SSaxErr_SYNTAX_ERR,"关闭节点名称不匹配!");
				m_ppsTextNow = &sValidXmlText;
				return false;
			}
			//去空格
			while((*sXmlText> 0 && isspace(*sXmlText)) && *sXmlText!='\0')
				sXmlText++;
			if(*sXmlText != '>')
			{
				//关闭节点没有封装
				if(pSaxAttr != NULL)
					delete[] pSaxAttr;
				if(!bLastText)
				{
					m_ppsTextNow = &sValidXmlText;
					return true;
				}
				Error(SSaxErr_SYNTAX_ERR,"关闭节点没有封装!");
				m_ppsTextNow = &sValidXmlText;
				return false;
			}
			sXmlText++;
			sValidXmlText = sXmlText;

			//弹出栈中的顶节点
			if(pThisNode != NULL)
				delete pThisNode;
			pThisNode = m_XmlStackEx.pop();
			if(m_XmlStackEx.size() > 0)
			{
				pChoice = m_XmlStackEx.top()->pChoice;
			}
			else
			{
				pChoice = NULL;
			}
			//EndElement(pThisNode->m_sNodeName.data());
			break;

		default:
			//提取节点信息到pNode中
			sValidXmlText = sXmlText;
			m_sNodeName[0] = '\0';
			m_iNodeNameLen = 0;
			while(!((*sXmlText> 0 && isspace(*sXmlText)) || '='==*sXmlText || '\n'==*sXmlText || '>'==*sXmlText || '/' == *sXmlText || '\0' == *sXmlText))
			{
				m_sNodeName[m_iNodeNameLen++] = *sXmlText;
				// 				m_sNodeName[strlen(m_sNodeName)+1] = '\0';
				// 				m_sNodeName[strlen(m_sNodeName)] = *sXmlText;
				sXmlText++;
			}
			m_sNodeName[m_iNodeNameLen] = '\0';
			// 			s0 = strstr(m_sNodeName,":");
			// 			if(s0 != NULL)
			// 				s0 += 1;
			// 			else
			s0 = m_sNodeName;
// 			p_maohao = strstr(s0,":");
// 			if(p_maohao != 0)
// 				s0 = p_maohao+1;
			if(m_iNodeNameLen == 0)
			{
				//缺少节点名称
				if(pSaxAttr != NULL)
					delete[] pSaxAttr;
				if(!bLastText)
				{
					m_ppsTextNow = &sValidXmlText;
					return true;
				}
				Error(SSaxErr_SYNTAX_ERR,"缺少节点名称!");
				m_ppsTextNow = &sValidXmlText;
				return false;
			}
			if(pChoice == NULL)
			{
				//根节点
				if(m_sRootNodeName.length() > 0 && m_sRootNodeName != s0)
				{
					//根节点名称不匹配
					if(pSaxAttr != NULL)
						delete[] pSaxAttr;
					if(!bLastText)
					{
						m_ppsTextNow = &sValidXmlText;
						return true;
					}
					Error(SSaxErr_SYNTAX_ERR,"根节点名称不匹配!");
					m_ppsTextNow = &sValidXmlText;
					return false;
				}
			}
			else
			{
				//根据当前的选择项，验证是否为合法的子节点
				pThisChoice = pChoice;
				while(pThisChoice->sNodeName != NULL)
				{
					if(SString::equals((char*)pThisChoice->sNodeName,(char*)s0))
						break;
					pThisChoice++;
				}
				if(pThisChoice->sNodeName == NULL)
				{
					//忽略当前节点
					//是判断当前节点是否立即关闭
					s1 = strstr(sXmlText,">");
					if(s1 == NULL)
					{
						//文档非法结束
						if(pSaxAttr != NULL)
							delete[] pSaxAttr;
						if(!bLastText)
						{
							m_ppsTextNow = &sValidXmlText;
							return true;
						}
						Error(SSaxErr_SYNTAX_ERR,"文档非法结束!");
						m_ppsTextNow = &sValidXmlText;
						return false;
					}
					if(s1[-1] == '/')
					{
						//立即结束
						//跳过当前节点
						sXmlText = s1+1;
						break;
					}
					else
					{
						s2 = s1;
						aga_seek:
						sTemp = "</";
						sTemp += m_sNodeName;
						sTemp += ">";
						s1 = strstr(s1,sTemp.data());
						if(s1 == NULL)
						{
							//文档非法结束
							if(pSaxAttr != NULL)
								delete[] pSaxAttr;
							if(!bLastText)
							{
								m_ppsTextNow = &sValidXmlText;
								return true;
							}
							Error(SSaxErr_SYNTAX_ERR,"文档非法结束!");
							m_ppsTextNow = &sValidXmlText;
							return false;
						}
						else
						{
							//将s1位置临时改为结束符
							*s1 = '\0';//原来是<

							//是否中间中重复名称的子节点
							s3 = strstr(s2,SString::toFormat("<%s ",m_sNodeName).data());
							if(s3 == NULL)
								s3 = strstr(s2,SString::toFormat("<%s>",m_sNodeName).data());
							*s1 = '<';//还原
							if(s3 != NULL && s3 < s1)
							{
								//有重复节点
								s1 = s2 = s1+2;
								goto aga_seek;
							}
						}
						//跳过当前节点
						sXmlText = s1+sTemp.length();
						break;
					}
				}
			}

			//去空格
			while((*sXmlText> 0 && isspace(*sXmlText)) && *sXmlText!='\0')
				sXmlText++;
			//提取属性
			iThisAttrs = 0;
			while(*sXmlText != '>' && *sXmlText != '/' && *sXmlText != '\0')
			{
				m_sAttrName[0] = '\0';
				m_iAttrNameLen = 0;
				//register char ch = *sXmlText;
				while(!((*sXmlText> 0 && isspace(*sXmlText)) || '='==*sXmlText || '\n'==*sXmlText || '>'==*sXmlText || '/' == *sXmlText))
					//while(!((isspace(ch) && (unsigned char)ch<0x80) || '='==ch || '\n'==ch || '>'==ch || '/' == ch))
				{
					if(*sXmlText == '\0')
					{
						if(pSaxAttr != NULL)
							delete[] pSaxAttr;
						if(!bLastText)
						{
							m_ppsTextNow = &sValidXmlText;
							return true;
						}
						Error(SSaxErr_SYNTAX_ERR,"无效的XML结尾!");
						m_ppsTextNow = &sValidXmlText;
						return false;
					}
					m_sAttrName[m_iAttrNameLen++] = *sXmlText;//ch;
					// 					m_sAttrName[strlen(m_sAttrName)+1] = '\0';
					// 					m_sAttrName[strlen(m_sAttrName)] = *sXmlText;//ch;
					sXmlText++;
					//ch = *sXmlText;
				}
				m_sAttrName[m_iAttrNameLen] = '\0';

				if(m_sAttrName[0] == '\0' || *sXmlText == '\0')
				{
					//缺少属性名
					if(pSaxAttr != NULL)
						delete[] pSaxAttr;
					if(!bLastText)
					{
						m_ppsTextNow = &sValidXmlText;
						return true;
					}
					Error(SSaxErr_SYNTAX_ERR,"缺少属性名称!");
					m_ppsTextNow = &sValidXmlText;
					return false;
				}
				//去空格
				while((*sXmlText> 0 && isspace(*sXmlText)) && *sXmlText!='\0')
					sXmlText++;
				if(*sXmlText != '=')
				{
					//缺少属性=
					if(pSaxAttr != NULL)
						delete[] pSaxAttr;
					if(!bLastText)
					{
						m_ppsTextNow = &sValidXmlText;
						return true;
					}
					Error(SSaxErr_SYNTAX_ERR,"缺少属性‘=’!");
					m_ppsTextNow = &sValidXmlText;
					return false;
				}
				sXmlText++;
				//去空格
				while((*sXmlText> 0 && isspace(*sXmlText)) && *sXmlText!='\0')
					sXmlText++;
				if(*sXmlText != '\'' && *sXmlText != '"')
				{
					//缺少属性值的引号
					if(pSaxAttr != NULL)
						delete[] pSaxAttr;
					if(!bLastText)
					{
						m_ppsTextNow = &sValidXmlText;
						return true;
					}
					Error(SSaxErr_SYNTAX_ERR,"缺少属性值的引号!");
					m_ppsTextNow = &sValidXmlText;
					return false;
				}
				prefix = *sXmlText;
				sXmlText++;
				m_sAttrValue[0] = '\0';
				m_iAttrValueLen = 0;
				while(*sXmlText != prefix && *sXmlText != '\0')
				{
					if(*sXmlText == '&')
					{
						if(SString::equals(sXmlText,"&apos;"))
						{
							m_sAttrValue[m_iAttrValueLen++] = '\'';
							// 							m_sAttrValue[strlen(m_sAttrValue)+1] = '\0';
							// 							m_sAttrValue[strlen(m_sAttrValue)] = '\'';
							sXmlText += 6;
							continue;
						}
						else if(SString::equals(sXmlText,"&quot;"))
						{
							m_sAttrValue[m_iAttrValueLen++] = '"';
							// 							m_sAttrValue[strlen(m_sAttrValue)+1] = '\0';
							// 							m_sAttrValue[strlen(m_sAttrValue)] = '"';
							sXmlText += 6;
							continue;
						}
						else if(SString::equals(sXmlText,"&lt;"))
						{
							m_sAttrValue[m_iAttrValueLen++] = '<';
							// 							m_sAttrValue[strlen(m_sAttrValue)+1] = '\0';
							// 							m_sAttrValue[strlen(m_sAttrValue)] = '<';
							sXmlText += 4;
							continue;
						}
						else if(SString::equals(sXmlText,"&gt;"))
						{
							m_sAttrValue[m_iAttrValueLen++] = '>';
							// 							m_sAttrValue[strlen(m_sAttrValue)+1] = '\0';
							// 							m_sAttrValue[strlen(m_sAttrValue)] = '>';
							sXmlText += 4;
							continue;
						}
						else if(SString::equals(sXmlText,"&amp;"))
						{
							m_sAttrValue[m_iAttrValueLen++] = '&';
							// 							m_sAttrValue[strlen(m_sAttrValue)+1] = '\0';
							// 							m_sAttrValue[strlen(m_sAttrValue)] = '&';
							sXmlText += 5;
							continue;
						}
					}
					m_sAttrValue[m_iAttrValueLen++] = *sXmlText;
					// 					m_sAttrValue[strlen(m_sAttrValue)+1] = '\0';
					// 					m_sAttrValue[strlen(m_sAttrValue)] = *sXmlText;
					sXmlText++;
				}
				m_sAttrValue[m_iAttrValueLen] = '\0';

				if(*sXmlText != prefix)
				{
					if(pSaxAttr != NULL)
						delete[] pSaxAttr;
					if(bLastText == false)
					{
						m_ppsTextNow = &sValidXmlText;
						return true;
					}
					//属性值没有结束
					Error(SSaxErr_SYNTAX_ERR,"属性值 没有结束!");
					m_ppsTextNow = &sValidXmlText;
					return false;
				}
				if(iThisAttrs >= iAttrBufSum)
				{
					iAttrBufSum *= 2;
					SSaxAttribute *pNew = new SSaxAttribute[iAttrBufSum];
					for(int i=0;i<iThisAttrs;i++)
					{
						pNew[i].m_sAttrName = pSaxAttr[i].m_sAttrName;
						pNew[i].m_sAttrValue = pSaxAttr[i].m_sAttrValue;
					}
					delete[] pSaxAttr;
					pSaxAttr = pNew;
				}
				pSaxAttr[iThisAttrs].m_sAttrName = m_sAttrName;
				pSaxAttr[iThisAttrs].m_sAttrValue = m_sAttrValue;
				iThisAttrs ++;
				sXmlText++;
				//去空格
				while((*sXmlText> 0 && isspace(*sXmlText)) && *sXmlText!='\0')
					sXmlText++;
			}//end while
			if(pThisNode == NULL)
			{
				pThisNode = new SSaxNodeEx;
				if(m_XmlStackEx.size() == 0)
				{
					//ROOT Node
					pChoice = pThisNode->pChoice = m_pRootNodeCallbackFun(this,pSaxAttr,iThisAttrs,(char*)"");
				}
				else
				{
					pChoice = pThisNode->pChoice = m_XmlStackEx.top()->pChoice;
				}
			}
			if(*sXmlText == '/')
			{
				//无下级节点及节点值，立即关闭当前节点
				sXmlText++;
				if(*sXmlText != '>')
				{
					//'/'后没有跟'>'
					if(pSaxAttr != NULL)
						delete[] pSaxAttr;
					if(!bLastText)
					{
						return true;
						m_ppsTextNow = &sValidXmlText;
					}
					Error(SSaxErr_SYNTAX_ERR,"'/'后没有跟'>' !");
					m_ppsTextNow = &sValidXmlText;
					return false;
				}
				if(pThisChoice != NULL)
				{
					/*pChoice = */pThisChoice->pCallbackFun(this,pSaxAttr,iThisAttrs,(char*)"");
					pThisChoice = NULL;
				}
				//StartElement(s0,pSaxAttr,iThisAttrs);
				//EndElement(s0);
				sXmlText++;
				sValidXmlText = sXmlText;
				break;
			}
			if(*sXmlText == '\0')
			{
				if(bLastText)
				{
					//意外的结尾
					//节点未关闭
					if(pSaxAttr != NULL)
						delete[] pSaxAttr;
					if(!bLastText)
					{
						m_ppsTextNow = &sValidXmlText;
						return true;
					}
					Error(SSaxErr_SYNTAX_ERR,"节点未关闭!");
					m_ppsTextNow = &sValidXmlText;
					return false;
				}
				break;
			}
			if(*sXmlText != '>' && bLastText)
			{
				//节点未关闭
				if(pSaxAttr != NULL)
					delete[] pSaxAttr;
				if(!bLastText)
				{
					m_ppsTextNow = &sValidXmlText;
					return true;
				}
				Error(SSaxErr_SYNTAX_ERR,"节点未关闭!");
				m_ppsTextNow = &sValidXmlText;
				return false;
			}
			sXmlText ++;

			if(pThisChoice != NULL)
			{
				//取可能出现的节点值内容
				while((*sXmlText> 0 && isspace(*sXmlText)) && *sXmlText!='\0')
					sXmlText++;
				if(sXmlText[0] != '<')
				{
					pStr = strstr(sXmlText,"<");
					if(pThisChoice != NULL && pThisChoice->pCallbackFun != NULL)
					{
						int len = (int)(pStr-sXmlText);
						char ch = sXmlText[len];
						sXmlText[len] = '\0';
						pChoice = pThisChoice->pCallbackFun(this,pSaxAttr,iThisAttrs,sXmlText);
						pThisChoice = NULL;
						sXmlText[len] = ch;
					}
					sXmlText = pStr;
				}
				else if(strcmp(sXmlText,"<![CDATA[") == 0)
				{
					sXmlText += 8;
					pStr = strstr(sXmlText,"]]>");
					if(pStr != NULL)
					{
						//认为是原始文本
						//sNodeValue = SString::toString(sXmlText,pStr-sXmlText);
						if(!m_XmlStackEx.is_empty())
						{
							if(pThisChoice != NULL && pThisChoice->pCallbackFun != NULL)
							{
								int len = (int)(pStr-sXmlText);
								char ch = sXmlText[len];
								sXmlText[len] = '\0';
								pChoice = pThisChoice->pCallbackFun(this,pSaxAttr,iThisAttrs,sXmlText);
								pThisChoice = NULL;
								sXmlText[len] = ch;
							}
							//Characters(sNodeValue.data());
						}
						sXmlText = pStr + 3;
						sValidXmlText = (char*)sXmlText;
					}
				}
				else if(sXmlText[0] == '<')
				{
					pChoice = pThisChoice->pCallbackFun(this,pSaxAttr,iThisAttrs,(char*)"");
					pThisChoice = NULL;
					sXmlText--;
				}
			}
			//StartElement(s0,pSaxAttr,iThisAttrs);
			pThisNode->psNodeName = (char*)m_ConstString.CopyToConstString(s0);// s0;
			pThisNode->pChoice = pChoice;
			m_XmlStackEx.push(pThisNode);
			pThisNode = NULL;

			sValidXmlText = sXmlText;
			//提取下级节点
			break;
		}

	}
	if(pSaxAttr != NULL)
		delete[] pSaxAttr;
	if(pThisNode != NULL)
	{
		delete pThisNode;
		pThisNode = NULL;
	}
	if(bLastText && !m_XmlStackEx.is_empty())
	{
		Error(SSaxErr_SYNTAX_ERR,"XML未正常结束!");
		m_ppsTextNow = &sValidXmlText;
		return false;
	}
	m_ppsTextNow = &sValidXmlText;
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  复位解析器，为下一个完整的XML解析做准备
// 作    者:  邵凯田
// 创建时间:  2015-4-8 9:54
// 参数说明:  void
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SSaxXmlEx::ResetParser()
{
	SSaxNodeEx *p;
	while(!m_XmlStackEx.is_empty())
	{
		p = m_XmlStackEx.pop();
		delete p;
	}
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  取当前处理进度百分比
// 作    者:  邵凯田
// 创建时间:  2015-12-22 9:14
// 参数说明:  void
// 返 回 值:  int(0~100)
//////////////////////////////////////////////////////////////////////////
int SSaxXmlEx::GetProgressPecent()
{
	char *p = m_psTextStart;
	char **pp= m_ppsTextNow;
	if(p == NULL)
	{
		if(pp == NULL)
			return 100;
		else
			return 0;
	}
	else if(pp == NULL)
		return 100;
	char *p2 = *pp;
	int len = (int)(((float)(p2-p)*100.0f)/(float)m_iXmlTextSize);
	if(len < 0)
		len = 0;
	else if(len > 100)
		len = 100;
	return len;
}

