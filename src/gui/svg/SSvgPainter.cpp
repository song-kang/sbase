/**
 *
 * �� �� �� : SSvgPainter.h
 * �������� : 2007-04-08 15:46
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : SVG���ֶ���Ļ���,��װ��SVG���ֻ���Ԫ�صĻ��Ʒ���
 * �޸ļ�¼ : 
 *            $Log: $
 *
 **/

#include "SSvgPainter.h"
#include "SSvgObject.h"
#include "../SPicture.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SSvgPainter::SSvgPainter()
{
	m_fScale = 1;
	m_ViewRect.left = 0;
	m_ViewRect.right = 0;
	m_ViewRect.top = 0;
	m_ViewRect.bottom = 0;
	m_bEditAllow = false;
	m_pSvgDataModel = NULL;
//	m_bDrawFastMode = false;
	m_iDrawUnSelectedMode = 0;
	m_pRedrawTimer = NULL;
	m_iLastDrawSoc = m_iLastDrawUSec = 0;
	m_iAutoFlashType = 0;
}

SSvgPainter::~SSvgPainter()
{
}

//���Ƶ�ǰ�������µ�ȫ������
void SSvgPainter::DrawObject(SBaseDC *pDC, SSvgObject *pObj, int x, int y,bool bOnlyOne,bool bUseObj/*=false*/)
{
	//������ƽ��//������
	int cx = 0,cy = 0;
	//float rx = 1,ry = 1;
	SSvgObject *p=pObj;
	SSvgObject *pDraw;
	while(p)
	{
		pDraw = p;
		if(p->GetType() == SSvgObject::TYPE_USE)
		{
			SSvgObject *pSymbol = m_Document.SearchSymbolObj(p->GetAttribute("xlink:href"));
			if(pSymbol == NULL)
				return;
			pDraw = pSymbol;
			cx = GetCoordX(p->GetAttributeI("x"));
			cy = GetCoordY(p->GetAttributeI("y"));
			SString transform = p->GetAttribute("transform");
			int p1,p2;
			p1 = transform.find("translate(");
			if(p1)
			{
				p1 += strlen("translate(");
				p2 = transform.find(")",p1);
				if(p2 > 0)
				{
					SString translate = transform.mid(p1,p2-p1-1);
// 					cx += SString::GetIdAttributeI(1,translate,",");
// 					cy += SString::GetIdAttributeI(2,translate,",");
				}
			}
	// 		p1 = transform.find("scale(");
	// 		if(p1)
	// 		{
	// 			p1 += strlen("scale(");
	// 			p2 = transform.find(")",p1);
	// 			if(p2 > 0)
	// 			{
	// 				SString scale = transform.mid(p1,p2-p1-1);
	// 				if(scale.find(",")>0)
	// 				{
	// 					rx = SString::GetIdAttributeF(1,scale,",");
	// 					ry = SString::GetIdAttributeF(2,scale,",");
	// 				}
	// 				else
	// 					rx =ry = scale.toFloat();
	// 			}
	// 		}
		}
		if(p->GetParent() == m_Document.GetRootObject())
		{
			//����
			if(p->IsFlash())
			{
				//��������ͼԪ
				p = p->GetNext();
				continue;
			}
			//��ǰ�����Ƿ���ѡ���б��У��������������ﻭ,��̬��ʱ���ж�
			//��ʹ�����ַ�ʽ�����ĳ�ֻҪѡ�п�
			if(m_iDrawUnSelectedMode == 1)
			{
				if(!m_SelectedObjList.exist(p))
				{
					p = p->GetNext();
					continue;
				}
			}
			else if(m_iDrawUnSelectedMode == 2)
			{
				if(m_SelectedObjList.exist(p))
				{
					p = p->GetNext();
					continue;
				}
			}
		}

		//if(0)//TODO !IsInViewRect(p))
		if((!IsInViewRect(p) || pDraw != p) && !bUseObj)
		{
			DrawObject(pDC,pDraw->GetChild(),x+cx,y+cy,false,pDraw != p);
			if(bOnlyOne)
				break;
			p = p->GetNext();
			continue;
		}
#ifdef QT_GUI_LIB
		int rotate = p->GetAttributeI("rotate")%360;
		SRect rect;
		SSvgObject::SVG_RECT *pSvgRect = p->GetRect();
		rect.left = GetCoordX(pSvgRect->left);
		rect.top = GetCoordY(pSvgRect->top);
		rect.right = GetCoordX(pSvgRect->right);
		rect.bottom = GetCoordY(pSvgRect->bottom);
		rect.OffsetRect(x+cx,y+cy);
		QPointF center((rect.right+rect.left)/2,(rect.bottom+rect.top)/2);
		QPainter *painter = ((QPainter*)pDC->GetHandle());
		if(rotate != 0)
		{
			painter->translate(center);
			painter->rotate(rotate);
			painter->translate(-center);
		}
#endif
		//����Ԫ�ز���
//		if(m_bEditAllow == false && p == m_Document.GetRootObject()->GetChild())
//		{
//			p = p->GetNext();
//			continue;
//		}
		switch(p->GetType())
		{
			case SSvgObject::TYPE_RECT:
				DrawRect(pDC,p,x+cx,y+cy);
				break;
			case SSvgObject::TYPE_RECTTEXT:
				DrawRectText(pDC,p,x+cx,y+cy);
				break;
			case SSvgObject::TYPE_TEXT:
				DrawSvgText(pDC,p,x+cx,y+cy);
				break;
			case SSvgObject::TYPE_ELLIPSE:
				DrawEllipse(pDC,p,x+cx,y+cy);
				break;
			case SSvgObject::TYPE_CIRCLE:
				DrawCircle(pDC,p,x+cx,y+cy);
				break;
			case SSvgObject::TYPE_LINE:
				DrawLine(pDC,p,x+cx,y+cy);
				break;
			case SSvgObject::TYPE_POLYLINE:
				DrawPolyLine(pDC,p,x+cx,y+cy);
				break;
			case SSvgObject::TYPE_PATH:
				DrawPath(pDC,p,x+cx,y+cy);
				break;
			case SSvgObject::TYPE_PIC:
				DrawPic(pDC,p,x+cx,y+cy);
				break;
			default:
				break;
		}
		if(p->GetChild() != NULL)
		{
			SSvgObject *pSub = p->GetChild();
			SString show_st = p->GetAttribute("show_st");//��ǰ���show_stֻ�Ե�һ���ӽڵ��st_val��Ч
			if(show_st.length() == 0)
				DrawObject(pDC,p->GetChild(),x+cx,y+cy);
			else
			{
				SString sub_show_st;
				while(pSub)
				{
					if(show_st.length() > 0)
					{
						sub_show_st = pSub->GetAttribute("st_val");
						if(sub_show_st.length() > 0 && sub_show_st != show_st)
						{
							//���Էǵ�ǰ״̬
							pSub = pSub->GetNext();
							continue;
						}
					}
					DrawObject(pDC,pSub,x+cx,y+cy,true);
					pSub = pSub->GetNext();
				}
			}
		}

#ifdef QT_GUI_LIB
		if(rotate != 0)
		{
			painter->translate(center);
			painter->rotate(-rotate);
			painter->translate(-center);
		}
#endif
		if(bOnlyOne)
			break;		
		p = p->GetNext();
		continue;

nextPos:
#ifdef QT_GUI_LIB
		if(rotate != 0)
		{
			painter->translate(center);
			painter->rotate(-rotate);
			painter->translate(-center);
		}
#endif
		continue;

breakPos:
#ifdef QT_GUI_LIB
		if(rotate != 0)
		{
			painter->translate(center);
			painter->rotate(-rotate);
			painter->translate(-center);
		}
#endif
		break;
	}
}

