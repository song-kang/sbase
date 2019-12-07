/**
 *
 * �� �� �� : SQLabel.cpp
 * �������� : 2015-8-4 9:53
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : ����QLabel�İ�ť����ʵ���࣬����������ƽ��Ƴ�����ɫ
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-8-4	�ۿ�������ļ�
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
	//��������˾ʹ���clicked�ź�
	if (event->button() == Qt::LeftButton)
	{
		//����clicked�ź�
		m_bPress = true;
		emit clicked();
	}
	//�����¼��������ദ��
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
