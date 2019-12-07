/**
 *
 * �� �� �� : SSaxXml.cpp
 * �������� : 2015-4-7 16:18
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : ����SAX��XML������
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-4-7	�ۿ�������ļ�
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
// ��    ��:  ����XML�ļ�
// ��    ��:  �ۿ���
// ����ʱ��:  2015-4-8 9:28
// ����˵��:  @xXmlFileNameΪXML�ļ�����ȫ·��
//         :  @encoding��ʾ�����ı������ͣ�Ĭ��ΪGBK
// �� �� ֵ:  true��ʾ�����ɹ���false��ʾ����ʧ��
//////////////////////////////////////////////////////////////////////////
bool SSaxXml::ParseXmlFile(char* sXmlFileName, SSaxXmlEncoding encoding)
{
	char *sXMLData,*sValidXml;

	//��λ������
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
			//UTF-8������ҪתΪGBK����ʱ������GBתΪUTF8�����������ʹ�ã�
#ifdef WIN32
			//Windows��ֻ��ת��һ��
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
// ��    ��:  ����һ��XML�ļ����ɷֶ�ν���һ���ļ����ı�Ӧ����GBK/GB18030�ַ�����
// ��    ��:  �ۿ���
// ����ʱ��:  2015-4-8 9:34
// ����˵��:  @sXmlText��ʾ���������ı�����
//         :  @sValidXmlText��ʾ����Ч�������ı�λ�ã���һ�ν���ʱӦ�ӽ���λ�ÿ�ʼ
//         :  @bLastText��ʾ�Ƿ�Ϊ���һ���������ı�
// �� �� ֵ:  true��ʾ�����ɹ���false��ʾ����ʧ��
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
	int iAttrBufSum = 128;//�����������
	int iThisAttrs;//��ǰ�ڵ����������
	SSaxAttribute *pSaxAttr=new SSaxAttribute[iAttrBufSum];
	SSaxNode *pThisNode = NULL;
	SString sNodeValue;
	while(1)
	{
		//ȥ�ո�
		while((*sXmlText> 0 && isspace(*sXmlText)) && *sXmlText!='\0')
			sXmlText++;
		if(*sXmlText == '\0')
		{
			if(bLastText && !m_XmlStack.is_empty())
			{
				Error(SSaxErr_SYNTAX_ERR,"XML��β���Ϸ�!");
				if(pSaxAttr != NULL)
					delete[] pSaxAttr;
				return false;
			}
			break;
		}
		if (*sXmlText != '<') // �Ҳ����ڵ���ʼ��־!
		{
			if(!m_XmlStack.is_empty())
			{
				//��ȡ�ڵ�ֵ����ǰ�ڵ�
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
		case '?': // �����汾���
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
				Error(SSaxErr_SYNTAX_ERR,"��Ч��XML�﷨����!");
				return false;
			}

			sXmlVer += '>';
			sValidXmlText = sXmlText;
			break;

		case '!': // ����ע��
			//<![CDATA[content]]>
			//if(SString::equals(sXmlText,"![CDATA["))
			if(sXmlText[0] == '!' && sXmlText[1] == '[' && sXmlText[2] == 'C' && sXmlText[3] == 'D' && sXmlText[4] == 'A' && sXmlText[5] == 'T' && sXmlText[6] == 'A' && sXmlText[7] == '[')
			{
				sXmlText += 8;
				pStr = strstr(sXmlText,"]]>");
				if(pStr != NULL)
				{
					//��Ϊ��ԭʼ�ı�
					sNodeValue = SString::toString(sXmlText,(int)(pStr-sXmlText));
					if(!m_XmlStack.is_empty())
						Characters(sNodeValue.data());
					sXmlText = pStr + 3;
					sValidXmlText = sXmlText;
				}
				else if(bLastText)
				{
					//δ�رյ�ԭʼ�ı�
					if(pSaxAttr != NULL)
						delete[] pSaxAttr;
					if(!bLastText)
						return true;
					Error(SSaxErr_SYNTAX_ERR,"δ�رյ�ԭʼ�ı�CDATA!");
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
				Error(SSaxErr_SYNTAX_ERR,"δ�رյ�ע��!");
				return false;
			}
			sXmlText += 3;//strlen("-->");
			sValidXmlText = sXmlText;
			break;

		case '/': // �ڵ�ջ���־: </XXX>
			//��ǰ��ǰ���ڵ�
			if(m_XmlStack.is_empty())
			{
				//û��Ҫ�رյĽڵ�
				if(pSaxAttr != NULL)
					delete[] pSaxAttr;
				if(!bLastText)
					return true;
				Error(SSaxErr_SYNTAX_ERR,"û��Ҫ�رյĽڵ�!");
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
				//ȱ�ٽڵ�����
				if(pSaxAttr != NULL)
					delete[] pSaxAttr;
				if(!bLastText)
					return true;
				Error(SSaxErr_SYNTAX_ERR,"ȱ�ٴ��رյĽڵ�����!");
				return false;
			}
			if(strcmp(s0, m_XmlStack.top()->m_sNodeName.data()) != 0)
			{
				//�رսڵ�������ȷ
				if(pSaxAttr != NULL)
					delete[] pSaxAttr;
				if(!bLastText)
					return true;
				Error(SSaxErr_SYNTAX_ERR,"�رսڵ����Ʋ�ƥ��!");
				return false;
			}
			//ȥ�ո�
			while((*sXmlText> 0 && isspace(*sXmlText)) && *sXmlText!='\0')
				sXmlText++;
			if(*sXmlText != '>')
			{
				//�رսڵ�û�з�װ
				if(pSaxAttr != NULL)
					delete[] pSaxAttr;
				if(!bLastText)
					return true;
				Error(SSaxErr_SYNTAX_ERR,"�رսڵ�û�з�װ!");
				return false;
			}
			sXmlText++;
			sValidXmlText = sXmlText;

			//����ջ�еĶ��ڵ�
			if(pThisNode != NULL)
				delete pThisNode;
			pThisNode = m_XmlStack.pop();
			EndElement(pThisNode->m_sNodeName.data());
			break;

		default:
			//��ȡ�ڵ���Ϣ��pNode��
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
				//ȱ�ٽڵ�����
				if(pSaxAttr != NULL)
					delete[] pSaxAttr;
				if(!bLastText)
					return true;
				Error(SSaxErr_SYNTAX_ERR,"ȱ�ٽڵ�����!");
				return false;
			}

			//ȥ�ո�
			while((*sXmlText> 0 && isspace(*sXmlText)) && *sXmlText!='\0')
				sXmlText++;
			//��ȡ����
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
						Error(SSaxErr_SYNTAX_ERR,"��Ч��XML��β!");
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
					//ȱ��������
					if(pSaxAttr != NULL)
						delete[] pSaxAttr;
					if(!bLastText)
						return true;
					Error(SSaxErr_SYNTAX_ERR,"ȱ����������!");
					return false;
				}
				//ȥ�ո�
				while((*sXmlText> 0 && isspace(*sXmlText)) && *sXmlText!='\0')
					sXmlText++;
				if(*sXmlText != '=')
				{
					//ȱ������=
					if(pSaxAttr != NULL)
						delete[] pSaxAttr;
					if(!bLastText)
						return true;
					Error(SSaxErr_SYNTAX_ERR,"ȱ�����ԡ�=��!");
					return false;
				}
				sXmlText++;
				//ȥ�ո�
				while((*sXmlText> 0 && isspace(*sXmlText)) && *sXmlText!='\0')
					sXmlText++;
				if(*sXmlText != '\'' && *sXmlText != '"')
				{
					//ȱ������ֵ������
					if(pSaxAttr != NULL)
						delete[] pSaxAttr;
					if(!bLastText)
						return true;
					Error(SSaxErr_SYNTAX_ERR,"ȱ������ֵ������!");
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
					//����ֵû�н���
					Error(SSaxErr_SYNTAX_ERR,"����ֵ û�н���!");
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
				//ȥ�ո�
				while((*sXmlText> 0 && isspace(*sXmlText)) && *sXmlText!='\0')
					sXmlText++;
			}//end while
			
			if(*sXmlText == '/')
			{
				//���¼��ڵ㼰�ڵ�ֵ�������رյ�ǰ�ڵ�
				sXmlText++;
				if(*sXmlText != '>')
				{
					//'/'��û�и�'>'
					if(pSaxAttr != NULL)
						delete[] pSaxAttr;
					if(!bLastText)
						return true;
					Error(SSaxErr_SYNTAX_ERR,"'/'��û�и�'>' !");
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
					//����Ľ�β
					//�ڵ�δ�ر�
					if(pSaxAttr != NULL)
						delete[] pSaxAttr;
					if(!bLastText)
						return true;
					Error(SSaxErr_SYNTAX_ERR,"�ڵ�δ�ر�!");
					return false;
				}
				break;
			}
			if(*sXmlText != '>' && bLastText)
			{
				//�ڵ�δ�ر�
				if(pSaxAttr != NULL)
					delete[] pSaxAttr;
				if(!bLastText)
					return true;
				Error(SSaxErr_SYNTAX_ERR,"�ڵ�δ�ر�!");
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
			//��ȡ�¼��ڵ�
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
		Error(SSaxErr_SYNTAX_ERR,"XMLδ��������!");
		return false;
	}
	
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ��λ��������Ϊ��һ��������XML������׼��
// ��    ��:  �ۿ���
// ����ʱ��:  2015-4-8 9:54
// ����˵��:  void
// �� �� ֵ:  void
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
// ��    ��:  SSaxXmlEx
// ��    ��:  �ۿ���
// ����ʱ��:  2015-4-8 11:15
// ��    ��:  ����SAX��������XML�����࣬ͨ����ע��ص���������XML��
//            �ϲ㴦���಻��Ҫ���䵱ǰ����ڵ�λ�ã������ڲ�ͬ�ڵ�ע�᲻ͬ����ڻص�������
//			  ����ʹ�ý�SSaxXml��������Ч�ʸ���SSaxXml��
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
// ��    ��:  ����XML�ļ�
// ��    ��:  �ۿ���
// ����ʱ��:  2015-4-8 9:28
// ����˵��:  @xXmlFileNameΪXML�ļ�����ȫ·��
//         :  @encoding��ʾ�����ı������ͣ�Ĭ��ΪGBK
// �� �� ֵ:  true��ʾ�����ɹ���false��ʾ����ʧ��
//////////////////////////////////////////////////////////////////////////
bool SSaxXmlEx::ParseXmlFile(char* sXmlFileName, SSaxXmlEncoding encoding)
{
	if(m_pRootNodeCallbackFun == NULL)
		return false;
	//��λ������
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
	//���ļ�����10Mʱ������ȡ
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
			//UTF-8������ҪתΪGBK����ʱ������GBתΪUTF8�����������ʹ�ã�
#if 0//def WIN32//�ļ�̫��ʱ�����⣬ͳһ����ת
			//Windows��ֻ��ת��һ��
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
// ��    ��:  ����һ��XML�ļ����ɷֶ�ν���һ���ļ����ı�Ӧ����GBK/GB18030�ַ�����
// ��    ��:  �ۿ���
// ����ʱ��:  2015-4-8 9:34
// ����˵��:  @sXmlText��ʾ���������ı�����
//         :  @sValidXmlText��ʾ����Ч�������ı�λ�ã���һ�ν���ʱӦ�ӽ���λ�ÿ�ʼ
//         :  @bLastText��ʾ�Ƿ�Ϊ���һ���������ı�
// �� �� ֵ:  true��ʾ�����ɹ���false��ʾ����ʧ��
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
	int iAttrBufSum = 128;//�����������
	int iThisAttrs;//��ǰ�ڵ����������
	SSaxAttribute *pSaxAttr=new SSaxAttribute[iAttrBufSum];
	SSaxNodeEx *pThisNode = NULL;
	SString sNodeValue;
	stuSaxChoice *pChoice = NULL;//�ӽڵ����
	stuSaxChoice *pThisChoice = NULL;//��ǰѡ�е��ӽڵ�
	while(1)
	{
		//ȥ�ո�
		while((*sXmlText> 0 && isspace(*sXmlText)) && *sXmlText!='\0')
			sXmlText++;
		if(*sXmlText == '\0')
		{
			if(bLastText && !m_XmlStackEx.is_empty())
			{
				Error(SSaxErr_SYNTAX_ERR,"XML��β���Ϸ�!");
				if(pSaxAttr != NULL)
					delete[] pSaxAttr;
				m_ppsTextNow = &sValidXmlText;
				return false;
			}
			break;
		}
		if (*sXmlText != '<') // �Ҳ����ڵ���ʼ��־!
		{
			if(!m_XmlStackEx.is_empty())
			{
				//��ȡ�ڵ�ֵ����ǰ�ڵ�
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
					//��Ϊ��ԭʼ�ı�
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
					//δ�رյ�ԭʼ�ı�
					if(pSaxAttr != NULL)
						delete[] pSaxAttr;
					if(!bLastText)
					{
						m_ppsTextNow = &sValidXmlText;
						return true;
					}
					Error(SSaxErr_SYNTAX_ERR,"δ�رյ�ԭʼ�ı�CDATA!");
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
		case '?': // �����汾���
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
				Error(SSaxErr_SYNTAX_ERR,"��Ч��XML�﷨����!");
				m_ppsTextNow = &sValidXmlText;
				return false;
			}

			sXmlVer += '>';
			sValidXmlText = sXmlText;
			break;

		case '!': // ����ע��
			//<![CDATA[content]]>
			if(SString::equals(sXmlText,"![CDATA["))
			{
				sXmlText += 8;
				pStr = strstr(sXmlText,"]]>");
				if(pStr != NULL)
				{
					//��Ϊ��ԭʼ�ı�
					sNodeValue = SString::toString(sXmlText,(int)(pStr-sXmlText));
// 					if(!m_XmlStack.is_empty())
// 						Characters(sNodeValue.data());
					sXmlText = pStr + 3;
					sValidXmlText = sXmlText;
				}
				else if(bLastText)
				{
					//δ�رյ�ԭʼ�ı�
					if(pSaxAttr != NULL)
						delete[] pSaxAttr;
					if(!bLastText)
					{
						m_ppsTextNow = &sValidXmlText;
						return true;
					}
					Error(SSaxErr_SYNTAX_ERR,"δ�رյ�ԭʼ�ı�CDATA!");
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
				Error(SSaxErr_SYNTAX_ERR,"δ�رյ�ע��!");
				m_ppsTextNow = &sValidXmlText;
				return false;
			}
			sXmlText += 3;//strlen("-->");
			sValidXmlText = sXmlText;
			break;

		case '/': // �ڵ�ջ���־: </XXX>
			//��ǰ��ǰ���ڵ�
			if(m_XmlStackEx.is_empty())
			{
				//û��Ҫ�رյĽڵ�
				if(pSaxAttr != NULL)
					delete[] pSaxAttr;
				if(!bLastText)
				{
					m_ppsTextNow = &sValidXmlText;
					return true;
				}
				Error(SSaxErr_SYNTAX_ERR,"û��Ҫ�رյĽڵ�!");
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
				//ȱ�ٽڵ�����
				if(pSaxAttr != NULL)
					delete[] pSaxAttr;
				if(!bLastText)
				{
					m_ppsTextNow = &sValidXmlText;
					return true;
				}
				Error(SSaxErr_SYNTAX_ERR,"ȱ�ٴ��رյĽڵ�����!");
				m_ppsTextNow = &sValidXmlText;
				return false;
			}
			if(strcmp(s0, m_XmlStackEx.top()->psNodeName) != 0)
			{
				//�رսڵ�������ȷ
				if(pSaxAttr != NULL)
					delete[] pSaxAttr;
				if(!bLastText)
				{
					m_ppsTextNow = &sValidXmlText;
					return true;
				}
				Error(SSaxErr_SYNTAX_ERR,"�رսڵ����Ʋ�ƥ��!");
				m_ppsTextNow = &sValidXmlText;
				return false;
			}
			//ȥ�ո�
			while((*sXmlText> 0 && isspace(*sXmlText)) && *sXmlText!='\0')
				sXmlText++;
			if(*sXmlText != '>')
			{
				//�رսڵ�û�з�װ
				if(pSaxAttr != NULL)
					delete[] pSaxAttr;
				if(!bLastText)
				{
					m_ppsTextNow = &sValidXmlText;
					return true;
				}
				Error(SSaxErr_SYNTAX_ERR,"�رսڵ�û�з�װ!");
				m_ppsTextNow = &sValidXmlText;
				return false;
			}
			sXmlText++;
			sValidXmlText = sXmlText;

			//����ջ�еĶ��ڵ�
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
			//��ȡ�ڵ���Ϣ��pNode��
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
				//ȱ�ٽڵ�����
				if(pSaxAttr != NULL)
					delete[] pSaxAttr;
				if(!bLastText)
				{
					m_ppsTextNow = &sValidXmlText;
					return true;
				}
				Error(SSaxErr_SYNTAX_ERR,"ȱ�ٽڵ�����!");
				m_ppsTextNow = &sValidXmlText;
				return false;
			}
			if(pChoice == NULL)
			{
				//���ڵ�
				if(m_sRootNodeName.length() > 0 && m_sRootNodeName != s0)
				{
					//���ڵ����Ʋ�ƥ��
					if(pSaxAttr != NULL)
						delete[] pSaxAttr;
					if(!bLastText)
					{
						m_ppsTextNow = &sValidXmlText;
						return true;
					}
					Error(SSaxErr_SYNTAX_ERR,"���ڵ����Ʋ�ƥ��!");
					m_ppsTextNow = &sValidXmlText;
					return false;
				}
			}
			else
			{
				//���ݵ�ǰ��ѡ�����֤�Ƿ�Ϊ�Ϸ����ӽڵ�
				pThisChoice = pChoice;
				while(pThisChoice->sNodeName != NULL)
				{
					if(SString::equals((char*)pThisChoice->sNodeName,(char*)s0))
						break;
					pThisChoice++;
				}
				if(pThisChoice->sNodeName == NULL)
				{
					//���Ե�ǰ�ڵ�
					//���жϵ�ǰ�ڵ��Ƿ������ر�
					s1 = strstr(sXmlText,">");
					if(s1 == NULL)
					{
						//�ĵ��Ƿ�����
						if(pSaxAttr != NULL)
							delete[] pSaxAttr;
						if(!bLastText)
						{
							m_ppsTextNow = &sValidXmlText;
							return true;
						}
						Error(SSaxErr_SYNTAX_ERR,"�ĵ��Ƿ�����!");
						m_ppsTextNow = &sValidXmlText;
						return false;
					}
					if(s1[-1] == '/')
					{
						//��������
						//������ǰ�ڵ�
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
							//�ĵ��Ƿ�����
							if(pSaxAttr != NULL)
								delete[] pSaxAttr;
							if(!bLastText)
							{
								m_ppsTextNow = &sValidXmlText;
								return true;
							}
							Error(SSaxErr_SYNTAX_ERR,"�ĵ��Ƿ�����!");
							m_ppsTextNow = &sValidXmlText;
							return false;
						}
						else
						{
							//��s1λ����ʱ��Ϊ������
							*s1 = '\0';//ԭ����<

							//�Ƿ��м����ظ����Ƶ��ӽڵ�
							s3 = strstr(s2,SString::toFormat("<%s ",m_sNodeName).data());
							if(s3 == NULL)
								s3 = strstr(s2,SString::toFormat("<%s>",m_sNodeName).data());
							*s1 = '<';//��ԭ
							if(s3 != NULL && s3 < s1)
							{
								//���ظ��ڵ�
								s1 = s2 = s1+2;
								goto aga_seek;
							}
						}
						//������ǰ�ڵ�
						sXmlText = s1+sTemp.length();
						break;
					}
				}
			}

			//ȥ�ո�
			while((*sXmlText> 0 && isspace(*sXmlText)) && *sXmlText!='\0')
				sXmlText++;
			//��ȡ����
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
						Error(SSaxErr_SYNTAX_ERR,"��Ч��XML��β!");
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
					//ȱ��������
					if(pSaxAttr != NULL)
						delete[] pSaxAttr;
					if(!bLastText)
					{
						m_ppsTextNow = &sValidXmlText;
						return true;
					}
					Error(SSaxErr_SYNTAX_ERR,"ȱ����������!");
					m_ppsTextNow = &sValidXmlText;
					return false;
				}
				//ȥ�ո�
				while((*sXmlText> 0 && isspace(*sXmlText)) && *sXmlText!='\0')
					sXmlText++;
				if(*sXmlText != '=')
				{
					//ȱ������=
					if(pSaxAttr != NULL)
						delete[] pSaxAttr;
					if(!bLastText)
					{
						m_ppsTextNow = &sValidXmlText;
						return true;
					}
					Error(SSaxErr_SYNTAX_ERR,"ȱ�����ԡ�=��!");
					m_ppsTextNow = &sValidXmlText;
					return false;
				}
				sXmlText++;
				//ȥ�ո�
				while((*sXmlText> 0 && isspace(*sXmlText)) && *sXmlText!='\0')
					sXmlText++;
				if(*sXmlText != '\'' && *sXmlText != '"')
				{
					//ȱ������ֵ������
					if(pSaxAttr != NULL)
						delete[] pSaxAttr;
					if(!bLastText)
					{
						m_ppsTextNow = &sValidXmlText;
						return true;
					}
					Error(SSaxErr_SYNTAX_ERR,"ȱ������ֵ������!");
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
					//����ֵû�н���
					Error(SSaxErr_SYNTAX_ERR,"����ֵ û�н���!");
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
				//ȥ�ո�
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
				//���¼��ڵ㼰�ڵ�ֵ�������رյ�ǰ�ڵ�
				sXmlText++;
				if(*sXmlText != '>')
				{
					//'/'��û�и�'>'
					if(pSaxAttr != NULL)
						delete[] pSaxAttr;
					if(!bLastText)
					{
						return true;
						m_ppsTextNow = &sValidXmlText;
					}
					Error(SSaxErr_SYNTAX_ERR,"'/'��û�и�'>' !");
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
					//����Ľ�β
					//�ڵ�δ�ر�
					if(pSaxAttr != NULL)
						delete[] pSaxAttr;
					if(!bLastText)
					{
						m_ppsTextNow = &sValidXmlText;
						return true;
					}
					Error(SSaxErr_SYNTAX_ERR,"�ڵ�δ�ر�!");
					m_ppsTextNow = &sValidXmlText;
					return false;
				}
				break;
			}
			if(*sXmlText != '>' && bLastText)
			{
				//�ڵ�δ�ر�
				if(pSaxAttr != NULL)
					delete[] pSaxAttr;
				if(!bLastText)
				{
					m_ppsTextNow = &sValidXmlText;
					return true;
				}
				Error(SSaxErr_SYNTAX_ERR,"�ڵ�δ�ر�!");
				m_ppsTextNow = &sValidXmlText;
				return false;
			}
			sXmlText ++;

			if(pThisChoice != NULL)
			{
				//ȡ���ܳ��ֵĽڵ�ֵ����
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
						//��Ϊ��ԭʼ�ı�
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
			//��ȡ�¼��ڵ�
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
		Error(SSaxErr_SYNTAX_ERR,"XMLδ��������!");
		m_ppsTextNow = &sValidXmlText;
		return false;
	}
	m_ppsTextNow = &sValidXmlText;
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ��λ��������Ϊ��һ��������XML������׼��
// ��    ��:  �ۿ���
// ����ʱ��:  2015-4-8 9:54
// ����˵��:  void
// �� �� ֵ:  void
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
// ��    ��:  ȡ��ǰ������Ȱٷֱ�
// ��    ��:  �ۿ���
// ����ʱ��:  2015-12-22 9:14
// ����˵��:  void
// �� �� ֵ:  int(0~100)
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

