/**
 *
 * 文 件 名 : SUnitTest.cpp
 * 创建日期 : 2015-6-27 9:30
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : 单元测试封装类
 * 修改记录 : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-6-27	邵凯田　创建文件
 *
 **/

#include "SUnitTest.h"
#ifndef WIN32
#include <typeinfo>
#endif

SUnitTestMgr g_UnitTestMgr;

SUnitTest::SUnitTest()
{
	m_bAutoDelete = false;
	m_iRunTimes = 1;
	g_UnitTestMgr.AddTestCase(this);
}
SUnitTest::SUnitTest(int times,bool bAutoDelete)
{
	m_bAutoDelete = bAutoDelete;
	m_iRunTimes = times;
	g_UnitTestMgr.m_Tests.append(this);
}
SUnitTest::~SUnitTest()
{
	g_UnitTestMgr.m_Tests.remove(this);
}

void SUnitTest::AddTestMessage(SString msg)
{
	g_UnitTestMgr.m_slMessage.append(msg);
}


SUnitTestMgr::SUnitTestMgr()
{
	m_Tests.setAutoDelete(false);
}

SUnitTestMgr::~SUnitTestMgr()
{
	SPtrList<SUnitTest> del;
	del.setAutoDelete(true);
	unsigned long pos;
	SUnitTest *p = m_Tests.FetchFirst(pos);
	while(p)
	{
		if(p->m_bAutoDelete)
			del.append(p);
		p = m_Tests.FetchNext(pos);
	}
	m_Tests.clear();
	del.clear();
}

void SUnitTestMgr::AddTestCase(SUnitTest* pTest)
{
	unsigned long pos;
	SUnitTest *p = g_UnitTestMgr.m_Tests.FetchFirst(pos);
	while(p)
	{
		if(typeid(*p) == typeid(*pTest))
			return;
	}
	g_UnitTestMgr.m_Tests.append(pTest);
}

void SUnitTestMgr::Run()
{
	SUnitTestMgr *pThis = &g_UnitTestMgr;
	unsigned long pos;
	int soc,usec;
	int soc2,usec2;
	float sec;
	int i;
	int tc=0;
	SUnitTest *p = pThis->m_Tests.FetchFirst(pos);
	while(p)
	{
		tc++;
		SDateTime::getSystemTime(soc,usec);
		UT_ADD_MESSAGE(SString::toFormat("\n################################################################################\n").data());
		UT_ADD_MESSAGE(SString::toFormat("单元测试%s[%s]%s 开始...\n\n",
			pThis->m_Tests.count()<=1?"":SString::toFormat("%d/%d",tc,pThis->m_Tests.count()).data(),
			typeid(*p).name(),p->m_iRunTimes==1?"":SString::toFormat("共%d次",p->m_iRunTimes).data()).data());
		for(i=0;i<p->m_iRunTimes;i++)
		{
			p->UnitTest();
		}
		SDateTime::getSystemTime(soc2,usec2);
		sec = (soc2-soc)+((float)usec2-(float)usec)/1000000.0f;
		UT_ADD_MESSAGE(SString::toFormat("\n--------------------------------------------------------------------------------\n").data());
		UT_ADD_MESSAGE(SString::toFormat("单元测试%s[%s]%s 已完成! 累计用时:%f秒!%s\n\n",
				pThis->m_Tests.count()<=1?"":SString::toFormat("%d/%d",tc,pThis->m_Tests.count()).data(),
				typeid(*p).name(),
				p->m_iRunTimes==1?"":SString::toFormat("共%d次",p->m_iRunTimes).data(), sec,
				p->m_iRunTimes==1?"":SString::toFormat("平均每次用时:%f秒/次",sec/p->m_iRunTimes).data()).data());
		p = pThis->m_Tests.FetchNext(pos);
	}
	printf("\n################################################################################\n\n");
	for(i=0;i<(int)pThis->m_slMessage.count();i++)
	{
		printf("%s\n",pThis->m_slMessage.at(i)->data());
	}
	printf("\n测试结束，请按回车键退出...");
	getchar();
}
