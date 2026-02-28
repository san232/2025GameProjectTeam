#include "pch.h"
#include "FastBullet.h"
#include "Collider.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "GDISelector.h"

FastBullet::FastBullet()
{
	SetSize(Vec2(30.f, 30.f));
	if (Collider* col = GetComponent<Collider>())
		col->SetSize({ 30.f, 30.f });
	
	SetMoveSpeed(1500.f);
	SetAttackPower(10);
	
	m_pTex = GET_SINGLE(ResourceManager)->GetTexture(L"WizardBullet");
}

FastBullet::~FastBullet()
{
}

void FastBullet::Move()
{
	Bullet::Move();
}

void FastBullet::Render(HDC _hdc)
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
		GDISelector brush(_hdc, BrushType::RED);
		ELLIPSE_RENDER(_hdc, pos.x, pos.y, size.x, size.y);
	}

	ComponentRender(_hdc);
}
