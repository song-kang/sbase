/**
 *
 * 文 件 名 : SZip.h
 * 创建日期 : 2015-4-3 10:03
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : 压缩、解压缩封装类
 * 修改记录 : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-4-3	邵凯田　创建文件
 *
 **/

#ifndef __SKT_SZIP_H__
#define __SKT_SZIP_H__

#include <string>
#include <vector>
#include <set>
#include <string.h>
#include "SObject.h"

extern "C"
{
#include "zconf.h"
#include "zlib.h"
#include "zip.h"
#include "unzip.h"
}

typedef unsigned char UChar;
typedef unsigned long ULong;
typedef unsigned int  UInt;
typedef unsigned short UShort;

#ifndef MAX_PATH
#define MAX_PATH	260
#endif

enum SZipZLevels
{
	SZip_ZLevel_NoCompression		= Z_NO_COMPRESSION,		// 不压缩
	SZip_ZLevel_BestSpeed			= Z_BEST_SPEED,			// 最快速
	SZip_ZLevel_DefaultCompression	= Z_DEFAULT_COMPRESSION,// 默认压缩
	SZip_ZLevel_BestCompression		= Z_BEST_COMPRESSION	// 最大压缩率
};

//////////////////////////////////////////////////////////////////////////
// 名    称:  SMemZip
// 作    者:  邵凯田
// 创建时间:  2015-4-3 10:08
// 描    述:  内存压缩类
//////////////////////////////////////////////////////////////////////////
class SBASE_ZIP_EXPORT SMemZip
{
public:
	SMemZip();
	~SMemZip();

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  内存区压缩
	// 作    者:  邵凯田
	// 创建时间:  2015-4-3 10:54
	// 参数说明:  @dest目标缓冲区，提前申请好足够的内存，至少为CompressBound返回大小
	//         :  @destLen目标内容长度，可通过CompressBound获得，成功后指针内容置为最终的目标大小
	//         :  @source源数据
	//         :  @sourceLen源数据长度
	//         :  @level表示压缩等级（SZipZLevels）
	// 返 回 值:  0表示成功，Z_BUF_ERROR表示目标缓冲区不足，其它失败其它原因失败
	//////////////////////////////////////////////////////////////////////////
	static int Compress(UChar* dest, ULong* destLen, const UChar* source, ULong sourceLen, SZipZLevels level=SZip_ZLevel_BestSpeed);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  内存区解压缩
	// 作    者:  邵凯田
	// 创建时间:  2015-4-3 10:54
	// 参数说明:  @desc表示目标缓冲区（解压后）
	//         :  @destLen表示目标缓冲区大小，成功后指针内容置为最终的目标大小
	//         :  @source表示源缓冲区（待解压的数据）
	//         :  @sourceLen表示源数据的大小
	// 返 回 值:  0表示成功，Z_BUF_ERROR表示目标缓冲区不足，其它失败其它原因失败
	//////////////////////////////////////////////////////////////////////////
	static int Uncompress(UChar* dest, ULong* destLen, const UChar* source, ULong sourceLen);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取源压缩数据长度对应的最大可能的目标缓冲区大小，Compress使用其获得压缩缓冲区大小
	// 作    者:  邵凯田
	// 创建时间:  2015-4-3 10:55
	// 参数说明:  sourceLen表示压缩前的缓冲大小
	// 返 回 值:  压缩后最大可能的大小
	//////////////////////////////////////////////////////////////////////////
	static ULong CompressBound(ULong sourceLen);

};


//////////////////////////////////////////////////////////////////////////
// 名    称:  SZip
// 作    者:  邵凯田
// 创建时间:  2015-4-3 10:11
// 描    述:  文件/目录压缩封装类
//////////////////////////////////////////////////////////////////////////
class SBASE_ZIP_EXPORT SZip
{
public:
	enum SBASE_ZIP_EXPORT ZipModes
	{
		ZMode_Create = APPEND_STATUS_CREATE,
		ZMode_Append = APPEND_STATUS_ADDINZIP
	};

	SZip();
	~SZip();

