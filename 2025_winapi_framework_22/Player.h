#pragma once
#include "Entity.h"
#include "Enums.h"
#include "StateMachine.h"

class Texture;
class Collider;
class Rigidbody;
class PlayerIdleState;
class PlayerMoveState;

class Player : public Entity
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

public:
	Rigidbody* m_rigidCompo;

private:
	PlayerIdleState* idleState;
	PlayerMoveState* moveState;


private:
	StateMachine* m_stateMachine = new StateMachine;
	Texture* m_pTex;
	float m_movementSpeed;
	float m_rollingSpeed;
	bool m_isRolling;
	float m_rollingCooltime;
	float m_curTime;
	bool m_isCanRolling;

	// Entity을(를) 통해 상속됨
	void Attack() override;
	void Dead() override;
	void Move() override;
};
