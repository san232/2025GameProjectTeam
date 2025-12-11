#include "pch.h"
#include "BoomerangBullet.h"
#include "Collider.h"
#include "Player.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "GDISelector.h"
#include "Texture.h"

BoomerangBullet::BoomerangBullet()
    : m_direction{ 1.f, 0.f }
    , m_lifeTime(0.f)
    , m_maxLifeTime(3.f)
    , m_turnTime(1.0f)
    , m_isReturning(false)
{
    AddComponent<Collider>()->SetSize({ 25.f, 25.f });
    SetHp(1);
    SetMoveSpeed(400.f);
    SetAttackPower(5);
    m_pTex = GET_SINGLE(ResourceManager)->GetTexture(L"MirrorBossBullet");
}

BoomerangBullet::~BoomerangBullet() {}

void BoomerangBullet::Update()
{
    Entity::Update();
    Move();

    m_lifeTime += fDT;
    if (m_lifeTime >= m_maxLifeTime)
    {
        Dead();
    }

    if (!m_isReturning && m_lifeTime >= m_turnTime)
    {
        m_isReturning = true;
        m_direction = -m_direction;
    }
}

void BoomerangBullet::Move()
{
    Translate(m_direction * GetMoveSpeed() * fDT);
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
}

void BoomerangBullet::EnterCollision(Collider* _other)
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

void BoomerangBullet::Dead()
{
    GET_SINGLE(SceneManager)->GetCurScene()->RequestDestroy(this);
}