/**
 *
 * �� �� �� : SSvgEditorWnd.h
 * �������� : 2015-8-21 14:16
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : SVG�༭����
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-8-21	�ۿ�������ļ�
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
	// ��    ��:  �༭ͼԪ��չ����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-20 22:22
	// ����˵��:  @pObjΪ���༭�Ķ���ָ��
	//         :  @sExtAttrΪ��չ�����ַ��������÷���
	// �� �� ֵ:  true��ʾ�ѱ༭���������ݲ�û�б䣬��ֻҪ��Ӧ�˾ͱ��뷵��true)��false��ʾδ����
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
