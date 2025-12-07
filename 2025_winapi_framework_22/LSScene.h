#pragma once
#include "Scene.h"

class SubWindowManager;

class LSScene : public Scene
{
public:
    virtual void Init() override;
    virtual void Update() override;
    virtual ~LSScene();

private:
    SubWindowManager* subWindowManager = nullptr;
};
