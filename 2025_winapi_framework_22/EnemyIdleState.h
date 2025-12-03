#pragma once
#include "State.h"
#include "BaseEnemy.h"

class EnemyIdleState : public State
{
public:
    explicit EnemyIdleState(BaseEnemy* _owner);

    void Enter(StateMachine& _owner) override;
    void Update(StateMachine& _owner) override;
    void Exit(StateMachine& _owner) override;

private:
    BaseEnemy* m_enemy;
};
