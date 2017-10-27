/**
 *
 * �� �� �� : SQLabel.h
 * �������� : 2015-8-4 9:51
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
	//�Զ���clicked()�ź�,��mousePressEvent�¼�����ʱ����
	void clicked();

public slots:

private:
	bool m_bOver;
	bool m_bPress;
};

#endif//__SKT_BTN_QLABEL_H__
