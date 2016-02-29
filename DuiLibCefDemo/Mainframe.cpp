#include "stdafx.h"
#include "BrowserClient.h"
#include "Mainframe.h"
#include "common/CommonInterface.h"
#include "resource.h"

#define WM_CLOSE_BIG_REPORT_DLG					WM_USER+0x1084
#define WM_RESIZE_BORWSERLAYOUT					WM_USER + 0x1085

DUI_BEGIN_MESSAGE_MAP(CMainFrame, CNotifyPump)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()
CefRefPtr<CBrowserClient> g_webBrowserClient = NULL;
extern wstring g_strParames;
extern wstring g_strParames2;
extern wstring g_strParames3;
extern DWORD g_dwInterfaceType;
CMainFrame::CMainFrame()
{
}


CMainFrame::~CMainFrame()
{
}
CDuiString CMainFrame::GetSkinFolder()
{
	return _T("skin\\duilibcefdemo\\");
}

CDuiString CMainFrame::GetSkinFile()
{
	//if (g_dwInterfaceType == 0)
	//{
	return _T("MainFrame.xml");
	//}
	//else if (g_dwInterfaceType==1)
	//{
	//	return _T("MainFrame_Student.xml");
	//}
	//else if (g_dwInterfaceType == 2)
	//{
	//	return _T("BigReportWin.xml");
	//}
}

LPCTSTR CMainFrame::GetWindowClassName(void) const
{
	//if (g_dwInterfaceType == 0)
	//{
	return _T("WKGLDUICEFMainFrameWnd");
	//}
	//else if (g_dwInterfaceType == 1)
	//{
	//	return _T("WKGLStudenglayoutMainFrameWnd");
	//}
	//else if (g_dwInterfaceType == 2)
	//{
	//	return _T("WKGLReportlayoutMainFrameWnd");
	//}
}

void CMainFrame::OnClick(DuiLib::TNotifyUI& msg)
{
	__super::OnClick(msg);
}

LRESULT CMainFrame::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//if (g_dwInterfaceType == 2)
	//{
	//	HWND hd = FindWindow(_T("CourseManageMainFrameWnd"), NULL);
	//	if (hd)
	//	{
	//		CCommonInterface com;
	//		string strId = com.UtoAnsi(g_strParames3);
	//		int id = atoi(strId.c_str());
	//		::SendMessage(hd, WM_CLOSE_BIG_REPORT_DLG, id, 0);
	//	}
	//}
	
	::PostQuitMessage(1L);
	return __super::OnClose(uMsg, wParam, lParam, bHandled);
}
void CMainFrame::InitWindow()
{
	__super::InitWindow();
	HICON hIcon1 = LoadIcon(DuiLib::CPaintManagerUI::GetInstance(), MAKEINTRESOURCE(IDI_SMALL));
	HICON hIcon2 = LoadIcon(DuiLib::CPaintManagerUI::GetInstance(), MAKEINTRESOURCE(IDI_DUILIBCEFDEMO));
	::SendMessage((HWND)GetHWND(), WM_SETICON, ICON_BIG, (LPARAM)hIcon2);
	::SendMessage((HWND)GetHWND(), WM_SETICON, ICON_SMALL, (LPARAM)hIcon1);
	m_layout_browser = static_cast<CVerticalLayoutUI *>(m_PaintManager.FindControl(_T("BrowserClientLayout")));
	CLabelUI *pTitleText = static_cast<CLabelUI *>(m_PaintManager.FindControl(_T("titleText")));
	if (pTitleText != NULL)
	{
		pTitleText->SetText(_T("duilibCefDemo"));
		//if (g_dwInterfaceType == 1)
		//{
		//	wstring strTitle = _T("");
		//	strTitle += _T("学生端预览：");
		//	strTitle += g_strParames2;
		//	pTitleText->SetText(strTitle.c_str());
		//	pTitleText->SetToolTip(strTitle.c_str());
		//}
		//else if (g_dwInterfaceType == 2)
		//{
		//	wstring strTitle = g_strParames2;
		//	pTitleText->SetText(strTitle.c_str());
		//}
	}
	CreateBrowser();
}
void CMainFrame::CreateBrowser()
{
	//初始化创建浏览器
	g_webBrowserClient = new CBrowserClient();
	//在浏览器中设置主窗口为MainFrame
	g_webBrowserClient->SetMainHwnd(*this);
	//m_webClient.get()->GetGlobalManager().get()->SetStoragePath(_T("C:\\Users\\admin\\AppData\\Local\\Haoti"));
	RECT rect = { 0, 0, 1, 1 };
	//创建一个MainFrame的子窗口
	CefWindowInfo info;
	info.SetAsChild(*this, rect);
	//无透明值
	info.SetTransparentPainting(FALSE);

	//在这里指定的CEF的浏览器设置。
	CefBrowserSettings browserSettings;
	CefString(&browserSettings.default_encoding).FromASCII("UTF-8");
	browserSettings.universal_access_from_file_urls = STATE_ENABLED;
	//正式创建浏览器窗口
	CCommonInterface ComInterface;
	
	//TCHAR szDebugInfo[MAX_PATH] = { 0 };
	//wsprintf(szDebugInfo, _T("...videopath = %s"), g_strParames.c_str());
	//OutputDebugString(szDebugInfo);
	//string strPar = ComInterface.UtoUtf8(g_strParames);
	CefBrowserHost::CreateBrowser(info, g_webBrowserClient.get(),
		"www.daydays.com", browserSettings, NULL);
}

