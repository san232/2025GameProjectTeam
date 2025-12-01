#pragma once
#include <windows.h>
#include "InvisibleEnemyOverlayRenderer.h"

class Scene;
class SubWindow;

class SubWindowRenderer {
public:
    SubWindowRenderer(HWND mainWindow, Scene* scene);
    ~SubWindowRenderer();

    void Render(HDC hdc, SubWindow* window, HDC mainBackDC, RECT mainWndRect);

    void RenderLegacy(HDC hdc);

private:
    HWND mainWindow;
    Scene* scene;
    InvisibleEnemyOverlayRenderer* m_legacyRenderer;
};