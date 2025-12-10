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

    int  GetCurrentWave() const { return m_currentWave; }
    void ResetWave() { m_currentWave = 0; }

private:
    void StartNextWave();
    bool AreAllEnemiesDead() const;
    Vec2 GetRandomOffScreenSpawnPos() const;
    void SpawnWaveEnemies(Scene* _scene, bool _bossWave);

private:
    int  m_currentWave = 0;
    int  m_bossWaveInterval = 5;
    bool m_waveActive = true;
};
