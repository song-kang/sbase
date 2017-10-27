/**
 *
 * �� �� �� : SSvgDocument.h
 * �������� : 2007-04-08 18:55
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : SVG�ĵ���
 * �޸ļ�¼ : 
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
		SString m_sImageDefSn;//ͼƬΨһ��ţ���Ϊ��������
		SString m_sImageHex;//ͼƬ���ݵ�16�����ı�
		SString m_sLocalFileName;//�����ļ���
		SPicture *m_pPic;//ͼƬ��Դ��
		int m_iWidth,m_iHeight;//ͼƬԭʼ�Ŀ���
		int m_iUsedTimes;//�����õĴ��������μ���ʱʹ�ã����Ϊ����ɾ��
	};

	struct stuGradientStop//������
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

	//����SVG�ļ�
	bool LoadSvgFile(SString sSvgFile);
	bool LoadSvgCfg(SBaseConfig *pRootNode);
	//����SVG�ı�
	bool LoadSvgText(const char* sSvgText);
	//����SVG�ļ�
	bool SaveSvgFile(SString sSvgFile,bool bUtf8=false);
	bool SaveSvgFileAsUtf8(SString sSvgFile);

	//����SVG���ַ���
	bool SaveSvgFileToText(SString &sSvgText,bool bUtf8=false);

	//�ݹ�дSVG�ڵ�
	bool WriteSvgNode(SSvgObject *pSvgObj, SFile *pFile, int iLevel=0,SString *pOutText=NULL);
	
	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  дԤ������Դ�ڵ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-11-26 9:14
	// ����˵��:  
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool WriteSvgDefs(SFile *pFile, int iLevel=0,SString *pOutText=NULL,bool bWithoutImg=false);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����ָ����ͼƬ�Ƿ�ʹ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-11-26 19:03
	// ����˵��:  
	// �� �� ֵ:  true��ʾ��ʹ�ã�false��ʾδʹ��
	//////////////////////////////////////////////////////////////////////////
	bool IsImageDefUsed(SSvgObject *pParentObj,SString sImgDefSn);

	//д���Ե��ļ�
	bool WriteAttribute(SSvgObject *pSvgObj, SFile *pFile, SString sAttrName, SString sAttrValue="'{<NONE>}'");
	bool WriteAttributeToText(SSvgObject *pSvgObj, SString sAttrName,SString *pOutText=NULL, SString sAttrValue="'{<NONE>}'");

	//�ݹ����SVGԪ�ص���������
	bool ReadSvgNode(SSvgObject *pParentSvgObj, SBaseConfig *pXmlNode);
	//������ж���
	bool RemoveObjects(bool bBkThread=false);
	//ȡ������
	SSvgObject* GetRootObject(){return m_pSvgRoot;};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���ø��ڵ�������ڿ����滻SVG����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-12-8 17:37
	// ����˵��:  @pRoot
	// �� �� ֵ:  void
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
	// ��    ��:  ����Ԥ�����defs��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-11-26 8:52
	// ����˵��:  pXmlNodeΪdefs�ڵ�
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void ProcessDefsNode(SBaseConfig *pXmlNode);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����Ԥ�����ͼƬ��Դ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-11-26 8:57
	// ����˵��:  pImgNode
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void ProcessImageDef(SBaseConfig *pImgNode);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����Ԥ�����ģ����Դ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-11-8 15:35
	// ����˵��:  @pSynbolNode
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void ProcessSymbol(SBaseConfig *pSymbolNode);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �������Խ���Ԥ����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-9-7 14:25
	// ����˵��:  @pNode
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void ProcessLinearGradient(SBaseConfig *pNode);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �������Խ���Ԥ����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-9-7 14:25
	// ����˵��:  @pNode
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void ProcessRadialGradient(SBaseConfig *pNode);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����ָ��ID��ģ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-11-8 15:43
	// ����˵��:  @idΪ���ݣ�����'#'
	// �� �� ֵ:  SSvgObject*, NULL��ʾ�Ҳ���
	//////////////////////////////////////////////////////////////////////////
	SSvgObject* SearchSymbolObj(SString id);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��ǰ�ĵ���������ͼԪ�Ķ����б�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-11-24 16:58
	// ����˵��:  void
	// �� �� ֵ:  SPtrList<SSvgObject>*
	//////////////////////////////////////////////////////////////////////////
	SPtrList<SSvgObject>* GetSymbolObjectsPtr(){return &m_SymbolObjects;};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����ͼƬ��Դ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-11-26 10:29
	// ����˵��:  
	// �� �� ֵ:  stuImageDefine*
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

	SString m_sSvgFile;//SVG�ļ���
	SString m_sSvgPath;//SVG·��
	SSvgObject *m_pSvgRoot;//SVG������
	bool m_bUtf8;
	SPtrList<stuImageDefine> m_ImageDefs;//ͼƬ��Դ��
	SPtrList<SSvgObject> m_AllObjects;//���еĶ���ͼԪ�����ֲ�Σ�
	SPtrList<SSvgObject> m_DeletedObjects;//�����Ѿ�ɾ����ͼԪ�����ֲ�Σ�����ɾ���Լ��ؼ���ʱʹ��
	SPtrList<SSvgObject> m_SymbolObjects;//���е�ģ����Ŷ���
	SPtrList<stuLinearGradient> m_LinearGradients;
	SPtrList<stuRadialGradient> m_RadialGradients;
	int m_iCoorType;//0��ʾ���Ͻ�ԭ�㣬1��ʾ���½�ԭ��
	float m_fOrigX,m_fOrigY;//ԭ�������ֵ������Ϊ0����0ʱ��ʾԭ��ƫ��
	float m_fHeight,m_fWidth;
};

#endif // !defined(AFX_YSSVGDOCUMENT_H__F7C1AB6F_CEDE_4835_8D08_4D5EEA9BA80F__INCLUDED_)
