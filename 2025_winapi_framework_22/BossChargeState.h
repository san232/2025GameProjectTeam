#pragma once
#include "State.h"

class BossKnight;

class BossChargeState : public State
{
public:
	explicit BossChargeState(BossKnight* _owner);
	~BossChargeState() override = default;

	void Enter(StateMachine& _owner) override;
	void Update(StateMachine& _owner) override;
	void Exit(StateMachine& _owner) override;

private:
	BossKnight* m_boss;
	float m_timer;
	float m_duration;
};
