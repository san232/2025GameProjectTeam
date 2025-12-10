#include "pch.h"
#include "MirrorBossChargeState.h"
#include "MirrorBoss.h"

MirrorBossChargeState::MirrorBossChargeState(MirrorBoss* owner)
	: m_boss(owner)
{
}

void MirrorBossChargeState::Enter(StateMachine& owner)
{
	m_boss->ChangeAnimation(L"Charge", true);
	m_boss->StartCharge();
}

void MirrorBossChargeState::Update(StateMachine& owner)
{
}

void MirrorBossChargeState::Exit(StateMachine& owner)
{
	m_boss->EndCharge();
}
