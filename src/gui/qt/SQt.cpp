/**
 *
 * �� �� �� : SQt.h
 * �������� : 2015-3-31 9:52
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : QT���ý��泣�ù��ܷ�װ
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-3-31	�ۿ�������ļ�
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
// ��    ��:  �������ؼ��е�����
// ��    ��:  �ۿ���
// ����ʱ��:  2015-3-31 14:20
// ����˵��:  @pTableΪ���ؼ�
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SQt::TableExort(SString title,QTableWidget *pTable)
{
	QString fileName;
#if 0//ndef WIN32
	SString sMountDir = SApi::MountUsbDisk();
	if(sMountDir.length() == 0)
	{
		QMessageBox::information(NULL,QObject::tr("��ʾ"),QObject::tr("δ��⵽U��!\r\n���Ȳ���U��!"));
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
		QMessageBox::information(NULL,QObject::tr("����"),QObject::tr("��������ʧ��!\r\nĿ���ļ�")+fileName);
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
	QMessageBox::information(NULL,QObject::tr("��ʾ"),QObject::tr("���ݳɹ����浽U��!"));
#else
	QMessageBox::information(NULL,QObject::tr("��ʾ"),QObject::tr("���ݱ���ɹ�!"));
#endif
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ӡ���ؼ��е�����
// ��    ��:  �ۿ���
// ����ʱ��:  2015-3-31 14:20
// ����˵��:  @pTableΪ���ؼ�
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SQt::TablePrint(SString title,QTableWidget *pTable)
{
	SString sText = "            ";
	sText += title+"\r\n";
	TableToText(pTable,sText);
	PrintText(sText);
}


////////////////////////////////////////////////////////////////////////
// ��    ��:  �����ؼ�ת��Ϊ�ı�
// ��    ��:  �ۿ���
// ����ʱ��:  2015-3-31 14:24
// ����˵��:  @pTableΪ���ؼ�
//         :  @sTextΪ����ı�
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SQt::TableToText(QTableWidget *pTable,SString &sText)
{
	int row,rows = pTable->rowCount();
	int col,cols = pTable->columnCount();
	if(cols <= 0)
		return;
	SString text;
	int len;
	//�����еı��������������
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
	//��ɨ��ȫ�����ݣ�ȷ���������ݳ���
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

	//���ɱ�ͷ
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
// ��    ��:  �����ؼ�ת��ΪCSV�ı�
// ��    ��:  �ۿ���
// ����ʱ��:  2015-3-31 14:24
// ����˵��:  @pTableΪ���ؼ�
//         :  @sTextΪ����ı�
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SQt::TableToCSVText(QTableWidget *pTable,SString &sText)
{
	int row,rows = pTable->rowCount();
	int col,cols = pTable->columnCount();
	if(cols <= 0)
		return;
	//�����еı��������������
	int rows2 = 0;
	for(row=0;row<rows;row++)
	{
		if(!pTable->isRowHidden(row))
			rows2++;
	}

	//���ɱ�ͷ
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
// ��    ��:  ������¼���е�����
// ��    ��:  �ۿ���
// ����ʱ��:  2013:9:16 16:20
// ����˵��:  @rsΪ��¼��
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SQt::RecordsetExort(SString title,SRecordset &rs)
{
	QString fileName;
#if 0//ndef WIN32
	SString sMountDir = SApi::MountUsbDisk();
	if(sMountDir.length() == 0)
	{
		QMessageBox::information(NULL,QObject::tr("��ʾ"),QObject::tr("δ��⵽U��!\r\n���Ȳ���U��!"));
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
		QMessageBox::information(NULL,QObject::tr("����"),QObject::tr("��������ʧ��!\r\nĿ���ļ�")+fileName);
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
	QMessageBox::information(NULL,"��ʾ","���ݳɹ����浽U��!");
#else
	QMessageBox::information(NULL,"��ʾ","���ݱ���ɹ�!");
#endif

}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ӡ��¼���е�����
// ��    ��:  �ۿ���
// ����ʱ��:  2013:9:16 16:20
// ����˵��:  @rsΪ��¼��
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SQt::RecordsetPrint(SString title,SRecordset &rs)
{
	SString sText = "            ";
	sText += title+"\r\n";
	RecordsetToText(rs,sText);
	PrintText(sText);

}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ����¼��ת��Ϊ�����ı�
// ��    ��:  �ۿ���
// ����ʱ��:  2015-3-31 14:24
// ����˵��:  @rsΪ��¼��
//         :  @sTextΪ����ı�
// �� �� ֵ:  void
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
	//��ɨ��ȫ�����ݣ�ȷ���������ݳ���
	for(row=0;row<rows;row++)
	{
		for(col=0;col<cols;col++)
		{
			len = rs.GetValue(row,col).length();
			if(len > col_len[col])
				col_len[col] = len;
		}
	}

	//���ɱ�ͷ
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
	sText += SString::toFormat("�ϼ�:%d\r\n",rows);
	delete[] col_len;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ����¼��ת��ΪCSV�ı�
// ��    ��:  �ۿ���
// ����ʱ��:  2015-3-31 14:24
// ����˵��:  @rsΪ��¼��
//         :  @sTextΪ����ı�
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SQt::RecordsetToCSVText(SRecordset &rs,SString &sText)
{
	int row,rows = rs.GetRows();
	int col,cols = rs.GetColumns();
	if(cols <= 0)
		return;
	SString text;

	//���ɱ�ͷ
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
// ��    ��:  ��ӡָ�����ı�
// ��    ��:  �ۿ���
// ����ʱ��:  2015-3-31 14:53
// ����˵��:  
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SQt::PrintText(SString &sText)
{
#ifdef ESC_PRINT
	SEscPrintUsb print;
	print.Init("");
	if(print.Open("") == false)
	{
		QMessageBox::information(NULL,QObject::tr("����"),QObject::tr("δ����֧���ͺŵ���ʽ��ӡ��!"));
		return;
	}

	SString sLine;
	int i,cnt = SString::GetAttributeCount(sText,"\n");
	QProgressDialog progress(QObject::tr("���ڴ�ӡ..."), QObject::tr("ȡ��(&C)"), 0, cnt, NULL);
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
	//��ֽ
	for(i=0;i<20;i++)
	{
		print.PrintLF();
	}
	print.Beep(3,500);
	print.Close();
	progress.close();
#else
	//	QMessageBox::information(NULL,QObject::tr("����"),QObject::tr("�˰汾��֧�ָ���Ϣ��ӡ!"));
	QPrinter printer(QPrinter::HighResolution);
	//printer.setOrientation(QPrinter::Landscape);//��Ϊ����

	QPrintDialog dlg(&printer,NULL);
	if(dlg.exec() != QDialog::Accepted)
		return;

	QProgressDialog progress(QObject::tr("�������ɴ�ӡ����..."), QObject::tr("ȡ��(&C)"),
		printer.fromPage(), printer.toPage(), NULL);
	progress.setWindowModality(Qt::ApplicationModal);
	progress.setWindowTitle(QObject::tr("��ӡ"));
	QPainter painter;
	painter.begin(&printer);

	QRect pageRect = printer.pageRect();//QPrinter.Millimeter //ҳ���С
	QFont font(QObject::tr("����"),10,0);
	painter.setFont(font);
	font = QFont(font,painter.device());
	QFontMetricsF fontMetrics(font);
	QRectF rect = fontMetrics.boundingRect("�ۿ���");
	int iTextH = (int)(rect.height()*1.2);//�ı��ĸ߶�
	//#ifdef WIN32
	iTextH = pageRect.width()/45;
	//#endif
	//float tscale = iTextH/18;
	int pageMarage = iTextH*2;
	int pageTop = iTextH*2;
	int pageH = pageRect.height()-pageMarage*2;//-pageTop;
	//int pageW = pageRect.width()-pageMarage*2;
	SString sLine;

	int iTextLines = SString::GetAttributeCount(sText,"\n")+1;//�ı����������
	int iTextLinesOfPage = pageH / iTextH;//һҳ������ʾ������ı�����
	//int iTGSpaceH = iTextH * 3;//�ı���ͼ�μ�ļ����С�����ı�����ʱδ����߶�һ��ʱΪ�����ı��ߣ�ͼ�ν�����һҳ��������ͼ������һҳ
// 	bool bGraphicNewPage=false;//ͼ��ģʽ�����ı����ͬһҳ����
// 	//int iTextHInGraphic = ((iTextLines * iTextH) % pageH);//ͼ��ҳ�����ı�ռ�õĸ߶�
// 	if(((iTextLines * iTextH) % pageH) < iTextH/2 ) 
// 	{
// 		//iTGSpaceH = iTextH - ((iTextLines * iTextH) % pageH);//Ϊ�ı�ҳ���µĸ߶ȣ�ʹͼ��ҳ����һҳ��ʼ
// 		bGraphicNewPage = true;//ͼ��ģʽ����һҳ�л���
// 	}
	int textPages = (iTextLines * iTextH)%pageH==0?(iTextLines * iTextH)/pageH:((iTextLines * iTextH)/pageH+1);//�ı�ҳ��
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

		//�����ı�
		if(page <= textPages)
		{
			//���ı���Ҫ��ӡ
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
// ��    ��:  ȡ���ؼ���ѡ���к�
// ��    ��:  �ۿ���
// ����ʱ��:  2015-03-31 11:52
// ����˵��:  @pTableΪ���
//         :  @rowsΪ�кŶ���
// �� �� ֵ:  ѡ������
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
// ��    ��:  ��ʾ��ʾ����
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-31 11:20
// ����˵��:  @tipΪ���ڱ���
//         :  @textΪ��ʾ����
//         :  @parentΪ�����ڣ�ȱʡΪNULL
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SQt::ShowInfoBox(QString tip,QString text,QWidget *parent)
{
	QMessageBox::information(parent,tip,text);
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ʾ���󴰿�
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-31 11:20
// ����˵��:  @tipΪ���ڱ���
//         :  @textΪ��ʾ����
//         :  @parentΪ�����ڣ�ȱʡΪNULL
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SQt::ShowErrorBox(QString tip,QString text,QWidget *parent)
{
	QMessageBox::warning(parent,tip,text);
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ʾ���ⴰ��
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-31 11:20
// ����˵��:  @tipΪ���ڱ���
//         :  @textΪ��ʾ����
//         :  @parentΪ�����ڣ�ȱʡΪNULL
// �� �� ֵ:  true/false
//////////////////////////////////////////////////////////////////////////
bool SQt::ShowQuestionBox(QString tip,QString text,QWidget *parent)
{
	return QMessageBox::question(parent,tip,text,QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ����QTableWidgetΪͳһ����ʾ���
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-31 15:56
// ����˵��:  @pTableWidgetΪ�ؼ�ָ��
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SQt::SetTableWidgetStyle(QTableWidget *pTableWidget)
{
	//�����ʽ
	//pTableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{border:2px groove gray;border-radius:10px;padding:2px 4px;background-color: rgb(100, 179, 238);}");
	//pTableWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(220, 230, 240);alternate-background-color: rgb(188, 206, 220);selection-background-color: rgb(96,134,185);"));
	//MEMO: ��Ϊ�����˼���� [2018-9-12 16:42 �ۿ���] pTableWidget->setStyleSheet(QString::fromUtf8("alternate-background-color: rgb(220, 230, 240);selection-background-color: rgb(96,134,185);"));//background-color: rgb(255,255,255);
	//pTableWidget->setStyleSheet(QString::fromUtf8("alternate-background-color: rgb(43, 48, 91);selection-background-color: rgb(96,134,185);"));//background-color: rgb(255,255,255);
	pTableWidget->setAlternatingRowColors(true);
	pTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	//pTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

	pTableWidget->verticalHeader()->setDefaultSectionSize(22);
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ����QStringΪ��ʽ�������и�ʽ���ַ������������ؽ����,һ����tr���ʹ��
// ��    ��:  �ۿ���
// ����ʱ��:  2015-8-3 16:58
// ����˵��:  @qformat
// �� �� ֵ:  QString
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
// ��    ��:  �����˻������е�δ�������飬��ˢ�½����
// ��    ��:  �ۿ���
// ����ʱ��:  2015-8-5 15:33
// ����˵��:  void
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SQt::ProcessUiEvents()
{
	qApp->processEvents();
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ͨ����ʾ�������ı�
// ��    ��:  �ۿ���
// ����ʱ��:  2015-8-13 14:46
// ����˵��:  @titleΪ���ڱ���
//         :  @tipΪ��ʾ����
//         :  @defvalΪȱʡ��������
//         :  @parentΪ������
// �� �� ֵ:  SString
//////////////////////////////////////////////////////////////////////////
SString SQt::GetInput(QString title,QString tip,const char* defval/*=NULL*/,QWidget *parent/*=NULL*/)
{
	return QInputDialog::getText(parent,title,tip,QLineEdit::Normal,defval==NULL?"":defval).toStdString().data();
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ͨ����ʾ���������룬������������벻ֱ����ʾ����Ļ
// ��    ��:  �ۿ���
// ����ʱ��:  2015-8-13 14:56
// ����˵��:  @titleΪ���ڱ���
//         :  @tipΪ��ʾ����
//         :  @defvalΪȱʡ��������
//         :  @parentΪ������
// �� �� ֵ:  SString
//////////////////////////////////////////////////////////////////////////
SString SQt::GetPasswordInput(QString title,QString tip,char* defval/*=NULL*/,QWidget *parent/*=NULL*/)
{
	return QInputDialog::getText(parent,title,tip,QLineEdit::Password,defval==NULL?"":defval).toStdString().data();
}
