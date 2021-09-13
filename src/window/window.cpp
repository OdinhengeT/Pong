#include <windows.h>

#include "window.hpp"

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CLOSE:
            DestroyWindow(hWnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

Window::Window(std::wstring title, int width, int height): hInstance(GetModuleHandle(nullptr)) {

    WNDCLASSW wndClass = {};

    wndClass.lpszClassName = L"My_Window_Class";
    wndClass.hInstance = this->hInstance;
    wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.lpfnWndProc = WindowProc;

    RegisterClassW(&wndClass);

    DWORD style = WS_CAPTION | WS_SYSMENU;

    RECT screen;
    screen.top = 100;
    screen.left = 100;
    screen.right = screen.left + width;
    screen.bottom = screen.top + height;

    AdjustWindowRect(&screen, style, false);

    this->hWnd = CreateWindowExW(
        0,
        wndClass.lpszClassName, 
        title.c_str(), 
        style, 
        screen.left, 
        screen.top, 
        screen.right - screen.left, 
        screen.bottom - screen.top, 
        NULL, 
        NULL, 
        this->hInstance, 
        NULL
    );

    ShowWindow(this->hWnd, SW_SHOW);
}

Window::~Window() {
    UnregisterClassW(L"My_Window_Class", this->hInstance);
}

bool Window::MsgProc() {
    MSG msg = {};
    while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE)) {
        if (msg.message == WM_QUIT) return false;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    } 
    return true;
}

int Window::getHeight() {
    RECT temp;
    GetClientRect(this->hWnd, &temp);
    return temp.bottom - temp.top;
}

int Window::getWidth() {
    RECT temp;
    GetClientRect(this->hWnd, &temp);
    return temp.right - temp.left;
}

void Window::drawFrame(BITMAPINFO* bitmap_info, void* frame) {
    RECT screen;
    GetClientRect(this->hWnd, &screen);

    StretchDIBits(
        GetDC(this->hWnd),
        0,
        0,
        screen.right - screen.left,
        screen.bottom - screen.top,
        0,
        0,
        bitmap_info->bmiHeader.biWidth,
        bitmap_info->bmiHeader.biHeight,
        frame,
        bitmap_info,
        DIB_RGB_COLORS,
        SRCCOPY
    );
}