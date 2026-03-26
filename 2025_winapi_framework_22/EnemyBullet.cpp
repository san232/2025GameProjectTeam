// EnemyBullet.cpp  (»õ ÆÄÀÏ)
#include "pch.h"
#include "EnemyBullet.h"

#include "Collider.h"
#include "Player.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Scene.h"

#include <cmath>

EnemyBullet::EnemyBullet(const EnemyBulletStat& spec)
    : m_homingStrength(spec.homingStrength)
    , m_maxLifeTime(spec.maxLifeTime)
{
    AddComponent<Collider>()->SetSize(spec.colliderSize);
    SetSize(spec.defaultSize);

    SetHp(1);
    SetMoveSpeed(spec.moveSpeed);
    SetAttackPower(spec.attackPower);

    m_pTex = GET_SINGLE(ResourceManager)->GetTexture(spec.textureKey);
}

void EnemyBullet::Update()
{
    Entity::Update();

    Move();

    m_lifeTime += fDT;
    if (m_lifeTime >= m_maxLifeTime)
    {
        Dead();
    }
}

void EnemyBullet::Render(HDC _hdc)
{
    if (m_pTex)
    {
        Vec2 pos = GetPos();
        Vec2 size = GetSize();

        ::TransparentBlt(
            _hdc,
            (int)(pos.x - size.x * 0.5f),
            (int)(pos.y - size.y * 0.5f),
            (int)size.x,
            (int)size.y,
            m_pTex->GetTextureDC(),
            0, 0,
            m_pTex->GetWidth(),
            m_pTex->GetHeight(),
            RGB(255, 0, 255)
        );
    }

    ComponentRender(_hdc);
}

void EnemyBullet::EnterCollision(Collider* _other)
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

void EnemyBullet::Move()
{
    if (m_homingStrength > 0.f && m_target && !m_target->GetIsDead())
    {
        Vec2 toTarget = m_target->GetPos();
        toTarget -= GetPos();

        const float len = std::sqrt(toTarget.x * toTarget.x + toTarget.y * toTarget.y);
        if (len > 0.f)
        {
            Vec2 desired(toTarget.x / len, toTarget.y / len);

            float t = m_homingStrength * fDT;
            if (t > 1.f) t = 1.f;

            m_direction.x += (desired.x - m_direction.x) * t;
            m_direction.y += (desired.y - m_direction.y) * t;
        }
    }

    Translate(m_direction * GetMoveSpeed() * fDT);
}

void EnemyBullet::Dead()
{
    OnDead();

    std::shared_ptr<Scene> curScene = GET_SINGLE(SceneManager)->GetCurScene();
    if (curScene)
    {
        curScene->RequestDestroy(this);
    }
}
