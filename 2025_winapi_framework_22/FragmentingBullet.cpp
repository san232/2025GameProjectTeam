#include "pch.h"
#include "FragmentingBullet.h"
#include "FragmentBullet.h"
#include "Collider.h"
#include "Player.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GDISelector.h"
#include "ResourceManager.h"
#include "Texture.h"

FragmentingBullet::FragmentingBullet()
    : m_direction{ 1.f, 0.f }
    , m_lifeTime(0.f)
    , m_explodeTime(0.7f)
{
    AddComponent<Collider>()->SetSize({ 30.f, 30.f });
    SetHp(1);
    SetMoveSpeed(300.f);
    SetAttackPower(5);
    m_pTex = GET_SINGLE(ResourceManager)->GetTexture(L"MirrorBossBullet");
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
}

void FragmentingBullet::EnterCollision(Collider* _other)
{
    Entity* otherObj = dynamic_cast<Entity*>(_other->GetOwner());
    if (!otherObj || otherObj->GetIsDead()) return;

    otherObj->TakeDamage(GetAttackPower());
    Dead();
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