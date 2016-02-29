#pragma once
#include <string>
class CWinVersion
{
public:
	CWinVersion(void);
	~CWinVersion(void);

	BOOL CheckNTVersion();

	BOOL CheckVistaVersion();

	BOOL GetMajorVersion(DWORD& dwMajorVersion);
	BOOL GetMinorVersion(DWORD& dwMinorVersion);

	BOOL GetPackVersion(LPTSTR lpVersion);

	BOOL GetServicePackVersion(DWORD& dwVersion);

	BOOL IsWow64();//huabin

	BOOL IsWorkStation();
	///获取系统信息
	std::wstring GetOsVerDetail();
	/********获取操作系统版本，Service pack版本、系统类型************/
	void GetOSVersion(std::wstring &strOSVersion, std::wstring &strServiceVersion);
	/***获取物理内存和可用内存大小***/
	void GetMemoryInfo(DWORD &dwTotalPhys, DWORD &dwAvailPhys);
	/****获取CPU名称、内核数目、主频*******/
	void GetCpuInfo(std::wstring &chProcessorName, std::wstring &chProcessorType, DWORD &dwNum, DWORD &dwMaxClockSpeed);
	//获取MAC地址
	std::wstring GetMacAddress();
	////获取屏幕分辨率
	void GetXYScreen(int &xScreen, int &yScreen);
private:
	BOOL OpenVersion();

private:
	OSVERSIONINFOEX m_osvi;
	BOOL m_bOpened;
};
