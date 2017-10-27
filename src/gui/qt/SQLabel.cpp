/**
 *
 * 文 件 名 : SQLabel.cpp
 * 创建日期 : 2015-8-4 9:53
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

#include "SQLabel.h"
#include <QMouseEvent>
#include <QPainter>
#include <QPalette>

SQLabel::SQLabel(QString str,QWidget *parent,bool bUnderLine) :QLabel(parent)
{
	QPalette palette;
	palette.setColor(QPalette::WindowText,QColor(50,255,255));
	this->setText(str);
	this->setPalette(palette);
	setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
	//  this->setStyleSheet("background-color:blue");
	if(bUnderLine)
	{
		QFont font;
		font.setUnderline(true);
		setFont(font);
	}
	this->setCursor(Qt::PointingHandCursor);
	setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
	m_bPress = false;
	m_bOver = false;
	this->setMouseTracking(true);
}

void SQLabel::mousePressEvent(QMouseEvent *event)
{
	//如果单击了就触发clicked信号
	if (event->button() == Qt::LeftButton)
	{
		//触发clicked信号
		m_bPress = true;
		emit clicked();
	}
	//将该事件传给父类处理
	QLabel::mousePressEvent(event);
}

void SQLabel:: mouseReleaseEvent(QMouseEvent *event)
{
	S_UNUSED(event);
	m_bPress = false;
	update();
}

void SQLabel::enterEvent(QEvent *event)
{
	S_UNUSED(event);
	m_bOver =true;
	setStyleSheet(QString::fromUtf8("color: rgb(85, 0, 255);"));
	update();
}

void SQLabel::leaveEvent(QEvent *event)
{
	S_UNUSED(event);
	m_bOver = false;
	setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
	update();
}

void SQLabel::paintEvent(QPaintEvent *event)
{
	QLabel::paintEvent(event);
}
