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

#include "SQt.h"

SQt::SQt()
{

}

SQt::~SQt()
{

}




////////////////////////////////////////////////////////////////////////
// 描    述:  导出表格控件中的内容
// 作    者:  邵凯田
// 创建时间:  2015-3-31 14:20
// 参数说明:  @pTable为表格控件
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SQt::TableExort(SString title,QTableWidget *pTable)
{
	QString fileName;
#if 0//ndef WIN32
	SString sMountDir = SApi::MountUsbDisk();
	if(sMountDir.length() == 0)
	{
		QMessageBox::information(NULL,QObject::tr("提示"),QObject::tr("未检测到U盘!\r\n请先插入U盘!"));
		return;
	}
	sMountDir += SDateTime::currentDateTime().toString("Export_yyyy-MM-dd_hh-mm-ss.csv");
	fileName = sMountDir.data();// + G2U(title) + ".txt";
#else
	fileName = QFileDialog::getSaveFileName(NULL,SString::toFormat("%s.csv",title.data()).data(), title.data(), ("CSV files (*.csv);;Text files (*.txt)"));
	if(fileName.length() == 0)
		return;
#endif
#ifndef FILESYS_UTF8
	SString str = fileName.toStdString().data();
#else
	SString str = fileName.toUtf8().data();
#endif
	SFile file2(str);
	if(!file2.open(IO_Truncate))
	{
#if 0//ndef WIN32
		SApi::UmountUsbDisk();
#endif
		printf("create csv file error!errno=%d, file:%s\n",errno,str.data());
		QMessageBox::information(NULL,QObject::tr("错误"),QObject::tr("导出数据失败!\r\n目标文件")+fileName);
		return;
	}
	SString sText = "            ";
	sText += title+"\r\n";
	if(str.right(4) == ".txt")
		TableToText(pTable,sText);
	else
		TableToCSVText(pTable,sText);

	file2.writeBlock((BYTE*)sText.data(),sText.length());
	file2.close();
#if 0//ndef WIN32
	SApi::UmountUsbDisk();
	QMessageBox::information(NULL,QObject::tr("提示"),QObject::tr("数据成功保存到U盘!"));
#else
	QMessageBox::information(NULL,QObject::tr("提示"),QObject::tr("数据保存成功!"));
#endif
}

////////////////////////////////////////////////////////////////////////
// 描    述:  打印表格控件中的内容
// 作    者:  邵凯田
// 创建时间:  2015-3-31 14:20
// 参数说明:  @pTable为表格控件
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SQt::TablePrint(SString title,QTableWidget *pTable)
{
	SString sText = "            ";
	sText += title+"\r\n";
	TableToText(pTable,sText);
	PrintText(sText);
}


////////////////////////////////////////////////////////////////////////
// 描    述:  将表格控件转换为文本
// 作    者:  邵凯田
// 创建时间:  2015-3-31 14:24
// 参数说明:  @pTable为表格控件
//         :  @sText为输出文本
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SQt::TableToText(QTableWidget *pTable,SString &sText)
{
	int row,rows = pTable->rowCount();
	int col,cols = pTable->columnCount();
	if(cols <= 0)
		return;
	SString text;
	int len;
	//可能有的表格设置了行隐藏
	int rows2 = 0;
	for(row=0;row<rows;row++)
	{
		if(!pTable->isRowHidden(row))
			rows2++;
	}
	
	int *col_len = new int[cols];
	for(col=0;col<cols;col++)
	{
		if(pTable->horizontalHeaderItem(col) == NULL)
			col_len[col] = 0;
		else
			col_len[col] = pTable->horizontalHeaderItem(col)->text().toStdString().length();
	}
	//先扫描全部数据，确定各列数据长度
	QTableWidgetItem *pItem;
	for(row=0;row<rows;row++)
	{
		if(pTable->isRowHidden(row))
			continue;
		for(col=0;col<cols;col++)
		{
			pItem = pTable->item(row,col);
			if(pItem != NULL)
				len = pItem->text().toStdString().length();
			else
				len = 0;
			if(len > col_len[col])
				col_len[col] = len;
		}
	}

	//生成表头
	if(pTable->horizontalHeaderItem(0) != NULL)
	{
		for(col=0;col<cols;col++)
		{
			if(pTable->horizontalHeaderItem(col) != NULL)
				sText += SString::toFillString(pTable->horizontalHeaderItem(col)->text().toStdString().data(),col_len[col],' ',false);
			else
				sText += SString::toFillString("",col_len[col],' ',false);
			sText += " ";
		}
		sText += "\r\n";
		for(col=0;col<cols;col++)
		{
			sText += SString::toFillString("-",col_len[col],'-',false);
			sText += " ";
		}
		sText += "\r\n";
	}

	for(row=0;row<rows;row++)
	{
		if(pTable->isRowHidden(row))
			continue;
		for(col=0;col<cols;col++)
		{
			pItem = pTable->item(row,col);
			if(pItem != NULL)
				sText += SString::toFillString(pItem->text().toStdString().data(),col_len[col],' ',false);
			sText += " ";
		}
		sText += "\r\n";
	}
	delete[] col_len;
}


