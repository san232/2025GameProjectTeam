#include "pch.h"
#include "PlayerBullet.h"

#include "Collider.h"
#include "BaseEnemy.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "Texture.h"
#include "GDISelector.h"
#include "Defines.h"

PlayerBullet::PlayerBullet()
    : m_direction{ 1.f, 0.f }
    , m_lifeTime(0.f)
    , m_maxLifeTime(2.f)
{
    m_pTex = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerBullet");
    AddComponent<Collider>();
    SetHp(1);
    SetMoveSpeed(800.f);
}

PlayerBullet::~PlayerBullet()
{
}

void PlayerBullet::Update()
{
    Entity::Update();

    Move();

    m_lifeTime += fDT;
    if (m_lifeTime >= m_maxLifeTime)
    {
        Dead();
    }
}

void PlayerBullet::Render(HDC _hdc)
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

void PlayerBullet::EnterCollision(Collider* _other)
{
    Object* otherObj = _other->GetOwner();
    BaseEnemy* enemy = dynamic_cast<BaseEnemy*>(otherObj);

    if (enemy == nullptr) return;
    if (enemy->GetDeadState()) return;

    enemy->OnHit(GetAttackPower());
    Dead();
}

void PlayerBullet::StayCollision(Collider* _other)
{
}

void PlayerBullet::ExitCollision(Collider* _other)
{
}

void PlayerBullet::Move()
{
    Translate(m_direction * m_moveSpeed * fDT);
}

void PlayerBullet::Dead()
{
    std::shared_ptr<Scene> curScene = GET_SINGLE(SceneManager)->GetCurScene();
    if (curScene)
    {
        curScene->RequestDestroy(this);
    }
}
