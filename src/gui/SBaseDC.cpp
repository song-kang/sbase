/**
 *
 * �� �� �� : SBaseDC.cpp
 * �������� : 2008-01-17 14:25
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : ͨ�õĴ�����ӿڣ��������׼�Ĵ��ڲ�����������ʵ��
 * �޸ļ�¼ : 
 *            $Log: $
 *
 **/
#include "SBaseDC.h"

SCOLOR SRGB(BYTE r,BYTE g,BYTE b,BYTE a/*=0*/)
{
	SCOLOR color;
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;
	return color;
};
