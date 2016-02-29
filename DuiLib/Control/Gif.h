#ifndef GIF_H
#define GIF_H
namespace DuiLib
{
	class CGifUI : public CButtonUI
	{
	public:
		CGifUI();
		~CGifUI();

		LPCTSTR GetClass() const
		{
			return _T("GifUI");
		}


		LPVOID GetInterface(LPCTSTR pstrName)
		{
			if (_tcscmp(pstrName, _T("GifUI")) == 0)
				return static_cast<CGifUI*>(this);

			return CControlUI::GetInterface(pstrName);
		}

		void SetNormalGifFile(LPCTSTR pstrName);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		virtual void DoEvent(TEventUI& event);
		virtual void SetVisible(bool bVisible = true);//设置控件可视性
		virtual void PaintStatusImage(HDC hDC);

	protected:
		enum
		{
			GIF_TIMER_ID = 15
		};

		bool m_isUpdateTime;
		CGifHandler* m_pGif;
		int m_nPreUpdateDelay;
	private:
		void DrawGifImage(HDC hDC, HBITMAP hBitmap, const RECT& rc, const RECT& rcPaint,
			const RECT& rcBmpPart, const RECT& rcCorners);
	};
}

#endif
