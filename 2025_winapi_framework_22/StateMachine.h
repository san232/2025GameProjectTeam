#pragma once
#include "State.h"

class StateMachine
{
public:
    StateMachine();

    void ChangeState(State* _nextState);
    void Update();

    State* GetCurState() { return m_currentState; }

private:
    State* m_currentState;
};
