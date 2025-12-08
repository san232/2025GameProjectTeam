#include "pch.h"
#include "DeadScene.h"
#include "Core.h"
#include "InputManager.h"
#include "SceneManager.h"
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
}

void DeadScene::Update()
{
    Scene::Update();

    if (GET_KEYDOWN(KEY_TYPE::LBUTTON))
    {
        POINT mousePos = GET_MOUSEPOS;

        if (PtInRect(&m_btnRetry, mousePos))
        {
            GET_SINGLE(SceneManager)->LoadScene(L"LSScene");
        }
    }
}

void DeadScene::Render(HDC _hdc)
{
    Scene::Render(_hdc);

    RECT rect;
    GetClientRect(GET_SINGLE(Core)->GetHwnd(), &rect);

    {
        GDISelector fontSel(_hdc, FontType::TITLE);
        
        RECT textRect = rect;
        textRect.bottom = rect.bottom / 2;

        SetTextColor(_hdc, RGB(255, 0, 0));
        DrawText(_hdc, L"GAME OVER", -1, &textRect, DT_CENTER | DT_BOTTOM | DT_SINGLELINE);
    }

    {
        GDISelector brushSel(_hdc, BrushType::LIGHTGRAY);
        GDISelector penSel(_hdc, PenType::BLACK);

        Rectangle(_hdc, m_btnRetry.left, m_btnRetry.top, m_btnRetry.right, m_btnRetry.bottom);
        
        SetTextColor(_hdc, RGB(0, 0, 0));
        DrawText(_hdc, L"RETRY", -1, &m_btnRetry, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }
}

void DeadScene::Release()
{
    Scene::Release();
}
