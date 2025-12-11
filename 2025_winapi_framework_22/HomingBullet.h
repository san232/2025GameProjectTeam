#pragma once
#include "Entity.h"

class Texture;
class Collider;
class Player;

class HomingBullet : public Entity
{
public:
	HomingBullet();
	~HomingBullet() override;

public:
	void Update() override;
	void Render(HDC _hdc) override;
	void EnterCollision(Collider* _other) override;

public:
	void SetDirection(Vec2 dir) { m_direction = dir; }
	void SetTarget(Entity* target) { m_target = target; }

protected:
	void Attack() override {}
	void Dead() override;
	void Move() override;

private:
	Vec2 m_direction;
	Entity* m_target;
	Texture* m_pTex;
	float m_lifeTime;
	float m_maxLifeTime;
};