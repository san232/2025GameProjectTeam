#include "pch.h"
#include "NecromancerBullet.h"
#include "Collider.h"
#include "Player.h"
#include "Texture.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "GDISelector.h"
#include "Defines.h"

#include <cmath>

NecromancerBullet::NecromancerBullet()
    : m_direction{ 1.f,0.f }
    , m_lifeTime(0.f)
    , m_maxLifeTime(5.f)
    , m_homingStrength(1.5f)
{
    AddComponent<Collider>()->SetSize({ 20.f,20.f });
    SetHp(1);
    SetMoveSpeed(200.f);
    SetAttackPower(1);
    m_pTex = GET_SINGLE(ResourceManager)->GetTexture(L"NecromancerBullet");
}

NecromancerBullet::~NecromancerBullet()
{

}

void NecromancerBullet::Update()
{
    Entity::Update();

    Move();

    m_lifeTime += fDT;
    if (m_lifeTime >= m_maxLifeTime)
    {
        Dead();
    }
}

void NecromancerBullet::Render(HDC _hdc)
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

void NecromancerBullet::EnterCollision(Collider* _other)
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

void NecromancerBullet::StayCollision(Collider* _other)
{
}

void NecromancerBullet::ExitCollision(Collider* _other)
{
}

void NecromancerBullet::Move()
{
    Translate(m_direction * GetMoveSpeed() * fDT);
}




void NecromancerBullet::Dead()
{
    GET_SINGLE(SceneManager)->GetCurScene()->RequestDestroy(this);
}