//���Ƶ�ǰ�������µ�ȫ������
void SSvgPainter::DrawFlashObject(SBaseDC *pDC, SSvgObject *pObj, int x, int y,bool bOnlyOne,bool bUseObj/*=false*/)
{
	//������ƽ��//������
	int cx = 0,cy = 0;
	//float rx = 1,ry = 1;
	SSvgObject *p=pObj;
	SSvgObject *pDraw;
	while(p)
	{
		pDraw = p;
		if(p->GetType() == SSvgObject::TYPE_USE)
		{
			SSvgObject *pSymbol = m_Document.SearchSymbolObj(p->GetAttribute("xlink:href"));
			if(pSymbol == NULL)
				return;
			pDraw = pSymbol;
			cx = GetCoordX(p->GetAttributeI("x"));
			cy = GetCoordY(p->GetAttributeI("y"));
			SString transform = p->GetAttribute("transform");
			int p1,p2;
			p1 = transform.find("translate(");
			if(p1)
			{
				p1 += strlen("translate(");
				p2 = transform.find(")",p1);
				if(p2 > 0)
				{
					SString translate = transform.mid(p1,p2-p1-1);
					// 					cx += SString::GetIdAttributeI(1,translate,",");
					// 					cy += SString::GetIdAttributeI(2,translate,",");
				}
			}
			// 		p1 = transform.find("scale(");
			// 		if(p1)
			// 		{
			// 			p1 += strlen("scale(");
			// 			p2 = transform.find(")",p1);
			// 			if(p2 > 0)
			// 			{
			// 				SString scale = transform.mid(p1,p2-p1-1);
			// 				if(scale.find(",")>0)
			// 				{
			// 					rx = SString::GetIdAttributeF(1,scale,",");
			// 					ry = SString::GetIdAttributeF(2,scale,",");
			// 				}
			// 				else
			// 					rx =ry = scale.toFloat();
			// 			}
			// 		}
		}
		if(p->GetParent() == m_Document.GetRootObject())
		{
			//����
			if(!p->IsFlash())
			{
				//�����Ƕ���ͼԪ
				p = p->GetNext();
				continue;
			}
			//��ǰ�����Ƿ���ѡ���б��У��������������ﻭ,��̬��ʱ���ж�
			//��ʹ�����ַ�ʽ�����ĳ�ֻҪѡ�п�
			if(m_iDrawUnSelectedMode == 1)
			{
				if(!m_SelectedObjList.exist(p))
				{
					p = p->GetNext();
					continue;
				}
			}
			else if(m_iDrawUnSelectedMode == 2)
			{
				if(m_SelectedObjList.exist(p))
				{
					p = p->GetNext();
					continue;
				}
			}
		}
		if(m_iAutoFlashType == 2 && p->IsAutoFlash())
		{
			p = p->GetNext();
			continue;
		}

		//if(0)//TODO !IsInViewRect(p))
		if((!IsInViewRect(p) || pDraw != p) && !bUseObj)
		{
			DrawFlashObject(pDC,pDraw->GetChild(),x+cx,y+cy,false,pDraw != p);
			if(bOnlyOne)
				break;
			p = p->GetNext();
			continue;
		}
#ifdef QT_GUI_LIB
		int rotate = p->GetAttributeI("rotate")%360;
		SRect rect;
		SSvgObject::SVG_RECT *pSvgRect = p->GetRect();
		rect.left = GetCoordX(pSvgRect->left);
		rect.top = GetCoordY(pSvgRect->top);
		rect.right = GetCoordX(pSvgRect->right);
		rect.bottom = GetCoordY(pSvgRect->bottom);
		rect.OffsetRect(x+cx,y+cy);
		QPointF center((rect.right+rect.left)/2,(rect.bottom+rect.top)/2);
		QPainter *painter = ((QPainter*)pDC->GetHandle());
		if(rotate != 0)
		{
			painter->translate(center);
			painter->rotate(rotate);
			painter->translate(-center);
		}
#endif
		//����Ԫ�ز���
		//		if(m_bEditAllow == false && p == m_Document.GetRootObject()->GetChild())
		//		{
		//			p = p->GetNext();
		//			continue;
		//		}
		switch(p->GetType())
		{
		case SSvgObject::TYPE_RECT:
			DrawRect(pDC,p,x+cx,y+cy);
			break;
		case SSvgObject::TYPE_RECTTEXT:
			DrawRectText(pDC,p,x+cx,y+cy);
			break;
		case SSvgObject::TYPE_TEXT:
			DrawSvgText(pDC,p,x+cx,y+cy);
			break;
		case SSvgObject::TYPE_ELLIPSE:
			DrawEllipse(pDC,p,x+cx,y+cy);
			break;
		case SSvgObject::TYPE_CIRCLE:
			DrawCircle(pDC,p,x+cx,y+cy);
			break;
		case SSvgObject::TYPE_LINE:
			DrawLine(pDC,p,x+cx,y+cy);
			break;
		case SSvgObject::TYPE_POLYLINE:
			DrawPolyLine(pDC,p,x+cx,y+cy);
			break;
		case SSvgObject::TYPE_PATH:
			DrawPath(pDC,p,x+cx,y+cy);
			break;
		case SSvgObject::TYPE_PIC:
			DrawPic(pDC,p,x+cx,y+cy);
			break;
		default:
			break;
		}
		if(p->GetChild() != NULL)
		{
			SSvgObject *pSub = p->GetChild();
			SString show_st = p->GetAttribute("show_st");//��ǰ���show_stֻ�Ե�һ���ӽڵ��st_val��Ч
			if(show_st.length() == 0)
				DrawFlashObject(pDC,p->GetChild(),x+cx,y+cy);
			else
			{
				SString sub_show_st;
				while(pSub)
				{
					if(show_st.length() > 0)
					{
						sub_show_st = pSub->GetAttribute("st_val");
						if(sub_show_st.length() > 0 && sub_show_st != show_st)
						{
							//���Էǵ�ǰ״̬
							pSub = pSub->GetNext();
							continue;
						}
					}
					DrawFlashObject(pDC,pSub,x+cx,y+cy,true);
					pSub = pSub->GetNext();
				}
			}
		}

#ifdef QT_GUI_LIB
		if(rotate != 0)
		{
			painter->translate(center);
			painter->rotate(-rotate);
			painter->translate(-center);
		}
#endif
		if(bOnlyOne)
			break;		
		p = p->GetNext();
		continue;

nextPos:
#ifdef QT_GUI_LIB
		if(rotate != 0)
		{
			painter->translate(center);
			painter->rotate(-rotate);
			painter->translate(-center);
		}
#endif
		continue;

breakPos:
#ifdef QT_GUI_LIB
		if(rotate != 0)
		{
			painter->translate(center);
			painter->rotate(-rotate);
			painter->translate(-center);
		}
#endif
		break;
	}
}
/*
void SSvgPainter::DrawFlashObject(SBaseDC *pDC, SSvgObject *pObj, int x, int y)
{
	SSvgObject *p=pObj;
	while(p)
	{
		if(p->GetParent() == m_Document.GetRootObject())
		{
			//����
			if(!p->IsFlash())
			{
				//�����Ƕ���ͼԪ
				p = p->GetNext();
				continue;
			}
		}
		if(!IsInViewRect(p))
		{
			DrawFlashObject(pDC,p->GetChild(),x,y);
			p = p->GetNext();
			continue;
		}
		if(m_iAutoFlashType == 2 && p->IsAutoFlash())
		{
			p = p->GetNext();
			continue;
		}

		switch(p->GetType())
		{
			case SSvgObject::TYPE_RECT:
				DrawRect(pDC,p,x,y);
				break;
			case SSvgObject::TYPE_RECTTEXT:
				DrawRectText(pDC,p,x,y);
				break;
			case SSvgObject::TYPE_TEXT:
				DrawSvgText(pDC,p,x,y);
				break;
			case SSvgObject::TYPE_ELLIPSE:
				DrawEllipse(pDC,p,x,y);
				break;
			case SSvgObject::TYPE_CIRCLE:
				DrawCircle(pDC,p,x,y);
				break;
			case SSvgObject::TYPE_LINE:
				DrawLine(pDC,p,x,y);
				break;
			case SSvgObject::TYPE_POLYLINE:
				DrawPolyLine(pDC,p,x,y);
				break;
			case SSvgObject::TYPE_PATH:
				DrawPath(pDC,p,x,y);
				break;
			case SSvgObject::TYPE_PIC:
				DrawPic(pDC,p,x,y);
				break;
			default:
				break;
		}
		DrawFlashObject(pDC,p->GetChild(),x,y);
		p = p->GetNext();
	}
}
*/

//���ݸ�������ƶ����
void SSvgPainter::DrawAlignPoint(SBaseDC *pDC/*, SSvgObject *pObj*/, int x, int y,int w,int h)
{
//	m_ViewRect.left
	float x0,y0,sep=10,x2,y2;
	x0 = x;//GetCoordX(m_ViewRect.left);
	y0 = y;//GetCoordY(m_ViewRect.top);
	x2 = x+w;//GetCoordX(m_ViewRect.right);
	y2 = y+h;//GetCoordY(m_ViewRect.bottom);
	SPen pen(PS_SOLID,1,SRGB(100,100,100));
	pDC->SelectObject(&pen);
	int px,py;
	for(px=sep+x;px<x2;px+=sep)
	{
		if(px<x0 || px>x2)
			continue;
		for(py=sep+y;py<y2;py+=sep)
		{
			if(py<y0 || py>y2)
				continue;
			//pDC->SetPixel(px,py,SRGB(255,0,0));
			pDC->MoveTo(px,py);
			pDC->LineTo(px,py);
		}
	}
}

void SSvgPainter::DrawObjectSelected(SBaseDC *pDC, SSvgObject *pObj, int x, int y, 
																			 SCOLOR colTag,bool bEditable)
{
	SRect rect,r2;
	rect.left = GetCoordX(pObj->GetRect()->left);
	rect.top = GetCoordY(pObj->GetRect()->top);
	rect.right = GetCoordX(pObj->GetRect()->right);
	rect.bottom = GetCoordY(pObj->GetRect()->bottom);
	rect.OffsetRect(x,y);
	SBrush b(colTag);
	SPen p(PS_DOT,1,SRGB(0,0,0));
	SPen* pOld = pDC->SelectObject(&p);
	switch(pObj->GetType())
	{
		case SSvgObject::TYPE_GROUP:
		case SSvgObject::TYPE_RECT:
		case SSvgObject::TYPE_RECTTEXT:
		case SSvgObject::TYPE_TEXT:
		case SSvgObject::TYPE_ELLIPSE:
		case SSvgObject::TYPE_CIRCLE:
		case SSvgObject::TYPE_PATH:
		case SSvgObject::TYPE_PIC:
		case SSvgObject::TYPE_USE:
// #ifdef QT_GUI_LIB
// 			if(pObj->GetType() == SSvgObject::TYPE_PIC)
// 			{
// 				int rotate = pObj->GetAttributeI("rotate")%360;
// 				QPointF center((rect.right+rect.left)/2,(rect.bottom+rect.top)/2);
// 				QPainter *painter = ((QPainter*)pDC->GetHandle());
// 				if(rotate > 0)
// 				{
// 					painter->translate(center);
// 					painter->rotate(rotate);
// 					painter->translate(-center);
// 				}
// 			}
// #endif


			if(bEditable== false || 
				(pObj->GetType() != SSvgObject::TYPE_RECT &&
				 pObj->GetType() != SSvgObject::TYPE_GROUP &&
				 pObj->GetType() != SSvgObject::TYPE_USE &&
				 pObj->GetType() != SSvgObject::TYPE_PIC ))
			{
				pDC->MoveTo(rect.left,rect.top);
				pDC->LineTo(rect.right,rect.top);
				pDC->LineTo(rect.right,rect.bottom);
				pDC->LineTo(rect.left,rect.bottom);
				pDC->LineTo(rect.left,rect.top);
			}
			if(bEditable)
			{
				//���Ͻ�
				r2.left = rect.left - C_TAG_RECT_RADIUS;
				r2.top = rect.top - C_TAG_RECT_RADIUS;
				r2.right = rect.left + C_TAG_RECT_RADIUS;
				r2.bottom = rect.top + C_TAG_RECT_RADIUS;
				pDC->FillRect(&r2,&b);
				//���½�
				r2.left = rect.left - C_TAG_RECT_RADIUS;
				r2.top = rect.bottom - C_TAG_RECT_RADIUS;
				r2.right = rect.left + C_TAG_RECT_RADIUS;
				r2.bottom = rect.bottom + C_TAG_RECT_RADIUS;
				pDC->FillRect(&r2,&b);
				//���Ͻ�
				r2.left = rect.right - C_TAG_RECT_RADIUS;
				r2.top = rect.top - C_TAG_RECT_RADIUS;
				r2.right = rect.right + C_TAG_RECT_RADIUS;
				r2.bottom = rect.top + C_TAG_RECT_RADIUS;
				pDC->FillRect(&r2,&b);
				//���½�
				r2.left = rect.right - C_TAG_RECT_RADIUS;
				r2.top = rect.bottom - C_TAG_RECT_RADIUS;
				r2.right = rect.right + C_TAG_RECT_RADIUS;
				r2.bottom = rect.bottom + C_TAG_RECT_RADIUS;
				pDC->FillRect(&r2,&b);
				if(pObj->GetType() != SSvgObject::TYPE_TEXT)//���ֲ���Ҫ�в�����
				{
					//����
					r2.left = rect.left - C_TAG_RECT_RADIUS;
					r2.top = rect.top+(rect.bottom-rect.top)/2 - C_TAG_RECT_RADIUS;
					r2.right = rect.left + C_TAG_RECT_RADIUS;
					r2.bottom = rect.top+(rect.bottom-rect.top)/2 + C_TAG_RECT_RADIUS;
					pDC->FillRect(&r2,&b);
					//����
					r2.left = rect.right - C_TAG_RECT_RADIUS;
					r2.top = rect.top+(rect.bottom-rect.top)/2 - C_TAG_RECT_RADIUS;
					r2.right = rect.right + C_TAG_RECT_RADIUS;
					r2.bottom = rect.top+(rect.bottom-rect.top)/2 + C_TAG_RECT_RADIUS;
					pDC->FillRect(&r2,&b);
					//����
					r2.left = rect.left+(rect.right-rect.left)/2 - C_TAG_RECT_RADIUS;
					r2.top = rect.top - C_TAG_RECT_RADIUS;
					r2.right = rect.left+(rect.right-rect.left)/2 + C_TAG_RECT_RADIUS;
					r2.bottom = rect.top + C_TAG_RECT_RADIUS;
					pDC->FillRect(&r2,&b);
					//����
					r2.left = rect.left+(rect.right-rect.left)/2 - C_TAG_RECT_RADIUS;
					r2.top = rect.bottom - C_TAG_RECT_RADIUS;
					r2.right = rect.left+(rect.right-rect.left)/2 + C_TAG_RECT_RADIUS;
					r2.bottom = rect.bottom + C_TAG_RECT_RADIUS;
					pDC->FillRect(&r2,&b);
				}
			}


// #ifdef QT_GUI_LIB
// 			if(pObj->GetType() == SSvgObject::TYPE_PIC)
// 			{
// 				int rotate = pObj->GetAttributeI("rotate")%360;
// 				QPointF center((rect.right+rect.left)/2,(rect.bottom+rect.top)/2);
// 				QPainter *painter = ((QPainter*)pDC->GetHandle());
// 				painter->translate(center);
// 				painter->rotate(-rotate);
// 				painter->translate(-center);
// 			}
// #endif
			break;
		case SSvgObject::TYPE_LINE:
			{
				float x1=GetCoordX(pObj->GetAttributeF("x1"))+x;
				float y1=GetCoordY(pObj->GetAttributeF("y1"))+y;
				float x2=GetCoordX(pObj->GetAttributeF("x2"))+x;
				float y2=GetCoordY(pObj->GetAttributeF("y2"))+y;
//				pDC->MoveTo(x1,y1);
//				pDC->LineTo(x2,y2);
				if(bEditable)
				{
					//P1
					r2.left = x1+ - C_TAG_RECT_RADIUS;
					r2.top = y1 - C_TAG_RECT_RADIUS;
					r2.right = x1+ C_TAG_RECT_RADIUS;
					r2.bottom = y1 + C_TAG_RECT_RADIUS;
					pDC->FillRect(&r2,&b);
					//P2
					r2.left = x2+ - C_TAG_RECT_RADIUS;
					r2.top = y2 - C_TAG_RECT_RADIUS;
					r2.right = x2+ C_TAG_RECT_RADIUS;
					r2.bottom = y2 + C_TAG_RECT_RADIUS;
					pDC->FillRect(&r2,&b);
				}
			}
			break;
		case SSvgObject::TYPE_POLYLINE:
			{
				SString points=pObj->GetAttribute("points");
				int p1=0,p2,p3;
				int x0,y0;
				int index=0;
				int hover_idx=-1;
				SString hover_idx_str = pObj->GetAttribute("hover_idx");
				if(hover_idx_str.length() > 0)
					hover_idx = hover_idx_str.toInt();
				while(1)
				{
					p2=points.Find(",",p1);
					if(p2 < 0)
						break;
					x0 = GetCoordX(SString::toInt(points.Mid(p1,p2-p1)))+x;
					p2 ++;
					p3=points.Find(" ",p2);
					if(p3 < 0)
						p3 = points.GetLength();
					y0 = GetCoordY(SString::toInt(points.Mid(p2,p3-p2)))+y;
					p1 = p3+1;
//					if(index==0)
//						pDC->MoveTo(x0, y0);
//					else
//						pDC->LineTo(x0, y0);
					if(bEditable)
					{
						if(hover_idx == index)
						{
							//���������
							r2.left = x0 - C_TAG_RECT_RADIUS*2;
							r2.top = y0 - C_TAG_RECT_RADIUS*2;
							r2.right = x0+ C_TAG_RECT_RADIUS*2;
							r2.bottom = y0 + C_TAG_RECT_RADIUS*2;
						}
						else
						{
							r2.left = x0 - C_TAG_RECT_RADIUS;
							r2.top = y0 - C_TAG_RECT_RADIUS;
							r2.right = x0+ C_TAG_RECT_RADIUS;
							r2.bottom = y0 + C_TAG_RECT_RADIUS;
						}
						pDC->FillRect(&r2,&b);
					}
					index++;
				}

			}
			break;
		default:
			break;
	}
	pDC->SelectObject(pOld);
}

