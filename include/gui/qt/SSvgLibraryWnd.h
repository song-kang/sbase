/**
 *
 * �� �� �� : SSvgLibraryWnd.h
 * �������� : 2015-8-19 20:00
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : SVGͼԪ�ⴰ��
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-8-19	�ۿ�������ļ�
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
	// ��    ��:  ����SVGͼԪ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-20 10:45
	// ����˵��:  @sSvgTextΪSVG�ı�����
	//         :  @bAutoAddSvgHeadΪtrue��ʾ�Զ����SVGͷ
	// �� �� ֵ:  true/false
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
		SSvgLibraryWnd->setWindowTitle(QObject::tr("SVGͼԪ��"));
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
	// ��    ��:  ȡȫ������ʵ��ָ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-10-26 11:15
	// ����˵��:  
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	static SXmlConfig* GetGlobalXmlConfig();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���ļ�����ȱʡ��ͼԪ�⣬��̬����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-10-26 9:31
	// ����˵��:  @sFileΪ�ļ���
	// �� �� ֵ:  true��ʾ���سɹ���false��ʾ����ʧ��
	//////////////////////////////////////////////////////////////////////////
	static bool LoadDefaultByFile(SString sFile);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��XML�ı�����ȱʡͼԪ�����ã���̬����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-10-26 9:31
	// ����˵��:  @sFileΪ�ļ���
	// �� �� ֵ:  true��ʾ���سɹ���false��ʾ����ʧ��
	//////////////////////////////////////////////////////////////////////////
	static bool LoadDefaltByXmlText(char* sXmlText);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���ļ�����SVGͼԪ������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-19 20:14
	// ����˵��:  @sFileΪ�ļ���
	// �� �� ֵ:  true��ʾ���سɹ���false��ʾ����ʧ��
	//////////////////////////////////////////////////////////////////////////
	bool LoadByFile(SString sFile);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��XML�ı�����ͼԪ������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-19 20:15
	// ����˵��:  @sXmlTextΪXML����
	// �� �� ֵ:  true��ʾ���سɹ���false��ʾ����ʧ��
	//////////////////////////////////////////////////////////////////////////
	bool LoadByXmlText(char* sXmlText);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����ȫ�ֵ�SVG�����ã�����SVGͼԪ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-10-23 17:46
	// ����˵��:  void
	// �� �� ֵ:  bool
	//////////////////////////////////////////////////////////////////////////
	bool LoadSvgLib();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �ӵ�ǰ�򿪵�SVG�ļ���ˢ�¶�Ӧ������ͼ�Σ����������<�ڲ�����ͼԪ>ҳ�����û��ȥ����ҳ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-11-24 16:55
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void RefreshSvgSymbol();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����ı��ı䴦��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-20 13:08
	// ����˵��:  
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	void OnFilterChanged();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-21 13:39
	// ����˵��:  
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	virtual void OnClickObject(SBaseConfig *pSvgNode);

	void SetSvgWnd(SSvgWnd *p){m_pSvgWnd = p;};

	public slots:
		void OnCurrentIndexChanged(int index);
		void OnTableCurrentCellChanged ( int currentRow, int currentColumn, int previousRow, int previousColumn );
		void OnTableCellClicked ( int row, int column );
private:
	Ui::SSvgLibraryWnd ui;	
	SBaseConfig *m_pThisType;//NULL��ʾȫ��
	SSvgWnd *m_pSvgWnd;//SVG�����ڵ�ָ��
	SStringList m_slSymbolObjectsText;//���÷���ͼԪ��SVG�ı�
	bool m_bUseSymbol;//�Ƿ�����ʹ�÷���
	SSvgObject m_TempUseObject;//��ʱ����ͼԪ
};

#endif // __SSVGLIBRARYWND_H__
