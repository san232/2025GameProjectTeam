#pragma once
class WindowManager
{
	DECLARE_SINGLE(WindowManager);
public:
	void Init(HWND _mainHWnd);
public:
	void RegisterSubWindow(HWND _subHWnd);
public:
	HWND GetMainWindowHandle() const { return m_hMainWnd; }
	HWND GetSubWindowHandle() const { return m_hSubWnd; }
private:
	HWND m_hMainWnd = nullptr;
	HWND m_hSubWnd = nullptr;
};

