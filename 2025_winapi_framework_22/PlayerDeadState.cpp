#include "pch.h"
#include "PlayerDeadState.h"

PlayerDeadState::PlayerDeadState(Player* _owner)
	: m_player(_owner)
{

}

void PlayerDeadState::Enter(StateMachine& _owner)
{
	m_player->ChangeAnimation(L"Dead");
}

void PlayerDeadState::Update(StateMachine& _owner)
{

}

void PlayerDeadState::Exit(StateMachine& _owner)
{

}
