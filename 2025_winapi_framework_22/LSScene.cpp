#include "pch.h"
#include "LSScene.h"
#include "WindowManager.h"
#include "SubWindowManager.h"
#include "CollisionManager.h"
#include "Player.h"
#include "Zombie.h"

void LSScene::Init()
{
    HWND mainWindowHwnd = GET_SINGLE(WindowManager)->GetMainWindowHandle();

    subWindowManager = new SubWindowManager();
    subWindowManager->Init(mainWindowHwnd, this);

    Spawn<Player>(Layer::PLAYER, { WINDOW_WIDTH / 2, WINDOW_HEIGHT / 4 }, { 80.f, 80.f });
    GET_SINGLE(CollisionManager)->CheckLayer(Layer::DEFAULTENEMY, Layer::DEFAULTENEMY);
    GET_SINGLE(CollisionManager)->CheckLayer(Layer::DEFAULTENEMY, Layer::PLAYERBULLET);
    GET_SINGLE(CollisionManager)->CheckLayer(Layer::PLAYER, Layer::ENEMYBULLET);
}

void LSScene::Update()
{
    Scene::Update();
    
    if (subWindowManager)
    {
        vector<Entity*> entities;
        for (int i = 0; i < (int)Layer::END; ++i)
        {
            const vector<Object*>& objs = GetLayerObjects((Layer)i);
            for (Object* obj : objs)
            {
                Entity* ent = dynamic_cast<Entity*>(obj);
                if (ent)
                    entities.push_back(ent);
            }
        }
        subWindowManager->Update(fDT, entities);
        subWindowManager->Render();
    }
}

LSScene::~LSScene()
{
    SAFE_DELETE(subWindowManager);
}
