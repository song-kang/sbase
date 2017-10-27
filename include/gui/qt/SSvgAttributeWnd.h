/**
 *
 * �� �� �� : SSvgAttributeWnd.h
 * �������� : 2015-8-20 14:36
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : SVGͼԪ���Դ���
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-8-20	�ۿ�������ļ�
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
	// ��    ��:  ����ָ���Ķ��󵽵�ǰ���Դ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-20 14:47
	// ����˵��:  @pObj�����صĶ���,NULL��ʾ�����ʾ�Ķ���
	// �� �� ֵ:  true��ʾ���سɹ���false��ʾ����ʧ��
	//////////////////////////////////////////////////////////////////////////
	bool LoadObject(SSvgObject *pObj=NULL);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �������ֵ�������б�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-20 15:05
	// ����˵��:  @sNameΪ��������
	//         :  @pObjΪ����ָ��
	// �� �� ֵ:  ���ر���к�
	//////////////////////////////////////////////////////////////////////////
	int AddAttribute(SString sName,SSvgObject *pObj);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ӻ��������б�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-22 10:18
	// ����˵��:  
	// �� �� ֵ:  
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
	bool m_bDisableAttrEdit;//�������Ա༭����¼���ˢ������ʱ���ã���ֵֹ��д��
	QTabWidget *m_pTabWidget;//��ǰ���ڶ�Ӧ��TAB�ؼ�
	int m_iTabIndex;//��ǰ���ڶ�Ӧ��TAB���
	SSvgPointsEditDlg *m_pPointsEditDlg;
};


#endif //__SKT_SVG_ATTRIBUTE_H__
