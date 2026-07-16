#include "pch.h"
#include "SpriteRenderer.h"
#include "DeadScene.h"
#include "Texture.h"
#include "Core.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "EnemySpawnManager.h"
#include "ResourceManager.h"
#include "GDISelector.h"

DeadScene::DeadScene()
{
}

DeadScene::~DeadScene()
{
}

void DeadScene::Init()
{
    GET_SINGLE(ResourceManager)->Play(L"GameOver");
    RECT rect;
    GetClientRect(GET_SINGLE(Core)->GetHwnd(), &rect);

    int btnWidth = 200;
    int btnHeight = 60;
    
    int centerX = (rect.right - rect.left) / 2;
    int centerY = (rect.bottom - rect.top) / 2;

    m_btnRetry.left = centerX - (btnWidth / 2);
    m_btnRetry.top = centerY + 50; 
    m_btnRetry.right = centerX + (btnWidth / 2);
    m_btnRetry.bottom = centerY + 50 + btnHeight;

    m_btnExit.left = centerX - (btnWidth / 2);
    m_btnExit.top = centerY + 120;
    m_btnExit.right = centerX + (btnWidth / 2);
    m_btnExit.bottom = centerY + 120 + btnHeight;
}

void DeadScene::Update()
{
    Scene::Update();

    if (GET_KEYDOWN(KEY_TYPE::LBUTTON))
    {
        POINT mousePos = GET_MOUSEPOS;

        if (PtInRect(&m_btnRetry, mousePos))
        {
            GET_SINGLE(EnemySpawnManager)->ResetWave();
            GET_SINGLE(SceneManager)->LoadSceneWithTransition(L"LSScene");
        }

        if (PtInRect(&m_btnExit, mousePos))
        {
            PostQuitMessage(0);
        }
    }
}

void DeadScene::Render(HDC _hdc)
{
    Scene::Render(_hdc);

    RECT rect;
    ::GetClientRect(GET_SINGLE(Core)->GetHwnd(), &rect);

    Texture* pTex = GET_SINGLE(ResourceManager)->GetTexture(L"GameOverScene");
    if (pTex != nullptr)
    {
        int texWidth = pTex->GetWidth();
        int texHeight = pTex->GetHeight();

        GET_SINGLE(SpriteRenderer)->Draw(pTex->GetSRV(), 0, 0, (float)rect.right, (float)rect.bottom, 0, 0, (float)texWidth, (float)texHeight, (float)texWidth, (float)texHeight);
    }

    {
        GET_SINGLE(SpriteRenderer)->DrawFilledRect((float)m_btnRetry.left, (float)m_btnRetry.top, (float)(m_btnRetry.right - m_btnRetry.left), (float)(m_btnRetry.bottom - m_btnRetry.top), XMFLOAT4(0.827f, 0.827f, 0.827f, 1.0f));
        GET_SINGLE(SpriteRenderer)->DrawRect((float)m_btnRetry.left, (float)m_btnRetry.top, (float)(m_btnRetry.right - m_btnRetry.left), (float)(m_btnRetry.bottom - m_btnRetry.top), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
//         DrawText(_hdc, L"RETRY", -1, &m_btnRetry, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }

    {
        GET_SINGLE(SpriteRenderer)->DrawFilledRect((float)m_btnExit.left, (float)m_btnExit.top, (float)(m_btnExit.right - m_btnExit.left), (float)(m_btnExit.bottom - m_btnExit.top), XMFLOAT4(0.502f, 0.502f, 0.502f, 1.0f));
        GET_SINGLE(SpriteRenderer)->DrawRect((float)m_btnExit.left, (float)m_btnExit.top, (float)(m_btnExit.right - m_btnExit.left), (float)(m_btnExit.bottom - m_btnExit.top), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
//         DrawText(_hdc, L"EXIT", -1, &m_btnExit, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }
}

void DeadScene::Release()
{
    GET_SINGLE(ResourceManager)->Stop(SOUND_CHANNEL::BGM);
    Scene::Release();
}
