// EnemyMoveState.cpp :contentReference[oaicite:1]{index=1}
#include "pch.h"
#include "EnemyMoveState.h"

EnemyMoveState::EnemyMoveState(BaseEnemy* _owner)
    : m_enemy(_owner)
{
}

void EnemyMoveState::Enter(StateMachine& _owner)
{
    cout << "Move";
}

void EnemyMoveState::Update(StateMachine& _owner)
{
    m_enemy->Move();
}

void EnemyMoveState::Exit(StateMachine& _owner)
{
}
