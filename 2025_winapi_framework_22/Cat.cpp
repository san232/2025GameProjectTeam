#include "pch.h"
#include "Cat.h"
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

Cat::Cat() 
    : ProjectileEnemy({ 0.3f, 0.5f })
{
    SetHp(5);
    SetMoveSpeed(70.f);
    SetAttackCooltime(0.6f);
    SetAttackRange(300.f);
    SetExp(15);
    SetAttackDelay(0.f);
    SetDefaultLookRight(false);

    SetStatMulti();

    m_pTex = GET_SINGLE(ResourceManager)->GetTexture(L"Cat");

    m_animator->CreateAnimation(L"Idle", m_pTex, { 0.f, 0.f }, { 64.f, 64.f }, { 64.f, 0.f }, 5, 0.12f);
    m_animator->CreateAnimation(L"Move", m_pTex, { 0.f, 60.f }, { 64.f, 64.f }, { 64.f, 0.f }, 3, 0.08f);
    m_animator->CreateAnimation(L"Hit", m_pTex, { 0.f, 115.f }, { 64.f, 64.f }, { 64.f, 0.f }, 2, 0.06f);
    m_animator->CreateAnimation(L"Attack", m_pTex, { 0.f, 170.f }, { 64.f, 64.f }, { 64.f, 0.f }, 7, 0.06f);
    m_animator->CreateAnimation(L"Dead", m_pTex, { 0.f, 225.f }, { 64.f, 64.f }, { 64.f, 0.f }, 8, 0.08f);

    m_collider->SetSize({ 40.f, 37.f });
    m_collider->SetOffSetPos({ 0.f, -8.f });
}

void Cat::SpawnProjectiles(Scene* scene, Player* target, const Vec2& origin, const Vec2& dir)
{
    if (!scene || !target || target->GetIsDead())
        return;

    auto* bullet = new CatBullet;
    bullet->SetPos(origin);
    bullet->SetSize({ 20.f, 20.f });
    bullet->SetDirection(dir);
    bullet->SetTarget(target);

    scene->AddObject(bullet, Layer::ENEMYBULLET);
}