/**
 *
 * 文 件 名 : SSvgDocument.h
 * 创建日期 : 2007-04-08 18:55
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : SVG文档类
 * 修改记录 : 
 *            $Log: $
 *
 **/

#include "SSvgDocument.h"
#include "SQt.h"
#include "SSvgPainter.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void SSvgDocument::stuGradientStop::Set(SBaseConfig *pStop)
{
	offset = pStop->GetAttributeI("offset");
	color = SSvgPainter::GetColorByString(pStop->GetAttribute("stop-color"));
	opacity = pStop->GetAttributeI("stop-opacity");
	SString style = pStop->GetAttribute("style");
	if(style.length() > 0)
	{
		SString str = SString::GetAttributeValue(style,"stop-color",":",";");
		if(str.length() > 0)
			color = SSvgPainter::GetColorByString(str);
		str = SString::GetAttributeValue(style,"stop-opacity",":",";");
		if(str.length() > 0)
			opacity = str.toInt();
	}
}

SSvgDocument::SSvgDocument()
{
	m_pSvgRoot = NULL;
	m_AllObjects.setAutoDelete(false);
	m_DeletedObjects.setAutoDelete(false);
	m_ImageDefs.setAutoDelete(true);
	m_SymbolObjects.setAutoDelete(false);
	m_LinearGradients.setAutoDelete(true);
	m_RadialGradients.setAutoDelete(true);
}

SSvgDocument::~SSvgDocument()
{
	RemoveObjects();
	m_DeletedObjects.setAutoDelete(true);
	m_DeletedObjects.clear();
}
int g_iSvgDefaultZOrder=1;
bool SSvgDocument::LoadSvgFile(SString sSvgFile)
{
	if(sSvgFile == "")
	{
		return NewFile();
	}
	{
		//针对南思无效XML头的处理
		FILE *fp = fopen(sSvgFile.GetBuffer(0),"r+");
		if(fp == NULL)
			return false;
		fseek(fp,39,SEEK_SET);
		char buf[128];
		memset(buf,0,sizeof(buf));
		fread(buf,105,1,fp);
		SString str="";
		str+=buf;
		memset(buf,' ',sizeof(buf));
		if(str == "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.0//EN\" \"http://www.w3.org/TR/2001/PR-SVG-20010719/DTD/svg10.dtd\">")
		{
			fseek(fp,39,SEEK_SET);
			fwrite(buf,105,1,fp);
		}
		fclose(fp);
	}
	SXmlConfig xml;
	if(!xml.ReadConfig(sSvgFile))
		return false;
	m_sSvgFile = sSvgFile;
	m_sSvgPath = m_sSvgFile;
	int p0 = m_sSvgPath.findRev('/');
	if(p0>0)
	{
		m_sSvgPath = m_sSvgPath.Left(p0);
	}
	else
	{
		p0 = m_sSvgPath.findRev('\\');
		if(p0>0)
			m_sSvgPath = m_sSvgPath.Left(p0);
	}
	m_sSvgPath += "/";

	SBaseConfig* pRootNode=&xml;
	LoadSvgCfg(pRootNode);

	/*
	g_iSvgDefaultZOrder = 1;
	RemoveObjects(true);
	if(m_pSvgRoot == NULL)
		m_pSvgRoot = NewObject();
	SString viewBox = pRootNode->GetAttribute("viewBox");
	if(viewBox.length() > 0 || (pRootNode->GetAttributeI("width")>0 && pRootNode->GetAttributeI("height")>0))
	{
		int w = SString::GetIdAttributeI(3,viewBox," ");
		int h = SString::GetIdAttributeI(4,viewBox," ");
		if(w==0||h==0)
		{
			w = pRootNode->GetAttributeI("width");
			h = pRootNode->GetAttributeI("height");
		}
		if(w > 0 && h > 0)
		{
			//添加一个首对象
			SSvgObject *pFirst = NewObject();
			m_pSvgRoot->AddChild(pFirst);
			pFirst->SetName("rect");
			pFirst->SetAttributeValue("x","0");
			pFirst->SetAttributeValue("y","0");
			pFirst->SetAttributeValue("width",SString::toFormat("%d",w).data());
			pFirst->SetAttributeValue("height",SString::toFormat("%d",h).data());
			pFirst->SetAttributeValue("stroke","#333333");
			pFirst->SetAttributeValue("z","-100");
			pFirst->SetAttributeValue("stroke-width","1");
			pFirst->SetAttributeValue("stroke-dasharray","0");
			pFirst->SetAttributeValue("fill","#ffffff");
		}
	}
	ReadSvgNode(m_pSvgRoot, pRootNode);
	if(m_pSvgRoot)
	{
		unsigned long pos2;
		SBaseConfig::SConfigAttribute *pAttr = pRootNode->GetAttributePtr()->FetchFirst(pos2);
		while(pAttr)
		{
			m_pSvgRoot->SetAttributeValue(pAttr->m_sName.data(),pAttr->m_sValue.data(),false,false);				
			pAttr = pRootNode->GetAttributePtr()->FetchNext(pos2);
		}
		m_pSvgRoot->SetName("svg");
		m_pSvgRoot->BuildObject(m_pSvgRoot);
		m_pSvgRoot->CalcObjectRect();
		m_iCoorType = (m_pSvgRoot->GetAttribute("coor-type") == "left_bottom")?1:0;
		m_fOrigX = m_pSvgRoot->GetAttributeF("orig-x");
		m_fOrigY = m_pSvgRoot->GetAttributeF("orig-y");
	}
	SSvgObject *p = GetFirstObject();
	if(p != NULL)
	{
		SString sFill = p->GetAttribute("fill");
		SCOLOR col = SSvgPainter::GetColorByString(sFill);
		col.r = 255 - col.r;
		col.g = 255 - col.g;
		col.b = 255 - col.b;
		sFill = SSvgPainter::GetStringByColor(col);
		p->SetAttributeValue("stroke",sFill);
		m_fWidth = p->GetAttributeF("width");
		m_fHeight = p->GetAttributeF("height");
	}
	*/
	return true;
}

