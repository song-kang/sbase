 /**
 *
 * �� �� �� : SString.cpp
 * �������� : 2006-09-27
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: 2011/08/31 08:43:35 $
 * ��ǰ�汾 : $Revision: 1.1 $
 * �������� : a string class just like QString
 * �޸ļ�¼ : 
 *            $Log: SString.cpp,v $
 *            Revision 1.1  2011/08/31 08:43:35  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.6  2011/04/29 08:53:08  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.5  2011/04/23 10:53:52  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.4  2011/04/12 01:35:30  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2011/04/11 12:08:36  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2011/02/28 01:28:33  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2010/12/20 08:38:28  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2010/08/09 03:04:44  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2010/01/22 01:24:25  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2009/09/15 02:14:09  shaokt
 *            ͨѸ��¼����װ����Ŀ�״��ύ
 *
 *            Revision 1.5  2009/05/19 02:50:38  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.4  2009/01/31 08:06:42  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2009/01/31 06:37:34  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2008/12/22 08:54:13  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/01 11:38:28  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/01 08:58:03  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.8  2007/12/10 06:40:28  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.7  2007/10/29 07:30:23  shaokt
 *            ��Ӷ����ַ�Ƿ�Ϊ4�ı����ļ�⺯����Ϊ���ARM������pack(1)��ɵ��ڴ�����
 *
 *            Revision 1.6  2007/10/19 11:57:58  shaokt
 *            ����>/>=/</<=���ַ����ȽϺ���
 *
 *            Revision 1.5  2007/09/29 07:27:05  shaokt
 *            ��������
 *
 *            Revision 1.4  2007/09/29 05:13:35  shaokt
 *            ���Բ���ʹ�����÷�ʽ
 *
 *            Revision 1.3  2007/09/21 07:53:01  shaokt
 *            ʹ�û�������дSString,����ԭ��ʹ�õ�std::string��
 *
 *            Revision 1.2  2007/08/27 09:51:07  shaokt
 *            �������
 *
 *            Revision 1.1  2007/08/15 03:09:22  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2007/05/28 07:33:59  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2006/10/19 02:06:41  tester
 *            �޸�communicate,xml,ini
 *
 **/

#include "SString.h"
const char g_sstring_null[1]={0};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SString::SString()
{
	SKTOBJECT("SString");
	m_pStr = NULL;
	m_iSize = 0;
	m_iStrLen = 0;
	SetString("");
}

SString::SString(int size)
{
	SKTOBJECT("SString");
	m_pStr = NULL;
	m_iSize = 0;
	m_iStrLen = 0;
	SetBuffer(size+1);
}

SString::SString(const char *pStr)

{
	SKTOBJECT("SString");
	m_pStr = NULL;
	m_iSize = 0;
	m_iStrLen = 0;
	SetString(pStr);
}

