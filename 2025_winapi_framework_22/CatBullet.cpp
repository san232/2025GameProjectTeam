#include "pch.h"
#include "CatBullet.h"
#include "Collider.h"
#include "Player.h"
#include "Texture.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "GDISelector.h"
#include "Defines.h"

#include <cmath>

CatBullet::CatBullet()
    : m_direction{ 1.f,0.f }
    , m_speed(150.f)
    , m_lifeTime(0.f)
    , m_maxLifeTime(5.f)
    , m_homingStrength(2.0f)
    , m_target(nullptr)
    , m_damage(1)
{
    AddComponent<Collider>();
    SetHp(1);
    SetAttackPower(10);
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

    player->TakeDamage(m_damage);
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
        Vec2 targetPos = m_target->GetPos();
        Vec2 desired = { targetPos.x - pos.x, targetPos.y - pos.y };

        float len = std::sqrt(desired.x * desired.x + desired.y * desired.y);
        if (len > 0.f)
        {
            desired.x /= len;
            desired.y /= len;

            float t = m_homingStrength * fDT;
            if (t > 1.f) t = 1.f;

            m_direction.x = m_direction.x + (desired.x - m_direction.x) * t;
            m_direction.y = m_direction.y + (desired.y - m_direction.y) * t;

            float lenDir = std::sqrt(m_direction.x * m_direction.x + m_direction.y * m_direction.y);
            if (lenDir > 0.f)
            {
                m_direction.x /= lenDir;
                m_direction.y /= lenDir;
            }
        }
    }

    Translate(m_direction * m_speed * fDT);
}

void CatBullet::Dead()
{
    std::shared_ptr<Scene> curScene = GET_SINGLE(SceneManager)->GetCurScene();
    if (curScene)
    {
        curScene->RequestDestroy(this);
    }
}
