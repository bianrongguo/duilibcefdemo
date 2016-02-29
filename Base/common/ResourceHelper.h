#pragma once

// GetFile_OUTEx�����Ĳ���
#define CREATEFILE_REPLACE_EXISTING		0x00000001
#define CREATEFILE_SUCCESS_IF_EXIST		0x00000002

class CResourceHelper
{
public:
	CResourceHelper(void);
	CResourceHelper(LPCTSTR lpLibFileName);
	~CResourceHelper(void);

	BOOL GetResourceData(LPCTSTR lpName, DWORD& dwData);

	// ע��: pData�����ķ���ֵ��Ҫʹ��FreeResourceData���������ͷ��ڴ�.
	BOOL GetResourceData(LPCTSTR lpName, LPVOID * pData, LPDWORD lpDataBytes);
	BOOL GetResourceData(LPCTSTR lpName, LPCTSTR lpType, LPVOID * pData, LPDWORD lpDataBytes);

	// �ͷŻ�ȡ��Դʱ������ڴ���Դ.
	BOOL FreeResourceData(LPVOID pData);

	// ��ȡָ����Դ��������,��������Դ��Ϣ����ָ���ļ�
	BOOL GetFile_OUT(LPCTSTR lpResourceName, LPCTSTR lpFilePath, BOOL bForce = FALSE);

	// ��ȡָ����Դ��������,��������Դ��Ϣ����ָ���ļ�
	BOOL GetFile_OUTEx(LPCTSTR lpResourceName, LPCTSTR lpFilePath, DWORD dwFlags);

	// ��ȡָ����Դ��������,������ָ���ļ����ƴ����ļ�
	BOOL GetFile_OUT(LPCTSTR lpResourceName, LPCTSTR lpFilePath, LPCTSTR lpFileName, BOOL bForce);

private:

	// ������Դ,����ȡ��Դ��С.
	HGLOBAL LoadResource(LPCTSTR lpName, LPCTSTR lpType, LPDWORD lpDataBytes);

	// ������Դ�ж����ļ������ļ�
	BOOL ParseFile(LPCTSTR lpFilePath, LPBYTE lpData, DWORD dwDataBytes, BOOL bForce);

	// ����Ԥ�����ļ������ļ�
	BOOL ParseFile(LPCTSTR lpFilePath, LPCTSTR lpFileName, LPBYTE lpData, DWORD dwFileBytes, BOOL bForce);

	// �����ļ���.
	BOOL ParseFileName(LPBYTE & lpData, LPTSTR lpFileName, DWORD cchLen);

	// ����Դ�л�ȡ�ļ���
	BOOL GetFileName(LPCTSTR lpResourceName, LPTSTR lpFileName, DWORD cchLen);

	// �����ļ�����
	BOOL ParseFileAttributes(LPBYTE & lpData, DWORD & dwFileAttributes);

	// �����ļ���С
	BOOL ParseFileSize(LPBYTE & lpData, DWORD & dwFileSize);

	// �����ļ�����
	BOOL ParseFileData(LPCTSTR lpFileName, LPBYTE & lpData);

	// �����ļ�ʱ��
	BOOL ParseFileTime(LPBYTE & lpData, LPFILETIME lpCreationTime, LPFILETIME lpLastAccessTime, LPFILETIME lpLastWriteTime);

private:
	HMODULE m_hModule;

};
