/**
 *
 * �� �� �� : SCom.h
 * �������� : 2007-11-06
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: 2011/08/31 08:43:33 $
 * ��ǰ�汾 : $Revision: 1.1 $
 * �������� : ����ͨѸ��
 * �޸ļ�¼ : 
 *            $Log: SCom.h,v $
 *            Revision 1.1  2011/08/31 08:43:33  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2011/02/25 03:12:06  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2010/08/09 03:04:43  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2009/09/15 02:14:07  shaokt
 *            ͨѸ��¼����װ����Ŀ�״��ύ
 *
 *            Revision 1.2  2008/09/11 09:47:28  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/01 11:38:28  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/01 08:58:00  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2007/11/19 03:48:18  shaokt
 *            ����ͨѸ��
 *
 *
 **/

#ifndef __SKTCOM_H_
#define __SKTCOM_H_

#ifndef WIN32
	#include     <stdio.h>      /*��׼�����������*/
	#include     <stdlib.h>     /*��׼�����ⶨ��*/
	#include     <unistd.h>     /*Unix��׼��������*/
	#include     <sys/types.h>  /**/
	#include     <sys/stat.h>   /**/
	#include     <fcntl.h>      /*�ļ����ƶ���*/
	#include     <termios.h>    /*PPSIX�ն˿��ƶ���*/
	#include     <errno.h>      /*����Ŷ���*/
#else
	#define FC_DTRDSR       0x01
	#define FC_RTSCTS       0x02
	#define FC_XONXOFF      0x04
	#define ASCII_BEL       0x07
	#define ASCII_BS        0x08
	#define ASCII_LF        0x0A
	#define ASCII_CR        0x0D
	#define ASCII_XON       0x11
	#define ASCII_XOFF      0x13
#endif

#include "SApi.h"
#include "SLog.h"
#include "SDateTime.h"
class SBASE_EXPORT SCom
{
public:
	SCom();
	virtual ~SCom();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �򿪴����豸
	// ��    ��:  �ۿ���
	// ����ʱ��:  2007-11-6
	// ����˵��:  szComDev�����豸��:win:com1/com2... linux:/dev/ttyS0;/dev/ttyS1...(WINƽ̨>=com10ʱʹ��"\\\\.\\COM10")
	//						iBaudrate������,default=9600
	//						cParityУ�鷽ʽ��N��У��,EżУ��,O��У��,SֹͣУ��
	//						iDataBits����λ���ȣ�default=8
	//						iStopBitsֹͣλ����,default=1
	//						iFlowControl��������,default=0,0:��;1:Ӳ��;2:XONXOFF;3:RTS and XonXoff
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool Open(char *szComDev,int iBaudrate=9600,char cParity='N',
		int iDataBits=8,float fStopBits=1,int iFlowControl=0);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �رմ����豸
	// ��    ��:  �ۿ���
	// ����ʱ��:  2007-11-7
	// ����˵��:  
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	bool Close();
	
	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����ָ�����ȵ�����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2007-11-7
	// ����˵��:  �������������ճ���
	// �� �� ֵ:  ʵ�ʽ��յ�������<0��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	int Receive(BYTE *pBuffer,int iNeedBytes);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����ָ��������ָ����С������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2007-11-7
	// ����˵��:  �����������ͳ���
	// �� �� ֵ:  ʵ�ʷ����ĳ���
	//////////////////////////////////////////////////////////////////////////
	int Send(BYTE *pBufer,int iSendBytes);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���ó�ʱʱ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:04
	// ����˵��:  @iRecvTimeout��ʾ���ճ�ʱʱ�䣬��λ����
	//            @iSendTimeout��ʾ���ͳ�ʱʱ�䣬��λ����
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	inline void SetTimeOut(int iRecvTimeout,int iSendTimeout)
	{
		m_iSendTimeout = iSendTimeout;
		m_iRecvTimeout = iRecvTimeout;
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ���ͳ�ʱʱ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:05
	// ����˵��:  void
	// �� �� ֵ:  ���ͳ�ʱʱ�䣬��λ����
	//////////////////////////////////////////////////////////////////////////
	inline int GetSendTimeout(){return m_iSendTimeout;}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ���ճ�ʱʱ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:05
	// ����˵��:  void
	// �� �� ֵ:  ���ճ�ʱʱ�䣬��λ����
	//////////////////////////////////////////////////////////////////////////
	inline int GetRecvTimeout(){return m_iRecvTimeout;}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��ǰ���ڲ������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2011-11-14 15:05
	// ����˵��:  void
	// �� �� ֵ:  �����
	//////////////////////////////////////////////////////////////////////////
	inline DWORD GetHandle()   {return (DWORD)m_hCom;}

private:
#ifdef WIN32
	HANDLE m_hCom;
	OVERLAPPED m_OverlappedRead, m_OverlappedWrite;
#else
	int m_hCom;
#endif
	int m_iSendTimeout,m_iRecvTimeout;//���ͺͽ��յĳ�ʱ����λ����
	bool m_bOpened;
};

#endif /*__SKTCOM_H_*/
