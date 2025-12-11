#include "pch.h"
#include "BoomerangBullet.h"
#include "Collider.h"
#include "Player.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "GDISelector.h"

BoomerangBullet::BoomerangBullet()
    : m_direction{ 1.f, 0.f }
    , m_lifeTime(0.f)
    , m_maxLifeTime(3.f)
    , m_turnTime(1.0f)
    , m_isReturning(false)
{
    AddComponent<Collider>()->SetSize({ 25.f, 25.f });
    SetHp(1);
    SetMoveSpeed(400.f);
    SetAttackPower(15);
}

BoomerangBullet::~BoomerangBullet() {}

void BoomerangBullet::Update()
{
    Entity::Update();
    Move();

    m_lifeTime += fDT;
    if (m_lifeTime >= m_maxLifeTime)
    {
        Dead();
    }

    if (!m_isReturning && m_lifeTime >= m_turnTime)
    {
        m_isReturning = true;
        m_direction = -m_direction;
    }
}

void BoomerangBullet::Move()
{
    Translate(m_direction * GetMoveSpeed() * fDT);
}

void BoomerangBullet::Render(HDC _hdc)
{
    Vec2 pos = GetPos();
    Vec2 size = GetSize();
    GDISelector brush(_hdc, BrushType::MAGENTA);
    ELLIPSE_RENDER(_hdc, pos.x, pos.y, size.x, size.y);
    ComponentRender(_hdc);
}

void BoomerangBullet::EnterCollision(Collider* _other)
{
    Entity* otherObj = dynamic_cast<Entity*>(_other->GetOwner());
    if (!otherObj || otherObj->GetIsDead()) return;

    if (dynamic_cast<Player*>(otherObj))
    {
        otherObj->TakeDamage(GetAttackPower());
        Dead();
    }
}

void BoomerangBullet::Dead()
{
    GET_SINGLE(SceneManager)->GetCurScene()->RequestDestroy(this);
}