#include "pch.h"
#include "Slime.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "ResourceManager.h"
#include "Animator.h"
#include "Player.h"
#include "SceneManager.h"
#include "Scene.h"
#include "SlimeBullet.h"
#include "Defines.h"

#include <cmath>

Slime::Slime()
{
    SetHp(10);
    SetMoveSpeed(70.f);
    SetAttackCooltime(2.f);
    SetAttackRange(300.f);
    SetExp(20);
    SetAttackDelay(0.f);
    SetDefaultLookRight(true);

    SetStatMulti();

    m_pTex = GET_SINGLE(ResourceManager)->GetTexture(L"Slime");

    m_animator->CreateAnimation(L"Idle", m_pTex, { 0.f,  10.f }, { 64.f, 64.f }, { 64.f, 0.f }, 6, 0.12f);
    m_animator->CreateAnimation(L"Move", m_pTex, { 0.f, 75.f }, { 64.f, 64.f }, { 64.f, 0.f }, 6, 0.08f);
    m_animator->CreateAnimation(L"Dead", m_pTex, { 0.f, 135.f }, { 64.f, 64.f }, { 64.f, 0.f }, 5, 0.08f);
    m_animator->CreateAnimation(L"Attack", m_pTex, { 0.f, 200.f }, { 64.f, 64.f }, { 64.f, 0.f }, 7, 0.06f);
    m_animator->CreateAnimation(L"Hit", m_pTex, { 0.f, 265.f }, { 64.f, 64.f }, { 64.f, 0.f }, 3, 0.06f);

    m_collider->SetOffSetPos({ 0.f,3.f });
}

Slime::~Slime()
{
}

void Slime::Update()
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

void Slime::Render(HDC _hdc)
{
    BaseEnemy::Render(_hdc);
}

void Slime::EnterCollision(Collider* _other)
{
}

void Slime::StayCollision(Collider* _other)
{
}

void Slime::ExitCollision(Collider* _other)
{
}

void Slime::Attack()
{
    if (m_isShotCharging)
        return;

    Player* player = GetTargetPlayer();
    if (!player || player->GetIsDead())
        return;

    m_isShotCharging = true;
    m_attackShotTimer = 0.f;
}

void Slime::Dead()
{
    BaseEnemy::Dead();
}

void Slime::SpawnBullet()
{
    Vec2 wizardPos = GetPos();

    Vec2 dirs[8] = { {0.f, 1.f}, { 1.f, 0.f}, {-1.f, 0.f}, {0.f, -1.f}, {0.7f,0.7f}, {0.7f, -0.7f}, {-0.7f, 0.7f}, {-0.7f, -0.7f} };

    Vec2 bulletSize = { 20.f, 20.f };

    auto scene = GET_SINGLE(SceneManager)->GetCurScene();

    for (int i = 0; i < 8; ++i)
    {
        float dLen = std::sqrt(dirs[i].x * dirs[i].x + dirs[i].y * dirs[i].y);

        dirs[i].x /= dLen;
        dirs[i].y /= dLen;

        SlimeBullet* bullet = new SlimeBullet;
        bullet->SetPos(wizardPos);
        bullet->SetSize(bulletSize);
        bullet->SetDirection(dirs[i]);
        scene->AddObject(bullet, Layer::ENEMYBULLET);
    }
}
