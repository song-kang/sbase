/**
 *
 * �� �� �� : SPicture.h
 * �������� : 2008-02-15 14:25
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : ͨ�õĴ�����ӿڣ��������׼�Ĵ��ڲ�����������ʵ��
 * �޸ļ�¼ : 
 *            $Log: $
 *
 **/

#ifndef __SKT_PICTURE_INC_H__
#define __SKT_PICTURE_INC_H__

#include "SBaseDC.h"

#ifdef QT_GUI_LIB
#include "qt/SDC.h"
#include <QPixmap>
#else
#ifndef WIN32 
#include "qt/SDC.h"
#endif

#endif


class SBASE_QTGUI_EXPORT SPicture
{
public:
	SPicture();
	~SPicture();
	bool Load(const char* sFileName);
	void Render(unsigned long uDC,int x,int y,int width,int height,int alpha=255);//alpha=255��ʾ��͸����=0��ʾȫ͸��

	inline int GetWidth()		{return m_iWidth;};
	inline int GetHeight()	{return m_iHeight;};
public:
#ifdef QT_GUI_LIB
	QPixmap m_pixmap;
#else
#ifdef WIN32
	HANDLE m_hBitmap;	//λͼ���
	HDC m_hDCMem;			//�ڴ�DC
#else
	BITMAP *m_pBitmap;
#endif
#endif
	int m_iWidth,m_iHeight;//λͼԭʼ��С
};


#endif//__YS_PICTURE_INC_H__
