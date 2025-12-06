#pragma once
#include "Scene.h"

class SubWindowRenderer;
class SubWindow;
class SubWindowController;
class SubWindowManager;
class ISubWindowEffect;

class LSScene : public Scene
{
public:
    virtual void Init() override;
    virtual void Update() override;
    virtual ~LSScene();

private:
    SubWindowRenderer* subWindowRenderer = nullptr;
    SubWindowManager* subWindowManager = nullptr;

    SubWindow* m_subWindow = nullptr;
    SubWindowController* m_subWindowController = nullptr;
    
    vector<ISubWindowEffect*> m_buffEffects;
    int m_currentBuffIndex = 0;
};
