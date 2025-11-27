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

Player::Player()
	: m_pTex(nullptr)
	, m_rigidCompo(nullptr)
	, m_movementSpeed(200.f)
	, m_rollingSpeed(500.f)
	, m_isRolling(false)
	, m_rollingCooltime(3.f)
	, m_curTime(0.f)
	, m_isCanRolling(true)
{
	AddComponent<Collider>();
	AddComponent<Rigidbody>();
	AddComponent<Animator>();

	m_rigidCompo = GetComponent<Rigidbody>();
}

Player::~Player()
{
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
	UpdateInput();
	CooldownRollingTime();
}


void Player::UpdateInput()
{
	if (m_rigidCompo == nullptr)
		return;

	Vec2 dir = {};
	if (GET_KEY(KEY_TYPE::A)) dir.x -= 1.f;
	if (GET_KEY(KEY_TYPE::D)) dir.x += 1.f;
	if (GET_KEY(KEY_TYPE::W)) dir.y -= 1.f;
	if (GET_KEY(KEY_TYPE::S)) dir.y += 1.f;

	bool hasInput = (dir.x != 0.f || dir.y != 0.f);
	if (hasInput)
	{
		float len = sqrtf(dir.x * dir.x + dir.y * dir.y);
		dir.x /= len;
		dir.y /= len;
	}

	Translate({ fDT * dir.x * m_movementSpeed, fDT * dir.y * m_movementSpeed });

	if (GET_KEYDOWN(KEY_TYPE::SPACE) && dir != Vec2{0.f,0.f} && m_isCanRolling)
	{
		m_rigidCompo->AddImpulse(dir * m_rollingSpeed);
		m_isCanRolling = false;
	}
}

void Player::CooldownRollingTime()
{
	if (m_isCanRolling == false)
		m_curTime += fDT;

	if (m_curTime >= m_rollingCooltime)
	{
		m_isCanRolling = true;
		m_curTime = 0;
	}
}
