#pragma once
#include "Entity.h"
#include "Enums.h"
#include "StateMachine.h"

class Texture;
class Collider;
class Rigidbody;
class PlayerIdleState;
class PlayerMoveState;
class PlayerAttackState;
class PlayerDeadState;

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

private:
	PlayerIdleState* m_idleState;
	PlayerMoveState* m_moveState;
	PlayerAttackState* m_attackState;
	PlayerDeadState* m_deadState;


private:
	StateMachine* m_stateMachine = new StateMachine;
	Texture* m_pTex;
	Rigidbody* m_rigidCompo;
	Vec2 m_moveDirection;
	float m_dashPower;
	float m_curTime;
	bool m_isCanAttack;

public:
	void Attack() override;
	void Dead() override;
	void Move() override;
	void StopMoving();
};
