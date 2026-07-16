#include "pch.h"
#include "SpriteRenderer.h"
#include "LevelUpManager.h"
#include "Player.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "Scene.h"
#include "GDISelector.h"
#include "ResourceManager.h"
#include "Texture.h"

void LevelUpManager::Init()
{
    m_player = nullptr;
    m_isActive = false;
    m_elapsed = 0.f;

    m_cardIcons[0] = GET_SINGLE(ResourceManager)->GetTexture(L"HpUpIcon");
    m_cardIcons[1] = GET_SINGLE(ResourceManager)->GetTexture(L"AttackPowerUpIcon");
    m_cardIcons[2] = GET_SINGLE(ResourceManager)->GetTexture(L"RollingCooltimeDownIcon");
    m_cardIcons[3] = GET_SINGLE(ResourceManager)->GetTexture(L"AttackSpeedUpIcon");
}

void LevelUpManager::Update()
{
    if (!m_isActive)
        return;

    m_elapsed += fDT;

    if (GET_KEYDOWN(KEY_TYPE::NUM_1))
        LevelUp(LevelUpType::FullHp);
    else if (GET_KEYDOWN(KEY_TYPE::NUM_2))
        LevelUp(LevelUpType::DamageUp);
    else if (GET_KEYDOWN(KEY_TYPE::NUM_3))
        LevelUp(LevelUpType::RollingCoolTimeDown);
    else if (GET_KEYDOWN(KEY_TYPE::NUM_4))
        LevelUp(LevelUpType::AttackCoolTimeDown);
}

void LevelUpManager::Render(HDC _hdc)
{
    if (!m_isActive)
        return;

    const int   cardCount = 4;
    const float panelW = 180.f;
    const float panelH = 260.f;
    const float spacing = 20.f;

    float totalW = panelW * cardCount + spacing * (cardCount - 1);
    float centerX = WINDOW_WIDTH * 0.5f;
    float centerY = WINDOW_HEIGHT * 0.5f;

    float t = m_elapsed / m_appearDuration;
    if (t < 0.f) t = 0.f;
    if (t > 1.f) t = 1.f;
    float yOffset = (1.f - t) * 120.f;

    float uiW = totalW + 80.f;
    float uiH = panelH + 100.f;

    float uiCx = centerX;
    float uiCy = centerY + yOffset;

    int uiLeft = (int)(uiCx - uiW * 0.5f);
    int uiTop = (int)(uiCy - uiH * 0.5f);
    int uiRight = (int)(uiCx + uiW * 0.5f);
    int uiBottom = (int)(uiCy + uiH * 0.5f);

    GET_SINGLE(SpriteRenderer)->DrawFilledRect((float)uiLeft, (float)uiTop, (float)(uiRight - uiLeft), (float)(uiBottom - uiTop), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
    GET_SINGLE(SpriteRenderer)->DrawRect((float)uiLeft, (float)uiTop, (float)(uiRight - uiLeft), (float)(uiBottom - uiTop), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

    std::wstring info = L" ! ϴ ī带 ϼ!";
    RECT infoRc;
    infoRc.left = uiLeft;
    infoRc.right = uiRight;
    infoRc.top = uiTop + 10;
    infoRc.bottom = uiTop + 40;

    // Text rendering is disabled until the DX11 font path is implemented.

    for (int i = 0; i < cardCount; ++i)
    {
        float cx = centerX - totalW * 0.5f + panelW * 0.5f + i * (panelW + spacing);
        float cy = centerY + yOffset;

        int left = (int)(cx - panelW * 0.5f);
        int top = (int)(cy - panelH * 0.5f);
        int right = (int)(cx + panelW * 0.5f);
        int bottom = (int)(cy + panelH * 0.5f);

        GET_SINGLE(SpriteRenderer)->DrawRect((float)left, (float)top, (float)(right - left), (float)(bottom - top), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

        Texture* iconTex = m_cardIcons[i];
        if (iconTex)
        {
            int iconDestW = 64;
            int iconDestH = 64;

            int iconDx = (left + right) / 2 - iconDestW / 2;
            int iconDy = top + 20;

            int srcW = (int)iconTex->GetWidth();
            int srcH = (int)iconTex->GetHeight();

            

            GET_SINGLE(SpriteRenderer)->Draw(iconTex->GetSRV(),
                (float)iconDx, (float)iconDy, (float)iconDestW, (float)iconDestH,
                0.0f, 0.0f, (float)srcW, (float)srcH,
                (float)srcW, (float)srcH);
        }

        std::wstring title;
        std::wstring desc;

        if (i == 0)
        {
            title = L"ü  ȸ";
            desc = L"ִ ü \nü Ǯȸ";
        }
        else if (i == 1)
        {
            title = L"ݷ ";
            desc = L"ݷ մϴ.";
        }
        else if (i == 2)
        {
            title = L" Ÿ ";
            desc = L" Ÿ\nմϴ.";
        }
        else if (i == 3)
        {
            title = L" Ÿ ";
            desc = L" Ÿ\nմϴ.";
        }

        RECT titleRc;
        titleRc.left = left + 10;
        titleRc.right = right - 10;
        titleRc.top = top + 20 + 70;
        titleRc.bottom = top + 20 + 110;

        // Text rendering is disabled until the DX11 font path is implemented.

        RECT descRc;
        descRc.left = left + 14;
        descRc.right = right - 14;
        descRc.top = top + 20 + 120;
        descRc.bottom = bottom - 60;

        // Text rendering is disabled until the DX11 font path is implemented.

        wchar_t keyMsg[16];
        swprintf_s(keyMsg, L"%d Ű", i + 1);

        RECT keyRc;
        keyRc.left = left;
        keyRc.right = right;
        keyRc.top = bottom - 40;
        keyRc.bottom = bottom - 10;

        // Text rendering is disabled until the DX11 font path is implemented.
    }
}

void LevelUpManager::LevelUpPlayer(Player* _player)
{
    m_player = _player;
    m_isActive = true;
    m_elapsed = 0.f;

    GET_SINGLE(SceneManager)->GetCurScene()->SetStopObject(true);
}

void LevelUpManager::LevelUp(LevelUpType _type)
{
    if (!m_player)
        return;

    switch (_type)
    {
    case LevelUpType::FullHp:
        m_player->SetMaxHp(m_player->GetMaxHp() + 3);
        m_player->SetHp(m_player->GetMaxHp());
        break;
    case LevelUpType::DamageUp:
        m_player->SetAttackPower(m_player->GetAttackPower() + 3);
        break;
    case LevelUpType::RollingCoolTimeDown:
        m_player->SetRollingCooltime(m_player->GetRollingCooltime() - 0.1f);
        break;
    case LevelUpType::AttackCoolTimeDown:
        m_player->SetAttackCooltime(m_player->GetAttackCooltime() - 0.05f);
        break;
    }

    GET_SINGLE(SceneManager)->GetCurScene()->SetStopObject(false);

    m_isActive = false;
    m_elapsed = 0.f;
    m_player = nullptr;
}
