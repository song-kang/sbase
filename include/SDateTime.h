/**
*
* �� �� �� : SDateTime.h
* �������� : 2007-01-01
* ��    �� : �ۿ���(skt001@163.com)
* �޸����� : $Date: 2011/08/31 08:43:33 $
* ��ǰ�汾 : $Revision: 1.1 $
* �������� : ��QT������ʱ����
* �޸ļ�¼ : 
*            $Log: SDateTime.h,v $
*            Revision 1.1  2011/08/31 08:43:33  shaokt
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
*            Revision 1.6  2009/06/09 08:46:14  shaokt
*            *** empty log message ***
*
*            Revision 1.5  2009/01/31 06:37:34  shaokt
*            *** empty log message ***
*
*            Revision 1.4  2009/01/06 09:58:18  shaokt
*            *** empty log message ***
*
*            Revision 1.3  2008/12/22 08:54:13  shaokt
*            *** empty log message ***
*
*            Revision 1.2  2008/09/16 05:32:17  shaokt
*            *** empty log message ***
*
*            Revision 1.1  2008/09/01 11:38:28  shaokt
*            *** empty log message ***
*
*            Revision 1.1  2008/09/01 08:58:01  shaokt
*            *** empty log message ***
*
*            Revision 1.12  2008/02/18 01:07:55  shaokt
*            *** empty log message ***
*
*            Revision 1.11  2007/11/14 11:30:49  shaokt
*            WINDOWS���޸�ϵͳʱ��ģ�8Сʱʱ������
*
*            Revision 1.10  2007/10/29 07:30:23  shaokt
*            ���Ӷ����ַ�Ƿ�Ϊ4�ı����ļ�⺯����Ϊ���ARM������pack(1)��ɵ��ڴ�����
*
*            Revision 1.9  2007/10/19 06:17:09  shaokt
*            ����time_t�Ĺ��캯��
*
*            Revision 1.8  2007/10/15 08:54:54  shaokt
*            *** empty log message ***
*
*            Revision 1.7  2007/10/12 09:40:09  shaokt
*            *** empty log message ***
*
*            Revision 1.6  2007/10/11 13:16:46  shaokt
*            ��������ϵͳ��ǰʱ��ĺ���
*
*            Revision 1.5  2007/09/27 05:09:23  shaokt
*            �޸����Ӹ�����ʱ��BUG
*
*            Revision 1.4  2007/09/27 01:51:28  shaokt
*            ���Ӵ��ַ���ʽ��������ʱ��ĺ���
*
*            Revision 1.3  2007/09/26 02:54:37  shaokt
*            �޸�����N��ʱ��δ�������£���1970����1��������
*
*            Revision 1.2  2007/09/11 08:17:52  shaokt
*            �������
*
*            Revision 1.1  2007/08/15 03:09:22  shaokt
*            *** empty log message ***
*
**/

#ifndef __SKT_DATE_TIME_H__
#define __SKT_DATE_TIME_H__

#include <time.h>
#ifndef WIN32
#include <sys/time.h>
#endif
#include "SString.h"

class SDateTime;

//////////////////////////////////////////////////////////////////////////
// ��    ��:  SDate
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:08
// ��    ��:  ������
//////////////////////////////////////////////////////////////////////////
class SBASE_EXPORT SDate
{
public:
	SDate();
	SDate(int y,int m,int d);
	~SDate();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:09
	// ����˵��:  void
	// �� �� ֵ:  ��
	//////////////////////////////////////////////////////////////////////////
	int year();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:09
	// ����˵��:  void
	// �� �� ֵ:  ��
	//////////////////////////////////////////////////////////////////////////
	int month();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:09
	// ����˵��:  void
	// �� �� ֵ:  ��
	//////////////////////////////////////////////////////////////////////////
	int day();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ���ڼ�0~6��ʾ������~������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-12-15 15:34
	// ����˵��:  void
	// �� �� ֵ:  int
	//////////////////////////////////////////////////////////////////////////
	int wday();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ֵ���������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:09
	// ����˵��:  @d�ο�ֵ
	// �� �� ֵ:  this
	//////////////////////////////////////////////////////////////////////////
	SDate& operator=(const SDate &d);
	
	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���ڱȽ����������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:10
	// ����˵��:  @other��ʾ�ο�ֵ
	// �� �� ֵ:  true��ʾ��ο�ֵ��ȣ�false��ʾ��ο�ֵ����
	//////////////////////////////////////////////////////////////////////////
	bool operator==(SDate &other);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����ڱȽ����������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:11
	// ����˵��:  @other��ʾ�ο�ֵ
	// �� �� ֵ:  true��ʾ��ο�ֵ���ȣ�false��ʾ�����ֵ���
	//////////////////////////////////////////////////////////////////////////
	bool operator!=(SDate &other);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��ǰϵͳ����,��̬����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:12
	// ����˵��:  void
	// �� �� ֵ:  SDate
	//////////////////////////////////////////////////////////////////////////
	static SDate currentDate();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ʽ������Ϊһ���ַ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:12
	// ����˵��:  @fmt��ʾ��ʽ���ַ����ĸ�ʽ������yyyy��ʾ��,MM��ʾ��,dd��ʾ��
	// �� �� ֵ:  �ַ�����ʽ������
	//////////////////////////////////////////////////////////////////////////
	SString toString(SString fmt);//yyyyMMdd

