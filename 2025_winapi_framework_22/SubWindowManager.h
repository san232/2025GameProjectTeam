#pragma once

class SubWindow;
class Entity;
class SubWindowRenderer;
class SubWindowController;
class ISubWindowEffect;
class Scene;

class SubWindowManager
{
public:
    SubWindowManager();
    ~SubWindowManager();

    void Init(HWND hMainWnd, Scene* ownerScene);
    void Update(float deltaTime, const std::vector<Entity*>& allEntities);
    void Render();

private:
    void ResetWindow();
    POINT WorldToScreen(const Vec2& worldPos);

private:
    SubWindow* m_subWindow;
    SubWindowRenderer* m_renderer;
    SubWindowController* m_controller;
    
    std::vector<ISubWindowEffect*> m_buffEffects;
    int m_currentBuffIndex;

    std::unordered_set<Entity*> m_prevFrameEntities;
};
