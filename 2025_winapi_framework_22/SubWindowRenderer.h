#pragma once
#include "pch.h"

class Scene;
class SubWindow;

class SubWindowRenderer {
public:
    SubWindowRenderer(HWND mainWindow, Scene* scene);
    ~SubWindowRenderer();

    void Render(SubWindow* subWin);

private:
    HWND mainWindow;
    Scene* scene;
};