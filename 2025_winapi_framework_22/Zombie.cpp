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
	SetMoveSpeed(80.f);
	SetAttackPower(1);
	SetAttackCooltime(0.8f);
	SetAttackRange(40.f);
	SetExp(20);
	SetDefaultLookRight(true);

	m_pTex = GET_SINGLE(ResourceManager)->GetTexture(L"Zombie");

	m_animator->CreateAnimation(L"Move", m_pTex, { 0.f, 0.f }, { 64.f, 64.f }, { 64.f,0.f }, 7, 0.1f);
	m_animator->CreateAnimation(L"Dead", m_pTex, { 0.f, 64.f }, { 64.f, 64.f }, { 64.f, 0.f }, 7, 0.1f);
	m_animator->CreateAnimation(L"Attack", m_pTex, { 0.f, 128.f }, { 64.f, 64.f }, { 64.f,0.f }, 6, 0.1f);
	m_animator->CreateAnimation(L"Idle", m_pTex, { 0.f, 192.f }, { 64.f, 64.f }, { 64.f,0.f }, 4, 0.1f);
	m_animator->CreateAnimation(L"Hit", m_pTex, { 0.f, 256.f }, { 64.f, 64.f }, { 64.f,  0.f }, 3, 0.1f);
}

Zombie::~Zombie()
{

}

void Zombie::Update()
{
	BaseEnemy::Update();
}

void Zombie::Render(HDC _hdc)
{
	BaseEnemy::Render(_hdc);

	Vec2 pos = GetPos();
	Vec2 size = GetSize();

	BrushType brush = BrushType::HOLLOW;
	PenType   pen = PenType::RED;

	GDISelector brushSelector(_hdc, brush);
	GDISelector penSelector(_hdc, pen);

	Vec2 attackSize = { size.x * m_attackHitboxScale,
						size.y * m_attackHitboxScale };

	RECT_RENDER(_hdc, pos.x, pos.y, attackSize.x, attackSize.y);
}

void Zombie::EnterCollision(Collider* _other)
{

}

void Zombie::StayCollision(Collider* _other)
{

}

void Zombie::ExitCollision(Collider* _other)
{

}

void Zombie::Attack()
{
	Player* player = GetTargetPlayer();
	if (!player || player->GetIsDead())
		return;

	Vec2 zombiePos = GetPos();
	Vec2 zombieSize = GetSize();

	Vec2 playerPos = player->GetPos();
	Vec2 playerSize = player->GetSize();

	Vec2 attackSize = { zombieSize.x * m_attackHitboxScale,
						zombieSize.y * m_attackHitboxScale };

	float halfAw = attackSize.x * 0.5f;
	float halfAh = attackSize.y * 0.5f;
	float halfPw = playerSize.x * 0.5f;
	float halfPh = playerSize.y * 0.5f;

	float dx = std::fabs(playerPos.x - zombiePos.x);
	float dy = std::fabs(playerPos.y - zombiePos.y);

	bool isHit =
		(dx <= (halfAw + halfPw)) &&
		(dy <= (halfAh + halfPh));

	if (isHit)
	{
		player->TakeDamage(GetAttackPower());
		cout << "공격 성공";
	}
}

void Zombie::Dead()
{
	BaseEnemy::Dead();
}