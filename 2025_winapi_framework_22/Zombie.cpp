#include "pch.h"
#include "Zombie.h"
#include "Rigidbody.h"
#include "ResourceManager.h"
#include "GDISelector.h"

Zombie::Zombie()
{
    SetHp(150);
    SetMoveSpeed(80.f);
    SetAttackPower(15);
    SetAttackCooltime(0.8f);
}

Zombie::~Zombie()
{
    BaseEnemy::~BaseEnemy();
}

void Zombie::Update()
{
    BaseEnemy::Update();
    m_position = GetPos();
}

void Zombie::Render(HDC _hdc)
{
    BaseEnemy::Render(_hdc);
}

void Zombie::EnterCollision(Collider* _other)
{

}

void Zombie::StayCollision(Collider* _other)
{

}

void Zombie::ExitCollision(Collider* _other)
{

}
