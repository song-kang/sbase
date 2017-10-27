/**
 *
 * �� �� �� : SZip.h
 * �������� : 2015-4-3 10:03
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : ѹ������ѹ����װ��
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-4-3	�ۿ�������ļ�
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
	SZip_ZLevel_NoCompression		= Z_NO_COMPRESSION,		// ��ѹ��
	SZip_ZLevel_BestSpeed			= Z_BEST_SPEED,			// �����
	SZip_ZLevel_DefaultCompression	= Z_DEFAULT_COMPRESSION,// Ĭ��ѹ��
	SZip_ZLevel_BestCompression		= Z_BEST_COMPRESSION	// ���ѹ����
};

//////////////////////////////////////////////////////////////////////////
// ��    ��:  SMemZip
// ��    ��:  �ۿ���
// ����ʱ��:  2015-4-3 10:08
// ��    ��:  �ڴ�ѹ����
//////////////////////////////////////////////////////////////////////////
class SBASE_ZIP_EXPORT SMemZip
{
public:
	SMemZip();
	~SMemZip();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �ڴ���ѹ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-4-3 10:54
	// ����˵��:  @destĿ�껺��������ǰ������㹻���ڴ棬����ΪCompressBound���ش�С
	//         :  @destLenĿ�����ݳ��ȣ���ͨ��CompressBound��ã��ɹ���ָ��������Ϊ���յ�Ŀ���С
	//         :  @sourceԴ����
	//         :  @sourceLenԴ���ݳ���
	//         :  @level��ʾѹ���ȼ���SZipZLevels��
	// �� �� ֵ:  0��ʾ�ɹ���Z_BUF_ERROR��ʾĿ�껺�������㣬����ʧ������ԭ��ʧ��
	//////////////////////////////////////////////////////////////////////////
	static int Compress(UChar* dest, ULong* destLen, const UChar* source, ULong sourceLen, SZipZLevels level=SZip_ZLevel_BestSpeed);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �ڴ�����ѹ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-4-3 10:54
	// ����˵��:  @desc��ʾĿ�껺��������ѹ��
	//         :  @destLen��ʾĿ�껺������С���ɹ���ָ��������Ϊ���յ�Ŀ���С
	//         :  @source��ʾԴ������������ѹ�����ݣ�
	//         :  @sourceLen��ʾԴ���ݵĴ�С
	// �� �� ֵ:  0��ʾ�ɹ���Z_BUF_ERROR��ʾĿ�껺�������㣬����ʧ������ԭ��ʧ��
	//////////////////////////////////////////////////////////////////////////
	static int Uncompress(UChar* dest, ULong* destLen, const UChar* source, ULong sourceLen);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡԴѹ�����ݳ��ȶ�Ӧ�������ܵ�Ŀ�껺������С��Compressʹ������ѹ����������С
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-4-3 10:55
	// ����˵��:  sourceLen��ʾѹ��ǰ�Ļ����С
	// �� �� ֵ:  ѹ���������ܵĴ�С
	//////////////////////////////////////////////////////////////////////////
	static ULong CompressBound(ULong sourceLen);

};


//////////////////////////////////////////////////////////////////////////
// ��    ��:  SZip
// ��    ��:  �ۿ���
// ����ʱ��:  2015-4-3 10:11
// ��    ��:  �ļ�/Ŀ¼ѹ����װ��
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
	// ��    ��:  ��һ��.zip�ļ���׼��ѹ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-4-3 10:35
	// ����˵��:  @file ��ʾzip�ļ���
	//         :  @crc��ʾ�Ƿ�ʹ��CRCЧ�飬���ʹ����CRCЧ�飬����ͨ��setPassword��������
	//         :  @mode��ʾ���ļ���ģʽ��
	//				SZip::ZMode_Create-��һ��zip�ļ�������ļ��Ѵ��򸲸�ԭ��������
	//				SZip::ZMode_Append-��һ��zip�ļ������ļ�β����µ�����
	// �� �� ֵ:  �ɹ�����true��ʧ�ܷ���false
	//////////////////////////////////////////////////////////////////////////
	bool Open(const std::string& file, bool crc=false, ZipModes mode=ZMode_Create);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �ر�ѹ���ļ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-4-3 10:38
	// ����˵��:  void
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool Close();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ѹ���ļ���ͬʱ��ӵ�.zip�ļ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-4-3 10:39
	// ����˵��:  @file��ʾ�ļ�����·����������Ŀ¼
	//         :  @level��ʾѹ���ȼ�
	// �� �� ֵ:  �ɹ�����true��ʧ�ܷ���false
	//////////////////////////////////////////////////////////////////////////
	bool AddFile(const std::string& file, SZipZLevels level=SZip_ZLevel_DefaultCompression);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��һ��.zip�ļ���׼��ѹ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-4-3 10:41
	// ����˵��:  @password-���룬ֻ��ʹ����CRCЧ��ʱ����Ż���Ч
	// �� �� ֵ:  void
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
	SZipZLevels		m_Level;			// ѹ���ȼ�
	ZipModes	m_Mode;			// ���ļ���ģʽ
	bool		m_Crc;			// �Ƿ�֧��crcЧ��
	std::string	m_Password;		// ѹ������
	std::string m_DestFile;		// Ŀ���ļ���(.zip)
};

//////////////////////////////////////////////////////////////////////////
// ��    ��:  SUnZip
// ��    ��:  �ۿ���
// ����ʱ��:  2015-4-3 10:42
// ��    ��:  ��ѹ����װ��
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
		FileTypes	type;		// ����
		std::string name;		// �ļ���
		std::string uzSize;		// δѹ����С
		std::string zSize;		// ѹ�����С
		std::string method;		// ѹ���㷨
		std::string dateTime;	// �޸����ڡ�ʱ��
		std::string crc32;		// CRCЧ����
	};

	SUnzip();
	~SUnzip();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ѹ���ļ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-4-3 10:42
	// ����˵��:  @file��ʾѹ���ļ���
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool Open(const std::string& file);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �ر�ѹ���ļ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-4-3 10:42
	// ����˵��:  void
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool Close();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����ļ�Ŀ¼��Ϣ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-4-3 10:43
	// ����˵��:  @vec��ʾ���ص��ļ��б�
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool GetFileList(std::vector<FileInfo>& vec);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ȡzip�е��ļ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-4-3 10:44
	// ����˵��:  @file-Ҫ��ȡ������ļ���
	//         :  @folder-���Ŀ¼
	//         :  @password-����
	//         :  @overWrite-�Ƿ񸲸�ԭ�ļ�
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool Extract(const std::string& folder, const std::string& password, bool overWrite=true);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ѹ��������ȡָ�����ļ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-4-3 10:47
	// ����˵��:  @file��ʾ����ȡ���ļ������ձ�ʾ��ȡȫ���ļ�
	//         :  @folder��ʾĿ���ļ���
	//         :  @password��ʾ����
	//         :  @overWrite��ʾ�ļ�����ʱ�Ƿ񸲸�
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool ExtractEx(const std::string& file, const std::string& folder, const std::string& password, bool overWrite=true);

private:
	bool ExtractCurrentFile(const std::string& folder, const std::string& password, bool overWrite);
	std::string Get64BitSize(ZPOS64_T size);
	void CreateDir(const std::string& dirName);
	void SetFileTime(const std::string& file, unz_file_info64& info, bool bArchive=false);
	
	unzFile		m_Uzip;
	std::string	m_DestFolder;	// Ŀ��Ŀ¼
	std::set<std::string> m_SetDir;
};

#endif// __SKT_SZIP_H__ 
