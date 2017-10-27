/**
 *
 * 文 件 名 : SSvgDocument.h
 * 创建日期 : 2007-04-08 18:55
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : SVG文档类
 * 修改记录 : 
 *            $Log: $
 *
 **/

#if !defined(AFX_SKTSVGDOCUMENT_H__F7C1AB6F_CEDE_4835_8D08_4D5EEA9BA80F__INCLUDED_)
#define AFX_SKTSVGDOCUMENT_H__F7C1AB6F_CEDE_4835_8D08_4D5EEA9BA80F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../SString.h"
#include "../../SFile.h"
#include "../../SXmlConfig.h"

#include "SSvgObject.h"
#include "SPicture.h"
class SSvgObject;

class SBASE_QTGUI_EXPORT SSvgDocument  
{
public:
	friend class SSvgObject;

	struct stuImageDefine
	{
		stuImageDefine()
		{
			m_pPic = NULL;
		}
		~stuImageDefine()
		{
			if(m_pPic != NULL)
				delete m_pPic;
		}
		SString m_sImageDefSn;//图片唯一序号，作为引用名称
		SString m_sImageHex;//图片内容的16进制文本
		SString m_sLocalFileName;//本地文件名
		SPicture *m_pPic;//图片资源类
		int m_iWidth,m_iHeight;//图片原始的宽、高
		int m_iUsedTimes;//被引用的次数，初次加载时使用，如果为零则删除
	};

	struct stuGradientStop//渐变项
	{
		int offset;//%
		SCOLOR color;//stop color
		int opacity;
		void Set(SBaseConfig *pStop);
	};

	struct stuLinearGradient
	{
		stuLinearGradient()
		{
			Stops.setAutoDelete(true);
		}
		SString id;
		int x1,y1,x2,y2;
		SPtrList<stuGradientStop> Stops;
	};

	struct stuRadialGradient
	{
		stuRadialGradient()
		{
			Stops.setAutoDelete(true);
		}
		SString id;
		int cx,cy,r,fx,fy;
		SPtrList<stuGradientStop> Stops;
	};
	inline stuLinearGradient* SearchLinearGradientById(SString id)
	{
		unsigned long pos;
		stuLinearGradient* p = m_LinearGradients.FetchFirst(pos);
		while(p)
		{
			if(p->id == id)
				return p;
			p = m_LinearGradients.FetchNext(pos);
		}
		return NULL;
	}
	inline stuRadialGradient* SearchRadialGradientById(SString id)
	{
		unsigned long pos;
		stuRadialGradient* p = m_RadialGradients.FetchFirst(pos);
		while(p)
		{
			if(p->id == id)
				return p;
			p = m_RadialGradients.FetchNext(pos);
		}
		return NULL;
	}
	void ParseStyle(SSvgObject *pObj, SString style);
	SSvgObject* AddObject(SString sName, SString sAttr);
	SString GetAllObjects();
	SSvgObject* SearchByURI(SString uri);
	SSvgObject* SearchByAttribute(const char* attr,const char* val);
	void SortObject(SSvgObject *pObj);
	bool NewFile();
	SSvgDocument();
	virtual ~SSvgDocument();

	//加载SVG文件
	bool LoadSvgFile(SString sSvgFile);
	bool LoadSvgCfg(SBaseConfig *pRootNode);
	//加载SVG文本
	bool LoadSvgText(const char* sSvgText);
	//保存SVG文件
	bool SaveSvgFile(SString sSvgFile,bool bUtf8=false);
	bool SaveSvgFileAsUtf8(SString sSvgFile);

	//保存SVG到字符串
	bool SaveSvgFileToText(SString &sSvgText,bool bUtf8=false);

	//递归写SVG节点
	bool WriteSvgNode(SSvgObject *pSvgObj, SFile *pFile, int iLevel=0,SString *pOutText=NULL);
	
