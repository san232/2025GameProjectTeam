#include "pch.h"
#include "SpriteRenderer.h"
#include "WizardBullet.h"
#include "Collider.h"
#include "Player.h"
#include "Texture.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "GDISelector.h"
#include "Defines.h"

#include <cmath>

WizardBullet::WizardBullet()
    : m_direction{ 1.f,0.f }
    , m_lifeTime(0.f)
    , m_maxLifeTime(20.f)
{
    AddComponent<Collider>()->SetSize({10.f,12.f});
    SetHp(1);
    SetMoveSpeed(400.f);
    SetAttackPower(1);
    m_pTex = GET_SINGLE(ResourceManager)->GetTexture(L"WizardBullet");
}

WizardBullet::~WizardBullet()
{

}

void WizardBullet::Update()
{
    Entity::Update();

    Move();

    m_lifeTime += fDT;
    if (m_lifeTime >= m_maxLifeTime)
    {
        Dead();
    }
}

void WizardBullet::Render(HDC _hdc)
{
    Vec2 pos = GetPos();
    Vec2 size = GetSize();
    LONG width = m_pTex->GetWidth();
    LONG height = m_pTex->GetHeight();

    GET_SINGLE(SpriteRenderer)->Draw(m_pTex->GetSRV(), (float)((int)(pos.x - size.x / 2)), (float)((int)(pos.y - size.y / 2)), (float)(size.x), (float)(size.y), (float)(0), (float)(0), (float)(width), (float)(height), (float)(width), (float)(height));

    ComponentRender(_hdc);
}

void WizardBullet::EnterCollision(Collider* _other)
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

void WizardBullet::StayCollision(Collider* _other)
{
}

void WizardBullet::ExitCollision(Collider* _other)
{
}

void WizardBullet::Move()
{
    Translate(m_direction * GetMoveSpeed() * fDT);
}




void WizardBullet::Dead()
{
    GET_SINGLE(SceneManager)->GetCurScene()->RequestDestroy(this);
}
