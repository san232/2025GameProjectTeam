#include "pch.h"
#include "Bullet.h"
#include "Collider.h"
#include "SceneManager.h"
#include "Scene.h"

Bullet::Bullet()
    : m_direction{}
    , m_pTex(nullptr)
    , m_lifeTime(0.f)
    , m_maxLifeTime(3.f)
{
    AddComponent<Collider>();
}

Bullet::~Bullet()
{
}

void Bullet::Update()
{
    Entity::Update();
    m_lifeTime += fDT;
    if (m_lifeTime >= m_maxLifeTime)
    {
        Dead();
    }
    Move();
}

void Bullet::Render(HDC _hdc)
{
    ComponentRender(_hdc);
}

void Bullet::EnterCollision(Collider* _other)
{
    // Default collision: deal damage and die
    Object* obj = _other->GetOwner();
    Entity* target = dynamic_cast<Entity*>(obj);
    if (target && target->GetLayer() != GetLayer())
    {
        target->TakeDamage(GetAttackPower());
        Dead();
    }
}

void Bullet::Move()
{
    Translate(m_direction * m_moveSpeed * fDT);
}

void Bullet::Dead()
{
    GET_SINGLE(SceneManager)->GetCurScene()->RequestDestroy(this);
}
