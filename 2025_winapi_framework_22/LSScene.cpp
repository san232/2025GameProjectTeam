#include "pch.h"
#include "SpriteRenderer.h"
#include "LSScene.h"
#include "WindowManager.h"
#include "SubWindowManager.h"
#include "CollisionManager.h"
#include "Player.h"
#include "Zombie.h"
#include "BossKnight.h"
#include "Core.h"
#include "ResourceManager.h"
#include "Texture.h"

void LSScene::Init()
{
    HWND mainWindowHwnd = GET_SINGLE(WindowManager)->GetMainWindowHandle();

    subWindowManager = new SubWindowManager();
    subWindowManager->Init(mainWindowHwnd, this);

    Spawn<Player>(Layer::PLAYER, { WINDOW_WIDTH / 2, WINDOW_HEIGHT / 4 }, { 80.f, 80.f });

    GET_SINGLE(CollisionManager)->CheckLayer(Layer::DEFAULTENEMY, Layer::DEFAULTENEMY);
    GET_SINGLE(CollisionManager)->CheckLayer(Layer::PLAYER, Layer::DEFAULTENEMY);
    GET_SINGLE(CollisionManager)->CheckLayer(Layer::DEFAULTENEMY, Layer::PLAYERBULLET);
    GET_SINGLE(CollisionManager)->CheckLayer(Layer::PLAYER, Layer::ENEMYBULLET);

    GET_SINGLE(ResourceManager)->Play(L"BGM");
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
    }
}

void LSScene::Render(HDC _hdc)
{
    RECT rect;
    GetClientRect(GET_SINGLE(Core)->GetHwnd(), &rect);

    Texture* pTex = GET_SINGLE(ResourceManager)->GetTexture(L"Map");
    if (pTex)
    {
        int texWidth = pTex->GetWidth();
        int texHeight = pTex->GetHeight();
        
        GET_SINGLE(SpriteRenderer)->Draw(pTex->GetSRV(), 0, 0, (float)rect.right, (float)rect.bottom, 0, 0, (float)texWidth, (float)texHeight, (float)texWidth, (float)texHeight);
    }

    Scene::Render(_hdc);
}

void LSScene::RenderSubWindows()
{
    if (subWindowManager)
        subWindowManager->Render();
}

LSScene::~LSScene()
{
    SAFE_DELETE(subWindowManager);
}

void LSScene::Release()
{
    GET_SINGLE(ResourceManager)->Stop(SOUND_CHANNEL::BGM);
    Scene::Release();
    SAFE_DELETE(subWindowManager);
}
