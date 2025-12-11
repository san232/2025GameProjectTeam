#pragma once
#include "Entity.h"

class Texture;
class FragmentingBullet : public Entity
{
public:
    FragmentingBullet();
    ~FragmentingBullet() override;

    void Update() override;
    void Render(HDC _hdc) override;
    void EnterCollision(Collider* _other) override;
    void SetDirection(const Vec2& dir) { m_direction = dir; }

protected:
    void Attack() override {}
    void Dead() override;
    void Move() override;

private:
    void Explode();

	Texture* m_pTex;
    Vec2 m_direction;
    float m_lifeTime;
    float m_explodeTime;
};