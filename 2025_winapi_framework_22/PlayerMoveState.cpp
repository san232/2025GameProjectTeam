#include "pch.h"
#include "PlayerMoveState.h"

PlayerMoveState::PlayerMoveState(Player* owner)
	: player(owner)
{

}

void PlayerMoveState::Enter(StateMachine& owner)
{
	cout << "Move";
}

void PlayerMoveState::Update(StateMachine& owner)
{
	player->Move();
}

void PlayerMoveState::Exit(StateMachine& owner)
{

}
