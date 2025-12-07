// EnemyHitState.cpp :contentReference[oaicite:4]{index=4}
#include "pch.h"
#include "EnemyHitState.h"
#include "Rigidbody.h"

EnemyHitState::EnemyHitState(BaseEnemy* _owner)
    : m_enemy(_owner)
{
}

void EnemyHitState::Enter(StateMachine& _owner)
{
    m_enemy->ChangeAnimation(L"Hit");
    if (Rigidbody* rb = m_enemy->GetComponent<Rigidbody>())
    {
        rb->SetVelocity({ 0.f, 0.f });
    }
}

void EnemyHitState::Update(StateMachine& _owner)
{
}

void EnemyHitState::Exit(StateMachine& _owner)
{
}
