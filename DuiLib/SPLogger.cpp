#include "stdafx.h"
#include <windows.h>
#include "SPLogger.h"
#include <stdio.h>
#include <Shlwapi.h>
#include <shlobj.h>
#if _MSC_VER >= 1400
#include <codeanalysis\warnings.h>
#pragma warning( push )
#pragma warning ( disable : ALL_CODE_ANALYSIS_WARNINGS )
#endif

#include <strsafe.h>

#if _MSC_VER >= 1400
#pragma warning( pop )
#endif


#define LOG_MAX_SIZE				0x200000	// ÿ����־�ļ����2MB
#define LOG_MAX_BUFFER				512			// ÿ����־������ַ���

#ifndef countof
#define countof(buf) (sizeof(buf) / sizeof(TCHAR))
#endif

std::map<TCHAR*, CSPLogger*> CSPLogger::m_loggers;

CSPLogger::CSPLogger(TCHAR* logname)
: m_nFileSize(0)
#ifdef _DEBUG
, m_nLogLevel(CSPLogger::LOG_LEVEL_DEBUG)
#else
, m_nLogLevel(CSPLogger::LOG_LEVEL_INFO)
#endif
{
	ZeroMemory(m_szLogFileName, sizeof(m_szLogFileName));
	wchar_t sysdir[MAX_PATH] = {0};
#ifdef _WIN64
	if (0 == GetSystemWow64Directory(sysdir, MAX_PATH))
	{
		// ��ȡ����ϵͳ�ļ��У�Ĭ��ʹ��C��
		StringCchPrintf(sysdir, countof(sysdir), _T("C:"));
	}
#else
	if (0 == GetSystemDirectory(sysdir, MAX_PATH))
	{
		// ��ȡ����ϵͳ�ļ��У�Ĭ��ʹ��C��
		StringCchPrintf(sysdir, countof(sysdir), _T("C:"));
	}
#endif
	TCHAR tszModule[MAX_PATH + 1] = { 0 };
	////����־�ļ�д��AppData/Local�£��������ϴ�
	//::GetModuleFileName(NULL, tszModule, MAX_PATH);
	SHGetSpecialFolderPath(
		HWND_DESKTOP,
		tszModule,
		CSIDL_LOCAL_APPDATA,
		FALSE);
	/*TCHAR *pModulePos = _tcsrchr(tszModule, L'\\');
	if (pModulePos != NULL)
	{
	*pModulePos = L'\0';
	}*/
	//���ж�Ŀ¼�Ƿ���ڣ���������Ҫ�ȴ���Ŀ¼
	TCHAR szDirectoryLog[MAX_PATH] = { 0 };
	//StringCchPrintf(szDirectoryLog, _countof(szDirectoryLog), _T("%s\\%s"), tszModule, WSP_LOG_DIR);
	TCHAR szDirectoryDump[MAX_PATH] = { 0 };
	StringCchPrintf(szDirectoryDump, _countof(szDirectoryDump), _T("%s\\Haoti\\crashdump"), tszModule);
	if (!PathFileExists(szDirectoryDump))
	{
		CreateDirectory(szDirectoryDump, NULL);
	}
	StringCchPrintf(szDirectoryLog, _countof(szDirectoryLog), _T("%s\\Haoti\\crashdump\\%s"), tszModule, WSP_LOG_DIR);
	if (!PathFileExists(szDirectoryLog))
	{
		CreateDirectory(szDirectoryLog, NULL);
	}
	/*StringCchPrintf(m_szLogFileName, countof(m_szLogFileName),
	L"%s\\%s\\%s.log", tszModule, WSP_LOG_DIR, logname);*/
	StringCchPrintf(m_szLogFileName, countof(m_szLogFileName),
		L"%s\\Haoti\\crashdump\\%s\\%s.log", tszModule, WSP_LOG_DIR, logname);
}

