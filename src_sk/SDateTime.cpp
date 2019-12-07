/**
*
* �� �� �� : SDateTime.cpp
* �������� : 2007-01-01
* ��    �� : �ۿ���(skt001@163.com)
* �޸����� : $Date: 2011/08/31 08:43:33 $
* ��ǰ�汾 : $Revision: 1.1 $
* �������� : ��SDateTime����
* �޸ļ�¼ : 
*            $Log: SDateTime.cpp,v $
*            Revision 1.1  2011/08/31 08:43:33  shaokt
*            *** empty log message ***
*
*            Revision 1.3  2011/04/19 09:10:58  shaokt
*            *** empty log message ***
*
*            Revision 1.2  2011/03/23 01:07:49  shaokt
*            *** empty log message ***
*
*            Revision 1.1  2011/02/25 03:12:07  shaokt
*            *** empty log message ***
*
*            Revision 1.1  2010/08/09 03:04:43  shaokt
*            *** empty log message ***
*
*            Revision 1.1  2009/09/15 02:14:07  shaokt
*            ͨѸ��¼����װ����Ŀ�״��ύ
*
*            Revision 1.7  2009/01/31 06:37:33  shaokt
*            *** empty log message ***
*
*            Revision 1.6  2009/01/06 09:58:18  shaokt
*            *** empty log message ***
*
*            Revision 1.5  2008/12/22 08:54:13  shaokt
*            *** empty log message ***
*
*            Revision 1.4  2008/09/16 05:32:17  shaokt
*            *** empty log message ***
*
*            Revision 1.3  2008/09/12 05:04:48  shaokt
*            *** empty log message ***
*
*            Revision 1.2  2008/09/11 05:17:25  shaokt
*            *** empty log message ***
*
*            Revision 1.1  2008/09/01 11:38:28  shaokt
*            *** empty log message ***
*
*            Revision 1.1  2008/09/01 08:58:00  shaokt
*            *** empty log message ***
*
*
**/

