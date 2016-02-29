#include "stdafx.h"
#include "BrowserClient.h"
#include "include/cef_app.h"
#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/cef_frame.h"
#include "include/cef_runnable.h"
#include "include/cef_web_plugin.h"
#include "UIlib.h"
#include <ShellAPI.h>

#define REQUIRE_UI_THREAD()   ASSERT(CefCurrentlyOn(TID_UI));
#define REQUIRE_IO_THREAD()   ASSERT(CefCurrentlyOn(TID_IO));
#define REQUIRE_FILE_THREAD() ASSERT(CefCurrentlyOn(TID_FILE));
extern std::wstring Utf8toU(std::string arg);
CBrowserClient::CBrowserClient()
{
	m_BrowserCount = 0;
	m_ClientRc = { 0 };
}


CBrowserClient::~CBrowserClient()
{
	m_main_wnd = NULL;
}

void CBrowserClient::SetMainHwnd(CefWindowHandle hwnd)
{
	AutoLock lock_scope(this);
	m_main_wnd = hwnd;
}

void CBrowserClient::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
	AutoLock lock_scope(this);
	std::wstring url = browser->GetMainFrame()->GetURL();
	if (!m_browser.get())
	{
		m_browser = browser;
		m_BrowserId = browser->GetIdentifier();
		::PostMessage(m_main_wnd, WM_AFTER_BROSWER_CREATE, 0, (WPARAM)(HWND)browser->GetHost()->GetWindowHandle());
	}	
	else if (browser->IsPopup())
	{
	}
	m_BrowserCount++;
	::MoveWindow(browser->GetHost()->GetWindowHandle(), m_ClientRc.left, m_ClientRc.top, m_ClientRc.right - m_ClientRc.left, m_ClientRc.bottom - m_ClientRc.top, true);
}
void CBrowserClient::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
	REQUIRE_UI_THREAD();
	// Protect data members from access on multiple threads.
	AutoLock lock_scope(this);

	if (m_BrowserId == browser->GetIdentifier()) {
		// Free the browser pointer so that the browser can be destroyed.
		m_browser = NULL;
	}
	else if (browser->IsPopup()) {
		std::multimap<std::wstring/*browsername*/, CefRefPtr<CefBrowser>>::iterator bit(m_ChildBrowsers.begin());

		for (; m_ChildBrowsers.size() > 0; bit++)
		{
			if ((bit->second)->IsSame(browser))
			{
				std::pair<std::wstring, CefRefPtr<CefBrowser>> cur = *bit;
				cur.second= NULL;
				m_ChildBrowsers.erase(bit);
				break;
				//break;
			}

		}
		//m_ChildBrowsers.clear();
	}
	if (--m_BrowserCount == 0) {
		// All browser windows have closed. Quit the application message loop.
		//CefQuitMessageLoop();
	}
}
bool CBrowserClient::DoClose(CefRefPtr<CefBrowser> browser) {
	// Must be executed on the UI thread.
	REQUIRE_UI_THREAD();
	// Protect data members from access on multiple threads.
	AutoLock lock_scope(this);

	// Closing the main window requires special handling. See the DoClose()
	// documentation in the CEF header for a detailed description of this
	// process.
	if (m_BrowserId == browser->GetIdentifier()) {
		// Notify the browser that the parent window is about to close.
		browser->GetHost()->ParentWindowWillClose();

		// Set a flag to indicate that the window close should be allowed.
		m_bIsClosing = true;
	}

	// Allow the close. For windowed browsers this will result in the OS close
	// event being sent.
	return false;
}
bool CBrowserClient::OnBeforePopup(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	const CefString& target_url,
	const CefString& target_frame_name,
	const CefPopupFeatures& popupFeatures,
	CefWindowInfo& windowInfo,
	CefRefPtr<CefClient>& client,
	CefBrowserSettings& settings,
	bool* no_javascript_access)
{
	std::wstring urlFind = target_url;
	//if (!JugeIsAdvertisementWindow(target_url))
	//{
	//	RECT rc = { 0, 0, 1, 1, };
	//	windowInfo.SetAsChild(m_main_wnd, rc);
	//}
	//else
	{
		InvokeDefaultBrowser(urlFind);
		return true;
	}

	return false;
}
bool CBrowserClient::OnContextMenuCommand(
	CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefContextMenuParams> params,
	int command_id, EventFlags event_flags) {
	switch (command_id) {
	case 1:
		return true;
	default:  // Allow default handling, if any.
		return false;
	}
}
void CBrowserClient::OnBeforeContextMenu(
	CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefContextMenuParams> params,
	CefRefPtr<CefMenuModel> model) {
	if ((params->GetTypeFlags() & (CM_TYPEFLAG_PAGE | CM_TYPEFLAG_FRAME)) != 0) {
		// Add a separator if the menu already has items.
		//modify by lch 2014-5-23
		model->Clear();
		return;
		if (model->GetCount() > 0)
		{
			//for (int index = 0; index < model->GetCount(); index++)
			//{
			//	if (!JugeMenuToShow(model->GetCommandIdAt(index)))
			//	{
			//		model->Clear();
			//		return;
			//	}
			//	else
			//		break;
			//}
			model->Remove(MENU_ID_PRINT);
			model->Remove(MENU_ID_FORWARD);
			model->Remove(MENU_ID_BACK);
			model->Remove(MENU_ID_VIEW_SOURCE);
			//model->AddSeparator();
		}
	}
}
bool CBrowserClient::OnDragEnter(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefDragData> dragData,
	DragOperationsMask mask){
	if (mask & DRAG_OPERATION_LINK)
		return true;
	return false;
}

