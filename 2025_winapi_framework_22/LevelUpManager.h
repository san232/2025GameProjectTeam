#pragma once
#include "Defines.h"
#include "Enums.h"

class Player;
class Texture;

class LevelUpManager
{
    DECLARE_SINGLE(LevelUpManager);

public:
    void Init();
    void Update();
    void Render(HDC _hdc);

public:
    void LevelUpPlayer(Player* _player);

private:
    void LevelUp(LevelUpType _type);

private:
    Player* m_player = nullptr;
    bool  m_isActive = false;
    float m_elapsed = 0.f;
    float m_appearDuration = 0.3f;
    Texture* m_cardIcons[4] = {};
};
