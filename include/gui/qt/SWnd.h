/**
 *
 * �� �� �� : SWnd.h
 * �������� : 2007-12-20 15:22
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : 
 * �޸ļ�¼ : 
 *            $Log: $
 *
 **/

#ifndef __SKT_QT_WND_H__
#define __SKT_QT_WND_H__

#ifndef QT_GUI_LIB
#error "You cann't use qt svg code under non-qt plateform!!!"
#endif

#include "../SBaseWnd.h"
#include "SDC.h"

#pragma once
#include <QWidget>

class SBASE_QTGUI_EXPORT SWnd : public QWidget,public SBaseWnd
{
	Q_OBJECT
public:
#ifdef WIN32_UNUSED
	struct SBASE_QTGUI_EXPORT stuWndAttach
	{
		HWND hWnd;
		SWnd *pWnd;
	};
	HWND m_hWnd;
	HDC	 m_hMemDC;
#endif
public:
	SWnd()
	{
#ifdef WIN32_UNUSED
		m_hWnd = NULL;
		m_hMemDC = NULL;
#endif
		m_pParentWnd = NULL;
		m_bUserDraw = true;
		//m_pTimer = new QTimer(this);
		//connect(m_pTimer,SIGNAL(timeout()),this,SLOT(mouseClick()));
	}
	SWnd(QWidget *parent) :QWidget(parent)
	{
#ifdef WIN32_UNUSED
		m_hWnd = NULL;
		m_hMemDC = NULL;
#endif
		m_pParentWnd = (SWnd*)parent;
		m_bUserDraw = true;
	}
	virtual ~SWnd()
	{
	}
	void mousePressEvent(QMouseEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
	void  mouseReleaseEvent ( QMouseEvent * event );

	//public slots:
	//void mouseClick();

public:

	void  mouseMoveEvent ( QMouseEvent * event );
	void wheelEvent(QWheelEvent *event);
	void paintEvent(QPaintEvent *event);
	virtual void resizeEvent(QResizeEvent * event);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  
	// ��    ��:  �ۿ���
	// ����ʱ��:  2007-12-20
	// ����˵��:  
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	virtual bool RedrawWindow();

#ifdef WIN32_UNUSED
	static SWnd* GetAttachWnd(HWND hWnd);
	static void AddAttachWnd(HWND hWnd,SWnd* pWnd);
	static void RemoveAttachWnd(HWND hWnd);
	static void RemoveAttachWnd(SWnd* pWnd);
	bool IsWindow()
	{
		return m_hWnd == NULL;
	}
	inline SWnd* GetParentWindow()
	{
		return m_pParentWnd;
	}
	inline void SetUserDraw(bool b)
	{
		m_bUserDraw = b;
	}
	virtual bool IsWindowVisible()
	{
		return ((bool)::IsWindowVisible(m_hWnd));
	}
	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2007-12-20
	// ����˵��:  
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	virtual bool Create(char *sClassName,char *sWinName,unsigned long style,
		int x,int y,int w,int h,
		SWnd *pParentWnd=NULL,unsigned int nID=0,unsigned long dwAddData=0);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ʾ����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2007-12-20
	// ����˵��:  
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	virtual bool ShowWindow( int nCmdShow );

#endif

	virtual SBaseDC* CreateDC();
	virtual void ReleaseDC(SBaseDC *pDC);
	virtual bool OnPaint(SBaseDC *pDC){S_UNUSED(pDC);return true;};

#ifdef WIN32_UNUSED
	virtual bool OnCommand(DWORD wParam, DWORD lParam){return true;};
	virtual bool OnMessage(int message,DWORD wParam, DWORD lParam){return true;};
	virtual int OnCreate();
	virtual int OnClose(){return 1;};
	virtual int OnSize(int type,int cx,int cy){return 1;};
	virtual int OnKeyDown(int nVirtKey,int iKeyData){return 1;};
	virtual int OnKeyUp(int nVirtKey,int iKeyData){return 1;};
	virtual int OnLButtonDblClk(int nFlag,SPoint point){return 1;};
	virtual int OnLButtonDown(int nFlag,SPoint point){return 1;};
	virtual int OnLButtonUp(int nFlag,SPoint point){return 1;};
	virtual int OnMouseMove(int nFlag,SPoint point){return 1;};
	virtual int OnMouseWheel(int nFlag,int delta){return 1;}
	virtual int OnRButtonDblClk(int nFlag,SPoint point){return 1;};
	virtual int OnRButtonDown(int nFlag,SPoint point){return 1;};
	virtual int OnRButtonUp(int nFlag,SPoint point){return 1;};
	virtual int OnNmclick(int nFlag,SPoint point){return 1;};//qcj	
	virtual int OnVscroll(DWORD wParam, DWORD lParam){return 1;};
    virtual int OnHscroll(DWORD wParam, DWORD lParam){return 1;};
	virtual int OnTimer(int nIDEvent){return 1;};
	virtual bool SetTimer(int nIDEvent,int nElapse);
	virtual bool KillTimer(int nIDEvent);
	virtual bool SendMessage(int message,void* wParam,void* lParam);
	int MessageBox(char* sMsg,char* sTitle,DWORD style);
#endif
	SRECT GetClientRect();
private:
#ifdef WIN32_UNUSED
	static LRESULT CALLBACK SWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
#endif
	SWnd *m_pParentWnd;
	bool m_bUserDraw;//�Ƿ��û��Ի�����
//	QTimer *m_pTimer;
//	int m_iMouseAct;//1���£�2�Ұ��£�3����4����
//	int m_x,m_y;//���ĵ��λ��
};

#endif //__SKT_QT_WND_H__

