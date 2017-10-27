/**
 *
 * �� �� �� : SCom.cpp
 * �������� : 2007-01-01
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: 2011/08/31 08:43:33 $
 * ��ǰ�汾 : $Revision: 1.1 $
 * �������� : ����ͨѸ��
 * �޸ļ�¼ : 
 *            $Log: SCom.cpp,v $
 *            Revision 1.1  2011/08/31 08:43:33  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.5  2011/04/27 02:58:48  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.4  2011/04/21 06:42:22  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2011/04/08 12:07:26  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2011/04/06 00:35:32  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2011/02/25 03:12:06  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2010/08/09 03:04:43  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2009/09/15 02:14:06  shaokt
 *            ͨѸ��¼����װ����Ŀ�״��ύ
 *
 *            Revision 1.3  2008/12/03 15:00:48  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2008/12/03 14:55:08  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/01 11:38:28  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/01 08:58:00  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2008/01/24 07:01:19  shaokt
 *            LINUX�´��ڽ��ղ���XON��XOFF�Ĺ���
 *
 *            Revision 1.2  2007/12/10 06:40:28  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2007/11/19 03:48:18  shaokt
 *            ����ͨѸ��
 *
 *
 **/

#include "SCom.h"

////////////////////////////////////////////////////////////////////////////
/// SCom

SCom::SCom()
{
	m_bOpened = false;
	m_hCom = 0;
	SetTimeOut(5000,5000);//Ĭ��5�볬ʱ
#ifdef WIN32
	memset( &m_OverlappedRead, 0, sizeof( OVERLAPPED ) );
 	memset( &m_OverlappedWrite, 0, sizeof( OVERLAPPED ) );
#endif
}

