#include "pch.h"
#include "CatBullet.h"
#include "Collider.h"
#include "Player.h"
#include "Texture.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "EffectManager.h"
#include "Scene.h"
#include "CatBulletDeadEffect.h"
#include "GDISelector.h"
#include "Defines.h"

#include <cmath>

CatBullet::CatBullet()
    : m_direction{ 1.f,0.f }
    , m_lifeTime(0.f)
    , m_maxLifeTime(5.f)
    , m_homingStrength(1.5f)
    , m_target(nullptr)
{
    AddComponent<Collider>()->SetSize({ 10.f,10.f });
    SetHp(1);
    SetMoveSpeed(200.f);
    SetAttackPower(1);
    m_pTex = GET_SINGLE(ResourceManager)->GetTexture(L"CatBullet");
    
}

CatBullet::~CatBullet()
{
}

void CatBullet::Update()
{
    Entity::Update();

    Move();

    m_lifeTime += fDT;
    if (m_lifeTime >= m_maxLifeTime)
    {
        Dead();
    }
}

void CatBullet::Render(HDC _hdc)
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

void CatBullet::EnterCollision(Collider* _other)
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

void CatBullet::StayCollision(Collider* _other)
{
}

void CatBullet::ExitCollision(Collider* _other)
{
}

void CatBullet::Move()
{
    Vec2 pos = GetPos();

    if (m_target && !m_target->GetIsDead())
    {
        Vec2 toTarget = { m_target->GetPos().x - pos.x, m_target->GetPos().y - pos.y };

        float len = std::sqrt(toTarget.x * toTarget.x + toTarget.y * toTarget.y);
        if (len > 0.f)
        {
            Vec2 desired = { toTarget.x / len, toTarget.y / len };

            float t = m_homingStrength * fDT;
            if (t > 1.f) t = 1.f;

            m_direction.x = m_direction.x + (desired.x - m_direction.x) * t;
            m_direction.y = m_direction.y + (desired.y - m_direction.y) * t;
        }
    }

    Translate(m_direction * GetMoveSpeed() * fDT);
}




void CatBullet::Dead()
{
    GET_SINGLE(EffectManager)->CreateEffect<CatBulletDeadEffect>(GetPos(), { 70.f,70.f }, 2.f);
    GET_SINGLE(SceneManager)->GetCurScene()->RequestDestroy(this);
}
