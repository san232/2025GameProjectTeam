#pragma once
#include "Entity.h"

class Player;
class Texture;
class Collider;

struct EnemyBulletStat
{
    const wchar_t* textureKey = L"";
    Vec2 colliderSize = { 10.f, 10.f };
    Vec2 defaultSize = { 20.f, 20.f };
    float moveSpeed = 200.f;
    int attackPower = 1;
    float maxLifeTime = 5.f;
    float homingStrength = 0.f;
};

class EnemyBullet : public Entity
{
public:
    explicit EnemyBullet(const EnemyBulletStat& spec);
    ~EnemyBullet() override = default;

public:
    void Update() override;
    void Render(HDC _hdc) override;

    void EnterCollision(Collider* _other) override;
    void StayCollision(Collider* _other) override {}
    void ExitCollision(Collider* _other) override {}

public:
    void SetDirection(const Vec2& dir) { m_direction = dir; }
    void SetTarget(Player* target) { m_target = target; }

protected:
    void Attack() override {}
    void Move() override;
    void Dead() override;

    virtual void OnDead() {}

protected:
    Texture* m_pTex = nullptr;
    Vec2 m_direction = { 1.f, 0.f };
    Player* m_target = nullptr;
    float m_homingStrength = 0.f;

private:
    float m_lifeTime = 0.f;
    float m_maxLifeTime = 5.f;
};