/**
 *
 * 文 件 名 : SSvgAttributeWnd.cpp
 * 创建日期 : 2015-8-20 14:37
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : SVG图元属性窗口
 * 修改记录 : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-8-20	邵凯田　创建文件
 *
 **/

#include "SSvgAttributeWnd.h"
#include "SSvgPainter.h"
#include "SSvgWnd.h"
#include "SSvgEditorWnd.h"

SSvgMultiEditDlg::SSvgMultiEditDlg(QWidget *parent) : QDialog(parent)
{
	this->resize(400, 300);
	gridLayout = new QGridLayout(this);
	verticalLayout = new QVBoxLayout();
	edtText = new QPlainTextEdit(this);
	edtText->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	verticalLayout->addWidget(edtText);
	horizontalLayout = new QHBoxLayout();
	horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	horizontalLayout->addItem(horizontalSpacer);
	btnOk = new QPushButton(this);
	btnOk->setText(tr("确定(&S)"));
	horizontalLayout->addWidget(btnOk);
	btnCancel = new QPushButton(this);
	btnCancel->setText(tr("取消(&C)"));
	horizontalLayout->addWidget(btnCancel);
	verticalLayout->addLayout(horizontalLayout);
	gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);
	connect(btnOk,SIGNAL(clicked()),this,SLOT(OnOk()));
	connect(btnCancel,SIGNAL(clicked()),this,SLOT(OnCancel()));
}

SSvgMultiEditDlg::~SSvgMultiEditDlg()
{

}

bool SSvgMultiEditDlg::ShowMultiInput(SString sTitle,SString &sText)
{
	setWindowTitle(sTitle.data());
	edtText->setPlainText(sText.data());
	if(exec() != QDialog::Accepted)
		return false;
	sText = edtText->toPlainText().toStdString().data();
	return true;
}

void SSvgMultiEditDlg::OnOk()
{
	accept();
	close();
}

void SSvgMultiEditDlg::OnCancel()
{
	reject();
	close();
}

SSvgPointsEditDlg::SSvgPointsEditDlg(QWidget *parent) : QDialog(parent)
{
	m_bInited = false;
	this->resize(300, 400);
	setWindowFlags( Qt::Dialog | Qt::Tool | Qt::WindowStaysOnTopHint /*| Qt::MSWindowsFixedSizeDialogHint*/);

	gridLayout = new QGridLayout(this);
	verticalLayout = new QVBoxLayout();
	tableWidget = new QTableWidget(this);
	verticalLayout->addWidget(tableWidget);
	horizontalLayout = new QHBoxLayout();
	horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	horizontalLayout->addItem(horizontalSpacer);
	btnAdd = new QPushButton(this);
	btnAdd->setText(tr("+"));
	btnAdd->setToolTip("在末尾添加点");
	horizontalLayout->addWidget(btnAdd);
	btnAdd->setMaximumWidth(30);
	btnInsert = new QPushButton(this);
	btnInsert->setToolTip("在当前位置前插入点");
	btnInsert->setText(tr("^"));
	horizontalLayout->addWidget(btnInsert);
	btnInsert->setMaximumWidth(30);
	btnDel = new QPushButton(this);
	btnDel->setToolTip("删除当前点");
	btnDel->setText(tr("-"));
	horizontalLayout->addWidget(btnDel);
	btnDel->setMaximumWidth(30);

	btnOk = new QPushButton(this);
	btnOk->setText(tr("确定(&S)"));
	horizontalLayout->addWidget(btnOk);
	btnCancel = new QPushButton(this);
	btnCancel->setText(tr("取消(&C)"));
	horizontalLayout->addWidget(btnCancel);
	verticalLayout->addLayout(horizontalLayout);
	gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


	SQt::SetTableWidgetStyle(tableWidget);
	tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: rgb(100, 179, 238);}");
	tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	tableWidget->setColumnCount(3);
	QStringList slTitle;
	slTitle << tr("X坐标") << tr("Y坐标") << tr("属性");
	tableWidget->setHorizontalHeaderLabels(slTitle);
	QHeaderView *headerView = tableWidget->horizontalHeader();
	headerView->resizeSection(0,100);
	headerView->resizeSection(1,100);
	headerView->resizeSection(2,60);
	tableWidget->setEditTriggers(QTableWidget::AllEditTriggers);//可编辑
	tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	tableWidget->setAutoScroll(false);
	tableWidget->verticalHeader()->setVisible(true);

	connect(tableWidget,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));
	connect(tableWidget,SIGNAL(currentCellChanged(int,int,int,int)),this,SLOT(onCurrentCellChanged(int,int,int,int)));

	connect(btnAdd,SIGNAL(clicked()),this,SLOT(OnAdd()));
	connect(btnInsert,SIGNAL(clicked()),this,SLOT(OnInsert()));
	connect(btnDel,SIGNAL(clicked()),this,SLOT(OnDel()));
	connect(btnOk,SIGNAL(clicked()),this,SLOT(OnOk()));
	connect(btnCancel,SIGNAL(clicked()),this,SLOT(OnCancel()));
}

SSvgPointsEditDlg::~SSvgPointsEditDlg()
{

}

void SSvgPointsEditDlg::onCurrentCellChanged ( int currentRow, int currentColumn, int previousRow, int previousColumn )
{
	S_UNUSED(currentColumn);
	S_UNUSED(previousRow);
	S_UNUSED(previousColumn);
	m_pSvgObject->SetAttributeValue("hover_idx",SString::toFormat("%d",currentRow));
	((SSvgWnd*)m_pSvgWnd)->Render();
	m_pSvgWnd->repaint();
}

void SSvgPointsEditDlg::GetPoints(SString &points,SString &extattr)
{
	points = "";
	extattr = "";
	int i,cnt = tableWidget->rowCount();
	QTableWidgetItem *p0,*p1,*p2;
	for(i=0;i<cnt;i++)
	{
		if(points.length() > 0)
			points += " ";
		if(extattr.length() > 0)
			extattr += ",";
		p0 = tableWidget->item(i,0);
		p1 = tableWidget->item(i,1);
		p2 = tableWidget->item(i,2);
		if(p0 == NULL || p1 == NULL || p2 == NULL)
			continue;
		points += SString::toFormat("%s,%s", 
			p0->text().toStdString().data(),
			p1->text().toStdString().data());
		extattr += p2->text().toStdString().data();
	}
}

