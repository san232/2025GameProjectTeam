#include "pch.h"
#include "LSScene.h"
#include "WindowManager.h"
#include "SubWindowRenderer.h"
#include "SubWindow.h"
#include "SubWindowController.h"
#include "Player.h"
#include "InputManager.h"
#include "SubWindowManager.h"
#include "BuffEffects.h"
#include "Entity.h"

void LSScene::Init()
{
    HWND mainWindowHwnd = GET_SINGLE(WindowManager)->GetMainWindowHandle();

    subWindowRenderer = new SubWindowRenderer(mainWindowHwnd, this);
    subWindowManager = new SubWindowManager();
    buffEffect = new AttackBuffEffect();

    subWindow = new SubWindow();
    if (subWindow->Create(mainWindowHwnd, subWindowRenderer))
    {
        HWND subHwnd = subWindow->GetHWnd();
        GET_SINGLE(WindowManager)->RegisterSubWindow(subHwnd);

        subWindow->SetEffect(buffEffect);
        subWindowManager->RegisterSubWindow(subWindow);

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

    if (GET_SINGLE(InputManager)->IsDown(KEY_TYPE::LBUTTON))
    {
        POINT mousePos;
        ::GetCursorPos(&mousePos);
        RECT subRect = subWindow->GetRect();

        if (subWindowController->IsMoving())
        {
            subWindowController->ToggleMovement();
        }
        else
        {
            if (::PtInRect(&subRect, mousePos))
            {
                subWindowController->ToggleMovement();
            }
        }
    }

    subWindowController->Update();
    
    // Update SubWindowManager
    if (subWindowManager)
    {
        vector<Entity*> entities;
        for (int i = 0; i < (int)Layer::END; ++i)
        {
            const vector<Object*>& objs = GetLayerObjects((Layer)i);
            for (Object* obj : objs)
            {
                Entity* ent = dynamic_cast<Entity*>(obj);
                if (ent)
                    entities.push_back(ent);
            }
        }
        subWindowManager->Update(fDT, entities);
    }

    ::InvalidateRect(subWindow->GetHWnd(), nullptr, FALSE);
}

LSScene::~LSScene()
{
    SAFE_DELETE(subWindowController);
    SAFE_DELETE(subWindow);
    SAFE_DELETE(subWindowRenderer);
    SAFE_DELETE(subWindowManager);
    SAFE_DELETE(buffEffect);
}
