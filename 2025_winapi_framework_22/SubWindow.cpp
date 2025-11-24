#include "pch.h"
#include "SubWindow.h"
#include "SubWindowRenderer.h"
#include <unordered_map>


static std::unordered_map<HWND, SubWindow*> g_subWindows;


bool SubWindow::Create(HWND parent, SubWindowRenderer* renderer) {
	this->renderer = renderer;


	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc;
	wc.hInstance = GetModuleHandle(nullptr);
	wc.lpszClassName = L"SubWindowClass";
	wc.hbrBackground = nullptr;
	RegisterClass(&wc);


	hWnd = CreateWindowEx(
		WS_EX_LAYERED | WS_EX_TOOLWINDOW,
		wc.lpszClassName,
		nullptr,
		WS_POPUP,
		0, 0, 160, 160,
		parent, nullptr, wc.hInstance, nullptr);


	if (!hWnd) return false;


	g_subWindows[hWnd] = this;
	ShowWindow(hWnd, SW_SHOW);
	SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)this);
	return true;
}


LRESULT CALLBACK SubWindow::WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	auto self = GetThis(hWnd);
	switch (msg) {
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);


		POINT pt;
		GetCursorPos(&pt);
		if (self && self->renderer) {
			self->renderer->Render(hdc, pt);
		}


		EndPaint(hWnd, &ps);
		return 0;
	}
	}
	return DefWindowProc(hWnd, msg, wp, lp);
}


SubWindow* SubWindow::GetThis(HWND hWnd) {
	auto it = g_subWindows.find(hWnd);
	return (it != g_subWindows.end()) ? it->second : nullptr;
}