#include "pch.h"
#include "StateMachine.h"

StateMachine::StateMachine()
	: currentState(nullptr)
{
}

void StateMachine::ChangeState(State* nextState)
{
	if (currentState == nextState) return;

	if(currentState)
		currentState->Exit(*this);
	currentState = nextState;
	currentState->Enter(*this);
}

void StateMachine::Update()
{
	currentState->Update(*this);
}
