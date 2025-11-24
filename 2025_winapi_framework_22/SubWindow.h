#pragma once
class SubWindowRenderer;
class SubWindow {
public:
	bool Create(HWND parent, SubWindowRenderer* renderer);
	HWND GetHWnd() const { return hWnd; }


private:
	HWND hWnd = nullptr;
	SubWindowRenderer* renderer = nullptr;


	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
	static SubWindow* GetThis(HWND hWnd);
};