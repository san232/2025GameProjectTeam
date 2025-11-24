#include "pch.h"
#include "LCMScene.h"
#include "Player.h"

void LCMScene::Init()
{
	Spawn<Player>(Layer::PLAYER, { WINDOW_WIDTH / 2, WINDOW_HEIGHT / 4 }, { 100.f,100.f });
}

void LCMScene::Update()
{

}
