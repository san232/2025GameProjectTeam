#include "pch.h"
#include "SubWindow.h"
#include "SubWindowRenderer.h"
#include "Core.h"
#include "InputManager.h"
#include "ISubWindowEffect.h"

static std::unordered_map<HWND, SubWindow*> g_subWindows;
static const wchar_t* kSubWindowClass = L"SubWindowClass";

SubWindow::SubWindow()
    : m_hWnd(nullptr)
    , m_renderer(nullptr)
    , m_tintColor(RGB(255, 255, 255))
    , m_alpha(0.3f)
    , m_effect(nullptr)
    , m_isActive(true)
    , m_isRevealLens(false)
{
}

SubWindow::~SubWindow()
{
    if (m_hWnd != nullptr)
    {
        g_subWindows.erase(m_hWnd);
        ::DestroyWindow(m_hWnd);
        m_hWnd = nullptr;
    }
}

void SubWindow::SetEffect(ISubWindowEffect* effect)
{
    m_effect = effect;
    if (m_hWnd && m_effect)
    {
        ::SetWindowText(m_hWnd, m_effect->GetName().c_str());
        SetTintColor(m_effect->GetColor(), m_alpha);
        ::InvalidateRect(m_hWnd, nullptr, FALSE);
    }
}

ATOM SubWindow::RegisterClassOnce(HINSTANCE hInst)
{
    static ATOM s_atom = 0;
    if (s_atom != 0)
        return s_atom;

    WNDCLASSEXW wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEXW);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = SubWindow::WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInst;
    wcex.hIcon = nullptr;
    wcex.hCursor = ::LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = kSubWindowClass;
    wcex.hIconSm = nullptr;

    s_atom = ::RegisterClassExW(&wcex);
    return s_atom;
}

bool SubWindow::Create(HWND parent, SubWindowRenderer* renderer, int width, int height)
{
    m_renderer = renderer;

    HINSTANCE hInst = (HINSTANCE)::GetModuleHandle(nullptr);
    if (RegisterClassOnce(hInst) == 0)
        return false;

    const DWORD exStyle = WS_EX_WINDOWEDGE;
    const DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_VISIBLE;

    RECT windowRect = { 0, 0, width, height };
    ::AdjustWindowRectEx(&windowRect, style, FALSE, exStyle);
    int windowWidth = windowRect.right - windowRect.left;
    int windowHeight = windowRect.bottom - windowRect.top;

    m_hWnd = ::CreateWindowExW(
        exStyle,
        kSubWindowClass,
        L"SubWindow",
        style,
        CW_USEDEFAULT, CW_USEDEFAULT, windowWidth, windowHeight,
        parent, nullptr, hInst, nullptr);

    if (m_hWnd == nullptr)
        return false;

    g_subWindows[m_hWnd] = this;
    ::SetWindowLongPtr(m_hWnd, GWLP_USERDATA, (LONG_PTR)this);

    RECT clientRect = {};
    ::GetClientRect(m_hWnd, &clientRect);
    int clientWidth = clientRect.right - clientRect.left;
    int clientHeight = clientRect.bottom - clientRect.top;
    if (clientWidth <= 0) clientWidth = width;
    if (clientHeight <= 0) clientHeight = height;

    // Create SwapChain and RTV for the drawable client area.
    DXGI_SWAP_CHAIN_DESC scd = {};
    scd.BufferCount = 1;
    scd.BufferDesc.Width = clientWidth;
    scd.BufferDesc.Height = clientHeight;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferDesc.RefreshRate.Numerator = 60;
    scd.BufferDesc.RefreshRate.Denominator = 1;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = m_hWnd;
    scd.SampleDesc.Count = 1;
    scd.SampleDesc.Quality = 0;
    scd.Windowed = TRUE;

    ComPtr<IDXGIDevice> dxgiDevice;
    GET_SINGLE(Core)->GetDevice()->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
    ComPtr<IDXGIAdapter> adapter;
    dxgiDevice->GetAdapter(&adapter);
    ComPtr<IDXGIFactory> factory;
    adapter->GetParent(__uuidof(IDXGIFactory), (void**)&factory);
    factory->CreateSwapChain(GET_SINGLE(Core)->GetDevice(), &scd, &m_swapChain);

    ComPtr<ID3D11Texture2D> backBuffer;
    m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
    GET_SINGLE(Core)->GetDevice()->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_rtv);

    ::ShowWindow(m_hWnd, SW_SHOW);
    ::UpdateWindow(m_hWnd);
    return true;
}

void SubWindow::SetActive(bool active)
{
    m_isActive = active;
    if (m_hWnd)
    {
        ::ShowWindow(m_hWnd, active ? SW_SHOW : SW_HIDE);
    }
}

RECT SubWindow::GetRect() const
{
    RECT rc = {};
    if (m_hWnd)
    {
        ::GetClientRect(m_hWnd, &rc);

        POINT lt = { rc.left, rc.top };
        POINT rb = { rc.right, rc.bottom };
        ::ClientToScreen(m_hWnd, &lt);
        ::ClientToScreen(m_hWnd, &rb);

        rc.left = lt.x;
        rc.top = lt.y;
        rc.right = rb.x;
        rc.bottom = rb.y;
    }
    return rc;
}

void SubWindow::Update()
{
    if (!m_hWnd || !m_isActive) return;
}

LRESULT CALLBACK SubWindow::WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    SubWindow* self = SubWindow::GetThis(hWnd);

    switch (msg)
    {
    //case WM_NCHITTEST:
    //{
    //    LRESULT hit = ::DefWindowProc(hWnd, msg, wp, lp);
    //    if (hit == HTCLIENT)
    //        return HTTRANSPARENT;
    //    return hit;
    //}

    case WM_NCLBUTTONDOWN:
    {
        if (wp == HTCAPTION)
            return 0;
        break;
    }

    case WM_SYSCOMMAND:
    {
        UINT command = (wp & 0xFFF0);
        if (command == SC_MOVE || command == SC_CLOSE || command == SC_KEYMENU)
            return 0;
        break;
    }

    case WM_CLOSE:
        return 0;

    case WM_ERASEBKGND:
        return 1;

    case WM_PAINT:
    {
        PAINTSTRUCT ps = {};
        HDC hdc = ::BeginPaint(hWnd, &ps);
        ::EndPaint(hWnd, &ps);
        return 0;
    }
    }

    return ::DefWindowProc(hWnd, msg, wp, lp);
}

SubWindow* SubWindow::GetThis(HWND hWnd)
{
    const auto it = g_subWindows.find(hWnd);
    return (it != g_subWindows.end()) ? it->second : nullptr;
}
