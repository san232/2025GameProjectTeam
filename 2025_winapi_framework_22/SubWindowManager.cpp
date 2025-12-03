#include "pch.h"
#include "SubWindowManager.h"
#include "Entity.h"
#include "Core.h"

SubWindowManager::SubWindowManager()
{
}

SubWindowManager::~SubWindowManager()
{
}

void SubWindowManager::RegisterSubWindow(SubWindow* window)
{
    m_subWindows.push_back(window);
}

void SubWindowManager::UnregisterSubWindow(SubWindow* window)
{
    auto it = std::find(m_subWindows.begin(), m_subWindows.end(), window);
    if (it != m_subWindows.end())
    {
        m_subWindows.erase(it);
        m_windowEntityMap.erase(window);
    }
}

void SubWindowManager::Update(float deltaTime, const std::vector<Entity*>& allEntities)
{
    for (SubWindow* window : m_subWindows)
    {
        if (!window->IsActive()) continue;

        RECT winRect = window->GetRect();
        ISubWindowEffect* effect = window->GetEffect();

        
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

        std::unordered_set<Entity*>& prevFrameEntities = m_windowEntityMap[window];

        for (Entity* entity : currentFrameEntities)
        {
            if (prevFrameEntities.find(entity) == prevFrameEntities.end())
            {
                if (effect) effect->OnEnter(entity);
            }
            else
            {
                if (effect) effect->OnStay(entity, deltaTime);
            }
        }

        for (Entity* entity : prevFrameEntities)
        {
            if (currentFrameEntities.find(entity) == currentFrameEntities.end())
            {
                if (effect) effect->OnExit(entity);
            }
        }
        
        m_windowEntityMap[window] = currentFrameEntities;
    }
}

void SubWindowManager::RenderAll()
{
    
    HWND hPrev = HWND_BOTTOM;
    for (SubWindow* win : m_subWindows)
    {
        if (win->IsActive() && win->GetHWnd())
        {
            
            ::SetWindowPos(win->GetHWnd(), hPrev, 0, 0, 0, 0, 
                SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
            hPrev = win->GetHWnd();
        }
    }
    
    for (SubWindow* win : m_subWindows)
    {
        if (win->IsActive() && win->GetHWnd())
        {
            ::InvalidateRect(win->GetHWnd(), nullptr, FALSE);
            ::UpdateWindow(win->GetHWnd());
        }
    }
}

POINT SubWindowManager::WorldToScreen(const Vec2& worldPos)
{
    
    HWND hMain = GET_SINGLE(Core)->GetHwnd();
    POINT pt = { (LONG)worldPos.x, (LONG)worldPos.y };
    ::ClientToScreen(hMain, &pt);
    return pt;
}
