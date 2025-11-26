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
	, rigidCompo(nullptr)
	, movementSpeed(200.f)
	, rollingSpeed(500.f)
	, isRolling(false)
	, rollingCooltime(3.f)
	, curTime(0.f)
	, isCanRolling(true)
{
	AddComponent<Collider>();
	AddComponent<Rigidbody>();
	AddComponent<Animator>();

	rigidCompo = GetComponent<Rigidbody>();
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
	if (rigidCompo == nullptr)
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

	Translate({ fDT * dir.x * movementSpeed, fDT * dir.y * movementSpeed });

	isRolling = GET_KEYDOWN(KEY_TYPE::SPACE);
	if (isRolling && hasInput && isCanRolling)
	{
		rigidCompo->AddImpulse(dir * rollingSpeed);
		isCanRolling = false;
	}
}

void Player::CooldownRollingTime()
{
	if (isCanRolling == false)
		curTime += fDT;

	if (curTime >= rollingCooltime)
	{
		isCanRolling = true;
		curTime = 0;
	}
}
