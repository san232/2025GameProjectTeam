#pragma once
#include "BaseEnemy.h"

class Ghost : public BaseEnemy
{
public:
    Ghost();
    ~Ghost() override;

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
    float m_attackHitboxScale = 1.3f;
};
