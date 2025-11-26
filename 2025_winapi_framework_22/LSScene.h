#pragma once
#include "Scene.h"

class SubWindowRenderer;
class SubWindow;
class SubWindowController;

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
};
