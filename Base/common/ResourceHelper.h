#pragma once

// GetFile_OUTEx函数的参数
#define CREATEFILE_REPLACE_EXISTING		0x00000001
#define CREATEFILE_SUCCESS_IF_EXIST		0x00000002

class CResourceHelper
{
public:
	CResourceHelper(void);
	CResourceHelper(LPCTSTR lpLibFileName);
	~CResourceHelper(void);

	BOOL GetResourceData(LPCTSTR lpName, DWORD& dwData);

	// 注意: pData参数的返回值需要使用FreeResourceData函数进行释放内存.
	BOOL GetResourceData(LPCTSTR lpName, LPVOID * pData, LPDWORD lpDataBytes);
	BOOL GetResourceData(LPCTSTR lpName, LPCTSTR lpType, LPVOID * pData, LPDWORD lpDataBytes);

	// 释放获取资源时分配的内存资源.
	BOOL FreeResourceData(LPVOID pData);

	// 获取指定资源名的数据,并根据资源信息创建指定文件
	BOOL GetFile_OUT(LPCTSTR lpResourceName, LPCTSTR lpFilePath, BOOL bForce = FALSE);

	// 获取指定资源名的数据,并根据资源信息创建指定文件
	BOOL GetFile_OUTEx(LPCTSTR lpResourceName, LPCTSTR lpFilePath, DWORD dwFlags);

	// 获取指定资源名的数据,并根据指定文件名称创建文件
	BOOL GetFile_OUT(LPCTSTR lpResourceName, LPCTSTR lpFilePath, LPCTSTR lpFileName, BOOL bForce);

private:

	// 加载资源,并获取资源大小.
	HGLOBAL LoadResource(LPCTSTR lpName, LPCTSTR lpType, LPDWORD lpDataBytes);

	// 解析资源中定义文件名的文件
	BOOL ParseFile(LPCTSTR lpFilePath, LPBYTE lpData, DWORD dwDataBytes, BOOL bForce);

	// 解析预定义文件名的文件
	BOOL ParseFile(LPCTSTR lpFilePath, LPCTSTR lpFileName, LPBYTE lpData, DWORD dwFileBytes, BOOL bForce);

	// 解析文件名.
	BOOL ParseFileName(LPBYTE & lpData, LPTSTR lpFileName, DWORD cchLen);

	// 从资源中获取文件名
	BOOL GetFileName(LPCTSTR lpResourceName, LPTSTR lpFileName, DWORD cchLen);

	// 解析文件属性
	BOOL ParseFileAttributes(LPBYTE & lpData, DWORD & dwFileAttributes);

	// 解析文件大小
	BOOL ParseFileSize(LPBYTE & lpData, DWORD & dwFileSize);

	// 解析文件内容
	BOOL ParseFileData(LPCTSTR lpFileName, LPBYTE & lpData);

	// 解析文件时间
	BOOL ParseFileTime(LPBYTE & lpData, LPFILETIME lpCreationTime, LPFILETIME lpLastAccessTime, LPFILETIME lpLastWriteTime);

private:
	HMODULE m_hModule;

};