	int m_year;//��
	int m_month;//��
	int m_day;//��
	int m_wday;//����
private:
	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ת��ָ�����ֶ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:14
	// ����˵��:  @fmt����ѡ��yyyy��MM��dd�ֱ��Ӧ�ꡢ����
	// �� �� ֵ:  ��ʽ������ַ���
	//////////////////////////////////////////////////////////////////////////
	SString toStr(SString fmt);

};
//typedef SDate QDate;

//////////////////////////////////////////////////////////////////////////
// ��    ��:  STime
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:21
// ��    ��:  ʱ����
//////////////////////////////////////////////////////////////////////////
class SBASE_EXPORT STime
{
public:
	STime();
	STime(int h,int m,int s=0, int ms=0);
	STime(time_t* pt);
	~STime();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡʱ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:23
	// ����˵��:  void
	// �� �� ֵ:  ʱ
	//////////////////////////////////////////////////////////////////////////
	int hour();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:23
	// ����˵��:  void
	// �� �� ֵ:  ��
	//////////////////////////////////////////////////////////////////////////
	int minute();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:23
	// ����˵��:  void
	// �� �� ֵ:  ��
	//////////////////////////////////////////////////////////////////////////
	int second();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:23
	// ����˵��:  void
	// �� �� ֵ:  ��
	//////////////////////////////////////////////////////////////////////////
	int msec();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ΢��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:23
	// ����˵��:  void
	// �� �� ֵ:  ΢��
	//////////////////////////////////////////////////////////////////////////
	int usec();
	
	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ֵ���������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:25
	// ����˵��:  @t�ο�ʱ�����
	// �� �� ֵ:  this
	//////////////////////////////////////////////////////////////////////////
	STime& operator=(const STime &t);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��ǰʱ�䣬��̬����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:25
	// ����˵��:  void
	// �� �� ֵ:  STime
	//////////////////////////////////////////////////////////////////////////
	static STime currentTime();
	
	int m_hour;//ʱ
	int m_minute;//��
	int m_second;//��
	int m_msecond;//����
	int m_usecond;//΢��
};
//typedef STime QTime;

