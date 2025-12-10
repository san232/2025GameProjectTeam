#pragma once
#include "Scene.h"

class DeadScene :
    public Scene
{
public:
    DeadScene();
    virtual ~DeadScene();

public:
    virtual void Init() override;
    virtual void Update() override;
    virtual void Render(HDC _hdc) override;
    virtual void Release() override;

private:
    RECT m_btnRetry;
    RECT m_btnExit;
};
