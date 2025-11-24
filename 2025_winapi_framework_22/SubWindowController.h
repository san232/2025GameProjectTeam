#pragma once
class SubWindowController {
private:
    HWND subWindow;
    SIZE windowSize;

public:
    SubWindowController(HWND hWnd, SIZE size)
        : subWindow(hWnd), windowSize(size) {
    }
public:
    void Update();
};

