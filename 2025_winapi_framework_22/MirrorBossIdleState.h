#pragma once
#include "State.h"

class MirrorBoss;

class MirrorBossIdleState : public State
{
public:
	explicit MirrorBossIdleState(MirrorBoss* owner);
	~MirrorBossIdleState() override = default;

	void Enter(StateMachine& owner) override;
	void Update(StateMachine& owner) override;
	void Exit(StateMachine& owner) override;

private:
	MirrorBoss* m_boss;
};
