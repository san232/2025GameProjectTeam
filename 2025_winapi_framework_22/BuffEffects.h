#pragma once
#include "ISubWindowEffect.h"
#include "Entity.h"
#include <iostream> 
#include <string>
#include <unordered_map>

class AttackBuffEffect : public ISubWindowEffect
{
private:
    int m_buffAmount = 2;
    std::wstring m_name = L"Attack Buff";
    COLORREF m_color = RGB(255, 0, 0);
    std::unordered_map<Entity*, int> m_originalPowers;

public:
    void OnEnter(Entity* entity) override {
        if (entity)
        {
            int currentPower = entity->GetAttackPower();
            m_originalPowers[entity] = currentPower;
            entity->SetAttackPower(currentPower * m_buffAmount);
        }
    }
    void OnStay(Entity* entity, float dt) override {
    }
    void OnExit(Entity* entity) override {
        if (entity && m_originalPowers.find(entity) != m_originalPowers.end())
        {
            entity->SetAttackPower(m_originalPowers[entity]);
            m_originalPowers.erase(entity);
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
    std::unordered_map<Entity*, float> m_originalCooltimes;

public:
    void OnEnter(Entity* entity) override {
        if (entity)
        {
            float currentAtkSpeed = entity->GetAttackCooltime();
            m_originalCooltimes[entity] = currentAtkSpeed;
            entity->SetAttackCooltime(currentAtkSpeed * m_speedBuffAmount);
        }
    }
    void OnStay(Entity* entity, float dt) override {
    }
    void OnExit(Entity* entity) override {
        if (entity && m_originalCooltimes.find(entity) != m_originalCooltimes.end())
        {
            entity->SetAttackCooltime(m_originalCooltimes[entity]);
            m_originalCooltimes.erase(entity);
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
    std::unordered_map<Entity*, float> m_originalSpeeds;

public:
    void OnEnter(Entity* entity) override {
        if (entity)
        {
            float currentSpeed = entity->GetMoveSpeed();
            m_originalSpeeds[entity] = currentSpeed;
            entity->SetMoveSpeed(currentSpeed * m_slowFactor);
        }
    }
    void OnStay(Entity* entity, float dt) override {
    }
    void OnExit(Entity* entity) override {
        if (entity && m_originalSpeeds.find(entity) != m_originalSpeeds.end())
        {
            entity->SetMoveSpeed(m_originalSpeeds[entity]);
            m_originalSpeeds.erase(entity);
        }
    }

    const std::wstring& GetName() const override { return m_name; }
    COLORREF GetColor() const override { return m_color; }
};