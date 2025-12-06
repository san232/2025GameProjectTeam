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
    
    ISubWindowEffect* attackBuff = new AttackBuffEffect();
    ISubWindowEffect* speedBuff = new MoveSpeedBuffEffect();
    m_buffEffects.push_back(attackBuff);
    m_buffEffects.push_back(speedBuff);
    m_currentBuffIndex = 0;

    m_subWindow = new SubWindow();
    if (m_subWindow->Create(mainWindowHwnd, subWindowRenderer, 200, 200))
    {
        if (!m_buffEffects.empty())
        {
            m_subWindow->SetEffect(m_buffEffects[m_currentBuffIndex]);
        }
        
        ::SetWindowPos(m_subWindow->GetHWnd(), nullptr, 100, 100, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

        HWND subHwnd = m_subWindow->GetHWnd();
        GET_SINGLE(WindowManager)->RegisterSubWindow(subHwnd);
        subWindowManager->RegisterSubWindow(m_subWindow);

        RECT clientRect = {};
        ::GetClientRect(subHwnd, &clientRect);
        SIZE windowSize = { clientRect.right - clientRect.left, clientRect.bottom - clientRect.top };

        m_subWindowController = new SubWindowController(subHwnd, windowSize);
    }

    Spawn<Player>(Layer::PLAYER, { WINDOW_WIDTH / 2, WINDOW_HEIGHT / 4 }, { 50.f, 50.f });
    GET_SINGLE(CollisionManager)->CheckLayer(Layer::DEFAULTENEMY, Layer::DEFAULTENEMY);
    GET_SINGLE(CollisionManager)->CheckLayer(Layer::DEFAULTENEMY, Layer::PLAYERBULLET);
}

void LSScene::Update()
{
    Scene::Update();

    if (GET_KEYDOWN(KEY_TYPE::TAB))
    {
        if (!m_buffEffects.empty() && m_subWindow)
        {
            if (subWindowManager)
                subWindowManager->ResetWindow(m_subWindow);

            m_currentBuffIndex = (m_currentBuffIndex + 1) % m_buffEffects.size();
            m_subWindow->SetEffect(m_buffEffects[m_currentBuffIndex]);
        }
    }

    if (GET_SINGLE(InputManager)->IsDown(KEY_TYPE::LBUTTON))
    {
        POINT mousePos = GET_MOUSE_SCREEN_POS;

        if (m_subWindowController && m_subWindowController->IsMoving())
        {
            m_subWindowController->ToggleMovement();
        }
        else if (m_subWindow && m_subWindow->IsActive())
        {
            RECT subRect = m_subWindow->GetRect();
            if (::PtInRect(&subRect, mousePos))
            {
                if (m_subWindowController)
                    m_subWindowController->ToggleMovement();
            }
        }
    }

    if (m_subWindowController)
        m_subWindowController->Update();
    
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

    if (m_subWindow && m_subWindow->GetHWnd())
        ::InvalidateRect(m_subWindow->GetHWnd(), nullptr, FALSE);
}

LSScene::~LSScene()
{
    SAFE_DELETE(m_subWindowController);
    SAFE_DELETE(m_subWindow);

    for (auto* effect : m_buffEffects)
        SAFE_DELETE(effect);
    m_buffEffects.clear();

    SAFE_DELETE(subWindowRenderer);
    SAFE_DELETE(subWindowManager);
}
