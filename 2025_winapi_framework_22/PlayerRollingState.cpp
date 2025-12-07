#include "pch.h"
#include "PlayerRollingState.h"

PlayerRollingState::PlayerRollingState(Player* _owner)
	: m_player(_owner)
{

}

void PlayerRollingState::Enter(StateMachine& _owner)
{
	m_player->ChangeAnimation(L"Rolling");
	m_player->Rolling();
	m_player->SetInvincibility(true);
}

void PlayerRollingState::Update(StateMachine& _owner)
{

}

void PlayerRollingState::Exit(StateMachine& _owner)
{
	m_player->SetInvincibility(false);
}
