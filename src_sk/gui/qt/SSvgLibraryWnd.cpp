/**
 *
 * �� �� �� : SSvgLibraryWnd.cpp
 * �������� : 2015-8-19 20:00
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : SVGͼԪ�ⴰ��
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-8-19	�ۿ�������ļ�
 *
 **/

#include "SSvgLibraryWnd.h"
#define C_SVG_LIB_WIDTH 40

static SXmlConfig g_SvgLibXml;

CSvgEmbeddedWnd::CSvgEmbeddedWnd(QWidget *parent/* =NULL */) : SSvgWnd(parent)
{
	SetShowRootRect(false);
	m_pSSvgLibraryWnd = NULL;
}
CSvgEmbeddedWnd::~CSvgEmbeddedWnd()
{

}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ����SVGͼԪ
// ��    ��:  �ۿ���
// ����ʱ��:  2015-8-20 10:45
// ����˵��:  @sSvgTextΪSVG�ı�����
// �� �� ֵ:  true/false
//////////////////////////////////////////////////////////////////////////
bool CSvgEmbeddedWnd::LoadSvg(char *sSvgText,bool bAutoAddSvgHead)
{
	SString sSvg;
	if(bAutoAddSvgHead)
	{
		sSvg = "<?xml version=\"1.0\" encoding=\"GBK\"?>\r\n"
		"<svg width=\"\" height=\"\" >\r\n"
		"<rect uri=\"\" caption=\"\" x=\"0\" y=\"0\" z=\"\" width=\"1000\" height=\"1500\" stroke=\"#000000\" stroke-width=\"1\" stroke-dasharray=\"0\" fill=\"\" />\r\n";
		sSvg += sSvgText;
		sSvg += "</svg>\r\n";
		if(!SSvgWnd::LoadText(sSvg.data()))
			return false;
	}
	else
	{
		if(!SSvgWnd::LoadText(sSvgText))
			return false;
	}
	SSvgObject *pRangeRect = m_Document.GetFirstObject();
	if(pRangeRect == NULL)
		return false;
	SSvgObject::SVG_RECT rect;
	memset(&rect,0,sizeof(rect));
	SSvgObject *pObj = pRangeRect->GetNext();
	if(pObj != NULL)
	{
		rect = *pObj->GetRect();
		pObj = pObj->GetNext();
	}
	while(pObj)
	{
		if(pObj->GetRect()->left < rect.left)
			rect.left = pObj->GetRect()->left;
		if(pObj->GetRect()->top < rect.top)
			rect.top = pObj->GetRect()->top;
		if(pObj->GetRect()->right > rect.right)
			rect.right = pObj->GetRect()->right;
		if(pObj->GetRect()->bottom > rect.bottom)
			rect.bottom = pObj->GetRect()->bottom;
		pObj = pObj->GetNext();
	}
	pRangeRect->SetAttributeValue("width",SString::toFormat("%f",rect.right-rect.left));
	pRangeRect->SetAttributeValue("height",SString::toFormat("%f",rect.bottom-rect.top));
	pRangeRect->CalcObjectRect();

	pObj = pRangeRect->GetNext();
	while(pObj)
	{
		pObj->Offset(-rect.left,-rect.top);
		pObj->CalcObjectRect();
		pObj = pObj->GetNext();
	}
	ToFull();
	ToCenter();
	RedrawWindow();
	return true;
}
int CSvgEmbeddedWnd::OnLButtonDown(int nFlag,SPoint point)
{
	SSvgWnd::OnLButtonDblClk(nFlag,point);
	((SSvgLibraryWnd*)m_pSSvgLibraryWnd)->OnClickObject(m_pSvgNode);
	return 0;
}

void CSvgFilterEdit::keyPressEvent(QKeyEvent* event)
{
	QLineEdit::keyPressEvent(event);
	((SSvgLibraryWnd*)m_pWnd)->OnFilterChanged();
};
void CSvgFilterEdit::dragEnterEvent(QDragEnterEvent* e)
{
	S_UNUSED(e);
};
void CSvgFilterEdit::dragLeaveEvent(QDragLeaveEvent* e)
{
	S_UNUSED(e);
};
void CSvgFilterEdit::dragMoveEvent(QDragMoveEvent* e)
{
	S_UNUSED(e);
};


