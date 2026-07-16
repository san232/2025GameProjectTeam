#include "pch.h"
#include "SubWindowRenderer.h"
#include "SubWindow.h"
#include "Scene.h"
#include "Core.h"
#include "SpriteRenderer.h"

SubWindowRenderer::SubWindowRenderer(HWND inMainWindow, Scene* inScene)
    : mainWindow(inMainWindow)
    , scene(inScene)
{
}

SubWindowRenderer::~SubWindowRenderer()
{
}

void SubWindowRenderer::Render(SubWindow* subWin)
{
    if (!subWin || !subWin->IsActive()) return;

    auto context = GET_SINGLE(Core)->GetContext();
    auto rtv = subWin->GetRTV();
    auto swapChain = subWin->GetSwapChain();
    if (!rtv || !swapChain) return;

    POINT mainPtTL = { 0, 0 };
    ::ClientToScreen(mainWindow, &mainPtTL);
    RECT mainRect;
    ::GetClientRect(mainWindow, &mainRect);
    int mainW = mainRect.right - mainRect.left;
    int mainH = mainRect.bottom - mainRect.top;

    POINT subPtTL = { 0, 0 };
    ::ClientToScreen(subWin->GetHWnd(), &subPtTL);
    RECT subClient;
    ::GetClientRect(subWin->GetHWnd(), &subClient);
    int w = subClient.right - subClient.left;
    int h = subClient.bottom - subClient.top;
    if (mainW <= 0 || mainH <= 0 || w <= 0 || h <= 0)
        return;

    ComPtr<ID3D11Texture2D> mainBackBuffer;
    GET_SINGLE(Core)->GetSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&mainBackBuffer);
    
    ComPtr<ID3D11Texture2D> subBackBuffer;
    swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&subBackBuffer);

    int srcX = subPtTL.x - mainPtTL.x;
    int srcY = subPtTL.y - mainPtTL.y;

    float clearColor[4] = { 1, 1, 1, 1 };
    context->ClearRenderTargetView(rtv, clearColor);

    if (!subWin->IsRevealLens())
    {
        int copyLeft = max(0, srcX);
        int copyTop = max(0, srcY);
        int copyRight = min(mainW, srcX + w);
        int copyBottom = min(mainH, srcY + h);

        if (copyRight > copyLeft && copyBottom > copyTop)
        {
            UINT dstX = (UINT)(copyLeft - srcX);
            UINT dstY = (UINT)(copyTop - srcY);

            D3D11_BOX srcBox = {};
            srcBox.left = (UINT)copyLeft;
            srcBox.top = (UINT)copyTop;
            srcBox.right = (UINT)copyRight;
            srcBox.bottom = (UINT)copyBottom;
            srcBox.front = 0;
            srcBox.back = 1;

            context->CopySubresourceRegion(subBackBuffer.Get(), 0, dstX, dstY, 0, mainBackBuffer.Get(), 0, &srcBox);
        }
    }

    context->OMSetRenderTargets(1, &rtv, nullptr);

    // Setup viewport for subwindow before building the 2D projection.
    D3D11_VIEWPORT vp = {};
    vp.Width = (FLOAT)w;
    vp.Height = (FLOAT)h;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    context->RSSetViewports(1, &vp);

    GET_SINGLE(SpriteRenderer)->Begin((float)w, (float)h);
    COLORREF c = subWin->GetTintColor();
    float r = GetRValue(c) / 255.0f;
    float g = GetGValue(c) / 255.0f;
    float b = GetBValue(c) / 255.0f;
    float a = subWin->GetAlpha();

    GET_SINGLE(SpriteRenderer)->DrawFilledRect(0, 0, (float)w, (float)h, XMFLOAT4(r, g, b, a));
    GET_SINGLE(SpriteRenderer)->End();

    swapChain->Present(1, 0);

    // Restore main render state because SpriteRenderer uses shared DX11 state.
    auto mainRtv = GET_SINGLE(Core)->GetRenderTargetView();
    context->OMSetRenderTargets(1, &mainRtv, nullptr);
    
    D3D11_VIEWPORT mainVp = {};
    mainVp.Width = (FLOAT)mainW;
    mainVp.Height = (FLOAT)mainH;
    mainVp.MinDepth = 0.0f;
    mainVp.MaxDepth = 1.0f;
    context->RSSetViewports(1, &mainVp);

    GET_SINGLE(SpriteRenderer)->Begin((float)mainW, (float)mainH);
}
