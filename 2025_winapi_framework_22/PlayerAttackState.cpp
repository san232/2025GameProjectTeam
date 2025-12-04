#include "pch.h"
#include "PlayerAttackState.h"

PlayerAttackState::PlayerAttackState(Player* _owner)
	: m_player(_owner)
{

}

void PlayerAttackState::Enter(StateMachine& _owner)
{
	m_player->ChangeAnimation(L"Attack");
	m_player->Attack();
}

void PlayerAttackState::Update(StateMachine& _owner)
{

}

void PlayerAttackState::Exit(StateMachine& _owner)
{

}
