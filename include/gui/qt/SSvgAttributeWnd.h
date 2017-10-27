/**
 *
 * 文 件 名 : SSvgAttributeWnd.h
 * 创建日期 : 2015-8-20 14:36
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : SVG图元属性窗口
 * 修改记录 : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-8-20	邵凯田　创建文件
 *
 **/

#ifndef __SKT_SVG_ATTRIBUTE_H__
#define __SKT_SVG_ATTRIBUTE_H__

#include <QWidget>
#include <QtGui/QGridLayout>
#include <QtGui/QTableWidget>
#include <QColorDialog>
#include <QComboBox>
#include "../svg/SSvgObject.h"
#include "SQt.h"

class SBASE_QTGUI_EXPORT SSvgMultiEditDlg : public QDialog
{
	Q_OBJECT
public:
	SSvgMultiEditDlg(QWidget *parent=NULL);
	virtual ~SSvgMultiEditDlg();

	bool ShowMultiInput(SString sTitle,SString &sText);
	
public slots:
	void OnOk();
	void OnCancel();

private:
	QGridLayout *gridLayout;
	QVBoxLayout *verticalLayout;
	QPlainTextEdit *edtText;
	QHBoxLayout *horizontalLayout;
	QSpacerItem *horizontalSpacer;
	QPushButton *btnOk;
	QPushButton *btnCancel;

};


class SBASE_QTGUI_EXPORT SSvgPointsEditDlg : public QDialog
{
	Q_OBJECT
public:
	SSvgPointsEditDlg(QWidget *parent=NULL);
	virtual ~SSvgPointsEditDlg();

	bool ShowEdit(SSvgObject *pObj,SString sText);

	void GetPoints(SString &points,SString &extattr);
public slots:
	void OnAdd();
	void OnInsert();
	void OnDel();

	void OnOk();
	void OnCancel();
	void on_tableWidget_itemChanged(QTableWidgetItem * item);
	void onCurrentCellChanged ( int currentRow, int currentColumn, int previousRow, int previousColumn );

public:
	QWidget *m_pSvgWnd;
	SString m_sOldText;
	SSvgObject *m_pSvgObject;

private:
	QGridLayout *gridLayout;
	QVBoxLayout *verticalLayout;
	QTableWidget *tableWidget;
	QHBoxLayout *horizontalLayout;
	QSpacerItem *horizontalSpacer;
	QPushButton *btnAdd;
	QPushButton *btnInsert;
	QPushButton *btnDel;
	QPushButton *btnOk;
	QPushButton *btnCancel;
	bool m_bInited;
};

class SBASE_QTGUI_EXPORT SSvgAttributeWnd : public QWidget
{
	Q_OBJECT
public:
	SSvgAttributeWnd(QWidget *p,bool bEmbedded=false);
	virtual ~SSvgAttributeWnd();

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  加载指定的对象到当前属性窗口
	// 作    者:  邵凯田
	// 创建时间:  2015-8-20 14:47
	// 参数说明:  @pObj待加载的对象,NULL表示最后显示的对象
	// 返 回 值:  true表示加载成功，false表示加载失败
	//////////////////////////////////////////////////////////////////////////
	bool LoadObject(SSvgObject *pObj=NULL);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  添加属性值到属性列表
	// 作    者:  邵凯田
	// 创建时间:  2015-8-20 15:05
	// 参数说明:  @sName为属性名称
	//         :  @pObj为对象指针
	// 返 回 值:  返回表格行号
	//////////////////////////////////////////////////////////////////////////
	int AddAttribute(SString sName,SSvgObject *pObj);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  添加画布属性列表
	// 作    者:  邵凯田
	// 创建时间:  2015-8-22 10:18
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	int AddAttributeEx(const char* sName,const char* sTitle,SString sValue);
	inline int AddAttributeEx(SString sName,SString sTitle,SString sValue)
	{
		return AddAttributeEx(sName.data(),sTitle.data(),sValue);
	}

	QColor SColor2QColor(SCOLOR col);
	SCOLOR QColor2SColor(QColor col);

	void Show();
	void Hide();

	void SetTableWidgetInfo(QTabWidget *p,int idx){m_pTabWidget = p;m_iTabIndex = idx;};
	void SetSvgWidget(QWidget *p);
	void SetEditorWidget(QWidget *p){m_pEditorWnd = p;};
public slots:
	void OnCellClicked(int row,int column);
	void OnCursorPos(float x,float y);
	void on_tableWidget_itemChanged(QTableWidgetItem * item);
	void OnLTCurrentIndexChanged(int idx);
private:
	QGridLayout *m_gridLayout;
	QTableWidget *m_tableWidget;
	QWidget *m_pSvgWnd;
	QWidget *m_pEditorWnd;
	SSvgObject *m_pThisObj;
	bool m_bDisableAttrEdit;//禁用属性编辑完成事件，刷新属性时禁用，防止值被写错
	QTabWidget *m_pTabWidget;//当前窗口对应的TAB控件
	int m_iTabIndex;//当前窗口对应的TAB序号
	SSvgPointsEditDlg *m_pPointsEditDlg;
};


#endif //__SKT_SVG_ATTRIBUTE_H__
