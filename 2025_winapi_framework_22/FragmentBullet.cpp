#include "pch.h"
#include "FragmentBullet.h"
#include "Collider.h"
#include "Player.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Texture.h"
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
    m_pTex = GET_SINGLE(ResourceManager)->GetTexture(L"MirrorBossBullet");
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

void FragmentBullet::EnterCollision(Collider* _other)
{
    Object* otherObj = _other->GetOwner();
    if (!otherObj)
        return;

    Player* player = dynamic_cast<Player*>(otherObj);
    if (!player)
        return;

    player->TakeDamage(GetAttackPower());
    Dead();
}

void FragmentBullet::Dead()
{
    GET_SINGLE(SceneManager)->GetCurScene()->RequestDestroy(this);
}