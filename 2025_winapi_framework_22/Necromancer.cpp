#include "pch.h"
#include "Necromancer.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "ResourceManager.h"
#include "Animator.h"
#include "Player.h"
#include "SceneManager.h"
#include "Scene.h"
#include "NecromancerBullet.h"
#include "Defines.h"

#include <cmath>

Necromancer::Necromancer()
{
    SetHp(10);
    SetMoveSpeed(70.f);
    SetAttackCooltime(0.9f);
    SetAttackRange(300.f);
    SetExp(15);
    SetAttackDelay(0.f);
    SetDefaultLookRight(true);

    SetStatMulti();

    m_pTex = GET_SINGLE(ResourceManager)->GetTexture(L"Necromancer");

    m_attackShotDelay = 0.3f;
    m_attackShotTimer = 0.f;
    m_isShotCharging = false;

    m_animator->CreateAnimation(L"Idle", m_pTex, { 0.f,  5.f }, { 64.f, 64.f }, { 64.f, 0.f }, 6, 0.1f);
    m_animator->CreateAnimation(L"Move", m_pTex, { 0.f, 70.f }, { 64.f, 64.f }, { 64.f, 0.f }, 5, 0.1f);
    m_animator->CreateAnimation(L"Hit", m_pTex, { 0.f, 130.f }, { 64.f, 64.f }, { 64.f, 0.f }, 3, 0.1f);
    m_animator->CreateAnimation(L"Attack", m_pTex, { 0.f, 195. }, { 64.f, 64.f }, { 64.f, 0.f }, 9, 0.03f);
    m_animator->CreateAnimation(L"Dead", m_pTex, { 0.f, 260.f }, { 64.f, 64.f }, { 64.f, 0.f }, 10, 0.05f);
}

Necromancer::~Necromancer()
{
} 

void Necromancer::Update()
{
    BaseEnemy::Update();

    if (m_isShotCharging)
    {
        m_attackShotTimer += fDT;
        if (m_attackShotTimer >= m_attackShotDelay)
        {
            m_attackShotTimer = 0.f;
            m_isShotCharging = false;
            SpawnBullet();
        }
    }
}

void Necromancer::Render(HDC _hdc)
{
    BaseEnemy::Render(_hdc);
}

void Necromancer::EnterCollision(Collider* _other)
{
}

void Necromancer::StayCollision(Collider* _other)
{
}

void Necromancer::ExitCollision(Collider* _other)
{
}

void Necromancer::Attack()
{
    if (m_isShotCharging)
        return;

    Player* player = GetTargetPlayer();
    if (!player || player->GetIsDead())
        return;

    m_isShotCharging = true;
    m_attackShotTimer = 0.f;
}

void Necromancer::Dead()
{
    BaseEnemy::Dead();
}

void Necromancer::SpawnBullet()
{
    Player* player = GetTargetPlayer();
    if (!player || player->GetIsDead())
        return;

    Vec2 wizardPos = GetPos();
    Vec2 playerPos = player->GetPos();

    Vec2 dir = playerPos - wizardPos;
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (len <= 0.f)
        return;

    dir.x /= len;
    dir.y /= len;

    Vec2 normal(-dir.y, dir.x);
    float spread = 0.4f;

    Vec2 dirs[3];
    dirs[0] = dir + normal * spread;
    dirs[1] = dir;
    dirs[2] = dir - normal * spread;

    Vec2 bulletSize = { 20.f, 20.f };

    auto scene = GET_SINGLE(SceneManager)->GetCurScene();

    for (int i = 0; i < 3; ++i)
    {
        float dLen = std::sqrt(dirs[i].x * dirs[i].x + dirs[i].y * dirs[i].y);
        if (dLen <= 0.f)
            continue;

        dirs[i].x /= dLen;
        dirs[i].y /= dLen;

        NecromancerBullet* bullet = new NecromancerBullet;
        bullet->SetPos(wizardPos);
        bullet->SetSize(bulletSize);
        bullet->SetDirection(dirs[i]);
        scene->AddObject(bullet, Layer::ENEMYBULLET);
    }
}

