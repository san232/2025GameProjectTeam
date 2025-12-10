#include "pch.h"
#include "MirrorBossIdleState.h"
#include "MirrorBoss.h"

MirrorBossIdleState::MirrorBossIdleState(MirrorBoss* owner)
	: m_boss(owner)
{
}

void MirrorBossIdleState::Enter(StateMachine& owner)
{
	m_boss->ChangeAnimation(L"Idle");
}

void MirrorBossIdleState::Update(StateMachine& owner)
{
}

void MirrorBossIdleState::Exit(StateMachine& owner)
{
}