//ת��SVG���굽��ǰ����
int SSvgPainter::GetCoord(float x)
{
	return (int)(x*m_fScale);
}
int SSvgPainter::GetCoordX(float x)
{
	SSvgObject *pRootObj = m_Document.GetRootObject();
	if(pRootObj != NULL && m_Document.GetOrigX() != 0)
	{
		x = x - m_Document.GetOrigX();
	}
	return (int)(x*m_fScale);
}

int SSvgPainter::GetCoordY(float y)
{
	SSvgObject *pRootObj = m_Document.GetRootObject();
	if(pRootObj != NULL /*&& m_Document.GetOrigY() != 0*/)
	{
		if(m_Document.GetCoorType() == 1)
		{
			//�������½�����
			y = m_Document.GetHeight() - (y - m_Document.GetOrigY());
		}
		else
		{
			//�������Ͻ�����
			y = y - m_Document.GetOrigY();
		}
	}
	return (int)(y*m_fScale);
}
//ת����ǰ���ڵ�SVG����
float SSvgPainter::GetSvgCoord(float x)
{
	return x = x/m_fScale;
}
float SSvgPainter::GetSvgCoordX(float x)
{
	x = x/m_fScale;
	SSvgObject *pRootObj = m_Document.GetRootObject();
	if(pRootObj != NULL && m_Document.GetOrigX() != 0)
	{
		x = x + m_Document.GetOrigX();
	}
	return x;
}
float SSvgPainter::GetSvgCoordY(float y)
{
	y = y/m_fScale;
	SSvgObject *pRootObj = m_Document.GetRootObject();
	if(pRootObj != NULL /*&& m_Document.GetOrigY() != 0*/)
	{
		if(m_Document.GetCoorType() == 1)
		{
			//�������½�����
			y = m_Document.GetHeight() - (y + m_Document.GetOrigY());
		}
		else
		{
			//�������Ͻ�����
			y = y + m_Document.GetOrigY();
		}
	}
	return y;
}
int GetHexByStr(char *str)
{
	int b=0;
	char ch;
	ch=str[0];
	if(ch>='A' && ch <='F')
		ch = 10 + (ch -'A');
	else if(ch >='a' && ch <='f')
		ch = 10 + (ch - 'a');
	else
		ch = ch - '0';
	b = ch;
	ch=str[1];
	if(ch>='A' && ch <='F')
		ch = 10 + (ch -'A');
	else if(ch >='a' && ch <='f')
		ch = 10 + (ch - 'a');
	else
		ch = ch - '0';
	b = (b<<4) + ch;
	return b;
}

//ת����ɫ
SCOLOR SSvgPainter::GetColorByString(SString sColor)
{
	if(sColor == "none")
		return SRGB(0,0,0,0);
	BYTE r=0,g=0,b=0;
	char *pStr=(char*)sColor.GetBuffer(0);
	switch(*pStr)
	{
	case '#':
		r = GetHexByStr(pStr+1);
		g = GetHexByStr(pStr+3);
		b = GetHexByStr(pStr+5);
		break;
	case 'r'://rgb/red
	//case 'R':
		if(pStr[1] == 'e')
		{
			//red
			return SRGB(255,0,0);
		}
		else
		{
			//rgb
			pStr+=4;
			r = (BYTE)atoi(pStr);
			pStr = strstr(pStr,",");
			if(pStr++ != NULL)
			{
				g = (BYTE)atoi(pStr);
				pStr = strstr(pStr,",");
				if(pStr++ != NULL)
					b = (BYTE)atoi(pStr);
				else
					b = 0;
			}
			else
			{
				g = 0;
				b = 0;				
			}
		}
		break;
	case 'b'://black/blue
	//case 'B':
		if(pStr[2] == 'a')
		{
			//black
			return SRGB(0,0,0);
		}
		else
		{
			//blue
			return SRGB(0,0,255);
		}
		break;
	case 'w'://white
	//case 'W':
		return SRGB(255,255,255);
		break;
	case 'g'://green
	//case 'G':
		return SRGB(0,255,0);

	}
	return SRGB(r,g,b);
}

SString SSvgPainter::GetStringByColor(SCOLOR color)
{
	static char buf[16];
	memset(buf,0,sizeof(buf));
	sprintf(buf,"#%02X%02X%02X",color.r,color.g,color.b);
	return buf;
}

SString SSvgPainter::GetRGBStringByColor(SCOLOR color)
{
	static char buf[16];
	memset(buf,0,sizeof(buf));
	sprintf(buf,"%d,%d,%d",color.r,color.g,color.b);
	return buf;
}

