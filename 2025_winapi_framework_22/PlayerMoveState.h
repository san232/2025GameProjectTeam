#pragma once
#include "State.h"
#include "Player.h"

class PlayerMoveState : public State
{
public:
    explicit PlayerMoveState(Player* _owner);

    void Enter(StateMachine& _owner) override;
    void Update(StateMachine& _owner) override;
    void Exit(StateMachine& _owner) override;

private:
    Player* m_player;
};