SSvgLibraryWnd::SSvgLibraryWnd(QWidget *parent) : QWidget(parent)
{
	ui.setupUi(this);
	m_bUseSymbol = false;
	m_pSvgWnd = NULL;
	ui.edtFilter->m_pWnd = this;
	SQt::SetTableWidgetStyle(ui.tabItems);
	ui.tabItems->setColumnCount(3);
	QStringList slHeader;
	slHeader << "" << tr("ͼԪ") << tr("����");
	ui.tabItems->setHorizontalHeaderLabels(slHeader);
	QHeaderView *headerView = ui.tabItems->horizontalHeader();
#ifndef QT_V5
	headerView->setMovable(false);//ȥ����ͷ���ƶ�
#endif
	headerView->resizeSection(0,0);//���õ�һ�п�
	headerView->resizeSection(1,C_SVG_LIB_WIDTH);//���õڶ��п�
	headerView->resizeSection(2,220);//���õ����п�
	ui.tabItems->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//ȥ��ˮƽ������
	ui.tabItems->setAutoScroll(false);//ȥ���Զ�����
	ui.tabItems->verticalHeader()->setVisible(false);
	ui.tabItems->horizontalHeader()->setVisible(false); 
	ui.tabItems->setEditTriggers(QTableWidget::NoEditTriggers);//���ܱ༭
	connect(ui.cmbType,SIGNAL(currentIndexChanged(int)),this,SLOT(OnCurrentIndexChanged(int)));
	connect(ui.tabItems,SIGNAL(currentCellChanged(int,int,int,int)),this,SLOT(OnTableCurrentCellChanged(int,int,int,int)));
	connect(ui.tabItems,SIGNAL(cellClicked(int,int)),this,SLOT(OnTableCellClicked(int,int)));
	g_SvgLibXml.SetShared(true);
	LoadSvgLib();
}

