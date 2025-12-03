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

    vector<SubWindow*> m_subWindows;
    vector<SubWindowController*> m_subWindowControllers;
    vector<ISubWindowEffect*> m_buffEffects;
};
