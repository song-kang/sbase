/**
 *
 * 文 件 名 : SSaxXml.h
 * 创建日期 : 2015-4-7 16:18
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : 基于SAX的轻量级XML解析类
 * 修改记录 : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-4-7	邵凯田　创建文件
 *
 **/

#ifndef __SKT_SAX_XML_H__
#define __SKT_SAX_XML_H__

#include "SString.h"
#include "SStack.h"

enum SBASE_EXPORT SSaxXmlEncoding
{
	SSaxXml_GBK=0,
	SSaxXml_UTF8
};

enum SBASE_EXPORT SSaxXmlError
{
	SSaxErr_FILE_ERR	=1,	//文件错误
	SSaxErr_SYNTAX_ERR,		//XML语法错误
	SSaxErr_SCHEMA_ERR,		//SCHEMA约束错误
};

class SBASE_EXPORT SSaxAttribute
{
public:
	SSaxAttribute()
	{

	}
	~SSaxAttribute()
	{

	}

	SString m_sAttrName;//属性名称
	SString m_sAttrValue;//属性值
};

class SBASE_EXPORT SSaxNode
{
public:
	SString m_sNodeName;//节点名称
};

//////////////////////////////////////////////////////////////////////////
// 名    称:  SSaxXml
// 作    者:  邵凯田
// 创建时间:  2015-4-8 11:15
// 描    述:  基于SAX的轻量级XML解析类，通过Start/End函数回调派生处理类，
//            派生类需要记忆当前节点位置并根据节点名进行处理
//////////////////////////////////////////////////////////////////////////
class SBASE_EXPORT SSaxXml
{
public:
	SSaxXml();
	virtual ~SSaxXml();

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  解析XML文件
	// 作    者:  邵凯田
	// 创建时间:  2015-4-8 9:28
	// 参数说明:  @xXmlFileName为XML文件名的全路径
	//         :  @encoding表示期望的编码类型，默认为GBK
	// 返 回 值:  true表示解析成功，false表示解析失败
	//////////////////////////////////////////////////////////////////////////
	bool ParseXmlFile(char* sXmlFileName, SSaxXmlEncoding encoding=SSaxXml_GBK);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  解析一段XML文件，可分多次解析一个文件，文本应采用GBK/GB18030字符编码
	// 作    者:  邵凯田
	// 创建时间:  2015-4-8 9:34
	// 参数说明:  @sXmlText表示待解析的文本内容
	//         :  @sValidXmlText表示被有效解析的文本位置，下一次解析时应从将该位置开始
	//         :  @bLastText表示是否为最后一批待解析文本
	// 返 回 值:  true表示解析成功，false表示解析失败
	//////////////////////////////////////////////////////////////////////////
	bool ParseText(char* sXmlText, char* &sValidXmlText,bool bLastText);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  复位解析器，为下一个完整的XML解析做准备
	// 作    者:  邵凯田
	// 创建时间:  2015-4-8 9:54
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void ResetParser();

	//////////////////////////////////////////////////////////////////////////
	// Callback
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  开始新的XML文档解析
	// 作    者:  邵凯田
	// 创建时间:  2015-4-8 9:56
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void StartDocument(){};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  结束XML文件的解析
	// 作    者:  邵凯田
	// 创建时间:  2015-4-8 9:57
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void EndDocument(){};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  开始一个新的XML节点元素
	// 作    者:  邵凯田
	// 创建时间:  2015-4-8 9:57
	// 参数说明:  @sNodeName表示节点名称
	//         :  @pAttribute表示节点属性指针，可含0~N个属性
	//         :  @iAttributeCount表示节点属性数量
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void StartElement(char* sNodeName,SSaxAttribute *pAttribute,int iAttributeCount){S_UNUSED(sNodeName);S_UNUSED(pAttribute);S_UNUSED(iAttributeCount);};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  结束一个XML节点元素
	// 作    者:  邵凯田
	// 创建时间:  2015-4-8 10:10
	// 参数说明:  @sNodeName表示节点名称
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void EndElement(char* sNodeName){S_UNUSED(sNodeName);};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  新的节点值内容
	// 作    者:  邵凯田
	// 创建时间:  2015-4-8 10:12
	// 参数说明:  @sNodeValue表示指定的字符串首地址
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void Characters(char* sNodeValue){S_UNUSED(sNodeValue);};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  错误事件
	// 作    者:  邵凯田
	// 创建时间:  2015-4-9 9:29
	// 参数说明:  @iErrCode表示错误代码
	//         :  @sErrText表示错误描述
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void Error(int iErrCode,SString sErrText)
	{
		S_UNUSED(iErrCode);
		S_UNUSED(sErrText);
		printf("error in saxxml!\n");
	};

private:
	SPtrStack<SSaxNode,1024> m_XmlStack;//XML栈，1024为XML节点的最大深度
	SSaxXmlEncoding m_Encoding;//当前字符编码
	char m_sNodeName[128];
	char m_sAttrName[128];
	char m_sAttrValue[1024];
	int m_iNodeNameLen;
	int m_iAttrNameLen;
	int m_iAttrValueLen;

};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
struct stuSaxChoice;
//////////////////////////////////////////////////////////////////////////
// 描    述:  节点处理回调函数
// 作    者:  邵凯田
// 创建时间:  2015-12-18 13:31
// 参数说明:  @cbParam为SSaxXmlEx的的实例指针
// 返 回 值:  true接受该节点，false表示放弃该节点
//////////////////////////////////////////////////////////////////////////
typedef stuSaxChoice* (*CallbackSax_Node)(void* cbParam, SSaxAttribute *pAttribute,int iAttributeCount,char* sNodeValue);

