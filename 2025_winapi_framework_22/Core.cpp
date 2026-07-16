#include "pch.h"
#include "Core.h"
#include "TimeManager.h"
#include "WindowManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "CollisionManager.h"
#include "EnemySpawnManager.h"
#include "EffectManager.h"
#include "LevelUpManager.h"
#include "SpriteRenderer.h"

bool Core::Init(HWND _hWnd)
{
    m_hWnd = _hWnd;
    
    DXGI_SWAP_CHAIN_DESC scd = {};
    scd.BufferCount = 1;
    scd.BufferDesc.Width = WINDOW_WIDTH;
    scd.BufferDesc.Height = WINDOW_HEIGHT;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferDesc.RefreshRate.Numerator = 60;
    scd.BufferDesc.RefreshRate.Denominator = 1;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = m_hWnd;
    scd.SampleDesc.Count = 1;
    scd.SampleDesc.Quality = 0;
    scd.Windowed = TRUE;

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[1] = { D3D_FEATURE_LEVEL_11_0 };
    HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 1, D3D11_SDK_VERSION, &scd, m_swapChain.GetAddressOf(), m_device.GetAddressOf(), &featureLevel, m_context.GetAddressOf());
    if (FAILED(hr)) return false;

    ComPtr<ID3D11Texture2D> backBuffer;
    m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)backBuffer.GetAddressOf());
    m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, m_rtv.GetAddressOf());

    D3D11_VIEWPORT vp = {};
    vp.Width = (FLOAT)WINDOW_WIDTH;
    vp.Height = (FLOAT)WINDOW_HEIGHT;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    m_context->RSSetViewports(1, &vp);

    // 1
    GET_SINGLE(TimeManager)->Init();
    // 2
    GET_SINGLE(InputManager)->Init();
    // 3
	GET_SINGLE(WindowManager)->Init(_hWnd);
 
    if (!GET_SINGLE(SpriteRenderer)->Init())
        return false;

    if (!GET_SINGLE(ResourceManager)->Init())
        return false;
    GET_SINGLE(SceneManager)->Init();
    GET_SINGLE(EnemySpawnManager)->Init();
    GET_SINGLE(EffectManager)->Init();
    GET_SINGLE(LevelUpManager)->Init();

    return true;
}

void Core::MainUpdate()
{
    GET_SINGLE(TimeManager)->Update();
    {
        static float accmulator = 0.f;
        const float fixedDT = 1.f / 60.f;
        accmulator += fDT;
        while (accmulator >= fixedDT)
        {
            GET_SINGLE(SceneManager)->FixedUpdate(fixedDT);
            GET_SINGLE(CollisionManager)->Update();
            accmulator -= fixedDT;
        }
    }
    GET_SINGLE(InputManager)->Update();
    GET_SINGLE(ResourceManager)->FmodUpdate();
    GET_SINGLE(SceneManager)->Update();
    GET_SINGLE(EnemySpawnManager)->Update();
    GET_SINGLE(LevelUpManager)->Update();

    //Vec2 pos = m_obj.GetPos();
    //
    ////if (GetAsyncKeyState(VK_LEFT) & 0x8000)
    ////if(GET_SINGLE(InputManager)->IsPress(KEY_TYPE::LEFT))
    //if(GET_KEY(KEY_TYPE::LEFT))
    //    pos.x -= 200.f * fDT;
    ////if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
    ////if (GET_SINGLE(InputManager)->IsUp(KEY_TYPE::RIGHT))
    //if(GET_KEYUP(KEY_TYPE::RIGHT))
    //    pos.x += 200.f;// *fDT;
    //if(GET_KEYWIDTH(KEY_TYPE::CTRL, KEY_TYPE::SPACE))
    //    pos.y += 200.f * fDT;// *fDT;
    //m_obj.SetPos(pos);

}


void Core::MainRender()
{
    RECT rc;
    ::GetClientRect(m_hWnd, &rc);
    float width = (float)(rc.right - rc.left);
    float height = (float)(rc.bottom - rc.top);
    if (width <= 0.0f) width = WINDOW_WIDTH;
    if (height <= 0.0f) height = WINDOW_HEIGHT;

    D3D11_VIEWPORT vp = {};
    vp.Width = width;
    vp.Height = height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    m_context->RSSetViewports(1, &vp);

    // Set Main Render Target
    m_context->OMSetRenderTargets(1, m_rtv.GetAddressOf(), nullptr);

    float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; // White background
    m_context->ClearRenderTargetView(m_rtv.Get(), clearColor);

    GET_SINGLE(SpriteRenderer)->Begin(width, height);

    // 2. draw
    GET_SINGLE(SceneManager)->Render((HDC)m_context.Get()); // Temporary cast or update interfaces later
    GET_SINGLE(EnemySpawnManager)->Render((HDC)m_context.Get());
    GET_SINGLE(LevelUpManager)->Render((HDC)m_context.Get());
    GET_SINGLE(SceneManager)->RenderSubWindows();

    GET_SINGLE(SpriteRenderer)->End();

    // 3. display
    m_swapChain->Present(1, 0);
}


void Core::GameLoop()
{
    //INT A = 0;
    //static int cnt = 0;
    //++cnt;
    //static int prev = GetTickCount64();
    //int cur = GetTickCount64();
    //if (cur - prev >= 1000)
    //{
    //    prev = cur;
    //    cnt = 0;
    //}

    MainUpdate();
    MainRender();
    GET_SINGLE(SceneManager)->GetCurScene()->FlushEvent();
 }

void Core::CleanUp()
{
    GET_SINGLE(SpriteRenderer)->CleanUp();
    m_rtv.Reset();
    m_swapChain.Reset();
    m_context.Reset();
    m_device.Reset();
    GET_SINGLE(ResourceManager)->Release();
}


