/**
 *
 * 文 件 名 : SSvgEditorWnd.h
 * 创建日期 : 2015-8-21 14:16
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : SVG编辑窗口
 * 修改记录 : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-8-21	邵凯田　创建文件
 *
 **/

#ifndef __SKT_SVG_EDITOR_H__
#define __SKT_SVG_EDITOR_H__

#include "SSvgLibraryWnd.h"

class SBASE_QTGUI_EXPORT SSvgEditorWnd : public QWidget
{
Q_OBJECT
public:
	SSvgEditorWnd(QWidget *parent=NULL);
	virtual ~SSvgEditorWnd();

	bool LoadSvgByFile(SString sFile);
	bool LoadLibraryByFile(SString sFile);
	bool LoadSvgByText(char *sText);
	bool LoadLibraryByText(char *sText);

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
	
	inline SSvgWnd* GetSvgWnd(){return m_wndSvg;};
	inline SSvgAttributeWnd* GetAttributeWnd(){return m_wndProperty;};
	inline SSvgLibraryWnd* GetLibraryWnd(){return m_wndLibrary;};

private:
	QGridLayout *gridLayout_3;
	QSplitter *m_splitter;
	QTabWidget *m_tabWidget;
	QWidget *m_tab;
	QGridLayout *gridLayout_2;
	QWidget *m_tab_2;
	QGridLayout *gridLayout;
	SSvgAttributeWnd *m_wndProperty;
	SSvgWnd *m_wndSvg;
	SSvgLibraryWnd *m_wndLibrary;

};


#endif //__SKT_SVG_EDITOR_H__
