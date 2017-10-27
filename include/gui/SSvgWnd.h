/**
 *
 * 文 件 名 : SSvgWnd.h
 * 创建日期 : 2008-02-15 14:25
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : 通用的窗口类接口，仅定义标准的窗口操作，不具体实现
 * 修改记录 : 
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
	// 描    述:  显示SVG属性窗口
	// 作    者:  邵凯田
	// 创建时间:  2015-8-20 15:35
	// 参数说明:  @pObj为对象指针
	// 返 回 值:  void
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

	virtual void keyPressEvent(QKeyEvent *e);//按键处理
	virtual bool event(QEvent *event);
	//点击回调
	virtual void OnClickObject(SSvgObject *pSvgObj){S_UNUSED(pSvgObj);};
	//鼠标移动上去回调,pSvgObj=NULL表移出所有对象
	virtual void OnMoveToObject(SSvgObject *pSvgObj){S_UNUSED(pSvgObj);};
	//取弹出提示文本回调
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
	// 描    述:  显示全图
	// 作    者:  邵凯田
	// 创建时间:  2015-8-20 11:01
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	void ToFull();
	void ToRaw();
	void ToWidthFull();
	void ToHeightFull();
	void ToExport();

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  将当前所有选中的图元成组
	// 作    者:  邵凯田
	// 创建时间:  2015-8-13 9:59
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void MakeGroup();

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  解除当前选中的成组图元
	// 作    者:  邵凯田
	// 创建时间:  2015-8-13 9:59
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void CancelGroup();

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  复制选中的图元
	// 作    者:  邵凯田
	// 创建时间:  2015-8-13 10:53
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void CopySelected();

	void ShowAlignPoint();
	void HideAlignPoint();
	void CopyCoordPos();

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  将所有选中图元左对齐
	// 作    者:  邵凯田
	// 创建时间:  2015-8-13 14:21
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void AlignLeft();

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  将所有选中图元右对齐
	// 作    者:  邵凯田
	// 创建时间:  2015-8-13 14:22
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void AlignRight();

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  将所有选中图元上对齐
	// 作    者:  邵凯田
	// 创建时间:  2015-8-13 14:22
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void AlignTop();

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  将所有选中图元下对齐
	// 作    者:  邵凯田
	// 创建时间:  2015-8-13 14:22
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void AlignBottom();

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  将所有选中图元水平居中对齐
	// 作    者:  邵凯田
	// 创建时间:  2015-8-13 14:23
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void AlignCenter();

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  将所有选中图元垂直居中对齐
	// 作    者:  邵凯田
	// 创建时间:  2015-8-13 14:24
	// 参数说明:  void
	// 返 回 值:  void
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
	// 描    述:  设置是否显示对齐点,编辑模式有效
	// 作    者:  邵凯田
	// 创建时间:  2015-8-13 14:05
	// 参数说明:  @b
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	inline void SetShowAlignPoint(bool b){m_bShowAlignPoint=b;};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  是否显示对齐点
	// 作    者:  邵凯田
	// 创建时间:  2015-8-13 14:05
	// 参数说明:  void
	// 返 回 值:  bool
	//////////////////////////////////////////////////////////////////////////
	inline bool IsShowAlignPoint(){return m_bShowAlignPoint;};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  删除指定的顶层图元
	// 作    者:  邵凯田
	// 创建时间:  2015-8-13 10:01
	// 参数说明:  @pObj为待删除图元
	//         :  @bDestory为是否释放对象,释放后对象将不能再使用
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void DeleteTopObject(SSvgObject *pObj,bool bDestory=false);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  将所有选中图元按指定的间隔水平排列
	// 作    者:  邵凯田
	// 创建时间:  2015-8-13 14:25
	// 参数说明:  @sep为两个图元间的间隔（即前一图元最右侧与后一图元最左侧的距离），单位为屏幕像素
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void AutoPlaceX(int sep=10);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  将所有图元按指定的间隔垂直排列
	// 作    者:  邵凯田
	// 创建时间:  2015-8-13 14:28
	// 参数说明:  @sep为两个图元间的间隔（即前一图元最下侧与后一图元最上侧的距离），单位为屏幕像素
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void AutoPlaceY(int sep=10);

	void SetShowRootRect(bool b){m_bShowRootRect = b;};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  编辑图元扩展属性
	// 作    者:  邵凯田
	// 创建时间:  2015-8-20 22:22
	// 参数说明:  @pObj为待编辑的对象指针
	//         :  @sExtAttr为扩展属性字符串，引用返回
	// 返 回 值:  true表示已编辑（可能内容并没有变，但只要响应了就必须返回true)，false表示未处理
	//////////////////////////////////////////////////////////////////////////
	virtual bool EditExtAttribute(SSvgObject *pObj,SString &sExtAttr){S_UNUSED(pObj);S_UNUSED(sExtAttr);return false;};
	virtual bool EditUriAttribute(SSvgObject *pObj,SString &sExtAttr){S_UNUSED(pObj);S_UNUSED(sExtAttr);return false;};

	void SetSvgAttributeWnd(SSvgAttributeWnd* pWnd){m_pWndAttribute = pWnd;};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  设置临时复制用的对象指针
	// 作    者:  邵凯田
	// 创建时间:  2015-8-21 22:08
	// 参数说明:  
	// 返 回 值:  
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
	// 描    述:  添加全局SVG对象标题获取函数，根据SVG对象的URI确定对象类型，并返回对象的提示文本
	// 作    者:  邵凯田
	// 创建时间:  2017-7-6 8:54
	// 参数说明:  @pFun为函数指针
	//         :  @cbParam为用户指针
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	static void RegistGlobalSvgObjCaptionCallback(GlobalSvgObjCaptionCallback pFun,void *cbParam);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  去除全局SVG对象标题获取函数注册
	// 作    者:  邵凯田
	// 创建时间:  2017-7-6 9:25
	// 参数说明:  @pFun为函数指针
	//         :  @cbParam为用户指针
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	static void UnregistGlobalSvgObjCaptionCallback(GlobalSvgObjCaptionCallback pFun,void *cbParam);

private:
	int m_iCursorType;//光标操作类型：0-正常，1-添加折线段，2-移动折线某个点
	int m_iPolyLinePoint;//m_iCursorType=2时有效，正在移动的点号，从1开始
	int m_iLineLastX,m_iLineLastY;//画线时的最后一次点，用于画水平、垂直线参考使用
	SString m_sSvgPath;//svg文件的绝对路径
	bool m_bMovable;//是否可移动
	int m_iMoving;//是否正在移动画布，0未移动，1准备移动，2已经移动过且仍准备移动
	int m_iOffsetX,m_iOffsetY;//X、Y方向的偏移
	SPoint m_ptLast;//上一次的点
	SPoint m_ptThis;//这一次的点
	SSvgObject *m_pThisObj;//当前操作的SVG图元
	bool m_bResizing;//是否正在改变图元大小
	int m_iResizeMode;//改变图元大小的方式，0：未改变，1：左上角，2：上，3：右上角，4：右，5：右下，6：下，7：左下，8：左
	bool m_bObjMoving;//是否在移动图元
	int m_iMouseMoveTimes;
	bool m_bMouseDown;//是否鼠标是压下状态
	bool m_bDrawSelectRange;//是否绘制选择区域
	bool m_bShowAlignPoint;//是否绘制对齐点
	bool m_bShowRootRect;//是绘制范围线条
	SSvgAttributeWnd *m_pWndAttribute;
	SSvgObject *m_pTempCopyObject;//临时复制用的对象指针，一般为图元库的指针
	QPixmap m_pixmapWnd;//窗口位图
	QPixmap m_pixmapWnd2;//窗口边沿位图，用于在窗口位置移动后绘制边沿
	QSize m_PixmapSizeWnd;//窗口位图大小
	int m_iPixmapOffX,m_iPixmapOffY;
	SString m_sRClickPos;
};


#endif//__YS_SVGWND_INC_H__
