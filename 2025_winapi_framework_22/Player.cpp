#include "pch.h"
#include "Player.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "Collider.h"
#include "Animator.h"
#include "Animation.h"
#include "Rigidbody.h"
#include "GDISelector.h"
#include "Defines.h"
#include "PlayerIdleState.h"
#include "PlayerMoveState.h"
#include "PlayerAttackState.h"
#include "PlayerDeadState.h"

Player::Player()
	: m_pTex(nullptr)
	, rigidCompo(nullptr)
	, moveDirection({0.f,0.f})
	, dashPower(500.f)
	, curTime(0.f)
	, isCanAttack(true)
{
	AddComponent<Collider>();
	AddComponent<Rigidbody>();
	AddComponent<Animator>();

	rigidCompo = GetComponent<Rigidbody>();

	idleState = new PlayerIdleState(this);
	moveState = new PlayerMoveState(this);
	attackState = new PlayerAttackState(this);
	deadState = new PlayerDeadState(this);

	stateMachine->ChangeState(idleState);

	SetHp(10);
	SetMoveSpeed(200.0f);
	SetAttackCooltime(3.f);
	SetAttackPower(2);
}

Player::~Player()
{
	SAFE_DELETE(idleState);
	SAFE_DELETE(moveState);
	SAFE_DELETE(attackState);
	SAFE_DELETE(deadState);
	SAFE_DELETE(stateMachine);
}

void Player::Render(HDC _hdc)
{
	Vec2 pos = GetPos();
	Vec2 size = GetSize();

	BrushType brush = BrushType::HOLLOW;
	PenType pen = PenType::RED;

	GDISelector brushSelector(_hdc, brush);
	GDISelector penSelector(_hdc, pen);

	RECT_RENDER(_hdc, pos.x, pos.y, size.x, size.y);

	ComponentRender(_hdc);
}

void Player::EnterCollision(Collider* _other)
{
}

void Player::StayCollision(Collider* _other)
{
}

void Player::ExitCollision(Collider* _other)
{
}

void Player::Update()
{
	Entity::Update();
	stateMachine->Update();

	UpdateInput();
	CooldownRollingTime();
	BlockPlayer();
}


void Player::UpdateInput()
{
	if (rigidCompo == nullptr)
		return;

	moveDirection = { 0.f,0.f };
	if (GET_KEY(KEY_TYPE::A)) moveDirection.x -= 1.f;
	if (GET_KEY(KEY_TYPE::D)) moveDirection.x += 1.f;
	if (GET_KEY(KEY_TYPE::W)) moveDirection.y -= 1.f;
	if (GET_KEY(KEY_TYPE::S)) moveDirection.y += 1.f;

	bool hasInput = moveDirection != Vec2{ 0.f,0.f };
	if (hasInput)
	{
		float len = sqrtf(moveDirection.x * moveDirection.x + moveDirection.y * moveDirection.y);
		moveDirection.x /= len;
		moveDirection.y /= len;
		stateMachine->ChangeState(moveState);
	}
	else
	{
		stateMachine->ChangeState(idleState);
	}


	if (GET_KEYDOWN(KEY_TYPE::SPACE) && hasInput && isCanAttack)
	{
		stateMachine->ChangeState(attackState);
		isCanAttack = false;
	}
}

void Player::CooldownRollingTime()
{
	if (isCanAttack == false)
		curTime += fDT;

	if (curTime >= m_attackCooltime)
	{
		isCanAttack = true;
		curTime = 0;
	}
}

void Player::BlockPlayer()
{
	Vec2 pos = GetPos();
	Vec2 size = GetSize();

	float x = size.x * 0.5f;
	float y = size.y * 0.5f;

	float minX = x;
	float maxX = WINDOW_WIDTH - x;
	float minY = y;
	float maxY = WINDOW_HEIGHT - y;

	pos.x = std::clamp(pos.x, minX, maxX);
	pos.y = std::clamp(pos.y, minY, maxY);

	SetPos(pos);
}

void Player::StopMoving()
{
	rigidCompo->SetVelocity({ 0.f,0.f });
}

void Player::Attack()
{
	rigidCompo->AddImpulse(moveDirection * dashPower);
}

void Player::Dead()
{

}

void Player::Move()
{
	Translate({ fDT * moveDirection.x * m_moveSpeed, fDT * moveDirection.y * m_moveSpeed });
}
