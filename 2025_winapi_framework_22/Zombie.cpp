// Zombie.cpp
#include "pch.h"
#include "Zombie.h"

#include "Rigidbody.h"
#include "Collider.h"
#include "ResourceManager.h"
#include "GDISelector.h"
#include "Player.h"
#include "Defines.h"

Zombie::Zombie()
{
    SetHp(150);
    SetMoveSpeed(80.f);
    SetAttackPower(15);
    SetAttackCooltime(0.8f);
    SetAttackRange(50.f);
}

Zombie::~Zombie()
{
}

void Zombie::Update()
{
    BaseEnemy::Update();
}

void Zombie::Render(HDC _hdc)
{
    BaseEnemy::Render(_hdc);
    Vec2 pos = GetPos();
    Vec2 size = GetSize();

    BrushType brush = BrushType::RED;
    PenType pen = PenType::GREEN;

    GDISelector brushSelector(_hdc, brush);
    GDISelector penSelector(_hdc, pen);

    RECT_RENDER(_hdc, pos.x, pos.y, size.x, size.y);
}

void Zombie::EnterCollision(Collider* _other)
{
    Object* otherObj = _other->GetOwner();
    if (!otherObj || otherObj->GetIsDead())
        return;

    Player* player = dynamic_cast<Player*>(otherObj);
    if (!player)
        return;

    player->TakeDamage(GetAttackPower());
}

void Zombie::StayCollision(Collider* _other)
{
}

void Zombie::ExitCollision(Collider* _other)
{
}

void Zombie::Attack()
{
}

void Zombie::Dead()
{
    BaseEnemy::Dead();
}
