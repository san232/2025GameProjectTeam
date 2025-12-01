#pragma once
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "SubWindow.h"

class Entity;

class SubWindowManager
{
public:
    SubWindowManager();
    ~SubWindowManager();

    void RegisterSubWindow(SubWindow* window);
    void UnregisterSubWindow(SubWindow* window);

    void Update(float deltaTime, const std::vector<Entity*>& allEntities);

    void RenderAll(); 

private:
    POINT WorldToScreen(const Vec2& worldPos);

private:
    std::vector<SubWindow*> m_subWindows;
    std::unordered_map<SubWindow*, std::unordered_set<Entity*>> m_windowEntityMap;
};
