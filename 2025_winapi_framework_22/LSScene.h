#pragma once
#include "Scene.h"

class SubWindowManager;

class LSScene : public Scene
{
public:
    virtual void Init() override;
    virtual void Update() override;
    virtual void Render(HDC _hdc) override;
    virtual void Release() override;
    virtual ~LSScene();

private:
    SubWindowManager* subWindowManager = nullptr;
};
