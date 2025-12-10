#pragma once
#include "Entity.h"

class Collider;
class Player;
class Texture;

class WormBullet : public Entity
{
public:
    WormBullet();
    ~WormBullet() override;

public:
    void Update() override;
    void Render(HDC _hdc) override;

    void EnterCollision(Collider* _other) override;
    void StayCollision(Collider* _other) override;
    void ExitCollision(Collider* _other) override;

public:
    void SetDirection(const Vec2& dir) { m_direction = dir; }
    void SetTarget(Player* target) { m_target = target; }

protected:
    void Attack() override {}
    void Dead() override;
    void Move() override;

private:
    Texture* m_pTex;
    Vec2 m_direction;
    float m_lifeTime;
    float m_maxLifeTime;
    float m_homingStrength;

    Player* m_target;
};