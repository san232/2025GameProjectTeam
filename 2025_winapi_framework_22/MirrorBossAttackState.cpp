#include "pch.h"
#include "MirrorBossAttackState.h"
#include "MirrorBoss.h"
#include "StateMachine.h"

MirrorBossAttackState::MirrorBossAttackState(MirrorBoss* owner)
	: m_boss(owner)
{
}

void MirrorBossAttackState::Enter(StateMachine& owner)
{
	m_boss->ChangeAnimation(L"Attack", false);
}

void MirrorBossAttackState::Update(StateMachine& owner)
{
}

void MirrorBossAttackState::Exit(StateMachine& owner)
{
}
