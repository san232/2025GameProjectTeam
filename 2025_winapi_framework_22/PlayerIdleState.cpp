#include "pch.h"
#include "PlayerIdleState.h"
#include "Rigidbody.h"

PlayerIdleState::PlayerIdleState(Player* _owner)
	: m_player(_owner)
{
}

void PlayerIdleState::Enter(StateMachine& _owner)
{
	m_player->StopMoving();
}

void PlayerIdleState::Update(StateMachine& _owner)
{

}

void PlayerIdleState::Exit(StateMachine& _owner)
{

}