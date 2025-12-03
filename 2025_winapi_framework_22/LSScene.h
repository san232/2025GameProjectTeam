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
    SubWindow* subWindow = nullptr;
    SubWindowController* subWindowController = nullptr;
    SubWindowManager* subWindowManager = nullptr;
    ISubWindowEffect* buffEffect = nullptr;
};
