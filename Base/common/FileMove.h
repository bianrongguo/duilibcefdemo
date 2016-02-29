#pragma once

#include <windows.h>

class CFileMove
{
public:
	CFileMove(BOOL bIsWow64);
	~CFileMove(void);

	BOOL MoveFile(LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName, DWORD dwFlags);

private:
	BOOL Wow64DisableWow64FsRedirection(PVOID *OldValue);

	BOOL Wow64RevertWow64FsRedirection(PVOID OldValue);

	UINT GetWow64Directory(LPTSTR lpBuffer, UINT uSize);

	void RevertFsRedirection(LPCTSTR lpFileName, LPCTSTR lpWow64Folder,
		LPCTSTR lpNativeFolder, LPTSTR lpResult, DWORD cchResult);

	BOOL RemoveReadonlyAttribute(LPCTSTR lpFileName);

private:
	BOOL m_bIsWow64;

};
