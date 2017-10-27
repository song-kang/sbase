/**
 *
 * 文 件 名 : SZip.cpp
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


#ifdef WIN32
#pragma warning(disable : 4786)
#include <windows.h>
#include <direct.h>
#else
#include <ftw.h>
#include <unistd.h>
#include <utime.h>
#endif
#include "SZip.h"


using std::string;
using std::vector;
using std::set;

//////////////////////////////////////////////////////////////////////////
// 名    称:  SMemZip
// 作    者:  邵凯田
// 创建时间:  2015-4-3 10:08
// 描    述:  内存压缩类
//////////////////////////////////////////////////////////////////////////
SMemZip::SMemZip()
{

}
SMemZip::~SMemZip()
{

}

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
int SMemZip::Compress(UChar* dest, ULong* destLen, const UChar* source, ULong sourceLen, SZipZLevels level)
{
	return compress2(dest, destLen, source, sourceLen, level);
}

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
int SMemZip::Uncompress(UChar* dest, ULong* destLen, const UChar* source, ULong sourceLen)
{
	return uncompress(dest, destLen, source, sourceLen);
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  取源压缩数据长度对应的最大可能的目标缓冲区大小，Compress使用其获得压缩缓冲区大小
// 作    者:  邵凯田
// 创建时间:  2015-4-3 10:55
// 参数说明:  sourceLen表示压缩前的缓冲大小
// 返 回 值:  压缩后最大可能的大小
//////////////////////////////////////////////////////////////////////////
ULong SMemZip::CompressBound(ULong sourceLen)
{
	return compressBound(sourceLen);
}

//////////////////////////////////////////////////////////////////////////
// 名    称:  SZip
// 作    者:  邵凯田
// 创建时间:  2015-4-3 10:11
// 描    述:  文件/目录压缩封装类
//////////////////////////////////////////////////////////////////////////

SZip *g_pZip = NULL;
SZip::SZip()
{
	Initialize();
}

SZip::~SZip()
{
	Close();
}

SZip* SZip::GetPtr()
{
	return g_pZip;
}

void SZip::Initialize()
{
	m_Zip = NULL;
	m_Mode = ZMode_Create;
	m_Level = SZip_ZLevel_DefaultCompression;
	m_Crc = false;
	m_DestFile = "";
}

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
bool SZip::Open(const std::string& file, bool crc, ZipModes mode)
{
	m_DestFile = file;
	m_Mode = mode;
	m_Crc = crc;
	m_Zip = zipOpen64(m_DestFile.c_str(), m_Mode);
	return m_Zip != NULL;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  关闭压缩文件
// 作    者:  邵凯田
// 创建时间:  2015-4-3 10:38
// 参数说明:  void
// 返 回 值:  true/false
//////////////////////////////////////////////////////////////////////////
bool SZip::Close()
{
	if(m_Zip != NULL)
	{
		if(zipClose(m_Zip, NULL) != Z_OK)
			return false;
		Initialize();
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  压缩文件，同时添加到.zip文件中
// 作    者:  邵凯田
// 创建时间:  2015-4-3 10:39
// 参数说明:  @file表示文件绝对路径，可以是目录
//         :  @level表示压缩等级
// 返 回 值:  成功返回true，失败返回false
//////////////////////////////////////////////////////////////////////////
bool SZip::AddFile(const std::string& file, SZipZLevels level)
{
	m_Level = level;
	return AddDirectory("", file);
}

bool SZip::AddFile(const std::string& file, const std::string& path)
{
	zip_fileinfo fileInfo;
	memset(&fileInfo, 0, sizeof(fileInfo));
	GetFileTime(path, fileInfo.tmz_date, fileInfo.dosDate);

	int zip64 = 0;
	zip64 = (int)IsLargeFile(path);
	
	const ULong BUFFER_SIZE = 16384;
	UChar byBuffer[BUFFER_SIZE] = { 0 };
	ULong crc = 0;
	if(m_Crc) GetFileCrc(path, byBuffer, BUFFER_SIZE, crc);
	if(zipOpenNewFileInZip3_64(m_Zip, file.c_str(), &fileInfo,
		NULL, 0, NULL, 0, NULL,
		Z_DEFLATED, m_Level, 0,
		-MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY,
		m_Password.empty() ? NULL : m_Password.c_str(), crc, zip64) != ZIP_OK)
		return false;

	FILE* stream;
	stream = fopen64(path.c_str(), "rb");
	if(stream == NULL) return false;
	
	int err = ZIP_OK;
	int sizeRead;
	do
	{
		err = ZIP_OK;
		sizeRead = (int)fread(byBuffer, 1, BUFFER_SIZE, stream);
		if(sizeRead < (int)BUFFER_SIZE && (int)feof(stream) == 0)
			err = ZIP_ERRNO;
		if(sizeRead > 0)
			err = zipWriteInFileInZip(m_Zip, byBuffer, sizeRead);
	} while((err == ZIP_OK) && (sizeRead > 0));
	if(stream != NULL) fclose(stream);
	if(err < 0) return false;
	if(zipCloseFileInZip(m_Zip) != ZIP_OK)
		return false;
	return true;
}

bool SZip::AddDirectory(const std::string& zipDir, const std::string& dir)
{
#ifdef WIN32	
	WIN32_FIND_DATAA wfd;
    ZeroMemory(&wfd, sizeof(wfd));
	
    HANDLE hFind;
	hFind = FindFirstFileA(dir.c_str(), &wfd);
    if(hFind == INVALID_HANDLE_VALUE) return false;	
	
    string strFolder = dir;
	int pos;
	if((pos = strFolder.rfind('\\')) != -1 ||
		(pos = strFolder.rfind('/')) != -1)
		strFolder = strFolder.substr(0, pos + 1);
	else
		strFolder = "";
	
	string fileInZip = zipDir;   
    do 
    {
		string fileName = wfd.cFileName;
        if(fileName == "." || fileName == "..") continue;		
        if((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
        {
            if(!AddDirectory(
				(fileInZip + fileName + "\\").c_str(),
				(strFolder + fileName + "\\*").c_str()))
				return false;
		}
        else
        {
            if(!AddFile(
				(fileInZip + fileName).c_str(),
				(strFolder + fileName).c_str()))
				return false;
        }	
    } while(FindNextFileA(hFind, &wfd));
	return true;
#else
	g_pZip = this;
	return ftw(dir.c_str(), AddDirectory2, 1000) == 0;
#endif
}

#ifndef WIN32
int SZip::AddDirectory2(const char* file, const struct stat* sb, int flag)
{
	static bool firstRun = true;
	static string rootDir;
	
	if(firstRun && flag == FTW_D)
	{
		firstRun = false;
		rootDir = file;
		int pos = rootDir.rfind('/');
		if(pos != -1) rootDir = rootDir.substr(pos + 1, rootDir.size());
		else rootDir = "";
	}
	if(flag == FTW_F)
	{
		string fileInZip = file;
		int pos = fileInZip.find(rootDir);
		if(pos != -1) fileInZip = fileInZip.substr(pos, fileInZip.size());
		
		if(!SZip::GetPtr()->AddFile(
			fileInZip.c_str(),
			file))
			return -1;
	}
	return 0;
}
#endif

bool SZip::IsLargeFile(const std::string& file)
{
	bool flag = false;
	ZPOS64_T pos = 0;
	FILE* stream = fopen64(file.c_str(), "rb");
	if(stream == NULL) return flag;
	fseeko64(stream, 0, SEEK_END);		
	pos = ftello64(stream);
	if(pos >= 0xffffffff) flag = true;		
	fclose(stream);
	return flag;
}

bool SZip::GetFileCrc(const std::string& fileInZip, UChar* buffer, ULong sizeBuf, ULong& crc)
{
	int err = ZIP_OK;
	ULong calculateCrc = 0;
	ULong sizeRead = 0;
	//ULong totalRead = 0;
	FILE* stream = fopen64(fileInZip.c_str(), "rb");
	if(stream == NULL)
	{
		crc = calculateCrc;
		return false;
	}
	do
	{
		err = ZIP_OK;
		sizeRead = fread(buffer, 1, sizeBuf, stream);
		if(sizeRead < sizeBuf && feof(stream) == 0)
			err = ZIP_ERRNO;
		if(sizeRead > 0)
			calculateCrc = crc32(calculateCrc, buffer, sizeRead);
	} while(err == ZIP_OK && sizeRead > 0);
	fclose(stream);
	crc = calculateCrc;
	return err == ZIP_OK;
}

bool SZip::GetFileTime(const std::string& file, tm_zip& tmZip, ULong& dt)
{
#ifdef WIN32
	FILETIME ftLocal;
	HANDLE hFind;
	WIN32_FIND_DATAA ff32;
	bool flag = false;
	
	hFind = FindFirstFileA(file.c_str(), &ff32);
	if(hFind != INVALID_HANDLE_VALUE)
	{
		FileTimeToLocalFileTime(&(ff32.ftLastWriteTime), &ftLocal);
		FileTimeToDosDateTime(&ftLocal, ((UShort*)&dt) + 1, ((UShort*)&dt) + 0);
		FindClose(hFind);
		flag = true;
	}
	return flag;
#else
	struct stat s;
	tm* filedate;
	time_t tm_t = 0;
	int flag = false;
	
	std::string name = file;
	if(name != "-")
	{
		if(name[name.size() - 1] == '/')
			name = name.substr(0, name.size() - 2);
		if(stat(name.c_str(), &s) == 0)
			flag = true;
	}
	filedate = localtime(&tm_t);
	tmZip.tm_sec  = filedate->tm_sec;
	tmZip.tm_min  = filedate->tm_min;
	tmZip.tm_hour = filedate->tm_hour;
	tmZip.tm_mday = filedate->tm_mday;
	tmZip.tm_mon  = filedate->tm_mon ;
	tmZip.tm_year = filedate->tm_year;
	return flag;
#endif
}

//////////////////////////////////////////////////////////////////////////
// YsUnzip

SUnzip::SUnzip()
: m_Uzip(NULL), m_DestFolder("")
{

}

SUnzip::~SUnzip()
{
	Close();
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  打开压缩文件
// 作    者:  邵凯田
// 创建时间:  2015-4-3 10:42
// 参数说明:  @file表示压缩文件名
// 返 回 值:  true/false
//////////////////////////////////////////////////////////////////////////
bool SUnzip::Open(const string& file)
{
	m_Uzip = unzOpen64(file.c_str());
	return m_Uzip != NULL;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  关闭压缩文件
// 作    者:  邵凯田
// 创建时间:  2015-4-3 10:42
// 参数说明:  void
// 返 回 值:  true/false
//////////////////////////////////////////////////////////////////////////
bool SUnzip::Close()
{
	if(m_Uzip != NULL)
	{
		unzClose(m_Uzip);
		m_Uzip = NULL;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  获得文件目录信息
// 作    者:  邵凯田
// 创建时间:  2015-4-3 10:43
// 参数说明:  @vec表示返回的文件列表
// 返 回 值:  true/false
//////////////////////////////////////////////////////////////////////////
bool SUnzip::GetFileList(vector<FileInfo>& vec)
{
	unz_global_info64 unzInfo;

	if(unzGetGlobalInfo64(m_Uzip, &unzInfo) != UNZ_OK)
		return false;

	for(ULong i = 0; i < unzInfo.number_entry; ++i)
	{
		unz_file_info64 fileInfo;
		char buffer[MAX_PATH];
		FileInfo data;

		if(unzGetCurrentFileInfo64(m_Uzip, &fileInfo, buffer, MAX_PATH, NULL, 0, NULL, 0) != UNZ_OK)
			break;

		data.name = buffer;

		if((fileInfo.flag & 1) != 0)
			data.type = FileInfo::FType_Dir;
		else
			data.type = FileInfo::FType_File;

		data.uzSize = Get64BitSize(fileInfo.uncompressed_size);
		data.zSize = Get64BitSize(fileInfo.compressed_size);

		sprintf(buffer, "%8.8lx", fileInfo.crc);
		data.crc32 = buffer;

		sprintf(buffer, "%4.4lu-%2.2lu-%2.2lu  %2.2lu:%2.2lu",
			(ULong)fileInfo.tmu_date.tm_year,
			(ULong)fileInfo.tmu_date.tm_mon + 1,
			(ULong)fileInfo.tmu_date.tm_mday,
			(ULong)fileInfo.tmu_date.tm_hour,
			(ULong)fileInfo.tmu_date.tm_min);
		data.dateTime = buffer;

		if (fileInfo.compression_method == 0)
            data.method = "Stored";
        else
			if (fileInfo.compression_method == Z_DEFLATED)
			{
				UInt iLevel = (UInt)((fileInfo.flag & 0x6)/2);
				if (iLevel==0)
					data.method = "Defl:N";
				else if(iLevel == 1)
					data.method = "Defl:X";
				else if((iLevel == 2) || (iLevel == 3))
					data.method = "Defl:F";
			}
			else
				if(fileInfo.compression_method == Z_BZIP2ED)
					data.method = "BZip2 ";
				else
					data.method = "Unkn. ";
		vec.push_back(data);

		if((i + 1) < unzInfo.number_entry)
		{
			if(unzGoToNextFile(m_Uzip) != UNZ_OK)
				break;
		}
	}
	return true;
}

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
bool SUnzip::Extract(const string& folder, const string& password, bool overWrite)
{
	m_SetDir.clear();

	string destFolder = folder;
	if(folder[folder.size() - 1] != '\\' && folder[folder.size() - 1] != '/')
		destFolder += "/";

	unz_global_info64 unzInfo;
    if(unzGetGlobalInfo64(m_Uzip, &unzInfo) != UNZ_OK)
		return false;

    for(ULong i = 0; i < unzInfo.number_entry; i++)
    {
        if(!ExtractCurrentFile(destFolder, password, overWrite))
			return false;
        if((i+1) < unzInfo.number_entry)
        {
            if(unzGoToNextFile(m_Uzip) != UNZ_OK)
				return false;
        }
    }
	return true;
}

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
bool SUnzip::ExtractEx(const string& file, const string& folder, const string& password, bool overWrite)
{
	if(file.empty())
		return Extract(folder, password, overWrite);
	// 解压指定文件
	m_SetDir.clear();

	string destFolder = folder;
	if(folder[folder.size() - 1] != '\\' && folder[folder.size() - 1] != '/')
		destFolder += "/";

	if(unzLocateFile(m_Uzip, file.c_str(), 0) != UNZ_OK)
		return false;	
	return ExtractCurrentFile(destFolder, password, overWrite);
}

bool SUnzip::ExtractCurrentFile(const string& folder, const string& password, bool overWrite)
{
	using namespace std;
	const unsigned int WRITE_BUFFER_SIZE = 8192;

    unz_file_info64 fileInfo;
    char fileInZip[MAX_PATH];
    if(unzGetCurrentFileInfo64(m_Uzip, &fileInfo, fileInZip,
		MAX_PATH, NULL, 0, NULL, 0) != UNZ_OK)
        return false;
	if(unzOpenCurrentFile(m_Uzip) != UNZ_OK)
		return false;

	string destPath = folder;
	int len = strlen(fileInZip);
	char* fileName = fileInZip;
	set<string>::iterator it;
	for(int i = 0; i < MAX_PATH && *(fileInZip + i) != '\0'; ++i)
	{
		if(*(fileInZip + i) == '/' || *(fileInZip + i) == '\\')
		{
			*(fileInZip + i) = '\0';
			destPath += fileName;
			fileName = fileInZip + i + 1;
			it = m_SetDir.find(destPath);
			if(it == m_SetDir.end())
			{
				CreateDir(destPath);
				SetFileTime(destPath, fileInfo, true);
				m_SetDir.insert(destPath);
			}
			destPath += "/";
		}
	}
	if(*(fileInZip + len - 1) == '\0')
		return true;
	destPath += fileName;

	if(password != "")
	{
		if(unzOpenCurrentFilePassword(m_Uzip, password.c_str()) != UNZ_OK)
			return true;
	}

	string args = overWrite ? "wb" : "ab";
	FILE* fout = fopen64(destPath.c_str(), args.c_str());
	if(fout == NULL) return false;
	
	UChar* buffer = new UChar[WRITE_BUFFER_SIZE];
	int err = UNZ_OK;
	do
	{
		err = unzReadCurrentFile(m_Uzip, (void*)buffer, WRITE_BUFFER_SIZE);
		if(err< 0) break;
		if(err > 0)
		{
			if(fwrite(buffer, 1, err, fout) == 1)
			{
				err = UNZ_ERRNO;
				break;
			}
		}
	} while(err > 0);
	if(fout) fclose(fout);
	if(err == 0)
		SetFileTime(destPath, fileInfo);
	if(unzCloseCurrentFile(m_Uzip) != UNZ_OK)
		return false;
	return true;
}

void SUnzip::SetFileTime(const std::string& file, unz_file_info64& info, bool bArchive)
{
#ifdef WIN32
	HANDLE hFile;
	FILETIME ftm, ftLocal, ftCreate, ftLastAcc, ftLastWrite;
	
	hFile = ::CreateFile(file.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL,
				OPEN_EXISTING, bArchive ? FILE_FLAG_BACKUP_SEMANTICS : 0, NULL);
	::GetFileTime(hFile, &ftCreate, &ftLastAcc, &ftLastWrite);
	::DosDateTimeToFileTime((UShort)(info.dosDate >> 16), (UShort)info.dosDate, &ftLocal);
	::LocalFileTimeToFileTime(&ftLocal, &ftm);
	if(bArchive)
		::SetFileTime(hFile, &ftm, &ftLastAcc, &ftCreate);
	else
		::SetFileTime(hFile, &ftCreate, &ftm, &ftm);
	::CloseHandle(hFile);
#else
	struct utimbuf ut;
	struct tm newdate;
	newdate.tm_sec = info.tmu_date.tm_sec;
	newdate.tm_min = info.tmu_date.tm_min;
	newdate.tm_hour = info.tmu_date.tm_hour;
	newdate.tm_mday = info.tmu_date.tm_mday;
	newdate.tm_mon = info.tmu_date.tm_mon;
	if(info.tmu_date.tm_year > 1900)
		newdate.tm_year = info.tmu_date.tm_year - 1900;
	else
		newdate.tm_year = info.tmu_date.tm_year ;
	newdate.tm_isdst = -1;
	
	ut.actime = ut.modtime = mktime(&newdate);
	utime(file.c_str(), &ut);
#endif
}

string SUnzip::Get64BitSize(ZPOS64_T size)
{
	char number[21];
 	int offset = 19;
 	number[20] = 0;
 	while(offset >= 0)
	{
		number[offset] = (char)((size % 10) + '0');
		size /= 10;
		--offset;
 	}
	return string(number);
}

void SUnzip::CreateDir(const std::string& dirName)
{
#ifdef WIN32
	_mkdir(dirName.c_str());
#else
	mkdir(dirName.c_str(), 0775);
#endif
}
