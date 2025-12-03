#include "pch.h"
#include "StateMachine.h"

StateMachine::StateMachine()
	: m_currentState(nullptr)
{
}

void StateMachine::ChangeState(State* _nextState)
{
	if (m_currentState == _nextState) return;

	if(m_currentState)
		m_currentState->Exit(*this);
	m_currentState = _nextState;
	m_currentState->Enter(*this);
}

void StateMachine::Update()
{
	m_currentState->Update(*this);
}
