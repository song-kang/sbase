/**
 *
 * 文 件 名 : SJson.cpp
 * 创建日期 : 2016-9-7 10:51
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : JSON读写访问封装类
 * 修改记录 : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2016-9-7	邵凯田　创建文件
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
// 描    述:  跳过所有空格、回车等无效字符，并将pText移动到第一个有效字符处
// 作    者:  邵凯田
// 创建时间:  2016-9-7 14:14
// 参数说明:  #pText
// 返 回 值:  char, '\0'表示到了字符串末尾
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
// 描    述:  从字符串当前位置读取一个字符串(允许以双引号定界，无定界时以,}]三字符结束)，并将pText移动到字符串末尾
// 作    者:  邵凯田
// 创建时间:  2016-9-7 14:08
// 参数说明:  @pText
// 返 回 值:  SString，字符串内容，不含引号
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
			//没有引用只允许出现数字和引号
			if(!(*pText == '-' || (*pText >= '0' && *pText <= '9') || (*pText >= 'a' && *pText <= 'z') || (*pText >= 'Z' && *pText <= 'Z') ))
				break;
		}
		if(*pText == '\\')
		{
			//处理转义符
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
// 描    述:  解析JSON文本到当前对象
// 作    者:  邵凯田
// 创建时间:  2016-9-7 13:19
// 参数说明:  pText为JSON全文本,引用变量，解析完成后指针将被指向文本末尾
// 返 回 值:  true表示解析成功，false表示解析失败
//////////////////////////////////////////////////////////////////////////
bool SJsonObject::ParseText(char* &pText)
{
	//解析文本到对象时，先删除原有的对象，防止内容重复
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
			//不是
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
// 描    述:  取指定名称的子变量对象
// 作    者:  邵凯田
// 创建时间:  2016-9-7 11:19
// 参数说明:  @sValueName表示变量名称
// 返 回 值:  SJsonObject*,NULL表示指定变量不存在
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
// 描    述:  取指定序号的数组子变量
// 作    者:  邵凯田
// 创建时间:  2016-9-7 11:16
// 参数说明:  @idx为数组子变量的序号，从0开始编号
// 返 回 值:  SJsonObject*
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
// 描    述:  添加子变量
// 作    者:  邵凯田
// 创建时间:  2016-9-7 13:06
// 参数说明:  @pObj
// 返 回 值:  void
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
// 描    述:  添加数组元素到当前变量
// 作    者:  邵凯田
// 创建时间:  2016-9-7 13:07
// 参数说明:  @pObj
// 返 回 值:  void
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
// 描    述:  删除子变量及其下所有子变量和数组元素
// 作    者:  邵凯田
// 创建时间:  2016-9-7 13:12
// 参数说明:  @pObj
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SJsonObject::DeleteChildObject(SJsonObject *pObj)
{
	if(m_pChildObjects == NULL)
		return;
	m_pChildObjects->remove(pObj);
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  删除变量数据元素及其下所有子变量和数组元素
// 作    者:  邵凯田
// 创建时间:  2016-9-7 13:13
// 参数说明:  @pObj
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SJsonObject::DeleteArrayObject(SJsonObject *pObj)
{
	if(m_pArrayObjects == NULL)
		return;
	m_pArrayObjects->remove(pObj);
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  保存JSON对象的引用的字符串对象
// 作    者:  邵凯田
// 创建时间:  2016-9-7 15:47
// 参数说明:  @sText引用返回的字符串
//         :  @pObj为等保存的对象指针
//         :  @level表示缩进级别，外部调用时(0表示正常缩进,-1表示不缩进紧凑文本)
// 返 回 值:  true表示成功，false表示失败
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
