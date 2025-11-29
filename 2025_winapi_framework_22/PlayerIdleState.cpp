#include "pch.h"
#include "PlayerIdleState.h"
#include "Rigidbody.h"

PlayerIdleState::PlayerIdleState(Player* owner)
	: player(owner)
{
}

void PlayerIdleState::Enter(StateMachine& owner)
{
	player->m_rigidCompo->SetVelocity({ 0.f,0.f });
}

void PlayerIdleState::Update(StateMachine& owner)
{

}

void PlayerIdleState::Exit(StateMachine& owner)
{

}