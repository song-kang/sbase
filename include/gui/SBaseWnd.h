/**
 *
 * �� �� �� : SBaseWnd.h
 * �������� : 2007-12-20 14:25
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : ͨ�õĴ�����ӿڣ��������׼�Ĵ��ڲ�����������ʵ��
 * �޸ļ�¼ : 
 *            $Log: $
 *
 **/

#ifndef __SKT_BASE_WND_INC_H__
#define __SKT_BASE_WND_INC_H__

#include "SBaseDC.h"
#include "../SApi.h"
class SBASE_QTGUI_EXPORT SBaseWnd
{
public:
	SBaseWnd()
	{
	}
	virtual ~SBaseWnd()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���麯�����ɵײ���ʵ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2008-1-18
	// ����˵��:  
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	virtual bool IsWindowVisible(){return false;};
	virtual SBaseDC* CreateDC(){return NULL;};
	virtual void ReleaseDC(SBaseDC *pDC){S_UNUSED(pDC);};
	virtual bool OnPaint(SBaseDC *pDC){S_UNUSED(pDC);return true;};
	virtual bool OnCommand(DWORD wParam, DWORD lParam){S_UNUSED(wParam);S_UNUSED(lParam);return true;};
	virtual bool OnMessage(int message,DWORD wParam, DWORD lParam){S_UNUSED(message);S_UNUSED(wParam);S_UNUSED(lParam);return true;};
	virtual int OnCreate(){return 1;};
	virtual int OnClose(){return 1;};
	virtual int OnSize(int type,int cx,int cy){S_UNUSED(type);S_UNUSED(cx);S_UNUSED(cy);return 1;};
	virtual int OnKeyDown(int nVirtKey,int iKeyData){S_UNUSED(nVirtKey);S_UNUSED(iKeyData);return 1;};
	virtual int OnKeyUp(int nVirtKey,int iKeyData){S_UNUSED(nVirtKey);S_UNUSED(iKeyData);return 1;};
	virtual int OnLButtonDblClk(int nFlag,SPoint point){S_UNUSED(nFlag);S_UNUSED(point);return 1;};
	virtual int OnLButtonDown(int nFlag,SPoint point){S_UNUSED(nFlag);S_UNUSED(point);return 1;};
	virtual int OnLButtonUp(int nFlag,SPoint point){S_UNUSED(nFlag);S_UNUSED(point);return 1;};
	virtual int OnMouseMove(int nFlag,SPoint point){S_UNUSED(nFlag);S_UNUSED(point);return 1;};
	virtual int OnMouseWheel(int nFlag,int delta){S_UNUSED(nFlag);S_UNUSED(delta);return 1;}
	virtual int OnRButtonDblClk(int nFlag,SPoint point){S_UNUSED(nFlag);S_UNUSED(point);return 1;};
	virtual int OnRButtonDown(int nFlag,SPoint point){S_UNUSED(nFlag);S_UNUSED(point);return 1;};
	virtual int OnRButtonUp(int nFlag,SPoint point){S_UNUSED(nFlag);S_UNUSED(point);return 1;};
	virtual int OnTimer(int nIDEvent){S_UNUSED(nIDEvent);return 1;};
	virtual bool SendMessage(int message,void* wParam,void* lParam){S_UNUSED(message);S_UNUSED(wParam);S_UNUSED(lParam);return true;};
	virtual bool SetTimer(int nIDEvent,int nElapse){S_UNUSED(nIDEvent);S_UNUSED(nElapse);return 1;};

private:
	
};



#endif//__SKT_BASE_WND_INC_H__
