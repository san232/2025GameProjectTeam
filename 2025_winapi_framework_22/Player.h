#pragma once
#include "Object.h"
#include "StateMachine.h"
#include "PlayerState.h"
#include "Enums.h"

class Texture;
class Collider;

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

	void ChangeState(PlayerStateType stateType);

private:
	Texture* m_pTex;

	StateMachine<Player, PlayerStateType> m_stateMachine;
	PlayerIdleState m_idleState;
	PlayerMoveState m_moveState;
	PlayerRollingState m_rollingState;
	PlayerDeadState m_deadState;
};
