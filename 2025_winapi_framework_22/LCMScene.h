#pragma once
#include "Scene.h"

class LCMScene : public Scene
{
	// Scene을(를) 통해 상속됨
	void Init() override;
	void Update() override;
};