	static SZip* GetPtr();

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  打开一个.zip文件，准备压缩
	// 作    者:  邵凯田
	// 创建时间:  2015-4-3 10:35
	// 参数说明:  @file 表示zip文件名
	//         :  @crc表示是否使用CRC效验，如果使用了CRC效验，可以通过setPassword设置密码
	//         :  @mode表示打开文件的模式：
	//				SZip::ZMode_Create-打开一个zip文件，如果文件已存则覆盖原来的内容
	//				SZip::ZMode_Append-打开一个zip文件，在文件尾添加新的内容
	// 返 回 值:  成功返回true，失败返回false
	//////////////////////////////////////////////////////////////////////////
	bool Open(const std::string& file, bool crc=false, ZipModes mode=ZMode_Create);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  关闭压缩文件
	// 作    者:  邵凯田
	// 创建时间:  2015-4-3 10:38
	// 参数说明:  void
	// 返 回 值:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool Close();

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  压缩文件，同时添加到.zip文件中
	// 作    者:  邵凯田
	// 创建时间:  2015-4-3 10:39
	// 参数说明:  @file表示文件绝对路径，可以是目录
	//         :  @level表示压缩等级
	// 返 回 值:  成功返回true，失败返回false
	//////////////////////////////////////////////////////////////////////////
	bool AddFile(const std::string& file, SZipZLevels level=SZip_ZLevel_DefaultCompression);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  打开一个.zip文件，准备压缩
	// 作    者:  邵凯田
	// 创建时间:  2015-4-3 10:41
	// 参数说明:  @password-密码，只有使用了CRC效验时密码才会生效
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void SetPassword(const std::string& password) { m_Password = password; }

private:
	void Initialize();
	bool IsLargeFile(const std::string& file);
	bool AddFile(const std::string& file, const std::string& path);
	bool AddDirectory(const std::string& zipDir, const std::string& dir);
	bool GetFileCrc(const std::string& fileInZip, UChar* buffer, ULong sizeBuf, ULong& crc);
	bool GetFileTime(const std::string& file, tm_zip& tmZip, ULong& dt);
#ifndef WIN32
	static int AddDirectory2(const char* file, const struct stat* sb, int flag);
#endif
	zipFile		m_Zip;
	SZipZLevels		m_Level;			// 压缩等级
	ZipModes	m_Mode;			// 打开文件的模式
	bool		m_Crc;			// 是否支持crc效验
	std::string	m_Password;		// 压缩密码
	std::string m_DestFile;		// 目标文件名(.zip)
};

//////////////////////////////////////////////////////////////////////////
// 名    称:  SUnZip
// 作    者:  邵凯田
// 创建时间:  2015-4-3 10:42
// 描    述:  解压缩封装类
//////////////////////////////////////////////////////////////////////////
class SBASE_ZIP_EXPORT SUnzip
{
public:
	struct SBASE_ZIP_EXPORT FileInfo
	{
		enum FileTypes
		{
			FType_File	= 1,
			FType_Dir	= 2
		};
		FileTypes	type;		// 类型
		std::string name;		// 文件名
		std::string uzSize;		// 未压缩大小
		std::string zSize;		// 压缩后大小
		std::string method;		// 压缩算法
		std::string dateTime;	// 修改日期、时间
		std::string crc32;		// CRC效验码
	};

	SUnzip();
	~SUnzip();

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  打开压缩文件
	// 作    者:  邵凯田
	// 创建时间:  2015-4-3 10:42
	// 参数说明:  @file表示压缩文件名
	// 返 回 值:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool Open(const std::string& file);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  关闭压缩文件
	// 作    者:  邵凯田
	// 创建时间:  2015-4-3 10:42
	// 参数说明:  void
	// 返 回 值:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool Close();

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  获得文件目录信息
	// 作    者:  邵凯田
	// 创建时间:  2015-4-3 10:43
	// 参数说明:  @vec表示返回的文件列表
	// 返 回 值:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool GetFileList(std::vector<FileInfo>& vec);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  提取zip中的文件
	// 作    者:  邵凯田
	// 创建时间:  2015-4-3 10:44
	// 参数说明:  @file-要提取的相对文件名
	//         :  @folder-存放目录
	//         :  @password-密码
	//         :  @overWrite-是否覆盖原文件
	// 返 回 值:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool Extract(const std::string& folder, const std::string& password, bool overWrite=true);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  从压缩包中提取指定的文件
	// 作    者:  邵凯田
	// 创建时间:  2015-4-3 10:47
	// 参数说明:  @file表示待提取的文件名，空表示提取全部文件
	//         :  @folder表示目标文件夹
	//         :  @password表示密码
	//         :  @overWrite表示文件存在时是否覆盖
	// 返 回 值:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool ExtractEx(const std::string& file, const std::string& folder, const std::string& password, bool overWrite=true);

private:
	bool ExtractCurrentFile(const std::string& folder, const std::string& password, bool overWrite);
	std::string Get64BitSize(ZPOS64_T size);
	void CreateDir(const std::string& dirName);
	void SetFileTime(const std::string& file, unz_file_info64& info, bool bArchive=false);
	
	unzFile		m_Uzip;
	std::string	m_DestFolder;	// 目标目录
	std::set<std::string> m_SetDir;
};

#endif// __SKT_SZIP_H__ 
