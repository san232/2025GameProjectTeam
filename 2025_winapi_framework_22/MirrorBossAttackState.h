#pragma once
#include "State.h"

class MirrorBoss;

class MirrorBossAttackState : public State
{
public:
	explicit MirrorBossAttackState(MirrorBoss* owner);
	~MirrorBossAttackState() override = default;

	void Enter(StateMachine& owner) override;
	void Update(StateMachine& owner) override;
	void Exit(StateMachine& owner) override;

private:
	MirrorBoss* m_boss;
	float m_timer;
	float m_shotTimer;
	int m_firedCount;
};
