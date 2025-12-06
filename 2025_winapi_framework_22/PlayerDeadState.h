#pragma once
#include "State.h"
#include "Player.h"

class PlayerDeadState : public State
{
public:
    explicit PlayerDeadState(Player* _owner);

    void Enter(StateMachine& _owner) override;
    void Update(StateMachine& _owner) override;
    void Exit(StateMachine& _owner) override;

private:
    Player* m_player;
    float m_curTime;
    float m_deadTime;
};

