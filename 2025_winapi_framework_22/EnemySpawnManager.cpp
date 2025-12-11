#include "pch.h"
#include "EnemySpawnManager.h"

#include "GDISelector.h"
#include "SceneManager.h"
#include "Scene.h"
#include "LSScene.h"
#include "Object.h"
#include "BaseEnemy.h"
#include "Zombie.h"
#include "Ghost.h"
#include "Boom.h"
#include "Cat.h"
#include "Snail.h"
#include "Wizard.h"
#include "Conch.h"
#include "Necromancer.h"
#include "Slime.h"
#include "Worm.h"
#include "BossKnight.h"
#include "MirrorBoss.h"

void EnemySpawnManager::Init()
{
	std::srand((unsigned int)std::time(nullptr));
}

void EnemySpawnManager::Update()
{
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

void EnemySpawnManager::Render(HDC _hdc)
{
	auto curScene = GET_SINGLE(SceneManager)->GetCurScene();
	if (m_currentWave <= 0 || dynamic_cast<LSScene*>(curScene.get()) == nullptr)
		return;

	std::wstring text = L"WAVE " + std::to_wstring(m_currentWave);

	int margin = 25;

	SIZE textSize{};
	GetTextExtentPoint32(_hdc, text.c_str(), (int)text.length(), &textSize);

	int paddingX = 20;
	int paddingY = 12;

	int right = WINDOW_WIDTH - margin;
	int top = margin;
	int left = right - textSize.cx - paddingX * 2;
	int bottom = top + textSize.cy + paddingY * 2;

	GDISelector brushSelector(_hdc, BrushType::BROWN);
	GDISelector penSelector(_hdc, PenType::BLACK);

	Rectangle(_hdc, left, top, right, bottom);
	SetBkMode(_hdc, TRANSPARENT); 

	int textX = left + paddingX;
	int textY = top + paddingY;

	GDISelector fontSelector(_hdc, FontType::TITLE);
	TextOut(_hdc, textX, textY, text.c_str(), (int)text.length());
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

bool EnemySpawnManager::AreAllEnemiesDead()
{
	int enemyCnt = 0;
	std::shared_ptr<Scene> curScene = GET_SINGLE(SceneManager)->GetCurScene();
	const vector<Object*>& enemyLayer = curScene->GetLayerObjects(Layer::DEFAULTENEMY);

	for (Object* obj : enemyLayer)
	{
		BaseEnemy* enemy = dynamic_cast<BaseEnemy*>(obj);
		if (!enemy->GetDeadState())
			enemyCnt++;
	}

	return enemyCnt == 0;
}

Vec2 EnemySpawnManager::GetRandomOffScreenSpawnPos()
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

void EnemySpawnManager::SpawnWaveEnemies(Scene* _scene, bool _bossWave)
{
	int baseCount = 3;
	int enemyCount = baseCount + (int)(m_currentWave / 2);

	for (int i = 0; i < enemyCount; ++i)
	{
		Vec2 spawnPos = GetRandomOffScreenSpawnPos();
		int typeCount = 0;

		if(GetCurrentWave() == 1)
			typeCount = 2;
		else
			typeCount = 10;

		int rand = std::rand() % typeCount;

		switch (rand)
		{
		case 0:
			_scene->Spawn<Zombie>(Layer::DEFAULTENEMY, spawnPos, {70.f,70.f});
			break;
		case 1:
			_scene->Spawn<Ghost>(Layer::DEFAULTENEMY, spawnPos, { 70.f,70.f });
			break;
		case 2:
			_scene->Spawn<Boom>(Layer::DEFAULTENEMY, spawnPos, { 70.f,70.f });
			break;
		case 3:
			_scene->Spawn<Cat>(Layer::DEFAULTENEMY, spawnPos, { 70.f,70.f });
			break;
		case 4:
			_scene->Spawn<Snail>(Layer::DEFAULTENEMY, spawnPos, { 100.f,50.f });
			break;
		case 5:
			_scene->Spawn<Wizard>(Layer::DEFAULTENEMY, spawnPos, { 70.f,70.f });
			break;
		case 6:
			_scene->Spawn<Conch>(Layer::DEFAULTENEMY, spawnPos, { 120.f, 120.f });
			break;
		case 7:
			_scene->Spawn<Necromancer>(Layer::DEFAULTENEMY, spawnPos, { 70.f,70.f });
			break;
		case 8:
			_scene->Spawn<Slime>(Layer::DEFAULTENEMY, spawnPos, { 70.f,70.f });
			break;
		case 9:
			_scene->Spawn<Worm>(Layer::DEFAULTENEMY, spawnPos, { 70.f,70.f });
			break;
		}
	}

	if (_bossWave)
	{
		_scene->Spawn<BossKnight>(Layer::DEFAULTENEMY, {0.f,-600.f}, { 100.f,100.f });
	}
}