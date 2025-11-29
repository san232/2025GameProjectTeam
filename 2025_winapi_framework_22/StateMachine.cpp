#include "pch.h"
#include "StateMachine.h"

StateMachine::StateMachine()
    : currentState(nullptr)
{
}

void StateMachine::ChangeState(State* nextState)
{
    if (currentState)
        currentState->Exit(*this);

    currentState = nextState;

    if (currentState)
        currentState->Enter(*this);
}

void StateMachine::Update()
{
    if (currentState)
        currentState->Update(*this);
}
