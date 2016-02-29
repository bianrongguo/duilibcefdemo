#include "stdafx.h"

#include <strsafe.h>
#include <atlbase.h>
#include "winversion.h"
#include "Winbase.h"
#include <Iphlpapi.h>
#pragma warning(disable:4996)
#pragma comment(lib,"Iphlpapi.lib")
CWinVersion::CWinVersion(void)
: m_bOpened(FALSE)
{
}

CWinVersion::~CWinVersion(void)
{
}

BOOL CWinVersion::CheckNTVersion()
{
	if (!OpenVersion())
		return FALSE;

	if (m_osvi.dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		if (m_osvi.dwMajorVersion >= 5)
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CWinVersion::CheckVistaVersion()
{
	if (!OpenVersion())
		return FALSE;

	if (m_osvi.dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		if (m_osvi.dwMajorVersion == 6 && m_osvi.dwMinorVersion == 0)
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CWinVersion::GetMajorVersion(DWORD& dwMajorVersion)
{
	if (!OpenVersion())
		return FALSE;

	dwMajorVersion = m_osvi.dwMajorVersion;

	return TRUE;
}

BOOL CWinVersion::GetMinorVersion(DWORD& dwMinorVersion)
{
	if (!OpenVersion())
		return FALSE;

	dwMinorVersion = m_osvi.dwMinorVersion;

	return TRUE;
}

BOOL CWinVersion::GetPackVersion(LPTSTR lpVersion)
{
	if (!OpenVersion())
		return FALSE;

	StringCchCopy(lpVersion, MAX_PATH, m_osvi.szCSDVersion);

	return TRUE;
}

BOOL CWinVersion::GetServicePackVersion(DWORD& dwVersion)
{
	if (!OpenVersion())
		return FALSE;

	dwVersion = m_osvi.wServicePackMajor;
	return TRUE;
}

BOOL CWinVersion::OpenVersion()
{
	if (!m_bOpened)
	{
		ZeroMemory(&m_osvi, sizeof(OSVERSIONINFOEX));
		m_osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

		if (!(GetVersionEx((OSVERSIONINFO *)&m_osvi)))
		{
			// If OSVERSIONINFOEX doesn't work,try OSVERSIONINFO.
			m_osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

			if (!GetVersionEx((OSVERSIONINFO *)&m_osvi))
			{
				return FALSE;
			}
		}

		m_bOpened = TRUE;
	}

	return TRUE;
}

//判断是否x64系统

BOOL CWinVersion::IsWow64()
{
	typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL); 
	LPFN_ISWOW64PROCESS fnIsWow64Process; 
	BOOL bIsWow64 = FALSE; 
	fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress( GetModuleHandle(_T("kernel32")),"IsWow64Process"); 
	if (NULL != fnIsWow64Process) 
	{ 
		fnIsWow64Process(GetCurrentProcess(),&bIsWow64);
	} 
	return bIsWow64; 
}

BOOL CWinVersion::IsWorkStation()
{
	if (!OpenVersion())
		return FALSE;

	return (m_osvi.wProductType == VER_NT_WORKSTATION) ? TRUE : FALSE;
}

std::wstring CWinVersion::GetOsVerDetail()
{
	OSVERSIONINFO  osvi;
	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);
	std::wstring strVer = _T("");
	if (osvi.dwPlatformId == 2)
	{
		//VER_PLATFORM_WIN32_NT windows 平台
		if (osvi.dwMajorVersion == 6)
		{
			switch (osvi.dwMinorVersion)
			{
			case 3:
				//Windows 8.1
				strVer += _T("Windows 8.1");
				break;
			case 2:
				strVer += _T("Windows 8");
				break;
			case 1:
				strVer += _T("Windows 7");
				break;
			case 0:
				strVer += _T("Windows Vista");
				break;
			default:
				break;
			}
		}
		else if (osvi.dwMajorVersion == 5)
		{
			switch (osvi.dwMinorVersion)
			{
			case 2:
				if (GetSystemMetrics(SM_SERVERR2) != 0)
					strVer += _T("Windows Server 2003 R2");
				else
					strVer += _T("Windows Server 2003");
				break;
			case 1:
				strVer += _T("Windows XP");
				break;
			case 0:
				strVer += _T("Windows 2000");
				break;
			default:
				break;
			}
		}

		TCHAR strbuild[20];
		StringCchPrintf(strbuild,20, _T(" Build:%d "), osvi.dwBuildNumber);
		strVer += strbuild;
		strVer += osvi.szCSDVersion;
		return strVer;
	}
	else
	{
		return _T("");
	}
}
		

void CWinVersion::GetOSVersion(std::wstring &strOSVersion, std::wstring &strServiceVersion)
{
	std::wstring str;
	OSVERSIONINFOEX osvi;
	SYSTEM_INFO si;
	BOOL bOsVersionInfoEx;

	ZeroMemory(&si, sizeof(SYSTEM_INFO));
	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));

	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	if (!(bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO *)&osvi)))
	{
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		GetVersionEx((OSVERSIONINFO *)&osvi);
	}


	GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")),
		"GetNativeSystemInfo");

	GetSystemInfo(&si);
	switch (osvi.dwPlatformId)
	{
	case VER_PLATFORM_WIN32_NT:
		if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0)
		{
			if (osvi.wProductType == VER_NT_WORKSTATION)
			{
				str = _T("Windows Vista ");
			}
			else
			{
				str = _T("Windows Server \"Longhorn\" ");
			}
		}
		if (osvi.dwMajorVersion == 6)
		{
			switch (osvi.dwMinorVersion !=0 )
			{
			case 3:
				//Windows 8.1
				str += _T("Windows 8.1");
				break;
			case 2:
				str += _T("Windows 8");
				break;
			case 1:
				str += _T("Windows 7");
				break;
			case 0:
				
				break;
			default:
				break;
			}
		}
		else if (osvi.dwMajorVersion == 10)
		{
			str += _T("Windows 10");
		}
		if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2)
		{
			if (GetSystemMetrics(SM_SERVERR2))
			{
				str = _T("Microsoft Windows Server 2003 \"R2\" ");
			}
			else if (osvi.wProductType == VER_NT_WORKSTATION &&
				si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
			{
				str = _T("Microsoft Windows XP Professional x64 Edition ");
			}
			else
			{
				str = _T("Microsoft Windows Server 2003, ");
			}
		}

		if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1)
		{
			str = _T("Microsoft Windows XP ");
		}

		if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0)
			str = _T("Microsoft Windows 2000 ");

		if (osvi.dwMajorVersion <= 4)
		{
			str = _T("Microsoft Windows NT ");
		}

		// Test for specific product on Windows NT 4.0 SP6 and later.
		if (bOsVersionInfoEx)
		{
			//将Service Pack 版本保存
			TCHAR temp[100] = { 0 };
			StringCchPrintf(temp, 100, _T(" Service Pack %d "), osvi.wServicePackMajor);
			strServiceVersion = temp;
			// Test for the workstation type.
			if (osvi.wProductType == VER_NT_WORKSTATION &&
				si.wProcessorArchitecture != PROCESSOR_ARCHITECTURE_AMD64)
			{
				if (osvi.dwMajorVersion == 4)
					str = str + _T("Workstation 4.0");
				else if (osvi.wSuiteMask & VER_SUITE_PERSONAL)
					str = str + _T("Home Edition");
				else str = str + _T("Professional");
			}

			// Test for the server type.
			else if (osvi.wProductType == VER_NT_SERVER ||
				osvi.wProductType == VER_NT_DOMAIN_CONTROLLER)
			{
				if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2)
				{
					if (si.wProcessorArchitecture ==
						PROCESSOR_ARCHITECTURE_IA64)
					{
						if (osvi.wSuiteMask & VER_SUITE_DATACENTER)
							str = str + _T("Datacenter Edition for Itanium-based Systems");
						else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
							str = str + _T("Enterprise Edition for Itanium-based Systems");
					}

					else if (si.wProcessorArchitecture ==
						PROCESSOR_ARCHITECTURE_AMD64)
					{
						if (osvi.wSuiteMask & VER_SUITE_DATACENTER)
							str = str + _T("Datacenter x64 Edition ");
						else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
							str = str + _T("Enterprise x64 Edition ");
						else str = str + _T("Standard x64 Edition ");
					}

					else
					{
						if (osvi.wSuiteMask & VER_SUITE_DATACENTER)
							str = str + _T("Datacenter Edition ");
						else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
							str = str + _T("Enterprise Edition ");
						else if (osvi.wSuiteMask & VER_SUITE_BLADE)
							str = str + _T("Web Edition ");
						else str = str + _T("Standard Edition ");
					}
				}
				else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0)
				{
					if (osvi.wSuiteMask & VER_SUITE_DATACENTER)
						str = str + _T("Datacenter Server ");
					else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
						str = str + _T("Advanced Server ");
					else str = str + _T("Server ");
				}
				else  // Windows NT 4.0 
				{
					if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
						str = str + _T("Server 4.0, Enterprise Edition ");
					else str = str + _T("Server 4.0 ");
				}
			}
		}
		// Test for specific product on Windows NT 4.0 SP5 and earlier
		else
		{
			HKEY hKey;
			TCHAR szProductType[256];
			DWORD dwBufLen = 256 * sizeof(TCHAR);
			LONG lRet;

			lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
				_T("SYSTEM\\CurrentControlSet\\Control\\ProductOptions"), 0, KEY_QUERY_VALUE, &hKey);
			if (lRet != ERROR_SUCCESS)
				strOSVersion = str;
			return;

			lRet = RegQueryValueEx(hKey, TEXT("ProductType"),
				NULL, NULL, (LPBYTE)szProductType, &dwBufLen);
			RegCloseKey(hKey);

			if ((lRet != ERROR_SUCCESS) ||
				(dwBufLen > 256 * sizeof(TCHAR)))
				strOSVersion = str;
			return;

			if (lstrcmpi(TEXT("WINNT"), szProductType) == 0)
				str = str + _T("Workstation ");
			if (lstrcmpi(TEXT("LANMANNT"), szProductType) == 0)
				str = str + _T("Server ");
			if (lstrcmpi(TEXT("SERVERNT"), szProductType) == 0)
				str = str + _T("Advanced Server ");

			TCHAR temp[100] = { 0 };
			StringCchPrintf(temp, 100, _T("%d.%d"),  osvi.dwMajorVersion, osvi.dwMinorVersion);
			str = temp;
		}

		// Display service pack (if any) and build number.

		if (osvi.dwMajorVersion == 4 &&
			lstrcmpi(osvi.szCSDVersion, TEXT("Service Pack 6")) == 0)
		{
			HKEY hKey;
			LONG lRet;

			// Test for SP6 versus SP6a.
			lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
				_T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Hotfix\\Q246009"), 0, KEY_QUERY_VALUE, &hKey);
			if (lRet == ERROR_SUCCESS)
			{
				TCHAR temp[100] = { 0 };
				StringCchPrintf(temp, 100, _T("ervice Pack 6a (Build %d)\n"), osvi.dwBuildNumber & 0xFFFF);
				str = temp;
			}
			else // Windows NT 4.0 prior to SP6a
			{
				_tprintf(TEXT("%s (Build %d)\n"), osvi.szCSDVersion, osvi.dwBuildNumber & 0xFFFF);
			}

			RegCloseKey(hKey);
		}
		else // not Windows NT 4.0 
		{
			_tprintf(TEXT("%s (Build %d)\n"), osvi.szCSDVersion, osvi.dwBuildNumber & 0xFFFF);
		}

		break;

		// Test for the Windows Me/98/95.
	case VER_PLATFORM_WIN32_WINDOWS:

		if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
		{
			str = _T("Microsoft Windows 95 ");
			if (osvi.szCSDVersion[1] == 'C' || osvi.szCSDVersion[1] == 'B')
				str = str + _T("OSR2 ");
		}

		if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
		{
			str = _T("Microsoft Windows 98 ");
			if (osvi.szCSDVersion[1] == 'A' || osvi.szCSDVersion[1] == 'B')
				str = str + _T("SE ");
		}
		if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90)
		{
			str = _T("Microsoft Windows Millennium Edition\n");
		}
		break;

	case VER_PLATFORM_WIN32s:
		str = _T("Microsoft Win32s\n");
		break;
	default:
		break;
	}

	strOSVersion = str;
}

