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
	, m_rigidCompo(nullptr)
	, m_moveDirection({0.f,0.f})
	, m_dashPower(500.f)
	, m_curTime(0.f)
	, m_isCanAttack(true)
{
	AddComponent<Collider>();
	AddComponent<Rigidbody>();
	AddComponent<Animator>();

	m_rigidCompo = GetComponent<Rigidbody>();

	m_idleState = new PlayerIdleState(this);
	m_moveState = new PlayerMoveState(this);
	m_attackState = new PlayerAttackState(this);
	m_deadState = new PlayerDeadState(this);

	m_stateMachine->ChangeState(m_idleState);

	SetHp(10);
	SetMoveSpeed(200.0f);
	SetAttackCooltime(3.f);
	SetAttackPower(2);
}

Player::~Player()
{
	SAFE_DELETE(m_idleState);
	SAFE_DELETE(m_moveState);
	SAFE_DELETE(m_attackState);
	SAFE_DELETE(m_deadState);
	SAFE_DELETE(m_stateMachine);
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
	m_stateMachine->Update();

	UpdateInput();
	CooldownRollingTime();
	BlockPlayer();
}


void Player::UpdateInput()
{
	if (m_rigidCompo == nullptr)
		return;

	m_moveDirection = { 0.f,0.f };
	if (GET_KEY(KEY_TYPE::A)) m_moveDirection.x -= 1.f;
	if (GET_KEY(KEY_TYPE::D)) m_moveDirection.x += 1.f;
	if (GET_KEY(KEY_TYPE::W)) m_moveDirection.y -= 1.f;
	if (GET_KEY(KEY_TYPE::S)) m_moveDirection.y += 1.f;

	bool hasInput = m_moveDirection != Vec2{ 0.f,0.f };
	if (hasInput)
	{
		float len = sqrtf(m_moveDirection.x * m_moveDirection.x + m_moveDirection.y * m_moveDirection.y);
		m_moveDirection.x /= len;
		m_moveDirection.y /= len;
		m_stateMachine->ChangeState(m_moveState);
	}
	else
	{
		m_stateMachine->ChangeState(m_idleState);
	}


	if (GET_KEYDOWN(KEY_TYPE::SPACE) && hasInput && m_isCanAttack)
	{
		m_stateMachine->ChangeState(m_attackState);
		m_isCanAttack = false;
	}
}

void Player::CooldownRollingTime()
{
	if (m_isCanAttack == false)
		m_curTime += fDT;

	if (m_curTime >= m_attackCooltime)
	{
		m_isCanAttack = true;
		m_curTime = 0;
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
	m_rigidCompo->SetVelocity({ 0.f,0.f });
}

void Player::Attack()
{
	m_rigidCompo->AddImpulse(m_moveDirection * m_dashPower);
}

void Player::Dead()
{

}

void Player::Move()
{
	Translate({ fDT * m_moveDirection.x * m_moveSpeed, fDT * m_moveDirection.y * m_moveSpeed });
}
