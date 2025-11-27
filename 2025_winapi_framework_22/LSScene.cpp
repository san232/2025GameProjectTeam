#include "pch.h"
#include "LSScene.h"
#include "WindowManager.h"
#include "SubWindowRenderer.h"
#include "SubWindow.h"
#include "SubWindowController.h"
#include "Player.h"

void LSScene::Init()
{
    HWND mainWindowHwnd = GET_SINGLE(WindowManager)->GetMainWindowHandle();

    subWindowRenderer = new SubWindowRenderer(mainWindowHwnd, this);

    subWindow = new SubWindow();
    if (subWindow->Create(mainWindowHwnd, subWindowRenderer))
    {
        HWND subHwnd = subWindow->GetHWnd();
        GET_SINGLE(WindowManager)->RegisterSubWindow(subHwnd);

        RECT clientRect = {};
        ::GetClientRect(subHwnd, &clientRect);

        SIZE windowSize = {};
        windowSize.cx = clientRect.right - clientRect.left;
        windowSize.cy = clientRect.bottom - clientRect.top;

        subWindowController = new SubWindowController(subHwnd, windowSize);
    }

    Spawn<Player>(Layer::INVISIBLEENEMY, { WINDOW_WIDTH / 2, WINDOW_HEIGHT / 4 }, { 100.f,100.f });
    Spawn<Player>(Layer::DEFAULTENEMY, { WINDOW_WIDTH / 3, WINDOW_HEIGHT / 2 }, { 100.f,100.f });

}

void LSScene::Update()
{
    Scene::Update();

    if (subWindowController == nullptr || subWindow == nullptr)
        return;

    subWindowController->Update();
    ::InvalidateRect(subWindow->GetHWnd(), nullptr, FALSE);
}

LSScene::~LSScene()
{
    SAFE_DELETE(subWindowController);
    SAFE_DELETE(subWindow);
    SAFE_DELETE(subWindowRenderer);
}
