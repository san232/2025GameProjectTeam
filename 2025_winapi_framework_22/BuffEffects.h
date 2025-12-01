#pragma once
#include "ISubWindowEffect.h"
#include "Entity.h"
#include <iostream> 

class AttackBuffEffect : public ISubWindowEffect
{
public:
    void OnEnter(Entity* entity) override {
    }
    void OnStay(Entity* entity, float dt) override {
    }
    void OnExit(Entity* entity) override {
    }
};

class SlowDebuffEffect : public ISubWindowEffect
{
public:
    void OnEnter(Entity* entity) override {
    }
    void OnStay(Entity* entity, float dt) override {
    }
    void OnExit(Entity* entity) override {
    }
};

