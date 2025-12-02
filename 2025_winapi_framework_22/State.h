#pragma once

class StateMachine;

class State
{
public:
    virtual ~State() = default;

    virtual void Enter(StateMachine& _owner) abstract;
    virtual void Update(StateMachine& _owner) abstract;
    virtual void Exit(StateMachine& _owner) abstract;
};
