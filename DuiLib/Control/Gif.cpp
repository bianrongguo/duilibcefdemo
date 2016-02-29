#include "stdafx.h"
#include "Gif.h"

namespace DuiLib
{
	CGifUI::CGifUI()
		:m_pGif(NULL)
		, m_nPreUpdateDelay(0)
		, m_isUpdateTime(false)
	{
	}

	CGifUI::~CGifUI()
	{
		m_pManager->KillTimer(this, GIF_TIMER_ID);

		if (m_pGif)
		{
			delete m_pGif;
			m_pGif = NULL;
		}
	}

	void CGifUI::SetVisible(bool bVisible /*= true*/)
	{
		if (bVisible == false)
			m_pManager->KillTimer(this, GIF_TIMER_ID);

		CButtonUI::SetVisible(bVisible);
	}

	static COLORREF PixelAlpha(COLORREF clrSrc, double src_darken, COLORREF clrDest, double dest_darken)
	{
		return RGB(GetRValue(clrSrc) * src_darken + GetRValue(clrDest) * dest_darken,
			GetGValue(clrSrc) * src_darken + GetGValue(clrDest) * dest_darken,
			GetBValue(clrSrc) * src_darken + GetBValue(clrDest) * dest_darken);
		//return RGB(GetRValue(clrDest), GetGValue(clrDest), GetBValue(clrDest));
	}

	static BOOL WINAPI AlphaBitBlt(HDC hDC, int nDestX, int nDestY, int dwWidth, int dwHeight, HDC hSrcDC, \
		int nSrcX, int nSrcY, int wSrc, int hSrc, BLENDFUNCTION ftn)
	{
		HDC hTempDC = ::CreateCompatibleDC(hDC);
		if (NULL == hTempDC)
			return FALSE;
		LPBITMAPINFO lpbiSrc = NULL;
		lpbiSrc = (LPBITMAPINFO) new BYTE[sizeof(BITMAPINFOHEADER)];
		if (lpbiSrc == NULL)
		{
			::DeleteDC(hTempDC);
			return FALSE;
		}
		lpbiSrc->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		lpbiSrc->bmiHeader.biWidth = dwWidth;
		lpbiSrc->bmiHeader.biHeight = dwHeight;
		lpbiSrc->bmiHeader.biPlanes = 1;
		lpbiSrc->bmiHeader.biBitCount = 32;
		lpbiSrc->bmiHeader.biCompression = BI_RGB;
		lpbiSrc->bmiHeader.biSizeImage = dwWidth * dwHeight;
		lpbiSrc->bmiHeader.biXPelsPerMeter = 0;
		lpbiSrc->bmiHeader.biYPelsPerMeter = 0;
		lpbiSrc->bmiHeader.biClrUsed = 0;
		lpbiSrc->bmiHeader.biClrImportant = 0;

		COLORREF* pSrcBits = NULL;
		HBITMAP hSrcDib = CreateDIBSection(
			hSrcDC, lpbiSrc, DIB_RGB_COLORS, (void **)&pSrcBits,
			NULL, NULL);

		if ((NULL == hSrcDib) || (NULL == pSrcBits))
		{
			delete[] lpbiSrc;
			::DeleteDC(hTempDC);
			return FALSE;
		}

		HBITMAP hOldTempBmp = (HBITMAP)::SelectObject(hTempDC, hSrcDib);
		::StretchBlt(hTempDC, 0, 0, dwWidth, dwHeight, hSrcDC, nSrcX, nSrcY, wSrc, hSrc, SRCCOPY);
		::SelectObject(hTempDC, hOldTempBmp);

		LPBITMAPINFO lpbiDest = NULL;
		lpbiDest = (LPBITMAPINFO) new BYTE[sizeof(BITMAPINFOHEADER)];
		if (lpbiDest == NULL)
		{
			delete[] lpbiSrc;
			::DeleteObject(hSrcDib);
			::DeleteDC(hTempDC);
			return FALSE;
		}

		lpbiDest->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		lpbiDest->bmiHeader.biWidth = dwWidth;
		lpbiDest->bmiHeader.biHeight = dwHeight;
		lpbiDest->bmiHeader.biPlanes = 1;
		lpbiDest->bmiHeader.biBitCount = 32;
		lpbiDest->bmiHeader.biCompression = BI_RGB;
		lpbiDest->bmiHeader.biSizeImage = dwWidth * dwHeight;
		lpbiDest->bmiHeader.biXPelsPerMeter = 0;
		lpbiDest->bmiHeader.biYPelsPerMeter = 0;
		lpbiDest->bmiHeader.biClrUsed = 0;
		lpbiDest->bmiHeader.biClrImportant = 0;

		COLORREF* pDestBits = NULL;
		HBITMAP hDestDib = CreateDIBSection(
			hDC, lpbiDest, DIB_RGB_COLORS, (void **)&pDestBits,
			NULL, NULL);

		if ((NULL == hDestDib) || (NULL == pDestBits))
		{
			delete[] lpbiSrc;
			::DeleteObject(hSrcDib);
			::DeleteDC(hTempDC);
			return FALSE;
		}

		::SelectObject(hTempDC, hDestDib);
		::BitBlt(hTempDC, 0, 0, dwWidth, dwHeight, hDC, nDestX, nDestY, SRCCOPY);
		::SelectObject(hTempDC, hOldTempBmp);

		double src_darken;
		BYTE nAlpha;

		for (int pixel = 0; pixel < dwWidth * dwHeight; pixel++, pSrcBits++, pDestBits++)
		{
			nAlpha = LOBYTE(*pSrcBits >> 24);
			src_darken = (double)(nAlpha * ftn.SourceConstantAlpha) / 255.0 / 255.0;
			if (src_darken < 0.0)
			{
				src_darken = 0.0;
			}
			*pDestBits = PixelAlpha(*pSrcBits, src_darken, *pDestBits, 1.0 - src_darken);
		}

		::SelectObject(hTempDC, hDestDib);
		::BitBlt(hDC, nDestX, nDestY, dwWidth, dwHeight, hTempDC, 0, 0, SRCCOPY);
		::SelectObject(hTempDC, hOldTempBmp);

		delete[] lpbiDest;
		::DeleteObject(hDestDib);

		delete[] lpbiSrc;
		::DeleteObject(hSrcDib);

		::DeleteDC(hTempDC);
		return TRUE;
	}

