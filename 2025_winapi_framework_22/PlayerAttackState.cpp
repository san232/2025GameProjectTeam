#include "pch.h"
#include "PlayerAttackState.h"

PlayerAttackState::PlayerAttackState(Player* owner)
	: player(owner)
{

}

void PlayerAttackState::Enter(StateMachine& owner)
{
	cout << "Attack";
	player->Attack();
}

void PlayerAttackState::Update(StateMachine& owner)
{

}

void PlayerAttackState::Exit(StateMachine& owner)
{

}
