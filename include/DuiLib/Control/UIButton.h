#ifndef __UIBUTTON_H__
#define __UIBUTTON_H__

#pragma once

namespace DuiLib
{
	class UILIB_API CButtonUI : public CLabelUI
	{
	public:
		CButtonUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		UINT GetControlFlags() const;

		bool Activate();
		void SetEnabled(bool bEnable = true);
		void DoEvent(TEventUI& event);

		LPCTSTR GetNormalImage();
		void SetNormalImage(LPCTSTR pStrImage);
		LPCTSTR GetHotImage();
		void SetHotImage(LPCTSTR pStrImage);
		LPCTSTR GetPushedImage();
		void SetPushedImage(LPCTSTR pStrImage);
		LPCTSTR GetFocusedImage();
		void SetFocusedImage(LPCTSTR pStrImage);
		LPCTSTR GetDisabledImage();
		void SetDisabledImage(LPCTSTR pStrImage);
		LPCTSTR GetForeImage();
		void SetForeImage(LPCTSTR pStrImage);
		LPCTSTR GetHotForeImage();
		void SetHotForeImage(LPCTSTR pStrImage);

		void SetHotBkColor(DWORD dwColor);
		DWORD GetHotBkColor() const;
		void SetHotTextColor(DWORD dwColor);
		DWORD GetHotTextColor() const;
		void SetPushedTextColor(DWORD dwColor);
		DWORD GetPushedTextColor() const;
		void SetFocusedTextColor(DWORD dwColor);
		DWORD GetFocusedTextColor() const;
		SIZE EstimateSize(SIZE szAvailable);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		void PaintText(HDC hDC);
		void PaintStatusImage(HDC hDC);

		// zhangcl 2015-02-28 Button�ؼ������hot״̬�¸ı�����
		void SetHotFont(int index);
		int GetHotFont();
		//====================================================

		// zhangcl 2015-02-28 Button�ؼ����������pushed״̬�ı�����
		void SetPushedFont(int index);
		int GetPushedFont();
		//========================================================
		// �������cursor��״
		void setHandCursor(bool isSet);
		// zhangcl 2015-07-27 ��Ӧ�س��¼�
		bool responseEnterKey();
	protected:
		UINT m_uButtonState;

		DWORD m_dwHotBkColor;
		DWORD m_dwHotTextColor;
		DWORD m_dwPushedTextColor;
		DWORD m_dwFocusedTextColor;

		CDuiString m_sNormalImage;
		CDuiString m_sHotImage;
		CDuiString m_sHotForeImage;
		CDuiString m_sPushedImage;
		CDuiString m_sPushedForeImage;
		CDuiString m_sFocusedImage;
		CDuiString m_sDisabledImage;

		// zhangcl 2015-02-28 Button�ؼ������hot״̬�¸ı�����
		int m_iHotFont;
		// zhangcl 2015-02-28 Button�ؼ����������pushed״̬�ı�����
		int m_iPushedFont;
		// zhangcl 2015-06-15 ���ù���Ƿ�ΪС��
		bool m_isHandCursor;
	};

}	// namespace DuiLib

#endif // __UIBUTTON_H__