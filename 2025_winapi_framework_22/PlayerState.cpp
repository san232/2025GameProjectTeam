#include "pch.h"
#include "PlayerState.h"
#include "Player.h"
#include "Animator.h"
#include "Animation.h"
#include "Rigidbody.h"
#include "InputManager.h"
#include "Defines.h"

PlayerMoveState::PlayerMoveState()
	: m_moveSpeed(200.f)
{
}

PlayerRollingState::PlayerRollingState()
	: m_duration(0.3f)
	, m_elapsed(0.f)
	, m_rollSpeed(400.f)
{
}

void PlayerIdleState::Enter(Player& owner)
{
	/*Animator* animator = owner.GetComponent<Animator>();
	if (animator != nullptr)
		animator->Play(L"Idle", PlayMode::Loop, 1, 1.f);*/

	Rigidbody* rigidbody = owner.GetComponent<Rigidbody>();
	if (rigidbody != nullptr)
	{
		rigidbody->SetVelocity({ 0.f,0.f });
	}
}

void PlayerIdleState::Update(Player& owner)
{
	bool left = GET_KEY(KEY_TYPE::LEFT) || GET_KEY(KEY_TYPE::A);
	bool right = GET_KEY(KEY_TYPE::RIGHT) || GET_KEY(KEY_TYPE::D);
	bool roll = GET_KEYDOWN(KEY_TYPE::SPACE);

	if (roll)
	{
		owner.ChangeState(PlayerStateType::Rolling);
		return;
	}

	if (left || right)
	{
		owner.ChangeState(PlayerStateType::Move);
		return;
	}
}

void PlayerIdleState::Exit(Player& owner)
{
}

void PlayerMoveState::Enter(Player& owner)
{
	/*Animator* animator = owner.GetComponent<Animator>();
	if (animator != nullptr)
		animator->Play(L"Move", PlayMode::Loop, 1, 1.f);*/
}

void PlayerMoveState::Update(Player& owner)
{
	bool left = GET_KEY(KEY_TYPE::LEFT) || GET_KEY(KEY_TYPE::A);
	bool right = GET_KEY(KEY_TYPE::RIGHT) || GET_KEY(KEY_TYPE::D);
	bool roll = GET_KEYDOWN(KEY_TYPE::SPACE);

	if (roll)
	{
		owner.ChangeState(PlayerStateType::Rolling);
		return;
	}

	int dir = 0;
	if (left)
		dir -= m_moveSpeed;
	if (right)
		dir += m_moveSpeed;

	if (dir == 0)
	{
		owner.ChangeState(PlayerStateType::Idle);
		return;
	}

	Rigidbody* rigidbody = owner.GetComponent<Rigidbody>();
	if (rigidbody != nullptr)
	{
		Vec2 velocity = rigidbody->GetVelocity();
		velocity.x = m_moveSpeed * static_cast<float>(dir);
		rigidbody->SetVelocity(velocity);
	}
}

void PlayerMoveState::Exit(Player& owner)
{
}

void PlayerRollingState::Enter(Player& owner)
{
	m_elapsed = 0.f;

	bool left = GET_KEY(KEY_TYPE::LEFT) || GET_KEY(KEY_TYPE::A);
	bool right = GET_KEY(KEY_TYPE::RIGHT) || GET_KEY(KEY_TYPE::D);

	int dir = 0;
	if (left)
		dir -= 1;
	if (right)
		dir += 1;
	if (dir == 0)
		dir = 1;

	Rigidbody* rigidbody = owner.GetComponent<Rigidbody>();
	if (rigidbody != nullptr)
	{
		Vec2 velocity = rigidbody->GetVelocity();
		velocity.x = m_rollSpeed * static_cast<float>(dir);
		rigidbody->SetVelocity(velocity);
	}

	/*Animator* animator = owner.GetComponent<Animator>();
	if (animator != nullptr)
		animator->Play(L"Roll", PlayMode::Once, 1, 1.f);*/
}

void PlayerRollingState::Update(Player& owner)
{
	m_elapsed += fDT;
	if (m_elapsed < m_duration)
		return;

	bool left = GET_KEY(KEY_TYPE::LEFT) || GET_KEY(KEY_TYPE::A);
	bool right = GET_KEY(KEY_TYPE::RIGHT) || GET_KEY(KEY_TYPE::D);

	if (left || right)
		owner.ChangeState(PlayerStateType::Move);
	else
		owner.ChangeState(PlayerStateType::Idle);
}

void PlayerRollingState::Exit(Player& owner)
{
}

void PlayerDeadState::Enter(Player& owner)
{
	/*Animator* animator = owner.GetComponent<Animator>();
	if (animator != nullptr)
		animator->Play(L"Dead", PlayMode::Once, 1, 1.f);*/

	Rigidbody* rigidbody = owner.GetComponent<Rigidbody>();
	if (rigidbody != nullptr)
	{
		Vec2 velocity = rigidbody->GetVelocity();
		velocity.x = 0.f;
		rigidbody->SetVelocity(velocity);
	}
}

void PlayerDeadState::Update(Player& owner)
{
}

void PlayerDeadState::Exit(Player& owner)
{
}
