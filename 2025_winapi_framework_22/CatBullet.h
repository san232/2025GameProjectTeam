#pragma once
#include "Entity.h"

class Collider;
class Player;
class Texture;

class CatBullet : public Entity
{
public:
    CatBullet();
    ~CatBullet() override;

public:
    void Update() override;
    void Render(HDC _hdc) override;

    void EnterCollision(Collider* _other) override;
    void StayCollision(Collider* _other) override;
    void ExitCollision(Collider* _other) override;

public:
    void SetDirection(const Vec2& dir) { m_direction = dir; }
    void SetTarget(Player* target) { m_target = target; }
    void SetDamage(int damage) { m_damage = damage; }

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
    int m_damage;
};
