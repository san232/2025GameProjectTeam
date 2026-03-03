#include "pch.h"
#include "MirrorBossAttackState.h"
#include "MirrorBoss.h"
#include "StateMachine.h"

MirrorBossAttackState::MirrorBossAttackState(MirrorBoss* owner)
	: m_boss(owner)
	, m_timer(0.f)
	, m_shotTimer(0.f)
	, m_firedCount(0)
{
}

void MirrorBossAttackState::Enter(StateMachine& owner)
{
	m_boss->ChangeAnimation(L"Attack", false);
	m_timer = 0.f;
	m_shotTimer = 0.f;
	m_firedCount = 0;
}

void MirrorBossAttackState::Update(StateMachine& owner)
{
	m_timer += fDT;
	m_shotTimer += fDT;

	float fireInterval = 1.f / m_boss->GetAttackSpeed();

	if (m_firedCount < m_boss->GetVolleyCount())
	{
		if (m_shotTimer >= fireInterval)
		{
			m_boss->FireRandomBullet();
			m_firedCount++;
			m_shotTimer = 0.f;
		}
	}
	
	// Transition back to Idle if all bullets are fired AND the state duration has passed.
	if (m_firedCount >= m_boss->GetVolleyCount() && m_timer >= m_boss->GetAttackDuration())
	{
		owner.ChangeState(m_boss->GetIdleState());
	}
}

void MirrorBossAttackState::Exit(StateMachine& owner)
{
}
