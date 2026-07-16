#include "pch.h"
#include "SpriteRenderer.h"
#include "HomingBullet.h"
#include "Collider.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "GDISelector.h"

HomingBullet::HomingBullet()
	: m_target(nullptr)
{
	SetSize(Vec2(30.f, 30.f));
	if (Collider* col = GetComponent<Collider>())
		col->SetSize({ 30.f, 30.f });
	
	SetMoveSpeed(250.f);
	SetAttackPower(10);
	SetLifeTime(5.f);
	
	m_pTex = GET_SINGLE(ResourceManager)->GetTexture(L"MirrorBossBullet");
}

HomingBullet::~HomingBullet()
{
}

void HomingBullet::Move()
{
	if (m_target != nullptr && !m_target->GetIsDead())
	{
		Vec2 targetPos = m_target->GetPos();
		Vec2 myPos = GetPos();
		Vec2 dir = targetPos - myPos;
		dir.Normalize();
		m_direction = dir; 
	}

	Bullet::Move();
}

void HomingBullet::Render(HDC _hdc)
{
	Vec2 pos = GetPos();
	Vec2 size = GetSize();

	if (m_pTex)
	{
		LONG width = m_pTex->GetWidth();
		LONG height = m_pTex->GetHeight();

		GET_SINGLE(SpriteRenderer)->Draw(m_pTex->GetSRV(), (float)((int)(pos.x - size.x / 2)), (float)((int)(pos.y - size.y / 2)), (float)(size.x), (float)(size.y), (float)(0), (float)(0), (float)(width), (float)(height), (float)(width), (float)(height));
	}
	else
	{
		GET_SINGLE(SpriteRenderer)->DrawFilledRect(pos.x - size.x / 2.0f, pos.y - size.y / 2.0f, size.x, size.y, XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
	}

	ComponentRender(_hdc);
}
