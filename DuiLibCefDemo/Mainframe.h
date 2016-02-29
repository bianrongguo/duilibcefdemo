/*
* Copyright (c) 2015,���������ý����Ƽ����޹�˾C++����
* All rights reserved.
*
* �ļ����ƣ�MainFrame.h
* ժ Ҫ���ͻ����������࣬ͷ�ļ�
*
* �� �ߣ�liulong
* ������ڣ�2015-05-09
*
*/
#pragma once
#include "..\include\DuiLib\UIlib.h"
#include "..\include\DuiLib\Utils\WinImplBase.h"
#include "..\include\DuiLib\Core\UIDefine.h"

using namespace DuiLib;
class CMainFrame : public WindowImplBase
{
public:
	CMainFrame();
	~CMainFrame();
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName(void) const;
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual void InitWindow();
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual void OnClick(DuiLib::TNotifyUI& msg);
	virtual void Notify(TNotifyUI& msg);
	//����Browser����
	void CreateBrowser();
	//�ػ��Ӵ���
	void ResizeClientWnd();
	DUI_DECLARE_MESSAGE_MAP();
private:
	CVerticalLayoutUI* m_layout_browser;
};