	//////////////////////////////////////////////////////////////////////////
	// 描    述:  写预定义资源节点
	// 作    者:  邵凯田
	// 创建时间:  2015-11-26 9:14
	// 参数说明:  
	// 返 回 值:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool WriteSvgDefs(SFile *pFile, int iLevel=0,SString *pOutText=NULL,bool bWithoutImg=false);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  测试指定的图片是否被使用
	// 作    者:  邵凯田
	// 创建时间:  2015-11-26 19:03
	// 参数说明:  
	// 返 回 值:  true表示被使用，false表示未使用
	//////////////////////////////////////////////////////////////////////////
	bool IsImageDefUsed(SSvgObject *pParentObj,SString sImgDefSn);

	//写属性到文件
	bool WriteAttribute(SSvgObject *pSvgObj, SFile *pFile, SString sAttrName, SString sAttrValue="'{<NONE>}'");
	bool WriteAttributeToText(SSvgObject *pSvgObj, SString sAttrName,SString *pOutText=NULL, SString sAttrValue="'{<NONE>}'");

	//递归加载SVG元素到根对象下
	bool ReadSvgNode(SSvgObject *pParentSvgObj, SBaseConfig *pXmlNode);
	//清除所有对象
	bool RemoveObjects(bool bBkThread=false);
	//取根对象
	SSvgObject* GetRootObject(){return m_pSvgRoot;};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  设置根节点对象，用于快速替换SVG对象
	// 作    者:  邵凯田
	// 创建时间:  2015-12-8 17:37
	// 参数说明:  @pRoot
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void SetRootObject(SSvgObject* pRoot){m_pSvgRoot = pRoot;};

	SSvgObject* GetFirstObject()
	{
		SSvgObject *pObj = GetRootObject();
		if(pObj == NULL)
			return NULL;
		pObj = pObj->GetChild();
		return pObj;
	};
	inline SString GetSvgPath(){return m_sSvgPath;};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  处理预定义的defs段
	// 作    者:  邵凯田
	// 创建时间:  2015-11-26 8:52
	// 参数说明:  pXmlNode为defs节点
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void ProcessDefsNode(SBaseConfig *pXmlNode);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  处理预定义的图片资源
	// 作    者:  邵凯田
	// 创建时间:  2015-11-26 8:57
	// 参数说明:  pImgNode
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void ProcessImageDef(SBaseConfig *pImgNode);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  处理预定义的模板资源
	// 作    者:  邵凯田
	// 创建时间:  2016-11-8 15:35
	// 参数说明:  @pSynbolNode
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void ProcessSymbol(SBaseConfig *pSymbolNode);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  处理线性渐变预定义
	// 作    者:  邵凯田
	// 创建时间:  2017-9-7 14:25
	// 参数说明:  @pNode
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void ProcessLinearGradient(SBaseConfig *pNode);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  处理线性渐变预定义
	// 作    者:  邵凯田
	// 创建时间:  2017-9-7 14:25
	// 参数说明:  @pNode
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void ProcessRadialGradient(SBaseConfig *pNode);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  检索指定ID的模板
	// 作    者:  邵凯田
	// 创建时间:  2016-11-8 15:43
	// 参数说明:  @id为内容，不含'#'
	// 返 回 值:  SSvgObject*, NULL表示找不到
	//////////////////////////////////////////////////////////////////////////
	SSvgObject* SearchSymbolObj(SString id);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取当前文档符号引用图元的对象列表
	// 作    者:  邵凯田
	// 创建时间:  2016-11-24 16:58
	// 参数说明:  void
	// 返 回 值:  SPtrList<SSvgObject>*
	//////////////////////////////////////////////////////////////////////////
	SPtrList<SSvgObject>* GetSymbolObjectsPtr(){return &m_SymbolObjects;};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  加载图片资源
	// 作    者:  邵凯田
	// 创建时间:  2015-11-26 10:29
	// 参数说明:  
	// 返 回 值:  stuImageDefine*
	//////////////////////////////////////////////////////////////////////////
	stuImageDefine* LoadImage(int w,int h,SString &sImgHex);
	stuImageDefine* LoadImage(SString sFileName,bool bInsertInner);
	stuImageDefine* SearchImage(SString sImageDefSn);
	stuImageDefine* SearchImageByLocalFile(SString sLocalFileName);
	SString NewImageSn();
	SPtrList<stuImageDefine>* GetImageDefs(){return &m_ImageDefs;};
	static void SwapObjectList(SSvgDocument *pDoc1,SSvgDocument *pDoc2)
	{
		SPtrList<SSvgObject> objs;
		objs.setAutoDelete(false);

		pDoc1->m_AllObjects.copyto(objs);
		pDoc1->m_AllObjects.clear();
		pDoc2->m_AllObjects.copyto(pDoc1->m_AllObjects);
		pDoc2->m_AllObjects.clear();
		objs.copyto(pDoc2->m_AllObjects);
		objs.clear();

		pDoc1->m_DeletedObjects.copyto(objs);
		pDoc1->m_DeletedObjects.clear();
		pDoc2->m_DeletedObjects.copyto(pDoc1->m_DeletedObjects);
		pDoc2->m_DeletedObjects.clear();
		objs.copyto(pDoc2->m_DeletedObjects);
		objs.clear();
	}
	void SetCoorType(SString coor_type){m_iCoorType = (coor_type=="left_bottom")?1:0;};
	void SetOrigX(float x){m_fOrigX = x;};
	void SetOrigY(float y){m_fOrigY = y;};
	void SetHeight(float a){m_fHeight = a;};
	void SetWidth(float w){m_fWidth = w;};

