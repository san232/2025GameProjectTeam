#pragma once
#include "Entity.h"

class FragmentBullet : public Entity
{
public:
    FragmentBullet();
    ~FragmentBullet() override;

    void Update() override;
    void Render(HDC _hdc) override;
    void EnterCollision(Collider* _other) override;
    void SetDirection(const Vec2& dir) { m_direction = dir; }

protected:
    void Attack() override {}
    void Dead() override;
    void Move() override;

private:
    Vec2 m_direction;
    float m_lifeTime;
    float m_maxLifeTime;
};