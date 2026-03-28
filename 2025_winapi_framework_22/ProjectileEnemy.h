#pragma once
#include "BaseEnemy.h"

class Scene;
class Player;
class Collider;

class ProjectileEnemy : public BaseEnemy
{
public:
    struct RangedStat
    {
        float chargeDelay = 0.3f;
        float attackStateDuration = 0.48f;
    };

protected:
    ProjectileEnemy(const RangedStat& stat);

public:
    void Update() override;
    void Render(HDC _hdc) override;

    void EnterCollision(Collider* _other) override;
    void StayCollision(Collider* _other) override;
    void ExitCollision(Collider* _other) override;

protected:
    void Attack() override;
    void Dead() override;

    bool Aim(Player*& outTarget, Vec2& outOrigin, Vec2& outDir) const;
    virtual void SpawnProjectiles(Scene* scene, Player* target, const Vec2& origin, const Vec2& dir) = 0;

private:
    float m_attackShotDelay = 0.3f;
    float m_attackShotTimer = 0.f;
    bool m_isShotCharging = false;
};
