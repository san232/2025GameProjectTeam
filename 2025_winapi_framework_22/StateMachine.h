#pragma once
#include "State.h"

class StateMachine
{
public:
    StateMachine();

    void ChangeState(State* _nextState);
    void Update();

private:
    State* m_currentState;
};
