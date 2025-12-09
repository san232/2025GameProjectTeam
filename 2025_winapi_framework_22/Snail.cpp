#include "pch.h"
#include "Snail.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "ResourceManager.h"
#include "Animator.h"
#include "Player.h"
#include "SceneManager.h"
#include "Scene.h"
#include "CatBullet.h"
#include "Defines.h"

#include <cmath>

Snail::Snail()
{
    SetHp(5);
    SetMoveSpeed(70.f);
    SetAttackCooltime(1.2f);
    SetAttackRange(200.f);
    SetExp(15);
    SetAttackDelay(0.f);
    SetDefaultLookRight(false);

    SetStatMulti();

    m_pTex = GET_SINGLE(ResourceManager)->GetTexture(L"Snail");

    m_attackShotDelay = 0.3f;
    m_attackShotTimer = 0.f;
    m_isShotCharging = false;

    m_animator->CreateAnimation(L"Idle", m_pTex, { 0.f, 0.f }, { 64.f, 30.f }, { 64.f,0.f }, 2, 0.12f);
    m_animator->CreateAnimation(L"Move", m_pTex, { 0.f,  35.f }, { 64.f, 30.f }, { 64.f,0.f }, 4, 0.08f);
    m_animator->CreateAnimation(L"Attack", m_pTex, { 0.f, 60.f }, { 64.f, 30.f }, { 64.f,0.f }, 7, 0.06f);
    m_animator->CreateAnimation(L"Hit", m_pTex, { 0.f, 90.f }, { 64.f, 30.f }, { 64.f,  0.f }, 2, 0.06f);
    m_animator->CreateAnimation(L"Dead", m_pTex, { 0.f, 120.f }, { 64.f, 30.f }, { 64.f, 0.f }, 10, 0.08f);
}

Snail::~Snail()
{
}

void Snail::Update()
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

void Snail::Render(HDC _hdc)
{
    BaseEnemy::Render(_hdc);
}

void Snail::EnterCollision(Collider* _other)
{
}

void Snail::StayCollision(Collider* _other)
{
}

void Snail::ExitCollision(Collider* _other)
{
}

void Snail::Attack()
{
    Player* player = GetTargetPlayer();
    if (!player || player->GetIsDead())
        return;

    m_isShotCharging = true;
    m_attackShotTimer = 0.f;
}

void Snail::Dead()
{
    BaseEnemy::Dead();
}

void Snail::SpawnBullet()
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

    CatBullet* bullet = new CatBullet;
    bullet->SetPos(catPos);
    bullet->SetSize(bulletSize);
    bullet->SetDirection(dir);
    bullet->SetTarget(player);

    curScene->AddObject(bullet, Layer::ENEMYBULLET);
}
