#include "pch.h"
#include "FragmentingBullet.h"
#include "FragmentBullet.h"
#include "Collider.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "Texture.h"

FragmentingBullet::FragmentingBullet()
    : m_explodeTime(0.7f)
{
    if (Collider* col = GetComponent<Collider>())
        col->SetSize({ 30.f, 30.f });
    SetHp(1);
    SetMoveSpeed(300.f);
    SetAttackPower(5);
    SetLifeTime(m_explodeTime);
    m_pTex = GET_SINGLE(ResourceManager)->GetTexture(L"MirrorBossBullet");
}

FragmentingBullet::~FragmentingBullet() {}

void FragmentingBullet::Move()
{
    Bullet::Move();
}

void FragmentingBullet::Render(HDC _hdc)
{
    Vec2 pos = GetPos();
    Vec2 size = GetSize();
    if (m_pTex)
    {
        LONG width = m_pTex->GetWidth();
        LONG height = m_pTex->GetHeight();

        ::TransparentBlt(_hdc
            , (int)(pos.x - size.x / 2)
            , (int)(pos.y - size.y / 2)
            , size.x
            , size.y
            , m_pTex->GetTextureDC()
            , 0, 0, width, height,
            RGB(255, 0, 255));
    }
    ComponentRender(_hdc);
}

void FragmentingBullet::Dead()
{
    Explode();
    Bullet::Dead();
}

void FragmentingBullet::Explode()
{
    int fragmentCount = 8;
    for (int i = 0; i < fragmentCount; ++i)
    {
        FragmentBullet* pBullet = new FragmentBullet();
        pBullet->SetPos(GetPos());
        
        float angle = (360.f / fragmentCount) * i;
        Vec2 dir = { cosf(angle * PI / 180.f), sinf(angle * PI / 180.f) };
        
        pBullet->SetDirection(dir);
        GET_SINGLE(SceneManager)->GetCurScene()->AddObject(pBullet, Layer::ENEMYBULLET);
    }
}