SString::SString(const SString &str)
{
	SKTOBJECT("SString");
	m_pStr = NULL;
	m_iSize = 0;
	m_iStrLen = 0;
	SetString(str.m_pStr);
}
SString::SString(const std::string &str)
{
	SKTOBJECT("SString");
	m_pStr = NULL;
	m_iSize = 0;
	m_iStrLen = 0;
	SetString(str.c_str());
}
SString::~SString()
{
	if(m_pStr != NULL)
		delete[] m_pStr;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ���û�������С
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-24 13:06
// ����˵��:  @sizeΪ�µĴ�С
// �� �� ֵ:  true��ʾ���óɹ���false��ʾ�´�СС��Ŀǰ���ַ�������+1ʱ���ش���
//////////////////////////////////////////////////////////////////////////
bool SString::SetBuffer(int size)
{
	if(m_iSize < size)
	{
		char *p = new char[size];
		memset(p,0,size);
		if(m_iSize > 0)
			memcpy(p,m_pStr,m_iSize);
		p[m_iSize]='\0';
		char *p0 = m_pStr;
		m_pStr = p;
		m_iSize = size;
		delete[] p0;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �����ַ����������������¼����ַ�����С�����ⲿ�޸��ַ��������ݺ�ʹ��
// ��    ��:  �ۿ���
// ����ʱ��:  2013-3-6 9:16
// ����˵��:  void
// �� �� ֵ:  ���ص�ǰ�ַ�����ʵ���ȣ����޸�m_iStrLen
//////////////////////////////////////////////////////////////////////////
int SString::PrepareStrLen()
{
	if(m_pStr == NULL)
		m_iStrLen = 0;
	else
		m_iStrLen = (int)strlen(m_pStr);
	return m_iStrLen;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  �����ַ������ͷŶ���Ļ�����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-24 13:36
// ����˵��:  void
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SString::FreeExtra()
{
	if(m_pStr != NULL)
	{
		int size = m_iStrLen;
		if(m_iSize > size)
		{
			//����
			if(size == 0)
			{
				delete[] m_pStr;
				m_pStr = NULL;
				m_iSize= 0;
			}
			else
			{
				char *pStr = new char[size+1];
				pStr[size] = 0;
				memcpy(pStr,m_pStr,size);
				delete[] m_pStr;
				m_pStr = pStr;
			}
		}
	}
}

bool SString::isEmpty() const
{
	return 0 == m_iStrLen ;
}

char* SString::data() const
{
	return m_pStr;
}

char SString::at(int index) const
{
	if(index >= m_iStrLen)
		return '\0';
	return m_pStr[index];	
}

SString SString::substr(int beg, int len)
{
	SString str;
	if(beg<0 || beg>m_iStrLen)
		beg = m_iStrLen;
	if(len == -1 || len > m_iStrLen-beg)
		len = m_iStrLen-beg;

	delete[] str.m_pStr;
	str.m_pStr = new char[len+1];
	str.m_iSize= len+1;
	str.m_iStrLen = len;
	str.m_pStr[len] = '\0';
	if(len > 0)
		memcpy(str.m_pStr,m_pStr+beg,len);
	return str;
}

int SString::length() const
{
	return m_iStrLen;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ���һ��������ָ�����ȵ��ַ���
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-24 13:24
// ����˵��:  @aΪ����������ֵ
//            @fieldWidthΪ�������ַ������ȣ�>0��ʾ����������߲��룬<0�������ұ߲���
//            @baseΪ���������̶�Ϊ10
//            @fillCharΪ�����ַ�
// �� �� ֵ:  �ַ���
//////////////////////////////////////////////////////////////////////////
SString SString::arg(int a, int fieldWidth /*= 0*/, int base /*= 10*/, char fillChar /*= ' '*/)
{
	base=base;	char buf[32];
	memset(buf,0,32);
	sprintf(buf,"%d",a);
	//itoa(a,buf,base);
	SString str(buf);
	if(fieldWidth>0)
	{
		while(m_iStrLen < fieldWidth)
		{
			//*this = fillChar + *this;
			SString s2= fillChar;
			s2 += *this;
			*this = s2;
		}
	}
	else if(fieldWidth<0)
	{
		fieldWidth = -fieldWidth;
		while(m_iStrLen < fieldWidth)
		{
			*this = *this + fillChar;
		}
	}
	return str;
}

void SString::clear()
{
	SetString("");
}

void SString::SetString(const char *pStr,int len/*=-1*/)
{
	if(pStr == NULL)
	{
		SetString("");
		return;
	}
	int size = (int)(len==-1?(strlen(pStr)+1):(len+1));
	if(size > m_iSize)
	{
		if(m_pStr != NULL)
			delete[] m_pStr;
		m_pStr = new char[size];
		if(m_pStr == NULL)
		{
			m_pStr = new char[1];
			m_iSize = 1;
			m_iStrLen = 0;
			m_pStr[0] = '\0';
			printf("SString ERROR:alloc bytes=%d return null!",size);
			return ;
		}
		m_iSize = size;
	}
	m_iStrLen = size-1;
	m_pStr[size-1] = '\0';
	if(size > 1)
		memcpy(m_pStr,pStr,size-1);
}

SString &SString::operator=(char c)
{
	char buf[2];
	buf[0] = c;
	buf[1] = '\0';
	SetString(buf);
	return *this;
}

SString &SString::operator=(const SString &s)
{
	SetString(s.m_pStr);
	return *this;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  �ַ����Ƚ�
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-24 13:29
// ����˵��:  @���Ƚϵ��ַ���
// �� �� ֵ:  <0��ʾ��ǰ��С��str,=0��ʾ��ǰ������str,>0��ʾ��ǰ������str
//////////////////////////////////////////////////////////////////////////
int SString::compare(const SString &str)
{
	return strcmp(m_pStr,str.m_pStr);
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �Ƚ������ַ����Ƿ���ȫ���
// ��    ��:  �ۿ���
// ����ʱ��:  2015-10-27 13:30
// ����˵��:  @s1Ϊ�ַ���1
//         :  @s2Ϊ�ַ���2
// �� �� ֵ:  true/false
//////////////////////////////////////////////////////////////////////////
bool SString::equals(const register char* s1,const register char* s2)
{
	if(s1 == NULL)
	{
		if(s2 == NULL || (s2 != NULL && s2[0] == '\0'))
			return true;
		return false;
	}
	else if(s2 == NULL)
	{
		if(s1 == NULL || (s1 != NULL && s1[0] == '\0'))
			return true;
		return false;
	}
	
	while(*s1 != '\0' && *s2 != '\0' && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*s1 == '\0' && *s2 == '\0');
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ���ذ�ȫ�ַ�����������ΪNULLʱ�����ؿ��ַ���,���򷵻����봮
// ��    ��:  �ۿ���
// ����ʱ��:  2015-12-25 10:38
// ����˵��:  
// �� �� ֵ:  
//////////////////////////////////////////////////////////////////////////
char* SString::safeChar(const char* str)
{
	if(str == NULL)
		return (char*)g_sstring_null;
	else
		return (char*)str;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  �ַ�����Сд�����бȽ�
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-24 13:29
// ����˵��:  @���Ƚϵ��ַ���
// �� �� ֵ:  <0��ʾ��ǰ��С��str,=0��ʾ��ǰ������str,>0��ʾ��ǰ������str
//////////////////////////////////////////////////////////////////////////
int SString::compareNoCase(const SString &str)
{
#ifdef WIN32
	SString s1=*this;
	SString s2=str;
	_strupr(s1.m_pStr);
	_strupr(s2.m_pStr);
	return strcmp(s1.m_pStr,s2.m_pStr);
#else
	return strcasecmp(m_pStr,str.m_pStr);
#endif
}

int SString::find(char c, int i /*= 0*/, bool cs /*= true*/)//�����ַ�,cs�Ƿ��Сд����
{
	SString cmp;
	cmp=c;
	return find(cmp,i,cs);
}

int SString::find(SString s, int i /*= -1*/, bool cs /*= true*/)//�����ַ���,cs�Ƿ��Сд����
{
	SString s1,s2;
	if(i>m_iStrLen)
		i = m_iStrLen;
	if(cs == false)
	{
		s1=toUpper();
		s2 = s.toUpper();
	}
	else
	{
		s1 = *this;
		s2 = s;
	}
	char *pHexesult = strstr(s1.m_pStr+i,s2.m_pStr);
	if(pHexesult == NULL)
		return -1;
	return (int)(pHexesult - s1.m_pStr);
}

int SString::findRev(char c, int i /*= -1*/, bool cs /*= true*/)//��������ַ�,cs�Ƿ��Сд����
{
	if(i == -1)
		i = m_iStrLen-1;
	for(;i>=0;i--)
	{
		if(m_pStr[i] == c)
			return i;
	}
	return -1;
}

int SString::findRev(SString s, int i /*= -1*/, bool cs /*= true*/)//��������ַ���,cs�Ƿ��Сд����
{
	if(i == -1 || i>m_iStrLen-1)
		i = m_iStrLen-1;
	if(i < 0)
		return -1;

	SString s1,s2;
	if(cs == false)
	{
		s1=toUpper();
		s2 = s1.toUpper();
	}
	else
	{
		s1 = *this;
		s2 = s;
	}
	char *pStrTmp=s1.m_pStr+i;
	for(;pStrTmp!=s1.m_pStr;pStrTmp--)
	{
		char *pHexesult = strstr(pStrTmp,s2.m_pStr);
		if(pHexesult != NULL)
		{
			return (int)(pHexesult - s1.m_pStr);
		}
	}
	if(strstr(pStrTmp,s2.m_pStr) != NULL)
		return 0;
	return -1;
}

bool SString::operator==(const SString &s)
{
	return (m_iStrLen==s.m_iStrLen) && (equals(m_pStr,s.m_pStr));//compare
}

bool SString::operator==(const char *s)
{
	return equals(m_pStr,s);//compare
}
//compare
bool SString::operator>(const char *s)
{
	return compare(s==NULL?"":s) > 0;
}
bool SString::operator>=(const char *s)
{
	if(s == NULL)
		return false;
	return compare(s==NULL?"":s) >= 0;
}
bool SString::operator<(const char *s)
{
	if(s == NULL)
		return false;
	return compare(s==NULL?"":s) < 0;
}
bool SString::operator<=(const char *s)
{
	if(s == NULL)
		return false;
	return compare(s==NULL?"":s) <= 0;
}
bool SString::operator>(const SString &s)
{
	return compare(s) > 0;
}
bool SString::operator>=(const SString &s)
{
	return compare(s) >= 0;
}
bool SString::operator<(const SString &s)
{
	return compare(s) < 0;
}
bool SString::operator<=(const SString &s)
{
	return compare(s) <= 0;
}

bool SString::operator!=(const SString &s)
{
	return (m_iStrLen!=s.m_iStrLen) || (!equals(m_pStr,s.m_pStr));
}

bool SString::operator!=(const char *s)
{
	return (!equals(m_pStr,s));
}

SString & SString::operator+= ( const SString & other )
{
	int size  = m_iStrLen + other.m_iStrLen+1;
	if(m_iSize < size)
	{
		//�ռ䲻�㣬�Ŵ�һ��
		SetBuffer(size<<1);
	}
	memcpy(m_pStr+m_iStrLen,other.m_pStr,other.m_iStrLen);
	m_iStrLen += other.m_iStrLen;
	m_pStr[m_iStrLen] = '\0';
	return *this;
}

SString & SString::operator+=( char c )
{
	int size  = m_iStrLen + 2;
	if(m_iSize < size)
	{
		//�ռ䲻�㣬�Ŵ�һ��
		SetBuffer(size<<1);
	}
	m_pStr[m_iStrLen]=c;
	m_iStrLen ++;
	m_pStr[m_iStrLen] = '\0';
	return *this;
}

SString & SString::operator+=( char *str )
{
	int len = (int)strlen(str);
	int size  = m_iStrLen + len +1;
	if(m_iSize < size)
	{
		//�ռ䲻�㣬�Ŵ�һ��
		SetBuffer(size<<1);
	}
	memcpy(m_pStr+m_iStrLen,str,len);
	m_iStrLen += len;
	m_pStr[m_iStrLen] = '\0';
	return *this;
}

SString & SString::operator+=(const  char *str )
{
	int len = (int)strlen(str);
	int size  = m_iStrLen + len +1;
	if(m_iSize < size)
	{
		//�ռ䲻�㣬�Ŵ�һ��
		SetBuffer(size<<1);
	}
	memcpy(m_pStr+m_iStrLen,str,len);
	m_iStrLen += len;
	m_pStr[m_iStrLen] = '\0';
	return *this;
}

SString SString::operator+(const SString &s)
{
	int size  = m_iStrLen + s.m_iStrLen;
	SString str(size);
	if(m_iStrLen>0)
		memcpy(str.m_pStr,m_pStr,m_iStrLen);
	if(s.m_iStrLen>0)
		memcpy(str.m_pStr+m_iStrLen,s.m_pStr,s.m_iStrLen);
	
	str.m_pStr[size] ='\0';
	str.m_iStrLen = size;
	return str;

}

SString SString::operator+(char * s) 
{
	int len = (int)strlen(s);
	int size  = m_iStrLen + len;
	SString str(size);
	if(m_iStrLen>0)
		memcpy(str.m_pStr,m_pStr,m_iStrLen);
	if(len>0)
		memcpy(str.m_pStr+m_iStrLen,s,len);
	
	str.m_pStr[size] ='\0';
	str.m_iStrLen = size;
	return str;
}

SString SString::operator+(const char *s)
{
	int len = (int)strlen(s);
	int size = m_iStrLen + len;
	SString str(size);
	if (m_iStrLen>0)
		memcpy(str.m_pStr, m_pStr, m_iStrLen);
	if (len>0)
		memcpy(str.m_pStr + m_iStrLen, s, len);

	str.m_pStr[size] = '\0';
	str.m_iStrLen = size;
	return str;
}

SString SString::operator+(char c)
{
	int size  = m_iStrLen + 1;
	SString str(size);
	if(m_iStrLen>0)
		memcpy(str.m_pStr,m_pStr,m_iStrLen);
	str.m_pStr[m_iStrLen] = c;
	str.m_pStr[m_iStrLen+1] = '\0';
	str.m_iStrLen = m_iStrLen+1;
	return str;
}

SString SString::left(int len)
{
	return mid(0,len);
}

SString SString::right(int len)
{
	int i=m_iStrLen-len;
	if(i<0)
		i=0;
	return mid(i,m_iStrLen-i);
}

SString SString::mid(int i, int len /*= -1*/)
{
	return substr(i,len);
}

SString SString::replace ( int pos, int len, const SString & sDesc )
{
	*this = left(pos) + sDesc + right(m_iStrLen - pos - len);
	return *this;
}

SString SString::replace ( int pos, int len, const char * str)
{
	*this = left(pos) + (str==NULL?"":str) + right(m_iStrLen - pos - len);
	return *this;
}

SString SString::replace ( int pos, int len, char c )
{
	*this = left(pos) + c + right(m_iStrLen - pos - len);
	return *this;
}

SString SString::replace ( const SString &before, const SString &after, bool cs/*=true*/)//cs: case sensitive
{
	if(before.length() == 0)
		return *this;
	int pos=0;
	while(1)
	{
		pos=find(before,pos,cs);
		if(pos < 0)
			return *this;
		replace(pos,before.m_iStrLen,after);
		pos += after.m_iStrLen;
	}
	
	return *this;
}

SString SString::replace (const char* before,const char *after, bool cs/*=true*/)//cs: case sensitive
{
	SString sb,sa;
	sb = before;
	sa = after;
	return replace(sb,sa,cs);
}

SString& SString::sprintf(const char * cformat, ...)
{
	char buf[10240];
	memset(buf,0,10240);
	va_list ap;
	va_start(ap, cformat);
	vsprintf(buf,cformat,ap);
	va_end(ap);
	*this=buf;
	return *this;
}

SString SString::toUpper()
{
	SString s=*this;
	char *p = (char*)s.m_pStr;
	register int i,cnt=m_iStrLen;
	for(i=0;i<cnt;i++)
	{
		if(p[i] >= 'a' && p[i] <= 'z')
		{
			p[i] = p[i] + ('A' - 'a');
		}
	}
	return s;
}

SString SString::toLower()
{
	SString s=*this;
	char *p = (char*)s.m_pStr;
	register int i,cnt=m_iStrLen;
	for(i=0;i<cnt;i++)
	{
		if(p[i] >= 'A' && p[i] <= 'Z')
		{
			p[i] = p[i] + ('a' - 'A');
		}
	}
	return s;	
}

short SString::toShort(bool * ok)
{
	int v=atoi(m_pStr);
	return (short)v;
}

int SString::toInt(bool * ok)
{
	SString sTemp = *this;
	sTemp = sTemp.toUpper();
	if(sTemp.left(2) == "0X")
	{
		//16����
		sTemp = sTemp.mid(2);
		int v = 0;
		while(sTemp.m_iStrLen > 0)
		{
			v *= 16;
			char ch = sTemp.at(0);
			if(ch >= 'A' && ch <= 'F')
				v += 10 + ch - 'A';
			else
				v += ch - '0';
			sTemp = sTemp.mid(1);
		}
		return v;
	}
	else
	{
		int v=atoi(m_pStr);
		return v;
	}
}

long SString::toLong(bool * ok)
{
	SString sTemp = *this;
	sTemp = sTemp.toUpper();
	if(sTemp.left(2) == "0X")
	{
		//16����
		sTemp = sTemp.mid(2);
		int v = 0;
		while(sTemp.m_iStrLen > 0)
		{
			v *= 16;
			char ch = sTemp.at(0);
			if(ch >= 'A' && ch <= 'F')
				v += 10 + ch - 'A';
			else
				v += ch - '0';
			sTemp = sTemp.mid(1);
		}
		return v;
	}
	else
	{
		long v=atol(m_pStr);
		return v;
	}
}

unsigned short SString::toUShort(bool * ok)
{
	return (unsigned short)toShort();
}

unsigned int SString::toUInt(bool * ok)
{
	return (unsigned int)toInt(ok);
}

unsigned long SString::toULong(bool * ok)
{
	return (unsigned long)toLong(ok);
}

float SString::toFloat(bool *ok)
{
	float v=(float)atof(m_pStr);
	return v;
}

double SString::toDouble(bool *ok)
{
	return (double)toFloat(ok);
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ָ����MAC��ַ�ַ���ת��Ϊ6���ֽڻ�����(00:00:00:00:00:00��00-00-00-00-00-00)
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-24 14:07
// ����˵��:  @strΪԴ�ַ�����ʽMAC��ַ
//            @pHexΪMAC��ַ�Ĵ�Ż�������6�ֽ�
// �� �� ֵ:  true��ʾת���ɹ���false��ʾʧ��
//////////////////////////////////////////////////////////////////////////
bool SString::MacStrToHex(SString str,unsigned char *pHex)
{
	if(str.length() != 17)
		return false;
	if(str.find(":") >= 0)
		str = str.replace((char*)":",(char*)"-");
	str = str.toUpper();
	for(int i2=0;i2<6;i2++)
	{
		SString s2 = SString::GetIdAttribute(i2+1,str,"-");
		if(s2.m_iStrLen > 2)
			s2 = s2.Left(2);
		while(s2.m_iStrLen < 2)
			s2 = "0"+s2;
		SString::StrToHex(s2,&pHex[i2]);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ��16���Ƶ�MAC��ַת�����ַ���
// ��    ��:  �ۿ���
// ����ʱ��:  2013-2-27 17:38
// ����˵��:  @pMacByteΪMAC��ַ
//            @pHexStrΪ�ַ���������������ռ���ڵ���18���ַ�
//            @chΪ�������ȱʡΪ':'
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SString::MacHexToStr(BYTE *pMacByte,char *pHexStr,char ch/*=':'*/)
{
	BYTE bHexChar[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	register int i,pos=0;
	for (i=0;i<6;i++)
	{
		pHexStr[pos] = bHexChar[pMacByte[i]>>4];
		pHexStr[pos+1] = bHexChar[pMacByte[i]&0x0F];
		pHexStr[pos+2] = ch;
		pos +=3;
	}
	pHexStr[17] = '\0';
}


////////////////////////////////////////////////////////////////////////
// ��    ��:  ���������ַ�����ʽ��16������ת����BYTE������
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-24 14:03
// ����˵��:  @strΪ16���Ʊ�ʾ���������ֽ��м��޷ָ�����ÿ���ֽ�ռ�����ַ�
//            @pHexΪĿ�껺����
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SString::StrToHex(char* str,int str_len,unsigned char *pHex)
{
	register int i,times=str_len>>1;
	register BYTE b;
	register char ch;
	for(i=0;i<times;i++)
	{
		ch = *str;
		if(ch>='0' && ch<='9')
			ch -= '0';
		else if(ch >='a' && ch <= 'f')
			ch = ch + 10 - 'a';
		else if(ch >= 'A' && ch <= 'F')
			ch = ch + 10 - 'A';
		else
			ch=0;
		b=(ch<<4);
		str++;

		ch = *str;
		if(ch>='0' && ch<='9')
			ch -= '0';
		else if(ch >='a' && ch <= 'f')
			ch = ch + 10 - 'a';
		else if(ch >= 'A' && ch <= 'F')
			ch = ch + 10 - 'A';
		else
			ch=0;
		b|=ch;
		str++;

		*pHex = b;
		pHex++;
	}
}
void SString::StrToHex(SString str,unsigned char *pHex)
{
	StrToHex(str.data(),str.length(),pHex);
// 	SString s;
// 	int index=0;
// 	while(str.m_iStrLen >= 2)
// 	{
// 		s = "0x"+str.left(2);
// 		str = str.mid(2);
// 		pHex[index++] = s.toInt();
// 	}
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ָ����16���ƻ���������ת�����ַ���
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-24 14:05
// ����˵��:  @pHexΪ����������
//            @iBytesΪ�ֽ���
//            @bSpaceΪ���ֽڼ��Ƿ���ո�
//         :  @iMaxBytes��ʾ�������ֽ�����0��ʾ������
// �� �� ֵ:  �µ��ַ�����ÿ���ֽ�ռ�����ַ�����bSpaceΪtrue�ټ�һ���ո��
//////////////////////////////////////////////////////////////////////////
SString SString::HexToStr(unsigned char *pHex,int iBytes,bool bSpace,int iMaxBytes)
{
	static BYTE bHexChar[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	if(iMaxBytes != 0 && iBytes > iMaxBytes)
		iBytes = iMaxBytes;
    char *pr=NULL;
	if(bSpace)
	{
		pr=new char[3*iBytes +1];
		pr[3*iBytes]=0;
		int  i;
		int pos=0;
		for (i=0;i<iBytes;i++)
		{
			pr[pos] = bHexChar[pHex[i]>>4];
			pr[pos+1] = bHexChar[pHex[i] & 0x0F];
			pr[pos+2]=' ';
			pos +=3;
		}
	}
	else
	{
		pr=new char[2*iBytes +1];
		pr[2*iBytes]=0;
		int  i;
		int pos=0;
		for (i=0;i<iBytes;i++)
		{
			pr[pos] = bHexChar[pHex[i]>>4];
			pr[pos+1] = bHexChar[pHex[i] & 0x0F];
			pos +=2;
		}
	}
	SString str=pr;
	delete[] pr;
	return str;
}

SString SString::upper()
{
	return toUpper();
}
SString SString::lower()
{
	return toLower();
}
SString SString::stripWhiteSpace()
{
	while(left(1) == " " || left(1) == "\r" || left(1) == "\n" || left(1) == "\t")
		*this = right(m_iStrLen-1);
	while(right(1) == " " || right(1) == "\r" || right(1) == "\n" || right(1) == "\t")
		*this = left(m_iStrLen-1);
	return *this;
}

SString SString::fill(char ch, int size/*=-1*/)
{
	int i,cnt;
	if(size == -1)
	{
		cnt = m_iStrLen;
		char *p = (char*)m_pStr;
		for(i=0;i<cnt;i++)
		{
			p[i] = ch;
		}
	}
	else
	{
		*this="";
		for(i=0;i<size;i++)
			*this += ch;
	}
	return *this;
}
SString SString::limit(int size)
{
	SString str = *this;
	if(str.m_iStrLen > size)
		str = str.left(size);
	return str;
}


////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ʽ���ַ���,����ȷ����ʽ������ַ�������С��10240�ֽڣ���������쳣
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-24 13:45
// ����˵��:  ...
// �� �� ֵ:  SString
//////////////////////////////////////////////////////////////////////////
SString SString::toFormat(const char * cformat, ...)
{
	char buf[10240];
	memset(buf,0,10240);
	va_list ap;
	va_start(ap, cformat);
	vsprintf(buf,cformat,ap);
	va_end(ap);
	SString str(buf);
	return str;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ָ��Ŀ�껺�峤�ȵ��ַ�������
// ��    ��:  �ۿ���
// ����ʱ��:  2015-11-6 16:04
// ����˵��:  @sDstΪĿ���ַ���
//         :  @iMaxDstLenĿ���ַ������������ȣ���'\0'λ��
//         :  @sSrcΪԴ�ַ���
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SString::strncpy(char* sDst, int iMaxDstLen, const char* sSrc)
{
	iMaxDstLen--;
	while(iMaxDstLen > 0 && (*sSrc) != '\0')
	{
		*sDst = *sSrc;
		sDst++;
		sSrc++;
		iMaxDstLen--;
	}
	*sDst = '\0';
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �������еĿո��Ʊ�����س�������
// ��    ��:  �ۿ���
// ����ʱ��:  2016-10-29 9:50
// ����˵��:  @pStrΪԴ�ַ���
// �� �� ֵ:  char* ��ʾ���ַ�������pStrָ��֮��ΪpStr���Ӵ�
//////////////////////////////////////////////////////////////////////////
char* SString::SkipSpaceChar(char *pStr)
{
	while(*pStr == ' ' || *pStr == '\t' || *pStr == '\r' || *pStr == '\n')
		pStr++;
	return pStr;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ת��longΪ�ַ���
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-24 13:52
// ����˵��:  @vΪ��ֵ
//            @senΪ��������֧��10���ƺ�16����
// �� �� ֵ:  �µ��ַ���
//////////////////////////////////////////////////////////////////////////
SString SString::toString(long v,int sen/*=10*/)
{
	if (sen == 10)
		return toFormat("%ld", v);
	else if (sen == 16)
		return toFormat("%LX", v);
	return toFormat("%X", v);
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ����ǰָ�����ַ�����Ϊָ���ĳ���
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-24 13:53
// ����˵��:  @strԴ�ַ���
//            @lenΪ�´�����
//            @chFillΪ����ַ�
//            @bAddAtleftΪtrue��ʾ�������䣬false��ʾ�ұ����
// �� �� ֵ:  ���ַ���
//////////////////////////////////////////////////////////////////////////
SString SString::toFillString(SString str,int len,char chFill/*=' '*/,bool bAddAtLeft/*=true*/)
{
	SString s = str;
	while(s.m_iStrLen < len)
	{
		if(bAddAtLeft)
			s = SString::toString(&chFill,1) + s;
		else
			s += chFill;
	}
	return s;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ת��intΪ�ַ���
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-24 13:52
// ����˵��:  @vΪ��ֵ
//            @senΪ��������֧��10���ƺ�16����
// �� �� ֵ:  �µ��ַ���
//////////////////////////////////////////////////////////////////////////
SString SString::toString(int v,int sen/*=10*/)
{
	SString s;
	if(sen == 16)
		s.sprintf("%X",v);
	else
		s.sprintf("%d",v);
	return s;
}
////////////////////////////////////////////////////////////////////////
// ��    ��:  �ӻ����������ַ���������ȷ���ַ����������Ƿ��н�����ʱʹ�øú���ָ������
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-24 13:54
// ����˵��:  @pStrΪ�ַ�������
//            @iLengthΪ����
// �� �� ֵ:  �µ��ַ���
//////////////////////////////////////////////////////////////////////////
SString SString::toString(const char* pStr,int iLength)
{
	char *pBuf = new char[iLength+1];
	memset(pBuf,0,iLength+1);
	memcpy(pBuf,pStr,iLength);
	SString s = pBuf;
	delete[] pBuf;
	return s;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ת��ucharΪ�ַ���
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-24 13:52
// ����˵��:  @vΪ��ֵ
//            @senΪ��������֧��10���ƺ�16����
// �� �� ֵ:  �µ��ַ���
//////////////////////////////////////////////////////////////////////////
SString SString::toString(unsigned char v)
{
	SString s;
	s.sprintf("%d",v);
	return s;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ת��shortΪ�ַ���
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-24 13:52
// ����˵��:  @vΪ��ֵ
//            @senΪ��������֧��10���ƺ�16����
// �� �� ֵ:  �µ��ַ���
//////////////////////////////////////////////////////////////////////////
SString SString::toString(short v)
{
	SString s;
	s.sprintf("%d",v);
	return s;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ת��floatΪ�ַ���
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-24 13:52
// ����˵��:  @vΪ��ֵ
//            @senΪ��������֧��10���ƺ�16����
// �� �� ֵ:  �µ��ַ���
//////////////////////////////////////////////////////////////////////////
SString SString::toString(float v,int d/*=-1*/)
{
	SString s;
	if(d == -1)
		s.sprintf("%f",v);
	else if(d == 1)
		s.sprintf("%.1f",v);
	else if(d == 2)
		s.sprintf("%.2f",v);
	else if(d == 3)
		s.sprintf("%.3f",v);
	else if(d == 4)
		s.sprintf("%.4f",v);
	return s;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ת��doubleΪ�ַ���
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-24 13:52
// ����˵��:  @vΪ��ֵ
//            @senΪ��������֧��10���ƺ�16����
// �� �� ֵ:  �µ��ַ���
//////////////////////////////////////////////////////////////////////////
SString SString::toString(double v,int d/*=-1*/)
{
	SString s;
	if(d == -1)
		s.sprintf("%f",v);
	else
	{
		SString mask;
		mask.sprintf("%%.%df",d);
		s.sprintf(mask.m_pStr,v);
	}
	return s;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ת��ָ���ַ���Ϊint
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-24 13:52
// ����˵��:  @strΪָ�����ַ���
// �� �� ֵ:  int
//////////////////////////////////////////////////////////////////////////
int SString::toInt(SString str)
{
	return str.toInt();
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ת��ָ���ַ���Ϊfloat
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-24 13:52
// ����˵��:  @strΪָ�����ַ���
// �� �� ֵ:  float
//////////////////////////////////////////////////////////////////////////
float SString::toFloat(SString str)
{
	return str.toFloat();
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ת��ָ���ַ���Ϊdouble
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-24 13:52
// ����˵��:  @strΪָ�����ַ���
// �� �� ֵ:  double
//////////////////////////////////////////////////////////////////////////
double SString::toDouble(SString str)
{
	return str.toDouble();
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ���ֽ�������G�ֽڣ�ת��Ϊ�ַ���
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-24 13:57
// ����˵��:  @bytesΪ�ֽ���
//            @gbytesΪG�ֽ���(1024*1024*1024)
// �� �� ֵ:  ���ַ���
//////////////////////////////////////////////////////////////////////////
SString SString::toBytesString(int bytes,int gbytes/*=0*/)
{
	SString str;
	float gb = (float)gbytes;
	float b = (float)bytes;
	if(gb > 0)
	{
		if(gb >= 1024)
			str.sprintf("%0.2fT",gb/1024);
		else
			str.sprintf("%0.2fG",gb+b/1073741824);
	}
	else
	{
		if(b >= 1048576)
			str.sprintf("%0.2fM",b/1048576);
		else if(b >= 1024)
			str.sprintf("%0.2fK",b/1024);
		else
			str.sprintf("%d",bytes);
	}
	return str;
}


/************************************************************************/
/* ���Դ�                                                               */
/************************************************************************/

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ȡ�ַ�����ָ�����ַ�
// ��    ��:  �ۿ���
// ����ʱ��:  2012-3-14 12:54
// ����˵��:  @pSrcStrΪԴ�ַ�������ȡһ�κ��Զ�����ƶ���ֹͣ�ַ���֮��
//            @sStopCharΪֹͣ�ַ���
//            @sResultΪ����ַ���
// �� �� ֵ:  true��ʾ�ҵ���false��ʾ�Ҳ���
//////////////////////////////////////////////////////////////////////////
bool SString::ReadToChar(char* &pSrcStr,char *sStopChar,SString &sResult)
{
	char* str = strstr(pSrcStr,sStopChar);
	if(str == NULL)
	{
		if(*pSrcStr == '\0')
			return false;
		sResult.SetString(pSrcStr);
		pSrcStr += strlen(pSrcStr);
		return true;
	}
	sResult.SetString(pSrcStr,(int)(str-pSrcStr));
	pSrcStr = str + strlen(sStopChar);
	return true;
}


////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡָ���ָ��ַ����еĵ�index���Ӵ�
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-24 14:40
// ����˵��:  @indexΪ�Ӵ���ţ���1��ʼ���
//            @sAttrΪ�ַ���
//            @chSepΪ�ָ��ַ�
// �� �� ֵ:  �Ӵ�����
//////////////////////////////////////////////////////////////////////////
SString SString::GetIdAttribute(int index,SString &sAttr,SString chSep/*=";"*/)
{
	char* pStr = sAttr.data();
	for(int i=1;i<index;i++)
	{
		pStr = strstr(pStr,chSep.data());
		if(pStr == NULL)
			return "";
		pStr += chSep.length();
	}
	char* pStr2 = strstr(pStr,chSep.data());
	if(pStr2 == NULL)
		return sAttr.mid((int)(pStr-sAttr.data()));
	else
		return sAttr.mid((int)(pStr-sAttr.data()),(int)(pStr2-pStr));
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡָ���ָ��ַ����еĵ�index���Ӵ�,��ת��Ϊint����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-24 14:40
// ����˵��:  @indexΪ�Ӵ���ţ���1��ʼ���
//            @sAttrΪ�ַ���
//            @chSepΪ�ָ��ַ�
// �� �� ֵ:  int
//////////////////////////////////////////////////////////////////////////
int SString::GetIdAttributeI(int index,SString &sAttr,SString chSep/*=";"*/)
{
	return GetIdAttribute(index,sAttr,chSep).toInt();
}

#ifdef WIN32
__int64 SString::GetIdAttributeI64(int index,SString &sAttr,SString chSep)
{
	SString str = GetIdAttribute(index,sAttr,chSep);
	return _atoi64(str.data());
}
#else
long long SString::GetIdAttributeI64(int index,SString &sAttr,SString chSep)
{
	SString str = GetIdAttribute(index,sAttr,chSep);
	return atoll(str.data());
}
#endif

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡָ���ָ��ַ����еĵ�index���Ӵ�,��ת��Ϊfloat����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-24 14:40
// ����˵��:  @indexΪ�Ӵ���ţ���1��ʼ���
//            @sAttrΪ�ַ���
//            @chSepΪ�ָ��ַ�
// �� �� ֵ:  float
//////////////////////////////////////////////////////////////////////////
float SString::GetIdAttributeF(int index,SString &sAttr,SString chSep/*=";"*/)
{
	return GetIdAttribute(index,sAttr,chSep).toFloat();
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ���ַ����ĸ���
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-24 14:50
// ����˵��:  @sAttrΪ�ַ���
//            @chSepΪ�ָ��ַ�
// �� �� ֵ:  ����
//////////////////////////////////////////////////////////////////////////
int SString::GetAttributeCount(SString &sAttr,SString chSep/*=";"*/)
{
	int p=0,cnt=1,p1;
	while(1)
	{
		p1 = sAttr.find(chSep,p);
		if(p1 < 0)
			break;
		p = p1+chSep.m_iStrLen;
		cnt++;
	}
	return cnt;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ����ֵname1=value1;name2=value2;
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-24 14:50
// ����˵��:  @sAttrΪ�����ַ������磺name1=value1;name2=value2;
//            @sNameΪ��������
//            @chEqualΪ���ں��ַ���ȱʡΪ"=",Ҳ�����Ƕ���ַ�
//            @chSep�ָ������ݣ�ȱʡΪ";",Ҳ�����Ƕ���ַ�
// �� �� ֵ:  ����ֵ
//////////////////////////////////////////////////////////////////////////
SString SString::GetAttributeValue(SString &sAttr,SString sName,SString chEqual/*="="*/,SString chSep/*=";"*/)
{
	int iPos1,iPos2;
	iPos1 = sAttr.find(sName+chEqual);
	if(iPos1==0)
	{
		iPos1 += (int)(strlen(sName)+strlen(chEqual));
		iPos2 = sAttr.find(chSep,iPos1);
		if(iPos2 < 0)
		{
			return sAttr.mid(iPos1);
		}
		else
		{
			return sAttr.mid(iPos1,iPos2-iPos1);
		}
	}
	else
	{
		iPos1 = sAttr.find(chSep+sName+chEqual);
		if(iPos1 < 0)
			return "";
		iPos1 += (int)strlen(sName)+1+chSep.m_iStrLen;
		iPos2 = sAttr.find(chSep,iPos1);
		if(iPos2 < 0)
			return sAttr.mid(iPos1);
		else
			return sAttr.mid(iPos1,iPos2-iPos1);
	}
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ��������ֵname1=value1;name2=vale2;��������Դ������޸ģ������������
// ��    ��:  �ۿ���
// ����ʱ��:  2015-11-16 9:11
// ����˵��:  @sAttrΪ�����ַ������磺name1=value1;name2=value2;�����÷���
//            @sNameΪ��������
//         :  @sValueΪ�µ�����ֵ
//            @chEqualΪ���ں��ַ���ȱʡΪ"=",Ҳ�����Ƕ���ַ�
//            @chSep�ָ������ݣ�ȱʡΪ";",Ҳ�����Ƕ���ַ�
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SString::SetAttributeValue(SString &sAttr,SString sName,SString sValue,SString chEqual/*="="*/,SString chSep/*=";"*/)
{
	int p1;
	p1 = sAttr.find(sName+chEqual);
	bool bExist=false;
	if(p1==0)
	{
		bExist = true;
		p1 += sName.length()+chEqual.length();
	}
	else
	{
		p1 = sAttr.find(chSep+sName+chEqual);
		if(p1 >= 0)
		{
			bExist = true;
			p1 += chSep.length()+sName.length()+chEqual.length();
		}
	}
	if(bExist)
	{
		int p2 = sAttr.find(chSep,p1);
		if(p2 < 0)
			p2 = sAttr.length();
		sAttr = sAttr.left(p1)+sValue+sAttr.right(sAttr.length()-p2);
		if(sAttr.right(1) != chSep)
			sAttr += chSep;
	}
	else
	{
		if(sAttr.length() > 0 && sAttr.right(1) != chSep)
			sAttr += chSep;
		sAttr += sName+chEqual+sValue+chSep;
	}
}

void SString::SetAttributeValueI(SString &sAttr,SString sName,int iValue,SString chEqual/*="="*/,SString chSep/*=";"*/)
{
	SetAttributeValue(sAttr,sName,SString::toString(iValue),chEqual,chSep);
}

void SString::SetAttributeValueF(SString &sAttr,SString sName,float fValue,SString chEqual/*="="*/,SString chSep/*=";"*/)
{
	SetAttributeValue(sAttr,sName,SString::toString(fValue),chEqual,chSep);
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ǰ�������Ƿ����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-24 14:50
// ����˵��:  @sAttrΪ�����ַ������磺name1=value1;name2=value2;
//            @sNameΪ��������
//            @chEqualΪ���ں��ַ���ȱʡΪ"=",Ҳ�����Ƕ���ַ�
//            @chSep�ָ������ݣ�ȱʡΪ";",Ҳ�����Ƕ���ַ�
// �� �� ֵ:  ����ֵ
//////////////////////////////////////////////////////////////////////////
bool SString::IsAttribute(SString &sAttr,SString sName,SString chEqual/*="="*/,SString chSep/*=";"*/)
{
	int iPos1;
	iPos1 = sAttr.find(sName+chEqual);
	if(iPos1==0)
		return true;
	else
	{
		iPos1 = sAttr.find(chSep+sName+chEqual);
		if(iPos1 < 0)
			return false;
		return true;
	}
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ����ֵ��ת��Ϊint
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-24 14:50
// ����˵��:  @sAttrΪ�����ַ������磺name1=value1;name2=value2;
//            @sNameΪ��������
//            @chEqualΪ���ں��ַ���ȱʡΪ"=",Ҳ�����Ƕ���ַ�
//            @chSep�ָ������ݣ�ȱʡΪ";",Ҳ�����Ƕ���ַ�
// �� �� ֵ:  int
//////////////////////////////////////////////////////////////////////////
int SString::GetAttributeValueI(SString &sAttr,SString sName,SString chEqual/*="="*/,SString chSep/*=";"*/)
{
	return GetAttributeValue(sAttr,sName,chEqual,chSep).toInt();
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ����ֵ��ת����float
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-24 14:50
// ����˵��:  @sAttrΪ�����ַ������磺name1=value1;name2=value2;
//            @sNameΪ��������
//            @chEqualΪ���ں��ַ���ȱʡΪ"=",Ҳ�����Ƕ���ַ�
//            @chSep�ָ������ݣ�ȱʡΪ";",Ҳ�����Ƕ���ַ�
// �� �� ֵ:  float
//////////////////////////////////////////////////////////////////////////
float SString::GetAttributeValueF(SString &sAttr,SString sName,SString chEqual/*="="*/,SString chSep/*=";"*/)
{
	return GetAttributeValue(sAttr,sName,chEqual,chSep).toFloat();
}

#ifndef WIN32
int StringCharsetConvert2(const char* from_charset,const char* to_charset,char* inbuf,int inlen,char* outbuf,int outlen)
{
	//printf("into StringCharsetConvert iconv_open(%s,%s)\n",to_charset,from_charset);
	iconv_t cd = iconv_open(to_charset,from_charset);
	if(cd == (iconv_t)-1)
	{
		printf("iconv_open error=%d\n",errno);
		return -1;
	}
	char *inbuf2 = inbuf;
	char* outbuf2 = outbuf;
	size_t inlen2 = inlen;
	size_t outlen2 = outlen;

	//char *outbuf0 = outbuf;
	char **pin = &inbuf2;
	char **pout = &outbuf2;
	memset(outbuf,0,outlen);
	//printf("StringCharsetConvert cd=%d,inlen=%d,outlen=%ld,in=%s,out=%s\n",(long)cd,inlen,outlen,inbuf,outbuf);
	//printf("begin iconv...\n");
	int ret = iconv(cd,pin,(size_t *)&inlen2,pout,(size_t *)&outlen2);
	if(ret == -1)
	{
		printf("StringCharsetConvert iconv error! errno=%d, src=%s, desc=%s\n",errno,inbuf,outbuf);
		iconv_close(cd);
		return -2;
	}
	//printf("StringCharsetConvert iconv ret=%d,errno=%d,inlen=%d,outlen=%d,in=%s,out=%s\n",ret,errno,inlen,outlen,inbuf,outbuf0);
	iconv_close(cd);
	return 0;
}
#endif

////////////////////////////////////////////////////////////////////////
// ��    ��:  ����ǰ�ַ���������GB�ַ���ת��ΪUTF-8����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-24 14:10
// ����˵��:  @pInΪԭ�ַ�
// �� �� ֵ:  ���ַ���
//////////////////////////////////////////////////////////////////////////
SString SString::Gb2Utf8(char *pIn)
{
	SString sRet;	
#ifdef WIN32
     WCHAR *strSrc;
     TCHAR *szRes;

     //�����ʱ�����Ĵ�С
     int i = MultiByteToWideChar(CP_ACP, 0, pIn, -1, NULL, 0);
     strSrc = new WCHAR[i+1];
     MultiByteToWideChar(CP_ACP, 0, pIn, -1, strSrc, i);

     //�����ʱ�����Ĵ�С
     i = WideCharToMultiByte(CP_UTF8, 0, strSrc, -1, NULL, 0, NULL, NULL);
     szRes = new TCHAR[i+1];
#ifdef UNICODE
     WideCharToMultiByte(CP_UTF8, 0, strSrc, -1, (LPSTR)szRes, i, NULL, NULL);
#else
	 WideCharToMultiByte(CP_UTF8, 0, strSrc, -1, szRes, i, NULL, NULL);
#endif
     sRet = (char*)szRes;
     delete []strSrc;
     delete []szRes;
#else
	int iInLen = strlen(pIn);
	int iOutLen = strlen(pIn)*2+1;
	char *pOut = new char[iOutLen];
	pOut[0] = '\0';
	StringCharsetConvert2("gb18030//IGNORE","utf-8//IGNORE",pIn,iInLen,pOut,iOutLen);
	//CodeConverter cc("gb2312//IGNORE","utf-8//IGNORE");
	//CodeConverter cc("gb18030//IGNORE","utf-8//IGNORE");
	//cc.convert(pIn,iInLen,pOut,iOutLen);
	/*
	printf("pIn=%s, iInlen=%d, iOutLen=%d\n",pIn,iInLen,iOutLen);
	iconv_t cd;
	cd = iconv_open("utf-8","gb2312");
	printf("cd=%X\n",(int)cd);
	char **pin = &pIn;
	char **pout = &pOut;	
	memset(pOut,0,iOutLen);
	iconv(cd,pin,(size_t *)&iInLen,pout,(size_t *)&iOutLen);
	printf("pOut=%s\n",pOut);
	iconv_close(cd);
	*/
	sRet = pOut;
//	printf("sRet=%s\n",sRet.data());
//	printf("&pOut=%X\n",pOut);
	delete[] pOut;
#endif

	return sRet;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ָ�����ַ���������ת��UTF-8����ΪGB����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-24 14:34
// ����˵��:  @pInΪԭ�ַ�
//            @iSizeΪ����
// �� �� ֵ:  true��ʾת���ɹ���false��ʾʧ��
//////////////////////////////////////////////////////////////////////////
bool SString::Utf82Gb(char *pIn,int iSize)
{
#ifdef WIN32
     WCHAR *strSrc;// = (TCHAR*)pOut;
     TCHAR *szRes = (TCHAR*)pIn;

     //�����ʱ�����Ĵ�С
     int i = MultiByteToWideChar(CP_UTF8, 0, pIn, -1, NULL, 0);
     strSrc = new WCHAR[i+1];
	 memset(strSrc,0,sizeof(WCHAR)*(i+1));
     int ret = MultiByteToWideChar(CP_UTF8, 0, pIn, -1, strSrc, i);

     //�����ʱ�����Ĵ�С
     i = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
     //szRes = new TCHAR[i+1];
	 //memset(szRes,0,sizeof(TCHAR)*(i+1));
	 memset(pIn,0,iSize);
#ifdef UNICODE
	 ret = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, (LPSTR)szRes, i, NULL, NULL);
#else
	 ret = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szRes, i, NULL, NULL);
#endif

     delete []strSrc;
#else
	int iInLen = strlen(pIn);
	int iOutLen = strlen(pIn)/**2*/+1;
	char *pOut = new char[iOutLen];
	pOut[0] = '\0';
	StringCharsetConvert2("utf-8//IGNORE","gb18030//IGNORE",pIn,iInLen,pOut,iOutLen);
	//CodeConverter cc("utf-8//IGNORE","gb18030//IGNORE");
	//cc.convert(pIn,iInLen,pOut,iOutLen);
	memset(pIn,0,iSize);
	if(iOutLen > iSize - 1)
		iOutLen = iSize - 1;
	memcpy(pIn,pOut,iOutLen);
	delete[] pOut;
#endif

	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ��UTF16�ַ�ת��ΪUTF8
// ��    ��:  �ۿ���
// ����ʱ��:  2012-12-19 13:41
// ����˵��:  @pUTF16StartԴUTF16��ʼλ��,
//            @pUTF16EndԴUTF16����λ��,
//            @pUTF8StartĿ��UTF8��������ʼλ�ã�
//            @pUTF8EndĿ��UTF8����������λ�ã�UTF������ӦΪUTF16��������С������
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SString::Utf16_to_Utf8(UTF16* pUTF16Start,UTF16* pUTF16End,UTF8* pUTF8Start,UTF8* pUTF8End)
{
	UTF16* pTempUTF16 = pUTF16Start;
	UTF8* pTempUTF8 = pUTF8Start;
	while(pTempUTF16 <= pUTF16End)
	{
		if( *pTempUTF16 <= UTF8_ONE_END && pTempUTF8+1 < pUTF8End)
		{
			*pTempUTF8++=(UTF8)*pTempUTF16;
		}
		else if(*pTempUTF16 >= UTF8_TWO_START && *pTempUTF16 <= UTF8_TWO_END && pTempUTF8+2 < pUTF8End)
		{
			*pTempUTF8 ++=(*pTempUTF16>>6)|0xC0;
			*pTempUTF8 ++=(*pTempUTF16 & 0x3f) | 0x80;
		}
		else if(*pTempUTF16 >= UTF8_THERE_START && *pTempUTF16 <= UTF8_THERE_END && pTempUTF8+3 < pUTF8End)
		{
			*pTempUTF8++=(*pTempUTF16>>12)|0xE0;
			*pTempUTF8++=((*pTempUTF16>>6)&0x3f)|0x80;
			*pTempUTF8++=(*pTempUTF16&0x3f)|0x80;
		}
		else
			break;
		pTempUTF16++;
	}
	*pTempUTF8=0;
}

SString SString::Utf82Gb(char *pIn)
{
	SString sRet;

#ifdef WIN32
     WCHAR *strSrc;
     TCHAR *szRes;

     //�����ʱ�����Ĵ�С
     int i = MultiByteToWideChar(CP_UTF8, 0, pIn, -1, NULL, 0);
     strSrc = new WCHAR[i+1];
	 memset(strSrc,0,sizeof(WCHAR)*(i+1));
     MultiByteToWideChar(CP_UTF8, 0, pIn, -1, strSrc, i);

     //�����ʱ�����Ĵ�С
     i = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
     szRes = (TCHAR*)new char[i+1];
	 memset(szRes,0,sizeof(char)*(i+1));
#ifdef UNICODE
	 WideCharToMultiByte(CP_ACP, 0, strSrc, -1, (LPSTR)szRes, i, NULL, NULL);
#else
	 WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szRes, i, NULL, NULL);
#endif

     sRet = (char*)szRes;
     delete []strSrc;
     delete []szRes;
#else
	int iInLen = strlen(pIn);
	int iOutLen = strlen(pIn)*2+1;
//	printf("pIn=%s, iInlen=%d, iOutLen=%d\n",pIn,iInLen,iOutLen);

	char *pOut = new char[iOutLen];
	memset(pOut,0,iOutLen);
	pOut[0] = '\0';

	
	//CodeConverter cc("utf-8//IGNORE","gb2312");
	//CodeConverter cc("utf-8//IGNORE","gb2312//IGNORE");
	StringCharsetConvert2("utf-8//IGNORE","gb18030//IGNORE",pIn,iInLen,pOut,iOutLen);
	//CodeConverter cc("utf-8//IGNORE","gb18030//IGNORE");
	//cc.convert(pIn,iInLen,pOut,iOutLen);

	/*
	iconv_t cd;
	cd = iconv_open("utf-8","gb2312");
	char **pin = &pIn;
	char **pout = &pOut;	
	memset(pOut,0,iOutLen);
	iconv(cd,pin,(size_t *)&iInLen,pout,(size_t *)&iOutLen);
	iconv_close(cd);
	printf("pOut=%s\n",pOut);
	*/
	sRet = pOut;
//	printf("Utf82Gb::sRet=%s\n",sRet.data());
//	printf("Utf82Gb::sRet.right(100)=%s,length=%d\n",sRet.right(100).data(),sRet.length());
//	printf("&pOut=%X\n",pOut);

	delete[] pOut;
#endif

	return sRet;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ǰ�ַ���ת��Ϊ�µ�UTF-8������ַ���
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-24 14:35
// ����˵��:  void
// �� �� ֵ:  ���ַ���
//////////////////////////////////////////////////////////////////////////
SString SString::toUtf8()
{
	return Gb2Utf8(m_pStr);
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ǰ�ַ���ת��Ϊ�µ�GB������ַ���
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-24 14:35
// ����˵��:  void
// �� �� ֵ:  ���ַ���
//////////////////////////////////////////////////////////////////////////
SString SString::toGb2312()
{
	return Utf82Gb(m_pStr);
}
