#include "StdAfx.h"
#include "ResourceHelper.h"
#include <strsafe.h>

CResourceHelper::CResourceHelper(void)
: m_hModule(NULL)
{
}

CResourceHelper::CResourceHelper(LPCTSTR lpLibFileName)
: m_hModule(NULL)
{
	m_hModule = ::LoadLibrary(lpLibFileName);
}

CResourceHelper::~CResourceHelper(void)
{
	if (m_hModule != NULL)
	{
		DWORD dwLastError = ::GetLastError();

		::FreeLibrary(m_hModule);
		m_hModule = NULL;

		::SetLastError(dwLastError);
	}
}

BOOL CResourceHelper::GetResourceData(LPCTSTR lpName, DWORD& dwData)
{
	DWORD dwBytes = 0;
	HGLOBAL hResData = LoadResource(lpName, RT_RCDATA, &dwBytes);
	if (hResData == NULL)
	{
		return FALSE;
	}

	// 该数据类型资源较多,避免频繁的小内存分配,采用该方式.
	LPVOID data = ::LockResource(hResData);
	if (data == NULL)
	{
		::FreeResource(hResData);
		return FALSE;
	}

	// 按照DWORD方式解析数据格式,避免兼容性问题,不对dwBytes数据大小进行校验.
	dwData = *(LPDWORD)data;
	::FreeResource(hResData);

	return TRUE;
}

BOOL CResourceHelper::GetResourceData(LPCTSTR lpName, LPVOID * pData, LPDWORD lpDataBytes)
{
	return GetResourceData(lpName, RT_RCDATA, pData, lpDataBytes);
}

BOOL CResourceHelper::GetResourceData(LPCTSTR lpName, LPCTSTR lpType, LPVOID * pData, LPDWORD lpDataBytes)
{
	if (lpName == NULL || lpType == NULL || pData == NULL || lpDataBytes == NULL)
	{
		::SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}

	DWORD dwSize = 0;
	HGLOBAL hResData = LoadResource(lpName, RT_RCDATA, &dwSize);
	if (hResData == NULL)
	{
		return FALSE;
	}

	LPVOID data = ::LockResource(hResData);
	if (data == NULL)
	{
		FreeResource(hResData);
		return FALSE;
	}

	// 分配内存保存资源信息
	LPVOID pResource = new BYTE[dwSize];
	if (pResource == NULL)
	{
		::FreeResource(hResData);
		::SetLastError(ERROR_OUTOFMEMORY);
		return FALSE;
	}

	memcpy(pResource, data, dwSize);
	::FreeResource(hResData);

	*pData = pResource;
	*lpDataBytes = dwSize;

	return TRUE;
}

BOOL CResourceHelper::FreeResourceData(LPVOID pData)
{
	if (pData == NULL)
	{
		::SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}

	delete [] pData;
	return TRUE;
}

HGLOBAL CResourceHelper::LoadResource(LPCTSTR lpName, LPCTSTR lpType, LPDWORD lpDataBytes)
{
	if (lpName == NULL || lpType == NULL || lpDataBytes == NULL)
	{
		::SetLastError(ERROR_INVALID_PARAMETER);
		return NULL;
	}

	HRSRC hResLoad = ::FindResource(m_hModule, lpName, lpType);
	if (hResLoad == NULL)
	{
		return NULL;
	}

	DWORD dwSize = ::SizeofResource(m_hModule, hResLoad);
	if (dwSize == 0)
	{
		return NULL;
	}

	HGLOBAL hResData = ::LoadResource(m_hModule, hResLoad);
	if (hResData == NULL)
	{
		return NULL;
	}

	*lpDataBytes = dwSize;
	return hResData;
}

BOOL
CResourceHelper::GetFile_OUT(
	LPCTSTR lpResourceName,
	LPCTSTR lpFilePath,
	BOOL bForce /* = FALSE */
	)
{
	return GetFile_OUT(lpResourceName, lpFilePath, NULL, bForce);
}

