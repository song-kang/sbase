/**
 *
 * 文 件 名 : SSvgWnd.cpp
 * 创建日期 : 2008-02-15 14:25
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : 通用的窗口类接口，仅定义标准的窗口操作，不具体实现
 * 修改记录 : 
 *            $Log: $
 *
 **/
#include "SSvgWnd.h"
#include <QApplication>
#include "SQt.h"

SPtrList<SSvgWnd::stuGlobalSvgObjCaptionCallback> g_GlobalSvgObjCaptionCallback;

SSvgWnd::SSvgWnd()
{
	m_bMovable = false;
	m_iMoving = 0;
	m_iOffsetX = m_iOffsetY = 0;
	m_bResizing = false;
	m_iResizeMode = 0;
	m_bObjMoving = false;
	m_iMouseMoveTimes = 0;
	m_bMouseDown = false;
	m_bShowAlignPoint = false;
	m_bDrawSelectRange = false;
	m_bShowRootRect = false;
	m_pWndAttribute = NULL;
	m_pTempCopyObject = NULL;
	m_iCursorType = 0;
	m_PixmapSizeWnd = QSize(0,0);//位图大小
	m_iPixmapOffX=m_iPixmapOffY = 0;
	m_bRender = true;
	m_bDrawOutPixmapRect = false;
	m_pRedrawTimer = new QTimer();
	connect(m_pRedrawTimer,SIGNAL(timeout()),this,SLOT(OnTimerRepaint()));
	setMouseTracking(true);
	g_GlobalSvgObjCaptionCallback.setAutoDelete(true);
#ifdef QT_GUI_LIB
	m_slEmbeddedWnd.setAutoDelete(true);
	m_slEmbeddedWnd.setShared(true);
#endif
}

