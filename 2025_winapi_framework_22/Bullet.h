#pragma once
#include "Entity.h"

class Texture;

class Bullet : public Entity
{
public:
    Bullet();
    virtual ~Bullet();

    virtual void Update() override;
    virtual void Render(HDC _hdc) override;
    virtual void EnterCollision(Collider* _other) override;

    void SetDirection(Vec2 _dir) { m_direction = _dir; }
    Vec2 GetDirection() const { return m_direction; }

    void SetLifeTime(float _life) { m_maxLifeTime = _life; }
    float GetLifeTime() const { return m_lifeTime; }

protected:
    virtual void Move() override;
    virtual void Dead() override;
    virtual void Attack() override {}

protected:
    Vec2 m_direction;
    Texture* m_pTex;
    float m_lifeTime;
    float m_maxLifeTime;
};
