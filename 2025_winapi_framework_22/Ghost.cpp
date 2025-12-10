#include "pch.h"
#include "Ghost.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "ResourceManager.h"
#include "GDISelector.h"
#include "Player.h"
#include "Animator.h"
#include "Defines.h"

#include <cmath>

Ghost::Ghost()
{
	SetHp(6);
	SetMoveSpeed(160.f);
	SetAttackPower(1);
	SetAttackCooltime(0.8f);
	SetAttackRange(50.f);
	SetExp(20);
	SetDefaultLookRight(true);

	SetStatMulti();

	m_pTex = GET_SINGLE(ResourceManager)->GetTexture(L"Ghost");

	m_animator->CreateAnimation(L"Idle", m_pTex, { 0.f, 0.f }, { 64.f, 64.f }, { 64.f,0.f }, 5, 0.12f);
	m_animator->CreateAnimation(L"Move", m_pTex, { 0.f,  50.f }, { 64.f, 64.f }, { 64.f,0.f }, 4, 0.08f);
	m_animator->CreateAnimation(L"Hit", m_pTex, { 0.f, 100.f }, { 64.f, 64.f }, { 64.f,  0.f }, 3, 0.06f);
	m_animator->CreateAnimation(L"Attack", m_pTex, { 0.f, 155.f }, { 64.f, 64.f }, { 64.f,0.f }, 8, 0.06f);
	m_animator->CreateAnimation(L"Dead", m_pTex, { 0.f, 210.f }, { 64.f, 64.f }, { 64.f, 0.f }, 9, 0.08f);
}

Ghost::~Ghost()
{

}

void Ghost::Update()
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

void Ghost::Render(HDC _hdc)
{
	BaseEnemy::Render(_hdc);
}

void Ghost::EnterCollision(Collider* _other)
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

void Ghost::StayCollision(Collider* _other)
{
}

void Ghost::ExitCollision(Collider* _other)
{
}

void Ghost::Attack()
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

void Ghost::Dead()
{
	BaseEnemy::Dead();
}