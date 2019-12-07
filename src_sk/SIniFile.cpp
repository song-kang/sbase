/**
 *
 * �� �� �� : SIniFile.cpp
 * �������� : 2002-05-14
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: 2008/09/02 01:50:02 $
 * ��ǰ�汾 : $Revision: 1.1 $
 * �������� : INI�ļ������࣬�ɶ�дINI�ļ�
 * �޸ļ�¼ : 
 *            $Log: IniFile.cpp,v $
 *            Revision 1.1  2008/09/02 01:50:02  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2007/10/29 07:30:23  shaokt
 *            ��Ӷ����ַ�Ƿ�Ϊ4�ı����ļ�⺯����Ϊ���ARM������pack(1)��ɵ��ڴ�����
 *
 *            Revision 1.1  2007/08/15 03:09:22  shaokt
 *            *** empty log message ***
 *
 **/

#ifdef WIN32
#pragma warning(disable: 4786)
#pragma warning(disable: 4503)
#endif

#include "SIniFile.h"

/////////////////////////////////////////////////////////////////////
// Construction/Destruction
/////////////////////////////////////////////////////////////////////
SIniFile::SIniFile()
{
	SKTOBJECT("SIniFile");
	m_pFirstGroup = NULL;
	m_bOpened = false;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  �ͷ��������ö����ͷ�����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-13 16:24
// ����˵��:  void
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SIniFile::Release()
{
	SIniFile::stuIniFileGroup *pGroup = m_pFirstGroup,*pG;
	SIniFile::stuIniFileField *pField,*pF;
	while(pGroup != NULL)
	{
		pField = pGroup->pFirstField;
		while(pField != NULL)
		{
			pF = pField->pNext;
			delete pField;
			pField = pF;
		}
		pG = pGroup->pNext;
		delete pGroup;
		pGroup = pG;
	}
	m_pFirstGroup = NULL;
}

SIniFile::~SIniFile()
{
	Release();
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ����INI�ļ�ȫ·��
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-13 16:09
// ����˵��:  @sPathFile��ʾ�ļ���ȫ·��
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SIniFile::SetPath(SString sPathFile)
{
	m_sPathFile = sPathFile;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ȡINI�ļ�����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-13 16:10
// ����˵��:  @sPathFile��ʾ���򿪵��ļ�����Ϊ��ʱʹ��SetPath������ļ���
// �� �� ֵ:  true��ʾ��ȡ�ɹ�,false��ʾ��ȡʧ��
//////////////////////////////////////////////////////////////////////////
bool SIniFile::ReadIniFile(SString sPathFile)
{
	SString sLine,sField,sVal;
	FILE *fp;
	char ch;
	SString sGroup = "";
	int pos;
	SIniFile::stuIniFileField *pField=NULL;

	if(sPathFile.size() > 0)
		SetPath(sPathFile);
	
	fp = fopen(sPathFile.data(),"r");
	if(!fp)
	{
	    fp = fopen(sPathFile.data(),"a+");
	    if(!fp)
	    {
			return false;
	    }
	    fclose(fp);
	    fp = fopen(sPathFile.data(),"r");
	    if(!fp)
		return false;
	}
	bool bExit=false;
	while(1)
	{
		sLine = "";
		while(1)
		{
			if(fread(&ch,sizeof(char),1,fp) <=0)
			{
				bExit = true;
				break;
			}
			if(ch == '\n')
				break;
			if(ch != '\r')
				sLine += ch;
		}
		if(bExit)
			break;

		pos = sLine.find('=');
		
		if(sLine.size() > 0 && sLine.at(0) == '[' && sLine.find(']') >0 )
		{
			sGroup = sLine.substr(1,sLine.find(']') - 1);
			continue;
		}
		else if(pos > 0 && sLine.at(0) != '#')
		{
			sField = sLine.substr(0,pos);
			sVal = sLine.substr(pos + 1);
			if(sVal.at(sVal.size()-1) == '\r' || sVal.at(sVal.size()-1) == '\n')
				sVal = sVal.substr(0,sVal.size()-1);
			if(sVal.at(sVal.size()-1) == '\r' || sVal.at(sVal.size()-1) == '\n')
				sVal = sVal.substr(0,sVal.size()-1);
			pField = GetField(sGroup,sField);
			pField->sValue = sVal;
		}
		else //if(sLine.GetLength() == 0 || sLine.GetAt(0) == '#')		
		{
			if(pField == NULL)
			{
				pField=GetField("","");
				pField->sValue=sLine;
			}
			else
			{
				SIniFile::stuIniFileField *pNew=new SIniFile::stuIniFileField;
				pNew->pNext=pField->pNext;
				pField->pNext=pNew;
				pNew->sFieldName="";
				pNew->sValue=sLine;
			}
		}

	}

	fclose(fp);
	m_bOpened = true;
	return true;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ����ָ�����������Ƿ����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-13 16:11
// ����˵��:  @sGroup��ʾ������(�����ŵı���)
//            @sField��ʾ���ڵ�����������
// �� �� ֵ:  true��ʾָ������������ڣ�false��ʾָ�������������
//////////////////////////////////////////////////////////////////////////
bool SIniFile::IsKey(SString sGroup, SString sField)
{
	SString sVal;
	if(m_bOpened == false)
		return false;

	SIniFile::stuIniFileField *pField;
	pField = GetField(sGroup,sField,false);
	if(pField == NULL)
		return false;
	return true;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ȡָ���������ֵ����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-13 16:13
// ����˵��:  @sGroup��ʾ������(�����ŵı���)
//            @sField��ʾ���ڵ�����������
// �� �� ֵ:  ��������������
//////////////////////////////////////////////////////////////////////////
SString SIniFile::GetValue(SString sGroup,SString sField)
{
	SString sVal;
	if(m_bOpened == false)
		return "";

	SIniFile::stuIniFileField *pField;
	pField = GetField(sGroup,sField,false);
	if(pField == NULL)
		return "";
	sVal = pField->sValue;

	return sVal;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ȡָ���������������ֵ��������ָ��δ����ʱ��ȱʡֵ
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-13 16:14
// ����˵��:  @sGroup��ʾ������(�����ŵı���)
//            @sField��ʾ���ڵ�����������
//            @idefault��ʾ��ȡʧ��(δ���õ������)ʱ��ȱʡֵ
// �� �� ֵ:  ����ֵ
//////////////////////////////////////////////////////////////////////////
int SIniFile::GetKeyIntValue(SString sGroup, SString sField, int idefault /*= 0*/)
{
	if(!IsKey(sGroup,sField))
		return idefault;

	SString val=GetValue(sGroup,sField);
	if(val.size() == 0)
		return idefault;

	return atoi(val.data());
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ȡָ������������ַ���ֵ��������ָ��δ����ʱ��ȱʡֵ
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-13 16:20
// ����˵��:  @sGroup��ʾ������(�����ŵı���)
//            @sField��ʾ���ڵ�����������
//            @szdefault��ʾ��ȡʧ��(δ���õ������)ʱ��ȱʡֵ
// �� �� ֵ:  ����ֵ
//////////////////////////////////////////////////////////////////////////
SString SIniFile::GetKeyStringValue(SString sGroup, SString sField, SString szdefault /*= ""*/)
{
	if(!IsKey(sGroup,sField))
		return szdefault;
	return GetValue(sGroup,sField);
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ����ָ���������ֵ����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-13 16:21
// ����˵��:  @sGroup��ʾ������(�����ŵı���)
//            @sField��ʾ���ڵ�����������
//            @sVal��ʾ����ֵ
// �� �� ֵ:  true��ʾ���óɹ���false��ʾ����ʧ��
//////////////////////////////////////////////////////////////////////////
bool SIniFile::SetValue(SString sGroup,SString sField,SString sVal)
{
	if(m_bOpened == false)
		return false;
	SIniFile::stuIniFileField *pField;
	pField = GetField(sGroup,sField);
	if(pField == NULL)
		return false;
	pField->sValue = sVal;

	return true;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ����ָ���������������ֵ���������Զ������ļ�
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-13 16:22
// ����˵��:  @sGroup��ʾ������(�����ŵı���)
//            @sField��ʾ���ڵ�����������
//            @ivalue��ʾ����ֵ
//            @autoSave��ʾ�Ƿ��Զ����棬true�Զ����棬false������
// �� �� ֵ:  true��ʾ���óɹ���false��ʾ����ʧ��
//////////////////////////////////////////////////////////////////////////
bool SIniFile::SetKeyIntValue(SString sGroup, SString sField, int ivalue, bool autoSave/*=true*/)
{
	char buf[32];
	memset(buf,0,32);
	sprintf(buf,"%d",ivalue);
	SetValue(sGroup,sField,buf);
	if(autoSave && !this->WriteFile())
		return false;
	return true;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ����ָ����������ַ�����ֵ���������Զ������ļ�
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-13 16:23
// ����˵��:  @sGroup��ʾ������(�����ŵı���)
//            @sField��ʾ���ڵ�����������
//            @szvalue��ʾ����ֵ
//            @autoSave��ʾ�Ƿ��Զ����棬true�Զ����棬false������
// �� �� ֵ:  true��ʾ���óɹ���false��ʾ����ʧ��
//////////////////////////////////////////////////////////////////////////
bool SIniFile::SetKeyStringValue(SString sGroup, SString sField, SString szvalue, bool autoSave/*=true*/)
{
	SetValue(sGroup,sField,szvalue);
	if(autoSave && !this->WriteFile())
		return false;
	return true;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ����ʱ���ļ���������INI�ļ�
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-13 16:24
// ����˵��:  void
// �� �� ֵ:  true��ʾ����ɹ���false��ʾ����ʧ��
//////////////////////////////////////////////////////////////////////////
bool SIniFile::WriteFile()
{
	if(m_bOpened == false)
		return false;
	SIniFile::stuIniFileGroup *pGroup;
	SIniFile::stuIniFileField *pField;
	pGroup = m_pFirstGroup;
	FILE *fp;;

	fp = fopen(m_sPathFile.data(),"w");
	if(!fp)
		return false;
	SString sLine;

	while(pGroup != NULL)
	{
		if(pGroup->sGroupName.size() > 0)
		{
			sLine = "[" + pGroup->sGroupName + "]\r\n";
			fwrite(sLine.data(),sizeof(char),sLine.size(),fp);
		}
		pField = pGroup->pFirstField;
		while(pField != NULL)
		{
			if(pField->sFieldName.size() > 0)
			{
				sLine = pField->sFieldName + "=" + pField->sValue + "\r\n";
				fwrite(sLine.data(),sizeof(char),sLine.size(),fp);
			}
			else
			{
				sLine = pField->sValue + "\r\n";
				fwrite(sLine.data(), sizeof(char), sLine.size(), fp);
			}
			pField = pField->pNext;
		}

		pGroup = pGroup->pNext;
	}
	fclose(fp);

	return true;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ����ָ������,��������������Զ�����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-13 16:26
// ����˵��:  @sGroup��ʾ������(�����ŵı���)
//            @bCreate�Ƿ����鲻���ڵ�ʱ�򴴽��µ���,true��������false������
// �� �� ֵ:  stuIniFileGroup*
//////////////////////////////////////////////////////////////////////////
SIniFile::stuIniFileGroup * SIniFile::GetGroup(SString sGroup,bool bCreate)
{
	SIniFile::stuIniFileGroup *pLast = m_pFirstGroup,*p = NULL;
	while(pLast != NULL)
	{
		if(pLast->sGroupName.compare(sGroup) == 0)
			break;
		p = pLast;
		pLast = pLast->pNext;
	}
	if(pLast == NULL && bCreate)
	{
		pLast = new SIniFile::stuIniFileGroup;
		pLast->sGroupName = sGroup;
		pLast->pNext = NULL;
		pLast->pFirstField = NULL;
		if(p == NULL)
			m_pFirstGroup = pLast;
		else
			p->pNext = pLast;
	}

	return pLast;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  Ͷ��ָ��������ָ������������������Զ�����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-13 16:28
// ����˵��:  @sGroup��ʾ������(�����ŵı���)
//            @sField��ʾ����������
//            @bCreate�Ƿ����鲻���ڵ�ʱ�򴴽��µ����������,true��������false������
// �� �� ֵ:  
//////////////////////////////////////////////////////////////////////////
SIniFile::stuIniFileField * SIniFile::GetField(SString sGroup,SString sField,bool bCreate)
{
	SIniFile::stuIniFileGroup *pGroup;
	SIniFile::stuIniFileField *pField,*p = NULL;
	pGroup = GetGroup(sGroup,bCreate);
	if(pGroup == NULL)
		return NULL;
	pField = pGroup->pFirstField;
	while(pField != NULL)
	{
		if(pField->sFieldName.compare(sField) == 0)
			break;
		p = pField;
		pField = pField->pNext;
	}
	if(pField == NULL && bCreate)
	{
		pField = new SIniFile::stuIniFileField;
		pField->pNext = NULL;
		pField->sFieldName = sField;
		pField->sValue = "";
		if(p == NULL)
			pGroup->pFirstField = pField;
		else
			p->pNext = pField;
	}
	return pField;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ��һ��INI��
// ��    ��:  �ۿ���
// ����ʱ��:  2014-12-5 13:09
// ����˵��:  @hGroup��ʾ��һ��ľ����NULL��ʾȡ��һ����
// �� �� ֵ:  long, NULL��ʾû����һ��
//////////////////////////////////////////////////////////////////////////
long SIniFile::GetNextGroup(long hGroup)
{
	stuIniFileGroup *pGroup = (stuIniFileGroup *)hGroup;
	if (pGroup == NULL)
		return (long)m_pFirstGroup;
	return (long)pGroup->pNext;

}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡָ��INI���ڵ���һ������
// ��    ��:  �ۿ���
// ����ʱ��:  2014-12-5 13:09
// ����˵��:  @hGroup��ʾ����
//            @hField��ʾ��һ���Եľ����NULL��ʾȡ����һ����
// �� �� ֵ:  long, NULL��ʾû����һ��
//////////////////////////////////////////////////////////////////////////
long SIniFile::GetNextField(long hGroup, long hField)
{
	stuIniFileGroup *pGroup = (stuIniFileGroup *)hGroup;
	stuIniFileField *pField = (stuIniFileField *)hField;
	if (pGroup == NULL)
		return 0;
	if (pField == NULL)
		return (long)pGroup->pFirstField;
	return (long)pField->pNext;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡָ��������������
// ��    ��:  �ۿ���
// ����ʱ��:  2014-12-5 13:19
// ����˵��:  @hGroup��ʾ������������ͨ��GetNextGroup��ȡ����Ч���
// �� �� ֵ:  ������
//////////////////////////////////////////////////////////////////////////
SString SIniFile::GetGroupName(long hGroup)
{
	stuIniFileGroup *pGroup = (stuIniFileGroup *)hGroup;
	if (pGroup == NULL)
		return "";
	return pGroup->sGroupName;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡָ����������Ե�����
// ��    ��:  �ۿ���
// ����ʱ��:  2014-12-5 13:19
// ����˵��:  @hField��ʾ���Ծ����������ͨ��GetNextField��ȡ����Ч���
// �� �� ֵ:  ��������
//////////////////////////////////////////////////////////////////////////
SString SIniFile::GetFieldName(long hField)
{
	stuIniFileField *pField = (stuIniFileField *)hField;
	if (pField == NULL)
		return "";
	return pField->sFieldName;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡָ����������Ե�ֵ
// ��    ��:  �ۿ���
// ����ʱ��:  2014-12-5 13:19
// ����˵��:  @hField��ʾ���Ծ����������ͨ��GetNextField��ȡ����Ч���
// �� �� ֵ:  ����ֵ
//////////////////////////////////////////////////////////////////////////
SString SIniFile::GetFieldValue(long hField)
{
	stuIniFileField *pField = (stuIniFileField *)hField;
	if (pField == NULL)
		return "";
	return pField->sValue;
}
