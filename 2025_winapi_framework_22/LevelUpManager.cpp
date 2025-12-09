#include "pch.h"
#include "LevelUpManager.h"
#include "Player.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "Scene.h"
#include "GDISelector.h"

void LevelUpManager::Init()
{
    m_player = nullptr;
    m_isActive = false;
    m_elapsed = 0.f;
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

    GDISelector brushSelector(_hdc, BrushType::BROWN);
    GDISelector penSelector(_hdc, PenType::BLACK);

    SetBkMode(_hdc, TRANSPARENT);
    SetTextColor(_hdc, RGB(0, 0, 0));

    for (int i = 0; i < cardCount; ++i)
    {
        float cx = centerX - totalW * 0.5f + panelW * 0.5f + i * (panelW + spacing);
        float cy = centerY + yOffset;

        int left = (int)(cx - panelW * 0.5f);
        int top = (int)(cy - panelH * 0.5f);
        int right = (int)(cx + panelW * 0.5f);
        int bottom = (int)(cy + panelH * 0.5f);

        Rectangle(_hdc, left, top, right, bottom);

        std::wstring title;
        std::wstring desc;

        if (i == 0)
        {
            title = L"체력 완전 회복";
            desc = L"HP를 최대치까지 회복합니다.";
        }
        else if (i == 1)
        {
            title = L"공격력 증가";
            desc = L"공격력이 +2 증가합니다.";
        }
        else if (i == 2)
        {
            title = L"구르기 쿨타임 감소";
            desc = L"구르기 쿨타임이 감소합니다.";
        }
        else if (i == 3)
        {
            title = L"공격 쿨타임 감소";
            desc = L"공격 쿨타임이 감소합니다.";
        }

        int textX = left + 16;
        int titleY = top + 40;
        int descY = top + 80;
        int keyY = bottom - 40;

        TextOut(_hdc, textX, titleY, title.c_str(), (int)title.length());
        TextOut(_hdc, textX, descY, desc.c_str(), (int)desc.length());

        wchar_t keyMsg[16];
        swprintf_s(keyMsg, L"%d번 키", i + 1);
        TextOut(_hdc, textX, keyY, keyMsg, (int)wcslen(keyMsg));
    }

    std::wstring info = L"레벨 업! 원하는 카드를 선택하세요!";
    int infoX = (int)(WINDOW_WIDTH * 0.5f - info.length() * 4);
    int infoY = (int)(centerY - panelH * 0.8f);
    TextOut(_hdc, infoX, infoY, info.c_str(), (int)info.length());
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
        m_player->SetHp(m_player->GetMaxHp());
        break;
    case LevelUpType::DamageUp:
        m_player->SetAttackPower(m_player->GetAttackPower() + 2);
        break;
    case LevelUpType::RollingCoolTimeDown:
        m_player->SetRollingCooltime(m_player->GetRollingCooltime() - 0.1f);
        break;
    case LevelUpType::AttackCoolTimeDown:
        m_player->SetAttackCooltime(m_player->GetAttackCooltime() - 0.1f);
        break;
    }

    GET_SINGLE(SceneManager)->GetCurScene()->SetStopObject(false);

    m_isActive = false;
    m_elapsed = 0.f;
    m_player = nullptr;
}
