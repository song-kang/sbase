/**
 *
 * �� �� �� : SInclude.h
 * �������� : 2007-07-01
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: 2011/08/31 08:43:33 $
 * ��ǰ�汾 : $Revision: 1.1 $
 * �������� : ����ͷ�ļ�
 * �޸ļ�¼ : 
 *            $Log: SInclude.h,v $
 *            Revision 1.1  2011/08/31 08:43:33  shaokt
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
 *            Revision 1.2  2008/11/08 11:47:15  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/01 11:38:28  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/01 08:58:01  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.5  2008/02/18 01:07:55  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.4  2007/10/29 07:30:23  shaokt
 *            ��Ӷ����ַ�Ƿ�Ϊ4�ı����ļ�⺯����Ϊ���ARM������pack(1)��ɵ��ڴ�����
 *
 *            Revision 1.3  2007/09/28 11:33:13  shaokt
 *            ���Ӷ�5.0��Ϣ���е�֧�֣���Ϣ�����뷢��
 *
 *            Revision 1.2  2007/09/11 08:17:52  shaokt
 *            �������
 *
 *            Revision 1.1  2007/08/15 03:09:22  shaokt
 *            *** empty log message ***
 *
 **/

#ifndef __SKTINCLUDE_H__

#define __SKTINCLUDE_H__


#ifdef WIN32
	//#include <afx.h>
	//#include <afxwin.h>         // MFC core and standard components
	//#include <afxext.h>         // MFC extensions
	//#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
	//#pragma warning(disable: 4786)

	#define WIN32_LEAN_AND_MEAN
	//#include <windows.h>
	#include <locale.h>
	#include <string.h>
	#include <time.h>		/* timespec{} for pselect() */
	#include <stdio.h>
	#include <iostream>
	#include <fstream>
	#include <process.h>//for thread
	#include <fcntl.h>		/* for nonblocking */
#else
	#include <stdlib.h>
	#include <ctype.h>
	#include <fcntl.h>		/* for nonblocking */
	#include <signal.h>
	#include <signal.h>
	//#include <varargs.h>
	//#include <stdarg.h>
	#include <execinfo.h>
	#include <unistd.h>
	#include <netdb.h>
	#include <langinfo.h>
	#include <pthread.h>
	#include <sys/ioctl.h>
	#include <sys/socket.h>	/* basic socket definitions */
	#include <sys/time.h>	/* timeval{} for select() */
	#include <sys/stat.h>	/* for S_xxx file mode constants */
	#include <sys/uio.h>		/* for iovec{} and readv/writev */
	#include <sys/wait.h>
	#include <sys/un.h>		/* for Unix domain sockets */
	#include <sys/sem.h>
	#include <semaphore.h>
	#include <sys/types.h>
	#include <sys/ipc.h>
	#include <sys/msg.h>
	#include <sys/shm.h>
	#include <sys/mman.h>
	#include <sys/wait.h>
	#include <dlfcn.h>


	#include <linux/kernel.h>
//#ifndef _ARM
//	#include <linux/ext2_fs.h>
//#endif
	#include <netinet/in.h>	/* sockaddr_in{} and other Internet defns */
	#include <arpa/inet.h>	/* inet(3) functions */
#endif



#include "SLog.h"
#include "SSocket.h"
#include "SString.h"
#include "SDateTime.h"
#include "SList.h"
#include "SLock.h"
#include "SFile.h"
#include "SBaseConfig.h"
#include "SXmlConfig.h"
#include "SLock.h"
#include "SObject.h"
#include "SApi.h"
#include "SRing.h"
#include "SBuffer.h"
#include "SCom.h"


#endif//__SKTINCLUDE_H__

