#pragma once

template <typename TOwner>
class State
{
public:
    virtual ~State() = default;

    virtual void Enter(TOwner& owner) = 0;
    virtual void Update(TOwner& owner) = 0;
    virtual void Exit(TOwner& owner) = 0;
};

