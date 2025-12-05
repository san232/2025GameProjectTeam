#include "pch.h"
#include "SubWindow.h"
#include "SubWindowRenderer.h"
#include "Core.h"
#include "InputManager.h"

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

    m_hWnd = ::CreateWindowExW(
        WS_EX_WINDOWEDGE,
        kSubWindowClass,
        L"SubWindow",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        parent, nullptr, hInst, nullptr);

    if (m_hWnd == nullptr)
        return false;

    g_subWindows[m_hWnd] = this;
    ::SetWindowLongPtr(m_hWnd, GWLP_USERDATA, (LONG_PTR)this);

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
        ::GetWindowRect(m_hWnd, &rc);
    }
    return rc;
}

void SubWindow::Update()
{
    if (!m_hWnd || !m_isActive) return;

    POINT mousePos;
    ::GetCursorPos(&mousePos);

    if (GET_KEYDOWN(KEY_TYPE::LBUTTON))
    {
        RECT rc = GetRect();
        if (::PtInRect(&rc, mousePos))
        {
            m_isFollowingMouse = !m_isFollowingMouse;
        }
    }

    if (m_isFollowingMouse)
    {
        RECT rc = GetRect();
        int width = rc.right - rc.left;
        int height = rc.bottom - rc.top;

        int x = mousePos.x - width / 2;
        int y = mousePos.y - height / 2;

        ::SetWindowPos(m_hWnd, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
    }
}

LRESULT CALLBACK SubWindow::WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    SubWindow* self = SubWindow::GetThis(hWnd);

    switch (msg)
    {
    case WM_NCHITTEST:
    {
        LRESULT hit = ::DefWindowProc(hWnd, msg, wp, lp);
        if (hit == HTCLIENT)
            return HTTRANSPARENT;
        return hit;
    }

    case WM_NCLBUTTONDOWN:
    {
        if (wp == HTCAPTION)
            return 0;
        break;
    }

    case WM_SYSCOMMAND:
    {
        if ((wp & 0xFFF0) == SC_MOVE || (wp & 0xFFF0) == SC_CLOSE)
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

        if (self != nullptr && self->m_renderer != nullptr)
        {
            HDC mainBackDC = GET_SINGLE(Core)->GetBackDC();
            
            self->m_renderer->Render(hdc, self, mainBackDC);

            if (self->IsRevealLens())
            {
                 POINT ptSub = {0, 0};
                 ::ClientToScreen(hWnd, &ptSub);
                 HWND hMain = GET_SINGLE(Core)->GetHwnd();
                 POINT ptMain = {0, 0};
                 ::ClientToScreen(hMain, &ptMain);
                 
                 int sx = ptSub.x - ptMain.x;
                 int sy = ptSub.y - ptMain.y;
                 
                 int saved = ::SaveDC(hdc);
                 ::SetViewportOrgEx(hdc, -sx, -sy, nullptr);
                 self->m_renderer->RenderLegacy(hdc);
                 ::RestoreDC(hdc, saved);
            }
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