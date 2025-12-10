#include "pch.h"
#include "Zombie.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "ResourceManager.h"
#include "GDISelector.h"
#include "Player.h"
#include "Animator.h"
#include "Defines.h"

#include <cmath>

Zombie::Zombie()
{
	SetHp(6);
	SetMoveSpeed(150.f);
	SetAttackPower(1);
	SetAttackCooltime(0.f);
	SetAttackRange(50.f);
	SetExp(20);
	SetDefaultLookRight(true);

	SetStatMulti();

	m_attackDelay = 0.1f;

	m_pTex = GET_SINGLE(ResourceManager)->GetTexture(L"Zombie");

	m_animator->CreateAnimation(L"Move", m_pTex, { 0.f,   0.f }, { 64.f, 64.f }, { 63.f, 0.f }, 7, 0.1f);
	m_animator->CreateAnimation(L"Dead", m_pTex, { 0.f,  65.f }, { 64.f, 64.f }, { 63.f, 0.f }, 8, 0.1f);
	m_animator->CreateAnimation(L"Attack", m_pTex, { 0.f, 120.f }, { 64.f, 64.f }, { 63.f, 0.f }, 6, 0.1f);
	m_animator->CreateAnimation(L"Idle", m_pTex, { 0.f, 180.f }, { 64.f, 64.f }, { 63.f, 0.f }, 4, 0.1f);
	m_animator->CreateAnimation(L"Hit", m_pTex, { 0.f, 240.f }, { 64.f, 64.f }, { 63.f, 0.f }, 3, 0.1f);
}

Zombie::~Zombie()
{
}

void Zombie::Update()
{
	BaseEnemy::Update();

	if (m_isDashing)
	{
		m_dashTimer += fDT;
		if (m_dashTimer >= m_dashDuration)
		{
			m_dashTimer = 0.f;
			m_isDashing = false;
			m_hasHitPlayerThisDash = false;

			m_rigidbody->SetVelocity(Vec2(0.f, 0.f));
		}
	}
}

void Zombie::Render(HDC _hdc)
{
	BaseEnemy::Render(_hdc);
}

void Zombie::EnterCollision(Collider* _other)
{
	if (!m_isDashing || m_hasHitPlayerThisDash || !_other)
		return;

	Object* otherObj = _other->GetOwner();
	Player* player = dynamic_cast<Player*>(otherObj);
	if (!player || player->GetIsDead())
		return;

	player->TakeDamage(GetAttackPower());
	m_hasHitPlayerThisDash = true;
}

void Zombie::StayCollision(Collider* _other)
{
}

void Zombie::ExitCollision(Collider* _other)
{
}

void Zombie::Attack()
{
	if (m_isDashing)
		return;

	Player* player = GetTargetPlayer();
	if (!player || player->GetIsDead())
		return;

	Vec2 toPlayer = player->GetPos();
	toPlayer -= GetPos();

	float lenSq = toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y;
	if (lenSq <= 0.f)
		return;

	float len = std::sqrt(lenSq);
	Vec2 dir(toPlayer.x / len, toPlayer.y / len);

	m_isDashing = true;
	m_dashTimer = 0.f;
	m_hasHitPlayerThisDash = false;

	m_rigidbody->SetVelocity(Vec2(0.f, 0.f));
	m_rigidbody->AddImpulse(dir * m_dashImpulse);
}

void Zombie::Dead()
{
	BaseEnemy::Dead();
}
