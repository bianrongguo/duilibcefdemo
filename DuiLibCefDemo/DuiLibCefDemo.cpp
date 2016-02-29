// DuiLibCefDemo.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "cefclient/client_app.h"
#include "DuiLibCefDemo.h"
#include "Mainframe.h"
#include <ShlObj.h>
#include <wchar.h>
#include "common/CommonInterface.h"
//启动参数，启视频地址
wstring g_strParames = _T("");
wstring g_strParames2 = _T("");//第二个参数
wstring g_strParames3 = _T("");//第三个参数

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	CPaintManagerUI::SetInstance(hInstance);
#ifdef _DEBUG
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("..\\Release\\skin\\duilibcefdemo"));
#else
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin\\duilibcefdemo"));
#endif
	HRESULT Hr = ::CoInitialize(NULL);
	if (FAILED(Hr)) return 0;

	/***************************************begin初始化cef*******************************************/
	CefMainArgs main_args(hInstance);
	CefRefPtr<ClientApp> spApp(new ClientApp);

	// Execute the secondary process, if any.
	int exit_code = CefExecuteProcess(main_args, spApp.get());
	if (exit_code >= 0)
		return exit_code;
	CefSettings cSettings;
	TCHAR szFolderPath[MAX_PATH] = { 0 };
	SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA | CSIDL_FLAG_CREATE, NULL, 0, szFolderPath);
	wcscat_s(szFolderPath, _T("\\CefDemo\\main"));
	cSettings.log_severity = LOGSEVERITY_DISABLE;
	TCHAR szlogPath[_MAX_PATH] = { 0 };
	wstring logpath = szFolderPath;
	logpath += _T("\\Runlog");
	if (!PathFileExists(logpath.c_str()))
	{
		CCommonInterface nCommonInterface;
		nCommonInterface.CreateDir(logpath.c_str());
	}
	logpath += _T("\\run.log");
	wcscat_s(szlogPath, logpath.c_str());
	CefString(&cSettings.log_file).FromWString(szlogPath);
	////////////////////////////////////////
	CefString(&cSettings.cache_path).FromWString(szFolderPath);
	// Execute the secondary process, if any.
	CefInitialize(main_args, cSettings, spApp.get());
	/***************************************结束初始化cef*******************************************/

	CMainFrame * pFrame = new CMainFrame();
	if (pFrame == NULL) return 0;
	pFrame->Create(NULL, _T("名师云平台"), UI_WNDSTYLE_FRAME, 0);
	pFrame->CenterWindow();
	pFrame->ShowWindow(TRUE);
	if (!cSettings.multi_threaded_message_loop) {
		// Run the CEF message loop. This function will block until the application
		// recieves a WM_QUIT message.
		CefRunMessageLoop();
	}
	else {
		DuiLib::CPaintManagerUI::MessageLoop();
	}

	CefShutdown();

	::CoUninitialize();
	return 1;
}