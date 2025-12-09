#pragma once
#include "Entity.h"

class Collider;
class Player;
class Texture;

class WizardBullet : public Entity
{
public:
    WizardBullet();
    ~WizardBullet() override;

public:
    void Update() override;
    void Render(HDC _hdc) override;

    void EnterCollision(Collider* _other) override;
    void StayCollision(Collider* _other) override;
    void ExitCollision(Collider* _other) override;

public:
    void SetDirection(const Vec2& dir) { m_direction = dir; }

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
};
