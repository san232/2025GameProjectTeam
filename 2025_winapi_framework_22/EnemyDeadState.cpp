#include "pch.h"
#include "EnemyDeadState.h"
#include "Rigidbody.h"
#include "SceneManager.h"
#include "Scene.h"

EnemyDeadState::EnemyDeadState(BaseEnemy* _owner)
    : m_enemy(_owner)
{
}

void EnemyDeadState::Enter(StateMachine& _owner)
{
    m_enemy->Dead();

    if (Rigidbody* rb = m_enemy->GetComponent<Rigidbody>())
    {
        rb->SetVelocity({ 0.f, 0.f });
    }

}

void EnemyDeadState::Update(StateMachine& _owner)
{
}

void EnemyDeadState::Exit(StateMachine& _owner)
{
}