struct SBASE_EXPORT stuSaxChoice
{
	const char* sNodeName;
	CallbackSax_Node pCallbackFun;
};

class SBASE_EXPORT SSaxNodeEx
{
public:
	char* psNodeName;//节点名称
	stuSaxChoice *pChoice;
};

//////////////////////////////////////////////////////////////////////////
// 名    称:  SSaxXmlEx
// 作    者:  邵凯田
// 创建时间:  2015-4-8 11:15
// 描    述:  基于SAX的轻量级XML解析类，通过逐级注册回调方法处理XML，
//            上层处理类不需要记忆当前处理节点位置，而不在不同节点注册不同的入口回调函数，
//			  该类使用较SSaxXml繁琐，但效率高于SSaxXml类
//////////////////////////////////////////////////////////////////////////
class SBASE_EXPORT SSaxXmlEx
{
public:
	
	SSaxXmlEx();
	virtual ~SSaxXmlEx();

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  解析XML文件
	// 作    者:  邵凯田
	// 创建时间:  2015-4-8 9:28
	// 参数说明:  @xXmlFileName为XML文件名的全路径
	//         :  @encoding表示期望的编码类型，默认为GBK
	// 返 回 值:  true表示解析成功，false表示解析失败
	//////////////////////////////////////////////////////////////////////////
	bool ParseXmlFile(char* sXmlFileName, SSaxXmlEncoding encoding=SSaxXml_GBK);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  解析一段XML文件，可分多次解析一个文件，文本应采用GBK/GB18030字符编码
	// 作    者:  邵凯田
	// 创建时间:  2015-4-8 9:34
	// 参数说明:  @sXmlText表示待解析的文本内容
	//         :  @sValidXmlText表示被有效解析的文本位置，下一次解析时应从将该位置开始
	//         :  @bLastText表示是否为最后一批待解析文本
	// 返 回 值:  true表示解析成功，false表示解析失败
	//////////////////////////////////////////////////////////////////////////
	bool ParseText(char* sXmlText0, char* &sValidXmlText,bool bLastText);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  复位解析器，为下一个完整的XML解析做准备
	// 作    者:  邵凯田
	// 创建时间:  2015-4-8 9:54
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void ResetParser();

	//////////////////////////////////////////////////////////////////////////
	// Callback
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  开始新的XML文档解析
	// 作    者:  邵凯田
	// 创建时间:  2015-4-8 9:56
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void StartDocument(){};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  结束XML文件的解析
	// 作    者:  邵凯田
	// 创建时间:  2015-4-8 9:57
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void EndDocument(){};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  错误事件
	// 作    者:  邵凯田
	// 创建时间:  2015-4-9 9:29
	// 参数说明:  @iErrCode表示错误代码
	//         :  @sErrText表示错误描述
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void Error(int iErrCode,SString sErrText)
	{
		S_UNUSED(iErrCode);
		S_UNUSED(sErrText);
		printf("error in saxxml!\n");
	};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取当前处理进度百分比
	// 作    者:  邵凯田
	// 创建时间:  2015-12-22 9:14
	// 参数说明:  void
	// 返 回 值:  int(0~100)
	//////////////////////////////////////////////////////////////////////////
	int GetProgressPecent();

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  处理
	// 作    者:  邵凯田
	// 创建时间:  2015-12-18 14:04
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	void ProcessChildNode(stuSaxChoice *pChoice);

	void SetRootCallbackFun(CallbackSax_Node p){m_pRootNodeCallbackFun = p;};
	void SetRootNodeName(SString sRootNodeName){m_sRootNodeName = sRootNodeName;};

	SConstBuffer m_ConstString;
private:
	SPtrStack<SSaxNodeEx,1024> m_XmlStackEx;//XML栈，1024为XML节点的最大深度
	SSaxXmlEncoding m_Encoding;//当前字符编码
	char m_sNodeName[128];
	char m_sAttrName[128];
	char m_sAttrValue[1024];
	int m_iNodeNameLen;
	int m_iAttrNameLen;
	int m_iAttrValueLen;
	CallbackSax_Node m_pRootNodeCallbackFun;
	SString m_sRootNodeName;//根节点的名称,空表示不验证根节点名
	char *m_psTextStart;//起始字符串地址
	char *m_psValidText;// 有效的字符串地址
	char **m_ppsTextNow;//当前处理字符串地址的指针
	int m_iXmlTextSize;//字符串总长度
};

#endif//__SKT_SAX_XML_H__