#include "SDateTime.h"
SDate::SDate()
{
	SKTOBJECT("SDate");
	m_year=m_month=m_day=m_wday=0;
}
SDate::SDate(int y,int m,int d)
{
	SKTOBJECT("SDate");
	m_year = y;
	m_month = m;
	m_day = d;
	m_wday = 0;
}
SDate::~SDate()
{
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ��
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:09
// ����˵��:  void
// �� �� ֵ:  ��
//////////////////////////////////////////////////////////////////////////
int SDate::year()		{return m_year;};

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ��
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:09
// ����˵��:  void
// �� �� ֵ:  ��
//////////////////////////////////////////////////////////////////////////
int SDate::month()		{return m_month;};

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ��
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:09
// ����˵��:  void
// �� �� ֵ:  ��
//////////////////////////////////////////////////////////////////////////
int SDate::day()		{return m_day;};

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ���ڼ�0~6��ʾ������~������
// ��    ��:  �ۿ���
// ����ʱ��:  2015-12-15 15:34
// ����˵��:  void
// �� �� ֵ:  int
//////////////////////////////////////////////////////////////////////////
int SDate::wday()		{return m_wday;};

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ֵ���������
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:09
// ����˵��:  @d�ο�ֵ
// �� �� ֵ:  this
//////////////////////////////////////////////////////////////////////////
SDate& SDate::operator=(const SDate &d)
{
	memcpy(this, &d, sizeof(d));return *this;
};

////////////////////////////////////////////////////////////////////////
// ��    ��:  ���ڱȽ����������
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:10
// ����˵��:  @other��ʾ�ο�ֵ
// �� �� ֵ:  true��ʾ��ο�ֵ��ȣ�false��ʾ��ο�ֵ����
//////////////////////////////////////////////////////////////////////////
bool SDate::operator==(SDate &other)
{
	if(m_year == other.m_year &&
		m_month == other.m_month &&
		m_day == other.m_day)
		return true;
	else
		return false;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  �����ڱȽ����������
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:11
// ����˵��:  @other��ʾ�ο�ֵ
// �� �� ֵ:  true��ʾ��ο�ֵ���ȣ�false��ʾ�����ֵ���
//////////////////////////////////////////////////////////////////////////
bool SDate::operator!=(SDate &other)
{
	if(m_year == other.m_year &&
		m_month == other.m_month &&
		m_day == other.m_day)
		return false;
	else
		return true;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ��ǰϵͳ����,��̬����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:12
// ����˵��:  void
// �� �� ֵ:  SDate
//////////////////////////////////////////////////////////////////////////
SDate SDate::currentDate()
{
	SDate d;
	time_t now;
	time(&now);
	struct  tm t;
#ifdef WIN32
	localtime_s(&t,&now);
#else
	localtime_r(&now,&t);
#endif
	//struct  tm *t;
	//t = localtime(&now);
	d.m_year = 1900+t.tm_year;
	d.m_month=1+t.tm_mon;
	d.m_day=t.tm_mday;
	d.m_wday=t.tm_wday;
	return d;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ʽ������Ϊһ���ַ���
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:12
// ����˵��:  @fmt��ʾ��ʽ���ַ����ĸ�ʽ������yyyy��ʾ��,MM��ʾ��,dd��ʾ��
// �� �� ֵ:  �ַ�����ʽ������
//////////////////////////////////////////////////////////////////////////
SString SDate::toString(SString fmt)//yyyyMMdd
{
	SString s=fmt;
	s.replace("yyyy",toStr("yyyy"));
	s.replace("MM",toStr("MM"));
	s.replace("dd",toStr("dd"));
	return s;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ת��ָ�����ֶ�
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:14
// ����˵��:  @fmt����ѡ��yyyy��MM��dd�ֱ��Ӧ�ꡢ����
// �� �� ֵ:  ��ʽ������ַ���
//////////////////////////////////////////////////////////////////////////
SString SDate::toStr(SString fmt)
{
	int len=2;
	int i=0;
	if(fmt == "yyyy")
	{
		len=4;
		i=m_year;
	}
	else if(fmt == "MM")
		i=m_month;
	else if(fmt == "dd")
		i=m_day;
	fmt.sprintf("%d",i);
	while(fmt.length() < len)
		fmt = "0"+fmt;
	return fmt;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  STime
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:21
// ��    ��:  ʱ����
//////////////////////////////////////////////////////////////////////////
STime::STime()
{
	SKTOBJECT("STime");
	m_hour=m_minute=m_second=m_msecond=0;
}
STime::STime(int h,int m,int s/*=0*/, int ms/*=0*/)
{
	SKTOBJECT("STime");
	m_hour=h;
	m_minute=m;
	m_second=s;
	m_msecond=ms;
}
STime::STime(time_t* pt)
{
	SKTOBJECT("STime");
	//tm *t;
	//t = localtime(pt);
	struct  tm t;
#ifdef WIN32
	localtime_s(&t,pt);
#else
	localtime_r(pt,&t);
#endif
	m_hour=t.tm_hour;
	m_minute=t.tm_min;
	m_second=t.tm_sec;
	m_msecond=0;
}
STime::~STime()
{
};

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡʱ
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:23
// ����˵��:  void
// �� �� ֵ:  ʱ
//////////////////////////////////////////////////////////////////////////
int STime::hour()	{return m_hour;}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ��
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:23
// ����˵��:  void
// �� �� ֵ:  ��
//////////////////////////////////////////////////////////////////////////
int STime::minute(){return m_minute;}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ��
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:23
// ����˵��:  void
// �� �� ֵ:  ��
//////////////////////////////////////////////////////////////////////////
int STime::second(){return m_second;}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:23
// ����˵��:  void
// �� �� ֵ:  ��
//////////////////////////////////////////////////////////////////////////
int STime::msec()	{return m_msecond;}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ΢��
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:23
// ����˵��:  void
// �� �� ֵ:  ΢��
//////////////////////////////////////////////////////////////////////////
int STime::usec()	{return m_usecond;}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ֵ���������
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:25
// ����˵��:  @t�ο�ʱ�����
// �� �� ֵ:  this
//////////////////////////////////////////////////////////////////////////
STime& STime::operator=(const STime &t)
{
	memcpy(this, &t, sizeof(t));return *this;
};

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ��ǰʱ�䣬��̬����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:25
// ����˵��:  void
// �� �� ֵ:  STime
//////////////////////////////////////////////////////////////////////////
STime STime::currentTime()
{
	STime tt;
	time_t now;
	time(&now);
	struct  tm t;
#ifdef WIN32
	localtime_s(&t,&now);
#else
	localtime_r(&now,&t);
#endif
	//t = localtime(&now);
// 	tt.m_hour=t->tm_hour;
// 	tt.m_minute=t->tm_min;
// 	tt.m_second=t->tm_sec;
	tt.m_hour=t.tm_hour;
	tt.m_minute=t.tm_min;
	tt.m_second=t.tm_sec;

	#ifdef WIN32
		SYSTEMTIME st;
		GetLocalTime(&st); 
		tt.m_msecond=st.wMilliseconds;
	#else
		timeval tv;
		memset(&tv,0,sizeof(tv));
		gettimeofday(&tv,NULL);
		tt.m_msecond = tv.tv_usec/1000;
	#endif
	return tt;
}


//////////////////////////////////////////////////////////////////////////
// ��    ��:  SDateTime
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:49
// ��    ��:  ����ʱ����
//////////////////////////////////////////////////////////////////////////
SDateTime::SDateTime()
{
	SKTOBJECT("SDateTime");
};
SDateTime::SDateTime(const SDate &d)
{
	SKTOBJECT("SDateTime");
	m_date = d;
}
SDateTime::SDateTime(const SDate &d, const STime &t)
{
	SKTOBJECT("SDateTime");
	m_date = d;
	m_time = t;
}
SDateTime::SDateTime(const SDateTime &dt)
{
	SKTOBJECT("SDateTime");
	*this = dt;
}
SDateTime::SDateTime(time_t time)
{
	SKTOBJECT("SDateTime");
	struct  tm t;
#ifdef WIN32
	localtime_s(&t,&time);
#else
	localtime_r(&time,&t);
#endif
// 	struct  tm *t;
// 	t = localtime(&time);
// 	if(t == NULL)
// 		return;
	m_t = time;
	m_date.m_year = 1900+t.tm_year;
	m_date.m_month=1+t.tm_mon;
	m_date.m_day=t.tm_mday;
	m_date.m_wday=t.tm_wday;
	m_time.m_hour=t.tm_hour;
	m_time.m_minute=t.tm_min;
	m_time.m_second=t.tm_sec;
	m_time.m_msecond=0;
	m_time.m_usecond=0;
}
	
////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ǰ����ʱ�����ָ������
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:28
// ����˵��:  @days��ʾ����������Ϊ����
// �� �� ֵ:  ��Ӻ���¶���
//////////////////////////////////////////////////////////////////////////
SDateTime SDateTime::addDays(int days)
{
	return addHours(days*24);
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ǰ����ʱ�����ָ��Сʱ
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:28
// ����˵��:  @hours��ʾСʱ������Ϊ����
// �� �� ֵ:  ��Ӻ���¶���
//////////////////////////////////////////////////////////////////////////
SDateTime SDateTime::addHours(int hours)
{
	return addMinutes(hours*60);
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ǰ����ʱ�����ָ������
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:28
// ����˵��:  @minutes��ʾ���ӣ�����Ϊ����
// �� �� ֵ:  ��Ӻ���¶���
//////////////////////////////////////////////////////////////////////////
SDateTime SDateTime::addMinutes(int minutes)
{
	return addSecs(minutes*60);
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ǰ����ʱ�����ָ����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:28
// ����˵��:  @secs��ʾ�룬����Ϊ����
// �� �� ֵ:  ��Ӻ���¶���
//////////////////////////////////////////////////////////////////////////
SDateTime SDateTime::addSecs(int secs)
{
	SDateTime dt=*this;
	struct tm timestart;
	memset(&timestart,0,sizeof(timestart));
	timestart.tm_sec	= dt.m_time.second();
	timestart.tm_min	= dt.m_time.minute();
	timestart.tm_hour = dt.m_time.hour();
	timestart.tm_mday = dt.m_date.day();
	timestart.tm_mon	= dt.m_date.month()-1;
	timestart.tm_year = dt.m_date.year()-1900;
	
	time_t time = mktime(&timestart)+secs;
	struct  tm t;
#ifdef WIN32
	localtime_s(&t,&time);
#else
	localtime_r(&time,&t);
#endif
// 	struct tm* t = localtime(&time);
// 	if(t == NULL)//TODO 
// 		return dt;

	dt.m_t = time;
	dt.m_date.m_year = 1900+t.tm_year;
	dt.m_date.m_month=1+t.tm_mon;
	dt.m_date.m_day=t.tm_mday;
	dt.m_date.m_wday=t.tm_wday;
	dt.m_time.m_hour=t.tm_hour;
	dt.m_time.m_minute=t.tm_min;
	dt.m_time.m_second=t.tm_sec;
	
	return dt;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ǰ����ʱ�����ָ������
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:28
// ����˵��:  @minutes��ʾ���룬����Ϊ����
// �� �� ֵ:  ��Ӻ���¶���
//////////////////////////////////////////////////////////////////////////
SDateTime SDateTime::addMSecs(int msecs)
{
	SDateTime dt=*this;
	dt.m_time.m_msecond += msecs;
	if(dt.m_time.m_msecond>100)
	{
		int secadd = dt.m_time.m_msecond/1000;
		dt.m_time.m_msecond = dt.m_time.m_msecond%1000;
		dt = dt.addSecs(secadd);
	}
	else if(dt.m_time.m_msecond<0)
	{
		int secadd = 0;
		while(dt.m_time.m_msecond<0)
		{
			secadd --;
			dt.m_time.m_msecond += 1000;
		}
		dt = dt.addSecs(secadd);			
	}
	return dt;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ǰ����ʱ�����ָ��΢��
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:28
// ����˵��:  @usecs��ʾ΢�룬����Ϊ����
// �� �� ֵ:  ��Ӻ���¶���
//////////////////////////////////////////////////////////////////////////
SDateTime SDateTime::addUSecs(int usecs)
{
	SDateTime dt=*this;
	dt.m_time.m_usecond += usecs;
	if(dt.m_time.m_usecond>1000)
	{
		int msecadd = dt.m_time.m_usecond/1000;
		dt.m_time.m_usecond = dt.m_time.m_usecond%1000;
		dt = dt.addMSecs(msecadd);
	}
	else if(dt.m_time.m_usecond<0)
	{
		int msecadd = 0;
		while(dt.m_time.m_usecond<0)
		{
			msecadd --;
			dt.m_time.m_usecond += 1000;
		}
		dt = dt.addMSecs(msecadd);			
	}
	return dt;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ͨ����ʽ�ַ�����ʽ��ʱ�䣺yyyyMMddhhmmsszzz
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:31
// ����˵��:  @fmtΪ��ʽ����������ΪyyyyMMddhhmmsszzzuuu
// �� �� ֵ:  ��ʽ�����ʱ��
//////////////////////////////////////////////////////////////////////////
SString SDateTime::toString(SString fmt)//yyyyMMddhhmmsszzz
{
	SString s=fmt;
	s.replace("yyyy",toStr("yyyy"));
	s.replace("MM",toStr("MM"));
	s.replace("dd",toStr("dd"));
	s.replace("hh",toStr("hh"));
	s.replace("mm",toStr("mm"));
	s.replace("ss",toStr("ss"));
	s.replace("zzz",toStr("zzz"));
	s.replace("uuu",toStr("uuu"));
	return s;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  �ɸ�ʽ��ʱ��ת����yyyyMMddhhmmsszzz
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:33
// ����˵��:  @dtmaskΪ��ʽ����������ΪyyyyMMddhhmmsszzzuuu
//            @datetimeΪ�ַ�����ʾ��ʱ�䣬������@dtmask��ʽ��ȫһ��
// �� �� ֵ:  ���ɵ�ʱ�����SDateTime
//////////////////////////////////////////////////////////////////////////
SDateTime SDateTime::makeDateTime(SString dtmask,SString datetime)
{
	SDateTime dt;
	dt.m_date.m_year		= getDateTimeItemByFormatStr(dtmask,datetime,"yyyy");
	dt.m_date.m_month		= getDateTimeItemByFormatStr(dtmask,datetime,"MM");
	dt.m_date.m_day			= getDateTimeItemByFormatStr(dtmask,datetime,"dd");
	dt.m_time.m_hour		= getDateTimeItemByFormatStr(dtmask,datetime,"hh");
	dt.m_time.m_minute	= getDateTimeItemByFormatStr(dtmask,datetime,"mm");
	dt.m_time.m_second	= getDateTimeItemByFormatStr(dtmask,datetime,"ss");
	dt.m_time.m_msecond	= getDateTimeItemByFormatStr(dtmask,datetime,"zzz");
	dt.m_time.m_usecond	= getDateTimeItemByFormatStr(dtmask,datetime,"uuu");
	
	struct tm t;   
	memset(&t,0,sizeof(t));
	t.tm_year  = dt.year()-1900;
	t.tm_mon   = dt.month()-1;//��Ҫ��1
	t.tm_mday  = dt.day();
	t.tm_hour  = dt.hour();
	t.tm_min   = dt.minute();
	t.tm_sec   = dt.second();
	dt.m_t=mktime(&t);   
	return dt;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��������ʱ���������SDateTime����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:35
// ����˵��:  @y/M/d/h/m/s/ms/us��ʾ������ʱ�������΢��
// �� �� ֵ:  ���ɵ�ʱ�����SDateTime
//////////////////////////////////////////////////////////////////////////
SDateTime SDateTime::makeDateTime(int y,int M,int d,int h,int m,int s,int ms/*=0*/,int us/*=0*/)
{
	SDateTime dt;
	dt.m_date.m_year	= y;
	dt.m_date.m_month	= M;
	dt.m_date.m_day		= d;
	dt.m_time.m_hour	= h;
	dt.m_time.m_minute	= m;
	dt.m_time.m_second	= s;
	dt.m_time.m_msecond	= ms;
	dt.m_time.m_usecond	= us;
	
	struct tm t;
	memset(&t,0,sizeof(t));
	t.tm_year  = dt.year()-1900;
	t.tm_mon   = dt.month()-1;//��Ҫ��1
	t.tm_mday  = dt.day();
	t.tm_hour  = dt.hour();
	t.tm_min   = dt.minute();
	t.tm_sec   = dt.second();
	dt.m_t=mktime(&t);   
	return dt;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��time_t(��SOC)����SDateTime����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:36
// ����˵��:  @t��ʾԴSOCʱ��
// �� �� ֵ:  ���ɵ�ʱ�����SDateTime
//////////////////////////////////////////////////////////////////////////
SDateTime SDateTime::makeDateTime(time_t t,int ms,int us)
{
	SDateTime dt(t);
	dt.m_time.m_msecond = ms;
	dt.m_time.m_usecond = us;
	return dt;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ��ǰϵͳʱ��
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:37
// ����˵��:  void
// �� �� ֵ:  ���ɵ�ʱ�����SDateTime
//////////////////////////////////////////////////////////////////////////
SDateTime SDateTime::currentDateTime()
{
	SDateTime dt;
	time_t now;
	::time(&now);
	struct  tm t;
#ifdef WIN32
	localtime_s(&t,&now);
#else
	localtime_r(&now,&t);
#endif
// 	struct  tm *t;
// 	t = localtime(&now);
	dt.m_t = now;
	dt.m_date.m_year = 1900+t.tm_year;
	dt.m_date.m_month=1+t.tm_mon;
	dt.m_date.m_day=t.tm_mday;
	dt.m_date.m_wday=t.tm_wday;
	dt.m_time.m_hour=t.tm_hour;
	dt.m_time.m_minute=t.tm_min;
	dt.m_time.m_second=t.tm_sec;
	
	#ifdef WIN32
		SYSTEMTIME st;
		GetLocalTime(&st); 
		dt.m_time.m_msecond=st.wMilliseconds;
		dt.m_time.m_usecond=0;
	#else
		timeval tv;
		memset(&tv,0,sizeof(tv));
		gettimeofday(&tv,NULL);
		dt.m_time.m_msecond = tv.tv_usec/1000;
		dt.m_time.m_usecond = tv.tv_usec%1000;
	#endif

	return dt;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ��ǰϵͳʱ�䵽���͵�SOC��΢�����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:37
// ����˵��:  @soc���ڴ��SOC������
//            @usec���ڴ��΢��
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SDateTime::getSystemTime(int &soc,int &usec)
{
#ifdef WIN32
	soc = (int)::time(NULL);
	SYSTEMTIME st;
	GetLocalTime(&st); 
	usec = st.wMilliseconds*1000;
#else
// 	timeval tv;
// 	memset(&tv,0,sizeof(tv));
// 	gettimeofday(&tv,NULL);
// 	soc = tv.tv_sec;
// 	usec= tv.tv_usec;
	timespec ts;
	memset(&ts,0,sizeof(ts));
	clock_gettime(CLOCK_REALTIME,&ts);
	soc = ts.tv_sec;
	usec= ts.tv_nsec/1000;
#endif
}

void SDateTime::getSystemTimeNs(int &soc,int &nsec)
{
#ifdef WIN32
	soc = (int)::time(NULL);
	SYSTEMTIME st;
	GetLocalTime(&st); 
	nsec = st.wMilliseconds*1000000;
#else
// 	timeval tv;
// 	memset(&tv,0,sizeof(tv));
// 	gettimeofday(&tv,NULL);
// 	soc = tv.tv_sec;
// 	nsec= tv.tv_nsec;
	timespec ts;
	memset(&ts,0,sizeof(ts));
	clock_gettime(CLOCK_REALTIME,&ts);
	soc = ts.tv_sec;
	nsec= ts.tv_nsec;
#endif
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ��ǰϵͳʱ��SOC
// ��    ��:  �ۿ���
// ����ʱ��:  2014-12-4 11:08
// ����˵��:  void
// �� �� ֵ:  SOC������
//////////////////////////////////////////////////////////////////////////
time_t SDateTime::getNowSoc()
{
	return ::time(NULL);
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ���õ�ǰϵͳʱ��
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:38
// ����˵��:  @dt��ʾ׼�����õ���ʱ�䣨���ػ�ʱ������Ϣ)
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SDateTime::setSystemTime(SDateTime dt)
{
#ifdef WIN32
	SYSTEMTIME t;
	//dt = dt.addHours(-8);//����ʱ�������ʱ���ʱ����-8Сʱ
	//memset(&t,0,sizeof(t));
	t.wYear				= dt.year();
	t.wMonth			= dt.month();
	t.wDay				= dt.day();
	t.wHour				= dt.hour();
	t.wMinute			= dt.minute();
	t.wSecond			= dt.second();
	t.wDayOfWeek  = 0;
	t.wMilliseconds = dt.msec();
	SetLocalTime(&t); //��ϵͳ�����ػ�ʱ�䵽UTCʱ���ת��������ʱ��������ʱ
#else
	struct tm t;
	memset(&t,0,sizeof(t));
	time_t newtime;   
	t.tm_year  = dt.year()-1900;
	t.tm_mon   = dt.month()-1;//��ԭʱ��Ҫ��1
	t.tm_mday  = dt.day();
	t.tm_hour  = dt.hour();
	t.tm_min   = dt.minute();
	t.tm_sec   = dt.second();
	newtime=mktime(&t);   
	stime((long*)&newtime);
//#ifdef _POWERPC
	//PPCƽ̨��ʱ����Ҫ�ô�����ͬ����Ӳ��RTC��
	::system("hwclock -w");//--systohc
//#endif
#endif
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��this��ʱ�䵽other��ʱ�䣬�ж�����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:39
// ����˵��:  @other�ο�ʱ�����
// �� �� ֵ:  ��ʱ��thisСother��ʱΪ����������Ϊ��������ͬΪ0
//////////////////////////////////////////////////////////////////////////
int SDateTime::secsTo(SDateTime other)
{
	int secs=(int)difftime(other.m_t,m_t);
	return secs;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��this��ʱ�䵽other��ʱ�䣬�ж��ٺ���
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:40
// ����˵��:  @other�ο�ʱ�����
// �� �� ֵ:  ��ʱ��thisСother��ʱΪ����������Ϊ��������ͬΪ0
//////////////////////////////////////////////////////////////////////////
int SDateTime::msecsTo(SDateTime other)
{
	int msecs=(int)difftime(other.m_t,m_t) * 1000 + 
		(other.m_time.m_msecond - m_time.m_msecond);
	return msecs;
}


////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ֵ���������
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:58
// ����˵��:  @dt��ʾ�ο�ֵ
// �� �� ֵ:  this
//////////////////////////////////////////////////////////////////////////
SDateTime& SDateTime::operator=(const SDateTime &dt)
{
	m_date = dt.m_date;
	m_time = dt.m_time;
	m_t = dt.m_t;
	return *this;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  С�ڱȽ����������
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:41
// ����˵��:  @otherΪ�ο�����
// �� �� ֵ:  this<otherʱ����true�����򷵻�false
//////////////////////////////////////////////////////////////////////////
bool SDateTime::operator<(SDateTime &other)
{
	return secsTo(other)>0;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ���ڱȽ����������
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:41
// ����˵��:  @otherΪ�ο�����
// �� �� ֵ:  this>otherʱ����true�����򷵻�false
//////////////////////////////////////////////////////////////////////////
bool SDateTime::operator>(SDateTime &other)
{
	return secsTo(other)<0;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  С�ڵ��ڱȽ����������
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:41
// ����˵��:  @otherΪ�ο�����
// �� �� ֵ:  this<=otherʱ����true�����򷵻�false
//////////////////////////////////////////////////////////////////////////
bool SDateTime::operator<=(SDateTime &other)
{
	return secsTo(other)>=0;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ���ڵ��ڱȽ����������
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:41
// ����˵��:  @otherΪ�ο�����
// �� �� ֵ:  this>=otherʱ����true�����򷵻�false
//////////////////////////////////////////////////////////////////////////
bool SDateTime::operator>=(SDateTime &other)
{
	return secsTo(other)<=0;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ���ڱȽ����������
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:41
// ����˵��:  @otherΪ�ο�����
// �� �� ֵ:  this==otherʱ����true�����򷵻�false
//////////////////////////////////////////////////////////////////////////
bool SDateTime::operator==(SDateTime &other)
{
	return secsTo(other)==0;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  �����ڱȽ����������
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:41
// ����˵��:  @otherΪ�ο�����
// �� �� ֵ:  this!=otherʱ����true�����򷵻�false
//////////////////////////////////////////////////////////////////////////
bool SDateTime::operator!=(SDateTime &other)
{
	return secsTo(other)!=0;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  �������������
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:41
// ����˵��:  @otherΪ�ο�����
// �� �� ֵ:  ����this-other��ʱ�������
//////////////////////////////////////////////////////////////////////////
int SDateTime::operator-(SDateTime &other)
{
	return other.secsTo(*this);
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ���ڶ���
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:41
// ����˵��:  void
// �� �� ֵ:  SDate
//////////////////////////////////////////////////////////////////////////
SDate SDateTime::date(){return m_date;}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡʱ�����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:41
// ����˵��:  void
// �� �� ֵ:  STime
//////////////////////////////////////////////////////////////////////////
STime SDateTime::time(){return m_time;}


////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ��
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:41
// ����˵��:  void
// �� �� ֵ:  ��
//////////////////////////////////////////////////////////////////////////
int SDateTime::year()		{return m_date.year();};

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ��
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:41
// ����˵��:  void
// �� �� ֵ:  ��
//////////////////////////////////////////////////////////////////////////
int SDateTime::month()		{return m_date.month();};

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ��
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:41
// ����˵��:  void
// �� �� ֵ:  ��
//////////////////////////////////////////////////////////////////////////
int SDateTime::day()		{return m_date.day();};

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ���ڼ�0~6��ʾ������~������
// ��    ��:  �ۿ���
// ����ʱ��:  2015-12-15 15:34
// ����˵��:  void
// �� �� ֵ:  int
//////////////////////////////////////////////////////////////////////////
int SDateTime::wday()		{return m_date.wday();};

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡʱ
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:41
// ����˵��:  void
// �� �� ֵ:  ʱ
//////////////////////////////////////////////////////////////////////////
int SDateTime::hour()		{return m_time.hour();};

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ��
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:41
// ����˵��:  void
// �� �� ֵ:  ��
//////////////////////////////////////////////////////////////////////////
int SDateTime::minute()	{return m_time.minute();};

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ��
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:41
// ����˵��:  void
// �� �� ֵ:  ��
//////////////////////////////////////////////////////////////////////////
int SDateTime::second()	{return m_time.second();};

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:41
// ����˵��:  void
// �� �� ֵ:  ����
//////////////////////////////////////////////////////////////////////////
int SDateTime::msec()		{return m_time.msec();};

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ΢��
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:41
// ����˵��:  void
// �� �� ֵ:  ΢��
//////////////////////////////////////////////////////////////////////////
int SDateTime::usec()		{return m_time.m_usecond;};

////////////////////////////////////////////////////////////////////////
// ��    ��:  ���ɸ�ʽ���ֶ�
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:54
// ����˵��:  @fmt����Ϊ�����ֶε�����ַ�����yyyyMMddhhmmsszzzuuu��֮һ
// �� �� ֵ:  ��ʽ��Ϊ�ַ�����ָ��ʱ���ֶ�
//////////////////////////////////////////////////////////////////////////
SString SDateTime::toStr(SString fmt)
{
	int len=2;
	int i=0;
	if(fmt == "yyyy")
	{
		len=4;
		i=m_date.m_year;
	}
	else if(fmt == "MM")
		i=m_date.m_month;
	else if(fmt == "dd")
		i=m_date.m_day;
	else if(fmt == "hh")
		i=m_time.m_hour;
	else if(fmt == "mm")
		i=m_time.m_minute;
	else if(fmt == "ss")
		i=m_time.m_second;
	else if(fmt == "zzz")
	{
		len=3;
		i=m_time.m_msecond;
	}
	else if(fmt == "uuu")
	{
		len=3;
		i=m_time.m_usecond;
	}
	fmt.sprintf("%d",i);
	while(fmt.length() < len)
		fmt = "0"+fmt;
	return fmt;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  �Ӹ�����ʱ��͸�ʽ������ȡָ���ֶε�ֵ
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:56
// ����˵��:  @dtmask����Ϊ�����ֶε�����ַ����磺yyyyMMddhhmmsszzzuuu
//            @datetimeΪʵ��ʱ��ֵ
//            @fmtΪ��ȡ���ֶ�����ַ�����yyyyMMddhhmmsszzzuuu��֮һ
// �� �� ֵ:  int
//////////////////////////////////////////////////////////////////////////
int SDateTime::getDateTimeItemByFormatStr(SString &dtmask,SString &datetime,SString fmt)
{
	int p,l;
	p = dtmask.find(fmt);
	l = fmt.length();
	if(p<0)
		return 0;
	return datetime.mid(p,l).toInt();
}