void CWinVersion::GetCpuInfo(std::wstring &chProcessorName, std::wstring &chProcessorType, DWORD &dwNum, DWORD &dwMaxClockSpeed)
{

	std::wstring strPath = _T("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0");//注册表子键路径HKEY_LOCAL_MACHINE\HARDWARE\DESCRIPTION\System\CentralProcessor\0
	CRegKey regkey;//定义注册表类对象
	LONG lResult;//LONG型变量－反应结果
	lResult = regkey.Open(HKEY_LOCAL_MACHINE, strPath.c_str(), KEY_READ); //打开注册表键
	if (lResult != ERROR_SUCCESS)
	{
		return;
	}
	TCHAR chCPUName[50] = { 0 };
	DWORD dwSize = 50;

	//获取ProcessorNameString字段值
	if (ERROR_SUCCESS == regkey.QueryStringValue(_T("ProcessorNameString"), chCPUName, &dwSize))
	{
		chProcessorName = chCPUName;
	}

	//查询CPU主频
	DWORD dwValue;
	if (ERROR_SUCCESS == regkey.QueryDWORDValue(_T("~MHz"), dwValue))
	{
		dwMaxClockSpeed = dwValue;
	}
	regkey.Close();//关闭注册表
	//UpdateData(FALSE);

	//获取CPU核心数目
	SYSTEM_INFO si;
	memset(&si, 0, sizeof(SYSTEM_INFO));
	GetSystemInfo(&si);
	dwNum = si.dwNumberOfProcessors;

	switch (si.dwProcessorType)
	{
	case PROCESSOR_INTEL_386:
	{
		chProcessorType = _T("Intel 386 processor");
	}
	break;
	case PROCESSOR_INTEL_486:
	{
		chProcessorType = _T("Intel 486 Processor");
	}
	break;
	case PROCESSOR_INTEL_PENTIUM:
	{
		chProcessorType = _T("Intel Pentium Processor");
	}
	break;
	case PROCESSOR_INTEL_IA64:
	{
		chProcessorType = _T("Intel IA64 Processor");
	}
	break;
	case PROCESSOR_AMD_X8664:
	{
		chProcessorType = _T("AMD X8664 Processor");
	}
	break;
	default:
		chProcessorType = _T("未知");
		break;
	}
}

