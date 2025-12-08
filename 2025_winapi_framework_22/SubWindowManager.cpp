#include "pch.h"
#include "SubWindowManager.h"
#include "Entity.h"
#include "SubWindow.h"
#include "SubWindowRenderer.h"
#include "SubWindowController.h"
#include "BuffEffects.h"
#include "Core.h"
#include "InputManager.h"
#include "WindowManager.h"

SubWindowManager::SubWindowManager()
    : m_subWindow(nullptr)
    , m_renderer(nullptr)
    , m_controller(nullptr)
    , m_currentBuffIndex(0)
{
}

SubWindowManager::~SubWindowManager()
{
    if (m_subWindow)
    {
        HWND hWnd = m_subWindow->GetHWnd();
        if (hWnd)
        {
            
            
        }
    }

    SAFE_DELETE(m_controller);
    SAFE_DELETE(m_subWindow);
    SAFE_DELETE(m_renderer);

    for (auto* effect : m_buffEffects)
        SAFE_DELETE(effect);
    m_buffEffects.clear();
}

void SubWindowManager::Init(HWND hMainWnd, Scene* ownerScene)
{
    m_renderer = new SubWindowRenderer(hMainWnd, ownerScene);

    ISubWindowEffect* attackBuff = new AttackBuffEffect();
    ISubWindowEffect* speedBuff = new MoveSpeedBuffEffect();
    ISubWindowEffect* timeSlowBuff = new TimeSlowEffect();
    m_buffEffects.push_back(attackBuff);
    m_buffEffects.push_back(speedBuff);
    m_buffEffects.push_back(timeSlowBuff);
    m_currentBuffIndex = 0;

    m_subWindow = new SubWindow();
    if (m_subWindow->Create(hMainWnd, m_renderer, 200, 200))
    {
        if (!m_buffEffects.empty())
        {
            m_subWindow->SetEffect(m_buffEffects[m_currentBuffIndex]);
        }

        ::SetWindowPos(m_subWindow->GetHWnd(), nullptr, 100, 100, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

        HWND subHwnd = m_subWindow->GetHWnd();
        GET_SINGLE(WindowManager)->RegisterSubWindow(subHwnd);

        RECT clientRect = {};
        ::GetClientRect(subHwnd, &clientRect);
        SIZE windowSize = { clientRect.right - clientRect.left, clientRect.bottom - clientRect.top };

        m_controller = new SubWindowController(subHwnd, windowSize);
    }
}

void SubWindowManager::Update(float deltaTime, const std::vector<Entity*>& allEntities)
{
    if (GET_KEYDOWN(KEY_TYPE::TAB))
    {
        if (!m_buffEffects.empty() && m_subWindow)
        {
            ResetWindow();

            m_currentBuffIndex = (m_currentBuffIndex + 1) % m_buffEffects.size();
            m_subWindow->SetEffect(m_buffEffects[m_currentBuffIndex]);
        }
    }

    if (GET_SINGLE(InputManager)->IsDown(KEY_TYPE::LBUTTON))
    {
        POINT mousePos = GET_MOUSE_SCREEN_POS;

        if (m_controller && m_controller->IsMoving())
        {
            m_controller->ToggleMovement();
        }
        else if (m_subWindow && m_subWindow->IsActive())
        {
            RECT subRect = m_subWindow->GetRect();
            if (::PtInRect(&subRect, mousePos))
            {
                if (m_controller)
                    m_controller->ToggleMovement();
            }
        }
    }

    if (m_controller)
        m_controller->Update();
    
    if (m_subWindow && m_subWindow->IsActive())
    {
        m_subWindow->Update();

        RECT winRect = m_subWindow->GetRect();
        ISubWindowEffect* effect = m_subWindow->GetEffect();

        std::unordered_set<Entity*> currentFrameEntities;

        for (Entity* entity : allEntities)
        {
            if (entity->GetIsDead()) continue;

            POINT screenPos = WorldToScreen(entity->GetPos());

            if (screenPos.x >= winRect.left && screenPos.x <= winRect.right &&
                screenPos.y >= winRect.top && screenPos.y <= winRect.bottom)
            {
                currentFrameEntities.insert(entity);
            }
        }

        for (Entity* entity : currentFrameEntities)
        {
            if (m_prevFrameEntities.find(entity) == m_prevFrameEntities.end())
            {
                if (effect) effect->OnEnter(entity);
            }
            else
            {
                if (effect) effect->OnStay(entity, deltaTime);
            }
        }

        for (Entity* entity : m_prevFrameEntities)
        {
            if (currentFrameEntities.find(entity) == currentFrameEntities.end())
            {
                if (effect) effect->OnExit(entity);
            }
        }

        m_prevFrameEntities = currentFrameEntities;
    }
}

void SubWindowManager::Render()
{
    if (m_subWindow && m_subWindow->IsActive() && m_subWindow->GetHWnd())
    {
        ::InvalidateRect(m_subWindow->GetHWnd(), nullptr, FALSE);
        ::UpdateWindow(m_subWindow->GetHWnd());
    }
}

void SubWindowManager::ResetWindow()
{
    if (m_subWindow)
    {
        ISubWindowEffect* effect = m_subWindow->GetEffect();
        if (effect)
        {
            for (Entity* entity : m_prevFrameEntities)
            {
                effect->OnExit(entity);
            }
        }
        m_prevFrameEntities.clear();
    }
}

POINT SubWindowManager::WorldToScreen(const Vec2& worldPos)
{
    HWND hMain = GET_SINGLE(Core)->GetHwnd();
    POINT pt = { (LONG)worldPos.x, (LONG)worldPos.y };
    ::ClientToScreen(hMain, &pt);
    return pt;
}
