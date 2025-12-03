// Zombie.h
#pragma once
#include "BaseEnemy.h"

class Zombie : public BaseEnemy
{
public:
    Zombie();
    ~Zombie();

public:
    void Update() override;
    void Render(HDC _hdc) override;
    void EnterCollision(Collider* _other) override;
    void StayCollision(Collider* _other) override;
    void ExitCollision(Collider* _other) override;

protected:
    void Attack() override;
    void Dead() override;
};