SCom::~SCom()
{
	Close();
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �򿪴����豸
// ��    ��:  �ۿ���
// ����ʱ��:  2007-11-6
// ����˵��:  szComDev�����豸��:win:com1/com2... linux:/dev/ttyS0;/dev/ttyS1...(WINƽ̨>=com10ʱʹ��"\\\\.\\COM10")
//						iBaudrate������,default=9600
//						cParityУ�鷽ʽ��N��У��,EżУ��,O��У��,SֹͣУ��
//						iDataBits����λ���ȣ�default=8
//						fStopBitsֹͣλ����,default=1
// �� �� ֵ:  true/false
//////////////////////////////////////////////////////////////////////////
bool SCom::Open(char *szComDev,int iBaudrate/*=9600*/,char cParity/*='N'*/,
								 int iDataBits/*=8*/,float fStopBits/*=1*/,int iFlowControl/*=0*/)
{
	LOGBASEDEBUG("�򿪴���(%s,%d,%C,%d,%f,%d)",szComDev,iBaudrate,cParity,iDataBits,fStopBits,iFlowControl);
	if(m_bOpened)
	{
		LOGWARN("����'%s'�Ѿ���!",szComDev);
		return false;
	}
#ifdef WIN32
	DCB dcb;
#ifdef UNICODE
	WCHAR *pStr = (WCHAR*)szComDev;
#else
	char *pStr = szComDev;
#endif
	m_hCom = CreateFile( pStr, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0/*FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED*/, NULL );
	if( m_hCom == NULL ) return( FALSE );

	memset( &m_OverlappedRead, 0, sizeof( OVERLAPPED ) );
 	memset( &m_OverlappedWrite, 0, sizeof( OVERLAPPED ) );

	COMMTIMEOUTS CommTimeOuts;
	CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF;
	CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
	CommTimeOuts.ReadTotalTimeoutConstant = 0;
	CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
	CommTimeOuts.WriteTotalTimeoutConstant = 5000;
	SetCommTimeouts( m_hCom, &CommTimeOuts );

	m_OverlappedRead.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
	m_OverlappedWrite.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL );

	dcb.DCBlength = sizeof( DCB );
	GetCommState( m_hCom, &dcb );
	//9600,n,8,1
	//������
	dcb.fBinary=TRUE;
	dcb.BaudRate = iBaudrate;
	//parity=��żЧ��;//0-4=no,odd,even,mark,space
	dcb.fParity=TRUE;
	switch(cParity)
	{
		case 'N':
		case 'n':
			dcb.Parity=NOPARITY;
			dcb.fParity=FALSE;
			break;
		case 'O':
		case 'o':
			dcb.Parity=ODDPARITY;
			break;
		case 'E':
		case 'e':
			dcb.Parity=EVENPARITY;
			break;
		case 'M':
		case 'm':
			dcb.Parity=MARKPARITY;
			break;
		case 'S':
		case 's':
			dcb.Parity=SPACEPARITY;
			break;
	}
	//����λ
	dcb.ByteSize = iDataBits;
	//ֹͣλ
	if(fStopBits == 1)
		dcb.StopBits=ONESTOPBIT;
	if(fStopBits == 1.5)
		dcb.StopBits=ONE5STOPBITS;
	if(fStopBits == 2)
		dcb.StopBits=TWOSTOPBITS;
	//��������flow  0�ޡ�1Ӳ����2XONXOFF 3RTS and XonXoff
//	dcb.fOutxCtsFlow=TRUE;
//	dcb.fOutxDsrFlow=TRUE;
//	dcb.fTXContinueOnXoff=FALSE;
//	dcb.fDtrControl=DTR_CONTROL_ENABLE;
	switch(iFlowControl)
	{
		case 0:
			dcb.fRtsControl=RTS_CONTROL_DISABLE;
			dcb.fOutX=FALSE;
			dcb.fInX=FALSE;
			break;
		case 1:
			dcb.fRtsControl=RTS_CONTROL_ENABLE;
			dcb.fOutX=FALSE;
			dcb.fInX=FALSE;
			break;
		case 2:
			dcb.fRtsControl=RTS_CONTROL_DISABLE;
			dcb.fOutX=TRUE;
			dcb.fInX=TRUE;
			break;
		case 3:
			dcb.fRtsControl=RTS_CONTROL_ENABLE;
			dcb.fOutX=TRUE;
			dcb.fInX=TRUE;
			break;
	}

	if( !SetCommState( m_hCom, &dcb ) ||
		!SetupComm( m_hCom, 10000, 10000 ) ||
		m_OverlappedRead.hEvent == NULL ||
		m_OverlappedWrite.hEvent == NULL )
	{
		DWORD dwError = GetLastError();
		LOGERROR("�򿪴����豸'%s'ʧ��:%d!",szComDev,dwError);
		if( m_OverlappedRead.hEvent != NULL ) CloseHandle( m_OverlappedRead.hEvent );
		if( m_OverlappedWrite.hEvent != NULL ) CloseHandle( m_OverlappedWrite.hEvent );
		CloseHandle( m_hCom );
		m_hCom = 0;
		return false;
	}

#else
	//linux/unix
	//m_hCom = open(szComDev,O_RDWR | O_NOCTTY | O_NONBLOCK | O_NDELAY);//
	m_hCom = open(szComDev,O_RDWR | O_NONBLOCK | O_NOCTTY);
	if(m_hCom == -1)
	{
		LOGERROR("�򿪴����豸'%s'ʧ��!errno=%d",szComDev,errno);
		return false;
	}
	LOGDEBUG("SCom(%s) open=%d",szComDev,m_hCom);

	//���ò�����
	int speed_arr[] = {B115200,B57600, B38400, B19200, B9600, B4800, B2400, B1200, B300,
				B38400, B19200, B9600, B4800, B2400, B1200, B300, };
	int name_arr[] = {115200,57600,38400,  19200,  9600,  4800,  2400,  1200,  300,
				38400,  19200,  9600, 4800, 2400, 1200,  300, };
  int i;
  int status;
  struct termios Opt;
  tcgetattr(m_hCom, &Opt);
	for( i= 0; i < (int)(sizeof(speed_arr) / sizeof(int));  i++)
	{
		if(iBaudrate == name_arr[i])
		{
			tcflush(m_hCom, TCIOFLUSH);
			cfsetispeed(&Opt, speed_arr[i]);
			cfsetospeed(&Opt, speed_arr[i]);
			status = tcsetattr(m_hCom, TCSANOW, &Opt);
			if(status != 0)
			{
				LOGERROR("m_hCom=%d,�������ô���!���ڴ򿪲���ʧ��!",m_hCom);
				Close();
				return false;
				//perror("tcsetattr fd1");
			}
			break;
		}
		tcflush(m_hCom,TCIOFLUSH);
	}

	//����У�顢����λ��ֹͣλ
	struct termios options;
	if( tcgetattr( m_hCom,&options)  !=  0)
	{
		LOGERROR("tcgetattr return != 0");
		//perror("SetupSerial 1");
		Close();
		return false;
	}
	options.c_cflag &= ~CSIZE;
	//�������롢����ĳ�ʼ����
	options.c_iflag = 0;
	options.c_oflag = 0;

	switch (iDataBits) /*��������λ��*/
	{
	case 7:
		options.c_cflag |= CS7;
		break;
	case 8:
		options.c_cflag |= CS8;
		break;
	default:
		LOGERROR("��֧�ֵ�����λ��С:%d",iDataBits);
		Close();
		return false;
	}
	switch (cParity)
	{
	case 'n':
	case 'N':
		options.c_cflag &= ~PARENB;   /* Clear parity enable */
		options.c_iflag &= ~INPCK;     /* Enable parity checking */
		break;
	case 'o':
	case 'O':
		options.c_cflag |= (PARODD | PARENB);  /* ����Ϊ��Ч��*/ 
		options.c_iflag |= INPCK;             /* Disnable parity checking */
		break;
	case 'e':
	case 'E':
		options.c_cflag |= PARENB;     /* Enable parity */
		options.c_cflag &= ~PARODD;   /* ת��ΪżЧ��*/  
		options.c_iflag |= INPCK;       /* Disnable parity checking */
		break;
	case 'S':
	case 's':  /*as no parity*/
		options.c_cflag &= ~PARENB;
		options.c_cflag &= ~CSTOPB;
		break;
	default:
		LOGERROR("��֧�ֵ�У�鷽ʽ:%C",cParity);
		Close();
		return false;
	}
	/* ����ֹͣλ*/   
	if(fStopBits == 1)
		options.c_cflag &= ~CSTOPB;
	else if(fStopBits == 2)
		options.c_cflag |= CSTOPB;
	else
	{
		LOGERROR("��֧�ֵ�ֹͣλ:%f",fStopBits);
		Close();
		return false;
	}

	/* Set input parity option */
	if (cParity != 'n')
		options.c_iflag |= INPCK;
	options.c_cc[VTIME] = 150; // 15 seconds
	options.c_cc[VMIN] = 0;
	
	//������ǿ����ն�֮��ģ�ֻ�Ǵ��ڴ������ݣ�������Ҫ������������ôʹ��ԭʼģʽ(Raw Mode)��ʽ��ͨѶ�����÷�ʽ���£� 
	options.c_lflag  &= ~(ICANON | ECHO | ECHOE | ISIG);  /*Input*/
	options.c_oflag  &= ~OPOST;   /*Output*/

	//Raw ģʽ���. 
	//options.c_oflag = 0; 
	//ICANON  : ���ܱ�׼����, ʹ���л�Ӧ����ͣ��, �����ͳ��ź��Խ��ó�ʽ 
	//options.c_lflag = ICANON;
	tcflush(m_hCom,TCIFLUSH); /* Update the options and do it NOW */
	if (tcsetattr(m_hCom,TCSANOW,&options) != 0)
	{
		LOGERROR("tcsetattr return != 0");
		Close();
		return false;
	}

#endif

	m_bOpened = true;
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �رմ����豸
// ��    ��:  �ۿ���
// ����ʱ��:  2007-11-7
// ����˵��:  
// �� �� ֵ:  
//////////////////////////////////////////////////////////////////////////
bool SCom::Close()
{
	LOGBASEDEBUG("�رմ���,handle=%d",m_hCom);
	if(!m_bOpened && m_hCom <= 0)
	{
		//LOGWARN("�����豸��δ�򿪣����Թر�!");
		return false;
	}
#ifdef WIN32
	if( m_OverlappedRead.hEvent != NULL ) 
		CloseHandle( m_OverlappedRead.hEvent );
	if( m_OverlappedWrite.hEvent != NULL ) 
		CloseHandle( m_OverlappedWrite.hEvent );
	CloseHandle(m_hCom);
#else
	//linux/unix
	close(m_hCom);
#endif
	m_hCom = 0;
	m_bOpened = false;
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ����ָ�����ȵ�����
// ��    ��:  �ۿ���
// ����ʱ��:  2007-11-7
// ����˵��:  �������������ճ���
// �� �� ֵ:  ʵ�ʽ��յ�������<0��ʾʧ��
//////////////////////////////////////////////////////////////////////////
int SCom::Receive(BYTE *pBuffer,int iNeedBytes)
{
	LOGBASEDEBUG("Into SCom::Receive(%p,%d)",pBuffer,iNeedBytes);
	int iRet = 0;
	int iTotal = 0,iLeftBytes = iNeedBytes;
	if(!m_bOpened || m_hCom == 0)
	{
		LOGWARN("�����豸δ��!��������ʧ��!");
		return -1;
	}
	SDateTime first = SDateTime::currentDateTime();
begin:
	iRet = 0;
#ifdef WIN32
	unsigned long dwBytesRead, dwErrorFlags;
	COMSTAT ComStat;
	ClearCommError( m_hCom, &dwErrorFlags, &ComStat );
	if(ComStat.cbInQue)
	{
		dwBytesRead = (DWORD) ComStat.cbInQue;
		if( iLeftBytes < (int) dwBytesRead )
			dwBytesRead = (DWORD) (iLeftBytes);
		BOOL bReadStatus = ReadFile( m_hCom, pBuffer+iTotal, dwBytesRead, &dwBytesRead, &m_OverlappedRead );
		if( !bReadStatus )
		{
			if( GetLastError() == ERROR_IO_PENDING )
			{
				WaitForSingleObject( m_OverlappedRead.hEvent, 2000 );
				iRet = (int) dwBytesRead;
			}
			return( 0 );
		}
		else
			iRet = (int) dwBytesRead;
	}
#else
	//linux/unix
	iRet = read(m_hCom,pBuffer+iTotal,iLeftBytes);
	if(iRet < 0)
	{
		iRet = 0;
	}
#endif
	if(iRet < 0)
		return -1;
	//if(iRet > 0)
	//	return iRet;
	iTotal += iRet;
	SDateTime now = SDateTime::currentDateTime();
	//���ǵ�ʱ����ܱ�����޸�
	if(abs(first.secsTo(now)) > (m_iRecvTimeout<1000?1:(m_iRecvTimeout/1000)) * 2)
		first = now;
	if(first.secsTo(now)*1000+(now.msec()-first.msec()) > m_iRecvTimeout)
		goto end;//��ʱ�˳�
	if(iTotal < iNeedBytes)
	{
		iLeftBytes = iNeedBytes - iTotal;
		SApi::UsSleep(1000);
		goto begin;
	}
end:
	LOGBASEDEBUG("SCom::Receive return=%d",iTotal);
	return iTotal;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ����ָ��������ָ����С������
// ��    ��:  �ۿ���
// ����ʱ��:  2007-11-7
// ����˵��:  �����������ͳ���
// �� �� ֵ:  ʵ�ʷ����ĳ���
//////////////////////////////////////////////////////////////////////////
int SCom::Send(BYTE *pBuffer,int iSendBytes)
{
	LOGBASEDEBUG("Into SCom::Send(%p,%d)",pBuffer,iSendBytes);
	int iRet = 0;
#ifdef WIN32
	BOOL bWriteStat;
	unsigned long dwBytesWritten=0;

	bWriteStat = WriteFile( m_hCom, pBuffer, iSendBytes, &dwBytesWritten, &m_OverlappedWrite );
	if( !bWriteStat && ( GetLastError() == ERROR_IO_PENDING ) )
	{
		if( WaitForSingleObject( m_OverlappedWrite.hEvent, 5000 ) ) 
		{
			//dwBytesWritten = 0;
		}
		else
		{
			GetOverlappedResult( m_hCom, &m_OverlappedWrite, &dwBytesWritten, FALSE );
			m_OverlappedWrite.Offset += dwBytesWritten;
		}
	}

	iRet = dwBytesWritten;
#else
	//linux/unix
	iRet = write(m_hCom,pBuffer,iSendBytes);
#endif

	LOGBASEDEBUG("SCom::Receive return=%d",iRet);
	return iRet;
}
