#include "pch.h"
#include "Boom.h"
#include "ResourceManager.h"
#include "Animator.h"

Boom::Boom()
    : MeleeEnemy({ 0.25f, 700.f })
{
    SetHp(15);
    SetMoveSpeed(350.f);
    SetAttackPower(1);
    SetAttackCooltime(0.5f);
    SetAttackRange(50.f);
    SetExp(20);
    SetDefaultLookRight(false);
    m_attackDelay = 0.f;

    SetStatMulti();

    m_pTex = GET_SINGLE(ResourceManager)->GetTexture(L"BoomEnemy");

    m_animator->CreateAnimation(L"Idle", m_pTex, { 0.f, 274.f }, { 64.f, 64.f }, { 64.f, 0.f }, 5, 0.12f);
    m_animator->CreateAnimation(L"Move", m_pTex, { 0.f, 82.f }, { 64.f, 64.f }, { 64.f, 0.f }, 4, 0.08f);
    m_animator->CreateAnimation(L"Attack", m_pTex, { 0.f, 210.f }, { 64.f, 64.f }, { 64.f, 0.f }, 8, 0.06f);
    m_animator->CreateAnimation(L"Hit", m_pTex, { 0.f, 146.f }, { 64.f, 64.f }, { 64.f, 0.f }, 3, 0.06f);
    m_animator->CreateAnimation(L"Dead", m_pTex, { 0.f, 18.f }, { 64.f, 64.f }, { 64.f, 0.f }, 7, 0.08f);
}
