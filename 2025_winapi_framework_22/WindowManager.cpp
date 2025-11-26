#include "pch.h"
#include "WindowManager.h"

void WindowManager::Init(HWND _mainHWnd)
{
	m_hMainWnd = _mainHWnd;
}

void WindowManager::RegisterSubWindow(HWND _subHWnd)
{
	m_hSubWnd = _subHWnd;
}
