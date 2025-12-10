#include "pch.h"
#include "SnailBullet.h"
#include "Collider.h"
#include "Player.h"
#include "Texture.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "GDISelector.h"
#include "Defines.h"

#include <cmath>

SnailBullet::SnailBullet()
    : m_direction{ 1.f,0.f }
    , m_lifeTime(0.f)
    , m_maxLifeTime(20.f)
{
    AddComponent<Collider>();
    SetHp(1);
    SetMoveSpeed(300.f);
    SetAttackPower(1);
    m_pTex = GET_SINGLE(ResourceManager)->GetTexture(L"SnailBullet");
}

SnailBullet::~SnailBullet()
{

}

void SnailBullet::Update()
{
    Entity::Update();

    Move();

    m_lifeTime += fDT;
    if (m_lifeTime >= m_maxLifeTime)
    {
        Dead();
    }
}

void SnailBullet::Render(HDC _hdc)
{
    Vec2 pos = GetPos();
    Vec2 size = GetSize();
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

    ComponentRender(_hdc);
}

void SnailBullet::EnterCollision(Collider* _other)
{
    Object* otherObj = _other->GetOwner();
    if (!otherObj || otherObj->GetIsDead())
        return;

    Player* player = dynamic_cast<Player*>(otherObj);
    if (!player)
        return;

    player->TakeDamage(GetAttackPower());
    Dead();
}

void SnailBullet::StayCollision(Collider* _other)
{
}

void SnailBullet::ExitCollision(Collider* _other)
{
}

void SnailBullet::Move()
{
    Translate(m_direction * GetMoveSpeed() * fDT);
}




void SnailBullet::Dead()
{
    GET_SINGLE(SceneManager)->GetCurScene()->RequestDestroy(this);
}
