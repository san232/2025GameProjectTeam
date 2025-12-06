#pragma once

class SubWindow;
class Entity;

class SubWindowManager
{
public:
    SubWindowManager();
    ~SubWindowManager();

    void RegisterSubWindow(SubWindow* window);
    void UnregisterSubWindow(SubWindow* window);

    void ResetWindow(SubWindow* window);

    void Update(float deltaTime, const std::vector<Entity*>& allEntities);

    void RenderAll(); 

private:
    POINT WorldToScreen(const Vec2& worldPos);

private:
    std::vector<SubWindow*> m_subWindows;
    std::unordered_map<SubWindow*, std::unordered_set<Entity*>> m_windowEntityMap;
};
