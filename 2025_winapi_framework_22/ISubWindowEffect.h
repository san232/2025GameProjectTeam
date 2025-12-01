#pragma once
class Entity;

class ISubWindowEffect
{
public:
    virtual ~ISubWindowEffect() {}
    
    virtual void OnEnter(Entity* entity) = 0;
    
    virtual void OnStay(Entity* entity, float deltaTime) = 0;
    
    virtual void OnExit(Entity* entity) = 0;
};
