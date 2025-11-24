#pragma once
class Scene;
class SubWindowRenderer {
public:
    SubWindowRenderer(HWND mainWindow, Scene* scene);
    void Render(HDC hdc, const POINT& globalPos);

private:
    HWND mainWindow;
    Scene* scene;
};