	void CGifUI::DrawGifImage(HDC hDC, HBITMAP hBitmap, const RECT& rc, const RECT& rcPaint,
		const RECT& rcBmpPart, const RECT& rcCorners)
	{
		ASSERT(::GetObjectType(hDC) == OBJ_DC || ::GetObjectType(hDC) == OBJ_MEMDC);
		if (hBitmap == NULL) return;

		HDC hCloneDC = ::CreateCompatibleDC(hDC);
		HBITMAP hOldBitmap = (HBITMAP) ::SelectObject(hCloneDC, hBitmap);
		::SetStretchBltMode(hDC, HALFTONE);

		RECT rcTemp = { 0 };
		RECT rcDest = { 0 };
		BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
		rcDest.left = rc.left + rcCorners.left;
		rcDest.top = rc.top + rcCorners.top;
		rcDest.right = rc.right - rc.left - rcCorners.left - rcCorners.right;
		rcDest.bottom = rc.bottom - rc.top - rcCorners.top - rcCorners.bottom;
		rcDest.right += rcDest.left;
		rcDest.bottom += rcDest.top;
		if (::IntersectRect(&rcTemp, &rcPaint, &rcDest))
		{
			rcDest.right -= rcDest.left;
			rcDest.bottom -= rcDest.top;
			AlphaBitBlt(hDC, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
				rcBmpPart.left + rcCorners.left, rcBmpPart.top + rcCorners.top, \
				rcBmpPart.right - rcBmpPart.left - rcCorners.left - rcCorners.right, \
				rcBmpPart.bottom - rcBmpPart.top - rcCorners.top - rcCorners.bottom, bf);
		}
		::SelectObject(hCloneDC, hOldBitmap);
		::DeleteDC(hCloneDC);
	}

