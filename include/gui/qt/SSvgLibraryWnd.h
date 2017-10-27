/**
 *
 * 文 件 名 : SSvgLibraryWnd.h
 * 创建日期 : 2015-8-19 20:00
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : SVG图元库窗口
 * 修改记录 : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-8-19	邵凯田　创建文件
 *
 **/
#ifndef __SSVGLIBRARYWND_H__
#define __SSVGLIBRARYWND_H__

#include <QWidget>
//#include "ui_SSvgLibraryWnd.h"
#include "../SSvgWnd.h"
#include "SQt.h"
#include "SChinesePy.h"

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTableWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>


class SBASE_QTGUI_EXPORT CSvgEmbeddedWnd : public SSvgWnd
{
public:
	CSvgEmbeddedWnd(QWidget *parent=NULL);
	virtual ~CSvgEmbeddedWnd();

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  加载SVG图元
	// 作    者:  邵凯田
	// 创建时间:  2015-8-20 10:45
	// 参数说明:  @sSvgText为SVG文本内容
	//         :  @bAutoAddSvgHead为true表示自动添加SVG头
	// 返 回 值:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool LoadSvg(char *sSvgText,bool bAutoAddSvgHead=true);

	virtual int OnLButtonDown(int nFlag,SPoint point);

	QWidget *m_pSSvgLibraryWnd;
	SBaseConfig *m_pSvgNode;
};


class SBASE_QTGUI_EXPORT CSvgFilterEdit : public QLineEdit
{
public:
	friend class SSvgLibraryWnd;
	CSvgFilterEdit(QWidget *p):QLineEdit(p){m_pWnd = NULL;};
	virtual ~CSvgFilterEdit(){};

	virtual void keyPressEvent(QKeyEvent* event);
	virtual void dragEnterEvent(QDragEnterEvent* e);
	virtual void dragLeaveEvent(QDragLeaveEvent* e);
	virtual void dragMoveEvent(QDragMoveEvent* e);

	void *m_pWnd;
};


QT_BEGIN_NAMESPACE


class SBASE_QTGUI_EXPORT Ui_SSvgLibraryWnd
{
public:
	QGridLayout *gridLayout;
	QVBoxLayout *verticalLayout;
	QHBoxLayout *horizontalLayout;
	QComboBox *cmbType;
	CSvgFilterEdit *edtFilter;
	QSpacerItem *horizontalSpacer;
	QTableWidget *tabItems;

	void setupUi(QWidget *SSvgLibraryWnd)
	{
		if (SSvgLibraryWnd->objectName().isEmpty())
			SSvgLibraryWnd->setObjectName(QString::fromUtf8("SSvgLibraryWnd"));
		SSvgLibraryWnd->resize(210, 400);
		gridLayout = new QGridLayout(SSvgLibraryWnd);
		gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
		gridLayout->setContentsMargins(4, 4, 4, 4);
		verticalLayout = new QVBoxLayout();
		verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
		horizontalLayout = new QHBoxLayout();
		horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
		cmbType = new QComboBox(SSvgLibraryWnd);
		cmbType->setObjectName(QString::fromUtf8("cmbType"));
		cmbType->setMinimumSize(QSize(100, 0));

		horizontalLayout->addWidget(cmbType);

		edtFilter = new CSvgFilterEdit(SSvgLibraryWnd);
		edtFilter->setObjectName(QString::fromUtf8("edtFilter"));

		horizontalLayout->addWidget(edtFilter);

		horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

		horizontalLayout->addItem(horizontalSpacer);

		verticalLayout->addLayout(horizontalLayout);

		tabItems = new QTableWidget(SSvgLibraryWnd);
		tabItems->setObjectName(QString::fromUtf8("tabItems"));

		verticalLayout->addWidget(tabItems);

		gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);

		retranslateUi(SSvgLibraryWnd);

