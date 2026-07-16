#include "pch.h"
#include "SpriteRenderer.h"
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

    GET_SINGLE(SpriteRenderer)->Draw(m_pTex->GetSRV(), (float)((int)(pos.x - size.x / 2)), (float)((int)(pos.y - size.y / 2)), (float)(size.x), (float)(size.y), (float)(0), (float)(0), (float)(width), (float)(height), (float)(width), (float)(height));

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
