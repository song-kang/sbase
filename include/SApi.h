/**
 *
 * �� �� ��	: SApi.h
 * ��������	: 2007-06-25
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸�����	: $Date: 2011/08/31 08:43:33 $
 * ��ǰ�汾	: $Revision: 1.1 $
 * �������� : ƽ̨�޹صĹ��ú�����������Linux/Windows
 * �޸ļ�¼ : 
 *            $Log: SApi.h,v $
 *            Revision 1.1  2011/08/31 08:43:33  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.4  2011/04/21 12:32:17  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2011/04/16 08:35:07  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2011/02/28 11:21:06  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2011/02/25 03:12:05  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2010/11/19 02:03:22  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2010/10/29 08:48:41  shaokt
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
 *            Revision 1.6  2009/05/19 02:50:38  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.5  2008/10/17 03:33:13  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.4  2008/10/16 09:16:50  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2008/10/16 03:14:25  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2008/09/05 12:39:39  shaokt
 *            ���ȡ��ǰģ��EXE�ļ�����Ŀ¼�ĺ���
 *
 *            Revision 1.1  2008/09/01 11:38:28  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/01 08:58:00  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.10  2008/02/18 01:07:55  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.9  2007/12/10 08:18:57  shaokt
 *            �޸�ȡ��¼�ŵĳ��򣬽���ʶ��ţ��豸�ţ��ŵ����棬��֤��¼����ǰ��ΪyyyyMMddhhmmsszzz
 *
 *            Revision 1.8  2007/11/19 11:10:47  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.7  2007/11/14 11:31:15  shaokt
 *            DWORD���͸�Ϊunsigned long
 *
 *            Revision 1.6  2007/10/30 10:46:20  shaokt
 *            ��������ʱ���һ�����Ӳ���������ʱ����·��
 *
 *            Revision 1.5  2007/10/25 10:40:31  shaokt
 *            LINUX��ȡϵͳ������Ϣʱ������δ�����BUG
 *
 *            Revision 1.4  2007/10/17 10:00:33  shaokt
 *            ��չ���̺���
 *
 *            Revision 1.3  2007/09/11 08:17:52  shaokt
 *            �������
 *
 *            Revision 1.2  2007/08/15 03:23:21  shaokt
 *            �����ֽ���ע��
 *
 *            Revision 1.1  2007/08/15 03:09:22  shaokt
 *            *** empty log message ***
 *
 **/

#if !defined(AFX_SKTAPI_H__3082C864_723C_4BE6_AB17_2CFE75460894__INCLUDED_)
#define AFX_SKTAPI_H__3082C864_723C_4BE6_AB17_2CFE75460894__INCLUDED_

#pragma once

#include "SInclude.h"
#include "SDateTime.h"

#ifdef WIN32
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <tlhelp32.h>
#else
#include <signal.h>   
#endif

enum SBASE_EXPORT SKT_BYTE_ORDER
{
	SKT_LITTLE_ENDIAN=1,//��λ��ǰ��INTEL�ֽ���
	SKT_BIG_ENDIAN=2,//��λ��ǰ������/POWERPC/SPARC�ֽ���
	SKT_UNKNOWN_ENDIAN=3//δ֪
};

//���¶�������ʵ�ָ�Ч�Ľ����ֽ���
//��������_POWERPC����ʱ�����ֽڣ����򲻽����ֽ��򽻻�
//POWERPCƽ̨��float��X86��ARM��float�ֽ�˳��Ҳ���෴�ģ���X86��floatΪ��׼
#ifdef _POWERPC
#define _SKT_NUMBER_SWAP //�Ƿ���Ҫ�ֽ�˳�򽻻�
#endif

#ifdef _SKT_NUMBER_SWAP

