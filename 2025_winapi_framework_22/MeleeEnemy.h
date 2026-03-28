#pragma once
#include "BaseEnemy.h"

class Collider;
class Player;

class MeleeEnemy : public BaseEnemy
{
public:
    struct DashStat
    {
        float dashDuration = 0.25f;
        float dashImpulse = 700.f;
    };

protected:
    MeleeEnemy(const DashStat& stat);
    virtual ~MeleeEnemy() = default;

public:
    void Update() override;
    void Render(HDC _hdc) override;

    void EnterCollision(Collider* _other) override;
    void StayCollision(Collider* _other) override;
    void ExitCollision(Collider* _other) override;

protected:
    void Attack() override;
    void Dead() override;

private:
    float m_dashDuration = 0.25f;
    float m_dashTimer = 0.f;
    bool  m_isDashing = false;
    bool  m_hasHitPlayerThisDash = false;
    float m_dashImpulse = 700.f;
};
