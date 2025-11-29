#pragma once
#include "State.h"

class StateMachine
{
public:
    StateMachine();

    void ChangeState(State* nextState);
    void Update();

private:
    State* currentState;
};
