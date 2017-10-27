/**
 *
 * �� �� �� : SSvgObject.h
 * �������� : 2007-04-09 08:55
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : SVG�������,��װSVG�Ļ���Ԫ��
 * �޸ļ�¼ : 
 *            $Log: $
 *
 **/

#if !defined(AFX_SKTSVGOBJECT_H__486DC941_8FDD_4B7B_8C09_947ECDB2F642__INCLUDED_)
#define AFX_SKTSVGOBJECT_H__486DC941_8FDD_4B7B_8C09_947ECDB2F642__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef QT_GUI_LIB
#include <QtGui>
#endif

#include "../../SString.h"
#include "../../SFile.h"
#include "../../SXmlConfig.h"
#include "../SBaseDC.h"
#include "../SBaseWnd.h"

class SBASE_QTGUI_EXPORT SSvgAttribute
{
public:
	struct SBASE_QTGUI_EXPORT stuAttribute
	{
		SString name;
		SString value;
	};
	SSvgAttribute()
	{
		m_Attribute.setAutoDelete(false);
		m_DeletedAttribute.setAutoDelete(false);
	}
	~SSvgAttribute()
	{
		m_Attribute.setAutoDelete(true);
		m_Attribute.clear();
		m_DeletedAttribute.setAutoDelete(true);
		m_DeletedAttribute.clear();
	}
	void Clear()
	{
		m_Attribute.copyto(m_DeletedAttribute);
		m_Attribute.clear();
	}
	inline stuAttribute* Find(SString &sName){return Find(sName.data());};
	stuAttribute* Find(const char* sName)
	{
		unsigned long pos = 0;
		stuAttribute *pAttr = m_Attribute.FetchFirst(pos);
		while(pAttr)
		{
			if(SString::equals(pAttr->name.data(),sName))
				return pAttr;
			pAttr = m_Attribute.FetchNext(pos);
		}
		return NULL;
	}
	inline bool Lookup(SString &sName,SString &sVal){return Lookup(sName.data(),sVal);};
	bool Lookup(const char* sName,SString &sVal)
	{
		stuAttribute *pAttr = Find(sName);
		if(pAttr == NULL)
			return false;
		sVal = pAttr->value;
		return true;
	}
	void SetAt(SString sName,SString sValue)
	{
		stuAttribute *pAttr = Find(sName);
		if(pAttr != NULL)
		{
			pAttr->value = sValue;
		}
		else
		{
			pAttr = m_DeletedAttribute[0];
			if(pAttr == NULL)
				pAttr = new stuAttribute();
			else
			{
				m_DeletedAttribute.remove(pAttr);
			}
			pAttr->name = sName;
			pAttr->value = sValue;
			m_Attribute.append(pAttr);
		}		
	}
	int GetCount()
	{
		return m_Attribute.count();
	}
	bool GetItem(int index,SString &sName,SString &sValue)
	{
		if(index<0 && index>=m_Attribute.count())
			return false;
		stuAttribute *pAttr = m_Attribute[index];
		if(pAttr == NULL)
			return false;
		sName = pAttr->name;
		sValue= pAttr->value;
		return true;
	}

private:
	SPtrList<stuAttribute> m_Attribute;
	SPtrList<stuAttribute> m_DeletedAttribute;
};

class SSvgObject;

