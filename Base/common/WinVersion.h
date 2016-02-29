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
	///��ȡϵͳ��Ϣ
	std::wstring GetOsVerDetail();
	/********��ȡ����ϵͳ�汾��Service pack�汾��ϵͳ����************/
	void GetOSVersion(std::wstring &strOSVersion, std::wstring &strServiceVersion);
	/***��ȡ�����ڴ�Ϳ����ڴ��С***/
	void GetMemoryInfo(DWORD &dwTotalPhys, DWORD &dwAvailPhys);
	/****��ȡCPU���ơ��ں���Ŀ����Ƶ*******/
	void GetCpuInfo(std::wstring &chProcessorName, std::wstring &chProcessorType, DWORD &dwNum, DWORD &dwMaxClockSpeed);
	//��ȡMAC��ַ
	std::wstring GetMacAddress();
	////��ȡ��Ļ�ֱ���
	void GetXYScreen(int &xScreen, int &yScreen);
private:
	BOOL OpenVersion();

private:
	OSVERSIONINFOEX m_osvi;
	BOOL m_bOpened;
};
