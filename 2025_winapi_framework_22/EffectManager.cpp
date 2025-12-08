#include "pch.h"
#include "EffectManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Effect.h"
#include "Object.h"

void EffectManager::Init()
{
}

bool EffectManager::RegisterEffect(Effect* _effect)
{
	Scene* curScene = SceneManager::GetInst()->GetCurScene().get();
	if (curScene)
	{
		curScene->RequestSpawn(_effect, Layer::VFX);
		return true;
	}
	
	return false;
}
