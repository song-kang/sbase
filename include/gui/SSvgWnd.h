/**
 *
 * �� �� �� : SSvgWnd.h
 * �������� : 2008-02-15 14:25
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : ͨ�õĴ�����ӿڣ��������׼�Ĵ��ڲ�����������ʵ��
 * �޸ļ�¼ : 
 *            $Log: $
 *
 **/

#ifndef __SKT_SVGWND_INC_H__
#define __SKT_SVGWND_INC_H__

#include "SGui.h"
#include "svg/SSvgDefine.h"
#include "svg/SSvgDocument.h"
#include "svg/SSvgPainter.h"
#include "qt/SSvgAttributeWnd.h"

typedef bool (*GlobalSvgObjCaptionCallback)(void* cbParam,SSvgObject *pObj,SString &caption);

class SBASE_QTGUI_EXPORT SSvgWnd : public SWnd, public SSvgPainter
{
#ifdef QT_GUI_LIB
		Q_OBJECT
#endif
public:
	struct SBASE_QTGUI_EXPORT stuGlobalSvgObjCaptionCallback
	{
		GlobalSvgObjCaptionCallback pFun;
		void* cbParam;
	};

	bool SetSvgText(SString sCaption,SString sText);
	void SetMovable(bool b);
	SSvgWnd();
#ifdef QT_GUI_LIB
	SSvgWnd(QWidget *parent);
#endif
	virtual ~SSvgWnd();

	inline void SetSvgPath(SString sPath){m_sSvgPath = sPath;};
	bool Load(SString sSvgFile);
	bool Load(SBaseConfig *pRootNode);
	bool LoadText(const char* sSvgText,bool bRedrawNow=true);
	
	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ʾSVG���Դ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-20 15:35
	// ����˵��:  @pObjΪ����ָ��
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void ShowAttributeWnd(SSvgObject *pObj,bool bShowAttrDlg=true);
	void HideAttributeWnd();
	inline void RefreshAttributeWnd()
	{
		if(m_pWndAttribute != NULL && m_pWndAttribute->isVisible())
		{
			ShowAttributeWnd(m_pThisObj);
		}
	}

	SSvgObject* GetSvgObjectByCaption(SString sCaption);
	SSvgObject* GetSvgObjectByUri(SString sUri);
	
	virtual bool OnPaint(SBaseDC *pDC);
	virtual int OnLButtonDown(int nFlag,SPoint point);
	virtual int OnLButtonUp(int nFlag,SPoint point);
	virtual int OnMouseMove(int nFlag,SPoint point);
	virtual int OnMouseWheel(int nFlag,int delta);
	virtual int OnRButtonDown(int nFlag,SPoint point);
	virtual int OnSize(int type,int cx,int cy);

	virtual void keyPressEvent(QKeyEvent *e);//��������
	virtual bool event(QEvent *event);
	//����ص�
	virtual void OnClickObject(SSvgObject *pSvgObj){S_UNUSED(pSvgObj);};
	//����ƶ���ȥ�ص�,pSvgObj=NULL���Ƴ����ж���
	virtual void OnMoveToObject(SSvgObject *pSvgObj){S_UNUSED(pSvgObj);};
	//ȡ������ʾ�ı��ص�
	virtual SString GetSvgObjectTipText(SSvgObject *pSvgObj);

	virtual int OnTimer(int nIDEvent){S_UNUSED(nIDEvent);/*printf("@@@@@@@@@@@@@@@@@@@@@SSvgWnd::OnTimer(),this=%p\n",this);*/return 1;};
	SPoint  GetOffsetPoint();
	void SetOffset(int x,int y)
	{
		m_iOffsetX = x;
		m_iOffsetY = y;
	}
	int GetOffsetX(){return m_iOffsetX;}
	int GetOffsetY(){return m_iOffsetY;}

#ifdef QT_GUI_LIB
signals:
	void sigCursorPos(float x,float y);
public slots:
#endif
	void ToCenter();
	void ToTopCenter();
	void OnTimerRepaint();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ʾȫͼ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-20 11:01
	// ����˵��:  
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	void ToFull();
	void ToRaw();
	void ToWidthFull();
	void ToHeightFull();
	void ToExport();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����ǰ����ѡ�е�ͼԪ����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-13 9:59
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void MakeGroup();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����ǰѡ�еĳ���ͼԪ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-13 9:59
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void CancelGroup();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����ѡ�е�ͼԪ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-13 10:53
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void CopySelected();

	void ShowAlignPoint();
	void HideAlignPoint();
	void CopyCoordPos();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ������ѡ��ͼԪ�����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-13 14:21
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void AlignLeft();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ������ѡ��ͼԪ�Ҷ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-13 14:22
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void AlignRight();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ������ѡ��ͼԪ�϶���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-13 14:22
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void AlignTop();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ������ѡ��ͼԪ�¶���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-13 14:22
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void AlignBottom();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ������ѡ��ͼԪˮƽ���ж���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-13 14:23
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void AlignCenter();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ������ѡ��ͼԪ��ֱ���ж���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-13 14:24
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void AlignMiddle();

	void OnAutoPlaceX();
	void OnAutoPlaceY();

	void DelSelected();

