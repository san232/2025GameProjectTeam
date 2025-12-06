#include "pch.h"
#include "LSScene.h"
#include "WindowManager.h"
#include "SubWindowManager.h"
#include "CollisionManager.h"
#include "SubWindowRenderer.h"
#include "SubWindow.h"
#include "InputManager.h"
#include "BuffEffects.h"
#include "SubWindowController.h"
#include "Player.h"
#include "Zombie.h"

void LSScene::Init()
{
    HWND mainWindowHwnd = GET_SINGLE(WindowManager)->GetMainWindowHandle();

    subWindowRenderer = new SubWindowRenderer(mainWindowHwnd, this);
    subWindowManager = new SubWindowManager();
    
    // Create Effects
    ISubWindowEffect* attackBuff = new AttackBuffEffect();
    ISubWindowEffect* speedBuff = new MoveSpeedBuffEffect();
    m_buffEffects.push_back(attackBuff);
    m_buffEffects.push_back(speedBuff);

    // --- Window 1: Attack Buff ---
    SubWindow* win1 = new SubWindow();
    if (win1->Create(mainWindowHwnd, subWindowRenderer, 200, 200))
    {
        win1->SetEffect(attackBuff);
        win1->SetTintColor(RGB(255, 100, 100), 0.3f); // Red Tint
        ::SetWindowPos(win1->GetHWnd(), nullptr, 100, 100, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

        HWND subHwnd = win1->GetHWnd();
        GET_SINGLE(WindowManager)->RegisterSubWindow(subHwnd);
        subWindowManager->RegisterSubWindow(win1);

        RECT clientRect = {};
        ::GetClientRect(subHwnd, &clientRect);
        SIZE windowSize = { clientRect.right - clientRect.left, clientRect.bottom - clientRect.top };

        m_subWindows.push_back(win1);
        m_subWindowControllers.push_back(new SubWindowController(subHwnd, windowSize));
    }

    // --- Window 2: Speed Buff ---
    SubWindow* win2 = new SubWindow();
    if (win2->Create(mainWindowHwnd, subWindowRenderer, 200, 200))
    {
        win2->SetEffect(speedBuff);
        win2->SetTintColor(RGB(100, 100, 255), 0.3f); // Blue Tint
        ::SetWindowPos(win2->GetHWnd(), nullptr, 400, 100, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

        HWND subHwnd = win2->GetHWnd();
        GET_SINGLE(WindowManager)->RegisterSubWindow(subHwnd);
        subWindowManager->RegisterSubWindow(win2);

        RECT clientRect = {};
        ::GetClientRect(subHwnd, &clientRect);
        SIZE windowSize = { clientRect.right - clientRect.left, clientRect.bottom - clientRect.top };

        m_subWindows.push_back(win2);
        m_subWindowControllers.push_back(new SubWindowController(subHwnd, windowSize));
    }

    Spawn<Player>(Layer::PLAYER, { WINDOW_WIDTH / 2, WINDOW_HEIGHT / 4 }, { 80.f, 80.f });
    GET_SINGLE(CollisionManager)->CheckLayer(Layer::DEFAULTENEMY, Layer::DEFAULTENEMY);
    GET_SINGLE(CollisionManager)->CheckLayer(Layer::DEFAULTENEMY, Layer::PLAYERBULLET);
}

void LSScene::Update()
{
    Scene::Update();

    if (GET_SINGLE(InputManager)->IsDown(KEY_TYPE::LBUTTON))
    {
        POINT mousePos;
        ::GetCursorPos(&mousePos);

        bool handled = false;

        // 1. Check if any window is currently moving -> Stop it
        for (auto* ctrl : m_subWindowControllers)
        {
            if (ctrl->IsMoving())
            {
                ctrl->ToggleMovement();
                handled = true;
                break; 
            }
        }

        // 2. If none were moving, check if we clicked on one to start moving
        if (!handled)
        {
            // Check in reverse order to pick the "top-most" if they overlap (assuming creation order implies z-order roughly)
            for (int i = (int)m_subWindows.size() - 1; i >= 0; --i)
            {
                if (!m_subWindows[i]->IsActive()) continue;

                RECT subRect = m_subWindows[i]->GetRect();
                if (::PtInRect(&subRect, mousePos))
                {
                    m_subWindowControllers[i]->ToggleMovement();
                    break;
                }
            }
        }
    }

    for (auto* ctrl : m_subWindowControllers)
    {
        ctrl->Update();
    }
    
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

    for (auto* win : m_subWindows)
    {
        if (win->GetHWnd())
            ::InvalidateRect(win->GetHWnd(), nullptr, FALSE);
    }
}

LSScene::~LSScene()
{
    for (auto* ctrl : m_subWindowControllers)
        SAFE_DELETE(ctrl);
    m_subWindowControllers.clear();

    for (auto* win : m_subWindows)
        SAFE_DELETE(win);
    m_subWindows.clear();

    for (auto* effect : m_buffEffects)
        SAFE_DELETE(effect);
    m_buffEffects.clear();

    SAFE_DELETE(subWindowRenderer);
    SAFE_DELETE(subWindowManager);
}
