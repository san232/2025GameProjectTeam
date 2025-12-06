#pragma once
#include "ISubWindowEffect.h"
#include "Entity.h"
#include <iostream> 
#include <string>

class AttackBuffEffect : public ISubWindowEffect
{
private:
    int m_buffAmount = 10;
    std::wstring m_name = L"Attack Buff";
    COLORREF m_color = RGB(255, 100, 100);

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

    const std::wstring& GetName() const override { return m_name; }
    COLORREF GetColor() const override { return m_color; }
};

class MoveSpeedBuffEffect : public ISubWindowEffect
{
private:
    float m_speedBuffAmount = 100.f;
    std::wstring m_name = L"Speed Buff";
    COLORREF m_color = RGB(100, 100, 255);

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

    const std::wstring& GetName() const override { return m_name; }
    COLORREF GetColor() const override { return m_color; }
};

