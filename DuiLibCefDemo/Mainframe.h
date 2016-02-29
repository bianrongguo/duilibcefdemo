/*
* Copyright (c) 2015,北京绩优堂教育科技有限公司C++部门
* All rights reserved.
*
* 文件名称：MainFrame.h
* 摘 要：客户端主界面类，头文件
*
* 作 者：liulong
* 完成日期：2015-05-09
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
	//创建Browser对象
	void CreateBrowser();
	//重画子窗口
	void ResizeClientWnd();
	DUI_DECLARE_MESSAGE_MAP();
private:
	CVerticalLayoutUI* m_layout_browser;
};

