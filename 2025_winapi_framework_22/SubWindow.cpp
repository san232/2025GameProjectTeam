#include "pch.h"
#include "SubWindow.h"
#include "SubWindowRenderer.h"

static std::unordered_map<HWND, SubWindow*> g_subWindows;
static const wchar_t* kSubWindowClass = L"SubWindowClass";

SubWindow::SubWindow()
    : m_hWnd(nullptr)
    , m_renderer(nullptr)
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

bool SubWindow::Create(HWND parent, SubWindowRenderer* renderer)
{
    m_renderer = renderer;

    HINSTANCE hInst = (HINSTANCE)::GetModuleHandle(nullptr);
    if (RegisterClassOnce(hInst) == 0)
        return false;

    m_hWnd = ::CreateWindowExW(
        WS_EX_TOOLWINDOW,
        kSubWindowClass,
        L"",
        WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE,
        0, 0, 160, 160,
        parent, nullptr, hInst, nullptr);

    if (m_hWnd == nullptr)
        return false;

    g_subWindows[m_hWnd] = this;
    ::SetWindowLongPtr(m_hWnd, GWLP_USERDATA, (LONG_PTR)this);

    ::ShowWindow(m_hWnd, SW_SHOW);
    ::UpdateWindow(m_hWnd);
    return true;
}

LRESULT CALLBACK SubWindow::WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    SubWindow* self = SubWindow::GetThis(hWnd);

    switch (msg)
    {
    case WM_NCHITTEST:
        return HTTRANSPARENT;

    case WM_ERASEBKGND:
        return 1;

    case WM_PAINT:
    {
        PAINTSTRUCT ps = {};
        HDC hdc = ::BeginPaint(hWnd, &ps);

        if (self != nullptr && self->m_renderer != nullptr)
        {
            POINT pt = {};
            ::GetCursorPos(&pt);

            HWND parent = ::GetParent(hWnd);
            if (parent != nullptr)
            {
                ::ScreenToClient(parent, &pt);
            }

            self->m_renderer->Render(hdc, pt);
        }

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