bool SSvgDocument::LoadSvgCfg(SBaseConfig *pRootNode)
{
	g_iSvgDefaultZOrder = 1;
	RemoveObjects(true);
	if(m_pSvgRoot == NULL)
		m_pSvgRoot = NewObject();
	SString viewBox = pRootNode->GetAttribute("viewBox");
	if(viewBox.length() > 0 || (pRootNode->GetAttributeI("width")>0 && pRootNode->GetAttributeI("height")>0))
	{
		int w = SString::GetIdAttributeI(3,viewBox," ");
		int h = SString::GetIdAttributeI(4,viewBox," ");
		if(w==0||h==0)
		{
			w = pRootNode->GetAttributeI("width");
			h = pRootNode->GetAttributeI("height");
		}
		if(w > 0 && h > 0)
		{
			//判断第一个rect是否存在宽高与svg相同的情况，存在则不添加首对象
			unsigned long pos;
			bool bMatch=false;
			SBaseConfig *p = pRootNode->GetChildPtr()->FetchFirst(pos);
			while(p)
			{
				if(p->GetNodeName() == "rect")
				{
					if(p->GetAttributeI("width") == w && p->GetAttributeI("height") == h)
						bMatch = true;
				}
				p = pRootNode->GetChildPtr()->FetchNext(pos);
			}
			if(!bMatch)
			{
				//添加一个首对象
				SSvgObject *pFirst = NewObject();
				m_pSvgRoot->AddChild(pFirst);
				pFirst->SetName("rect");
				pFirst->SetAttributeValue("x","0");
				pFirst->SetAttributeValue("y","0");
				pFirst->SetAttributeValue("width",SString::toFormat("%d",w).data());
				pFirst->SetAttributeValue("height",SString::toFormat("%d",h).data());
				pFirst->SetAttributeValue("stroke","#333333");
				pFirst->SetAttributeValue("z","-100");
				pFirst->SetAttributeValue("stroke-width","1");
				pFirst->SetAttributeValue("stroke-dasharray","0");
				pFirst->SetAttributeValue("fill","#ffffff");
			}
		}
	}
	ReadSvgNode(m_pSvgRoot, pRootNode);
	if(m_pSvgRoot)
	{
		unsigned long pos2;
		SBaseConfig::SConfigAttribute *pAttr = pRootNode->GetAttributePtr()->FetchFirst(pos2);
		while(pAttr)
		{
			m_pSvgRoot->SetAttributeValue(pAttr->m_sName.data(),pAttr->m_sValue.data(),false,false);				
			pAttr = pRootNode->GetAttributePtr()->FetchNext(pos2);
		}
		m_pSvgRoot->SetName("svg");
		m_pSvgRoot->BuildObject(m_pSvgRoot);
		m_pSvgRoot->CalcObjectRect();
		m_iCoorType = (m_pSvgRoot->GetAttribute("coor-type") == "left_bottom")?1:0;
		m_fOrigX = m_pSvgRoot->GetAttributeF("orig-x");
		m_fOrigY = m_pSvgRoot->GetAttributeF("orig-y");
	}
	SSvgObject *p = GetFirstObject();
	if(p != NULL)
	{
		SString sFill = p->GetAttribute("fill");
		SCOLOR col = SSvgPainter::GetColorByString(sFill);
		col.r = 255 - col.r;
		col.g = 255 - col.g;
		col.b = 255 - col.b;
		sFill = SSvgPainter::GetStringByColor(col);
		p->SetAttributeValue("stroke",sFill);
		m_fWidth = p->GetAttributeF("width");
		m_fHeight = p->GetAttributeF("height");
	}
	return true;

	/*
	g_iSvgDefaultZOrder=1;
	RemoveObjects(true);
	if(m_pSvgRoot == NULL)
		m_pSvgRoot = NewObject();
	ReadSvgNode(m_pSvgRoot, pRootNode);
	if(m_pSvgRoot)
	{
		unsigned long pos2;
		SBaseConfig::SConfigAttribute *pAttr = pRootNode->GetAttributePtr()->FetchFirst(pos2);
		while(pAttr)
		{
			m_pSvgRoot->SetAttributeValue(pAttr->m_sName.data(),pAttr->m_sValue.data(),false,false);				
			pAttr = pRootNode->GetAttributePtr()->FetchNext(pos2);
		}
		m_pSvgRoot->SetName("svg");
		m_pSvgRoot->BuildObject(m_pSvgRoot);
		m_pSvgRoot->CalcObjectRect();
		m_iCoorType = (m_pSvgRoot->GetAttribute("coor-type") == "left_bottom")?1:0;
		m_fOrigX = m_pSvgRoot->GetAttributeF("orig-x");
		m_fOrigY = m_pSvgRoot->GetAttributeF("orig-y");
	}
	SSvgObject *p = GetFirstObject();
	if(p != NULL)
	{
		SString sFill = p->GetAttribute("fill");
		SCOLOR col = SSvgPainter::GetColorByString(sFill);
		col.r = 255 - col.r;
		col.g = 255 - col.g;
		col.b = 255 - col.b;
		sFill = SSvgPainter::GetStringByColor(col);
		p->SetAttributeValue("stroke",sFill);
		m_fWidth = p->GetAttributeF("width");
		m_fHeight = p->GetAttributeF("height");
	}
	return true;
	*/
}
bool SSvgDocument::LoadSvgText(const char* sSvgText)
{
	g_iSvgDefaultZOrder = 1;
	if(sSvgText == NULL || sSvgText[0] == '\0')
	{
		return NewFile();
	}
	RemoveObjects(true);

	SXmlConfig xml;
	if(!xml.ReadText(sSvgText))
		return false;

	SBaseConfig* pRootNode=&xml;
	LoadSvgCfg(pRootNode);
	/*
	if(m_pSvgRoot == NULL)
		m_pSvgRoot = NewObject();
	SString viewBox = pRootNode->GetAttribute("viewBox");
	if(viewBox.length() > 0 || (pRootNode->GetAttributeI("width")>0 && pRootNode->GetAttributeI("height")>0))
	{
		int w = SString::GetIdAttributeI(3,viewBox," ");
		int h = SString::GetIdAttributeI(4,viewBox," ");
		if(w==0||h==0)
		{
			w = pRootNode->GetAttributeI("width");
			h = pRootNode->GetAttributeI("height");
		}
		if(w > 0 && h > 0)
		{
			//添加一个首对象
			SSvgObject *pFirst = NewObject();
			m_pSvgRoot->AddChild(pFirst);
			pFirst->SetName("rect");
			pFirst->SetAttributeValue("x","0");
			pFirst->SetAttributeValue("y","0");
			pFirst->SetAttributeValue("width",SString::toFormat("%d",w).data());
			pFirst->SetAttributeValue("height",SString::toFormat("%d",h).data());
			pFirst->SetAttributeValue("stroke","#333333");
			pFirst->SetAttributeValue("z","-100");
			pFirst->SetAttributeValue("stroke-width","1");
			pFirst->SetAttributeValue("stroke-dasharray","0");
			pFirst->SetAttributeValue("fill","#ffffff");
		}
	}
	ReadSvgNode(m_pSvgRoot, pRootNode);
	if(m_pSvgRoot)
	{
		unsigned long pos2;
		SBaseConfig::SConfigAttribute *pAttr = pRootNode->GetAttributePtr()->FetchFirst(pos2);
		while(pAttr)
		{
			m_pSvgRoot->SetAttributeValue(pAttr->m_sName.data(),pAttr->m_sValue.data(),false,false);				
			pAttr = pRootNode->GetAttributePtr()->FetchNext(pos2);
		}

		m_pSvgRoot->SetName("svg");
		if(xml.IsAttribute("id"))
		{
			SString id = xml.GetAttribute("id");
			m_pSvgRoot->SetAttributeValue("id",id,false,false);
		}
		m_pSvgRoot->BuildObject(m_pSvgRoot);
		m_pSvgRoot->CalcObjectRect();
		m_iCoorType = (m_pSvgRoot->GetAttribute("coor-type") == "left_bottom")?1:0;
		m_fOrigX = m_pSvgRoot->GetAttributeF("orig-x");
		m_fOrigY = m_pSvgRoot->GetAttributeF("orig-y");
	}
	SSvgObject *p = GetFirstObject();
	if(p != NULL)
	{
		SString sFill = p->GetAttribute("fill");
		SCOLOR col = SSvgPainter::GetColorByString(sFill);
		col.r = 255 - col.r;
		col.g = 255 - col.g;
		col.b = 255 - col.b;
		sFill = SSvgPainter::GetStringByColor(col);
		p->SetAttributeValue("stroke",sFill);
		m_fWidth = p->GetAttributeF("width");
		m_fHeight = p->GetAttributeF("height");
	}
	*/
	return true;
}

bool SSvgDocument::SaveSvgFileAsUtf8(SString sSvgFile)
{
	return SaveSvgFile(sSvgFile,true);
}

//保存SVG到字符串
bool SSvgDocument::SaveSvgFileToText(SString &sSvgText,bool bUtf8)
{
	m_bUtf8 = bUtf8;
	SString sLine;
	sSvgText = "";
	sSvgText += "<?xml version=\"1.0\" encoding=\"GBK\"?>\r\n";
	sSvgText += "<!--\r\n";
	SString str;
	str = "Generated by : Skt-SVG Editor\r\n";
	sSvgText += str;
	sSvgText += "Date         : "+SDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")+"\r\n";
	sSvgText += "Version      : 1.0.4.18\r\n";
	sSvgText += "-->\r\n";
	if(m_pSvgRoot != NULL)
		WriteSvgNode(m_pSvgRoot,NULL,0,&sSvgText);
	else
	{
		sSvgText += "<svg />\r\n";
	}
	return true;
}

