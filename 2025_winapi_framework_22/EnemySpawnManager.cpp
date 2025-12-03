#include "pch.h"
#include "EnemySpawnManager.h"

#include "SceneManager.h"
#include "Scene.h"
#include "Object.h"
#include "BaseEnemy.h"
#include "Zombie.h"

void EnemySpawnManager::Init()
{
    std::srand((unsigned int)std::time(nullptr));
}

void EnemySpawnManager::Update()
{
    std::shared_ptr<Scene> curScene = GET_SINGLE(SceneManager)->GetCurScene();
    if (!curScene)
        return;

    if (!m_waveActive)
    {
        StartNextWave();
    }
    else
    {
        if (AreAllEnemiesDead())
        {
            m_waveActive = false;
        }
    }
}

void EnemySpawnManager::StartNextWave()
{
    std::shared_ptr<Scene> curScene = GET_SINGLE(SceneManager)->GetCurScene();
    if (!curScene)
        return;

    ++m_currentWave;

    bool isBossWave = (m_currentWave % m_bossWaveInterval == 0);

    SpawnWaveEnemies(curScene.get(), isBossWave);

    m_waveActive = true;
}

bool EnemySpawnManager::AreAllEnemiesDead() const
{
    std::shared_ptr<Scene> curScene = GET_SINGLE(SceneManager)->GetCurScene();
    if (!curScene)
        return true;

    const vector<Object*>& enemyLayer = curScene->GetLayerObjects(Layer::DEFAULTENEMY);

    for (Object* obj : enemyLayer)
    {
        if (!obj)
            continue;

        BaseEnemy* enemy = dynamic_cast<BaseEnemy*>(obj);
        if (!enemy)
            continue;

        if (!enemy->GetIsDead())
            return false;
    }

    return true;
}

Vec2 EnemySpawnManager::GetRandomOffScreenSpawnPos() const
{
    const float margin = 50.f;

    int side = std::rand() % 4;

    float x = 0.f;
    float y = 0.f;

    switch (side)
    {
    case 0:
        x = -margin;
        y = static_cast<float>(std::rand() % WINDOW_HEIGHT);
        break;
    case 1:
        x = WINDOW_WIDTH + margin;
        y = static_cast<float>(std::rand() % WINDOW_HEIGHT);
        break;
    case 2:
        x = static_cast<float>(std::rand() % WINDOW_WIDTH);
        y = -margin;
        break;
    case 3:
    default:
        x = static_cast<float>(std::rand() % WINDOW_WIDTH);
        y = WINDOW_HEIGHT + margin;
        break;
    }

    return Vec2(x, y);
}

void EnemySpawnManager::SpawnWaveEnemies(Scene* scene, bool bossWave)
{
    if (!scene)
        return;

    int baseCount = 3;
    int enemyCount = baseCount + m_currentWave;

    for (int i = 0; i < enemyCount; ++i)
    {
        Vec2 spawnPos = GetRandomOffScreenSpawnPos();
        scene->Spawn<Zombie>(Layer::DEFAULTENEMY, spawnPos, m_enemySpawnSize);
    }

    if (bossWave)
    {
        // 보스 소환
    }
}