#define SKT_SWAP_WORD(w) w = (((WORD)w) << 8) | (((WORD)w) >> 8);
#define SKT_SWAP_DWORD(dw) dw = (((DWORD)dw) << 24 | ((DWORD)dw) >> 24) | ((((DWORD)dw)&0x0000ff00) << 8 | (((DWORD)dw)&0x00ff0000) >> 8);
#define SKT_SWAP_FLOAT(f) {DWORD dw;memcpy(&dw,&f,4);dw = (((DWORD)dw) << 24 | ((DWORD)dw) >> 24) | ((((DWORD)dw)&0x0000ff00) << 8 | (((DWORD)dw)&0x00ff0000) >> 8);memcpy(&f,&dw,4);}

#define SKT_SWAP_WORD_REV(w)
#define SKT_SWAP_DWORD_REV(w)
#define SKT_SWAP_FLOAT_REV(f)

#else

#define SKT_SWAP_WORD(w)
#define SKT_SWAP_DWORD(w)
#define SKT_SWAP_FLOAT(f)

#define SKT_SWAP_WORD_REV(w) w = (((WORD)w) << 8) | (((WORD)w) >> 8);
#define SKT_SWAP_DWORD_REV(dw) dw = (((DWORD)dw) << 24 | ((DWORD)dw) >> 24) | ((((DWORD)dw)&0x0000ff00) << 8 | (((DWORD)dw)&0x00ff0000) >> 8);
#define SKT_SWAP_FLOAT_REV(f) {DWORD dw;memcpy(&dw,&f,4);dw = (((DWORD)dw) << 24 | ((DWORD)dw) >> 24) | ((((DWORD)dw)&0x0000ff00) << 8 | (((DWORD)dw)&0x00ff0000) >> 8);memcpy(&f,&dw,4);}

#endif

#define SKT_SWAP_WORD_ALWAYS(w) w = (((WORD)w) << 8) | (((WORD)w) >> 8);
#define SKT_SWAP_DWORD_ALWAYS(dw) dw = (((DWORD)dw) << 24 | ((DWORD)dw) >> 24) | ((((DWORD)dw)&0x0000ff00) << 8 | (((DWORD)dw)&0x00ff0000) >> 8);
#define SKT_SWAP_FLOAT_ALWAYS(f) {DWORD dw;memcpy(&dw,&f,4);dw = (((DWORD)dw) << 24 | ((DWORD)dw) >> 24) | ((((DWORD)dw)&0x0000ff00) << 8 | (((DWORD)dw)&0x00ff0000) >> 8);memcpy(&f,&dw,4);}


//���ֽڵ�У���
#define SKT_BYTECHECKSUM_WORD(w) ((((WORD)w) >> 8) + (((WORD)w)  & 0xFF))
#define SKT_BYTECHECKSUM_DWORD(dw) ((((DWORD)dw>>24)&0xFF) + (((DWORD)dw>>16)&0xFF) + (((DWORD)dw>>8)&0xFF) + (((DWORD)dw)&0xFF))

//ȡ���ֵ
#define SKT_MAX(a,b) ((a)>(b)?(a):(b))

//ȡ��Сֵ
#define SKT_MIN(a,b) ((a)<(b)?(a):(b))

#ifdef WIN32
	//�����߳�
	#define SKT_CREATE_THREAD(fun,p) \
		{ \
			HANDLE h; \
			h = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)(int)fun, (LPVOID)p, 0, 0); \
			CloseHandle(h); \
		} 
	//�����̲߳�ָ��ջ��С��WINDOW����Ч
	#define SKT_CREATE_THREAD_S(fun,p,stack_size) \
		{ \
			HANDLE h; \
			h = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)(int)fun, (LPVOID)p, 0, 0); \
			CloseHandle(h); \
		} 

	#define SKT_INTO_THREAD

