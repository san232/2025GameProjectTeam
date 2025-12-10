#include "pch.h"
#include "Conch.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "ResourceManager.h"
#include "Animator.h"
#include "Player.h"
#include "SceneManager.h"
#include "Scene.h"
#include "ConchBullet.h"
#include "Defines.h"

#include <cmath>

Conch::Conch()
{
    SetHp(5);
    SetMoveSpeed(70.f);
    SetAttackCooltime(1.2f);
    SetAttackRange(300.f);
    SetExp(15);
    SetAttackDelay(0.f);
    SetDefaultLookRight(false);

    SetStatMulti();

    m_pTex = GET_SINGLE(ResourceManager)->GetTexture(L"Conch");

    m_attackShotDelay = 0.3f;
    m_attackShotTimer = 0.f;
    m_isShotCharging = false;

    m_animator->CreateAnimation(L"Idle", m_pTex, { 0.f, 0.f }, { 64.f, 64.f }, { 64.f,0.f }, 5, 0.12f);
    m_animator->CreateAnimation(L"Move", m_pTex, { 0.f,  60.f }, { 64.f, 64.f }, { 64.f,0.f }, 3, 0.08f);
    m_animator->CreateAnimation(L"Hit", m_pTex, { 0.f, 115.f }, { 64.f, 64.f }, { 64.f,  0.f }, 2, 0.06f);
    m_animator->CreateAnimation(L"Attack", m_pTex, { 0.f, 170.f }, { 64.f, 64.f }, { 64.f,0.f }, 7, 0.06f);
    m_animator->CreateAnimation(L"Dead", m_pTex, { 0.f, 225.f }, { 64.f, 64.f }, { 64.f, 0.f }, 8, 0.08f);
}

Conch::~Conch()
{
}

void Conch::Update()
{
    BaseEnemy::Update();

    if (m_isShotCharging)
    {
        m_attackShotTimer += fDT;
        if (m_attackShotTimer >= m_attackShotDelay)
        {
            m_attackShotTimer = 0.f;
            m_isShotCharging = false;
            Attack();
        }
    }
}

void Conch::Render(HDC _hdc)
{
    BaseEnemy::Render(_hdc);
}

void Conch::EnterCollision(Collider* _other)
{
}

void Conch::StayCollision(Collider* _other)
{
}

void Conch::ExitCollision(Collider* _other)
{
}

void Conch::Attack()
{
    Player* player = GetTargetPlayer();
    if (!player || player->GetIsDead())
        return;

    SpawnBullet();
}

void Conch::Dead()
{
    BaseEnemy::Dead();
}

void Conch::SpawnBullet()
{
    std::shared_ptr<Scene> curScene = GET_SINGLE(SceneManager)->GetCurScene();
    if (!curScene)
        return;

    Player* player = GetTargetPlayer();
    if (!player || player->GetIsDead())
        return;

    Vec2 catPos = GetPos();
    Vec2 playerPos = player->GetPos();

    Vec2 dir = playerPos - catPos;
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (len > 0.f)
    {
        dir.x /= len;
        dir.y /= len;
    }
    else
    {
        dir = Vec2{ 1.f,0.f };
    }

    Vec2 bulletSize = { 20.f,20.f };

    ConchBullet* bullet = new ConchBullet;
    bullet->SetPos(catPos);
    bullet->SetSize(bulletSize);
    bullet->SetDirection(dir);
    bullet->SetTarget(player);

    curScene->AddObject(bullet, Layer::ENEMYBULLET);
}
