#include "StdAfx.h"
#include "FileMove.h"
#include <strsafe.h>


#ifndef countof
#define countof(buf)	(sizeof(buf) / sizeof(TCHAR))
#endif


CFileMove::CFileMove(BOOL bIsWow64)
: m_bIsWow64(bIsWow64)
{
}

CFileMove::~CFileMove(void)
{
}

BOOL CFileMove::MoveFile(LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName, DWORD dwFlags)
{
	if (lpExistingFileName == NULL)
	{
		::SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}

	// 去掉只读属性
	RemoveReadonlyAttribute(lpExistingFileName);
	RemoveReadonlyAttribute(lpNewFileName);

	if (!m_bIsWow64)
	{
		return MoveFileEx(lpExistingFileName, lpNewFileName, dwFlags);
	}

	TCHAR szSystemWow64[MAX_PATH] = {0};
	if (!GetWow64Directory(szSystemWow64, countof(szSystemWow64)))
	{
		return FALSE;
	}

	TCHAR szSystem32[MAX_PATH] = {0};
	if (!GetSystemDirectory(szSystem32, countof(szSystem32)))
	{
		return FALSE;
	}

	TCHAR szSysNative[MAX_PATH] = {0};
	if (!GetWindowsDirectory(szSysNative, countof(szSysNative)))
	{
		return FALSE;
	}

	StringCchCat(szSysNative, countof(szSysNative), _T("\\Sysnative"));

	TCHAR szExistingFileName[MAX_PATH] = {0};
	TCHAR szNewFileName[MAX_PATH] = {0};
	TCHAR * pNewMoveFileName = NULL;

	RevertFsRedirection(lpExistingFileName, szSystem32, szSystemWow64,
		szExistingFileName, countof(szExistingFileName));

	RevertFsRedirection(szExistingFileName, szSysNative, szSystem32,
		szExistingFileName, countof(szExistingFileName));

	if (lpNewFileName != NULL)
	{
		RevertFsRedirection(lpNewFileName, szSystem32, szSystemWow64,
			szNewFileName, countof(szNewFileName));

		RevertFsRedirection(szNewFileName, szSysNative, szSystem32,
			szNewFileName, countof(szNewFileName));

		pNewMoveFileName = szNewFileName;
	}

	// 关闭32位程序在64位系统下的自动重定向功能

	PVOID pOldValue = NULL;
	if (!Wow64DisableWow64FsRedirection(&pOldValue))
	{
		return FALSE;
	}

	DWORD dwLastError = ERROR_SUCCESS;
	BOOL bMoveFile = MoveFileEx(szExistingFileName, pNewMoveFileName, dwFlags);
	if (!bMoveFile)
	{
		dwLastError = ::GetLastError();
	}

	// 恢复32位程序在64位系统下的自动重定向功能
	if (!Wow64RevertWow64FsRedirection(pOldValue))
	{
		return FALSE;
	}

	::SetLastError(dwLastError);
	return bMoveFile;
}

BOOL CFileMove::Wow64DisableWow64FsRedirection(PVOID *OldValue)
{
	typedef BOOL (WINAPI *LPFN_Wow64DisableWow64FsRedirection)(PVOID *); 
	LPFN_Wow64DisableWow64FsRedirection fnWow64DisableWow64FsRedirection;

	BOOL bReturn = FALSE;
	fnWow64DisableWow64FsRedirection = (LPFN_Wow64DisableWow64FsRedirection)
		GetProcAddress(GetModuleHandle(_T("kernel32")), "Wow64DisableWow64FsRedirection");
	if (NULL != fnWow64DisableWow64FsRedirection)
	{ 
		bReturn = fnWow64DisableWow64FsRedirection(OldValue);
	}

	return bReturn;
}

BOOL CFileMove::Wow64RevertWow64FsRedirection(PVOID OldValue)
{
	typedef BOOL (WINAPI *LPFN_Wow64RevertWow64FsRedirection)(PVOID); 
	LPFN_Wow64RevertWow64FsRedirection fnWow64RevertWow64FsRedirection;

	BOOL bReturn = FALSE;
	fnWow64RevertWow64FsRedirection = (LPFN_Wow64RevertWow64FsRedirection)
		GetProcAddress(GetModuleHandle(_T("kernel32")), "Wow64RevertWow64FsRedirection");
	if (NULL != fnWow64RevertWow64FsRedirection)
	{ 
		bReturn = fnWow64RevertWow64FsRedirection(OldValue);
	}

	return bReturn;
}

UINT CFileMove::GetWow64Directory(LPTSTR lpBuffer, UINT uSize)
{
#ifdef UNICODE
	typedef UINT (WINAPI *LPFN_GetSystemWow64Directory)(LPWSTR, UINT);
	LPCSTR lpProcName = "GetSystemWow64DirectoryW";
#else
	typedef UINT (WINAPI *LPFN_GetSystemWow64Directory)(LPSTR, UINT);
	LPCSTR lpProcName = "GetSystemWow64DirectoryA";
#endif

	LPFN_GetSystemWow64Directory fnGetSystemWow64Directory;

	UINT uReturn = 0;
	fnGetSystemWow64Directory = (LPFN_GetSystemWow64Directory)
		GetProcAddress(GetModuleHandle(_T("kernel32")), lpProcName);
	if (NULL != fnGetSystemWow64Directory)
	{
		uReturn = fnGetSystemWow64Directory(lpBuffer, uSize);
	}

	return uReturn;
}

void CFileMove::RevertFsRedirection(LPCTSTR lpFileName, LPCTSTR lpWow64Folder,
									LPCTSTR lpNativeFolder, LPTSTR lpResult,
									DWORD cchResult)
{
	if (lpResult == NULL || cchResult == 0)
	{
		return;
	}

	size_t nameLength = _tcslen(lpFileName);
	size_t wow64Length = _tcslen(lpWow64Folder);

	if (nameLength > wow64Length)
	{
		// 当文件路径和文件名存在替换的可能性时

		if (
			_tcsnicmp(lpFileName, lpWow64Folder, wow64Length) == 0
			&&
			(lpFileName[wow64Length] == _T('\\') || lpFileName[wow64Length] == _T('/'))
			)
		{
			// 替换文件路径为系统的真实路径
			StringCchPrintf(lpResult, cchResult, _T("%s%s"),
				lpNativeFolder, &lpFileName[wow64Length]);

			return;
		}
	}

	StringCchCopy(lpResult, cchResult, lpFileName);
	return;
}

BOOL CFileMove::RemoveReadonlyAttribute(LPCTSTR lpFileName)
{
	if (lpFileName == NULL)
	{
		::SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}

	DWORD dwAttrs = GetFileAttributes(lpFileName);
	if (INVALID_FILE_ATTRIBUTES == dwAttrs)
	{
		return FALSE;
	}

	if (FILE_ATTRIBUTE_READONLY & dwAttrs)
	{
		dwAttrs &= ~FILE_ATTRIBUTE_READONLY;
		return SetFileAttributes(lpFileName, dwAttrs);
	}

	return TRUE;
}