		QMetaObject::connectSlotsByName(SSvgLibraryWnd);
	} // setupUi

	void retranslateUi(QWidget *SSvgLibraryWnd)
	{
		//SSvgLibraryWnd->setWindowTitle(QApplication::translate("SSvgLibraryWnd", "SVG library", 0, QApplication::UnicodeUTF8));
		SSvgLibraryWnd->setWindowTitle(QObject::tr("SVG图元库"));
	} // retranslateUi

};

namespace Ui {
	class SSvgLibraryWnd: public Ui_SSvgLibraryWnd {};
} // namespace Ui

QT_END_NAMESPACE



class SBASE_QTGUI_EXPORT SSvgLibraryWnd : public QWidget
{
	Q_OBJECT
public:
	SSvgLibraryWnd(QWidget *parent=NULL);
	virtual ~SSvgLibraryWnd();

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取全局配置实例指针
	// 作    者:  邵凯田
	// 创建时间:  2015-10-26 11:15
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	static SXmlConfig* GetGlobalXmlConfig();

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  从文件加载缺省的图元库，静态函数
	// 作    者:  邵凯田
	// 创建时间:  2015-10-26 9:31
	// 参数说明:  @sFile为文件名
	// 返 回 值:  true表示加载成功，false表示加载失败
	//////////////////////////////////////////////////////////////////////////
	static bool LoadDefaultByFile(SString sFile);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  从XML文本加载缺省图元库配置，静态函数
	// 作    者:  邵凯田
	// 创建时间:  2015-10-26 9:31
	// 参数说明:  @sFile为文件名
	// 返 回 值:  true表示加载成功，false表示加载失败
	//////////////////////////////////////////////////////////////////////////
	static bool LoadDefaltByXmlText(char* sXmlText);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  从文件加载SVG图元库配置
	// 作    者:  邵凯田
	// 创建时间:  2015-8-19 20:14
	// 参数说明:  @sFile为文件名
	// 返 回 值:  true表示加载成功，false表示加载失败
	//////////////////////////////////////////////////////////////////////////
	bool LoadByFile(SString sFile);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  从XML文本加载图元库配置
	// 作    者:  邵凯田
	// 创建时间:  2015-8-19 20:15
	// 参数说明:  @sXmlText为XML内容
	// 返 回 值:  true表示加载成功，false表示加载失败
	//////////////////////////////////////////////////////////////////////////
	bool LoadByXmlText(char* sXmlText);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  根据全局的SVG库配置，加载SVG图元库
	// 作    者:  邵凯田
	// 创建时间:  2015-10-23 17:46
	// 参数说明:  void
	// 返 回 值:  bool
	//////////////////////////////////////////////////////////////////////////
	bool LoadSvgLib();

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  从当前打开的SVG文件中刷新对应的引用图形，如果有增加<内部引用图元>页，如果没有去除该页
	// 作    者:  邵凯田
	// 创建时间:  2016-11-24 16:55
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void RefreshSvgSymbol();

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  过滤文本改变处理
	// 作    者:  邵凯田
	// 创建时间:  2015-8-20 13:08
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	void OnFilterChanged();

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  点击对象
	// 作    者:  邵凯田
	// 创建时间:  2015-8-21 13:39
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	virtual void OnClickObject(SBaseConfig *pSvgNode);

	void SetSvgWnd(SSvgWnd *p){m_pSvgWnd = p;};

	public slots:
		void OnCurrentIndexChanged(int index);
		void OnTableCurrentCellChanged ( int currentRow, int currentColumn, int previousRow, int previousColumn );
		void OnTableCellClicked ( int row, int column );
private:
	Ui::SSvgLibraryWnd ui;	
	SBaseConfig *m_pThisType;//NULL表示全部
	SSvgWnd *m_pSvgWnd;//SVG主窗口的指针
	SStringList m_slSymbolObjectsText;//引用符号图元的SVG文本
	bool m_bUseSymbol;//是否正在使用符号
	SSvgObject m_TempUseObject;//临时引用图元
};

#endif // __SSVGLIBRARYWND_H__
