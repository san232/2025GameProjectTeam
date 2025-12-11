#include "pch.h"
#include "HomingBullet.h"
#include "Collider.h"
#include "Rigidbody.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Player.h"
#include "Texture.h"
#include "GDISelector.h"

HomingBullet::HomingBullet()
	: m_direction{}
	, m_target(nullptr)
	, m_lifeTime(0.f)
	, m_maxLifeTime(5.f)
	, m_pTex(nullptr)
{
	SetSize(Vec2(30.f, 30.f));
	AddComponent<Collider>()->SetSize({ 30.f, 30.f });
	
	SetMoveSpeed(250.f);
	SetAttackPower(10);
	
	m_pTex = GET_SINGLE(ResourceManager)->GetTexture(L"WizardBullet"); 
}

HomingBullet::~HomingBullet()
{
}

void HomingBullet::Update()
{
	Entity::Update();
	Move();

	m_lifeTime += fDT;
	if (m_lifeTime >= m_maxLifeTime)
	{
		Dead();
	}
}

void HomingBullet::Move()
{
	if (m_target && !m_target->GetIsDead())
	{
		Vec2 targetPos = m_target->GetPos();
		Vec2 myPos = GetPos();
		Vec2 dir = targetPos - myPos;
		dir.Normalize();
		m_direction = dir; 
	}

	Vec2 pos = GetPos();
	pos += m_direction * GetMoveSpeed() * fDT;
	SetPos(pos);
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

void HomingBullet::EnterCollision(Collider* _other)
{
	Object* obj = _other->GetOwner();
	Player* player = dynamic_cast<Player*>(obj);
	if (player && !player->GetIsDead())
	{
		player->TakeDamage(GetAttackPower());
		Dead();
	}
}

void HomingBullet::Dead()
{
	GET_SINGLE(SceneManager)->GetCurScene()->RequestDestroy(this);
}
