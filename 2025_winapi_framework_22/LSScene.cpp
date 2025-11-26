#include "pch.h"
#include "LSScene.h"
#include "BaseEnemy.h"

void LSScene::Init()
{
    Spawn<BaseEnemy>(Layer::DEFAULTENEMY, { WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 }, { 100.f, 100.f });
    Spawn<BaseEnemy>(Layer::INVISIBLEENEMY, { WINDOW_WIDTH / 2 + 150, WINDOW_HEIGHT / 2 }, { 100.f, 100.f });
}

void LSScene::Update()
{

}
