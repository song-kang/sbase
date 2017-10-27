/**
 *
 * 文 件 名 : SSvgObject.h
 * 创建日期 : 2007-04-09 08:55
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : SVG对象基类,封装SVG的基本元素
 * 修改记录 : 
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
// 名    称:  SSvgModel
// 作    者:  邵凯田
// 创建时间:  2015-8-18 17:11
// 描    述:  SVG数据模型类，用于关联SVG以动态获取显示内容及风格
//////////////////////////////////////////////////////////////////////////
class SBASE_QTGUI_EXPORT SSvgDataModel
{
public:
	SSvgDataModel(){};
	virtual ~SSvgDataModel(){};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取对象关联的显示文本
	// 作    者:  邵凯田
	// 创建时间:  2015-8-18 17:23
	// 参数说明:  @pObj为图元指针
	//         :  @sText为引用返回的文本内容
	// 返 回 值:  true表示取到关联数据，false表示未取到关联数据
	//////////////////////////////////////////////////////////////////////////
	virtual bool GetViewText(SSvgObject *pObj,SString &sText){S_UNUSED(pObj);S_UNUSED(sText);return false;};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取对象关联的文本字体
	// 作    者:  邵凯田
	// 创建时间:  2015-8-19 9:06
	// 参数说明:  @pObj为图元指针
	//         :  @sFont为引用返回的字体
	// 返 回 值:  true表示取到关联数据，false表示未取到关联数据
	//////////////////////////////////////////////////////////////////////////
	virtual bool GetViewTextFont(SSvgObject *pObj,SFont &sFont){S_UNUSED(pObj);S_UNUSED(sFont);return false;};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取对象关联的文本颜色
	// 作    者:  邵凯田
	// 创建时间:  2015-8-19 9:31
	// 参数说明:  @pObj为图元指针
	//         :  @sFont为引用返回的字体
	// 返 回 值:  true表示取到关联数据，false表示未取到关联数据
	//////////////////////////////////////////////////////////////////////////
	virtual bool GetViewTextColor(SSvgObject *pObj,SCOLOR &sColor){S_UNUSED(pObj);S_UNUSED(sColor);return false;};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取对象关联的线条颜色
	// 作    者:  邵凯田
	// 创建时间:  2015-8-18 17:24
	// 参数说明:  @pObj为图元指针
	//         :  @sColor为引用返回的颜色
	// 返 回 值:  true表示取到关联数据，false表示未取到关联数据
	//////////////////////////////////////////////////////////////////////////
	virtual bool GetViewLineColor(SSvgObject *pObj,SCOLOR &sColor){S_UNUSED(pObj);S_UNUSED(sColor);return false;};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取对象关联的填充颜色
	// 作    者:  邵凯田
	// 创建时间:  2015-8-18 17:25
	// 参数说明:  @pObj为图元指针
	//         :  @bFill为引用返回的是否填充
	//         :  @sColor为引用返回的颜色
	// 返 回 值:  true表示取到关联数据，false表示未取到关联数据
	//////////////////////////////////////////////////////////////////////////
	virtual bool GetViewBackColor(SSvgObject *pObj,bool &bFill,SCOLOR &sColor){S_UNUSED(pObj);S_UNUSED(bFill);S_UNUSED(sColor);return false;};
};


class SBASE_QTGUI_EXPORT SSvgObject// : public SString
{
public:
	//定义SVG元素类型
	enum SVG_TYPE
	{
		TYPE_SVG,			//根对象
		TYPE_GROUP,			//SVG组
		TYPE_RECT,			//区域
		TYPE_TEXT,			//文字
		TYPE_ELLIPSE,		//椭圆
		TYPE_LINE,			//直线
		TYPE_POLYLINE,		//折线
		TYPE_PATH,			//路径
		TYPE_PIC,			//图片
		TYPE_RECTTEXT,		//区域文本
		TYPE_USE,			//引用图元，对应symbol模板
		TYPE_CIRCLE,		//正圆
		TYPE_UNKNOWN		//未知
	};
	//定义SVG显示范围
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
	//设置属性值
	void SetAttributeValue(const char* sAttribute, const char* sValue, bool bSetNext=false,bool bSetChild=true);
	//设置属性值
	inline void SetAttributeValue(const char* sAttribute, SString &sValue, bool bSetNext=false,bool bSetChild=true)
	{
		SetAttributeValue(sAttribute,sValue.data(),bSetNext,bSetChild);
	}
	//增加下一个对象
	void SetNextObject(SSvgObject* pNewObj);
	//取下一个对象
	inline SSvgObject* GetNext(){return m_pNext;};
	inline void SetNext(SSvgObject* pNext){m_pNext = pNext;};
	//取下一层对象的首节点
	inline SSvgObject* GetChild(){return m_pChild;};
	//设置下一层对象的首节点
	inline void SetChild(SSvgObject* pChild){m_pChild = pChild;};
	//取父节点
	inline SSvgObject* GetParent(){return m_pParent;};
	//设置父节点
	inline void SetParent(SSvgObject* pParent){m_pParent = pParent;};
	//插入下级节点，同时根据下级的Z坐标值排序
	void AddChild(SSvgObject* pChild);
	void AddChildWithZ(SSvgObject* pChild);

	//设置对象类型名称
	inline void SetName(SString sName){m_sName = sName;};
	inline SString GetName(){return m_sName;};
	//删除当前对象及其后续节点及下级节点
	void Delete();
	//删除指定的子节点
	void DeleteChild(SSvgObject* pChild, bool bDestroy=false);
	//加载当前对象的有效属性,如对象类型、显示范围等
	bool BuildObject(SSvgObject *pObj);//
	//根据类型名称取对象类型
	int GetObjectTypeByName(SString sName);
	//依据当前对象类型，计算当前对象的显示范围
	bool CalcObjectRect();
	//取指定名称的属性值
	inline SString GetAttribute(SString sName){return GetAttribute(sName.data());};
	inline float GetAttributeF(SString sName){return GetAttributeF(sName.data());};
	inline int GetAttributeI(SString sName){return GetAttributeI(sName.data());};
	SString GetAttribute(const char* sName);
	inline float GetAttributeF(const char* sName){return GetAttribute(sName).toFloat();};
	inline int GetAttributeI(const char* sName){return GetAttribute(sName).toInt();};

	//取对象类型
	inline int GetType(){return m_SvgType;};
	inline void SetType(int type){m_SvgType = type;};

	inline SVG_RECT* GetRect(){return &m_Rect;};

	//测试当前对象是否完全处于给定区域范围内
	bool IsInRect(SVG_RECT *pRect);

	static bool IsRectInRect(SVG_RECT *pRect1, SVG_RECT *pRect2);
	//将当前对象及其下级偏移指定大小
	bool Offset(float dx, float dy);
	//将当前对象基于指定位置成比例缩放
	bool Scale(float scale,float x0=0,float y0=0);

	//复制自身对象及其下级,pDoc表示新生成图元的目标文档指针(SSvgDocument*)，空表示当前文档
	SSvgObject* Clone(SSvgObject *pParentObj=NULL, bool bWithNext=false,void *pDoc0=NULL);
	inline bool IsFlash(){return m_bFlash;};
	inline bool IsUserDraw(){return m_bUserDraw;};
	inline void SetFlash(bool b){m_bFlash = b;};
	inline bool IsAutoFlash(){return m_bAutoFlash;};
	inline void SetAutoFlash(bool b){m_bFlash = m_bAutoFlash = b;};
	inline void SetUserPtr(void *p){m_pUserPtr = p;};
	inline void* GetUserPtr(){return m_pUserPtr;};
	inline SSvgAttribute* GetAttributePtr(){return &m_mapAttribute;};

	SSvgObject *m_pChild;//指向下级节点的首节点
	SSvgObject *m_pLastChild;//指向下级节点的首节点
	void *m_pDocument;//SSvgDocument
	int m_z;//显示层数，0最底层，255最高层
	//QPixmap *m_pUserDrawPixmap;//用户自绘位图，NULL表示没有自绘
private:
	SSvgAttribute m_mapAttribute;//对象属性集合
	SSvgObject *m_pNext;//指向下一节点
	//SSvgObject *m_pChild;//指向下级节点的首节点
	SSvgObject *m_pParent;//指向父节点
	bool m_bFlash;//是否是动画图层
	bool m_bAutoFlash;//是否是自动闪烁的图层
	bool m_bUserDraw;//是否需要用户层绘制
	int m_SvgType;//对象类型
	SVG_RECT m_Rect;//当前元素的显示范围
	SString m_sName;//对象类型名称：line/text/path/rect
	void* m_pUserPtr;//用户指针，关联Model时使用

};

#endif // !defined(AFX_SKTSVGOBJECT_H__486DC941_8FDD_4B7B_8C09_947ECDB2F642__INCLUDED_)
