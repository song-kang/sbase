/**
 *
 * 文 件 名 : SUnitTest.h
 * 创建日期 : 2015-6-27 9:28
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

#include "SInclude.h"

#define UT_ADD_MESSAGE(s) {printf("#### UNITTEST MSG(%s:%d): %s \n",__FILE__,__LINE__,s);SUnitTest::AddTestMessage(s);}

#define UT_ASSERT_TRUE(t) if(!(t))		{UT_ADD_MESSAGE("UT_ASSERT_TRUE error!");	exit(0);}
#define UT_ASSERT_FALSE(f) if(f)		{UT_ADD_MESSAGE("UT_ASSERT_FALSE error!");	exit(0);}
#define UT_ASSERT_EQ(a,b) if((a) != (b)){UT_ADD_MESSAGE("UT_ASSERT_EQ error!");		exit(0);}
#define UT_ASSERT_NE(a,b) if((t) == (b)){UT_ADD_MESSAGE("UT_ASSERT_NE error!");		exit(0);}
#define UT_ASSERT_LT(a,b) if((a) >= (b)){UT_ADD_MESSAGE("UT_ASSERT_LT error!");		exit(0);}
#define UT_ASSERT_LE(a,b) if((t) > (b)) {UT_ADD_MESSAGE("UT_ASSERT_LE error!");		exit(0);}
#define UT_ASSERT_GT(a,b) if((a) <= (b)){UT_ADD_MESSAGE("UT_ASSERT_GT error!");		exit(0);}
#define UT_ASSERT_GE(a,b) if((t) < (b)) {UT_ADD_MESSAGE("UT_ASSERT_GE error!");		exit(0);}

#define UT_EXPECT_TRUE(t) if(!(t))		{UT_ADD_MESSAGE("UT_EXPECT_TRUE error!");}
#define UT_EXPECT_FALSE(f) if(f)		{UT_ADD_MESSAGE("UT_EXPECT_FALSE error!");}
#define UT_EXPECT_EQ(a,b) if((a) != (b)){UT_ADD_MESSAGE("UT_EXPECT_EQ error!");}
#define UT_EXPECT_NE(a,b) if((t) == (b)){UT_ADD_MESSAGE("UT_EXPECT_NE error!");}
#define UT_EXPECT_LT(a,b) if((a) >= (b)){UT_ADD_MESSAGE("UT_EXPECT_LT error!");}
#define UT_EXPECT_LE(a,b) if((t) > (b)) {UT_ADD_MESSAGE("UT_EXPECT_LE error!");}
#define UT_EXPECT_GT(a,b) if((a) <= (b)){UT_ADD_MESSAGE("UT_EXPECT_GT error!");}
#define UT_EXPECT_GE(a,b) if((t) < (b)) {UT_ADD_MESSAGE("UT_EXPECT_GE error!");}

class SBASE_EXPORT SUnitTest
{
public:
	SUnitTest();
	SUnitTest(int times,bool bAutoDelete=false);
	virtual ~SUnitTest();

	inline void SetTestTimes(int times){m_iRunTimes=times;};
	inline void SetTestAutoDelete(bool b){m_bAutoDelete=b;};

	virtual int UnitTest()=0;

	static void AddTestMessage(SString msg);

	int m_iRunTimes;//重复运行次数
	bool m_bAutoDelete;//是否自动释放对象
};

class SBASE_EXPORT SUnitTestMgr
{
public:
	SUnitTestMgr();
	virtual ~SUnitTestMgr();
	static void AddTestCase(SUnitTest* pTest);
	static void Run();

	SPtrList<SUnitTest> m_Tests;
	SStringList m_slMessage;
};

