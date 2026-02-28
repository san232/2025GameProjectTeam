#pragma once
#include "Bullet.h"

class FragmentingBullet : public Bullet
{
public:
    FragmentingBullet();
    ~FragmentingBullet() override;

    void Render(HDC _hdc) override;

protected:
    void Move() override;

private:
    void Explode();
    float m_explodeTime;
};