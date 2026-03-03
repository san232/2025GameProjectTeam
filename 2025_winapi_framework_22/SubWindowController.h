#pragma once


class SubWindowController {
public:
	SubWindowController(HWND subWindow, SIZE windowSize);
	void Update();

private:
    HWND m_subWindow;
    SIZE m_size;
};