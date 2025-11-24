#pragma once
class SubWindowRenderer;
class SubWindowWindow {
public:
    bool Create(HWND parent, SubWindowRenderer* renderer);
    HWND GetHWnd() const;

private:
    HWND hWnd;
    SubWindowRenderer* renderer;

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
    static SubWindowWindow* GetThis(HWND hWnd);
};