BOOL
CResourceHelper::GetFile_OUTEx(
	LPCTSTR lpResourceName,
	LPCTSTR lpFilePath,
	DWORD dwFlags
	)
{
	if (dwFlags & CREATEFILE_SUCCESS_IF_EXIST)
	{
		TCHAR szFileName[MAX_PATH] = {0};
		if (!GetFileName(lpResourceName, szFileName, MAX_PATH))
		{
			return FALSE;
		}

		TCHAR szFullFileName[MAX_PATH] = {0};
		StringCchPrintf(szFullFileName, MAX_PATH, _T("%s\\%s"), lpFilePath, szFileName);

		if (GetFileAttributes(szFullFileName) != INVALID_FILE_ATTRIBUTES)
		{
			return TRUE;
		}
	}

	return GetFile_OUT(lpResourceName, lpFilePath, NULL,
		(dwFlags & CREATEFILE_REPLACE_EXISTING) ? TRUE : FALSE);
}

BOOL
CResourceHelper::GetFile_OUT(
	LPCTSTR lpResourceName,
	LPCTSTR lpFilePath,
	LPCTSTR lpFileName,
	BOOL bForce
	)
{
	BOOL bReturn = FALSE;
	DWORD dwDataBytes = 0;
	HGLOBAL hResData = LoadResource(lpResourceName, RT_RCDATA, &dwDataBytes);
	if (hResData == NULL)
	{
		return FALSE;
	}

	DWORD dwErrorCode = ERROR_SUCCESS;

	LPBYTE lpData = (LPBYTE)::LockResource(hResData);
	if (lpData == NULL)
	{
		dwErrorCode = ::GetLastError();
		goto leave;
	}

	if (lpFileName == NULL)
	{
		if (!ParseFile(lpFilePath, lpData, dwDataBytes, bForce))
		{
			dwErrorCode = ::GetLastError();
			goto leave;
		}
	}
	else
	{
		if (!ParseFile(lpFilePath, lpFileName, lpData, dwDataBytes, bForce))
		{
			dwErrorCode = ::GetLastError();
			goto leave;
		}
	}

	bReturn = TRUE;

leave:
	if (hResData != NULL)
	{
		::FreeResource(hResData);
		hResData = NULL;
	}

	::SetLastError(dwErrorCode);
	return bReturn;
}

BOOL
CResourceHelper::ParseFile(
	LPCTSTR lpFilePath,
	LPBYTE lpData,
	DWORD dwDataBytes,
	BOOL bForce
	)
{
	if (lpFilePath == NULL || lpData == NULL || lpFilePath[0] == 0)
	{
		::SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}

	// 记录原始数据地址
	LPBYTE lpBaseData = lpData;

	TCHAR szFileName[MAX_PATH] = {0};
	StringCchCopy(szFileName, MAX_PATH, lpFilePath);
	DWORD dwPathLen = _tcslen(szFileName);
	StringCchCat(szFileName, MAX_PATH, _T("\\"));
	dwPathLen += 1;

	if (!ParseFileName(lpData, szFileName + dwPathLen, MAX_PATH - dwPathLen))
	{
		return FALSE;
	}

	DWORD dwFileAttributes = 0;
	ParseFileAttributes(lpData, dwFileAttributes);

	DWORD dwFileBytes = 0;
	ParseFileSize(lpData, dwFileBytes);

	LPCVOID lpBuffer = lpData;
	LPBYTE lpFileTime = lpData + dwFileBytes;
	FILETIME ftCreationTime = {0};
	FILETIME ftLastAccessTime = {0};
	FILETIME ftLastWriteTime = {0};
	ParseFileTime(lpFileTime, &ftCreationTime, &ftLastAccessTime, &ftLastWriteTime);

	if (lpFileTime != (lpBaseData + dwDataBytes))
	{
		//WriteDebug(_T("GetFile_OUT: Fail to check resource format for file %s."), szFileName);
	}

	HANDLE hFile = CreateFile(szFileName,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		CREATE_ALWAYS,
		dwFileAttributes,
		NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		if (!bForce)
		{
			if (
				::GetLastError() == ERROR_SHARING_VIOLATION
				||
				::GetLastError() == ERROR_ACCESS_DENIED
				)
			{
				//WriteDebug(_T("GetFile_OUT: File %s is already exist!"), szFileName);
				return TRUE;
			}
		}
		else
		{
			DeleteFile(szFileName);
		}

		hFile = CreateFile(szFileName,
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ,
			NULL,
			CREATE_ALWAYS,
			dwFileAttributes,
			NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			//WriteDebug(_T("GetFile_OUT: Fail to open file %s, errorcode is %d"), szFileName, ::GetLastError());
			return FALSE;
		}
	}

	DWORD dwWritten = 0;
	if (!WriteFile(hFile, lpBuffer, dwFileBytes, &dwWritten, NULL))
	{
		//WriteDebug(_T("GetFile_OUT: Fail to write file %s, errorcode is %d"), szFileName, ::GetLastError());
		CloseHandle(hFile);
		return FALSE;
	}

	if (!SetFileTime(hFile, (LPFILETIME) NULL, (LPFILETIME) NULL, &ftLastWriteTime))
	{
		//WriteDebug(_T("GetFile_OUT: Fail to set file %s time, errorcode is %d"), szFileName, ::GetLastError());
	}

	if (hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
		hFile = INVALID_HANDLE_VALUE;
	}

	return TRUE;
}

