#pragma once
#include "State.h"
#include "BaseEnemy.h"

class EnemyAttackState : public State
{
public:
    explicit EnemyAttackState(BaseEnemy* _owner);

    void Enter(StateMachine& _owner) override;
    void Update(StateMachine& _owner) override;
    void Exit(StateMachine& _owner) override;

private:
    BaseEnemy* m_enemy;
};
