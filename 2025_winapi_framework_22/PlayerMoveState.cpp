#include "pch.h"
#include "PlayerMoveState.h"

PlayerMoveState::PlayerMoveState(Player* _owner)
	: m_player(_owner)
{

}

void PlayerMoveState::Enter(StateMachine& _owner)
{
}

void PlayerMoveState::Update(StateMachine& _owner)
{
	m_player->Move();
}

void PlayerMoveState::Exit(StateMachine& _owner)
{

}
