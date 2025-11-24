#pragma once
#include <map>
#include "State.h"

template <typename TOwner, typename TStateId>
class StateMachine
{
public:
    explicit StateMachine(TOwner& owner)
        : m_owner(owner)
        , m_currentState(nullptr)
        , m_currentId()
    {
    }

    void AddState(TStateId id, State<TOwner>* state)
    {
        m_states[id] = state;
    }

    void ChangeState(TStateId id)
    {
        auto it = m_states.find(id);
        if (it == m_states.end())
            return;

        State<TOwner>* nextState = it->second;
        if (m_currentState == nextState)
            return;

        if (m_currentState != nullptr)
            m_currentState->Exit(m_owner);

        m_currentState = nextState;
        m_currentId = id;

        if (m_currentState != nullptr)
            m_currentState->Enter(m_owner);
    }

    void Update()
    {
        if (m_currentState != nullptr)
            m_currentState->Update(m_owner);
    }

    State<TOwner>* GetCurrentState() const
    {
        return m_currentState;
    }

    TStateId GetCurrentId() const
    {
        return m_currentId;
    }

    bool HasState(TStateId id) const
    {
        return m_states.find(id) != m_states.end();
    }

private:
    TOwner& m_owner;
    State<TOwner>* m_currentState;
    TStateId m_currentId;
    std::map<TStateId, State<TOwner>*> m_states;
};