/************************************************************************/
/* rect
 	x
	y
	z
	width
	height
	stroke		�߿���ɫ
	stroke-width �߿���
	fill		�����ɫ��none��ʾ�����,д����#55aa00��Ҳ����д��rgb(255,0,0)��
					�����ַ�ʽ��˼����ʶ
*/
/************************************************************************/
void SSvgPainter::DrawRect(SBaseDC *pDC, SSvgObject *pObj, int x, int y)
{
	SRect rect;
	rect.left = GetCoordX(pObj->GetAttributeF("x"));
	rect.top = GetCoordY(pObj->GetAttributeF("y"));
	rect.right = GetCoordX(pObj->GetAttributeF("x")+pObj->GetAttributeF("width"));
	rect.bottom = GetCoordY(pObj->GetAttributeF("y")+pObj->GetAttributeF("height"));
	rect.OffsetRect(x,y);
	SString sAlpha = pObj->GetAttribute("alpha");
	int alpha = 255;
	if(sAlpha.length() > 0)
		alpha = sAlpha.toInt();

#ifdef QT_GUI_LIB
	bool bFill=false;
	bool bGradient=false;//�Ƿ�Ϊ����ɫ���
	SString sGradientId;
	SCOLOR sBackColor,sLineColor;
	if(m_pSvgDataModel && m_pSvgDataModel->GetViewBackColor(pObj,bFill,sBackColor))
	{
	}
	else
	{
		SString sFill = GetAttributeByLevel(pObj,"fill");//pObj->GetAttribute("fill");
		if(sFill != "" && sFill.CompareNoCase("none") != 0)
		{
			//�������
			bFill = true;
			sFill = pObj->GetAttribute("fill");
			//�ж��Ƿ��ǽ���
			if(sFill.length() > 6 && sFill.left(5) == "url(#" && sFill.right(1) == ")")
			{
				bGradient = true;
				sGradientId = sFill.mid(5,sFill.length()-6);
			}
			else
				sBackColor = GetColorByString(sFill);
		}
	}
	if(!(m_pSvgDataModel && m_pSvgDataModel->GetViewLineColor(pObj,sLineColor)))
		sLineColor = GetColorByString(GetAttributeByLevel(pObj,"stroke"));
	if(!bFill && pObj->GetAttributeI("stroke-width") == 0)
		return;
	float line_width = pObj->GetAttributeI("stroke-width")*m_fScale;
	if(line_width < 1)
		line_width = 1;
	QBrush bLine(QColor(sLineColor.r,sLineColor.g,sLineColor.b,alpha));
	int penstyle = pObj->GetAttributeI("stroke-dasharray")+1;
	QPen pen(bLine,line_width,(Qt::PenStyle)((sLineColor.a==0)? (Qt::NoPen):penstyle));
	bool bFillRect = false;
	QPainter *painter = ((QPainter*)pDC->GetHandle());
	QRect qrect(rect.left,rect.top,rect.width(),rect.height());
	if(bFill)
	{
		if(bGradient)
		{
			//����
			SSvgDocument::stuLinearGradient *pLinear = m_Document.SearchLinearGradientById(sGradientId);
			SSvgDocument::stuRadialGradient *pRadial;
			if(pLinear != NULL)
			{
				
				QLinearGradient linear(QPointF(rect.left+pLinear->x1*rect.width()/100, rect.top+pLinear->y1*rect.height()/100), 
										QPointF(rect.left+pLinear->x2*rect.width()/100, rect.top+pLinear->y2*rect.height()/100));
				unsigned long pos;
				SSvgDocument::stuGradientStop *pStop = pLinear->Stops.FetchFirst(pos);
				while(pStop)
				{
					float fpos = ((float)pStop->offset)/100.0f;
					if(fpos < 0)
						fpos = 0;
					if(fpos > 1)
						fpos = 1;
					linear.setColorAt(fpos, QColor(pStop->color.r,pStop->color.g,pStop->color.b,alpha));
					pStop = pLinear->Stops.FetchNext(pos);
				}
				painter->setBrush(linear);
			}
			else if((pRadial=m_Document.SearchRadialGradientById(sGradientId)) != NULL)
			{
				QRadialGradient radial(rect.left+pRadial->cx*rect.width()/100,rect.top+pRadial->cy*rect.height()/100,
					pRadial->r*((rect.width()+rect.height())/2)/100,
					rect.left+pRadial->fx*rect.width()/100,rect.top+pRadial->fy*rect.height()/100);
				unsigned long pos;
				SSvgDocument::stuGradientStop *pStop = pRadial->Stops.FetchFirst(pos);
				while(pStop)
				{
					float fpos = ((float)pStop->offset)/100.0f;
					if(fpos < 0)
						fpos = 0;
					if(fpos > 1)
						fpos = 1;
					radial.setColorAt(fpos, QColor(pStop->color.r,pStop->color.g,pStop->color.b,alpha));
					pStop = pRadial->Stops.FetchNext(pos);
				}
				painter->setBrush(radial);
			}
		}
		else
		{
			//��ɫ
			QBrush bBack(QColor(sBackColor.r,sBackColor.g,sBackColor.b,alpha));
			painter->setBrush(bBack);
			bFillRect = true;
		}
	}
	else
		painter->setBrush(Qt::NoBrush);
	painter->setPen(GetAttributeByLevel(pObj,"stroke") == "none"?Qt::NoPen:pen);
	int round_x = GetCoord(pObj->GetAttributeI("rx"));
	int round_y = GetCoord(pObj->GetAttributeI("ry"));
	if(round_x == 0 && round_y == 0)
	{
		if(bFillRect)
			painter->fillRect(qrect,painter->brush());
		painter->drawLine(qrect.left(),qrect.top(),qrect.right(),qrect.top());
		painter->drawLine(qrect.right(),qrect.top(),qrect.right(),qrect.bottom());
		painter->drawLine(qrect.right(),qrect.bottom(),qrect.left(),qrect.bottom());
		painter->drawLine(qrect.left(),qrect.bottom(),qrect.left(),qrect.top());
		//painter->drawRect(qrect);
	}
	else
		painter->drawRoundedRect(qrect, round_x, round_y);
#else
	bool bFill;
	SCOLOR sColor;
	if(m_pSvgDataModel && m_pSvgDataModel->GetViewBackColor(pObj,bFill,sColor))
	{
		if(bFill)
		{
			SBrush b(sColor);
			pDC->FillRect(&rect,&b);
		}
	}
	else
	{
		SString sFill = GetAttributeByLevel(pObj,"fill");//pObj->GetAttribute("fill");
		if(sFill != "" && sFill.CompareNoCase("none") != 0)
		{
			//�������
			SBrush b(GetColorByString(pObj->GetAttribute("fill")));
			pDC->FillRect(&rect,&b);
		}
	}
	if(!(m_pSvgDataModel && m_pSvgDataModel->GetViewLineColor(pObj,sColor)))
		sColor = GetColorByString(GetAttributeByLevel(pObj,"stroke"));
	int line_width = GetCoord(pObj->GetAttributeI("stroke-width"));
	if(line_width <= 0)
		line_width = 1;
	SPen p(pObj->GetAttributeI("stroke-dasharray"),line_width/*pObj->GetAttributeI("stroke-width")*/,sColor);//pObj->GetAttribute("stroke")
	SPen* pOld = pDC->SelectObject(&p);
	pDC->MoveTo(rect.left,rect.top);
	pDC->LineTo(rect.right,rect.top);
	pDC->LineTo(rect.right,rect.bottom);
	pDC->LineTo(rect.left,rect.bottom);
	pDC->LineTo(rect.left,rect.top);
	pDC->SelectObject(pOld);
#endif
	//�������Ϊ�û��Ի�ʱ�������Ի溯��
	if(pObj->IsUserDraw())
	{
//#ifndef QT_GUI_LIB
#if 1
		DrawUser(pDC,rect,pObj);
#else
		if(m_bDrawFastMode == false || pObj->m_pUserDrawPixmap == NULL)
		{
			//���Ƶ�λͼ
			if(m_bDrawFastMode == false && pObj->m_pUserDrawPixmap != NULL &&
				(pObj->m_pUserDrawPixmap->width() != abs(rect.right-rect.left) ||  
					pObj->m_pUserDrawPixmap->height() != abs(rect.bottom-rect.top)))
			{
				delete pObj->m_pUserDrawPixmap;
				pObj->m_pUserDrawPixmap = NULL;
			}
			if(pObj->m_pUserDrawPixmap == NULL)
				pObj->m_pUserDrawPixmap = new QPixmap(QSize(abs(rect.right-rect.left),abs(rect.bottom-rect.top)));
			//������Ƶ�λͼ��
			QPainter *painter = new QPainter(pObj->m_pUserDrawPixmap);
			SDC qtDc((HDC)new QPainter(pObj->m_pUserDrawPixmap));
			SRect r;
			r.left = 0;
			r.top = 0;
			r.right = abs(rect.right-rect.left);
			r.bottom = abs(rect.bottom-rect.top);
			DrawUser(&qtDc,r,pObj);
		}

		//��λͼ�����Ƶ�����
		SDC *pQtDc = (SDC*)pDC;
		QRectF pt(rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top);
		QRectF source(0.0, 0.0, rect.right-rect.left, rect.bottom-rect.top);
		pQtDc->m_hDC->drawPixmap(pt,*pObj->m_pUserDrawPixmap,source);
#endif
	}
}

/************************************************************************/
/* text
	x
	y			��ʾ���ֵײ���λ��
	z			
	font-family	������
	font-size	�����С
	font-weight	����
	fill		��ɫ
*/
/************************************************************************/
#ifndef DEFAULT_CHARSET
#define DEFAULT_CHARSET 1
#endif

void SSvgPainter::DrawSvgText(SBaseDC *pDC, SSvgObject *pObj, int x, int y)
{
	SString weight = pObj->GetAttribute("font-weight");
	SString family = pObj->GetAttribute("font-family");
	SFont font;
	SString str;
	SString *pText = NULL;
	SSvgAttribute::stuAttribute *pAttr = pObj->GetAttributePtr()->Find("text");
	if(pAttr != NULL)
		pText = &pAttr->value;
	else
		pText = &str;
	if(!(m_pSvgDataModel && m_pSvgDataModel->GetViewText(pObj,*pText) && m_pSvgDataModel->GetViewTextFont(pObj,font)))
	{
		//memset(&font,0,sizeof(font));
		font.m_iHeight = pObj->GetAttributeF("font-size")*m_fScale;//GetCoordX(pObj->GetAttributeF("font-size"));
		if(font.m_iHeight <= 1)
			return;//����û�д�С������
		font.m_iWidth = 0;
	#ifdef QT_GUI_LIB
		if(weight=="bold")
			font.m_iWeight = 75;
		else
			font.m_iWeight = 50;
	#else
		if(weight=="bold")
			font.m_iWeight = 700;
		else
			font.m_iWeight = 500;
	#endif
		if(pObj->GetAttribute("font-italic") == "italic")
			font.fdwItalic = 1;
		else
			font.fdwItalic = 0;
		font.fdwCharSet  = DEFAULT_CHARSET;
		strcpy(font.szFace,family.GetBuffer());
	}
	pDC->SelectObject(&font);
	SSize textsize=GetTextSize(pDC,pText->data());
	SSvgObject::SVG_RECT *pr = pObj->GetRect();
	//pr->bottom = pr->top+textsize.cy;
	int x0 = GetCoordX(pObj->GetAttributeF("x"));
#ifdef QT_GUI_LIB
	int y0 = GetCoordY((pObj->GetAttributeF("y")));
#else
	int y0 = GetCoordY((pObj->GetAttributeF("y")-textsize.cy));
#endif
	SString text_anchor = pObj->GetAttribute("text-anchor");
	if(text_anchor == "middle")
	{
		x0 -= textsize.cx/2;
	}
	else if(text_anchor == "end")
	{
		x0 -= textsize.cx;
	}

	pr->left = GetSvgCoordX(x0);
	pr->right = GetSvgCoordX(x0+textsize.cx);
	pr->bottom = GetSvgCoordY(y0);
	pr->top = GetSvgCoordY(y0-textsize.cy);
	
	//int size = GetCoord(pObj->GetAttributeF("font-size"));
	SCOLOR sColor;
	if(!(m_pSvgDataModel && m_pSvgDataModel->GetViewTextColor(pObj,sColor)))
		sColor = GetColorByString(GetAttributeByLevel(pObj,"fill"));
	pDC->SetTextColor(sColor);//pObj->GetAttribute("fill")
	pDC->TextOut(x0+x,y0+y,pText->data());
}

