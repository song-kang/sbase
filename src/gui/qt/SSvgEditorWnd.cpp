/**
 *
 * 文 件 名 : SSvgEditorWnd.cpp
 * 创建日期 : 2015-8-21 14:17
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

#include "SSvgEditorWnd.h"

SSvgEditorWnd::SSvgEditorWnd(QWidget *parent/* =NULL */) : QWidget(parent)
{
	this->resize(1024, 768);
	gridLayout_3 = new QGridLayout(this);
	gridLayout_3->setContentsMargins(0, 0, 0, 0);
	m_splitter = new QSplitter(this);
	m_splitter->setOrientation(Qt::Horizontal);
	m_wndSvg = new SSvgWnd(m_splitter);
	
	m_wndSvg->SetShowRootRect(true);
	QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	sizePolicy.setHorizontalStretch(5);
	sizePolicy.setVerticalStretch(0);
	sizePolicy.setHeightForWidth(m_wndSvg->sizePolicy().hasHeightForWidth());
	m_wndSvg->setSizePolicy(sizePolicy);
	m_splitter->addWidget(m_wndSvg);
	m_tabWidget = new QTabWidget(m_splitter);
	//m_tabWidget->setStyleSheet(QString::fromUtf8("background-image: url();"));

	QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
	sizePolicy2.setHorizontalStretch(5);
	sizePolicy2.setVerticalStretch(0);
	sizePolicy2.setHeightForWidth(m_tabWidget->sizePolicy().hasHeightForWidth());
	 m_tabWidget->setSizePolicy(sizePolicy2);
	m_tabWidget->setMaximumSize(QSize(300, 16777215));
	m_tab = new QWidget();
	gridLayout_2 = new QGridLayout(m_tab);
	gridLayout_2->setContentsMargins(0, 0, 0, 0);
	m_wndLibrary = new SSvgLibraryWnd(m_tab);
	

	gridLayout_2->addWidget(m_wndLibrary, 0, 0, 1, 1);

	m_tabWidget->addTab(m_tab, QString());
	m_tab_2 = new QWidget();
	gridLayout = new QGridLayout(m_tab_2);
	gridLayout->setContentsMargins(0, 0, 0, 0);
	m_wndProperty = new SSvgAttributeWnd(m_tab_2,true);

	gridLayout->addWidget(m_wndProperty, 0, 0, 1, 1);

	m_tabWidget->addTab(m_tab_2, QString());
	m_splitter->addWidget(m_tabWidget);

	gridLayout_3->addWidget(m_splitter, 0, 0, 1, 1);

	m_tabWidget->setTabText(m_tabWidget->indexOf(m_tab), tr("图元库"));
	m_tabWidget->setTabText(m_tabWidget->indexOf(m_tab_2),tr("属性"));

	m_wndSvg->SetSvgAttributeWnd(m_wndProperty);
	m_wndSvg->SetEditMode(true);
	m_wndSvg->SetMovable(true);
	m_wndSvg->SetShowAlignPoint(true);
	m_wndSvg->SetShowRootRect(true);
	m_wndProperty->SetTableWidgetInfo(m_tabWidget,1);
	m_wndProperty->SetSvgWidget(m_wndSvg);
	m_wndProperty->SetEditorWidget(this);
	m_wndLibrary->SetSvgWnd(m_wndSvg);
	m_tabWidget->setFocusProxy(m_wndSvg);
	m_wndProperty->LoadObject(NULL);
}

SSvgEditorWnd::~SSvgEditorWnd()
{
	m_wndSvg->SetSvgAttributeWnd(NULL);
}

bool SSvgEditorWnd::LoadSvgByFile(SString sFile)
{
	bool ret = m_wndSvg->Load(sFile);
	m_wndProperty->LoadObject(NULL);
	return ret;
}

bool SSvgEditorWnd::LoadLibraryByFile(SString sFile)
{
	bool ret = m_wndLibrary->LoadByFile(sFile);
	m_wndProperty->LoadObject(NULL);
	return ret;
}

bool SSvgEditorWnd::LoadSvgByText(char *sText)
{
	bool ret = m_wndSvg->LoadText(sText);
	m_wndProperty->LoadObject(NULL);
	return ret;
}

bool SSvgEditorWnd::LoadLibraryByText(char *sText)
{
	bool ret = m_wndLibrary->LoadByXmlText(sText);
	m_wndProperty->LoadObject(NULL);
	return ret;
}