//////////////////////////////////////////////////////////////////////////
// ��    ��:  SSvgModel
// ��    ��:  �ۿ���
// ����ʱ��:  2015-8-18 17:11
// ��    ��:  SVG����ģ���࣬���ڹ���SVG�Զ�̬��ȡ��ʾ���ݼ����
//////////////////////////////////////////////////////////////////////////
class SBASE_QTGUI_EXPORT SSvgDataModel
{
public:
	SSvgDataModel(){};
	virtual ~SSvgDataModel(){};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ�����������ʾ�ı�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-18 17:23
	// ����˵��:  @pObjΪͼԪָ��
	//         :  @sTextΪ���÷��ص��ı�����
	// �� �� ֵ:  true��ʾȡ���������ݣ�false��ʾδȡ����������
	//////////////////////////////////////////////////////////////////////////
	virtual bool GetViewText(SSvgObject *pObj,SString &sText){S_UNUSED(pObj);S_UNUSED(sText);return false;};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ����������ı�����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-19 9:06
	// ����˵��:  @pObjΪͼԪָ��
	//         :  @sFontΪ���÷��ص�����
	// �� �� ֵ:  true��ʾȡ���������ݣ�false��ʾδȡ����������
	//////////////////////////////////////////////////////////////////////////
	virtual bool GetViewTextFont(SSvgObject *pObj,SFont &sFont){S_UNUSED(pObj);S_UNUSED(sFont);return false;};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ����������ı���ɫ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-19 9:31
	// ����˵��:  @pObjΪͼԪָ��
	//         :  @sFontΪ���÷��ص�����
	// �� �� ֵ:  true��ʾȡ���������ݣ�false��ʾδȡ����������
	//////////////////////////////////////////////////////////////////////////
	virtual bool GetViewTextColor(SSvgObject *pObj,SCOLOR &sColor){S_UNUSED(pObj);S_UNUSED(sColor);return false;};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ���������������ɫ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-18 17:24
	// ����˵��:  @pObjΪͼԪָ��
	//         :  @sColorΪ���÷��ص���ɫ
	// �� �� ֵ:  true��ʾȡ���������ݣ�false��ʾδȡ����������
	//////////////////////////////////////////////////////////////////////////
	virtual bool GetViewLineColor(SSvgObject *pObj,SCOLOR &sColor){S_UNUSED(pObj);S_UNUSED(sColor);return false;};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��������������ɫ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-18 17:25
	// ����˵��:  @pObjΪͼԪָ��
	//         :  @bFillΪ���÷��ص��Ƿ����
	//         :  @sColorΪ���÷��ص���ɫ
	// �� �� ֵ:  true��ʾȡ���������ݣ�false��ʾδȡ����������
	//////////////////////////////////////////////////////////////////////////
	virtual bool GetViewBackColor(SSvgObject *pObj,bool &bFill,SCOLOR &sColor){S_UNUSED(pObj);S_UNUSED(bFill);S_UNUSED(sColor);return false;};
};


class SBASE_QTGUI_EXPORT SSvgObject// : public SString
{
public:
	//����SVGԪ������
	enum SVG_TYPE
	{
		TYPE_SVG,			//������
		TYPE_GROUP,			//SVG��
		TYPE_RECT,			//����
		TYPE_TEXT,			//����
		TYPE_ELLIPSE,		//��Բ
		TYPE_LINE,			//ֱ��
		TYPE_POLYLINE,		//����
		TYPE_PATH,			//·��
		TYPE_PIC,			//ͼƬ
		TYPE_RECTTEXT,		//�����ı�
		TYPE_USE,			//����ͼԪ����Ӧsymbolģ��
		TYPE_CIRCLE,		//��Բ
		TYPE_UNKNOWN		//δ֪
	};
	//����SVG��ʾ��Χ
	struct SVG_RECT
	{
		float left,top,right,bottom;
		float width()
		{
			return right-left;
		}
		float height()
		{
			return bottom-top;
		}
		float centerx()
		{
			return (left+right)/2;
		}
		float centery()
		{
			return (top+bottom)/2;
		}
	};
	

public:
	SSvgObject();
	virtual ~SSvgObject();
	inline void Clear()
	{
		m_pNext = NULL;
		m_pChild = NULL;
		m_pParent = NULL;
		m_bFlash = false;
		m_bUserDraw = false;
		m_pUserPtr = NULL;
		m_bAutoFlash = false;
		m_mapAttribute.Clear();
	}

public:
	bool IsExistObj(SSvgObject *pObj);
	//��������ֵ
	void SetAttributeValue(const char* sAttribute, const char* sValue, bool bSetNext=false,bool bSetChild=true);
	//��������ֵ
	inline void SetAttributeValue(const char* sAttribute, SString &sValue, bool bSetNext=false,bool bSetChild=true)
	{
		SetAttributeValue(sAttribute,sValue.data(),bSetNext,bSetChild);
	}
	//������һ������
	void SetNextObject(SSvgObject* pNewObj);
	//ȡ��һ������
	inline SSvgObject* GetNext(){return m_pNext;};
	inline void SetNext(SSvgObject* pNext){m_pNext = pNext;};
	//ȡ��һ�������׽ڵ�
	inline SSvgObject* GetChild(){return m_pChild;};
	//������һ�������׽ڵ�
	inline void SetChild(SSvgObject* pChild){m_pChild = pChild;};
	//ȡ���ڵ�
	inline SSvgObject* GetParent(){return m_pParent;};
	//���ø��ڵ�
	inline void SetParent(SSvgObject* pParent){m_pParent = pParent;};
	//�����¼��ڵ㣬ͬʱ�����¼���Z����ֵ����
	void AddChild(SSvgObject* pChild);
	void AddChildWithZ(SSvgObject* pChild);