void SSvgPainter::DrawRectText(SBaseDC *pDC, SSvgObject *pObj, int x, int y)
{
	SRect rect;
	rect.left = GetCoordX(pObj->GetAttributeF("x"));
	rect.top = GetCoordY(pObj->GetAttributeF("y"));
	rect.right = GetCoordX(pObj->GetAttributeF("x")+pObj->GetAttributeF("width"));
	rect.bottom = GetCoordY(pObj->GetAttributeF("y")+pObj->GetAttributeF("height"));
	rect.OffsetRect(x,y);

	SString weight = pObj->GetAttribute("font-weight");
	SString family = pObj->GetAttribute("font-family");
	SFont font;
	SString str;
	SString *pText = NULL;
	SSvgAttribute::stuAttribute *pAttr = pObj->GetAttributePtr()->Find("text");
	if(pAttr != NULL)
		pText = &pAttr->value;
	else
		pText = &str;
	if(!(m_pSvgDataModel && m_pSvgDataModel->GetViewText(pObj,*pText) && m_pSvgDataModel->GetViewTextFont(pObj,font)))
	{
		//memset(&font,0,sizeof(font));
		font.m_iHeight = GetCoord(pObj->GetAttributeF("font-size"));
		if(font.m_iHeight <= 1)
			return;//����û�д�С������
		font.m_iWidth = 0;
#ifdef QT_GUI_LIB
		if(weight=="bold")
			font.m_iWeight = 75;
		else
			font.m_iWeight = 50;
#else
		if(weight=="bold")
			font.m_iWeight = 700;
		else
			font.m_iWeight = 500;
#endif
		if(pObj->GetAttribute("font-italic") == "italic")
			font.fdwItalic = 1;
		else
			font.fdwItalic = 0;
		font.fdwCharSet  = DEFAULT_CHARSET;
		strcpy(font.szFace,family.GetBuffer());
	}
	pDC->SelectObject(&font);
	SCOLOR sColor;
	if(!(m_pSvgDataModel && m_pSvgDataModel->GetViewTextColor(pObj,sColor)))
		sColor = GetColorByString(GetAttributeByLevel(pObj,"fill"));
	pDC->SetTextColor(sColor);//pObj->GetAttribute("fill")
	int flag = Qt::TextWordWrap;
	SString alignv = pObj->GetAttribute("alignv");
	SString alignh = pObj->GetAttribute("alignh");
	if(alignv == "top")
		flag += Qt::AlignTop;
	else if(alignv == "bottom")
		flag += Qt::AlignBottom;
	else
		flag += Qt::AlignVCenter;
	if(alignh == "left")
		flag += Qt::AlignLeft;
	else if(alignh == "right")
		flag += Qt::AlignRight;
	else
		flag += Qt::AlignHCenter;

#ifdef QT_GUI_LIB
	QRectF r(rect.left,rect.top,abs(rect.right-rect.left),abs(rect.bottom-rect.top));
	SDC *pQtDc = (SDC*)pDC;
	pQtDc->m_hDC->drawText(r,flag,QString(pText->data()));
#endif
}

SBaseWnd *m_pBaseWnd = NULL;
void SSvgPainter::SetBaseWnd(SBaseWnd *p)
{
	m_pBaseWnd = p;
};

SRect* SSvgPainter::PointToSelRect(int x,int y)
{
	static SRect r;
	const int sep = 3;
	r.left = x - sep;
	r.right = x + sep;
	r.top = y - sep;
	r.bottom = y + sep;
	return &r;
}

//ȡһ���ı��Ĵ�С
YSSIZE SSvgPainter::GetTextSize(SBaseDC *pDC, SSvgObject *pObj)
{
	bool bNew = false;
	if(pDC == NULL)
	{
		pDC = m_pBaseWnd->CreateDC();
		bNew = true;
	}
	YSSIZE tsize = pDC->GetTextSize(pObj->GetAttribute("text").data());
	if(bNew)
		m_pBaseWnd->ReleaseDC(pDC);

	return tsize;
}

//ȡһ���ı��Ĵ�С
YSSIZE SSvgPainter::GetTextSize(SBaseDC *pDC, char *pStr)
{
	bool bNew = false;
	if(pDC == NULL)
	{
		pDC = m_pBaseWnd->CreateDC();
		bNew = true;
	}
	YSSIZE tsize = pDC->GetTextSize(pStr);
	if(bNew)
		m_pBaseWnd->ReleaseDC(pDC);

	return tsize;
}

/************************************************************************/
/* ellipse
	z
	cx			X�������ĵ�
	cy			Y�������ĵ�
	rx			X����뾶
	ry			Y����뾶
	stroke		�߿���ɫ
	stroke-width �߿���
	stroke-dasharray ������ʽ(��ѡ)
	fill		�����ɫ��none��ʾ�����

	��ʽ��xx + yy = a*b (abΪrx,ry)
*/
/************************************************************************/
void SSvgPainter::DrawEllipse(SBaseDC *pDC, SSvgObject *pObj, int x, int y)
{
	int x1,y1,x2,y2;
	x1 = GetCoordX(pObj->GetAttributeF("cx")-pObj->GetAttributeF("rx"));
	y1 = GetCoordY(pObj->GetAttributeF("cy")-pObj->GetAttributeF("ry"));
	x2 = GetCoordX(pObj->GetAttributeF("cx")+pObj->GetAttributeF("rx"));
	y2 = GetCoordY(pObj->GetAttributeF("cy")+pObj->GetAttributeF("ry"));
	x1+=x;
	y1+=y;
	x2+=x;
	y2+=y;
	SString sAlpha = pObj->GetAttribute("alpha");
	int alpha = 255;
	if(sAlpha.length() > 0)
		alpha = sAlpha.toInt();

#ifdef QT_GUI_LIB
	bool bFill=false;
	bool bGradient=false;//�Ƿ�Ϊ����ɫ���
	SString sGradientId;
	SCOLOR sBackColor,sLineColor;
	if(m_pSvgDataModel && m_pSvgDataModel->GetViewBackColor(pObj,bFill,sBackColor))
	{
	}
	else
	{
		SString sFill = GetAttributeByLevel(pObj,"fill");//pObj->GetAttribute("fill");
		if(sFill != "" && sFill.CompareNoCase("none") != 0)
		{
			//�������
			bFill = true;
			sFill = pObj->GetAttribute("fill");
			//�ж��Ƿ��ǽ���
			if(sFill.length() > 6 && sFill.left(5) == "url(#" && sFill.right(1) == ")")
			{
				bGradient = true;
				sGradientId = sFill.mid(5,sFill.length()-6);
			}
			else
				sBackColor = GetColorByString(sFill);
		}
	}
	if(!(m_pSvgDataModel && m_pSvgDataModel->GetViewLineColor(pObj,sLineColor)))
		sLineColor = GetColorByString(GetAttributeByLevel(pObj,"stroke"));
	if(!bFill && pObj->GetAttributeI("stroke-width") == 0)
		return;
	float line_width = pObj->GetAttributeI("stroke-width")*m_fScale;
	if(line_width < 1)
		line_width = 1;

	QBrush bLine(QColor(sLineColor.r,sLineColor.g,sLineColor.b,alpha));
	int penstyle = pObj->GetAttributeI("stroke-dasharray")+1;
	QPen pen(bLine,line_width,(Qt::PenStyle)((sLineColor.a==0)? (Qt::NoPen):penstyle));
	QPainter *painter = ((QPainter*)pDC->GetHandle());
	SRect rect;
	rect.left = x1;
	rect.top = y1;
	rect.right = x2;
	rect.bottom = y2;
	QRect qrect(x1,y1,x2-x1,y2-y1);
	if(bFill)
	{
		if(bGradient)
		{
			//����
			SSvgDocument::stuLinearGradient *pLinear = m_Document.SearchLinearGradientById(sGradientId);
			SSvgDocument::stuRadialGradient *pRadial;
			if(pLinear != NULL)
			{

				QLinearGradient linear(QPointF(rect.left+pLinear->x1*rect.width()/100, rect.top+pLinear->y1*rect.height()/100), 
					QPointF(rect.left+pLinear->x2*rect.width()/100, rect.top+pLinear->y2*rect.height()/100));
				unsigned long pos;
				SSvgDocument::stuGradientStop *pStop = pLinear->Stops.FetchFirst(pos);
				while(pStop)
				{
					float fpos = ((float)pStop->offset)/100.0f;
					if(fpos < 0)
						fpos = 0;
					if(fpos > 1)
						fpos = 1;
					linear.setColorAt(fpos, QColor(pStop->color.r,pStop->color.g,pStop->color.b,alpha));
					pStop = pLinear->Stops.FetchNext(pos);
				}
				painter->setBrush(linear);
			}
			else if((pRadial=m_Document.SearchRadialGradientById(sGradientId)) != NULL)
			{
				QRadialGradient radial(rect.left+pRadial->cx*rect.width()/100,rect.top+pRadial->cy*rect.height()/100,
					pRadial->r*((rect.width()+rect.height())/2)/100,
					rect.left+pRadial->fx*rect.width()/100,rect.top+pRadial->fy*rect.height()/100);
				unsigned long pos;
				SSvgDocument::stuGradientStop *pStop = pRadial->Stops.FetchFirst(pos);
				while(pStop)
				{
					float fpos = ((float)pStop->offset)/100.0f;
					if(fpos < 0)
						fpos = 0;
					if(fpos > 1)
						fpos = 1;
					radial.setColorAt(fpos, QColor(pStop->color.r,pStop->color.g,pStop->color.b,alpha));
					pStop = pRadial->Stops.FetchNext(pos);
				}
				painter->setBrush(radial);
			}
		}
		else
		{
			//��ɫ
			QBrush bBack(QColor(sBackColor.r,sBackColor.g,sBackColor.b,alpha));
			painter->setBrush(bBack);
		}
	}
	else
		painter->setBrush(Qt::NoBrush);
	painter->setPen(GetAttributeByLevel(pObj,"stroke") == "none"?Qt::NoPen:pen);
	painter->drawEllipse(qrect);
#else
	SCOLOR sColor;
	if(!(m_pSvgDataModel && m_pSvgDataModel->GetViewLineColor(pObj,sColor)))
		sColor = GetColorByString(GetAttributeByLevel(pObj,"stroke"));
	int line_width = GetCoord(pObj->GetAttributeI("stroke-width"));
	if(line_width <= 0)
		line_width = 1;
	SPen p(pObj->GetAttributeI("stroke-dasharray"),line_width/*pObj->GetAttributeI("stroke-width")*/,sColor);//pObj->GetAttribute("stroke")
	SPen* pOld = pDC->SelectObject(&p);
	//�������
	bool bFill;
	if(m_pSvgDataModel && m_pSvgDataModel->GetViewBackColor(pObj,bFill,sColor))
	{
		if(bFill)
		{
			SBrush b(sColor);
			SBrush *pOldBrush=pDC->SelectObject(&b);
			pDC->Ellipse(x1,y1,x2,y2);
			pDC->SelectObject(pOldBrush);
		}
		else
		{
			SBrush b;
			b.CreateStockObject(NULL_BRUSH);//�����ˢ
			SBrush *pOldBrush=pDC->SelectObject(&b);
			pDC->Ellipse(x1,y1,x2,y2);
			pDC->SelectObject(pOldBrush);
		}
	}
	else
	{
		SString fill=GetAttributeByLevel(pObj,"fill");//pObj->GetAttribute("fill");
		if(fill != "" && fill!="none")
		{
			SBrush b(GetColorByString(fill));
			SBrush *pOldBrush=pDC->SelectObject(&b);
			pDC->Ellipse(x1,y1,x2,y2);
			pDC->SelectObject(pOldBrush);
		}
		else
		{
			SBrush b;
			b.CreateStockObject(NULL_BRUSH);//�����ˢ
			SBrush *pOldBrush=pDC->SelectObject(&b);
			pDC->Ellipse(x1,y1,x2,y2);
			pDC->SelectObject(pOldBrush);
		}
	}
	pDC->SelectObject(pOld);
#endif
}


