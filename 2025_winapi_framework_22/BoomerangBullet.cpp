#include "pch.h"
#include "BoomerangBullet.h"
#include "Collider.h"
#include "ResourceManager.h"
#include "Texture.h"

BoomerangBullet::BoomerangBullet()
    : m_turnTime(1.0f)
    , m_isReturning(false)
{
    if (Collider* col = GetComponent<Collider>())
        col->SetSize({ 25.f, 25.f });
    SetHp(1);
    SetMoveSpeed(400.f);
    SetAttackPower(5);
    m_pTex = GET_SINGLE(ResourceManager)->GetTexture(L"MirrorBossBullet");
}

BoomerangBullet::~BoomerangBullet() {}

void BoomerangBullet::Move()
{
    if (!m_isReturning && m_lifeTime >= m_turnTime)
    {
        m_isReturning = true;
        m_direction = -m_direction;
    }
    Bullet::Move();
}

void BoomerangBullet::Render(HDC _hdc)
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
    ComponentRender(_hdc);
}