void SSvgPointsEditDlg::on_tableWidget_itemChanged(QTableWidgetItem * item)
{
	S_UNUSED(item);
	if(!m_bInited)
		return;
	SString points,extattr;
	GetPoints(points,extattr);
	m_pSvgObject->SetAttributeValue("points",points,false,false);
	((SSvgWnd*)m_pSvgWnd)->Render();
	m_pSvgWnd->repaint();
}

bool SSvgPointsEditDlg::ShowEdit(SSvgObject *pObj,SString sText)
{
	m_bInited = false;
	tableWidget->setRowCount(0);
	m_sOldText = sText;
	m_pSvgObject = pObj;

	//p-attr=1,2,,,,3
	SString sExtAttr = pObj->GetAttribute("ext-attribute");
	if(sExtAttr.length() > 0)
		sExtAttr = SString::GetAttributeValue(sExtAttr,"p-attr");

 	setWindowTitle(tr("折线编辑窗口"));
	int i,cnt = SString::GetAttributeCount(m_sOldText," ");
	SString sLine,sX,sY,sAttr;
	int row = 0;
	for(i=1;i<=cnt;i++)
	{
		sLine = SString::GetIdAttribute(i,m_sOldText," ");
		if(sLine.length() == 0)
			continue;
		sX = SString::GetIdAttribute(1,sLine,",");
		sY = SString::GetIdAttribute(2,sLine,",");
		if(sExtAttr.length() > 0)
			sAttr = SString::GetIdAttribute(i,sExtAttr,",");
		row = tableWidget->rowCount();
		tableWidget->insertRow(row);
		tableWidget->setRowHeight(row,C_TABLE_ROW_HEIGHT);
		tableWidget->setItem(row,0,new QTableWidgetItem(sX.data()));
		tableWidget->setItem(row,1,new QTableWidgetItem(sY.data()));		
		tableWidget->setItem(row,2,new QTableWidgetItem(sAttr.data()));
	}
	m_bInited=true;
	return true;
}

void SSvgPointsEditDlg::OnAdd()
{
	int row = tableWidget->rowCount();
	tableWidget->insertRow(row);
	SString x="0",y="0",a="";
	if(row > 0)
	{
		x = tableWidget->item(row-1,0)->text().toStdString().data();
		y = tableWidget->item(row-1,1)->text().toStdString().data();
		a = tableWidget->item(row-1,2)->text().toStdString().data();
	}
	tableWidget->setItem(row,0,new QTableWidgetItem(x.data()));
	tableWidget->setItem(row,1,new QTableWidgetItem(y.data()));
	tableWidget->setItem(row,2,new QTableWidgetItem(a.data()));
	on_tableWidget_itemChanged(NULL);
}

void SSvgPointsEditDlg::OnInsert()
{
	int row = tableWidget->currentRow();
	if(row < 0)
	{
		SQt::ShowInfoBox("提示","请先选择插入位置!");
		return;
	}
	tableWidget->insertRow(row);
	SString x="0",y="0",a="";
	if(row < tableWidget->rowCount())
	{
		x = tableWidget->item(row+1,0)->text().toStdString().data();
		y = tableWidget->item(row+1,1)->text().toStdString().data();
		a = tableWidget->item(row+1,2)->text().toStdString().data();
	}
	tableWidget->setItem(row,0,new QTableWidgetItem(x.data()));
	tableWidget->setItem(row,1,new QTableWidgetItem(y.data()));
	tableWidget->setItem(row,2,new QTableWidgetItem(a.data()));
	on_tableWidget_itemChanged(NULL);
}

void SSvgPointsEditDlg::OnDel()
{
	int row = tableWidget->currentRow();
	if(row < 0)
		return;
	if(!SQt::ShowQuestionBox("提示","您确定要删除当前点吗？",this))
		return;
	tableWidget->removeRow(row);
	on_tableWidget_itemChanged(NULL);
}

void SSvgPointsEditDlg::OnOk()
{
	accept();
	SString points,extattr;
	GetPoints(points,extattr);
	m_pSvgObject->SetAttributeValue("points",points,false,false);
	extattr = "p-attr="+extattr;
	m_pSvgObject->SetAttributeValue("ext-attribute",extattr,false,false);
	m_pSvgObject->SetAttributeValue("hover_idx","");
	hide();
}

void SSvgPointsEditDlg::OnCancel()
{
	reject();
	m_pSvgObject->SetAttributeValue("points",m_sOldText,false,false);
	m_pSvgObject->SetAttributeValue("hover_idx","");
	hide();
}

SSvgAttributeWnd::SSvgAttributeWnd(QWidget *p,bool bEmbedded/*=false*/) : QWidget(p)
{
	m_pPointsEditDlg = NULL;
	m_pThisObj = NULL;
	m_bDisableAttrEdit = false;
	m_pSvgWnd = NULL;
	m_pEditorWnd = NULL;
	m_pTabWidget = NULL;
	this->resize(300, 288);
	if(!bEmbedded)
	{
#ifdef WIN32
		setWindowFlags( Qt::Dialog | Qt::Tool | Qt::WindowStaysOnTopHint);
#else
		setWindowFlags( Qt::Dialog );
#endif
	}
	
	m_gridLayout = new QGridLayout(this);
	m_gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
	m_gridLayout->setContentsMargins(0, 0, 0, 0);
	m_tableWidget = new QTableWidget(this);
	m_tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
	m_gridLayout->addWidget(m_tableWidget, 0, 0, 1, 1);
	SQt::SetTableWidgetStyle(m_tableWidget);
	m_tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: rgb(100, 179, 238);}");
	//m_tableWidget->setSelectionBehavior(QAbstractItemView::SelectItems);
	m_tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
	m_tableWidget->setColumnCount(4);
	QStringList slTitle;
	slTitle << tr("属性") << tr("属性名") << tr("属性值") << tr("操作");
	m_tableWidget->setHorizontalHeaderLabels(slTitle);
	QHeaderView *headerView = m_tableWidget->horizontalHeader();
	headerView->resizeSection(0,0);
	headerView->resizeSection(1,70);
	headerView->resizeSection(2,160);
	headerView->resizeSection(3,50);
	m_tableWidget->setEditTriggers(QTableWidget::NoEditTriggers);//不能编辑
	m_tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	//m_tableWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
	m_tableWidget->setAutoScroll(false);
	m_tableWidget->verticalHeader()->setVisible(false);
	//m_tableWidget->horizontalHeader()->setVisible(false);

	connect(m_tableWidget,SIGNAL(cellClicked(int,int)),this,SLOT(OnCellClicked(int,int)));
	connect(m_tableWidget,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));
}

