#pragma once
#include "State.h"
#include "Player.h"

class PlayerAttackState : public State
{
public:
    explicit PlayerAttackState(Player* owner);

    void Enter(StateMachine& owner) override;
    void Update(StateMachine& owner) override;
    void Exit(StateMachine& owner) override;

private:
    Player* player;
};

