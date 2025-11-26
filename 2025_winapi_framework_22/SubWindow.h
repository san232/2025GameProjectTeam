// SubWindow.h
#pragma once
#include <windows.h>

class SubWindowRenderer;

class SubWindow
{
public:
    SubWindow();
    ~SubWindow();

    bool Create(HWND parent, SubWindowRenderer* renderer);
    HWND GetHWnd() const { return m_hWnd; }

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

private:
    static ATOM RegisterClassOnce(HINSTANCE hInst);
    static SubWindow* GetThis(HWND hWnd);

private:
    HWND m_hWnd;
    SubWindowRenderer* m_renderer;
};
