#include "pch.h"
#include "EnemyDeadState.h"
#include "Rigidbody.h"
#include "Collider.h"

EnemyDeadState::EnemyDeadState(BaseEnemy* _owner)
    : m_enemy(_owner)
    , m_curTime(0.f)
    , m_deadTime(2.f)
{
}

void EnemyDeadState::Enter(StateMachine& _owner)
{
    m_enemy->ChangeAnimation(L"Dead", false);
    if (Rigidbody* rb = m_enemy->GetComponent<Rigidbody>())
    {
        rb->SetVelocity({ 0.f, 0.f });
    }
    if (Collider* col = m_enemy->GetComponent<Collider>())
    {
        col->SetTrigger(true);
    }
}

void EnemyDeadState::Update(StateMachine& _owner)
{
    m_curTime += fDT;

    if (m_deadTime <= m_curTime)
    {
        m_enemy->Destroy();
    }
}

void EnemyDeadState::Exit(StateMachine& _owner)
{
}
