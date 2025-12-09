#pragma once
#include "State.h"

class BossKnight;

class BossDashState : public State
{
public:
	explicit BossDashState(BossKnight* _owner);
	~BossDashState() override = default;

	void Enter(StateMachine& _owner) override;
	void Update(StateMachine& _owner) override;
	void Exit(StateMachine& _owner) override;

private:
	BossKnight* m_boss;
	Vec2 m_dashDir;
	float m_timer;
	float m_duration;
};
