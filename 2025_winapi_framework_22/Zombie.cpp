#include "pch.h"
#include "Zombie.h"
#include "ResourceManager.h"
#include "Animator.h"
#include "Collider.h"

Zombie::Zombie()
    : MeleeEnemy({ 0.25f, 700.f })
{
    SetHp(30);
    SetMoveSpeed(150.f);
    SetAttackPower(2);
    SetAttackCooltime(0.f);
    SetAttackRange(50.f);
    SetExp(20);
    SetDefaultLookRight(true);
    m_attackDelay = 0.15f;

    SetStatMulti();

    m_pTex = GET_SINGLE(ResourceManager)->GetTexture(L"Zombie");

    m_animator->CreateAnimation(L"Move", m_pTex, { 0.f, 0.f }, { 64.f, 64.f }, { 62.f, 0.f }, 7, 0.1f);
    m_animator->CreateAnimation(L"Dead", m_pTex, { 0.f, 65.f }, { 64.f, 64.f }, { 62.f, 0.f }, 7, 0.1f);
    m_animator->CreateAnimation(L"Attack", m_pTex, { 0.f, 120.f }, { 64.f, 64.f }, { 62.f, 0.f }, 6, 0.1f);
    m_animator->CreateAnimation(L"Idle", m_pTex, { 0.f, 180.f }, { 64.f, 64.f }, { 62.f, 0.f }, 4, 0.1f);
    m_animator->CreateAnimation(L"Hit", m_pTex, { 0.f, 240.f }, { 64.f, 64.f }, { 62.f, 0.f }, 3, 0.1f);

    m_collider->SetSize({ 30.f, 40.f });
}
