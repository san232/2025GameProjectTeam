#include "pch.h"
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

		::TransparentBlt(_hdc
			, (int)(pos.x - size.x / 2)
			, (int)(pos.y - size.y / 2)
			, size.x
			, size.y
			, m_pTex->GetTextureDC()
			, 0, 0, width, height,
			RGB(255, 0, 255));
	}
	else
	{
		GDISelector brush(_hdc, BrushType::GREEN);
		ELLIPSE_RENDER(_hdc, pos.x, pos.y, size.x, size.y);
	}

	ComponentRender(_hdc);
}
