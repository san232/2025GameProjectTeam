#pragma once
#include "ISubWindowEffect.h"
#include "Entity.h"
#include <iostream> 

class AttackBuffEffect : public ISubWindowEffect
{
private:
    int m_buffAmount = 10; // 증가시킬 공격력 수치

public:
    void OnEnter(Entity* entity) override {
        if (entity)
        {
            int currentPower = entity->GetAttackPower();
            entity->SetAttackPower(currentPower + m_buffAmount);
            // 디버깅용 출력 (필요시 사용)
            // std::cout << "Attack Buff Applied: " << currentPower << " -> " << entity->GetAttackPower() << std::endl;
        }
    }
    void OnStay(Entity* entity, float dt) override {
        // 지속적인 효과가 필요하다면 여기에 구현 (예: 도트 힐)
    }
    void OnExit(Entity* entity) override {
        if (entity)
        {
            int currentPower = entity->GetAttackPower();
            entity->SetAttackPower(currentPower - m_buffAmount);
            // std::cout << "Attack Buff Removed" << std::endl;
        }
    }
};

class MoveSpeedBuffEffect : public ISubWindowEffect
{
private:
    float m_speedBuffAmount = 100.f; // 증가시킬 이동속도 수치

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