/************************************************************************/
/* circle
	z
	cx			X�������ĵ�
	cy			Y�������ĵ�
	r			�뾶
	stroke		�߿���ɫ
	stroke-width �߿���
	stroke-dasharray ������ʽ(��ѡ)
	fill		�����ɫ��none��ʾ�����
*/
/************************************************************************/
void SSvgPainter::DrawCircle(SBaseDC *pDC, SSvgObject *pObj, int x, int y)
{
	int x1,y1,x2,y2;
	x1 = GetCoordX(pObj->GetAttributeF("cx")-pObj->GetAttributeF("r"));
	y1 = GetCoordY(pObj->GetAttributeF("cy")-pObj->GetAttributeF("r"));
	x2 = GetCoordX(pObj->GetAttributeF("cx")+pObj->GetAttributeF("r"));
	y2 = GetCoordY(pObj->GetAttributeF("cy")+pObj->GetAttributeF("r"));
	SCOLOR sColor;
	if(!(m_pSvgDataModel && m_pSvgDataModel->GetViewLineColor(pObj,sColor)))
		sColor = GetColorByString(GetAttributeByLevel(pObj,"stroke"));
	int line_width = GetCoord(pObj->GetAttributeI("stroke-width"));
	if(line_width <= 0)
		line_width = 1;
	SPen p(pObj->GetAttributeI("stroke-dasharray"),line_width/*pObj->GetAttributeI("stroke-width")*/,sColor);//pObj->GetAttribute("stroke")
	SPen* pOld = pDC->SelectObject(&p);
	//�������
	x1+=x;
	y1+=y;
	x2+=x;
	y2+=y;
	bool bFill;
	if(m_pSvgDataModel && m_pSvgDataModel->GetViewBackColor(pObj,bFill,sColor))
	{
		if(bFill)
		{
			SBrush b(sColor);
			SBrush *pOldBrush=pDC->SelectObject(&b);
			pDC->Ellipse(x1,y1,x2,y2);
			pDC->SelectObject(pOldBrush);
		}
		else
		{
			SBrush b;
			b.CreateStockObject(NULL_BRUSH);//�����ˢ
			SBrush *pOldBrush=pDC->SelectObject(&b);
			pDC->Ellipse(x1,y1,x2,y2);
			pDC->SelectObject(pOldBrush);
		}
	}
	else
	{
		SString fill=GetAttributeByLevel(pObj,"fill");//pObj->GetAttribute("fill");
		if(fill != "" && fill!="none")
		{
			SBrush b(GetColorByString(fill));
			SBrush *pOldBrush=pDC->SelectObject(&b);
			pDC->Ellipse(x1,y1,x2,y2);
			pDC->SelectObject(pOldBrush);
		}
		else
		{
			SBrush b;
			b.CreateStockObject(NULL_BRUSH);//�����ˢ
			SBrush *pOldBrush=pDC->SelectObject(&b);
			pDC->Ellipse(x1,y1,x2,y2);
			pDC->SelectObject(pOldBrush);
		}
	}
	pDC->SelectObject(pOld);
}


/************************************************************************/
/* line
	x1
	y1
	x2
	y2
	z
	stroke-width	�ߴ�
	stroke			������ɫ
	stroke-dasharray ������ʽ(��ѡ)
*/
/************************************************************************/
void SSvgPainter::DrawLine(SBaseDC *pDC, SSvgObject *pObj, int x, int y)
{
	float x1=GetCoordX(pObj->GetAttributeF("x1"));
	float y1=GetCoordY(pObj->GetAttributeF("y1"));
	float x2=GetCoordX(pObj->GetAttributeF("x2"));
	float y2=GetCoordY(pObj->GetAttributeF("y2"));
	SCOLOR sColor;
	if(!(m_pSvgDataModel && m_pSvgDataModel->GetViewLineColor(pObj,sColor)))
		sColor = GetColorByString(GetAttributeByLevel(pObj,"stroke"));
	int line_width = GetCoord(pObj->GetAttributeI("stroke-width"));
	if(line_width <= 0)
		line_width = 1;
	SPen p(pObj->GetAttributeI("stroke-dasharray"),line_width/*pObj->GetAttributeI("stroke-width")*/,sColor);//pObj->GetAttribute("stroke")
	SPen* pOld = pDC->SelectObject(&p);
	pDC->MoveTo(x1+x,y1+y);
	pDC->LineTo(x2+x,y2+y);
	pDC->SelectObject(pOld);

}

/************************************************************************/
/* polyline
	points			����ε�켣"270,208 270,193"
	stroke			��ɫ
	stroke-width	�߿�
*/
/************************************************************************/
void SSvgPainter::DrawPolyLine(SBaseDC *pDC, SSvgObject *pObj, int x, int y)
{
	SString points=pObj->GetAttribute("points");
	SCOLOR sColor;
	if(!(m_pSvgDataModel && m_pSvgDataModel->GetViewLineColor(pObj,sColor)))
		sColor = GetColorByString(GetAttributeByLevel(pObj,"stroke"));
	int line_width = GetCoord(pObj->GetAttributeI("stroke-width"));
	if(line_width <= 0)
		line_width = 1;
	SPen p(pObj->GetAttributeI("stroke-dasharray"),line_width/*pObj->GetAttributeI("stroke-width")*/,sColor);//pObj->GetAttribute("stroke")
	SPen* pOld = pDC->SelectObject(&p);
	int p1=0,p2,p3;
	int x0,y0;
	int index=0;
	SArray<YSPOINT> polygon;
	YSPOINT pt;	
	while(1)
	{
		p2=points.Find(",",p1);
		if(p2 < 0)
			break;
		x0 = GetCoordX(SString::toInt(points.Mid(p1,p2-p1)));
		p2 ++;
		p3=points.Find(" ",p2);
		if(p3 < 0)
			p3 = points.GetLength();
		y0 = GetCoordY(SString::toInt(points.Mid(p2,p3-p2)));
		p1 = p3+1;
		if(index==0)
			pDC->MoveTo(x0+x, y0+y);
		else
			pDC->LineTo(x0+x, y0+y);
		pt.x = x0+x;
		pt.y = y0+y;
		polygon.append(pt);

		index++;
	}
	//�Ƿ����
	bool bFill;
	if(m_pSvgDataModel && m_pSvgDataModel->GetViewBackColor(pObj,bFill,sColor))
	{
		if(bFill)
		{
			if(polygon.count()>=3)
			{
				//�������
				SBrush b(sColor);
				SBrush *pOldB=pDC->SelectObject(&b);
				memcpy(&pt, polygon.getAt(0),sizeof(pt));
				pDC->Polygon(polygon.getData(),polygon.count());
				pDC->SelectObject(pOldB);
			}
		}
	}
	else
	{
		SString sFill = GetAttributeByLevel(pObj,"fill");//pObj->GetAttribute("fill");
		if(sFill != "" && sFill.CompareNoCase("none") != 0)
		{
			if(polygon.count()>=3)
			{
				//�������
				SBrush b(GetColorByString(pObj->GetAttribute("fill")));
				SBrush *pOldB=pDC->SelectObject(&b);
				memcpy(&pt, polygon.getAt(0),sizeof(pt));
				pDC->Polygon(polygon.getData(),polygon.count());
				pDC->SelectObject(pOldB);
			}
		}
	}
	pDC->SelectObject(pOld);
}
/************************************************************************/
/* 
mode="Arc"
d="M79,443.5 A11,11 0 1 0 90,432.5"
*/
/************************************************************************/
void SSvgPainter::DrawPath(SBaseDC *pDC, SSvgObject *pObj, int x, int y)
{
	S_UNUSED(pDC);
	S_UNUSED(pObj);
	S_UNUSED(x);
	S_UNUSED(y);
	/*
	//���ڸ������������Ӿ���̫���ӣ�������ʱ������˼�ķ�ʽ��������Ӿ�������x/y/width/height
	int x1,y1,x2,y2;
	x1 = GetCoordX(pObj->GetAttributeF("x"))+x;
	y1 = GetCoordY(pObj->GetAttributeF("y"))+y;
	x2 = GetCoordX(pObj->GetAttributeF("x")+pObj->GetAttributeF("width"))+x;
	y2 = GetCoordY(pObj->GetAttributeF("y")+pObj->GetAttributeF("height"))+y;
	SString points=pObj->GetAttribute("d");
	SPen p(pObj->GetAttributeI("stroke-dasharray"),pObj->GetAttributeI("stroke-width"),
		GetColorByString(GetAttributeByLevel(pObj,"stroke")));//pObj->GetAttribute("stroke")
	SPen* pOld = pDC->SelectObject(&p);
	int p1=0,p2;
	float x0,y0,x00,y00;
	char oper,ch;
	SString str;
	CArray<POINT,POINT> polygon;
	POINT pt;	
	while(p1<points.GetLength())
	{
		oper = points.GetAt(p1++);//�������ͣ�Ŀǰ֧��M:MoveTo, L:LineTo, A:Arc
		switch(oper)
		{
		case 'M':
		case 'm':
		case 'L':
		case 'l':
			{
nextLine:
				p2 = points.Find(",",p1);
				if(p2 < 0)
					break;
				str = points.Mid(p1,p2-p1);
				x0 = SString::toFloat(str);
				p1 = p2+1;
				p2 = points.Find(' ',p1);
				if(p2 < 0)
					p2 = points.GetLength();
				str = points.Mid(p1,p2-p1);
				y0 = SString::toFloat(str);
				p1 = p2+1;
				if(oper == 'M' || oper == 'm')
				{
					//if(y0<0)
					//	y0 = -y0;
					pDC->MoveTo(GetCoordX(x0)+x, GetCoordY(y0)+y);
					x00 = x0;
					y00 = y0;
					
					pt.x = GetCoordX(x0)+x;
					pt.y = GetCoordY(y0)+y;
					polygon.Add(pt);
				}
				else
				{
					pDC->LineTo(GetCoordX(x00+x0)+x, GetCoordY(y00+y0)+y);
					x00 += x0;
					y00 += y0;
					pt.x = GetCoordX(x00+x0)+x;
					pt.y = GetCoordY(y00+y0)+y;
					polygon.Add(pt);
				}
				if(p1 < points.GetLength())
				{
					ch = points.GetAt(p1);
					if(ch >='0' && ch <= '9' || ch == '-')
						goto nextLine;//�ظ�
				}
			}
			break;
		case 'A':
		case 'a':
			{
				int rx,ry;
				float endx,endy;
				int xrotation;
				int large_arc_flag;
				int sweep_flag;
nextArc:
				//A11,11 0 1 0 90,432.5
				//rx
				p2 = points.Find(",",p1);
				if(p2 < 0)
					break;
				str = points.Mid(p1,p2-p1);
				rx = SString::toInt(str);
				p1 = p2+1;

				//ry
				p2 = points.Find(' ',p1);
				if(p2 < 0)
					break;
				str = points.Mid(p1,p2-p1);
				ry = SString::toInt(str);
				p1 = p2+1;
				
				//xrotation
				p2 = points.Find(' ',p1);
				if(p2 < 0)
					break;
				str = points.Mid(p1,p2-p1);
				xrotation = SString::toInt(str);
				p1 = p2+1;

				//large-arc-flag
				p2 = points.Find(' ',p1);
				if(p2 < 0)
					break;
				str = points.Mid(p1,p2-p1);
				large_arc_flag = SString::toInt(str);
				p1 = p2+1;

				//sweep-flag
				p2 = points.Find(' ',p1);
				if(p2 < 0)
					break;
				str = points.Mid(p1,p2-p1);
				sweep_flag = SString::toInt(str);
				p1 = p2+1;

				//x
				p2 = points.Find(',',p1);
				if(p2 < 0)
					break;
				str = points.Mid(p1,p2-p1);
				endx = SString::toFloat(str);
				p1 = p2+1;

				//y
				p2 = points.Find(' ',p1);
				if(p2 < 0)
					p2 = points.GetLength();
				str = points.Mid(p1,p2-p1);
				endy = SString::toFloat(str);
				p1 = p2+1;
				
				//DarwArc
				pDC->Arc(x1,y1,x2,y2,
								 GetCoordX(x0)+x,GetCoordY(y0)+y,GetCoordX(endx)+x,GetCoordY(endy)+y);
				if(p1 < points.GetLength())
				{
					ch = points.GetAt(p1);
					if(ch >='0' && ch <= '9' || ch == '-')
						goto nextArc;//�ظ�
				}
			}
			break;
		}
	}

	//�Ƿ����
	SString sFill = GetAttributeByLevel(pObj,"fill");//pObj->GetAttribute("fill");
	if(sFill != "" && sFill.CompareNoCase("none") != 0)
	{
		if(polygon.GetSize()>=3)
		{
			//�������
			SBrush b(GetColorByString(pObj->GetAttribute("fill")));
			SBrush *pOldB=pDC->SelectObject(&b);
			memcpy(&pt, &polygon.GetAt(0),sizeof(pt));
			pDC->Polygon(polygon.GetData(),polygon.GetSize());
			pDC->SelectObject(pOldB);
		}
	}
	pDC->SelectObject(pOld);
	*/
}

