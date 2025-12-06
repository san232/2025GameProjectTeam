#pragma once
#include "State.h"
#include "BaseEnemy.h"

class EnemyDeadState : public State
{
public:
    explicit EnemyDeadState(BaseEnemy* _owner);

    void Enter(StateMachine& _owner) override;
    void Update(StateMachine& _owner) override;
    void Exit(StateMachine& _owner) override;

private:
    BaseEnemy* m_enemy;
    float m_curTime;
    float m_deadTime;
};
