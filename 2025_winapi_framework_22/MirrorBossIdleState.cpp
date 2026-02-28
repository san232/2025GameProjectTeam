#include "pch.h"
#include "MirrorBossIdleState.h"
#include "MirrorBoss.h"

MirrorBossIdleState::MirrorBossIdleState(MirrorBoss* owner)
	: m_boss(owner)
	, m_timer(0.f)
{
}

void MirrorBossIdleState::Enter(StateMachine& owner)
{
	m_boss->ChangeAnimation(L"Idle");
	m_timer = 0.f;
}

void MirrorBossIdleState::Update(StateMachine& owner)
{
	m_timer += fDT;
	if (m_timer >= m_boss->GetIdleDuration())
	{
		owner.ChangeState(m_boss->GetChargeState());
	}
}

void MirrorBossIdleState::Exit(StateMachine& owner)
{
}
