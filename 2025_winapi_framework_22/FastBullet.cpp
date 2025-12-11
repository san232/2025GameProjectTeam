#include "pch.h"
#include "FastBullet.h"
#include "Collider.h"
#include "Rigidbody.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Player.h"
#include "Texture.h"
#include "GDISelector.h"

FastBullet::FastBullet()
	: m_direction{}
	, m_lifeTime(0.f)
	, m_maxLifeTime(3.f)
	, m_pTex(nullptr)
{
	SetSize(Vec2(30.f, 30.f));
	AddComponent<Collider>()->SetSize({ 30.f, 30.f });
	
	SetMoveSpeed(1500.f);
	SetAttackPower(10);
	
	m_pTex = GET_SINGLE(ResourceManager)->GetTexture(L"WizardBullet");
}

FastBullet::~FastBullet()
{
}

void FastBullet::Update()
{
	Entity::Update();
	Move();

	m_lifeTime += fDT;
	if (m_lifeTime >= m_maxLifeTime)
	{
		Dead();
	}
}

void FastBullet::Move()
{
	Vec2 pos = GetPos();
	pos += m_direction * GetMoveSpeed() * fDT;
	SetPos(pos);
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

void FastBullet::EnterCollision(Collider* _other)
{
	Object* obj = _other->GetOwner();
    Player* player = dynamic_cast<Player*>(obj);
	if (player && !player->GetIsDead())
	{
		player->TakeDamage(GetAttackPower());
		Dead();
	}
}

void FastBullet::Dead()
{
	GET_SINGLE(SceneManager)->GetCurScene()->RequestDestroy(this);
}