void  CWinVersion::GetMemoryInfo(DWORD &dwTotalPhys, DWORD &dwAvailPhys)
{
	MEMORYSTATUS   Mem;
	GlobalMemoryStatus(&Mem);
	dwTotalPhys = (DWORD)Mem.dwTotalPhys / (1024 * 1024);
	dwAvailPhys = (DWORD)Mem.dwAvailPhys / (1024 * 1024);
}

std::wstring CWinVersion::GetMacAddress()
{
	std::wstring strMac = _T("");
	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
	unsigned long stSize = sizeof(IP_ADAPTER_INFO);
	//调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量;其中stSize参数既是一个输入量也是一个输出量
	int nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
	if (ERROR_BUFFER_OVERFLOW == nRel)
	{
		delete pIpAdapterInfo;
		pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];
		nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
	}
	TCHAR szMac[MAX_PATH] = { 0 };
	if (ERROR_SUCCESS == nRel)
	{
		//输出网卡信息
		StringCchPrintf(szMac, sizeof(szMac), _T("%02X%02X%02X%02X%02X%02X"), pIpAdapterInfo->Address[0], pIpAdapterInfo->Address[1], pIpAdapterInfo->Address[2], pIpAdapterInfo->Address[3], pIpAdapterInfo->Address[4], pIpAdapterInfo->Address[5]);
	}
	//释放内存空间
	if (pIpAdapterInfo)
	{
		delete pIpAdapterInfo;
	}
	strMac = szMac;
	return strMac;
}

void CWinVersion::GetXYScreen(int &xScreen, int &yScreen)
{
	xScreen = GetSystemMetrics(SM_CXSCREEN);
	yScreen = GetSystemMetrics(SM_CYSCREEN);
}