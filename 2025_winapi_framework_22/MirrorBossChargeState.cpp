#include "pch.h"
#include "MirrorBossChargeState.h"
#include "MirrorBoss.h"

MirrorBossChargeState::MirrorBossChargeState(MirrorBoss* owner)
	: m_boss(owner)
	, m_timer(0.f)
{
}

void MirrorBossChargeState::Enter(StateMachine& owner)
{
	m_boss->ChangeAnimation(L"Charge", true);
	m_boss->StartCharge();
	m_timer = 0.f;
}

void MirrorBossChargeState::Update(StateMachine& owner)
{
	m_timer += fDT;
	if (m_timer >= m_boss->GetChargeDuration())
	{
		owner.ChangeState(m_boss->GetAttackState());
	}
}

void MirrorBossChargeState::Exit(StateMachine& owner)
{
	m_boss->EndCharge();
}