BOOL
CResourceHelper::ParseFile(
	LPCTSTR lpFilePath,
	LPCTSTR lpFileName,
	LPBYTE lpData,
	DWORD dwFileBytes,
	BOOL bForce
	)
{
	if (lpFilePath == NULL || lpFileName == NULL || lpData == NULL)
	{
		::SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}

	if (!CreateDirectory(lpFilePath, NULL))
	{
		if (GetLastError() != ERROR_ALREADY_EXISTS)
			return FALSE;
	}

	DWORD dwCreationDisposition = (bForce) ? CREATE_ALWAYS : CREATE_NEW;
	TCHAR szFileName[MAX_PATH] = {0};
	StringCchPrintf(szFileName, MAX_PATH, _T("%s\\%s"), lpFilePath, lpFileName);

	HANDLE hFile = CreateFile(szFileName,
		GENERIC_WRITE | GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		dwCreationDisposition,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		return FALSE;
	}

	DWORD dwWritten = 0;
	if (!WriteFile(hFile, lpData, dwFileBytes, &dwWritten, NULL))
	{
		//WriteDebug(_T("GetFile_OUT: Fail to write file %s, errorcode is %d"), szFileName, ::GetLastError());
		CloseHandle(hFile);
		return FALSE;
	}

	CloseHandle(hFile);
	return TRUE;
}

BOOL CResourceHelper::ParseFileName(LPBYTE & lpData, LPTSTR lpFileName, DWORD cchLen)
{
	if (lpFileName == NULL)
	{
		::SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}

	StringCchCopy(lpFileName, cchLen, (LPCTSTR)lpData);
	lpData += (_tcslen(lpFileName) + 1) * sizeof(TCHAR);
	return TRUE;
}

BOOL CResourceHelper::ParseFileAttributes(LPBYTE & lpData, DWORD & dwFileAttributes)
{
	dwFileAttributes = *(LPDWORD)lpData;
	lpData += sizeof(DWORD);
	return TRUE;
}

BOOL CResourceHelper::ParseFileSize(LPBYTE & lpData, DWORD & dwFileSize)
{
	dwFileSize = *(UINT *)lpData;
	lpData += sizeof(UINT);
	return TRUE;
}

BOOL
CResourceHelper::ParseFileTime(
	LPBYTE & lpData,
	LPFILETIME lpCreationTime,
	LPFILETIME lpLastAccessTime,
	LPFILETIME lpLastWriteTime
	)
{
	*lpCreationTime = *(FILETIME *)lpData;
	lpData += sizeof(FILETIME);
	*lpLastAccessTime = *(FILETIME *)lpData;
	lpData += sizeof(FILETIME);
	*lpLastWriteTime = *(FILETIME *)lpData;
	lpData += sizeof(FILETIME);

	return TRUE;
}

BOOL
CResourceHelper::GetFileName(
	LPCTSTR lpResourceName,
	LPTSTR lpFileName,
	DWORD cchLen
	)
{
	if (lpResourceName == NULL || lpFileName == NULL || cchLen == 0)
	{
		::SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}

	DWORD dwDataBytes = 0;
	HGLOBAL hResData = LoadResource(lpResourceName, RT_RCDATA, &dwDataBytes);
	if (hResData == NULL)
	{
		return FALSE;
	}

	DWORD dwErrorCode = ERROR_SUCCESS;

	LPBYTE lpData = (LPBYTE)::LockResource(hResData);
	if (lpData == NULL)
	{
		::FreeResource(hResData);
		return FALSE;
	}

	StringCchCopy(lpFileName, cchLen, (LPCTSTR)lpData);

	::FreeResource(hResData);

	return TRUE;
}
