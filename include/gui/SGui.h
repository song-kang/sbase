/**
 *
 * �� �� �� : SGui.h
 * �������� : 2007-12-20 14:25
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : ����GUI����
 * �޸ļ�¼ : 
 *            $Log: $
 *
 **/

#ifndef __SKT_GUI_INC_H__
#define __SKT_GUI_INC_H__

#ifdef QT_GUI_LIB
	//qt
	#include "qt/SWnd.h"
	#include "qt/SDC.h"

#else

	#ifdef WIN32

	#include "win32/SWnd.h"
	#include "win32/SDC.h"
	#include "win32/SListCtrl.h"
	#include "win32/SComboBox.h"
	#include "win32/SEdit.h"
	#include "win32/SCheckBox.h"
	#include "win32/SRadioBox.h"

	#else

	#include "linux/SWnd.h"
	#include "linux/SDC.h"
	#include "linux/SListCtrl.h"
	#include "linux/SComboBox.h"
	#include "linux/SEdit.h"
	#include "linux/SCheckBox.h"
	#include "linux/SRadioBox.h"

	#endif

#endif

#endif//__YS_GUI_INC_H__
