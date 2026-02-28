#pragma once
#include "Bullet.h"

class BoomerangBullet : public Bullet
{
public:
    BoomerangBullet();
    ~BoomerangBullet() override;

public:
    void Render(HDC _hdc) override;

protected:
    void Move() override;

private:
    float m_turnTime;
    bool m_isReturning;
};