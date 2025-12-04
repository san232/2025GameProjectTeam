#include "pch.h"
#include "Ghost.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "ResourceManager.h"
#include "GDISelector.h"
#include "Player.h"
#include "Defines.h"

#include <cmath>

Ghost::Ghost()
{
	SetHp(1);
	SetMoveSpeed(80.f);
	SetAttackPower(1);
	SetAttackCooltime(0.8f);
	SetAttackRange(30.f);
}

Ghost::~Ghost()
{

}

void Ghost::Update()
{
	BaseEnemy::Update();
}

void Ghost::Render(HDC _hdc)
{
	BaseEnemy::Render(_hdc);

	Vec2 pos = GetPos();
	Vec2 size = GetSize();

	BrushType brush = BrushType::HOLLOW;
	PenType   pen = PenType::RED;

	GDISelector brushSelector(_hdc, brush);
	GDISelector penSelector(_hdc, pen);

	RECT_RENDER(_hdc, pos.x, pos.y, size.x, size.y);

	Vec2 attackSize = { size.x * m_attackHitboxScale,
						size.y * m_attackHitboxScale };

	RECT_RENDER(_hdc, pos.x, pos.y, attackSize.x, attackSize.y);
}

void Ghost::EnterCollision(Collider* _other)
{

}

void Ghost::StayCollision(Collider* _other)
{

}

void Ghost::ExitCollision(Collider* _other)
{

}

void Ghost::Attack()
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

void Ghost::Dead()
{
	BaseEnemy::Dead();
}