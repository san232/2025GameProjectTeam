#pragma once
#include "ISubWindowEffect.h"
#include "Entity.h"
#include <iostream> 
#include <string>

class AttackBuffEffect : public ISubWindowEffect
{
private:
    int m_buffAmount = 2;
    std::wstring m_name = L"Attack Buff";
    COLORREF m_color = RGB(255, 0, 0);

public:
    void OnEnter(Entity* entity) override {
        if (entity)
        {
            int currentPower = entity->GetAttackPower();
            entity->SetAttackPower(currentPower * m_buffAmount);
        }
    }
    void OnStay(Entity* entity, float dt) override {
    }
    void OnExit(Entity* entity) override {
        if (entity)
        {
            int currentPower = entity->GetAttackPower();
            entity->SetAttackPower(currentPower / m_buffAmount);
        }
    }

    const std::wstring& GetName() const override { return m_name; }
    COLORREF GetColor() const override { return m_color; }
};

class AttackSpeedDebuffEffect : public ISubWindowEffect
{
private:
    float m_speedBuffAmount = 1.4f;
    std::wstring m_name = L"Attack Speed DeBuff";
    COLORREF m_color = RGB(255, 0, 255);

public:
    void OnEnter(Entity* entity) override {
        if (entity)
        {
            float currentAtkSpeed = entity->GetAttackCooltime();
            entity->SetAttackCooltime(currentAtkSpeed * m_speedBuffAmount);
        }
    }
    void OnStay(Entity* entity, float dt) override {
    }
    void OnExit(Entity* entity) override {
        if (entity)
        {
            float currentSpeed = entity->GetAttackCooltime();
            entity->SetAttackCooltime(currentSpeed / m_speedBuffAmount);
        }
    }

    const std::wstring& GetName() const override { return m_name; }
    COLORREF GetColor() const override { return m_color; }
};

class TimeSlowEffect : public ISubWindowEffect
{
private:
    float m_slowFactor = 0.5f;
    std::wstring m_name = L"Time Slow";
    COLORREF m_color = RGB(0, 255, 0);

public:
    void OnEnter(Entity* entity) override {
        if (entity)
        {
            float currentSpeed = entity->GetMoveSpeed();
            entity->SetMoveSpeed(currentSpeed * m_slowFactor);
        }
    }
    void OnStay(Entity* entity, float dt) override {
    }
    void OnExit(Entity* entity) override {
        if (entity)
        {
            float currentSpeed = entity->GetMoveSpeed();
            entity->SetMoveSpeed(currentSpeed / m_slowFactor);
        }
    }

    const std::wstring& GetName() const override { return m_name; }
    COLORREF GetColor() const override { return m_color; }
};