CSPLogger* CSPLogger::getLogger(TCHAR* logname)
{
	CErrorProtect errProtect;

	if (logname == NULL)
	{
		return GetUnknownLogger();
	}

	try
	{
		std::map<TCHAR*, CSPLogger*>::iterator pos;
		for (pos = m_loggers.begin(); pos != m_loggers.end(); pos++)
		{
			if (_tcscmp(pos->first, logname) == 0)
			{
				return pos->second;
			}
		}
	}
	catch (...)
	{
		return GetUnknownLogger();
	}

	size_t nLength = 0;
	HRESULT hResult = StringCchLength(logname, MAX_PATH, &nLength);
	if (FAILED(hResult) || nLength == 0)
	{
		return GetUnknownLogger();
	}

	TCHAR* lgname = NULL;
	CSPLogger* logger = NULL;

	try
	{
		lgname = new TCHAR[nLength + 1];
		logger = new CSPLogger(logname);
	}
	catch (...)
	{
	}

	if (lgname == NULL || logger == NULL)
	{
		// ȷ��loggerʵ�������ɹ�
		if (lgname != NULL)
		{
			delete [] lgname;
			lgname = NULL;
		}

		if (logger != NULL)
		{
			delete logger;
			logger = NULL;
		}

		return GetUnknownLogger();
	}

	StringCchCopy(lgname, nLength + 1, logname);
	lgname[nLength] = 0;

	m_loggers[lgname] = logger;
	return logger;
}

void CSPLogger::writeDebug(TCHAR* formatString, ...)
{
	if (m_nLogLevel > LOG_LEVEL_DEBUG)
		return;

	CErrorProtect errProtect;
	TCHAR wstrBuf[LOG_MAX_BUFFER] = {0};
	try
	{
		va_list argList;
		va_start(argList, formatString);
		StringCchVPrintf(wstrBuf, countof(wstrBuf), formatString, argList);
		va_end(argList);

		WriteData(_T("DEBUG"), wstrBuf);
	}
	catch(...)
	{
	}
}

void CSPLogger::writeInfo(TCHAR* formatString, ...)
{
	if (m_nLogLevel > LOG_LEVEL_INFO)
		return;

	CErrorProtect errProtect;
	TCHAR wstrBuf[LOG_MAX_BUFFER] = {0};
	try
	{
		va_list argList;
		va_start(argList, formatString);
		StringCchVPrintf(wstrBuf, countof(wstrBuf), formatString, argList);
		va_end(argList);

		WriteData(_T(" INFO"), wstrBuf);
	}
	catch(...)
	{
	}
}

void CSPLogger::writeWarn(TCHAR* formatString, ...)
{
	if (m_nLogLevel > LOG_LEVEL_WARN)
		return;

	CErrorProtect errProtect;
	TCHAR wstrBuf[LOG_MAX_BUFFER] = {0};
	try
	{
		va_list argList;
		va_start(argList, formatString);
		StringCchVPrintf(wstrBuf, countof(wstrBuf), formatString, argList);
		va_end(argList);

		WriteData(_T(" WARN"), wstrBuf);
	}
	catch(...)
	{
	}
}

void CSPLogger::writeError(TCHAR* formatString, ...)
{
	if (m_nLogLevel > LOG_LEVEL_ERROR)
		return;

	CErrorProtect errProtect;
	TCHAR wstrBuf[LOG_MAX_BUFFER] = {0};
	try
	{
		va_list argList;
		va_start(argList, formatString);
		StringCchVPrintf(wstrBuf, countof(wstrBuf), formatString, argList);
		va_end(argList);

		WriteData(_T("ERROR"), wstrBuf);
	}
	catch(...)
	{
	}
}

void CSPLogger::writeFatal(TCHAR* formatString, ...)
{
	if (m_nLogLevel > LOG_LEVEL_FATAL)
		return;

	CErrorProtect errProtect;
	TCHAR wstrBuf[LOG_MAX_BUFFER] = {0};
	try
	{
		va_list argList;
		va_start(argList, formatString);
		StringCchVPrintf(wstrBuf, countof(wstrBuf), formatString, argList);
		va_end(argList);

		WriteData(_T("FATAL"), wstrBuf);
	}
	catch(...)
	{
	}
}

