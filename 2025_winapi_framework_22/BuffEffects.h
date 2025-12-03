#pragma once
#include "ISubWindowEffect.h"
#include "Entity.h"
#include <iostream> 

class AttackBuffEffect : public ISubWindowEffect
{
private:
    int m_buffAmount = 10;

public:
    void OnEnter(Entity* entity) override {
        if (entity)
        {
            int currentPower = entity->GetAttackPower();
            entity->SetAttackPower(currentPower + m_buffAmount);
        }
    }
    void OnStay(Entity* entity, float dt) override {
    }
    void OnExit(Entity* entity) override {
        if (entity)
        {
            int currentPower = entity->GetAttackPower();
            entity->SetAttackPower(currentPower - m_buffAmount);
        }
    }
};

class MoveSpeedBuffEffect : public ISubWindowEffect
{
private:
    float m_speedBuffAmount = 100.f;

public:
    void OnEnter(Entity* entity) override {
        if (entity)
        {
            float currentSpeed = entity->GetMoveSpeed();
            entity->SetMoveSpeed(currentSpeed + m_speedBuffAmount);
        }
    }
    void OnStay(Entity* entity, float dt) override {
    }
    void OnExit(Entity* entity) override {
        if (entity)
        {
            float currentSpeed = entity->GetMoveSpeed();
            entity->SetMoveSpeed(currentSpeed - m_speedBuffAmount);
        }
    }
};

