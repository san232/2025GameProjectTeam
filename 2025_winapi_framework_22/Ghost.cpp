#include "pch.h"
#include "Ghost.h"
#include "ResourceManager.h"
#include "Animator.h"
#include "Collider.h"

Ghost::Ghost()
    : MeleeEnemy({ 0.25f, 700.f })
{
    SetHp(15);
    SetMoveSpeed(160.f);
    SetAttackPower(1);
    SetAttackCooltime(0.8f);
    SetAttackRange(50.f);
    SetExp(20);
    SetDefaultLookRight(true);
    m_attackDelay = 0.05f;

    SetStatMulti();

    m_pTex = GET_SINGLE(ResourceManager)->GetTexture(L"Ghost");

    m_animator->CreateAnimation(L"Idle", m_pTex, { 0.f, 0.f }, { 64.f, 64.f }, { 64.f, 0.f }, 5, 0.12f);
    m_animator->CreateAnimation(L"Move", m_pTex, { 0.f, 50.f }, { 64.f, 64.f }, { 64.f, 0.f }, 4, 0.08f);
    m_animator->CreateAnimation(L"Hit", m_pTex, { 0.f, 100.f }, { 64.f, 64.f }, { 64.f, 0.f }, 3, 0.06f);
    m_animator->CreateAnimation(L"Attack", m_pTex, { 0.f, 155.f }, { 64.f, 64.f }, { 64.f, 0.f }, 8, 0.06f);
    m_animator->CreateAnimation(L"Dead", m_pTex, { 0.f, 210.f }, { 64.f, 64.f }, { 64.f, 0.f }, 9, 0.08f);

    m_collider->SetSize({ 35.f, 35.f });
}