SSvgAttributeWnd::~SSvgAttributeWnd()
{
	if(m_pPointsEditDlg != NULL)
		delete m_pPointsEditDlg;
}

void SSvgAttributeWnd::SetSvgWidget(QWidget *p)
{
	m_pSvgWnd = p;
	connect(m_pSvgWnd,SIGNAL(sigCursorPos(float,float)),this,SLOT(OnCursorPos(float,float)));
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  加载指定的对象到当前属性窗口
// 作    者:  邵凯田
// 创建时间:  2015-8-20 14:47
// 参数说明:  @pObj待加载的对象
// 返 回 值:  true表示加载成功，false表示加载失败
//////////////////////////////////////////////////////////////////////////
bool SSvgAttributeWnd::LoadObject(SSvgObject *pObj)
{
	if(m_pThisObj != pObj || m_pThisObj == NULL)
	{
		m_pThisObj = pObj;
		if(m_tableWidget->rowCount()>=2)
			m_tableWidget->setRowCount(2);
	}
	if(m_tableWidget->rowCount()<2)
	{
		AddAttribute("cursor-x",NULL);
		AddAttribute("cursor-y",NULL);
	}
	if(pObj == NULL)
	{
		//加载画布属性
		int row;
		if(m_pSvgWnd == NULL)
			return false;
		SSvgObject *pObj = ((SSvgWnd*)m_pSvgWnd)->m_Document.GetFirstObject();
		if(pObj == NULL)
			return false;
		SSvgObject *pRoot = ((SSvgWnd*)m_pSvgWnd)->m_Document.GetRootObject();
		if(pRoot == NULL)
			return false;
		m_bDisableAttrEdit = true;
		row = AddAttributeEx("svg_width","画布宽度",pObj->GetAttribute("width"));
		row = AddAttributeEx("svg_height","画布高度",pObj->GetAttribute("height"));
		row = AddAttributeEx("svg_fill","背景颜色",pObj->GetAttribute("fill"));		
		m_bDisableAttrEdit = false;
		QTableWidgetItem *p = new QTableWidgetItem(tr("设置..."));
		QFont f;
		f.setItalic(true);
		f.setUnderline(true);
		p->setFont(f);
		p->setTextColor(QColor(0,0,255));
		m_tableWidget->setItem(row,3,p);
		m_tableWidget->item(row,2)->setBackgroundColor(SColor2QColor(SSvgPainter::GetColorByString(pObj->GetAttribute("fill"))));

		row = AddAttributeEx("predefine","预定义项","");		
		m_bDisableAttrEdit = false;
		p = new QTableWidgetItem(tr("设置..."));
		p->setFont(f);
		p->setTextColor(QColor(0,0,255));
		m_tableWidget->setItem(row,3,p);

// 		AddAttribute("svg_coor-type",pRoot);
// 		AddAttribute("svg_orig-x",pRoot);
// 		AddAttribute("svg_orig-y",pRoot);

		return true;
	}
	QString sDlgTitle=tr("SVG属性窗-");
	m_bDisableAttrEdit = true;
	switch(pObj->GetType())
	{
	case SSvgObject::TYPE_GROUP:		//SVG组
		sDlgTitle += tr("组");
		AddAttribute("uri",pObj);
		AddAttribute("caption",pObj);
		AddAttribute("z",pObj);
		AddAttribute("ext-attribute",pObj);
		break;
	case SSvgObject::TYPE_RECT:			//区域
		sDlgTitle += tr("矩形");
		AddAttribute("uri",pObj);
		AddAttribute("caption",pObj);
		AddAttribute("x",pObj);
		AddAttribute("y",pObj);
		AddAttribute("z",pObj);
		AddAttribute("width",pObj);
		AddAttribute("height",pObj);
		AddAttribute("stroke",pObj);
		AddAttribute("stroke-width",pObj);
		AddAttribute("stroke-dasharray",pObj);
		AddAttribute("fill",pObj);
		AddAttribute("rx",pObj);
		AddAttribute("ry",pObj);
		AddAttribute("alpha",pObj);
		AddAttribute("ext-attribute",pObj);
		break;
	case SSvgObject::TYPE_USE:				//引用符号
		sDlgTitle += tr("引用");
		AddAttribute("uri",pObj);
		AddAttribute("caption",pObj);
		AddAttribute("x",pObj);
		AddAttribute("y",pObj);
		AddAttribute("z",pObj);
		AddAttribute("xlink:href",pObj);
		AddAttribute("ext-attribute",pObj);
		break;
	case SSvgObject::TYPE_RECTTEXT:			//区域文字
		sDlgTitle += tr("区域文字");
		AddAttribute("uri",pObj);
		AddAttribute("caption",pObj);
		AddAttribute("x",pObj);
		AddAttribute("y",pObj);
		AddAttribute("z",pObj);
		AddAttribute("width",pObj);
		AddAttribute("height",pObj);
		AddAttribute("alignh",pObj);
		AddAttribute("alignv",pObj);
		AddAttribute("font",pObj);
		// 		AddAttribute("font-family",pObj);
		// 		AddAttribute("font-size",pObj);
		// 		AddAttribute("font-weight",pObj);
		AddAttribute("fill",pObj);
		AddAttribute("text",pObj);
		AddAttribute("ext-attribute",pObj);
		break;
	case SSvgObject::TYPE_TEXT:			//文字
		sDlgTitle += tr("文字");
		AddAttribute("uri",pObj);
		AddAttribute("caption",pObj);
		AddAttribute("x",pObj);
		AddAttribute("y",pObj);
		AddAttribute("z",pObj);
		AddAttribute("font",pObj);
// 		AddAttribute("font-family",pObj);
// 		AddAttribute("font-size",pObj);
// 		AddAttribute("font-weight",pObj);
		AddAttribute("fill",pObj);
		AddAttribute("text",pObj);
		AddAttribute("text-anchor",pObj);
		AddAttribute("ext-attribute",pObj);
		break;
	case SSvgObject::TYPE_ELLIPSE:		//椭圆
		sDlgTitle += tr("圆形");
		AddAttribute("uri",pObj);
		AddAttribute("caption",pObj);
		AddAttribute("cx",pObj);
		AddAttribute("cy",pObj);
		AddAttribute("z",pObj);
		AddAttribute("rx",pObj);
		AddAttribute("ry",pObj);
		AddAttribute("stroke",pObj);
		AddAttribute("stroke-width",pObj);
		AddAttribute("stroke-dasharray",pObj);
		AddAttribute("fill",pObj);
		AddAttribute("alpha",pObj);
		AddAttribute("ext-attribute",pObj);
		break;
	case SSvgObject::TYPE_LINE:			//直线
		sDlgTitle += tr("直线");
		AddAttribute("uri",pObj);
		AddAttribute("caption",pObj);
		AddAttribute("x1",pObj);
		AddAttribute("y1",pObj);
		AddAttribute("z",pObj);
		AddAttribute("x2",pObj);
		AddAttribute("y2",pObj);
		AddAttribute("stroke",pObj);
		AddAttribute("stroke-width",pObj);
		AddAttribute("stroke-dasharray",pObj);
		AddAttribute("ext-attribute",pObj);
		break;
	case SSvgObject::TYPE_POLYLINE:		//折线
		sDlgTitle += tr("折线");
		AddAttribute("uri",pObj);
		AddAttribute("caption",pObj);
		AddAttribute("points",pObj);
		AddAttribute("z",pObj);
		AddAttribute("stroke",pObj);
		AddAttribute("stroke-width",pObj);
		AddAttribute("stroke-dasharray",pObj);
		AddAttribute("fill",pObj);
		AddAttribute("ext-attribute",pObj);
		break;
	case SSvgObject::TYPE_PATH:			//路径
		sDlgTitle += tr("路径");
		AddAttribute("uri",pObj);
		AddAttribute("caption",pObj);
		AddAttribute("d",pObj);
		AddAttribute("z",pObj);
		AddAttribute("stroke",pObj);
		AddAttribute("stroke-width",pObj);
		AddAttribute("stroke-dasharray",pObj);
		AddAttribute("fill",pObj);
		AddAttribute("ext-attribute",pObj);
		break;
	case SSvgObject::TYPE_PIC:			//图片
		sDlgTitle += tr("图片");
		AddAttribute("uri",pObj);
		AddAttribute("caption",pObj);
		AddAttribute("x",pObj);
		AddAttribute("y",pObj);
		AddAttribute("z",pObj);
		AddAttribute("width",pObj);
		AddAttribute("height",pObj);
		AddAttribute("href",pObj);
		AddAttribute("rotate",pObj);
		AddAttribute("alpha",pObj);
		AddAttribute("ext-attribute",pObj);
		break;
	}
	setWindowTitle(sDlgTitle);
	m_bDisableAttrEdit = false;
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  添加画布属性列表
// 作    者:  邵凯田
// 创建时间:  2015-8-22 10:18
// 参数说明:  
// 返 回 值:  
//////////////////////////////////////////////////////////////////////////
int SSvgAttributeWnd::AddAttributeEx(const char* sName,const char* sTitle,SString sValue)
{
	int row = -1;
	for(int i=0;i<m_tableWidget->rowCount();i++)
	{
		if(m_tableWidget->item(i,0)->text() == sName)
		{
			row = i;
			break;
		}
	}
	if(row < 0)
	{
		row = m_tableWidget->rowCount();
		m_tableWidget->insertRow(row);
		m_tableWidget->setRowHeight(row,C_TABLE_ROW_HEIGHT);
	}
	QTableWidgetItem *p;
	m_tableWidget->setItem(row,0,(p=new QTableWidgetItem(sName)));
	m_tableWidget->setItem(row,1,(p=new QTableWidgetItem(sTitle)));
	p->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
	p->setBackgroundColor(QColor(100, 179, 238));
	m_tableWidget->setItem(row,2,new QTableWidgetItem(sValue.data()));
	return row;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  添加属性值到属性列表
// 作    者:  邵凯田
// 创建时间:  2015-8-20 15:05
// 参数说明:  @sName为属性名称
//         :  @pObj为对象指针
// 返 回 值:  返回表格行号
//////////////////////////////////////////////////////////////////////////
int SSvgAttributeWnd::AddAttribute(SString sName,SSvgObject *pObj)
{
	int row = -1;
	for(int i=0;i<m_tableWidget->rowCount();i++)
	{
		if(m_tableWidget->item(i,0)->text() == sName.data())
		{
			row = i;
			break;
		}
	}
	if(row < 0)
	{
		row = m_tableWidget->rowCount();
		m_tableWidget->insertRow(row);
		m_tableWidget->setRowHeight(row,C_TABLE_ROW_HEIGHT);
	}
	SString sTitle;
	if(sName == "uri")						sTitle = Q2C(tr("URI"));
	else if(sName == "caption")				sTitle = Q2C(tr("标题"));
	else if(sName == "x")					sTitle = Q2C(tr("X坐标"));
	else if(sName == "y")					sTitle = Q2C(tr("Y坐标"));
	else if(sName == "z")					sTitle = Q2C(tr("叠加次序Z"));
	else if(sName == "width")				sTitle = Q2C(tr("宽度"));
	else if(sName == "height")				sTitle = Q2C(tr("高度"));
	else if(sName == "stroke")				sTitle = Q2C(tr("线条颜色"));
	else if(sName == "stroke-width")		sTitle = Q2C(tr("线条宽度"));
	else if(sName == "stroke-dasharray")	sTitle = Q2C(tr("线条样式"));
	else if(sName == "text-anchor")			sTitle = Q2C(tr("文字对齐"));
//	else if(sName == "round-x")				sTitle = Q2C(tr("X圆角半径"));
//	else if(sName == "round-y")				sTitle = Q2C(tr("Y圆角半径"));
	else if(sName == "fill")				sTitle = Q2C(tr("填充颜色"));
	else if(sName == "alpha")				sTitle = Q2C(tr("透明度"));
	else if(sName == "font")				sTitle = Q2C(tr("字体"));
// 	else if(sName == "font-family")			sTitle = Q2C(tr("字体名称"));
// 	else if(sName == "font-size")			sTitle = Q2C(tr("字体大小"));
// 	else if(sName == "font-weight")			sTitle = Q2C(tr("加粗显示"));
	else if(sName == "x1")					sTitle = Q2C(tr("X1"));
	else if(sName == "y1")					sTitle = Q2C(tr("Y1"));
	else if(sName == "x2")					sTitle = Q2C(tr("X2"));
	else if(sName == "y2")					sTitle = Q2C(tr("Y2"));
	else if(sName == "cx")					sTitle = Q2C(tr("圆心X坐标"));
	else if(sName == "cy")					sTitle = Q2C(tr("圆心Y坐标"));
	else if(sName == "rx")					sTitle = Q2C(tr("X半径"));
	else if(sName == "ry")					sTitle = Q2C(tr("Y半径"));
	else if(sName == "points")				sTitle = Q2C(tr("折线坐标"));
	else if(sName == "d")					sTitle = Q2C(tr("路径"));
	else if(sName == "href")				sTitle = Q2C(tr("引用"));
	else if(sName == "text")				sTitle = Q2C(tr("文本内容"));
	else if(sName == "alignh")				sTitle = Q2C(tr("水平对齐"));
	else if(sName == "alignv")				sTitle = Q2C(tr("垂直对齐"));
	else if(sName == "ext-attribute")		sTitle = Q2C(tr("扩展属性"));
	else if(sName == "xlink:href")			sTitle = Q2C(tr("引用符号"));
	else if(sName == "svg_coor-type")		sTitle = Q2C(tr("坐标系类型"));
	else if(sName == "svg_orig-x")			sTitle = Q2C(tr("原点X偏移"));
	else if(sName == "svg_orig-y")			sTitle = Q2C(tr("原点Y偏移"));
	else if(sName == "cursor-x")			sTitle = Q2C(tr("鼠标位置X"));
	else if(sName == "cursor-y")			sTitle = Q2C(tr("鼠标位置Y"));
	else if(sName == "rotate")				sTitle = Q2C(tr("旋转角度"));
	else									sTitle = sName;
	QTableWidgetItem *p;
	m_tableWidget->setItem(row,0,(p=new QTableWidgetItem(sName.data())));
	m_tableWidget->setItem(row,1,(p=new QTableWidgetItem(sTitle.data())));
	p->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
	p->setBackgroundColor(QColor(100, 179, 238));
	SString sValue;
	if(pObj != NULL)
		sValue = sName.left(4)=="svg_"?pObj->GetAttribute(sName.mid(4)):pObj->GetAttribute(sName);
	m_tableWidget->setItem(row,2,new QTableWidgetItem(sValue.data()));
	if(sName == "fill" || sName == "stroke")
	{
		//checkbox
		p = new QTableWidgetItem(tr("设置..."));
		QFont f;
		f.setItalic(true);
		f.setUnderline(true);
		p->setFont(f);
		p->setTextColor(QColor(0,0,255));
		p->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
		m_tableWidget->setItem(row,3,p);
		if(sValue.left(5) != "url(#")
			m_tableWidget->item(row,2)->setBackgroundColor(SColor2QColor(SSvgPainter::GetColorByString(sValue)));
	}
	else if(sName == "stroke-dasharray")
	{
		//combobox;
		QComboBox *p = new QComboBox(m_tableWidget);
		p->addItem(tr("0-实线"));
		p->addItem(tr("1-虚线"));
		p->addItem(tr("2-点线"));
		p->addItem(tr("3-点画线"));
		p->addItem(tr("4-点点画线"));
		p->setCurrentIndex(sValue.toInt());
		m_tableWidget->setCellWidget(row,2,p);
		connect(p,SIGNAL(currentIndexChanged(int)),this,SLOT(OnLTCurrentIndexChanged(int)));
	}
	else if(sName == "text-anchor")
	{
		//combobox;
		QComboBox *p = new QComboBox(m_tableWidget);
		p->addItem(tr("左对齐"));
		p->addItem(tr("水平居中"));
		p->addItem(tr("右对齐"));
		if(sValue == "middle")
			p->setCurrentIndex(1);
		else if(sValue == "end")
			p->setCurrentIndex(2);
		else
			p->setCurrentIndex(0);
		m_tableWidget->setCellWidget(row,2,p);
		connect(p,SIGNAL(currentIndexChanged(int)),this,SLOT(OnLTCurrentIndexChanged(int)));
	}
	else if(sName == "alignh")
	{
		//combobox;
		QComboBox *p = new QComboBox(m_tableWidget);
		p->addItem(tr("左对齐"));
		p->addItem(tr("水平居中"));
		p->addItem(tr("右对齐"));
		if(sValue == "left")
			p->setCurrentIndex(0);
		else if(sValue == "right")
			p->setCurrentIndex(2);
		else
			p->setCurrentIndex(1);
		m_tableWidget->setCellWidget(row,2,p);
		connect(p,SIGNAL(currentIndexChanged(int)),this,SLOT(OnLTCurrentIndexChanged(int)));
	}
	else if(sName == "alignv")
	{
		//combobox;
		QComboBox *p = new QComboBox(m_tableWidget);
		p->addItem(tr("上对齐"));
		p->addItem(tr("垂直居中"));
		p->addItem(tr("下对齐"));
		if(sValue == "top")
			p->setCurrentIndex(0);
		else if(sValue == "bottom")
			p->setCurrentIndex(2);
		else
			p->setCurrentIndex(1);
		m_tableWidget->setCellWidget(row,2,p);
		connect(p,SIGNAL(currentIndexChanged(int)),this,SLOT(OnLTCurrentIndexChanged(int)));
	}
	else if(sName == "svg_coor-type")
	{
		//combobox;
		QComboBox *p = new QComboBox(m_tableWidget);
		p->addItem(tr("左上角原点"));
		p->addItem(tr("左下角原点"));
		if(sValue == "left_bottom")
			p->setCurrentIndex(1);
		else
			p->setCurrentIndex(0);
		m_tableWidget->setCellWidget(row,2,p);
		connect(p,SIGNAL(currentIndexChanged(int)),this,SLOT(OnLTCurrentIndexChanged(int)));
	}
	else if(sName == "font")
	{
// 		AddAttribute("font-family",pObj);
// 		AddAttribute("font-size",pObj);
// 		AddAttribute("font-weight",pObj);
		p = new QTableWidgetItem(tr("设置..."));
		QFont f;
		f.setItalic(true);
		f.setUnderline(true);
		p->setFont(f);
		p->setTextColor(QColor(0,0,255));
		p->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
		m_tableWidget->setItem(row,3,p);
		int fs = pObj->GetAttributeI("font-size");
		SString fw = pObj->GetAttribute("font-weight");
		SString fi = m_pThisObj->GetAttribute("font-italic");
		QString qff = pObj->GetAttribute("font-family").data();
		QFont f2(qff,fs,fw=="bold"?(QFont::Bold):(QFont::Normal),fi=="italic");
		if(fs+6 > C_TABLE_ROW_HEIGHT)
			m_tableWidget->setRowHeight(row,fs+6);
		else
			m_tableWidget->setRowHeight(row,C_TABLE_ROW_HEIGHT);
		p = m_tableWidget->item(row,2);
		p->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
		p->setFont(f2);
		p->setText(qff);
	}
	else if(sName == "ext-attribute" || sName == "text" || sName == "uri" || sName == "caption" || sName == "points")
	{
		QTableWidgetItem *p = new QTableWidgetItem(tr("设置..."));
		QFont f;
		f.setItalic(true);
		f.setUnderline(true);
		p->setFont(f);
		p->setTextColor(QColor(0,0,255));
		m_tableWidget->setItem(row,3,p);
	}
	else
	{
		//lineedit
// 		QLineEdit *p = new QLineEdit(m_tableWidget);
// 		m_tableWidget->setCellWidget(row,2,p);
// 		p->setText(sValue.data());
	}
	return row;
}

QColor SSvgAttributeWnd::SColor2QColor(SCOLOR col)
{
	QColor color(col.r,col.g,col.b);
	return color;
}

SCOLOR SSvgAttributeWnd::QColor2SColor(QColor col)
{
	SCOLOR color;
	color.r = col.red();
	color.g = col.green();
	color.b = col.blue();
	return color;
}

void SSvgAttributeWnd::Show()
{
	show();
	if(m_pTabWidget != NULL)
		m_pTabWidget->setCurrentIndex(m_iTabIndex);
}

void SSvgAttributeWnd::Hide()
{
	if(m_pTabWidget == NULL)
		hide();
	else
	{
		m_pThisObj = NULL;
		LoadObject(NULL);
	}
}

void SSvgAttributeWnd::OnCursorPos(float x,float y)
{
	QTableWidgetItem *p;
	p = m_tableWidget->item(0,2);
	if(p == NULL)
		return;
	p->setText(SString::toFormat("%f",x).data());
	p = m_tableWidget->item(1,2);
	if(p == NULL)
		return;
	p->setText(SString::toFormat("%f",y).data());
}
void SSvgAttributeWnd::OnCellClicked(int row,int column)
{
	if(row < 0 || (column != 2 && column != 3))
		return;
	if(column == 2)
	{
		QTableWidgetItem *p0,*p2;
		p0 = m_tableWidget->item(row,0);
		p2 = m_tableWidget->item(row,2);
		if(p0 == NULL || p2 == NULL)
			return;
		SString sName = p0->text().toStdString().data();
		SString sValue = p2->text().toStdString().data();
		if(/*sName == "fill" || sName == "svg_fill" || sName == "stroke" || */sName == "font" || sName == "cursor-x" || sName == "cursor-y" || sName == "points")
			return;
		m_tableWidget->editItem(p2);
	}
	else if(column == 3)
	{
		QTableWidgetItem *p0,*p1,*p2;
		p0 = m_tableWidget->item(row,0);
		p1 = m_tableWidget->item(row,1);
		p2 = m_tableWidget->item(row,2);
		if(p0 == NULL || p2 == NULL)
			return;
		SString sName = p0->text().toStdString().data();
		SString sValue = p2->text().toStdString().data();
		if(sName == "fill")
		{
			if(m_pThisObj->GetType() == SSvgObject::TYPE_TEXT || 
				m_pThisObj->GetType() == SSvgObject::TYPE_RECTTEXT || 
				SQt::ShowQuestionBox(tr("问题"),tr("是否需要进行颜色填充?"),this))
			{
				//select color
				SCOLOR color = SSvgPainter::GetColorByString(sValue);
				QColor newcolor = QColorDialog::getColor(QColor(color.r,color.g,color.b), this, tr("选择颜色"));
				if(!newcolor.isValid())
					return;
				p2->setBackgroundColor(newcolor);
				sValue = SSvgPainter::GetStringByColor(QColor2SColor(newcolor));
			}
			else
			{
				//p2->setBackgroundColor(p0->backgroundColor());
				sValue = "none";
			}
		}
		else if(sName == "stroke" || sName == "svg_fill")
		{
			//select color
			SCOLOR color = SSvgPainter::GetColorByString(sValue);
			QColor newcolor = QColorDialog::getColor(QColor(color.r,color.g,color.b), this, tr("选择颜色"));
			if(!newcolor.isValid())
				return;
			p2->setBackgroundColor(newcolor);
			color = QColor2SColor(newcolor);
			sValue = SSvgPainter::GetStringByColor(color);
			if(sName == "svg_fill")
			{
				if(m_pSvgWnd == NULL)
					return;
				SSvgObject *pObj = ((SSvgWnd*)m_pSvgWnd)->m_Document.GetFirstObject();
				if(pObj == NULL)
					return;
				pObj->SetAttributeValue((char*)"fill",sValue);
				color.r = 255 - color.r;
				color.g = 255 - color.g;
				color.b = 255 - color.b;
				SString sValue2 = SSvgPainter::GetStringByColor(color);
				pObj->SetAttributeValue((char*)"stroke",sValue2);
			}
		}
		else if(sName == "font")
		{
			SString ff = m_pThisObj->GetAttribute("font-family");
			int fs = m_pThisObj->GetAttributeI("font-size");
			SString fw = m_pThisObj->GetAttribute("font-weight");
			SString fi = m_pThisObj->GetAttribute("font-italic");
			QString qff = ff.data();
			QFont f2(qff,fs,fw=="bold"?(QFont::Bold):(QFont::Normal),fi=="italic");
			bool ok=false;
			QFont font = QFontDialog::getFont(&ok, f2, this, tr("选择字体"));  
			if(!ok)
				return;
			ff = font.family().toStdString().data();
			fs = font.pointSize();
			fw = font.bold()?"bold":"normal";
			m_pThisObj->SetAttributeValue((char*)"font-family",ff,false,false);
			m_pThisObj->SetAttributeValue("font-size",SString::toString(fs),false,false);
			m_pThisObj->SetAttributeValue("font-weight",fw,false,false);
			m_pThisObj->SetAttributeValue("font-italic",font.italic()?"italic":"",false,false);

			if(fs+6 > C_TABLE_ROW_HEIGHT)
				m_tableWidget->setRowHeight(row,fs+6);
			else
				m_tableWidget->setRowHeight(row,C_TABLE_ROW_HEIGHT);
			p2->setFont(font);
			p2->setText(ff.data());
		}
		else if(sName == "ext-attribute")
		{
			//判断是否有用户层处理函数
			bool bSet = false;
			if(m_pEditorWnd != NULL && ((SSvgEditorWnd*)m_pEditorWnd)->EditExtAttribute(m_pThisObj,sValue))
				bSet = true;
			if(!bSet && m_pSvgWnd != NULL && ((SSvgWnd*)m_pSvgWnd)->EditExtAttribute(m_pThisObj,sValue))
				bSet = true;
			if(!bSet)
			{
				SSvgMultiEditDlg dlg;
				if(dlg.ShowMultiInput(p1->text().toStdString().data(),sValue))
					bSet = true;
			}
			if(bSet)
			{
				m_pThisObj->SetAttributeValue(sName.data(),sValue,false,false);
				p2->setText(sValue.data());
			}
		}
		else if(sName == "text")
		{
			SSvgMultiEditDlg dlg;
			if(dlg.ShowMultiInput(p1->text().toStdString().data(),sValue))
			{
				m_pThisObj->SetAttributeValue(sName.data(),sValue,false,false);
				p2->setText(sValue.data());
			}
			else
			{
				return;
			}
		}
		else if(sName == "uri")
		{
			//判断是否有用户层处理函数
			bool bSet = false;
			if(m_pEditorWnd != NULL && ((SSvgEditorWnd*)m_pEditorWnd)->EditUriAttribute(m_pThisObj,sValue))
				bSet = true;
			if(!bSet && m_pSvgWnd != NULL && ((SSvgWnd*)m_pSvgWnd)->EditUriAttribute(m_pThisObj,sValue))
				bSet = true;
			if(!bSet)
			{
				SSvgMultiEditDlg dlg;
				if(dlg.ShowMultiInput(p1->text().toStdString().data(),sValue))
					bSet = true;
			}
			if(bSet)
			{
				m_pThisObj->SetAttributeValue(sName.data(),sValue,false,false);
				p2->setText(sValue.data());
			}
		}
		else if(sName == "caption")
		{
			SSvgMultiEditDlg dlg;
			if(dlg.ShowMultiInput(p1->text().toStdString().data(),sValue))
			{
				m_pThisObj->SetAttributeValue(sName.data(),sValue,false,false);
				p2->setText(sValue.data());
			}
			else
			{
				return;
			}
		}
		else if(sName == "points")
		{
			if(m_pPointsEditDlg == NULL)
			{
				m_pPointsEditDlg = new SSvgPointsEditDlg();
			}
			m_pPointsEditDlg->m_pSvgWnd = m_pSvgWnd;			
			m_pPointsEditDlg->ShowEdit(m_pThisObj,sValue);
			m_pPointsEditDlg->show();
			/*
			if(dlg.ShowEdit(m_pThisObj,sValue))
			{
				m_pThisObj->SetAttributeValue(sName.data(),sValue,false,false);
				p2->setText(sValue.data());
			}
			else
			{
				return;
			}*/
		}
		else if(sName == "predefine")
		{
			//预定义项
			SString sXml;
			((SSvgWnd*)m_pSvgWnd)->m_Document.WriteSvgDefs(NULL,0,&sXml,true);
			SSvgMultiEditDlg dlg;
			again:
			if(dlg.ShowMultiInput(p1->text().toStdString().data(),sXml))
			{
				SXmlConfig xml;
				if(!xml.ReadText(sXml.data()))
				{
					SQt::ShowErrorBox("错误","预定义格式不正确!请重新输入!");
					goto again;
				}
				((SSvgWnd*)m_pSvgWnd)->m_Document.ProcessDefsNode(&xml);
			}
			else
			{
				return;
			}
		}
		if(m_pThisObj != NULL)
			m_pThisObj->SetAttributeValue(sName.data(),sValue);
		if(m_pSvgWnd != NULL)
		{
			((SSvgWnd*)m_pSvgWnd)->Render();
			m_pSvgWnd->repaint();
		}
		p2->setText(sValue.data());
	}
}

void SSvgAttributeWnd::on_tableWidget_itemChanged(QTableWidgetItem* item)
{
	int row = item->row();
	if(m_bDisableAttrEdit || row < 0)
		return;
	QTableWidgetItem *p0,*p2;
	p0 = m_tableWidget->item(row,0);
	p2 = m_tableWidget->item(row,2);
	if(p0 == NULL || p2 == NULL)
		return;
	SString sName = p0->text().toStdString().data();
	SString sValue = p2->text().toStdString().data();
	if(m_pThisObj == NULL)
	{
		if(sName.left(4) != "svg_" || m_pSvgWnd == NULL)
			return;
		SSvgObject *pObj = ((SSvgWnd*)m_pSvgWnd)->m_Document.GetFirstObject();
		if(pObj == NULL)
			return;
		sName = sName.mid(4);
		if(sName == "orig-x" || sName == "orig-y")
		{
			pObj = ((SSvgWnd*)m_pSvgWnd)->m_Document.GetRootObject();
			if(pObj == NULL)
				return;
			pObj->SetAttributeValue(sName.data(),sValue,false,false);
			((SSvgWnd*)m_pSvgWnd)->m_Document.SetOrigX(pObj->GetAttributeF("orig-x"));
			((SSvgWnd*)m_pSvgWnd)->m_Document.SetOrigY(pObj->GetAttributeF("orig-y"));
		}
		else
		{
			pObj->SetAttributeValue(sName.data(),sValue,false,false);
			pObj->CalcObjectRect();
			((SSvgWnd*)m_pSvgWnd)->m_Document.SetWidth(pObj->GetAttributeF("width"));
			((SSvgWnd*)m_pSvgWnd)->m_Document.SetHeight(pObj->GetAttributeF("height"));
		}
// 		AddAttributeEx("svg_width","画布宽度");
// 		AddAttributeEx("svg_height","画布高度");
// 		AddAttributeEx("svg_fill","背景颜色");
		if(m_pSvgWnd != NULL)
		{
			((SSvgWnd*)m_pSvgWnd)->Render();
			m_pSvgWnd->repaint();
		}
		return;
	}
	if(m_pThisObj->GetAttribute(sName) != sValue)
	{
		m_pThisObj->SetAttributeValue(sName.data(),sValue,false,false);
		m_pThisObj->CalcObjectRect();

		if(sName == "z")
		{
			//TODO:调整Z序
			((SSvgWnd*)m_pSvgWnd)->m_Document.SortObject(m_pThisObj);
		}
		else if(sName == "fill" || sName == "stroke")
		{
			if(sValue.left(5) == "url(#")
				m_tableWidget->item(row,2)->setBackgroundColor(SColor2QColor(SSvgPainter::GetColorByString("#FFFFFF")));
			else
				m_tableWidget->item(row,2)->setBackgroundColor(SColor2QColor(SSvgPainter::GetColorByString(sValue)));
		}
		if(m_pSvgWnd != NULL)
		{
			((SSvgWnd*)m_pSvgWnd)->Render();
			m_pSvgWnd->repaint();
		}
	}
}

void SSvgAttributeWnd::OnLTCurrentIndexChanged(int idx)
{
	//查找对应的行
	int i,cnt = m_tableWidget->rowCount();
	QString name;
	for(i=0;i<cnt;i++)
	{
		name = m_tableWidget->item(i,0)->text();
		if(name == "stroke-dasharray")
		{
			SString sValue = SString::toString(idx);
			m_tableWidget->item(i,2)->setText(sValue.data());
			if(m_pThisObj->GetAttribute("stroke-dasharray") != sValue)
			{
				m_pThisObj->SetAttributeValue("stroke-dasharray",sValue,false,false);
				if(m_pSvgWnd != NULL)
				{
					((SSvgWnd*)m_pSvgWnd)->Render();
					m_pSvgWnd->repaint();
				}
			}
		}
		else if(name == "alignv")
		{
			QComboBox *p = (QComboBox *)m_tableWidget->cellWidget(i,2);
			if(p == NULL)
				return;
			SString sValue;
			idx = p->currentIndex();
			if(idx == 0)
				sValue = "top";
			else if(idx == 2)
				sValue = "bottom";
			else
				sValue = "center";
			m_tableWidget->item(i,2)->setText(sValue.data());
			if(m_pThisObj->GetAttribute("alignv") != sValue)
			{
				m_pThisObj->SetAttributeValue("alignv",sValue,false,false);
				if(m_pSvgWnd != NULL)
				{
					((SSvgWnd*)m_pSvgWnd)->Render();
					m_pSvgWnd->repaint();
				}
			}
		}
		else if(name == "alignh")
		{
			QComboBox *p = (QComboBox *)m_tableWidget->cellWidget(i,2);
			if(p == NULL)
				return;
			SString sValue;
			idx = p->currentIndex();
			if(idx == 0)
				sValue = "left";
			else if(idx == 2)
				sValue = "right";
			else
				sValue = "center";
			m_tableWidget->item(i,2)->setText(sValue.data());
			if(m_pThisObj->GetAttribute("alignh") != sValue)
			{
				m_pThisObj->SetAttributeValue("alignh",sValue,false,false);
				if(m_pSvgWnd != NULL)
				{
					((SSvgWnd*)m_pSvgWnd)->Render();
					m_pSvgWnd->repaint();
				}
			}
		}
		else if(name == "text-anchor")
		{
			QComboBox *p = (QComboBox *)m_tableWidget->cellWidget(i,2);
			if(p == NULL)
				return;
			SString sValue;
			idx = p->currentIndex();
			if(idx == 1)
				sValue = "middle";
			else if(idx == 2)
				sValue = "end";
			else
				sValue = "start";
			m_tableWidget->item(i,2)->setText(sValue.data());
			if(m_pThisObj->GetAttribute("text-anchor") != sValue)
			{
				m_pThisObj->SetAttributeValue("text-anchor",sValue,false,false);
				if(m_pSvgWnd != NULL)
				{
					((SSvgWnd*)m_pSvgWnd)->Render();
					m_pSvgWnd->repaint();
				}
			}
		}
		else if(name == "svg_coor-type")
		{
			SSvgObject *pObj = ((SSvgWnd*)m_pSvgWnd)->m_Document.GetRootObject();
			if(pObj == NULL)
				return;
			QComboBox *p = (QComboBox *)m_tableWidget->cellWidget(i,2);
			if(p == NULL)
				return;
			SString sValue;
			idx = p->currentIndex();
			if(idx == 1)
				sValue = "left_bottom";
			else
				sValue = "";
			m_tableWidget->item(i,2)->setText(sValue.data());
			if(pObj->GetAttribute("coor-type") != sValue)
			{
				pObj->SetAttributeValue("coor-type",sValue,false,false);
				((SSvgWnd*)m_pSvgWnd)->m_Document.SetCoorType(sValue);
				if(m_pSvgWnd != NULL)
				{
					((SSvgWnd*)m_pSvgWnd)->Render();
					m_pSvgWnd->repaint();
				}
			}
		}
	}
}