bool SSvgDocument::SaveSvgFile(SString sSvgFile,bool bUtf8/*=false*/)
{
	//return SaveSvgFileAsUtf8(sSvgFile);
	m_bUtf8 = bUtf8;
	SString sLine;
	if(sSvgFile == "")
		sSvgFile = m_sSvgFile;
	SFile file(sSvgFile);
	if(!file.open(IO_Truncate))
	{
		return false;
	}

	file.writeString("<?xml version=\"1.0\" encoding=\"GBK\"?>\r\n");
	file.writeString("<!--\r\n");
	SString str;
	str = "Generated by : Skt-SVG Editor\r\n";
	file.writeString(str);
	file.writeString("Date         : "+SDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")+"\r\n");
	file.writeString("Version      : 1.0.4.18\r\n");
	file.writeString("-->\r\n");
	if(m_pSvgRoot != NULL)
		WriteSvgNode(m_pSvgRoot,&file,0);
	else
	{
		file.writeString("<svg />\r\n");
	}
	file.close();
	return true;
}

//递归写SVG节点
bool SSvgDocument::WriteSvgNode(SSvgObject *pSvgObj, SFile *pFile, int iLevel,SString *pOutText/*=NULL*/)
{
	if(pSvgObj == NULL)
		return false;
	SSvgObject *pObj = pSvgObj;
	SString str;
	int i;
	if(pOutText != NULL)
	{
		while(pObj)
		{
			for(i=1;i<=iLevel;i++)
				*pOutText += "\t";
			*pOutText += "<"+pObj->GetName()+" ";
			switch(pObj->GetType())
			{
			case SSvgObject::TYPE_SVG:
				WriteAttributeToText(pObj,"width",pOutText);
				WriteAttributeToText(pObj,"height",pOutText);
				if(pObj->GetAttribute("id").length() > 0)
				{
					WriteAttributeToText(pObj,"id",pOutText);
				}
				if(pObj->GetAttribute("coor-type").length() > 0)
				{
					WriteAttributeToText(pObj,"coor-type",pOutText);
				}
				if(pObj->GetAttribute("orig-x").length() > 0)
				{
					WriteAttributeToText(pObj,"orig-x",pOutText);
				}
				if(pObj->GetAttribute("orig-y").length() > 0)
				{
					WriteAttributeToText(pObj,"orig-y",pOutText);
				}
				if(pObj->GetAttribute("version").length() > 0)
					WriteAttributeToText(pObj,"version",pOutText);
				if(pObj->GetAttribute("xmlns").length() > 0)
					WriteAttributeToText(pObj,"xmlns",pOutText);
				break;
			case SSvgObject::TYPE_GROUP:
				if(pObj->GetAttribute("id").length() > 0)
					WriteAttributeToText(pObj,"id",pOutText);
				if(pObj->GetAttribute("uri").length() > 0)
					WriteAttributeToText(pObj,"uri",pOutText);
				if(pObj->GetAttribute("caption").length() > 0)
					WriteAttributeToText(pObj,"caption",pOutText);
				WriteAttributeToText(pObj,"z",pOutText);
				break;
			case SSvgObject::TYPE_USE:
				if(pObj->GetAttribute("uri").length() > 0)
					WriteAttributeToText(pObj,"uri",pOutText);
				if(pObj->GetAttribute("caption").length() > 0)
					WriteAttributeToText(pObj,"caption",pOutText);
				WriteAttributeToText(pObj,"x",pOutText);
				WriteAttributeToText(pObj,"y",pOutText);
				WriteAttributeToText(pObj,"z",pOutText);
				WriteAttributeToText(pObj,"width",pOutText);
				WriteAttributeToText(pObj,"height",pOutText);
				WriteAttributeToText(pObj,"xlink:href",pOutText,"#"+pObj->GetAttribute("xlink:href"));
				break;
			case SSvgObject::TYPE_RECT:
				if(pObj->GetAttribute("uri").length() > 0)
					WriteAttributeToText(pObj,"uri",pOutText);
				if(pObj->GetAttribute("caption").length() > 0)
					WriteAttributeToText(pObj,"caption",pOutText);
				WriteAttributeToText(pObj,"x",pOutText);
				WriteAttributeToText(pObj,"y",pOutText);
				WriteAttributeToText(pObj,"z",pOutText);
				WriteAttributeToText(pObj,"width",pOutText);
				WriteAttributeToText(pObj,"height",pOutText);
				WriteAttributeToText(pObj,"stroke",pOutText);
				WriteAttributeToText(pObj,"stroke-width",pOutText);
				WriteAttributeToText(pObj,"stroke-dasharray",pOutText);
				WriteAttributeToText(pObj,"fill",pOutText);
				if(pObj->GetAttribute("rx").length() > 0)
					WriteAttributeToText(pObj,"rx",pOutText);
				if(pObj->GetAttribute("ry").length() > 0)
					WriteAttributeToText(pObj,"ry",pOutText);
				if(pObj->GetAttribute("alpha").length() > 0)
					WriteAttributeToText(pObj,"alpha",pOutText);
				break;
			case SSvgObject::TYPE_PIC:
				if(pObj->GetAttribute("uri").length() > 0)
					WriteAttributeToText(pObj,"uri",pOutText);
				if(pObj->GetAttribute("caption").length() > 0)
					WriteAttributeToText(pObj,"caption",pOutText);
				WriteAttributeToText(pObj,"x",pOutText);
				WriteAttributeToText(pObj,"y",pOutText);
				WriteAttributeToText(pObj,"z",pOutText);
				WriteAttributeToText(pObj,"width",pOutText);
				WriteAttributeToText(pObj,"height",pOutText);
				WriteAttributeToText(pObj,"imgsn",pOutText);
				if(pObj->GetAttribute("rotate").length() > 0)
					WriteAttributeToText(pObj,"rotate",pOutText);
				if(pObj->GetAttribute("xlink:href").length() > 0)
					WriteAttributeToText(pObj,"xlink:href",pOutText);
				if(pObj->GetAttribute("alpha").length() > 0)
					WriteAttributeToText(pObj,"alpha",pOutText);
			break;
			case SSvgObject::TYPE_RECTTEXT:
				if(pObj->GetAttribute("uri").length() > 0)
					WriteAttributeToText(pObj,"uri",pOutText);
				if(pObj->GetAttribute("caption").length() > 0)
					WriteAttributeToText(pObj,"caption",pOutText);
				WriteAttributeToText(pObj,"x",pOutText);
				WriteAttributeToText(pObj,"y",pOutText);
				WriteAttributeToText(pObj,"z",pOutText);
				WriteAttributeToText(pObj,"width",pOutText);
				WriteAttributeToText(pObj,"height",pOutText);
				WriteAttributeToText(pObj,"alignv",pOutText);
				WriteAttributeToText(pObj,"alignh",pOutText);
				str = pObj->GetAttribute("font-family");
				{
					if(m_bUtf8)
					{
						//char *pStr = SString::translateCharToUTF_8(str.GetBuffer(0),str.GetLength());
						//char *pStr = SString::ConvertGBToUTF8(str.GetBuffer(0),str.GetLength());
						//WriteAttribute(pObj,pFile,"font-family");
						//delete[] pStr;
					}
					else
						WriteAttributeToText(pObj,"font-family",pOutText,str);
				}
				WriteAttributeToText(pObj,"font-size",pOutText);
				WriteAttributeToText(pObj,"font-weight",pOutText);
				WriteAttributeToText(pObj,"fill",pOutText);
				break;
			case SSvgObject::TYPE_TEXT:
				if(pObj->GetAttribute("uri").length() > 0)
					WriteAttributeToText(pObj,"uri",pOutText);
				if(pObj->GetAttribute("caption").length() > 0)
					WriteAttributeToText(pObj,"caption",pOutText);
				WriteAttributeToText(pObj,"x",pOutText);
				WriteAttributeToText(pObj,"y",pOutText);
				WriteAttributeToText(pObj,"z",pOutText);
				if(pObj->GetAttribute("text-anchor").length() > 0)
					WriteAttributeToText(pObj,"text-anchor",pOutText);
				str = pObj->GetAttribute("font-family");
				{
					if(m_bUtf8)
					{
						//char *pStr = SString::translateCharToUTF_8(str.GetBuffer(0),str.GetLength());
						//char *pStr = SString::ConvertGBToUTF8(str.GetBuffer(0),str.GetLength());
						//WriteAttribute(pObj,pFile,"font-family");
						//delete[] pStr;
					}
					else
						WriteAttributeToText(pObj,"font-family",pOutText,str);
				}
				WriteAttributeToText(pObj,"font-size",pOutText);
				WriteAttributeToText(pObj,"font-weight",pOutText);
				WriteAttributeToText(pObj,"fill",pOutText);
				break;
			case SSvgObject::TYPE_ELLIPSE:
				if(pObj->GetAttribute("uri").length() > 0)
					WriteAttributeToText(pObj,"uri",pOutText);
				if(pObj->GetAttribute("caption").length() > 0)
					WriteAttributeToText(pObj,"caption",pOutText);
				WriteAttributeToText(pObj,"cx",pOutText);
				WriteAttributeToText(pObj,"cy",pOutText);
				WriteAttributeToText(pObj,"z",pOutText);
				WriteAttributeToText(pObj,"rx",pOutText);
				WriteAttributeToText(pObj,"ry",pOutText);
				WriteAttributeToText(pObj,"stroke",pOutText);
				WriteAttributeToText(pObj,"stroke-width",pOutText);
				WriteAttributeToText(pObj,"stroke-dasharray",pOutText);
				WriteAttributeToText(pObj,"fill",pOutText);
				if(pObj->GetAttribute("alpha").length() > 0)
					WriteAttributeToText(pObj,"alpha",pOutText);
			break;
			case SSvgObject::TYPE_CIRCLE:
				if(pObj->GetAttribute("uri").length() > 0)
					WriteAttributeToText(pObj,"uri",pOutText);
				if(pObj->GetAttribute("caption").length() > 0)
					WriteAttributeToText(pObj,"caption",pOutText);
				WriteAttributeToText(pObj,"cx",pOutText);
				WriteAttributeToText(pObj,"cy",pOutText);
				WriteAttributeToText(pObj,"z",pOutText);
				WriteAttributeToText(pObj,"r",pOutText);
				WriteAttributeToText(pObj,"stroke",pOutText);
				WriteAttributeToText(pObj,"stroke-width",pOutText);
				WriteAttributeToText(pObj,"stroke-dasharray",pOutText);
				WriteAttributeToText(pObj,"fill",pOutText);
				break;
			case SSvgObject::TYPE_LINE:
				if(pObj->GetAttribute("uri").length() > 0)
					WriteAttributeToText(pObj,"uri",pOutText);
				if(pObj->GetAttribute("caption").length() > 0)
					WriteAttributeToText(pObj,"caption",pOutText);
				WriteAttributeToText(pObj,"x1",pOutText);
				WriteAttributeToText(pObj,"y1",pOutText);
				WriteAttributeToText(pObj,"z",pOutText);
				WriteAttributeToText(pObj,"x2",pOutText);
				WriteAttributeToText(pObj,"y2",pOutText);
				WriteAttributeToText(pObj,"stroke",pOutText);
				WriteAttributeToText(pObj,"stroke-width",pOutText);
				WriteAttributeToText(pObj,"stroke-dasharray",pOutText);
				break;
			case SSvgObject::TYPE_POLYLINE:
				if(pObj->GetAttribute("uri").length() > 0)
					WriteAttributeToText(pObj,"uri",pOutText);
				if(pObj->GetAttribute("caption").length() > 0)
					WriteAttributeToText(pObj,"caption",pOutText);
				WriteAttributeToText(pObj,"z",pOutText);
				WriteAttributeToText(pObj,"points",pOutText);
				WriteAttributeToText(pObj,"stroke",pOutText);
				WriteAttributeToText(pObj,"stroke-width",pOutText);
				WriteAttributeToText(pObj,"stroke-dasharray",pOutText);
				WriteAttributeToText(pObj,"fill",pOutText);			
				break;
			case SSvgObject::TYPE_PATH:
				if(pObj->GetAttribute("uri").length() > 0)
					WriteAttributeToText(pObj,"uri",pOutText);
				if(pObj->GetAttribute("caption").length() > 0)
					WriteAttributeToText(pObj,"caption",pOutText);
				WriteAttributeToText(pObj,"x",pOutText);
				WriteAttributeToText(pObj,"y",pOutText);
				WriteAttributeToText(pObj,"z",pOutText);
				WriteAttributeToText(pObj,"width",pOutText);
				WriteAttributeToText(pObj,"height",pOutText);
				WriteAttributeToText(pObj,"stroke",pOutText);
				WriteAttributeToText(pObj,"stroke-width",pOutText);
				WriteAttributeToText(pObj,"stroke-dasharray",pOutText);
				WriteAttributeToText(pObj,"fill",pOutText);
				WriteAttributeToText(pObj,"d",pOutText);
				break;
			default:
				//未知节点则保存所有携带的属性
				{
					int cnt = pObj->GetAttributePtr()->GetCount();
					SString sName,sValue;
					for(i=0;i<cnt;i++)
					{
						pObj->GetAttributePtr()->GetItem(i,sName,sValue);
						WriteAttributeToText(pObj,sName,pOutText,sValue);
					}
				}
				break;
			}
			//onmousedown="onmousedown(evt)"
			//onmouseover='change_attribut1(evt)'
			//onmouseout='change_attribut2(evt)'

			if(pObj->GetAttribute("uri") != "")
			{
				WriteAttributeToText(pObj,"onmousedown",pOutText,"onmousedown(evt)");
				WriteAttributeToText(pObj,"onmouseover",pOutText,"change_attribut1(evt)");
				WriteAttributeToText(pObj,"onmouseout",pOutText,"change_attribut2(evt)");
			}
			if(pObj->GetAttribute("ext-attribute").length() > 0)
			{
				WriteAttributeToText(pObj,"ext-attribute",pOutText);
			}
			if(pObj->GetType() == SSvgObject::TYPE_TEXT || pObj->GetType() == SSvgObject::TYPE_RECTTEXT)
			{
				//文字直接写内容并关闭
				str = pObj->GetAttribute("text");
				*pOutText += ">";
				if(m_bUtf8)
				{
					//char *pStr = SString::translateCharToUTF_8(str.GetBuffer(0),str.GetLength());
					//char *pStr = SString::ConvertGBToUTF8(str.GetBuffer(0),str.GetLength());
					//pFile->writeString(pStr);
					//delete[] pStr;
				}
				else
					*pOutText += str;
				*pOutText += "</" + pObj->GetName()+">\r\n";//"</text>\r\n";
				pObj = pObj->GetNext();
				continue;
			}
			if(pObj->GetType() == SSvgObject::TYPE_PIC)
			{
				*pOutText += ">";
				*pOutText += pObj->GetAttribute("pichex");
				*pOutText += "</image>\r\n";
				pObj = pObj->GetNext();
				continue;
			}
			else if(pObj->GetChild() == NULL)//没有下级，直接关闭节点
			{
				*pOutText += "/>\r\n";
			}
			else
			{
				*pOutText += ">\r\n";
				if(pObj == m_pSvgRoot)
					WriteSvgDefs(pFile,iLevel+1,pOutText);
				WriteSvgNode(pObj->GetChild(),pFile,iLevel+1,pOutText);
				for(i=1;i<=iLevel;i++)
					*pOutText += "\t";
				*pOutText += "</"+pObj->GetName()+">\r\n";
			}

			pObj = pObj->GetNext();
		}		
		return true;
	}

	//write to file
	while(pObj)
	{
		for(i=1;i<=iLevel;i++)
			pFile->writeString("\t");
		pFile->writeString("<"+pObj->GetName()+" ");
		switch(pObj->GetType())
		{
		case SSvgObject::TYPE_SVG:
			WriteAttribute(pObj,pFile,"width");
			WriteAttribute(pObj,pFile,"height");
			if(pObj->GetAttribute("coor-type").length() > 0)
				WriteAttribute(pObj,pFile,"coor-type");
			if(pObj->GetAttribute("orig-x").length() > 0)
				WriteAttribute(pObj,pFile,"orig-x");
			if(pObj->GetAttribute("orig-y").length() > 0)
				WriteAttribute(pObj,pFile,"orig-y");
			if(pObj->GetAttribute("version").length() > 0)
				WriteAttribute(pObj,pFile,"version");
			if(pObj->GetAttribute("xmlns").length() > 0)
				WriteAttribute(pObj,pFile,"xmlns");
			break;
		case SSvgObject::TYPE_GROUP:
			if(pObj->GetAttribute("id").length() > 0)
				WriteAttribute(pObj,pFile,"id");
			if(pObj->GetAttribute("uri").length() > 0)
				WriteAttribute(pObj,pFile,"uri");
			if(pObj->GetAttribute("caption").length() > 0)
				WriteAttribute(pObj,pFile,"caption");
			if(pObj->GetAttribute("z").length() > 0)
				WriteAttribute(pObj,pFile,"z");
			break;
		case SSvgObject::TYPE_USE:
			if(pObj->GetAttribute("uri").length() > 0)
				WriteAttribute(pObj,pFile,"uri");
			if(pObj->GetAttribute("caption").length() > 0)
				WriteAttribute(pObj,pFile,"caption");			
			WriteAttribute(pObj,pFile,"x");
			WriteAttribute(pObj,pFile,"y");
			WriteAttribute(pObj,pFile,"z");
			WriteAttribute(pObj,pFile,"width");
			WriteAttribute(pObj,pFile,"height");
			WriteAttribute(pObj,pFile,"xlink:href","#"+pObj->GetAttribute("xlink:href"));
			break;
		case SSvgObject::TYPE_RECT:
			if(pObj->GetAttribute("uri").length() > 0)
				WriteAttribute(pObj,pFile,"uri");
			if(pObj->GetAttribute("caption").length() > 0)
				WriteAttribute(pObj,pFile,"caption");
			WriteAttribute(pObj,pFile,"x");
			WriteAttribute(pObj,pFile,"y");
			WriteAttribute(pObj,pFile,"z");
			WriteAttribute(pObj,pFile,"width");
			WriteAttribute(pObj,pFile,"height");
			WriteAttribute(pObj,pFile,"stroke");
			WriteAttribute(pObj,pFile,"stroke-width");
			WriteAttribute(pObj,pFile,"stroke-dasharray");
			WriteAttribute(pObj,pFile,"fill");
			if(pObj->GetAttribute("rx").length() > 0)
				WriteAttribute(pObj,pFile,"rx");
			if(pObj->GetAttribute("ry").length() > 0)
				WriteAttribute(pObj,pFile,"ry");
			if(pObj->GetAttribute("alpha").length() > 0)
				WriteAttribute(pObj,pFile,"alpha");
			break;
		case SSvgObject::TYPE_PIC:
			if(pObj->GetAttribute("uri").length() > 0)
				WriteAttribute(pObj,pFile,"uri");
			if(pObj->GetAttribute("caption").length() > 0)
				WriteAttribute(pObj,pFile,"caption");
			WriteAttribute(pObj,pFile,"x");
			WriteAttribute(pObj,pFile,"y");
			WriteAttribute(pObj,pFile,"z");
			WriteAttribute(pObj,pFile,"width");
			WriteAttribute(pObj,pFile,"height");
			WriteAttribute(pObj,pFile,"imgsn");
			if(pObj->GetAttribute("rotate").length() > 0)
				WriteAttribute(pObj,pFile,"rotate");
			if(pObj->GetAttribute("xlink:href").length() > 0)
				WriteAttribute(pObj,pFile,"xlink:href");
			if(pObj->GetAttribute("alpha").length() > 0)
				WriteAttribute(pObj,pFile,"alpha");
			break;
		case SSvgObject::TYPE_RECTTEXT:
			if(pObj->GetAttribute("uri").length() > 0)
				WriteAttribute(pObj,pFile,"uri");
			if(pObj->GetAttribute("caption").length() > 0)
				WriteAttribute(pObj,pFile,"caption");
			WriteAttribute(pObj,pFile,"x");
			WriteAttribute(pObj,pFile,"y");
			WriteAttribute(pObj,pFile,"z");
			WriteAttribute(pObj,pFile,"width");
			WriteAttribute(pObj,pFile,"height");
			WriteAttribute(pObj,pFile,"alignv");
			WriteAttribute(pObj,pFile,"alignh");
			str = pObj->GetAttribute("font-family");
			{
				if(m_bUtf8)
				{
					//char *pStr = SString::translateCharToUTF_8(str.GetBuffer(0),str.GetLength());
					//char *pStr = SString::ConvertGBToUTF8(str.GetBuffer(0),str.GetLength());
					//WriteAttribute(pObj,pFile,"font-family");
					//delete[] pStr;
				}
				else
					WriteAttribute(pObj,pFile,"font-family",str);
			}
			WriteAttribute(pObj,pFile,"font-size");
			WriteAttribute(pObj,pFile,"font-weight");
			WriteAttribute(pObj,pFile,"fill");
			break;
		case SSvgObject::TYPE_TEXT:
			if(pObj->GetAttribute("uri").length() > 0)
				WriteAttribute(pObj,pFile,"uri");
			if(pObj->GetAttribute("caption").length() > 0)
				WriteAttribute(pObj,pFile,"caption");
			WriteAttribute(pObj,pFile,"x");
			WriteAttribute(pObj,pFile,"y");
			WriteAttribute(pObj,pFile,"z");
			str = pObj->GetAttribute("font-family");
			{
				if(m_bUtf8)
				{
					//char *pStr = SString::translateCharToUTF_8(str.GetBuffer(0),str.GetLength());
					//char *pStr = SString::ConvertGBToUTF8(str.GetBuffer(0),str.GetLength());
					//WriteAttribute(pObj,pFile,"font-family");
					//delete[] pStr;
				}
				else
					WriteAttribute(pObj,pFile,"font-family",str);
			}
			WriteAttribute(pObj,pFile,"font-size");
			WriteAttribute(pObj,pFile,"font-weight");
			WriteAttribute(pObj,pFile,"fill");
			if(pObj->GetAttribute("text-anchor").length() > 0)
				WriteAttribute(pObj,pFile,"text-anchor");
			break;
		case SSvgObject::TYPE_ELLIPSE:
			if(pObj->GetAttribute("uri").length() > 0)
				WriteAttribute(pObj,pFile,"uri");
			if(pObj->GetAttribute("caption").length() > 0)
				WriteAttribute(pObj,pFile,"caption");
			WriteAttribute(pObj,pFile,"cx");
			WriteAttribute(pObj,pFile,"cy");
			WriteAttribute(pObj,pFile,"z");
			WriteAttribute(pObj,pFile,"rx");
			WriteAttribute(pObj,pFile,"ry");
			WriteAttribute(pObj,pFile,"stroke");
			WriteAttribute(pObj,pFile,"stroke-width");
			WriteAttribute(pObj,pFile,"stroke-dasharray");
			WriteAttribute(pObj,pFile,"fill");
			if(pObj->GetAttribute("alpha").length() > 0)
				WriteAttribute(pObj,pFile,"alpha");
			break;
		case SSvgObject::TYPE_CIRCLE:
			if(pObj->GetAttribute("uri").length() > 0)
				WriteAttribute(pObj,pFile,"uri");
			if(pObj->GetAttribute("caption").length() > 0)
				WriteAttribute(pObj,pFile,"caption");
			WriteAttribute(pObj,pFile,"cx");
			WriteAttribute(pObj,pFile,"cy");
			WriteAttribute(pObj,pFile,"z");
			WriteAttribute(pObj,pFile,"r");
			WriteAttribute(pObj,pFile,"stroke");
			WriteAttribute(pObj,pFile,"stroke-width");
			WriteAttribute(pObj,pFile,"stroke-dasharray");
			WriteAttribute(pObj,pFile,"fill");
			break;
		case SSvgObject::TYPE_LINE:
			if(pObj->GetAttribute("uri").length() > 0)
				WriteAttribute(pObj,pFile,"uri");
			if(pObj->GetAttribute("caption").length() > 0)
				WriteAttribute(pObj,pFile,"caption");
			WriteAttribute(pObj,pFile,"x1");
			WriteAttribute(pObj,pFile,"y1");
			WriteAttribute(pObj,pFile,"z");
			WriteAttribute(pObj,pFile,"x2");
			WriteAttribute(pObj,pFile,"y2");
			WriteAttribute(pObj,pFile,"stroke");
			WriteAttribute(pObj,pFile,"stroke-width");
			WriteAttribute(pObj,pFile,"stroke-dasharray");
			break;
		case SSvgObject::TYPE_POLYLINE:
			if(pObj->GetAttribute("uri").length() > 0)
				WriteAttribute(pObj,pFile,"uri");
			if(pObj->GetAttribute("caption").length() > 0)
				WriteAttribute(pObj,pFile,"caption");
			WriteAttribute(pObj,pFile,"z");
			WriteAttribute(pObj,pFile,"points");
			WriteAttribute(pObj,pFile,"stroke");
			WriteAttribute(pObj,pFile,"stroke-width");
			WriteAttribute(pObj,pFile,"stroke-dasharray");
			WriteAttribute(pObj,pFile,"fill");			
			break;
		case SSvgObject::TYPE_PATH:
			if(pObj->GetAttribute("uri").length() > 0)
				WriteAttribute(pObj,pFile,"uri");
			if(pObj->GetAttribute("caption").length() > 0)
				WriteAttribute(pObj,pFile,"caption");
			WriteAttribute(pObj,pFile,"x");
			WriteAttribute(pObj,pFile,"y");
			WriteAttribute(pObj,pFile,"z");
			WriteAttribute(pObj,pFile,"width");
			WriteAttribute(pObj,pFile,"height");
			WriteAttribute(pObj,pFile,"stroke");
			WriteAttribute(pObj,pFile,"stroke-width");
			WriteAttribute(pObj,pFile,"stroke-dasharray");
			WriteAttribute(pObj,pFile,"fill");
			WriteAttribute(pObj,pFile,"d");
			break;
		default:
			//未知节点则保存所有携带的属性
			{
				int cnt = pObj->GetAttributePtr()->GetCount();
				SString sName,sValue;
				for(i=0;i<cnt;i++)
				{
					pObj->GetAttributePtr()->GetItem(i,sName,sValue);
					WriteAttribute(pObj,pFile,sName,sValue);
				}
			}
			break;
		}
		//onmousedown="onmousedown(evt)"
		//onmouseover='change_attribut1(evt)'
		//onmouseout='change_attribut2(evt)'

		if(pObj->GetAttribute("uri") != "")
		{
			WriteAttribute(pObj,pFile,"onmousedown","onmousedown(evt)");
			WriteAttribute(pObj,pFile,"onmouseover","change_attribut1(evt)");
			WriteAttribute(pObj,pFile,"onmouseout","change_attribut2(evt)");
		}
		if(pObj->GetAttribute("ext-attribute").length() > 0)
		{
			WriteAttribute(pObj,pFile,"ext-attribute");
		}
		if(pObj->GetType() == SSvgObject::TYPE_TEXT || pObj->GetType() == SSvgObject::TYPE_RECTTEXT)
		{
			//文字直接写内容并关闭
			str = pObj->GetAttribute("text");
			pFile->writeString(">");
			if(m_bUtf8)
			{
				//char *pStr = SString::translateCharToUTF_8(str.GetBuffer(0),str.GetLength());
				//char *pStr = SString::ConvertGBToUTF8(str.GetBuffer(0),str.GetLength());
				//pFile->writeString(pStr);
				//delete[] pStr;
			}
			else
				pFile->writeString(str);
			pFile->writeString("</"+pObj->GetName()+">\r\n");//pFile->writeString("</text>\r\n");
			pObj = pObj->GetNext();
			continue;
		}
		if(pObj->GetType() == SSvgObject::TYPE_PIC)
		{
			pFile->writeString(">");
			pFile->writeString(pObj->GetAttribute("pichex"));
			pFile->writeString("</image>\r\n");
			pObj = pObj->GetNext();
			continue;
		}
		else if(pObj->GetChild() == NULL)//没有下级，直接关闭节点
		{
			pFile->writeString("/>\r\n");
		}
		else
		{
			pFile->writeString(">\r\n");
			if(pObj == m_pSvgRoot)
				WriteSvgDefs(pFile,iLevel+1);
			WriteSvgNode(pObj->GetChild(),pFile,iLevel+1);
			for(i=1;i<=iLevel;i++)
				pFile->writeString("\t");
			pFile->writeString("</"+pObj->GetName()+">\r\n");
		}

		pObj = pObj->GetNext();
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  测试指定的图片是否被使用
// 作    者:  邵凯田
// 创建时间:  2015-11-26 19:03
// 参数说明:  
// 返 回 值:  true表示被使用，false表示未使用
//////////////////////////////////////////////////////////////////////////
bool SSvgDocument::IsImageDefUsed(SSvgObject *pParentObj,SString sImgDefSn)
{
	//遍历所有图元
	SSvgObject *pObj = pParentObj;
	while(pObj)
	{
		if(pObj->GetType() == SSvgObject::TYPE_PIC && pObj->GetAttribute("imgsn") == sImgDefSn)
			return true;
		if(pObj->GetChild() != NULL && IsImageDefUsed(pObj->GetChild(),sImgDefSn))
			return true;
		pObj = pObj->GetNext();
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  写预定义资源节点
// 作    者:  邵凯田
// 创建时间:  2015-11-26 9:14
// 参数说明:  
// 返 回 值:  true/false
//////////////////////////////////////////////////////////////////////////
bool SSvgDocument::WriteSvgDefs(SFile *pFile, int iLevel/*=0*/,SString *pOutText/*=NULL*/,bool bWithoutImg/*=false*/)
{
	if(m_ImageDefs.count() == 0 && m_SymbolObjects.count() == 0 && m_LinearGradients.count() == 0 && m_RadialGradients.count() == 0)
		return true;
	int i;
	if(pOutText != NULL)
	{
		for(i=1;i<=iLevel;i++)
			*pOutText += "\t";
		*pOutText += "<defs>\r\n";

		//填写图片资源预定义
		unsigned long pos=0,pos2=0;
		if(!bWithoutImg)
		{
			stuImageDefine *pImg = m_ImageDefs.FetchFirst(pos);
			while(pImg)
			{
				if(IsImageDefUsed(GetRootObject(),pImg->m_sImageDefSn) && pImg->m_sImageHex.length() > 0)
				{
					for(i=0;i<=iLevel;i++)
						*pOutText += "\t";
					*pOutText += SString::toFormat("<image_def sn=\"%s\" width=\"%d\" height=\"%d\">\r\n",
						pImg->m_sImageDefSn.data(),pImg->m_iWidth,pImg->m_iHeight);
					*pOutText += pImg->m_sImageHex;
					*pOutText += "</image_def>\r\n";
				}
				pImg = m_ImageDefs.FetchNext(pos);
			}
		}
		SSvgObject *pObj = m_SymbolObjects.FetchFirst(pos);
		while(pObj)
		{
			for(i=0;i<=iLevel;i++)
				*pOutText += "\t";
			*pOutText += SString::toFormat("<symbol id=\"%s\" viewBox=\"%s\">\r\n",
				pObj->GetAttribute("id").data(),pObj->GetAttribute("viewBox").data());
			WriteSvgNode(pObj->GetChild(),pFile,iLevel+2,pOutText);
			for(i=0;i<=iLevel;i++)
				*pOutText += "\t";
			*pOutText += "</symbol>\r\n";
			pObj = m_SymbolObjects.FetchNext(pos);
		}

		for(i=1;i<=iLevel;i++)
			*pOutText += "\t";

		//写渐变定义
		stuLinearGradient *pLinear = m_LinearGradients.FetchFirst(pos);
		while(pLinear)
		{
			for(i=0;i<=iLevel;i++)
				*pOutText += "\t";
			*pOutText += SString::toFormat("<linearGradient id=\"%s\" x1=\"%d%%\" y1=\"%d%%\" x2=\"%d%%\" y2=\"%d%%\">\r\n",
				pLinear->id.data(),pLinear->x1,pLinear->y1,pLinear->x2,pLinear->y2);
			stuGradientStop *pStop = pLinear->Stops.FetchFirst(pos2);
			while(pStop)
			{
				for(i=0;i<=iLevel+1;i++)
					*pOutText += "\t";
				*pOutText += SString::toFormat("<stop offset=\"%d%%\" style=\"stop-color:%s;stop-opacity:%d\" />\r\n",
					pStop->offset,SSvgPainter::GetStringByColor(pStop->color).data(),pStop->opacity);
				pStop = pLinear->Stops.FetchNext(pos2);
			}
			for(i=0;i<=iLevel;i++)
				*pOutText += "\t";
			*pOutText += "</linearGradient>\r\n";
			pLinear = m_LinearGradients.FetchNext(pos);
		}

		stuRadialGradient *pRadial = m_RadialGradients.FetchFirst(pos);
		while(pRadial)
		{
			for(i=0;i<=iLevel;i++)
				*pOutText += "\t";
			*pOutText += SString::toFormat("<radialGradient id=\"%s\" cx=\"%d%%\" cy=\"%d%%\" r=\"%d%%\" fx=\"%d%%\" fy=\"%d%%\">\r\n",
				pRadial->id.data(),pRadial->cx,pRadial->cy,pRadial->r,pRadial->fx,pRadial->fy);
			stuGradientStop *pStop = pRadial->Stops.FetchFirst(pos2);
			while(pStop)
			{
				for(i=0;i<=iLevel+1;i++)
					*pOutText += "\t";
				*pOutText += SString::toFormat("<stop offset=\"%d%%\" style=\"stop-color:%s;stop-opacity:%d\" />\r\n",
					pStop->offset,SSvgPainter::GetStringByColor(pStop->color).data(),pStop->opacity);
				pStop = pRadial->Stops.FetchNext(pos2);
			}
			for(i=0;i<=iLevel;i++)
				*pOutText += "\t";
			*pOutText += "</linearGradient>\r\n";
			pRadial = m_RadialGradients.FetchNext(pos);
		}

		*pOutText += "</defs>\r\n";
	}
	else
	{
		for(i=1;i<=iLevel;i++)
			pFile->writeString("\t");
		pFile->writeString("<defs>\r\n");

		//填写图片资源预定义
		unsigned long pos=0,pos2=0;
		stuImageDefine *pImg = m_ImageDefs.FetchFirst(pos);
		while(pImg)
		{
			if(IsImageDefUsed(GetRootObject(),pImg->m_sImageDefSn) && pImg->m_sImageHex.length() > 0)
			{
				for(i=0;i<=iLevel;i++)
					pFile->writeString("\t");
				pFile->fprintf("<image_def sn=\"%s\" width=\"%d\" height=\"%d\">\r\n",
					pImg->m_sImageDefSn.data(),pImg->m_iWidth,pImg->m_iHeight);
				pFile->writeString(pImg->m_sImageHex);
				pFile->writeString("</image_def>\r\n");
			}
			pImg = m_ImageDefs.FetchNext(pos);
		}
		SSvgObject *pObj = m_SymbolObjects.FetchFirst(pos);
		while(pObj)
		{
			for(i=0;i<=iLevel;i++)
				pFile->writeString("\t");
			pFile->fprintf("<symbol id=\"%s\" viewBox=\"%s\">\r\n",
				pObj->GetAttribute("id").data(),pObj->GetAttribute("viewBox").data());
			WriteSvgNode(pObj->GetChild(),pFile,iLevel+2,pOutText);
			for(i=0;i<=iLevel;i++)
				pFile->writeString("\t");
			pFile->writeString("</symbol>\r\n");
			pObj = m_SymbolObjects.FetchNext(pos);
		}

		//写渐变定义
		stuLinearGradient *pLinear = m_LinearGradients.FetchFirst(pos);
		while(pLinear)
		{
			for(i=0;i<=iLevel;i++)
				pFile->writeString("\t");
			pFile->fprintf("<linearGradient id=\"%s\" x1=\"%d%%\" y1=\"%d%%\" x2=\"%d%%\" y2=\"%d%%\">\r\n",
				pLinear->id.data(),pLinear->x1,pLinear->y1,pLinear->x2,pLinear->y2);
			stuGradientStop *pStop = pLinear->Stops.FetchFirst(pos2);
			while(pStop)
			{
				for(i=0;i<=iLevel+1;i++)
					pFile->writeString("\t");
				pFile->fprintf("<stop offset=\"%d%%\" style=\"stop-color:%s;stop-opacity:%d\" />\r\n",
					pStop->offset,SSvgPainter::GetStringByColor(pStop->color).data(),pStop->opacity);
				pStop = pLinear->Stops.FetchNext(pos2);
			}
			for(i=0;i<=iLevel;i++)
				pFile->writeString("\t");
			pFile->writeString("</linearGradient>\r\n");
			pLinear = m_LinearGradients.FetchNext(pos);
		}

		stuRadialGradient *pRadial = m_RadialGradients.FetchFirst(pos);
		while(pRadial)
		{
			for(i=0;i<=iLevel;i++)
				pFile->writeString("\t");
			pFile->fprintf("<radialGradient id=\"%s\" cx=\"%d%%\" cy=\"%d%%\" r=\"%d%%\" fx=\"%d%%\" fy=\"%d%%\">\r\n",
				pRadial->id.data(),pRadial->cx,pRadial->cy,pRadial->r,pRadial->fx,pRadial->fy);
			stuGradientStop *pStop = pRadial->Stops.FetchFirst(pos2);
			while(pStop)
			{
				for(i=0;i<=iLevel+1;i++)
					pFile->writeString("\t");
				pFile->fprintf("<stop offset=\"%d%%\" style=\"stop-color:%s;stop-opacity:%d\" />\r\n",
					pStop->offset,SSvgPainter::GetStringByColor(pStop->color).data(),pStop->opacity);
				pStop = pRadial->Stops.FetchNext(pos2);
			}
			for(i=0;i<=iLevel;i++)
				pFile->writeString("\t");
			pFile->writeString("</radialGradient>\r\n");
			pRadial = m_RadialGradients.FetchNext(pos);
		}
		for(i=1;i<=iLevel;i++)
			pFile->writeString("\t");
		pFile->writeString("</defs>\r\n");
	}
	return true;
}

//写属性到文件
bool SSvgDocument::WriteAttribute(SSvgObject *pSvgObj, SFile *pFile, SString sAttrName, SString sAttrValue)
{
	if(sAttrValue == "'{<NONE>}'")
		pFile->writeString(sAttrName+"=\""+pSvgObj->GetAttribute(sAttrName)+"\" ");
	else
	{
		sAttrValue.replace("<","&lt;");
		sAttrValue.replace(">","&gt;");
		pFile->writeString(sAttrName+"=\""+sAttrValue+"\" ");
	}
	return true;
}

bool SSvgDocument::WriteAttributeToText(SSvgObject *pSvgObj, SString sAttrName,SString *pOutText, SString sAttrValue)
{
	if(sAttrValue == "'{<NONE>}'")
		*pOutText += sAttrName+"=\""+pSvgObj->GetAttribute(sAttrName)+"\" ";
		//pFile->writeString(sAttrName+"=\""+pSvgObj->GetAttribute(sAttrName)+"\" ");
	else
 	{
		sAttrValue.replace("<","&lt;");
 		sAttrValue.replace(">","&gt;");
		*pOutText += sAttrName+"=\""+sAttrValue+"\" ";
		//pFile->writeString(sAttrName+"=\""+sAttrValue+"\" ");
	}
	return true;
}

/************************************************************************/
/*
	递归加载SVG元素到根对象下
*/
/************************************************************************/
bool SSvgDocument::ReadSvgNode(SSvgObject *pParentSvgObj, SBaseConfig* pXmlNode)
{
	if(pXmlNode == NULL)
		return false;
	//加载当前及及其后续节点
	//SSvgObject *pPrev=NULL;
	SString sName,sNodeName;
	//int i,cnt = pXmlNode->GetChildCount();
	unsigned long pos,pos2;
	SSvgObject *pNewObj;
	SBaseConfig::SConfigAttribute *pAttr;
	SBaseConfig *pNode = pXmlNode->GetChildPtr()->FetchFirst(pos);
	while(pNode)
	//for(i=0;i<cnt;i++)
	{
		//SBaseConfig* pNode = pXmlNode->GetChildNode(i);
		sName = pNode->GetNodeName();
		if(sName == "#text")
		{
			//文字内容或注释
			pNode = pXmlNode->GetChildPtr()->FetchNext(pos);
			continue;
		}
		if(sName == "defs")
		{
			ProcessDefsNode(pNode);
			pNode = pXmlNode->GetChildPtr()->FetchNext(pos);
			continue;
		}
		pNewObj = NewObject();
		pNewObj->SetName(sName);
		//加载当前节点属性到SVG对象
#if 1
		pAttr = pNode->GetAttributePtr()->FetchFirst(pos2);
		while(pAttr)
		{
			if(pAttr->m_sName == "style")//样式表
			{
				ParseStyle(pNewObj,pAttr->m_sValue);
			}
			else
			{
				pNewObj->SetAttributeValue(pAttr->m_sName.data(),pAttr->m_sValue.data());				
			}
			pAttr = pNode->GetAttributePtr()->FetchNext(pos2);
		}
		if(pNewObj->GetAttribute("zvalue").length() > 0 && pNewObj->GetAttribute("z").length() == 0)
		{
			pNewObj->SetAttributeValue("z",pNewObj->GetAttribute("zvalue"));
		}
// 		else if(pNode->IsAttribute("z") == false)
// 		{
// 			pNode->SetAttribute("z",g_iSvgDefaultZOrder++);
// 		}

#else
		int j,cnt2 = pNode->GetAttributeCount();
		for(j=0;j<cnt2;j++)
		{
			sNodeName = pNode->GetAttributeName(j);
			if(sNodeName == "style")//样式表
			{
				sNodeName = "";
				sNodeName += pNode->GetAttribute(sNodeName);
				ParseStyle(pNewObj,sNodeName);
			}
			else
				pNewObj->SetAttributeValue(sNodeName.data(),pNode->GetAttribute(sNodeName).data());
		}
#endif

		//插入对象树
		if(m_pSvgRoot == NULL)
		{
			m_pSvgRoot = pNewObj;
		}
		else
		{
			//MEMO: 加入Z处理 [2016-3-1 20:09 邵凯田]  pParentSvgObj->AddChild(pNewObj);
			pParentSvgObj->AddChildWithZ(pNewObj);
		}
		//如果是文字需要读取其下级的文字内容，文字不需要读取下级
		if(sName == "text" || sName == "recttext")
		{
			//why? if(pNode->GetChildCount()>0)
			{
				//固定加一个text属性用来存放文字的内容
				pNewObj->SetAttributeValue("text",pNode->GetNodeValue());
			}
		}
		else if(sName == "image")
		{
			//兼容以前的内嵌图片，并将其修改为新的引用格式
			SString sHex = pNode->GetNodeValue();
			int w = pNode->GetAttributeI("width");
			int h = pNode->GetAttributeI("height");
			SString imgsn = pNode->GetAttribute("imgsn");
			stuImageDefine *pImageDef = NULL;
			if(imgsn.length() > 0)
			{
				pImageDef = SearchImage(imgsn);
			}
			if(pImageDef == NULL && sHex.length() > 8)
			{
				pImageDef = LoadImage(w,h,sHex);
			}
			if(pImageDef == NULL)
			{
				//从开地加载图片
				SString href = pNode->GetAttribute("xlink:href");
				if(href.length() > 0)
				{
					//先查找
					pImageDef = SearchImageByLocalFile(href);
					if(pImageDef == NULL)
					{
						pImageDef = LoadImage(href,false);
					}
				}
			}
			if(pImageDef != NULL)
			{
				pNewObj->SetAttributeValue("imgsn",pImageDef->m_sImageDefSn);
				pNewObj->SetAttributeValue("img_def_obj",SString::toFormat("%ld",(unsigned long)pImageDef));
			}
			//pNewObj->SetAttributeValue("pichex",pNode->GetNodeValue());
		}
		else if(sName == "use")//引用图元
		{
			SString href = pNode->GetAttribute("xlink:href");
			if(href.left(1) != "#")
				href = "";
			else
				href = href.mid(1);
			pNewObj->SetAttributeValue("xlink:href",href);
		}
		else
		{
			//加载当前节点的下级节点
			ReadSvgNode(pNewObj,pNode);		
		}
		pNode = pXmlNode->GetChildPtr()->FetchNext(pos);
	}

	return true;
}

void* SSvgDocument::ThreadRemoveObjs(void *lp)
{
	SSvgObject *pRoot = (SSvgObject*)lp;
	pRoot->Delete();
	return NULL;
}
bool SSvgDocument::RemoveObjects(bool bBkThread)
{
	S_UNUSED(bBkThread);
	m_AllObjects.copyto(m_DeletedObjects);
	m_AllObjects.clear();

// 	if(m_pSvgRoot)
// 	{
// 		if(bBkThread)
// 		{
// 			SKT_CREATE_THREAD(ThreadRemoveObjs,m_pSvgRoot);
// 		}
// 		else
// 			m_pSvgRoot->Delete();
// 		m_pSvgRoot = NULL;
// 	}
	m_pSvgRoot = NULL;
	m_ImageDefs.clear();
	return true;
}


/************************************************************************/
/* 
	创建新的SVG文件，即空文档
*/
/************************************************************************/
bool SSvgDocument::NewFile()
{
	RemoveObjects();
	m_pSvgRoot = NewObject();
	m_pSvgRoot->SetName("svg");
	m_pSvgRoot->BuildObject(m_pSvgRoot);
	SSvgObject *pNewObj = NewObject();
	pNewObj->SetName("rect");
	pNewObj->SetAttributeValue("x","0");
	pNewObj->SetAttributeValue("y","0");
	pNewObj->SetAttributeValue("z","-100");//表示背景虚拟图元
	pNewObj->SetAttributeValue("width","1280");
	pNewObj->SetAttributeValue("height","1024");
	pNewObj->SetAttributeValue("stroke","#000000");
	pNewObj->SetAttributeValue("stroke-width","1");
	pNewObj->SetAttributeValue("stroke-dasharray","0");
	pNewObj->SetAttributeValue("fill","#000000");
	pNewObj->BuildObject(pNewObj);
	//m_pSvgRoot->AddChild(pNewObj);
	m_pSvgRoot->AddChildWithZ(pNewObj);
	m_pSvgRoot->CalcObjectRect();
	return true;
}

/************************************************************************/
/*
	对当前的SVG根对象按Z序进行重新排序，第一个页面对象保证在最前不参加排序 
*/
/************************************************************************/
void SSvgDocument::SortObject(SSvgObject *pObj)
{
	SSvgObject *pRoot = GetRootObject();
	if(pRoot == NULL )
		return;

	pRoot->DeleteChild(pObj);
	pRoot->AddChildWithZ(pObj);

}

SSvgObject* SSvgDocument::SearchByURI(SString uri)
{
	SSvgObject *pFirst = GetFirstObject();
	if(pFirst == NULL )
		return NULL;
	while(pFirst)
	{
		if(pFirst->GetAttribute("uri") == uri)
			return pFirst;
		pFirst = pFirst->GetNext();
	}
	return NULL;
}

SSvgObject* SSvgDocument::SearchByAttribute(const char* attr,const char* val)
{
	SSvgObject *pFirst = GetFirstObject();
	//SSvgObject *pFirst1=pFirst;
	if(pFirst == NULL )
		return NULL;
	while(pFirst)
	{
		if(pFirst->GetAttribute(attr) == val)
			return pFirst;
		pFirst = pFirst->GetNext();
	}
	return NULL;
}

SString SSvgDocument::GetAllObjects()
{
	SSvgObject *pFirst = GetFirstObject();
	if(pFirst == NULL )
		return "";
	SString str="";
	while(pFirst)
	{
		str += SString::toString((long)pFirst)+",";
		pFirst = pFirst->GetNext();
	}
	if(str.GetLength()>0)
		str = str.Left(str.GetLength()-1);
	return str;
}

/************************************************************************/
/* 
	增加一个图元，name为rect/text/line..., attr表示属性如stroke=#FF0000;x=100;y=200;
	返回新增对象的句柄，0表示失败
*/
/************************************************************************/
SSvgObject* SSvgDocument::AddObject(SString sName, SString sAttr)
{
	SSvgObject *pRoot = GetRootObject();
	if(pRoot == NULL)
		return NULL;
	SSvgObject *pNewObj = NewObject();
	pNewObj->SetName(sName);
	ParseStyle(pNewObj,sAttr);
	pNewObj->BuildObject(pNewObj);
	pNewObj->CalcObjectRect();
	//pRoot->AddChild(pNewObj);
	pRoot->AddChildWithZ(pNewObj);
	return pNewObj;
}

void SSvgDocument::ParseStyle(SSvgObject *pObj, SString style)
{
	//加载当前节点属性到SVG对象
	SString sAttrName,sAttrVal;
	int i,cnt=SString::GetAttributeCount(style,";");
	for(i=1;i<=cnt;i++)
	{
		sAttrName = SString::GetIdAttribute(i,style,";");
		if(sAttrName == "")
			continue;
		if(sAttrName.Find("=")>0)
		{
			sAttrVal = SString::GetIdAttribute(2,sAttrName,"=");
			sAttrName = SString::GetIdAttribute(1,sAttrName,"=");
			if(sAttrName == "")
				continue;
		}
		else
		{
			sAttrVal = SString::GetIdAttribute(2,sAttrName,":");
			sAttrName = SString::GetIdAttribute(1,sAttrName,":");
			if(sAttrName == "")
				continue;
		}
		pObj->SetAttributeValue(sAttrName.GetBuffer(0),sAttrVal);
	}

}

//////////////////////////////////////////////////////////////////////////
// 描    述:  处理预定义的defs段
// 作    者:  邵凯田
// 创建时间:  2015-11-26 8:52
// 参数说明:  pXmlNode为defs节点
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SSvgDocument::ProcessDefsNode(SBaseConfig *pXmlNode)
{
	//清除图片以外的预定义对象
	m_SymbolObjects.clear();
	m_LinearGradients.clear();
	m_RadialGradients.clear();
	unsigned long pos=0;
	SBaseConfig *pSubNode = pXmlNode->GetChildPtr()->FetchFirst(pos);
	while(pSubNode)
	{
		if(pSubNode->GetNodeName() == "image_def")
		{
			ProcessImageDef(pSubNode);
		}
		else if(pSubNode->GetNodeName() == "symbol")
		{
			ProcessSymbol(pSubNode);
		}
		else if(pSubNode->GetNodeName() == "linearGradient")
		{
			ProcessLinearGradient(pSubNode);
		}
		else if(pSubNode->GetNodeName() == "radialGradient")
		{
			ProcessRadialGradient(pSubNode);
		}
		pSubNode = pXmlNode->GetChildPtr()->FetchNext(pos);
	}
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  处理预定义的图片资源
// 作    者:  邵凯田
// 创建时间:  2015-11-26 8:57
// 参数说明:  pImgNode
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SSvgDocument::ProcessImageDef(SBaseConfig *pImgNode)
{
	if(pImgNode->GetNodeValue().length() <= 20)
		return;
	stuImageDefine *pDef = new stuImageDefine();
	pDef->m_sImageDefSn= pImgNode->GetAttribute("sn");
	pDef->m_sImageHex = pImgNode->GetNodeValue();

	pDef->m_pPic = new SPicture();
	int len = pDef->m_sImageHex.length()/2;
	BYTE *pHex = new BYTE[len];
	SString::StrToHex(pDef->m_sImageHex.data(),pDef->m_sImageHex.length(),pHex);
#ifdef QT_GUI_LIB
	if(!pDef->m_pPic->m_pixmap.loadFromData(pHex,len))
	{
		LOGWARN("加载内嵌图片资源失败!");
		delete[] pHex;
		delete pDef;
		return;
	}
	pDef->m_iHeight = pImgNode->GetAttributeI("height");
	pDef->m_iWidth = pImgNode->GetAttributeI("width");
#endif
	delete[] pHex;
	m_ImageDefs.append(pDef);
}


//////////////////////////////////////////////////////////////////////////
// 描    述:  处理预定义的模板资源
// 作    者:  邵凯田
// 创建时间:  2016-11-8 15:35
// 参数说明:  @pSynbolNode
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SSvgDocument::ProcessSymbol(SBaseConfig *pSymbolNode)
{
	SSvgObject *pSymbol = NewObject();
	pSymbol->SetAttributeValue("preserveAspectRatio",pSymbolNode->GetAttribute("preserveAspectRatio"));
	pSymbol->SetAttributeValue("id",pSymbolNode->GetAttribute("id"));
	pSymbol->SetAttributeValue("viewBox",pSymbolNode->GetAttribute("viewBox"));
	m_SymbolObjects.append(pSymbol);
	ReadSvgNode(pSymbol,pSymbolNode);
	pSymbol->BuildObject(pSymbol);
}


//////////////////////////////////////////////////////////////////////////
// 描    述:  处理线性渐变预定义
// 作    者:  邵凯田
// 创建时间:  2017-9-7 14:25
// 参数说明:  @pNode
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SSvgDocument::ProcessLinearGradient(SBaseConfig *pNode)
{
	stuLinearGradient *p = new stuLinearGradient();
	p->id = pNode->GetAttribute("id");
	p->x1 = pNode->GetAttributeI("x1");
	p->y1 = pNode->GetAttributeI("y1");
	p->x2 = pNode->GetAttributeI("x2");
	p->y2 = pNode->GetAttributeI("y2");
	unsigned long pos;
	SBaseConfig *pStop = pNode->GetChildPtr()->FetchFirst(pos);
	while(pStop)
	{
		if(pStop->GetNodeName() == "stop")
		{
			stuGradientStop *pNewStop = new stuGradientStop();
			pNewStop->Set(pStop);
			p->Stops.append(pNewStop);
		}
		pStop = pNode->GetChildPtr()->FetchNext(pos);
	}
	m_LinearGradients.append(p);
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  处理线性渐变预定义
// 作    者:  邵凯田
// 创建时间:  2017-9-7 14:25
// 参数说明:  @pNode
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SSvgDocument::ProcessRadialGradient(SBaseConfig *pNode)
{
	stuRadialGradient *p = new stuRadialGradient();
	p->id = pNode->GetAttribute("id");
	p->cx = pNode->GetAttributeI("cx");
	p->cy = pNode->GetAttributeI("cy");
	p->r = pNode->GetAttributeI("r");
	p->fx = pNode->GetAttributeI("fx");
	p->fy = pNode->GetAttributeI("fy");
	unsigned long pos;
	SBaseConfig *pStop = pNode->GetChildPtr()->FetchFirst(pos);
	while(pStop)
	{
		if(pStop->GetNodeName() == "stop")
		{
			stuGradientStop *pNewStop = new stuGradientStop();
			pNewStop->Set(pStop);
			p->Stops.append(pNewStop);
		}
		pStop = pNode->GetChildPtr()->FetchNext(pos);
	}
	m_RadialGradients.append(p);
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  检索指定ID的模板
// 作    者:  邵凯田
// 创建时间:  2016-11-8 15:43
// 参数说明:  @id为内容，不含'#'
// 返 回 值:  SSvgObject*, NULL表示找不到
//////////////////////////////////////////////////////////////////////////
SSvgObject* SSvgDocument::SearchSymbolObj(SString id)
{
	unsigned long pos;
	SSvgObject *pObj = m_SymbolObjects.FetchFirst(pos);
	while(pObj)
	{
		if(pObj->GetAttribute("id") == id)
			return pObj;
		pObj = m_SymbolObjects.FetchNext(pos);
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  加载图片资源
// 作    者:  邵凯田
// 创建时间:  2015-11-26 10:29
// 参数说明:  
// 返 回 值:  stuImageDefine*
//////////////////////////////////////////////////////////////////////////
SSvgDocument::stuImageDefine* SSvgDocument::LoadImage(int w,int h,SString &sImgHex)
{
	unsigned long pos=0;
	stuImageDefine *p = m_ImageDefs.FetchFirst(pos);
	while(p)
	{
		if(p->m_sImageHex == sImgHex)
			return p;
		p = m_ImageDefs.FetchNext(pos);
	}
	p = new stuImageDefine();
	p->m_iWidth = w;
	p->m_iHeight = h;
	p->m_pPic = new SPicture();
	p->m_sImageHex = sImgHex;
	p->m_sImageDefSn = NewImageSn();
	int len = sImgHex.length()/2;
	BYTE *pHex = new BYTE[len];
	SString::StrToHex(sImgHex.data(),sImgHex.length(),pHex);
#ifdef QT_GUI_LIB
	if(!p->m_pPic->m_pixmap.loadFromData(pHex,len))
	{
		LOGWARN("加载内嵌图片失败!");
		delete[] pHex;
		delete p;
		return NULL;
	}
#endif
 	delete[] pHex;
	m_ImageDefs.append(p);
	return p;
}

SString SSvgDocument::NewImageSn()
{
	int max=0;
	unsigned long pos=0;
	stuImageDefine *p = m_ImageDefs.FetchFirst(pos);
	while(p)
	{
		if(p->m_sImageDefSn.toInt() > max)
			max = p->m_sImageDefSn.toInt();
		p = m_ImageDefs.FetchNext(pos);
	}
	return SString::toFormat("%d",max+1);
}

SSvgDocument::stuImageDefine* SSvgDocument::LoadImage(SString sFileName,bool bInsertInner)
{
#ifdef QT_GUI_LIB
	SString sHome = SDir::currentDirPath();
	if(sHome.right(1) == "\\" || sHome.right(1) == "/")
		sHome = sHome.left(sHome.length()-1);
	if(sHome.right(4) == "/bin" || sHome.right(4) == "\\bin")
		sHome = sHome.left(sHome.length()-4);
#ifdef WIN32
	if(sFileName.at(1) != ':')
		sFileName = sHome+"\\"+sFileName;
#else
	if(sFileName.at(0) != '/')
		sFileName = sHome+"/"+sFileName;
#endif
	SFile f(sFileName);
	QPixmap img;
	if(!f.open(IO_ReadOnly) || !img.load(sFileName.data()))
	{
		//SQt::ShowErrorBox(QObject::tr("错误"),QObject::tr("加载图片时失败!"),NULL);
		LOGWARN("加载内嵌图片失败!");
		return NULL;
	}
	int h = img.height();
	int w = img.width();
	int len = f.size();
	f.seekBegin();
	BYTE *pHex = new BYTE[len];
	f.readBlock(pHex,len);
	f.close();

	stuImageDefine *p = new stuImageDefine();
	p->m_sLocalFileName = sFileName;
	p->m_iWidth = w;
	p->m_iHeight = h;
	p->m_pPic = new SPicture();
	if(bInsertInner)
	{		
		p->m_sImageHex = SString::HexToStr(pHex,len,false);
		p->m_sImageDefSn = NewImageSn();
	}
	else
		p->m_sImageDefSn = "unused";
	if(!p->m_pPic->m_pixmap.loadFromData(pHex,len))
	{
		LOGWARN("加载内嵌图片失败!");
		delete[] pHex;
		delete p;
		return NULL;
	}

	m_ImageDefs.append(p);
	return p;
#else
	return NULL;
#endif
}

SSvgDocument::stuImageDefine* SSvgDocument::SearchImage(SString sImageDefSn)
{
	unsigned long pos=0;
	stuImageDefine *p = m_ImageDefs.FetchFirst(pos);
	while(p)
	{
		if(p->m_sImageDefSn == sImageDefSn)
			return p;
		p = m_ImageDefs.FetchNext(pos);
	}
	return NULL;
}

SSvgDocument::stuImageDefine* SSvgDocument::SearchImageByLocalFile(SString sLocalFileName)
{
	unsigned long pos=0;
	stuImageDefine *p = m_ImageDefs.FetchFirst(pos);
	while(p)
	{
		if(p->m_sLocalFileName == sLocalFileName)
			return p;
		p = m_ImageDefs.FetchNext(pos);
	}
	return NULL;
}