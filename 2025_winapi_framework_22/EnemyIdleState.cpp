// EnemyIdleState.cpp :contentReference[oaicite:0]{index=0}
#include "pch.h"
#include "EnemyIdleState.h"
#include "Rigidbody.h"

EnemyIdleState::EnemyIdleState(BaseEnemy* _owner)
    : m_enemy(_owner)
{
}

void EnemyIdleState::Enter(StateMachine& _owner)
{
    if (Rigidbody* rb = m_enemy->GetComponent<Rigidbody>())
    {
        rb->SetVelocity({ 0.f, 0.f });
    }
}

void EnemyIdleState::Update(StateMachine& _owner)
{
}

void EnemyIdleState::Exit(StateMachine& _owner)
{
}
