#pragma once
#include "State.h"
#include "Player.h"

class PlayerIdleState : public State
{
public:
    explicit PlayerIdleState(Player* owner);

    void Enter(StateMachine& owner) override;
    void Update(StateMachine& owner) override;
    void Exit(StateMachine& owner) override;

private:
    Player* player;
};