void CSPLogger::WriteData(const TCHAR* szLevel, const TCHAR* szMsg)
{
	HANDLE hFile = INVALID_HANDLE_VALUE;

	try
	{
		// check log file's size
		CheckLogFile();

		hFile = CreateFile(
			m_szLogFileName,			// file to open 
			GENERIC_WRITE,				// open for writing 
			0,							// do not share 
			NULL,						// default security 
			OPEN_ALWAYS,				// overwrite existing 
			FILE_ATTRIBUTE_NORMAL,		// normal file 
			NULL);						// no attr. template 

		if (INVALID_HANDLE_VALUE == hFile)
		{
			return;
		}

		DWORD dwLen = 0;

		if (ERROR_ALREADY_EXISTS != ::GetLastError())
		{
			// ����ļ������ڣ������״δ����ļ�������ļ���ʽ
			// write unicode flag
			WIN32_FILE_ATTRIBUTE_DATA attr;
			GetFileAttributesExW(m_szLogFileName, GetFileExInfoStandard, &attr);
			if (attr.nFileSizeLow == 0 && attr.nFileSizeHigh == 0)
			{
				WORD wSignature = 0xFEFF;
				WriteFile(hFile, &wSignature, 2, &dwLen, NULL);
				m_nFileSize += sizeof(wSignature);
			}
		}

		// move to the end of file
		SetFilePointer(hFile, 0, 0, FILE_END);

		// get time
		SYSTEMTIME tm;
		GetLocalTime(&tm);

		TCHAR* pszBuffer = NULL;

		// ��%04d-%02d-%02d %02d:%02d:%02d [%s] - ���ṹ����Ӳ���룬��ߴ����ٶ�
		// ���ݳ���Ϊ25��������ʽʱ��ض�Ӳ�������ݽ�����Ӧ�Ե���
		// \r\n����Ӳ���룬��ߴ����ٶȣ����ݳ���Ϊ2
		size_t nBufferLength = _tcslen(szLevel) + _tcslen(szMsg) + 25 + 2;
		try
		{
			pszBuffer = new TCHAR[nBufferLength + 1];
		}
		catch (...)
		{
		}

		if (pszBuffer == NULL)
		{
			return;
		}

		// ��ʽ������
		StringCchPrintf(pszBuffer, nBufferLength + 1,
			_T("%04d-%02d-%02d %02d:%02d:%02d [%s] - %s\r\n"),
			tm.wYear, tm.wMonth, tm.wDay,
			tm.wHour, tm.wMinute, tm.wSecond,
			szLevel, szMsg);

		// ��дһ��Ӳ�̣�����I/O����
		WriteFile(hFile, pszBuffer, (DWORD)nBufferLength * sizeof(TCHAR), &dwLen, NULL);
		delete [] pszBuffer;

		// ���µ�ǰ��־�ļ���С
		m_nFileSize += dwLen;

	}
	catch (...)
	{
	}

	if (hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
		hFile = INVALID_HANDLE_VALUE;
	}
}

void CSPLogger::CheckLogFile()
{
	if (0 == m_szLogFileName[0])
		return;

	if (m_nFileSize > 0 && m_nFileSize < LOG_MAX_SIZE)
	{
		// ��־�ļ���Сû�г�������ֵ
		return;
	}

	// �����־�ļ��������ƣ������µ���־�ļ�
	__try
	{
		WIN32_FILE_ATTRIBUTE_DATA attr;
		memset(&attr, 0, sizeof(attr));
		GetFileAttributesEx(m_szLogFileName, GetFileExInfoStandard, &attr);
		if (attr.nFileSizeLow > LOG_MAX_SIZE || attr.nFileSizeHigh > 0)
		{
			TCHAR szBakFile[MAX_PATH] = {0};
			StringCchCopy(szBakFile, countof(szBakFile), m_szLogFileName);
			size_t nLen = MAX_PATH;
			StringCchLength(szBakFile, MAX_PATH, &nLen);
			nLen -= 4;
			szBakFile[nLen] = 0;
			StringCchCat(szBakFile, countof(szBakFile), _T(".0.log"));

			MoveFileExW(m_szLogFileName, szBakFile,
				MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH);
			m_nFileSize = 0;
		}
		else
		{
			m_nFileSize = attr.nFileSizeLow;
		}
	}
	__except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ?
				EXCEPTION_EXECUTE_HANDLER : EXCEPTION_EXECUTE_HANDLER)
	{
		GetExceptionCode();
	}
}

inline CSPLogger* CSPLogger::GetUnknownLogger()
{
	static CSPLogger sLogger(_T("unknown"));
	return &sLogger;
}

void CSPLogger::SetLogLevel(int nLogLevel)
{
	if (nLogLevel < LOG_LEVEL_DEBUG)
	{
		return;
	}
	else if (nLogLevel > LOG_LEVEL_CLOSED)
	{
		return;
	}

	m_nLogLevel = nLogLevel;
	return;
}

int CSPLogger::GetLogLevel(void) const
{
	return m_nLogLevel;
}