void CBrowserClient::OnBeforeDownload(
	CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefDownloadItem> download_item,
	const CefString& suggested_name,
	CefRefPtr<CefBeforeDownloadCallback> callback) {
	REQUIRE_UI_THREAD();
}
void CBrowserClient::OnDownloadUpdated(
	CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefDownloadItem> download_item,
	CefRefPtr<CefDownloadItemCallback> callback) {
	REQUIRE_UI_THREAD();
}
bool CBrowserClient::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) {
	if (!m_OSRHandler.get())
		return false;
	return m_OSRHandler->GetViewRect(browser, rect);
}
void CBrowserClient::OnPaint(CefRefPtr<CefBrowser> browser,
	PaintElementType type,
	const RectList& dirtyRects,
	const void* buffer,
	int width,
	int height) {
	if (!m_OSRHandler.get())
		return;
	m_OSRHandler->OnPaint(browser, type, dirtyRects, buffer, width, height);
}
extern std::wstring GetUrlParam(std::wstring url, LPCTSTR lpParame);
extern BOOL WStringToString(const std::wstring &wstr, std::string &str);
void CBrowserClient::OnLoadStart(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame)
{
	std::wstring url = browser->GetMainFrame()->GetURL();
	return __super::OnLoadStart(browser, frame);
}
void CBrowserClient::OnLoadEnd(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	int httpStatusCode)
{
	std::wstring url = browser->GetMainFrame()->GetURL();
}

void CBrowserClient::OnLoadError(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	ErrorCode errorCode,
	const CefString& errorText,
	const CefString& failedUrl)
{
}
void CBrowserClient::CloseAllBrowsers(bool force_close) {
	if (!CefCurrentlyOn(TID_UI)) {
		// Execute on the UI thread.
		CefPostTask(TID_UI,
			NewCefRunnableMethod(this, &CBrowserClient::CloseAllBrowsers,
			force_close));
		return;
	}

	if (!m_ChildBrowsers.empty()) {
		// Request that any popup browsers close.
		BrowserMap::const_iterator it = m_ChildBrowsers.begin();
		int i = 0;
		for (; it != m_ChildBrowsers.end(); ++it)
		{
			if ((*it).second != NULL)
			{		
				(*it).second->GetHost()->CloseBrowser(force_close);
				std::pair<std::wstring, CefRefPtr<CefBrowser>> cur = *it;
				cur.second = NULL;
			}
			i++;
		}
	}
	m_ChildBrowsers.clear();
	if (m_browser != NULL && m_browser.get())
	{
		m_browser->GetHost()->CloseBrowser(force_close);
		m_browser = NULL;
	}
	//if (m_browser.get()) {
	//	// Request that the main browser close.
	//	m_browser->GetHost()->CloseBrowser(force_close);
	//}
}
bool  CBrowserClient::JugeIsAdvertisementWindow(CefString strUrl)
{
	std::wstring url = strUrl;
	if ((-1 != url.find(_T("https"))) ||
		(-1 != url.find(_T("http"))))
	{
		return false;
	}
	return true;
}
void CBrowserClient::InvokeDefaultBrowser(std::wstring url)
{
	::ShellExecuteW(NULL, _T("open"), url.c_str(), NULL, NULL, SW_NORMAL);
}