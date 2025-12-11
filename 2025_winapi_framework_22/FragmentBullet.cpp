#include "pch.h"
#include "FragmentBullet.h"
#include "Collider.h"
#include "Player.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GDISelector.h"

FragmentBullet::FragmentBullet()
    : m_direction{ 1.f, 0.f }
    , m_lifeTime(0.f)
    , m_maxLifeTime(1.5f)
{
    SetSize({10.f, 10.f});
    AddComponent<Collider>()->SetSize(GetSize());
    SetHp(1);
    SetMoveSpeed(350.f);
    SetAttackPower(5);
}

FragmentBullet::~FragmentBullet() {}

void FragmentBullet::Update()
{
    Entity::Update();
    Move();
    m_lifeTime += fDT;
    if (m_lifeTime >= m_maxLifeTime)
    {
        Dead();
    }
}

void FragmentBullet::Move()
{
    Translate(m_direction * GetMoveSpeed() * fDT);
}

void FragmentBullet::Render(HDC _hdc)
{
    Vec2 pos = GetPos();
    Vec2 size = GetSize();
    GDISelector brush(_hdc, BrushType::YELLOW);
    ELLIPSE_RENDER(_hdc, pos.x, pos.y, size.x, size.y);
    ComponentRender(_hdc);
}

void FragmentBullet::EnterCollision(Collider* _other)
{
    Object* otherObj = _other->GetOwner();
    if (!otherObj || otherObj->GetIsDead()) return;

    if (dynamic_cast<Player*>(otherObj))
    {
        otherObj->TakeDamage(GetAttackPower());
        Dead();
    }
}

void FragmentBullet::Dead()
{
    GET_SINGLE(SceneManager)->GetCurScene()->RequestDestroy(this);
}