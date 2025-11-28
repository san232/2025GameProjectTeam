#pragma once
#include "Object.h"
#include "Enums.h"

class Texture;
class Collider;
class Rigidbody;

class Player : public Object
{
public:
	Player();
	~Player();

public:
	void Update() override;
	void Render(HDC _hdc) override;
	void EnterCollision(Collider* _other) override;
	void StayCollision(Collider* _other) override;
	void ExitCollision(Collider* _other) override;
private:
	void UpdateInput();
	void CooldownRollingTime();
	void BlockPlayer();

private:
	Texture* m_pTex;
	Rigidbody* m_rigidCompo;
	float m_movementSpeed;
	float m_rollingSpeed;
	bool m_isRolling;
	float m_rollingCooltime;
	float m_curTime;
	bool m_isCanRolling;
};