	inline int GetCoorType(){return m_iCoorType;};
	inline float GetOrigX(){return m_fOrigX;};
	inline float GetOrigY(){return m_fOrigY;};
	inline float GetHeight(){return m_fHeight;};
	inline float GetWidth(){return m_fWidth;};


private:
	static void* ThreadRemoveObjs(void *lp);
	inline SSvgObject* NewObject()
	{
		SSvgObject *p = m_DeletedObjects[0];
		if(p == NULL)
		{
			p = new SSvgObject();
			p->m_pDocument = this;
		}
		else
		{
			p->SetNext(NULL);
			p->SetParent(NULL);
			SSvgObject *pChild = p->GetChild();
			SSvgObject *pChild0;
			while(pChild != NULL)
			{
				pChild0 = pChild->GetNext();
				p->DeleteChild(pChild);
				pChild = pChild0;
			}
			m_DeletedObjects.remove(p);
		}
		m_AllObjects.append(p);
		p->Clear();
		return p;
	}

	SString m_sSvgFile;//SVG文件名
	SString m_sSvgPath;//SVG路径
	SSvgObject *m_pSvgRoot;//SVG根对象
	bool m_bUtf8;
	SPtrList<stuImageDefine> m_ImageDefs;//图片资源集
	SPtrList<SSvgObject> m_AllObjects;//所有的对象图元，不分层次；
	SPtrList<SSvgObject> m_DeletedObjects;//所有已经删除的图元，不分层次，用于删除以及重加载时使用
	SPtrList<SSvgObject> m_SymbolObjects;//所有的模板符号对象
	SPtrList<stuLinearGradient> m_LinearGradients;
	SPtrList<stuRadialGradient> m_RadialGradients;
	int m_iCoorType;//0表示左上角原点，1表示左下解原点
	float m_fOrigX,m_fOrigY;//原点坐标的值，正常为0，非0时表示原点偏移
	float m_fHeight,m_fWidth;
};

#endif // !defined(AFX_YSSVGDOCUMENT_H__F7C1AB6F_CEDE_4835_8D08_4D5EEA9BA80F__INCLUDED_)