	void CGifUI::PaintStatusImage(HDC hDC)
	{
		if (m_pGif)
		{
			TImageInfo* pImageInfo = NULL;
			if (m_isUpdateTime)
			{
				m_isUpdateTime = false;
				pImageInfo = m_pGif->GetNextFrameInfo();
			}
			else
			{
				pImageInfo = m_pGif->GetCurrentFrameInfo();
			}
			if (pImageInfo)
			{
				RECT rcBmpPart = { 0 };
				RECT rcCorners = { 0 };
				rcBmpPart.right = pImageInfo->nX;
				rcBmpPart.bottom = pImageInfo->nY;
				//CRenderEngine::DrawImage(hDC,pImageInfo->hBitmap,m_rcItem, m_rcPaint,rcBmpPart,rcCorners,pImageInfo->alphaChannel,255);
				DrawGifImage(hDC, pImageInfo->hBitmap, m_rcItem, m_rcPaint, rcBmpPart, rcCorners);
				if (m_nPreUpdateDelay != pImageInfo->delay)
				{
					m_pManager->KillTimer(this, GIF_TIMER_ID);
					m_pManager->SetTimer(this, GIF_TIMER_ID, pImageInfo->delay);
					m_nPreUpdateDelay = pImageInfo->delay;
				}

			}
		}
		//没有gif图片,则与普通按钮一样
		else
		{
			if (IsFocused()) m_uButtonState |= UISTATE_FOCUSED;
			else m_uButtonState &= ~UISTATE_FOCUSED;
			if (!IsEnabled()) m_uButtonState |= UISTATE_DISABLED;
			else m_uButtonState &= ~UISTATE_DISABLED;

			if ((m_uButtonState & UISTATE_DISABLED) != 0) {
				if (!m_sDisabledImage.IsEmpty()) {
					if (!DrawImage(hDC, (LPCTSTR)m_sDisabledImage)) m_sDisabledImage.Empty();
					else return;
				}
			}
			else if ((m_uButtonState & UISTATE_PUSHED) != 0) {
				if (!m_sPushedImage.IsEmpty()) {
					if (!DrawImage(hDC, (LPCTSTR)m_sPushedImage)) m_sPushedImage.Empty();
					else return;
				}
			}
			else if ((m_uButtonState & UISTATE_HOT) != 0) {
				if (!m_sHotImage.IsEmpty()) {
					if (!DrawImage(hDC, (LPCTSTR)m_sHotImage)) m_sHotImage.Empty();
					else return;
				}
			}
			else if ((m_uButtonState & UISTATE_FOCUSED) != 0) {
				if (!m_sFocusedImage.IsEmpty()) {
					if (!DrawImage(hDC, (LPCTSTR)m_sFocusedImage)) m_sFocusedImage.Empty();
					else return;
				}
			}

			if (!m_sNormalImage.IsEmpty()) {
				if (!DrawImage(hDC, (LPCTSTR)m_sNormalImage)) m_sNormalImage.Empty();
				else return;
			}
		}


	}

	void CGifUI::DoEvent(TEventUI& event)
	{
		if (event.Type == UIEVENT_TIMER && event.wParam == GIF_TIMER_ID)
		{
			m_isUpdateTime = true;
			Invalidate();
			return;
		}

		CButtonUI::DoEvent(event);
	}

	void CGifUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if (_tcscmp(pstrName, _T("NormalGifFile")) == 0)
		{
			SetNormalGifFile(pstrValue);
		}

		CButtonUI::SetAttribute(pstrName, pstrValue);
	}

	void CGifUI::SetNormalGifFile(LPCTSTR pstrName)
	{
		if (pstrName == NULL) return;

		if (m_pGif)
		{
			m_pManager->KillTimer(this, GIF_TIMER_ID);
			m_nPreUpdateDelay = 0;
			delete m_pGif;
			m_pGif = NULL;
		}

		m_pGif = CRenderEngine::LoadGif(STRINGorID(pstrName), 0, 0);

		Invalidate();
	}

}
