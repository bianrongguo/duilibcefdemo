#pragma once
#include "include\cef_client.h"
#define USING_CEF_SHARED
#include "include/capi/cef_dom_capi.h"
#include "cefclient/cefclient.h"
class CBrowserClient
	: virtual public CefClient
	, virtual public CefLifeSpanHandler
	, virtual public CefLoadHandler
	, virtual public CefRequestHandler
	, virtual public CefDownloadHandler
	, virtual public CefRenderHandler
	, virtual public CefCookieVisitor
	, virtual public CefContextMenuHandler
	, virtual public CefCompletionHandler
	, virtual public CefDragHandler
{
public:
	CBrowserClient();
	~CBrowserClient();
	class RenderHandler : public CefRenderHandler {
	public:
		virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) = 0;
	};
	virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE{
		return this;
	}
	virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE
	{ return this; }
	virtual CefRefPtr<CefRequestHandler> GetRequestHandler() OVERRIDE{
		return this;
	}
	virtual CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() OVERRIDE{
		return this;
	}
	virtual bool OnDragEnter(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefDragData> dragData,
		DragOperationsMask mask) OVERRIDE;
	bool OnContextMenuCommand(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefContextMenuParams> params,
		int command_id, EventFlags event_flags)OVERRIDE;
	void OnBeforeContextMenu(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefContextMenuParams> params,
		CefRefPtr<CefMenuModel> model)OVERRIDE;
	virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
	virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
	virtual bool OnBeforePopup(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		const CefString& target_url,
		const CefString& target_frame_name,
		const CefPopupFeatures& popupFeatures,
		CefWindowInfo& windowInfo,
		CefRefPtr<CefClient>& client,
		CefBrowserSettings& settings,
		bool* no_javascript_access) OVERRIDE;
	virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		int httpStatusCode) OVERRIDE;
	virtual void OnLoadStart(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame) OVERRIDE;
	virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		ErrorCode errorCode,
		const CefString& errorText,
		const CefString& failedUrl) OVERRIDE;
	void OnBeforeDownload(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefDownloadItem> download_item,
		const CefString& suggested_name,
		CefRefPtr<CefBeforeDownloadCallback> callback)OVERRIDE;
	void OnDownloadUpdated(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefDownloadItem> download_item,
		CefRefPtr<CefDownloadItemCallback> callback)OVERRIDE;
	virtual bool GetViewRect(CefRefPtr<CefBrowser> browser,
		CefRect& rect) OVERRIDE;
	virtual void OnPaint(CefRefPtr<CefBrowser> browser,
		PaintElementType type,
		const RectList& dirtyRects,
		const void* buffer,
		int width,
		int height) OVERRIDE;
	virtual bool Visit(const CefCookie& cookie, int count, int total,
		bool& deleteCookie)OVERRIDE
	{
		return true;
	};
	virtual void OnComplete()OVERRIDE
	{
		//cookie flush is completed
		//comment add by lch 
	}
	bool DoClose(CefRefPtr<CefBrowser> browser);
	void SetMainHwnd(CefWindowHandle hwnd);
	CefRefPtr<CefBrowser> GetBrowser() { return m_browser; }
	void InvokeDefaultBrowser(std::wstring url);
	bool JugeIsAdvertisementWindow(CefString strUrl);
	void CloseAllBrowsers(bool force_close);
	typedef std::multimap<std::wstring/*browsername*/, CefRefPtr<CefBrowser>> BrowserMap;
	BrowserMap m_ChildBrowsers;
	std::wstring m_strSinaAccessBackURL;
	std::wstring m_strQQAccessBackURL;
	CefRefPtr<CefBrowser> m_browser;
	//main browser rect
	RECT m_ClientRc;
	int m_BrowserId;
private:
	CefRefPtr<RenderHandler> m_OSRHandler;
	CefRefPtr<CefCookieManager> cookiemange = NULL;
	CefWindowHandle m_main_wnd = NULL;
	
	int m_BrowserCount;
	bool m_bIsClosing;

protected:
	IMPLEMENT_REFCOUNTING(CBrowserClient);
	IMPLEMENT_LOCKING(CBrowserClient);
};

