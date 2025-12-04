#include "pch.h"
#include "EnemyAttackState.h"

EnemyAttackState::EnemyAttackState(BaseEnemy* _owner)
    : m_enemy(_owner)
{
}

void EnemyAttackState::Enter(StateMachine& _owner)
{
    m_enemy->Attack();
}

void EnemyAttackState::Update(StateMachine& _owner)
{
}

void EnemyAttackState::Exit(StateMachine& _owner)
{
}
