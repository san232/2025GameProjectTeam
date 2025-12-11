#include "pch.h"
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
    SetAttackPower(5); 
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

        ::TransparentBlt(_hdc
            , (int)(pos.x - size.x / 2)
            , (int)(pos.y - size.y / 2)
            , size.x
            , size.y
            , m_pTex->GetTextureDC()
            , 0, 0, width, height,
            RGB(255, 0, 255));
    }
    else
    {
        // Fallback rendering if texture is missing
        GDISelector brush(_hdc, BrushType::RED);
        ELLIPSE_RENDER(_hdc, pos.x, pos.y, size.x, size.y);
    }

    ComponentRender(_hdc);
}

void BossBullet::EnterCollision(Collider* _other)
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
