#include "pch.h"
#include "BossChargeState.h"
#include "BossKnight.h"
#include "BossDashState.h" 
#include "StateMachine.h"
#include "Rigidbody.h"
#include "Player.h"
#include "Vec2.h"

BossChargeState::BossChargeState(BossKnight* _owner)
	: m_boss(_owner)
	, m_timer(0.f)
	, m_duration(0.6f) 
{
}

void BossChargeState::Enter(StateMachine& _owner)
{
	m_timer = 0.f;

	if (Rigidbody* rb = m_boss->GetComponent<Rigidbody>())
	{
		rb->SetVelocity(Vec2(0.f, 0.f));
	}

	Player* target = m_boss->GetTargetPlayer();
	Vec2 dashDir = Vec2(1.f, 0.f); // Default direction

	if (target)
	{
		Vec2 myPos = m_boss->GetPos();
		Vec2 targetPos = target->GetPos();
		dashDir = targetPos - myPos;
		dashDir.Normalize();
	}

	// Lock the dash direction now, so it doesn't change during the 0.6s charge duration.
	m_boss->GetDashState()->SetDashDir(dashDir);

	m_boss->ChangeAnimation(L"Charge", false);
}

void BossChargeState::Update(StateMachine& _owner)
{
	m_timer += fDT;

	if (m_timer >= m_duration)
	{
		_owner.ChangeState(m_boss->GetDashState());
	}
}

void BossChargeState::Exit(StateMachine& _owner)
{
}