#else
	//�����߳�
	#define SKT_CREATE_THREAD(fun,p) \
		{ \
			pthread_t t=0; \
			int stacksize = 204800;/*20480Ϊ20K*/; \
			pthread_attr_t attr,*pattr=NULL; \
			if(pthread_attr_init(&attr) == 0 && pthread_attr_setstacksize(&attr, stacksize) == 0) \
				pattr = &attr; \
			else \
				printf("@@@@ pthread_attr_setstacksize to %d error!\r\n",stacksize); \
			pthread_create(&t, pattr, fun,(void*)p); \
			if(t == 0) \
				printf("@@@ pthread_create error!"); \
		} 
	//�����̲߳�ָ��ջ��С
	#define SKT_CREATE_THREAD_S(fun,p,stack_size) \
		{ \
			pthread_t t=0; \
			int stacksize = stack_size;/*20480Ϊ20K*/; \
			pthread_attr_t attr,*pattr=NULL; \
			if(pthread_attr_init(&attr) == 0 && pthread_attr_setstacksize(&attr, stacksize) == 0) \
				pattr = &attr; \
			else \
				printf("@@@@ pthread_attr_setstacksize to %d error!\r\n",stacksize); \
			pthread_create(&t, pattr, fun,(void*)p); \
			if(t == 0) \
				printf("@@@ pthread_create error!"); \
		} 
	#define SKT_INTO_THREAD  pthread_detach(pthread_self());\
							signal(SIGPIPE,SIG_IGN);\
							sigset_t signal_mask;\
							sigemptyset (&signal_mask);\
							sigaddset (&signal_mask, SIGPIPE);


#endif
#define S_CREATE_THREAD SKT_CREATE_THREAD
#define S_CREATE_THREAD_S SKT_CREATE_THREAD_S
#define S_INTO_THREAD SKT_INTO_THREAD

