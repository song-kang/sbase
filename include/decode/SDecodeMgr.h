/**
 *
 * 文 件 名 : SDecoder.h
 * 创建日期 : 2015-5-7 16:35
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : 以太网报文解码器
 * 修改记录 : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-5-7	邵凯田　创建文件
 *
 **/

#ifndef __SKT_DECODER_MGR_H__
#define __SKT_DECODER_MGR_H__

#include "SDecodeBase.h"


//////////////////////////////////////////////////////////////////////////
// 名    称:  SDecodeMgr
// 作    者:  邵凯田
// 创建时间:  2015-5-7 16:39
// 描    述:  解码管理器，维护一个解码实例
//////////////////////////////////////////////////////////////////////////
class SDecodeMgr
{
public:
	SDecodeMgr();
	~SDecodeMgr();

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  开始解码管理器
	// 作    者:  邵凯田
	// 创建时间:  2015-5-7 16:43
	// 参数说明:  @bRemoveTcpSessionWhenDiscon表示是否在TCP会话断开以后销毁会话实例（离线分析时为false,在线分析时为true）
	//         :  @sAttr为初始化属性字符串，使用指示开启哪些解码协议（后续定义）
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	bool Start(bool bRemoveTcpSessionWhenDiscon,SString sAttr="");

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  停止解码，清除所有暂存信息、会话
	// 作    者:  邵凯田
	// 创建时间:  2015-5-7 16:48
	// 参数说明:  void
	// 返 回 值:  true表示成功，false表示失败
	//////////////////////////////////////////////////////////////////////////
	bool Stop();

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  预解码，获得报文的类型，以及其对应协议有效数据的起始位置和长度
	// 作    者:  邵凯田
	// 创建时间:  2015-5-8 14:10
	// 参数说明:  @soc表示当前报文的时标
	//         :  @pBuffer表示报文缓冲区
	//         :  @iLen表示报文长度
	// 返 回 值:  eSDecodeProtType，对应的协议类型
	//////////////////////////////////////////////////////////////////////////
	eSDecodeProtType PreDecode(int soc,BYTE *pBuffer,int iLen);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  添加一帧报文
	// 作    者:  邵凯田
	// 创建时间:  2015-5-7 16:51
	// 参数说明:  @soc表示报文时标（秒）
	//         :  @nsec表示报文时标（纳秒）
	//         :  @pBuffer表示报文缓冲区地址
	//         :  @iLen表示报文实际长度
	// 返 回 值:  int,对应eSDecodeFmtError,0表示解码成功
	//////////////////////////////////////////////////////////////////////////
	int AddFrame(int soc,int nsec,BYTE *pBuffer,int iLen);

	inline stuSDecodePreprocessInfo* GetPreInfo(){return &m_PreInfo;};

#ifdef SDECODE_OFFLINE
	//////////////////////////////////////////////////////////////////////////
	// 描    述:  设置解码结果树的根节点指针，默认为NULL（表示解码时不需要生成解码树）
	// 作    者:  邵凯田
	// 创建时间:  2015-5-10 14:06
	// 参数说明:  @p树的根节点，每次解码时将清除其中的内容，并有新的结果构建结果树
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void SetDecodeTreeRootPtr(SDecodeTree *p);
	inline void SetFrameIndex(int i){m_iFrameIndex = i;};
#else
	//////////////////////////////////////////////////////////////////////////
	// 描    述:  当报文接收超时的时候，调用该函数进行链路状态的更新
	// 作    者:  邵凯田
	// 创建时间:  2015-5-10 12:44
	// 参数说明:  @soc为预计的时间（世纪秒），不需要太精确，满足秒级精度即可
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void FrameTimeout(int soc);
#endif

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  设置事件处理的回调函数
	// 作    者:  邵凯田
	// 创建时间:  2015-5-10 14:20
	// 参数说明:  @OnDecodeEvent回调函数【 void (*OnDecodeEvent)(void* cbParam,stuSDecodeEvent *pEvent)  】
	//         :  @cbParam回调时的第一个参数（通常用于回调函数区分不同的解码实例)
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void SetEventCallback(OnDecodeEvent pFun,void *cbParam);

	int GetProtocolCount(){return SDECODE_PROT_COUNT;};
	SDecodeBase* GetProtocol(int i){if(i<0||i>=SDECODE_PROT_COUNT)return NULL;return m_DecodeProt[i];};

protected:
	//////////////////////////////////////////////////////////////////////////
	// 描    述:  定期检查是否有需要删除的TCP会话
	// 作    者:  邵凯田
	// 创建时间:  2015-5-10 13:17
	// 参数说明:  @soc为当前时间
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void CheckForSessionRemove(int soc);

private:
	SDecodeBase* m_DecodeProt[SDECODE_PROT_COUNT];//所有已知的协议
	stuSDecodePreprocessInfo m_PreInfo;//预处理信息
	//bool m_bRemoveTcpSessionWhenDiscon;//是否在TCP会话断开以后销毁会话实例（离线分析时为false,在线分析时为true）
	int m_iLastCheckRmSOC;//最后一次检测会话删除的时间
	SDecodeTree *m_pDecodeTreeRoot;//解码结果输出树的根节点，NULL表示解码时不需要生成结构树
	OnDecodeEvent m_pEventCallbackFun;
	void *m_pEventCallbackParam;
	SPtrList<SDecodeTree> m_TempTree;//缓存的结果树节点，不删除，而是在协议对应的解码实例中暂存
	int m_iFrameIndex;//报文序号，从1开始
};



#endif//__SKT_DECODER_MGR_H__

