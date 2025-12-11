#pragma once
#include "Defines.h"
#include "Enums.h"
#include "Vec2.h"

class Scene;

class EnemySpawnManager
{
    DECLARE_SINGLE(EnemySpawnManager);

public:
    void Init();
    void Update();
    void Render(HDC _hdc);

    int  GetCurrentWave() { return m_currentWave; }
    void ResetWave() { m_currentWave = 0; }

private:
    void StartNextWave();
    bool AreAllEnemiesDead();
    Vec2 GetRandomOffScreenSpawnPos();
    void SpawnWaveEnemies(Scene* _scene, bool _bossWave);

private:
    int  m_currentWave = 0;
    int  m_bossWaveInterval = 5;
    bool m_waveActive = true;
};
