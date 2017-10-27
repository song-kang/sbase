/**
 *
 * 文 件 名 : SQLabel.h
 * 创建日期 : 2015-8-4 9:51
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : 基于QLabel的按钮功能实现类，文字随鼠标移进移出改颜色
 * 修改记录 : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-8-4	邵凯田　创建文件
 *
 **/
#ifndef __SKT_BTN_QLABEL_H__
#define __SKT_BTN_QLABEL_H__

#include <QLabel>
#include "SInclude.h"
class SBASE_QTGUI_EXPORT SQLabel :public QLabel
{
	Q_OBJECT
public:
	/*explicit */SQLabel(QString str="",QWidget *parent = 0,bool bUnderLine=true);
	virtual void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void enterEvent(QEvent *);
	void leaveEvent(QEvent *);
	void paintEvent(QPaintEvent *event);

signals:
	//自定义clicked()信号,在mousePressEvent事件发生时触发
	void clicked();

public slots:

private:
	bool m_bOver;
	bool m_bPress;
};

#endif//__SKT_BTN_QLABEL_H__
