#pragma once
#include "State.h"

class MirrorBoss;

class MirrorBossChargeState : public State
{
public:
	explicit MirrorBossChargeState(MirrorBoss* owner);
	~MirrorBossChargeState() override = default;

	void Enter(StateMachine& owner) override;
	void Update(StateMachine& owner) override;
	void Exit(StateMachine& owner) override;

private:
	MirrorBoss* m_boss;
};
