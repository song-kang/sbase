/**
 *
 * �� �� ��	: SApi.cpp
 * ��������	: 2008-03-24
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸�����	: $Date: 2011/08/31 08:43:33 $
 * ��ǰ�汾	: $Revision: 1.1 $
 * �������� : ��SApi.h�н������Ƶ�.cpp��,��ʵ�ֱ��뵽.so��
 * �޸ļ�¼ : 
 *            $Log: SApi.cpp,v $
 *            Revision 1.1  2011/08/31 08:43:33  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2011/05/01 02:30:03  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2011/02/25 03:12:05  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2010/11/16 08:28:22  shaokt
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
 *            Revision 1.10  2009/07/13 02:46:56  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.9  2009/06/09 08:46:14  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.8  2009/05/19 02:50:38  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.7  2009/03/10 07:44:22  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.6  2009/02/21 15:55:39  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.5  2008/10/17 03:33:13  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.4  2008/10/16 09:16:49  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2008/09/12 05:18:45  shaokt
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
 *
 **/


#include "SApi.h"
#ifdef WIN32
#include <Psapi.h>
#pragma comment(lib,"Psapi.lib")
#endif
////////////////////////////////////////////////////////////////////////
// ��    ��:  ���ߣ���λ΢�룬WINƽ̨��ֻ�ܾ�ȷ���ߵ����뼶
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 12:59
// ����˵��:  @mms��ʾ���ߵ�΢����
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SApi::UsSleep(long mms)
{
	#ifdef WIN32
		int ms = mms/1000;
		if(ms <= 0)
			ms = 1;
		Sleep(ms);
	#else
		usleep(mms);
	#endif
};
	

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �ó���ǰ�̵߳�CPUʱ�䣬�����������̣߳���������ִ���߳�ִ����Ϻ�ᱻ����ִ��
// ��    ��:  �ۿ���
// ����ʱ��:  2016-1-5 9:33
// ����˵��:  void
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SApi::ThreadYield()
{
#ifdef WIN32
	Sleep(0);
#else
	pthread_yield();
#endif
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ��ǰ����CPU���ֽ�˳��
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 13:02
// ����˵��:  void
// �� �� ֵ:  SKT_BYTE_ORDER(SKT_LITTLE_ENDIAN / SKT_BIG_ENDIAN)
//////////////////////////////////////////////////////////////////////////
SKT_BYTE_ORDER SApi::GetSysByteOrder()
{
	static SKT_BYTE_ORDER sysOrder=SKT_UNKNOWN_ENDIAN;
	if(sysOrder != SKT_UNKNOWN_ENDIAN)
		return sysOrder;
	unsigned short uTest=0x1234;
	if(((unsigned char*)&uTest)[0] == 0x34)
		sysOrder = SKT_LITTLE_ENDIAN;//��ǰϵͳ���ֽ�˳��,�Ƿ���ֽ���ǰ?
	else
		sysOrder = SKT_BIG_ENDIAN;
	return sysOrder;
};

////////////////////////////////////////////////////////////////////////
// ��    ��:  ת��2�ֽ��������ֽ�˳��Ϊָ�����ֽ���
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 13:03
// ����˵��:  @byteOrderҪ��ת������ֽ�˳��
//            @w��ת��ֵ
// �� �� ֵ:  ת�����ֵ
//////////////////////////////////////////////////////////////////////////
WORD SApi::hton(SKT_BYTE_ORDER byteOrder,WORD w1)
{
	if(GetSysByteOrder() == byteOrder)
		return w1;
	BYTE b=((BYTE*)&w1)[0];
	((BYTE*)&w1)[0] = ((BYTE*)&w1)[1];
	((BYTE*)&w1)[1] = b;
	return w1;
};

////////////////////////////////////////////////////////////////////////
// ��    ��:  ת��4�ֽ��������ֽ�˳��Ϊָ�����ֽ���
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 13:03
// ����˵��:  @byteOrderҪ��ת������ֽ�˳��
//            @w��ת��ֵ
// �� �� ֵ:  ת�����ֵ
//////////////////////////////////////////////////////////////////////////
DWORD SApi::hton(SKT_BYTE_ORDER byteOrder,DWORD w)
{
	if(GetSysByteOrder() == byteOrder)
		return w;
	BYTE b=((BYTE*)&w)[0];
	((BYTE*)&w)[0] = ((BYTE*)&w)[3];
	((BYTE*)&w)[3] = b;
	b=((BYTE*)&w)[1];
	((BYTE*)&w)[1] = ((BYTE*)&w)[2];
	((BYTE*)&w)[2] = b;
	return w;
};

////////////////////////////////////////////////////////////////////////
// ��    ��:  ת��2�ֽ��������ֽ�˳��Ϊָ�����ֽ���
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 13:03
// ����˵��:  @byteOrderҪ��ת������ֽ�˳��
//            @w��ת��ֵ
// �� �� ֵ:  ת�����ֵ
//////////////////////////////////////////////////////////////////////////
WORD SApi::ntoh(SKT_BYTE_ORDER byteOrder,WORD w)
{
	return hton(byteOrder,w);
};

////////////////////////////////////////////////////////////////////////
// ��    ��:  ת��4�ֽ��������ֽ�˳��Ϊָ�����ֽ���
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 13:03
// ����˵��:  @byteOrderҪ��ת������ֽ�˳��
//            @w��ת��ֵ
// �� �� ֵ:  ת�����ֵ
//////////////////////////////////////////////////////////////////////////
DWORD SApi::ntoh(SKT_BYTE_ORDER byteOrder,DWORD w)
{
	return hton(byteOrder,w);
};

////////////////////////////////////////////////////////////////////////
// ��    ��:  �����ַ��ͼ�¼��
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 13:05
// ����˵��:  idΪ���ӱ�ʶ��
// �� �� ֵ:  23���ַ���ɵļ�¼��
//////////////////////////////////////////////////////////////////////////
SString SApi::GetRecordIndex(int id/*=0*/)
{
	static int ms=0;
	SString rid;
	if(ms > 999)
		ms = 0;
	ms ++;
	if(id == 0)
	{
		///û�з��豸ʱʹ���̺߳��Է�ֹ����̼����ɵ������г�ͻ
		#ifdef WIN32
			id = (unsigned int)GetCurrentThreadId()%1000;
		#else
			id = (unsigned int)pthread_self()%1000;
		#endif
	}
#ifndef WIN32
	rid.sprintf("%s%03d%03d",SDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz").data(),ms,id);
#else
	for(int i=0;i<5;i++)
	{
		try
		{
			rid.sprintf("%s%03d%03d",SDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz").data(),ms,id);
			break;
		}
		catch (...)
		{
			int err = GetLastError();
			err = errno;
		}
	}
#endif
	return rid;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡϵͳ��ǰ���н�����Ϣ��һ���ַ����б���
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 13:07
// ����˵��:  @plistΪ��Ž�����Ϣ���ַ����б�,ÿ��������Ϣ�ı�ʾ��ʽΪ(pid,ppid,name;....)
//         :  @@sProcNameLike��ʾ��������Ҫƥ����ַ�����NULL��ʾƥ�����н���
// �� �� ֵ:  =0��ʾ��ȡʧ�ܣ�>0��ʾ�ɹ��ҷ��ص��ǽ�������
//////////////////////////////////////////////////////////////////////////
int SApi::GetSystemProcess(SStringList &plist,char* sProcNameLike)
{
	int index=0;
	SString sRet = "";
#ifdef WIN32
	HANDLE   SS;   
	PROCESSENTRY32   processinfo;   
	processinfo.dwSize=sizeof(processinfo);   
	SS=::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);   
	if(SS==NULL)   
	{
		return 0;   
	}   
	
	BOOL status=Process32First(SS,&processinfo);   
	while(status)   
	{
		if(sProcNameLike != NULL && strstr((char*)processinfo.szExeFile,(char*)sProcNameLike) == NULL)
		{
			status=Process32Next(SS,&processinfo);   
			continue;
		}
		sRet = "";
		sRet += SString::toString((int)processinfo.th32ProcessID)+","+
						SString::toString((int)processinfo.th32ParentProcessID)+",";
		sRet += processinfo.szExeFile;
		//if(sRet.right(4).lower() == ".exe")
		//	sRet = sRet.left(sRet.length()-4);
		sRet += "";
		plist.append(sRet);
		index++;
		status=Process32Next(SS,&processinfo);   
	}   
	::CloseHandle(SS);   
#else
	//Ϊ������flash��Ӳ�̣���/proc�²��ҽ�����Ϣ
	SDir dir("/proc");
	SString sDir;
	int i,cnt = dir.count();
	int j,cnt2;
	bool bProcess;
	int pid,ppid;
	SString sCmdLine;
	for(i=0;i<cnt;i++)
	{
		sDir = dir[i];
		//�Ƿ��ǽ���Ŀ¼?ȫ����Ŀ¼Ϊ����Ŀ¼
		bProcess = true;
		cnt2 = sDir.length();
		for(j=0;j<cnt2;j++)
		{
			if(!(sDir.at(j) >= '0' && sDir.at(j) <= '9'))
			{
				bProcess = false;
				break;
			}
		}
		if(!bProcess)
			continue;
		pid = sDir.toInt();
		ppid = 0;//Ϊ�ٷ��ʣ��ݲ�ʵ��ppid��ȡ
		SFile file("/proc/"+sDir+"/cmdline");
		if(!file.open(IO_ReadOnly))
			continue;
		sCmdLine = file.readLine();
		file.close();
		sRet = SString::toString((int)pid)+","+
			SString::toString((int)ppid)+","+sCmdLine;
		if(sProcNameLike != NULL && strstr(sCmdLine.data(),sProcNameLike) == NULL)
		{
			continue;
		}
		plist.append(sRet);
		index++;
	}
#endif
	return index;
}
	
////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ��ǰ���н��̵�ִ���ļ�����·��
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 13:10
// ����˵��:  void
// �� �� ֵ:  ִ���ļ�����·��
//////////////////////////////////////////////////////////////////////////
SString SApi::GetExeModulePath()
{
	SString str;
#if defined(UNICODE) && defined(WIN32)
	WCHAR currentName[SKT_PATH_MAX+1];
#else
	char currentName[SKT_PATH_MAX+1];
#endif
	memset(currentName,0,SKT_PATH_MAX+1);
#ifdef WIN32
	GetModuleFileName(NULL,currentName,SKT_PATH_MAX);
	str = currentName;
	int pos=str.findRev("/");
	if(pos<0)
		pos = str.findRev("\\");
	if(pos>0)
		str = str.left(pos+1);
#else
	if(getcwd(currentName,SKT_PATH_MAX))
		str = currentName;
	else
		str = "";
	int j,pid = getpid();
	SString sCmdLine;
	SFile file("/proc/"+SString::toString(pid)+"/cmdline");
	if(!file.open(IO_ReadOnly))
	{
		printf("open proc file error! pid=%d\n",pid);
		return str;
	}
	sCmdLine = file.readLine();
	file.close();
	j = sCmdLine.Find("/");
	if(j>0)
	{
		sCmdLine = sCmdLine.left(j+1);
		if(sCmdLine.left(1) == "/")
			str = sCmdLine;
		else
		{
			if(str.Right(1) != "/")
				str += "/";
			str += sCmdLine;
		}
	}
#endif
	return str;
}

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
int SApi::SystemExec(SString sCmdLine,SString sModule,SString sArg,SString sPath/*=""*/)
{
	if(sPath.length() > 0 && sPath.right(1) != "/" && sPath.right(1) != "\\")
#ifdef WIN32
		sPath += "\\";
#else
		sPath += "/";
#endif
	LOGDEBUG("Into SystemExec(%s,%s,%s)",sCmdLine.data(),sModule.data(),sPath.data());
#ifdef WIN32
	if(sModule.right(4).toLower() != ".exe")
		sModule += ".exe";
#endif
	if(!SFile::exists(sPath+sModule))//��ֹ�ļ�������ʱ����LINUX��execl���Զ�fork����
	{
		SString sTemp=sPath+sModule;
		LOGERROR("File(%s) is not exists! �ļ�������!",sTemp.data());
		return 0;
	}
	if(!SFile::is_exec_able(sPath+sModule))
	{
		LOGERROR("File(%s) Permission denied! ������Ȩ��!",sCmdLine.data());
		return 0;
	}
	int ret=0;
	#ifdef WIN32
		sCmdLine+=" "+sArg; 
		ret = (int)::WinExec(sCmdLine.data(),SW_SHOWNORMAL);
		if(ret <= 31)
			ret = -1;
		return ret;
	#else
		pid_t child;
		if ((child=fork()) == -1)
		{
			LOGERROR("fork() error:%d",errno);
			return -1;
		}
		else if (child == 0)
		{
			//������Ҫһ��һ������ȥ��if(SString::GetAttributeCount(sArg," "))
			//�д����ŵĲ���
			SStringList slArgs;
			SString sTemp;
			bool begin_dot=false;//�Ƿ����ſ�ʼ
			sTemp = "";
			for(int i=0;i<sArg.length();i++)
			{
				char ch = sArg.at(i);
				if(ch == '\"')
				{
					if(begin_dot)
					{
						//���ǽ�������
						if(sTemp.length() > 0)
							slArgs.append(sTemp);
						sTemp = "";
						begin_dot = false;
					}
					else
					{
						//��ʼһ������
						begin_dot = true;
					}
					continue;
				}
				if( ch == ' ')
				{
					if(!begin_dot)
					{
						//û������ʱ������һ���ո񣬼�Ϊ�ָ�������
						if(sTemp.length() > 0)
							slArgs.append(sTemp);
						sTemp = "";
					}
					else
						sTemp += ch;
				}
				else
				{
					sTemp += ch;
				}
			}
			if(sTemp.length() > 0)
				slArgs.append(sTemp);

			int argc = slArgs.count();//SString::GetAttributeCount(sArg," ");
			if(argc <= 0)
			{
				if(execl(sCmdLine.data(), sModule.data(), (char *)0) == -1)
				{
					LOGERROR("execl(%s, %s) error:%d!",sCmdLine.data(),sModule.data(),errno);
					return -1;
				}
			}
			if(argc <= 1)
			{
				if(execl(sCmdLine.data(), sModule.data(), slArgs.at(0)->data(), (char *)0) == -1)
				{
					LOGERROR("execl(%s, %s) error:%d!",sCmdLine.data(),sModule.data(),errno);
					return -1;
				}
			}
			else if(argc == 2)
			{
				if(execl(sCmdLine.data(), sModule.data(),
						slArgs.at(0)->data(), 
						slArgs.at(1)->data(), 
						(char *)0 ) == -1)
				{
					LOGERROR("execl(%s, %s) error:%d!",sCmdLine.data(),sModule.data(),errno);
					return -1;
				}
			}
			else if(argc == 3)
			{
				if(execl(sCmdLine.data(), sModule.data(),
						slArgs.at(0)->data(), 
						slArgs.at(1)->data(), 
						slArgs.at(2)->data(), 
						(char *)0 ) == -1)
				{
					LOGERROR("execl(%s, %s) error:%d!",sCmdLine.data(),sModule.data(),errno);
					return -1;
				}
			}
			else if(argc == 4)
			{
				if(execl(sCmdLine.data(), sModule.data(),
						slArgs.at(0)->data(), 
						slArgs.at(1)->data(), 
						slArgs.at(2)->data(), 
						slArgs.at(3)->data(), 
						(char *)0 ) == -1)
				{
					LOGERROR("execl(%s, %s) error:%d!",sCmdLine.data(),sModule.data(),errno);
					return -1;
				}
			}
			else if(argc == 5)
			{
				if(execl(sCmdLine.data(), sModule.data(),
						slArgs.at(0)->data(), 
						slArgs.at(1)->data(), 
						slArgs.at(2)->data(), 
						slArgs.at(3)->data(), 
						slArgs.at(4)->data(), 
						(char *)0 ) == -1)
				{
					LOGERROR("execl(%s, %s) error:%d!",sCmdLine.data(),sModule.data(),errno);
					return -1;
				}
			}
			else if(argc == 6)
			{
				if(execl(sCmdLine.data(), sModule.data(),
						slArgs.at(0)->data(), 
						slArgs.at(1)->data(), 
						slArgs.at(2)->data(), 
						slArgs.at(3)->data(), 
						slArgs.at(4)->data(), 
						slArgs.at(5)->data(), 
						(char *)0 ) == -1)
				{
					LOGERROR("execl(%s, %s) error:%d!",sCmdLine.data(),sModule.data(),errno);
					return -1;
				}
			}
			else if(argc == 7)
			{
				if(execl(sCmdLine.data(), sModule.data(),
						slArgs.at(0)->data(), 
						slArgs.at(1)->data(), 
						slArgs.at(2)->data(), 
						slArgs.at(3)->data(), 
						slArgs.at(4)->data(), 
						slArgs.at(5)->data(), 
						slArgs.at(6)->data(), 
						(char *)0 ) == -1)
				{
					LOGERROR("execl(%s, %s) error:%d!",sCmdLine.data(),sModule.data(),errno);
					return -1;
				}
			}
			else if(argc == 8)
			{
				if(execl(sCmdLine.data(), sModule.data(),
						slArgs.at(0)->data(), 
						slArgs.at(1)->data(), 
						slArgs.at(2)->data(), 
						slArgs.at(3)->data(), 
						slArgs.at(4)->data(), 
						slArgs.at(5)->data(), 
						slArgs.at(6)->data(), 
						slArgs.at(7)->data(), 
						(char *)0 ) == -1)
				{
					LOGERROR("execl(%s, %s) error:%d!",sCmdLine.data(),sModule.data(),errno);
					return -1;
				}
			}
		}
		ret = (int)child;
	#endif
	return ret;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ǿ����ָֹ�����̺ŵĽ���
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 13:21
// ����˵��:  @pid��ʾҪ��ֹ�Ľ��̵Ľ��̺�
// �� �� ֵ:  true��ʾ��ֹ�ɹ�,false��ʾʧ��
//////////////////////////////////////////////////////////////////////////
bool SApi::SystemKillProcess(int pid)
{
	bool ret=true;
	#ifdef WIN32
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS,0,(DWORD)pid);   
		if(TerminateProcess(hProcess,0) == FALSE)
			ret = false;
	#else
		kill(pid,SIGUSR1);   
		waitpid(pid,NULL,0);   
	#endif
	return ret;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ������������ϵͳ
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 13:22
// ����˵��:  void
// �� �� ֵ:  true��ʾִ�гɹ�,false��ʾִ��ʧ��
//////////////////////////////////////////////////////////////////////////
bool SApi::SystemReboot()
{
#ifndef WIN32
	::system("reboot");
#else
#endif
	return true;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  �رղ���ϵͳ
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 13:23
// ����˵��:  void
// �� �� ֵ:  true��ʾִ�гɹ�,false��ʾִ��ʧ��
//////////////////////////////////////////////////////////////////////////
bool SApi::SystemHalt()
{
#ifndef WIN32
	::system("halt");
#else
#endif
	return true;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ȡtimeval��ʽ�ĵ�ǰϵͳʱ��
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 13:24
// ����˵��:  @tmValΪ���÷��ص�ʱ��ṹ��
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SApi::GetLocalDayTime(timeval& tmVal)
{
#if !defined(WIN32) && !defined(WINDOWS)
	/// ����ϵͳʱ��
	gettimeofday(&tmVal, NULL);
#else
	time_t tmCurrent = time(NULL);
	tmVal.tv_sec = (long)tmCurrent;
	SYSTEMTIME st;
	GetLocalTime(&st);
	tmVal.tv_usec = st.wMilliseconds;        
#endif
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ʹ��timeval��ʽ���õ�ǰϵͳʱ��
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 13:24
// ����˵��:  @tmVal��ʾ��Ҫ���õ���ʱ��
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SApi::SetLocalDayTime(timeval& tmVal)
{
#if !defined(WIN32) && !defined(WINDOWS)
	/// ����ϵͳʱ��
	settimeofday(&tmVal, NULL);
#else
	SDateTime dt((time_t)tmVal.tv_sec);
	SYSTEMTIME t;
	dt = dt.addHours(-8);//����ʱ�������ʱ���ʱ����-8Сʱ
	//memset(&t,0,sizeof(t));
	t.wYear				= dt.year();
	t.wMonth			= dt.month();
	t.wDay				= dt.day();
	t.wHour				= dt.hour();
	t.wMinute			= dt.minute();
	t.wSecond			= dt.second();
	t.wDayOfWeek  = 0;
	t.wMilliseconds = (unsigned short)(tmVal.tv_usec/1000);
	SetSystemTime(&t);
#endif
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡCPUʹ���ʰٷֱȵ�λ%
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 13:25
// ����˵��:  void
// �� �� ֵ:  ��ǰCPU�ٷֱ�
//////////////////////////////////////////////////////////////////////////
int SApi::GetCpuUsedPercent()//ȡCPUʹ���ʰٷֱ�
{
	int nNewUsges=0;

#ifdef WIN32

	#define SystemBasicInformation       0
	#define SystemPerformanceInformation 2
	#define SystemTimeInformation        3
	#define Li2Double(x) ((double)((x).HighPart) * 4.294967296E9 + (double)((x).LowPart))
	typedef struct
	{
		DWORD   dwUnknown1;
		ULONG   uKeMaximumIncrement;
		ULONG   uPageSize;
		ULONG   uMmNumberOfPhysicalPages;
		ULONG   uMmLowestPhysicalPage;
		ULONG   uMmHighestPhysicalPage;
		ULONG   uAllocationGranularity;
		PVOID   pLowestUserAddress;
		PVOID   pMmHighestUserAddress;
		ULONG   uKeActiveProcessors;
		BYTE    bKeNumberProcessors;
		BYTE    bUnknown2;
		WORD    wUnknown3;
	} SYSTEM_BASIC_INFORMATION;

	typedef struct
	{
		LARGE_INTEGER   liIdleTime;
		DWORD           dwSpare[76];
	} SYSTEM_PERFORMANCE_INFORMATION;

	typedef struct
	{
		LARGE_INTEGER liKeBootTime;
		LARGE_INTEGER liKeSystemTime;
		LARGE_INTEGER liExpTimeZoneBias;
		ULONG         uCurrentTimeZoneId;
		DWORD         dwReserved;
	} SYSTEM_TIME_INFORMATION;

	static LARGE_INTEGER   m_liOldIdleTime;
    static LARGE_INTEGER   m_liOldSystemTime;

	SYSTEM_PERFORMANCE_INFORMATION SysPerfInfo;
    SYSTEM_TIME_INFORMATION        SysTimeInfo;
    SYSTEM_BASIC_INFORMATION       SysBaseInfo;
    double                         dbIdleTime;
    double                         dbSystemTime;
    typedef LONG (WINAPI *PROCNTQSI)(UINT,PVOID,ULONG,PULONG);
    PROCNTQSI NtQuerySystemInformation;
	
#ifdef UNICODE
	if (!(NtQuerySystemInformation = (PROCNTQSI)GetProcAddress(GetModuleHandleW(L"ntdll"), "NtQuerySystemInformation")))
		return 0;
#else
	if (!(NtQuerySystemInformation = (PROCNTQSI)GetProcAddress(GetModuleHandle("ntdll"), "NtQuerySystemInformation")))
		return 0;
#endif
	
    // get number of processors in the system
    if (NtQuerySystemInformation(SystemBasicInformation, &SysBaseInfo,sizeof(SysBaseInfo),NULL) != NO_ERROR)
        return 0;	
	if (NtQuerySystemInformation(SystemTimeInformation, &SysTimeInfo,sizeof(SysTimeInfo),0) != NO_ERROR)
		return 0;
	
	// get new CPU's idle time
	if (NtQuerySystemInformation(SystemPerformanceInformation, &SysPerfInfo,sizeof(SysPerfInfo),NULL) != NO_ERROR)
	{
		return 0;
	}
	
	// if it's a first call - skip it
	if (m_liOldIdleTime.QuadPart != 0)
	{
		// CurrentValue = NewValue - OldValue
		dbIdleTime = Li2Double(SysPerfInfo.liIdleTime) - Li2Double(m_liOldIdleTime);
		dbSystemTime = Li2Double(SysTimeInfo.liKeSystemTime) - Li2Double(m_liOldSystemTime);
		
		// CurrentCpuIdle = IdleTime / SystemTime
		dbIdleTime = dbIdleTime / dbSystemTime;
		
		// CurrentCpuUsage% = 100 - (CurrentCpuIdle * 100) / NumberOfProcessors
		dbIdleTime = 100.0 - dbIdleTime * 100.0 / (double)SysBaseInfo.bKeNumberProcessors + 0.5;		
		nNewUsges = (int)dbIdleTime;
	}
	
	// store new CPU's idle and system time
	m_liOldIdleTime = SysPerfInfo.liIdleTime;
	m_liOldSystemTime = SysTimeInfo.liKeSystemTime;
	
#else
	///proc/stat
	//cpu 432661 13295 86656 422145968 171474 233 5346
	//cpu  191874482 0 368089387 2380888254 16212754 3594669 17876247 0 0
	/*
	CPU �Լ�CPU0��CPU1��CPU2��CPU3ÿ�е�ÿ��������˼���Ե�һ��Ϊ����Ϊ��
	���� ����
	user (432661) ��ϵͳ������ʼ�ۼƵ���ǰʱ�̣��û�̬��CPUʱ�䣨��λ��jiffies�� �������� niceֵΪ�����̡�1jiffies=0.01��
	nice (13295) ��ϵͳ������ʼ�ۼƵ���ǰʱ�̣�niceֵΪ���Ľ�����ռ�õ�CPUʱ�䣨��λ��jiffies�� 
	system (86656) ��ϵͳ������ʼ�ۼƵ���ǰʱ�̣�����ʱ�䣨��λ��jiffies�� 
	idle (422145968) ��ϵͳ������ʼ�ۼƵ���ǰʱ�̣���Ӳ��IO�ȴ�ʱ�����������ȴ�ʱ�䣨��λ��jiffies�� 
	iowait (171474) ��ϵͳ������ʼ�ۼƵ���ǰʱ�̣�Ӳ��IO�ȴ�ʱ�䣨��λ��jiffies�� ��
	irq (233) ��ϵͳ������ʼ�ۼƵ���ǰʱ�̣�Ӳ�ж�ʱ�䣨��λ��jiffies�� 
	softirq (5346) ��ϵͳ������ʼ�ۼƵ���ǰʱ�̣����ж�ʱ�䣨��λ��jiffies��
	*/
	//CPUʱ��=user+system+nice+idle+iowait+irq+softirq
	//cpu usage=[(user_2 +sys_2+nice_2) - (user_1 + sys_1+nice_1)]/(total_2 - total_1)*100
	SFile file("/proc/stat");
	if(!file.open(IO_ReadOnly))
		return 0;
	SString sLine = file.readLine();
	sLine = sLine.mid(3);
	sLine = sLine.stripWhiteSpace();
	file.close();
	static long long user1=0,nice1,system1,total1=0/*,idle1,iowait1,irq1,softirq1*/;
	static long long lastp=0;
	long long user2,nice2,system2,idle2,iowait2,irq2,softirq2,total2;
	user2=SString::GetIdAttributeI64(1,sLine," ");
	nice2=SString::GetIdAttributeI64(2,sLine," ");
	system2=SString::GetIdAttributeI64(3,sLine," ");
	idle2=SString::GetIdAttributeI64(4,sLine," ");
	iowait2=SString::GetIdAttributeI64(5,sLine," ");
	irq2=SString::GetIdAttributeI64(6,sLine," ");
	softirq2=SString::GetIdAttributeI64(7,sLine," ");
	total2=user2+system2+nice2+idle2+iowait2+irq2+softirq2;

	if(user1 != 0)
	{
		if(total2 - total1 == 0)
			nNewUsges = 0;
		else
			nNewUsges = (int)(((float)((user2 +system2+nice2) - (user1 + system1+nice1)))/(total2 - total1)*100);
		if(nNewUsges < 0 || nNewUsges > 100)
			nNewUsges = lastp;
		else
			lastp = nNewUsges;
	}
	user1 = user2;
	nice1 = nice2;
	system1 = system2;
	/*idle1 = idle2;*/
	/*iowait1 = iowait2;*/
	/*irq1 = irq2;*/
	/*softirq1 = softirq2;*/
	total1 = total2;

#endif
	return nNewUsges;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ��ǰϵͳ�ڴ��ʹ��״̬
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 13:25
// ����˵��:  @iMemAll��ʾ�ڴ��ܴ�С����λK�ֽ�
//            @iMemUsed��ʾ�ڴ���ʹ�õĴ�С����λK�ֽ�
//            @iMemFree��ʾ�ڴ���д�С����λK�ֽ�
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SApi::GetMemStatus(int &iMemAll,int &iMemUsed,int &iMemFree)//ȡ�������ݵ�״̬��λK
{
#ifdef WIN32
	MEMORYSTATUS m_MemStat;
	GlobalMemoryStatus(&m_MemStat);
	iMemAll = (int)m_MemStat.dwTotalPhys/1024;
	iMemFree= (int)m_MemStat.dwAvailPhys/1024;
	iMemUsed= iMemAll - iMemFree;
#else
	///proc/meminfo
	// MemTotal:        7997016 kB
	// MemFree:          209236 kB
	// MemAvailable:    1454036 kB
	// Buffers:              44 kB
	// Cached:          4677340 kB
	SFile file("/proc/meminfo");
	if(!file.open(IO_ReadOnly))
		return ;
	SString s1 = file.readLine();
	SString s2 = file.readLine();
	SString s3 = file.readLine();
	SString s4 = file.readLine();
	SString s5 = file.readLine();
	file.close();
	s1 = SString::GetIdAttribute(2,s1,":").stripWhiteSpace();
	s2 = SString::GetIdAttribute(2,s2,":").stripWhiteSpace();
	s3 = SString::GetIdAttribute(2,s3,":").stripWhiteSpace();
	s4 = SString::GetIdAttribute(2,s4,":").stripWhiteSpace();
	s5 = SString::GetIdAttribute(2,s5,":").stripWhiteSpace();
	
	iMemAll = s1.toInt();
	iMemFree= s2.toInt() + s4.toInt() + s5.toInt();
	iMemUsed= iMemAll - iMemFree;

#endif
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡָ��PID�Ľ����ڴ�ʹ��״̬
// ��    ��:  �ۿ���
// ����ʱ��:  2014-6-16 8:28
// ����˵��:  @iPidΪ���̺ţ�0��ʾ��ǰ����
//         :  @iPhysicMemKΪ�����ڴ��С����λΪKB
//         :  @iVirtualMemKΪ�����ڴ��С����λΪKB
// �� �� ֵ:  true/false
//////////////////////////////////////////////////////////////////////////
bool SApi::GetMemStatusByPid(int iPid,int &iPhysicMemK,int &iVirtualMemK)
{
	if(iPid == 0)
		iPid = GetPid();
#ifdef WIN32
	DWORD dwProcessId = (DWORD)iPid;
	//ȡȨ��
	HANDLE hToken;
	LUID uID;
	TOKEN_PRIVILEGES tp;
	if(OpenProcessToken(GetCurrentProcess(),TOKEN_ALL_ACCESS, &hToken))
	{
		LookupPrivilegeValue(NULL,SE_DEBUG_NAME ,&uID);
		tp.PrivilegeCount           = 1;
		tp.Privileges[0].Luid       = uID;
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		AdjustTokenPrivileges(hToken ,FALSE, &tp, sizeof(tp), NULL, NULL);
		CloseHandle(hToken);
	}
	//���̾��
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ, FALSE, dwProcessId);
	if (NULL != hProcess)
	{
		//�ڴ����
		PROCESS_MEMORY_COUNTERS pmc;
		if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
		{
			iPhysicMemK = (int)pmc.WorkingSetSize/1024;
			iVirtualMemK = (int)pmc.PagefileUsage/1024;
		}
		return true;
	}
	return false;
#else
	//VmSize:
	//VmRSS:
	///proc/status
	// VmPeak:	  120352 kB
	// VmSize:	  119584 kB	�����ڴ��С
	// VmLck:	       0 kB
	// VmHWM:	   31668 kB
	// VmRSS:	   31144 kB �����ڴ��С
	// VmData:	    6996 kB
	// VmStk:	     108 kB
	// VmExe:	     612 kB
	// VmLib:	   23772 kB
	// VmPTE:	     112 kB
	/*

	*/
	SFile file(SString::toFormat("/proc/%d/status",iPid));
	if(!file.open(IO_ReadOnly))
		return false;
	int VmSize=-1;
	int VmRSS=-1;
	SString sLine;
	while(!file.atEnd())
	{
		sLine = file.readLine();
		if(sLine.find("VmSize:") == 0)
		{
			VmSize = sLine.mid(7).stripWhiteSpace().toInt();
		}
		if(sLine.find("VmRSS:") == 0)
		{
			VmRSS = sLine.mid(6).stripWhiteSpace().toInt();
		}
	}
	file.close();
	if(VmSize == -1 || VmRSS == -1)
		return false;
	iPhysicMemK = VmRSS;
	iVirtualMemK = VmSize;
	return true;
#endif
}

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
bool SApi::GetDiskSpace(SString sDiskDev,int &iDiskAll,int &iDiskUsed,int &iDiskFree,int &iDiskUsePer)//ȡָ��Ӳ�̷�����ʹ�����
{
	iDiskAll = iDiskUsed = iDiskFree = 0;
	iDiskUsePer = 0;
#ifdef WIN32
	return false;
#else
	SString sTempFile = "disk.tmp";
	SString sLine;
	///dev/hda4            113916332  29436004  78693716  28% /usr/local/pgsql
	sLine.sprintf("df >> %s",sTempFile.data());
	system(sLine.data());
	SFile file(sTempFile);
	if(file.open(IO_ReadOnly) == false)
		return false;
	bool bok=false;
	while(file.atEnd() == false)
	{
		sLine = file.readLine();
		while(sLine.find("  ") >= 0)
			sLine.replace((char*)"  ",(char*)" ");
		if(sLine.Find(sDiskDev) >= 0)
		{
			iDiskAll = SString::GetIdAttributeI(2,sLine," ");
			iDiskUsed = SString::GetIdAttributeI(3,sLine," ");
			iDiskFree = SString::GetIdAttributeI(4,sLine," ");
			iDiskUsePer = SString::GetIdAttributeI(5,sLine," ");
			/*
			sLine = sLine.mid(sDiskDev.length()).stripWhiteSpace();
			iDiskAll = sLine.toInt();
			sLine = sLine.mid(sLine.find(" ")).stripWhiteSpace();
			iDiskUsed= sLine.toInt();
			sLine = sLine.mid(sLine.find(" ")).stripWhiteSpace();
			iDiskFree= sLine.toInt();
			sLine = sLine.mid(sLine.find(" ")).stripWhiteSpace();
			iDiskUsePer= sLine.toInt();
			*/

			bok = true;
			break;
		}
	}
	file.close();
	SFile::remove(sTempFile);
	return bok;
#endif
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡָ��Ӳ�̵��ܴ�С�������LINUXϵͳ
// ��    ��:  �ۿ���
// ����ʱ��:  2012-7-25 11:40
// ����˵��:  @sDiskDevΪӲ���豸��
// �� �� ֵ:  ��С�ӵ�λ���磺320.0 GB / 8109 MB
//////////////////////////////////////////////////////////////////////////
SString SApi::GetDiskSize(SString sDiskDev/*="/dev/sda"*/)
{
#ifdef WIN32
	return "";
#else
	SString sTempFile = "disk.tmp";
	SString sLine,sRet;
	//Disk /dev/sda: 214.7 GB, 214748364800 bytes
	//Disk /dev/hda: 4193 MB, 4193796096 bytes
	//sLine.sprintf("fdisk -l | grep %s: > %s",sDiskDev.data(),sTempFile.data());
	sLine.sprintf("fdisk -l > %s", sTempFile.data());
	::system(sLine.data());
	SFile file(sTempFile);
	if(file.open(IO_ReadOnly) == false)
		return "";
	sRet = "";
	while(!file.atEnd())
	{
		sLine = file.readLine();
		if(sLine.find(sDiskDev+":") >= 0)
		{
			sLine = sLine.mid(sLine.find(sDiskDev)+sDiskDev.length()+1);
			while(sLine.left(1) == " ")
				sLine = sLine.mid(1);
			sLine = SString::GetIdAttribute(1,sLine,",");
			SString sTemp2;
			sTemp2 = SString::GetIdAttribute(1,sLine," ");
			if(sTemp2.find(".")>0)
			{
				while(sTemp2.right(1) == "0")
					sTemp2 = sTemp2.left(sTemp2.length()-1);
				if(sTemp2.right(1) == ".")
					sTemp2 = sTemp2.left(sTemp2.length()-1);
			}
			sRet = sTemp2+SString::GetIdAttribute(2,sLine," ");
			break;
		}
	}
	file.close();
	SFile::remove(sTempFile);
	return sRet;
#endif

}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡָ��Ŀ¼���ܴ�С
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 13:31
// ����˵��:  @sDir��ʾĿ¼��ȫ·����
// �� �� ֵ:  Ŀ¼����Ŀ¼��ȫ���ļ��ܴ�С����λK�ֽ�
//////////////////////////////////////////////////////////////////////////
float SApi::GetDirSpace(SString sDir)//ȡָ��Ŀ¼���ܴ�С��λK
{
#ifdef WIN32
	float totals = 0;
	SDir dir(sDir,"*");
	int i,cnt = dir.count();
	SString sSubPath,sName,sTemp;
	SStringList slSubPath,slSubPath0;
	SStringList slIdDir,slIdDir0;
	for(i=0;i<cnt;i++)
	{
		sTemp = dir.attribute(i);
		sName = dir[i];
		if(SString::GetAttributeValueI(sTemp,"isdir") == 0)
		{
			SFile f(sDir+sName);
			totals += ((float)f.size())/1024;
		}
		else
		{
			if(sName == "." || sName == "..")
				continue;
			totals += GetDirSpace(sDir+sName+"/") + 4;//һ��Ŀ¼�̶�����4K
		}
	}
	return totals;
#else
	SString sTempFile = "disk.tmp";
	SString sLine;
	//2344352 /usr/local/pgsql/ysr/fault
	sLine.sprintf("du %s -s >> %s",sDir.data(),sTempFile.data());
	system(sLine.data());
	SFile file(sTempFile);
	if(file.open(IO_ReadOnly) == false)
		return 0;
	sLine = file.readLine();
	file.close();
	SFile::remove(sTempFile);
	return sLine.toInt();
#endif
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ��������CRC��
// ��    ��:  �ۿ���
// ����ʱ��:  2008-3-21
// ����˵��:  
// �� �� ֵ:  
//////////////////////////////////////////////////////////////////////////
#define poly 0x04c11db7
unsigned int g_CRC32_table[256];


void SApi::Create_CRC_table()
{
    unsigned int temp;
    int i;
    for(i=0; i<256; i++)
    {
        temp = i<<24;
        for(int j=0; j<8; j++)
        {
            if(temp & 0x80000000)
            {
                temp = (temp << 1)^poly;
            }
            else
            {
                temp = temp << 1;
            }
        }
        g_CRC32_table[i] = temp;
    }
}
 
//����CRC32У����
unsigned long SApi::Cal_CRC(unsigned char *p_begin, unsigned char *p_end)
{
    unsigned int CRC = 0x00;
    while(p_begin < p_end)
    {
        CRC = ((CRC << 8) | ((unsigned int)*p_begin)) ^g_CRC32_table[CRC>>24];
        p_begin ++;
    }
    return CRC;
}
 
//��֤CRC32У����
unsigned int SApi::Check_CRC(unsigned char *p_begin, unsigned char *p_end)
{
    unsigned int CRC = 0xffffffff;
    while(p_begin < p_end)
    {
        CRC = (CRC << 8) ^ g_CRC32_table[(CRC>>24)^((unsigned int)*p_begin)];
        p_begin ++;
    }
    return CRC;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ָ�����ȵĻ�������������32λ��CRCУ����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 13:32
// ����˵��:  @DataBuf��ʾ������ָ��
//            @len��ʾ����������
// �� �� ֵ:  DWORD�͵�CRCУ����
//////////////////////////////////////////////////////////////////////////
DWORD SApi::GenerateCRC32(unsigned char * DataBuf,int  len)
{
	static bool bInit = false;
	if(!bInit)
	{
		Create_CRC_table();
		bInit = true;
	}
	return (DWORD)Check_CRC((unsigned char*)DataBuf,(unsigned char*)DataBuf+len);
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ָ�����ļ�����32λ��CRCУ���룬������Ϊ��16���Ƹ�ʽ������ַ���
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-14 13:34
// ����˵��:  @filename��ʾ�ļ���
//            @err��ʾ����ʱ�ķ��ش�������
// �� �� ֵ:  ���ַ�����ʾ���ɴ���(����������err��)�����򷵻ذ�16���Ƹ�ʽ������ַ�����8���ֽ�
//////////////////////////////////////////////////////////////////////////
SString SApi::GenerateCRCByFile(SString filename,SString &err)
{
	
	FILE *fp = fopen(filename.data(),"rb");
	if(fp == NULL)
	{
		err.sprintf("�ļ�[%s]��ʧ��!",filename.data());
		return "";
	}
	fseek(fp,0,SEEK_END);
	int len = ftell(fp);
	if(len > 1024*1024*50)
	{
		fclose(fp);
		err.sprintf("�ļ�[%s]��С(%d�ֽ�)̫�������CRC��Ҫ�ܳ�ʱ�䣬��˲������!",filename.data(),len);
		return "";
	}
	fseek(fp,0,SEEK_SET);
	unsigned char *pBuf = new unsigned char[len];
	int ret = (int)fread(pBuf,1,len,fp);
	fclose(fp);
	if(ret != len)
	{
		delete[] pBuf;
		err.sprintf("�ļ�[%s]��ȡȫ������ʧ��!",filename.data());
		return "";
	}
	unsigned long crc = GenerateCRC32((unsigned char*)pBuf,len);
	delete[] pBuf;
	SString sCrc;
	sCrc.sprintf("%08X",crc);
	return sCrc;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ����USB�̣�ֻ֧��LINUXϵͳ
// ��    ��:  �ۿ���
// ����ʱ��:  2012-10-12 10:08
// ����˵��:  @sUsbDevStart��ʾUSB�豸����ʼ�豸����Ĭ��Ϊsdb
// �� �� ֵ:  ���ع��ص㣬�մ���ʾ��U��
//////////////////////////////////////////////////////////////////////////
SString SApi::MountUsbDisk(SString sUsbDevStart/*="sdb"*/)
{
#ifdef WIN32
	return "";
#else
	//����U���豸 fdisk -l
	//"/dev/sdb1   *          10        6864     3948036   b Win95 FAT32"
	::system("fdisk -l >> /fdisk.txt");
	SFile file("/fdisk.txt");
	if(!file.open(IO_ReadOnly))
	{
		//QMessageBox::information(this,"��ʾ","���U��ʧ��!");
		return "";
	}
	//����Ƿ���sdb1�豸
	SString sLine;
	bool bHdb1=false;
	int sdb=1;
	SString sHdbDev;
#ifdef _ARM
	char ch='b';
#else 
	#ifdef _POWERPC
		char ch='b';
	#else //x86ƽ̨Ŀǰ��sdb1��ʼʱU�̣�Ӳ����sda1���˴�Ӧ������ĿӲ��ƽ̨����޸�
		char ch='b';
	#endif
#endif
	if(sUsbDevStart.length() == 3)
		ch = sUsbDevStart[2];
	while(!bHdb1 && ch<='x')
	{
		sdb=1;
		while(!bHdb1 && sdb <= 8)
		{
			sHdbDev.sprintf("/dev/sd%c%d",ch,sdb);
			file.seekBegin();
			while(!file.atEnd())
			{
				sLine = file.readLine();
				if(sLine.find(sHdbDev) >= 0)
				{
					bHdb1 = true;
					break;
				}
			}
			sdb ++;
		}
		ch++;
	}

	file.close();
	SFile::remove("/fdisk.txt");
	if(!bHdb1)
	{
		//QMessageBox::information(this,"��ʾ","δ��⵽U��!\r\n���Ȳ���U��!");
		return "";
	}
	::system("mkdir /mnt/tempu");
	::system(SString::toFormat("mount %s /mnt/tempu",sHdbDev.data()).data());
	return "/mnt/tempu/";
#endif
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ж��U�̣�ֻ֧��LINUXϵͳ����MountUsbDisk���ʹ��
// ��    ��:  �ۿ���
// ����ʱ��:  2012-10-12 10:09
// ����˵��:  void
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SApi::UmountUsbDisk()
{
#ifdef WIN32
	return;
#else
	::system("umount /mnt/tempu");
	::system("rm -rf /mnt/tempu");
#endif
}


//////////////////////////////////////////////////////////////////////////
//MEMO: DES/3DES �����㷨 [2013:10:15 14:23 �ۿ���]

static unsigned long KnL[32] = { 0L };
static unsigned long KnR[32] = { 0L };
static unsigned long Kn3[32] = { 0L };
// static unsigned char Df_Key[24] = {
// 0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,
// 0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10,
// 0x89,0xab,0xcd,0xef,0x01,0x23,0x45,0x67 };

static unsigned short bytebit[8] = {
0200, 0100, 040, 020, 010, 04, 02, 01 };

static unsigned long bigbyte[24] = {
0x800000L, 0x400000L, 0x200000L, 0x100000L,
0x80000L, 0x40000L, 0x20000L, 0x10000L,
0x8000L, 0x4000L, 0x2000L, 0x1000L,
0x800L, 0x400L, 0x200L, 0x100L,
0x80L, 0x40L, 0x20L, 0x10L,
0x8L, 0x4L, 0x2L, 0x1L };

/* Use the key schedule specified in the Standard (ANSI X3.92-1981). */
static unsigned char pc1[56] = {
56, 48, 40, 32, 24, 16, 8, 0, 57, 49, 41, 33, 25, 17,
9, 1, 58, 50, 42, 34, 26, 18, 10, 2, 59, 51, 43, 35,
62, 54, 46, 38, 30, 22, 14, 6, 61, 53, 45, 37, 29, 21,
13, 5, 60, 52, 44, 36, 28, 20, 12, 4, 27, 19, 11, 3 };

static unsigned char totrot[16] = {
1,2,4,6,8,10,12,14,15,17,19,21,23,25,27,28 };

static unsigned char pc2[48] = {
13, 16, 10, 23, 0, 4, 2, 27, 14, 5, 20, 9,
22, 18, 11, 3, 25, 7, 15, 6, 26, 19, 12, 1,
40, 51, 30, 36, 46, 54, 29, 39, 50, 44, 32, 47,
43, 48, 38, 55, 33, 52, 45, 41, 49, 35, 28, 31 };

void SApi::deskey(unsigned char *key,short edf) /* Thanks to James Gillogly & Phil Karn! */
{
	register int i, j, l, m, n;
	unsigned char pc1m[56], pcr[56];
	unsigned long kn[32];
	for ( j = 0; j < 56; j++ ) {
		l = pc1[j];
		m = l & 07;
		pc1m[j] = (key[l >> 3] & bytebit[m]) ? 1 : 0;
	}
	for( i = 0; i < 16; i++ ) 
	{
		if( edf == 1 ) m = (15 - i) << 1;
		else m = i << 1;
		n = m + 1;
		kn[m] = kn[n] = 0L;
		for( j = 0; j < 28; j++ ) {
			l = j + totrot[i];
			if( l < 28 ) pcr[j] = pc1m[l];
			else pcr[j] = pc1m[l - 28];
		}
		for( j = 28; j < 56; j++ ) {
			l = j + totrot[i];
			if( l < 56 ) pcr[j] = pc1m[l];
			else pcr[j] = pc1m[l - 28];
		}
		for( j = 0; j < 24; j++ ) {
			if( pcr[pc2[j]] ) kn[m] |= bigbyte[j];
			if( pcr[pc2[j+24]] ) kn[n] |= bigbyte[j];
		}
	}
	cookey(kn);
	return;
}
void SApi::cookey(register unsigned long *raw1)
{
	register unsigned long *cook, *raw0;
	unsigned long dough[32];
	register int i;
	cook = dough;
	for( i = 0; i < 16; i++, raw1++ ) {
	raw0 = raw1++;
	*cook = (*raw0 & 0x00fc0000L) << 6;
	*cook |= (*raw0 & 0x00000fc0L) << 10;
	*cook |= (*raw1 & 0x00fc0000L) >> 10;
	*cook++ |= (*raw1 & 0x00000fc0L) >> 6;
	*cook = (*raw0 & 0x0003f000L) << 12;
	*cook |= (*raw0 & 0x0000003fL) << 16;
	*cook |= (*raw1 & 0x0003f000L) >> 4;
	*cook++ |= (*raw1 & 0x0000003fL);
	}
	usekey(dough);
	return;
}
void SApi::cpkey(register unsigned long *into)
{
	register unsigned long *from, *endp;
	from = KnL, endp = &KnL[32];
	while( from < endp ) *into++ = *from++;
	return;
}

void SApi::usekey(register unsigned long *from)
{
	register unsigned long *to, *endp;
	to = KnL, endp = &KnL[32];
	while( to < endp ) *to++ = *from++;
	return;
}
void SApi::des(unsigned char *inblock, unsigned char *outblock)
{
	unsigned long work[2];
	scrunch(inblock, work);
	desfunc(work, KnL);
	unscrun(work, outblock);
	return;
}
void SApi::scrunch(register unsigned char *outof, register unsigned long *into)
{
	*into = (*outof++ & 0xffL) << 24;
	*into |= (*outof++ & 0xffL) << 16;
	*into |= (*outof++ & 0xffL) << 8;
	*into++ |= (*outof++ & 0xffL);
	*into = (*outof++ & 0xffL) << 24;
	*into |= (*outof++ & 0xffL) << 16;
	*into |= (*outof++ & 0xffL) << 8;
	*into |= (*outof & 0xffL);
	return;
}
void SApi::unscrun(register unsigned long *outof, register unsigned char *into)
{
	*into++ = (*outof >> 24) & 0xffL;
	*into++ = (*outof >> 16) & 0xffL;
	*into++ = (*outof >> 8) & 0xffL;
	*into++ = *outof++ & 0xffL;
	*into++ = (*outof >> 24) & 0xffL;
	*into++ = (*outof >> 16) & 0xffL;
	*into++ = (*outof >> 8) & 0xffL;
	*into = *outof & 0xffL;
	return;
}
static unsigned long SP1[64] = {
0x01010400L, 0x00000000L, 0x00010000L, 0x01010404L,
0x01010004L, 0x00010404L, 0x00000004L, 0x00010000L,
0x00000400L, 0x01010400L, 0x01010404L, 0x00000400L,
0x01000404L, 0x01010004L, 0x01000000L, 0x00000004L,
0x00000404L, 0x01000400L, 0x01000400L, 0x00010400L,
0x00010400L, 0x01010000L, 0x01010000L, 0x01000404L,
0x00010004L, 0x01000004L, 0x01000004L, 0x00010004L,
0x00000000L, 0x00000404L, 0x00010404L, 0x01000000L,
0x00010000L, 0x01010404L, 0x00000004L, 0x01010000L,
0x01010400L, 0x01000000L, 0x01000000L, 0x00000400L,
0x01010004L, 0x00010000L, 0x00010400L, 0x01000004L,
0x00000400L, 0x00000004L, 0x01000404L, 0x00010404L,
0x01010404L, 0x00010004L, 0x01010000L, 0x01000404L,
0x01000004L, 0x00000404L, 0x00010404L, 0x01010400L,
0x00000404L, 0x01000400L, 0x01000400L, 0x00000000L,
0x00010004L, 0x00010400L, 0x00000000L, 0x01010004L };

static unsigned long SP2[64] = {
0x80108020L, 0x80008000L, 0x00008000L, 0x00108020L,
0x00100000L, 0x00000020L, 0x80100020L, 0x80008020L,
0x80000020L, 0x80108020L, 0x80108000L, 0x80000000L,
0x80008000L, 0x00100000L, 0x00000020L, 0x80100020L,
0x00108000L, 0x00100020L, 0x80008020L, 0x00000000L,
0x80000000L, 0x00008000L, 0x00108020L, 0x80100000L,
0x00100020L, 0x80000020L, 0x00000000L, 0x00108000L,
0x00008020L, 0x80108000L, 0x80100000L, 0x00008020L,
0x00000000L, 0x00108020L, 0x80100020L, 0x00100000L,
0x80008020L, 0x80100000L, 0x80108000L, 0x00008000L,
0x80100000L, 0x80008000L, 0x00000020L, 0x80108020L,
0x00108020L, 0x00000020L, 0x00008000L, 0x80000000L,
0x00008020L, 0x80108000L, 0x00100000L, 0x80000020L,
0x00100020L, 0x80008020L, 0x80000020L, 0x00100020L,
0x00108000L, 0x00000000L, 0x80008000L, 0x00008020L,
0x80000000L, 0x80100020L, 0x80108020L, 0x00108000L };

static unsigned long SP3[64] = {
0x00000208L, 0x08020200L, 0x00000000L, 0x08020008L,
0x08000200L, 0x00000000L, 0x00020208L, 0x08000200L,
0x00020008L, 0x08000008L, 0x08000008L, 0x00020000L,
0x08020208L, 0x00020008L, 0x08020000L, 0x00000208L,
0x08000000L, 0x00000008L, 0x08020200L, 0x00000200L,
0x00020200L, 0x08020000L, 0x08020008L, 0x00020208L,
0x08000208L, 0x00020200L, 0x00020000L, 0x08000208L,
0x00000008L, 0x08020208L, 0x00000200L, 0x08000000L,
0x08020200L, 0x08000000L, 0x00020008L, 0x00000208L,
0x00020000L, 0x08020200L, 0x08000200L, 0x00000000L,
0x00000200L, 0x00020008L, 0x08020208L, 0x08000200L,
0x08000008L, 0x00000200L, 0x00000000L, 0x08020008L,
0x08000208L, 0x00020000L, 0x08000000L, 0x08020208L,
0x00000008L, 0x00020208L, 0x00020200L, 0x08000008L,
0x08020000L, 0x08000208L, 0x00000208L, 0x08020000L,
0x00020208L, 0x00000008L, 0x08020008L, 0x00020200L };

static unsigned long SP4[64] = {
0x00802001L, 0x00002081L, 0x00002081L, 0x00000080L,
0x00802080L, 0x00800081L, 0x00800001L, 0x00002001L,
0x00000000L, 0x00802000L, 0x00802000L, 0x00802081L,
0x00000081L, 0x00000000L, 0x00800080L, 0x00800001L,
0x00000001L, 0x00002000L, 0x00800000L, 0x00802001L,
0x00000080L, 0x00800000L, 0x00002001L, 0x00002080L,
0x00800081L, 0x00000001L, 0x00002080L, 0x00800080L,
0x00002000L, 0x00802080L, 0x00802081L, 0x00000081L,
0x00800080L, 0x00800001L, 0x00802000L, 0x00802081L,
0x00000081L, 0x00000000L, 0x00000000L, 0x00802000L,
0x00002080L, 0x00800080L, 0x00800081L, 0x00000001L,
0x00802001L, 0x00002081L, 0x00002081L, 0x00000080L,
0x00802081L, 0x00000081L, 0x00000001L, 0x00002000L,
0x00800001L, 0x00002001L, 0x00802080L, 0x00800081L,
0x00002001L, 0x00002080L, 0x00800000L, 0x00802001L,
0x00000080L, 0x00800000L, 0x00002000L, 0x00802080L };

static unsigned long SP5[64] = {
0x00000100L, 0x02080100L, 0x02080000L, 0x42000100L,
0x00080000L, 0x00000100L, 0x40000000L, 0x02080000L,
0x40080100L, 0x00080000L, 0x02000100L, 0x40080100L,
0x42000100L, 0x42080000L, 0x00080100L, 0x40000000L,
0x02000000L, 0x40080000L, 0x40080000L, 0x00000000L,
0x40000100L, 0x42080100L, 0x42080100L, 0x02000100L,
0x42080000L, 0x40000100L, 0x00000000L, 0x42000000L,
0x02080100L, 0x02000000L, 0x42000000L, 0x00080100L,
0x00080000L, 0x42000100L, 0x00000100L, 0x02000000L,
0x40000000L, 0x02080000L, 0x42000100L, 0x40080100L,
0x02000100L, 0x40000000L, 0x42080000L, 0x02080100L,
0x40080100L, 0x00000100L, 0x02000000L, 0x42080000L,
0x42080100L, 0x00080100L, 0x42000000L, 0x42080100L,
0x02080000L, 0x00000000L, 0x40080000L, 0x42000000L,
0x00080100L, 0x02000100L, 0x40000100L, 0x00080000L,
0x00000000L, 0x40080000L, 0x02080100L, 0x40000100L };

static unsigned long SP6[64] = {
0x20000010L, 0x20400000L, 0x00004000L, 0x20404010L,
0x20400000L, 0x00000010L, 0x20404010L, 0x00400000L,
0x20004000L, 0x00404010L, 0x00400000L, 0x20000010L,
0x00400010L, 0x20004000L, 0x20000000L, 0x00004010L,
0x00000000L, 0x00400010L, 0x20004010L, 0x00004000L,
0x00404000L, 0x20004010L, 0x00000010L, 0x20400010L,
0x20400010L, 0x00000000L, 0x00404010L, 0x20404000L,
0x00004010L, 0x00404000L, 0x20404000L, 0x20000000L,
0x20004000L, 0x00000010L, 0x20400010L, 0x00404000L,
0x20404010L, 0x00400000L, 0x00004010L, 0x20000010L,
0x00400000L, 0x20004000L, 0x20000000L, 0x00004010L,
0x20000010L, 0x20404010L, 0x00404000L, 0x20400000L,
0x00404010L, 0x20404000L, 0x00000000L, 0x20400010L,
0x00000010L, 0x00004000L, 0x20400000L, 0x00404010L,
0x00004000L, 0x00400010L, 0x20004010L, 0x00000000L,
0x20404000L, 0x20000000L, 0x00400010L, 0x20004010L };

static unsigned long SP7[64] = {
0x00200000L, 0x04200002L, 0x04000802L, 0x00000000L,
0x00000800L, 0x04000802L, 0x00200802L, 0x04200800L,
0x04200802L, 0x00200000L, 0x00000000L, 0x04000002L,
0x00000002L, 0x04000000L, 0x04200002L, 0x00000802L,
0x04000800L, 0x00200802L, 0x00200002L, 0x04000800L,
0x04000002L, 0x04200000L, 0x04200800L, 0x00200002L,
0x04200000L, 0x00000800L, 0x00000802L, 0x04200802L,
0x00200800L, 0x00000002L, 0x04000000L, 0x00200800L,
0x04000000L, 0x00200800L, 0x00200000L, 0x04000802L,
0x04000802L, 0x04200002L, 0x04200002L, 0x00000002L,
0x00200002L, 0x04000000L, 0x04000800L, 0x00200000L,
0x04200800L, 0x00000802L, 0x00200802L, 0x04200800L,
0x00000802L, 0x04000002L, 0x04200802L, 0x04200000L,
0x00200800L, 0x00000000L, 0x00000002L, 0x04200802L,
0x00000000L, 0x00200802L, 0x04200000L, 0x00000800L,
0x04000002L, 0x04000800L, 0x00000800L, 0x00200002L };

static unsigned long SP8[64] = {
0x10001040L, 0x00001000L, 0x00040000L, 0x10041040L,
0x10000000L, 0x10001040L, 0x00000040L, 0x10000000L,
0x00040040L, 0x10040000L, 0x10041040L, 0x00041000L,
0x10041000L, 0x00041040L, 0x00001000L, 0x00000040L,
0x10040000L, 0x10000040L, 0x10001000L, 0x00001040L,
0x00041000L, 0x00040040L, 0x10040040L, 0x10041000L,
0x00001040L, 0x00000000L, 0x00000000L, 0x10040040L,
0x10000040L, 0x10001000L, 0x00041040L, 0x00040000L,
0x00041040L, 0x00040000L, 0x10041000L, 0x00001000L,
0x00000040L, 0x10040040L, 0x00001000L, 0x00041040L,
0x10001000L, 0x00000040L, 0x10000040L, 0x10040000L,
0x10040040L, 0x10000000L, 0x00040000L, 0x10001040L,
0x00000000L, 0x10041040L, 0x00040040L, 0x10000040L,
0x10040000L, 0x10001000L, 0x10001040L, 0x00000000L,
0x10041040L, 0x00041000L, 0x00041000L, 0x00001040L,
0x00001040L, 0x00040040L, 0x10000000L, 0x10041000L
};

void SApi::desfunc(register unsigned long *block, register unsigned long *keys)
{
	register unsigned long fval, work, right, leftt;
	register int round;
	leftt = block[0];
	right = block[1];
	work = ((leftt >> 4) ^ right) & 0x0f0f0f0fL;
	right ^= work;
	leftt ^= (work << 4);
	work = ((leftt >> 16) ^ right) & 0x0000ffffL;
	right ^= work;
	leftt ^= (work << 16);
	work = ((right >> 2) ^ leftt) & 0x33333333L;
	leftt ^= work;
	right ^= (work << 2);
	work = ((right >> 8) ^ leftt) & 0x00ff00ffL;
	leftt ^= work;
	right ^= (work << 8);
	right = ((right << 1) | ((right >> 31) & 1L)) & 0xffffffffL;
	work = (leftt ^ right) & 0xaaaaaaaaL;
	leftt ^= work;
	right ^= work;
	leftt = ((leftt << 1) | ((leftt >> 31) & 1L)) & 0xffffffffL;
	for( round = 0; round < 8; round++ ) {
	work = (right << 28) | (right >> 4);
	work ^= *keys++;
	fval = SP7[ work & 0x3fL];
	fval |= SP5[(work >> 8) & 0x3fL];
	fval |= SP3[(work >> 16) & 0x3fL];
	fval |= SP1[(work >> 24) & 0x3fL];
	work = right ^ *keys++;
	fval |= SP8[ work & 0x3fL];
	fval |= SP6[(work >> 8) & 0x3fL];
	fval |= SP4[(work >> 16) & 0x3fL];
	fval |= SP2[(work >> 24) & 0x3fL];
	leftt ^= fval;
	work = (leftt << 28) | (leftt >> 4);
	work ^= *keys++;
	fval = SP7[ work & 0x3fL];
	fval |= SP5[(work >> 8) & 0x3fL];
	fval |= SP3[(work >> 16) & 0x3fL];
	fval |= SP1[(work >> 24) & 0x3fL];
	work = leftt ^ *keys++;
	fval |= SP8[ work & 0x3fL];
	fval |= SP6[(work >> 8) & 0x3fL];
	fval |= SP4[(work >> 16) & 0x3fL];
	fval |= SP2[(work >> 24) & 0x3fL];
	right ^= fval;
	}
	right = (right << 31) | (right >> 1);
	work = (leftt ^ right) & 0xaaaaaaaaL;
	leftt ^= work;
	right ^= work;
	leftt = (leftt << 31) | (leftt >> 1);
	work = ((leftt >> 8) ^ right) & 0x00ff00ffL;
	right ^= work;
	leftt ^= (work << 8);
	work = ((leftt >> 2) ^ right) & 0x33333333L;
	right ^= work;
	leftt ^= (work << 2);
	work = ((right >> 16) ^ leftt) & 0x0000ffffL;
	leftt ^= work;
	right ^= (work << 16);
	work = ((right >> 4) ^ leftt) & 0x0f0f0f0fL;
	leftt ^= work;
	right ^= (work << 4);
	*block++ = right;
	*block = leftt;
	return;
}
void SApi::des2key(unsigned char *hexkey, short mode) /* stomps on Kn3 too */
{
	short revmod;
	revmod = (mode == 0) ? 1 : 0;
	deskey(&hexkey[8], revmod);
	cpkey(KnR);
	deskey(hexkey, mode);
	cpkey(Kn3); /* Kn3 = KnL */
	return;
}
void SApi::Ddes(unsigned char *from, unsigned char *into)
{
	unsigned long work[2];
	scrunch(from, work);
	desfunc(work, KnL);
	desfunc(work, KnR);
	desfunc(work, Kn3);
	unscrun(work, into);
	return;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  3DES����
// ��    ��:  �ۿ���
// ����ʱ��:  20xx
// ����˵��:  @bEnspot��true��ʾ���ܣ�false��ʾ����
//         :  @pbyKey��ָ����м��ܵ���Կ
//         :  @nLength����Ҫ���м��ܵ����ݳ��ȣ����ֽ�Ϊ��λ����ҪΪ8�ı���
//         :  @pbySource��ָ����Ҫ���м��ܵ�������ָ��
//         :  @pbyTarget��ָ�򷵻صļ��ܺ��������ָ��
// �� �� ֵ:  true��ʾ�ɹ���false��ʾʧ��
//////////////////////////////////////////////////////////////////////////
bool SApi::Encrypt_3DES(bool bEnspot, unsigned char* pbyKey,int nLength, unsigned char* pbySource, unsigned char* pbyTarget)
{
	int i,section;
	if(nLength==0 || nLength%8)
		return false;
	section = nLength/8;
	des2key(pbyKey,bEnspot?0:1);
	for(i=0;i<section;i++)
		Ddes(pbySource+8*i,pbyTarget+8*i);
	return true;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  DES����
// ��    ��:  �ۿ���
// ����ʱ��:  20xx
// ����˵��:  @bEnspot��true��ʾ���ܣ�false��ʾ����
//         :  @pbyKey��ָ����м��ܵ���Կ
//         :  @nLength����Ҫ���м��ܵ����ݳ��ȣ����ֽ�Ϊ��λ����ҪΪ8�ı���
//         :  @pbySource��ָ����Ҫ���м��ܵ�������ָ��
//         :  @pbyTarget��ָ�򷵻صļ��ܺ��������ָ��
// �� �� ֵ:  true��ʾ�ɹ���false��ʾʧ��
//////////////////////////////////////////////////////////////////////////
bool SApi::Encrypt_DES(bool bEnspot, unsigned char* pbyKey,int nLength, unsigned char* pbySource, unsigned char* pbyTarget)
{
	int section,i;
	if(nLength==0 || nLength%8)
		return false;
	section = nLength/8;
	deskey(pbyKey,bEnspot?0:1);
	for(i = 0 ;i<section;i++)
		des(pbySource+8*i,pbyTarget+8*i);
	return true;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  �ַ�������
// ��    ��:  �ۿ���
// ����ʱ��:  2013:10:15 15:02
//         :  @sSrcΪԴ�ַ�������
//         :  @sKeyΪ��Կ������Ϊ16�ַ�Ϊ��
// �� �� ֵ:  ����
//////////////////////////////////////////////////////////////////////////
SString SApi::Encrypt_String(SString sSrc,SString sKey)
{
	int len = sSrc.length();
	if(len % 8 != 0)
		len = ((len/8)+1)*8;
	BYTE *pSrc = new BYTE[len];
	memset(pSrc,0,len);
	if(sSrc.length() > 0)
		memcpy(pSrc,sSrc.data(),sSrc.length());
	BYTE *pDst = new BYTE[len];
	memset(pDst,0,len);
	BYTE key[128];
	memset(key,0,sizeof(key));
	memcpy(key,sKey.data(),sKey.length()>128?128:sKey.length());
	Encrypt_3DES(true,key,len,pSrc,pDst);

	SString str = SString::HexToStr(pDst,len,false);
	delete[] pSrc;
	delete[] pDst;
	return str;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  �ַ�������
// ��    ��:  �ۿ���
// ����ʱ��:  2013:10:15 15:02
//         :  @sSrcΪ�����˵��ַ�������
//         :  @sKeyΪ��Կ������Ϊ16�ַ�Ϊ��
// �� �� ֵ:  ����
//////////////////////////////////////////////////////////////////////////
SString SApi::Decrypt_String(SString sSrc,SString sKey)
{
	int len = sSrc.length()/2;
	if(len <= 0)
		return "";
	BYTE *pHex = new BYTE[len];
	SString::StrToHex(sSrc,pHex);
	
	BYTE *pDst = new BYTE[len+1];
	memset(pDst,0,len+1);
	BYTE key[128];
	memset(key,0,sizeof(key));
	memcpy(key,sKey.data(),sKey.length()>128?128:sKey.length());
	Encrypt_3DES(false,key,len,pHex,pDst);

	SString str((char*)pDst);
	delete[] pHex;
	delete[] pDst;
	return str;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ��ǰ���̺�
// ��    ��:  �ۿ���
// ����ʱ��:  2014:4:22 11:41
// ����˵��:  void
// �� �� ֵ:  ���̺�
//////////////////////////////////////////////////////////////////////////
int SApi::GetPid()
{
#ifdef _WIN32
	return _getpid();
#else
	return getpid();
#endif
}