////////////////////////////////////////////////////////////////////////
// 描    述:  将表格控件转换为CSV文本
// 作    者:  邵凯田
// 创建时间:  2015-3-31 14:24
// 参数说明:  @pTable为表格控件
//         :  @sText为输出文本
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SQt::TableToCSVText(QTableWidget *pTable,SString &sText)
{
	int row,rows = pTable->rowCount();
	int col,cols = pTable->columnCount();
	if(cols <= 0)
		return;
	//可能有的表格设置了行隐藏
	int rows2 = 0;
	for(row=0;row<rows;row++)
	{
		if(!pTable->isRowHidden(row))
			rows2++;
	}

	//生成表头
	if(pTable->horizontalHeaderItem(0) != NULL)
	{
		for(col=0;col<cols;col++)
		{
			if(pTable->horizontalHeaderItem(col) != NULL)
				sText += pTable->horizontalHeaderItem(col)->text().toStdString().data();
			sText += ",";
		}
		sText += "\r\n";
	}
	QTableWidgetItem *pItem;
	for(row=0;row<rows;row++)
	{
		if(pTable->isRowHidden(row))
			continue;
		for(col=0;col<cols;col++)
		{
			pItem = pTable->item(row,col);
			if(pItem != NULL)
				sText += pItem->text().toStdString().data();
			sText += ",";
		}
		sText += "\r\n";
	}

}

////////////////////////////////////////////////////////////////////////
// 描    述:  导出记录集中的内容
// 作    者:  邵凯田
// 创建时间:  2013:9:16 16:20
// 参数说明:  @rs为记录集
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SQt::RecordsetExort(SString title,SRecordset &rs)
{
	QString fileName;
#if 0//ndef WIN32
	SString sMountDir = SApi::MountUsbDisk();
	if(sMountDir.length() == 0)
	{
		QMessageBox::information(NULL,QObject::tr("提示"),QObject::tr("未检测到U盘!\r\n请先插入U盘!"));
		return;
	}
	sMountDir += SDateTime::currentDateTime().toString("Export_yyyy-MM-dd_hh-mm-ss.csv");
	fileName = sMountDir.data();// + G2U(title) + ".txt";
#else
	fileName = QFileDialog::getSaveFileName(NULL,SString::toFormat("%s.csv",title.data()).data(), title.data(), ("CSV files (*.csv);;Text files (*.txt)"));
	if(fileName.length() == 0)
		return;
#endif
#ifndef FILESYS_UTF8
	SString str = fileName.toStdString().data();
#else
	SString str = fileName.toUtf8().data();
#endif
	SFile file2(str);
	if(!file2.open(IO_Truncate))
	{
#if 0//ndef WIN32
		SApi::UmountUsbDisk();
#endif
		printf("create csv file error! file:%s\n",str.data());
		QMessageBox::information(NULL,QObject::tr("错误"),QObject::tr("导出数据失败!\r\n目标文件")+fileName);
		return;
	}
	SString sText = "            ";
	sText += title+"\r\n";
	if(str.right(4) == ".txt")
		RecordsetToText(rs,sText);
	else
		RecordsetToCSVText(rs,sText);

	file2.writeBlock((BYTE*)sText.data(),sText.length());
	file2.close();
#if 0//ndef WIN32
	SApi::UmountUsbDisk();
	QMessageBox::information(NULL,"提示","数据成功保存到U盘!");
#else
	QMessageBox::information(NULL,"提示","数据保存成功!");
#endif

}

////////////////////////////////////////////////////////////////////////
// 描    述:  打印记录集中的内容
// 作    者:  邵凯田
// 创建时间:  2013:9:16 16:20
// 参数说明:  @rs为记录集
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SQt::RecordsetPrint(SString title,SRecordset &rs)
{
	SString sText = "            ";
	sText += title+"\r\n";
	RecordsetToText(rs,sText);
	PrintText(sText);

}

