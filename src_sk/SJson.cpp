/**
 *
 * �� �� �� : SJson.cpp
 * �������� : 2016-9-7 10:51
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : JSON��д���ʷ�װ��
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2016-9-7	�ۿ�������ļ�
 *
 **/

#include "SJson.h"


SJsonObject::SJsonObject()
{
	m_ValueType = VT_NULL;
	m_pChildObjects = m_pArrayObjects = NULL;
}

SJsonObject::~SJsonObject()
{
	if(m_pChildObjects != NULL)
	{
		delete m_pChildObjects;
		m_pChildObjects = NULL;
	}
	if(m_pArrayObjects != NULL)
	{
		delete m_pArrayObjects;
		m_pArrayObjects = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �������пո񡢻س�����Ч�ַ�������pText�ƶ�����һ����Ч�ַ���
// ��    ��:  �ۿ���
// ����ʱ��:  2016-9-7 14:14
// ����˵��:  #pText
// �� �� ֵ:  char, '\0'��ʾ�����ַ���ĩβ
//////////////////////////////////////////////////////////////////////////
char SJsonObject::SkipSpaceChar(char* &pText)
{
	while(*pText)
	{
		if(*pText == ' ' || *pText == '\r' || *pText == '\n' || *pText == '\t')
		{
			pText++;
			continue;
		}
		break;
	}
	return *pText;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ���ַ�����ǰλ�ö�ȡһ���ַ���(������˫���Ŷ��磬�޶���ʱ��,}]���ַ�����)������pText�ƶ����ַ���ĩβ
// ��    ��:  �ۿ���
// ����ʱ��:  2016-9-7 14:08
// ����˵��:  @pText
// �� �� ֵ:  SString���ַ������ݣ���������
//////////////////////////////////////////////////////////////////////////
SString SJsonObject::ReadString(char* &pText)
{
	SString sText;

	bool bSYH = *pText == '\"';
	if(bSYH)
		pText++;
	while(*pText != '\0')
	{
		if(bSYH)
		{
			if(*pText == '\"')
			{
				pText ++;
				break;
			}
		}
		else
		{
			//û������ֻ����������ֺ�����
			if(!(*pText == '-' || (*pText >= '0' && *pText <= '9') || (*pText >= 'a' && *pText <= 'z') || (*pText >= 'Z' && *pText <= 'Z') ))
				break;
		}
		if(*pText == '\\')
		{
			//����ת���
			pText ++;
			switch(*pText)
			{
			case '\0':
				return sText;
			case '\"':
				sText += '\"';
				break;
			case '\\':
				sText += '\\';
				break;
			case '/':
				sText += '/';
				break;
			case 'b':
				sText += '\b';
				break;
			case 'f':
				sText += '\f';
				break;
			case 'n':
				sText += '\n';
				break;
			case 'r':
				sText += '\r';
				break;
			case 't':
				sText += '\t';
				break;
			case 'u':
				{
					if(pText[1] != '\0' && pText[2] != '\0' && pText[3] != '\0' && pText[4] != '\0')
					{
						//is valid 4 hexadecimal digits
						char str[3];
						memset(str,0,sizeof(str));
						SString::StrToHex(pText,4,(unsigned char*)str);
						sText += str;
						pText += 4;
					}
					else
					{
						LOGERROR("invalid Json format in \\u 4 hexadecimal digits");
						while(*pText != '\0')
							pText ++;
						return sText;
					}
				}
				break;
			default:
				sText += *pText;
				break;
			}
			pText++;
		}
		else
		{
			sText += *pText;
			pText++;
		}
	}
	SkipSpaceChar(pText);
	return sText;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ����JSON�ı�����ǰ����
// ��    ��:  �ۿ���
// ����ʱ��:  2016-9-7 13:19
// ����˵��:  pTextΪJSONȫ�ı�,���ñ�����������ɺ�ָ�뽫��ָ���ı�ĩβ
// �� �� ֵ:  true��ʾ�����ɹ���false��ʾ����ʧ��
//////////////////////////////////////////////////////////////////////////
bool SJsonObject::ParseText(char* &pText)
{
	//�����ı�������ʱ����ɾ��ԭ�еĶ��󣬷�ֹ�����ظ�
	if(m_pChildObjects != NULL)
	{
		delete m_pChildObjects;
		m_pChildObjects = NULL;
	}
	if(m_pArrayObjects != NULL)
	{
		delete m_pArrayObjects;
		m_pArrayObjects = NULL;
	}
	//bool bHaveVarName = false;

	//seek to valid char
	if(SkipSpaceChar(pText) == '\0')
		return false;
		
	if(*pText == '\"')
	{
		m_sVarName = ReadString(pText);
		if(*pText == '\0')
			return false;
		if(m_sVarName.length() > 0)
		{
			//bHaveVarName = true;
			if(*pText != ':')
				return false;//':' not found
			pText++;
		}
		//seek to valid char
		if(SkipSpaceChar(pText) == '\0')
			return false;
	}
	if(*pText == '{')
	{
		//object
		next_object:
		m_ValueType = VT_OBJECT;
		pText++;
		SJsonObject *pChild = new SJsonObject();
		if(!pChild->ParseText(pText))
		{
			delete pChild;
			return false;
		}
		AddChildObject(pChild);
		if(*pText == ',')
			goto next_object;
		else if(*pText == '}')
			pText++;
		else
			return false;
	}
	else if(*pText == '[')
	{
		//array
		next_array:
		m_ValueType = VT_ARRAY;
		pText++;
		SJsonObject *pArray = new SJsonObject();
		if(!pArray->ParseText(pText))
		{
			delete pArray;
			return false;
		}
		AddArrayObject(pArray);
		if(*pText == ',')
			goto next_array;
		else if(*pText == ']')
			pText++;
		else
			return false;
	}
	else
	{
		//other value type
		char pFirstCh = *pText;
		m_sValue = ReadString(pText);
		if(pFirstCh == '\"')
		{
			m_ValueType = VT_STRING;
		}
		else
		{
			//����
			char ch = m_sValue.Left(1).toLower().at(0);
			if(ch == 't' && m_sValue.toLower() == "true")
				m_ValueType = VT_BOOL;
			else if(ch == 'f' && m_sValue.toLower() == "false")
				m_ValueType = VT_BOOL;
			else if(ch == 'n' && m_sValue.toLower() == "null")
				m_ValueType = VT_NULL;
			else if(ch == '-' || (ch >= '0' && ch <= '9'))
				m_ValueType = VT_NUMBER;
			else
				m_ValueType = VT_STRING;
		}
	}

	SkipSpaceChar(pText);
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡָ�����Ƶ��ӱ�������
// ��    ��:  �ۿ���
// ����ʱ��:  2016-9-7 11:19
// ����˵��:  @sValueName��ʾ��������
// �� �� ֵ:  SJsonObject*,NULL��ʾָ������������
//////////////////////////////////////////////////////////////////////////
SJsonObject* SJsonObject::operator[](char* sVarName)
{
	return GetChildObject(sVarName);
}

SJsonObject* SJsonObject::operator[](SString &sVarName)
{
	return GetChildObject(sVarName.data());
}

SJsonObject* SJsonObject::GetChildObject(char* sVarName)
{
	if(m_pChildObjects == NULL)
		return NULL;
	unsigned long pos;
	SJsonObject *p = m_pChildObjects->FetchFirst(pos);
	while(p)
	{
		if(SString::equals(p->m_sVarName.data(),sVarName))
			return p;
		p = m_pChildObjects->FetchNext(pos);
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡָ����ŵ������ӱ���
// ��    ��:  �ۿ���
// ����ʱ��:  2016-9-7 11:16
// ����˵��:  @idxΪ�����ӱ�������ţ���0��ʼ���
// �� �� ֵ:  SJsonObject*
//////////////////////////////////////////////////////////////////////////
SJsonObject* SJsonObject::operator[](int idx)
{
	if(m_pArrayObjects == NULL)
		return NULL;
	return m_pArrayObjects->at(idx);
}

SJsonObject* SJsonObject::GetArrayObject(int idx)
{
	if(m_pArrayObjects == NULL)
		return NULL;
	return m_pArrayObjects->at(idx);
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ����ӱ���
// ��    ��:  �ۿ���
// ����ʱ��:  2016-9-7 13:06
// ����˵��:  @pObj
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SJsonObject::AddChildObject(SJsonObject *pObj)
{
	if(m_pChildObjects == NULL)
	{
		m_pChildObjects = new SPtrList<SJsonObject>();
		m_pChildObjects->setAutoDelete(true);
	}
	m_pChildObjects->append(pObj);
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �������Ԫ�ص���ǰ����
// ��    ��:  �ۿ���
// ����ʱ��:  2016-9-7 13:07
// ����˵��:  @pObj
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SJsonObject::AddArrayObject(SJsonObject *pObj)
{
	if(m_pArrayObjects == NULL)
	{
		m_pArrayObjects = new SPtrList<SJsonObject>();
		m_pArrayObjects->setAutoDelete(true);
	}
	m_pArrayObjects->append(pObj);
}


//////////////////////////////////////////////////////////////////////////
// ��    ��:  ɾ���ӱ��������������ӱ���������Ԫ��
// ��    ��:  �ۿ���
// ����ʱ��:  2016-9-7 13:12
// ����˵��:  @pObj
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SJsonObject::DeleteChildObject(SJsonObject *pObj)
{
	if(m_pChildObjects == NULL)
		return;
	m_pChildObjects->remove(pObj);
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ɾ����������Ԫ�ؼ����������ӱ���������Ԫ��
// ��    ��:  �ۿ���
// ����ʱ��:  2016-9-7 13:13
// ����˵��:  @pObj
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SJsonObject::DeleteArrayObject(SJsonObject *pObj)
{
	if(m_pArrayObjects == NULL)
		return;
	m_pArrayObjects->remove(pObj);
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ����JSON��������õ��ַ�������
// ��    ��:  �ۿ���
// ����ʱ��:  2016-9-7 15:47
// ����˵��:  @sText���÷��ص��ַ���
//         :  @pObjΪ�ȱ���Ķ���ָ��
//         :  @level��ʾ���������ⲿ����ʱ(0��ʾ��������,-1��ʾ�����������ı�)
// �� �� ֵ:  true��ʾ�ɹ���false��ʾʧ��
//////////////////////////////////////////////////////////////////////////
bool SJsonObject::SaveToString(SString &sText,SJsonObject *pObj,int level)
{
	if(pObj->GetValueType() == SJsonObject::VT_OBJECT)
	{
		if(level >= 0)
			sText += SString::toFillString("",level*4,' ');
		if(pObj->GetVarName().length() > 0)
			sText += SString::toFormat("\"%s\":",pObj->GetVarName().data());
		if(level >= 0)
			sText += "{\r\n";
		else
			sText += "{";
		if(pObj->GetChildObjectsPtr() != NULL)
		{
			unsigned long pos;
			SJsonObject *p = pObj->GetChildObjectsPtr()->FetchFirst(pos);
			while(p)
			{
				SaveToString(sText,p,level<0?level:(level+1));
				p = pObj->GetChildObjectsPtr()->FetchNext(pos);
				if(level >= 0)
				{
					if(p != NULL)
						sText += ",\r\n";
					else
						sText += "\r\n";
				}
				else
				{
					if(p != NULL)
						sText += ",";
				}
			}
		}
		sText += SString::toFillString("}",level>=0?(level*4):0,' ');
	}
	else if(pObj->GetValueType() == SJsonObject::VT_ARRAY)
	{
		if(level >= 0)
			sText += SString::toFillString("",level*4,' ');
		if(pObj->GetVarName().length() > 0)
			sText += SString::toFormat("\"%s\":",pObj->GetVarName().data());
		if(level >= 0)
			sText += "[\r\n";
		else
			sText += "[";
		if(pObj->GetArrayObjectsPtr() != NULL)
		{
			unsigned long pos;
			SJsonObject *p = pObj->GetArrayObjectsPtr()->FetchFirst(pos);
			while(p)
			{
				SaveToString(sText,p,level<0?level:(level+1));
				p = pObj->GetArrayObjectsPtr()->FetchNext(pos);
				if(level >= 0)
				{
					if(p != NULL)
						sText += ",\r\n";
					else
						sText += "\r\n";
				}
				else
				{
					if(p != NULL)
						sText += ",";
				}
			}
		}
		sText += SString::toFillString("]",level>=0?(level*4):0,' ');
	}
	else
	{
		if(pObj->GetValueType() == VT_STRING)
			sText += SString::toFormat("%s\"%s\":\"%s\"",	SString::toFillString("",level>=0?(level*4):0,' ').data(),pObj->GetVarName().data(),pObj->GetValue().data());
		else
			sText += SString::toFormat("%s\"%s\":%s",	SString::toFillString("",level>=0?(level*4):0,' ').data(),pObj->GetVarName().data(),pObj->GetValue().data());
	}
	return true;
}