void SSvgPainter::DrawPic(SBaseDC *pDC, SSvgObject *pObj, int x, int y)
{
	SRect rect;
	rect.left = GetCoordX(pObj->GetAttributeF("x"));
	rect.top = GetCoordY(pObj->GetAttributeF("y"));
	rect.right = GetCoordX(pObj->GetAttributeF("x")+pObj->GetAttributeF("width"));
	rect.bottom = GetCoordY(pObj->GetAttributeF("y")+pObj->GetAttributeF("height"));
	rect.OffsetRect(x,y);
	SSvgDocument::stuImageDefine *pImgDef = (SSvgDocument::stuImageDefine*)pObj->GetAttribute("img_def_obj").toULong();
	SPicture *pPic = pImgDef == NULL?NULL:pImgDef->m_pPic;
// 	if(pPic == NULL)
// 	{
// 		pPic = new SPicture();
// 		if(pObj->GetAttribute("href").length() > 0)
// 		{
// 			//����ͼƬ
// 			SString href = pObj->GetAttribute("href");
// 			SString sPicFile;
// 			if(href.length() > 2 && (href.at(0) == '/' || href.at(1) == ':'))
// 				sPicFile = href;
// 			else
// 				sPicFile = this->m_Document.GetSvgPath() + href;
// 			if(!pPic->Load(sPicFile.data()))
// 			{
// 				LOGWARN("����ͼƬ[%s]ʧ��!",sPicFile.data());
// 				delete pPic;
// 				return;
// 			}
// 		}
// 		else
// 		{
// 			SString sHexPic = pObj->GetAttribute("pichex");
// 			int len = sHexPic.length()/2;
// 			if(len > 20)
// 			{
// 				BYTE *pHex = new BYTE[len];
// 				SString::StrToHex(sHexPic.data(),sHexPic.length(),pHex);
// 	#ifdef QT_GUI_LIB
// 				if(!pPic->m_pixmap.loadFromData(pHex,len))
// 				{
// 					LOGWARN("������ǶͼƬʧ��!");
// 					delete[] pHex;
// 					delete pPic;
// 					return;
// 				}
// 	#endif
// 				delete[] pHex;
// 			}
// 		}
// 		pObj->SetAttributeValue("picobj",SString::toString((int)pPic));
// 		pObj->SetAttributeValue("rawh",SString::toString(pPic->m_pixmap.height()));
// 		pObj->SetAttributeValue("raww",SString::toString(pPic->m_pixmap.width()));
// 	}
// #ifdef QT_GUI_LIB
// 	int rotate = pObj->GetAttributeI("rotate")%360;
// 	QPointF center((rect.right+rect.left)/2,(rect.bottom+rect.top)/2);
// 	QPainter *painter = ((QPainter*)pDC->GetHandle());
// 	if(rotate > 0)
// 	{
// 		painter->translate(center);
// 		painter->rotate(rotate);
// 		painter->translate(-center);
// 	}
// #endif
	if(pPic != NULL)
	{
		SString sAlpha = pObj->GetAttribute("alpha");
		int alpha = 255;
		if(sAlpha.length() > 0)
			alpha = sAlpha.toInt();

		pPic->Render(pDC->GetHandle(),rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,alpha);
	}
// #ifdef QT_GUI_LIB
// 	if(rotate > 0)
// 	{
// 		painter->translate(center);
// 		painter->rotate(-rotate);
// 		painter->translate(-center);
// 	}
// #endif
}

//����һ�������Ƿ����ʾ
bool SSvgPainter::IsInViewRect(SSvgObject *pObj)
{
	SSvgObject::SVG_RECT rect;
	memcpy(&rect,pObj->GetRect(),sizeof(rect));
	if(rect.left==0 && rect.right == 0 && rect.top==0 && rect.bottom==0)
		return true;
	if((rect.left < m_ViewRect.left && rect.right < m_ViewRect.left) ||
		 (rect.left > m_ViewRect.right && rect.right > m_ViewRect.right) ||
		 (rect.top < m_ViewRect.top && rect.bottom < m_ViewRect.top) ||
		 (rect.top > m_ViewRect.bottom && rect.bottom > m_ViewRect.bottom))
		return false;
	if(m_bDrawOutPixmapRect)
	{
		//�������ڴ�ͼƬ���Ԫ��
		if(rect.left > m_PixmapRect.left && rect.right < m_PixmapRect.right &&
			rect.top > m_PixmapRect.top && rect.bottom < m_PixmapRect.bottom)
			return false;
		else
			return true;
	}
	return true;
}
//���Ե�ǰ���Ƿ񾭹���ǰ�������µ��ֵܽڵ㣬x,yΪSVG����ϵ
SSvgObject* SSvgPainter::IsPointInObjectRect(SSvgObject *pObj, float x,float y)
{
	while(pObj != NULL)
	{
		SSvgObject::SVG_RECT *pRect = pObj->GetRect();
		if(x>=pRect->left-3 && x <=pRect->right+3 && 
			 y>=pRect->top-3 && y <= pRect->bottom+3)
		{
			return pObj;
		}
		pObj = pObj->GetNext();
	}

	return NULL;
}