////////////////////////////////////////////////////////////////////////
// 描    述:  将记录集转换为对齐文本
// 作    者:  邵凯田
// 创建时间:  2015-3-31 14:24
// 参数说明:  @rs为记录集
//         :  @sText为输出文本
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SQt::RecordsetToText(SRecordset &rs,SString &sText)
{
	int row,rows = rs.GetRows();
	int col,cols = rs.GetColumns();
	if(cols <= 0)
		return;
	SString text;
	int len;
	int *col_len = new int[cols];
	for(col=0;col<cols;col++)
	{
		col_len[col] = rs.GetColumnName(col).length();
	}
	//先扫描全部数据，确定各列数据长度
	for(row=0;row<rows;row++)
	{
		for(col=0;col<cols;col++)
		{
			len = rs.GetValue(row,col).length();
			if(len > col_len[col])
				col_len[col] = len;
		}
	}

	//生成表头
	for(col=0;col<cols;col++)
	{
		sText += SString::toFillString(rs.GetColumnName(col),col_len[col],' ',false);
		sText += " ";
	}
	sText += "\r\n";
	for(col=0;col<cols;col++)
	{
		sText += SString::toFillString("-",col_len[col],'-',false);
		sText += " ";
	}
	sText += "\r\n";

	for(row=0;row<rows;row++)
	{
		for(col=0;col<cols;col++)
		{
			sText += SString::toFillString(rs.GetValue(row,col),col_len[col],' ',false);
			sText += " ";
		}
		sText += "\r\n";
	}
	for(col=0;col<cols;col++)
	{
		sText += SString::toFillString("-",col_len[col],'-',false);
		sText += " ";
	}
	sText += "\r\n";
	sText += SString::toFormat("合计:%d\r\n",rows);
	delete[] col_len;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  将记录集转换为CSV文本
// 作    者:  邵凯田
// 创建时间:  2015-3-31 14:24
// 参数说明:  @rs为记录集
//         :  @sText为输出文本
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SQt::RecordsetToCSVText(SRecordset &rs,SString &sText)
{
	int row,rows = rs.GetRows();
	int col,cols = rs.GetColumns();
	if(cols <= 0)
		return;
	SString text;

	//生成表头
	for(col=0;col<cols;col++)
	{
		sText += rs.GetColumnName(col);
		sText += ",";
	}
	sText += "\r\n";

	for(row=0;row<rows;row++)
	{
		for(col=0;col<cols;col++)
		{
			sText += rs.GetValue(row,col);
			sText += ",";
		}
		sText += "\r\n";
	}

}

#ifdef ESC_PRINT
#include "print/SEscPrintUsb.h"
#endif

////////////////////////////////////////////////////////////////////////
// 描    述:  打印指定的文本
// 作    者:  邵凯田
// 创建时间:  2015-3-31 14:53
// 参数说明:  
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SQt::PrintText(SString &sText)
{
#ifdef ESC_PRINT
	SEscPrintUsb print;
	print.Init("");
	if(print.Open("") == false)
	{
		QMessageBox::information(NULL,QObject::tr("错误"),QObject::tr("未发现支持型号的针式打印机!"));
		return;
	}

	SString sLine;
	int i,cnt = SString::GetAttributeCount(sText,"\n");
	QProgressDialog progress(QObject::tr("正在打印..."), QObject::tr("取消(&C)"), 0, cnt, NULL);
	progress.setWindowModality(Qt::ApplicationModal);
	progress.setRange(0,cnt);
	bool bBreak=false;
	for(i=1;i<=cnt;i++)
	{
		sLine = SString::GetIdAttribute(i,sText,"\n");
		print.PrintText(sLine);
		print.PrintLF();
		progress.setValue(i);
		for(int j=0;j<100;j++)
		{
			qApp->processEvents();
			if (progress.wasCanceled())
			{
				bBreak = true;
				break;
			}
			SApi::UsSleep(3000);
		}
		if(bBreak)
			break;
	}
	//走纸
	for(i=0;i<20;i++)
	{
		print.PrintLF();
	}
	print.Beep(3,500);
	print.Close();
	progress.close();
#else
	//	QMessageBox::information(NULL,QObject::tr("错误"),QObject::tr("此版本不支持该信息打印!"));
	QPrinter printer(QPrinter::HighResolution);
	//printer.setOrientation(QPrinter::Landscape);//设为横向

	QPrintDialog dlg(&printer,NULL);
	if(dlg.exec() != QDialog::Accepted)
		return;

	QProgressDialog progress(QObject::tr("正在生成打印内容..."), QObject::tr("取消(&C)"),
		printer.fromPage(), printer.toPage(), NULL);
	progress.setWindowModality(Qt::ApplicationModal);
	progress.setWindowTitle(QObject::tr("打印"));
	QPainter painter;
	painter.begin(&printer);

	QRect pageRect = printer.pageRect();//QPrinter.Millimeter //页面大小
	QFont font(QObject::tr("宋体"),10,0);
	painter.setFont(font);
	font = QFont(font,painter.device());
	QFontMetricsF fontMetrics(font);
	QRectF rect = fontMetrics.boundingRect("邵凯田");
	int iTextH = (int)(rect.height()*1.2);//文本的高度
	//#ifdef WIN32
	iTextH = pageRect.width()/45;
	//#endif
	//float tscale = iTextH/18;
	int pageMarage = iTextH*2;
	int pageTop = iTextH*2;
	int pageH = pageRect.height()-pageMarage*2;//-pageTop;
	//int pageW = pageRect.width()-pageMarage*2;
	SString sLine;

	int iTextLines = SString::GetAttributeCount(sText,"\n")+1;//文本报告的行数
	int iTextLinesOfPage = pageH / iTextH;//一页可以显示的最大文本行数
	//int iTGSpaceH = iTextH * 3;//文本与图形间的间隔大小，当文本结束时未到达高度一半时为三行文本高（图形接在这一页），否则图形另起一页
// 	bool bGraphicNewPage=false;//图形模式接在文本后的同一页绘制
// 	//int iTextHInGraphic = ((iTextLines * iTextH) % pageH);//图形页面中文本占用的高度
// 	if(((iTextLines * iTextH) % pageH) < iTextH/2 ) 
// 	{
// 		//iTGSpaceH = iTextH - ((iTextLines * iTextH) % pageH);//为文本页余下的高度，使图形页从下一页开始
// 		bGraphicNewPage = true;//图形模式在新一页中绘制
// 	}
	int textPages = (iTextLines * iTextH)%pageH==0?(iTextLines * iTextH)/pageH:((iTextLines * iTextH)/pageH+1);//文本页数
// 	int graghicPageFrom;
// 	if(bGraphicNewPage)
// 		graghicPageFrom = textPages+1;
// 	else
// 		graghicPageFrom = textPages;
	int pages = (iTextLines)*iTextH;
	if(pages % pageH < 5)
		pages /= pageH;
	else
		pages = (pages/pageH) + 1;

	int from = printer.fromPage();
	int to = printer.toPage();
	if(from <= 0 && to <= 0)
	{
		from = 1;
		to = pages;
	}
	if(from <= 0)
		from = 1;
	if(to <= 0)
		to = 1;
	if(from > pages)
		from = pages;
	if(to > pages)
		to = pages;
	printer.setFromTo(from,to);
	progress.setMinimum(from);
	progress.setMaximum(to);
	bool firstPage = true;
	for (int page = from; page <= to; ++page) 
	{
		if (!firstPage)
			printer.newPage();

		qApp->processEvents();
		if (progress.wasCanceled())
			break;

		float y=pageMarage+pageTop;

		//绘制文本
		if(page <= textPages)
		{
			//有文本需要打印
			int froml = iTextLinesOfPage*(page-1)+1;
			int tol = froml + iTextLinesOfPage;
			if(tol > iTextLines)
				tol = iTextLines;
			for(int line=froml;line < tol ;line++)
			{
				sLine = SString::GetIdAttribute(line,sText,"\n");
				painter.drawText(pageMarage,y,sLine.data());
				y += iTextH;
			}
		}
		progress.setValue(page);
		firstPage = false;
	}

	progress.close();
	painter.end();
#endif
}

////////////////////////////////////////////////////////////////////////
// 描    述:  取表格控件的选中行号
// 作    者:  邵凯田
// 创建时间:  2015-03-31 11:52
// 参数说明:  @pTable为表格
//         :  @rows为行号队列
// 返 回 值:  选中行数
//////////////////////////////////////////////////////////////////////////
int SQt::GetTableSelectedRows(QTableWidget *pTable, SValueList<int> &rows)
{
	QList<QTableWidgetItem*> sels= pTable->selectedItems();
	if(sels.count() <= 0)
	{
		return 0;
	}
	int r;
	int i,cnt = sels.count();
	for(i=0;i<cnt;i++)
	{
		QTableWidgetItem *pItem = sels.at(i);
		if(pItem == NULL)
			continue;
		r = pItem->row();
		bool bFind = false;
		for(int j=0;j<(int)rows.count();j++)
		{
			if(*rows[j] == r)
			{
				bFind=true;
				break;
			}
		}
		if(!bFind)
			rows.append(r);
	}
	return rows.count();
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  显示提示窗口
// 作    者:  邵凯田
// 创建时间:  2015-7-31 11:20
// 参数说明:  @tip为窗口标题
//         :  @text为提示内容
//         :  @parent为父窗口，缺省为NULL
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SQt::ShowInfoBox(QString tip,QString text,QWidget *parent)
{
	QMessageBox::information(parent,tip,text);
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  显示错误窗口
// 作    者:  邵凯田
// 创建时间:  2015-7-31 11:20
// 参数说明:  @tip为窗口标题
//         :  @text为提示内容
//         :  @parent为父窗口，缺省为NULL
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SQt::ShowErrorBox(QString tip,QString text,QWidget *parent)
{
	QMessageBox::warning(parent,tip,text);
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  显示问题窗口
// 作    者:  邵凯田
// 创建时间:  2015-7-31 11:20
// 参数说明:  @tip为窗口标题
//         :  @text为提示内容
//         :  @parent为父窗口，缺省为NULL
// 返 回 值:  true/false
//////////////////////////////////////////////////////////////////////////
bool SQt::ShowQuestionBox(QString tip,QString text,QWidget *parent)
{
	return QMessageBox::question(parent,tip,text,QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  设置QTableWidget为统一的显示风格
// 作    者:  邵凯田
// 创建时间:  2015-7-31 15:56
// 参数说明:  @pTableWidget为控件指针
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SQt::SetTableWidgetStyle(QTableWidget *pTableWidget)
{
	//表格样式
	//pTableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{border:2px groove gray;border-radius:10px;padding:2px 4px;background-color: rgb(100, 179, 238);}");
	//pTableWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(220, 230, 240);alternate-background-color: rgb(188, 206, 220);selection-background-color: rgb(96,134,185);"));
	//MEMO: 改为国网运检测试 [2018-9-12 16:42 邵凯田] pTableWidget->setStyleSheet(QString::fromUtf8("alternate-background-color: rgb(220, 230, 240);selection-background-color: rgb(96,134,185);"));//background-color: rgb(255,255,255);
	//pTableWidget->setStyleSheet(QString::fromUtf8("alternate-background-color: rgb(43, 48, 91);selection-background-color: rgb(96,134,185);"));//background-color: rgb(255,255,255);
	pTableWidget->setAlternatingRowColors(true);
	pTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	//pTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

	pTableWidget->verticalHeader()->setDefaultSectionSize(22);
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  采用QString为格式化串进行格式化字符串操作并返回结果串,一般与tr配合使用
// 作    者:  邵凯田
// 创建时间:  2015-8-3 16:58
// 参数说明:  @qformat
// 返 回 值:  QString
//////////////////////////////////////////////////////////////////////////
QString SQt::qsprintf(QString qformat, ...)
{
	char buf[10240];
	memset(buf,0,10240);
	va_list ap;
	va_start(ap, qformat);
	vsprintf(buf,qformat.toStdString().data(),ap);
	va_end(ap);
	QString str = buf;
	return str;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  处理人机界面中的未处理事情，如刷新界面等
// 作    者:  邵凯田
// 创建时间:  2015-8-5 15:33
// 参数说明:  void
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SQt::ProcessUiEvents()
{
	qApp->processEvents();
}

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
SString SQt::GetInput(QString title,QString tip,const char* defval/*=NULL*/,QWidget *parent/*=NULL*/)
{
	return QInputDialog::getText(parent,title,tip,QLineEdit::Normal,defval==NULL?"":defval).toStdString().data();
}

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
SString SQt::GetPasswordInput(QString title,QString tip,char* defval/*=NULL*/,QWidget *parent/*=NULL*/)
{
	return QInputDialog::getText(parent,title,tip,QLineEdit::Password,defval==NULL?"":defval).toStdString().data();
}
