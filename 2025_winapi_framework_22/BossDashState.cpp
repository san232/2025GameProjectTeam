#include "pch.h"
#include "BossDashState.h"
#include "BossKnight.h"
#include "StateMachine.h"
#include "Rigidbody.h"
#include "Player.h"

BossDashState::BossDashState(BossKnight* _owner)
	: m_boss(_owner)
	, m_dashDir{}
	, m_timer(0.f)
	, m_duration(0.8f)
{
}

void BossDashState::Enter(StateMachine& _owner)
{
	m_timer = 0.f;

	Player* target = m_boss->GetTargetPlayer();
	if (target)
	{
		Vec2 myPos = m_boss->GetPos();
		Vec2 targetPos = target->GetPos();
		Vec2 dir = targetPos - myPos;
		dir.Normalize();
		m_dashDir = dir;
	}
	else
	{
		m_dashDir = Vec2(1.f, 0.f);
	}

	m_boss->ChangeAnimation(L"Dash", true);
}

void BossDashState::Update(StateMachine& _owner)
{
	m_timer += fDT;

	if (Rigidbody* rb = m_boss->GetComponent<Rigidbody>())
	{
		float currentSpeed = m_boss->GetMoveSpeed();
		float dashSpeed = currentSpeed * 4.0f; 
		rb->SetVelocity(m_dashDir * dashSpeed);
	}

	if (m_timer >= m_duration)
	{
	}
}

void BossDashState::Exit(StateMachine& _owner)
{
	if (Rigidbody* rb = m_boss->GetComponent<Rigidbody>())
	{
		rb->SetVelocity(Vec2(0.f, 0.f));
	}
}