#ifdef QT_GUI_LIB
SSvgWnd::SSvgWnd(QWidget *parent) : SWnd(parent)
{
	m_bMovable = false;
	m_iMoving = 0;
	m_iOffsetX = m_iOffsetY = 0;
	m_bResizing = false;
	m_iResizeMode = 0;
	m_bObjMoving = false;
	m_iMouseMoveTimes = 0;
	m_bMouseDown = false;
	m_bShowAlignPoint = false;
	m_bDrawSelectRange = false;
	m_bShowRootRect = false;
	m_pWndAttribute = NULL;
	m_pTempCopyObject = NULL;
	m_iCursorType = 0;
	m_PixmapSizeWnd = QSize(0,0);//位图大小
	m_iPixmapOffX=m_iPixmapOffY = 0;
	m_bRender = true;
	m_bDrawOutPixmapRect = false;
	m_pRedrawTimer = new QTimer();
	connect(m_pRedrawTimer,SIGNAL(timeout()),this,SLOT(OnTimerRepaint()));
	setMouseTracking(true);
	g_GlobalSvgObjCaptionCallback.setAutoDelete(true);
}
//////////////////////////////////////////////////////////////////////////
// 描    述:  向SVG窗口中pObj对象对应位置插入一个嵌入式窗口
// 作    者:  邵凯田
// 创建时间:  2017-12-7 8:53
// 参数说明:  @pObj为等插入对象的窗口
//         :  @pWnd为窗口指针
// 返 回 值:  true表示成功，false表示失败
//////////////////////////////////////////////////////////////////////////
bool SSvgWnd::AddEmbeddedWnd(SSvgObject *pObj,QWidget *pWnd)
{
	if(pObj == NULL || pWnd == NULL)
		return false;
	stuEmbeddedWnd *p = new stuEmbeddedWnd();
	p->pObj = pObj;
	p->pEmbeddedWnd = pWnd;
	m_slEmbeddedWnd.append(p);
	pWnd->setParent(this);
	ResizeEmbeddedWnd(p->pObj,p->pEmbeddedWnd);
	p->pEmbeddedWnd->show();
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  重布置所有嵌入式窗口
// 作    者:  邵凯田
// 创建时间:  2017-12-7 9:07
// 参数说明:  void
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SSvgWnd::ResizeAllEmbeddedWnd()
{
	unsigned long pos;
	stuEmbeddedWnd *p = m_slEmbeddedWnd.FetchFirst(pos);
	while(p)
	{
		ResizeEmbeddedWnd(p->pObj,p->pEmbeddedWnd);
		p = m_slEmbeddedWnd.FetchNext(pos);
	}
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  重布指定的嵌入式窗口
// 作    者:  邵凯田
// 创建时间:  2017-12-7 9:10
// 参数说明:  @pObj为等插入对象的窗口
//         :  @pWnd为窗口指针
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SSvgWnd::ResizeEmbeddedWnd(SSvgObject *pObj,QWidget *pWnd)
{
	if(pObj == NULL || pWnd == NULL)
		return;
	SRect rect;
	rect.left = GetCoordX(pObj->GetAttributeF("x"));
	rect.top = GetCoordY(pObj->GetAttributeF("y"));
	rect.right = GetCoordX(pObj->GetAttributeF("x")+pObj->GetAttributeF("width"));
	rect.bottom = GetCoordY(pObj->GetAttributeF("y")+pObj->GetAttributeF("height"));
	rect.OffsetRect(m_iOffsetX,m_iOffsetY);
	pWnd->setGeometry(QRect(rect.left, rect.top, rect.width(), rect.height()));
}

#endif
SSvgWnd::~SSvgWnd()
{
	if(m_pRedrawTimer != NULL)
		delete m_pRedrawTimer;
	SetTempCopyObject(NULL);
	if(m_pWndAttribute != NULL)
		delete m_pWndAttribute;
#ifdef QT_GUI_LIB
	m_slEmbeddedWnd.clear();
#endif
}

void SSvgWnd::OnTimerRepaint()
{
	m_iAutoFlashType = m_iAutoFlashType==1?2:1;
	RedrawWindow();
}

bool SSvgWnd::Load(SString sSvgFile)
{
	LOGDEBUG("Load svg:%s",sSvgFile.data());
	m_sSvgPath = sSvgFile;
	SetBaseWnd((SBaseWnd *)this);
	// sSvgFile="d:/znl4monitor/svg/null.svg";
	if(!m_Document.LoadSvgFile(sSvgFile))
		return false;
	Render();
	ToCenter();
	setMouseTracking(true);
	return true;
}

bool SSvgWnd::Load(SBaseConfig *pRootNode)
{
	LOGDEBUG("Load svg by xmlnode");
	SetBaseWnd((SBaseWnd *)this);
	if(!m_Document.LoadSvgCfg(pRootNode))
		return false;
	Render();
	RedrawWindow();
	//ToCenter();
	setMouseTracking(true);
	return true;
}

bool SSvgWnd::LoadText(const char* sSvgText,bool bRedrawNow/*=true*/)
{
	LOGDEBUG("Load svg text");
	SetBaseWnd((SBaseWnd *)this);
	// sSvgFile="d:/znl4monitor/svg/null.svg";
	if(!m_Document.LoadSvgText(sSvgText))
		return false;
	if(bRedrawNow)
	{
		Render();
		RedrawWindow();
	}
	//ToCenter();
	setMouseTracking(true);
	return true;
}

bool SSvgWnd::OnPaint(SBaseDC *pDC)
{
//	pDC->TextOutA(10,10,"My QT SVG!");
	SSvgObject *pFirstObj = m_Document.GetRootObject();
	if(pFirstObj != NULL)
		pFirstObj = pFirstObj->GetChild();
//	if(pFirstObj != NULL)
//		pFirstObj = pFirstObj->GetNext();
	if( pFirstObj == NULL)
		return true;
	bool bNeetAutoFlash = false;
	SSvgObject *pTmpObj;
// 	pTmpObj = m_SelectedObjList[0];
// 	if(pTmpObj != NULL)
// 		pTmpObj->SetAutoFlash(true);
	pTmpObj = pFirstObj->GetNext();
	while(pTmpObj != NULL)
	{
		if(pTmpObj->IsAutoFlash())
		{
			bNeetAutoFlash = true;
			break;
		}
		pTmpObj = pTmpObj->GetNext();
	}
	if(bNeetAutoFlash)
	{
		if(m_iAutoFlashType == 0)
		{
			m_iAutoFlashType = 1;
			m_pRedrawTimer->start(500);
		}
// 		int soc,usec,ms;
// 		SDateTime::getSystemTime(soc,usec);
// 		ms = abs((soc-m_iLastDrawSoc)*1000+(usec-m_iLastDrawUSec)/1000);
// 		if(m_iAutoFlashType == 1)
// 		{
// 			if(ms >= 450)
// 			{
// 				m_iLastDrawSoc = soc;
// 				m_iLastDrawUSec = usec;
// 				m_iAutoFlashType = m_iAutoFlashType==1?2:1;
// 			}
// 		}
// 		else
// 		{
// 			if(ms >= 450)
// 			{
// 				m_iLastDrawSoc = soc;
// 				m_iLastDrawUSec = usec;
// 				m_iAutoFlashType = m_iAutoFlashType==1?2:1;
// 			}
// 		}
	}
	else
	{
		if(m_iAutoFlashType != 0)
		{
			m_pRedrawTimer->stop();
			m_iAutoFlashType = 0;
		}
	}

	if(m_PixmapSizeWnd != size())//parentWidget()->
	{
		m_pixmapWnd = QPixmap(size());
		m_pixmapWnd2 = QPixmap(size());
		m_pixmapWnd.fill(Qt::transparent);
		m_pixmapWnd2.fill(Qt::transparent);
		m_PixmapSizeWnd.setWidth(m_pixmapWnd.size().width());
		m_PixmapSizeWnd.setHeight(m_pixmapWnd.size().height());
		m_bRender = true;
	}
	SCOLOR colBack;
	SCOLOR colFrant;

	SRECT r = GetClientRect();

	m_ViewRect.left =GetSvgCoord((double)-m_iOffsetX);
	m_ViewRect.top = GetSvgCoord((double)-m_iOffsetY );
	m_ViewRect.right = m_ViewRect.left + GetSvgCoord((double)(r.right-r.left/*-m_iOffsetX*/));
	m_ViewRect.bottom = m_ViewRect.top + GetSvgCoord((double)(r.bottom-r.top/*-m_iOffsetY*/));
	if(m_bEditAllow && m_SelectedObjList.count() > 10)
		m_bRender = true;
	if(!m_bRender && (
		m_PixmapRect.right < (m_ViewRect.right+m_ViewRect.left)/2 ||
		m_PixmapRect.left > (m_ViewRect.right+m_ViewRect.left)/2 ||
		m_PixmapRect.top > (m_ViewRect.top+m_ViewRect.bottom)/2 ||
		m_PixmapRect.bottom < (m_ViewRect.top+m_ViewRect.bottom)/2 ) )
		m_bRender = true;
	SString sFill = pFirstObj->GetAttribute("fill");
	colBack = GetColorByString(sFill);
	colFrant.r = 255 - colBack.r;
	colFrant.g = 255 - colBack.g;
	colFrant.b = 255 - colBack.b;

	SDC *pQtDc = (SDC*)pDC;
	QPainter *pWndPainter = pQtDc->m_hDC;
	if(sFill.length() > 0 && sFill != "none")
		pDC->FillRect(0,0,r.right-r.left,r.bottom-r.top,colBack);
	if(m_bRender)
	{
		m_PixmapRect = m_ViewRect;
		m_pixmapWnd.fill(Qt::transparent);
		QPainter *pPixmapPainter = new QPainter(&m_pixmapWnd);
		pQtDc->m_hDC = pPixmapPainter;
		if(sFill.length() > 0 && sFill != "none")
			pDC->FillRect(0,0,r.right-r.left,r.bottom-r.top,colBack);
		m_iDrawUnSelectedMode = 2;
		if(m_Document.GetOrigX() != 0)
			pFirstObj->SetAttributeValue("x",SString::toFormat("%f",pFirstObj->GetAttributeF("x")+m_Document.GetOrigX()));
		if(m_Document.GetOrigY() != 0)
			pFirstObj->SetAttributeValue("y",SString::toFormat("%f",pFirstObj->GetAttributeF("y")+m_Document.GetOrigY()));
		DrawObject(pDC,m_bShowRootRect?pFirstObj:pFirstObj->GetNext(),m_iOffsetX,m_iOffsetY);
		if(m_Document.GetOrigX() != 0)
			pFirstObj->SetAttributeValue("x",SString::toFormat("%f",pFirstObj->GetAttributeF("x")-m_Document.GetOrigX()));
		if(m_Document.GetOrigY() != 0)
			pFirstObj->SetAttributeValue("y",SString::toFormat("%f",pFirstObj->GetAttributeF("y")-m_Document.GetOrigY()));
		m_iDrawUnSelectedMode = 0;
		pQtDc->m_hDC = pWndPainter;
		m_iPixmapOffX = m_iOffsetX;
		m_iPixmapOffY = m_iOffsetY;
		delete pPixmapPainter;
	}
	
	pWndPainter->drawPixmap(m_iOffsetX-m_iPixmapOffX,m_iOffsetY-m_iPixmapOffY,m_pixmapWnd,0,0,m_pixmapWnd.width(),m_pixmapWnd.height());
	if(!m_bRender && (m_iOffsetX-m_iPixmapOffX != 0 || m_iOffsetY-m_iPixmapOffY != 0))
	{
		m_pixmapWnd2.fill(Qt::transparent);
		QPainter *pPixmapPainter = new QPainter(&m_pixmapWnd2);
		pQtDc->m_hDC = pPixmapPainter;

		m_bDrawOutPixmapRect = true;
		if(sFill.length() > 0 && sFill != "none")
			pDC->FillRect(0,0,r.right-r.left,r.bottom-r.top,colBack);
		DrawObject(pDC,m_bShowRootRect?pFirstObj:pFirstObj->GetNext(),m_iOffsetX,m_iOffsetY);
		m_bDrawOutPixmapRect = false;

		pQtDc->m_hDC = pWndPainter;
		delete pPixmapPainter;

		//复制边沿位图
		int sep = m_iOffsetX-m_iPixmapOffX;
		if(sep > 0)
		{
			//左边沿
			pWndPainter->drawPixmap(0,0,m_pixmapWnd2,0,0,sep,m_pixmapWnd2.height());
		}
		else if(sep < 0)
		{
			sep = -sep;
			//右边沿
			pWndPainter->drawPixmap(m_pixmapWnd.width()-sep,0,m_pixmapWnd2,m_pixmapWnd.width()-sep,0,sep,m_pixmapWnd2.height());
		}
		sep = m_iOffsetY-m_iPixmapOffY;
		if(sep > 0)
		{
			//上边沿
			pWndPainter->drawPixmap(0,0,m_pixmapWnd2,0,0,m_pixmapWnd2.width(),sep);
		}
		else if(sep < 0)
		{
			sep = -sep;
			//下边沿
			pWndPainter->drawPixmap(0,m_pixmapWnd.height()-sep,m_pixmapWnd2,0,m_pixmapWnd.height()-sep,m_pixmapWnd2.width(),sep);
		}
	}
	if(m_bRender)
		m_bRender = false;

	//直接绘制选中图元
	m_iDrawUnSelectedMode = 1;
	DrawObject(pDC,m_bShowRootRect?pFirstObj:pFirstObj->GetNext(),m_iOffsetX,m_iOffsetY);
	m_iDrawUnSelectedMode = 0;

	DrawFlashObject(pDC,pFirstObj,m_iOffsetX,m_iOffsetY);
	if(m_bEditAllow)
	{
		unsigned long pos=0;
		SSvgObject *pObj = m_SelectedObjList.FetchFirst(pos);
		if(pObj)
		{
			DrawObjectSelected(pDC,pObj,m_iOffsetX,m_iOffsetY,SRGB(0,255,0),m_bEditAllow);
			pObj = m_SelectedObjList.FetchNext(pos);
		}
		while(pObj)
		{
			DrawObjectSelected(pDC,pObj,m_iOffsetX,m_iOffsetY,SRGB(0,0,255),m_bEditAllow);
			pObj = m_SelectedObjList.FetchNext(pos);
		}
		if(m_pTempCopyObject != NULL)
		{
			DrawObject(pDC,m_pTempCopyObject,m_iOffsetX,m_iOffsetY);
		}
		if(m_bShowAlignPoint && m_bEditAllow)
		{
			DrawAlignPoint(pDC,0,0,width(),height());
		}
		if(m_bDrawSelectRange)
		{
			SPen pen(PS_DOT,1,colFrant);
			/*SPen* pOld = */pDC->SelectObject(&pen);
			pDC->MoveTo(m_ptLast.x,m_ptLast.y);
			pDC->LineTo(m_ptThis.x,m_ptLast.y);
			pDC->LineTo(m_ptThis.x,m_ptThis.y);
			pDC->LineTo(m_ptLast.x,m_ptThis.y);
			pDC->LineTo(m_ptLast.x,m_ptLast.y);
		}
	}

	return true;
}

bool SSvgWnd::event(QEvent *event)
{
	if (event->type() == QEvent::KeyPress)
	{
		QKeyEvent *ke = static_cast<QKeyEvent *>(event);
		if (ke->key() == Qt::Key_Tab)
		{
			//选择下一个图元
			if(m_SelectedObjList.count() == 1)
			{
				SSvgObject *p = m_SelectedObjList[0];
				m_SelectedObjList.clear();
				p = p->GetNext();
				if(p == NULL)
				{
					p = m_Document.GetFirstObject();
					if(p != NULL)
						p = p->GetNext();
				}
				if(p != NULL)
				{
					m_SelectedObjList.append(p);
				}
				Render();
				RedrawWindow();
				ShowAttributeWnd(p,false);
			}
			return true;
		}
	}
	return QWidget::event(event);

}

void SSvgWnd::DelSelected()
{

	unsigned long pos=0;
	SSvgObject *p = m_SelectedObjList.FetchFirst(pos);
	while(p)
	{
		DeleteTopObject(p,true);				
		p = m_SelectedObjList.FetchNext(pos);
	}
	m_SelectedObjList.clear();
	if(m_pWndAttribute)
		m_pWndAttribute->LoadObject(NULL);
	Render();
	RedrawWindow();
}

void SSvgWnd::SetSameWidth()
{
	if(m_SelectedObjList.count() <= 1)
		return;
	unsigned long pos=0;
	SSvgObject *pFirst = m_SelectedObjList.FetchFirst(pos);
	SSvgObject *pObj = m_SelectedObjList.FetchNext(pos);
	if(pFirst->GetAttribute("width").length() == 0)
		return;
	while(pObj)
	{
		if(pObj->GetAttribute("width").length() != 0)
		{
			pObj->SetAttributeValue("width",pFirst->GetAttribute("width"));
			pObj->CalcObjectRect();
		}
		pObj = m_SelectedObjList.FetchNext(pos);
	}
	RedrawWindow();
}

void SSvgWnd::SetSameHeight()
{
	if(m_SelectedObjList.count() <= 1)
		return;
	unsigned long pos=0;
	SSvgObject *pFirst = m_SelectedObjList.FetchFirst(pos);
	SSvgObject *pObj = m_SelectedObjList.FetchNext(pos);
	if(pFirst->GetAttribute("height").length() == 0)
		return;
	while(pObj)
	{
		if(pObj->GetAttribute("height").length() != 0)
		{
			pObj->SetAttributeValue("height",pFirst->GetAttribute("height"));
			pObj->CalcObjectRect();
		}
		pObj = m_SelectedObjList.FetchNext(pos);
	}
	RedrawWindow();
}
void SSvgWnd::SetSameSize()
{
	if(m_SelectedObjList.count() <= 1)
		return;
	unsigned long pos=0;
	SSvgObject *pFirst = m_SelectedObjList.FetchFirst(pos);
	SSvgObject *pObj = m_SelectedObjList.FetchNext(pos);
	if(pFirst->GetAttribute("width").length() == 0 && pFirst->GetAttribute("height").length() == 0)
		return;
	while(pObj)
	{
		if(pObj->GetAttribute("width").length() != 0 && pFirst->GetAttribute("width").length() != 0)
		{
			pObj->SetAttributeValue("width",pFirst->GetAttribute("width"));
			pObj->CalcObjectRect();
		}
		if(pFirst->GetAttribute("height").length() != 0 && pObj->GetAttribute("height").length() != 0)
		{
			pObj->SetAttributeValue("height",pFirst->GetAttribute("height"));
			pObj->CalcObjectRect();
		}
		pObj = m_SelectedObjList.FetchNext(pos);
	}
	RedrawWindow();
}
void SSvgWnd::keyPressEvent(QKeyEvent *e)//按键处理
{
	int cx=0,cy=0;
	int sep = 1;
	if(QApplication::keyboardModifiers () == Qt::AltModifier)
		sep *= 20;
	switch(e->key())
	{
	case Qt::Key_Left:
		cx = -sep;
		break;
	case Qt::Key_Right:
		cx = sep;
		break;
	case Qt::Key_Up:
		cy = -sep;
		break;
	case Qt::Key_Down:
		cy = sep;
		break;
	case Qt::Key_Delete:
		DelSelected();
		break;
	default:
		break;
	}
	if(QApplication::keyboardModifiers () == Qt::ControlModifier)
	{
		switch(e->key())
		{
		case Qt::Key_G://成组
			MakeGroup();
			break;
		case Qt::Key_D://解组
			CancelGroup();
			break;
		case Qt::Key_C://复制
			CopySelected();
			break;
		case Qt::Key_L://左对齐
			AlignLeft();
			break;
		case Qt::Key_R://右对齐
			AlignRight();
			break;
		case Qt::Key_T://上对齐
			AlignTop();
			break;
		case Qt::Key_B://下对齐
			AlignBottom();
			break;
		case Qt::Key_H://水平居中对齐
			AlignCenter();
			break;
		case Qt::Key_M://垂直居中对齐
			AlignMiddle();
			break;
		case Qt::Key_X://水平排列
			{
				OnAutoPlaceX();
			}
			break;
		case Qt::Key_Y://垂直排列
			{
				OnAutoPlaceY();
			}
			break;
		}
	}
	if(cx != 0 || cy != 0)
	{
		float sx = GetSvgCoord(cx);
		float sy = GetSvgCoord(cy);
		unsigned long pos=0;
		SSvgObject *p = m_SelectedObjList.FetchFirst(pos);
		while(p)
		{
			p->Offset(sx,sy);
			ResizeEmbeddedWnd(p,GetObjEmbeddedWnd(p));
			p = m_SelectedObjList.FetchNext(pos);
		}
		RedrawWindow();
	}
	RefreshAttributeWnd();
}

void SSvgWnd::CloneObjectImageRes(SSvgObject *pObj,bool bWithNext)
{
	while(pObj != NULL)
	{
		if(pObj->GetType() == SSvgObject::TYPE_PIC)
		{
			SSvgDocument::stuImageDefine *pImgDef = (SSvgDocument::stuImageDefine*)pObj->GetAttribute("img_def_obj").toULong();
			if(pImgDef != NULL)
			{
				pImgDef = m_Document.LoadImage(pImgDef->m_iWidth,pImgDef->m_iHeight,pImgDef->m_sImageHex);
			}
			if(pImgDef != NULL)
			{
				pObj->SetAttributeValue("img_def_obj",SString::toFormat("%ld",(unsigned long)pImgDef));
				pObj->SetAttributeValue("imgsn",pImgDef->m_sImageDefSn);
			}
		}
		else if(pObj->GetType() == SSvgObject::TYPE_GROUP && pObj->GetChild() != NULL)
		{
			CloneObjectImageRes(pObj->GetChild(),true);
		}
		if(!bWithNext)
			break;
		pObj = pObj->GetNext();
	}
}

int SSvgWnd::OnLButtonDown(int nFlag,SPoint point)
{
	S_UNUSED(nFlag);
	if(m_iCursorType == 1)
	{
		return 1;
	}
	m_iMouseMoveTimes = 0;
	m_bMouseDown = true;
	m_ptLast = point;
	setFocus();
	if(m_bMovable && m_bEditAllow && QApplication::keyboardModifiers () == Qt::AltModifier)
	{
		//可移动
		m_iMoving = 1;
		//m_bDrawFastMode = true;
		return 1;
	}
	if(m_bEditAllow)
	{
		//编辑模式
		if(m_pTempCopyObject != NULL)
		{
			//复制m_pTempCopyObject节点
			SSvgObject *pNewSvg = m_pTempCopyObject->Clone(m_Document.GetRootObject());
			//需要处理可能出现的子图元中的图片对象
			CloneObjectImageRes(pNewSvg,false);
// 			if(pNewSvg->GetType() == SSvgObject::TYPE_PIC)
// 			{
// 				SSvgDocument::stuImageDefine *pImgDef = (SSvgDocument::stuImageDefine*)m_pTempCopyObject->GetAttribute("img_def_obj").toULong();
// 				if(pImgDef != NULL)
// 				{
// 					pImgDef = m_Document.LoadImage(pImgDef->m_iWidth,pImgDef->m_iHeight,pImgDef->m_sImageHex);
// 				}
// 				if(pImgDef != NULL)
// 				{
// 					pNewSvg->SetAttributeValue("img_def_obj",SString::toFormat("%ld",(unsigned long)pImgDef));
// 					pNewSvg->SetAttributeValue("imgsn",pImgDef->m_sImageDefSn);
// 				}
// 			}
			m_SelectedObjList.clear();
			m_SelectedObjList.append(pNewSvg);
			Render();
			RedrawWindow();
			ShowAttributeWnd(pNewSvg,false);
			return 0;
		}
		//是否鼠标在移动大小上?
		if(m_iResizeMode != 0 && m_pThisObj != NULL)
		{
			m_bResizing = true;
			return 0;
		}
		//正常点击
		float x = GetSvgCoordX(point.x - m_iOffsetX);
		float y = GetSvgCoordY(point.y - m_iOffsetY);
		SSvgObject *pObj = m_Document.GetRootObject();
		if(pObj)
			pObj = pObj->GetChild();
		if(pObj)
			pObj = pObj->GetNext();
		pObj = IsObjectInPoint(pObj,x,y);
		m_iMoving = 0;
		if(pObj != NULL )
		{
			if(pObj->GetType() == SSvgObject::TYPE_POLYLINE)
			{
				//折线，判断是否点击了某个点
				SString points = pObj->GetAttribute("points");
				int i,cnt = SString::GetAttributeCount(points," ");
				for(i=1;i<=cnt;i++)
				{
					SString pt = SString::GetIdAttribute(i,points," ");
					int x = SString::GetIdAttributeI(1,pt,",");
					int y = SString::GetIdAttributeI(2,pt,",");
					if(IsPointNearPoint(point.x,point.y,GetCoordX((float)x)+m_iOffsetX,GetCoordY((float)y)+m_iOffsetY))
					{
						m_iPolyLinePoint = i;
						m_iCursorType = 2;
						m_bMouseDown = false;

						//保存上一个点的坐标，如果没有选下一个点的
						pt = SString::GetIdAttribute(i==1?(i+1):(i-1),points," ");
						m_iLineLastX = SString::GetIdAttributeI(1,pt,",");
						m_iLineLastY = SString::GetIdAttributeI(2,pt,",");
						return 1;
					}
				}
			}
			m_pThisObj = pObj;
			m_bObjMoving = true;
			Render();
			RedrawWindow();
		}
	}
	if(m_bMovable && (!m_bEditAllow || QApplication::keyboardModifiers () == Qt::AltModifier))
	{
		//可移动
		m_iMoving = 1;
		//m_bDrawFastMode = true;
		return 1;
	}

	return 1;
}

int SSvgWnd::OnRButtonDown(int nFlag,SPoint point)
{
	SWnd::OnRButtonDown(nFlag,point);
	if(m_iCursorType == 1)
	{
		//画折线段
// 		float x = GetSvgCoordX(point.x-m_iOffsetX);
// 		float y = GetSvgCoordY(point.y-m_iOffsetY);
		SSvgObject *pObj = m_SelectedObjList[0];
		if(pObj != NULL && pObj->GetType() == SSvgObject::TYPE_POLYLINE)
		{
			SString points = pObj->GetAttribute("points");
			//1,2 3,4
			int pos = points.findRev(" ");
			points = points.left(pos);
			pObj->SetAttributeValue("points",points);
			pObj->CalcObjectRect();
			Render();
			RedrawWindow();
		}
		m_iCursorType = 0;
		m_pThisObj = pObj;
		if(m_SelectedObjList.count() == 0)
			HideAttributeWnd();
		else
			RefreshAttributeWnd();
		return 1;
	}

	if(m_pTempCopyObject != NULL)
	{
		SetTempCopyObject(NULL);
		RedrawWindow();
	}
	else if(m_bEditAllow)// if(m_SelectedObjList.count() == 0)
	{

#ifdef QT_GUI_LIB
		//显示弹出的创建对象菜单
		m_ptLast = point;
		QMenu menu;
		if(m_SelectedObjList.count() == 0)
		{
			menu.addAction(tr("添加文字图元"),this,SLOT(OnNewText()));
			menu.addAction(tr("添加区域文字图元"),this,SLOT(OnNewRectText()));
			menu.addAction(tr("添加矩形图元"),this,SLOT(OnNewRect()));
			menu.addAction(tr("添加圆形图元"),this,SLOT(OnNewEllipse()));
			menu.addAction(tr("添加直线图元"),this,SLOT(OnNewLine()));
			menu.addAction(tr("添加折线图元"),this,SLOT(OnNewPolyline()));
			menu.addAction(tr("添加内嵌图片"),this,SLOT(OnNewInnerImage()));
			menu.addAction(tr("添加链接图片"),this,SLOT(OnNewHrefImage()));
		}
		else if(m_SelectedObjList.count() == 1)
		{
			menu.addAction(tr("复制选中图元(Ctrl+C)"),this,SLOT(CopySelected()));
			menu.addAction(tr("删除选中图元(Delete)"),this,SLOT(DelSelected()));
			if(m_SelectedObjList[0]->GetType() == SSvgObject::TYPE_GROUP)
				menu.addAction(tr("解除组合(Ctrl+D)"),this,SLOT(CancelGroup()));
		}
		else if(m_SelectedObjList.count() > 1)
		{
			menu.addAction(tr("复制选中图元(Ctrl+C)"),this,SLOT(CopySelected()));
			menu.addAction(tr("删除选中图元(Delete)"),this,SLOT(DelSelected()));
			menu.addAction(tr("组合图元(Ctrl+G)"),this,SLOT(MakeGroup()));
			menu.addSeparator();
			menu.addAction(tr("左对齐(Ctrl+L)"),this,SLOT(AlignLeft()));
			menu.addAction(tr("右对齐(Ctrl+R)"),this,SLOT(AlignRight()));
			menu.addAction(tr("上对齐(Ctrl+T)"),this,SLOT(AlignTop()));
			menu.addAction(tr("下对齐(Ctrl+B)"),this,SLOT(AlignBottom()));
			menu.addAction(tr("水平居中对齐(Ctrl+H)"),this,SLOT(AlignCenter()));
			menu.addAction(tr("垂直居中对齐(Ctrl+M)"),this,SLOT(AlignMiddle()));
			menu.addSeparator();
			menu.addAction(tr("设置选中对象等宽"),this,SLOT(SetSameWidth()));
			menu.addAction(tr("设置选中对象等高"),this,SLOT(SetSameHeight()));
			menu.addAction(tr("设置选中对象等大小"),this,SLOT(SetSameSize()));
			menu.addAction(tr("水平自动排列图元(Ctrl+X)..."),this,SLOT(OnAutoPlaceX()));
			menu.addAction(tr("垂直自动排列图元(Ctrl+Y)..."),this,SLOT(OnAutoPlaceY()));
		}
		menu.addSeparator();
		menu.addAction(tr("显示全图"),this,SLOT(ToFull()));
		menu.addAction(tr("居中显示"),this,SLOT(ToCenter()));
		menu.addAction(tr("适应宽度显示"),this,SLOT(ToWidthFull()));
		menu.addAction(tr("适应高度显示"),this,SLOT(ToHeightFull()));
		menu.addAction(tr("原始大小显示"),this,SLOT(ToRaw()));
		menu.addSeparator();
		if(!IsShowAlignPoint())
			menu.addAction(tr("显示对齐点"),this,SLOT(ShowAlignPoint()));
		else
			menu.addAction(tr("隐藏对齐点"),this,SLOT(HideAlignPoint()));
		m_sRClickPos.sprintf("%f,%f",GetSvgCoordX(point.x-m_iOffsetX), GetSvgCoordY(point.y-m_iOffsetY));
		menu.addAction(tr("复制坐标位置"),this,SLOT(CopyCoordPos()));

// 		menu.addSeparator();
// 		menu.addAction(tr("增加时间标注"),this,SLOT(OnAddLabelTimeLine()));
// 		menu.addAction(tr("增加文本标注"),this,SLOT(OnAddLabelText()));

		menu.addSeparator();
		menu.addAction(tr("导出为SVG文件..."),this,SLOT(ToExport()));

		menu.exec(QCursor::pos()/*, at*/);
#endif
	}
	else
	{
#ifdef QT_GUI_LIB
		//显示弹出的创建对象菜单
		m_ptLast = point;
		QMenu menu;
		menu.addAction(tr("显示全图"),this,SLOT(ToFull()));
		menu.addAction(tr("居中显示"),this,SLOT(ToCenter()));
		menu.addAction(tr("适应宽度显示"),this,SLOT(ToWidthFull()));
		menu.addAction(tr("适应高度显示"),this,SLOT(ToHeightFull()));
		menu.addAction(tr("原始大小显示"),this,SLOT(ToRaw()));

		menu.addSeparator();
		menu.addAction(tr("导出为SVG文件..."),this,SLOT(ToExport()));

		menu.exec(QCursor::pos()/*, at*/);
#endif
	}
	return 1;
}

void SSvgWnd::CopyCoordPos()
{
	QClipboard *board = QApplication::clipboard();
	board->setText(m_sRClickPos.data());
}

void SSvgWnd::OnNewText()
{
	int x = GetSvgCoordX(m_ptLast.x-m_iOffsetX);
	int y = GetSvgCoordY(m_ptLast.y-m_iOffsetY);
	SString sAttr;
	SString sText = SQt::GetInput("新文本图元","请输入文本内容","新文本",this);
	sAttr.sprintf("x=%d;y=%d;z=100;font-family=宋体;font-size=14;font-weight=normal;fill=#0000FF;" ,
		x,y);
	SSvgObject *pNewObj = this->m_Document.AddObject("text",sAttr);
	pNewObj->SetAttributeValue("text",sText);
	m_SelectedObjList.clear();
	m_SelectedObjList.append(pNewObj);
	Render();
	RedrawWindow();
	ShowAttributeWnd(pNewObj);
}

void SSvgWnd::OnNewRectText()
{
	int x = GetSvgCoordX(m_ptLast.x-m_iOffsetX);
	int y = GetSvgCoordY(m_ptLast.y-m_iOffsetY);
	SString sAttr;
	SString sText = SQt::GetInput("新文本图元","请输入文本内容","新文本",this);
	sAttr.sprintf("x=%d;y=%d;z=100;width=200;height=50;alignh=center;alignv=center;font-family=宋体;font-size=14;font-weight=normal;fill=#0000FF;" ,
		x,y);
	SSvgObject *pNewObj = this->m_Document.AddObject("recttext",sAttr);
	pNewObj->SetAttributeValue("text",sText);
	m_SelectedObjList.clear();
	m_SelectedObjList.append(pNewObj);
	Render();
	RedrawWindow();
	ShowAttributeWnd(pNewObj);
}
void SSvgWnd::OnNewRect()
{
	int x = GetSvgCoordX(m_ptLast.x-m_iOffsetX);
	int y = GetSvgCoordY(m_ptLast.y-m_iOffsetY);
	SString sAttr;
	sAttr.sprintf("x=%d;y=%d;z=100;width=50;height=50;stroke=#ff0000;stroke-width=1;stroke-dasharray=0;fill=#cccccc;" ,
		x,y);
	SSvgObject *pNewObj = this->m_Document.AddObject("rect",sAttr);
	m_SelectedObjList.clear();
	m_SelectedObjList.append(pNewObj);
	Render();
	RedrawWindow();
	ShowAttributeWnd(pNewObj);
}
void SSvgWnd::OnNewEllipse()
{
	int x = GetSvgCoordX(m_ptLast.x-m_iOffsetX);
	int y = GetSvgCoordY(m_ptLast.y-m_iOffsetY);
	SString sAttr;
	sAttr.sprintf("cx=%d;cy=%d;z=100;rx=50;ry=50;stroke=#ff0000;stroke-width=1;stroke-dasharray=0;fill=#cccccc;" ,
		x,y);
	SSvgObject *pNewObj = this->m_Document.AddObject("ellipse",sAttr);
	m_SelectedObjList.clear();
	m_SelectedObjList.append(pNewObj);
	Render();
	RedrawWindow();
	ShowAttributeWnd(pNewObj);
}
void SSvgWnd::OnNewLine()
{
	int x = GetSvgCoordX(m_ptLast.x-m_iOffsetX);
	int y = GetSvgCoordY(m_ptLast.y-m_iOffsetY);
	SString sAttr;
	sAttr.sprintf("x1=%d;y1=%d;z=100;x2=%d;y2=%d;stroke=#FF0000;stroke-width=1;stroke-dasharray=0;" ,
		x,y,x+50,y);
	SSvgObject *pNewObj = this->m_Document.AddObject("line",sAttr);
	m_SelectedObjList.clear();
	m_SelectedObjList.append(pNewObj);
	Render();
	RedrawWindow();
	ShowAttributeWnd(pNewObj);
}
void SSvgWnd::OnNewPolyline()
{
	//<polyline uri="" caption="" z="100" points="289.492798,176.87551 289.492798,229.617529 260.633484,229.61756 260.633484,203.744055" stroke="#FF0000" stroke-width="1" stroke-dasharray="0" fill="" />
	int x = GetSvgCoordX(m_ptLast.x-m_iOffsetX);
	int y = GetSvgCoordY(m_ptLast.y-m_iOffsetY);
	m_iLineLastX = x;
	m_iLineLastY = y;
	m_iCursorType = 1;
	SString sAttr;
	sAttr.sprintf("z=100;stroke=#FF0000;stroke-width=1;stroke-dasharray=0;points=%d,%d %d,%d" ,
		x,y,x+5,y+5);
	SSvgObject *pNewObj = this->m_Document.AddObject("polyline",sAttr);
	m_SelectedObjList.clear();
	m_SelectedObjList.append(pNewObj);
	Render();
	RedrawWindow();
	ShowAttributeWnd(pNewObj);
}

void SSvgWnd::OnNewInnerImage()
{
#ifdef QT_GUI_LIB
	int x = GetSvgCoordX(m_ptLast.x-m_iOffsetX);
	int y = GetSvgCoordY(m_ptLast.y-m_iOffsetY);
	QString qstr = QFileDialog::getOpenFileName(this,tr("选择图片"),QDir::currentPath(),tr("Images (*.jpg *.png *.bmp *.gif)"));
	SSvgDocument::stuImageDefine *pImageDef = m_Document.LoadImage(qstr.toStdString().c_str(),true);
	if(pImageDef == NULL)
		return;	
	SString sAttr;
	sAttr.sprintf("x=%d;y=%d;z=100;width=%d;height=%d;imgsn=%s;img_def_obj=%ld;" ,
		x,y,pImageDef->m_iWidth,pImageDef->m_iHeight,pImageDef->m_sImageDefSn.data(),(long)pImageDef);
	SSvgObject *pNewObj = this->m_Document.AddObject("image",sAttr);
	m_SelectedObjList.clear();
	m_SelectedObjList.append(pNewObj);
	Render();
	RedrawWindow();
	ShowAttributeWnd(pNewObj);
#endif
}

void SSvgWnd::OnNewHrefImage()
{
#ifdef QT_GUI_LIB
	int x = GetSvgCoordX(m_ptLast.x-m_iOffsetX);
	int y = GetSvgCoordY(m_ptLast.y-m_iOffsetY);
	QString qstr = QFileDialog::getOpenFileName(this,tr("选择图片"),QDir::currentPath(),tr("Images (*.jpg *.png *.bmp *.gif)"));
	SSvgDocument::stuImageDefine *pImageDef = m_Document.LoadImage(qstr.toStdString().c_str(),false);
	if(pImageDef == NULL)
		return;

	SString sAttr;
	sAttr.sprintf("x=%d;y=%d;z=100;width=%d;height=%d;imgsn=%s;img_def_obj=%ld;xlink:href=%s;" ,
		x,y,pImageDef->m_iWidth,pImageDef->m_iHeight,pImageDef->m_sImageDefSn.data(),(long)pImageDef,qstr.toStdString().c_str());
	SSvgObject *pNewObj = this->m_Document.AddObject("image",sAttr);
	m_SelectedObjList.clear();
	m_SelectedObjList.append(pNewObj);
	Render();
	RedrawWindow();
	ShowAttributeWnd(pNewObj);
#endif

}

int SSvgWnd::OnSize(int type,int cx,int cy)
{
	S_UNUSED(type);
	S_UNUSED(cx);
	S_UNUSED(cy);
	Render();
	//ToCenter();
	return 0;
}

int SSvgWnd::OnLButtonUp(int nFlag,SPoint point)
{
	if(m_iCursorType == 1)
	{
		//画折线段
		float x = GetSvgCoordX(point.x-m_iOffsetX);
		float y = GetSvgCoordY(point.y-m_iOffsetY);
		SSvgObject *pObj = m_SelectedObjList[0];
		if(pObj != NULL && pObj->GetType() == SSvgObject::TYPE_POLYLINE)
		{
			SString points = pObj->GetAttribute("points");
			//1,2 3,4
			int pos = points.findRev(" ")+1;
			if(QApplication::keyboardModifiers () == Qt::ShiftModifier)
			{
				//水平 or 垂直				
				if(fabs(x-m_iLineLastX) > fabs(y-m_iLineLastY))
				{
					//水平
					y = m_iLineLastY;
				}
				else
				{
					x = m_iLineLastX;
				}
			}
			m_iLineLastX = x;
			m_iLineLastY = y;
			points = points.left(pos)+SString::toFormat("%f,%f %f,%f",x,y,x,y);
			pObj->SetAttributeValue("points",points);
			RedrawWindow();
		}
		m_bDrawSelectRange = false;
		m_pThisObj = pObj;
		if(m_SelectedObjList.count() == 0)
			HideAttributeWnd();
		else
			RefreshAttributeWnd();

		return 1;
	}
	else if(m_iCursorType == 2)
	{
		m_iCursorType = 0;
		m_bDrawSelectRange = false;
		m_pThisObj = m_SelectedObjList[0];
		if(m_SelectedObjList.count() == 0)
			HideAttributeWnd();
		else
			RefreshAttributeWnd();

		return 1;
	}

	m_bObjMoving = false;
	m_bResizing = false;
	m_bMouseDown = false;
	if(!m_bEditAllow && m_iMoving == 1 )
		m_iMoving = 0;
	if(m_pTempCopyObject != NULL)
	{
		SetTempCopyObject(NULL);
		return 0;
	}
	if(m_iMoving >= 2)
	{
		OnMouseMove(nFlag,point);
		m_iMoving = 0;
		//m_bDrawFastMode = false;
	}
	else if(m_bDrawSelectRange)
	{
		SSvgObject::SVG_RECT r;
		if(m_ptLast.x < m_ptThis.x)
		{
			r.left = GetSvgCoordX(m_ptLast.x-m_iOffsetX);
			r.right = GetSvgCoordX(m_ptThis.x-m_iOffsetX);
		}
		else
		{
			r.left = GetSvgCoordX(m_ptThis.x-m_iOffsetX);
			r.right = GetSvgCoordX(m_ptLast.x-m_iOffsetX);
		}
		if(m_ptLast.y < m_ptThis.y)
		{
			r.top = GetSvgCoordY(m_ptLast.y-m_iOffsetY);
			r.bottom = GetSvgCoordY(m_ptThis.y-m_iOffsetY);
		}
		else
		{
			r.top = GetSvgCoordY(m_ptThis.y-m_iOffsetY);
			r.bottom = GetSvgCoordY(m_ptLast.y-m_iOffsetY);
		}
		SSvgObject *pObj = m_Document.GetRootObject();
		if(pObj)
			pObj = pObj->GetChild();
		if(pObj)
			pObj = pObj->GetNext();
		if(QApplication::keyboardModifiers () != Qt::ControlModifier)
			m_SelectedObjList.clear();
		while(pObj)
		{
				
			if(pObj->IsInRect(&r) && !m_SelectedObjList.exist(pObj))
				m_SelectedObjList.append(pObj);
			pObj = pObj->GetNext();
		}
		Render();
		RedrawWindow();
	}
	// 未移动点击响应
	else if (m_iMoving == 0 && !m_bEditAllow)
	{
		float x = GetSvgCoordX(point.x - m_iOffsetX);
		float y = GetSvgCoordY(point.y - m_iOffsetY);
		SSvgObject *pObj = m_Document.GetRootObject();
		if(pObj)
			pObj = pObj->GetChild();
		if(pObj)
			pObj = pObj->GetNext();
		pObj = IsObjectInPoint(pObj,x,y);
		m_iMoving = 0;
		if(pObj != NULL)
		{
			OnClickObject(pObj);
		}
	}
	else if(m_bEditAllow)
	{
		//正常点击
		float x = GetSvgCoordX(point.x - m_iOffsetX);
		float y = GetSvgCoordY(point.y - m_iOffsetY);
		SSvgObject *pObj = m_Document.GetRootObject();
		if(pObj)
			pObj = pObj->GetChild();
		if(pObj)
			pObj = pObj->GetNext();
		pObj = IsObjectInPoint(pObj,x,y);
		m_iMoving = 0;
		if(m_iMouseMoveTimes == 0)
		{
			if(pObj != NULL)
			{
				//选中图元
				if(QApplication::keyboardModifiers () == Qt::ControlModifier)//Qt::AltModifier
				{
					//按下了Ctrl，添加到选中列表
					if(m_SelectedObjList.exist(pObj))
						m_SelectedObjList.remove(pObj);
					else
						m_SelectedObjList.append(pObj);
				}
				else
				{
					//替换当前选中
					m_SelectedObjList.clear();
					m_SelectedObjList.append(pObj);
				}
				setCursor(Qt::SizeAllCursor);
				Render();
				RedrawWindow();
				if(m_bEditAllow)
					ShowAttributeWnd(pObj);
			}
			else
			{
				if(m_SelectedObjList.count() > 0)
				{
					m_SelectedObjList.clear();
					Render();
					RedrawWindow();
				}
			}
		}

	}
	m_bDrawSelectRange = false;
	if(m_SelectedObjList.count() == 0)
		HideAttributeWnd();
	else
		RefreshAttributeWnd();
	return 1;
}

void SSvgWnd::ToTopCenter()
{
	SRECT rWnd = GetClientRect();
	SSvgObject *pFirstObj = m_Document.GetFirstObject();
	if(pFirstObj == NULL)
		return ;
	int svgw = GetCoordX(pFirstObj->GetRect()->right);
	//int svgh = GetCoord(pFirstObj->GetRect()->bottom);

	int x,y;
	x = (rWnd.right - svgw)/2;
	y = 0;
	SetOffset(x,y);

	Render();
	RedrawWindow();
}

void SSvgWnd::ToCenter()
{
	SRECT rWnd = GetClientRect();
	SSvgObject *pFirstObj = m_Document.GetFirstObject();
	if(pFirstObj == NULL)
		return ;
	int svgw = GetCoordX(pFirstObj->GetRect()->right);
	int svgh = GetCoordY(pFirstObj->GetRect()->bottom);

	int x,y;
	x = (rWnd.right - svgw)/2;
	y = (rWnd.bottom - svgh)/2;
	SetOffset(x,y);
	/*
	int wndw = rWnd.right - rWnd.left;
	int wndh = rWnd.bottom - rWnd.top;
	svgw = GetCoordX(pFirstObj->GetRect()->right);
	int svgh = GetCoordY(pFirstObj->GetRect()->bottom);
	if(wndw < svgw)
	{
		//窗口宽度小于SVG宽度时
		if(m_iOffsetX < wndw-svgw)
			m_iOffsetX = wndw-svgw;
		if(m_iOffsetX > 0)
			m_iOffsetX = 0;
	}
	else
	{
		//窗口宽度大于SVG宽度时
		if(m_iOffsetX < 0)
			m_iOffsetX = 0;
		if(m_iOffsetX > wndw - svgw)
			m_iOffsetX = wndw - svgw;
	}
	if(wndh < svgh)
	{
		//窗口高度小于SVG宽度时
		if(m_iOffsetY < wndh-svgh)
			m_iOffsetY = wndh-svgh;
		if(m_iOffsetY > 0)
			m_iOffsetY = 0;
	}
	else
	{
		//窗口高度大于SVG宽度时
		if(m_iOffsetY < 0)
			m_iOffsetY = 0;
		if(m_iOffsetY > wndh - svgh)
			m_iOffsetY = wndh - svgh;
	}
	*/
	Render();
	RedrawWindow();
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  显示全图
// 作    者:  邵凯田
// 创建时间:  2015-8-20 11:01
// 参数说明:  
// 返 回 值:  
//////////////////////////////////////////////////////////////////////////
void SSvgWnd::ToFull()
{
	SRECT rWnd = GetClientRect();
	SSvgObject *pFirstObj = m_Document.GetFirstObject();
	if(pFirstObj == NULL)
		return;
	SSvgObject::SVG_RECT* pSvgRect = pFirstObj->GetRect();
	float scale1,scale2;
	if(pSvgRect->right-pSvgRect->left < 0.001)
		scale1 = 1;
	else
		scale1 = ((float)rWnd.right-rWnd.left-10)/(pSvgRect->right-pSvgRect->left);
	if(pSvgRect->bottom-pSvgRect->top < 0.001)
		scale2 = 1;
	else
		scale2 = ((float)rWnd.bottom-rWnd.top-10)/(pSvgRect->bottom-pSvgRect->top);
	float scale = scale1<scale2?scale1:scale2;
	//scale = 1/scale;
	SetScale(scale);

	int svgw = GetCoord(pFirstObj->GetRect()->right-pFirstObj->GetRect()->left);
	int svgh = GetCoord(pFirstObj->GetRect()->bottom-pFirstObj->GetRect()->top);
	int x,y;
	x = (rWnd.right - svgw)/2;
	y = (rWnd.bottom - svgh)/2;
	SetOffset(x,y);
	Render();
	RedrawWindow();
}
void SSvgWnd::ToRaw()
{
	SetScale(1);
	SSvgObject *pFirst = m_Document.GetFirstObject();
	if(pFirst != NULL && pFirst->GetType() == SSvgObject::TYPE_RECT)
	{
		float w = pFirst->GetRect()->width();
		float wnd_w = (float)rect().width();
		SetOffset(-(w-wnd_w)/2,0);
	}
	Render();
	RedrawWindow();
}
void SSvgWnd::ToWidthFull()
{
	SSvgObject *pFirst = m_Document.GetFirstObject();
	if(pFirst != NULL && pFirst->GetType() == SSvgObject::TYPE_RECT)
	{
		float w = pFirst->GetRect()->width();
		float wnd_w = (float)rect().width();
		SetScale(wnd_w/w);
		SetOffset(0,0);
		Render();
		RedrawWindow();
	}
}

void SSvgWnd::ToHeightFull()
{
	SSvgObject *pFirst = m_Document.GetFirstObject();
	if(pFirst != NULL && pFirst->GetType() == SSvgObject::TYPE_RECT)
	{
		float w = pFirst->GetRect()->height();
		float wnd_w = (float)rect().height();
		SetScale(wnd_w/w);
		SetOffset(0,0);
		Render();
		RedrawWindow();
	}
}

void SSvgWnd::ToExport()
{
	QString fileName = QFileDialog::getSaveFileName(NULL,"export.svg","", ("SVG files (*.svg)"));
	if(fileName.length() == 0)
		return ;
	SString sFile = fileName.toStdString().data();
	if(sFile.right(4) != ".svg")
		sFile += ".svg";
	if(m_Document.SaveSvgFile(sFile))
	{
		SQt::ShowInfoBox(tr("提示"),tr("SVG文件导出成功!"));
	}
	else
	{
		SQt::ShowErrorBox(tr("提示"),tr("SVG文件导出失败!"));
	}
}

int SSvgWnd::OnMouseMove(int nFlag,SPoint point)
{
	S_UNUSED(nFlag);
	m_iMouseMoveTimes++;
	bool bTip=false;
	emit sigCursorPos(GetSvgCoordX(point.x-m_iOffsetX), GetSvgCoordY(point.y-m_iOffsetY));
	if(m_iCursorType == 1)
	{
		//画折线段
		float x = GetSvgCoordX(point.x-m_iOffsetX);
		float y = GetSvgCoordY(point.y-m_iOffsetY);
		SSvgObject *pObj = m_SelectedObjList[0];
		if(pObj != NULL && pObj->GetType() == SSvgObject::TYPE_POLYLINE)
		{
			SString points = pObj->GetAttribute("points");
			//1,2 3,4
			int pos = points.findRev(" ")+1;
			if(QApplication::keyboardModifiers () == Qt::ShiftModifier)
			{
				//水平 or 垂直
				if(fabs(x-m_iLineLastX) > fabs(y-m_iLineLastY))
				{
					//水平
					y = m_iLineLastY;
				}
				else
				{
					x = m_iLineLastX;
				}
			}
			points = points.left(pos)+SString::toFormat("%f,%f",x,y);
			pObj->SetAttributeValue("points",points);
			pObj->CalcObjectRect();
			RedrawWindow();
		}
		return 1;
	}
	else if(m_iCursorType == 2)
	{
		//移动折线点
		float x = GetSvgCoordX(point.x-m_iOffsetX);
		float y = GetSvgCoordY(point.y-m_iOffsetY);
		SSvgObject *pObj = m_SelectedObjList[0];
		if(pObj != NULL && pObj->GetType() == SSvgObject::TYPE_POLYLINE)
		{
			SString points = pObj->GetAttribute("points");
			SString newpoints="";
			int i,cnt = SString::GetAttributeCount(points," ");
			for(i=1;i<=cnt;i++)
			{
				SString pt = SString::GetIdAttribute(i,points," ");
				if(i == m_iPolyLinePoint)
				{
					if(QApplication::keyboardModifiers () == Qt::ShiftModifier)
					{
						//水平 or 垂直				
						if(fabs(x-m_iLineLastX) > fabs(y-m_iLineLastY))
						{
							//水平
							y = m_iLineLastY;
						}
						else
						{
							x = m_iLineLastX;
						}
					}
					newpoints += SString::toFormat("%f,%f",x,y);
				}
				else
				{
					newpoints += pt;
				}
				if(i != cnt)
					newpoints += " ";
			}
			pObj->SetAttributeValue("points",newpoints);
			pObj->CalcObjectRect();
			RedrawWindow();
		}
		return 1;
	}

	if(m_pTempCopyObject != NULL)
	{
		//移动临时图元库
		float x = GetSvgCoordX(point.x-m_iOffsetX);
		float y = GetSvgCoordY(point.y-m_iOffsetY);
		x += m_pTempCopyObject->GetRect()->width()/2;
		y += m_pTempCopyObject->GetRect()->height()/2;
		int dx = x-m_pTempCopyObject->GetRect()->centerx();
		int dy = y-m_pTempCopyObject->GetRect()->centery();
		dx -= m_pTempCopyObject->GetRect()->width()/2;
		dy -= m_pTempCopyObject->GetRect()->height()/2;
		m_pTempCopyObject->Offset(dx,dy);
		m_pTempCopyObject->CalcObjectRect();
		RedrawWindow();
	}
	else if(m_bResizing && m_pThisObj != NULL && m_SelectedObjList.count() == 1)//改变图元大小
	{
		int cx= m_ptLast.x-point.x;
		int cy= m_ptLast.y-point.y;
		float sx = GetSvgCoord(cx);
		float sy = GetSvgCoord(cy);
		SSvgObject *p = m_pThisObj;
		switch(p->GetType())
		{
		case SSvgObject::TYPE_RECT:
		case SSvgObject::TYPE_RECTTEXT:
		case SSvgObject::TYPE_PIC:
			if(m_iResizeMode == 1)
			{
				p->SetAttributeValue("x",SString::toString(p->GetAttributeF("x")-sx));
				p->SetAttributeValue("width",SString::toString(p->GetAttributeF("width")+sx));
				p->SetAttributeValue("y",SString::toString(p->GetAttributeF("y")-sy));
				p->SetAttributeValue("height",SString::toString(p->GetAttributeF("height")+sy));
			}
			else if(m_iResizeMode == 2)
			{
				p->SetAttributeValue("y",SString::toString(p->GetAttributeF("y")-sy));
				p->SetAttributeValue("height",SString::toString(p->GetAttributeF("height")+sy));
			}
			else if(m_iResizeMode == 3)
			{
				p->SetAttributeValue("width",SString::toString(p->GetAttributeF("width")-sx));
				p->SetAttributeValue("y",SString::toString(p->GetAttributeF("y")-sy));
				p->SetAttributeValue("height",SString::toString(p->GetAttributeF("height")+sy));
			}
			else if(m_iResizeMode == 4)
			{
				p->SetAttributeValue("width",SString::toString(p->GetAttributeF("width")-sx));
			}
			else if(m_iResizeMode == 5)
			{
				p->SetAttributeValue("width",SString::toString(p->GetAttributeF("width")-sx));
				p->SetAttributeValue("height",SString::toString(p->GetAttributeF("height")-sy));
			}
			else if(m_iResizeMode == 6)
			{
				p->SetAttributeValue("height",SString::toString(p->GetAttributeF("height")-sy));
			}
			else if(m_iResizeMode == 7)
			{
				p->SetAttributeValue("x",SString::toString(p->GetAttributeF("x")-sx));
				p->SetAttributeValue("width",SString::toString(p->GetAttributeF("width")+sx));
				p->SetAttributeValue("height",SString::toString(p->GetAttributeF("height")-sy));
			}
			else if(m_iResizeMode == 8)
			{
				p->SetAttributeValue("x",SString::toString(p->GetAttributeF("x")-sx));
				p->SetAttributeValue("width",SString::toString(p->GetAttributeF("width")+sx));
			}
			if(QApplication::keyboardModifiers () == Qt::ShiftModifier)
			{
				if(p->GetAttributeI("rawh")==0 && p->GetAttributeI("raww")==0)
				{
					//正方形
					float len = p->GetAttributeF("width");
					if(len < p->GetAttributeF("height"))
						len = p->GetAttributeF("height");
					p->SetAttributeValue("width",SString::toString(len));
					p->SetAttributeValue("height",SString::toString(len));
				}
				else
				{
					//保持原始长宽比
					float w = p->GetAttributeF("width");
					float h = p->GetAttributeF("height");
					float f = p->GetAttributeF("raww")/p->GetAttributeF("rawh");
					if(w/h > f)
					{
						w = h * f;
					}
					else
					{
						h = w / f;
					}
					p->SetAttributeValue("width",SString::toString(w));
					p->SetAttributeValue("height",SString::toString(h));
				}
			}
			break;
		case SSvgObject::TYPE_TEXT:

			break;
		case SSvgObject::TYPE_ELLIPSE:
			if(m_iResizeMode == 1)
			{
				p->SetAttributeValue("cx",SString::toString(p->GetAttributeF("cx")-sx/2));
				p->SetAttributeValue("rx",SString::toString(p->GetAttributeF("rx")+sx/2));
				p->SetAttributeValue("cy",SString::toString(p->GetAttributeF("cy")-sy/2));
				p->SetAttributeValue("ry",SString::toString(p->GetAttributeF("ry")+sy/2));
			}
			else if(m_iResizeMode == 2)
			{
				p->SetAttributeValue("cy",SString::toString(p->GetAttributeF("cy")-sy/2));
				p->SetAttributeValue("ry",SString::toString(p->GetAttributeF("ry")+sy/2));
			}
			else if(m_iResizeMode == 3)
			{
				p->SetAttributeValue("cx",SString::toString(p->GetAttributeF("cx")-sx/2));
				p->SetAttributeValue("rx",SString::toString(p->GetAttributeF("rx")-sx/2));
				p->SetAttributeValue("cy",SString::toString(p->GetAttributeF("cy")-sy/2));
				p->SetAttributeValue("ry",SString::toString(p->GetAttributeF("ry")+sy/2));
			}
			else if(m_iResizeMode == 4)
			{
				p->SetAttributeValue("cx",SString::toString(p->GetAttributeF("cx")-sx/2));
				p->SetAttributeValue("rx",SString::toString(p->GetAttributeF("rx")-sx/2));
			}
			else if(m_iResizeMode == 5)
			{
				p->SetAttributeValue("cx",SString::toString(p->GetAttributeF("cx")-sx/2));
				p->SetAttributeValue("rx",SString::toString(p->GetAttributeF("rx")-sx/2));
				p->SetAttributeValue("cy",SString::toString(p->GetAttributeF("cy")-sy/2));
				p->SetAttributeValue("ry",SString::toString(p->GetAttributeF("ry")-sy/2));
			}
			else if(m_iResizeMode == 6)
			{
				p->SetAttributeValue("cy",SString::toString(p->GetAttributeF("cy")-sy/2));
				p->SetAttributeValue("ry",SString::toString(p->GetAttributeF("ry")-sy/2));
			}
			else if(m_iResizeMode == 7)
			{
				p->SetAttributeValue("cx",SString::toString(p->GetAttributeF("cx")-sx/2));
				p->SetAttributeValue("rx",SString::toString(p->GetAttributeF("rx")+sx/2));
				p->SetAttributeValue("cy",SString::toString(p->GetAttributeF("cy")-sy/2));
				p->SetAttributeValue("ry",SString::toString(p->GetAttributeF("ry")-sy/2));
			}
			else if(m_iResizeMode == 8)
			{
				p->SetAttributeValue("cx",SString::toString(p->GetAttributeF("cx")-sx/2));
				p->SetAttributeValue("rx",SString::toString(p->GetAttributeF("rx")+sx/2));
			}
			if(QApplication::keyboardModifiers () == Qt::ShiftModifier)
			{
				//正方形
				float len = p->GetAttributeF("rx");
				if(len < p->GetAttributeF("ry"))
					len = p->GetAttributeF("ry");
				float x0 = p->GetAttributeF("cx")-p->GetAttributeF("rx");
				float y0 = p->GetAttributeF("cy")-p->GetAttributeF("ry");

				p->SetAttributeValue("cx",SString::toString(x0+len));
				p->SetAttributeValue("cy",SString::toString(y0+len));
				p->SetAttributeValue("rx",SString::toString(len));
				p->SetAttributeValue("ry",SString::toString(len));
			}
			break;
		case SSvgObject::TYPE_LINE:
			if(IsPointNearPoint(m_ptLast.x,m_ptLast.y,GetCoordX(p->GetAttributeF("x1"))+m_iOffsetX,GetCoordY(p->GetAttributeF("y1"))+m_iOffsetY))
			{
				float x = p->GetAttributeF("x1")-sx;
				float y = p->GetAttributeF("y1")-sy;
				m_iLineLastX = p->GetAttributeI("x2");
				m_iLineLastY = p->GetAttributeI("y2");
				if(QApplication::keyboardModifiers () == Qt::ShiftModifier)
				{
					//水平 or 垂直				
					if(fabs(x-m_iLineLastX) > fabs(y-m_iLineLastY))
					{
						//水平
						y = m_iLineLastY;
					}
					else
					{
						x = m_iLineLastX;
					}
				}
				p->SetAttributeValue("x1",SString::toString(x));
				p->SetAttributeValue("y1",SString::toString(y));
			}
			else
			{
				float x = p->GetAttributeF("x2")-sx;
				float y = p->GetAttributeF("y2")-sy;
				m_iLineLastX = p->GetAttributeI("x1");
				m_iLineLastY = p->GetAttributeI("y1");
				if(QApplication::keyboardModifiers () == Qt::ShiftModifier)
				{
					//水平 or 垂直				
					if(fabs(x-m_iLineLastX) > fabs(y-m_iLineLastY))
					{
						//水平
						y = m_iLineLastY;
					}
					else
					{
						x = m_iLineLastX;
					}
				}
				p->SetAttributeValue("x2",SString::toString(x));
				p->SetAttributeValue("y2",SString::toString(y));
			}

			break;
		case SSvgObject::TYPE_POLYLINE:

			break;
		case SSvgObject::TYPE_PATH:

			break;
		default:
			break;
		}
		p->CalcObjectRect();
		RedrawWindow();
		ResizeEmbeddedWnd(p,GetObjEmbeddedWnd(p));

		//RefreshAttributeWnd();
		m_ptLast = point;
	}
	else if(m_bObjMoving && m_pThisObj != NULL)
	{
		//移动图元
		int cx=point.x - m_ptLast.x;
		int cy=point.y - m_ptLast.y;
		float sx = GetSvgCoord(cx);
		float sy = GetSvgCoord(cy);
		m_ptLast = point;
		if(m_SelectedObjList.exist(m_pThisObj) == false)
		{
			if(QApplication::keyboardModifiers () != Qt::ControlModifier)
				m_SelectedObjList.clear();
			m_SelectedObjList.append(m_pThisObj);
			Render();
		}
		unsigned long pos=0;
		SSvgObject *p = m_SelectedObjList.FetchFirst(pos);
		while(p)
		{
			p->Offset(sx,sy);
			ResizeEmbeddedWnd(p,GetObjEmbeddedWnd(p));
			p = m_SelectedObjList.FetchNext(pos);
		}
		setCursor(Qt::SizeAllCursor);
		RedrawWindow();
		//RefreshAttributeWnd();
	}
	else if(m_iMoving > 0)//移动画布
	{
		int sx=point.x - m_ptLast.x;
		int sy=point.y - m_ptLast.y;
		m_ptLast = point;
		m_iOffsetX += sx;
		m_iOffsetY += sy;
		//判断是否超出画板范围
		SRECT rWnd = GetClientRect();
		int wndw = rWnd.right - rWnd.left;
		int wndh = rWnd.bottom - rWnd.top;
		SSvgObject *pFirstObj = m_Document.GetRootObject();
		if(pFirstObj == NULL)
			return 1;
		int svgw = GetCoordX(pFirstObj->GetRect()->right);
		int svgh = GetCoordY(pFirstObj->GetRect()->bottom);
		if(wndw < svgw)
		{
			//窗口宽度小于SVG宽度时
			if(m_iOffsetX < wndw-svgw)
				m_iOffsetX = wndw-svgw;
			if(m_iOffsetX > 0)
				m_iOffsetX = 0;
		}
		else
		{
			//窗口宽度大于SVG宽度时
			if(m_iOffsetX < 0)
				m_iOffsetX = 0;
			if(m_iOffsetX > wndw - svgw)
				m_iOffsetX = wndw - svgw;
		}
		if(wndh < svgh)
		{
			//窗口高度小于SVG宽度时
			if(m_iOffsetY < wndh-svgh)
				m_iOffsetY = wndh-svgh;
			if(m_iOffsetY > 0)
				m_iOffsetY = 0;
		}
		else
		{
			//窗口高度大于SVG宽度时
			if(m_iOffsetY < 0)
				m_iOffsetY = 0;
			if(m_iOffsetY > wndh - svgh)
				m_iOffsetY = wndh - svgh;
		}
		RedrawWindow();
		ResizeAllEmbeddedWnd();
		m_iMoving = 2;
	}
	else
	{
		m_pThisObj = NULL;
		float x = GetSvgCoordX(point.x - m_iOffsetX);
		float y = GetSvgCoordY(point.y - m_iOffsetY);
		SSvgObject *pObj = NULL;
		SSvgObject *pObj0 = NULL;
		//优先从选中图元中选择
		unsigned long pos=0;
		pObj = m_SelectedObjList.FetchFirst(pos);
		while(pObj)
		{
			if(IsObjectInPoint(pObj,x,y))
			{
				pObj0 = pObj;
				if( pObj->GetAttribute("uri").length() > 0)
					break;
			}
			pObj = m_SelectedObjList.FetchNext(pos);
		}
		pObj = pObj0;
		if(pObj == NULL)
		{
			pObj = m_Document.GetRootObject();
			if(pObj)
				pObj = pObj->GetChild();
			if(pObj)
				pObj = pObj->GetNext();
			pObj = IsObjectInPoint(pObj,x,y,true);
			if(pObj == NULL)
			{
				pObj = m_Document.GetRootObject();
				if(pObj)
					pObj = pObj->GetChild();
				if(pObj)
					pObj = pObj->GetNext();
				pObj = IsObjectInPoint(pObj,x,y,false);
			}
		}
		if(pObj != NULL)
		{
			bool bCatch = false;			
			if(m_bEditAllow && m_SelectedObjList.exist(pObj) && 
				pObj->GetType() != SSvgObject::TYPE_TEXT && 
				pObj->GetType() != SSvgObject::TYPE_POLYLINE &&
				 m_SelectedObjList.count() == 1)
			{
				//编辑模式下优先匹配当前对象的选中块，更改鼠标形状
				int px = point.x - m_iOffsetX;
				int py = point.y - m_iOffsetY;
				SRect r;
				r.left = GetCoordX(pObj->GetRect()->left);
				r.right = GetCoordX(pObj->GetRect()->right);
				r.top = GetCoordY(pObj->GetRect()->top);
				r.bottom = GetCoordY(pObj->GetRect()->bottom);
				if(IsPointNearPoint(px,py,r.left,r.top))
				{
					//左上
					m_iResizeMode = 1;
					bCatch = true;
					setCursor(Qt::SizeFDiagCursor);
				}
				else if(IsPointNearPoint(px,py,r.right,r.bottom))
				{
					//右下
					m_iResizeMode = 5;
					bCatch = true;
					setCursor(Qt::SizeFDiagCursor);
				}
				else if(IsPointNearPoint(px,py,r.right,r.top))
				{
					//右上
					m_iResizeMode = 3;
					bCatch = true;
					setCursor(Qt::SizeBDiagCursor);
				}
				else if(IsPointNearPoint(px,py,r.left,r.bottom))
				{
					//左下
					m_iResizeMode = 7;
					bCatch = true;
					setCursor(Qt::SizeBDiagCursor);
				}
				else if(IsPointNearPoint(px,py,r.left,(r.top+r.bottom)/2))
				{
					//左
					m_iResizeMode = 8;
					bCatch = true;
					setCursor(Qt::SizeHorCursor);
				}
				else if(IsPointNearPoint(px,py,r.right,(r.top+r.bottom)/2))
				{
					//右
					m_iResizeMode = 4;
					bCatch = true;
					setCursor(Qt::SizeHorCursor);
				}
				else if(IsPointNearPoint(px,py,(r.left+r.right)/2,r.top))
				{
					//上
					m_iResizeMode = 2;
					bCatch = true;
					setCursor(Qt::SizeVerCursor);
				}
				else if(IsPointNearPoint(px,py,(r.left+r.right)/2,r.bottom))
				{
					//下
					m_iResizeMode = 6;
					bCatch = true;
					setCursor(Qt::SizeVerCursor);
				}
				if(bCatch)
				{
					m_pThisObj = pObj;
					if(pObj->GetType() == SSvgObject::TYPE_LINE)
						setCursor(Qt::CrossCursor);
				}
			}
			SString caption = GetSvgObjectTipText(pObj);// pObj->GetAttribute("caption");
			if(caption.length() > 0)
			{
				QPoint p(point.x,point.y);
				p = this->mapToGlobal(p);
				QToolTip::showText(p,caption.data());
				bTip=true;
			}
			if(!bCatch)
			{
				m_iResizeMode = 0;
				if(m_SelectedObjList.exist(pObj))
					setCursor(Qt::SizeAllCursor);
				else
				{
					SString sExtAttr = pObj->GetAttribute("ext-attribute");
					SString type = SString::GetAttributeValue(sExtAttr,"type");
					SString act = SString::GetAttributeValue(sExtAttr,"act");
					if(pObj->GetAttribute("uri").length() > 0 || 
						type == "jump_fun" || type == "jump_wnd" || type == "float_wnd" || type == "general_act" ||
						act == "jump_fun" || act == "jump_wnd" || act == "float_wnd" || act == "general_act")
						setCursor(Qt::PointingHandCursor);
					else
						setCursor(Qt::ArrowCursor);
				}
			}
			OnMoveToObject(pObj);
		}
		else
		{
			m_iResizeMode = 0;
			setCursor(Qt::ArrowCursor);
			OnMoveToObject(NULL);
		}
	}
	if(!bTip)
	{
		QToolTip::hideText();
	}
	if(m_bMouseDown && m_pThisObj == NULL && m_bEditAllow)
	{
		//空白区域拖动
		m_ptThis = point;
		m_bDrawSelectRange = true;
		RedrawWindow();
	}
	return 1;
}

int SSvgWnd::OnMouseWheel(int nFlag,int delta)
{
	S_UNUSED(nFlag);
	if(m_bMovable)
	{
		if(QApplication::keyboardModifiers () != Qt::ControlModifier)
		{
			//未按Ctrl时为上下滚动画布
			int sx=0;
			int sy=delta>0?50:-50;
			m_iOffsetX += sx;
			m_iOffsetY += sy;
			//判断是否超出画板范围
			SRECT rWnd = GetClientRect();
			int wndw = rWnd.right - rWnd.left;
			int wndh = rWnd.bottom - rWnd.top;
			SSvgObject *pFirstObj = m_Document.GetRootObject();
			if(pFirstObj == NULL)
				return 1;
			int svgw = GetCoordX(pFirstObj->GetRect()->right);
			int svgh = GetCoordY(pFirstObj->GetRect()->bottom);
			if(wndw < svgw)
			{
				//窗口宽度小于SVG宽度时
				if(m_iOffsetX < wndw-svgw)
					m_iOffsetX = wndw-svgw;
				if(m_iOffsetX > 0)
					m_iOffsetX = 0;
			}
			else
			{
				//窗口宽度大于SVG宽度时
				if(m_iOffsetX < 0)
					m_iOffsetX = 0;
				if(m_iOffsetX > wndw - svgw)
					m_iOffsetX = wndw - svgw;
			}
			if(wndh < svgh)
			{
				//窗口高度小于SVG宽度时
				if(m_iOffsetY < wndh-svgh)
					m_iOffsetY = wndh-svgh;
				if(m_iOffsetY > 0)
					m_iOffsetY = 0;
			}
			else
			{
				//窗口高度大于SVG宽度时
				if(m_iOffsetY < 0)
					m_iOffsetY = 0;
				if(m_iOffsetY > wndh - svgh)
					m_iOffsetY = wndh - svgh;
			}
		}
		else
		{
			//只是可移动画布时才可缩放
			if(delta > 0)
			{
				if(m_fScale < 100)
					SSvgPainter::SetScale(m_fScale*1.1);
			}
			else if(delta < 0)
			{
				if(m_fScale > 0.1)
					SSvgPainter::SetScale(m_fScale/1.1);
			}
		}
		this->RedrawWindow();
		ResizeAllEmbeddedWnd();

	}
	return 0;
}



SSvgObject* SSvgWnd::GetSvgObjectByCaption(SString sCaption)
{
	return this->m_Document.SearchByAttribute("caption",sCaption.data());
}

SSvgObject* SSvgWnd::GetSvgObjectByUri(SString sUri)
{
	return this->m_Document.SearchByURI(sUri);
}

void SSvgWnd::SetMovable(bool b)
{
	m_bMovable = b;
}

void SSvgWnd::ShowAlignPoint()
{
	SetShowAlignPoint(true);
	RedrawWindow();
}
void SSvgWnd::HideAlignPoint()
{
	SetShowAlignPoint(false);
	RedrawWindow();
}

bool SSvgWnd::SetSvgText(SString sCaption, SString sText)
{
	LOGDEBUG("SetSvgText(%s,%s)",sCaption.data(),sText.data());
	SSvgObject *pSvgObj = this->GetSvgObjectByCaption(sCaption);
	if(pSvgObj == NULL)
	{
		LOGWARN("[%s]未在SVG中发现!修改内容为[%s]失败!",sCaption.data(),sText.data());
		return false;
	}
	pSvgObj->SetAttributeValue("text",sText);
#ifdef WIN32_UNUSED
	RECT rect;
	rect.left = pSvgObj->GetRect()->left;
	rect.top = pSvgObj->GetRect()->top;
	rect.right = pSvgObj->GetRect()->right;
	rect.bottom = pSvgObj->GetRect()->bottom;
	::InvalidateRect(m_hWnd,&rect,TRUE);
#else
#endif
	return true;
}
SPoint  SSvgWnd::GetOffsetPoint()
{
	SPoint point;
	point.x=m_iOffsetX;
	point.y=m_iOffsetY;
	return point;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  删除指定的顶层图元
// 作    者:  邵凯田
// 创建时间:  2015-8-13 10:01
// 参数说明:  @pObj为待删除图元
//         :  @bDestory为是否释放对象,释放后对象将不能再使用
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SSvgWnd::DeleteTopObject(SSvgObject *pObj,bool bDestory)
{
	m_Document.GetRootObject()->DeleteChild(pObj,bDestory);
}


//////////////////////////////////////////////////////////////////////////
// 描    述:  将当前所有选中的图元成组
// 作    者:  邵凯田
// 创建时间:  2015-8-13 9:59
// 参数说明:  void
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SSvgWnd::MakeGroup()
{
	if(m_SelectedObjList.count() == 0)
		return;
	unsigned long pos=0;
	SSvgObject *pObj = m_SelectedObjList.FetchFirst(pos);
	SSvgObject *pG = m_Document.AddObject("g","");
	while(pObj)
	{
		DeleteTopObject(pObj,false);
		pG->AddChild(pObj);
		pObj = m_SelectedObjList.FetchNext(pos);
	}
	pG->CalcObjectRect();
	m_SelectedObjList.clear();
	m_SelectedObjList.append(pG);
	Render();
	RedrawWindow();
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  解除当前选中的成组图元
// 作    者:  邵凯田
// 创建时间:  2015-8-13 9:59
// 参数说明:  void
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SSvgWnd::CancelGroup()
{
	if(m_SelectedObjList.count() == 0)
		return;
	SPtrList<SSvgObject> sel,sel2;
	unsigned long pos=0,pos2;
	SSvgObject *pObj, *pG = m_SelectedObjList.FetchFirst(pos);
	while(pG)
	{
		if(pG->GetType() != SSvgObject::TYPE_GROUP)
		{
			sel.append(pG);		
			pG = m_SelectedObjList.FetchNext(pos);
			continue;
		}
		DeleteTopObject(pG,false);
		pObj = pG->GetChild();
		pG->SetChild(NULL);
		while(pObj)
		{
			sel.append(pObj);			
			sel2.append(pObj);			
			pObj = pObj->GetNext();
		}
		while(pG->GetChild() != NULL)
			pG->DeleteChild(pG->GetChild());

		pObj = sel2.FetchFirst(pos2);
		while(pObj)
		{
			pObj->SetNext(NULL);
			m_Document.GetRootObject()->AddChild(pObj);
			pObj = sel2.FetchNext(pos2);
		}
		sel2.clear();

		pG = m_SelectedObjList.FetchNext(pos);
	}
	m_SelectedObjList.clear();
	sel.copyto(m_SelectedObjList);
	Render();
	RedrawWindow();
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  复制选中的图元
// 作    者:  邵凯田
// 创建时间:  2015-8-13 10:53
// 参数说明:  void
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SSvgWnd::CopySelected()
{
	if(m_SelectedObjList.count() == 0)
		return;
	SPtrList<SSvgObject> sel;
	unsigned long pos=0;
	SSvgObject *pNew,*pObj = m_SelectedObjList.FetchFirst(pos);
	SSvgObject *pRoot = m_Document.GetRootObject();
	while(pObj)
	{
		pNew = pObj->Clone(pRoot);
		//pNew->SetNext(NULL);
		pNew->Offset(10,10);
		sel.append(pNew);
		pObj = m_SelectedObjList.FetchNext(pos);
	}
	m_SelectedObjList.clear();
	sel.copyto(m_SelectedObjList);
	Render();
	RedrawWindow();
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  将所有选中图元左对齐
// 作    者:  邵凯田
// 创建时间:  2015-8-13 14:21
// 参数说明:  void
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SSvgWnd::AlignLeft()
{
	if(m_SelectedObjList.count() <= 1)
		return;
	unsigned long pos=0;
	float cx=0,cy=0;
	SSvgObject *pFirst = m_SelectedObjList.FetchFirst(pos);
	SSvgObject *pObj = m_SelectedObjList.FetchNext(pos);
	while(pObj)
	{
		cx = pFirst->GetRect()->left - pObj->GetRect()->left;
		pObj->Offset(cx,cy);
		pObj = m_SelectedObjList.FetchNext(pos);
	}
	RedrawWindow();
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  将所有选中图元右对齐
// 作    者:  邵凯田
// 创建时间:  2015-8-13 14:22
// 参数说明:  void
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SSvgWnd::AlignRight()
{
	if(m_SelectedObjList.count() <= 1)
		return;
	unsigned long pos=0;
	float cx=0,cy=0;
	SSvgObject *pFirst = m_SelectedObjList.FetchFirst(pos);
	SSvgObject *pObj = m_SelectedObjList.FetchNext(pos);
	while(pObj)
	{
		cx = pFirst->GetRect()->right - pObj->GetRect()->right;
		pObj->Offset(cx,cy);
		pObj = m_SelectedObjList.FetchNext(pos);
	}
	RedrawWindow();
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  将所有选中图元上对齐
// 作    者:  邵凯田
// 创建时间:  2015-8-13 14:22
// 参数说明:  void
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SSvgWnd::AlignTop()
{
	if(m_SelectedObjList.count() <= 1)
		return;
	unsigned long pos=0;
	float cx=0,cy=0;
	SSvgObject *pFirst = m_SelectedObjList.FetchFirst(pos);
	SSvgObject *pObj = m_SelectedObjList.FetchNext(pos);
	while(pObj)
	{
		cy = pFirst->GetRect()->top - pObj->GetRect()->top;
		pObj->Offset(cx,cy);
		pObj = m_SelectedObjList.FetchNext(pos);
	}
	RedrawWindow();
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  将所有选中图元下对齐
// 作    者:  邵凯田
// 创建时间:  2015-8-13 14:22
// 参数说明:  void
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SSvgWnd::AlignBottom()
{
	if(m_SelectedObjList.count() <= 1)
		return;
	unsigned long pos=0;
	float cx=0,cy=0;
	SSvgObject *pFirst = m_SelectedObjList.FetchFirst(pos);
	SSvgObject *pObj = m_SelectedObjList.FetchNext(pos);
	while(pObj)
	{
		cy = pFirst->GetRect()->bottom - pObj->GetRect()->bottom;
		pObj->Offset(cx,cy);
		pObj = m_SelectedObjList.FetchNext(pos);
	}
	RedrawWindow();
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  将所有选中图元水平居中对齐
// 作    者:  邵凯田
// 创建时间:  2015-8-13 14:23
// 参数说明:  void
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SSvgWnd::AlignCenter()
{
	if(m_SelectedObjList.count() <= 1)
		return;
	unsigned long pos=0;
	float cx=0,cy=0;
	SSvgObject *pFirst = m_SelectedObjList.FetchFirst(pos);
	SSvgObject *pObj = m_SelectedObjList.FetchNext(pos);
	while(pObj)
	{
		cx = ((pFirst->GetRect()->left+pFirst->GetRect()->right)/2 - (pObj->GetRect()->left+pObj->GetRect()->right)/2);
		pObj->Offset(cx,cy);
		pObj = m_SelectedObjList.FetchNext(pos);
	}
	RedrawWindow();
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  将所有选中图元垂直居中对齐
// 作    者:  邵凯田
// 创建时间:  2015-8-13 14:24
// 参数说明:  void
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SSvgWnd::AlignMiddle()
{
	if(m_SelectedObjList.count() <= 1)
		return;
	unsigned long pos=0;
	float cx=0,cy=0;
	SSvgObject *pFirst = m_SelectedObjList.FetchFirst(pos);
	SSvgObject *pObj = m_SelectedObjList.FetchNext(pos);
	while(pObj)
	{
		cy = ((pFirst->GetRect()->top+pFirst->GetRect()->bottom)/2 - (pObj->GetRect()->top+pObj->GetRect()->bottom)/2);
		pObj->Offset(cx,cy);
		pObj = m_SelectedObjList.FetchNext(pos);
	}
	RedrawWindow();
}

void SSvgWnd::OnAutoPlaceX()
{
	static int ssep=10;
	SString str = SQt::GetInput(tr("输入"),tr("请输入自动排列的水平间隔(像素):"),SString::toFormat("%d",ssep).data(),this);
	int sep = str.toInt();
	if(str.length() == 0)
		return;
// 	if(sep == 0)
// 		sep = 10;
	ssep = sep;
	AutoPlaceX(sep);
}

void SSvgWnd::OnAutoPlaceY()
{
	static int ssep = 10;
	SString str = SQt::GetInput(tr("输入"),tr("请输入自动排列的垂直间隔(像素):"),SString::toFormat("%d",ssep).data(),this);
	int sep = str.toInt();
	if(str.length() == 0)
		return;
// 	if(sep == 0)
// 		sep = 10;
	ssep = sep;
	AutoPlaceY(sep);
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  将所有选中图元按指定的间隔水平排列
// 作    者:  邵凯田
// 创建时间:  2015-8-13 14:25
// 参数说明:  @sep为两个图元间的间隔（即前一图元最右侧与后一图元最左侧的距离），单位为屏幕像素
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SSvgWnd::AutoPlaceX(int sep/*=10*/)
{
	if(m_SelectedObjList.count() <= 1)
		return;
	unsigned long pos=0;
	float fstart = 0;
	float fsep = sep/m_fScale;
	float cx=0,cy=0;
	SSvgObject *pFirst = m_SelectedObjList.FetchFirst(pos);
	SSvgObject *pObj = m_SelectedObjList.FetchNext(pos);
	fstart = pFirst->GetRect()->right;
	while(pObj)
	{
		cx = fstart+fsep - pObj->GetRect()->left;
		fstart = pObj->GetRect()->right + cx;
		pObj->Offset(cx,cy);
		pObj = m_SelectedObjList.FetchNext(pos);
	}
	RedrawWindow();
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  将所有图元按指定的间隔垂直排列
// 作    者:  邵凯田
// 创建时间:  2015-8-13 14:28
// 参数说明:  @sep为两个图元间的间隔（即前一图元最下侧与后一图元最上侧的距离），单位为屏幕像素
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SSvgWnd::AutoPlaceY(int sep/*=10*/)
{
	if(m_SelectedObjList.count() <= 1)
		return;
	unsigned long pos=0;
	float fstart = 0;
	float fsep = sep/m_fScale;//GetSvgCoord(sep);
	float cx=0,cy=0;
	SSvgObject *pFirst = m_SelectedObjList.FetchFirst(pos);
	SSvgObject *pObj = m_SelectedObjList.FetchNext(pos);
	fstart = pFirst->GetRect()->bottom;
	while(pObj)
	{
		cy = fstart+fsep - pObj->GetRect()->top;
		fstart = pObj->GetRect()->bottom + cy;
		pObj->Offset(cx,cy);
		pObj = m_SelectedObjList.FetchNext(pos);
	}
	RedrawWindow();
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  显示SVG属性窗口
// 作    者:  邵凯田
// 创建时间:  2015-8-20 15:35
// 参数说明:  @pObj为对象指针
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SSvgWnd::ShowAttributeWnd(SSvgObject *pObj,bool bShowAttrDlg/*=true*/)
{
	if(m_pWndAttribute == NULL)
	{
		m_pWndAttribute = new SSvgAttributeWnd(this);
		int x = width()-m_pWndAttribute->width()-4;
		int y = height()-m_pWndAttribute->height();
		if(x < 0)
			x = 0;
		if(y < 0)
			y = 0;
		m_pWndAttribute->move(x,y);
	}
	if(bShowAttrDlg)
		m_pWndAttribute->Show();
	m_pWndAttribute->LoadObject(pObj);
}

void SSvgWnd::HideAttributeWnd()
{
	if(m_pWndAttribute != NULL)
		m_pWndAttribute->Hide();
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  设置临时复制用的对象指针
// 作    者:  邵凯田
// 创建时间:  2015-8-21 22:08
// 参数说明:  
// 返 回 值:  
//////////////////////////////////////////////////////////////////////////
void SSvgWnd::SetTempCopyObject(SSvgObject *p)
{
	if(m_pTempCopyObject != NULL)
	{
		m_pTempCopyObject->Delete();
	}
	if(p == NULL)
	{
		m_pTempCopyObject = NULL;
		return;
	}
	m_pTempCopyObject = p->Clone(NULL,false,&m_Document);
	
}

SString SSvgWnd::GetSvgObjectTipText(SSvgObject *pSvgObj)
{
	SString caption;
	unsigned long pos;
	caption = pSvgObj->GetAttribute("caption");
	stuGlobalSvgObjCaptionCallback *p = g_GlobalSvgObjCaptionCallback.FetchFirst(pos);
	while(p)
	{
		if(p->pFun(p->cbParam,pSvgObj,caption))
			break;
		p = g_GlobalSvgObjCaptionCallback.FetchNext(pos);
	}
	return caption;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  添加全局SVG对象标题获取函数，根据SVG对象的URI确定对象类型，并返回对象的提示文本
// 作    者:  邵凯田
// 创建时间:  2017-7-6 8:54
// 参数说明:  @pFun为函数指针
//         :  @cbParam为用户指针
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SSvgWnd::RegistGlobalSvgObjCaptionCallback(GlobalSvgObjCaptionCallback pFun,void *cbParam)
{
	stuGlobalSvgObjCaptionCallback *p = new stuGlobalSvgObjCaptionCallback();
	p->pFun = pFun;
	p->cbParam = cbParam;
	g_GlobalSvgObjCaptionCallback.append(p);
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  去除全局SVG对象标题获取函数注册
// 作    者:  邵凯田
// 创建时间:  2017-7-6 9:25
// 参数说明:  @pFun为函数指针
//         :  @cbParam为用户指针
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SSvgWnd::UnregistGlobalSvgObjCaptionCallback(GlobalSvgObjCaptionCallback pFun,void *cbParam)
{
	unsigned long pos;
	bool bDel;
	stuGlobalSvgObjCaptionCallback *p0,*p = g_GlobalSvgObjCaptionCallback.FetchFirst(pos);
	while(p)
	{
		if(p->pFun == pFun && p->cbParam == cbParam)
			bDel = true;
		else
			bDel = false;
		p0 = p;
		p = g_GlobalSvgObjCaptionCallback.FetchNext(pos);
		if(bDel)
		{
			g_GlobalSvgObjCaptionCallback.remove(p0);
		}
	}
}
