/**
 *
 * �� �� �� : SBaseConfig.cpp
 * �������� : 2008-03-24
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: 2011/08/31 08:43:33 $
 * ��ǰ�汾 : $Revision: 1.1 $
 * �������� : ��SBaseConfig����
 * �޸ļ�¼ : 
 *            $Log: SBaseConfig.cpp,v $
 *            Revision 1.1  2011/08/31 08:43:33  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2011/03/21 05:14:56  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2011/02/25 03:12:05  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2010/08/09 03:04:43  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2010/01/22 01:24:25  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2009/09/15 02:14:06  shaokt
 *            ͨѸ��¼����װ����Ŀ�״��ύ
 *
 *            Revision 1.1  2008/09/01 11:38:28  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/01 08:58:00  shaokt
 *            *** empty log message ***
 *
 *
 **/


#include "SBaseConfig.h"

//////////////////////////////////////////////////////////////////////////
// ��    ��:  SBaseConfig
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 14:00
// ��    ��:  ���νṹ��������
//////////////////////////////////////////////////////////////////////////
SBaseConfig::SBaseConfig()
{
	SKTOBJECT("SBaseConfig");
	m_Attribute.setAutoDelete(true);
	m_ChildConfig.setAutoDelete(true);
	m_sNodeName = "";
	m_pParent = NULL;
}
SBaseConfig::~SBaseConfig()
{
	clear();
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  �����ǰ�ڵ�������ӽڵ㼰����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 14:02
// ����˵��:  void
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SBaseConfig::clear()
{
	m_Attribute.clear();
	m_ChildConfig.clear();
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ��ǰ�ڵ��·����һֱ׷˷�������ڵ�
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 14:03
// ����˵��:  void
// �� �� ֵ:  ����ʹ��"."�ָ���·���������ڵ�����Ϊm_sNodeName��Ӧ����
//////////////////////////////////////////////////////////////////////////
SString SBaseConfig::GetPath()
{
	SString sPath=m_sNodeName;
	SBaseConfig *p = this->m_pParent;
	while(p)
	{
		sPath = p->m_sNodeName+"."+sPath;
		p = p->m_pParent;
	}
	return sPath;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  �����¼��ڵ�
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 14:04
// ����˵��:  @sNodeName�ӽڵ������
// �� �� ֵ:  ���������ӵ��ӽڵ�ʵ��ָ��
//////////////////////////////////////////////////////////////////////////
SBaseConfig * SBaseConfig::AddChildNode(SString sNodeName/*=""*/, const char* pStrAttributs/*=NULL*/, const char* pStrNodeValue/*=NULL*/)
{
	SBaseConfig *pNew = new SBaseConfig();
	pNew->m_sNodeName = sNodeName;
	m_ChildConfig.append(pNew);
	pNew->m_pParent = this;
	if(pStrAttributs != NULL)
	{
		SString sAttr = pStrAttributs;
		int i,cnt=SString::GetAttributeCount(sAttr,";");
		SString sLine,name,value;
		for(i=1;i<=cnt;i++)
		{
			sLine = SString::GetIdAttribute(i,sAttr,";");
			name = SString::GetIdAttribute(1,sLine,"=");
			value = SString::GetIdAttribute(2,sLine,"=");
			pNew->SetAttributeEx(name.data(),value.data());
		}
	}
	if(pStrNodeValue != NULL)
		pNew->SetNodeValueEx(pStrNodeValue);
	return pNew;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  �����µĽڵ㣬�½ڵ������new�����ģ�������ɵ�ǰʵ�������ͷ�
// ��    ��:  �ۿ���
// ����ʱ��:  2013:12:11 10:28
// ����˵��:  @pNew
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SBaseConfig::AddChildNode(SBaseConfig *pNew)
{
	m_ChildConfig.append(pNew);
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  �ӵ�ǰʵ���Ƴ�ָ���Ľڵ㣬�������ͷű��Ƴ��Ľڵ㣬������Ӧ�����ͷ�
// ��    ��:  �ۿ���
// ����ʱ��:  2013:12:11 10:36
// ����˵��:  @pSub
// �� �� ֵ:  true/false
//////////////////////////////////////////////////////////////////////////
bool SBaseConfig::RemoveChildNode(SBaseConfig *pSub)
{
	m_ChildConfig.setAutoDelete(false);
	bool b = m_ChildConfig.remove(pSub);
	m_ChildConfig.setAutoDelete(true);
	return b;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  �жϵ�ǰ�ڵ㼰���ӽڵ��Ƿ���ȫ��ͬ
// ��    ��:  �ۿ���
// ����ʱ��:  2013:12:12 16:13
// ����˵��:  @pSubΪ��һ���ڵ�
// �� �� ֵ:  true��ʾ��ͬ��false��ʾ��ͬ
//////////////////////////////////////////////////////////////////////////
bool SBaseConfig::IsNodeSame(SBaseConfig *pNode)
{
	if(this->GetChildCount() != pNode->GetChildCount())
		return false;
	if(this->GetAttributeCount() != pNode->GetAttributeCount())
		return false;
	if(this->GetNodeName() != pNode->GetNodeName())
		return false;
	if(this->GetNodeValue().trim() != pNode->GetNodeValue().trim())
		return false;
	SString sAttrName;
	int i,cnt = GetAttributeCount();
	//�жϸ�������
	for(i=0;i<cnt;i++)
	{
		sAttrName = this->GetAttributeName(i);
		if(GetAttribute(sAttrName) != pNode->GetAttribute(sAttrName))
			return false;
	}
	for(i=0;i<cnt;i++)
	{
		sAttrName = pNode->GetAttributeName(i);
		if(GetAttribute(sAttrName) != pNode->GetAttribute(sAttrName))
			return false;
	}
	//�жϸ��ӽڵ�
	cnt = GetChildCount();
	for(i=0;i<cnt;i++)
	{
		if(!GetChildPtr()->at(i)->IsNodeSame(pNode->GetChildPtr()->at(i)))
			return false;
	}
	return true;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  �滻��ǰ�ڵ㼰���ӽڵ������ֵ���������oldVal���滻ΪnewVal
// ��    ��:  �ۿ���
// ����ʱ��:  2013:12:12 16:18
// ����˵��:  
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SBaseConfig::ReplaceNodeAttrVal(SString &oldVal,SString  &newVal)
{
	int i,cnt = GetAttributeCount();
	SString sAttrName;
	//�жϸ�������
	for(i=0;i<cnt;i++)
	{
		sAttrName = this->GetAttributeName(i);
		if(GetAttribute(sAttrName) == oldVal)
			SetAttribute(sAttrName,newVal);
	}
	cnt = GetChildCount();
	for(i=0;i<cnt;i++)
	{
		GetChildPtr()->at(i)->ReplaceNodeAttrVal(oldVal,newVal);
	}
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  �����¼��ڵ�
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 14:05
// ����˵��:  @sNodeName�ӽڵ������
// �� �� ֵ:  ���������ӵ��ӽڵ�ʵ��ָ��
//////////////////////////////////////////////////////////////////////////
SBaseConfig * SBaseConfig::AddChildNodeEx(const char* sNodeName/*=""*/)
{
	SBaseConfig *pNew = new SBaseConfig();
	pNew->m_sNodeName = sNodeName;
	m_ChildConfig.append(pNew);
	pNew->m_pParent = this;
	return pNew;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ���ýڵ�����,�ַ���
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 14:05
// ����˵��:  @sAttrName��������
//            @sAttrValue����ֵ
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SBaseConfig::SetAttribute(SString sAttrName,SString sAttrValue)
{
	//�Ȳ����Ƿ����
	int i,cnt=m_Attribute.count();
	for(i=0;i<cnt;i++)
	{
		if(m_Attribute[i]->m_sName == sAttrName)
		{
			m_Attribute[i]->m_sValue = sAttrValue;
			return;
		}
	}
	SBaseConfig::SConfigAttribute *pNew = new SConfigAttribute();
	pNew->m_sName = sAttrName;
	pNew->m_sValue = sAttrValue;
	m_Attribute.append(pNew);	
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ���ýڵ����ԣ�����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 14:05
// ����˵��:  @sAttrName��������
//            @iAttrValue����ֵ
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SBaseConfig::SetAttributeI(SString sAttrName,int iAttrValue)
{
	SetAttribute(sAttrName,SString::toString(iAttrValue));
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ���ýڵ����ԣ�������
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 14:05
// ����˵��:  @sAttrName��������
//            @fAttrValue����ֵ
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SBaseConfig::SetAttributeF(SString sAttrName,float fAttrValue)
{
	SetAttribute(sAttrName,SString::toString(fAttrValue));
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ���ýڵ����ԣ�voidָ����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 14:05
// ����˵��:  @sAttrName��������
//            @sAttrValue����ֵ
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SBaseConfig::SetAttributeP(SString sAttrName,void* fAttrValue)
{
	SetAttribute(sAttrName,SString::toString((long)fAttrValue));
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ���ýڵ����ԣ��ַ��ͣ���Ч����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 14:05
// ����˵��:  @sAttrName��������
//            @sAttrValue����ֵ
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SBaseConfig::SetAttributeEx(const char* sAttrName,const char* sAttrValue)
{
	//�Ȳ����Ƿ����
	int i,cnt=m_Attribute.count();
	for(i=0;i<cnt;i++)
	{
		if(strcmp(m_Attribute[i]->m_sName.data(), sAttrName) == 0)
		{
			m_Attribute[i]->m_sValue = sAttrValue;
			return;
		}
	}
	SBaseConfig::SConfigAttribute *pNew = new SConfigAttribute();
	pNew->m_sName = sAttrName;
	pNew->m_sValue = sAttrValue;
	m_Attribute.append(pNew);	
}


////////////////////////////////////////////////////////////////////////
// ��    ��:  ���ýڵ�����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 14:09
// ����˵��:  @s��ʾ�ڵ�����
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SBaseConfig::SetNodeName(SString s)
{
	m_sNodeName = s;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ���ýڵ�����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 14:09
// ����˵��:  @s��ʾ�ڵ�����
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SBaseConfig::SetNodeNameEx(const char* s)
{
	m_sNodeName = s;	
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ���ýڵ�ֵ
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 14:10
// ����˵��:  @s��ʾ�ڵ�ֵ
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SBaseConfig::SetNodeValue(SString s)
{
	m_sNodeValue = s;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ���ýڵ�ֵ
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 14:11
// ����˵��:  @s��ʾ�ڵ�ֵ
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SBaseConfig::SetNodeValueEx(const char* s)
{
	m_sNodeValue = s;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  �����Ƿ�ǰ�ڵ����ָ��������
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 14:13
// ����˵��:  @sAttrName��ʾ������
// �� �� ֵ:  true��ʾ���Դ��ڣ�false��ʾ���Բ�����
//////////////////////////////////////////////////////////////////////////
bool SBaseConfig::IsAttribute(SString sAttrName)
{
	int i,cnt=m_Attribute.count();
	for(i=0;i<cnt;i++)
	{
		if(m_Attribute[i]->m_sName == sAttrName)
			return true;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ��ǰ�ڵ��µ�ָ��������
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 14:14
// ����˵��:  @sAttrname��ʾ������
// �� �� ֵ:  ����ֵ��ȱ�ٴ�����ʱ���ؿ��ַ���
//////////////////////////////////////////////////////////////////////////
SString SBaseConfig::GetAttribute(SString sAttrName)
{
	int i,cnt=m_Attribute.count();
	for(i=0;i<cnt;i++)
	{
		if(m_Attribute[i]->m_sName == sAttrName)
			return m_Attribute[i]->m_sValue;
	}
	return "";
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ��ǰ�ڵ��µ�ָ������������
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 14:15
// ����˵��:  @sAttrName��ʾ������
// �� �� ֵ:  ����ֵ��ȱ��ʱ����0
//////////////////////////////////////////////////////////////////////////
int SBaseConfig::GetAttributeI(SString sAttrName)
{
	return GetAttribute(sAttrName).toInt();
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ��ǰ�ڵ��µ�ָ���ĸ���������
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 14:15
// ����˵��:  @sAttrName��ʾ������
// �� �� ֵ:  ����ֵ��ȱ��ʱ����0
//////////////////////////////////////////////////////////////////////////
float SBaseConfig::GetAttributeF(SString sAttrName)
{
	return GetAttribute(sAttrName).toFloat();
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ���÷�ʽȡ���ԣ��ַ���
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 14:16
// ����˵��:  @sAttrName��ʾ������
//            @sAttrValue��ʾ����ֵ
// �� �� ֵ:  true��ʾ���Դ��ڣ���ֵ�Ѿ�����sAttrValue��false��ʾ���Բ�����
//////////////////////////////////////////////////////////////////////////
bool SBaseConfig::GetAttribute(SString sAttrName,SString &sAttrValue)
{
	int i,cnt=m_Attribute.count();
	for(i=0;i<cnt;i++)
	{
		if(m_Attribute[i]->m_sName == sAttrName)
		{
			sAttrValue = m_Attribute[i]->m_sValue;
			return true;
		}
	}
	return false;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ���÷�ʽȡ���ԣ�����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 14:16
// ����˵��:  @sAttrName��ʾ������
//            @iAttrValue��ʾ����ֵ
// �� �� ֵ:  true��ʾ���Դ��ڣ���ֵ�Ѿ�����iAttrValue��false��ʾ���Բ�����
//////////////////////////////////////////////////////////////////////////
bool SBaseConfig::GetAttribute(SString sAttrName,int &iAttrValue)
{
	SString val;
	if(!GetAttribute(sAttrName,val))
		return false;
	iAttrValue = val.toInt();
	return true;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ���÷�ʽȡ���ԣ�������
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 14:16
// ����˵��:  @sAttrName��ʾ������
//            @fAttrValue��ʾ����ֵ
// �� �� ֵ:  true��ʾ���Դ��ڣ���ֵ�Ѿ�����fAttrValue��false��ʾ���Բ�����
//////////////////////////////////////////////////////////////////////////
bool SBaseConfig::GetAttribute(SString sAttrName,float &fAttrValue)
{
	SString val;
	if(!GetAttribute(sAttrName,val))
		return false;
	fAttrValue = val.toFloat();
	return true;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ��ǰ�ڵ���ָ��·���µ�ĳ������
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 14:18
// ����˵��:  @sNodePath��ʾ�ڵ�·�����磺body.GROUPTITLE(0).GROUP<character='PARAM'>
//            @sAttrName��ʾ������
// �� �� ֵ:  ����ֵ
//////////////////////////////////////////////////////////////////////////
SString SBaseConfig::GetAttribute(SString sNodePath,const char* sAttrName)
{
	SBaseConfig *pConfig = this->SearchChild(sNodePath);
	if(pConfig == NULL)
		return "";
	return pConfig->GetAttribute(sAttrName);
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ��ǰ�ڵ���ָ��·���µĽڵ�����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 14:19
// ����˵��:  @sNodePath��ʾ�ڵ�·�����磺body.GROUPTITLE(0).GROUP<character='PARAM'>
// �� �� ֵ:  �ڵ�����
//////////////////////////////////////////////////////////////////////////
SString SBaseConfig::GetNodeValue(SString sNodePath)
{
	SBaseConfig *pConfig = this->SearchChild(sNodePath);
	if(pConfig == NULL)
		return "";
	return pConfig->GetNodeValue();
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ��ǰ�ڵ����������
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 14:23
// ����˵��:  void
// �� �� ֵ:  ��������
//////////////////////////////////////////////////////////////////////////
int SBaseConfig::GetAttributeCount()
{
	return m_Attribute.count();
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ��ǰ�ڵ��ָ����index�����Ե�������
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 14:23
// ����˵��:  @indexΪ������ţ���0��ʼ���
// �� �� ֵ:  ������
//////////////////////////////////////////////////////////////////////////
SString SBaseConfig::GetAttributeName(int index)
{
	if(index >= m_Attribute.count())
	{
		return "";
	}
	return m_Attribute[index]->m_sName;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ�¼��ڵ�����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 14:25
// ����˵��:  @sNodeName��ʾ��ͳ���¼��ڵ�����ƣ��ձ�ʾȫ���ڵ�
//            @sCondition��ʾͳ���������ո�ָ�����������磺name1='val1' name2='val2'
// �� �� ֵ:  ��������������
//////////////////////////////////////////////////////////////////////////
int SBaseConfig::GetChildCount(SString sNodeName/*=""*/,SString sCondition/*=""*/)
{
	if(sNodeName == "" && sCondition == "")
		return m_ChildConfig.count();
	
	int i,cnt,size=0;
	cnt = m_ChildConfig.count();
	for(i=0;i<cnt;i++)
	{
		if(sNodeName.length()>0)
		{
			if(sNodeName != m_ChildConfig[i]->GetNodeName())
				continue;
			if(sCondition.length()>0 && !m_ChildConfig[i]->CheckNodeAttribute(sCondition))
				continue;
		}
		size ++;
	}
	return size;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ��index���¼��ڵ�,index��0��ʼ
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 14:30
// ����˵��:  @index��ʾ���,index��0��ʼ
//            @sNodeName��ʾ��ͳ���¼��ڵ�����ƣ��ձ�ʾȫ���ڵ�
//            @sCondition��ʾͳ���������ո�ָ�����������磺name1='val1' name2='val2'
// �� �� ֵ:  ָ�����ӽڵ�ʵ��ָ�룬������ʱ����NULL
//////////////////////////////////////////////////////////////////////////
SBaseConfig* SBaseConfig::GetChildNode(int index,SString sNodeName/*=""*/,SString sCondition/*=""*/)
{
	if(sNodeName == "" && sCondition == "")
	{
		if(index >= m_ChildConfig.count())
			return NULL;
		return m_ChildConfig[index];
	}

	int i,size=0,cnt=m_ChildConfig.count();
	for(i=0;i<cnt;i++)
	{
		if(sNodeName != m_ChildConfig[i]->GetNodeName())
			continue;
		if(!m_ChildConfig[i]->CheckNodeAttribute(sCondition))
			continue;
		if(size >= index)
			return m_ChildConfig[i];
		size ++;
	}
	return NULL;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ������������ȡ�¼��ڵ�
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 14:31
// ����˵��:  @sNodeName��ʾ�ڵ�����
//            @sAttrName1��ʾ����������1���ձ�ʾ��ʹ�ô�����
//            @sAttrVal1��ʾ��������ֵ1
//            �����ַ�ͬ��
// �� �� ֵ:  ָ�����ӽڵ�ʵ��ָ�룬������ʱ����NULL
//////////////////////////////////////////////////////////////////////////
SBaseConfig* SBaseConfig::GetChildNode(SString sNodeName,
				SString sAttrName1,SString sAttrVal1,
				SString sAttrName2,SString sAttrVal2,
				SString sAttrName3,SString sAttrVal3,
				SString sAttrName4,SString sAttrVal4,
				SString sAttrName5,SString sAttrVal5,
				SString sAttrName6,SString sAttrVal6)
{
	int i,cnt=m_ChildConfig.count();
	for(i=0;i<cnt;i++)
	{
		SBaseConfig* p = m_ChildConfig[i];
		if(p->m_sNodeName != sNodeName)
			continue;
		if( (sAttrName1!="" && p->GetAttribute(sAttrName1) !=sAttrVal1) ||
			(sAttrName2!="" && p->GetAttribute(sAttrName2) !=sAttrVal2) ||
			(sAttrName3!="" && p->GetAttribute(sAttrName3) !=sAttrVal3) ||
			(sAttrName4!="" && p->GetAttribute(sAttrName4) !=sAttrVal4) ||
			(sAttrName5!="" && p->GetAttribute(sAttrName5) !=sAttrVal5) ||
			(sAttrName6!="" && p->GetAttribute(sAttrName6) !=sAttrVal6) )
			continue;
		return p;
	}
	return NULL;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��鵱ǰ�ڵ��Ƿ����ָ��������
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 14:33
// ����˵��:  @sAttr��ʾ�ڵ����ԵĹ����ַ������ո�ָ�����������磺name1='val1' name2='val2'
// �� �� ֵ:  true��ʾ�ڵ�����ָ������
//////////////////////////////////////////////////////////////////////////
bool SBaseConfig::CheckNodeAttribute(SString sAttr)
{
	int iCnt = sAttr.length();//GetAttributeCount();
	SString sAttrName="",sAttrVal,sTemp;
	int flag = 0;
	char sep='\'',ch;
	for(int i = 0; i < iCnt; i++)
	{
		ch = sAttr.at(i);
		if(flag == 0)//ƴ���ƶ�
		{
			if(ch == '=')//����ƴ����
			{
				sAttrVal = "";
				flag = 1;
			}
			else if(ch == ' ')
				continue;
			else
				sAttrName += ch;
		}
		else if(flag == 1)//�������ݶεĵ�һ���ַ�
		{
			if(ch == '\'' || ch == '\"')
				sep = ch;//�зָ���
			else
				sep = ' ';//�޷ָ���
			flag = 2;
		}
		else
		{
			if(ch == sep)//����
			{
				//begin 
				if(sAttrVal.length() > 0)//���û��ƥ��ֵ����ʹ�ô�����
				{
					sAttrVal = sAttrVal.lower();
					SString sTemp;
					sTemp = GetAttribute(sAttrName);
					sTemp = sTemp.lower();
					if(sTemp != sAttrVal)
						return false;
				}
				//end 
				sAttrName="";
				flag = 0;
			}
			else
				sAttrVal += ch;
		}
	}
	return true;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  �����ض��Ĳ���ַ���ȡ�ڵ�ָ��
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 14:34
// ����˵��:  @sGroup��ʾ�ڵ�·�����磺body.GROUPTITLE(0).GROUP<character='PARAM'>
// �� �� ֵ:  ָ�����ӽڵ�ʵ��ָ�룬������ʱ����NULL
//////////////////////////////////////////////////////////////////////////
SBaseConfig* SBaseConfig::SearchChild(SString sGroup)
{
	int i,id,iCnt,iIndex,iPos1,iPos2;
	SString sNode,sName,sAttr,sTemp;
	SBaseConfig* pParent=NULL, *pNode = NULL;
	unsigned long pos;

	pParent = this;
	pNode = pParent->m_ChildConfig.at(0);

	iCnt = SString::GetAttributeCount(sGroup,".");	
	
	for(i = 1; i <= iCnt; i ++)
	{
		if(pNode == NULL)
			return NULL;

		sNode = SString::GetIdAttribute(i,sGroup,".");
		//check node name, whether it have index number,'(X)'
		if(sNode.find("(") > 0 && sNode.find(")",sNode.find("(")+1) > 0)
		{
			iPos1 = sNode.find("(");
			iPos2 = sNode.find(")",iPos1 + 1);
			sTemp = sNode.mid(iPos1+1, iPos2 - iPos1 - 1);
			iIndex = sTemp.toInt();
			sNode = sNode.left(iPos1);
		}
		else
			iIndex = 1;
		if(sNode.find("<") > 0 && sNode.find(">",sNode.find("<")+1) > 0)
		{
			iPos1 = sNode.find("<");
			iPos2 = sNode.find(">",iPos1 + 1);
			sAttr = sNode.mid(iPos1+1, iPos2 - iPos1 - 1);
			sNode = sNode.left(iPos1);
		}
		else
			sAttr = "";
		sNode = sNode.toLower();
		id = 0;
#if 0 //���㷨��Ч�ʲ���
		pi=0;
		while(1)
		{
			if(pNode == NULL)
			{
				return NULL;
			}
			sName = "";sName += pNode->GetNodeName();
			sName = sName.toLower();
			if(sNode == sName)
			{
				id ++;
				if(sAttr.length() == 0)
				{
					if(iIndex == id)
						//�ҵ���
						break;
				}
				else//����������
				{
					if(pNode->CheckNodeAttribute(sAttr))
						break;//�ҵ���
				}
			}

			//goto next
			pNode = pParent->m_ChildConfig.at(++pi);
		}

#else	//MEMO: ʹ��Fetch��ʽ [2013:9:18 10:51 �ۿ���]

		pNode = pParent->m_ChildConfig.FetchFirst(pos);
		while(1)
		{
			if(pNode == NULL)
			{
				return NULL;
			}
			sName = "";sName += pNode->GetNodeName();
			sName = sName.toLower();
			if(sNode == sName)
			{
				id ++;
				if(sAttr.length() == 0)
				{
					if(iIndex == id)
						//�ҵ���
						break;
				}
				else//����������
				{
					if(pNode->CheckNodeAttribute(sAttr))
						break;//�ҵ���
				}
			}

			//goto next
			pNode = pParent->m_ChildConfig.FetchNext(pos);
		}

#endif
		//�Ѿ��ҵ���ǰ�Ľڵ�pNode
		if(i == iCnt)
			return pNode;

		//goto son level
		pParent = pNode;
		pNode = pNode->m_ChildConfig.at(0);
	}

	return pNode;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ����ָ���ڵ���ַ�������
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 14:35
// ����˵��:  @sGroup��ʾ�ڵ�·�����磺body.GROUPTITLE(0).GROUP<character='PARAM'>
//            @sAttr��ʾ��������
// �� �� ֵ:  ����ֵ
//////////////////////////////////////////////////////////////////////////
SString SBaseConfig::SearchNodeAttribute(SString sGroup,SString sAttr)
{
	SBaseConfig *pCfg = SearchChild(sGroup);
	if(pCfg == NULL)
		return "";
	return pCfg->GetAttribute(sAttr);
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ����ָ���ڵ����������
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 14:35
// ����˵��:  @sGroup��ʾ�ڵ�·�����磺body.GROUPTITLE(0).GROUP<character='PARAM'>
//            @sAttr��ʾ��������
// �� �� ֵ:  ����ֵ
//////////////////////////////////////////////////////////////////////////
int SBaseConfig::SearchNodeAttributeI(SString sGroup,SString sAttr)
{
	return SearchNodeAttribute(sGroup,sAttr).toInt();
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ����ָ���ڵ�ĸ�������
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 14:35
// ����˵��:  @sGroup��ʾ�ڵ�·�����磺body.GROUPTITLE(0).GROUP<character='PARAM'>
//            @sAttr��ʾ��������
// �� �� ֵ:  ����ֵ
//////////////////////////////////////////////////////////////////////////
float SBaseConfig::SearchNodeAttributeF(SString sGroup,SString sAttr)
{
	return SearchNodeAttribute(sGroup,sAttr).toFloat();
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ����ָ���ڵ��ֵ
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 14:36
// ����˵��:  @sGroup��ʾ�ڵ�·�����磺body.GROUPTITLE(0).GROUP<character='PARAM'>
// �� �� ֵ:  ƥ��ڵ�Ľڵ�ֵ����ƥ��ڵ�ʱ���ؿ��ַ���
//////////////////////////////////////////////////////////////////////////
SString SBaseConfig::SearchNodeValue(SString sGroup)
{
	SBaseConfig *pCfg = SearchChild(sGroup);
	if(pCfg == NULL)
		return "";
	return pCfg->GetNodeValue();
}



//////////////////////////////////////////////////////////////////////////
// ��    ��:  SBaseConfigSet
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 14:40
// ��    ��:  �����ļ�������
//////////////////////////////////////////////////////////////////////////

SBaseConfigSet::SBaseConfigSet()
{
	SKTOBJECT("SBaseConfigSet");
	m_ConfigSet.setShared(true);
	m_ConfigSet.setAutoDelete(true);
	m_ConfigName.setShared(true);
}
SBaseConfigSet::~SBaseConfigSet()
{
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ����µ�����ʵ������
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 14:41
// ����˵��:  @pCfg��ʾ������ʵ�����׽ڵ�ָ��
//            @sName��ʾ�����õ�����
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SBaseConfigSet::AddConfig(SBaseConfig* pCfg,SString sName)
{
	m_ConfigSet.append(pCfg);
	m_ConfigName.append(sName);
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡָ��������ʵ������ָ��
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 14:42
// ����˵��:  @sName����ȡ����ʵ��������
// �� �� ֵ:  Ŀ������ʵ�����׽ڵ�ָ�룬��ƥ��ʱ����NULL
//////////////////////////////////////////////////////////////////////////
SBaseConfig* SBaseConfigSet::GetConfig(SString sName)
{
	int i,cnt=m_ConfigName.count();
	for(i=0;i<cnt;i++)
	{
		if(*m_ConfigName[i] == sName)
		{
			return m_ConfigSet[i];
		}
	}
	return NULL;
}