//////////////////////////////////////////////////////////////////////////
// ��    ��:  SDateTime
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 15:49
// ��    ��:  ����ʱ����
//////////////////////////////////////////////////////////////////////////
class SBASE_EXPORT SDateTime
{
public:
	SDateTime();
	SDateTime(const SDate &d);
	SDateTime(const SDate &d, const STime &t);
	SDateTime(const SDateTime &dt);
	SDateTime(time_t time);
	
	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ǰ����ʱ������ָ������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:28
	// ����˵��:  @days��ʾ����������Ϊ����
	// �� �� ֵ:  ���Ӻ���¶���
	//////////////////////////////////////////////////////////////////////////
	SDateTime addDays(int days);
	
	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ǰ����ʱ������ָ��Сʱ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:28
	// ����˵��:  @hours��ʾСʱ������Ϊ����
	// �� �� ֵ:  ���Ӻ���¶���
	//////////////////////////////////////////////////////////////////////////
	SDateTime addHours(int hours);
	
	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ǰ����ʱ������ָ������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:28
	// ����˵��:  @minutes��ʾ���ӣ�����Ϊ����
	// �� �� ֵ:  ���Ӻ���¶���
	//////////////////////////////////////////////////////////////////////////
	SDateTime addMinutes(int minutes);
	
	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ǰ����ʱ������ָ����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:28
	// ����˵��:  @secs��ʾ�룬����Ϊ����
	// �� �� ֵ:  ���Ӻ���¶���
	//////////////////////////////////////////////////////////////////////////
	SDateTime addSecs(int secs);
	
	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ǰ����ʱ������ָ������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:28
	// ����˵��:  @minutes��ʾ���룬����Ϊ����
	// �� �� ֵ:  ���Ӻ���¶���
	//////////////////////////////////////////////////////////////////////////
	SDateTime addMSecs(int msecs);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ǰ����ʱ������ָ��΢��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:28
	// ����˵��:  @usecs��ʾ΢�룬����Ϊ����
	// �� �� ֵ:  ���Ӻ���¶���
	//////////////////////////////////////////////////////////////////////////
	SDateTime addUSecs(int usecs);



	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ͨ����ʽ�ַ�����ʽ��ʱ�䣺yyyyMMddhhmmsszzz
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:31
	// ����˵��:  @fmtΪ��ʽ����������ΪyyyyMMddhhmmsszzzuuu
	// �� �� ֵ:  ��ʽ�����ʱ��
	//////////////////////////////////////////////////////////////////////////
	SString toString(SString fmt);
	
	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �ɸ�ʽ��ʱ��ת����yyyyMMddhhmmsszzz
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:33
	// ����˵��:  @dtmaskΪ��ʽ����������ΪyyyyMMddhhmmsszzzuuu
	//            @datetimeΪ�ַ�����ʾ��ʱ�䣬������@dtmask��ʽ��ȫһ��
	// �� �� ֵ:  ���ɵ�ʱ�����SDateTime
	//////////////////////////////////////////////////////////////////////////
	static SDateTime makeDateTime(SString dtmask,SString datetime);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����SOC������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:34
	// ����˵��:  @y/M/d/h/m/s��ʾ������ʱ����
	// �� �� ֵ:  SOC
	//////////////////////////////////////////////////////////////////////////
	static inline int makeSOC(int y,int M,int d,int h,int m,int s)
	{
		struct tm t;
		memset(&t,0,sizeof(t));
		t.tm_year  = y-1900;
		t.tm_mon   = M-1;//��Ҫ��1
		t.tm_mday  = d;
		t.tm_hour  = h;
		t.tm_min   = m;
		t.tm_sec   = s;
		return (int)mktime(&t);   
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��������ʱ���������SDateTime����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:35
	// ����˵��:  @y/M/d/h/m/s/ms/us��ʾ������ʱ�������΢��
	// �� �� ֵ:  ���ɵ�ʱ�����SDateTime
	//////////////////////////////////////////////////////////////////////////
	static SDateTime makeDateTime(int y,int M,int d,int h,int m,int s,int ms=0,int us=0);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��time_t(��SOC)����SDateTime����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:36
	// ����˵��:  @t��ʾԴSOCʱ��
	// �� �� ֵ:  ���ɵ�ʱ�����SDateTime
	//////////////////////////////////////////////////////////////////////////
	static SDateTime makeDateTime(time_t t,int ms=0,int us=0);
	
	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��ǰϵͳʱ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:37
	// ����˵��:  void
	// �� �� ֵ:  ���ɵ�ʱ�����SDateTime
	//////////////////////////////////////////////////////////////////////////
	static SDateTime currentDateTime();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��ǰϵͳʱ�䵽���͵�SOC��΢�����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:37
	// ����˵��:  @soc���ڴ��SOC������
	//            @usec���ڴ��΢��
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	static void getSystemTime(int &soc,int &usec);
	static void getSystemTimeNs(int &soc,int &nsec);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��ǰϵͳʱ��SOC
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-12-4 11:08
	// ����˵��:  void
	// �� �� ֵ:  SOC������
	//////////////////////////////////////////////////////////////////////////
	static time_t getNowSoc();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���õ�ǰϵͳʱ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:38
	// ����˵��:  @dt��ʾ׼�����õ���ʱ��
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	static void setSystemTime(SDateTime dt);
	
	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��this��ʱ�䵽other��ʱ�䣬�ж�����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:39
	// ����˵��:  @other�ο�ʱ�����
	// �� �� ֵ:  ��ʱ��thisСother��ʱΪ����������Ϊ��������ͬΪ0
	//////////////////////////////////////////////////////////////////////////
	int secsTo(SDateTime other);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��this��ʱ�䵽other��ʱ�䣬�ж��ٺ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:40
	// ����˵��:  @other�ο�ʱ�����
	// �� �� ֵ:  ��ʱ��thisСother��ʱΪ����������Ϊ��������ͬΪ0
	//////////////////////////////////////////////////////////////////////////
	int msecsTo(SDateTime other);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ֵ���������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:58
	// ����˵��:  @dt��ʾ�ο�ֵ
	// �� �� ֵ:  this
	//////////////////////////////////////////////////////////////////////////
	SDateTime& operator=(const SDateTime &dt);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  С�ڱȽ����������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:41
	// ����˵��:  @otherΪ�ο�����
	// �� �� ֵ:  this<otherʱ����true�����򷵻�false
	//////////////////////////////////////////////////////////////////////////
	bool operator<(SDateTime &other);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���ڱȽ����������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:41
	// ����˵��:  @otherΪ�ο�����
	// �� �� ֵ:  this>otherʱ����true�����򷵻�false
	//////////////////////////////////////////////////////////////////////////
	bool operator>(SDateTime &other);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  С�ڵ��ڱȽ����������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:41
	// ����˵��:  @otherΪ�ο�����
	// �� �� ֵ:  this<=otherʱ����true�����򷵻�false
	//////////////////////////////////////////////////////////////////////////
	bool operator<=(SDateTime &other);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���ڵ��ڱȽ����������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:41
	// ����˵��:  @otherΪ�ο�����
	// �� �� ֵ:  this>=otherʱ����true�����򷵻�false
	//////////////////////////////////////////////////////////////////////////
	bool operator>=(SDateTime &other);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���ڱȽ����������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:41
	// ����˵��:  @otherΪ�ο�����
	// �� �� ֵ:  this==otherʱ����true�����򷵻�false
	//////////////////////////////////////////////////////////////////////////
	bool operator==(SDateTime &other);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����ڱȽ����������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:41
	// ����˵��:  @otherΪ�ο�����
	// �� �� ֵ:  this!=otherʱ����true�����򷵻�false
	//////////////////////////////////////////////////////////////////////////
	bool operator!=(SDateTime &other);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �������������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:41
	// ����˵��:  @otherΪ�ο�����
	// �� �� ֵ:  ����this-other��ʱ�������
	//////////////////////////////////////////////////////////////////////////
	int operator-(SDateTime &other);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ���ڶ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:41
	// ����˵��:  void
	// �� �� ֵ:  SDate
	//////////////////////////////////////////////////////////////////////////
	SDate date();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡʱ�����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:41
	// ����˵��:  void
	// �� �� ֵ:  STime
	//////////////////////////////////////////////////////////////////////////
	STime time();


	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:41
	// ����˵��:  void
	// �� �� ֵ:  ��
	//////////////////////////////////////////////////////////////////////////
	int year();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:41
	// ����˵��:  void
	// �� �� ֵ:  ��
	//////////////////////////////////////////////////////////////////////////
	int month();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:41
	// ����˵��:  void
	// �� �� ֵ:  ��
	//////////////////////////////////////////////////////////////////////////
	int day();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ���ڼ�0~6��ʾ������~������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-12-15 15:34
	// ����˵��:  void
	// �� �� ֵ:  int
	//////////////////////////////////////////////////////////////////////////
	int wday();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡʱ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:41
	// ����˵��:  void
	// �� �� ֵ:  ʱ
	//////////////////////////////////////////////////////////////////////////
	int hour();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:41
	// ����˵��:  void
	// �� �� ֵ:  ��
	//////////////////////////////////////////////////////////////////////////
	int minute();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:41
	// ����˵��:  void
	// �� �� ֵ:  ��
	//////////////////////////////////////////////////////////////////////////
	int second();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:41
	// ����˵��:  void
	// �� �� ֵ:  ����
	//////////////////////////////////////////////////////////////////////////
	int msec();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ΢��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:41
	// ����˵��:  void
	// �� �� ֵ:  ΢��
	//////////////////////////////////////////////////////////////////////////
	int usec();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ������SOC
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:41
	// ����˵��:  void
	// �� �� ֵ:  SOC
	//////////////////////////////////////////////////////////////////////////
	inline int soc()	{return (int)m_t;};

	
	SDate m_date;//���ڶ���
	STime m_time;//ʱ�����
	time_t m_t;//������SOC
private:
	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���ɸ�ʽ���ֶ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:54
	// ����˵��:  @fmt����Ϊ�����ֶε�����ַ�����yyyyMMddhhmmsszzzuuu��֮һ
	// �� �� ֵ:  ��ʽ��Ϊ�ַ�����ָ��ʱ���ֶ�
	//////////////////////////////////////////////////////////////////////////
	SString toStr(SString fmt);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �Ӹ�����ʱ��͸�ʽ������ȡָ���ֶε�ֵ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:56
	// ����˵��:  @dtmask����Ϊ�����ֶε�����ַ����磺yyyyMMddhhmmsszzzuuu
	//            @datetimeΪʵ��ʱ��ֵ
	//            @fmtΪ��ȡ���ֶ�����ַ�����yyyyMMddhhmmsszzzuuu��֮һ
	// �� �� ֵ:  int
	//////////////////////////////////////////////////////////////////////////
	static int getDateTimeItemByFormatStr(SString &dtmask,SString &datetime,SString fmt);
};
//typedef SDateTime QDateTime;

#endif