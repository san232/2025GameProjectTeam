#include "pch.h"
#include "FragmentingBullet.h"
#include "FragmentBullet.h"
#include "Collider.h"
#include "Player.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GDISelector.h"

FragmentingBullet::FragmentingBullet()
    : m_direction{ 1.f, 0.f }
    , m_lifeTime(0.f)
    , m_explodeTime(0.7f)
{
    AddComponent<Collider>()->SetSize({ 30.f, 30.f });
    SetHp(1);
    SetMoveSpeed(300.f);
    SetAttackPower(5);
}

FragmentingBullet::~FragmentingBullet() {}

void FragmentingBullet::Update()
{
    Entity::Update();
    Move();

    m_lifeTime += fDT;
    if (m_lifeTime >= m_explodeTime)
    {
        Dead();
    }
}

void FragmentingBullet::Move()
{
    Translate(m_direction * GetMoveSpeed() * fDT);
}

void FragmentingBullet::Render(HDC _hdc)
{
    Vec2 pos = GetPos();
    Vec2 size = GetSize();
    GDISelector brush(_hdc, BrushType::YELLOW);
    RECT_RENDER(_hdc, pos.x, pos.y, size.x, size.y);
    ComponentRender(_hdc);
}

void FragmentingBullet::EnterCollision(Collider* _other)
{
    Object* otherObj = _other->GetOwner();
    if (!otherObj || otherObj->GetIsDead()) return;

    if (dynamic_cast<Player*>(otherObj))
    {
        otherObj->TakeDamage(GetAttackPower());
        Dead();
    }
}

void FragmentingBullet::Dead()
{
    Explode();
    GET_SINGLE(SceneManager)->GetCurScene()->RequestDestroy(this);
}

void FragmentingBullet::Explode()
{
    int fragmentCount = 8;
    for (int i = 0; i < fragmentCount; ++i)
    {
        FragmentBullet* pBullet = new FragmentBullet();
        pBullet->SetPos(GetPos());
        
        float angle = (360.f / fragmentCount) * i;
        Vec2 dir = { cosf(angle * PI / 180.f), sinf(angle * PI / 180.f) };
        
        pBullet->SetDirection(dir);
        GET_SINGLE(SceneManager)->GetCurScene()->AddObject(pBullet, Layer::ENEMYBULLET);
    }
}