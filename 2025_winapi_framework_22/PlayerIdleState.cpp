#include "pch.h"
#include "PlayerIdleState.h"
#include "Rigidbody.h"

PlayerIdleState::PlayerIdleState(Player* owner)
	: player(owner)
{
}

void PlayerIdleState::Enter(StateMachine& owner)
{
	cout << "Idle";
	player->StopMoving();
}

void PlayerIdleState::Update(StateMachine& owner)
{

}

void PlayerIdleState::Exit(StateMachine& owner)
{

}