	//���ö�����������
	inline void SetName(SString sName){m_sName = sName;};
	inline SString GetName(){return m_sName;};
	//ɾ����ǰ����������ڵ㼰�¼��ڵ�
	void Delete();
	//ɾ��ָ�����ӽڵ�
	void DeleteChild(SSvgObject* pChild, bool bDestroy=false);
	//���ص�ǰ�������Ч����,��������͡���ʾ��Χ��
	bool BuildObject(SSvgObject *pObj);//
	//������������ȡ��������
	int GetObjectTypeByName(SString sName);
	//���ݵ�ǰ�������ͣ����㵱ǰ�������ʾ��Χ
	bool CalcObjectRect();
	//ȡָ�����Ƶ�����ֵ
	inline SString GetAttribute(SString sName){return GetAttribute(sName.data());};
	inline float GetAttributeF(SString sName){return GetAttributeF(sName.data());};
	inline int GetAttributeI(SString sName){return GetAttributeI(sName.data());};
	SString GetAttribute(const char* sName);
	inline float GetAttributeF(const char* sName){return GetAttribute(sName).toFloat();};
	inline int GetAttributeI(const char* sName){return GetAttribute(sName).toInt();};

	//ȡ��������
	inline int GetType(){return m_SvgType;};
	inline void SetType(int type){m_SvgType = type;};

	inline SVG_RECT* GetRect(){return &m_Rect;};

	//���Ե�ǰ�����Ƿ���ȫ���ڸ�������Χ��
	bool IsInRect(SVG_RECT *pRect);

	static bool IsRectInRect(SVG_RECT *pRect1, SVG_RECT *pRect2);
	//����ǰ�������¼�ƫ��ָ����С
	bool Offset(float dx, float dy);
	//����ǰ�������ָ��λ�óɱ�������
	bool Scale(float scale,float x0=0,float y0=0);

	//��������������¼�,pDoc��ʾ������ͼԪ��Ŀ���ĵ�ָ��(SSvgDocument*)���ձ�ʾ��ǰ�ĵ�
	SSvgObject* Clone(SSvgObject *pParentObj=NULL, bool bWithNext=false,void *pDoc0=NULL);
	inline bool IsFlash(){return m_bFlash;};
	inline bool IsUserDraw(){return m_bUserDraw;};
	inline void SetFlash(bool b){m_bFlash = b;};
	inline bool IsAutoFlash(){return m_bAutoFlash;};
	inline void SetAutoFlash(bool b){m_bFlash = m_bAutoFlash = b;};
	inline void SetUserPtr(void *p){m_pUserPtr = p;};
	inline void* GetUserPtr(){return m_pUserPtr;};
	inline SSvgAttribute* GetAttributePtr(){return &m_mapAttribute;};

	SSvgObject *m_pChild;//ָ���¼��ڵ���׽ڵ�
	SSvgObject *m_pLastChild;//ָ���¼��ڵ���׽ڵ�
	void *m_pDocument;//SSvgDocument
	int m_z;//��ʾ������0��ײ㣬255��߲�
	//QPixmap *m_pUserDrawPixmap;//�û��Ի�λͼ��NULL��ʾû���Ի�
private:
	SSvgAttribute m_mapAttribute;//�������Լ���
	SSvgObject *m_pNext;//ָ����һ�ڵ�
	//SSvgObject *m_pChild;//ָ���¼��ڵ���׽ڵ�
	SSvgObject *m_pParent;//ָ�򸸽ڵ�
	bool m_bFlash;//�Ƿ��Ƕ���ͼ��
	bool m_bAutoFlash;//�Ƿ����Զ���˸��ͼ��
	bool m_bUserDraw;//�Ƿ���Ҫ�û������
	int m_SvgType;//��������
	SVG_RECT m_Rect;//��ǰԪ�ص���ʾ��Χ
	SString m_sName;//�����������ƣ�line/text/path/rect
	void* m_pUserPtr;//�û�ָ�룬����Modelʱʹ��

};

#endif // !defined(AFX_SKTSVGOBJECT_H__486DC941_8FDD_4B7B_8C09_947ECDB2F642__INCLUDED_)
