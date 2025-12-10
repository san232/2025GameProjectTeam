#pragma once
#include "BaseEnemy.h"

class Conch : public BaseEnemy
{
public:
    Conch();
    ~Conch() override;

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
    void SpawnBullet();

private:
    float m_attackShotDelay = 0.3f;
    float m_attackShotTimer = 0.f;
    bool m_isShotCharging = false;
};
