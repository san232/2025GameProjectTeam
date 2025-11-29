#pragma once

class StateMachine;

class State
{
public:
    virtual ~State() = default;

    virtual void Enter(StateMachine& owner) abstract;
    virtual void Update(StateMachine& owner) abstract;
    virtual void Exit(StateMachine& owner) abstract;
};