SSvgLibraryWnd::~SSvgLibraryWnd()
{

}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡȫ������ʵ��ָ��
// ��    ��:  �ۿ���
// ����ʱ��:  2015-10-26 11:15
// ����˵��:  
// �� �� ֵ:  
//////////////////////////////////////////////////////////////////////////
SXmlConfig* SSvgLibraryWnd::GetGlobalXmlConfig()
{
	return &g_SvgLibXml;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ���ļ�����SVGͼԪ������
// ��    ��:  �ۿ���
// ����ʱ��:  2015-8-19 20:14
// ����˵��:  @sFileΪ�ļ���
// �� �� ֵ:  true��ʾ���سɹ���false��ʾ����ʧ��
//////////////////////////////////////////////////////////////////////////
bool SSvgLibraryWnd::LoadByFile(SString sFile)
{
	int nRead;
	char *sXMLData;

	SFile file(sFile);
	if(!file.open(IO_ReadOnly))
	{
		return false;
	}
	int filelen = file.size();
	sXMLData = new char[filelen+1];
	memset(sXMLData,0,filelen+1);
	for(nRead=0;nRead<filelen;)
	{
		int size = filelen - nRead;
		if(size > 10240)
			size = 10240;
		int ret = file.readBlock((BYTE*)(sXMLData+nRead),size);
		if(ret <= 0)
			break;
		nRead += ret;
	}
	file.close();

	sXMLData[nRead] = 0;

	char *pEncoding = strstr(sXMLData,"encoding=");
	if(pEncoding != NULL)
	{
		pEncoding += 9;
		while(*pEncoding != '\'' && *pEncoding != '\"' && *pEncoding != '?' )
			pEncoding++;
		if(SString::toString(pEncoding,5).toUpper() == "UTF-8")
		{
			//UTF-8����
#ifdef WIN32
			//Windows��ֻ��ת��һ��
			SString::Utf82Gb(sXMLData,filelen+1);
			filelen = strlen(sXMLData);
#else
			//char *pSrc = sXMLData;
			SString sTemp,sNew;
			int i=0,cnt=nRead;
			int s;
			int size = 10240;//10240
			sNew.SetBuffer(filelen);
			for(i=0;i<cnt;i+=s)
			{
				s = nRead-i;
				if(s > size)
					s = size;
				while(((unsigned char)sXMLData[i+s-1]) >= 128 && s > size/2)
				{
					s --;
				}
				sTemp = SString::toString(sXMLData+i,s);
				sNew += sTemp.toGb2312();
			}
			char *pNewStr = sNew.data();
			filelen = strlen(pNewStr);
			while(filelen > 0 && *pNewStr != '\0' && *pNewStr != '<')
			{
				pNewStr++;
				filelen--;
			}
			memcpy(sXMLData,pNewStr,filelen);
			sXMLData[filelen] = '\0';
#endif
		}
	}
	bool ret = LoadByXmlText(sXMLData);

	delete[] sXMLData;
	return ret;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ���ļ�����ȱʡ��ͼԪ�⣬��̬����
// ��    ��:  �ۿ���
// ����ʱ��:  2015-10-26 9:31
// ����˵��:  @sFileΪ�ļ���
// �� �� ֵ:  true��ʾ���سɹ���false��ʾ����ʧ��
//////////////////////////////////////////////////////////////////////////
bool SSvgLibraryWnd::LoadDefaultByFile(SString sFile)
{
	int nRead;
	char *sXMLData;
	SFile file(sFile);
	if(!file.open(IO_ReadOnly))
	{
		return false;
	}
	int filelen = file.size();
	sXMLData = new char[filelen+1];
	memset(sXMLData,0,filelen+1);
	for(nRead=0;nRead<filelen;)
	{
		int size = filelen - nRead;
		if(size > 10240)
			size = 10240;
		int ret = file.readBlock((BYTE*)(sXMLData+nRead),size);
		if(ret <= 0)
			break;
		nRead += ret;
	}
	file.close();

	sXMLData[nRead] = 0;

	char *pEncoding = strstr(sXMLData,"encoding=");
	if(pEncoding != NULL)
	{
		pEncoding += 9;
		while(*pEncoding != '\'' && *pEncoding != '\"' && *pEncoding != '?' )
			pEncoding++;
		if(SString::toString(pEncoding,5).toUpper() == "UTF-8")
		{
			//UTF-8����
#ifdef WIN32
			//Windows��ֻ��ת��һ��
			SString::Utf82Gb(sXMLData,filelen+1);
			filelen = strlen(sXMLData);
#else
			//char *pSrc = sXMLData;
			SString sTemp,sNew;
			int i=0,cnt=nRead;
			int s;
			int size = 10240;//10240
			sNew.SetBuffer(filelen);
			for(i=0;i<cnt;i+=s)
			{
				s = nRead-i;
				if(s > size)
					s = size;
				while(((unsigned char)sXMLData[i+s-1]) >= 128 && s > size/2)
				{
					s --;
				}
				sTemp = SString::toString(sXMLData+i,s);
				sNew += sTemp.toGb2312();
			}
			char *pNewStr = sNew.data();
			filelen = strlen(pNewStr);
			while(filelen > 0 && *pNewStr != '\0' && *pNewStr != '<')
			{
				pNewStr++;
				filelen--;
			}
			memcpy(sXMLData,pNewStr,filelen);
			sXMLData[filelen] = '\0';
#endif
		}
	}
	bool ret = LoadDefaltByXmlText(sXMLData);

	delete[] sXMLData;
	return ret;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ��XML�ı�����ȱʡͼԪ�����ã���̬����
// ��    ��:  �ۿ���
// ����ʱ��:  2015-10-26 9:31
// ����˵��:  @sFileΪ�ļ���
// �� �� ֵ:  true��ʾ���سɹ���false��ʾ����ʧ��
//////////////////////////////////////////////////////////////////////////
bool SSvgLibraryWnd::LoadDefaltByXmlText(char* sXmlText)
{
	g_SvgLibXml.SetShared(true);
	g_SvgLibXml.clear();
	if(!g_SvgLibXml.ReadText(sXmlText))
		return false;
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ��XML�ı�����ͼԪ������
// ��    ��:  �ۿ���
// ����ʱ��:  2015-8-19 20:15
// ����˵��:  @sXmlTextΪXML����
// �� �� ֵ:  true��ʾ���سɹ���false��ʾ����ʧ��
//////////////////////////////////////////////////////////////////////////
bool SSvgLibraryWnd::LoadByXmlText(char* sXmlText)
{
	g_SvgLibXml.lock();
	g_SvgLibXml.clear();
	if(!g_SvgLibXml.ReadText(sXmlText))
	{
		g_SvgLibXml.unlock();
		return false;
	}
	g_SvgLibXml.unlock();
	LoadSvgLib();
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ����ȫ�ֵ�SVG�����ã�����SVGͼԪ��
// ��    ��:  �ۿ���
// ����ʱ��:  2015-10-23 17:46
// ����˵��:  void
// �� �� ֵ:  bool
//////////////////////////////////////////////////////////////////////////
bool SSvgLibraryWnd::LoadSvgLib()
{
	m_pThisType = NULL;
	while(ui.cmbType->count() > 0)
		ui.cmbType->removeItem(0);
	ui.tabItems->setRowCount(0);

	ui.cmbType->addItem(tr("<ȫ��>"));
	unsigned long pos;
	g_SvgLibXml.lock();
	SBaseConfig *pSvgType = g_SvgLibXml.GetChildPtr()->FetchFirst(pos);
	while(pSvgType)
	{
		if(pSvgType->GetNodeName() == "svg-type")
		{
			ui.cmbType->addItem(pSvgType->GetAttribute("name").data());
		}
		pSvgType = g_SvgLibXml.GetChildPtr()->FetchNext(pos);
	}
	g_SvgLibXml.unlock();
	ui.cmbType->setCurrentIndex(ui.cmbType->count()<=1?0:1);
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �ӵ�ǰ�򿪵�SVG�ļ���ˢ�¶�Ӧ������ͼ�Σ����������<�ڲ�����ͼԪ>ҳ�����û��ȥ����ҳ
// ��    ��:  �ۿ���
// ����ʱ��:  2016-11-24 16:55
// ����˵��:  void
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SSvgLibraryWnd::RefreshSvgSymbol()
{
	SString sSymbolTabName = "<�ڲ�����ͼԪ>";
	int iSymbolIdx = -1;
	for(int i=0;i<ui.cmbType->count();i++)
	{
		if(sSymbolTabName == ui.cmbType->itemText(i).toStdString().c_str())
		{
			iSymbolIdx = i;
			break;
		}
	}
	if(m_pSvgWnd->m_Document.GetSymbolObjectsPtr()->count() == 0)
	{
		//û�ж����������ͼԪ���������<�ڲ�����ͼԪ>ҳ��ȥ��֮
		if(iSymbolIdx >= 0)
			ui.cmbType->removeItem(iSymbolIdx);
		m_slSymbolObjectsText.clear();
	}
	else
	{
		//������ͼԪ�����������TABҳ������
		if(iSymbolIdx < 0)
			ui.cmbType->addItem(sSymbolTabName.data());
		m_slSymbolObjectsText.clear();
		unsigned long pos;
		SString sName;
		SSvgObject *pObj = m_pSvgWnd->m_Document.GetSymbolObjectsPtr()->FetchFirst(pos);
		while(pObj)
		{
			SString sOutSvgText;
			sName = pObj->GetName();
			int type = pObj->GetType();
			pObj->SetName("svg");
			pObj->SetType(SSvgObject::TYPE_SVG);
			m_pSvgWnd->m_Document.WriteSvgNode(pObj,NULL,0,&sOutSvgText);
			int p = sOutSvgText.find(">");
			sOutSvgText = "<?xml version=\"1.0\" encoding=\"GBK\"?>\r\n" + sOutSvgText.left(p+1)+
				"<rect uri=\"\" caption=\"\" x=\"0\" y=\"0\" z=\"\" width=\"1000\" height=\"1500\" stroke=\"#000000\" stroke-width=\"1\" stroke-dasharray=\"0\" fill=\"\" />\r\n"+
				sOutSvgText.mid(p+1);
			m_slSymbolObjectsText.append(sOutSvgText);
			pObj->SetName(sName);
			pObj->SetType(type);
			pObj = m_pSvgWnd->m_Document.GetSymbolObjectsPtr()->FetchNext(pos);
		}
	}
}

void SSvgLibraryWnd::OnTableCurrentCellChanged ( int currentRow, int currentColumn, int previousRow, int previousColumn )
{
	S_UNUSED(currentColumn);
	S_UNUSED(previousColumn);
	S_UNUSED(previousRow);
	if(currentRow < 0)
		return;
	SSvgObject *pObj;
	CSvgEmbeddedWnd *pWnd = (CSvgEmbeddedWnd*)(ui.tabItems->cellWidget(currentRow,1));
	if(m_bUseSymbol)
	{
		//���÷���ͼԪ
		pObj = &m_TempUseObject;
		m_TempUseObject.SetName("use");
		m_TempUseObject.SetType(SSvgObject::TYPE_USE);
		pObj->SetAttributeValue("xlink:href",pWnd->m_Document.GetRootObject()->GetAttribute("id"));
	}
	else
	{
		pObj = pWnd->m_Document.GetFirstObject();
		if(pObj != NULL)
			pObj = pObj->GetNext();
	}
	if(m_pSvgWnd != NULL && pObj != NULL)
		m_pSvgWnd->SetTempCopyObject(pObj);
}

void SSvgLibraryWnd::OnTableCellClicked( int row, int column )
{
	S_UNUSED(column);
	OnTableCurrentCellChanged(row,0,0,0);
}

void SSvgLibraryWnd::OnCurrentIndexChanged(int index)
{
	if(index < 0)
		return;
	SString sType;
	sType = ui.cmbType->itemText(index).toStdString().data();
	SBaseConfig *p = NULL;
	m_bUseSymbol = false;
	g_SvgLibXml.lock();
	if(sType == Q2C(tr("<�ڲ�����ͼԪ>")))
	{
		m_bUseSymbol = true;
	}
	if(m_bUseSymbol == false && sType != Q2C(tr("<ȫ��>")))
	{
		p = g_SvgLibXml.SearchChild(SString::toFormat("svg-type<name='%s'>",sType.data()));
		if(p == NULL)
		{
			g_SvgLibXml.unlock();
			return;
		}
	}
	m_pThisType = p;
	if(m_pSvgWnd != NULL)
		m_pSvgWnd->SetTempCopyObject(NULL);
	ui.tabItems->setRowCount(0);
	if(m_bUseSymbol)
	{
		g_SvgLibXml.unlock();
		for(int i=0;i<(int)m_slSymbolObjectsText.count();i++)
		{
			int row = ui.tabItems->rowCount();
			ui.tabItems->insertRow(row);
			ui.tabItems->setRowHeight(row,C_SVG_LIB_WIDTH);
			ui.tabItems->setItem(row,0,new QTableWidgetItem(SString::toFormat("%ld",(unsigned long)p).data()));
			QTableWidgetItem *pNewItem = new QTableWidgetItem("");
			CSvgEmbeddedWnd *pNewWnd = new CSvgEmbeddedWnd(ui.tabItems);
			pNewWnd->m_pSSvgLibraryWnd = this;
			pNewWnd->m_pSvgNode = p;
			ui.tabItems->setItem(row,1,pNewItem);
			ui.tabItems->setCellWidget(row,1,pNewWnd);
			pNewWnd->setMaximumWidth(C_SVG_LIB_WIDTH);
			pNewWnd->setMaximumHeight(C_SVG_LIB_WIDTH);
			pNewWnd->setMinimumWidth(C_SVG_LIB_WIDTH);
			pNewWnd->setMinimumHeight(C_SVG_LIB_WIDTH);
			pNewWnd->setFocusProxy(this);
			pNewWnd->LoadSvg(m_slSymbolObjectsText.at(i)->data(),false);
			SString sName;
			if(pNewWnd->m_Document.GetRootObject() != NULL)
				sName = pNewWnd->m_Document.GetRootObject()->GetAttribute("id");
			ui.tabItems->setItem(row,2,new QTableWidgetItem(sName.data()));
		}
		OnFilterChanged();
		return;
	}
	unsigned long pos,pos2;
	SBaseConfig *pSvgType = g_SvgLibXml.GetChildPtr()->FetchFirst(pos);
	while(pSvgType)
	{
		if(pSvgType->GetNodeName() != "svg-type" || (m_pThisType != NULL && m_pThisType != pSvgType))
		{
			pSvgType = g_SvgLibXml.GetChildPtr()->FetchNext(pos);
			continue;
		}
		p = pSvgType->GetChildPtr()->FetchFirst(pos2);
		while(p)
		{
			if(p->GetNodeName() != "svg")
			{
				p = pSvgType->GetChildPtr()->FetchNext(pos2);
				continue;
			}
			int row = ui.tabItems->rowCount();
			ui.tabItems->insertRow(row);
			ui.tabItems->setRowHeight(row,C_SVG_LIB_WIDTH);
			ui.tabItems->setItem(row,0,new QTableWidgetItem(SString::toFormat("%ld",(unsigned long)p).data()));
			QTableWidgetItem *pNewItem = new QTableWidgetItem("");
			CSvgEmbeddedWnd *pNewWnd = new CSvgEmbeddedWnd(ui.tabItems);
			pNewWnd->m_pSSvgLibraryWnd = this;
			pNewWnd->m_pSvgNode = p;
			ui.tabItems->setItem(row,1,pNewItem);
			ui.tabItems->setCellWidget(row,1,pNewWnd);
			pNewWnd->setMaximumWidth(C_SVG_LIB_WIDTH);
			pNewWnd->setMaximumHeight(C_SVG_LIB_WIDTH);
			pNewWnd->setMinimumWidth(C_SVG_LIB_WIDTH);
			pNewWnd->setMinimumHeight(C_SVG_LIB_WIDTH);
			pNewWnd->setFocusProxy(this);
			pNewWnd->LoadSvg(p->GetNodeValue().data());
			ui.tabItems->setItem(row,2,new QTableWidgetItem(p->GetAttribute("name").data()));

			p = pSvgType->GetChildPtr()->FetchNext(pos2);
		}

		pSvgType = g_SvgLibXml.GetChildPtr()->FetchNext(pos);
	}
	g_SvgLibXml.unlock();
	OnFilterChanged();
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �����ı��ı䴦��
// ��    ��:  �ۿ���
// ����ʱ��:  2015-8-20 13:08
// ����˵��:  
// �� �� ֵ:  
//////////////////////////////////////////////////////////////////////////
void SSvgLibraryWnd::OnFilterChanged()
{
	SString sFilterText = ui.edtFilter->text().toStdString().data();
	SString sText;
	int i,cnt = ui.tabItems->rowCount();
	for(i=0;i<cnt;i++)
	{
		sText = ui.tabItems->item(i,2)->text().toStdString().data();
		if(SChinesePy::IsMatchText(sText.data(),sFilterText.data()))
			ui.tabItems->setRowHidden(i,false);
		else
			ui.tabItems->setRowHidden(i,true);
	}
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �������
// ��    ��:  �ۿ���
// ����ʱ��:  2015-8-21 13:39
// ����˵��:  
// �� �� ֵ:  
//////////////////////////////////////////////////////////////////////////
void SSvgLibraryWnd::OnClickObject(SBaseConfig *pSvgNode)
{
	int i,cnt = ui.tabItems->rowCount();
	for(i=0;i<cnt;i++)
	{
		SBaseConfig *p = (SBaseConfig*)atol(ui.tabItems->item(i,0)->text().toStdString().data());
		if(p == pSvgNode)
		{
			ui.tabItems->setCurrentCell(i,0);
			break;
		}
	}
}

