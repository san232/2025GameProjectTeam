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

    void SetBossWaveInterval(int interval)
    {
        if (interval <= 0)
            interval = 1;
        m_bossWaveInterval = interval;
    }

    int  GetBossWaveInterval() const { return m_bossWaveInterval; }
    int  GetCurrentWave() const { return m_currentWave; }

    void SetEnemySpawnSize(const Vec2& size) { m_enemySpawnSize = size; }
    const Vec2& GetEnemySpawnSize() const { return m_enemySpawnSize; }

private:
    void StartNextWave();
    bool AreAllEnemiesDead() const;
    Vec2 GetRandomOffScreenSpawnPos() const;
    void SpawnWaveEnemies(Scene* scene, bool bossWave);

private:
    int  m_currentWave = 0;
    int  m_bossWaveInterval = 5;
    bool m_waveActive = false;

    Vec2 m_enemySpawnSize = Vec2(50.f, 50.f);
};