//////////////////////////////////////////////////////////////////////////
// ��    ��:  SApi
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 12:58
// ��    ��:  ����API�ӿں��������Ծ�̬������ʽ
//////////////////////////////////////////////////////////////////////////
class SBASE_EXPORT SApi
{
public:
	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���ߣ���λ΢�룬WINƽ̨��ֻ�ܾ�ȷ���ߵ����뼶
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 12:59
	// ����˵��:  @mms��ʾ���ߵ�΢����
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	static void UsSleep(long mms);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �ó���ǰ�̵߳�CPUʱ�䣬�����������̣߳���������ִ���߳�ִ����Ϻ�ᱻ����ִ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-1-5 9:33
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	static void ThreadYield();

	
	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��ǰ����CPU���ֽ�˳����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 13:02
	// ����˵��:  void
	// �� �� ֵ:  SKT_BYTE_ORDER(SKT_LITTLE_ENDIAN / SKT_BIG_ENDIAN)
	//////////////////////////////////////////////////////////////////////////
	static SKT_BYTE_ORDER GetSysByteOrder();
	
	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ת��2�ֽ��������ֽ�˳��Ϊָ�����ֽ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 13:03
	// ����˵��:  @byteOrderҪ��ת������ֽ�˳��
	//            @w��ת��ֵ
	// �� �� ֵ:  ת�����ֵ
	//////////////////////////////////////////////////////////////////////////
	static WORD hton(SKT_BYTE_ORDER byteOrder,WORD w1);
	static WORD hton(SKT_BYTE_ORDER byteOrder,short w1)
	{
		return hton(byteOrder,(WORD)w1);
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ת��4�ֽ��������ֽ�˳��Ϊָ�����ֽ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 13:03
	// ����˵��:  @byteOrderҪ��ת������ֽ�˳��
	//            @w��ת��ֵ
	// �� �� ֵ:  ת�����ֵ
	//////////////////////////////////////////////////////////////////////////
	static DWORD hton(SKT_BYTE_ORDER byteOrder,DWORD w1);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ת��2�ֽ��������ֽ�˳��Ϊָ�����ֽ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 13:03
	// ����˵��:  @byteOrderҪ��ת������ֽ�˳��
	//            @w��ת��ֵ
	// �� �� ֵ:  ת�����ֵ
	//////////////////////////////////////////////////////////////////////////
	static WORD ntoh(SKT_BYTE_ORDER byteOrder,WORD w1);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ת��4�ֽ��������ֽ�˳��Ϊָ�����ֽ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 13:03
	// ����˵��:  @byteOrderҪ��ת������ֽ�˳��
	//            @w��ת��ֵ
	// �� �� ֵ:  ת�����ֵ
	//////////////////////////////////////////////////////////////////////////
	static DWORD ntoh(SKT_BYTE_ORDER byteOrder,DWORD w1);
	static DWORD ntoh(SKT_BYTE_ORDER byteOrder,int w1)
	{
		return ntoh(byteOrder,(DWORD)w1);
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����ַ��ͼ�¼��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 13:05
	// ����˵��:  idΪ���ӱ�ʶ��
	// �� �� ֵ:  23���ַ���ɵļ�¼��
	//////////////////////////////////////////////////////////////////////////
	static SString GetRecordIndex(int id=0);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡϵͳ��ǰ���н�����Ϣ��һ���ַ����б���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 13:07
	// ����˵��:  @plistΪ��Ž�����Ϣ���ַ����б�,ÿ��������Ϣ�ı�ʾ��ʽΪ(pid,ppid,name;....)
	//         :  @sProcNameLike��ʾ��������Ҫƥ����ַ�����NULL��ʾƥ�����н���
	// �� �� ֵ:  =0��ʾ��ȡʧ�ܣ�>0��ʾ�ɹ��ҷ��ص��ǽ�������
	//////////////////////////////////////////////////////////////////////////
	static int GetSystemProcess(SStringList &plist,char* sProcNameLike=NULL);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��ǰ���н��̵�ִ���ļ�����·��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 13:10
	// ����˵��:  void
	// �� �� ֵ:  ִ���ļ�����·��
	//////////////////////////////////////////////////////////////////////////
	static SString GetExeModulePath();
		
	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ִ��һ���ⲿ�����У��Ҳ�Ӱ�챾���̵�����(����������ǰ����,���صȴ��ⲿ�����˳���ŷ���)
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 13:13
	// ����˵��:  @sCmdLine��ʾ����ʱ�����У�����Ϊ·����ִ���ļ���
	//            @sModule��ʾ����ģ��Ŀ�ִ�г����ļ���������·����
	//            @sArg��ʾ���в�����������������ÿո�ָ��������Է�8��������ͬ�������в�������ֿո�
	//            @sPath��ʾִ���ļ����е�·��
	// �� �� ֵ:  >0��ʾִ�гɹ�������Ϊִ��ʧ��
	//////////////////////////////////////////////////////////////////////////
	static int SystemExec(SString sCmdLine,SString sModule,SString sArg,SString sPath="");

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ǿ����ָֹ�����̺ŵĽ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 13:21
	// ����˵��:  @pid��ʾҪ��ֹ�Ľ��̵Ľ��̺�
	// �� �� ֵ:  true��ʾ��ֹ�ɹ�,false��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	static bool SystemKillProcess(int pid);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ������������ϵͳ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 13:22
	// ����˵��:  void
	// �� �� ֵ:  true��ʾִ�гɹ�,false��ʾִ��ʧ��
	//////////////////////////////////////////////////////////////////////////
	static bool SystemReboot();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �رղ���ϵͳ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 13:23
	// ����˵��:  void
	// �� �� ֵ:  true��ʾִ�гɹ�,false��ʾִ��ʧ��
	//////////////////////////////////////////////////////////////////////////
	static bool SystemHalt();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ȡtimeval��ʽ�ĵ�ǰϵͳʱ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 13:24
	// ����˵��:  @tmValΪ���÷��ص�ʱ��ṹ��
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	static void GetLocalDayTime(timeval& tmVal);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ʹ��timeval��ʽ���õ�ǰϵͳʱ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 13:24
	// ����˵��:  @tmVal��ʾ��Ҫ���õ���ʱ��
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	static void SetLocalDayTime(timeval& tmVal);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡCPUʹ���ʰٷֱȵ�λ%
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 13:25
	// ����˵��:  void
	// �� �� ֵ:  ��ǰCPU�ٷֱ�
	//////////////////////////////////////////////////////////////////////////
	static int GetCpuUsedPercent();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��ǰϵͳ�ڴ��ʹ��״̬
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 13:25
	// ����˵��:  @iMemAll��ʾ�ڴ��ܴ�С����λK�ֽ�
	//            @iMemUsed��ʾ�ڴ���ʹ�õĴ�С����λK�ֽ�
	//            @iMemFree��ʾ�ڴ���д�С����λK�ֽ�
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	static void GetMemStatus(int &iMemAll,int &iMemUsed,int &iMemFree);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡָ��PID�Ľ����ڴ�ʹ��״̬
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014-6-16 8:28
	// ����˵��:  @iPidΪ���̺ţ�0��ʾ��ǰ����
	//         :  @iPhysicMemKΪ�����ڴ��С����λΪKB
	//         :  @iVirtualMemKΪ�����ڴ��С����λΪKB
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	static bool GetMemStatusByPid(int iPid,int &iPhysicMemK,int &iVirtualMemK);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡָ��Ӳ�̷�����ʹ�����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 13:27
	// ����˵��:  @sDiskDev��ʾӲ���豸����
	//            @iDiskAll��ʾӲ���ܴ�С����λK�ֽ�
	//            @iDiskUsed��ʾӲ����ʹ�ô�С����λK�ֽ�
	//            @iDiskFree��ʾӲ��ʣ��ռ��С����λK�ֽ�
	//            @iDiskUsePer��ʾӲ����ʹ�õİٷֱ�
	// �� �� ֵ:  true��ʾ��ȡ�ɹ���false��ʾ��ȡʧ��
	//////////////////////////////////////////////////////////////////////////
	static bool GetDiskSpace(SString sDiskDev,int &iDiskAll,int &iDiskUsed,int &iDiskFree,int &iDiskUsePer);
	
	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡָ��Ӳ�̵��ܴ�С�������LINUXϵͳ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2012-7-25 11:40
	// ����˵��:  @sDiskDevΪӲ���豸��
	// �� �� ֵ:  ��С�ӵ�λ���磺320.0 GB / 8109 MB
	//////////////////////////////////////////////////////////////////////////
	static SString GetDiskSize(SString sDiskDev="/dev/sda");

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡָ��Ŀ¼���ܴ�С
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 13:31
	// ����˵��:  @sDir��ʾĿ¼��ȫ·����
	// �� �� ֵ:  Ŀ¼����Ŀ¼��ȫ���ļ��ܴ�С����λK�ֽ�
	//////////////////////////////////////////////////////////////////////////
	static float GetDirSpace(SString sDir);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ָ�����ȵĻ�������������32λ��CRCУ����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 13:32
	// ����˵��:  @DataBuf��ʾ������ָ��
	//            @len��ʾ����������
	// �� �� ֵ:  DWORD�͵�CRCУ����
	//////////////////////////////////////////////////////////////////////////
	static DWORD GenerateCRC32(unsigned char * DataBuf,int  len);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ָ�����ļ�����32λ��CRCУ���룬������Ϊ��16���Ƹ�ʽ������ַ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 13:34
	// ����˵��:  @filename��ʾ�ļ���
	//            @err��ʾ����ʱ�ķ��ش�������
	// �� �� ֵ:  ���ַ�����ʾ���ɴ���(����������err��)�����򷵻ذ�16���Ƹ�ʽ������ַ�����8���ֽ�
	//////////////////////////////////////////////////////////////////////////
	static SString GenerateCRCByFile(SString filename,SString &err);
	
	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����USB�̣�ֻ֧��LINUXϵͳ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2012-10-12 10:08
	// ����˵��:  @sUsbDevStart��ʾUSB�豸����ʼ�豸����Ĭ��Ϊsdb
	// �� �� ֵ:  ���ع��ص㣬�մ���ʾ��U��
	//////////////////////////////////////////////////////////////////////////
	static SString MountUsbDisk(SString sUsbDevStart="sdb");

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ж��U�̣�ֻ֧��LINUXϵͳ����MountUsbDisk���ʹ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2012-10-12 10:09
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	static void UmountUsbDisk();

	static void Create_CRC_table();
		//����CRC32У����
	static unsigned long Cal_CRC(unsigned char *p_begin, unsigned char *p_end);
		//��֤CRC32У����
	static unsigned int Check_CRC(unsigned char *p_begin, unsigned char *p_end);



	//////////////////////////////////////////////////////////////////////////
	//MEMO: DES/3DES �����㷨 [2013:10:15 14:23 �ۿ���]

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  DES����
	// ��    ��:  �ۿ���
	// ����ʱ��:  20xx
	// ����˵��:  @bEnspot��true��ʾ���ܣ�false��ʾ����
	//         :  @pbyKey��ָ����м��ܵ���Կ��8�ֽ�
	//         :  @nLength����Ҫ���м��ܵ����ݳ��ȣ����ֽ�Ϊ��λ����ҪΪ8�ı���
	//         :  @pbySource��ָ����Ҫ���м��ܵ�������ָ��
	//         :  @pbyTarget��ָ�򷵻صļ��ܺ��������ָ��
	// �� �� ֵ:  true��ʾ�ɹ���false��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	static bool Encrypt_DES(bool bEnspot, unsigned char* pbyKey,int nLength, unsigned char* pbySource, unsigned char* pbyTarget);


	////////////////////////////////////////////////////////////////////////
	// ��    ��:  3DES����
	// ��    ��:  �ۿ���
	// ����ʱ��:  20xx
	// ����˵��:  @bEnspot��true��ʾ���ܣ�false��ʾ����
	//         :  @pbyKey��ָ����м��ܵ���Կ��8�ֽ�
	//         :  @nLength����Ҫ���м��ܵ����ݳ��ȣ����ֽ�Ϊ��λ����ҪΪ8�ı���
	//         :  @pbySource��ָ����Ҫ���м��ܵ�������ָ��
	//         :  @pbyTarget��ָ�򷵻صļ��ܺ��������ָ��
	// �� �� ֵ:  true��ʾ�ɹ���false��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	static bool Encrypt_3DES(bool bEnspot, unsigned char* pbyKey,int nLength, unsigned char* pbySource,unsigned char* pbyTarget);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �ַ�������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2013:10:15 15:02
	//         :  @sSrcΪԴ�ַ�������
	//         :  @sKeyΪ��Կ��16�ֽ�
	// �� �� ֵ:  ����
	//////////////////////////////////////////////////////////////////////////
	static SString Encrypt_String(SString sSrc,SString sKey);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �ַ�������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2013:10:15 15:02
	//         :  @sSrcΪ�����˵��ַ�������
	//         :  @sKeyΪ��Կ��16�ֽ�
	// �� �� ֵ:  ����
	//////////////////////////////////////////////////////////////////////////
	static SString Decrypt_String(SString sSrc,SString sKey);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��ǰ���̺�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2014:4:22 11:41
	// ����˵��:  void
	// �� �� ֵ:  ���̺�
	//////////////////////////////////////////////////////////////////////////
	static int GetPid(); 

private:
	static void deskey(unsigned char *key,short edf);
	static void cookey(register unsigned long *raw1);
	static void cpkey(register unsigned long *into);
	static void usekey(register unsigned long *from);
	static void des(unsigned char *inblock, unsigned char *outblock);
	static void scrunch(register unsigned char *outof, register unsigned long *into);
	static void unscrun(register unsigned long *outof, register unsigned char *into);
	static void desfunc(register unsigned long *block, register unsigned long *keys);
	static void des2key(unsigned char *hexkey, short mode);
	static void Ddes(unsigned char *from, unsigned char *into);

	//end des/3des


public:

};

#endif // !defined(AFX_SKTAPI_H__3082C864_723C_4BE6_AB17_2CFE75460894__INCLUDED_)