	void SetSameWidth();
	void SetSameHeight();
	void SetSameSize();

public:

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����Ƿ���ʾ�����,�༭ģʽ��Ч
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-13 14:05
	// ����˵��:  @b
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	inline void SetShowAlignPoint(bool b){m_bShowAlignPoint=b;};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �Ƿ���ʾ�����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-13 14:05
	// ����˵��:  void
	// �� �� ֵ:  bool
	//////////////////////////////////////////////////////////////////////////
	inline bool IsShowAlignPoint(){return m_bShowAlignPoint;};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ɾ��ָ���Ķ���ͼԪ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-13 10:01
	// ����˵��:  @pObjΪ��ɾ��ͼԪ
	//         :  @bDestoryΪ�Ƿ��ͷŶ���,�ͷź���󽫲�����ʹ��
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void DeleteTopObject(SSvgObject *pObj,bool bDestory=false);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ������ѡ��ͼԪ��ָ���ļ��ˮƽ����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-13 14:25
	// ����˵��:  @sepΪ����ͼԪ��ļ������ǰһͼԪ���Ҳ����һͼԪ�����ľ��룩����λΪ��Ļ����
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void AutoPlaceX(int sep=10);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ������ͼԪ��ָ���ļ����ֱ����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-13 14:28
	// ����˵��:  @sepΪ����ͼԪ��ļ������ǰһͼԪ���²����һͼԪ���ϲ�ľ��룩����λΪ��Ļ����
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void AutoPlaceY(int sep=10);

	void SetShowRootRect(bool b){m_bShowRootRect = b;};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �༭ͼԪ��չ����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-20 22:22
	// ����˵��:  @pObjΪ���༭�Ķ���ָ��
	//         :  @sExtAttrΪ��չ�����ַ��������÷���
	// �� �� ֵ:  true��ʾ�ѱ༭���������ݲ�û�б䣬��ֻҪ��Ӧ�˾ͱ��뷵��true)��false��ʾδ����
	//////////////////////////////////////////////////////////////////////////
	virtual bool EditExtAttribute(SSvgObject *pObj,SString &sExtAttr){S_UNUSED(pObj);S_UNUSED(sExtAttr);return false;};
	virtual bool EditUriAttribute(SSvgObject *pObj,SString &sExtAttr){S_UNUSED(pObj);S_UNUSED(sExtAttr);return false;};

	void SetSvgAttributeWnd(SSvgAttributeWnd* pWnd){m_pWndAttribute = pWnd;};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ������ʱ�����õĶ���ָ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-21 22:08
	// ����˵��:  
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	void SetTempCopyObject(SSvgObject *p);

#ifdef QT_GUI_LIB
public slots:
	void OnNewText();
	void OnNewRectText();
	void OnNewRect();
	void OnNewEllipse();
	void OnNewLine();
	void OnNewPolyline();
	void OnNewInnerImage();
	void OnNewHrefImage();
#endif

	void CloneObjectImageRes(SSvgObject *pObj,bool bWithNext);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���ȫ��SVG��������ȡ����������SVG�����URIȷ���������ͣ������ض������ʾ�ı�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-7-6 8:54
	// ����˵��:  @pFunΪ����ָ��
	//         :  @cbParamΪ�û�ָ��
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	static void RegistGlobalSvgObjCaptionCallback(GlobalSvgObjCaptionCallback pFun,void *cbParam);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȥ��ȫ��SVG��������ȡ����ע��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-7-6 9:25
	// ����˵��:  @pFunΪ����ָ��
	//         :  @cbParamΪ�û�ָ��
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	static void UnregistGlobalSvgObjCaptionCallback(GlobalSvgObjCaptionCallback pFun,void *cbParam);

private:
	int m_iCursorType;//���������ͣ�0-������1-������߶Σ�2-�ƶ�����ĳ����
	int m_iPolyLinePoint;//m_iCursorType=2ʱ��Ч�������ƶ��ĵ�ţ���1��ʼ
	int m_iLineLastX,m_iLineLastY;//����ʱ�����һ�ε㣬���ڻ�ˮƽ����ֱ�߲ο�ʹ��
	SString m_sSvgPath;//svg�ļ��ľ���·��
	bool m_bMovable;//�Ƿ���ƶ�
	int m_iMoving;//�Ƿ������ƶ�������0δ�ƶ���1׼���ƶ���2�Ѿ��ƶ�������׼���ƶ�
	int m_iOffsetX,m_iOffsetY;//X��Y�����ƫ��
	SPoint m_ptLast;//��һ�εĵ�
	SPoint m_ptThis;//��һ�εĵ�
	SSvgObject *m_pThisObj;//��ǰ������SVGͼԪ
	bool m_bResizing;//�Ƿ����ڸı�ͼԪ��С
	int m_iResizeMode;//�ı�ͼԪ��С�ķ�ʽ��0��δ�ı䣬1�����Ͻǣ�2���ϣ�3�����Ͻǣ�4���ң�5�����£�6���£�7�����£�8����
	bool m_bObjMoving;//�Ƿ����ƶ�ͼԪ
	int m_iMouseMoveTimes;
	bool m_bMouseDown;//�Ƿ������ѹ��״̬
	bool m_bDrawSelectRange;//�Ƿ����ѡ������
	bool m_bShowAlignPoint;//�Ƿ���ƶ����
	bool m_bShowRootRect;//�ǻ��Ʒ�Χ����
	SSvgAttributeWnd *m_pWndAttribute;
	SSvgObject *m_pTempCopyObject;//��ʱ�����õĶ���ָ�룬һ��ΪͼԪ���ָ��
	QPixmap m_pixmapWnd;//����λͼ
	QPixmap m_pixmapWnd2;//���ڱ���λͼ�������ڴ���λ���ƶ�����Ʊ���
	QSize m_PixmapSizeWnd;//����λͼ��С
	int m_iPixmapOffX,m_iPixmapOffY;
	SString m_sRClickPos;
};


#endif//__YS_SVGWND_INC_H__
