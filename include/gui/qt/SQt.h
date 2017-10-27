/**
 *
 * 文 件 名 : SQt.h
 * 创建日期 : 2015-3-31 9:52
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : QT常用界面常用功能封装
 * 修改记录 : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-3-31	邵凯田　创建文件
 *
 **/

#ifndef __SKT_QT_BASE_H__
#define __SKT_QT_BASE_H__

#ifndef QT_GUI_LIB
#error "You cann't use qt svg code under non-qt plateform!!!"
#endif

#pragma once
#include <QObject>
#include <QtGui>
#include "SString.h"
#include "SFile.h"
#include "db/SDatabase.h"
#include "SApi.h"
#include "SQLabel.h"
#ifdef QT_V5
#include <QtWidgets>
#include <QtPrintSupport>
#endif

//QTableWidget的行高
#define C_TABLE_ROW_HEIGHT 22

//GBK与UNICODE字符编码互转
#define G2U(s) QTextCodec::codecForName("GBK")->toUnicode(s)
#define U2G(s) QTextCodec::codecForName("GBK")->fromUnicode(s)

//QString与SString互转
#define Q2S(str) SString(str.toStdString().data())
#define S2Q(str) QString(str.data())

//QString/SString转char*
#define Q2C(str) str.toStdString().data()
#define S2C(str) str.data()


class SBASE_QTGUI_EXPORT SQt
{
public:
	SQt();
	~SQt();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  导出表格控件中的内容
	// 作    者:  邵凯田
	// 创建时间:  2015-03-31 14:20
	// 参数说明:  @pTable为表格控件
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	static void TableExort(SString title,QTableWidget *pTable);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  打印表格控件中的内容
	// 作    者:  邵凯田
	// 创建时间:  2015-03-31 14:20
	// 参数说明:  @pTable为表格控件
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	static void TablePrint(SString title,QTableWidget *pTable);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  将表格控件转换为文本
	// 作    者:  邵凯田
	// 创建时间:  2015-03-31 14:24
	// 参数说明:  @pTable为表格控件
	//         :  @sText为输出文本
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	static void TableToText(QTableWidget *pTable,SString &sText);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  将表格控件转换为CSV文本
	// 作    者:  邵凯田
	// 创建时间:  2015-03-31 14:24
	// 参数说明:  @pTable为表格控件
	//         :  @sText为输出文本
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	static void TableToCSVText(QTableWidget *pTable,SString &sText);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  导出记录集中的内容
	// 作    者:  邵凯田
	// 创建时间:  2013:9:16 16:20
	// 参数说明:  @rs为记录集
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	static void RecordsetExort(SString title,SRecordset &rs);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  打印记录集中的内容
	// 作    者:  邵凯田
	// 创建时间:  2013:9:16 16:20
	// 参数说明:  @rs为记录集
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	static void RecordsetPrint(SString title,SRecordset &rs);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  将记录集转换为对齐文本
	// 作    者:  邵凯田
	// 创建时间:  2015-03-31 14:24
	// 参数说明:  @rs为记录集
	//         :  @sText为输出文本
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	static void RecordsetToText(SRecordset &rs,SString &sText);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  将记录集转换为CSV文本
	// 作    者:  邵凯田
	// 创建时间:  2015-03-31 14:24
	// 参数说明:  @rs为记录集
	//         :  @sText为输出文本
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	static void RecordsetToCSVText(SRecordset &rs,SString &sText);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  打印指定的文本
	// 作    者:  邵凯田
	// 创建时间:  2015-03-31 14:53
	// 参数说明:  @sText为待打印的文件内容，\n表示换行
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	static void PrintText(SString &sText);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取表格控件的选中行号
	// 作    者:  邵凯田
	// 创建时间:  2015-03-31 11:52
	// 参数说明:  @pTable为表格
	//         :  @rows为行号队列
	// 返 回 值:  选中行数
	//////////////////////////////////////////////////////////////////////////
	static int GetTableSelectedRows(QTableWidget *pTable, SValueList<int> &rows);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  显示提示窗口
	// 作    者:  邵凯田
	// 创建时间:  2015-7-31 11:20
	// 参数说明:  @tip为窗口标题
	//         :  @text为提示内容
	//         :  @parent为父窗口，缺省为NULL
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	static void ShowInfoBox(QString tip,QString text,QWidget *parent=NULL);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  显示错误窗口
	// 作    者:  邵凯田
	// 创建时间:  2015-7-31 11:20
	// 参数说明:  @tip为窗口标题
	//         :  @text为提示内容
	//         :  @parent为父窗口，缺省为NULL
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	static void ShowErrorBox(QString tip,QString text,QWidget *parent=NULL);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  显示问题窗口
	// 作    者:  邵凯田
	// 创建时间:  2015-7-31 11:20
	// 参数说明:  @tip为窗口标题
	//         :  @text为提示内容
	//         :  @parent为父窗口，缺省为NULL
	// 返 回 值:  true/false
	//////////////////////////////////////////////////////////////////////////
	static bool ShowQuestionBox(QString tip,QString text,QWidget *parent=NULL);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  设置QTableWidget为统一的显示风格
	// 作    者:  邵凯田
	// 创建时间:  2015-7-31 15:56
	// 参数说明:  @pTableWidget为控件指针
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	static void SetTableWidgetStyle(QTableWidget *pTableWidget);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  采用QString为格式化串进行格式化字符串操作并返回结果串,一般与tr配合使用
	// 作    者:  邵凯田
	// 创建时间:  2015-8-3 16:58
	// 参数说明:  @qformat
	// 返 回 值:  QString
	//////////////////////////////////////////////////////////////////////////
	static QString qsprintf(QString qformat, ...);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  处理人机界面中的未处理事情，如刷新界面等
	// 作    者:  邵凯田
	// 创建时间:  2015-8-5 15:33
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	static void ProcessUiEvents();

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  通过提示框输入文本
	// 作    者:  邵凯田
	// 创建时间:  2015-8-13 14:46
	// 参数说明:  @title为窗口标题
	//         :  @tip为提示内容
	//         :  @defval为缺省输入内容
	//         :  @parent为父窗口
	// 返 回 值:  SString
	//////////////////////////////////////////////////////////////////////////
	static SString GetInput(QString title,QString tip,const char* defval=NULL,QWidget *parent=NULL);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  通过提示框输入密码，输入过程中密码不直接显示在屏幕
	// 作    者:  邵凯田
	// 创建时间:  2015-8-13 14:56
	// 参数说明:  @title为窗口标题
	//         :  @tip为提示内容
	//         :  @defval为缺省输入内容
	//         :  @parent为父窗口
	// 返 回 值:  SString
	//////////////////////////////////////////////////////////////////////////
	static SString GetPasswordInput(QString title,QString tip,char* defval=NULL,QWidget *parent=NULL);
};


#endif //__SKT_QT_BASE_H__

