#pragma once
#include "State.h"
#include "Enums.h"

class Player;

class PlayerIdleState : public State<Player>
{
public:
	void Enter(Player& owner) override;
	void Update(Player& owner) override;
	void Exit(Player& owner) override;
};

class PlayerMoveState : public State<Player>
{
public:
	PlayerMoveState();

	void Enter(Player& owner) override;
	void Update(Player& owner) override;
	void Exit(Player& owner) override;

private:
	float m_moveSpeed;
};

class PlayerRollingState : public State<Player>
{
public:
	PlayerRollingState();

	void Enter(Player& owner) override;
	void Update(Player& owner) override;
	void Exit(Player& owner) override;

private:
	float m_duration;
	float m_elapsed;
	float m_rollSpeed;
};

class PlayerDeadState : public State<Player>
{
public:
	void Enter(Player& owner) override;
	void Update(Player& owner) override;
	void Exit(Player& owner) override;
};
