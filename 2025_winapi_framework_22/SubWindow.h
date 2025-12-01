#pragma once
#include <windows.h>
#include "ISubWindowEffect.h"

class SubWindowRenderer;

class SubWindow
{
public:
    SubWindow();
    ~SubWindow();

    bool Create(HWND parent, SubWindowRenderer* renderer, int width = 160, int height = 160);
    HWND GetHWnd() const { return m_hWnd; }

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

    void SetTintColor(COLORREF color, float alpha) { m_tintColor = color; m_alpha = alpha; }
    COLORREF GetTintColor() const { return m_tintColor; }
    float GetAlpha() const { return m_alpha; }

    void SetEffect(ISubWindowEffect* effect) { m_effect = effect; }
    ISubWindowEffect* GetEffect() const { return m_effect; }

    bool IsActive() const { return m_isActive; }
    void SetActive(bool active);

    RECT GetRect() const;

    void SetRevealLens(bool reveal) { m_isRevealLens = reveal; }
    bool IsRevealLens() const { return m_isRevealLens; }

private:
    static ATOM RegisterClassOnce(HINSTANCE hInst);
    static SubWindow* GetThis(HWND hWnd);

private:
    HWND m_hWnd;
    SubWindowRenderer* m_renderer;

    COLORREF m_tintColor;
    float m_alpha;
    ISubWindowEffect* m_effect;
    bool m_isActive;
    bool m_isRevealLens;
};