//���Ե�ǰ���Ƿ񾭹���ǰ�������µ��ֵܽڵ㣬x,yΪSVG����ϵ
SSvgObject* SSvgPainter::IsObjectInPoint(SSvgObject *pObj, float x,float y,bool bHasUri)
{
	while(pObj != NULL)
	{
		SSvgObject::SVG_RECT *pRect = pObj->GetRect();
		if(x>=pRect->left-3 && x <=pRect->right+3 && 
			 y>=pRect->top-3 && y <= pRect->bottom+3)
		{
			//ȷ�����䷶Χ�ڣ��ٸ��ݽڵ����ͷֲ�ͬ�������Ƿ�ץȡ��
			switch(pObj->GetType())
			{
				case SSvgObject::TYPE_GROUP:
					{
						//���ѡ����������Ƿ�ѡ�У�����һ��ѡ�����鱻ѡ��
						SSvgObject *pChild=pObj->GetChild();
						while(pChild)
						{
							if(IsObjectInPoint(pChild,x,y,bHasUri) != NULL)
							{
								//�ȿ������滹��û������ģ��������Ⱥ����
								SSvgObject *pNext = IsObjectInPoint(pObj->GetNext(),x,y,bHasUri);
								if(pNext != NULL)
									return pNext;
								if(!bHasUri || pObj->GetAttribute("uri").length() > 0)
									return pObj;
							}
							pChild = pChild->GetNext();
						}
					}
					break;
				case SSvgObject::TYPE_RECT:
				case SSvgObject::TYPE_USE:
				case SSvgObject::TYPE_RECTTEXT:
					{
						//��ǰ�����Ƿ��������ɫ��������Ը����������ץȡ������Ҫ���ݱ���ץȡ
						SString sFill = pObj->GetAttribute("fill");
						if(sFill != "" && sFill.CompareNoCase("none") != 0)
						{
							//�������
							//�ȿ������滹��û������ģ��������Ⱥ����
							SSvgObject *pNext = IsObjectInPoint(pObj->GetNext(),x,y,bHasUri);
							if(pNext != NULL)
								return pNext;
							if(!bHasUri || pObj->GetAttribute("uri").length() > 0)
								return pObj;
						}
						//���������
						if(IsNear(x,pRect->left) ||
							 IsNear(x,pRect->right) ||
							 IsNear(y,pRect->top) ||
							 IsNear(y,pRect->bottom))
						{
							//�ȿ������滹��û������ģ��������Ⱥ����
							SSvgObject *pNext = IsObjectInPoint(pObj->GetNext(),x,y,bHasUri);
							if(pNext != NULL)
								return pNext;
							if(!bHasUri || pObj->GetAttribute("uri").length() > 0)
								return pObj;
						}
					}
					break;
				case SSvgObject::TYPE_PIC:
					{
						//�ȿ������滹��û������ģ��������Ⱥ����
						SSvgObject *pNext = IsObjectInPoint(pObj->GetNext(),x,y,bHasUri);
						if(pNext != NULL)
							return pNext;
						if(!bHasUri || pObj->GetAttribute("uri").length() > 0)
							return pObj;
					}
					break;
				case SSvgObject::TYPE_TEXT:
					{
						//�ȿ������滹��û������ģ��������Ⱥ����
						SSvgObject *pNext = IsObjectInPoint(pObj->GetNext(),x,y,bHasUri);
						if(pNext != NULL)
							return pNext;
						if(!bHasUri || pObj->GetAttribute("uri").length() > 0)
							return pObj;
					}
				case SSvgObject::TYPE_ELLIPSE:
					{
						SString sFill = pObj->GetAttribute("fill");
						if(sFill != "" && sFill.CompareNoCase("none") != 0)
						{
							//�������
							//�ȿ������滹��û������ģ��������Ⱥ����
							SSvgObject *pNext = IsObjectInPoint(pObj->GetNext(),x,y,bHasUri);
							if(pNext != NULL)
								return pNext;
							if(!bHasUri || pObj->GetAttribute("uri").length() > 0)
								return pObj;
						}
						//��Բ����ѡȡ
						float cx=pObj->GetAttributeF("cx");
						float cy=pObj->GetAttributeF("cy");
						float rx=pObj->GetAttributeF("rx");
						float ry=pObj->GetAttributeF("ry");
						//cx -= x;
						//cy -= y;
						cx = x - cx;
						cy = y - cy;
						//if(IsNear(sqrt(cx*cx+cy*cy) , sqrt(rx*ry)))
						if(fabs(cx*cx/(rx*rx)+cy*cy/(ry*ry) - 1) < 0.1)
						{
							//�ȿ������滹��û������ģ��������Ⱥ����
							SSvgObject *pNext = IsObjectInPoint(pObj->GetNext(),x,y,bHasUri);
							if(pNext != NULL)
								return pNext;
							if(!bHasUri || pObj->GetAttribute("uri").length() > 0)
								return pObj;
						}
					}
					break;
				case SSvgObject::TYPE_CIRCLE:
					{
						SString sFill = pObj->GetAttribute("fill");
						if(sFill != "" && sFill.CompareNoCase("none") != 0)
						{
							//�������
							//�ȿ������滹��û������ģ��������Ⱥ����
							SSvgObject *pNext = IsObjectInPoint(pObj->GetNext(),x,y,bHasUri);
							if(pNext != NULL)
								return pNext;
							if(!bHasUri || pObj->GetAttribute("uri").length() > 0)
								return pObj;
						}
						//��Բ����ѡȡ
						float cx=pObj->GetAttributeF("cx");
						float cy=pObj->GetAttributeF("cy");
						float rx=pObj->GetAttributeF("r");
						float ry=pObj->GetAttributeF("r");
						//cx -= x;
						//cy -= y;
						cx = x - cx;
						cy = y - cy;
						//if(IsNear(sqrt(cx*cx+cy*cy) , sqrt(rx*ry)))
						if(fabs(cx*cx/(rx*rx)+cy*cy/(ry*ry) - 1) < 0.1)
						{
							//�ȿ������滹��û������ģ��������Ⱥ����
							SSvgObject *pNext = IsObjectInPoint(pObj->GetNext(),x,y,bHasUri);
							if(pNext != NULL)
								return pNext;
							if(!bHasUri || pObj->GetAttribute("uri").length() > 0)
								return pObj;
						}
					}
					break;
				case SSvgObject::TYPE_PATH:
					{
						//�ȿ������滹��û������ģ��������Ⱥ����
						SSvgObject *pNext = IsObjectInPoint(pObj->GetNext(),x,y,bHasUri);
						if(pNext != NULL)
							return pNext;
						if(!bHasUri || pObj->GetAttribute("uri").length() > 0)
							return pObj;
					
						SString sFill = pObj->GetAttribute("fill");
						if(sFill != "" && sFill.CompareNoCase("none") != 0)
						{
							//�������
							//�ȿ������滹��û������ģ��������Ⱥ����
							SSvgObject *pNext = IsObjectInPoint(pObj->GetNext(),x,y,bHasUri);
							if(pNext != NULL)
								return pNext;
							if(!bHasUri || pObj->GetAttribute("uri").length() > 0)
								return pObj;
						}
						//��Բ����ѡȡ
						float cx=pObj->GetAttributeF("x")+pObj->GetAttributeF("width")/2;
						float cy=pObj->GetAttributeF("y")+pObj->GetAttributeF("height")/2;
						float rx=pObj->GetAttributeF("width")/2;
						float ry=pObj->GetAttributeF("height")/2;
						//cx -= x;
						//cy -= y;
						cx = x - cx;
						cy = y - cy;
						//if(IsNear(sqrt(cx*cx+cy*cy) , sqrt(rx*ry)))
						if(fabs(cx*cx/(rx*rx)+cy*cy/(ry*ry) - 1) < 0.1)
						{
							//�ȿ������滹��û������ģ��������Ⱥ����
							SSvgObject *pNext = IsObjectInPoint(pObj->GetNext(),x,y,bHasUri);
							if(pNext != NULL)
								return pNext;
							if(!bHasUri || pObj->GetAttribute("uri").length() > 0)
								return pObj;
						}
					}
					break;
				case SSvgObject::TYPE_LINE:
					{
						float x1=pObj->GetAttributeF("x1");
						float y1=pObj->GetAttributeF("y1");
						float x2=pObj->GetAttributeF("x2");
						float y2=pObj->GetAttributeF("y2");
						if(IsInLine(x1,y1,x2,y2,x,y))
						{
							//�ȿ������滹��û������ģ��������Ⱥ����
							SSvgObject *pNext = IsObjectInPoint(pObj->GetNext(),x,y,bHasUri);
							if(pNext != NULL)
								return pNext;
							if(!bHasUri || pObj->GetAttribute("uri").length() > 0)
								return pObj;
						}
					}
					break;
				case SSvgObject::TYPE_POLYLINE:
					{
						SString points=pObj->GetAttribute("points");
						int p1=0,p2,p3;
						float x0=0,y0=0,x1,y1;
						int index=0;
						while(1)
						{
							p2=points.Find(",",p1);
							if(p2 < 0)
								break;
							x1 = SString::toFloat(points.Mid(p1,p2-p1));
							p2 ++;
							p3=points.Find(" ",p2);
							if(p3 < 0)
								p3 = points.GetLength();
							y1 = SString::toFloat(points.Mid(p2,p3-p2));
							p1 = p3+1;
							if(index>0)
							{
								if(IsInLine(x0,y0,x1,y1,x,y))
								{
									//�ȿ������滹��û������ģ��������Ⱥ����
									SSvgObject *pNext = IsObjectInPoint(pObj->GetNext(),x,y,bHasUri);
									if(pNext != NULL)
										return pNext;
									if(!bHasUri || pObj->GetAttribute("uri").length() > 0)
										return pObj;
								}
							}
							x0=x1;
							y0=y1;
							index++;
						}
					}
					break;
				default:
					break;
			}

		}
		pObj = pObj->GetNext();
	}

	return NULL;
}

//���Ե�ǰ�����Ƿ����������ǰ�������µ��ֵܽڵ㣬x,yΪSVG����ϵ
void SSvgPainter::SelectObjectInRect(SSvgObject *pObj, SSvgObject::SVG_RECT &selRect)
{
	while(pObj != NULL)
	{
		SSvgObject::SVG_RECT *pRect = pObj->GetRect();
		if(pRect->left >= selRect.left && pRect->left <= selRect.right  &&
			 pRect->right >= selRect.left && pRect->right <= selRect.right &&
			 pRect->top  >= selRect.top  && pRect->top <=selRect.bottom   &&
			 pRect->bottom  >= selRect.top  && pRect->bottom<=selRect.bottom)
		{
			//�����У����뵽ѡ���б�
			if(!m_SelectedObjList.exist(pObj))
				m_SelectedObjList.append(pObj);
		}
		pObj = pObj->GetNext();
	}
}

//�����жϵ��Ƿ񿿽�
bool SSvgPainter::IsNear(float v1, float v2)
{
	if(fabs(v1 - v2) < 3)
		return true;
	return false;
}

//�����ж�һ�����Ƿ���ָ�����߶���
bool SSvgPainter::IsInLine(float x1, float y1, float x2, float y2, float px, float py)
{
	//����p����������ľ���֮���Ƿ�������������
	//A=y1-y2
	//B=x2-x1
	//C=x1y2-x2y1
	//d = abs(Ax0+By0+C)/sqrt(A*A+B*B)
	float A = y1-y2;
	float B = x2-x1;
	float C = x1*y2-x2*y1;
	float sq = sqrt(A*A+B*B);
	if(sq < 0.0001)
	{
		//sq = 0.0001;
		//���ڽӽ����߶Σ�����һ���㴦��
		return IsPointNearPoint(x1,y1,px,py);
	}
	float d = fabs(A*px+B*py+C)/sq;
	if(d*m_fScale < 8)//8�������ݲ�
		return true;
// 	float s1=sqrt((x1-px)*(x1-px)+(y1-py)*(y1-py));
// 	float s2=sqrt((x2-px)*(x2-px)+(y2-py)*(y2-py));
// 	float s=sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
// 	s1 = (s1+s2)-s;
// 	//if(fabs(s1)<0.1 || fabs(s1)/s <0.05)//0.05
// 	if(fabs(s1)<5 || fabs(s1)/s <0.005)//0.05
// 		return true;
	return false;
}

//����һ����Բ��һ�����㣬��λ�����Ӧ����Բ�ϵĵ�
void SSvgPainter::GetEllipseByPoint(float x,float y, 
						float width,float height,float px, float py,float &ex,float &ey)
{
	float cx=x + width/2;
	float cy=y + height/2;
	float rx=width/2;
	float ry=height/2;
	
	if(px < x)
		ex = x;
	else if(px > x+width)
		ex = x + width;
	else
		ex = px;
	ey = sqrt((1-(ex-cx)*(ex-cx)/(rx*rx))*(ry*ry));
	if(py - cy < 0)
	{
		ey = -ey;
	}
	ey += cy;
}

/************************************************************************/
/* 
	���ݼ���ȡ�������ԣ�����ϲ���������ʹ���ϲ�����ԣ�����ʹ���²������
*/
/************************************************************************/
SString SSvgPainter::GetAttributeByLevel(SSvgObject *pObj, SString sAttr)
{
	SPtrList<SSvgObject> objList;
	
	SString str;
	SSvgObject *pParent = pObj;
	while(pParent != m_Document.GetRootObject())
	{
		if(pParent == NULL)
		{
			pParent = pObj;
			break;
		}
		objList.append(pParent);
		pParent = pParent->GetParent();
	}
	int i,cnt=objList.count();
	for(i=cnt-1;i>=0;i--)
	{
		pParent = objList.at(i);
		str = pParent->GetAttribute(sAttr);
		if(str.GetLength()>0)
			return str;
	}
	return pObj->GetAttribute(sAttr);
}

void SSvgPainter::SetScale(float scale)
{
	m_fScale = scale;
	Render();
}
