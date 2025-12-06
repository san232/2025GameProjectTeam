#pragma once
#include <windows.h>

class Scene;
class SubWindow;

class SubWindowRenderer {
public:
    SubWindowRenderer(HWND mainWindow, Scene* scene);
    ~SubWindowRenderer();

    void Render(HDC subDC, SubWindow* subWin, HDC mainBackDC);
    void RenderLegacy(HDC hdc);


private:
    HWND mainWindow;
    Scene* scene;

    HDC m_memDC;
    HBITMAP m_hColorBitmap;
    COLORREF m_lastColor;
};