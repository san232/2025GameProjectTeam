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
	if (target)
	{
		Vec2 myPos = m_boss->GetPos();
		Vec2 targetPos = target->GetPos();
		/*
		if (targetPos.x < myPos.x)
			m_boss->SetDefaultLookRight(false);
		else
			m_boss->SetDefaultLookRight(true);*/
	}

	m_boss->ChangeAnimation(L"Charge", false);
}

void BossChargeState::Update(StateMachine& _owner)
{
	m_timer += fDT;
}

void BossChargeState::Exit(StateMachine& _owner)
{
}