LRESULT CMainFrame::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_MOVE:
	{
		ResizeClientWnd();
		break;
	}
	case WM_SIZE:
	{
		PostMessage(WM_RESIZE_BORWSERLAYOUT, 0, 0);
		ResizeClientWnd();
		break;
	}
	case WM_NCLBUTTONDBLCLK:
	{
		ResizeClientWnd();
		break;
	}
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_RETURN:
			return 0;
		case VK_SPACE:
			return 0;
		default:
			break;
		}
	}
	default:
		break;
	}
	return DuiLib::WindowImplBase::HandleMessage(uMsg, wParam, lParam);
}
LRESULT CMainFrame::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//if (uMsg == WM_AFTER_BROSWER_CREATE)
	//{
	//	ResizeClientWnd();
	//}
	CButtonUI * pButton = NULL;
	switch (uMsg)
	{
	case WM_RESIZE_BORWSERLAYOUT:
	case WM_AFTER_BROSWER_CREATE:
	{
		ResizeClientWnd();
		break;
	}
	default:
	break;
	}
	return 1;
}

void CMainFrame::Notify(TNotifyUI& msg)
{
	__super::Notify(msg);
}
//重画子窗口
void CMainFrame::ResizeClientWnd()
{
	if (g_webBrowserClient.get())
	{
		CefRefPtr<CefBrowser> browser = g_webBrowserClient->GetBrowser();
		if (browser&&browser->GetIdentifier() == g_webBrowserClient->m_BrowserId)
		{
			::UpdateWindow(this->GetHWND());
			DuiLib::CDuiRect rc;
			rc = m_layout_browser->GetPos();
			g_webBrowserClient->m_ClientRc = rc;
			if (browser && !rc.IsNull())
			{
				CefWindowHandle hwnd = browser->GetHost()->GetWindowHandle();
				wchar_t arr[128] = { 0 };
				//if (g_dwInterfaceType == 0 || g_dwInterfaceType == 2)
				//{
				::MoveWindow(hwnd, rc.left, rc.top, rc.GetWidth(), rc.GetHeight(), TRUE);
				//}
				//else
				//{
				//	::MoveWindow(hwnd, rc.left+12, rc.top+12, rc.GetWidth()-15, rc.GetHeight()-20, TRUE);
				//}
			}
		}
	}
}