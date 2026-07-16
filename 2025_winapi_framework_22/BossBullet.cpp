#include "pch.h"
#include "SpriteRenderer.h"
#include "BossBullet.h"
#include "Collider.h"
#include "Player.h"
#include "Texture.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "GDISelector.h"
#include "Defines.h"

BossBullet::BossBullet()
    : m_direction{ 1.f,0.f }
    , m_lifeTime(0.f)
    , m_maxLifeTime(5.f)
{
    AddComponent<Collider>();
    SetHp(1);
    SetMoveSpeed(300.f); 
    SetAttackPower(2); 
    m_pTex = GET_SINGLE(ResourceManager)->GetTexture(L"KnightBossBullet"); 
    
    if (Collider* col = GetComponent<Collider>())
    {
        col->SetSize({ 20.f, 20.f });
    }
}

BossBullet::~BossBullet()
{
}

void BossBullet::Update()
{
    Entity::Update();

    Move();

    m_lifeTime += fDT;
    if (m_lifeTime >= m_maxLifeTime)
    {
        Dead();
    }
}

void BossBullet::Render(HDC _hdc)
{
    Vec2 pos = GetPos();
    Vec2 size = GetSize();
    
    if (m_pTex)
    {
        LONG width = m_pTex->GetWidth();
        LONG height = m_pTex->GetHeight();

        GET_SINGLE(SpriteRenderer)->Draw(m_pTex->GetSRV(), (float)((int)(pos.x - size.x / 2)), (float)((int)(pos.y - size.y / 2)), (float)(size.x), (float)(size.y), (float)(0), (float)(0), (float)(width), (float)(height), (float)(width), (float)(height));
    }
    ComponentRender(_hdc);
}

void BossBullet::EnterCollision(Collider* _other)
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

void BossBullet::StayCollision(Collider* _other)
{
}

void BossBullet::ExitCollision(Collider* _other)
{
}

void BossBullet::Move()
{
    Translate(m_direction * GetMoveSpeed() * fDT);
}

void BossBullet::Dead()
{
    GET_SINGLE(SceneManager)->GetCurScene()->RequestDestroy(this);
}
